#ifndef BTL_COMMAND_HINT_WORK_H
#define BTL_COMMAND_HINT_WORK_H

#include "common.h"

/* The command-panel hint sprite allocation, shared by both titles. */
typedef struct BtlCommandHintWork {
    u8 pad00[0x10];
    s32 unk10;
    s16 fade;
    u8 pad16[2];
    u32 colors[4];
    u8 pad28[0x10];
    s32 x;
    s32 y;
} BtlCommandHintWork;

typedef char BtlCommandHintWorkSize[
    (sizeof(BtlCommandHintWork) == 0x40) ? 1 : -1];
typedef char BtlCommandHintWorkFadeOffset[
    ((unsigned long)&((BtlCommandHintWork *)0)->fade == 0x14) ? 1 : -1];
typedef char BtlCommandHintWorkColorsOffset[
    ((unsigned long)&((BtlCommandHintWork *)0)->colors == 0x18) ? 1 : -1];
typedef char BtlCommandHintWorkPositionOffset[
    ((unsigned long)&((BtlCommandHintWork *)0)->x == 0x38) ? 1 : -1];

#endif
