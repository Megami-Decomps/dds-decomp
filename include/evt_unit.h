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
    u8 padD2;
    u8 unkD3;                     /* 0xD3: byte parameter written by timeline keys */
    f32 unkD4;                    /* 0xD4: float parameter written by timeline keys */
    u32 unkD8Flags;                 /* 0xD8: tested and set at bit 0 */
    f32 unkDC;                      /* 0xDC: stored angle/state value */
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
    s16 unk108[12];                /* 0x108 */
    s16 unk120[12];                /* 0x120 */
    f32 unk138[12];                /* 0x138 */
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
} EvtUnit;

/* BE resource files have a 0x20-byte header and fixed 0x20-byte records. */
typedef struct EvtPackEntry {
    s32 kind;                     /* 0x00 */
    u8 pad04[8];
    u32 dataOffset;               /* 0x0C: relative to the retained file base */
    s32 secondaryResourceId;      /* 0x10 */
    s32 resourceId;               /* 0x14 */
    u8 pad18[8];
} EvtPackEntry;

typedef struct EvtPackHeader {
    u8 pad00[0x10];
    s32 entryCount;               /* 0x10 */
    u8 pad14[0xC];
    EvtPackEntry entries[0];      /* 0x20 */
} EvtPackHeader;

/* The camp constructor allocates/clears 0x48 bytes and publishes this state to
 * the event-pack load and release callbacks. */
typedef struct EvtPackLoadState {
    s32 eventId;                  /* 0x00 */
    s32 loaded;                   /* 0x04 */
    s32 fileHandle;               /* 0x08 */
    s32 resourceHandle;           /* 0x0C */
    u8 *data;                     /* 0x10 */
    EvtPackHeader *header;        /* 0x14 */
    EvtPackEntry *entries;        /* 0x18 */
    u8 *entryPoint;               /* 0x1C */
    u8 pad20[4];
    s32 sceneAllocation1;         /* 0x24 */
    u8 pad28[8];
    s32 sceneAllocation2;         /* 0x30 */
    u8 pad34[4];
    s32 effect72;                 /* 0x38 */
    s32 effect71;                 /* 0x3C */
    s32 effect76;                 /* 0x40 */
    s32 effect75;                 /* 0x44 */
} EvtPackLoadState;

s32 evtTickPackLoad(void);
void evtReleaseEventPackResources(void);

#endif /* EVT_UNIT_H */
