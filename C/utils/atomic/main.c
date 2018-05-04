#ifdef __STDC_NO_ATOMICS__ // since gcc 4.9
#error "Do not support C11 atomic"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>

#define N_THREADS 8
#define LOOP_NUM 10000

typedef struct _job {
	atomic_int *pval;
	int *p_normal;
	int id;
} job;

static void *worker(void *arg)
{
	job *p_job = arg;
	if (p_job->id & 0x1) {
		for (int i = 0; i < LOOP_NUM; i++) {
			usleep(100);
			atomic_fetch_sub(p_job->pval, 1);
			*(p_job->p_normal) = *(p_job->p_normal) - 1;
		}
	} else {
		for (int i = 0; i < LOOP_NUM; i++) {
			usleep(100);
			atomic_fetch_add(p_job->pval, 1);
			*(p_job->p_normal) = *(p_job->p_normal) + 1;
		}
	}

    return NULL;
}

int main(int argc, char**argv)
{
	pthread_t threads[N_THREADS];
	job jobs[N_THREADS];
	atomic_int val = ATOMIC_VAR_INIT(0);
	int normal_val = 0;

    for (int i = 0; i < N_THREADS; i++) {
		job *p_job = &jobs[i];
		p_job->pval = &val;
		p_job->p_normal = &normal_val;
		p_job->id = i;
		pthread_create(threads + i, NULL, worker, p_job);
	}

	for (int i = 0; i < N_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("normal: %d\n", normal_val);
	if (atomic_load(&val) != 0) {
		printf("test fail !!!\n");
	} else {
		printf("test pass.\n");
	}

	return 0;
}
