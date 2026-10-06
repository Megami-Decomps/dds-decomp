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
    u8 pad100[0x10];
    s32 state;               /* 0x110: aim waits for 0x1E, then resets this */
    s32 actionCode;          /* 0x114 */
    BtlIndexList *targetList; /* 0x118: indexed target list */
    s32 motionProgress;      /* 0x11C: one-shot aim latch */
    u8 pad120[0xC];
    s32 durationFrames;       /* 0x12C */
    f32 motionParameter;     /* 0x130: aim setup stores 10 */
} BtlLinkedCommand;
#endif /* VERSION_DDS1 */

#ifdef VERSION_DDS2
/* Queued action slot: some queries inspect the full word, others its ID. */
typedef union BattleActionSlot {
    s32 word;
    s16 actionId;
} BattleActionSlot;


/* The thirteen retained groups contain 32 fixed-size operand records each. */
typedef struct BtlOperandEntry {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    u8 pad14[4];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    u32 flags;
} BtlOperandEntry;

typedef struct BtlOperandGroup {
    u8 count;
    u8 pad01[3];
    s32 interval;
    s32 unk08;
    s32 unk0C;
    u8 unk10; /* Reset bytewise; some predicates read the +0x10/+0x11 pair. */
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[6];
    BtlOperandEntry entries[32]; /* 0x1C; group stride 0x59C */
} BtlOperandGroup;

/* Embedded 0x70-byte index work. DDS2 0x1DF700/0x1DF7B8/0x1DF810
 * reset, initialize and release its owned list and thirteen-group buffer. */
typedef struct BattleIndexWork {
    s32 phase;
    s32 skillId;
    s32 reference;
    s32 unk0C;
    s32 unk10;
    BtlUnit *linkedUnit;
    s32 unk18;
    s32 stageValue;
    s32 unk20;
    s32 slot;
    s32 adjustedValue;
    s8 resultKind;
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u16 stage;
    u8 pad32[2];
    s32 parameter;
    s32 wait;
    s32 unk3C;
    BtlIndexList *indices;
    u8 pad44[4];
    u64 unk48;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u16 flags;
    u8 pad5E[2];
    s32 unk60;
    u8 unk64;
    u8 pad65[3];
    BtlOperandGroup *groups;
    u32 allocationHandle;
} BattleIndexWork;

void btlInitBattleIndexWork(BattleIndexWork *work);
void btlResetIndexWork(BattleIndexWork *work);
void btlReleaseObjectBuffers(BattleIndexWork *work);

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
    s32 actionCode;          /* 0x134 */
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
