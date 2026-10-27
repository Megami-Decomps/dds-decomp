#ifndef MNU_MANTRA_H
#define MNU_MANTRA_H

#include "mnu.h"
#include "dat_state.h"

struct ItfMesSub;
struct FileRequest;
struct MenuList;
struct MenuListNode;
struct sdfMemHandle_t;
typedef struct MantraPanelPool MantraPanelPool;
struct MantraDrawPool;

typedef struct MtrSelectionState {
    s16 state;
    s16 outcome;
    s32 timer;
    s32 alpha;
    f32 scale;
    f32 highlightAlpha;
} MtrSelectionState;

typedef struct MtrSelectionFlags {
    u32 titleEffectsReadyPendingDisplay : 1;
    u32 visible : 1;
    u32 fadeProgress : 1;
    u32 profileReady : 1;
    u32 equipApplied : 1;
    u32 unk05 : 27;
} MtrSelectionFlags;

/* mtrAdvanceMantraResourceLoadState's progress through mtrMantraResourceFiles. */
typedef struct MtrResourceLoadState {
    struct FileRequest *fileEntry;
    struct FileRequest *archive; /* released through pacFileHandleFree_ when set */
    s16 state;
    u16 entryIndex;
    s32 partyMask; /* bit unitId - 1 per active party member; tested with srav */
} MtrResourceLoadState;

typedef char MtrResourceLoadStateFileEntryOffsetCheck[((u32)&((MtrResourceLoadState *)0)->fileEntry == 0x00) ? 1 : -1];

typedef struct MtrEquipState {
    s32 state;
    s32 timer;
} MtrEquipState;

/* The active and delayed-successor phases of mnuUpdateMantraTutorial. */
typedef u8 MtrTutorialState;
enum MtrTutorialStateValue {
    MTR_TUTORIAL_STATE_CHANGE_TO_TUTORIAL_OUT = 1,
    MTR_TUTORIAL_STATE_CHANGE_TO_TUTORIAL_IN = 2,
    MTR_TUTORIAL_STATE_CHANGE_TO_NORMAL_OUT = 3,
    MTR_TUTORIAL_STATE_CHANGE_TO_NORMAL_IN = 4,
    MTR_TUTORIAL_STATE_INITIALIZE = 5,
    MTR_TUTORIAL_STATE_EXIT = 6,
    MTR_TUTORIAL_STATE_STEP_000_RUN = 7,
    MTR_TUTORIAL_STATE_STEP_000 = 8,
    MTR_TUTORIAL_STATE_STEP_000_WAIT = 9,
    MTR_TUTORIAL_STATE_STEP_001_RUN = 10,
    MTR_TUTORIAL_STATE_STEP_001_RUN2 = 11,
    MTR_TUTORIAL_STATE_STEP_001 = 12,
    MTR_TUTORIAL_STATE_STEP_001_WAIT = 13,
    MTR_TUTORIAL_STATE_STEP_002_RUN = 14,
    MTR_TUTORIAL_STATE_STEP_002_RUN2 = 15,
    MTR_TUTORIAL_STATE_STEP_002 = 16,
    MTR_TUTORIAL_STATE_STEP_002_WAIT = 17,
    MTR_TUTORIAL_STATE_STEP_003_RUN = 18,
    MTR_TUTORIAL_STATE_STEP_003 = 19,
    MTR_TUTORIAL_STATE_STEP_003_WAIT = 20,
    MTR_TUTORIAL_STATE_STEP_004_RUN = 21,
    MTR_TUTORIAL_STATE_STEP_004 = 22,
    MTR_TUTORIAL_STATE_STEP_004_WAIT = 23,
    MTR_TUTORIAL_STATE_STEP_005_RUN = 24,
    MTR_TUTORIAL_STATE_STEP_005 = 25,
    MTR_TUTORIAL_STATE_STEP_005_WAIT = 26,
    MTR_TUTORIAL_STATE_STEP_006_RUN = 27,
    MTR_TUTORIAL_STATE_STEP_006 = 28,
    MTR_TUTORIAL_STATE_STEP_006_WAIT = 29,
    MTR_TUTORIAL_STATE_STEP_007_RUN = 30,
    MTR_TUTORIAL_STATE_STEP_007 = 31,
    MTR_TUTORIAL_STATE_STEP_007_WAIT = 32,
    MTR_TUTORIAL_STATE_DELAYED_TRANSITION = 37,
};

/* 00289CA0/00289CE8 store the ID byte; 00289CB0..00289CF8 update kind/marked as a halfword. */
typedef struct MtrUnitMenuEntry {
    union {
        u16 bits;
        struct {
            u16 nodeId : 8;
            u16 kind : 4;
            u16 unk12 : 1;
            u16 marked : 1;
            u16 unk14 : 2;
        };
    };
    u8 pad02[6];
} MtrUnitMenuEntry;

/* One eight-byte source-bank record: source selection and transition phases
 * use the same packed cell and retain their established field names. */
typedef struct MenuMantraRecord {
    union {
        u16 flags;
        u16 bits;
        struct {
            u8 mantraId;
            u8 flagsHigh;
        };
        struct {
            u16 unitIndex : 8;
            u16 kind : 4;
            u16 flag12 : 1;
            u16 queued : 1;
            u16 announced : 1; /* 0x4000: shown; mnuScanMantraSourceCandidates skips it */
            u16 unk15 : 1;
        } fields;
    };
    union {
        u16 eventFlag;
        u16 completionFlagId;
    };
    s8 element;
    s8 selectedIndex;
    s16 targetId;
} MenuMantraRecord;

typedef MenuMantraRecord MantraMenuSrc;

typedef struct MantraFlagResource {
    struct sdfMemHandle_t *allocation;
    u32 capacity;
    u16 *flags;
    u16 data[176];
} MantraFlagResource;

typedef union MenuPanelSelector {
    u32 packed;
    struct {
        u16 flags;
        s16 index;
    } fields;
} MenuPanelSelector;

/* The existing unit-entry flags and source-record tags are separate complete
 * 8-byte records. Native unit flags are accessed as halfwords. */

/* Both node-position tables use this complete 0x20-byte record. */
typedef struct MantraNodePos {
    union {
        MenuPanelSelector selector;
        struct {
            s32 kind : 4;
            s32 modelFlagState : 4;
            u32 reserved : 8;
            s16 id;
        };
    };
    s16 x;
    s16 y;
    struct MantraNodePos *neighbors[6];
} MantraNodePos;



/* The single menu work area starts at owner+0x240. Allocation and call
 * chains share it between selection, navigation and panel animation. */
typedef struct MantraMenuWork {
    s16 frame;
    s8 waitFrames;
    u8 nextMode;
    s8 sourceWaitFrames; /* source-transition pending state, separate from +2/+3 */
    u8 sourceNextState;
    s8 tutorialWaitFrames;
    MtrTutorialState tutorialNextState; /* delayed target state for the tutorial task */
    datUnitWork_t tutorialParty[3];
    union {
        u32 drawFlags;
        struct {
            u32 iconFade : 1;
            u32 hasSource : 1;
            u32 mode : 8;
            u32 drawRank : 4; /* max(model flag state, last enabled mantra category) */
            u32 drawEnabled : 1;
            u32 sourceDialogVariant : 1; /* Selects one of the two source-dialog variants. */
            u32 hasQueuedMastery : 1;
            u32 showOverlay : 1;
            u32 sourceKind : 8;
            u32 unk26 : 6;
        } drawBits;
    };
    s16 scrollX;
    s16 scrollY;
    s16 scrollEndX;
    s16 scrollEndY;
    MantraNodePos *defaultSelector;
    MantraNodePos *alternateSelector;
    u8 pad568[4];
    MantraFlagResource *slots[18];
    u32 collectedValues[8];
    s32 savedSelection;
    s32 collectedCount;
    MantraNodePos *savedSelector;
    s8 selectionIndex;
    s8 panelTransitionMode;
    u16 panelTransitionTimer;
    MenuMantraRecord records[112];
    MantraMenuSrc *src;
    u16 sourceMode;
    u16 sourceFlag;
    s32 sourcePartyIndex;
    MtrUnitMenuEntry unitEntries[5];
    MtrUnitMenuEntry *currentSlot;
    s32 masteryFrames;
    s32 delayFrames;
    s32 navigationState;
    u32 navigationMask;
    MantraPanelPool *resource;
    union {
        u32 flags;
        u8 flagBytes[4];
        MtrTutorialState tutorialState; /* low byte: active phase of the tutorial task */
    };
    u8 unk9B4;
    s8 tutorialTransitionMode; /* 0: hidden, 1: opening, 2: closing, 3: visible. */
    s16 tutorialTransitionFrame; /* Frame within the five-step tutorial fade. */
    MtrEquipState equip;
    struct MantraDrawPool *selectionController;
} MantraMenuWork;

/* mnuAllocateMantraResourceWork allocates and clears exactly 0xC08 bytes. The task keeps
 * this whole pointer while menu consumers address its work subobject. */
typedef struct MnuStatusResource {
    struct sdfMemHandle_t *allocation;
    struct MenuList *list;
    u32 resourceSlots[12];
    struct sdfMemHandle_t *resourceAllocationA;
    struct sdfMemHandle_t *resourceAllocationB;
    struct ItfMesSub *messageWindow;
    struct ItfMesSub *messageDefinition;
    MenuProgressHost *progressHost;
    s32 currency; /* 0x4C: copy of the saved currency taken when the menu opens */
    u8 pad50[4];
    datUnitWork_t snapshot;
    MtrSelectionFlags flags;
    MtrResourceLoadState resourceLoad;
    MtrSelectionState selection;
    MantraMenuWork menu;
    u8 padC04[4];
} MnuStatusResource;

typedef char MnuStatusResource_size[(sizeof(MnuStatusResource) == 0xC08) ? 1 : -1];
typedef char MantraMenuWork_size[(sizeof(MantraMenuWork) == 0x9C4) ? 1 : -1];
typedef char MtrTutorialState_width_check[(sizeof(MtrTutorialState) == 1) ? 1 : -1];
typedef char MantraTutorialStateOffsetsCheck[
    ((u32)&((MantraMenuWork *)0)->tutorialNextState == 7 &&
     (u32)&((MantraMenuWork *)0)->tutorialState == 0x9B0)
        ? 1
        : -1];
typedef char MantraNodePos_size[(sizeof(MantraNodePos) == 0x20) ? 1 : -1];
typedef char MantraFlagResource_size[(sizeof(MantraFlagResource) == 0x16C) ? 1 : -1];

typedef char MnuStatusResourceMenuOffsetCheck[((u32)&((MnuStatusResource *)0)->menu == 0x240) ? 1 : -1];
typedef char MnuStatusResourcePanelPoolOffsetCheck[((u32)&((MnuStatusResource *)0)->menu.resource == 0xBEC) ? 1 : -1];
typedef char MnuStatusResourceSnapshotOffsetCheck[((u32)&((MnuStatusResource *)0)->snapshot == 0x54) ? 1 : -1];
typedef char MnuStatusResourceResourceAllocationAOffsetCheck[((u32)&((MnuStatusResource *)0)->resourceAllocationA == 0x38) ? 1 : -1];
typedef char MnuStatusResourceResourceAllocationBOffsetCheck[((u32)&((MnuStatusResource *)0)->resourceAllocationB == 0x3C) ? 1 : -1];
typedef char MnuStatusResourceUnitEntriesOffsetCheck[((u32)&((MnuStatusResource *)0)->menu.unitEntries == 0xBB0) ? 1 : -1];

typedef char MantraTutorialPartyOffsetCheck[((u32)&((MantraMenuWork *)0)->tutorialParty == 8) ? 1 : -1];

typedef char MantraMenuSrc_size[(sizeof(MantraMenuSrc) == 8) ? 1 : -1];
typedef char MantraMenuSrcSelectedIndexOffsetCheck[((u32)&((MantraMenuSrc *)0)->selectedIndex == 5) ? 1 : -1];
typedef char MantraMenuSrcTargetIdOffsetCheck[((u32)&((MantraMenuSrc *)0)->targetId == 6) ? 1 : -1];
typedef char MantraSourceWaitOffsetCheck[((u32)&((MantraMenuWork *)0)->sourceWaitFrames == 4) ? 1 : -1];
typedef char MantraSourceNextStateOffsetCheck[((u32)&((MantraMenuWork *)0)->sourceNextState == 5) ? 1 : -1];
typedef char MantraSourcePartyIndexOffsetCheck[((u32)&((MantraMenuWork *)0)->sourcePartyIndex == 0x96C) ? 1 : -1];

#endif
