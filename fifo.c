#include "fifo.h"
#include "errInfo.h"
#include <stdio.h>

int initFifo(FifoT * buf, int size){
   MERROUT(buf);
   int i=0;
   e = sem_init(&buf->taken,0, 0); ERROUT(e);
   e = sem_init(&buf->empty,0, size); ERROUT(e);
   e = pthread_mutex_init(&buf->mutex, NULL); ERROUT(e);
   buf->size=size;
   buf->first=0;
   buf->last=(size-1);
   buf->element =(char*)malloc( sizeof(*buf->element) *size);
   MERROUT(buf->element);
   for (i=0; i< size; i++){
      buf->element[i]='a';
   }
   return EXIT_SUCCESS;
}

void push( FifoT *buffer, char data){
   e = sem_wait(&buffer->empty); ERROUT(e);
   e = pthread_mutex_lock(&buffer->mutex); ERROUT(e);

   buffer->last = (buffer->last + 1) % buffer->size;
   buffer->element[buffer->last] = data;

   e = pthread_mutex_unlock(&buffer->mutex); ERROUT(e);
   e = sem_post(&buffer->taken); ERROUT(e);
}

char pop( FifoT *buffer){
   char element = 0;
   e = sem_wait(&buffer->taken); ERROUT(e);
   e = pthread_mutex_lock(&buffer->mutex); ERROUT(e);

   element = buffer->element[buffer->first];
   buffer->first = (buffer->first + 1) % buffer->size;

   e = pthread_mutex_unlock(&buffer->mutex); ERROUT(e);
   e = sem_post(&buffer->empty); ERROUT(e);

   return element;
}

void destroyFifo(FifoT *buffer){
   free(buffer->element);
   e = pthread_mutex_destroy(&buffer->mutex); ERROUT(e);
   e = sem_destroy(&buffer->empty); ERROUT(e);
   e = sem_destroy(&buffer->taken); ERROUT(e);
   free(buffer);
}

void freeAll(FifoT *buffer){
   //Producer
   int val;
   e= sem_getvalue(&buffer->taken,&val ); ERROUT(e);
   while(val<1){
      e=sem_post(&buffer->taken);ERROUT(e);
   }
   e=sem_getvalue(&buffer->empty,&val);ERROUT(e);
   while(val<1){
      e=sem_post(&buffer->empty);ERROUT(e);
   }
}
