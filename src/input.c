#include "../include/input.h"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#ifdef _WIN32
static DWORD oldDwMode;
static COMMTIMEOUTS oldTimeouts;
#else
static struct termios oldt;
#endif

static InputType parse_arrow_key(unsigned char buf[ARROW_KEY_SIZE]);
static InputType parse_usual_key(unsigned char k);

void init_input()
{
#ifdef _WIN32
    // Enable non cannonical mode and disable echo
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hIn, &oldDwMode);
    dwMode = oldDwMode;
    dwMode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(hIn, dwMode);
    // Enable non blocking input
    COMMTIMEOUTS newTimeouts;
    GetCommTimeouts(hIn, &oldTimeouts);

    newTimeouts = oldTimeouts;
    newTimeouts.ReadIntervalTimeout = MAXDWORD;
    newTimeouts.ReadTotalTimeoutMultiplier = 0;
    newTimeouts.ReadTotalTimeoutConstant = 0;

    SetCommTimeouts(hIn, &newTimeouts);
#else
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif
}

// Parse buf into known input types and return amount of parse bytes
int parse_input(unsigned char buf[ARROW_KEY_SIZE], InputType *type)
{
    // one arrow key will push 3 values into the buffer:
    // '\033', '[' and 'A' or 'B' or 'C' or 'D'
    if (buf[0] == '\033') {
        *type = parse_arrow_key(buf);
        return ARROW_KEY_SIZE;
    } else { // handle other keys
        *type = parse_usual_key(buf[0]);
        return 1;
    }
}

static InputType parse_arrow_key(unsigned char buf[ARROW_KEY_SIZE])
{
    if (buf[0] == '\033' && buf[1] == '[') {
        switch (buf[2]) {
            case 'B': return DOWN;
            case 'C': return RIGHT;
            case 'D': return LEFT;
            default: // we dont need UP arrow key
                break;
        }
    }
    return UNKNOWN;
}

static InputType parse_usual_key(unsigned char k)
{
    if (k == ' ') { // Spacebar
        return ROTATE;
    }
    return UNKNOWN;
}

void reset_input()
{
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hIn, oldDwMode);
    SetCommTimeouts(hIn, &oldTimeouts);
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}
