#ifndef BTL_COMMAND_LAYOUT_WORK_H
#define BTL_COMMAND_LAYOUT_WORK_H

#include "common.h"

/* Two-row command-option geometry read by the native option-scene updater and
 * renderer. The trailing allocation bytes remain opaque. */
typedef struct BattleCommandLayoutWork {
    s32 state;
    s32 positions[2][4];
    s32 extents[2][2];
    s16 selectedPositions[4];
    s32 dimensions[2][4];
    s32 fixedDimensions[2][4];
    u8 pad7C[8];
} BattleCommandLayoutWork;

typedef char BattleCommandLayoutWorkSizeCheck[
    (sizeof(BattleCommandLayoutWork) == 0x84) ? 1 : -1];
typedef char BattleCommandLayoutWorkPositionsOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->positions == 0x04) ? 1 : -1];
typedef char BattleCommandLayoutWorkExtentsOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->extents == 0x24) ? 1 : -1];
typedef char BattleCommandLayoutWorkSelectedPositionsOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->selectedPositions == 0x34) ? 1 : -1];
typedef char BattleCommandLayoutWorkDimensionsOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->dimensions == 0x3C) ? 1 : -1];
typedef char BattleCommandLayoutWorkFixedDimensionsOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->fixedDimensions == 0x5C) ? 1 : -1];
typedef char BattleCommandLayoutWorkOpaqueTailOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->pad7C == 0x7C) ? 1 : -1];

#endif
