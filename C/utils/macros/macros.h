#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define fatal(fmt, args...) {\
  printf("\n*** fatal: " fmt, ##args);\
  exit(1);\
}

