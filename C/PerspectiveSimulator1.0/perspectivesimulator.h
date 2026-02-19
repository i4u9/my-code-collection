#ifndef PERSPECTIVESIMULATOR_H
#define PERSPECTIVESIMULATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define SDL_FLAGS (SDL_INIT_VIDEO)

#define WINDOW_TITLE "perspectiveSimulator"
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900
int window_width = 1500;
int window_height = 900;

#define VANISHING_MOVEMENT_FACTOR 2
#define SPACING_FACTOR 2
#define HORIZON_MOVIMENT_FACTOR 0.5
#define THICKNESS_INCR_DECR_FACTOR 0.1
#define LINES_INCR_DECR_FACTOR 1

enum REALLOC_ID{verticals, horizontals};

typedef struct SDL_FLine{
    SDL_FPoint p1;
    SDL_FPoint p2;
}SDL_FLine;

struct GraphicElements
{
    SDL_FPoint vanishing_point;
    float vertical_lines_spacing;
    float lines_thickness;

    int horizontals_quantity;
    int verticals_quantity;
    SDL_FLine *horizontals;
    SDL_FLine *verticals;
};

struct Application
{
    bool is_running;
    bool fullscreen;
    const bool *keyboardstate;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event events;
    
    struct GraphicElements *line_sets;
};

SDL_Color lines_color = {0, 255, 0, 255};

bool blind_mode = true;
SDL_FLine horizon = {
    .p1 = {0, WINDOW_HEIGHT / 2},
    .p2 = {WINDOW_WIDTH, WINDOW_HEIGHT / 2}
};

bool application_new(struct Application **app);
bool graphicelements_new(struct GraphicElements **line_set);
bool graphicelements_realloc(struct GraphicElements **line_set, int factor, int realloc_id);
void graphicelements_update(struct GraphicElements *ls);

void application_run(struct Application *a);
void events(struct Application *a);
void update(struct Application *a);
void draw(SDL_Renderer *application_renderer, struct GraphicElements *ls);

void application_free(struct Application **app);


void print_keybindings();

#endif