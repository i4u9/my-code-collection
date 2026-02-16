#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_events.h>

#define SDL_FLAGS (SDL_INIT_VIDEO)

#define WINDOW_TITLE "test"
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 700

int i = 0;
const int linecontent = 30;

struct Game
{
    bool is_running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
};

bool game_sdl_init(struct Game *g);
bool game_new(struct Game *g);
void game_events(struct Game *g);
void game_draw(struct Game *g, int, int, int);
void game_run(struct Game *g);
void game_free(struct Game *g, int);

int main(int argc, char *argv[])
{
    bool exit_status = EXIT_FAILURE;

    struct Game game = {NULL};

    if(game_new(&game) == true)
    {
        game_run(&game);

        exit_status = EXIT_SUCCESS;       
    }
    
    game_free(&game, exit_status);
    return exit_status;
}

bool game_sdl_init(struct Game *g)
{
    if(SDL_Init(SDL_INIT_VIDEO) == false)
    {
        fprintf(stderr, "Error initialing SDL: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(g->window == NULL)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return false;
    }
    g->renderer = SDL_CreateRenderer(g->window, NULL);
    if(g->renderer == NULL)
    {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool game_new(struct Game *g)
{
    if(game_sdl_init(g) != true)
    {
        return false;
    }

    g->is_running = true;

    return true;
}


void game_events(struct Game *g)
{
    FILE *pGetevent_txt = fopen("temp.txt", "a"); 

    while(SDL_PollEvent(&g->event) == true)
    {
        if (g->event.type == SDL_EVENT_QUIT)
        {
            g->is_running = false;
        }

        fprintf(pGetevent_txt, "%d ", g->event.type);
        i++;
        printf("%d\n", i);

        if(i == linecontent)
        {
            fprintf(pGetevent_txt, "\n");
            i = 0;
        }
    }
    fclose(pGetevent_txt);
}

void game_draw(struct Game *g, int R, int G, int B)
{
    SDL_SetRenderDrawColor(g->renderer, R, G, B, 255);
    SDL_RenderClear(g->renderer);
    SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g)
{
    int RGB[3] = {0, 0, 0};

    while(g->is_running == true)
    {
        game_events(g);

        game_draw(g, RGB[0], RGB[1], RGB[2]);

        SDL_Delay(16);
    }

}

void game_free(struct Game *g, int status)
{
    if(g->window != NULL)
    {
        SDL_DestroyWindow(g->window);
        g->window = NULL;
    }

    if(g->renderer != NULL)
    {
        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
    }

    SDL_Quit();
    printf("\n%d\n", status);
}