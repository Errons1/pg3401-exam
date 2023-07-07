#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "../lib/error/error.h"
#include "../lib/input/input.h"
#include "eksamen_v23_oppgave5_exec.h"
#include "protocol.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

/* Main function for starting client, tries to connect to server, shut down if none is available.
 * Makes two sockets, one for terminating and second for communicating. Starts two threads, one for each socket.
 * para1        array pointers string zero-terminated with terminal arguments from startup
 * return       int value 0 if closing successfully or anything else as bad shutdown */
int client_runningClient(char *apszArgV[]);

/* Tries to connect to server with given ipv4 and port
 * para1        pointer Address struct
 * return       int value SockedFd for connection to server */
int client_MakeConnection(Address *address);

/* Thread function for listening for termination message
 * para1        pointer ConnectionData struct */
void *client_SocketTerminate(void *arg);

/* Thread function for listening for communication messages
 * will execute terminal commando and return result
 * para1        pointer ConnectionData struct */
void *client_SocketMessage(void *arg);

/* Sends message to connected server
 * para1        pointer ConnectionData struct
 * para2        pointer Message struct*/
void client_SendMessage(ConnectionData *pConnectionData, Message *pMessage);

/* Receive message from connected server
 * para1        pointer ConnectionData struct
 * return       pointer Message struct */
Message *client_ReceiveMessage(ConnectionData *pConnectionData);

/* Terminates the application if called but need one call beforehand for priming static variables!
 * para1        pointer ConnectionData struct
 * para2        int value of running status of the application */
void client_Termination(ConnectionData *pConnectionData, int iStatus);

/* Signal handler for SIGINT and will activate terminating of server in a clean manner
 * para1        int value of SIGINT */
void client_sigintHandler(int iSigNum);

#endif //CLIENT_CLIENT_H
