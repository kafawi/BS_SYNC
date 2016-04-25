#include <stdio.h>
#include <errno.h>
#include <string.h>

#define S_ERROR_TXT "Failure at %s:"
char txt[100];

int puterr(char * s, int anyfail){
   if( anyfail == -1 ){
      fprints(txt, S_ERROR_TXT, s);
      perror(s);
   }
   return 0;
}
