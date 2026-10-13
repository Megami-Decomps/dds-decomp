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

/* Event-light init block copied into the retained event owner (0x30 bytes). */
typedef struct EffEventInit {
    f32 position[4];
    f32 orientation[4];
    f32 range;
    f32 height;
    f32 param;
    u32 color;
} EffEventInit;

typedef char EffEventInit_size_must_be_0x30[(sizeof(EffEventInit) == 0x30) ? 1 : -1];
typedef char EffEventInit_orientation_at_0x10[((u32)&((EffEventInit *)0)->orientation == 0x10) ? 1 : -1];
typedef char EffEventInit_range_at_0x20[((u32)&((EffEventInit *)0)->range == 0x20) ? 1 : -1];
typedef char EffEventInit_color_at_0x2C[((u32)&((EffEventInit *)0)->color == 0x2C) ? 1 : -1];

/* Copied from the per-title event-node template after its vector pair is
 * replaced by caller inputs; the trailing parameters and color are preserved. */
typedef struct EffectEventVectorParameters {
    f32 firstVector[4];
    f32 secondVector[4];
    f32 parameters[3];
    u32 color;
} EffectEventVectorParameters;

typedef char EffectEventVectorParameters_size_must_be_0x30[
    (sizeof(EffectEventVectorParameters) == 0x30) ? 1 : -1];
typedef char EffectEventVectorParameters_secondVector_at_0x10[
    ((u32)&((EffectEventVectorParameters *)0)->secondVector == 0x10) ? 1 : -1];
typedef char EffectEventVectorParameters_parameters_at_0x20[
    ((u32)&((EffectEventVectorParameters *)0)->parameters == 0x20) ? 1 : -1];
typedef char EffectEventVectorParameters_color_at_0x2C[
    ((u32)&((EffectEventVectorParameters *)0)->color == 0x2C) ? 1 : -1];

/* The event constructor copies a serialized, caller-specific 0x30-byte
 * parameter record. Its retained work owner is opaque to external users. */
EffEventWork *effEventCreate(struct SoundMixer *owner, u16 kind, const void *params);
/* Copy a caller-built 0x30-byte record into the event's initialization prefix. */
void effEventCopyFileRecordHeader(EffEventWork *destination, const void *source);
void effEventReleaseNode(EffEventWork *work);
void effEventSetScale(EffEventWork *work, f32 scale);
void effEventUpdateEffectParameters(EffEventWork *work);

ObjBase *dds3GetResourceOwnerHandle(EffWorldNode *object);

#endif
