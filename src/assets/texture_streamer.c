#include "texture_streamer.h"
#include "../platform/platform_win32_threading.h"
#define MAX_JOBS 256
#define MAX_PATH_LEN 256
#include "stdio.h"

#include <stdbool.h>
#include <string.h>

static PlatformThread *workerThread;
static PlatformMutex *lock;
static bool running;

typedef struct {
    char path[MAX_PATH_LEN];
} LoadJob;

static LoadJob jobQueue[MAX_JOBS];
static int jobHead = 0;
static int jobTail = 0;

static TextureStreamJob results[256];
static int resultCount;

void worker_proc(void *arg);

void loader_init(void) {
  lock = mutex_create();
  running = true;
  workerThread = thread_create(worker_proc, NULL);
}

void worker_proc(void *arg) {
  while (running) {

    char local_path[MAX_PATH_LEN];
    bool hasJob = false;

    mutex_lock(lock);

    if (jobHead != jobTail) {
      strncpy(local_path, jobQueue[jobHead].path, MAX_PATH_LEN);
      local_path[MAX_PATH_LEN-1] = '\0';

      jobHead = (jobHead + 1) % MAX_JOBS;
      hasJob = true;

    }

    mutex_unlock(lock);

    if (hasJob) {
      Image img = LoadImage(local_path);

      mutex_lock(lock);

      if (resultCount < 256) {
        strncpy(results[resultCount].path, local_path, 255);
        results[resultCount].path[255] = '\0';

        results[resultCount].img = img;
        results[resultCount].imageLoaded = true;
        results[resultCount].failed = (img.data == NULL);

        resultCount++;
      }

      mutex_unlock(lock);
    } else {
      thread_sleep(1);
    }
  }
}

void loader_start(const char **paths, int count) {
  mutex_lock(lock);
  for (int i = 0; i < count; i++) {
    int next = (jobTail + 1) % MAX_JOBS;

    if (next == jobHead) {
      break;
    }

    strncpy(jobQueue[jobTail].path, paths[i], 255);
    jobQueue[jobTail].path[255] = '\0';

    jobTail = next;
  }
  mutex_unlock(lock);
}

bool loader_poll(Image *outImage, char *outPath) {
  mutex_lock(lock);

  if (resultCount > 0) {
    *outImage = results[0].img;
    strcpy(outPath, results[0].path);
  } else {
    mutex_unlock(lock);
    return false;
  }

  resultCount--;

  for (int i = 0; i < resultCount; i++) {
    results[i] = results[i + 1];
  }

  mutex_unlock(lock);
  return true;
}

bool loader_is_done(void) {
  mutex_lock(lock);
  bool done = (jobHead == jobTail && resultCount == 0);
  mutex_unlock(lock);
  return done;
}

void loader_shutdown(void) {
  running = false;
  thread_join(workerThread);
  thread_destroy(workerThread);
  mutex_destroy(lock);
}
