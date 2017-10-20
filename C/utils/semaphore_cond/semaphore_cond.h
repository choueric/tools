#pragma once

#include <stdint.h>

typedef struct SemaphoreRec *SemaphoreHandle;

/**
 * Creates a counting semaphore.
 *
 * @param semaphore A pointer to the semaphore to initialize.
 * @param value The initial semaphore value.
 *
 * @retval 0 If successful, or the appropriate error code.
 */
int SemaphoreCreate(SemaphoreHandle *semaphore, uint32_t value);

/** Waits until the semaphore value becomes non-zero, then
 *  decrements the value and returns.
 *
 *  @param semaphore The semaphore to wait for.
 */
void SemaphoreWait(SemaphoreHandle semaphore);

/**
 * Waits for the given semaphore value to become non-zero with timeout. If
 * the semaphore value becomes non-zero before the timeout, then the value is
 * decremented and 0 is returned.
 *
 * @param semaphore The semaphore to wait for.
 * @param msec Timeout value in milliseconds.
 *        WAIT_INFINITE(0xFFFFFFFF) can be used to wait forever.
 *
 * @retval int_Timeout If the wait expires.
 */
int SemaphoreWaitTimeout(SemaphoreHandle semaphore, uint32_t msec);

/** Increments the semaphore value.
 *
 *  @param semaphore The semaphore to signal.
 */
void SemaphoreSignal(SemaphoreHandle semaphore);

/** Frees resources held by the semaphore.
 *
 *  Semaphores are reference counted across the computer (multiproceses),
 *  and a given semaphore will not be destroyed until the last reference has
 *  gone away.
 *
 *  @param semaphore The semaphore to destroy.
 *      Passing in a null semaphore is okay (no op).
 */
void SemaphoreDestroy(SemaphoreHandle semaphore);
