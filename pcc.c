#include "pcc.h"
#include "errInfo.h"
#include "fifo.h"
#include "setting.h"

void * produce(void * argProduce){
   ArgProduce * p = (ArgProduce *) argProduce;
   pthread_mutex_t mutex;
   e = pthread_mutex_init(&mutex, NULL); ERROUT(e);
   int e=0;
   unsigned int i=0;
   while(*p->isAlive){
      if (*p->isBlock == TRUE){
         e = pthread_mutex_lock(&mutex);ERROUT(e);
         e = pthread_cond_wait(p->cond, &mutex);ERROUT(e);
         e = pthread_mutex_unlock(&mutex);ERROUT(e);
         p->isBlock=FALSE;
      }
      e=sleep(WAIT_PRODUCE_SEK);
      push(p->buffer, p->cList[i]);
      i++;
      if (p->cList[i]== 0){
         *p->isAlive=0;
      }
   }
   e = pthread_mutex_destroy(&mutex); ERROUT(e);
   return 0;
}

void * consume(void * argConsume){
   ArgConsume * p = (ArgConsume *) argConsume;
   pthread_mutex_t mutex;
   e = pthread_mutex_init(&mutex, NULL); ERROUT(e);
   int e=0;
   char output = 0;
   while(*p->isAlive){

      if (*p->isBlock == TRUE){
         e = pthread_mutex_lock(&mutex);ERROUT(e);
         e = pthread_cond_wait(p->cond, &mutex);ERROUT(e);
         e = pthread_mutex_unlock(&mutex);ERROUT(e);
         *p->isBlock=FALSE;
      }

      e = sleep(WAIT_CONSUME_SEK); ERROUT(e);
      output = pop(p->buffer);
      // puts because putchar doesnt work, without a additional puts
      e = puts(&output); ERROUT(e);
   }
   e = pthread_mutex_destroy(&mutex); ERROUT(e);
   return 0;
}

void * control(void * argControl){
   ArgControl * p = (ArgControl *) argControl;
   char tmp = 0;
   while(p->isAlive){
      tmp=getchar();
      switch(tmp){
         case'1':
            e=pthread_cond_signal(p->condProd1); ERROUT(e);
            *p->isProdBlock1 = TRUE;
            break;
         case'2':
            e=pthread_cond_signal(p->condProd2); ERROUT(e);
            *p->isProdBlock2 = TRUE;
            break;
         case'C':
         case'c':
            e=pthread_cond_signal(p->condCons); ERROUT(e);
            *p->isConsBlock = TRUE;
            break;
         case 'Q':
         case 'q':
            e=pthread_cond_signal(p->condQuit); ERROUT(e);
            pthread_exit(NULL);
            break;
         case 'H':
         case 'h':
            e = puts(HELP_TXT); ERROUT(e);
         break;
         default:
            break;
      }
   }
   return 0;
}
