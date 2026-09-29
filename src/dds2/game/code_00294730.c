#include "common.h"

extern void func_00295D38();

extern void func_002958B0();

extern void func_00296018(s32, s32, s32, u8 *, s32);

extern void func_002960F0(s32, s32, s32, s32, u8 *, s32);

typedef struct EventSpriteObject {
    u8 pad00[8];
    s32 type;
} EventSpriteObject;

u32 func_00294730(EventSpriteObject *object) {
    u32 result;

    result = 0;
    if ((object->type == 1) || (object->type == 3)) {
        result = 0x3a;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00294758);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294930);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294B40);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294C68);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294D50);

INCLUDE_ASM(const s32, "game/code_00294730", func_00294EB8);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295030);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295400);

INCLUDE_ASM(const s32, "game/code_00294730", func_002958B0);

INCLUDE_ASM(const s32, "game/code_00294730", func_00295D38);

void mnuDrawIfActive(s32 a, s32 b, s32 c, u8 *obj, s32 d) {
    u8 *inner = *(u8 **)(obj + 0x18);

    if (*(s32 *)(inner + 0x20) != 0) {
        func_00296018(a, b, c, inner, d);
        func_002960F0(a, b, c, 0, obj, d);
        *(u32 *)(obj + 4) |= 4;
    }
}

INCLUDE_ASM(const s32, "game/code_00294730", func_00296018);

INCLUDE_ASM(const s32, "game/code_00294730", func_002960F0);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296298);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296430);

INCLUDE_ASM(const s32, "game/code_00294730", func_002967A0);

INCLUDE_ASM(const s32, "game/code_00294730", func_002968B8);

INCLUDE_ASM(const s32, "game/code_00294730", func_002969D8);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296AF8);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296B48);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296C58);

INCLUDE_ASM(const s32, "game/code_00294730", func_00296D90);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437968);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437970);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437978);

INCLUDE_SDATA(const s32, "game/code_00294730", D_00437980);

