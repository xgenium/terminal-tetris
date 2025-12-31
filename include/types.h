#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define WIDTH 10 // board width
#define HEIGHT 20 // board height

typedef struct {
    int8_t x, y;
} Vec2;

typedef enum {
    NONE,
    PIECE_I,
    PIECE_O,
    PIECE_T,
    PIECE_S,
    PIECE_Z,
    PIECE_J,
    PIECE_L,
    PIECE_COUNT
} PieceType;

typedef enum {
    RED = 1, // Z
    GREEN, // S
    YELLOW, // O
    BLUE, // J
    MAGENTA, // T
    CYAN, // I
    WHITE // L
} Color;

typedef enum {
    R_DEFAULT,
    R_CLOCKWISE,
    R_REVERSE,
    R_ANTI_CLOCKWISE,
    ROTATION_COUNT
} RotationType;

typedef PieceType Cell;

typedef struct {
    Vec2 pos;
    PieceType type;
    RotationType rotation;
} ActivePiece;

typedef struct {
    Cell board[HEIGHT][WIDTH];
    ActivePiece piece;
    int8_t game_over;
    uint32_t score;
} GameState;

static const uint16_t PIECE_SHAPES[PIECE_COUNT][ROTATION_COUNT] = {
    {0x0000,0x0000,0x0000,0x0000}, // NONE
    {0x2222,0x00F0,0x4444,0x0F00}, // I
    {0x0660,0x0660,0x0660,0x0660}, // O
    {0x0E40,0x4C40,0x4E00,0x4640}, // T
    {0x06C0,0x4620,0x0360,0x2310}, // S
    {0x0C60,0x2640,0x0630,0x1320}, // Z
    {0x8E00,0x6440,0x0E20,0x44C0}, // J
    {0x2E00,0x4460,0x0E80,0xC440,} // L
};

#endif
