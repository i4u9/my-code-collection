#ifndef MAIN_H
#define MAIN_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#define SDL_FLAGS (SDL_INIT_VIDEO)

#define WINDOW_TITLE "test"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define MIN_WINDOW_WIDTH 500
#define MIN_WINDOW_HEIGTH 500
int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

#define GAME_PATHS_TIME_DELAY 2000

#define PLAYER_VELOCITY 5.0f

#define ENEMY_TOLERANCE_RADIUS_SQUARED 10000.0f 
#define ENEMY_VELOCITY 6.0f
#define MAX_ENEMY_Y 400.0f
#define MAX_ENEMY_QUANTITY 20
#define ENEMY_DEFAULT_QUANTITY 4

#define BULLET_VELOCITY 8.0f

#define BACKGROUND_VELOCITY 2.0f

#define TEXT_SIZE 55.0f
#define TEXT_DEFAULT_POSITION (SDL_FPoint){0, 0}
#define TEXT_DEFAULT_FONT "fonts\\alienleaguebold.ttf" 

#define INPUT_BOX_THICKNESS 4
#define ENEMY_INPUT_SIZE 4

#define TARGET_FPS 60.0f 

#endif