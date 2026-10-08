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

/* Exponential interpolation also supplies the actor-panel expansion scale. */
typedef struct BtlExponentialRange {
    f32 start; /* 0x00: span */
    f32 end;   /* 0x04: current progress */
} BtlExponentialRange;

typedef struct BtlScalarRange {
    f32 start;       /* 0x00: initial span */
    f32 end;         /* 0x04: remaining span */
    f32 inverseSpan; /* 0x08: quadratic acceleration */
    f32 velocity;    /* 0x0C */
    f32 value;       /* 0x10: accumulated progress */
} BtlScalarRange;

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
    u32 status;              /* 0x104: command status used by callback dispatch. */
    s32 actionKind;          /* 0x108: previous camera action kind. */
    u16 stepKind;             /* 0x10C: camera initialization stores 9, 10 or 11. */
    u8 pad10E[2];
    s32 state;               /* 0x110: aim waits for 0x1E, then resets this */
    s32 actionCode;          /* 0x114 */
    BtlIndexList *targetList; /* 0x118: indexed target list */
    s32 motionProgress;      /* 0x11C: one-shot aim latch */
    u8 pad120[8];
    union {
        s32 progressBits;
        f32 progress;
    };                      /* 0x128: initialized as bits, interpolated as float */
    s32 durationFrames;       /* 0x12C */
    f32 motionParameter;     /* 0x130: aim setup stores 10 */
    BtlExponentialRange exponentialRange; /* 0x134 */
    BtlScalarRange quadraticRange;        /* 0x13C */
} BtlLinkedCommand;

BtlTask *btlCreateActionSeq(void);
void btlDestroyActionSeq(BtlTask *actor);
#endif /* VERSION_DDS1 */

#ifdef VERSION_DDS2

ActionStateLink *btlCreateActionSeq(void);
void btlDestroyActionSeq(ActionStateLink *actor);
ActionStateLink *btlFindUnitByActor(BtlUnit *unit);

typedef struct BtlLinkedCommand {
    BtlCamState camera;       /* 0x00 */
    u8 pad28[8];
    BtlCamState frontCamera;  /* 0x30 */
    u8 pad58[0x68];
    BtlCamState backCamera;   /* 0xC0 */
    u8 padE8[8];
    f32 translation[4];      /* 0xF0: applied to the embedded battle camera pose */
    f32 rotation[4];         /* 0x100: quaternion for that pose transform */
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
    f32 cameraDistanceOffset; /* 0x148: added after actor camera arrangement. */
    union {
        s32 progressBits;
        f32 progress;
    };                      /* 0x14C: initialized as bits, interpolated as float */
    s32 durationFrames;      /* 0x150 */
    f32 motionParameter;     /* 0x154: aim setup stores 10 */
    BtlExponentialRange exponentialRange; /* 0x158 */
    BtlScalarRange quadraticRange;        /* 0x160 */
} BtlLinkedCommand;
#endif /* VERSION_DDS2 */

void btlScalarRangeSetStartClearEnd(BtlExponentialRange *state, f32 start);
f32 btlScalarRangeStepExponential(BtlExponentialRange *state);
void btlScalarRangeInitQuadratic(BtlScalarRange *state, f32 start);
f32 btlScalarRangeStepQuadratic(BtlScalarRange *state, f32 timeStep);

#if defined(VERSION_DDS1) || defined(VERSION_DDS2)
s32 btlStepPoseBlendHalf(BtlLinkedCommand *command);
s32 btlStepPoseBlend(BtlLinkedCommand *command);
s32 btlStepPoseBlendFrame(BtlLinkedCommand *command);
s32 btlStepPoseBlendRatio(BtlLinkedCommand *command);
#endif

#endif /* BTL_COMMAND_H */
