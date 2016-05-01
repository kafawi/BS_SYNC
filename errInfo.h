
#ifndef ERRINFO_H_
#define ERRINFO_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERROR_TXT "Failure in %s:"

int puterr(char * s, int fail);

#endif /* ERRINFO_H_ */
