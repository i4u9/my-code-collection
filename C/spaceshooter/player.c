// game.h is included in bullet.c
#include "bullet.c"

bool player_new(struct Player **player, SDL_Renderer *game_renderer)
{
    *player = calloc(1, sizeof(struct Player));
    if (*player == NULL)
    {
        fprintf(stderr, "Error allocating memory to a new player\n");
        return false;
    }

    struct Player *p = *player;
    p->renderer = game_renderer;

    p->image = IMG_LoadTexture(p->renderer, "images\\shooter.bmp");
    if (p->image == NULL)
    {
        fprintf(stderr, "Error loading player texture: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_GetTextureSize(p->image, &p->rectangle.w, &p->rectangle.h) == false)
    {
        fprintf(stderr, "Error getting player texture size: %s\n", SDL_GetError());
        return false;
    }

    p->keybordstate = SDL_GetKeyboardState(NULL);
    p->bound_check_value = 0.01;
    p->rectangle.y = WINDOW_HEIGHT - p->rectangle.h;

    if (bullet_load(&p->bullet, p->renderer, p->rectangle) == false)
    {
        return false;
    }
    
    return true;
}

void player_update(struct Player *p, float delta_time)
{
    if ((p->keybordstate[SDL_SCANCODE_LEFT] || p->keybordstate[SDL_SCANCODE_A]) && p->rectangle.x - p->bound_check_value > 0)
    {
        p->rectangle.x -= PLAYER_VELOCITY * delta_time * TARGET_FPS;     // player moviment
    }
    if ((p->keybordstate[SDL_SCANCODE_RIGHT] || p->keybordstate[SDL_SCANCODE_D]) && (p->rectangle.x + p->rectangle.w) + p->bound_check_value < window_width)
    {
        p->rectangle.x += PLAYER_VELOCITY * delta_time * TARGET_FPS;     // player moviment
    }

    bullet_update(p->bullet, p->rectangle, p->keybordstate, delta_time);
}

void player_draw(const struct Player *p)
{
    bullet_draw(p->bullet);
    SDL_RenderTexture(p->renderer, p->image, NULL, &p->rectangle);
}

void player_free(struct Player **player)
{
    if (player != NULL)
    {
        struct Player *p = *player;

        bullet_free(&p->bullet);

        if (p->image != NULL)
        {
            SDL_DestroyTexture(p->image);
            p->image = NULL;
        }
        
        p->renderer = NULL;

        free(p);
        p = NULL;
        *player = NULL;

        printf("Player freed\n");
    }
}