#ifndef EFF_MODEL_OWNER_H
#define EFF_MODEL_OWNER_H

#include "common.h"

struct MdlCtx;
struct SdfLightingPacketStorage;

/* Heap-owned model wrapper allocated as a 0x10-byte chip block. */
typedef struct EffModelOwner {
    f32 scale;
    struct MdlCtx *model;
    struct SdfLightingPacketStorage *ownedBuffer;
    u32 flags;
} EffModelOwner;

typedef char EffModelOwner_size_check[
    (sizeof(EffModelOwner) == 0x10) ? 1 : -1];
typedef char EffModelOwner_model_offset_check[
    ((u32)&((EffModelOwner *)0)->model == 0x04) ? 1 : -1];
typedef char EffModelOwner_ownedBuffer_offset_check[
    ((u32)&((EffModelOwner *)0)->ownedBuffer == 0x08) ? 1 : -1];
typedef char EffModelOwner_flags_offset_check[
    ((u32)&((EffModelOwner *)0)->flags == 0x0C) ? 1 : -1];

#endif /* EFF_MODEL_OWNER_H */
