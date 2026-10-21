#ifndef BTL_BATTLE_WORK_H
#define BTL_BATTLE_WORK_H

#include "btl_state.h"

typedef struct BattleSub {
    union {
        s32 task;
        f32 scale;
        struct {
            u8 pad0[2];
            s8 active;
        } b;
    };
    union {
        s32 targetMode;
        BtlUnit *targetActor;
    };
    s8 b8;
    u8 pad09;
    s8 alternateFormation;
    u8 pad0B;
    s8 controlEnabled;
} BattleSub;

typedef struct BattleWork {
    u8 pad0[0x1E4];
    s16 scriptGroup; /* 0x1E4: selects the script resource path */
    u8 pad1E6[0x22];
    s32 soundTaskBase; /* 0x208: offset +6 selects the stationed sound task */
    s32 resourceHandle; /* 0x20C */
    u8 pad210[8];
    u32 resourceFlags; /* 0x218 */
    u8 pad21C[0x10];
    s32 state22C; /* A value of 5 blocks the world-effect helpers. */
    u8 pad230[0x18];
    struct ActionStateLink *actionActors; /* 0x248: linked action handles */
    BtlUnit *actorList;
    u8 pad250[0x18];
    u16 unk268;
    u8 pad26A[2];
    u16 phase;
    u8 pad26E[2];
    u16 phaseControl; /* 0x270: required phase in btlCheckPhaseOnePartyStatusGate */
    u8 pad272[2];
    s32 turnCount;
    u8 pad278[4];
    u8 encounterMode; /* 0x27C */
    u8 pad27D[0x23];
    s32 mode;
    u8 pad2A4[0x474];
    BattleSub *sub; /* 0x718: reused as BtlSelectCtrl in unit-selection modes. */
} BattleWork;

typedef char BattleSubSizeCheck[(sizeof(BattleSub) == 0x10) ? 1 : -1];
typedef char BattleWorkSizeCheck[(sizeof(BattleWork) == 0x71C) ? 1 : -1];
typedef char BattleWorkActionActorsOffsetCheck[
    ((unsigned long)&((BattleWork *)0)->actionActors == 0x248) ? 1 : -1];
typedef char BattleWorkSubOffsetCheck[
    ((unsigned long)&((BattleWork *)0)->sub == 0x718) ? 1 : -1];

#endif
