#ifndef PCC_H_
#define PCC_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "fifo.h"
#include "errInfo.h"

#define WAIT_CONSUME_SEK 2
#define WAIT_PRODUCE_SEK 3

#define HELP_TXT "\
1:   start/stop Producer for LowerCase\n \
2:   start/stop Producer for UpperCase\n \
C/c: start/stop Consumer\n \
Q/q: quit\n \
"

typedef struct {
   FifoT * buffer;
   char * cList;
   pthread_cond_t * cond;
   int * isBlock;
   int * isAlive;
} ArgProduce;

typedef struct {
   FifoT * buffer;
   pthread_cond_t * cond;
   int * isBlock;
   int * isAlive;
} ArgConsume;

typedef struct {
   pthread_cond_t * condProd1;
   int * isProdBlock1;
   pthread_cond_t * condProd2;
   int * isProdBlock2;
   pthread_cond_t * condCons;
   int * isConsBlock;
   pthread_cond_t * condQuit;
   int * isAlive;
} ArgControl;

void * produce(void * argProduce);

void * consume(void * argConsume);

void * control(void * argControl);

#endif /* PCC_H_ */
