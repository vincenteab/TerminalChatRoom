#include "socketHandler.h"

static char messageRx[MAXLEN];

int socketHandler_init(int localport)
{
    // Creating local socket address and binding it to the socket
    struct sockaddr_in localsin;
    memset(&localsin, 0, sizeof(localsin));
    localsin.sin_family = AF_INET;
    localsin.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to localhost
    localsin.sin_port = htons(localport);

    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketDescriptor == -1)
    {
        perror("Failed to create socket");
        return -1; // Return an error code
    }

    if (bind(socketDescriptor, (struct sockaddr *)&localsin, sizeof(localsin)) == -1)
    {
        perror("Failed to bind socket");
        close(socketDescriptor);
        return -1; // Return an error code
    }

    return socketDescriptor;
};

char *socketHandler_receive(char *localport)
{
    // Turn local port into an integer for use in socketHandler_init
    int port = atoi(localport);

    // Initialize a socket using socketHandler_init
    int socketDescriptor = socketHandler_init(port);
    if (socketDescriptor < 0)
    {
        perror("Failed to initialize socket");
        return NULL; // Handle socket initialization failure
    }

    // Create place to store remote socket information
    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);

    // Create a buffer to store the received data
    // char *messageRx = malloc(MAXLEN * sizeof(char));
    // if (messageRx == NULL)
    // {
    //     perror("Failed to allocate memory");
    //     return NULL; // Handle allocation failure
    // }

    // Receive data from the socket
    int bytesRx = recvfrom(socketDescriptor, messageRx, MAXLEN, 0, (struct sockaddr *)&sinRemote, &sin_len);

    // If the number of bytes received is less than 0, set messageRx to NULL
    if (bytesRx < 0)
    {
        memset(messageRx, 0, MAXLEN);
    }
    else
    {
        int terminatedIdx = bytesRx < MAXLEN ? bytesRx : MAXLEN - 1;
        messageRx[terminatedIdx] = '\0';
    }

    // Close the socket
    close(socketDescriptor);
    return messageRx;
};

void socketHandler_send(char *remoteName, char *remotePort, char *message)
{
    // Set up of structs for use in getaddrinfo
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // Use AF_UNSPEC for IPv4 or IPv6, AF_INET for IPv4
    hints.ai_socktype = SOCK_DGRAM;

    // Get the address information for the remote host
    int status = getaddrinfo(remoteName, remotePort, &hints, &servinfo);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return;
    }

    // Create a socket for remote host
    int socketDescriptor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (socketDescriptor == -1)
    {
        perror("socket error");
        freeaddrinfo(servinfo);
        return;
    }

    // Send the message to the remote host using the socket just created
    ssize_t len = sendto(socketDescriptor, message, strlen(message), 0, servinfo->ai_addr, servinfo->ai_addrlen);
    if (len == -1)
    {
        perror("sendto error");
    }

    // Clean up the socket and address information for the remote host
    freeaddrinfo(servinfo);
    close(socketDescriptor);
};