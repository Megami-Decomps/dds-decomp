#ifndef EVT_UNIT_H
#define EVT_UNIT_H

#include "common.h"
#include "kwln.h"
#include "eff_transform.h"

struct dds3ModelHandle_t;
struct dds3PathBasicControl_t;
struct sdfTexHandle_t;
struct SdfLightingPacketStorage;

/* Values stored in evtUnitHandle_t.motionState; the field remains a signed halfword. */
typedef enum EvtUnitMotionState {
    EVT_UNIT_MOTION_STATE_IDLE = 0,
    EVT_UNIT_MOTION_STATE_SOURCE = 1,
    EVT_UNIT_MOTION_STATE_MOTION = 2,
    EVT_UNIT_MOTION_STATE_VECTOR = 3,
    EVT_UNIT_MOTION_STATE_VALUE = 4
} EvtUnitMotionState;

/* Values observed in evtUnitHandle_t.transitionSourceKind; the field remains s16. */
typedef enum EvtUnitTransitionSourceKind {
    EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR = 0,
    EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE = 1,
    EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE = 2
} EvtUnitTransitionSourceKind;

/* Observed evtUnitHandle_t.motionSubmode values; storage remains a halfword. */
#define EVT_UNIT_MOTION_SUBMODE_TARGET_XZ 0
#define EVT_UNIT_MOTION_SUBMODE_TARGET_XZ_WITH_VERTICAL_STEP 1
#define EVT_UNIT_MOTION_SUBMODE_PATH_ENDPOINT 3

enum EvtUnitReferenceAngleCacheFlags {
    EVT_UNIT_REFERENCE_ANGLE_CACHE_VALID = 1
};

/* Bits consumed by the event unit's motion, value and visual transitions. */
enum EvtUnitFlags {
    EVT_UNIT_FLAG_PATH_REVERSE = 0x4,
    EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR = 0x100,
    EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR = 0x200,
    EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS = 0x300,
    EVT_UNIT_FLAG_RGB_TRANSITION = 0x8000,
    EVT_UNIT_FLAG_ALPHA_TRANSITION = 0x10000,
    EVT_UNIT_FLAG_VALUE_CHANGED = 0x20000,
    EVT_UNIT_FLAG_TARGET_TRANSITION = 0x40000,
    EVT_UNIT_FLAG_TARGET_BLEND_IN = 0x80000,
    EVT_UNIT_FLAG_TARGET_BLEND_OUT = 0x100000,
    EVT_UNIT_FLAG_TARGET_BLEND_PHASES = 0x180000
};

/* Shared light-target payload used by the event manager and battle base-light task. */
typedef struct EvtTargetInfo {
    f32 firstColor[4];           /* 0x00 */
    f32 direction[4];            /* 0x10 */
    u8 pad20[0x20];
    f32 secondColor[4];          /* 0x40 */
    f32 nearDistance, farDistance;
    f32 auxFirst, auxSecond;
    u32 unk60;
    u32 flags;                  /* 0x64: bit 3 selects the unit's own vector. */
} EvtTargetInfo;

typedef char EvtTargetInfoSizeCheck[sizeof(EvtTargetInfo) == 0x68 ? 1 : -1];

enum EvtTargetInfoFlags {
    EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR = 0x8
};

/* Event motion work, not the world-list node which contains its address.
 * Setup opcodes pass this same object to evtSetUnitMotion and
 * evtConfigureUnitMotionSlot; the direction updater passes it to planar aim.
 * targetVector is four floats even when copied as a quadword by VU0/MMI macros.
 * The manager copies the entire object by value: 0x170 (DDS1), 0x1D0 (DDS2).
 * No stronger alignment than the manager's original float layout is implied. */
typedef struct evtUnitHandle_t {
    u32 color;                     /* 0x00 */
    union {
        s32 objectId;               /* Legacy script-facing view at +0x04 */
        u32 firstCurrent;           /* Packed first-colour transition state */
    };
    union {
        u8 pad08[8];
        struct {
            u32 color08;            /* First-colour transition target */
            u32 color0C;            /* First-colour state before target influence */
        };
    };
    f32 vec10[4];                  /* 0x10 */
    f32 vec20[4];                  /* 0x20 */
    f32 vec30[4];                  /* 0x30 */
    union {
        u8 pad40[0x10];
        f32 vec40[4];               /* Direction before target influence */
    };
    u32 color50;                   /* 0x50 */
    union {
        u8 pad54[0xC];
        struct {
            u32 color54;            /* Second-colour transition state */
            u32 color58;            /* Second-colour transition target */
            u32 color5C;            /* Second-colour state before target influence */
        };
    };
    u32 color60;                   /* 0x60: packed color retained during RGB/alpha transitions */
    union {
        u8 pad64[4];
        u32 color64;                /* Destination packed RGB/alpha word */
    };
    struct SdfLightingPacketStorage *endpointWork; /* 0x68: owned lighting packet storage */
    u32 value;                     /* 0x6C */
    f32 targetVector[4];           /* 0x70 */
    dds3Basic_t *effObj;          /* 0x80: kind-5 world node owning this motion work. */
    dds3Basic_t *currentTransitionTarget;  /* 0x84: active target world node */
    dds3Basic_t *previousTransitionTarget; /* 0x88: prior target during blend-in */
    struct dds3ModelHandle_t *owner;          /* 0x8C: model context and its SDK motion slots */
    dds3Basic_t *linkedUnit;       /* 0x90: retained world/path transition source */
    s32 (*motionCallback)(struct evtUnitHandle_t *, s32); /* 0x94: returns 1 when motion completes. */
    f32 wobblePhase;               /* 0x98: wrapping radians used by model scale/rotation wobble. */
    s32 unk9C;                     /* 0x9C */
    struct dds3PathBasicControl_t *pathControl; /* 0xA0: owned path-control work */
    f32 pathSpeed;                 /* 0xA4: signed progress increment */
    u32 flags;                     /* 0xA8 */
    s16 motionState;               /* 0xAC */
    s16 transitionSourceKind;      /* 0xAE */
    s16 motionSubmode;             /* 0xB0 */
    s16 motionTicks;               /* 0xB2 */
    s16 motionParameter;           /* 0xB4: frames in setup; direction scale in updater */
    s16 directionOffset;           /* 0xB6: updater multiplies by 0.01 */
    f32 transitionMotionScale;     /* 0xB8: set by evtSetWaitMotionSpeed */
#ifdef VERSION_DDS1
    s16 pendingMotionIndex;        /* 0xBC: script-selected motion for the next blend */
    s16 motionBlendLeadFrames;     /* 0xBE: frames before the motion blend begins */
    s16 motionBlendDurationFrames; /* 0xC0: duration of the motion blend */
#else
    s16 pendingMotionIndex;        /* 0xBC: pending motion index for the next blend */
    s16 motionBlendLeadFrames;     /* 0xBE: frames before the motion blend begins */
    s16 motionBlendDurationFrames; /* 0xC0: duration of the motion blend */
#endif
    u8 padC2[2];
    s16 motionSetupId;             /* 0xC4: motion selected by evtSetUnitMotion */
    s16 motionSetupStartFrame;     /* 0xC6 */
    s16 motionSetupEndFrame;       /* 0xC8 */
    s16 motionEndFrame;            /* 0xCA: threshold for the timed motion's final phase */
    s16 movingTicks;               /* 0xCC: saturates at 32767 while moving */
    s16 idleTicks;                 /* 0xCE: six idle frames clear movingTicks */
    s8 firstSlot;                  /* 0xD0: consumed by stored-slot activation */
    s8 secondSlot;                 /* 0xD1 */
    u8 padD2;
    u8 shadowLevel;                /* 0xD3: timeline-key value passed to fldDrawShadowDisc. */
    f32 shadowY;                    /* 0xD4: float parameter written by timeline keys */
    u32 referenceAngleCacheFlags;   /* 0xD8: bit 0 marks a valid cached angle. */
    f32 cachedReferenceAngleDegrees; /* 0xDC: cached reference angle in degrees. */
    u8 slotFlags[12];              /* 0xE0 */
    u8 padEC[4];
#ifdef VERSION_DDS1
    s16 tableValues[1];            /* 0xF0: minimum known extent; script index is unchecked */
    u8 padF2[0x16];
    s16 slotA[12];                 /* 0x108: opaque per-slot parameters */
    s16 slotB[12];                 /* 0x120 */
    s16 slotC[12];                 /* 0x138 */
    union {
        s16 unused150;               /* Legacy signed view */
        u16 colorFramesRemaining;  /* 0x150: colour interpolation countdown */
    };
    union {
        s16 directionMode;         /* Legacy signed view */
        u16 directionFramesRemaining; /* 0x152: direction interpolation countdown */
    };
    union {
        u8 pad154[8];
        struct {
            u16 rgbElapsed;
            u16 rgbDuration;
            u16 alphaElapsed;
            u16 alphaDuration;
        };
    };
    s16 transitionElapsed;         /* 0x15C */
    union {
        s16 transitionDuration;    /* Legacy setter view, retaining the stored bits */
        u16 transitionFrameCount;  /* 0x15E: duration consumed as unsigned frames */
    };
    f32 speedY;                    /* 0x160 */
    s16 stepCount;                 /* 0x164 */
    u8 pad166[10];
#endif
#ifdef VERSION_DDS2
    s16 tableValues[12];           /* 0xF0 */
    s16 tableMotionBlendLeadFrames[12]; /* 0x108 */
    s16 tableMotionBlendDurationFrames[12]; /* 0x120 */
    f32 tableMotionFrameStep[12];  /* 0x138 */
    s16 slotA[12];                 /* 0x168: opaque per-slot parameters */
    s16 slotB[12];                 /* 0x180 */
    s16 slotC[12];                 /* 0x198 */
    union {
        s16 unused1B0;               /* Legacy signed view */
        u16 colorFramesRemaining;  /* 0x1B0: colour interpolation countdown */
    };
    union {
        s16 directionMode;         /* Legacy signed view */
        u16 directionFramesRemaining; /* 0x1B2: direction interpolation countdown */
    };
    union {
        u8 pad1B4[8];
        struct {
            u16 rgbElapsed;
            u16 rgbDuration;
            u16 alphaElapsed;
            u16 alphaDuration;
        };
    };
    s16 transitionElapsed;         /* 0x1BC */
    union {
        s16 transitionDuration;    /* Legacy setter view, retaining the stored bits */
        u16 transitionFrameCount;  /* 0x1BE: duration consumed as unsigned frames */
    };
    f32 speedY;                    /* 0x1C0 */
    s16 stepCount;                 /* 0x1C4 */
    u8 pad1C6[10];
#endif
} evtUnitHandle_t;

evtUnitHandle_t *evtCreateUnitTransitionWork(dds3Basic_t *effObj, struct dds3ModelHandle_t *owner);
s32 evtDeleteUnitHandle(evtUnitHandle_t *work);

void evtSetUnitValueTransition(evtUnitHandle_t *unit, dds3Basic_t *target, s32 duration);
void evtSetUnitLightDir(evtUnitHandle_t *unit, s32 duration);
void evtSetUnitColorRGB(evtUnitHandle_t *unit, s32 duration, u32 color);
void evtSetUnitColorA(evtUnitHandle_t *unit, s32 duration, u32 color);
void evtUnitLightOn(evtUnitHandle_t *unit);
evtUnitHandle_t *evtGetUnitHandle2(dds3Basic_t *unit);
void evtSetWaitMotion(evtUnitHandle_t *unit, s32 value);

s32 evtCheckScriptDone(dds3ProcessID_t *task);
void evtDestroyEventPackTask(dds3ProcessID_t *task);

#endif /* EVT_UNIT_H */
