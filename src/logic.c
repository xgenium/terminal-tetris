#include "../include/logic.h"
#include <stdlib.h>
#include <unistd.h>

static void reset_board(Cell board[HEIGHT][WIDTH]);

// Inititialize and GameState with zeroed values
GameState init_gamestate()
{
    GameState state;
    state.game_over = 0;

    // maybe change later to STATE_GAME_START or something
    // state.current_state = STATE_PLAYING;
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

// returns 1 if collides with locked piece; 2 if with bounds
int check_collisions(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece, Vec2 new_pos, RotationType new_rotation)
{
    uint16_t shape = PIECE_SHAPES[piece->type][new_rotation];
    for (int i = 0; i < 16; i++) {
	if (shape & (0x8000 >> i)) {
	    Vec2 bit_pos = get_shape_bit_pos(shape, i, new_pos);

	    // check wall/floor bounds
	    if (bit_pos.x < 0 || bit_pos.x >= WIDTH || bit_pos.y >= HEIGHT)
		return 2;

	    // board collision with locked pieces
	    // NOTE: y < 0 is for spawning pieces at the top
	    if (bit_pos.y >= 0) {
		if (board[bit_pos.y][bit_pos.x])
		    return 1;
	    }
	}
    }
    return 0;
}

void lock_piece(Cell board[HEIGHT][WIDTH], ActivePiece *piece)
{
    Vec2 bit_pos;
    uint16_t shape = PIECE_SHAPES[piece->type][piece->rotation];
    for (int i = 0; i < 16; i++) {
	if (shape & (0x8000 >> i)) {
	    bit_pos = get_shape_bit_pos(shape, i, piece->pos);
	    board[bit_pos.y][bit_pos.x] = piece->type;
	}
    }
}

// returns 0 on fail (no space to spawn)
int spawn_piece(const Cell board[HEIGHT][WIDTH], ActivePiece *piece, PieceType type)
{
    Vec2 spawn_pos = { SPAWN_X, SPAWN_Y };

    ActivePiece test_piece = *piece;
    test_piece.pos = spawn_pos;
    test_piece.rotation = R_DEFAULT;
    test_piece.type = type;
    if (!check_collisions(board, &test_piece, spawn_pos, test_piece.rotation)) {
	*piece = test_piece;
    } else {
	return 0;
    }
    return 1;
}

// Move right if direction is positive or left if negative;
// if direction == 0: do nothing
void move_piece_horizontal(const Cell board[HEIGHT][WIDTH], ActivePiece *piece, int direction)
{
    if (direction == 0) return;

    Vec2 new_pos = piece->pos;

    if (direction > 0) new_pos.x++;
    else new_pos.x--;

    if (!check_collisions(board, piece, new_pos, piece->rotation))
	piece->pos = new_pos;
}

void rotate_piece(const Cell board[HEIGHT][WIDTH], ActivePiece *piece)
{
    RotationType new_rot = (piece->rotation + 1) % ROTATION_COUNT;
    Vec2 new_pos;
    if (try_wall_kick(board, piece, new_rot, &new_pos)) {
	piece->rotation = new_rot;
	piece->pos = new_pos;
    }
}

// Find and clear all full lines and add score
// Returns 1 if clear happened, 0 otherwise
int handle_full_lines(GameState *state)
{
    // maybe move to engine.c
    int lines[HEIGHT], line_count;

    if ((line_count = get_full_lines(state->board, lines)) > 0) {
	clear_multiple_lines(state->board, lines);
	// add score based on 'line_count'
	// change state to animation
	move_lines(state->board, lines);
	return 1;
    }
    return 0;
}

void move_lines(Cell board[HEIGHT][WIDTH], const int cleared_lines[HEIGHT])
{
    int shift = 0;
    int cleared_line_idx = 0;
    // start from the bottom up
    for (int y = HEIGHT-1; y >= 0; y--)
	if (cleared_lines[cleared_line_idx] == y) {
	    shift++;
	    cleared_line_idx++;
	} // if we have gaps below us, move this row down
	else if (shift > 0) {
	    for (int x = 0; x < WIDTH; x++) {
		board[y + shift][x] = board[y][x];
		board[y][x] = 0;
	    }
	}
}

void clear_line(Cell board[HEIGHT][WIDTH], int line)
{
    for (int x = 0; x < WIDTH; x++)
	board[line][x] = 0;
}

void clear_multiple_lines(Cell board[HEIGHT][WIDTH], const int lines[HEIGHT])
{
    for (int i = 0; i < HEIGHT && lines[i] != -1; i++) {
	clear_line(board, lines[i]);
    }
}

// Writes indexes of full lines to lines[] (last index of array is -1);
// Returns amount of full lines or 0 if no lines were found;
int get_full_lines(const Cell board[HEIGHT][WIDTH], int lines[HEIGHT])
{
    int i = 0;
    // search from bottom to top
    for (int row = HEIGHT-1; row >= 0; row--) {
	int filled_cells = 0;
	for (int col = 0; col < WIDTH; col++) {
	    if (board[row][col]) filled_cells++;
	}
	if (filled_cells >= WIDTH) lines[i++] = row;
    }
    // important!!!
    lines[i] = -1;

    return i;
}

void game_tick(GameState *state)
{
    Vec2 new_pos = state->piece.pos;
    new_pos.y++;
    int8_t collision_type;
    if (!(collision_type = check_collisions(state->board, &state->piece, new_pos, state->piece.rotation))) {
	state->piece.pos = new_pos;
    } else if (collision_type == 1) { // lock piece only if active piece collides with locked pieces
	lock_piece(state->board, &state->piece);
	// clear lines before spawning next piece
	handle_full_lines(state);

	if (!spawn_piece(state->board, &state->piece, random() % PIECE_COUNT))
	    state->game_over = 1;
    }

}

Vec2 get_shape_bit_pos(uint16_t shape, int bit_index, Vec2 piece_pos)
{
    int row = bit_index / 4;
    int col = bit_index % 4;
    return (Vec2) { piece_pos.x + col,  piece_pos.y + row };
}

// writes valid pos to out_pos and returns 1 if found; 0 otherwise
int try_wall_kick(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece, RotationType new_rot, Vec2 *out_pos)
{
    int transition_idx = get_transition_index(piece->rotation, new_rot);
    const Vec2 (*kick_table)[SRS_KICK_TESTS] = (piece->type == PIECE_I) ? SRS_KICKS_I : SRS_KICKS_GENERIC;

    for (int i = 0; i < SRS_KICK_TESTS; i++) {

	Vec2 offset = kick_table[transition_idx][i];

	Vec2 test_pos = {
	    piece->pos.x + offset.x,
	    piece->pos.y - offset.y // flipped because Y increases downwards
	};

	if (!check_collisions(board, piece, test_pos, new_rot)) {
	    *out_pos = test_pos; // found valid pos
	    return 1;
	}
    }
    return 0;
}

int get_transition_index(RotationType old_rot, RotationType new_rot)
{
    int transition_idx;
    // dont touch
    switch (old_rot) {
	case ROTATION_COUNT: // dont handle
	case R_DEFAULT:
	    if (new_rot == R_CLOCKWISE) transition_idx = 0;
	    if (new_rot == R_ANTI_CLOCKWISE) transition_idx = 7;
	    break;
	case R_CLOCKWISE:
	    if (new_rot == R_DEFAULT) transition_idx = 1;
	    if (new_rot == R_REVERSE) transition_idx = 2;
	    break;
	case R_REVERSE:
	    if (new_rot == R_CLOCKWISE) transition_idx = 3;
	    if (new_rot == R_ANTI_CLOCKWISE) transition_idx = 4;
	    break;
	case R_ANTI_CLOCKWISE:
	    if (new_rot == R_REVERSE) transition_idx = 5;
	    if (new_rot == R_DEFAULT) transition_idx = 6;
	    break;
    }
    return transition_idx;
}
