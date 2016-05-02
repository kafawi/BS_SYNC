#include "pcc.h"
#include "errInfo.h"
#include "fifo.h"
#include "setting.h"

void * produce(void * argProduce){
   ArgProduce * p = (ArgProduce *) argProduce;
   pthread_mutex_t mutex;
   e=pthread_mutex_init(&mutex, NULL);ERROUT(e);
   int e=0;
   unsigned int i=0;
   while(*p->isAlive==TRUE){

      if (*p->isBlock==TRUE){
         e = pthread_cond_wait(p->cond, &mutex);ERROUT(e);
         *p->isBlock=FALSE;
         // springt raus wenn tot
         if (*p->isAlive==FALSE){
            break;
         }
      }

      e=sleep(WAIT_PRODUCE_SEK);
      push(p->buffer, p->cList[i]);
      printf("Producer: %c\n", p->cList[i] );
      i++;
      if (p->cList[i]== 0){
         i=0;
      }
   }
   e = pthread_mutex_destroy(&mutex); ERROUT(e);
   pthread_exit(EXIT_SUCCESS);
   return 0;
}

void * consume(void * argConsume){
   ArgConsume * p = (ArgConsume *) argConsume;
   pthread_mutex_t mutex;
   e=pthread_mutex_init(&mutex, NULL);
   char output = 0;
   while(*p->isAlive==TRUE){
      if (*p->isBlock==TRUE){
         e = pthread_cond_wait(p->cond, &mutex);ERROUT(e);
         // springt raus wenn tot
         if (*p->isAlive==FALSE){
            break;
         }
         *p->isBlock=FALSE;
      }

      e = sleep(WAIT_CONSUME_SEK); ERROUT(e);
      output = pop(p->buffer);

      // puts because putchar doesnt work, without a additional puts
      e = puts(&output); ERROUT(e);
   }
   e = pthread_mutex_destroy(&mutex); ERROUT(e);
   pthread_exit(EXIT_SUCCESS);
   return 0;
}

void * control(void * argControl){
   ArgControl * p = (ArgControl *) argControl;
   char tmp = 0;
   while(*p->isAlive==TRUE){
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
