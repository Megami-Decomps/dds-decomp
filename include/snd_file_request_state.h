#ifndef SND_FILE_REQUEST_STATE_H
#define SND_FILE_REQUEST_STATE_H

#include "common.h"

typedef struct SndFileRequestState {
    u8 path[0x100];
    s32 size;
    s32 pending;
    u32 data;
} SndFileRequestState;

#endif
