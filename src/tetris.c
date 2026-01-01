#include "../include/render.h"
#include "../include/logic.h"
#include "../include/engine.h"
#include <stdint.h>
#include <unistd.h>

int main()
{
    GameState state = init_game();
    spawn_piece(state.board, &state.piece, PIECE_I);

    for (int y = 17; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++) {
	    state.board[y][x] = (x != 5) ? 1 : 0;
	}
    }

    while (!state.game_over) {
	// handle_lines(&state);
	render_game(&state);
	game_tick(&state);
	usleep(MS_TO_MICROS(200));
    }
    draw_game_over();
    reset_render();
    return 0;
}
