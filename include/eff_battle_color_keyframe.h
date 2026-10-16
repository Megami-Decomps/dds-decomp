#ifndef EFF_BATTLE_COLOR_KEYFRAME_H
#define EFF_BATTLE_COLOR_KEYFRAME_H

#include "eff.h"

typedef struct EffBattleUnitRgbCommand {
    u32 color;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitRgbCommand;

typedef struct EffBattleUnitColorCommand {
    EffectVectorRequest request;
    u32 firstColor;
    u32 secondColor;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitColorCommand;

typedef char EffBattleUnitRgbCommandSizeCheck[(sizeof(EffBattleUnitRgbCommand) == 0x14) ? 1 : -1];
typedef char EffBattleUnitRgbCommandStartFrameOffsetCheck[
    ((u32)&((EffBattleUnitRgbCommand *)0)->startFrame == 0x04) ? 1 : -1];
typedef char EffBattleUnitRgbCommandStartDurationOffsetCheck[
    ((u32)&((EffBattleUnitRgbCommand *)0)->startDurationIndex == 0x08) ? 1 : -1];
typedef char EffBattleUnitRgbCommandEndFrameOffsetCheck[
    ((u32)&((EffBattleUnitRgbCommand *)0)->endFrame == 0x0C) ? 1 : -1];
typedef char EffBattleUnitRgbCommandEndDurationOffsetCheck[
    ((u32)&((EffBattleUnitRgbCommand *)0)->endDurationIndex == 0x10) ? 1 : -1];

typedef char EffBattleUnitColorCommandFirstColorOffsetCheck[
    ((u32)&((EffBattleUnitColorCommand *)0)->firstColor == 0x08) ? 1 : -1];
typedef char EffBattleUnitColorCommandSecondColorOffsetCheck[
    ((u32)&((EffBattleUnitColorCommand *)0)->secondColor == 0x0C) ? 1 : -1];
typedef char EffBattleUnitColorCommandStartFrameOffsetCheck[
    ((u32)&((EffBattleUnitColorCommand *)0)->startFrame == 0x10) ? 1 : -1];
typedef char EffBattleUnitColorCommandStartDurationOffsetCheck[
    ((u32)&((EffBattleUnitColorCommand *)0)->startDurationIndex == 0x14) ? 1 : -1];
typedef char EffBattleUnitColorCommandEndFrameOffsetCheck[
    ((u32)&((EffBattleUnitColorCommand *)0)->endFrame == 0x18) ? 1 : -1];
typedef char EffBattleUnitColorCommandEndDurationOffsetCheck[
    ((u32)&((EffBattleUnitColorCommand *)0)->endDurationIndex == 0x1C) ? 1 : -1];
typedef char EffBattleUnitColorCommandSizeCheck[(sizeof(EffBattleUnitColorCommand) == 0x20) ? 1 : -1];

#endif
