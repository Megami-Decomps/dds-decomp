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

#endif /* MNU_SPRITE_RESOURCE_H */
