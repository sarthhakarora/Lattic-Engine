#pragma once
#include <stdbool.h>

// Thread
typedef struct PlatformThread PlatformThread;
typedef void (*PlatformThreadFn)(void *arg);

PlatformThread *thread_create(PlatformThreadFn fn, void *arg);
void thread_join(PlatformThread *t);
void thread_destroy(PlatformThread *t);

// Mutex
typedef struct PlatformMutex PlatformMutex;

PlatformMutex *mutex_create(void);
void mutex_lock(PlatformMutex *m);
void mutex_unlock(PlatformMutex *m);
void mutex_destroy(PlatformMutex *m);

// Sleep
void thread_sleep(int ms);
