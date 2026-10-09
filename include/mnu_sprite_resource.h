#ifndef MNU_SPRITE_RESOURCE_H
#define MNU_SPRITE_RESOURCE_H

#include "common.h"

struct SdfMemBlock;
struct SdfList;

/* Shared owner for the mantra and title-movie sprite schedulers. */
typedef struct MnuSpriteResourceGroup {
    struct SdfMemBlock *allocation; /* 0x00 */
    struct SdfList *tasks[10];      /* 0x04 */
    s32 activeCount;                /* 0x2C */
    s32 spawnCountdown;             /* 0x30 */
    s32 owner;                      /* 0x34 */
    u8 pad38[0x0C];
    u8 variant;                     /* 0x44 */
    u8 sprite;                      /* 0x45 */
    u8 pad46[2];
} MnuSpriteResourceGroup;

typedef char MnuSpriteResourceGroup_size_must_be_0x48[
    (sizeof(MnuSpriteResourceGroup) == 0x48) ? 1 : -1];

/* Interleaved x/y hex-grid steps for the six directions, clockwise from up-right. */
typedef struct HexStepTable {
    s8 offsets[12];
} HexStepTable;

/* One drifting cue; children step one hex cell from their parent. */
typedef struct SpriteSpawnNode {
    s32 x;
    s32 y;
    s32 framesLeft;
    s32 duration;
    s8 direction;
    s8 mode;
    u8 generations;
    u8 variant;
} SpriteSpawnNode;

#endif /* MNU_SPRITE_RESOURCE_H */
