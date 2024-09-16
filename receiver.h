#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "socketHandler.h"
#include "list.h"

// Struct for the receiver arguments
typedef struct receiverArgs
{
    char *localport;
} receiverArgs;

// Receiver thread
void receiver_function(void *args);

// Starts the receiver thread
void receiver_start(pthread_cond_t *pEndMain, List *pOutputList, pthread_cond_t *pReceiveAndOutput, pthread_mutex_t *pReceiveAndOutputMutex, receiverArgs *argsR);

// Stops the receiver thread
void receiver_stop();
#endif