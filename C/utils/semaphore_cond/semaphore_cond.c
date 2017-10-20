#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "semaphore_cond.h"

typedef struct SemaphoreRec
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    uint32_t count; /* the sem value (not num of waiting threads) */
} Semaphore;

int SemaphoreCreate(SemaphoreHandle *Sem, uint32_t Cnt)
{
    Semaphore *s;

    s = malloc(sizeof(Semaphore));
    if (!s)
        return -ENOMEM;

    (void)pthread_mutex_init( &s->mutex, 0 );
    (void)pthread_cond_init( &s->cond, 0 );
    s->count = Cnt;

    *Sem = s;
    return 0;
}

int SemaphoreWaitTimeout(Semaphore *s, uint32_t msec)
{
    int ret = 0;

    pthread_mutex_lock( &s->mutex );

    if( s->count > 0 ) {
        goto wait_end;
    }

    if( !msec ) {
        ret = -ETIMEDOUT;
        goto wait_end;
    } else {
        struct timeval curTime;
        struct timespec timeoutVal;
        uint32_t  timeout_sec;
        uint32_t  timeout_nsec;
        int error;

        timeout_sec = msec / 1000;
        /* Timeout values greater than 18 hours would overflow */
        timeout_nsec = msec * 1000 * 1000 - (timeout_sec * 1000000000);
        if( gettimeofday(&curTime, NULL) ) {
            ret = -EOPNOTSUPP;
            goto wait_end;
        }

        timeoutVal.tv_sec = curTime.tv_sec + timeout_sec;
        timeoutVal.tv_nsec = (curTime.tv_usec * 1000) + timeout_nsec;

        error = pthread_cond_timedwait( &s->cond, &s->mutex, &timeoutVal );
        if( error ) {
            if( error == ETIMEDOUT || error == EINTR ) {
                ret = -ETIMEDOUT;
            } else {
                ret = -EOPNOTSUPP;
            }
        }
    }

wait_end:
    if(ret == 0) {
        s->count--;
    }

    pthread_mutex_unlock( &s->mutex );
    return ret;
}

void SemaphoreWait(Semaphore *s)
{
    pthread_mutex_lock( &s->mutex );

    while( !s->count ) {
        pthread_cond_wait( &s->cond, &s->mutex );
    }

    s->count--;

    pthread_mutex_unlock( &s->mutex );
}

void SemaphoreSignal(SemaphoreHandle s)
{
    pthread_mutex_lock( &s->mutex );
    s->count++;
    pthread_cond_signal( &s->cond );
    pthread_mutex_unlock( &s->mutex );
}

void SemaphoreDestroy(SemaphoreHandle s)
{
    (void)pthread_mutex_destroy( &s->mutex );
    (void)pthread_cond_destroy( &s->cond );

    free(s);
}
