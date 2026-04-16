#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <process.h>
#include <stdlib.h>
#include <windows.h>

#include "platform_win32_threading.h"

// Thread struct
struct PlatformThread {
  HANDLE handle;
};

// internal wrapper
typedef struct {
  PlatformThreadFn fn;
  void *arg;
} ThreadStartData;

static unsigned __stdcall thread_entry(void *arg) {
  ThreadStartData *data = (ThreadStartData *)arg;

  data->fn(data->arg);

  free(data);
  return 0;
}

// Thread API
PlatformThread *thread_create(PlatformThreadFn fn, void *arg) {
  PlatformThread *t = malloc(sizeof(PlatformThread));

  ThreadStartData *data = malloc(sizeof(ThreadStartData));
  data->fn = fn;
  data->arg = arg;

  t->handle = (HANDLE)_beginthreadex(NULL, 0, thread_entry, data, 0, NULL);

  return t;
}

void thread_join(PlatformThread *t) {
  WaitForSingleObject(t->handle, INFINITE);
}

void thread_destroy(PlatformThread *t) {
  CloseHandle(t->handle);
  free(t);
}

// Mutex struct
struct PlatformMutex {
  CRITICAL_SECTION cs;
};

// Mutex API
PlatformMutex *mutex_create(void) {
  PlatformMutex *m = malloc(sizeof(PlatformMutex));
  InitializeCriticalSection(&m->cs);
  return m;
}

void mutex_lock(PlatformMutex *m) { EnterCriticalSection(&m->cs); }

void mutex_unlock(PlatformMutex *m) { LeaveCriticalSection(&m->cs); }

void mutex_destroy(PlatformMutex *m) {
  DeleteCriticalSection(&m->cs);
  free(m);
}

// Sleep
void thread_sleep(int ms) { Sleep(ms); }
