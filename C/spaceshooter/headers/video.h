#ifndef VIDEO_H
#define VIDEO_H

#include "main.h"

struct Video
{
    SDL_Window *window;
    SDL_Renderer *renderer; 
    bool registered_window_resize;
    bool maximize_window_by_key;
    bool window_is_maximized;
};

bool video_new(struct Video **video);
void update_window(struct Video *v);
void video_free(struct Video **video);

#endif