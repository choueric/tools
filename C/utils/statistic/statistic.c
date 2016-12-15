#include "statistic.h"

#include <stdio.h>
#include <string.h>

/* return interval between @cur and @last, unit is us */
inline static int get_interval(struct timeval *cur, struct timeval *last)
{
	return (cur->tv_sec - last->tv_sec) * 1000000 + cur->tv_usec - last->tv_usec;
}

void init_statistic(struct statistic *p, char *name, int interval)
{
  strncpy(p->name, name, NAME_LEN);
  p->count = 0;
  gettimeofday(&p->last, NULL);
  p->interval = interval; // us
}

/*
 * add count, if now - last = interval , print 
 */
void print_statistic(struct statistic *p)
{
  gettimeofday(&p->now, NULL);

  p->count++;
  int interval = get_interval(&p->now, &p->last);
  if (interval <= p->interval) {
    return;
  }
    
  printf("\n[%s] %ld.%06ld: fps: %.2f in last %ds\n", p->name,
	  p->now.tv_sec, p->now.tv_usec,
	  (float)p->count * 1000000 / interval, p->interval / 1000000);
  p->count = 0;
  p->last.tv_sec = p->now.tv_sec;
  p->last.tv_usec = p->now.tv_usec;
}
