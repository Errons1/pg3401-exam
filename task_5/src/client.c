#include "client.h"

int client_runningClient(char *apszArgV[]) {
    Address *pAddress = NULL;
    char *szIPv4 = NULL;
    int iPort = 0;
    int iArgIndex = 2;
    int iSockFdTerminate = 0;
    int iSockFdMessage = 0;
    pthread_t threadSend;
    pthread_t threadReceive;
    ConnectionData *pConnectionData = NULL;

//    ipv4 check
    if (strncmp("-ipv4", apszArgV[iArgIndex], strlen("-ipv4")) != 0) {
        printf("Could not recognize argument %s\n", apszArgV[iArgIndex]);
        return 1;
    }
    iArgIndex++;
    szIPv4 = apszArgV[iArgIndex];
    iArgIndex++;

//    port check
    if (strncmp("-port", apszArgV[iArgIndex], strlen("-port")) != 0) {
        printf("Could not recognize argument %s\n", apszArgV[iArgIndex]);
        return 1;
    }
    iArgIndex++;
    iPort = (int) strtol(apszArgV[iArgIndex], NULL, 10);

//    connect to server for termination socket
    printf("Client starting up and connection to server\n");
    pAddress = createAddress(szIPv4, iPort);
    iSockFdTerminate = client_MakeConnection(pAddress);
    error_exit("Failed to connect to server", iSockFdTerminate);

    iSockFdMessage = client_MakeConnection(pAddress);
    error_exit("Failed to connect to server", iSockFdMessage);

    pConnectionData = creatConnectionData(RUNNING, iSockFdTerminate, iSockFdMessage);
    client_Termination(pConnectionData, RUNNING);
    printf("Client connected to server\n");
    signal(SIGINT, client_sigintHandler);


    if (pthread_create(&threadSend, NULL, client_SocketTerminate, (void *) pConnectionData) != 0) {
        perror("Could not create threadSend");
        exit(1);
    }

    if (pthread_create(&threadReceive, NULL, client_SocketMessage, (void *) pConnectionData) != 0) {
        perror("Could not create threadReceive");
        exit(1);
    }

    if (pthread_join(threadSend, NULL) != 0) {
        perror("Could not join threadSend");
        exit(1);
    }

    if (pthread_join(threadReceive, NULL) != 0) {
        perror("Could not join threadReceive");
        exit(1);
    }


    freeAddress(pAddress);
    freeConnectionData(pConnectionData);
    printf("Client shutting down\n");
    return 0;
}

void *client_SocketTerminate(void *arg) {
    ConnectionData *pConnectionData = (ConnectionData *) arg;
    Message *pMessage = NULL;
    char sBuffer[MAX_BUFFER_SIZE] = {0};

    while (pConnectionData->iStatus == RUNNING) {
        recv(pConnectionData->iSockFdTerminate, sBuffer, MAX_BUFFER_SIZE, MSG_DONTWAIT);

        if (strncmp(sBuffer, TERMINATING, strlen(TERMINATING)) == 0) {
            pMessage = createMessage("", TERMINATING);
            send(pConnectionData->iSockFdTerminate, pMessage->szMessageType, pMessage->iSizeMessageType, 0);
            pConnectionData->iStatus = CLOSING;
            freeMessage(pMessage);
        }
    }

    pthread_exit(NULL);
}

void *client_SocketMessage(void *arg) {
    ConnectionData *pConnectionData = (ConnectionData *) arg;
    Message *pMessage = NULL;
    char szInput[MAX_BUFFER_SIZE] = {0};
    char cInput = 0;
    int iInputIndex = 0;


    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    while (pConnectionData->iStatus == RUNNING) {
        cInput = (char) getchar();

        if (cInput == -1) {
            continue;
        }

        if (cInput == '\n' && iInputIndex < MAX_BUFFER_SIZE) {
//            send message
            pMessage = createMessage(szInput, CLIENT_TO_SERVER);
            client_SendMessage(pConnectionData, pMessage);
            freeMessage(pMessage);

//            receive message
            pMessage = client_ReceiveMessage(pConnectionData);
            printf("Client: %s\n", szInput);
            printf("Server: %s\n", pMessage->szBuffer);
            printf("\n");

//            free pointers and reset buffer
            memset(szInput, 0, sizeof(szInput));
            getchar();
            iInputIndex = 0;
            freeMessage(pMessage);
            continue;
        }

        szInput[iInputIndex] = cInput;
        iInputIndex++;
    }

    pthread_exit(NULL);
}

void client_SendMessage(ConnectionData *pConnectionData, Message *pMessage) {
    char sSizeBuffer[SEND_SIZE_OF_BUFFER] = {0};
    sprintf(sSizeBuffer, "%d", pMessage->iSizeBuffer);

//    sends type of message
    send(pConnectionData->iSockFdMessage, pMessage->szMessageType, pMessage->iSizeMessageType, 0);
//    sends size of message
    send(pConnectionData->iSockFdMessage, sSizeBuffer, SEND_SIZE_OF_BUFFER, 0);
//    sends message
    send(pConnectionData->iSockFdMessage, pMessage->szBuffer, pMessage->iSizeBuffer, 0);

}

Message *client_ReceiveMessage(ConnectionData *pConnectionData) {
    Message *pMessage = NULL;
    char sSizeBuffer[SEND_SIZE_OF_BUFFER] = {0};
    int iSizeBuffer = 0;
    char *sBuffer = NULL;

//    receive size
    recv(pConnectionData->iSockFdMessage, sSizeBuffer, SEND_SIZE_OF_BUFFER, 0);

    iSizeBuffer = (int) strtol(sSizeBuffer, NULL, 10);
    sBuffer = calloc(1, iSizeBuffer);

//    receive message
    recv(pConnectionData->iSockFdMessage, sBuffer, iSizeBuffer, 0);

    pMessage = createMessage(sBuffer, CLIENT_TO_SERVER);
    free(sBuffer);
    return pMessage;
}

int client_MakeConnection(Address *address) {
    struct sockaddr_in sockAddr = {0};
    int iSockFd = 0;
    int iCodeConnection = 0;

//    AF_INET == IPv4 protocol
//    SOCK_STREAM == Provides sequenced, reliable, two-way, connection-based byte streams.
//    0 == run default protocol aka TCP
    iSockFd = socket(AF_INET, SOCK_STREAM, 0);
    error_exit("Failed to make socket", iSockFd);

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(address->iPort);
    sockAddr.sin_addr.s_addr = inet_addr(address->szIpv4); // IPv4 from outside

//    Size of struct to see difference between IPv4 / IPv6
    iCodeConnection = connect(iSockFd, (struct sockaddr *) &sockAddr, sizeof(sockAddr));
    if (iCodeConnection < 0) {
        return -1;
    }

    return iSockFd;
}

void client_Termination(ConnectionData *pConnectionData, int iStatus) {
    static ConnectionData *stpConnectionData = NULL;
    Message *pMessage = NULL;

    if (pConnectionData != NULL && iStatus == RUNNING) {
        stpConnectionData = pConnectionData;
        return;

    } else if (pConnectionData == NULL && iStatus == CLOSING) {
        pMessage = createMessage("", TERMINATING);
        send(stpConnectionData->iSockFdTerminate, pMessage->szMessageType, pMessage->iSizeMessageType, 0);
        stpConnectionData->iStatus = CLOSING;
        freeMessage(pMessage);
    }
}

void client_sigintHandler(int iSigNum) {
    signal(SIGINT, client_sigintHandler);
    client_Termination(NULL, CLOSING);
    printf("\n");
    fflush(stdout);
}
