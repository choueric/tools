#ifndef _STATISTIC_H
#define _STATISTIC_H

#include <time.h>
#include <sys/time.h>

struct statistic {
  char *name;
  struct timeval last;
  struct timeval now;
  unsigned int count;
  int interval; // us
};

/* @interval: us */
void init_statistic(struct statistic *p, char *name, int interval);

/* add count, if now - last = interval , print */
void print_statistic(struct statistic *p);
#endif
