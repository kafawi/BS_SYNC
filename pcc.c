#include "pcc.h"
#include "errInfo.h"
#include "fifo.h"
#include "setting.h"

void * produce(void * argProduce){
   ArgProduce * p = (ArgProduce *) argProduce;
   int isAlive=1;
   int e=0;
   unsigned int i=0;
   while(isAlive){
      e=sleep(WAIT_PRODUCE_SEK);
      push(p->buffer, p->cList[i]);
      i++;
      if (i > 25){
         isAlive=0;
      }
   }
   return 0;
}

void * consume(void * argConsume){//al
   ArgConsume * p = (ArgConsume *) argConsume;//al
   int isAlive=1;
   int e=0;
   char output = 0;
   while(isAlive){
      e = sleep(WAIT_CONSUME_SEK); //ERROUT(e);
      output = pop(p->buffer);
      // puts because putchar doesnt work, without a additional puts
      e = puts(&output); //ERROUT(e);
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
            pthread_cond_signal(p->condProduce1);
            break;
         case'2':
            pthread_cond_signal(p->condProduce2);
            break;
         case'C':
         case'c':
            pthread_cond_signal(p->condConsume);
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
