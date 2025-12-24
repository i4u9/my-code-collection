#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#define SDL_FLAGS (SDL_INIT_VIDEO)

#define WINDOW_TITLE "test"
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 950
int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

#define COLOR_WHEEL_PATH "images/colorwheel.png"
#define COLOR_WHEEL_SCALING_FACTOR 2

#define ZOOM_FACTOR 10

#define TEXT_FONT "fonts/SegoeUIVF.ttf"
#define TEXT_SIZE 30
#define TEXT_BOX_DEFAULT_COLOR (SDL_Color){68, 67, 73, 255}
#define TEXT_ALIGNMENT_DISTANCE_FROM_BOXPOS 5
#define DISTANCE_BETWEEN_BOXES 5
#define TEXT_EXIBITION_TIME_MS (uint32_t)5500

typedef struct SDL_FCircle
{
    SDL_FPoint center;
    float radius;
}SDL_FCircle;

typedef struct Box
{
    SDL_FPoint position;
    SDL_FCircle left_circle;
    SDL_FRect body;
    SDL_FCircle right_circle;
}Box;

typedef struct TextBox
{
    Box box;
    TTF_Text *text;
}TextBox;

struct Application
{
    bool is_running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event_handler;

    bool lock_image;
    char *image_path;
    SDL_FRect image_rect;
    SDL_Texture *image_texture;
    SDL_Surface *image;

    SDL_FPoint offset;  //  related to drag 'n drop functionality

    TTF_Font *text_font;
    TTF_TextEngine *text_engine;
    unsigned int text_boxes_quantity;
    TextBox *text_boxes;

    bool render_color_wheel;
    SDL_FRect color_wheel_rect;
    SDL_Surface *color_wheel;
    SDL_Texture *color_wheel_texture;
};

// Uint32 text_callback_exibition(void *userdata, SDL_TimerID timer_id, uint32_t interval);
bool show_color_picker(SDL_Surface **color_picker_surface, bool *rendering_cond, SDL_FRect *color_picker_rect);
void hide_color_picker(SDL_Surface **color_picker_surface, bool *rendering_cond, SDL_FRect *color_picker_rect);
bool query_image_path(struct Application *app);
void image_rect_align_center(SDL_FRect *image_rect);

bool app_new(struct Application *app);
Box create_box(SDL_FPoint position, float radius, float body_w);
bool create_text_box(TTF_TextEngine *engine, TTF_Font *font, TextBox **tb, unsigned int *text_boxes_quantity, char *string,
                     unsigned int *count);

void app_run(struct Application *app);

void app_events(struct Application *app);

void app_update(struct Application *app);
void UpdateTextureFromSurface(SDL_Renderer *renderer, SDL_Texture **dst_texture, SDL_Surface *src_surface);
// void update_textboxes(TextBox **tb, unsigned int *text_boxes_quantity, unsigned int *text_boxes_terminated);

void app_draw(struct Application app);
void RenderFillCircleF(SDL_Renderer *renderer, SDL_FCircle circle, const float w_lower_bound, 
                       const float w_upper_bound, const float h_lower_bound, const float h_upper_bound);
void RenderBox(SDL_Renderer *renderer, Box box, SDL_Color color);
void render_text_boxes(SDL_Renderer *renderer, TextBox *text_boxes, unsigned int text_boxes_quantity);

void app_free(struct Application *app);
bool destroy_text_boxes(TextBox **tb, unsigned int start, unsigned int *text_boxes_quantity);

#endif