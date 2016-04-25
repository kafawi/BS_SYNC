/*
 * ccp.c
 *
 *  Created on: 25.04.2016
 *      Author: Sandy
 */

#include "fifo.h"
#include "ccp.h"

char sHelp[130] = strcat("\n1: start/stop Poducer 1 \n2:start/stop ", 
   "Producer2 \nC/c: start/stop Consumer \nQ/q: terminate all" )

pthread_mutex_t producer1_mutex;
pthread_mutex_t producer2_mutex;
pthread_mutex_t consumer_mutex;

pthread_cond_t producer1_cv;
pthread_cond_t producer2_cv;
pthread_cond_t consumer_cv;

char productList1[] = {'a','b','c','d','e','f','g','h','i','j','k','l',
'm','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char productList2[] = {'A','B','C','D','E','F','G','H','I','J','K','L',
'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
int pos1 = 0;
int pos2 = 0;

fifo_t *buffer = NULL;

int isRunning = 1; // Controller thread



void* producer1_thread(void* arg){

   pthread_mutex_init(&producer1_mutex, NULL);
   pthread_cond_init(&producer1_cv, NULL);

   while(producer1_thread){                 //alive
      pthread_mutex_lock(&producer1_mutex);
      while(!producer1_thread){                 //not running && alive
         pthrad_cond_wait(&producer1_cv, &producer1_mutex);
      }
      writeToBuffer(productList1, pos1);
      pthread_mutex_unlock(&producer1_mutex);
   }
   return 0;
}

void* producer2_thread(void* arg){

   pthread_mutex_init(&producer2_mutex, NULL);
   pthread_cond_init(&producer2_cv, NULL);

   while(producer2_thread){                      //alive
      pthread_mutex_lock(&producer2_mutex);
      while(!producer2_thread){                 //not running && alive
         pthrad_cond_wait(&producer2_cv, &producer2_mutex);
      }
      writeToBuffer(productList2, pos2);
      pthread_mutex_unlock(&producer2_mutex);
   }
   return 0;
}


void* writeToBuffer(productList, position){
   printf("Product: producing %c\n",((productList+position)));
   sleep(3);
   if(position <= 24){
      push(buffer, *(productList+position));
      position++;
   }else {
      push(buffer, *(productList[25]));
      position=0;
   }
   return 0;
}

void* consumer_thread(void* arg){

   pthread_mutex_init(&consumer_mutex, NULL);
   pthread_cond_init(&consumer_cv, NULL);

   while(consumer_thread){                    //alive
      pthread_mutex_lock(&consumer_mutex);
      while(!consumer_thread){               //not running && alive
         pthrad_cond_wait(&consumer_cv, &consumer_mutex);
      }
      readFromBuffer(buffer);
      pthread_mutex_unlock(&consumer_mutex);
   }
   return 0;
}

void * readFromBuffer(fifo_t){
   char tmp;
   printf("Consumer: consuming %c\n",(tmp));  //welches tmp bloß..
   sleep(2);
   pop(buffer);
   return 0;
}

controller(void* p){
   char tmp=-1;
   int prod1=1;
   int prod2=1;
   int cons=1;

   while(isRunning){
      tmp=getchar();

      switch(tmp){

         case'1': 
            if(prod1 == 1){
               prod1=0;
               printf("Controller Thread hat Producer 1 pausiert\n");
            }else{
               prod1=1;
               printf("Controller Thread hat Producer 1 fortgesetzt\n");
            }
            //change State of Prod1
            break;

         case'2': 
            if(prod2 == 1){
               prod2=0;
               printf("Controller Thread hat Producer 2 pausiert\n");
            }else{
               prod2=1;
               printf("Controller Thread hat Producer 2 fortgesetzt\n");
            }
            //change State of Prod2
            break;

         case'C':tmp=c;
         case'c': 
            if(cons == 1){
               cons=0;
               printf("Controller Thread hat Consumer pausiert\n");
            }else{
               cons=1;
               printf("Controller Thread hat Consumer fortgesetzt\n");
            }
            //change State of Cons
            break;

         case 'Q':tmp=q;
         case 'q':
            printf("Controller Thread terminiert alle Threads\n");
            isRunning=0;
            //terminate all
            break;
         case 'h':
            printf(sHelp);
         default:
            printf("\nController Thread Error: Invalid input")
            break;

      }
   }
   return 0;
}
