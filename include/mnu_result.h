#ifndef MNU_RESULT_H
#define MNU_RESULT_H

#include "mnu.h"
#include "dat_state.h"

typedef struct MenuIconRef {
    u16 id;
    u8 param;
    u8 pad3;
} MenuIconRef;

/* Writers 001A1530/001AA400: icons, macca, EXP, AP and five hunt-AP bonuses. */
typedef struct BrsRewardSummary {
    MenuIconRef icons[3];
    u32 macca;
    s32 totalExp;
    s32 totalAp;
    s32 unitApBonus[5];
#ifdef VERSION_DDS2
    u16 mitama;
    u8 pad2E[2];
#endif
} BrsRewardSummary;

/* The second value is EXP for reward rows and a party index for level-up rows. */
typedef struct BrsRewardValues {
    s32 amount;
    s32 secondaryValue;
    u32 profileValue;
    u32 totalExp;
    s32 partySlot;
} BrsRewardValues;

typedef struct BrsRewardRow {
    DatPartyRecord *unit;
    BrsRewardValues values;
} BrsRewardRow;

typedef struct BrsRewardBatch {
    BrsRewardRow rows[5];
    s32 count;
} BrsRewardBatch;

typedef struct BrsActiveProgressList {
    BrsProgressRow rows[5];
    u32 count;
} BrsActiveProgressList;

typedef struct BrsResultTransition {
    u8 data[0x4C];
    s32 state;
} BrsResultTransition;

/* Allocation/zeroing: DDS1 00262684/0026269C; DDS2 0029959C/002995B4. */
typedef struct BrsSkillPackageWork {
    s32 handle;
    u32 overlayFlags;
    BrsResultTransition transition;
    s32 fadeTarget;
    BrsRewardSummary rewards;
#ifdef VERSION_DDS1
    u8 pad88[8];
    s32 unitHandle;
    u8 pad94[4];
    BrsRewardRow *selectedRewardRow;
#else
    u8 pad8C[8];
    s32 unitHandle;
    u8 pad98[4];
    BrsRewardRow *selectedRewardRow;
#endif
    DatPartyRecord previewUnit;
    s32 selectedRow;
    s32 pendingSkillCount;
    s32 pendingSkillIndex;
    s32 selectionApplied;
    BrsRewardBatch secondaryRewards;
    BrsRewardBatch primaryRewards;
    BrsRewardBatch rewardState;
    u32 resetStateB;
    s32 availableStatPoints;
    s32 assignedStatPoints;
    s32 statGains[5];
    BrsActiveProgressList partyProgress;
    PrfSkillList skillList;
#ifdef VERSION_DDS1
    s32 group[2];
    s32 spriteArg0;
    u8 pad504[8];
    s32 spriteArg1;
    u8 pad510[4];
    s32 panelGroup;
    s32 panelOption;
    u8 pad51C[0x54];
    s32 setupState;
    PartyPanel partyPanel;
#else
    s32 panelGroup;
    s32 spriteArg0;
    s32 spriteArg1;
    s32 spriteArg2;
    u8 pad52C[0x54];
    s32 setupState;
    PartyPanel partyPanel;
#endif
    MenuPageWindow partyWindow;
    s32 panelHandle;
    s32 spriteHandle;
    s32 extentExhausted;
#ifdef VERSION_DDS1
    MenuAssets assets;
    s8 opacityReady;
    u8 padD3D[7];
    s32 teardownHandle;
    u32 opacity;
    u8 padD4C[0x828];
#else
    MenuCampEffect campEffect;
    s8 opacityReady;
    u8 padAEA9[7];
    s32 teardownHandle;
    u32 opacity;
    u8 padAEB8[0x828];
#endif
    s32 fadeProgress;
    u32 selectionInitialized;
    s32 iconFade;
    u32 commitComplete;
    u32 crossedSteps;
    s32 rewardMode;
    s32 rewardIndex;
#ifdef VERSION_DDS2
    u32 thresholdMessageShown;
    u8 padB700[4];
#endif
} BrsSkillPackageWork;

typedef char BrsRewardRow_size_must_be_0x18[(sizeof(BrsRewardRow) == 0x18) ? 1 : -1];
typedef char BrsRewardBatch_size_must_be_0x7C[(sizeof(BrsRewardBatch) == 0x7C) ? 1 : -1];
typedef char BrsActiveProgressList_size_must_be_0xE0[(sizeof(BrsActiveProgressList) == 0xE0) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char BrsRewardSummary_size_must_be_0x2C[(sizeof(BrsRewardSummary) == 0x2C) ? 1 : -1];
typedef char BrsSkillPackageWork_size_must_be_0x1590[(sizeof(BrsSkillPackageWork) == 0x1590) ? 1 : -1];
#else
typedef char BrsRewardSummary_size_must_be_0x30[(sizeof(BrsRewardSummary) == 0x30) ? 1 : -1];
typedef char BrsSkillPackageWork_size_must_be_0xB704[(sizeof(BrsSkillPackageWork) == 0xB704) ? 1 : -1];
#endif

#endif
