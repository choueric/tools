#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct _ThreadQueue ThreadQueue;

ThreadQueue *NewThreadQueue(void);
void DeleteThreadQueue(ThreadQueue *q);

void PushThreadQueue(ThreadQueue *q, void *p);

// unit of timeout is ms
void *PullThreadQueue(ThreadQueue *q, uint32_t timeout);

#if defined(__cplusplus)
}
#endif
