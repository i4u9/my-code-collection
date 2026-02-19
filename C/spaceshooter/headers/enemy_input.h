#ifndef ENEMY_INPUT_H
#define ENEMY_INPUT_H

#include "game.h"

bool enemies_quantity_input(struct Game *g);
void enemies_quantity_input_draw(struct Game *g, struct Text *enemies_input_top_text);
bool enemies_quantity_input_events(struct Game *g, struct Text *enemies_input_top_text, char *error_message, int error_message_size);

#endif