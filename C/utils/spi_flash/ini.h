#ifndef _INI_H_
#define _INI_H_

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

typedef int (*parsefunc_t) (char *key, char *val, void *arg);

FILE *OpenIniFile(const char *filename);

int CloseIniFile(FILE *pf);

int ParseIniLines(FILE *pf, parsefunc_t func, void *arg);

int ParseIni(char *filename, parsefunc_t func, void *arg);

#endif
