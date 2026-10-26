#ifndef MNU_TITLE_SOUND_H
#define MNU_TITLE_SOUND_H

#include "common.h"

/* The PCM voice owner stores idle as 0 and active playback as 1. */
typedef s32 MnuTitleSoundBusyState;
enum {
    MNU_TITLE_SOUND_IDLE = 0,
    MNU_TITLE_SOUND_BUSY = 1
};

MnuTitleSoundBusyState mnuQueryTitleSoundBusy(void);

/* Decoder format selectors used by the title sound buffer installer. */
typedef s32 MnuTitleSoundFormat;
enum {
    MNU_TITLE_SOUND_FORMAT_LARGE_FRAME = 1,
    MNU_TITLE_SOUND_FORMAT_MEDIUM_FRAME = 2,
    MNU_TITLE_SOUND_FORMAT_MEDIUM_FRAME_VARIANT = 3,
    MNU_TITLE_SOUND_FORMAT_SMALL_FRAME = 4
};

/* Copy compressed ATRAC data into the shared title-sound buffer. */
void mnuInstallTitleSoundBufferFromData(const void *compressedData, s32 dataBytes,
                                        MnuTitleSoundFormat format);

#endif /* MNU_TITLE_SOUND_H */
