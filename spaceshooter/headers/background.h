#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "main.h"

struct Background
{
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect rectangle;
    float texture_height;
};

bool background_new(struct Background **background, SDL_Renderer *game_renderer);
void background_update(struct Background *b, float delta_time);
void background_draw(const struct Background *b);
void background_free(struct Background **background);

#endif