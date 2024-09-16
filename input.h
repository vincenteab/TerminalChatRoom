#ifndef _INPUT_H
#define _INPUT_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "socketHandler.h"
#include "list.h"

// Input thread
void input_function();

// Starts the input thread
void input_start(List *pInputList, pthread_cond_t *pInputAndSend, pthread_mutex_t *pInputAndSendMutex);

// Stops the input thread
void input_stop();

#endif