#include "protocol.h"

Address *createAddress(char *szIPv4, int iPort) {
    Address *pAddress = (Address *) malloc(sizeof(Address));
    pAddress->szIpv4 = (char *) calloc(1, strlen(szIPv4) + 1);
    strcpy(pAddress->szIpv4, szIPv4);
    pAddress->iPort = iPort;

    return pAddress;
}

void freeAddress(Address *address) {
    free(address->szIpv4);
    free(address);
}

Message *createMessage(char *szMessage, char *szMessageType) {
    Message *pMessage = (Message *) calloc(1, sizeof(Message));

    pMessage->szMessageType = (char *) calloc(1, strlen(szMessageType) + 1);
    strcpy(pMessage->szMessageType, szMessageType);
    pMessage->iSizeMessageType = (int) strlen(szMessageType) + 1;

    pMessage->szBuffer = (char *) calloc(1, strlen(szMessage) + 1);
    strcpy(pMessage->szBuffer, szMessage);
    pMessage->iSizeBuffer = (int) strlen(szMessage) + 1;

    return pMessage;
}

void freeMessage(Message *pMessage) {
    free(pMessage->szMessageType);
    free(pMessage->szBuffer);
    free(pMessage);
}

ConnectionData *creatConnectionData(int iStatus, int iSockFdTerminate, int iSockFdMessage) {
    ConnectionData *pConnectionData = (ConnectionData *) calloc(1, sizeof(ConnectionData));
    pConnectionData->iStatus = iStatus;
    pConnectionData->iSockFdTerminate = iSockFdTerminate;
    pConnectionData->iSockFdMessage = iSockFdMessage;

    return pConnectionData;
}

void freeConnectionData(ConnectionData *pConnectionData) {
    free(pConnectionData);
}

