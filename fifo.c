#include "fifo.h"
#include "errInfo.h"
#include <stdio.h>

FifoT * initFifo(int size){
   //int fail=0;
   int i=0;
   FifoT * buffer = (FifoT*)malloc( sizeof(*buffer) );
   //fail =
   sem_init(&buffer->taken,0, 0);
   //puterr("fifo initFifo: sem_init taken",fail);
   //fail =
   sem_init(&buffer->empty,0, size);
   //puterr("fifo initFifo: sem_init free",fail);
   //fail =
   pthread_mutex_init(&buffer->mutex, NULL);
   //puterr("fifo initFifo: mutex_init",fail);
   buffer->size=size;
   buffer->first=0;
   buffer->last=size;
   buffer->element =(char*) malloc( sizeof(buffer->element) *size);

   if (buffer->element == NULL){
      puts("fifo iniFifo: malloc " );
   }
   for (i=0; i< size; i++){
      buffer->element[i]=0;
   }
   puts("2");
   return buffer;

}
void upFirst(FifoT *buffer){
   buffer->first++;
   if (buffer->first >= buffer->size){
      buffer->first=0;
   }
}
void upLast( FifoT *buffer){
   buffer->last++;
   if (buffer->last >= buffer->size){
      buffer->last=0;
   }
}

void push( FifoT *buffer, char data){
   //int fail =0;
   //fail =
   sem_wait(&buffer->empty);
   //puterr("fifo push: sem_wait free",fail);

   //fail =
   pthread_mutex_lock(&buffer->mutex);
   //puterr("fifo push: mutex_lock",fail);

   upLast(buffer);
   buffer->element[buffer->last] = data;


   //fail =
   pthread_mutex_unlock(&buffer->mutex);
   //puterr("fifo push: mutex_unlock",fail);

   sem_post(&buffer->taken);
   //puterr("fifo push: sem_post taken",fail);
}

char pop( FifoT *buffer){
   char element = 0;
//   int fail =0;

   //fail =
   sem_wait(&buffer->taken);
   //puterr("fifo pop: sem_wait taken",fail);

   //fail =
   pthread_mutex_lock(&buffer->mutex);
   //puterr("fifo pop: mutex_lock",fail);

   element = buffer->element[buffer->first];
   upFirst(buffer);

   //fail =
   pthread_mutex_unlock(&buffer->mutex);
   //puterr("fifo pop: mutex_unlock",fail);

   //fail =
   sem_post(&buffer->empty);
   //puterr("fifo pop: sem_post free",fail);

   return element;
}

void destroyFifo(FifoT *buffer){
   //int fail=0;
   free(buffer->element);

   //fail =
   pthread_mutex_destroy(&buffer->mutex);
   //puterr("Destroying Fifos mutex",fail);

   //fail =
   sem_destroy(&buffer->empty);
   //puterr("Destroying Fifos sem_t free",fail);

   //fail =
   sem_destroy(&buffer->taken);
   //puterr("Destroying Fifos sem_t taken",fail);
   // print fehler
   free(buffer);
}
