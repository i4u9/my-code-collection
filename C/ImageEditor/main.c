#include "main.h"

int main(int argc, char **argv)
{
    int status = EXIT_FAILURE;
    struct Application application = {0};

    if (app_new(&application))
    {
        app_run(&application);

        status = EXIT_SUCCESS;
    }
    
    app_free(&application);
    return status;
}

Uint32 end_application_callback(void *userdata, SDL_TimerID timerID, Uint32 interval)
{
    bool *pIs_running = (bool*)(userdata);
    *pIs_running = false;
    return 0;
}

// notice that you use circles to paint, not individual pixels
void paint_surface(SDL_FCircle circle, const SDL_Color color, SDL_Surface *surface, SDL_FRect surf_rect)
{
    const float w_low_bound = circle.center.x - circle.radius;
    const float w_up_bound = circle.center.x + circle.radius;
    const float h_low_bound = circle.center.y - circle.radius;
    const float h_up_bound = circle.center.y + circle.radius;

    for (float i = w_low_bound; i <= w_up_bound; i++)
    {
        for (float j = h_low_bound; j <= h_up_bound; j++)
        {
            float distance_squared = SDL_powf(i - circle.center.x, 2) + SDL_powf(j - circle.center.y, 2);
            if (distance_squared <= circle.radius * circle.radius)
            {
                SDL_WriteSurfacePixel(surface, (int)(i - surf_rect.x - (surf_rect.w - surface->w)), (int)(j - surf_rect.y - (surf_rect.h - surface->h)), color.r, color.g,color.b, 255);
            }    
        }
    }
}

bool show_color_picker(SDL_Surface **color_picker_surf, bool *rendering_cond, SDL_FRect *color_picker_rect, SDL_Surface **color_indicator_surf, SDL_FRect *color_indicator_rect, SDL_Color color_saved)
{
    *rendering_cond = true;
    *color_picker_surf = IMG_Load(COLOR_WHEEL_PATH);
    if (!(*color_picker_surf))
    {
        fprintf(stderr, "Error loadind color wheel image to surface, application error: %s", SDL_GetError());
        *rendering_cond = false;   
    }
    // printf("%s\n", SDL_GetError());
    int h = (*color_picker_surf)->h;
    int w = (*color_picker_surf)->w;
    *color_picker_rect = (SDL_FRect){0, window_height - h, h, w};

    *color_indicator_surf = IMG_Load(COLOR_INDICATOR_PATH);
    if (!(*color_indicator_surf))
    {
        fprintf(stderr, "Error loadind color indicator image to surface, application error: %s", SDL_GetError());
        *rendering_cond = false;
    }
    SDL_WriteSurfacePixel(*color_indicator_surf, 0, 0, color_saved.r, color_saved.g, color_saved.b, 255);
    // printf("%s\n", SDL_GetError());
    // if (pColor_saved) printf("%d %d %d\n", pColor_saved->r, pColor_saved->g, pColor_saved->b);

    *color_indicator_rect = (SDL_FRect){window_width - COLOR_INDICATOR_OFFSET - COLOR_INDICATOR_WIDTH, COLOR_INDICATOR_OFFSET, COLOR_INDICATOR_WIDTH, COLOR_INDICATOR_HEIGHT};
    

    return true;
}

void hide_color_picker(SDL_Surface **color_picker_surface, bool *rendering_cond, SDL_FRect *color_picker_rect, SDL_Surface **color_indicator_surf, SDL_FRect *color_indicator_rect)
{
    *rendering_cond = false;
    if (*color_picker_surface) SDL_DestroySurface(*color_picker_surface);
    *color_picker_surface = NULL;
    *color_picker_rect = (SDL_FRect){0};

    if (*color_indicator_surf) SDL_DestroySurface(*color_indicator_surf);
    *color_indicator_surf = NULL;
    *color_indicator_rect = (SDL_FRect){0};
}

bool query_image_path(struct Application *app)
{
    bool status = false;
    SDL_Event event_handler;
    bool is_running = true;

    create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, "Drag the image file to the window", 0);

    while (is_running)
    {
        // events
        while (SDL_PollEvent(&event_handler))
        {
            switch (event_handler.type)
            {
            case SDL_EVENT_QUIT:
                is_running = status = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event_handler.key.key == SDLK_ESCAPE)
                {
                    is_running =  status = false;
                }
                else if (event_handler.key.key == SDLK_RETURN && app->image_path != NULL)
                {
                    status = true;
                    is_running = false;
                }
                break;
            
            //  drag 'n drop to window functionality
            case SDL_EVENT_DROP_FILE:
                create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, "File detected! Press ENTER to confirm:", 0);

                app->image_path = calloc(strlen(event_handler.drop.data) + 1, sizeof(char));
                if (!app->image_path)
                {
                    fprintf(stderr, "Error callocating memory to image path");
                    is_running = status = false;
                }
                strcpy(app->image_path, event_handler.drop.data);
                create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, app->image_path, 0);
                create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, "Press M to enter edit mode, press S to save image, press R to reset image size", 0);
                break;
            
            default:
                break;
            }
        }

        // rendering
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);

        render_text_boxes(app->renderer, app->text_boxes, app->text_boxes_quantity);

        SDL_RenderPresent(app->renderer);
    }

    // printf("quantity before: %d\n", app->text_boxes_quantity);
    destroy_text_boxes(&app->text_boxes, 0, &app->text_boxes_quantity);
    
    // printf("quantity after: %d\n", app->text_boxes_quantity);
    return status;
}

void image_rect_align_center(SDL_FRect *image_rect)
{
    image_rect->x = (window_width - image_rect->w) / 2;
    image_rect->y = (window_height - image_rect->h) / 2;
}

bool app_new(struct Application *app)
{
    //  initialize SDL and app essentials
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    app->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!app->window)
    {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    app->renderer = SDL_CreateRenderer(app->window, NULL);
    if (!app->renderer)
    {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    //  initialize text essentials
    if (!TTF_Init())
    {
        fprintf(stderr, "Error initializing SDL_TTF library: %s\n", SDL_GetError());
        return false; 
    }

    app->text_engine = TTF_CreateRendererTextEngine(app->renderer);
    if (!app->text_engine)
    {
        fprintf(stderr, "Error creating text engine from renderer: %s\n", SDL_GetError());
        return false; 
    }
    
    app->text_font = TTF_OpenFont(TEXT_FONT, TEXT_SIZE);
    if (!app->text_font)
    {
        fprintf(stderr, "Error opening font: %s\n", SDL_GetError());
        return false; 
    }

    //  get the path of the image
    if (!query_image_path(app))
    {
        return false;
    }

    //  initialize image
    app->image = IMG_Load(app->image_path);
    if (!app->image)
    {
        fprintf(stderr, "Error loading the image to surface: %s\n", SDL_GetError()); //<---------D
        return false;
    }
        
    if (app->image->w > WINDOW_WIDTH || app->image->h > WINDOW_HEIGHT)
    {
        printf("Error: Trying to load a image bigger than the window dimensions\n");
        return false;
    }
    else
    {
        char img_size[100];
        sprintf(img_size, "Image size: %d x %d px", app->image->w, app->image->h);
        create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, img_size, NULL);

        char format[2048];
        strcpy(format, app->image_path); //<--------D
        strtok(format, ".");
        char message_slice[] = "Image format: .";
        strcat(message_slice, strtok(NULL, "."));
        create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, message_slice, NULL);

        char img_message[] = "Image pixel format: ";
        char img_format[100];
        strcat(img_format, SDL_GetPixelFormatName(app->image->format));
        strtok(img_format, "_");
        strtok(NULL, "_");
        strcat(img_message, strtok(NULL, "_"));
        create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, img_message, NULL);

        SDL_SetWindowMinimumSize(app->window, app->image->w, app->image->h);

        app->image_rect.w = app->image->w;
        app->image_rect.h = app->image->h;
        image_rect_align_center(&app->image_rect);

        app->offset = (SDL_FPoint){-1, -1};
    }

    app->is_running = true;

    return true;
}

Box create_box(SDL_FPoint position, float radius, float body_w)
{
    return (Box){position, {{position.x + radius, position.y + radius}, radius}, {position.x + radius, position.y, body_w, radius * 2 + 1}, {{position.x + radius + body_w, position.y + radius}, radius}};
}

bool create_text_box(TTF_TextEngine *engine, TTF_Font *font, TextBox **tb, unsigned int *text_boxes_quantity, char *string, unsigned int *count)
{
    if (*text_boxes_quantity == 0)
    {
        *tb = calloc(1, sizeof(TextBox));
        if (!*tb)
        {
            fprintf(stderr, "Error callocating memory to text object\n");
            return false;
        }
        
        (*tb)->text = TTF_CreateText(engine, font, string, strlen(string) + 1);
        if (!(*tb)->text)
        {
            fprintf(stderr, "Error creating text: %s\n", SDL_GetError());
            return false; 
        }

        int w, h = 0;
        if (!TTF_GetTextSize((*tb)->text, &w, &h))
        {
            fprintf(stderr, "Error getting text dimensions: %s", SDL_GetError());
            return false;
        }
        (*tb)->box = create_box((SDL_FPoint){0, 0}, h / 2, w);
    }
    else
    {
        TextBox *temp = realloc(*tb, (*text_boxes_quantity + 1) * sizeof(TextBox));
        if (!temp)
        {
            fprintf(stderr, "Error realocating memory to text object\n");
            return false;
        }

        *tb = temp;
        temp = NULL;

        (*tb)[*text_boxes_quantity].text = TTF_CreateText(engine, font, string, strlen(string) + 1);
        if (!(*tb)[*text_boxes_quantity].text)
        {
            fprintf(stderr, "Error creating text: %s\n", SDL_GetError());
            return false; 
        }
        
        int w, h = 0;
        if (!TTF_GetTextSize((*tb)[*text_boxes_quantity].text, &w, &h))
        {
            fprintf(stderr, "Error getting text dimensions: %s", SDL_GetError());
            return false;
        }
        float real_distance_between_boxes = DISTANCE_BETWEEN_BOXES * (*text_boxes_quantity);
        (*tb)[*text_boxes_quantity].box = create_box((SDL_FPoint){0, h * (*text_boxes_quantity) + real_distance_between_boxes}, h / 2, w);
    }

    if (count) (*count)++;

    (*text_boxes_quantity)++;

    // printf("*tb = %p\n", *tb);
    // for (int i = 0; i < *text_boxes_quantity; i++)
    // {
    //     printf("(*tb)[%d].text = %p\n", i, (*tb)[i].text);
    // }
    // printf("\n");
    
    return true;
}

void app_run(struct Application *app)
{
    while (app->is_running)
    {
        app_events(app);
        app_update(app);
        app_draw(*app); 

        SDL_Delay(5);
    }
}

void app_events(struct Application *app)
{   
    while (SDL_PollEvent(&app->event_handler))
    {
        switch (app->event_handler.type)
        {
        case SDL_EVENT_QUIT:
            app->is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (app->event_handler.key.key == SDLK_ESCAPE)
            {
                app->is_running = false;
            }

            //  reset functionality
            else if (app->event_handler.key.key == SDLK_R)
            {
                app->image_rect.w = app->image->w;
                app->image_rect.h = app->image->h;
                image_rect_align_center(&app->image_rect);
            }

            // color wheel functionality
            else if (app->event_handler.key.key == SDLK_M && !app->render_color_picker)
            {
                show_color_picker(&app->color_wheel, &app->render_color_picker, &app->color_wheel_rect, &app->color_indicator, &app->color_indicator_rect, app->color_saved);
            }
            else if (app->event_handler.key.key == SDLK_M && app->render_color_picker)
            {
                hide_color_picker(&app->color_wheel, &app->render_color_picker, &app->color_wheel_rect, &app->color_indicator, &app->color_indicator_rect);
            }

            // saving functionality
            if (app->event_handler.key.key == SDLK_S)
            {
                IMG_Save(app->image, "result/result.png") ? create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, "Image saved in 'result/', application will automatically close", NULL) : fprintf(stderr, "Error saving image: %s", SDL_GetError());

                // start timer
                void *userdata = (void*)(&app->is_running);
                SDL_AddTimer(INTERVAL_AFTER_SAVE_MS, end_application_callback, userdata);
            }
            break;

        //  zoom functionality
        case SDL_EVENT_MOUSE_WHEEL:
            float zoom_quantity = app->event_handler.wheel.y * ZOOM_FACTOR;
            app->image_rect.w += zoom_quantity;
            app->image_rect.h += zoom_quantity;
            app->image_rect.x -= zoom_quantity / 2;
            app->image_rect.y -= zoom_quantity / 2;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            //  drag 'n drop image functionality
            SDL_FPoint click = {app->event_handler.button.x, app->event_handler.button.y};
            bool click_was_inside_image = SDL_PointInRectFloat(&click, &app->image_rect);
            if (click_was_inside_image && !app->render_color_picker)
            {
                app->offset.x = click.x - app->image_rect.x;
                app->offset.y = click.y - app->image_rect.y;
            }

            // color picker functionality
            if (SDL_PointInRectFloat(&click, &app->color_wheel_rect) && app->color_saved_is_black)
            {
                SDL_ReadSurfacePixel(app->color_wheel, (int)(click.x - app->color_wheel_rect.x), (int)(click.y - app->color_wheel_rect.y), &app->color_saved.r, &app->color_saved.g, &app->color_saved.b, NULL);
            }
            else if (SDL_PointInRectFloat(&click, &app->color_wheel_rect) && !app->color_saved_is_black)
            {
                app->color_saved.r = app->color_saved.g = app->color_saved.b = 0;
            }

            // drawing functionality
            SDL_FRect result_on_down = {-1};
            SDL_GetRectIntersectionFloat(&app->image_rect, &app->color_wheel_rect, &result_on_down); 
            if (app->render_color_picker && click_was_inside_image && !SDL_PointInRectFloat(&click, &result_on_down))
            {
                SDL_FCircle paint_area = {{click.x, click.y}, PAINT_DEFAULT_RADIUS};
                paint_surface(paint_area, app->color_saved, app->image, app->image_rect);
            }
            break;

        case SDL_EVENT_MOUSE_MOTION:
            // drag 'n drop image functionality
            if (app->offset.x > 0)
            {
                SDL_FPoint click = {app->event_handler.button.x, app->event_handler.button.y};
                app->image_rect.x = click.x - app->offset.x;
                app->image_rect.y = click.y - app->offset.y;
            }

            // color picker functionality
            SDL_FPoint mouse_pos = {app->event_handler.motion.x, app->event_handler.motion.y};
            if (app->render_color_picker && SDL_PointInRectFloat(&mouse_pos, &app->color_wheel_rect))
            {
                Uint8 r, g, b = 0;
                SDL_ReadSurfacePixel(app->color_wheel, (int)(mouse_pos.x - app->color_wheel_rect.x), (int)(mouse_pos.y - app->color_wheel_rect.y), &r, &g, &b, NULL);
                SDL_WriteSurfacePixel(app->color_indicator, 0, 0, r, g, b, 255);
            }

            // drawing functionality
            bool mouse_is_inside_image = SDL_PointInRectFloat(&mouse_pos, &app->image_rect);
            SDL_FRect result_on_motion = {-1};
            SDL_GetRectIntersectionFloat(&app->image_rect, &app->color_wheel_rect, &result_on_motion); 
            if (app->render_color_picker && mouse_is_inside_image && !SDL_PointInRectFloat(&mouse_pos, &result_on_motion) && app->event_handler.motion.state == SDL_BUTTON_LEFT)
            {
                SDL_FCircle paint_area = {{mouse_pos.x, mouse_pos.y}, PAINT_DEFAULT_RADIUS};
                paint_surface(paint_area, app->color_saved, app->image, app->image_rect);
            }
            break;
             
        case SDL_EVENT_MOUSE_BUTTON_UP:
            app->offset.x = app->offset.y = -1;
            break;

        case SDL_EVENT_WINDOW_RESIZED: 
            SDL_GetWindowSize(app->window, &window_width, &window_height);
            image_rect_align_center(&app->image_rect);
            break;
        default:
            break;
        }
    }
}

void app_update(struct Application *app)
{
    app->color_saved_is_black = app->color_saved.r == 0 && app->color_saved.g == 0 && app->color_saved.b == 0;

    update_texture_from_surface(app->renderer, &app->image_texture, app->image);
    if (app->render_color_picker)
    {
        if (!app->color_saved_is_black) SDL_WriteSurfacePixel(app->color_indicator, 0, 0, app->color_saved.r, app->color_saved.g, app->color_saved.b, app->color_saved.a);

        update_texture_from_surface(app->renderer, &app->color_wheel_texture, app->color_wheel);
        update_texture_from_surface(app->renderer, &app->color_indicator_texture, app->color_indicator);
    }
}

void update_texture_from_surface(SDL_Renderer *renderer, SDL_Texture **dst_texture, SDL_Surface *src_surface)
{
    if (*dst_texture)
    {
        SDL_DestroyTexture(*dst_texture);
        *dst_texture = NULL;
    }
    SDL_Texture *temp_texture = SDL_CreateTextureFromSurface(renderer, src_surface);
    if (temp_texture)
    {
        *dst_texture = temp_texture;
        temp_texture = NULL;
    }
}

void app_draw(struct Application app)
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
    SDL_RenderClear(app.renderer);

    SDL_RenderTexture(app.renderer, app.image_texture, NULL, &app.image_rect);

    render_text_boxes(app.renderer, app.text_boxes, app.text_boxes_quantity);
    
    if (app.render_color_picker)
    {
        SDL_RenderTexture(app.renderer, app.color_wheel_texture, NULL, &app.color_wheel_rect);
        SDL_RenderTexture(app.renderer, app.color_indicator_texture, NULL, &app.color_indicator_rect);
    }

    SDL_RenderPresent(app.renderer);
}

void render_fill_circle_float(SDL_Renderer *renderer, SDL_FCircle circle, const float w_lower_bound, const float w_upper_bound, const float h_lower_bound, const float h_upper_bound)
{
    for (float i = w_lower_bound; i <= w_upper_bound; i++)
    {
        for (float j = h_lower_bound; j <= h_upper_bound; j++)
        {
            float distance_squared = SDL_powf(i - circle.center.x, 2) + SDL_powf(j - circle.center.y, 2);
            if (distance_squared <= circle.radius * circle.radius)
            {
                SDL_RenderPoint(renderer, i, j);
            }    
        }
    }
}

void render_box(SDL_Renderer *renderer, Box box, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    //  left corner
    render_fill_circle_float(renderer, box.left_circle, box.left_circle.center.x - box.left_circle.radius, box.left_circle.center.x, box.left_circle.center.y - box.left_circle.radius, box.left_circle.center.y + box.left_circle.radius);
    // box body
    SDL_RenderFillRect(renderer, &box.body);
    //  right corner
    render_fill_circle_float(renderer, box.right_circle, box.right_circle.center.x, box.right_circle.center.x + box.right_circle.radius,box.right_circle.center.y - box.right_circle.radius, box.right_circle.center.y + box.right_circle.radius);
}

void render_text_boxes(SDL_Renderer *renderer, TextBox *text_boxes, unsigned int text_boxes_quantity)
{
    for (int i = 0; i < text_boxes_quantity; i++)
    {
        render_box(renderer, text_boxes[i].box, TEXT_BOX_DEFAULT_COLOR);
        TTF_DrawRendererText(text_boxes[i].text, text_boxes[i].box.position.x + text_boxes[i].box.left_circle.radius, text_boxes[i].box.position.y - TEXT_ALIGNMENT_DISTANCE_FROM_BOXPOS);
    }
}

void app_free(struct Application *app)
{
    hide_color_picker(&app->color_wheel, &app->render_color_picker, &app->color_wheel_rect, &app->color_indicator, &app->color_indicator_rect);

    if (app->image)
    {
        SDL_DestroySurface(app->image);
        app->image = NULL;
    }

    if (app->renderer)
    {
        SDL_DestroyRenderer(app->renderer);
        app->renderer = NULL;
    }

    if (app->window)
    {
        SDL_DestroyWindow(app->window);
        app->window = NULL;
    }
    
    if (app->text_font)
    {
        TTF_CloseFont(app->text_font);
        app->text_font = NULL;
    }
    
    if (app->text_engine)
    {
        TTF_DestroyRendererTextEngine(app->text_engine);
        app->text_engine = NULL;
    }

    if (app->text_boxes) destroy_text_boxes(&app->text_boxes, 0, &app->text_boxes_quantity);

    SDL_Quit();
    TTF_Quit();
    printf("\nApplication freed!\n");
}

/* This function destroys the elements of the array of text boxes FROM the index START TO the END of the array, 
   reallocating memory when the new quantity of text boxes is NOT 0, if that's the case, then the array is freed. 
   Therefore, start need to be and is an index, quantity isn't a index. */
bool destroy_text_boxes(TextBox **tb, unsigned int start, unsigned int *text_boxes_quantity)
{
    for (int i = start; i < *text_boxes_quantity; i++)
    {
        if ((*tb)[i].text)
        {
            TTF_DestroyText((*tb)[i].text);
            (*tb)[i].text = NULL;
        }
    }

    unsigned int new_quantity = start;
    if (new_quantity == 0)
    {
        free(*tb);
        *tb = NULL;
    }
    else
    {
        TextBox *temp = realloc(*tb, new_quantity * sizeof(TextBox));
        if (!temp)
        {
            fprintf(stderr, "Error reallocating memory to text boxes(in destroy), application crashed\n");
            return false;
        }
    }
    (*text_boxes_quantity) = new_quantity;

    return true;
}