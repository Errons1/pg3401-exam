#include "main.h"

int main(int iArgC, char *apszArgV[]) {
    int iReturnCode = 0;

//    -client -ipv4 127.0.0.1 -port 8080
    if (iArgC != 6 && iArgC != 4) {
        printf("Socket shell needs arguments to run properly\n");
        printf("Client arguments: socketshell -client -ipv4 <IPv4 number> -port <port number>\n");
        printf("Server arguments: socketshell -server -port <port number>\n");
        exit(1);
    }

//    client check
    if (strncmp("-client", apszArgV[1], strlen("-client")) == 0) {
        iReturnCode = client_runningClient(apszArgV);

//        server check
    } else if (strncmp("-server", apszArgV[1], strlen("-server")) == 0) {
        iReturnCode = server_runningServer(apszArgV);

//        misspelled argument
    } else {
        printf("Could not recognize argument %s\n", apszArgV[1]);
        exit(1);
    }

    return iReturnCode;
}