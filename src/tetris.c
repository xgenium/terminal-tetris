#include "../include/render.h"
#include "../include/logic.h"
#include "../include/engine.h"
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main()
{
    GameState state = init_gamestate();
    init_render();

    spawn_piece(state.board, &state.piece, random() % PIECE_COUNT);
    int i = 0;
    while (!state.game_over) {
	if (i % 2) {
	    Vec2 out_pos;
	    RotationType new_rot = (state.piece.rotation + 1) % ROTATION_COUNT;
	    if (try_wall_kick(state.board, &state.piece, new_rot, &out_pos)) {
		state.piece.pos = out_pos;
		state.piece.rotation = new_rot;
	    }
	}
	render_game(&state);
	game_tick(&state);
	i++;
	usleep(MS_TO_MICROS(200));
    }
    draw_game_over();
    reset_render();
    return 0;
}
