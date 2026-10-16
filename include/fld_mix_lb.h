#ifndef FLD_MIX_LB_H
#define FLD_MIX_LB_H

#include "common.h"

struct SdfMemBlock;

/* fldmix.LB's node list is shared by the DDS1 and DDS2 field loaders. */
typedef struct FldMixLbNodeView {
    struct FldMixLbNodeView *next; /* 0x00 */
    u8 unk04[4];               /* 0x04: meaning not established */
    struct SdfMemBlock *resourceHandle; /* 0x08: allocation descriptor */
} FldMixLbNodeView;

typedef char FldMixLbNodeView_resourceHandle_offset[
    ((u32)&((FldMixLbNodeView *)0)->resourceHandle == 0x08) ? 1 : -1];
typedef char FldMixLbNodeView_size[
    (sizeof(FldMixLbNodeView) == 0x0C) ? 1 : -1];

/* Observed prefix of the queued file request; bytes before the list stay opaque. */
typedef struct FldMixLbHeaderView {
    u8 unk00[0x60];
    FldMixLbNodeView *nodes; /* 0x60 */
} FldMixLbHeaderView;

#endif
