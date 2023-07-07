#ifndef TASK_2_ANALYZENUMBERSFROMFILE_H
#define TASK_2_ANALYZENUMBERSFROMFILE_H

#include "../lib/eksamen_v23_oppgave2_abun.h"
#include "../lib/eksamen_v23_oppgave2_cube.h"
#include "../lib/eksamen_v23_oppgave2_def.h"
#include "../lib/eksamen_v23_oppgave2_fib.h"
#include "../lib/eksamen_v23_oppgave2_kvad.h"
#include "../lib/eksamen_v23_oppgave2_odd.h"
#include "../lib/eksamen_v23_oppgave2_perf.h"
#include "../lib/eksamen_v23_oppgave2_prim.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

typedef struct OPPGAVE2_TALL_METADATA {
    int iIndex; // Rekkefølge i filen, første tall = 1
    int iNumber; // Tallet, som lest fra input filen
    bool bIsFibonacci;
    bool bIsPrimeNumber;
    bool bIsSquareNumber;
    bool bIsCubeNumber;
    bool bIsPerfectNumber;
    bool bIsAbundantNumber;
    bool bIsDeficientNumber;
    bool bIsOddNumber;
} OPPGAVE2_TALL_METADATA;

#define BUFFER 1024
#define INTEGER_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c" // %c == unsigned char
// uses binary operation to map each bit in the integer to know what ascii char to print
#define INTEGER_TO_BINARY(INTEGER) \
    ((INTEGER) & 0x80 ? '1' : '0'), \
    ((INTEGER) & 0x40 ? '1' : '0'), \
    ((INTEGER) & 0x20 ? '1' : '0'), \
    ((INTEGER) & 0x10 ? '1' : '0'), \
    ((INTEGER) & 0x08 ? '1' : '0'), \
    ((INTEGER) & 0x04 ? '1' : '0'), \
    ((INTEGER) & 0x02 ? '1' : '0'), \
    ((INTEGER) & 0x01 ? '1' : '0')
#define BOOL_TO_STRING_PATTERN "%s"
#define BOOL_TO_STRING(BOOL) \
    ((BOOL) ? "True" : "False")

/* Starts the program using an url to wanted file as argument.
 * Analyze each number from the file and prints out information about it on a new file placed in ../resources/<output>
 * PARA1        string of url to file to analyze
 * RETURN       errno code. Default 0 or number if something went wrong */
int analyzeNumbersFromFile(const char *sFilePath);

/* Takes in struct for saving metadata and int number to analyze
 * PARA1        pointer to metadata struct
 * PARA2        int number to analyze */
void analyzeNumber(OPPGAVE2_TALL_METADATA *metadata, int iNumber);

/* Takes in a file writer and metadata struct to save it as readable ascii text in a file
 * PARA1        pointer to a FILE writer with modes "w"
 * PARA2        pointer to metadata struct */
void writeResultToFileAsciiText(FILE *pWriter, OPPGAVE2_TALL_METADATA *metadata);

/*Takes in a file writer and metadata struct to save all information as binary-data. Use hex reader to read file
 * PARA1        pointer to a FILE writer with modes "wb"
 * PARA2        pointer to metadata struct */
void writeResultToFilePureBinary(FILE *pWriter, OPPGAVE2_TALL_METADATA *metadata);

#endif //TASK_2_ANALYZENUMBERSFROMFILE_H