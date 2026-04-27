#pragma once
#include "stdbool.h"

typedef enum {
  LOG_ENGINE,
  LOG_LUA,
  LOG_ASSET
} LogType;

typedef enum {
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR,
} LogLevel;

void log_msg(LogType type, LogLevel level, const char *fmt, ...);

extern bool log_show_engine;
extern bool log_show_lua;
extern bool log_show_alert;
