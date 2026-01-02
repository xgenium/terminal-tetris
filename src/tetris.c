#include "../include/render.h"
#include "../include/engine.h"
#include <stdint.h>
#include <unistd.h>

int main()
{
    GameState state = init_game();
    main_loop(&state);
    // main loop exits only when game is over
    render_game_over();
    reset_all();
    return 0;
}
