#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "video.h"
#include "enemy.h"
#include "background.h"
#include "text.h"
#include "input_text.h"

struct Game
{
    bool is_running;
    bool success;
    bool fail;
    int kills_register;
    int kills;

    struct Background *background;
    struct Video *video;
    struct Text *text;
    struct Player *player;
    struct InputText *enemy_input;

    struct Enemy *enemies;
    SDL_Texture *enemy_image;
    int enemies_quantity;

    SDL_Event event;
};

SDL_Color renderer_color = {255, 255, 255, 255};

bool game_sdl_init(struct Game *g);
bool game_load_media(struct Game *g);
bool game_new(struct Game **game);

void game_events(struct Game *g);

void game_update(struct Game *g, float delta_ttime);
void game_draw(const struct Game *g);

void game_success(struct Game *g);
void game_fail(struct Game *g);

void game_run(struct Game *g);
void game_free(struct Game **game, int status);

#endif