#include "headers/enemy_input.h"

bool enemies_quantity_input_events(struct Game *g, struct Text *enemies_input_top_text, char *error_message, int error_message_size)
{
    while (SDL_PollEvent(&g->event) == true)
    {
        if (g->event.type == SDL_EVENT_KEY_DOWN && g->event.key.key == SDLK_RETURN)
        {   
            if (atoi(g->enemy_input->string) == 0)
            {
                g->enemy_input->string[0] = '\0';
                g->enemy_input->cursor = 0;
                TTF_SetTextString(g->enemy_input->text_object, g->enemy_input->string, g->enemy_input->string_size);
                TTF_SetTextString(enemies_input_top_text->text, error_message, error_message_size);
            }
            else
            {
                g->enemies_quantity = atoi(g->enemy_input->string);
                SDL_StopTextInput(g->video->window);
                g->enemy_input->input_completed = true;
            }
        }
        else if ((g->event.type == SDL_EVENT_KEY_DOWN && g->event.key.key == SDLK_ESCAPE) || g->event.type == SDL_EVENT_QUIT)
        {
            input_text_free(&g->enemy_input, "Enemy input freed\n");
            return false;
        }
        else if (g->event.type == SDL_EVENT_KEY_DOWN && g->event.key.key == SDLK_BACKSPACE)
        {
            if (g->enemy_input->cursor - 1 >= 0)
            {
                g->enemy_input->cursor--;
                g->enemy_input->string[g->enemy_input->cursor] = '\0';
            }
            else
            {
                g->enemy_input->cursor = 0;
            }
            TTF_SetTextString(g->enemy_input->text_object, g->enemy_input->string, g->enemy_input->string_size);
            // printf("cursor: %d\n", g->enemy_input->cursor);
        }
        else if (g->event.type == SDL_EVENT_TEXT_INPUT) 
        {
            if (g->enemy_input->cursor + 1 == g->enemy_input->string_size)
            {
                printf("Cannot exceed maximum input buffer of size %d\n", g->enemy_input->string_size);
            }
            else
            {
                // printf("text input event\n");
                SDL_strlcat(g->enemy_input->string, g->event.text.text, g->enemy_input->string_size);
                TTF_SetTextString(g->enemy_input->text_object, g->enemy_input->string, g->enemy_input->string_size);
                g->enemy_input->cursor++;
                // printf("cursor pos: %d\n", g->enemy_input->cursor);
            }
        }
    }

    return true;
}

void enemies_quantity_input_draw(struct Game *g, struct Text *enemies_input_top_text)
{
    SDL_SetRenderDrawColor(g->video->renderer, renderer_color.r, renderer_color.g, renderer_color.b, renderer_color.a);
    SDL_RenderClear(g->video->renderer);

    background_draw(g->background);
    text_draw(enemies_input_top_text);
    TTF_DrawRendererText(g->enemy_input->text_object, (float)g->enemy_input->area.x, (float)g->enemy_input->area.y);
    SDL_SetTextInputArea(g->video->window, &g->enemy_input->area, g->enemy_input->cursor);
    render_input_box(g->enemy_input->area, g->video->renderer, INPUT_BOX_THICKNESS);

    SDL_RenderPresent(g->video->renderer);
}

bool enemies_quantity_input(struct Game *g)
{
    struct Text *enemies_input_top_text = {0};
    char initial_message[] = "Enter quantity of enemies:";
    char error_message[] = "Please, enter only numbers!";

    if (input_text_new(&g->enemy_input, g->video->renderer, g->video->window, ENEMY_INPUT_SIZE) == false ||
        text_new(&enemies_input_top_text, g->video->renderer,  TEXT_DEFAULT_POSITION, "Enter quantity of enemies:", sizeof(initial_message)) == false)
    {
        return false;
    }
    center_text(enemies_input_top_text);
    enemies_input_top_text->position.y = 0; 
    
    while (g->enemy_input->input_completed == false)
    {
        if (enemies_quantity_input_events(g, enemies_input_top_text, error_message, sizeof(error_message)) == false)
        {
            return false;
        }
        enemies_quantity_input_draw(g, enemies_input_top_text);
    }
    
    // printf("%s\n", g->enemy_input->string);
    input_text_free(&g->enemy_input, "Enemy input freed\n");

    return true;
}