#ifndef FLD_MAP_SELECTION_H
#define FLD_MAP_SELECTION_H

#include "common.h"

/* Shared local-map selection count view used by counter selection helpers. */
typedef struct MapSelection {
    u8 opaque00[0x0C];
    s32 selectedCount; /* 0x0C */
    u8 opaque10[0x10];
    s32 maxCount;      /* 0x20 */
} MapSelection;

typedef char MapSelection_size_must_be_24[(sizeof(MapSelection) == 0x24) ? 1 : -1];
typedef char MapSelection_selectedCount_offset_must_be_0C[
    ((u32)&((MapSelection *)0)->selectedCount == 0x0C) ? 1 : -1];
typedef char MapSelection_maxCount_offset_must_be_20[
    ((u32)&((MapSelection *)0)->maxCount == 0x20) ? 1 : -1];

#endif /* FLD_MAP_SELECTION_H */
