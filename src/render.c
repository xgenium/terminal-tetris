#include "../include/render.h"
#include <stdint.h>
#include <stdio.h>

static Cell shadow_buffer[HEIGHT][WIDTH];

static void clear_screen();
static void init_shadow_buffer();
static void set_color_mode();
static void set_fg_color(uint8_t color);
static void set_bg_color(uint8_t color);
static void reset_colors();
static uint8_t get_color_by_type(PieceType type);
static void hide_cursor();
// static void reset_cursor();
static void show_cursor();
static void move_cursor(int8_t x, int8_t y);
static void compose_frame(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece, Cell frame[HEIGHT][WIDTH]);
static void render_cell(Cell cell);
static void render_board(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece);
static void render_outline();
static void render_hud(int16_t lines_cleared, int8_t level);
// TODO:
// static void render_score(uint32_t score);

static void init_shadow_buffer()
{
    for (int y = 0; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++) {
	    shadow_buffer[y][x] = 0;
	}
    }
}

void init_render()
{
    clear_screen();
    hide_cursor();
    set_color_mode();
    init_shadow_buffer();
    render_outline();
}

static void clear_screen()
{
    printf("\033[2J");
}

// Set 40x25 color (text) mode
static void set_color_mode()
{
    printf("\033[=1h");
}

static void reset_colors()
{
    printf("\033[0m");
}

static void set_fg_color(uint8_t color)
{
    printf("\033[%dm", color + FG_COLOR);
}

static void set_bg_color(uint8_t color)
{
    printf("\033[%dm", color + BG_COLOR);
}

static uint8_t get_color_by_type(PieceType type)
{
    switch (type) {
	case PIECE_I: return CYAN;
	case PIECE_O: return YELLOW;
	case PIECE_T: return MAGENTA;
	case PIECE_S: return GREEN;
	case PIECE_Z: return RED;
	case PIECE_J: return BLUE;
	case PIECE_L: return WHITE;
	case NONE: default: return 0;
    }
}

static void hide_cursor()
{
    printf("\033[?25l");
}

// static void reset_cursor()
// {
//     printf("\033[H");
// }

static void show_cursor()
{
    printf("\033[?25h");
}

static void move_cursor(int8_t x, int8_t y)
{
    // Lines and columns starts from 1 in terminal
    printf("\033[%d;%dH", y+1, x+1);
}

static void compose_frame(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece, Cell frame[HEIGHT][WIDTH])
{
    // copy board to frame
    for (int y = 0; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++)  {
	    frame[y][x] = board[y][x];
	}
    }
    uint16_t shape = PIECE_SHAPES[piece->type][piece->rotation];
    // copy active piece to frame
    for (int r = 0; r < 4; r++) {
	for (int c = 0; c < 4; c++) {
	    if (1 & (shape >> (15 - (r * 4 + c)))) {
		frame[piece->pos.y + r][piece->pos.x + c] = piece->type;
	    }
	}
    }
}

static void render_cell(Cell cell) {
    if (cell == 0)
	printf("  ");
    else
	printf("[]");
}

static void render_board(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece)
{
    Cell temp_buffer[HEIGHT][WIDTH];
    compose_frame(board, piece, temp_buffer);

    for (int y = 0; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++) {

	    Cell current = temp_buffer[y][x];

	    if (current != shadow_buffer[y][x]) {
		// for each x in board there is 2 chars in output
		move_cursor(OFFSET_X + x*2, OFFSET_Y + y);
		set_fg_color(get_color_by_type(current));
		set_bg_color(BLACK);
		render_cell(current);
		shadow_buffer[y][x] = current;
	    }
	}
    }
    fflush(stdout);
}

static void render_outline()
{
    for (int x = 0; x < WIDTH*2; x++) {
	// top
	move_cursor(OFFSET_X + x, OFFSET_Y - 1);
	printf("_");
	// bottom
	move_cursor(OFFSET_X + x, OFFSET_Y + HEIGHT);
	printf("-");
    }
    for (int y = 0; y < HEIGHT; y++) {
	// left side
	move_cursor(OFFSET_X - 1, OFFSET_Y + y);
	printf("|");
	// right side
	move_cursor(OFFSET_X + WIDTH*2, OFFSET_Y + y);
	printf("|");
    }
    fflush(stdout);
}

void render_hud(int16_t lines_cleared, int8_t level)
{
    set_bg_color(WHITE);
    set_fg_color(BLACK);

    move_cursor(OFFSET_X + WIDTH * 2 + 1, OFFSET_Y);
    printf("Lines cleared: %d", lines_cleared);

    move_cursor(OFFSET_X + WIDTH * 2 + 1, OFFSET_Y + OFFSET_Y/2);
    printf("Level: %d", level);
}

void render_game(const GameState *state)
{
    render_board(state->board, &state->piece);
    render_hud(state->total_lines_cleared, state->level);
}

// Reset colors, show and move cursor to normal position
void reset_render()
{
    reset_colors();
    // Move cursor after game board + offset_y
    move_cursor(1, OFFSET_Y*2 + HEIGHT);
    show_cursor();
    fflush(stdout);
}

void render_game_over()
{
    move_cursor(OFFSET_X + WIDTH/2, OFFSET_Y + HEIGHT/2);
    set_fg_color(WHITE);
    set_bg_color(RED);
    printf("GAME OVER");
}
