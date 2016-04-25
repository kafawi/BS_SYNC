#ifndef FIFO_H_
#define FIFO_H_

#include <semaphore.h>
#include <pthread.h>


struct typedef {
   char *element;
   int size;
   int last;
   int first;
   pthread_mutex_t mutex;
   sem_t taken;
   sem_t free;
} fifo_t;

//typedef struct buffer  fifo_t;

void init(fifo_t *buffer, int size);

void push(fifo_t *buffer, char data);

char pop(fifo_t *buffer);

void destroy(fifo_t *buffer);

#endif /* FIFO_H_ */
