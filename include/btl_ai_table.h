#ifndef BTL_AI_TABLE_H
#define BTL_AI_TABLE_H

#include "common.h"

/* Weighted action entry in an enemy species' AI table. */
typedef struct AiSlot {
    u8 weight; /* 0x00: used by the weighted-slot selector. */
    u8 pad01;
    u16 actionId; /* 0x02 */
    u32 actionArg; /* 0x04 */
} AiSlot;

typedef char AiSlot_size_must_be_8[(sizeof(AiSlot) == 8) ? 1 : -1];
typedef char AiSlot_actionId_offset_must_be_2[
    ((u32)&((AiSlot *)0)->actionId == 2) ? 1 : -1];
typedef char AiSlot_actionArg_offset_must_be_4[
    ((u32)&((AiSlot *)0)->actionArg == 4) ? 1 : -1];

/* One AICALC.TBL decision tier: three packed predicates and eight routes. */
typedef struct AiDecisionRow {
    u32 predicates[3];
    u8 routes[8];
} AiDecisionRow;

typedef char AiDecisionRow_size_must_be_0x14[
    (sizeof(AiDecisionRow) == 0x14) ? 1 : -1];
typedef char AiDecisionRow_routes_offset_must_be_0x0C[
    ((u32)&((AiDecisionRow *)0)->routes == 0x0C) ? 1 : -1];

/* One species' decision tiers and weighted action slots; other bytes stay opaque. */
typedef struct AiSpecies {
    u8 unknown00[4]; /* 0x00: byte 0 is tested by the support/status check. */
    AiDecisionRow decisions[3]; /* 0x04 */
    AiSlot slot[25]; /* 0x40: five rows of five weighted actions. */
    u8 unknown108[0x54]; /* 0x108 */
} AiSpecies;

typedef char AiSpecies_size_must_be_0x15C[
    (sizeof(AiSpecies) == 0x15C) ? 1 : -1];
typedef char AiSpecies_unknown00_offset_must_be_0[
    ((u32)&((AiSpecies *)0)->unknown00 == 0x00) ? 1 : -1];
typedef char AiSpecies_decisions_offset_must_be_0x04[
    ((u32)&((AiSpecies *)0)->decisions == 0x04) ? 1 : -1];
typedef char AiSpecies_slot_offset_must_be_0x40[
    ((u32)&((AiSpecies *)0)->slot == 0x40) ? 1 : -1];
typedef char AiSpecies_unknown108_offset_must_be_0x108[
    ((u32)&((AiSpecies *)0)->unknown108 == 0x108) ? 1 : -1];

#endif /* BTL_AI_TABLE_H */
