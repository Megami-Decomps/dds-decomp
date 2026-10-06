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

/* The actor-panel allocation has three active rows and four reserve rows. */
typedef struct BattleActorPanelEntry {
    s32 x;
    s32 y;
    u8 pad08[0xC];
    s8 presentationState;
    s8 presentationValue;
    u8 pad16[0xA];
    s32 transitionGeometry[4];
    u8 transitionFade[2];
    u8 pad32[2];
    s32 pulseDirection[2];
    s32 unk3C[2];
    s32 pulseOffsets[2][2];
    s16 pulseLevel[2];
    u32 highlightPhase[8];
    u8 highlightLevel[8];
    u8 transitionState;
    s8 secondaryPresentationValue;
    u8 pad82[0xA];
    s32 secondaryGeometry[4];
    u8 secondaryFade[2];
    u8 pad9E[0x12];
    s32 secondaryPulseOffsets[2][2];
    s16 secondaryPulseLevel[2];
    u8 padC4[0x28];
    s8 pendingSceneState;
    u8 padED[0x13];
    u8 unk100;
    u8 pad101[0xBB];
    s32 hpLevel;
    s32 mpLevel;
    u8 unk1C4;
    u8 unk1C5;
    u8 pad1C6[0x16];
    s32 hpTarget;
    s32 mpTarget;
    u8 pad1E4[0xAC];
} BattleActorPanelEntry;

typedef struct BattleActorPanelWork {
    struct SdfMemBlock *allocation;
    s32 activeCount;
    s32 reserveCount;
    BattleActorPanelEntry activeEntries[3];
    u16 unk7BC;
    s16 partyRecordIndex;
    BattleActorPanelEntry reserveEntries[4];
    struct SdfMemBlock *reserveUnitAllocation;
} BattleActorPanelWork;

typedef char BattleActorPanelEntrySizeCheck[sizeof(BattleActorPanelEntry) == 0x290 ? 1 : -1];
typedef char BattleActorPanelWorkSizeCheck[sizeof(BattleActorPanelWork) == 0x1204 ? 1 : -1];
typedef char BattleActorPanelActiveOffsetCheck[((u32)&((BattleActorPanelWork *)0)->activeEntries == 0xC) ? 1 : -1];
typedef char BattleActorPanelReserveOffsetCheck[((u32)&((BattleActorPanelWork *)0)->reserveEntries == 0x7C0) ? 1 : -1];

#endif /* BTL_UI_H */
