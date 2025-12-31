#include "../include/engine.h"
// #include "../include/logic.h"
// #include "../include/render.h"
#include <unistd.h>

static void reset_board(Cell board[HEIGHT][WIDTH]);

clock_t get_time_ms()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return S_TO_MS(tp.tv_sec) + NS_TO_MS(tp.tv_nsec);
}

GameState init_gamestate()
{
    GameState state;
    state.game_over = 0;
    reset_board(state.board);
    return state;
}

static void reset_board(Cell board[HEIGHT][WIDTH])
{
    for (int y = 0; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++) {
	    board[y][x] = 0;
	}
    }
}

/*int main()
{
    clock_t last_time = get_time_ms();
    clock_t accumulator = 0;
    GameState state;

    while (1) {
	clock_t current_time = get_time_ms();
	if (current_time - last_time >= FALL_SPEED) {
	    game_tick(&state);
	    last_time = current_time;
	}

	render_game(&state);
	usleep(MS_TO_MICROS(FRAMERATE_CAP));
    }
    return 0;
}*/
