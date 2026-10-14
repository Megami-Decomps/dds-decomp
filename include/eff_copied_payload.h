#ifndef EFF_COPIED_PAYLOAD_H
#define EFF_COPIED_PAYLOAD_H

#include "common.h"
#include "dds3_path.h"
#include "eff_transform.h"

struct SdfMemBlock;

/* Payload owner shared by the paired effect units. +0x3C remains opaque. */
typedef struct EffCopiedPayload {
    u8 *body;                       /* 0x00 */
    s32 size;                       /* 0x04 */
    u32 slotCount;                  /* 0x08: populated effect/target pairs */
    u32 selectedTargetIndex;        /* 0x0C: chosen target path slot */
    EffWorldNode *effects[5];      /* 0x10 */
    Dds3PathCurveWork *targets[5]; /* 0x24 */
    struct SdfMemBlock *allocation; /* 0x38 */
    u8 opaque3C[4];
} EffCopiedPayload;

typedef struct EffCopiedPayloadWork {
    u8 opaque00[0x30];
    EffCopiedPayload *payload; /* 0x30 */
    u8 opaque34[4];
    u32 parameter; /* 0x38 */
} EffCopiedPayloadWork;

typedef char EffCopiedPayload_size_check[
    (sizeof(EffCopiedPayload) == 0x40) ? 1 : -1];
typedef char EffCopiedPayload_body_offset_check[
    ((u32)&((EffCopiedPayload *)0)->body == 0x00) ? 1 : -1];
typedef char EffCopiedPayload_slotCount_offset_check[
    ((u32)&((EffCopiedPayload *)0)->slotCount == 0x08) ? 1 : -1];
typedef char EffCopiedPayload_selectedTargetIndex_offset_check[
    ((u32)&((EffCopiedPayload *)0)->selectedTargetIndex == 0x0C) ? 1 : -1];
typedef char EffCopiedPayload_effects_offset_check[
    ((u32)&((EffCopiedPayload *)0)->effects == 0x10) ? 1 : -1];
typedef char EffCopiedPayload_targets_offset_check[
    ((u32)&((EffCopiedPayload *)0)->targets == 0x24) ? 1 : -1];
typedef char EffCopiedPayload_allocation_offset_check[
    ((u32)&((EffCopiedPayload *)0)->allocation == 0x38) ? 1 : -1];
typedef char EffCopiedPayloadWork_size_check[
    (sizeof(EffCopiedPayloadWork) == 0x3C) ? 1 : -1];
typedef char EffCopiedPayloadWork_payload_offset_check[
    ((u32)&((EffCopiedPayloadWork *)0)->payload == 0x30) ? 1 : -1];
typedef char EffCopiedPayloadWork_parameter_offset_check[
    ((u32)&((EffCopiedPayloadWork *)0)->parameter == 0x38) ? 1 : -1];

#endif /* EFF_COPIED_PAYLOAD_H */
