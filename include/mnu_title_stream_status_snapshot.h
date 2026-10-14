#ifndef MNU_TITLE_STREAM_STATUS_SNAPSHOT_H
#define MNU_TITLE_STREAM_STATUS_SNAPSHOT_H

#include "common.h"

typedef struct TitleAudioStreamStatusSnapshot {
    u32 frameCount;
    u32 frameIndex;
    u32 repeatFrame;
} TitleAudioStreamStatusSnapshot;

typedef char TitleAudioStreamStatusSnapshot_size[
    (sizeof(TitleAudioStreamStatusSnapshot) == 0x0C) ? 1 : -1];
typedef char TitleAudioStreamStatusSnapshot_frameCount_offset[
    ((u32)&((TitleAudioStreamStatusSnapshot *)0)->frameCount == 0x00) ? 1 : -1];
typedef char TitleAudioStreamStatusSnapshot_frameIndex_offset[
    ((u32)&((TitleAudioStreamStatusSnapshot *)0)->frameIndex == 0x04) ? 1 : -1];
typedef char TitleAudioStreamStatusSnapshot_repeatFrame_offset[
    ((u32)&((TitleAudioStreamStatusSnapshot *)0)->repeatFrame == 0x08) ? 1 : -1];

#endif
