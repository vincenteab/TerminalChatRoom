#include "output.h"

// Thread for the output
static pthread_t output_thread;

// global variables that point to conditionals and mutexes
static pthread_cond_t *receiveAndOutput;
static pthread_mutex_t *receiveAndOutputMutex;

// global variable to check if the connection has been terminated
static bool connectionTerminated = false;

// global variable that points to the output list
static List *outputList;

void output_function()
{
    // Create a pointer that points to the message trimmed from the output list
    char *msg = NULL;
    // Create a buffer to output the message
    char buffer[MAXLEN] = "";
    // While the connection has not been terminated and the message being output is not "!\n"
    while (!connectionTerminated && (strcmp(buffer, "!\n") != 0))
    {
        // Acquire the mutex and wait for the output list to have a message
        pthread_mutex_lock(receiveAndOutputMutex);
        while (List_count(outputList) == 0)
        {
            pthread_cond_wait(receiveAndOutput, receiveAndOutputMutex);
        }

        // Critical Section
        // Get the message from the list
        msg = List_trim(outputList);

        // Wake up the receive thread
        pthread_cond_signal(receiveAndOutput);
        pthread_mutex_unlock(receiveAndOutputMutex);

        // if msg is over MAXLEN then it will be truncated
        if (strlen(msg) > MAXLEN)
        {
            msg[MAXLEN - 1] = '\0';
        }

        // Copy the message from msg to the buffer
        strncpy(buffer, msg, strlen(msg) + 1);

        // Free the memory allocated for the message
        free(msg);

        // Output the message
        fputs("Message from sender: ", stdout);
        fputs(buffer, stdout);
    }
}

void output_start(List *pOutputList, pthread_cond_t *pReceiveAndOutput, pthread_mutex_t *pReceiveAndOutputMutex)
{
    // Set the global pointers
    outputList = pOutputList;
    receiveAndOutput = pReceiveAndOutput;
    receiveAndOutputMutex = pReceiveAndOutputMutex;

    pthread_create(&output_thread, NULL, (void *)output_function, NULL);
}

void output_stop()
{
    connectionTerminated = true;
    pthread_cancel(output_thread);
    pthread_join(output_thread, NULL);
}