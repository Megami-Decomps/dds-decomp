#ifndef MNU_TITLE_SOUND_BUFFER_STATUS_H
#define MNU_TITLE_SOUND_BUFFER_STATUS_H

#include "common.h"

/* Locked query result: unallocated, allocated with control == 2, or allocated
 * with another control value. */
typedef s32 MnuTitleSoundBufferStatus;
enum {
    MNU_TITLE_SOUND_BUFFER_UNALLOCATED = 0,
    MNU_TITLE_SOUND_BUFFER_INACTIVE = 2,
    MNU_TITLE_SOUND_BUFFER_ALLOCATED_OTHER_CONTROL = 3,
};
typedef char MnuTitleSoundBufferStatus_size_must_be_4[
    (sizeof(MnuTitleSoundBufferStatus) == 4) ? 1 : -1];

MnuTitleSoundBufferStatus mnuGetSoundBufferStateLocked(void);

#endif /* MNU_TITLE_SOUND_BUFFER_STATUS_H */
