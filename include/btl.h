#ifndef BTL_H
#define BTL_H

#include "common.h"
#include "btl_task.h"

/* Battle effect actor, flags and timing (0x18); DDS1/2 identical views. */
typedef struct BattleEffectState {
    union {
        u32 owner;
        struct BtlUnit *actor;
        u8 statIndex;
    };
    u32 flags, value;        /* +0x04/+0x08 */
    u16 timer;               /* +0x0C */
    u8 active, phase;        /* +0x0E/+0x0F */
    union {
        u32 effect;
        f32 height;          /* +0x10: effect position's Y component */
    };
    f32 speed;               /* +0x14 */
} BattleEffectState;

typedef struct BtlUnit BtlUnit;

struct EvtUnit;

/* BtlUnit's status pair at +0x110. Retail touches it two ways: 32-bit flags/stateFlags
 * accesses that type-based aliasing sees as plain words (dds1 func_001C9098 hoists a
 * link->unit load above its `flags |=` store), and 64-bit mask tests read through this
 * union, which GCC treats as alias set 0 (btlAccumulateEnemyDefeatRewards keeps the ld
 * after the s32 experienceEarned store; a plain u64 read is hoisted above it). */
typedef union BtlUnitFlagPair {
    u64 bits;
    struct {
        s32 flags;
        u32 stateFlags;
    } words;
} BtlUnitFlagPair;

/* Six-byte status-entry record; each game's battle unit contains seven. */
typedef struct BtlUnitEntrySlot {
    s16 code;
    s16 unk02;
    s16 countdown;
} BtlUnitEntrySlot;

#ifdef VERSION_DDS1
/* Battle unit, DDS1 (0x348). Retail accessors distinguish world rotation at
 * +0x40 from the orientation quaternion at +0x70; body/muzzle offsets are
 * +0x90/+0xA0 respectively. Effect origin/scene extension are +0x31C/+0x320.
 * The actor chain ends at +0x344. Command state/action words are a different
 * object and must not be interpreted through this unit layout. */
typedef struct BtlUnit {
    u8 pad00[0x10];
    f32 colorStart[4]; /* 0x10: first source color used by the effect blend callback. */
    f32 colorEnd[4]; /* 0x20: second source color used by the effect blend callback. */
    f32 position[4]; /* 0x30: world position */
    f32 rotation[4]; /* 0x40: world rotation passed to btlSetUnitRotation */
    f32 effectScale; /* 0x50: scales the SDK-provided actor effect vectors. */
    u32 baseColor; /* 0x54: RGB restored after temporary battle effects */
    f32 unk58;
    u8 pad5C[4];
    f32 currentPosition[4]; /* 0x60: position retained by the unit setter */
    f32 orientation[4]; /* 0x70: quaternion converted to a VU matrix */
    f32 scale; /* 0x80 */
    u32 overlayColor; /* 0x84: packed transient actor color. */
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
    f32 cameraRadius; /* 0xC0: scaled actor clearance used by camera framing. */
    s32 resourceKind; /* 0xC4: selects the actor resource table, as in DDS2 */
    u32 species; /* 0xC8 */
    u8 unkCC;
    u8 padCD[3];
    s32 modelId; /* 0xD0: model passed to btlCreateModelChangeTask by the gun-change command */
    s32 modelVariant; /* 0xD4: its variant argument */
    u8 padD8[4];
    s32 unkDC; /* 0xDC: model for the slot-0x10 model-change command, with displaySpecies */
    s32 displaySpecies; /* 0xE0 */
    u8 padE4[4];
    u32 updateFlags; /* 0xE8: bit 1 forces the actor update's reset path */
    s32 unkEC;
    s32 effectState; /* 0xF0 */
    f32 motionRate; /* 0xF4: stored rate applied to the owner's primary SDK motion. */
    s16 effectTimerA; /* 0xF8 */
    s16 effectTimerB; /* 0xFA */
    s32 effectArgA; /* 0xFC */
    s32 effectArgB; /* 0x100 */
    f32 effectValue; /* 0x104 */
    u64 identity; /* 0x108: copied to effect tasks and compared to command IDs */
    s32 flags; /* 0x110: arithmetic-shift accessors use the signed low word. */
    u32 stateFlags; /* 0x114 */
    u32 gunResourceFlags; /* 0x118 */
    u8 lookupId; /* 0x11C: retail lookup consumers use unsigned byte loads. */
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
    s8 baseStats[5]; /* 0x136: signed stat bank copied to the saved party entry. */
    u8 pad13B[0x37];
    u16 bedAssetIndex; /* 0x172: SDK operand resolution selects the BED asset. */
    u8 pad174[0x138];
    s16 commandKind; /* 0x2AC: queued command kind, paired with actionSlot. */
    u16 unk2AE; /* 0x2AE: saved with commandKind and actionSlot in the party record. */
    s16 actionSlot; /* 0x2B0 */
    u8 pad2B2[0x12];
    u8 unk2C4; /* 0x2C4: saved party-entry index, read with lbu. */
    u8 pad2C5;
    BtlUnitEntrySlot entrySlots[7]; /* 0x2C6 */
    s32 selectedEntryIndex; /* 0x2F0: -1 denotes no selected entry. */
    u32 unk2F4;
    s32 resourceNode;
    s32 resourceLink;
    s32 link;
    s32 listNode;
    struct SoundSlotOwner *soundSlotOwner; /* 0x308: shared category/id motion-SE owner. */
    void *gunResource; /* 0x30C */
    u16 unk310; /* 0x310: battle-effect entry conditions test bits 0..2. */
    u8 pad312[2];
    s32 unk314;
    u8 firstCountdown; /* 0x318: linked-effect destruction decrements this */
    u8 secondCountdown; /* 0x319 */
    u8 pad31A[2];
    u32 effectObject; /* 0x31C: supplies the effect's first inner vector */
    struct EvtUnit *ext; /* 0x320: the event manager's complete 0x170-byte work. */
    s32 transparencyModel; /* 0x324: alternate SDF model retained during transparency. */
    struct BtlUnit *mirror; /* 0x328: unit drawn from this unit's transparency packet buffer */
    s32 unk32C;
    s32 unk330;
    u8 pad334[8];
    u32 handle; /* 0x33C */
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
typedef struct BtlUnit {
    s32 state;
    u16 unk4; /* Written as 1 by the action-sequence constructor. */
    u8 pad6[2];
    u32 seqFlags;
    u32 unkC;
    s32 stateTime;
    s32 unk14;
    struct BtlUnit *link18;
    u8 pad1C[0x14];
    f32 position[4]; /* 0x30: world position */
    f32 rotation[4]; /* 0x40: world rotation passed to btlSetUnitRotation */
    f32 unk50;
    u32 baseColor;
    f32 unk58;
    u8 pad5C[4];
    f32 currentPosition[4]; /* 0x60: position retained by the unit setter */
    f32 orientation[4]; /* 0x70: quaternion converted to a rotation matrix */
    f32 scale;      /* 0x80 */
    u32 overlayColor;
    f32 positionZOffset;
    u8 pad8C[4];
    f32 bodyOffset[4]; /* 0x90: scaled and rotated into the unit's body position */
    f32 muzzleOffset[4]; /* 0xA0 */
    f32 height;     /* 0xB0 */
    f32 reach;      /* 0xB4 */
    f32 unkB8; /* 0xB8: formation setup copies height here, alongside reach at +0xBC. */
    f32 unkBC;
    f32 unkC0;
    s32 resourceKind; /* 0xC4 */
    s32 resourceIndex; /* 0xC8 */
    u8 unkCC;
    u8 padCD[3];
    s32 modelId; /* 0xD0: model passed to btlCreateModelChangeTask by the gun-change command */
    s32 modelVariant; /* 0xD4: its variant argument */
    u8 padD8[4];
    s32 unkDC; /* 0xDC: mode actor selector paired with combatantKind. */
    s32 combatantKind; /* 0xE0: display/command kind before special-mode canonicalization */
    u8 padE4[4];
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
    s32 flags; /* 0x110: same signed status word as DDS1. */
    u32 stateFlags; /* 0x114 */
    s32 gunResourceFlags;
    u8 lookupId;
    u8 pad11D[3];
    u16 statBits; /* 0x120: queried for bit 0x2000; base of the stat accessors */
    u16 unk122;   /* 0x122: script-controlled unit parameter */
    u16 mode;     /* 0x124 */
    u16 hp;      /* 0x126 */
    u16 maxHp;   /* 0x128 */
    u16 unk12A;
    u16 unk12C;
    u16 conditionFlags; /* 0x12E */
    u8 pad130a[4];
    u16 actionTime; /* 0x134: action timestamp used by the low-HP delay check */
    u8 pad136[0xC];
    u16 cards[8]; /* 0x142: eight skill IDs searched for the active card range. */
    u8 pad152[0x20];
    u16 unk172; /* Index into datItemSkillRecords for the default action operand. */
    struct BtlUnit *prev; /* 0x174 */
    struct BtlUnit *next; /* 0x178 */
    u8 pad17C[0x154];
    s16 actionSlot; /* 0x2D0: same per-unit queued action operand as DDS1 */
    u8 pad2D2[0x12];
    u8 unk2E4;
    u8 pad2E5;
    BtlUnitEntrySlot entrySlots[7]; /* 0x2E6: btlClearActorEntrySlot clears each signed record. */
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
    struct EvtUnit *ext; /* 0x340: the event manager's complete 0x1D0-byte work. */
    s32 unk344; /* Alternate SDF model used by the transparency path. */
    struct BtlUnit *mirror; /* 0x348: created and destroyed by the model-change task. */
    s32 unk34C;
    s32 unk350;
    u8 pad354[8];
    u32 handle35C;
    struct BtlUnit *previousActor; /* 0x360 */
    struct BtlUnit *nextActor; /* 0x364 */
} BtlUnit;
typedef char BtlUnitMirrorOffsetCheck[((u32)&((BtlUnit *)0)->mirror == 0x348) ? 1 : -1];
typedef char BtlUnitSizeCheck[(sizeof(BtlUnit) == 0x368) ? 1 : -1];
#endif /* VERSION_DDS2 */

/* Whole status pair (flags low, stateFlags high) for the 64-bit mask tests. */
static inline u64 btlUnitStatusPair(BtlUnit *unit) {
    return ((BtlUnitFlagPair *)&unit->flags)->bits;
}

#endif /* BTL_H */
