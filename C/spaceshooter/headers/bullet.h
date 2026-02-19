#ifndef BULLET_H
#define BULLET_H

#include "main.h"

struct Bullet
{
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect rectangle;
    bool render_bullet;
};

bool bullet_load(struct Bullet **bullet, SDL_Renderer *game_renderer, SDL_FRect player_rectangle);
void bullet_update(struct Bullet *bullet, SDL_FRect player_rectangle, const bool *keyboardstate, float delta_time);
void bullet_draw(const struct Bullet *bullet);
void bullet_free(struct Bullet **bullet);

#endif