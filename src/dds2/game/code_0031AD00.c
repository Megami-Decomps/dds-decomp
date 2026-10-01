#include "common.h"

extern void itfSetFadeMode(void *, s32, s32);
extern void itfQueueFadeMode(void *, u32, u32, u32);
extern void func_0031EEE8(void *, u32);
extern void mnuDrawFadeSequenceOffset(void *);
extern void itfDrawFadeGlyphTriplet(void *);
extern void mnuDrawFadeSequenceTwo(void *);
void func_0031AD00(u8 *object) {
    s16 mode = *(s16 *)(object + 0x98);

    switch (mode) {
    case 0:
        itfSetFadeMode(object + 0x168, 1, 8);
        itfQueueFadeMode(object + 0x168, 0, 8, 0x78);
        func_0031EEE8(object + 0x168,
                      *(s16 *)(object + 0x96) + 1);
        return;
    case 1:
        mnuDrawFadeSequenceOffset(object + 0x168);
        return;
    case 5:
        itfDrawFadeGlyphTriplet(object + 0x184);
        return;
    case 10:
    case 11:
    case 12:
        mnuDrawFadeSequenceTwo(object + 0x19C);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031ADD8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AE48);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AEB8);

void func_0031AF58(s32 object) {
    *(u32 *)(object + 0x1d4) = 0;
}

void func_0031AF60(void) {
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AF68);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B080);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B0F8);

