#include "ccp.h"


void * produce(FifoT * buffer, char * cList, pthread_cond_t * cond){
   pthread_mutex_t mutex;
   pthread_cond_t finish;
   int i=0;
   int fail=0;
   fail = pthread_mutex_init(&mutex, NULL);
   fail = pthread_cond_init(&finish, NULL);
   
   while(1){                 //alive
      fail = pthread_mutex_lock(&mutex);
      //not running & alive // stop by control 
      fail = pthread_cond_wait(cond, &mutex);
      if (i >= cList.length){
         pthread_cond_signal(finish);
      }
       // witing because finish 
      fail = pthread_cond_wait(finish, &mutex);
      
      sleep(SLEEP_TIME_PRODUCE_SEK);
      push(&buffer, cList[i]);
      i++
      pthread_mutex_unlock(&mutex);
   }
   return 0;
   
}

void * produceThread(void * arg){
   ProduceParam * p = arg;
   produce(p->buffer, p->cList, p->cond);
}


void * consume(FifoT * buffer, pthread_cond_t * cond){
   pthread_mutex_t mutex;
   int fail=0;
   char output = 0;
   fail = pthread_mutex_init(&mutex, NULL);
   puterr("ccp.c consume: mutex_init", fail);
   
   while(1){                 //alive
      fail = pthread_mutex_lock(&mutex);
      puterr("ccp.c consume: mutex_lock", fail); 
      fail = pthread_cond_wait(cond, &mutex);
      puterr("ccp.c consume: cond_wait", fail);
      fail = sleep(SLEEP_TIME_CONSUME_SEK);
      puterr("ccp.c consume: sleep", fail);
      // wenn nix mehr im Puffer ist, wartet er im Puffer dann ist er fertig
      output = pop(&buffer); 
      putchar(output);
      fail = pthread_mutex_unlock(&mutex);
      puterr("ccp.c consume: mutex_unlock", fail);
   }
   return 0;
}

void * consumeThread(void * arg){
   ConsumeParam * p = arg;
   consume(p->buffer, p->cond);
}


void * control(
   pthread_cond_t * produce1CV, pthread_cond_t * produce2CV,
   pthread_cond_t * consumeCV
){
   int fail=0;

   while(isRunning){

      tmp=getchar();
      switch(tmp){

         case'1': 
            fail = pthread_cond_signal(produce1CV);
            puterr("ccp.c control: cond_signal(produce1CV)" fail);
            break;

         case'2': 
            fail = pthread_cond_signal(produce2CV);
            puterr("ccp.c control: cond_signal(produce2CV)" fail);
            break;

         case'C':
         case'c': 
            fail = pthread_cond_signal(consumeCV);
            puterr("ccp.c control: cond_signal(consumeCV)" fail);
            break;
         case 'Q':
         case 'q':
            fail = pthread_exit(NULL);
            puterr("ccp.c control: thread_exit" fail);
            break;
         case 'h':
            fail = printf(HELP_TXT1, HELP_TXT2);
            puterr("ccp.c control: printf" fail);
         default:
            break;

      }
   }
   return 0;
}

void * controlThread(void * arg){
   ControlParam * p = arg;
   control(p->buffer, p->cList, p->cond);
}


