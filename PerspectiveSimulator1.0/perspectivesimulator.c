#include "perspectivesimulator.h"

bool application_new(struct Application **app)
{
    *app = calloc(1, sizeof(struct Application));
    if (!*app)
    {
        perror("Error alocating memory to application\n");
        return false;
    }

    struct Application *a = *app;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Error initializing SDL video subsystem: %s\n", SDL_GetError());
        return false;
    }

    a->window = SDL_CreateWindow(WINDOW_TITLE, window_width, window_height, SDL_WINDOW_RESIZABLE);
    if (!a->window)
    {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }
    
    a->renderer = SDL_CreateRenderer(a->window, NULL);
    if (!a->renderer)
    {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    /////////////////////////////////////////////////////////////////////
    if (!graphicelements_new(&a->line_sets))
    {
        return false;
    }

    graphicelements_update(a->line_sets);
    /////////////////////////////////////////////////////////////////////

    a->keyboardstate = SDL_GetKeyboardState(NULL);

    a->is_running = true;

    return true;
}

bool graphicelements_new(struct GraphicElements **line_set)
{   
    *line_set = calloc(1, sizeof(struct GraphicElements));
    if (!*line_set)
    {
        perror("Error callocating memory to line set\n");
        return false;
    }

    struct GraphicElements *ls = *line_set;

    ls->lines_thickness = 3;
    ls->horizontals_quantity = 4;
    ls->verticals_quantity = 50;
    ls->vertical_lines_spacing = 100;
    ls->vanishing_point = (SDL_FPoint){window_width / 2, window_height / 8};

    ls->horizontals = calloc(ls->horizontals_quantity, sizeof(struct SDL_FLine));
    if (!ls->horizontals)
    {
        perror("Error callocating memory to horizontal lines\n");
        return false;
    }
    
    ls->verticals = calloc(ls->verticals_quantity, sizeof(struct SDL_FLine));
    if (!ls->verticals)
    {
        perror("Error callocating memory to vertical lines\n");
        return false;
    }

    return true;
}

bool graphicelements_realloc(struct GraphicElements **line_set, int factor, int realloc_id)
{
    struct GraphicElements *ls = *line_set;

    if (realloc_id == verticals)
    {
        SDL_FLine *temp = realloc(ls->verticals, sizeof(SDL_FLine) * (ls->verticals_quantity + factor));
        if (!temp)
        {
            perror("Error reallocating memory to vertical lines\n");
            return false;
        }
        ls->verticals = temp;
        temp = NULL;

        ls->verticals_quantity += factor;
    }
    else if (realloc_id == horizontals)
    {
        SDL_FLine *temp = realloc(ls->horizontals, sizeof(SDL_FLine) * (ls->horizontals_quantity + factor));
        if (!temp)
        {
            perror("Error reallocating memory to horizontal lines\n");
            return false;
        }
        ls->horizontals = temp;
        temp = NULL;

        ls->horizontals_quantity += factor;
    }

    return true;
}

void graphicelements_update(struct GraphicElements *ls)
{
    // horizontal lines are builted from bottom to top, left to right
    // for (int i = 0; i < ls->horizontals_quantity; i++)
    // {
    //     ls->horizontals[i] = (SDL_FLine){
    //         {           0, WINDOW_HEIGHT - (ls->horizontal_lines_spacing * i)},
    //         {WINDOW_WIDTH, WINDOW_HEIGHT - (ls->horizontal_lines_spacing * i)}
    //     };
    // }

    // horizontal lines are builted from bottom to top, left to right
    // float spacing = (WINDOW_HEIGHT - horizon.p1.y) / ls->horizontals_quantity + 1;
    // for (int i = 0; i < ls->horizontals_quantity; i++)
    // {
    //     ls->horizontals[i] = (SDL_FLine){
    //         {           0, horizon.p1.y + spacing * (i + 1)},
    //         {WINDOW_WIDTH, horizon.p1.y + spacing * (i + 1)}
    //     };
    // }

    float base_spacing = ((window_height - horizon.p1.y) / 2);
    for (int i = 0; i < ls->horizontals_quantity; i++)
    {
        ls->horizontals[i] = (SDL_FLine){
            {           0, horizon.p1.y + (base_spacing / SDL_pow(2, i))},
            {window_width, horizon.p1.y + (base_spacing / SDL_pow(2, i))}
        };
    }
    

    // vertical lines are builted from the vanishing point to the bottom
    float reference = ls->vanishing_point.x - (ls->vertical_lines_spacing * ((ls->verticals_quantity - 1) / 2));
    if ((int)(ls->verticals_quantity) % 2 == 0)
    {
        reference -= 0.5 * ls->vertical_lines_spacing;
    }
    for (int i = 0; i < ls->verticals_quantity; i++)
    {
        ls->verticals[i] = (SDL_FLine){
            ls->vanishing_point,
            {reference + (ls->vertical_lines_spacing * i), window_height}
        };
    }
}

void application_run(struct Application *a)
{
    while (a->is_running)
    {
        events(a);
        update(a);
        draw(a->renderer, a->line_sets);

        SDL_Delay(16);
    }
}

void update(struct Application *a)
{
    if (a->keyboardstate[SDL_SCANCODE_W] || a->keyboardstate[SDL_SCANCODE_UP])
    {
        a->line_sets->vanishing_point.y -= VANISHING_MOVEMENT_FACTOR;
    }
    if (a->keyboardstate[SDL_SCANCODE_A] || a->keyboardstate[SDL_SCANCODE_LEFT])
    {
        a->line_sets->vanishing_point.x -= VANISHING_MOVEMENT_FACTOR;
    }
    if (a->keyboardstate[SDL_SCANCODE_S] || a->keyboardstate[SDL_SCANCODE_DOWN])
    {
        a->line_sets->vanishing_point.y += VANISHING_MOVEMENT_FACTOR;
    }
    if (a->keyboardstate[SDL_SCANCODE_D] || a->keyboardstate[SDL_SCANCODE_RIGHT])
    {
        a->line_sets->vanishing_point.x += VANISHING_MOVEMENT_FACTOR;
    }
    if (a->keyboardstate[SDL_SCANCODE_V] && a->keyboardstate[SDL_SCANCODE_LSHIFT] && a->keyboardstate[SDL_SCANCODE_EQUALS])
    {
        a->line_sets->vertical_lines_spacing += SPACING_FACTOR;
    }
    if (a->keyboardstate[SDL_SCANCODE_V] && a->keyboardstate[SDL_SCANCODE_LSHIFT] && a->keyboardstate[SDL_SCANCODE_MINUS])
    {
        a->line_sets->vertical_lines_spacing -= SPACING_FACTOR;
    }
    if (a->keyboardstate[SDL_SCANCODE_H] && a->keyboardstate[SDL_SCANCODE_LSHIFT] && a->keyboardstate[SDL_SCANCODE_EQUALS])
    {
        horizon.p1.y -= SPACING_FACTOR;
        horizon.p2.y = horizon.p1.y;
    }
    if (a->keyboardstate[SDL_SCANCODE_H] && a->keyboardstate[SDL_SCANCODE_LSHIFT] && a->keyboardstate[SDL_SCANCODE_MINUS])
    {
        horizon.p1.y += SPACING_FACTOR;
        horizon.p2.y = horizon.p1.y;
    }
    if (a->keyboardstate[SDL_SCANCODE_T] && a->keyboardstate[SDL_SCANCODE_LSHIFT] && a->keyboardstate[SDL_SCANCODE_EQUALS])
    {
        a->line_sets->lines_thickness += THICKNESS_INCR_DECR_FACTOR;
    }
    if (a->keyboardstate[SDL_SCANCODE_T] && a->keyboardstate[SDL_SCANCODE_LSHIFT] && a->keyboardstate[SDL_SCANCODE_MINUS])
    {
        a->line_sets->lines_thickness -= THICKNESS_INCR_DECR_FACTOR;
    }

    if (a->keyboardstate[SDL_SCANCODE_L] && a->keyboardstate[SDL_SCANCODE_V] && a->keyboardstate[SDL_SCANCODE_EQUALS])
    {
        // a->line_sets->lines_thickness += LINES_INCR_DECR_FACTOR;
        graphicelements_realloc(&a->line_sets, LINES_INCR_DECR_FACTOR, verticals);
    }
    if (a->keyboardstate[SDL_SCANCODE_L] && a->keyboardstate[SDL_SCANCODE_V] && a->keyboardstate[SDL_SCANCODE_MINUS])
    {
        // a->line_sets->lines_thickness -= LINES_INCR_DECR_FACTOR;
        if (a->line_sets->verticals_quantity - LINES_INCR_DECR_FACTOR > 0)
        {
            graphicelements_realloc(&a->line_sets, -LINES_INCR_DECR_FACTOR, verticals);
        }
        else
        {
            printf("Error: you can't represent a negative number of lines\n");
        }
    }
    if (a->keyboardstate[SDL_SCANCODE_L] && a->keyboardstate[SDL_SCANCODE_H] && a->keyboardstate[SDL_SCANCODE_EQUALS])
    {
        // a->line_sets->lines_thickness += LINES_INCR_DECR_FACTOR;
        graphicelements_realloc(&a->line_sets, LINES_INCR_DECR_FACTOR, horizontals);
    }
    if (a->keyboardstate[SDL_SCANCODE_L] && a->keyboardstate[SDL_SCANCODE_H] && a->keyboardstate[SDL_SCANCODE_MINUS])
    {
        // a->line_sets->lines_thickness -= LINES_INCR_DECR_FACTOR;
        if (a->line_sets->horizontals_quantity - LINES_INCR_DECR_FACTOR > 0)
        {
            graphicelements_realloc(&a->line_sets, -LINES_INCR_DECR_FACTOR, horizontals);
        }
        else
        {
            printf("Error: you can't represent a negative number of lines\n");
        }
    }

    graphicelements_update(a->line_sets);
}

void events(struct Application *a)
{
    while (SDL_PollEvent(&a->events))
    {
        switch (a->events.type)
        {
        case SDL_EVENT_QUIT:
            a->is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (a->events.key.key == SDLK_ESCAPE)
            {
                a->is_running = false;
            }
            else if (a->events.key.key == SDLK_F11)
            {
                if (a->fullscreen == false)
                {
                    SDL_SetWindowFullscreen(a->window, true);
                    a->fullscreen = true;
                }
                else
                {
                    SDL_SetWindowFullscreen(a->window, false);
                    a->fullscreen = false;
                }
            }
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_GetWindowSize(a->window, &window_width, &window_height);
            horizon = (SDL_FLine){
                {0, window_height / 2},
                {window_width, window_height / 2}
            };
            graphicelements_update(a->line_sets);
        default:
            break;
        }
    }
}

void draw(SDL_Renderer *application_renderer, struct GraphicElements *ls)
{
    SDL_SetRenderDrawColor(application_renderer, 0, 0, 0, 0);
    SDL_RenderClear(application_renderer);

    SDL_SetRenderDrawColor(application_renderer, lines_color.r, lines_color.g, lines_color.b, lines_color.a);
    SDL_RenderLine(application_renderer, horizon.p1.x, horizon.p1.y, horizon.p2.x, horizon.p2.y);

    for (int i = 0; i < ls->horizontals_quantity; i++)
    {
        for (int j = 0; j < ls->lines_thickness; j++)
        {
            SDL_RenderLine(application_renderer, 
                ls->horizontals[i].p1.x, ls->horizontals[i].p1.y - j,
                ls->horizontals[i].p2.x, ls->horizontals[i].p2.y - j
            );
        }
    }

    for (int i = 0; i < ls->verticals_quantity; i++)
    {
        for (int j = 0; j < ls->lines_thickness; j++)
        {
            SDL_RenderLine(application_renderer, 
                ls->verticals[i].p1.x + j, ls->verticals[i].p1.y, 
                ls->verticals[i].p2.x + j, ls->verticals[i].p2.y
            );
        }
    }

    if (blind_mode)
    {
        SDL_SetRenderDrawColor(application_renderer, 0, 0, 0, 0);
        SDL_FRect rect = {0, 0, window_width, horizon.p1.y};
        SDL_RenderFillRect(application_renderer, &rect);
    }
    
    SDL_RenderPresent(application_renderer);
}

void application_free(struct Application **app)
{
    if (*app)
    {
        struct Application *a = *app;

        if (a->line_sets->verticals)
        {
            free(a->line_sets->verticals);
            a->line_sets->verticals = NULL;
        }

        if (a->line_sets->horizontals)
        {
            free(a->line_sets->horizontals);
            a->line_sets->horizontals = NULL;
        }

        if (a->line_sets)
        {
            free(a->line_sets);
            a->line_sets = NULL;
        }

        printf("Graphical elements freed...\n");

        if (a->renderer)
        {
            SDL_DestroyRenderer(a->renderer);
            a->renderer = NULL;
        }

        if (a->window)
        {
            SDL_DestroyWindow(a->window);
            a->window = NULL;
        }

        SDL_Quit();

        printf("SDL freed...\n");

        free(a);
        a = NULL;
        *app = NULL;

        printf("Application freed.\n");
    }
    
}

void print_keybindings()
{
    printf("\nKeybindings:\n");
    printf("Move vanishing point up - W or Arrow up\n");
    printf("Move vanishing point down - S or Arrow down\n");
    printf("Move vanishing point to left - A or Arrow left\n");
    printf("Move vanishing point to right - D or Arrow right\n");
    printf("Increase vertical lines spacing - V + LSHIFT + '='\n");
    printf("Decrease vertical lines spacing - V + LSHIFT + '-'\n");
    printf("Increase horizon height - H + LSHIFT + '='\n");
    printf("Decrease horizon height - H + LSHIFT + '-'\n");
    printf("Increase vertical lines thickness - T + LSHIFT + '='\n");
    printf("Decrease vertical lines thickness - T + LSHIFT + '-'\n");
    printf("Increase vertical lines quantity - L + V + '='\n");
    printf("Decrease vertical lines quantity - L + V + '-'\n");
    printf("Increase horizontal lines quantity - L + H + '='\n");
    printf("Decrease horizontal lines quantity - L + H + '-'\n");
}