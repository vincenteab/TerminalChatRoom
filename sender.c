#include "sender.h"

// Thread for the sender
static pthread_t sender_thread;

// global variables that point to conditionals and mutexes
static pthread_cond_t *endMain;
static pthread_cond_t *inputAndSend;
static pthread_mutex_t *inputAndSendMutex;

// global variable to check if the connection has been terminated
static bool connectionTerminated = false;

// global variable that points to the input list
static List *inputList;

void sender_function(void *args)
{
    // Unpack the arguments from the struct
    char *remoteName = ((senderArgs *)args)->remoteName;
    char *remotePort = ((senderArgs *)args)->remotePort;

    // Create a pointer that points to the message trimmed from the input list
    char *msg;
    // Create a buffer to send the message
    char buffer[MAXLEN] = "";
    // While the connection has not been terminated and the message being sent is not "!\n"
    while (!connectionTerminated && (strcmp(buffer, "!\n") != 0))
    {
        // Acquire the mutex and wait for the input list to have a message
        pthread_mutex_lock(inputAndSendMutex);
        while (List_count(inputList) == 0)
        {
            pthread_cond_wait(inputAndSend, inputAndSendMutex);
        }

        // Critical Section
        // Get the message from the list
        msg = List_trim(inputList);

        // Wake up the input thread
        pthread_cond_signal(inputAndSend);
        pthread_mutex_unlock(inputAndSendMutex);

        // If the message is not NULL, then send the message
        if (msg != NULL)
        {
            strncpy(buffer, msg, strlen(msg) + 1);
            free(msg);
            socketHandler_send(remoteName, remotePort, buffer);
        }
        else
        {
            memset(buffer, 0, MAXLEN);
        }
    }

    // Signal the main thread that the sender thread has ended
    pthread_cond_signal(endMain);
}

void sender_start(pthread_cond_t *pEndMain, List *pInputList, pthread_cond_t *pInputAndSend, pthread_mutex_t *pInputAndSendMutex, senderArgs *argsS)
{
    // Set the global pointers
    endMain = pEndMain;
    inputAndSend = pInputAndSend;
    inputAndSendMutex = pInputAndSendMutex;
    inputList = pInputList;

    pthread_create(&sender_thread, NULL, (void *)sender_function, argsS);
}

void sender_stop()
{
    connectionTerminated = true;
    pthread_cancel(sender_thread);
    pthread_join(sender_thread, NULL);
};