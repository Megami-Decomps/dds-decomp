#ifndef SDF_PAD_ENTRY_H
#define SDF_PAD_ENTRY_H

#include "common.h"

typedef struct SdfPadEntry {
    u8 port;                 /* 0x00 */
    u8 slot;                 /* 0x01 */
    u8 state;                /* 0x02 */
    u8 opaque03;
    u8 mode;                 /* 0x04 */
    u8 requestedMode;        /* 0x05 */
    u16 buttons;             /* 0x06 */
    u16 prevButtons;         /* 0x08 */
    u8 opaque0A[2];
    u32 repeatDeadline;      /* 0x0C */
    u8 stick[4];             /* 0x10 */
    u8 pressure[12];         /* 0x14 */
    s16 smallMotor;          /* 0x20 */
    s16 largeMotor;          /* 0x22 */
    s16 lastSmallMotor;      /* 0x24 */
    s16 lastLargeMotor;      /* 0x26 */
} SdfPadEntry;

typedef char SdfPadEntry_size[(sizeof(SdfPadEntry) == 0x28) ? 1 : -1];

#endif
