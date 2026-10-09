#ifndef SDF_PACKET_PATCH_H
#define SDF_PACKET_PATCH_H

#include "common.h"

/* The frame worker passes the node and queued buffer index. The registered
 * handlers project the node into their distinct complete packet owners. */
typedef void (*SdfPacketPatchCallback)();

typedef struct SdfPacketPatchLink {
    struct SdfPacketPatchLink *next;
    SdfPacketPatchCallback patch;
} SdfPacketPatchLink;

typedef char SdfPacketPatchLink_size_must_be_8[
    (sizeof(SdfPacketPatchLink) == 8) ? 1 : -1];
typedef char SdfPacketPatchLink_patch_at_4[
    ((u32)&((SdfPacketPatchLink *)0)->patch == 4) ? 1 : -1];

#endif /* SDF_PACKET_PATCH_H */
