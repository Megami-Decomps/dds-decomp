#ifndef DSP_ENTRY_H
#define DSP_ENTRY_H

#include "common.h"

/* A display list entry's unit: its unit ID and party level. */
typedef struct {
    u8 pad0[4];
    u16 unitId; /* 0x4: index into the unit-name and unit-sprite tables */
    u8 pad06[0xE];
    u16 level; /* 0x14: party level, displayed with the two-column level format */
} DspEntry;

#endif
