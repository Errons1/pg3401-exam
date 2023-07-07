#include "main.h"

int main(int iArgC, char *apszArgV[]) {
    if (iArgC != 2) {
        printf("Wrong amount of arguments\n");
        printf("Use one arguments that is filepath to .c file to beautify\n");
        exit(1);
    }

    char *sFile = NULL;
    char *sNewFile = NULL;
    char *sBeautifyFile = NULL;
    int iFileSizeCounter = 0;
    char cTmp = 0;
    char *szBeautified = "_beautified.c";
    char *szNewFileName = (char *) calloc(1, strlen(apszArgV[1]) + strlen(szBeautified) + 1);
    FILE *pFileRead = fopen(apszArgV[1], "r");
    FILE *pFileWrite = NULL;

    memcpy(szNewFileName, apszArgV[1], strlen(apszArgV[1]));
    strcpy(&szNewFileName[strlen(szNewFileName) - 2], szBeautified);
    pFileWrite = fopen(szNewFileName, "w");

//    check size of file
    while ((cTmp = (char) fgetc(pFileRead)) != EOF) {
        iFileSizeCounter++;
    }

//    reset reader and get pointers for strings
    cTmp = 0;
    rewind(pFileRead);
    sFile = (char *) calloc(1, iFileSizeCounter + 1);
    sNewFile = (char *) calloc(1, iFileSizeCounter * 2 + 1);

//    file to string
    for (int i = 0; (cTmp = (char) fgetc(pFileRead)) != EOF; ++i) {
        sFile[i] = cTmp;
    }

    spaceBeautify(sFile, sNewFile);
    free(sFile);
    sFile = sNewFile;
    sNewFile = (char *) calloc(1, strlen(sFile) * 2);

    hungarianNotationBeautify(sFile, sNewFile);
    free(sFile);
    sFile = sNewFile;
    sNewFile = (char *) calloc(1, strlen(sFile) * 2);

    loopBeautify(sFile, sNewFile);

    fprintf(pFileWrite, "%s", sNewFile);
    sBeautifyFile = (char *) calloc(1, iFileSizeCounter);

    fclose(pFileRead);
    fclose(pFileWrite);
    free(sFile);
    free(sNewFile);
    free(sBeautifyFile);
    return 0;
}

void spaceBeautify(char *sFile, char *sNewFile) {
    char *szSpace = "   ";
    char *szTmp = (char *) calloc(1, BUFFER);
    char cTab = 9; // ASCII 9 == Horizontal tab
    char cTmp = 0;
    int iIndexOld = 0;
    int iIndexNew = 0;

    while ((cTmp = sFile[iIndexOld]) != '\0') {
        if (cTmp == ' ') {
            if (memcmp(&sFile[iIndexOld], szSpace, strlen(szSpace)) == 0) {
                sNewFile[iIndexNew] = cTab;
                iIndexNew++;
                iIndexOld += 3;
                continue;
            }
        }

        sNewFile[iIndexNew] = cTmp;
        iIndexNew++;
        iIndexOld++;
    }

    free(szTmp);
}

void hungarianNotationBeautify(char *sFile, char *sNewFile) {
    char *szUnsignedInt = "unsigned int ";
    char *apszVariables[BUFFER] = {0};
    char *szTmp = (char *) calloc(1, BUFFER);
    char cTmp = 0;
    int iIndexOld = 0;
    int iIndexNew = 0;
    int iIndexVariables = 0;

    while ((cTmp = sFile[iIndexOld]) != '\0') {
//        check for phrase unsigned int to add the variable to the list of names to check
        if (cTmp == 'u') {
            if (memcmp(&sFile[iIndexOld], szUnsignedInt, strlen(szUnsignedInt)) == 0) {

//                insert "unsigned int " to the strings
                for (int i = 0; i < strlen(szUnsignedInt); ++i) {
                    cTmp = sFile[iIndexOld];
                    sNewFile[iIndexNew] = cTmp;
                    iIndexNew++;
                    iIndexOld++;
                }

//                get variable name
                for (int i = 0; (cTmp = sFile[i + iIndexOld]) != ' '; ++i) {
                    szTmp[i] = (sFile[i + iIndexOld]);
                }
//                jump indexes forward and then save it to array of variable names to change to ui<variable>
                iIndexOld += (int) strlen(szTmp);
                apszVariables[iIndexVariables] = (char *) calloc(1, strlen(szTmp) + 1);
                memcpy(apszVariables[iIndexVariables], szTmp, strlen(szTmp));
                iIndexVariables++;

//                write the new name into the new string
                sNewFile[iIndexNew++] = 'u';
                sNewFile[iIndexNew++] = 'i';
                for (int i = 0; i < strlen(szTmp); ++i) {
                    if (i == 0) {
                        sNewFile[iIndexNew++] = (szTmp[i] - 32);
                    } else {
                        sNewFile[iIndexNew++] = szTmp[i];
                    }
                }


                iIndexOld++;
//                reset buffer
                memset(szTmp, 0, BUFFER);
                continue;
            }
        }

//        loop for checking variable list
        for (int i = 0; i < sizeof(apszVariables); ++i) {
            if (apszVariables[i] == NULL) {
                break;
            }

//            if match fill in the new name to the new string
            if (cTmp == (apszVariables[i][0])) {
                if (memcmp(&sFile[iIndexOld], &apszVariables[i][0], strlen(apszVariables[i])) == 0) {
                    sNewFile[iIndexNew++] = 'u';
                    sNewFile[iIndexNew++] = 'i';
                    iIndexOld += (int) strlen(apszVariables[i]);
                    for (int j = 0; j < strlen(apszVariables[i]); ++j) {
                        if (j == 0) {
                            sNewFile[iIndexNew++] = (apszVariables[i][j] - 32);
                        } else {
                            sNewFile[iIndexNew++] = apszVariables[i][j];
                        }
                    }
                }
                continue;
            }
        }

        sNewFile[iIndexNew] = sFile[iIndexOld];
        iIndexNew++;
        iIndexOld++;
    }


    for (int i = 0; i < iIndexVariables; ++i) {
        free(apszVariables[i]);
    }
    free(szTmp);
}

void loopBeautify(char *sFile, char *sNewFile) {
    char *szWhile = "while (";
    char *szFor = "for (";
    char *szTmp = (char *) calloc(1, BUFFER);
    char cTmp = 0;
    int iIndexOld = 0;
    int iIndexNew = 0;
    int iIndexVarStart = 0;
    int iIndexVarEnd = 0;
    char *szVariableLine = NULL;
    char *szVariable = NULL;

    while ((cTmp = sFile[iIndexOld]) != '\0') {
        if (cTmp == 'w') {
            if (memcmp(&sFile[iIndexOld], szWhile, strlen(szWhile)) == 0) {
//                get to index before \t from 'w' in while
                iIndexVarStart = iIndexNew - 2;
                iIndexVarEnd = iIndexNew - 2;
                iIndexNew--;
                iIndexOld--;

//                find where \t is and move one index forward
                for (; sNewFile[iIndexVarStart] != '\t' ; --iIndexVarStart);
                iIndexVarStart++;

//                copy that string to variable
                szVariableLine = (char *) calloc(1, iIndexVarEnd - iIndexVarStart + 1);
                memcpy(szVariableLine, &sNewFile[iIndexVarStart], iIndexVarEnd - iIndexVarStart);

//                find the name of the variable
                for (; sNewFile[iIndexVarEnd] != '=' ; --iIndexVarEnd);
                iIndexVarEnd--;

//                copy variable name to variable
                szVariable = (char *) calloc(1, iIndexVarEnd - iIndexVarStart + 1);
                memcpy(szVariable, &sNewFile[iIndexVarStart], iIndexVarEnd - iIndexVarStart);

                iIndexNew = iIndexVarStart;
//                copy for (
                for (int i = 0; i < strlen(szFor); ++i) {
                    sNewFile[iIndexNew] = szFor[i];
                    iIndexNew++;
                }

//                move forward while (
                for (int i = 0; i < strlen(szWhile); ++i) {
                    iIndexOld++;
                }

//                copy a = 0;
                for (int i = 0; i < strlen(szVariableLine); ++i) {
                    sNewFile[iIndexNew] = szVariableLine[i];
                    iIndexNew++;
                }
                sNewFile[iIndexNew++] = ' ';
                iIndexOld++;

//                copy a < b )
                while ((cTmp = sFile[iIndexOld]) != ')') {
                    sNewFile[iIndexNew] = sFile[iIndexOld];
                    iIndexNew++;
                    iIndexOld++;
                }

//                copy ; a++
                sNewFile[iIndexNew++] = ';';
                sNewFile[iIndexNew++] = ' ';
                for (int i = 0; i < strlen(szVariable); ++i) {
                    sNewFile[iIndexNew++] = szVariable[i];
                }
                sNewFile[iIndexNew++] = '+';
                sNewFile[iIndexNew++] = '+';

                while (1) {
                    cTmp = sFile[iIndexOld];
                    if (cTmp == szVariable[0]) {
                        if (memcmp(&sFile[iIndexOld], szVariable, strlen(szVariable)) == 0) {
                            iIndexOld += (int) strlen(szVariable) + 3; // 3 plus comes from ++; after variable
                            break;
                        }
                    }
                    sNewFile[iIndexNew] = sFile[iIndexOld];
                    iIndexNew++;
                    iIndexOld++;
                }

                free(szVariableLine);
                free(szVariable);
                continue;
            }
        }

        sNewFile[iIndexNew] = sFile[iIndexOld];
        iIndexNew++;
        iIndexOld++;
    }

    free(szTmp);
}