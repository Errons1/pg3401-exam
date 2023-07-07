#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input_string(char *sInput, int iSize) {
    int iChar = 0;
    memset(sInput, 0, iSize);

//        Takes user input, prevent buffer overflow and empty stdin
    for (int i = 0; (iChar = getchar()) != '\n'; ++i) {
        if (i < iSize) {
            sInput[i] = (char) iChar;
        }
    }
}

int input_int(char *sInput, int iSize) {
    int iChar = 0;
    memset(sInput, 0, iSize);

//        Takes user input, prevent buffer overflow and empty stdin
    for (int i = 0; (iChar = getchar()) != '\n'; ++i) {
        if (i < iSize) {
            sInput[i] = (char) iChar;
        }
    }

    return (int) strtol(sInput, NULL, 10);
}