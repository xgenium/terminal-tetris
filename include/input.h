#ifndef INPUT_H
#define INPUT_H

#include "types.h"

#define ARROW_KEY_SIZE 3

void init_input();
int parse_input(unsigned char buf[ARROW_KEY_SIZE], InputType *type);
void reset_input();

#endif
