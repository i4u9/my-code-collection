#include "input_text.c"
#include "text.c"
#include "video.c"
#include "background.c"
#include "player.c"
#include "enemy_input.c"

#include "enemy.c"
bool game_sdl_init(struct Game *g)
{
    if (video_new(&g->video) == false)
    {
        return false;
    }
    
    if (TTF_Init() == false)
    {
        fprintf(stderr, "Error initializing SDL TTF: %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}