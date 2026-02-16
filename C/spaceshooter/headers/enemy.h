#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"

struct Enemy
{
    SDL_FRect rectangle;
    bool killed_enemy;
    SDL_FPoint center;
};

float distance_square_FPoints(SDL_FPoint p1, SDL_FPoint p2);
void enemy_check_overlap_by_distance(struct Enemy **enemy, int enemy_index, int enemies_quantity);
// void enemy_check_overlap(struct Enemy **enemy, int enemy_index, int enemies_quantity);

bool enemy_new(struct Enemy **enemy, SDL_Renderer *game_renderer, SDL_Texture **enemy_image, int enemies_quantity);
void enemy_update(struct Enemy *e, SDL_FRect bullet_rectangle, int enemies_quantity, int *kills, float delta_time);
void enemy_draw(const struct Enemy *e, SDL_Renderer *game_renderer, SDL_Texture *enemy_image, int enemies_quantity);
void enemy_free(struct Enemy **enemy, SDL_Texture **enemy_image, int enemies_quantity);

#endif