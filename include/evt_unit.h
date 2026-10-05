#ifndef EVT_UNIT_H
#define EVT_UNIT_H

#include "common.h"

struct EvtUnitMotion;

/* Owner tested by motion-idle and model-status commands. Its data record keeps
 * the transition-work backlink at +0x80; the motion record is separate. */
typedef struct EvtUnitOwner {
    u32 flags;                    /* 0x00: script commands toggle bit 0 */
    u8 pad04[0x14];
    void *data;                   /* 0x18 */
    struct EvtUnitMotion *motion; /* 0x1C */
} EvtUnitOwner;

/* Effect-vector data saved/restored during the motion dry run. Planar aim
 * passes orientation to the quaternion-to-matrix VU routine. */
typedef struct EvtEffData {
    u8 pad00[0x40];
    f32 position[4];               /* 0x40 */
    f32 orientation[4];            /* 0x50 */
} EvtEffData;

/* Target of the unit's vector updates, shared by planar aim and the manager. */
typedef struct EvtEffObj {
    u8 pad00[0x1C];
    EvtEffData *data;              /* 0x1C */
} EvtEffObj;

/* Event motion work, not the world-list node which contains its address.
 * Setup opcodes pass this same object to evtPrepareUnitMotionState and
 * evtConfigureUnitMotionSlot; the direction updater passes it to planar aim.
 * targetVector is four floats even when copied as a quadword by VU0/MMI macros.
 * The manager copies the entire object by value: 0x170 (DDS1), 0x1D0 (DDS2).
 * No stronger alignment than the manager's original float layout is implied. */
typedef struct EvtUnit {
    u32 color;                     /* 0x00 */
    s32 objectId;                  /* 0x04: returned to scripts */
    u8 pad08[8];
    f32 vec10[4];                  /* 0x10 */
    f32 vec20[4];                  /* 0x20 */
    f32 vec30[4];                  /* 0x30 */
    u8 pad40[0x10];
    u32 color50;                   /* 0x50 */
    u8 pad54[0xC];
    u32 color60;                   /* 0x60: packed color retained during RGB/alpha transitions */
    u8 pad64[4];
    s32 endpointWorkAddress;       /* 0x68: owned allocation used for endpoint setup */
    u32 value;                     /* 0x6C */
    f32 targetVector[4];           /* 0x70 */
    EvtEffObj *effObj;             /* 0x80 */
    s32 currentTransitionValue;    /* 0x84 */
    s32 previousTransitionValue;   /* 0x88 */
    EvtUnitOwner *owner;           /* 0x8C */
    void *linkedUnit;              /* 0x90: retained source of a transition */
    s32 unk94;                     /* 0x94 */
    s32 unk98;                     /* 0x98 */
    s32 unk9C;                     /* 0x9C */
    s32 pathHandle;                /* 0xA0: freed when replacing the path */
    f32 pathSpeed;                 /* 0xA4: signed progress increment */
    u32 flags;                     /* 0xA8 */
    s16 motionState;               /* 0xAC: idle 0, source 1, motion 2, vector 3, value 4 */
    s16 transitionSourceKind;      /* 0xAE */
    s16 motionSubmode;             /* 0xB0 */
    s16 motionTicks;               /* 0xB2 */
    s16 motionParameter;           /* 0xB4: frames in setup; direction scale in updater */
    s16 directionOffset;           /* 0xB6: updater multiplies by 0.01 */
    f32 unkB8;                     /* 0xB8: stored motion-scale parameter */
    u16 unkBC;                     /* 0xBC: stored script/motion parameter */
    s16 unkBE;                     /* 0xBE: first stored short parameter */
    s16 unkC0;                     /* 0xC0: second stored short parameter */
    u8 padC2[2];
    s16 unkC4;                     /* 0xC4 */
    s16 unkC6;                     /* 0xC6 */
    s16 unkC8;                     /* 0xC8 */
    u8 padCA[6];
    s8 firstSlot;                  /* 0xD0: consumed by stored-slot activation */
    s8 secondSlot;                 /* 0xD1 */
    u8 padD2[0xE];
    u8 slotFlags[12];              /* 0xE0 */
    u8 padEC[4];
#ifdef VERSION_DDS1
    s16 tableValues[1];            /* 0xF0: minimum known extent; script index is unchecked */
    u8 padF2[0x16];
    s16 slotA[12];                 /* 0x108: opaque per-slot parameters */
    s16 slotB[12];                 /* 0x120 */
    s16 slotC[12];                 /* 0x138 */
    s16 unused150;                 /* 0x150 */
    s16 directionMode;             /* 0x152 */
    u8 pad154[8];
    s16 transitionElapsed;         /* 0x15C */
    s16 transitionDuration;        /* 0x15E */
    f32 speedY;                    /* 0x160 */
    s16 stepCount;                 /* 0x164 */
    u8 pad166[10];
#endif
#ifdef VERSION_DDS2
    s16 tableValues[12];           /* 0xF0 */
    s16 unk108[12];                /* 0x108 */
    s16 unk120[12];                /* 0x120 */
    f32 unk138[12];                /* 0x138 */
    s16 slotA[12];                 /* 0x168: opaque per-slot parameters */
    s16 slotB[12];                 /* 0x180 */
    s16 slotC[12];                 /* 0x198 */
    s16 unused1B0;                 /* 0x1B0 */
    s16 directionMode;             /* 0x1B2 */
    u8 pad1B4[8];
    s16 transitionElapsed;         /* 0x1BC */
    s16 transitionDuration;        /* 0x1BE */
    f32 speedY;                    /* 0x1C0 */
    s16 stepCount;                 /* 0x1C4 */
    u8 pad1C6[10];
#endif
} EvtUnit;

#endif /* EVT_UNIT_H */
