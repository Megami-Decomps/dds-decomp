#ifndef PAR_KIND_RESOURCE_H
#define PAR_KIND_RESOURCE_H

#include "common.h"

/* Serialized kind-resource header; the record body starts at +0x10. */
typedef struct ParKindResource {
    s32 type;
    s32 offset;
    u8 opaque08[8];
} ParKindResource;

typedef char ParKindResource_size_must_be_0x10[
    sizeof(ParKindResource) == 0x10 ? 1 : -1];
typedef char ParKindResource_offset_at_0x04[
    ((u32)&((ParKindResource *)0)->offset == 0x04) ? 1 : -1];

#endif
