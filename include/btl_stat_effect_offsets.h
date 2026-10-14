#ifndef BTL_STAT_EFFECT_OFFSETS_H
#define BTL_STAT_EFFECT_OFFSETS_H

typedef struct BattleStatScreenOffset {
    s32 x;
    s32 y;
} BattleStatScreenOffset;

typedef struct BattleStatEffectOffsets {
    BattleStatScreenOffset hp;
    BattleStatScreenOffset mp;
} BattleStatEffectOffsets;

typedef char BattleStatScreenOffset_size_must_be_8[
    (sizeof(BattleStatScreenOffset) == 8) ? 1 : -1];
typedef char BattleStatEffectOffsets_size_must_be_0x10[
    (sizeof(BattleStatEffectOffsets) == 0x10) ? 1 : -1];
typedef char BattleStatEffectOffsets_hp_offset_must_be_0[
    ((u32)&((BattleStatEffectOffsets *)0)->hp == 0) ? 1 : -1];
typedef char BattleStatEffectOffsets_mp_offset_must_be_8[
    ((u32)&((BattleStatEffectOffsets *)0)->mp == 8) ? 1 : -1];

#endif
