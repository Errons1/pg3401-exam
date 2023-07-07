#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "../lib/error/error.h"
#include "../lib/input/input.h"
#include "eksamen_v23_oppgave5_exec.h"
#include "protocol.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/* Main function for starting server, waits for a client to connect. Makes two sockets, one for terminating and
 * second for communicating. Starts two threads, one for each socket.
 * para1        array pointers string zero-terminated with terminal arguments from startup
 * return       int value 0 if closing successfully or anything else as bad shutdown */
int server_runningServer(char *apszArgV[]);

/* Tells about what port to make a socket to listen for request
 * return       int value SocketFd for listening for coming connections */
int server_BindToPort(Address *address);

/* Listens for connections and return new SocketFd after client connects
 * para1        int value of SockedFd for listening
 * return       int value of SockedFd for communication */
int server_Listen(int iListenSockFd);

/* Thread function for listening for termination message
 * para1        pointer ConnectionData struct */
void *server_SocketTerminate(void *arg);

/* Thread function for listening for communication messages
 * will execute terminal commando and return result
 * para1        pointer ConnectionData struct */
void *server_SocketMessage(void *arg);

/* Sends message to connected client
 * para1        pointer ConnectionData struct
 * para2        pointer Message struct*/
void server_SendMessage(ConnectionData *pConnectionData, Message *pMessage);

/* Receive message from connected client
 * para1        pointer ConnectionData struct
 * return       pointer Message struct */
Message *server_ReceiveMessage(ConnectionData *pConnectionData);

/* Terminates the application if called but need one call beforehand for priming static variables!
 * para1        pointer ConnectionData struct
 * para2        int value of running status of the application */
void server_Termination(ConnectionData *pConnectionData, int iStatus);

/* Signal handler for SIGINT and will activate terminating of server in a clean manner
 * para1        int value of SIGINT */
void server_sigintHandler(int iSigNum);

#endif //SERVER_SERVER_H