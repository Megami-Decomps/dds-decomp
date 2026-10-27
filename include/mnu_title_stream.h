#ifndef MNU_TITLE_STREAM_H
#define MNU_TITLE_STREAM_H

#include "common.h"

/* The value moves from idle through file loading and then IOP commit. */
typedef s32 MnuTitleStreamLoadState;

#define MNU_STREAM_LOAD_IDLE 0
#define MNU_STREAM_LOAD_PENDING 1
#define MNU_STREAM_LOAD_COPIED 2
#define MNU_STREAM_COMMIT_READY 3
#define MNU_STREAM_COMMIT_COMPLETE 4

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
    struct sdfMemHandle_t *allocation;
    MnuTitleStreamLoadState loadState;
} TitleAudioStreamState;
typedef char TitleAudioStreamState_size[(sizeof(TitleAudioStreamState) == 0x28) ? 1 : -1];
typedef char TitleAudioStreamState_loadState_offset[
    ((u32)&((TitleAudioStreamState *)0)->loadState == 0x24) ? 1 : -1];

/* Shared title-audio stream operations, implemented by each title. */
void mnuQueueTitleSoundStreamEntry(s32 soundEntryIndex);
MnuTitleStreamLoadState mnuPollTitleStreamStateLocked(void);
void mnuMarkTitleStreamResetPending(void);
void mnuResetTitleStreamLocked(void);

#endif /* MNU_TITLE_STREAM_H */
