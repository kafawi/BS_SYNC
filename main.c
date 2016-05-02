#include "fifo.h"
#include "errInfo.h"
#include "pcc.h"
#include <pthread.h>
#include "setting.h"

#define FIFOSIZE 10
#define ABC_SIZE 26



   pthread_t producer1, producer2 , consumer, controler;
   pthread_cond_t prodBlock1, prodBlock2, consBlock;
   int * isAlive, *isProdBlock1, *isProdBlock2, *isConsBlock;
   FifoT * buffer;

   char *abcLowerCase, *abcUpperCase;

   ArgControl * argControl=NULL;
   ArgConsume * argConsume=NULL;
   ArgProduce * argProduce1=NULL;
   ArgProduce * argProduce2=NULL;


void init(){
   buffer = (FifoT *) malloc(sizeof(*buffer));
   //if(buffer==NULL){puts("mist");}
   initFifo(buffer, FIFOSIZE);

   abcLowerCase="abcdefghijklmnopqrstuvwxyz";
   abcUpperCase="ABCDEFGHIJKLMNOPQRSTUVWXYZ";

   argConsume = (ArgConsume*)malloc(sizeof(*argConsume));
   argConsume->buffer = buffer;
   argConsume->cond =NULL;// &consumeCV;


   argProduce1 = (ArgProduce*)malloc(sizeof(*argProduce1));
   argProduce1->buffer = buffer;
   argProduce1->cList = abcLowerCase;
   argProduce1->cond =NULL;// &produce1CV;



   argProduce2 = (ArgProduce*)malloc(sizeof(*argProduce2));
   argProduce2->buffer = buffer;
   argProduce2->cList = abcUpperCase;
   argProduce2->cond =NULL;// &produce2CV;


   // init threads


   pthread_create(&producer1, NULL, produce, (void *)argProduce1);
   pthread_create(&consumer,  NULL, consume, (void *)argConsume);
   pthread_create(&producer2, NULL, produce, (void *)argProduce2);
   //pthread_create(&controler, NULL, control, (void *)argControl);
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
   sleep(300);
   //pthread_join(controler, NULL);
   destroy();
   return 0;
}
