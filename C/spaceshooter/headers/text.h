#ifndef TEXT_H
#define TEXT_H

#include "main.h"

struct Text
{
    SDL_Renderer *renderer;
    TTF_Text *text;
    int string_size;
    char *string;
    TTF_TextEngine *engine;
    TTF_Font *font;
    SDL_FPoint position;
};

void center_text(struct Text *t);

bool text_new(struct Text **text, SDL_Renderer *game_renderer, SDL_FPoint position, char *string, int size);
void text_draw(const struct Text *t);
void text_free(struct Text **text);

#endif