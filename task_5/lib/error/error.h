#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>

/*
 * Returns documentation notes on what errno error means
 * RETURN       String describing the errno error */
char * errnoLookUp();

/*
 * Takes in custom message and value to check, if iCheck < 0 then print message and errno value
 * PARA1            Custom string message to print
 * PARA2            Value to check is less than 0
 * return           value of iCheck */
int error_warning(char *szMessage, int iCheck);

/*
 * Takes in custom message and value to check, if iCheck < 0 then print message, errno value and exit program with
 * errno value
 * PARA1            Custom string message to print
 * PARA2            Value to check is less than 0 */
void error_exit(char *szMessage, int iCheck);

#endif //ERROR_H
