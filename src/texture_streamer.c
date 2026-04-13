#include <string.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#define NOUSER
#include "process.h"
#include "stdio.h"
#include "texture_streamer.h"
#include "windows.h"
#include <stdbool.h>

static HANDLE workerThread;
static CRITICAL_SECTION lock;
static bool running;

static const char **jobPaths;
static int jobCount;
static int jobIndex;

static TextureStreamJob results[256];
static int resultCount;

unsigned __stdcall worker_proc(void *arg);

void loader_init(void) {
  InitializeCriticalSection(&lock);
  running = true;
  workerThread = (HANDLE)_beginthreadex(NULL, 0, worker_proc, NULL, 0, NULL);
}

unsigned __stdcall worker_proc(void *arg) {
  while (running) {

    const char *bufferimg = NULL;
    EnterCriticalSection(&lock);
    bool isJobAvi = (jobIndex < jobCount) ? true : false;

    int currentIndex = jobIndex;
    char local_path[256];
    if (isJobAvi) {
      strncpy(local_path, jobPaths[jobIndex], 255);

      local_path[255] = '\0';
      jobIndex++;
    }

    LeaveCriticalSection(&lock);

    if (isJobAvi) {
      results[currentIndex].img =
          LoadImage(local_path); // jobIndex-- gives current index
      results[currentIndex].imageLoaded = true;
      if (results[currentIndex].img.data == NULL) {
        results[currentIndex].failed = true;
      } else {
        EnterCriticalSection(&lock);
        resultCount++;
        LeaveCriticalSection(&lock);
      }
    } else {
      Sleep(1);
    }
  }

  return 0;
}

void loader_start(const char **paths, int count) {
  EnterCriticalSection(&lock);
  jobPaths = paths;
  jobCount = count;
  jobIndex = 0;
  LeaveCriticalSection(&lock);
}

bool loader_poll(Image *outImage, const char **outPath) {
  EnterCriticalSection(&lock);
  if (resultCount > 0) {
    *outImage = results[0].img;
    *outPath = results[0].path;
  } else {
    LeaveCriticalSection(&lock);
    return false;
  }
  resultCount--;
  for (int i = 0; i < resultCount; i++) {
    results[i] = results[i + 1];
  }
  LeaveCriticalSection(&lock);
  return true;
}

bool loader_is_done(void) {
  if (jobIndex >= jobCount && resultCount == 0) {
    return true;
  }
  return false;
}

void loader_shutdown(void) {
  running = false;
  WaitForSingleObject(workerThread, INFINITE);
  CloseHandle(workerThread);
  DeleteCriticalSection(&lock);
}
