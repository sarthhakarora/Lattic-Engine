#include "texture_streamer.h"
#include "platform/platform_win32_threading.h"
#include "stdio.h"

#include <stdbool.h>
#include <string.h>

static PlatformThread *workerThread;
static PlatformMutex *lock;
static bool running;

static const char **jobPaths;
static int jobCount;
static int jobIndex;

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

    mutex_lock(lock);
    bool isJobAvi = (jobIndex < jobCount);

    int currentIndex = jobIndex;
    char local_path[256];

    if (isJobAvi) {
      strncpy(local_path, jobPaths[currentIndex], 255);
      local_path[255] = '\0';

      strncpy(results[currentIndex].path, local_path, 255);
      results[currentIndex].path[255] = '\0';

      jobIndex++;
    }

    mutex_unlock(lock);

    if (isJobAvi) {
      results[currentIndex].img = LoadImage(local_path);
      results[currentIndex].imageLoaded = true;

      if (results[currentIndex].img.data == NULL) {
        results[currentIndex].failed = true;
      } else {
        mutex_lock(lock);
        resultCount++;
        mutex_unlock(lock);
      }
    } else {
      thread_sleep(1);
    }
  }
}

void loader_start(const char **paths, int count) {
  mutex_lock(lock);
  jobPaths = paths;
  jobCount = count;
  jobIndex = 0;
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

bool loader_is_done(void) { return (jobIndex >= jobCount && resultCount == 0); }

void loader_shutdown(void) {
  running = false;
  thread_join(workerThread);
  thread_destroy(workerThread);
  mutex_destroy(lock);
}
