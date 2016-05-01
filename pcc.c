#include "pcc.h"
#include "fifo.h"

void * produce(void * argProduce){
   ArgProduce * p = (ArgProduce *) argProduce;
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   //pthread_cond_t finish = PTHREAD_COND_INITIALIZER;
   int isAlive=1;
   unsigned int i=0;
   //pthread_mutex_init(&mutex, NULL);
   //pthread_cond_init(&finish, NULL);

   while(isAlive){                 //alive
      pthread_mutex_lock(&mutex);
      //pthread_cond_wait(&(p->cond), &mutex);
      sleep(WAIT_PRODUCE_SEK);
      push(p->buffer, p->cList[i]);
      i++;
      pthread_mutex_unlock(&mutex);

      if (i >= sizeof(p->cList)){
         //pthread_cond_signal(&finish);
         isAlive=0;
      }
      //pthread_cond_wait(&finish, &mutex);
   }
   return 0;
}


void * consume(void * argConsume){
   ArgConsume * p = (ArgConsume *) argConsume;
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   int isAlive=1;
   char output = 0;
   while(isAlive){                 //alive
      pthread_mutex_lock(&mutex);
      //pthread_cond_wait(&(p->cond), &mutex);
      sleep(WAIT_CONSUME_SEK);
      output = pop(p->buffer);
      putchar(output);
      pthread_mutex_unlock(&mutex);
   }
   return 0;
}

void * control(void * argControl){
   ArgControl * p = (ArgControl *) argControl;
   int isAlive=1;
   char tmp = 0;
   while(isAlive){
      tmp=getchar();
      switch(tmp){
         case'1':
            pthread_cond_signal(&(p->condProduce1));
            break;
         case'2':
            pthread_cond_signal(&(p->condProduce2));
            break;

         case'C':
         case'c':
            pthread_cond_signal(&(p->condConsume));
            break;
         case 'Q':
         case 'q':
            pthread_exit(NULL);
            break;
         case 'H':
         case 'h':
            puts(HELP_TXT);
			break;
         default:
            break;
      }
   }
   return 0;
}
