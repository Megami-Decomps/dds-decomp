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
    s8 backgroundState;
    u8 pad01[7];
    u32 backgroundOpacity;
    u8 pad0C[8];
    s8 portraitReady;
    u8 pad15[7];
    u32 portraitOpacity;
    s32 portraitPosition[2]; /* x, y */
} BrsFadeAnimation;

/* Both games use 0x68-byte progress rows, with different live field offsets. */
typedef struct BrsProgressAnimation {
#ifdef VERSION_DDS1
    s8 state;
    u8 pad01[7];
    s32 alpha;
    u8 pad0C[0xC];
    s16 level;
    u8 pad1A[2];
    s32 currentProgress;
    s32 remaining;
    s32 applied;
    s32 frames;
    s8 skipRamp;
    s8 unk2D;
    u8 pad2E[2];
    s32 iconFrame;
    s32 iconOpacity;
    s8 iconState;
    u8 pad39[3];
    s32 iconAngle;
    s32 iconColor;
    s32 iconPosition[2]; /* x, y */
    s8 completionState;
    u8 pad4D[3];
    s32 completionColor;
    s32 auxiliaryColor;
    s32 auxiliaryPosition[2]; /* x, y */
    s8 progressInitialized;
    u8 pad61[3];
    s32 previousProgress;
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
    s32 appliedStep; /* 0x24: increment consumed by the level-progress update. */
    s32 frames; /* 0x28: progress ramp counter, clamped to 0..120. */
    s8 skipRamp; /* 0x2C: use the fixed fast step instead of the ramp. */
    s8 unk2D;
    u8 pad2E[2];
    s32 flashFrame;
    s32 flashOpacity;
    s8 progressIconEnabled;
    u8 pad39[3];
    s32 progressIconAngle; /* 0x3C: signed 15-degree steps modulo 360. */
    s32 progressIconOpacity; /* Signed values are clamped to 0..128 by the animations. */
    s32 progressIconPosition[2]; /* x, y */
    s8 iconState;
    u8 pad4D[3];
    s32 iconOpacity;
    s32 unk54;
    s32 unk58[2]; /* Pair initialized by the result animations; no reading role established. */
    s8 progressInitialized;
    u8 pad61[3];
    s32 previousProgress;
#endif
} BrsProgressAnimation;

#ifdef VERSION_DDS1
/* Five 0x14-byte rows occupy +0xD50..+0xDB4. func_00264B08 updates the
 * first row and then four following rows, including the last count at +0xDA0.
 * Opacity, X and Y are at row +8, +C and +10; unk04 remains unidentified. */
typedef struct BrsSkillIconRow {
    s8 count;
    u8 pad01[3];
    s32 unk04;
    u32 opacity;
    s32 x;
    s32 y;
} BrsSkillIconRow;
#endif

/* Allocation/zeroing: DDS1 00262684/0026269C; DDS2 0029959C/002995B4. */
typedef struct BrsSkillPackageWork {
    struct SdfMemBlock *allocation;
    u32 overlayFlags;
    BrsResultTransition transition;
    struct EffectList *resourceList;
    BrsRewardSummary rewards;
#ifdef VERSION_DDS1
    u8 pad88[8];
    struct EffectSlotSet *unitResource;
    u8 pad94[4];
    BrsRewardRow *selectedRewardRow;
#else
    u8 pad8C[8];
    struct EffectSlotSet *unitResource;
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
    struct {
        s8 opacityReady;
        u8 padD3D[7];
        struct EffectSlotSet *teardownResource;
        u32 opacity;
        s8 resultPhase;
        s8 unkD4D;
        u8 padD4E[0x2];
        BrsSkillIconRow skillIconRows[5];
        BrsFadeAnimation fadeAnimation[5];
        u8 padE7C[0x78];
        BrsProgressAnimation levelAnimation[5];
        u8 pad10FC[0x138];
        BrsProgressAnimation profileAnimation[5];
        u8 pad143C[0x138];
    };
#else
    MenuCampEffect campEffect;
    struct {
        s8 opacityReady;
        u8 padAEA9[7];
        struct EffectSlotSet *teardownResource;
        u32 opacity;
        s8 resultPhase; /* 0xAEB8: 1 -> 2 once the result counters finish (func_0029DB58) */
        s8 unkAEB9;     /* 0xAEB9: set when the confirm input lands at full opacity */
        u8 padAEBA[0x66];
        BrsFadeAnimation fadeAnimation[5];
        u8 padAFE8[0x78];
        BrsProgressAnimation levelAnimation[5];
        u8 padB268[0x138];
        BrsProgressAnimation profileAnimation[5];
        u8 padB5A8[0x138];
    };
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
typedef char BrsFadeAnimation_backgroundOpacity_offset_check[
    ((u32)&((BrsFadeAnimation *)0)->backgroundOpacity == 0x08) ? 1 : -1];
typedef char BrsFadeAnimation_backgroundState_offset_check[
    ((u32)&((BrsFadeAnimation *)0)->backgroundState == 0x00) ? 1 : -1];
typedef char BrsFadeAnimation_portraitReady_offset_check[
    ((u32)&((BrsFadeAnimation *)0)->portraitReady == 0x14) ? 1 : -1];
typedef char BrsFadeAnimation_portraitOpacity_offset_check[
    ((u32)&((BrsFadeAnimation *)0)->portraitOpacity == 0x1C) ? 1 : -1];
typedef char BrsFadeAnimation_portraitPosition_offset_check[
    ((u32)&((BrsFadeAnimation *)0)->portraitPosition == 0x20) ? 1 : -1];
typedef char BrsRewardRow_size_must_be_0x18[(sizeof(BrsRewardRow) == 0x18) ? 1 : -1];
typedef char BrsRewardBatch_size_must_be_0x7C[(sizeof(BrsRewardBatch) == 0x7C) ? 1 : -1];
typedef char BrsActiveProgressList_size_must_be_0xE0[(sizeof(BrsActiveProgressList) == 0xE0) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char BrsSkillIconRow_size_must_be_0x14[(sizeof(BrsSkillIconRow) == 0x14) ? 1 : -1];
#endif
#ifdef VERSION_DDS1
typedef char BrsRewardSummary_size_must_be_0x2C[(sizeof(BrsRewardSummary) == 0x2C) ? 1 : -1];
typedef char BrsSkillPackageWork_size_must_be_0x1590[(sizeof(BrsSkillPackageWork) == 0x1590) ? 1 : -1];
typedef char BrsSkillPackageWork_staffSlots_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->staffSlots == 0x4F8) ? 1 : -1];
typedef char BrsSkillPackageWork_skillIconRows_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->skillIconRows == 0xD50) ? 1 : -1];
typedef char BrsSkillPackageWork_fadeAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->fadeAnimation == 0xDB4) ? 1 : -1];
typedef char BrsProgressAnimation_size_must_be_0x68[(sizeof(BrsProgressAnimation) == 0x68) ? 1 : -1];
typedef char BrsSkillPackageWork_levelAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->levelAnimation == 0xEF4) ? 1 : -1];
typedef char BrsSkillPackageWork_profileAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->profileAnimation == 0x1234) ? 1 : -1];
typedef char BrsProgressAnimation_alpha_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->alpha == 0x08) ? 1 : -1];
typedef char BrsProgressAnimation_currentProgress_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->currentProgress == 0x1C) ? 1 : -1];
typedef char BrsProgressAnimation_unk2D_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->unk2D == 0x2D) ? 1 : -1];
typedef char BrsProgressAnimation_iconFrame_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->iconFrame == 0x30) ? 1 : -1];
typedef char BrsProgressAnimation_iconOpacity_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->iconOpacity == 0x34) ? 1 : -1];
typedef char BrsProgressAnimation_iconState_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->iconState == 0x38) ? 1 : -1];
typedef char BrsProgressAnimation_iconColor_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->iconColor == 0x40) ? 1 : -1];
typedef char BrsProgressAnimation_iconPosition_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->iconPosition == 0x44) ? 1 : -1];
typedef char BrsProgressAnimation_completionColor_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->completionColor == 0x50) ? 1 : -1];
typedef char BrsProgressAnimation_auxiliaryColor_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->auxiliaryColor == 0x54) ? 1 : -1];
typedef char BrsProgressAnimation_auxiliaryPosition_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->auxiliaryPosition == 0x58) ? 1 : -1];
typedef char BrsProgressAnimation_progressInitialized_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->progressInitialized == 0x60) ? 1 : -1];
typedef char BrsProgressAnimation_previousProgress_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->previousProgress == 0x64) ? 1 : -1];
#else
typedef char BrsRewardSummary_size_must_be_0x30[(sizeof(BrsRewardSummary) == 0x30) ? 1 : -1];
typedef char BrsSkillPackageWork_size_must_be_0xB704[(sizeof(BrsSkillPackageWork) == 0xB704) ? 1 : -1];
typedef char BrsSkillPackageWork_staffSlots_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->staffSlots == 0x51C) ? 1 : -1];
typedef char BrsProgressAnimation_size_must_be_0x68[(sizeof(BrsProgressAnimation) == 0x68) ? 1 : -1];
typedef char BrsSkillPackageWork_fadeAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->fadeAnimation == 0xAF20) ? 1 : -1];
typedef char BrsSkillPackageWork_levelAnimation_offset_check[
    ((u32)&((BrsSkillPackageWork *)0)->levelAnimation == 0xB060) ? 1 : -1];
typedef char BrsProgressAnimation_drawPhase_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->drawPhase == 0x00) ? 1 : -1];
typedef char BrsProgressAnimation_alpha_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->alpha == 0x08) ? 1 : -1];
typedef char BrsProgressAnimation_appliedStep_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->appliedStep == 0x24) ? 1 : -1];
typedef char BrsProgressAnimation_frames_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->frames == 0x28) ? 1 : -1];
typedef char BrsProgressAnimation_skipRamp_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->skipRamp == 0x2C) ? 1 : -1];
typedef char BrsProgressAnimation_unk2D_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->unk2D == 0x2D) ? 1 : -1];
typedef char BrsProgressAnimation_flashFrame_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->flashFrame == 0x30) ? 1 : -1];
typedef char BrsProgressAnimation_flashOpacity_offset_check[
    ((u32)&((BrsProgressAnimation *)0)->flashOpacity == 0x34) ? 1 : -1];
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
