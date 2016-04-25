#include "fifo.h"
#include <stdio.h>
#include <stdlib.h>

void initFifo(fifo_t *buffer, int size){
   int i=0;
   sem_init(&buffer->taken,0, 0);
   sem_init(&buffer->free,1, size);
   pthread_mutex_init(&buffer->mutex, NULL);
   buffer->size=size;
   buffer->first=0;
   buffer->last=size;
   buffer->element = malloc(sizeof(buffer->element)*size);
   for (i=0; i< size; i++){
      buffer->element[i]=0;
   }
}

void upFirst(fifo_t *buffer){
   buffer->first++;
   if (buffer->first >= buffer->size){
      buffer->first=0;
   }
}
void upLast( fifo_t *buffer){
   buffer->last++;
   if (buffer->last >= buffer->size){
      buffer->last=0;
   }
}

void push( fifo_t *buffer, char data){
   sem_wait(&buffer->free);
   pthread_mutex_lock(&buffer->mutex);
   upLast(buffer);
   buffer->element[buffer->last] = data;
   pthread_mutex_unlock(&buffer->mutex);
   sem_post(&buffer->taken);
}

char pop( fifo_t *buffer){
   char element = 0;
   sem_wait(&buffer->taken);
   pthread_mutex_lock(&buffer->mutex);
   element = buffer->element[buffer->first];
   upFirst(buffer);
   pthread_mutex_unlock(&buffer->mutex);
   sem_post(&buffer->free);
   return element;
}

void destroy( fifo_t *buffer){
   //int errTmp= errno;
   free(buffer->element);
   // print fehler
   pthread_mutex_destroy(&buffer->mutex);
   // print fehler
   sem_destroy(&buffer->free);
   sem_destroy(&buffer->taken);
   // print fehler
}


int main() {
   fifo_t buffer;
   init(&buffer,10);
   char data[] = {'a','b','c','d','e','f','g','h','i','j','k'};
   int i;
   for (i=0;i<11;i++){
      printf("push: %c", data[i]);
      push(&buffer, data[i]);
	  printf(" %c\n", buffer.element[i]);
   }
   printf("\n");
   for (i=0;i<10;i++){
      printf("pop: %c\n", pop(&buffer));
   }
destroy(&buffer);
}
//print fehler methode bauen. 
