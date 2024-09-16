#include "receiver.h"

// Receiver thread
static pthread_t receiver_thread;

// global variables that point to conditionals and mutexes
static pthread_cond_t *endMain;
static pthread_cond_t *receiveAndOutput;
static pthread_mutex_t *receiveAndOutputMutex;

// global variable to check if the connection has been terminated
static bool connectionTerminated = false;

// global variable that points to the output list
static List *outputList;

void receiver_function(void *args)
{
    // Unpack the arguments from the struct
    char *localport = ((receiverArgs *)args)->localport;

    // Create a buffer to receive the message
    char *buffer = "";
    // Create a pointer to copy the message from the buffer to append to the output list
    char *msg;
    // Create a variable to store the length of the message
    int lenOfMsg = 0;
    // While the connection has not been terminated and the message being received is not "!\n"
    while (!connectionTerminated && (strcmp(buffer, "!\n") != 0))
    {
        // Receive the message in buffer
        buffer = socketHandler_receive(localport);
        assert(buffer != NULL);

        // Copy the message from the buffer to msg
        lenOfMsg = strlen(buffer) + 1;
        msg = malloc(lenOfMsg * sizeof(char));
        strncpy(msg, buffer, lenOfMsg);

        // Acquire the mutex and wait for the output list to have space
        pthread_mutex_lock(receiveAndOutputMutex);
        while (List_count(outputList) == 100)
        {
            pthread_cond_wait(receiveAndOutput, receiveAndOutputMutex);
        }

        // Critical Section
        // Append the message to the output list
        List_append(outputList, msg);

        // Wake up the output thread
        pthread_cond_signal(receiveAndOutput);
        pthread_mutex_unlock(receiveAndOutputMutex);
    }

    // Signal the main thread that the connection has been terminated
    pthread_cond_signal(endMain);
}

void receiver_start(pthread_cond_t *pEndMain, List *pOutputList, pthread_cond_t *pReceiveAndOutput, pthread_mutex_t *pReceiveAndOutputMutex, receiverArgs *argsR)
{
    // Set the global pointers
    endMain = pEndMain;
    outputList = pOutputList;
    receiveAndOutput = pReceiveAndOutput;
    receiveAndOutputMutex = pReceiveAndOutputMutex;

    pthread_create(&receiver_thread, NULL, (void *)receiver_function, argsR);
};

void receiver_stop()
{
    connectionTerminated = true;
    pthread_cancel(receiver_thread);
    pthread_join(receiver_thread, NULL);
};