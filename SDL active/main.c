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

bool show_color_picker(SDL_Surface **color_picker_surface, bool *rendering_cond, SDL_FRect *color_picker_rect)
{
    *rendering_cond = true;
    *color_picker_surface = IMG_Load(COLOR_WHEEL_PATH);
    if (!color_picker_surface)
    {
        fprintf(stderr, "Error loadind color wheel image to surface, application error: %s", SDL_GetError());
        *rendering_cond = false;   
    }

    int h = (*color_picker_surface)->h;
    int w = (*color_picker_surface)->w;
    *color_picker_rect = (SDL_FRect){0, WINDOW_HEIGHT - (h / COLOR_WHEEL_SCALING_FACTOR), h / COLOR_WHEEL_SCALING_FACTOR, w / COLOR_WHEEL_SCALING_FACTOR};

    return true;
}

void hide_color_picker(SDL_Surface **color_picker_surface, bool *rendering_cond, SDL_FRect *color_picker_rect)
{
    *rendering_cond = false;
    SDL_DestroySurface(*color_picker_surface);
    *color_picker_surface = NULL;
    *color_picker_rect = (SDL_FRect){0};
}

bool query_image_path(struct Application *app)
{
    bool status = false;
    SDL_Event event_handler;
    bool is_running = true;

    create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, "Drag the image file to the window",
                    0);

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
                create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity,
                                "File detected! Press ENTER to confirm:", 0);

                app->image_path = calloc(strlen(event_handler.drop.data) + 1, sizeof(char));
                if (!app->image_path)
                {
                    fprintf(stderr, "Error callocating memory to image path");
                    is_running = status = false;
                }
                strcpy(app->image_path, event_handler.drop.data);
                create_text_box(app->text_engine, app->text_font, &app->text_boxes, &app->text_boxes_quantity, app->image_path,
                                0);
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

    printf("quantity before: %d\n", app->text_boxes_quantity);
    destroy_text_boxes(&app->text_boxes, 0, &app->text_boxes_quantity);
    
    printf("quantity after: %d\n", app->text_boxes_quantity);
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
    // if (!query_image_path(app))
    // {
    //     return false;
    // }

    //  initialize image
    app->image = IMG_Load("images/cat.jpg");
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
        create_text_box(app->text_engine, app->text_font, &app->text_boxes,
                        &app->text_boxes_quantity, img_size, NULL);

        char format[2048];
        strcpy(format, "images/cat.jpg"); //<--------D
        strtok(format, ".");
        char message_slice[] = "Image format: .";
        strcat(message_slice, strtok(NULL, "."));
        create_text_box(app->text_engine, app->text_font, &app->text_boxes,
                        &app->text_boxes_quantity, message_slice, NULL);

        char img_message[] = "Image pixel format: ";
        char img_format[100];
        strcat(img_format, SDL_GetPixelFormatName(app->image->format));
        strtok(img_format, "_");
        strtok(NULL, "_");
        strcat(img_message, strtok(NULL, "_"));
        create_text_box(app->text_engine, app->text_font, &app->text_boxes,
                        &app->text_boxes_quantity, img_message, NULL);

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
    return (Box){position,
                 {{position.x + radius, position.y + radius}, radius},
                 {position.x + radius, position.y, body_w, radius * 2 + 1},
                 {{position.x + radius + body_w, position.y + radius}, radius}};
}

bool create_text_box(TTF_TextEngine *engine, TTF_Font *font, TextBox **tb, unsigned int *text_boxes_quantity, char *string,
                     unsigned int *count)
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
            else if (app->event_handler.key.key == SDLK_M && !app->render_color_wheel)
            {
                show_color_picker(&app->color_wheel, &app->render_color_wheel, &app->color_wheel_rect);
            }
            else if (app->event_handler.key.key == SDLK_M && app->render_color_wheel)
            {
                hide_color_picker(&app->color_wheel, &app->render_color_wheel, &app->color_wheel_rect);
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

        //  drag 'n drop image functionality
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SDL_FPoint click = {app->event_handler.button.x, app->event_handler.button.y};
            if (SDL_PointInRectFloat(&click, &app->image_rect) && !app->render_color_wheel)
            {
                app->offset.x = click.x - app->image_rect.x;
                app->offset.y = click.y - app->image_rect.y;
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            if (app->offset.x > 0)
            {
                SDL_FPoint click = {app->event_handler.button.x, app->event_handler.button.y};
                app->image_rect.x = click.x - app->offset.x;
                app->image_rect.y = click.y - app->offset.y;
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
    UpdateTextureFromSurface(app->renderer, &app->image_texture, app->image);
    if (app->render_color_wheel) UpdateTextureFromSurface(app->renderer, &app->color_wheel_texture, app->color_wheel);
}

void UpdateTextureFromSurface(SDL_Renderer *renderer, SDL_Texture **dst_texture, SDL_Surface *src_surface)
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
    
    if (app.render_color_wheel) SDL_RenderTexture(app.renderer, app.color_wheel_texture, NULL, &app.color_wheel_rect);;

    SDL_RenderPresent(app.renderer);
}

void RenderFillCircleF(SDL_Renderer *renderer, SDL_FCircle circle, const float w_lower_bound, 
                       const float w_upper_bound, const float h_lower_bound, const float h_upper_bound)
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

void RenderBox(SDL_Renderer *renderer, Box box, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    //  left corner
    RenderFillCircleF(renderer, box.left_circle, box.left_circle.center.x - box.left_circle.radius, box.left_circle.center.x,
                      box.left_circle.center.y - box.left_circle.radius, box.left_circle.center.y + box.left_circle.radius);
    // box body
    SDL_RenderFillRect(renderer, &box.body);
    //  right corner
    RenderFillCircleF(renderer, box.right_circle, box.right_circle.center.x, box.right_circle.center.x + box.right_circle.radius,
                      box.right_circle.center.y - box.right_circle.radius, box.right_circle.center.y + box.right_circle.radius);
}

void render_text_boxes(SDL_Renderer *renderer, TextBox *text_boxes, unsigned int text_boxes_quantity)
{
    for (int i = 0; i < text_boxes_quantity; i++)
    {
        RenderBox(renderer, text_boxes[i].box, TEXT_BOX_DEFAULT_COLOR);
        TTF_DrawRendererText(text_boxes[i].text, text_boxes[i].box.position.x + text_boxes[i].box.left_circle.radius,
                            text_boxes[i].box.position.y - TEXT_ALIGNMENT_DISTANCE_FROM_BOXPOS);
    }
}

void app_free(struct Application *app)
{
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