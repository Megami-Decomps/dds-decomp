#ifndef FILE_REQUEST_CALLBACK_WORK_H
#define FILE_REQUEST_CALLBACK_WORK_H

#include "common.h"

typedef struct FileRequestCallbackWork {
    u8 opaque00[3];
    u8 callbackMode;
    u8 opaque04[0x2C];
} FileRequestCallbackWork;

typedef char FileRequestCallbackWorkSizeCheck[
    (sizeof(FileRequestCallbackWork) == 0x30) ? 1 : -1];
typedef char FileRequestCallbackWorkModeOffsetCheck[
    ((u32)&((FileRequestCallbackWork *)0)->callbackMode == 0x03) ? 1 : -1];

#endif
