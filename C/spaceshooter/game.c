#include "get_int_digits.c"
#include "game_load_media.c"

bool game_new(struct Game **game)
{
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL)
    {
        fprintf(stderr, "Error callocating memory to a new game\n");
        return false;
    }

    struct Game *g = *game;

    if(game_sdl_init(g) == false)
    {
        return false;
    }

    if(game_load_media(g) == false)
    {
        return false;
    }

    if (player_new(&g->player, g->video->renderer) == false)
    {
        return false;
    }

    if (enemies_quantity_input(g) == false)
    {
        return false;
    }

    if (g->enemies_quantity > MAX_ENEMY_QUANTITY)
    {
        printf("Too many enemies! Error assigning enemies quantity\n");
        printf("Enemies quantity assigned to MAXIMUM: %d\n", MAX_ENEMY_QUANTITY);
        g->enemies_quantity = MAX_ENEMY_QUANTITY;
    }
    if (enemy_new(&g->enemies, g->video->renderer, &g->enemy_image, g->enemies_quantity) == false)
    {
        return false;
    }

    g->is_running = true;

    return true;
}

void game_events(struct Game *g)
{
    if (g->kills != g->kills_register)
    {
        g->kills_register = g->kills;
        char main_str[] = "Kills: ";
        char number_str[get_int_digits(g->enemies_quantity) + 1];
        itoa(g->kills_register, number_str, 10);
        strcat(main_str, number_str);
        TTF_SetTextString(g->text->text, main_str, sizeof(main_str));
    }

    if (g->kills == g->enemies_quantity)
    {
        g->success = true;
        g->is_running = false;
        return;
    }

    for (int i = 0; i < g->enemies_quantity; i++)
    {
        if (SDL_HasRectIntersectionFloat(&g->player->rectangle, &g->enemies[i].rectangle) == true)
        {
            g->fail = true;
            g->is_running = false;
            return;
        }
    }

    while(SDL_PollEvent(&g->event) == true)
    {
        switch(g->event.type)
        {
        case SDL_EVENT_QUIT:
            g->is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (g->event.key.scancode == 41)    
            {   
                g->is_running = false;  
            }
            else if (g->event.key.scancode == 16)
            {
                g->video->maximize_window_by_key = true;
            }
            break;
        break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SDL_Log("mouse button click event type detected!\n");
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            SDL_Log("mouse wheel event type detected!\n");
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_Log("window resize event detected!\n");
            g->video->registered_window_resize = true;
            break;
        default:
            break;
        } 
    }
}

void game_update(struct Game *g, float delta_time)
{
    update_window(g->video);
    background_update(g->background, delta_time);
    player_update(g->player, delta_time);
    enemy_update(g->enemies, g->player->bullet->rectangle, g->enemies_quantity, &g->kills, delta_time);
}

void game_draw(const struct Game *g)
{
    SDL_SetRenderDrawColor(g->video->renderer, renderer_color.r, renderer_color.g, renderer_color.b, renderer_color.a);
    SDL_RenderClear(g->video->renderer);

    background_draw(g->background);
    text_draw(g->text);
    player_draw(g->player);
    enemy_draw(g->enemies, g->video->renderer, g->enemy_image, g->enemies_quantity);

    SDL_RenderPresent(g->video->renderer);
}

void game_success(struct Game *g)
{
    char success_message[] = "Congratulations!\nYou completed the game!";
    TTF_SetTextString(g->text->text, success_message, sizeof(success_message));

    center_text(g->text);

    SDL_SetRenderDrawColor(g->video->renderer, renderer_color.r, renderer_color.g, renderer_color.b, renderer_color.a);
    SDL_RenderClear(g->video->renderer);

    background_draw(g->background);
    text_draw(g->text);

    SDL_RenderPresent(g->video->renderer);
    SDL_Delay(GAME_PATHS_TIME_DELAY);
}

void game_fail(struct Game *g)
{
    char fail_message[] = "Try again! you are too newbie\nto play this game!";   
    TTF_SetTextString(g->text->text, fail_message, sizeof(fail_message));     

    center_text(g->text);

    SDL_SetRenderDrawColor(g->video->renderer, renderer_color.r, renderer_color.g, renderer_color.b, renderer_color.a);
    SDL_RenderClear(g->video->renderer);

    background_draw(g->background);
    text_draw(g->text);

    SDL_RenderPresent(g->video->renderer);
    SDL_Delay(GAME_PATHS_TIME_DELAY);
}

void game_run(struct Game *g)
{
    Uint64 fps, lasttime = 0;
    Uint64 lasttick = SDL_GetTicks();
    char fps_string[6];

    while(g->is_running == true)
    {
        Uint64 currenttick = SDL_GetTicks();
        float deltatime = (SDL_GetTicks() - lasttick) / 1000.0f;
        lasttick = currenttick;

        game_events(g);
        game_update(g, deltatime);
        game_draw(g);
        
        SDL_Delay(16);
        fps++;
        if (currenttick > lasttime + 1000)
        {
            lasttime = currenttick;
            itoa(fps, fps_string, 10);
            SDL_SetWindowTitle(g->video->window, fps_string);
            fps = 0;
        }
        
        if (g->success == true)
        {
            game_success(g);
        }
        if (g->fail == true)
        {
            game_fail(g);
        }
    }
}

void game_free(struct Game **game, int status)
{
    if (*game != NULL)
    {
        struct Game *g = *game;

        if (g->enemies != NULL)
        {
            enemy_free(&g->enemies, &g->enemy_image, g->enemies_quantity);
        }

        if (g->player != NULL)
        {
            player_free(&g->player);
        }

        if (g->text != NULL)
        {
            text_free(&g->text);
        }

        if (g->background != NULL)
        {
            background_free(&g->background);
        }
        
        if (g->video != NULL)
        {
            video_free(&g->video);
        }

        SDL_Quit();

        free(g);
        g = NULL;
        *game = NULL;
    }
    printf("%d", status);
}