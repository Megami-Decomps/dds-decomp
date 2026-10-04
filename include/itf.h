#ifndef ITF_H
#define ITF_H

#include "common.h"

typedef struct SdfAllocation SdfAllocation;

/* Common draw record allocated by the interface object constructors (0x40). */
typedef struct UiSprite {
    SdfAllocation *allocation;
    SdfAllocation *payloadAllocation;
    u32 *payload;
    u8 pad0C[4];
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 unk20;
    s32 screenY;
    u8 pad28[0x14];
    s8 kind;
    u8 pad3D[3];
} UiSprite;

/* Grid render entries retain a saved per-vertex palette for temporary tinting. */
typedef struct GridRenderEntry {
    u8 pad00[0x14];
    u32 colors[4];       /* 0x14 */
    u8 pad24[0x48];
    s32 quantizedBounds[4]; /* 0x6C: x, y, width, height */
    u8 pad7C[8];
    u32 savedColors[4];  /* 0x84 */
    u8 pad94[0xC];
} GridRenderEntry;       /* 0xA0 */

typedef struct GridEntryStorage {
    u8 pad00[0x10];
    u8 *quantizedEntries; /* 0x10: 0x80 bytes per entry */
    u8 pad14[4];
    GridRenderEntry *renderEntries;
} GridEntryStorage;

#endif /* ITF_H */
