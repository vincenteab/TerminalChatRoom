#include "input.h"

// Thread for the input
static pthread_t input_thread;

// global variables that point to conditionals and mutexes
static pthread_cond_t *inputAndSend;
static pthread_mutex_t *inputAndSendMutex;

// global variable to check if the connection has been terminated
static bool connectionTerminated = false;

// global variable that points to the input list
static List *inputList;

void input_function()
{
    // Create a buffer to store the message typed by the user
    char buffer[MAXLEN] = "";
    // Create a pointer to copy the message from the buffer to append to the input list
    char *msg;
    // Create a variable to store the length of the message
    int lenOfMsg = 0;
    // While the connection has not been terminated and the message being typed is not "!\n"
    while (!connectionTerminated && (strcmp(buffer, "!\n") != 0))
    {
        // Clear the bugger before user inputs a message
        memset(buffer, 0, MAXLEN);

        // Get input from user
        fgets(buffer, MAXLEN, stdin);

        // Allocate memory for a string so that it can be appended to the list
        lenOfMsg = strlen(buffer) + 1;
        msg = malloc(lenOfMsg * sizeof(char));
        assert(msg != NULL);

        // Copy the buffer into the dynamically allocated string
        strncpy(msg, buffer, lenOfMsg);

        // Acquire the mutex and wait for input list to have space
        pthread_mutex_lock(inputAndSendMutex);
        while (List_count(inputList) == 100)
        {
            pthread_cond_wait(inputAndSend, inputAndSendMutex);
        }

        // Critical Section
        // Append the message to the input list
        List_append(inputList, msg);

        // Wake up the sender thread
        pthread_cond_signal(inputAndSend);
        pthread_mutex_unlock(inputAndSendMutex);
    }
}

void input_start(List *pInputList, pthread_cond_t *pInputAndSend, pthread_mutex_t *pInputAndSendMutex)
{
    // Set the global pointers
    inputList = pInputList;
    inputAndSend = pInputAndSend;
    inputAndSendMutex = pInputAndSendMutex;

    pthread_create(&input_thread, NULL, (void *)input_function, NULL);
}

void input_stop()
{
    connectionTerminated = true;
    pthread_cancel(input_thread);
    pthread_join(input_thread, NULL);
}