#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include "list.h"
#include "input.h"
#include "sender.h"
#include "receiver.h"
#include "output.h"

// Condition variable and mutex for the main thread
// If the sender sends a "!\n" then the program will shutdown all threads
// If the receiver receives a "!\n" then the program will shutdown all threads
pthread_cond_t endMain = PTHREAD_COND_INITIALIZER;
pthread_mutex_t endMainMutex = PTHREAD_MUTEX_INITIALIZER;

// Conditional variable and mutex for the input and send threads
pthread_cond_t inputAndSend = PTHREAD_COND_INITIALIZER;
pthread_mutex_t inputAndSendMutex = PTHREAD_MUTEX_INITIALIZER;

// Conditional variable and mutex for the receive and output threads
pthread_cond_t receiveAndOutput = PTHREAD_COND_INITIALIZER;
pthread_mutex_t receiveAndOutputMutex = PTHREAD_MUTEX_INITIALIZER;

// Function for List_free to free a node
void freeNode(void *item)
{
    if (item != NULL)
    {
        free(item);
    }
}

// Usage: s-talk localport remoteIP remoteport
int main(int argc, char **argv)
{
    char *localPort = argv[1];
    char *remoteName = argv[2];
    char *remotePort = argv[3];

    if (localPort == NULL || remoteName == NULL || remotePort == NULL)
    {
        printf("Usage: s-talk localport remoteIP remoteport\n");
        return 0;
    }

    List *inputList = List_create();
    List *outputList = List_create();
    assert(inputList != NULL);
    assert(outputList != NULL);
    // Create a struct to hold the arguments for the receiver thread
    receiverArgs *rArgs = malloc(sizeof(receiverArgs));
    assert(rArgs != NULL);
    rArgs->localport = malloc(strlen(localPort) + 1);
    // Pack the arguments into the struct
    assert(rArgs->localport != NULL);
    strncpy(rArgs->localport, localPort, strlen(localPort) + 1);

    // Create a struct to hold the arguments for the sender thread
    senderArgs *sArgs = malloc(sizeof(senderArgs));
    assert(sArgs != NULL);
    // Pack the arguments into the struct
    sArgs->remoteName = malloc(strlen(remoteName) + 1);
    assert(sArgs->remoteName != NULL);
    sArgs->remotePort = malloc(strlen(remotePort) + 1);
    assert(sArgs->remotePort != NULL);
    strncpy(sArgs->remoteName, remoteName, strlen(remoteName) + 1);
    strncpy(sArgs->remotePort, remotePort, strlen(remotePort) + 1);

    printf("Starting s-talk with host: %s on port: %s\n", remoteName, remotePort);

    // Starts the threads
    input_start(inputList, &inputAndSend, &inputAndSendMutex);
    sender_start(&endMain, inputList, &inputAndSend, &inputAndSendMutex, sArgs);
    receiver_start(&endMain, outputList, &receiveAndOutput, &receiveAndOutputMutex, rArgs);
    output_start(outputList, &receiveAndOutput, &receiveAndOutputMutex);

    // Waits for the user to receive or send a "!\n" to end the program
    pthread_mutex_lock(&endMainMutex);
    pthread_cond_wait(&endMain, &endMainMutex);
    pthread_mutex_unlock(&endMainMutex);

    // Stops the threads
    input_stop();
    sender_stop();
    receiver_stop();
    output_stop();

    // Destroy the condition variables and mutexes, free the lists
    pthread_mutex_destroy(&endMainMutex);
    pthread_cond_destroy(&endMain);
    pthread_cond_destroy(&inputAndSend);
    pthread_mutex_destroy(&inputAndSendMutex);
    pthread_cond_destroy(&receiveAndOutput);
    pthread_mutex_destroy(&receiveAndOutputMutex);
    List_free(inputList, freeNode);
    List_free(outputList, freeNode);
    free(rArgs->localport);
    free(rArgs);
    free(sArgs->remoteName);
    free(sArgs->remotePort);
    free(sArgs);

    printf("Connection has terminated\n");
    return 0;
}