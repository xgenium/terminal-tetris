#include "../include/input.h"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

static struct termios oldt;

void init_input()
{
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// Parse c into known input types
InputType parse_input(unsigned char buf[INPUT_BUF_SIZE])
{
    // one arrow key will push 3 values into the buffer:
    // '\033', '[' and 'A' or 'B' or 'C' or 'D'
    if (buf[0] == '\033') {
	switch (buf[2]) {
	    case 'B': return DOWN;
	    case 'C': return RIGHT;
	    case 'D': return LEFT;
	    default: // we dont need UP arrow key
		return UNKNOWN;
	}
    } else { // handle other keys
	if (buf[0] == ' ') return ROTATE;
	else return UNKNOWN;
    }
}

void reset_input()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
