#include "fifo.h"
#include "errInfo.h"
#include "pcc.h"
#include <pthread.h>
#include "setting.h"


   pthread_t producer1, producer2 , consumer, controler;

   pthread_cond_t prodBlock1, prodBlock2, consBlock;

   int isAlive, isProdBlock1, isProdBlock2, isConsBlock;

   FifoT * buffer;

   char *abcLowerCase, *abcUpperCase;

   ArgControl * argControl=NULL;
   ArgConsume * argConsume=NULL;
   ArgProduce * argProduce1=NULL;
   ArgProduce * argProduce2=NULL;


void init(){
  //-------------------------------------------------------------Buffer and Data
   buffer = (FifoT *) malloc(sizeof(*buffer));
   initFifo(buffer, FIFOSIZE);

   abcLowerCase=ABC_LOWER_CASE;
   abcUpperCase=ABC_UPPER_CASE;
   //---------------------------------------------------------- Conditional Vars
   isAlive = TRUE;
   isProdBlock1 = FALSE;
   isProdBlock2 = FALSE;
   isConsBlock = FALSE;

   //----------------------------------- Initialisierung pthread Conditions Vars
   e=pthread_cond_init(&consBlock, NULL);ERROUT(e);
   e=pthread_cond_init(&prodBlock1, NULL);ERROUT(e);
   e=pthread_cond_init(&prodBlock2, NULL);ERROUT(e);

   //------------------------------------------------------------ Control Args
   argControl = (ArgControl*)malloc(sizeof(*argControl));
   MERROUT(argControl);
   //-----------------------------------------------Prod 1
   argControl->condProd1=&prodBlock1;
   argControl->isProdBlock1=&isProdBlock1;
   //-----------------------------------------------Prod2
   argControl->condProd2=&prodBlock2;
   argControl->isProdBlock2=&isProdBlock2;
   //-----------------------------------------------Consume
   argControl->condCons=&consBlock;
   argControl->isConsBlock=&isConsBlock;
   //--------------------------------Life of Threads (while)
   argControl->isAlive=&isAlive;

   //------------------------------------------------------------ Consume   Args
   argConsume = (ArgConsume*)malloc(sizeof(*argConsume));
   MERROUT(argConsume);
   argConsume->buffer = buffer;
   argConsume->cond =&consBlock;
   argConsume->isBlock =&isConsBlock;
   argConsume->isAlive =&isAlive;

   //------------------------------------------------------------ Produce 1 Args
   argProduce1 = (ArgProduce*)malloc(sizeof(*argProduce1));
   MERROUT(argProduce1 );
   argProduce1->buffer = buffer;
   argProduce1->cList = abcLowerCase;
   argProduce1->cond = &prodBlock1;
   argProduce1->isBlock =&isProdBlock1;
   argProduce1->isAlive =&isAlive;

   //------------------------------------------------------------ Produce 2 Args
   argProduce2 = (ArgProduce*)malloc(sizeof(*argProduce2));
   MERROUT(argProduce2);
   argProduce2->buffer = buffer;
   argProduce2->cList = abcUpperCase;
   argProduce2->cond =&prodBlock2;
   argProduce2->isBlock=&isProdBlock2;
   argProduce2->isAlive =&isAlive;

   //----------------------------------------------- Initialisierung der Threads
   pthread_create(&producer1, NULL, produce, (void *)argProduce1);
   pthread_create(&consumer,  NULL, consume, (void *)argConsume);
   pthread_create(&producer2, NULL, produce, (void *)argProduce2);
   pthread_create(&controler, NULL, control, (void *)argControl);

}

void destroy(){
   puts("start demolition");

   puts("block P and C");
   isProdBlock1=TRUE;
   isProdBlock2=TRUE;
   isConsBlock=TRUE;

   puts("try kill");
   isAlive=FALSE; //

   puts("free them from the semaphoren");
   freeAll(buffer);

   puts("free them from local mutex");
   e= pthread_cond_signal(&prodBlock1); ERROUT(e);
   e= pthread_cond_signal(&prodBlock2); ERROUT(e);
   e= pthread_cond_signal(&consBlock); ERROUT(e);
   puts("wait terminating");
   e= pthread_join(producer1, NULL); ERROUT(e);
   e= pthread_join(producer2, NULL); ERROUT(e);
   e= pthread_join(consumer, NULL); ERROUT(e);

   puts("free the space");
   destroyFifo(buffer);
   free(argProduce1);
   free(argProduce2);
   free(argControl);
   free(argConsume);
}

int main(){
   init();
   pthread_join(controler, NULL);
   destroy();
   return 0;
}
