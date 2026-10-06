#ifndef BTL_COMMAND_H
#define BTL_COMMAND_H

#include "btl.h"

/* Camera pose payload (0x28); command members occupy SDK-aligned storage. */
typedef struct BtlCamState {
    f32 position[4];  /* 0x00 */
    f32 direction[4]; /* 0x10 */
    f32 distance;     /* 0x20 */
    f32 fov;          /* 0x24 */
} BtlCamState;

/* Linked motion command shared by actor selection, action cameras and aim.
 * It is not a BtlUnit or its owner. All three camera offsets are common to
 * both games; DDS2 inserts 0x20 bytes before the control fields. */
#ifdef VERSION_DDS1
typedef struct BtlLinkedCommand {
    BtlCamState camera;       /* 0x00 */
    u8 pad28[8];
    BtlCamState frontCamera;  /* 0x30 */
    u8 pad58[0x68];
    BtlCamState backCamera;   /* 0xC0 */
    u8 padE8[8];
    u32 flags;               /* 0xF0: includes facing-direction bit 0x200 */
    BtlTask *task;            /* 0xF4: fallback actor is task->unit */
    BtlUnit *linkedA;         /* 0xF8 */
    BtlUnit *linkedB;         /* 0xFC */
    BtlUnit *selectedUnit;    /* 0x100: actor selected for camera setup. */
    u8 pad104[0xC];
    s32 state;               /* 0x110: aim waits for 0x1E, then resets this */
    s32 actionCode;          /* 0x114 */
    BtlIndexList *targetList; /* 0x118: indexed target list */
    s32 motionProgress;      /* 0x11C: one-shot aim latch */
    u8 pad120[0xC];
    s32 durationFrames;       /* 0x12C */
    f32 motionParameter;     /* 0x130: aim setup stores 10 */
} BtlLinkedCommand;

BtlTask *btlCreateActionSeq(void);
void btlDestroyActionSeq(BtlTask *actor);
#endif /* VERSION_DDS1 */

#ifdef VERSION_DDS2
/* Queued action slot: some queries inspect the full word, others its ID. */
typedef union BattleActionSlot {
    s32 word;
    s16 actionId;
} BattleActionSlot;


/* DDS2 0x1DCF58 allocates this 0x180-byte command actor; its two list links
 * are at 0x174/0x178. It is distinct from the 0x368-byte world unit. */
typedef struct ActionStateLink {
    u32 state; /* 0x00: scene readiness compares this state as an unsigned word. */
    u16 actionNumber;
    u8 pad06[2];
    u32 pendingFlags; /* 0x08 */
    u32 flags; /* 0x0C */
    s32 stateTime;
    s32 completedTurns;
    BtlUnit *unit; /* 0x18 */
    u8 pad1C[4];
    BattleIndexWork indexWork; /* 0x20..0x8F */
    u16 aiCounter; /* 0x90: wraps as a halfword, then clamps to 0xFF */
    u8 pad92[0xBC];
    s8 lowHpActionHold; /* 0x14E: positive suppresses the low-HP action */
    u8 pad14F;
    BattleActionSlot actions[8]; /* 0x150 */
    s32 lastMode; /* 0x170 */
    struct ActionStateLink *prev;
    struct ActionStateLink *next;
    u8 pad17C[4];
} ActionStateLink;

ActionStateLink *btlCreateActionSeq(void);
void btlDestroyActionSeq(ActionStateLink *actor);
ActionStateLink *btlFindUnitByActor(BtlUnit *unit);

typedef struct BtlLinkedCommand {
    BtlCamState camera;       /* 0x00 */
    u8 pad28[8];
    BtlCamState frontCamera;  /* 0x30 */
    u8 pad58[0x68];
    BtlCamState backCamera;   /* 0xC0 */
    u8 padE8[0x28];
    u32 flags;               /* 0x110: includes facing-direction bit 0x200 */
    struct ActionStateLink *link; /* 0x114: fallback actor is link->unit (+0x18) */
    BtlUnit *linkedA;         /* 0x118: also read by func_002172B8 */
    BtlUnit *linkedB;         /* 0x11C */
    BtlUnit *focus;          /* 0x120 */
    u32 status;              /* 0x124 */
    s32 actionKind;          /* 0x128 */
    u16 stepKind;            /* 0x12C: action-camera dispatch kind */
    u8 pad12E[2];
    s32 state;               /* 0x130 */
    u32 actionCode;          /* 0x134: unsigned; the VURI camera range test is sltiu */
    BtlIndexList *targetList; /* 0x138 */
    s32 motionProgress;      /* 0x13C: timed-action count or one-shot aim latch */
    u8 pad140[4];
    s32 stageCount;          /* 0x144 */
    u8 pad148[4];
    union {
        s32 progressBits;
        f32 progress;
    };                      /* 0x14C: initialized as bits, interpolated as float */
    s32 durationFrames;      /* 0x150 */
    f32 motionParameter;     /* 0x154: aim setup stores 10 */
} BtlLinkedCommand;
#endif /* VERSION_DDS2 */

#endif /* BTL_COMMAND_H */
