#ifndef EFF_EVENT_H
#define EFF_EVENT_H

#include "dds3obj.h"

/* Kind-8 constructors allocate this complete 0x10-byte resource payload.
 * Only the owned resource pointer is initialized or consumed here. */
typedef struct DdsSlotResourceBlock {
    ObjBase *resourceState;
    u8 pad04[0xC];
} DdsSlotResourceBlock;

typedef char DdsSlotResourceBlock_size_must_be_0x10[(sizeof(DdsSlotResourceBlock) == 0x10) ? 1 : -1];

struct SoundMixer;
typedef struct EffEventWork EffEventWork;

/* The event constructor copies a serialized, caller-specific 0x30-byte
 * parameter record. Its retained work owner is opaque to external users. */
EffEventWork *effEventCreate(struct SoundMixer *owner, u16 kind, const void *params);
void effEventReleaseNode(EffEventWork *work);
void effEventSetScale(EffEventWork *work, f32 scale);

ObjBase *dds3GetResourceOwnerHandle(EffWorldNode *object);

#endif
