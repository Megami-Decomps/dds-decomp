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

#ifdef VERSION_DDS1
/* Battle unit, DDS1 (0x348). Retail accessors distinguish world rotation at
 * +0x40 from the orientation quaternion at +0x70; body/muzzle offsets are
 * +0x90/+0xA0 respectively. Effect origin/scene extension are +0x31C/+0x320.
 * The actor chain ends at +0x344. Command state/action words are a different
 * object and must not be interpreted through this unit layout. */
typedef struct BtlUnit {
    u8 pad00[0x30];
    f32 position[4]; /* 0x30: world position */
    f32 rotation[4]; /* 0x40: world rotation passed to btlSetUnitRotation */
    u8 pad50[4];
    u32 baseColor; /* 0x54: RGB restored after temporary battle effects */
    u8 pad58[0x18];
    f32 orientation[4]; /* 0x70: quaternion converted to a VU matrix */
    f32 scale; /* 0x80 */
    u8 pad84[4];
    f32 zOffset; /* 0x88: added before rotating a local offset */
    u8 pad8C[4];
    union {
        /* Accessor names describe caller intent: GetMuzzlePosVU reads this
         * body-reach offset; GetBodyPosVU reads muzzleOffset at +0xA0. */
        f32 bodyOffset[4]; /* 0x90: same field naming as DDS2 */
        struct {
            /* Some setup paths write zero as integer bits, not float stores. */
            s32 bodyOffsetXBits;
            f32 bodyOffsetY;
            f32 bodyOffsetZ;
            s32 bodyOffsetWBits;
        };
    };
    f32 muzzleOffset[4]; /* 0xA0: read by btlUnitGetBodyPosVU */
    f32 height; /* 0xB0 */
    f32 reach; /* 0xB4 */
    f32 unkB8; /* Setup stores mirror height/reach; meaning otherwise unknown. */
    f32 unkBC;
    u8 padC0[4];
    s32 resourceKind; /* 0xC4: selects the actor resource table, as in DDS2 */
    u32 species; /* 0xC8 */
    u8 padCC[0x14];
    s32 displaySpecies; /* 0xE0 */
    u8 padE4[4];
    u32 updateFlags; /* 0xE8: bit 1 forces the actor update's reset path */
    s32 unkEC;
    s32 effectState; /* 0xF0 */
    u8 padF4[4];
    s16 effectTimerA; /* 0xF8 */
    s16 effectTimerB; /* 0xFA */
    s32 effectArgA; /* 0xFC */
    s32 effectArgB; /* 0x100 */
    f32 effectValue; /* 0x104 */
    u64 identity; /* 0x108: copied to effect tasks and compared to command IDs */
    /* Keep the status pair flat: 32-bit updates and address-based 64-bit scans
     * coexist in retail. Preserve the original separate-member layout. */
    u32 flags; /* 0x110 */
    u32 stateFlags; /* 0x114 */
    u32 gunResourceFlags; /* 0x118 */
    u8 lookupId; /* 0x11C */
    u8 pad11D[3];
    u16 statBits; /* 0x120: base of the unit stat accessors */
    u16 unk122; /* 0x122: script-controlled unit parameter */
    u16 mode; /* 0x124 */
    u16 hp; /* 0x126 */
    u16 maxHp; /* 0x128 */
    u16 unk_12A;
    u16 unk12C; /* 0x12C: reset value for the unit parameter at 0x12A */
    u16 conditionFlags; /* 0x12E */
    u8 pad130[4];
    u16 actionTime; /* 0x134 */
    u8 pad136[0x17A];
    s16 actionSlot; /* 0x2B0 */
    u8 pad2B2[0x66];
    u8 firstCountdown; /* 0x318: linked-effect destruction decrements this */
    u8 secondCountdown; /* 0x319 */
    u8 pad31A[2];
    u32 effectObject; /* 0x31C: supplies the effect's first inner vector */
    BtlUnitModel *ext; /* 0x320: +0x8C points to model flags; also an event handle */
    u8 pad324[0x1C];
    struct BtlUnit *previousActor; /* 0x340 */
    struct BtlUnit *next; /* 0x344: actor-list link, not a task-chain link */
} BtlUnit;
#endif /* VERSION_DDS1 */



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
    f32 positionZ; /* 0x38 */
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
    u32 updateFlags; /* 0xE8: bit 1 forces the actor update's reset path */
    s32 unkEC;
    s32 effectState; /* 0xF0: same actor effect state as DDS1 */
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
    u16 actionTime; /* 0x134: action timestamp used by the low-HP delay check */
    u8 pad130[0x3C];
    u16 unk172; /* Index into datItemSkillRecords for the default action operand. */
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
    s32 unk328; /* Owner of the actor model's file slots and allocation handles. */
    void *gunResource;
    u16 unk330;
    u8 pad332[2];
    s32 unk334;
    u8 pad338[4];
    s32 effectObject; /* 0x33C: effect whose first inner vector becomes the origin */
    BtlUnitExt *ext; /* 0x340 */
    s32 unk344; /* Alternate SDF model used by the transparency path. */
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
