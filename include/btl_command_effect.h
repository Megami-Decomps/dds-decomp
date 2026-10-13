#ifndef BTL_COMMAND_EFFECT_H
#define BTL_COMMAND_EFFECT_H

#include "common.h"

typedef struct BtlCommandEffect {
    s32 command;
    s16 effect;
    u8 adjustSide;
    u8 pad07;
} BtlCommandEffect;

typedef char BtlCommandEffect_size_must_be_0x08[
    (sizeof(BtlCommandEffect) == 0x08) ? 1 : -1];
typedef char BtlCommandEffect_command_offset_must_be_0x00[
    ((u32)&((BtlCommandEffect *)0)->command == 0x00) ? 1 : -1];
typedef char BtlCommandEffect_effect_offset_must_be_0x04[
    ((u32)&((BtlCommandEffect *)0)->effect == 0x04) ? 1 : -1];
typedef char BtlCommandEffect_adjustSide_offset_must_be_0x06[
    ((u32)&((BtlCommandEffect *)0)->adjustSide == 0x06) ? 1 : -1];

#endif
