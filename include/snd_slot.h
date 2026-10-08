#ifndef SND_SLOT_H
#define SND_SLOT_H

#include "common.h"

/* Shared motion-SE cache, allocated as 0x108 bytes by DDS1 001F3E70 and
 * DDS2 00204B00. The work subobject follows the category/id key at +0x0C. */
typedef struct SoundSlotWork {
    u32 refCount;
    s32 pendingSoundId;
    s32 pendingSlot;
    s32 fileRequests[0x1D];
    s32 resourceHandles[0x1D];
} SoundSlotWork;

typedef struct SoundSlotOwner {
    u32 flags; /* 1 files queued, 2 files ready; 4 track pending, 8 loading, 0x10 ready. */
    s32 category;
    s32 id;
    SoundSlotWork work;
    struct SoundSlotOwner *prev;
    struct SoundSlotOwner *next;
} SoundSlotOwner;

typedef char SoundSlotWorkSizeCheck[(sizeof(SoundSlotWork) == 0xF4) ? 1 : -1];
typedef char SoundSlotOwnerSizeCheck[(sizeof(SoundSlotOwner) == 0x108) ? 1 : -1];
typedef char SoundSlotWorkOffsetCheck[((u32)&((SoundSlotOwner *)0)->work == 0x0C) ? 1 : -1];
typedef char SoundSlotRequestsOffsetCheck[((u32)&((SoundSlotOwner *)0)->work.fileRequests == 0x18) ? 1 : -1];
typedef char SoundSlotResourcesOffsetCheck[((u32)&((SoundSlotOwner *)0)->work.resourceHandles == 0x8C) ? 1 : -1];
typedef char SoundSlotPreviousOffsetCheck[((u32)&((SoundSlotOwner *)0)->prev == 0x100) ? 1 : -1];
typedef char SoundSlotNextOffsetCheck[((u32)&((SoundSlotOwner *)0)->next == 0x104) ? 1 : -1];

void sndReleaseSlotOwner(SoundSlotOwner *owner);

#endif
