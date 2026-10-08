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
    MenuPopupState data;
    s32 state;
} BrsResultTransition;

typedef struct BrsFadeAnimation {
#ifdef VERSION_DDS1
    u8 pad00[0x14];
    s8 state;
    u8 pad15[7];
    u32 opacity;
    u8 pad20[8];
#else
    u8 pad00[0x10];
    s8 state;
    u8 pad11[7];
    u32 opacity;
    u8 pad1C[0xC];
#endif
} BrsFadeAnimation;

/* Both games use 0x68-byte progress rows, with different live field offsets. */
typedef struct BrsProgressAnimation {
#ifdef VERSION_DDS1
    u8 pad00[0x14];
    s8 state;
    u8 pad15[0x17];
    s16 level;
    u8 pad2E[6];
    s32 remaining;
    s32 applied;
    s32 frames;
    s8 unk40;
    u8 pad41[0x27];
#else
    s8 drawPhase;
    u8 pad01[7];
    s32 alpha;
    u8 pad0C[4];
    s8 state;
    u8 pad11[7];
    s16 level;
    u8 pad1A[2];
    s32 remaining;
    s32 applied;
    u8 pad24[0x28];
    s8 iconState;
    u8 pad4D[3];
    u32 iconOpacity;
    u8 pad54[0xC];
    s8 progressInitialized;
    u8 pad61[3];
    s32 previousProgress;
#endif
} BrsProgressAnimation;

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
    s32 statGains[DAT_BASE_STAT_COUNT];
    BrsActiveProgressList partyProgress;
    PrfSkillList skillList;
    StaffSlots staffSlots;
    s32 setupState;
    PartyPanel partyPanel;
    MenuPageWindow partyWindow;
    MenuPanelGroup *panelHandle;
    MenuSpriteState *spriteHandle;
    s32 extentExhausted;
#ifdef VERSION_DDS1
    MenuAssets assets;
    s8 opacityReady;
    u8 padD3D[7];
    s32 teardownHandle;
    u32 opacity;
    s8 resultPhase;
    s8 unkD4D;
    u8 padD4E[0x52];
    BrsFadeAnimation fadeAnimation[5];
    u8 padE68[0x78];
    BrsProgressAnimation levelAnimation[5];
    u8 pad10E8[0x138];
    BrsProgressAnimation profileAnimation[5];
    u8 pad1428[0x14C];
#else
    MenuCampEffect campEffect;
    s8 opacityReady;
    u8 padAEA9[7];
    s32 teardownHandle;
    u32 opacity;
    s8 resultPhase; /* 0xAEB8: 1 -> 2 once the result counters finish (func_0029DB58) */
    s8 unkAEB9;     /* 0xAEB9: set when the confirm input lands at full opacity */
    u8 padAEBA[0x56];
    BrsFadeAnimation fadeAnimation[5];
    u8 padAFD8[0x88];
    BrsProgressAnimation levelAnimation[5];
    u8 padB268[0x138];
    BrsProgressAnimation profileAnimation[5];
    u8 padB5A8[0x138];
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
    u16 earnedItem;
    u8 padB702[2];
#endif
} BrsSkillPackageWork;

typedef char BrsFadeAnimation_size_must_be_0x28[(sizeof(BrsFadeAnimation) == 0x28) ? 1 : -1];
typedef char BrsRewardRow_size_must_be_0x18[(sizeof(BrsRewardRow) == 0x18) ? 1 : -1];
typedef char BrsRewardBatch_size_must_be_0x7C[(sizeof(BrsRewardBatch) == 0x7C) ? 1 : -1];
typedef char BrsActiveProgressList_size_must_be_0xE0[(sizeof(BrsActiveProgressList) == 0xE0) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char BrsRewardSummary_size_must_be_0x2C[(sizeof(BrsRewardSummary) == 0x2C) ? 1 : -1];
typedef char BrsSkillPackageWork_size_must_be_0x1590[(sizeof(BrsSkillPackageWork) == 0x1590) ? 1 : -1];
typedef char BrsSkillPackageWork_staffSlots_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->staffSlots == 0x4F8) ? 1 : -1];
typedef char BrsProgressAnimation_size_must_be_0x68[(sizeof(BrsProgressAnimation) == 0x68) ? 1 : -1];
typedef char BrsSkillPackageWork_levelAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->levelAnimation == 0xEE0) ? 1 : -1];
typedef char BrsSkillPackageWork_profileAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->profileAnimation == 0x1220) ? 1 : -1];
#else
typedef char BrsRewardSummary_size_must_be_0x30[(sizeof(BrsRewardSummary) == 0x30) ? 1 : -1];
typedef char BrsSkillPackageWork_size_must_be_0xB704[(sizeof(BrsSkillPackageWork) == 0xB704) ? 1 : -1];
typedef char BrsSkillPackageWork_staffSlots_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->staffSlots == 0x51C) ? 1 : -1];
typedef char BrsProgressAnimation_size_must_be_0x68[(sizeof(BrsProgressAnimation) == 0x68) ? 1 : -1];
typedef char BrsSkillPackageWork_levelAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->levelAnimation == 0xB060) ? 1 : -1];
typedef char BrsProgressAnimation_drawPhase_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->drawPhase == 0x00) ? 1 : -1];
typedef char BrsProgressAnimation_alpha_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->alpha == 0x08) ? 1 : -1];
typedef char BrsProgressAnimation_progressInitialized_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->progressInitialized == 0x60) ? 1 : -1];
typedef char BrsProgressAnimation_previousProgress_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->previousProgress == 0x64) ? 1 : -1];
typedef char BrsSkillPackageWork_profileAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->profileAnimation == 0xB3A0) ? 1 : -1];
typedef char BrsSkillPackageWork_earnedItem_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->earnedItem == 0xB700) ? 1 : -1];
#endif

#endif
