#ifndef BTL_STATUS_STYLE_H
#define BTL_STATUS_STYLE_H

#include "common.h"

/* One row of the status-effect style table: the effect started for the
 * status, the colour a unit is tinted with, the colour its model's release
 * restores, and whether the model's flag is set. The table has a row per
 * status button, the same in both games. */
typedef struct BtlStatusStyleEntry {
    u16 effectId;
    u16 pad02;
    u32 color;
    u32 releaseModelColor;
    u8 modelFlag;
    u8 pad0D[3];
} BtlStatusStyleEntry;

typedef char BtlStatusStyleEntrySizeCheck[(sizeof(BtlStatusStyleEntry) == 0x10) ? 1 : -1];

extern BtlStatusStyleEntry btlStatusEffectStyles[17];

#endif /* BTL_STATUS_STYLE_H */
