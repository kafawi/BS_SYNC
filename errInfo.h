
#ifndef ERRINFO_H_
#define ERRINFO_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERROR_TXT "Failure in %s:"

int puterr(char * s, int fail);

// MACRO


    #define MELDUNG(text) fprintf( stderr, \
            "Datei [%s], Zeile %d: %s\n" \
            __FILE__, __LINE__, text )

#endif /* ERRINFO_H_ */
