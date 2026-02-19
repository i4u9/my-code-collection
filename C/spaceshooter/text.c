#include "headers/game.h"

bool text_new(struct Text **text, SDL_Renderer *game_renderer, SDL_FPoint position, char *string, int size)
{
    *text = calloc(1, sizeof(struct Text));
    if (*text == NULL)
    {
        fprintf(stderr, "Error callocating memory to a new text\n");
        return false;
    }

    struct Text *t = *text;
    t->renderer = game_renderer;

    t->engine = TTF_CreateRendererTextEngine(t->renderer);
    if (t->engine == NULL)
    {
        fprintf(stderr, "Error creating renderer text engine: %s\n", SDL_GetError());
        return false;
    }

    t->font = TTF_OpenFont(TEXT_DEFAULT_FONT, TEXT_SIZE);
    if (t->font == NULL)
    {
        fprintf(stderr, "Error opening font: %s\n", SDL_GetError());
        return false;
    }

    t->string = string;
    t->string_size = size;
    t->text = TTF_CreateText(t->engine, t->font, t->string, t->string_size);
    if (t->text == NULL)
    {
        fprintf(stderr, "Error creating text: %s\n", SDL_GetError());
        return false;
    }
    
    t->position = position;

    return true;
}

void text_draw(const struct Text *t)
{
    TTF_DrawRendererText(t->text, t->position.x, t->position.y);
}

void text_free(struct Text **text)
{
    if (*text != NULL)
    {
        struct Text *t = *text;

        if (t->text != NULL)
        {
            TTF_DestroyText(t->text);
            t->text = NULL;
        }

        if (t->font != NULL)
        {
            TTF_CloseFont(t->font);
            t->font = NULL;
        }

        if (t->engine != NULL)
        {
            TTF_DestroyRendererTextEngine(t->engine);
            t->engine = NULL;
        }

        TTF_Quit();
        free(t);
        t = NULL;
        *text = NULL;
        printf("Text freed\n");
    }
}

void center_text(struct Text *t)
{
    int text_width, text_height = 0; 
    if (TTF_GetTextSize(t->text, &text_width, &text_height) == false)
    {
        t->position = (SDL_FPoint){0, 0};
    }
    else
    {
        t->position = (SDL_FPoint){(WINDOW_WIDTH - text_width) / 2.0, (WINDOW_HEIGHT - text_height) / 2.0};
    }
}