#ifndef BTL_COMMAND_LAYOUT_WORK_H
#define BTL_COMMAND_LAYOUT_WORK_H

#include "common.h"

/* Two-row command-option geometry read by the native option-scene updater and
 * renderer, and the scroll arrows' animation state the option-scene updater
 * keeps beside it (its phase, the arrows' bounce and the backdrop's fade). */
typedef struct BattleCommandLayoutWork {
    s8 phase;           /* 0x00: read and written as a byte */
    u8 pad01[3];
    s32 positions[2][4];
    s32 extents[2][2];
    s16 selectedPositions[4];
    s32 dimensions[2][4];
    s32 fixedDimensions[2][4];
    s32 bounceFrame;    /* 0x7C */
    s16 backdropFade;   /* 0x80 */
    u8 pad82[2];
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
typedef char BattleCommandLayoutWorkBounceFrameOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->bounceFrame == 0x7C) ? 1 : -1];
typedef char BattleCommandLayoutWorkBackdropFadeOffsetCheck[
    ((u32)&((BattleCommandLayoutWork *)0)->backdropFade == 0x80) ? 1 : -1];

#endif
