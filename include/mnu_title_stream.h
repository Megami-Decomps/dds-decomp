#ifndef MNU_TITLE_STREAM_H
#define MNU_TITLE_STREAM_H

#include "common.h"

/* The two title-audio states own the complete ten-word frame/decoder record. */
typedef struct TitleAudioStreamState {
    s32 frameCount;
    s32 frameIndex;
    s32 frameBytes;
    s32 repeatFrame;
    s32 control;
    u8 *compressedData;
    s16 *samples;
    u32 *decoder;
    struct SdfMemBlock *allocation;
    s32 loadState;
} TitleAudioStreamState;
typedef char TitleAudioStreamState_size[(sizeof(TitleAudioStreamState) == 0x28) ? 1 : -1];

#endif /* MNU_TITLE_STREAM_H */
