#include "headers/game.h"

bool bullet_load(struct Bullet **bullet, SDL_Renderer *game_renderer, SDL_FRect player_rectangle)
{
    *bullet = calloc(1, sizeof(struct Bullet));
    if (*bullet == NULL)
    {
        fprintf(stderr, "Error allocating memory to player bullet\n");
        return false;
    }

    struct Bullet *b = *bullet;
    b->renderer = game_renderer;

    b->image = IMG_LoadTexture(b->renderer, "images\\bullet.png");
    if (b->image == NULL)
    {
        fprintf(stderr, "Error loading bullet texture: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_GetTextureSize(b->image, &b->rectangle.w, &b->rectangle.h) == false)
    {
        fprintf(stderr, "Error getting bullet texture size: %s\n", SDL_GetError());
        return false;
    }

    b->rectangle.y = player_rectangle.y - b->rectangle.h;

    return true;
}

void bullet_update(struct Bullet *bullet, SDL_FRect player_rectangle, const bool *keybordstate, float delta_time)
{
    bullet->rectangle.y -= BULLET_VELOCITY * delta_time * TARGET_FPS;    // bullet moviment

    if (bullet->rectangle.y <= -bullet->rectangle.h)
    {
        bullet->rectangle.y = player_rectangle.y - bullet->rectangle.h;
        bullet->rectangle.x = player_rectangle.x;
    }
}

void bullet_draw(const struct Bullet *bullet)
{
    SDL_RenderTexture(bullet->renderer, bullet->image, NULL, &bullet->rectangle);
}

void bullet_free(struct Bullet **bullet)
{
    if (*bullet != NULL)
    {
        struct Bullet *b = *bullet;

        if (b->image != NULL)
        {
            SDL_DestroyTexture(b->image);
            b->image = NULL;
        }

        b->renderer = NULL;

        free(b);
        b = NULL;
        *bullet = NULL;
    }

    printf("Bullet freed\n");
}