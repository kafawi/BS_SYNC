#ifndef ERRINFO_H_
#define ERRINFO_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define AT \
   fprintf( stderr,\
      "        at file %s function %s in line %d!", \
      __FILE__ , __FUNCTION__ ,__LINE__ \
   );

#define ERROUT(e) \
   if (e < 0){ \
      perror("Failure"); \
      AT \
   }
#define MERROUT(p) \
   if ( p == NULL ) { \
      fprintf(stderr, "malloc faild, needs space!"); \
      AT \
   }

int e;
#endif /* ERRINFO_H_ */
