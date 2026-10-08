#ifndef MNU_PROFILE_PROGRESS_H
#define MNU_PROFILE_PROGRESS_H

#include "common.h"

struct DatPartyRecord;

typedef struct MnuProfileProgress {
    struct DatPartyRecord *partyRecord;
    s32 profileId;
    u32 value;
    u32 cap;
} MnuProfileProgress;

typedef char MnuProfileProgressSizeCheck[sizeof(MnuProfileProgress) == 0x10 ? 1 : -1];
typedef char MnuProfileProgressPartyRecordOffsetCheck[((u32)&((MnuProfileProgress *)0)->partyRecord == 0x00) ? 1 : -1];
typedef char MnuProfileProgressProfileIdOffsetCheck[((u32)&((MnuProfileProgress *)0)->profileId == 0x04) ? 1 : -1];
typedef char MnuProfileProgressValueOffsetCheck[((u32)&((MnuProfileProgress *)0)->value == 0x08) ? 1 : -1];
typedef char MnuProfileProgressCapOffsetCheck[((u32)&((MnuProfileProgress *)0)->cap == 0x0C) ? 1 : -1];

#endif
