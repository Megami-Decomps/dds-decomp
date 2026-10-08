#ifndef BTL_UI_H
#define BTL_UI_H

#include "common.h"

struct BtlTask;
struct SdfMemBlock;
struct ActionStateLink;
struct BattleIndexWork;

typedef struct BattleSceneSelection {
    u16 cursor;
    u16 entry;
} BattleSceneSelection;

/* Both fldInitializeSceneObject constructors clear one 0x30-byte allocation.
 * Its final two pointers refer to the owning task's command work and task;
 * the state word alone is not a complete scene-object allocation. */
typedef struct BattleSceneObject {
    s32 state;
    BattleSceneSelection selections[7]; /* +0x04: indexed by scene kind 0..6. */
    u8 pad20[8];
    struct BattleIndexWork *commandData; /* +0x28: owning actor's complete +0x20 work. */
#ifdef VERSION_DDS1
    struct BtlTask *owner; /* +0x2C: the 0x170-byte DDS1 actor */
#else
    struct ActionStateLink *owner; /* +0x2C: the 0x180-byte DDS2 actor */
#endif
} BattleSceneObject;

#ifdef VERSION_DDS1
void fldInitializeSceneObject(BattleSceneObject *object, struct BtlTask *owner);
#else
void fldInitializeSceneObject(BattleSceneObject *object, struct ActionStateLink *owner);
s32 fldSelectSceneMode(struct ActionStateLink *task);
void func_001CAB60(struct ActionStateLink *task);
#endif

/* One allocated 0xCC-byte command panel, not a header plus overlapping rows.
 * Rows begin at +0x14/+0x64; the class Y coordinate occupies +0x10. */
typedef struct BattleCmdPanelSlot {
    s8 hidden; /* Nonzero grid entries are omitted by the row renderer. */
    u8 unk01; /* Initialized to 0/1 by bank; no consumer read established. */
    s16 fadeValue; /* Signed narrow ramp; rendering uses its low color byte. */
    u8 pad04[4];
    s32 x;
    s32 y;
} BattleCmdPanelSlot;

typedef struct BattleCmdPanel {
    s8 state;
    s8 classIndex;
    u16 labelEntry;
    s16 labelFade;
    s16 classFade;
    u8 pad08[4];
    u32 classX;
    u32 classY;
    BattleCmdPanelSlot slotsA[5];
    BattleCmdPanelSlot slotsB[5];
    s16 cornerFade[4]; /* Four low-byte color levels used by the backdrop. */
    u32 cornerPhase[4]; /* Advanced by 8 modulo 360 before the sine pulse. */
} BattleCmdPanel;

/* Native 0x58-byte command UI work in both games. The first 0x38 bytes
 * are fourteen 32-bit task handles; they are not the whole allocation.
 * DDS1 accesses the status bytes separately, while DDS2 also masks the
 * enclosing word. The fade step shares that word's high halfword. */
typedef struct BattleTrackedTaskWork {
    s32 handles[14];
    u32 presentationState; /* +0x38: linked actor-slot panel transition. */
    union {
        u32 flags;
        struct {
            s8 state;
            s8 blocked;
            u16 fadeStep; /* 0x80 / transition duration. */
        } bytes;
    } status; /* +0x3C */
    s32 counter;
    s32 threshold;
    s8 fadeKindsCached;
    u8 pad49[3];
    s32 fadeKindACount;
    s32 fadeKindBCount;
    s8 phaseGate; /* +0x54: nonzero skips two phase-gated panel updates. */
    u8 pad55[3];
} BattleTrackedTaskWork;

/* The linked selection task allocates 0x30 bytes: a one-based selected row,
 * four signed fade levels, four screen positions and a backdrop fade.
 * Defaults place the rows 23 pixels apart; updates slide x toward 11. */
typedef struct BattleSelectionPosition {
    s32 x;
    s32 y;
} BattleSelectionPosition;

typedef struct BattleSelectionWork {
    s8 selectedRow;
    u8 pad01;
    s16 rowFade[4]; /* +0x02; one-based selection indexes rowFade[row - 1]. */
    u8 pad0A[2];
    BattleSelectionPosition positions[4]; /* +0x0C */
    s16 backdropFade; /* +0x2C */
    u8 pad2E[2];
} BattleSelectionWork;

typedef struct BattleStatPulse {
    s8 active;
    u8 pad01[3];
    u32 phase;
    s32 progress;
    s32 yOffset;
    s16 alpha;
    u8 pad12[2];
} BattleStatPulse;
typedef char BattleStatPulse_size_must_be_0x14[(sizeof(BattleStatPulse) == 0x14) ? 1 : -1];

/* 0x2C-byte action record shared by the initializer, updater and renderer. */
typedef struct BattleMirroredSpriteRecord {
    s8 active;
    u8 pad01;
    s16 slot;
    f32 scale;
    s32 restoredWidth;
    s32 restoredHeight;
    s32 width;
    s32 height;
    s32 x;
    s32 y;
    s32 secondX;
    s32 frame;
    s8 alpha;
    u8 pad29[3];
} BattleMirroredSpriteRecord;

/* The actor-panel allocation has three active rows and four reserve rows. */
/* The state/render block begins at +0x10 within each 0x290-byte row. */
typedef struct BattleActorPanelPresentation {
    s16 fade;
    u8 pad02[2];
    s8 presentationState;
    s8 presentationValue;
    u8 pad06[2];
    s32 unk08; /* Reserve initializer clears this word. */
    s32 unk0C; /* Reserve initializer sets this word to 180. */
    s32 transitionGeometry[4];
    u8 transitionFade[2];
    u8 pad22[2];
    s32 pulseDirection[2];
    s32 pulseTimer[2];
    s32 pulseOffsets[2][2];
    s16 pulseLevel[2];
    u32 highlightPhase[8];
    u8 highlightLevel[8];
    s8 transitionState;
    s8 secondaryPresentationValue;
    u8 pad72[0xA];
    s32 secondaryGeometry[4];
    u8 secondaryFade[2];
    u8 pad8E[2];
    s32 secondaryPulseDirection[2];
    s32 secondaryPulseTimer[2];
    s32 secondaryPulseOffsets[2][2];
    s16 secondaryPulseLevel[2];
    union {
        u8 padB4[0x28];
        struct {
            u32 trianglePhase[8];
            u8 triangleAlpha[8];
        };
    };
    s8 pendingSceneState;
    u8 padDD[0x13];
    u8 unkF0;
    u8 padF1[7];
    BattleMirroredSpriteRecord mirroredSprites[4];
    u8 pad1A8[4];
    s32 hpLevel;
    s32 mpLevel;
    s8 hpState;
    s8 mpState;
    u8 pad1B6[2];
    s32 offsetX;
    s32 offsetY;
    s32 unk1C0;
    s32 unk1C4;
    s16 hpHighlightLevel;
    s16 mpHighlightLevel;
    s32 hpTarget;
    s32 mpTarget;
    s8 hpEffectState;
    s8 mpEffectState;
    s16 hpEffectFade;
    s16 mpEffectFade;
    u8 pad1DA[2];
    BattleStatPulse hpBarPulse;
    BattleStatPulse mpBarPulse;
    /* Signed frame counters, clamped to 0..12 before stacking three pulses. */
    s8 hpPulseFrame;
    s8 mpPulseFrame;
    u8 pad206[2];
    BattleStatPulse hpBarPulses[3];
    BattleStatPulse mpBarPulses[3];
} BattleActorPanelPresentation;

typedef struct BattleActorPanelEntry {
    s32 x;
    s32 y;
    s32 baseX;
    s32 baseY;
    BattleActorPanelPresentation presentation;
} BattleActorPanelEntry;

typedef struct BattleActorPanelWork {
    struct SdfMemBlock *allocation;
    s32 activeCount;
    s32 reserveCount;
    BattleActorPanelEntry activeEntries[3];
    s16 selectedReserveIndex;
    s16 partyRecordIndex;
    BattleActorPanelEntry reserveEntries[4];
    struct SdfMemBlock *reserveUnitAllocation;
} BattleActorPanelWork;

typedef char BattleActorPanelPresentationSizeCheck[sizeof(BattleActorPanelPresentation) == 0x280 ? 1 : -1];
typedef char BattleActorPanelTriangleOffsetCheck[
    ((u32)&((BattleActorPanelPresentation *)0)->trianglePhase == 0xB4 &&
     (u32)&((BattleActorPanelPresentation *)0)->triangleAlpha == 0xD4 &&
     sizeof(((BattleActorPanelPresentation *)0)->trianglePhase) == 0x20 &&
     sizeof(((BattleActorPanelPresentation *)0)->triangleAlpha) == 8) ? 1 : -1];
typedef char BattleActorPanelPresentationOffsetCheck[((u32)&((BattleActorPanelEntry *)0)->presentation == 0x10) ? 1 : -1];

typedef char BattleActorPanelEntrySizeCheck[sizeof(BattleActorPanelEntry) == 0x290 ? 1 : -1];
typedef char BattleActorPanelWorkSizeCheck[sizeof(BattleActorPanelWork) == 0x1204 ? 1 : -1];
typedef char BattleActorPanelActiveOffsetCheck[((u32)&((BattleActorPanelWork *)0)->activeEntries == 0xC) ? 1 : -1];
typedef char BattleActorPanelReserveOffsetCheck[((u32)&((BattleActorPanelWork *)0)->reserveEntries == 0x7C0) ? 1 : -1];

void btlSlotBankPromoteStates(BattleActorPanelWork *bank);

#endif /* BTL_UI_H */
