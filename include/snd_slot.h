#ifndef SND_SLOT_H
#define SND_SLOT_H

#include "common.h"

struct FileRequest;
struct SdfMemBlock;

/* Shared motion-SE cache, allocated as 0x108 bytes by DDS1 001F3E70 and
 * DDS2 00204B00. The work subobject follows the category/id key at +0x0C. */
typedef struct SoundSlotWork {
    u32 refCount;
    s32 pendingSoundId;
    s32 pendingSlot;
    struct FileRequest *fileRequests[0x1D];
    struct SdfMemBlock *resourceHandles[0x1D];
} SoundSlotWork;

/* SoundSlotOwner flag bits observed across file and packed-track loading. */
typedef enum SoundSlotOwnerFlag {
    SOUND_SLOT_FILE_LOAD_PENDING = 1,
    SOUND_SLOT_FILES_READY = 2,
    SOUND_SLOT_TRACK_LOAD_REQUESTED = 4,
    SOUND_SLOT_TRACK_LOADING = 8,
    SOUND_SLOT_TRACK_READY = 0x10
} SoundSlotOwnerFlag;

typedef struct SoundSlotOwner {
    u32 flags; /* SoundSlotOwnerFlag masks; other bits remain unclassified. */
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

SoundSlotOwner *sndFindListNodeForChannel(s32 category, s32 id);
void sndReleaseSlotOwner(SoundSlotOwner *owner);

#endif
