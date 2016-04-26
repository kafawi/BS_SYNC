#include "fifo.h"
#include "ccp.h"

#define FIFOSIZE 10

#include <pthread.h>

   pthread_t producer1, producer2 , consumer, controller;
   pthread_cond_t producer1CV, producer2CV , consumerCV, controllerCV;
   pthread_mutex_t producer1M, producer2M , consumerM, controllerM;

int main(){
   
   
   init();
   pthread_join( controller, NULL);
   destroy();
   pthread_join( producer1, NULL );
   pthread_join( producer2, NULL );
   pthread_join( consumer, NULL );
   return 0;
}


void init(){
   // input
   char abcLowerCase[] = "abcdefghijklmnopqrstuvwxyz";
   char abcUpperCase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   // init the buffer
   Fifo_t buffer = NULL;
   buffer = initFifo(FIFOSIZE);
   // init mutexes
   pthread_mutex_init(&controllerM, NULL);
   pthread_mutex_init(&consumerM, NULL);
   pthread_mutex_init(&producer1M, NULL);
   pthread_mutex_init(&producer2M, NULL);
   // init cv
   pthread_cond_init(&controllerCV, NULL);
   pthread_cond_init(&consumerCV, NULL);
   pthread_cond_init(&producer1CV, NULL);
   pthread_cond_init(&producer2CV, NULL);
   // init the other threads
   pthread_create(&controller, NULL,control, p);
   pthread_create(&consumer, NULL,consum, p);
   pthread_create(&producer1, NULL,controller, p);
   pthread_create(&producer2, NULL,controller, p);
}


void destroy(){
   
}
