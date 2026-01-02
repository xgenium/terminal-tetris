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
static void apply_input(GameState *state, InputType input);

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
    reset_all();
    exit(sig);
}

void reset_all()
{
    reset_render();
    reset_input();
}

// Initialize everything and spawn a piece
GameState init_game()
{
    init_signal_handler();
    init_render();
    init_input();
    // ... add input init
    GameState state = init_gamestate();
    spawn_piece(state.board, &state.piece, get_random_piece_type());
    return state;
}

// Initialize all signal handlers
// (use this function in higher lvl function like init_game)
static void init_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = &sigint_handler;
    sigaction(SIGINT, &sa, NULL);
}

static void apply_input(GameState *state, InputType input)
{
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

// Processes up to 2 pressed_keys and reset all pressed_keys
void process_movement(GameState *state)
{
    int applied = 0;
    for (int shift = 0; shift < MOVEMENT_COUNT && applied <= MAX_PRESSED_KEYS; shift++) {
	InputType type = 1<<shift;
	if (state->pressed_keys & type) {
	    apply_input(state, type);
	}
    }
    // reset all pressed keys
    state->pressed_keys = 0;
}

// Sets pressed_keys up to 2 currently pressed keys
void handle_input(GameState *state)
{
    unsigned char buf[INPUT_BUF_SIZE];
    int n;
    // NOTE: change this later
    state->tick = DEFAULT_TICK;
    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {

	// Parse multiple keys
	for (int i = 0; i < n; i++) {
	    InputType type;
	    int parsed_bytes = 0;
	    parsed_bytes = parse_input(&buf[i], &type);
	    state->pressed_keys |= type;
	    if (parsed_bytes > 0) i += (parsed_bytes - 1);
	}
    }
}

void main_loop(GameState *state)
{
    clock_t last_time = get_time_ms();

    while (!state->game_over) {
	clock_t current_time = get_time_ms();

	handle_input(state);
	process_movement(state);

	if (current_time - last_time >= state->tick) {
	    game_tick(state);
	    last_time = current_time;
	}

	render_game(state);
	usleep(MS_TO_MICROS(FRAMERATE_CAP));
    }
}
