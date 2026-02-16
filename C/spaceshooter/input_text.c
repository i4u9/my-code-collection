#include "headers/game.h"

void render_input_box(SDL_Rect area_to_box, SDL_Renderer *game_renderer, int thickness)
{
    // horizontal lines
    for (int i = 0; i < thickness; i++)
    {
        float x1 = area_to_box.x;
        float y1 = area_to_box.y;
        float x2 = x1 + area_to_box.w;
        float y2 = y1;
        SDL_RenderLine(game_renderer, x1 - i, y1 - i, x2 + i, y2 - i);
        SDL_RenderLine(game_renderer, x1 - i, y1 + area_to_box.h + i, x2 + i, y2 + area_to_box.h + i);
    }

    // vertical lines
    for (int i = 0; i < thickness; i++)
    {
        float x1 = area_to_box.x;
        float y1 = area_to_box.y;
        float x2 = x1;
        float y2 = y1 + area_to_box.h;
        SDL_RenderLine(game_renderer, x1 - i, y1 - i, x2 - i, y2 + i);
        SDL_RenderLine(game_renderer, x1 + area_to_box.w + i, y1 - i, x1 + area_to_box.w + i, y2 + i);
    }
}

// argument "int size" needs to consider '\0'
bool input_text_new(struct InputText **input_text, SDL_Renderer *game_renderer, SDL_Window *game_window, int size)
{
    *input_text = calloc(1, sizeof(struct InputText));
    if (*input_text == NULL)
    {
        fprintf(stderr, "Error callocating memory to a input text\n");
        return false;
    }
    
    struct InputText *it = *input_text;
    it->renderer = game_renderer;
    it->window = game_window;

    it->engine = TTF_CreateRendererTextEngine(it->renderer);
    if (it->engine == NULL)
    {
        fprintf(stderr, "Error creating renderer input text engine: %s\n", SDL_GetError());
        return false;
    }

    it->font = TTF_OpenFont(TEXT_DEFAULT_FONT, TEXT_SIZE);
    if (it->font == NULL)
    {
        fprintf(stderr, "Error opening input text font: %s\n", SDL_GetError());
        return false;
    }

    it->string_size = size;
    it->string = (char*)calloc(it->string_size, sizeof(char));
    if (it->string == NULL)
    {
        perror("Error callocating memory to input text string\n");
    }

    it->text_object = TTF_CreateText(it->engine, it->font, it->string, sizeof(it->string));
    if (it->text_object == NULL)
    {
        fprintf(stderr, "Error creating input text object: %s\n", SDL_GetError());
        return false;
    }

    int width = (int)(0.25 * WINDOW_WIDTH);
    int height = TEXT_SIZE;
    it->area = (SDL_Rect){(int)((WINDOW_WIDTH - width) / 2), (int)((WINDOW_HEIGHT - height) / 2), width, height};

    if (SDL_SetTextInputArea(it->window, &it->area, it->cursor) == false)
    {
        fprintf(stderr, "Error setting input text area: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_StartTextInput(it->window) == false)
    {
        fprintf(stderr, "Error starting input text: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void input_text_free(struct InputText **input_text, char *message)
{
    if (*input_text != NULL)
    {
        struct InputText *it = *input_text;

        if (it->text_object != NULL)
        {
            TTF_DestroyText(it->text_object);
            it->text_object = NULL;
        }

        if (it->font != NULL)
        {
            TTF_CloseFont(it->font);
            it->font = NULL;
        }

        if (it->engine != NULL)
        {
            TTF_DestroyRendererTextEngine(it->engine);
            it->engine = NULL;
        }

        free(it);
        it = NULL;
        *input_text = NULL;

        printf("%s", message);
    }
}