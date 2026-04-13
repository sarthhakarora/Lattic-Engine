#pragma once

#include "../scriptdb.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"

typedef enum {
  PLATFORM_MSG_OK,
  PLATFORM_MSG_OK_CANCEL,
  PLATFORM_MSG_YES_NO,
  PLATFORM_MSG_YES_NO_CANCEL,
  PLATFORM_MSG_RETRY_CANCEL,

  PLATFORM_ICON_INFO,
  PLATFORM_ICON_WARNING,
  PLATFORM_ICON_ERROR,
  PLATFORM_ICON_QUESTION,

  PLATFORM_SYSTEM_MODAL
} PlatformMessageType;

bool platfrom_open_file(char *out_path, size_t max_len);
void platform_throw_error(char *message, char *title,
                          PlatformMessageType msg_type);
void platform_throw_error_without_exit(char *message, char *title,
                                       PlatformMessageType msg_type);
ScriptList scan_scripts(const char *directory);
