#ifndef FLD_MIX_LB_H
#define FLD_MIX_LB_H

#include "common.h"

/* fldmix.LB's node list is shared by the DDS1 and DDS2 field loaders. */
typedef struct FldMixLbNodeView {
    struct FldMixLbNodeView *next; /* 0x00 */
    u8 unk04[4];               /* 0x04: meaning not established */
    s32 resourceHandle;        /* 0x08: passed to sdfMemoryGetBlockAddress */
} FldMixLbNodeView;

/* Observed prefix of the queued file request; bytes before the list stay opaque. */
typedef struct FldMixLbHeaderView {
    u8 unk00[0x60];
    FldMixLbNodeView *nodes; /* 0x60 */
} FldMixLbHeaderView;

#endif
