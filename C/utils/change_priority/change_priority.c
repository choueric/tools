#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>

int set_priority(int priority)
{
	int policy = SCHED_FIFO;
	struct sched_param param;

	param.sched_priority = priority;
	if (0 != pthread_setschedparam(pthread_self(), policy, &param)) {
		printf("pthread_setschedparam failed: %m\n");
		return -1;
	}
	
	return 1;
}

int get_priority()
{
	int policy;
	struct sched_param param;

	if(0 != pthread_getschedparam(pthread_self(), &policy, &param)) {
		printf("pthread_getschedparam faild: %m\n");
		return -1;
	}
		
	printf("thread priority: %s/%d\n", (policy == SCHED_FIFO ? "FIFO"
				: (policy == SCHED_RR ? "RR" : (policy == SCHED_OTHER ? "OTHER"
				: "unknown"))), param.sched_priority);
	return 0;
}

int create_thread_with_priority(void *(*func)(void*), void *data)
{
	pthread_t id;
	pthread_attr_t attr;
	struct sched_param param;
	int res;

	pthread_attr_init (&attr);
	if ((res = pthread_attr_setschedpolicy (&attr, SCHED_FIFO)) != 0)
		printf("setschedpolicy: failure: %m\n");

	param.sched_priority = 30;
	if ((res = pthread_attr_setschedparam (&attr, &param)) != 0)
		printf("setschedparam: failure: %m\n");

	if ((res = pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED)) != 0)
		printf("setinheritsched: failure: %m\n");

	res = pthread_create (&id, &attr, (void *(*)(void *)) func, data);
	if (res != 0) {
		printf("pthread create faild: %m\n");
	}
	return 0;
}
