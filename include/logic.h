#ifndef LOGIC_H
#define LOGIC_H

#define TRANSITIONS_COUNT 8
#define SRS_KICK_TESTS 5

#define SPAWN_X 3
#define SPAWN_Y 0

#include "types.h"

// Rotations stages names:
// 0 == R_DEFAULT
// R == R_CLOCKWISE
// L == R_ANTI_CLOCKWISE
// 2 == R_REVERSE

// Transition indices: 0->R, R->2, etc.
static const Vec2 SRS_KICKS_GENERIC[TRANSITIONS_COUNT][SRS_KICK_TESTS] = {
    {{0,0}, {-1,0}, {-1,1},  {0,-2}, {-1,-2}}, // 0->R
    {{0,0}, {1, 0}, {1,-1},  {0, 2}, {1, 2}}, // R->0
    {{0,0}, {1, 0}, {1,-1},  {0, 2}, {1, 2}}, // R->2
    {{0,0}, {-1,0}, {-1,1},  {0,-2}, {-1,-2}}, // 2->R
    {{0,0}, {1, 0}, {1, 1},  {0,-2}, {1,-2}}, // 2->L
    {{0,0}, {-1,0}, {-1,-1}, {0, 2}, {-1,2}}, // L->2
    {{0,0}, {-1,0}, {-1,-1}, {0, 2}, {-1,2}}, // L->0
    {{0,0}, {1, 0}, {1, 1},  {0,-2}, {1,-2}}, // 0->L
};

static const Vec2 SRS_KICKS_I[TRANSITIONS_COUNT][SRS_KICK_TESTS] = {
    {{0,0}, {-2,0}, {1, 0},  {-2,-1}, {1, 2}}, // 0->R
    {{0,0}, {2, 0}, {-1,-0}, {2, 1},  {-1,-2}}, // R->0
    {{0,0}, {-1,0}, {2,-0},  {-1,2},  {2,-1}}, // R->2
    {{0,0}, {1, 0}, {-2,0},  {1,-2},  {-2,1}}, // 2->R
    {{0,0}, {2, 0}, {-1,0},  {2, 1},  {-1,-2}}, // 2->L
    {{0,0}, {-2,0}, {1, 0},  {-2,-1}, {1, 2}}, // L->2
    {{0,0}, {1, 0}, {-2,0},  {1,-2},  {-2,1}}, // L->0
    {{0,0}, {-1,0}, {2, 0},  {-1,2},  {2,-1}}, // 0->L
};

GameState init_gamestate();
int check_collisions(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece, Vec2 new_pos, RotationType new_rotation);
void lock_piece(Cell board[HEIGHT][WIDTH], ActivePiece *piece);
int spawn_piece(const Cell board[HEIGHT][WIDTH], ActivePiece *piece, PieceType type);
void move_piece_horizontal(const Cell board[HEIGHT][WIDTH], ActivePiece *piece, int direction);
void rotate_piece(const Cell board[HEIGHT][WIDTH], ActivePiece *piece);
int handle_full_lines(GameState *state);
void move_lines(Cell board[HEIGHT][WIDTH], const int cleared_lines[HEIGHT]);
void clear_line(Cell board[HEIGHT][WIDTH], int line);
void clear_multiple_lines(Cell board[HEIGHT][WIDTH], const int lines[HEIGHT]);
int get_full_lines(const Cell board[HEIGHT][WIDTH], int lines[HEIGHT]);
void game_tick(GameState *state);
Vec2 get_shape_bit_pos(uint16_t shape, int bit_index, Vec2 piece_pos);
int try_wall_kick(const Cell board[HEIGHT][WIDTH], const ActivePiece *piece, RotationType new_rot, Vec2 *out_pos);
int get_transition_index(RotationType old_rot, RotationType new_rot);
PieceType get_random_piece_type();

#endif
