#include "common.h"

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

extern u32 *D_003BC610;

extern s32 func_002D03F8(s32);

extern void *sdfMemoryGetBlockAddress(s32);

extern f32 func_002E8398(s32);

extern void *memset(void *, s32, u32);

extern u8 D_003BC620[];

void func_0026E160(s32 a, s32 b, s32 c, s32 d, s32 value) {
    mnuDrawSprite(a, b, c, d, 0x60, 0x1b, value);
}


extern void sdfDestroyTaskWork(s32);
extern void func_0026DED0(s32, s32, u8 *, s32);
extern u8 *sdfListRemoveNode(s32, u8 *);

s32 mnuTickMovieGroup(s32 owner, s32 group) {
    u8 *list = *(u8 **)(group + 8);
    u8 *node;

    if (list == NULL) {
        sdfDestroyTaskWork(group);
        return 0;
    }
    do {
        node = *(u8 **)(list + 0x10);
        *(s32 *)(node + 8) = *(s32 *)(node + 8) - 1;
        if (*(s32 *)(node + 8) == *(s32 *)(node + 0xC) - 5 && *(u8 *)(node + 0x12) != 0) {
            func_0026DED0(owner, group, node, *(s8 *)(node + 0x11));
        }
        if (*(s32 *)(node + 8) == 0) {
            list = sdfListRemoveNode(group, list);
        } else {
            list = *(u8 **)(list + 8);
        }
    } while (list != NULL);
    return group;
}

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E240);

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E388);

typedef struct {
    s32 allocation;    /* 0x00 */
    u8 pad04[0x2C];
    s32 lifetime;      /* 0x30 */
    s32 owner;         /* 0x34 */
    u8 pad38[0xC];
    u8 variant;       /* 0x44 */
    u8 sprite;        /* 0x45 */
    u8 pad46[2];
} MovieSpriteResource;

void *mnuCreateMovieSpriteResource(s32 owner, u8 sprite, u8 variant) {
    s32 allocation = func_002D03F8(0x48);
    MovieSpriteResource *resource = sdfMemoryGetBlockAddress(allocation);
    memset(resource, 0, 0x48);
    resource->allocation = allocation;
    resource->owner = owner;
    resource->sprite = sprite;
    resource->variant = variant;
    resource->lifetime = (s32)(func_002E8398(0) * 30.0f + 10.0f);
    return resource;
}

typedef struct {
    s32 allocation;
    s32 tasks[10];
} MovieResourceGroup;

void mnuReleaseMovieResourceGroup(MovieResourceGroup *resources) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (resources->tasks[i] != 0) {
            sdfDestroyTaskWork(resources->tasks[i]);
        }
    }
    func_002D0918(resources->allocation);
}


INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E608);

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E720);

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E798);

void mnuLoadMovieRollSprite(void) {
    D_003BC610[1] = effLoadIndexedResource(D_003BC620, "roll.spr", 0);
}

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E8D8);

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026EA70);

INCLUDE_SDATA(const s32, "game/code_0026E160", D_003BC610);

