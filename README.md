# Terminal-Tetris

![Demo](https://github.com/xgenium/terminal-tetris/blob/main/demo.gif)

A simple Tetris clone that runs entirely in the terminal, written in C.

## Requirements

- C compiler (e.g. `cc`, `gcc`, or `clang`)
- `make`
- A Unix-like environment (Linux, macOS, etc)
- ANSI-capable terminal

## Build

By default, the game is built into `./build/tetris`.

```bash
# Clone the repository
git clone https://github.com/xgenium/terminal-tetris.git
cd terminal-tetris

# Release build (optimized, default target)
make

# Or debug build (with debug symbols, lower optimization)
make debug
```

## Run

After building, run the game with:

```bash
./build/tetris
```

You can optionally specify a starting level (from 1 to 10)

```bash
# Start from level 1 (default if no argument is given)
./build/tetris

# Start from level 5
./build/tetris 5
```

If an invalid level is given (outside 1–10), the game falls back to the default level.

## Controls

Keyboard input is processed in `src/input.c` using raw terminal mode and escape sequences. The recognized controls are:

- **Left arrow**: move piece left
- **Right arrow**: move piece right
- **Down arrow**: increase fall speed (soft drop)
- **Spacebar**: rotate piece
- **Ctrl+C**: quit immediately (handled via signal; resets terminal state)

Notes:

- Arrow keys are parsed from escape sequences (`\033`, `[`, and `B/C/D` for down/right/left).
- The up arrow is explicitly ignored (no action).
- Space (`' '`) is mapped to `ROTATE`.

Signal handling (in `src/engine.c`) ensures that if you interrupt the game with `Ctrl+C`, the terminal colors and cursor visibility are reset via `reset_all()`.

## Code Structure

The code is split into small, focused modules:

- **Game loop & engine** (`src/engine.c`, `src/tetris.c`)
  Initializes the game (rendering, input, state), runs the main loop, and cleans up on exit or `Ctrl+C`.

- **Game logic** (`src/logic.c`)
  Manages the board, active piece, collision checks, locking pieces, spawning new pieces, and level progression based on cleared lines.

- **Input** (`src/input.c`)
  Puts the terminal into non-canonical mode, parses arrow keys and space into a small set of input actions, and restores settings on exit.

- **Rendering** (`src/render.c`)
  Draws the board and active piece using ANSI escape codes and colors, hides/shows the cursor, and keeps a shadow buffer to avoid unnecessary redraws.

Shared types and constants (board size, tick timing, piece shapes, etc.) live in the headers under `include/`.

## License

This project is licensed under the MIT License.
