#ifndef EFF_NODE_DESCRIPTOR_H
#define EFF_NODE_DESCRIPTOR_H

#include "common.h"

/* Complete serialized effect-resource header. The payload's size and format
 * depend on type; this is not the separately allocated live EffNode.
 * Compatibility conversion examines the complete type and argument words,
 * while node creation consumes their low halfwords. */
typedef struct EffNodeDescriptor {
    u32 type;       /* 0x00 */
    u32 arg;        /* 0x04 */
    u32 pad08;      /* 0x08 */
    f32 version;   /* 0x0C */
    u8 payload[0]; /* 0x10: variable-length tagged payload */
} EffNodeDescriptor;

typedef char EffNodeDescriptor_header_size_must_be_16[
    (sizeof(EffNodeDescriptor) == 0x10) ? 1 : -1];

#endif
