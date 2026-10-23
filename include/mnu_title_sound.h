#ifndef MNU_TITLE_SOUND_H
#define MNU_TITLE_SOUND_H

#include "common.h"

/* Copy compressed ATRAC data into the shared title-sound buffer. */
void mnuInstallTitleSoundBufferFromData(const void *compressedData, s32 dataBytes, s32 format);

#endif /* MNU_TITLE_SOUND_H */
