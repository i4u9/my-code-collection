#ifndef INPUTTEXT_H
#define INPUTTEXT_H

#include "main.h"

struct InputText
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_TextEngine *engine;
    TTF_Font *font;
    TTF_Text *text_object;
    SDL_Rect area;
    int cursor;
    int string_size;
    char *string;
    bool input_completed;
};

bool input_text_new(struct InputText **text_input, SDL_Renderer *game_renderer, SDL_Window *game_window, int size);
void render_input_box(SDL_Rect area_to_box, SDL_Renderer *game_renderer, int thickness);
void input_text_free(struct InputText **text_input, char *message);

#endif