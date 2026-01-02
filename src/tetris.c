#include "../include/render.h"
#include "../include/engine.h"
#include "../include/logic.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    GameState state = init_game();

    if (argc == 2) {
	int lvl = atoi(argv[1]);
	if (lvl >= 1 && lvl <= 10) {
	    state.level = lvl;
	    state.total_lines_cleared = (lvl - 1) * LINES_PER_LEVEL;
	}
    }

    main_loop(&state);
    // main loop exits only when game is over
    render_game_over();
    reset_all();
    return 0;
}
