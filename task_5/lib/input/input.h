#ifndef CLIENT_INPUT_H
#define CLIENT_INPUT_H

#include <stdio.h>
#include <stdlib.h>

#if !defined(SIZEOFARRAY)
#define SIZEOFARRAY(x) (sizeof((x)) / sizeof((x)[0]))
#endif

/* Takes in an input buffer and size and safely takes everything from stdin to it.
 * OBS! THe only trigger is '\n' and if size is wrong it can lead to buffer overflow
 * PARA1        char * to buffer
 * PARA2        int size of buffer*/
void input_string(char *sInput, int iSize);

/* Takes in an input buffer and size and safely takes everything from stdin to it.
 * try to convert input to int using strol.
 * OBS! only trigger is a '\n' and if size is wrong it can lead to buffer overflow
 * PARA1        char * to buffer
 * PARA2        int size of buffer
 * RETURN       int value of input or 0 failed*/
int input_int(char *sInput, int iSize);

#endif //CLIENT_INPUT_H
