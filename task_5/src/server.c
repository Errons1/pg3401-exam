#include "server.h"

int server_runningServer(char *apszArgV[]) {
    Address *pAddress = NULL;
    char *szIPv4 = "";
    int iPort = 0;
    int iArgIndex = 2;
    int iListenSockFd = 0;
    int iSockFdTerminate = 0;
    int iSockFdMessage = 0;
    pthread_t threadSend;
    pthread_t threadReceive;
    ConnectionData *pConnectionData = NULL;

//    port check
    if (strncmp("-port", apszArgV[iArgIndex], strlen("-port")) != 0) {
        printf("Could not recognize argument %s\n", apszArgV[iArgIndex]);
        return 1;
    }
    iArgIndex++;
    iPort = (int) strtol(apszArgV[iArgIndex], NULL, 10);

//    Server starts
    pAddress = createAddress(szIPv4, iPort);
    iListenSockFd = server_BindToPort(pAddress);

    printf("Server started \n");
    printf("Listening for client...\n");

    iSockFdTerminate = server_Listen(iListenSockFd);
    error_exit("Failed to connect to receive socket", iSockFdTerminate);

    iSockFdMessage = server_Listen(iListenSockFd);
    error_exit("Failed to connect to send socket", iSockFdMessage);

    pConnectionData = creatConnectionData(RUNNING, iSockFdTerminate, iSockFdMessage);
    server_Termination(pConnectionData, RUNNING);
    printf("Client connected\n");
    signal(SIGINT, server_sigintHandler);

    if (pthread_create(&threadSend, NULL, server_SocketTerminate, (void *) pConnectionData) != 0) {
        perror("Could not create threadSend");
        exit(1);
    }

    if (pthread_create(&threadReceive, NULL, server_SocketMessage, (void *) pConnectionData) != 0) {
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

    printf("Server shutting down\n");
    freeAddress(pAddress);
    return 0;
}

int server_BindToPort(Address *address) {
    struct sockaddr_in sockaddrIn = {0};
    int iSockFd = 0;
    int iPort = (int) address->iPort;
    int iCodeBind = 0;
    int iCodeListen = 0;

//    AF_INET == IPv4 protocol
//    SOCK_STREAM == Provides sequenced, reliable, two-way, connection-based byte streams.
//    0 == run default protocol aka TCP
    iSockFd = socket(AF_INET, SOCK_STREAM, 0);
    error_exit("Failed to make socket", iSockFd);

    sockaddrIn.sin_family = AF_INET; // AF_INET == IPv4 protocol
    sockaddrIn.sin_port = htons(iPort); // port = from user
    sockaddrIn.sin_addr.s_addr = INADDR_ANY;

    iCodeBind = bind(iSockFd, (struct sockaddr *) &sockaddrIn, sizeof(sockaddrIn));
    error_exit("Failed to bind socket", iCodeBind);

    iCodeListen = listen(iSockFd, 5);
    error_exit("Failed to start Listening", iCodeListen);

    return iSockFd;
}

int server_Listen(int iListenSockFd) {
    return accept(iListenSockFd, (struct sockaddr *) NULL, NULL);
}

void *server_SocketTerminate(void *arg) {
    ConnectionData *pConnectionData = (ConnectionData *) arg;
    Message *pMessage = NULL;
    char sBuffer[MAX_BUFFER_SIZE] = {0};

    while (pConnectionData->iStatus == RUNNING) {
        recv(pConnectionData->iSockFdTerminate, sBuffer, MAX_BUFFER_SIZE, MSG_DONTWAIT);

//        double checks if message is for termination
        if(strncmp(sBuffer, TERMINATING, strlen(TERMINATING)) == 0) {
            pMessage = createMessage("", TERMINATING);
            send(pConnectionData->iSockFdTerminate, pMessage->szMessageType, pMessage->iSizeMessageType, 0);
            pConnectionData->iStatus = CLOSING;
            freeMessage(pMessage);
        }
    }

    pthread_exit(NULL);
}

void *server_SocketMessage(void *arg) {
    ConnectionData *pConnectionData = (ConnectionData *) arg;
    Message *pMessage = NULL;
    char *szTerminalResponse = NULL;
    char sBuffer[MAX_BUFFER_SIZE] = {0};

    while (pConnectionData->iStatus == RUNNING) {
//        checking for message and is not a breaking point
        recv(pConnectionData->iSockFdMessage, sBuffer, sizeof(sBuffer), MSG_DONTWAIT);

//        check if message type and responds to it
        if (strncmp(sBuffer, CLIENT_TO_SERVER, strlen(CLIENT_TO_SERVER)) == 0) {
//            receive terminal commando and execute
            pMessage = server_ReceiveMessage(pConnectionData);
            szTerminalResponse = executeCommand(pMessage->szBuffer);
            printf("Client: %s\n", pMessage->szBuffer);
            printf("Server: %s\n", szTerminalResponse);
            printf("\n");
            freeMessage(pMessage);

//            sends result of terminal commando
            pMessage = createMessage(szTerminalResponse, SERVER_TO_CLIENT);
            server_SendMessage(pConnectionData, pMessage);

//            free and reset necessary memory
            memset(sBuffer, 0, sizeof(sBuffer));
            free(szTerminalResponse);
            freeMessage(pMessage);
        }
    }

    pthread_exit(NULL);
}

void server_SendMessage(ConnectionData *pConnectionData, Message *pMessage) {
    char sSizeBuffer[SEND_SIZE_OF_BUFFER] = {0};
    sprintf(sSizeBuffer, "%d", pMessage->iSizeBuffer);

//    sends size of message
    send(pConnectionData->iSockFdMessage, sSizeBuffer, SEND_SIZE_OF_BUFFER, 0);
//    sends message
    send(pConnectionData->iSockFdMessage, pMessage->szBuffer, pMessage->iSizeBuffer, 0);

}

Message *server_ReceiveMessage(ConnectionData *pConnectionData) {
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

void server_Termination(ConnectionData *pConnectionData, int iStatus) {
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

void server_sigintHandler(int iSigNum) {
    signal(SIGINT, server_sigintHandler);
    server_Termination(NULL, CLOSING);
    printf("\n");
    fflush(stdout);
}