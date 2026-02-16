#include "game_sdl_init.c"

bool game_load_media(struct Game *g)
{   
    if (background_new(&g->background, g->video->renderer) == false)
    {
        return false;
    }

    SDL_Surface *icon_surface = IMG_Load("images\\police.jpg");
    if(icon_surface == NULL)
    {
        fprintf(stderr, "Error creating SDL surface to icon: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_SetWindowIcon(g->video->window, icon_surface) == false)
    {
        fprintf(stderr, "Error setting window icon: %s\n", SDL_GetError());
        return false;
    }
    SDL_DestroySurface(icon_surface);
    icon_surface = NULL;

    char kills_string[] = "Kills: 0";
    if (text_new(&g->text, g->video->renderer, (SDL_FPoint){50, 50}, kills_string, sizeof(kills_string)) == false)
    {
        return false;
    }
    
    return true;
}