#ifndef EFF_ACTIVE_RESOURCE_H
#define EFF_ACTIVE_RESOURCE_H

#include "common.h"

/* Per-instance header shared by every resource-kind callback. payload points
 * to kind-specific bytes immediately after this 0x40-byte allocation prefix;
 * resource is the separately owned kind-specific object. */
typedef struct EffActiveResource {
    f32 position[4];
    f32 orientation[4];
    f32 scale;
    u32 color;
    u32 frame;
    union {
        u32 index;
        s32 signedIndex;
        u16 shortIndex;
    } kind;
    void *resource;
    u8 pad_34[4];
    void *payload;
    u8 pad_3C[4];
} EffActiveResource;

typedef char EffActiveResource_size_must_be_0x40[
    (sizeof(EffActiveResource) == 0x40) ? 1 : -1];
typedef char EffActiveResource_orientation_offset_must_be_0x10[
    ((u32)&((EffActiveResource *)0)->orientation == 0x10) ? 1 : -1];
typedef char EffActiveResource_scale_offset_must_be_0x20[
    ((u32)&((EffActiveResource *)0)->scale == 0x20) ? 1 : -1];
typedef char EffActiveResource_kind_offset_must_be_0x2C[
    ((u32)&((EffActiveResource *)0)->kind == 0x2C) ? 1 : -1];
typedef char EffActiveResource_resource_offset_must_be_0x30[
    ((u32)&((EffActiveResource *)0)->resource == 0x30) ? 1 : -1];
typedef char EffActiveResource_payload_offset_must_be_0x38[
    ((u32)&((EffActiveResource *)0)->payload == 0x38) ? 1 : -1];

#endif /* EFF_ACTIVE_RESOURCE_H */
