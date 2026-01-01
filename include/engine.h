#ifndef ENGINE_H
#define ENGINE_H

#include <time.h>
#include "types.h"

#define FRAMERATE_CAP 10       // 10 ms
#define DEFAULT_TICK 500       // 500 ms
#define MIN_TICK 50	      // 50 ms

#define MICROS_TO_MS(MICROS) ((MICROS) / 1000LL)
#define MS_TO_MICROS(MS) ((MS) * 1000LL)
#define NS_TO_MS(NS) ((NS) / (1e+6))
#define MS_TO_NANOS(MS) ((MS) * (1e+6))
#define S_TO_MS(S) ((S) * 1000LL)

clock_t get_time_ms();
GameState init_game();
void handle_lines(GameState *state);
void handle_input(GameState *state);
void main_loop(GameState *state);

#endif
