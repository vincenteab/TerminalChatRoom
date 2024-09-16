#ifndef _SENDER_H_
#define _SENDER_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "socketHandler.h"
#include "list.h"

// Struct for the sender arguments
struct senderArgs
{
    char *remoteName;
    char *remotePort;
} typedef senderArgs;

// Sender thread
void sender_function(void *args);

// Starts the sender thread
void sender_start(pthread_cond_t *pEndMain, List *pInputList, pthread_cond_t *pInputAndSend, pthread_mutex_t *pInputAndSendMutex, senderArgs *argsS);

// Stops the sender thread
void sender_stop();

#endif