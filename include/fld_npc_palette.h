#ifndef FLD_NPC_PALETTE_H
#define FLD_NPC_PALETTE_H

#include "common.h"

typedef struct FldNpcPaletteEntry {
    u8 colorA[3];
    u8 colorB[3];
    u8 textPair;
    u8 phase;
} FldNpcPaletteEntry;

typedef struct FldNpcPalette {
    FldNpcPaletteEntry entries[0x40];
} FldNpcPalette;

typedef char FldNpcPaletteEntry_size_check[
    (sizeof(FldNpcPaletteEntry) == 8 &&
     (u32)&((FldNpcPaletteEntry *)0)->colorA == 0 &&
     (u32)&((FldNpcPaletteEntry *)0)->colorB == 3 &&
     (u32)&((FldNpcPaletteEntry *)0)->textPair == 6 &&
     (u32)&((FldNpcPaletteEntry *)0)->phase == 7) ? 1 : -1];
typedef char FldNpcPalette_size_check[
    (sizeof(FldNpcPalette) == 0x200 &&
     (u32)&((FldNpcPalette *)0)->entries == 0) ? 1 : -1];

void fldSetNpcPalette(const FldNpcPalette *source);

#endif /* FLD_NPC_PALETTE_H */
