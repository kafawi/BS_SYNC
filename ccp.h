#ifndef CCP_H_
#define CCP_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "fifo.h"
#include "errInfo.h"

#define SLEEP_TIME_CONSUME_SEK 2L
#define SLEEP_TIME_PRODUCE_SEK 3L

#define HELP_TXT1 "1: start/stop Poducer 1 \n2:start/stop Producer2 %s"
#define HELP_TXT2 "\nC/c: start/stop Consumer \nQ/q: quit\n"

typedef struct {
   FifoT * buffer;
   char * cList;
   pthread_cond_t * cond;
} ProduceParam;

typedef struct {
   FifoT * buffer;
   pthread_cond_t * cond;
} ConsumeParam;

typedef struct {
   pthread_cond_t * produce1CV; 
   pthread_cond_t * produce2CV;
   pthread_cond_t * consumeCV;
} ControlParam;

void * produce(FifoT * buffer, char * cList, pthread_cond_t * cond);
void * produceThread(void *arg);

void * consume(FifoT * buffer, pthread_cond_t * cond);
void * consumeThread(void *arg);

void * control(
   pthread_cond_t * produce1CV, 
   pthread_cond_t * produce2CV,
   pthread_cond_t * consumeCV
);
void * controlThread(void * arg);



#endif /* CCP_H_ */
