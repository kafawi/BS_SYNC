#include "fifo.h"
#include "errInfo.h"
#include "pcc.h"
#include <pthread.h>

#define FIFOSIZE 10



   pthread_t producer1, producer2 , consumer, controler;
   pthread_cond_t produce1CV, produce2CV , consumeCV;
   FifoT * buffer;

   ArgControl * argControl=NULL;
   ArgConsume * argConsume=NULL;
   ArgProduce * argProduce1=NULL;
   ArgProduce * argProduce2=NULL;


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


   argControl = (ArgControl *) malloc(sizeof(*argControl));
   argControl->condProduce1 = produce1CV;
   argControl->condProduce1 = produce2CV;
   argControl->condConsume  = consumeCV;


   argConsume = (ArgConsume*)malloc(sizeof(*argConsume));
   argConsume->buffer = buffer;
   argConsume->cond = consumeCV;


   argProduce1 = (ArgProduce*)malloc(sizeof(*argProduce1));
   argProduce1->buffer = buffer;
   argProduce1->cList = abcLowerCase;
   argProduce1->cond = produce1CV;


   argProduce2 = (ArgProduce*)malloc(sizeof(*argProduce2));
   argProduce2->buffer = buffer;
   argProduce2->cList = abcUpperCase;
   argProduce2->cond = produce2CV;


   // init threads
   pthread_create(&controler, NULL, control, (void *)argControl);
   pthread_create(&consumer,  NULL, consume, (void *)argConsume);
   pthread_create(&producer1, NULL, produce, (void *)argProduce1);
   pthread_create(&producer2, NULL, produce, (void *)argProduce2);
}


void destroy(){
   free(argProduce1);
   free(argProduce2);
   free(argControl);
   free(argConsume);
   pthread_cancel(consumer);
   pthread_cancel(producer2);
   pthread_cancel(producer1);
   destroyFifo(buffer);
}

int main(){
   init();
   pthread_join(controler, NULL);
   destroy();
   return 0;
}
