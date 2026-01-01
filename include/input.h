#ifndef INPUT_H
#define INPUT_H

#include "types.h"

#define INPUT_BUF_SIZE 3

void init_input();
InputType parse_input(unsigned char buf[INPUT_BUF_SIZE]);
void reset_input();

#endif
