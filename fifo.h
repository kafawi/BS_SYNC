#ifndef FIFO_H_
#define FIFO_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "errInfo.h"

typedef struct {
   char *element;
   int size;
   int last;
   int first;
   pthread_mutex_t mutex;
   sem_t taken;
   sem_t empty;
} FifoT;

int initFifo(FifoT * buffer, int size);

void push(FifoT *buffer, char data);

char pop(FifoT *buffer);

void destroyFifo(FifoT *buffer);

#endif /* FIFO_H_ */
