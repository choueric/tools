#pragma once

#include <time.h>
#include <sys/time.h>

#define NAME_LEN 32

struct statistic {
  char name[NAME_LEN];
  struct timeval last;
  struct timeval now;
  unsigned int count;
  int interval; // us
};

/* @interval: us */
void init_statistic(struct statistic *p, char *name, int interval);

/* add count, if now - last = interval , print */
void print_statistic(struct statistic *p);
