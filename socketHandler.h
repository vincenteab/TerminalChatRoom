#ifndef _SOCKET_HANDLER_
#define _SOCKET_HANDLER_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLEN 512

// Initializes a socket and binds it to the local port
int socketHandler_init(int localport);

// Uses socketHandler_init to initialize a socket
// and then receives a message from the socket
char *socketHandler_receive(char *localport);

// Sends a message to a remote socket
void socketHandler_send(char *remoteName, char *remotePort, char *message);

#endif