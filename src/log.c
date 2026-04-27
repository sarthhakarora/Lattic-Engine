#include "stdio.h"
#include "stdarg.h"
#include "log.h"

void log_msg(LogType type, LogLevel level, const char *fmt, ...) {
    const char* typebuf = "";
    const char* levelbuf = "";

    switch (type) {
    case LOG_LUA:
      typebuf = "[LUA]";
      break;
    case LOG_ENGINE:
      typebuf = "[ENGINE]";
      break;
    case LOG_ASSET:
      typebuf = "[ASSET]";
      break;
    default:
      typebuf = "[UNKNOWN]";
    }

    switch (level) {
    case LOG_LEVEL_WARN:
      levelbuf = "[WARN]";
      break;
    case LOG_LEVEL_ERROR:
      levelbuf = "[ERROR]";
      break;
    case LOG_LEVEL_INFO:
      levelbuf = "[INFO]";
      break;
    default:
      levelbuf = "[UNKNOWN]";
    }

    printf("%s %s: ", typebuf, levelbuf);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");

}
