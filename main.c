#include "fifo.h"
#include "errInfo.h"
#include "ccp.h"
#include <pthread.h>

#define FIFOSIZE 10



   pthread_t producer1, producer2 , consumer, controller;
   pthread_cond_t produce1CV, produce2CV , consumeCV;
   FifoT * buffer;

   ControlParam * controlParam=NULL;
   ConsumeParam * consumeParam=NULL;
   ProduceParam * produceParam1=NULL;
   ProduceParam * produceParam2=NULL;


void init(){
   //int fail=0;
   // input
   char abcLowerCase[] = "abcdefghijklmnopqrstuvwxyz";
   char abcUpperCase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

   // init the buffer
   buffer = initFifo(FIFOSIZE);
   // init cv
   pthread_cond_init(&consumeCV, NULL);
   pthread_cond_init(&produce1CV, NULL);
   pthread_cond_init(&produce2CV, NULL);
   // init ParameterList


   controlParam = (ControlParam*) malloc(sizeof(*controlParam));
   controlParam->produce1CV = &produce1CV;
   controlParam->produce2CV = &produce2CV;
   controlParam->consumeCV  = &consumeCV;


   consumeParam = (ConsumeParam*)malloc(sizeof(*consumeParam));
   consumeParam->buffer = buffer;
   consumeParam->cond = &consumeCV;


   produceParam1 = (ProduceParam*)malloc(sizeof(*produceParam1));
   produceParam1->buffer = buffer;
   produceParam1->cList = abcLowerCase;
   produceParam1->cond = &consumeCV;


   produceParam2 = (ProduceParam*)malloc(sizeof(*produceParam2));
   produceParam2->buffer = buffer;
   produceParam2->cList = abcUpperCase;
   produceParam2->cond = &consumeCV;


   // init threads
   pthread_create(&controller,NULL, controlThread, (void *)controlParam);
   pthread_create(&consumer,  NULL, consumeThread, (void *)consumeParam);
   pthread_create(&producer1, NULL, produceThread, (void *)produceParam1);
   pthread_create(&producer2, NULL, produceThread, (void *)produceParam2);
}


void destroy(){
   free(produceParam1);
   free(produceParam2);
   free(controlParam);
   free(consumeParam);
   pthread_cancel(consumer);
   pthread_cancel(producer2);
   pthread_cancel(producer1);
   destroyFifo(buffer);
}

int main(){
   init();
   pthread_join(controller, NULL);
   destroy();
   return 0;
}
