#include "../include/engine.h"
#include "../include/logic.h"
#include "../include/render.h"
#include "../include/input.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static void sigint_handler(int sig);
static void init_signal_handler();

clock_t get_time_ms()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return S_TO_MS(tp.tv_sec) + NS_TO_MS(tp.tv_nsec);
}

// Reset colors, show and move cursor to normal position
// (makes life easier if ctrl c was pressed)
static void sigint_handler(int sig)
{
    reset_render();
    reset_input();
    exit(sig);
}

// Initialize everything
GameState init_game()
{
    init_signal_handler();
    init_render();
    init_input();
    // ... add input init
    return init_gamestate();
}

// Initialize all signal handlers
// (use this function in higher lvl function like init_game)
static void init_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = &sigint_handler;
    sigaction(SIGINT, &sa, NULL);
}

// Handles input and sets tick to MIN_TICK if down
// arrow key is pressed
void handle_input(GameState *state)
{
    unsigned char buf[INPUT_BUF_SIZE];
    int n;
    // NOTE: change this later
    state->tick = DEFAULT_TICK;
    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
	InputType input = parse_input(buf);
	switch (input) {
	    case LEFT:
		move_piece_horizontal(state->board, &state->piece, -1);
		break;
	    case RIGHT:
		move_piece_horizontal(state->board, &state->piece, 1);
		break;
	    case DOWN:
		state->tick = MIN_TICK;
		break;
	    case ROTATE:
		rotate_piece(state->board, &state->piece);
		break;
	    case UNKNOWN: default:
		break;
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
