#include "headers/game.h"

bool background_new(struct Background **background, SDL_Renderer *game_renderer)
{
    *background = calloc(1, sizeof(struct Background));
    if (*background == NULL)
    {
        fprintf(stderr, "Error callocating memory to a new background\n");
        return false;
    }

    struct Background *b = *background;
    b->renderer = game_renderer;

    b->image = IMG_LoadTexture(b->renderer, "images\\space2.jpg");
    if (b->image == NULL)
    {
        fprintf(stderr, "Error creating background texture: %s\n", SDL_GetError());
        return false;
    }

    float background_texture_width = 0;
    if (SDL_GetTextureSize(b->image, &background_texture_width, &b->texture_height) == false)
    {
        fprintf(stderr, "Error getting background size: %s\n", SDL_GetError());
        return false;
    }

    b->rectangle.w = WINDOW_WIDTH;
    b->rectangle.h = WINDOW_HEIGHT; 
    if (b->rectangle.w > background_texture_width || b->rectangle.h > b->texture_height)
    {
        perror("Background ractangle have dimensions greater than the background imgae itself\n");
        return false;
    }

    return true;
}

void background_update(struct Background *b, float delta_time)
{
    if (b->rectangle.y - BACKGROUND_VELOCITY > 0)
    {
        b->rectangle.y -= BACKGROUND_VELOCITY * delta_time * TARGET_FPS;
    }
    else
    {
        b->rectangle.y = b->texture_height - b->rectangle.h;
    }
}

void background_draw(const struct Background *b)
{
    SDL_RenderTexture(b->renderer, b->image, &b->rectangle, NULL);
}

void background_free(struct Background **background)
{
    if (*background != NULL)
    {
        struct Background *b = *background;

        if (b->image != NULL)
        {
            SDL_DestroyTexture(b->image);
            b->image = NULL;
        }

        free(b);
        b = NULL;
        *background = NULL;    
    }
}