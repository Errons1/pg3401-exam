#ifndef TASK_5_PROTOCOL_H
#define TASK_5_PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define MAX_BUFFER_SIZE 1024
#define SEND_SIZE_OF_BUFFER 10
#define CLIENT_TO_SERVER "100"
#define SERVER_TO_CLIENT "200"
#define TERMINATING "-100"

#define RUNNING 1
#define CLOSING 0

typedef struct Address {
    char *szIpv4;
    unsigned int iPort;
} Address;

typedef struct Message {
    int iSizeMessageType;
    char *szMessageType;
    int iSizeBuffer;
    char *szBuffer;
} Message;

typedef struct ConnectionData {
    int iStatus;
    int iSockFdTerminate;
    int iSockFdMessage;
} ConnectionData;

/* Creates Address struct in a safe manner
 * para1        string zero-terminated ipv4 eks: 127.0.0.1
 * para2        int value of port eks. 8080
 * return       pointer to struct Address */
Address * createAddress(char *szIPv4, int iPort);

/* Frees Address in a safe manner
 * para1        pointer to Address element */
void freeAddress(Address *address);

/* Creates Message struct in a safe manner
 * para1        string zero-terminated text to send
 * para2        string zero-terminated type of message*/
Message *createMessage(char *szMessage, char *szMessageType);

/* Frees Message elements in a safe manner
 * para1        pointer to Message element */
void freeMessage(Message *pMessage);

/* Creates ConnectionData struct in a safe manner
 * para1        int value of status RUNNING or CLOSING
 * para2        int value to SockFd for communication
 * para3        int value to SockFd for sending termination message
 * return       pointer to Connection struct */
ConnectionData *creatConnectionData(int iStatus, int iSockFdTerminate, int iSockFdMessage);

/* Frees ConnectionData struct in a safe manner
 * para1        pointer to ConnectionData struct*/
void freeConnectionData(ConnectionData *pConnectionData);

#endif //TASK_5_PROTOCOL_H