#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Windows.h"
#include "commdlg.h"

#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"

#include "platform_win32.h"

bool platfrom_open_file(char *out_path, size_t max_len) {
    OPENFILENAMEA ofn = {0};
    out_path[0] = '\0';

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = out_path;
    ofn.nMaxFile = (DWORD)max_len;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    return GetOpenFileNameA(&ofn);
}

static UINT platform_message_flags(PlatformMessageType type)
{
    switch (type) {
        case PLATFORM_MSG_OK:            return MB_OK;
        case PLATFORM_MSG_OK_CANCEL:     return MB_OKCANCEL;
        case PLATFORM_MSG_YES_NO:        return MB_YESNO;
        case PLATFORM_MSG_YES_NO_CANCEL: return MB_YESNOCANCEL;
        case PLATFORM_MSG_RETRY_CANCEL:  return MB_RETRYCANCEL;

        case PLATFORM_ICON_INFO:         return MB_ICONINFORMATION;
        case PLATFORM_ICON_WARNING:      return MB_ICONWARNING;
        case PLATFORM_ICON_ERROR:        return MB_ICONERROR;
        case PLATFORM_ICON_QUESTION:     return MB_ICONQUESTION;

        case PLATFORM_SYSTEM_MODAL:      return MB_OK | MB_ICONERROR | MB_SYSTEMMODAL;
        default:                         return MB_OK;
    }
}

void platform_throw_error(char *message, char *title, PlatformMessageType msg_type) {
    MessageBoxA(
        NULL,
        message,
        title,
        platform_message_flags(msg_type)
    );
    exit(EXIT_FAILURE);
}
