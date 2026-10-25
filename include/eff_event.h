#ifndef EFF_EVENT_H
#define EFF_EVENT_H

#include "dds3obj.h"

/* Kind-8 constructors allocate this complete 0x10-byte resource payload.
 * Only the owned resource pointer is initialized or consumed here. */
typedef struct DdsSlotResourceBlock {
    dds3ObjectBaseControlWorkData_t *resourceState;
    u8 pad04[0xC];
} DdsSlotResourceBlock;

typedef char DdsSlotResourceBlock_size_must_be_0x10[(sizeof(DdsSlotResourceBlock) == 0x10) ? 1 : -1];

struct SoundMixer;
typedef struct effEveBEDCutHandle_t effEveBEDCutHandle_t;

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

/* Identifies who releases the event handle shared by light-effect clones. */
typedef u8 EffSimpleBEDHandleOwnership;
#define EFF_SIMPLE_BED_HANDLE_BORROWED 0
#define EFF_SIMPLE_BED_HANDLE_OWNED 1

/* Per-title light-effect holder allocated by its create and clone paths. */
typedef struct effSimpleBED_t {
    struct SoundMixer *handle;
    effEveBEDCutHandle_t *owner;
    EffEventInit init;
    EffSimpleBEDHandleOwnership handleOwnership;
    u8 opaque39[3];
} effSimpleBED_t;

typedef char EffEventLight_size_must_be_0x3C[
    (sizeof(effSimpleBED_t) == 0x3C) ? 1 : -1];
typedef char EffEventLight_handle_offset_must_be_0x00[
    ((u32)&((effSimpleBED_t *)0)->handle == 0x00) ? 1 : -1];
typedef char EffEventLight_owner_offset_must_be_0x04[
    ((u32)&((effSimpleBED_t *)0)->owner == 0x04) ? 1 : -1];
typedef char EffEventLight_init_offset_must_be_0x08[
    ((u32)&((effSimpleBED_t *)0)->init == 0x08) ? 1 : -1];
typedef char EffEventLight_handleOwnership_offset_must_be_0x38[
    ((u32)&((effSimpleBED_t *)0)->handleOwnership == 0x38) ? 1 : -1];
typedef char EffEventLight_opaque39_offset_must_be_0x39[
    ((u32)&((effSimpleBED_t *)0)->opaque39 == 0x39) ? 1 : -1];

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
effEveBEDCutHandle_t *effEveBEDCutCreate(struct SoundMixer *owner, u16 kind, const void *params);
/* Copy a caller-built 0x30-byte record into the event's initialization prefix. */
void effEveBEDCutSetParams(effEveBEDCutHandle_t *destination, const void *source);
void effEveBEDCutDelete(effEveBEDCutHandle_t *work);
void effEveBEDCutSetScale(effEveBEDCutHandle_t *work, f32 scale);
void effEventUpdateEffectParameters(effEveBEDCutHandle_t *work);

dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(const dds3Basic_t *object);

#endif
