#ifndef PLAYER_H
#define PLAYER_H

#include "bullet.h"

struct Player 
{
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect rectangle;
    float bound_check_value;
    const bool *keybordstate;

    struct Bullet *bullet;
};

bool player_new(struct Player **player, SDL_Renderer *game_renderer);
void player_update(struct Player *p, float delta_time);
void player_draw(const struct Player *p);
void player_free(struct Player **player);

#endif