#include "common.h"

extern void func_0026BD80(s32, s32, s32, s32, s32, s32, s32);

extern u32 *D_003BC610;

extern s32 func_002D03F8(s32);

extern void *func_002D03F0(s32);

extern f32 func_002E8398(s32);

extern void *memset(void *, s32, u32);

extern u8 D_003BC620[];

void func_0026E160(s32 a, s32 b, s32 c, s32 d, s32 value) {
    func_0026BD80(a, b, c, d, 0x60, 0x1b, value);
}


INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E188);

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

void *createMovieSpriteResource(s32 owner, u8 sprite, u8 variant) {
    s32 allocation = func_002D03F8(0x48);
    MovieSpriteResource *resource = func_002D03F0(allocation);
    memset(resource, 0, 0x48);
    resource->allocation = allocation;
    resource->owner = owner;
    resource->sprite = sprite;
    resource->variant = variant;
    resource->lifetime = (s32)(func_002E8398(0) * 30.0f + 10.0f);
    return resource;
}

void func_0026E5A0(s32 *resources) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (resources[i + 1] != 0) {
            destroyTaskWork(resources[i + 1]);
        }
    }
    func_002D0918(resources[0]);
}


INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E608);

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E720);

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E798);

void loadMovieRollSprite(void) {
    D_003BC610[1] = func_002BC8F0(D_003BC620, "roll.spr", 0);
}

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026E8D8);

INCLUDE_ASM(const s32, "game/code_0026E160", func_0026EA70);

INCLUDE_SDATA(const s32, "game/code_0026E160", D_003BC610);

