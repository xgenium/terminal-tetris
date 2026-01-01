#include "../include/render.h"
#include "../include/logic.h"
#include "../include/engine.h"
#include <stdint.h>
#include <unistd.h>
#include <time.h>

int main()
{
    GameState state = init_game();
    spawn_piece(state.board, &state.piece, PIECE_I);

    for (int y = 17; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++) {
	    state.board[y][x] = (x != 5) ? 1 : 0;
	}
    }
    clock_t last_time = get_time_ms();

    while (1) {
	clock_t current_time = get_time_ms();

	handle_input(&state);

	if (current_time - last_time >= state.tick) {
	    game_tick(&state);
	    last_time = current_time;
	}

	render_game(&state);
	usleep(MS_TO_MICROS(FRAMERATE_CAP));
    }
    return 0;
    draw_game_over();
    reset_render();
    return 0;
}
