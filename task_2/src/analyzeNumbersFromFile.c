#include "analyzeNumbersFromFile.h"

int analyzeNumbersFromFile(const char *sFilePath) {
    FILE *pReader = fopen(sFilePath, "r");
    FILE *pWriterAscii = fopen("../resources/outputAscii", "w");
    FILE *pWriterBinary = fopen("../resources/outputBinary", "wb");
    char sBuffer[BUFFER] = {0};
    char cBuffer = 0;
    int iNumber = 0;
    OPPGAVE2_TALL_METADATA *metadata = NULL;

//    Main loop to go through read number from file and analyze them
    for (int i = 0; (cBuffer = (char) fgetc(pReader)) != EOF; ++i) {
        sBuffer[i] = cBuffer;

        if (cBuffer == '\n') {
            iNumber = (int) strtol(sBuffer, NULL, 10);
            memset(sBuffer, 0, BUFFER); // reset buffer for next number

            metadata = (OPPGAVE2_TALL_METADATA *) calloc(1, sizeof(OPPGAVE2_TALL_METADATA));
            analyzeNumber(metadata, iNumber);
            writeResultToFileAsciiText(pWriterAscii, metadata);
            writeResultToFilePureBinary(pWriterBinary, metadata);

            free(metadata);
            i = -1; // Is to reset the index for the sBuffer
        }
    }

    fclose(pReader);
    fclose(pWriterAscii);
    fclose(pWriterBinary);
    return errno;
}

void analyzeNumber(OPPGAVE2_TALL_METADATA *metadata, int iNumber) {
    static int iIndex = 1;

    metadata->iIndex = iIndex++;
    metadata->iNumber = iNumber;
    metadata->bIsFibonacci = isFibonacci(iNumber);
    metadata->bIsPrimeNumber = isPrime(iNumber);
    metadata->bIsSquareNumber = isSquareNumber(iNumber);
    metadata->bIsCubeNumber = isCubeNumber(iNumber);
    metadata->bIsPerfectNumber = isPerfectNumber(iNumber);
    metadata->bIsAbundantNumber = isAbundantNumber(iNumber);
    metadata->bIsDeficientNumber = isDeficientNumber(iNumber);
    metadata->bIsOddNumber = isOdd(iNumber);
}

void writeResultToFileAsciiText(FILE *pWriter, OPPGAVE2_TALL_METADATA *metadata) {
    fprintf(pWriter, "Index of number: %d\n", metadata->iIndex);
    fprintf(pWriter, "Binary of number: "INTEGER_TO_BINARY_PATTERN"\n", INTEGER_TO_BINARY(metadata->iNumber));
    fprintf(pWriter, "Is number Fibonacci: "BOOL_TO_STRING_PATTERN"\n", BOOL_TO_STRING(metadata->bIsFibonacci));
    fprintf(pWriter, "Is number Prime: "BOOL_TO_STRING_PATTERN"\n", (BOOL_TO_STRING(metadata->bIsPrimeNumber)));
    fprintf(pWriter, "Is number Square: "BOOL_TO_STRING_PATTERN"\n", (BOOL_TO_STRING(metadata->bIsSquareNumber)));
    fprintf(pWriter, "Is number Cube: "BOOL_TO_STRING_PATTERN"\n", (BOOL_TO_STRING(metadata->bIsCubeNumber)));
    fprintf(pWriter, "Is number Perfect: "BOOL_TO_STRING_PATTERN"\n", (BOOL_TO_STRING(metadata->bIsPerfectNumber)));
    fprintf(pWriter, "Is number Abundant: "BOOL_TO_STRING_PATTERN"\n", (BOOL_TO_STRING(metadata->bIsAbundantNumber)));
    fprintf(pWriter, "Is number Deficient: "BOOL_TO_STRING_PATTERN"\n", (BOOL_TO_STRING(metadata->bIsDeficientNumber)));
    fprintf(pWriter, "Is number Odd: "BOOL_TO_STRING_PATTERN"\n\n", (BOOL_TO_STRING(metadata->bIsOddNumber)));
}

void writeResultToFilePureBinary(FILE *pWriter, OPPGAVE2_TALL_METADATA *metadata) {
    fwrite(&metadata->iIndex, sizeof(int), 1, pWriter);
    fwrite(&metadata->iNumber, sizeof(int), 1, pWriter);
    fwrite(&metadata->bIsFibonacci, sizeof(bool), 1, pWriter);
    fwrite(&metadata->bIsPrimeNumber, sizeof(bool), 1, pWriter);
    fwrite(&metadata->bIsSquareNumber, sizeof(bool), 1, pWriter);
    fwrite(&metadata->bIsCubeNumber, sizeof(bool), 1, pWriter);
    fwrite(&metadata->bIsPerfectNumber, sizeof(bool), 1, pWriter);
    fwrite(&metadata->bIsAbundantNumber, sizeof(bool), 1, pWriter);
    fwrite(&metadata->bIsDeficientNumber, sizeof(bool), 1, pWriter);
    fwrite(&metadata->bIsOddNumber, sizeof(bool), 1, pWriter);
}