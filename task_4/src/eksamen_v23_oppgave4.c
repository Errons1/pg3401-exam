#include "../lib/input/input.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define BUFFER_SIZE 4096
#define BYTE_RANGE 95
#define ASCII_PRINTABLE_START 32
#define ASCII_PRINTABLE_END 127
#define TRUE 1
#define FALSE 0

typedef struct ThreadArgs {
    char *sFile;
    int count[BYTE_RANGE];
    unsigned char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex;
    sem_t sem_full, sem_empty;
    int bytes_in_buffer;
} ThreadArgs;

void *readFile(void *arg) {
    ThreadArgs *pThreadArgs = (ThreadArgs *) arg;

//    open file from arguments
    FILE *fp = fopen(pThreadArgs->sFile, "rb");
    if (!fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    while (1) {
//        if there is bytes in the buffer wait
        while (pThreadArgs->bytes_in_buffer > 0) {
            sem_wait(&pThreadArgs->sem_empty);
        }
//        need to lock the shared memory
        pthread_mutex_lock(&pThreadArgs->mutex);


        int read_bytes = fread(pThreadArgs->buffer + pThreadArgs->bytes_in_buffer, 1,
                               BUFFER_SIZE - pThreadArgs->bytes_in_buffer, fp);
        pThreadArgs->bytes_in_buffer += read_bytes;

//        open shared memory before signaling next thread to run
        pthread_mutex_unlock(&pThreadArgs->mutex);
        sem_post(&pThreadArgs->sem_full);

//        if EOF is met, use -1 as signal the other thread to finish up and exit
        if (read_bytes == 0 && feof(fp)) {
            pThreadArgs->bytes_in_buffer = -1;
            break;
        }
    }

    fclose(fp);
    pthread_exit(NULL);
}

void *countBytes(void *arg) {
    ThreadArgs *pThreadArgs = (ThreadArgs *) arg;
    char *asWords[] = {"and", "at", "it", "my", "Hamlet"};
    char *sWord = "";
    char sTmpWord[100];
    int aiOccurrence[sizeof(asWords) / sizeof(asWords[0])] = {0};
    int iEndOfWord = 0;
    int iCharValue = 0;

    memset(pThreadArgs->count, 0, sizeof(pThreadArgs->count));

    while (1) {
//        if buffer is empty wait for it to be filled up or EOF is met (-1 value)
        while (pThreadArgs->bytes_in_buffer == 0) {
            sem_wait(&pThreadArgs->sem_full);
        }
//        lock shared memory before reading
        pthread_mutex_lock(&pThreadArgs->mutex);

//        -1 == EOF and break main loop
        if (pThreadArgs->bytes_in_buffer == -1) {
            break;
        }

//        Count printable chars from buffer
        for (int i = 0; i < pThreadArgs->bytes_in_buffer; i++) {
            iCharValue = pThreadArgs->buffer[i];

            if (iCharValue >=ASCII_PRINTABLE_START && iCharValue <= ASCII_PRINTABLE_END)
            pThreadArgs->count[iCharValue - ASCII_PRINTABLE_START]++;
        }

//        Count amount specific word from buffer
        for (int iWord = 0; iWord < (sizeof(asWords) / sizeof(asWords[0])); ++iWord) {
            sWord = asWords[iWord];

            for (int i = 0; i < pThreadArgs->bytes_in_buffer; ++i) {
//                Check if first char match
                if (pThreadArgs->buffer[i] != sWord[0]) {
                    continue;
                }

//                check if index is out if bound (underflow)
                if (i - 1 > 0) {
//                    Check if char before was A-Z or a-z
                    if ((pThreadArgs->buffer[i - 1] >= 'A' && pThreadArgs->buffer[i - 1] <= 'Z') ||
                        (pThreadArgs->buffer[i - 1] >= 'a' && pThreadArgs->buffer[i - 1] <= 'z')) {
                        continue;
                    }
                }

//                get index after the word to compare
                iEndOfWord = i + ((int) strlen(sWord));

//                Check if index is out of bound (overflow)
                if (iEndOfWord >= pThreadArgs->bytes_in_buffer) {
                    continue;
                }

//                Check if index after is A-Z or a-z
                if ((pThreadArgs->buffer[iEndOfWord] >= 'A' && pThreadArgs->buffer[iEndOfWord] <= 'Z') ||
                    (pThreadArgs->buffer[iEndOfWord] >= 'a' && pThreadArgs->buffer[iEndOfWord] <= 'z')) {
                    continue;
                }

//                copy word from text to tmp string
                memccpy(sTmpWord, &pThreadArgs->buffer[i], 0, (strlen(sWord)));

//                compare tmpWord against specific word
                if (strncmp(sTmpWord, sWord, strlen(sWord)) == 0) {
                    aiOccurrence[iWord]++;
                }
            }
        }

        pThreadArgs->bytes_in_buffer = 0;
//        open shared memory and then signal reader thread
        pthread_mutex_unlock(&pThreadArgs->mutex);
        sem_post(&pThreadArgs->sem_empty);
    }

//    print count of chars
    for (int i = ASCII_PRINTABLE_START; i < ASCII_PRINTABLE_END; i++) {
        printf("%c : %d\n", i, pThreadArgs->count[i - ASCII_PRINTABLE_START]);
    }

//    print count of words
    for (int iWord = 0; iWord < (sizeof(asWords) / sizeof(asWords[0])); ++iWord) {
        printf("%s : %d\n", asWords[iWord], aiOccurrence[iWord]);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    ThreadArgs *pThreadArgs = (ThreadArgs *) calloc(1, sizeof(ThreadArgs));
    pthread_t threadA, threadB;
    char sInput[2] = {0};
    int iInput = 0;
    int bMenuRunning = TRUE;

//    NULL == default mutex attributes
    pthread_mutex_init(&pThreadArgs->mutex, NULL);
//    pshared 0 == semaphore is hared between threads.
//    value 1 == the semaphore lets one wait pass before stopping
    sem_init(&pThreadArgs->sem_empty, 0, 1);
    sem_init(&pThreadArgs->sem_full, 0, 0);

//    Gives menu option if started with one argument
    if (argc > 1) {
        printf("What file do you want to analyze?\n");
        printf("1. Default file of \"../resources/eksamen_v23_oppgave4_pg2265.txt\"?\n");
        printf("2. Custom file of \"%s\"?\n", argv[1]);

        while (bMenuRunning) {
            iInput = input_int(sInput, sizeof(sInput));

            switch (iInput) {
                case 1: {
                    printf("Selected file \"../resources/eksamen_v23_oppgave4_pg2265.txt\"\n");
                    pThreadArgs->sFile = "../resources/eksamen_v23_oppgave4_pg2265.txt";
                    bMenuRunning = FALSE;
                    break;
                }
                case 2: {
                    printf("Selected file \"%s\"\n", argv[1]);
                    pThreadArgs->sFile = argv[1];
                    bMenuRunning = FALSE;
                    break;
                }
                default:
                    printf("Invalid input\n");
            }
        }

//        Default file is Hamlet from exam if no file is given
    } else {
        pThreadArgs->sFile = "../resources/eksamen_v23_oppgave4_pg2265.txt";
    }


    if (pthread_create(&threadA, NULL, readFile, (void *) pThreadArgs) != 0) {
        perror("Could not create thread A");
        exit(1);
    }

    if (pthread_create(&threadB, NULL, countBytes, (void *) pThreadArgs) != 0) {
        perror("Could not create thread B");
        exit(1);
    }

    if (pthread_join(threadA, NULL) != 0) {
        perror("Could not join thread A");
        exit(1);
    }
    if (pthread_join(threadB, NULL) != 0) {
        perror("Could not join thread B");
        exit(1);
    }

    free(pThreadArgs);

    return 0;
}










