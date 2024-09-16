#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "socketHandler.h"
#include "list.h"

// Output thread
void output_function();

// Starts the output thread
void output_start(List *pOutputList, pthread_cond_t *pReceiveAndOutput, pthread_mutex_t *pReceiveAndOutputMutex);

// Stops the output thread
void output_stop();

#endif