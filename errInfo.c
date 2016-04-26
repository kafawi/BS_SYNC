#include "errInfo.h"


char txt[100];

int puterr(char * s, int fail){
   if( fail != 0 ){
      fprints(txt, ERROR_TXT, s);
      perror(s);
   }
   return 0;
}
