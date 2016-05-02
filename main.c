#include "fifo.h"
#include "errInfo.h"
#include "pcc.h"
#include <pthread.h>
#include "setting.h"

#define FIFOSIZE 10
#define ABC_SIZE 26



   pthread_t producer1, producer2 , consumer, controler;
   pthread_cond_t prodBlock1, prodBlock2, consBlock,condQuit;
   int isAlive, isProdBlock1, isProdBlock2, isConsBlock;
   FifoT * buffer;

   char *abcLowerCase, *abcUpperCase;

   ArgControl * argControl=NULL;
   ArgConsume * argConsume=NULL;
   ArgProduce * argProduce1=NULL;
   ArgProduce * argProduce2=NULL;


void init(){
   buffer = (FifoT *) malloc(sizeof(*buffer));
   initFifo(buffer, FIFOSIZE);

   abcLowerCase=ABC_LOWER_CASE;
   abcUpperCase=ABC_UPPER_CASE;

   isAlive = TRUE;
   isProdBlock1 = FALSE;
   isProdBlock2 = FALSE;
   isConsBlock = FALSE;

   // init Conds
   e=pthread_cond_init(&consBlock, NULL);ERROUT(e);
   e=pthread_cond_init(&prodBlock1, NULL);ERROUT(e);
   e=pthread_cond_init(&prodBlock2, NULL);ERROUT(e);
   e=pthread_cond_init(&condQuit, NULL);ERROUT(e);

   argControl = (ArgControl*)malloc(sizeof(*argControl));
   MERROUT(argControl);
   argControl->condProd1=&prodBlock1;
   argControl->isProdBlock1=&isProdBlock1;

   argControl->condProd2=&prodBlock2;
   argControl->isProdBlock2=&isProdBlock2;

   argControl->condCons=&consBlock;
   argControl->isConsBlock=&isConsBlock;

   argControl->condQuit=&condQuit;
   argControl->isAlive=&isAlive;


   argConsume = (ArgConsume*)malloc(sizeof(*argConsume));
   MERROUT(argConsume);
   argConsume->buffer = buffer;
   argConsume->cond =&consBlock;
   argConsume->isBlock =&isConsBlock;
   argConsume->isAlive =&isAlive;

   argProduce1 = (ArgProduce*)malloc(sizeof(*argProduce1));
   MERROUT(argProduce1 );
   argProduce1->buffer = buffer;
   argProduce1->cList = abcLowerCase;
   argProduce1->cond = &prodBlock1;
   argProduce1->isBlock =&isProdBlock1;
   argProduce1->isAlive =&isAlive;


   argProduce2 = (ArgProduce*)malloc(sizeof(*argProduce2));
   MERROUT(argProduce2);
   argProduce2->buffer = buffer;
   argProduce2->cList = abcUpperCase;
   argProduce2->cond =&prodBlock2;
   argProduce2->isBlock=&isProdBlock2;
   argProduce1->isAlive =&isAlive;



   // init threads


   pthread_create(&producer1, NULL, produce, (void *)argProduce1);
   pthread_create(&consumer,  NULL, consume, (void *)argConsume);
   pthread_create(&producer2, NULL, produce, (void *)argProduce2);
   pthread_create(&controler, NULL, control, (void *)argControl);

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
