#ifndef BTL_H
#define BTL_H

#include "common.h"
#include "btl_task.h"

/* Battle effect actor, flags and timing (0x18); DDS1/2 identical views. */
typedef struct BattleEffectState {
    u32 actor, flags, value; /* +0x00/+0x04/+0x08 */
    u16 timer;               /* +0x0C */
    u8 active, phase;        /* +0x0E/+0x0F */
    u32 effect;              /* +0x10 */
    f32 speed;               /* +0x14 */
} BattleEffectState;

typedef struct BtlUnit BtlUnit;



#ifdef VERSION_DDS2
/* Battle unit, DDS2. Six units declared this locally at varying depths of
 * detail; the offsets below are the union of those, machine-checked against
 * each unit's own disassembly (every unit's assembly reads owner at 0x108, the
 * flags pair at 0x110/0x114, statBits at 0x120, the extension pointer at 0x340
 * and the actor-list link at 0x364, so all five are the same object).
 * Fields no unit could name stay unkNN/padNN. */
typedef struct BtlUnitExt {
    u8 pad0[0x68];
    s32 unk68;
    u8 pad6C[0x18];
    struct BtlExtModel *model;
    u8 pad88[4];
    struct BtlUnitInfo *info;
    u8 pad90[0x18];
    u32 flagsA8;
} BtlUnitExt;

typedef struct BtlUnit {
    s32 state;
    u8 pad4[4];
    u32 seqFlags;
    u32 unkC;
    s32 stateTime;
    s32 unk14;
    struct BtlUnit *link18;
    u8 pad1C[0x14];
    f32 positionX;   /* 0x30: current unit position */
    f32 positionY;   /* 0x34 */
    f32 unk38;
    u8 pad3C[0x14];
    f32 unk50;
    u32 baseColor;
    u8 pad58[0x18];
    f32 orientation[4]; /* 0x70: quaternion converted to a rotation matrix */
    f32 scale;      /* 0x80 */
    u32 overlayColor;
    f32 positionZOffset;
    u8 pad8C[4];
    f32 bodyOffset[4]; /* 0x90: scaled and rotated into the unit's body position */
    f32 muzzleOffset[4]; /* 0xA0 */
    f32 height;     /* 0xB0 */
    f32 reach;      /* 0xB4 */
    u8 padB8[4];
    f32 unkBC;
    f32 unkC0;
    s32 resourceKind; /* 0xC4 */
    s32 resourceIndex; /* 0xC8 */
    u8 unkCC;
    u8 padCD[0x1B];
    u32 unkE8;
    s32 unkEC;
    s32 unkF0;
    f32 fF4;
    s16 unkF8;
    s16 unkFA;
    s32 effectIndex; /* 0xFC */
    s32 effectParameter; /* 0x100 */
    f32 effectScale; /* 0x104 */
    u64 owner;      /* 0x108: compared against the battle command's unit ID */
    union {
        u64 flags64; /* 0x110 */
        struct {
            u32 flags; /* 0x110 */
            u32 stateFlags; /* 0x114 */
        };
    };
    s32 gunResourceFlags;
    u8 lookupId;
    u8 pad11D[3];
    u16 statBits; /* 0x120: queried for bit 0x2000; base of the stat accessors */
    u16 unk122;   /* 0x122: script-controlled unit parameter */
    u16 mode;     /* 0x124 */
    u8 pad126[8];
    u16 conditionFlags; /* 0x12E */
    u8 pad130a[4];
    u16 unk134;   /* 0x134: queried unit parameter */
    u8 pad130[0x3C];
    u16 unk172;
    struct BtlUnit *prev; /* 0x174 */
    struct BtlUnit *next; /* 0x178 */
    u8 pad17C[0x168];
    u8 unk2E4;
    u8 pad2E5[0x2B];
    s32 unk310;
    s32 unk314;
    struct SoundResourceNode *node318;
    struct SoundResourceLink *link31C;
    struct SoundLink *link320;
    struct ActiveSoundNode *node324;
    s32 unk328;
    void *gunResource;
    u16 unk330;
    u8 pad332[2];
    s32 unk334;
    u8 pad338[4];
    s32 effectObject; /* 0x33C: effect whose first inner vector becomes the origin */
    BtlUnitExt *ext; /* 0x340 */
    s32 unk344;
    u8 pad348[4];
    s32 unk34C;
    s32 unk350;
    u8 pad354[8];
    u32 handle35C;
    struct BtlUnit *previousActor; /* 0x360 */
    struct BtlUnit *nextActor; /* 0x364 */
} BtlUnit;
#endif /* VERSION_DDS2 */

#endif /* BTL_H */
