#ifndef RENDER_H
#define RENDER_H

#include "types.h"

#define OFFSET_X 10
#define OFFSET_Y 5

#define FG_COLOR 30
#define BG_COLOR 40

#define MAX_ANIMATION_FRAMES 10

void init_render();
void render_game(const GameState *state);
void reset_render();
void draw_game_over();

#endif
