#ifndef FIFO_H_
#define FIFO_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errorno.h>


struct typedef {
   char *element;
   int size;
   int last;
   int first;
   pthread_mutex_t mutex;
   sem_t taken;
   sem_t free;
} FifoT;

//typedef struct buffer  FifoT;

void initFifo(FifoT *buffer, int size);

void push(FifoT *buffer, char data);

char pop(FifoT *buffer);

void destroy(FifoT *buffer);

#endif /* FIFO_H_ */