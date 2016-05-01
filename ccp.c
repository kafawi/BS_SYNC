#include "ccp.h"


void * produce(FifoT * buffer, char * cList, pthread_cond_t * cond){
   pthread_mutex_t mutex;
   pthread_cond_t finish;
   int i=0;
   int fail=0;
   fail = pthread_mutex_init(&mutex, NULL);
   puterr("ccp.c produce: mutex_init", fail);
   fail = pthread_cond_init(&finish, NULL);
   puterr("ccp.c produce: cond init finish", fail);
   
   while(1){                 //alive
      fail = pthread_mutex_lock(&mutex);
      puterr("ccp.c produce: mutex_lock", fail);
      //not running & alive // stop by control 
      //fail = pthread_cond_wait(cond, &mutex);
      puterr("ccp.c produce: cond_wait mutex", fail);
      if (i >= sizeof(cList)){
         pthread_cond_signal(&finish);
      }
       // waiting because finish till main kills it.
      fail = pthread_cond_wait(&finish, &mutex);
      puterr("ccp.c produce: cond_wait finish", fail);
      
      fail = sleep(SLEEP_TIME_PRODUCE_SEK);
      puterr("ccp.c produce: sleep", fail);
      push(buffer, cList[i]);
      i++;
      fail = pthread_mutex_unlock(&mutex);
      puterr("ccp.c produce: cond_wait finish", fail);
   }
   return 0;
   
}
// hie ist der fehler
void * produceThread(void *arg) {
   ProduceParam * p = (ProduceParam *) arg;
   produce(p->buffer, p->cList, p->cond);
   return 0;
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
      // wenn nix mehr im Puffer ist, wartet er im Puffer wegen semaphoren
      output = pop(buffer); 
      putchar(output);
      fail = pthread_mutex_unlock(&mutex);
      puterr("ccp.c consume: mutex_unlock", fail);
   }
   return 0;
}

void * consumeThread(void * arg){
   ConsumeParam * p =(ConsumeParam *) arg;
   consume(p->buffer, p->cond);
   return 0;
}


void * control(
   pthread_cond_t * produce1CV, pthread_cond_t * produce2CV,
   pthread_cond_t * consumeCV
){
   int fail=0;
   char tmp = 0;
   while(1){
	  // könte blockieren warum auch immer
      tmp=getchar();
      switch(tmp){
         case'1': 
            fail = pthread_cond_signal(produce1CV);
            puterr("ccp.c control: cond_signal(produce1CV)", fail);
            break;

         case'2': 
            fail = pthread_cond_signal(produce2CV);
            puterr("ccp.c control: cond_signal(produce2CV)", fail);
            break;

         case'C':
         case'c': 
            fail = pthread_cond_signal(consumeCV);
            puterr("ccp.c control: cond_signal(consumeCV)", fail);
            break;
         case 'Q':
         case 'q':
            pthread_exit(NULL);
            break;
         case 'H':
         case 'h':
            fail = printf(HELP_TXT1, HELP_TXT2);
            puterr("ccp.c control: printf", fail);
			break;
         default:
            break;

      }
   }
   return 0;
}

void * controlThread(void * arg){
   ControlParam * p = (ControlParam *)arg;
   control(p->produce1CV, p->produce2CV, p->consumeCV);
   return 0;
}
