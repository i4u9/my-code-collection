#include "headers/game.h"

bool video_new(struct Video **video)
{
    *video = calloc(1, sizeof(struct Video));
    if (*video == NULL)
    {
        fprintf(stderr, "Error callocating memory to a new game\n");
        return false;
    }

    struct Video *v = *video;

    if(SDL_Init(SDL_INIT_VIDEO) == false)
    {
        fprintf(stderr, "Error initialing SDL video: %s\n", SDL_GetError());
        return false;
    }

    v->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if(v->window == NULL)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return false;
    }

    v->renderer = SDL_CreateRenderer(v->window, NULL);
    if(v->renderer == NULL)
    {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void update_window(struct Video *v)
{
    if (v->maximize_window_by_key == true)
    {
        if (v->window_is_maximized == true)
        {
            printf("minimizing window!\n");
            SDL_RestoreWindow(v->window);
            v->window_is_maximized = false;
        }
        else
        {
            printf("maximizing window!\n");
            SDL_MaximizeWindow(v->window);
            v->window_is_maximized = true;
        }
        v->maximize_window_by_key = false;
    }

    if (v->registered_window_resize == true)
    {
        int next_window_width, next_window_height = 0;
        SDL_GetWindowSize(v->window, &next_window_width, &next_window_height);

        if (next_window_width < MIN_WINDOW_WIDTH || next_window_height < MIN_WINDOW_HEIGTH)
        {
            SDL_Log("invalid window resizing!\n");
            SDL_SetWindowSize(v->window, window_width, window_height);
        }
        else
        {
            SDL_Log("resizing window and elements contained!\n");
            SDL_SetRenderLogicalPresentation(v->renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        }

        v->registered_window_resize = false;
    }
}

void video_free(struct Video **video)
{
    if (*video != NULL)
    {
        struct Video *v = *video;

        if (v->renderer != NULL)
        {
            SDL_DestroyRenderer(v->renderer);
            v->renderer = NULL;
        }

        if (v->window != NULL)
        {
            SDL_DestroyWindow(v->window);
            v->window = NULL;
        }

        free(v);
        v = NULL;
        *video = NULL;

        printf("Video freed\n");
    }
}