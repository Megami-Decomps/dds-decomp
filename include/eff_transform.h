#ifndef EFF_TRANSFORM_H
#define EFF_TRANSFORM_H

#include "common.h"

enum { EFF_WORLD_KIND_COUNT = 18 };

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

typedef struct EffWorldNode EffWorldNode;

/* Separate 0xD0-byte allocation: DDS1 effObjInnerCreate (0010F570).
 * VU0 vector transfers and scalar camera reads use the same float vectors. */
typedef struct ObjectTransform {
    u128 matrix[4];
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
    f32 smoothedPosition[4];
    f32 savedPosition[4];
    f32 savedRotation[4];
    f32 savedScale[4];
    f32 unkB0[4];
    u32 flags;
    f32 radius;
    u32 unkC8;
    u8 padCC[4];
} ObjectTransform;

/* D_003299C0 selects six-word tables; D_00329A20 is kind 1.
 * The last two words in every populated retail table are zero. */
typedef struct EffWorldOps {
    s32 (*create)(EffWorldNode *);
    void (*destroy)(EffWorldNode *);
    s32 (*update)(EffWorldNode *);
    s32 (*draw)(EffWorldNode *);
    u32 unused10;
    u32 unused14;
} EffWorldOps;

/* Kind-1 value cursors use these halfwords at 00110490. The same prefix is
 * used by separate 0x10-byte world index nodes (0010FEC8). */
typedef struct WorldValueIndices {
    s16 headIndex;
    s16 tailIndex;
    s16 cursorIndex;
    u16 entryCount;
} WorldValueIndices;
typedef char WorldValueIndices_size_must_be_0x08[(sizeof(WorldValueIndices) == 0x08) ? 1 : -1];

/* Complete 68-byte (0x44, not 0x68) allocation from
 * dds3CreateWorldNodeForKind at DDS1 0010F418 / DDS2 0010F640.
 * Its kind byte at +0x0F determines both identity and payload interpretation. */
struct EffWorldNode {
    u32 word0;
    u32 key; /* Kind 2: evtSpawnActionObj2 at 00111188. */
    u32 value; /* Scalar or native caption address, selected by kind. */
    u32 kindTag; /* Kind occupies the high byte; constructor clears low 24 bits. */
    EffWorldOps *ops;
    u32 word14;
    void *data; /* Kind-specific payload; never the separately allocated inner. */
    ObjectTransform *inner;
    EffWorldNode *next;
    EffWorldNode *previous;
    u32 word28;
    u32 word2C;
    EffWorldNode *owner; /* Per-kind list membership: 00110928. */
    u32 color;
    u8 unk38[0x0C];
};

typedef char EffWorldNode_size_must_be_0x44[(sizeof(EffWorldNode) == 0x44) ? 1 : -1];
typedef char EffWorldOps_size_must_be_0x18[(sizeof(EffWorldOps) == 0x18) ? 1 : -1];
typedef char ObjectTransform_size_must_be_0xD0[(sizeof(ObjectTransform) == 0xD0) ? 1 : -1];

#endif
