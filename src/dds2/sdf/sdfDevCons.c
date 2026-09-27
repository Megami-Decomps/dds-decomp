#include "common.h"

typedef struct DevConsState {
    u8 pad0[4]; /* 0x0 */
    struct DevConsState *unk4; /* 0x4 */
    u16 unk8; /* 0x8 */
    u16 unkA; /* 0xA */
    s16 unkC; /* 0xC */
    s16 unkE; /* 0xE */
    u16 unk10; /* 0x10 */
    s16 unk12; /* 0x12 */
    u8 unk14; /* 0x14 */
    u8 pad15; /* 0x15 */
    u8 unk16; /* 0x16 */
    u8 unk17; /* 0x17 */
    u8 pad18[4]; /* 0x18 */
    u8 *unk1C; /* 0x1C */
} DevConsState;

extern void *memmove(void *dst, const void *src, u32 n);

extern void *memset(void *dst, s32 c, u32 n);

void func_0033CE08(DevConsState *arg0, s32 arg1, s32 arg2);

void func_00360E78(void *arg0, const char *arg1, void *arg2);

void func_0033CD78(DevConsState *arg0) {
    if (arg0->unk12 == arg0->unkE - 1) {
        s32 t = arg0->unkC * 2;
        s32 n = t * arg0->unk12;
        if (arg0->unk12 > 0) {
            memmove(arg0->unk1C, (u8 *)arg0->unk1C + t, n);
        }
        memset((u8 *)arg0->unk1C + n, 0, t);
    } else {
        arg0->unk12 = (u16)arg0->unk12 + 1;
    }
    arg0->unk10 = 0;
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033CE08);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033CF80);

s32 func_0033CF98(DevConsState *arg0, const char *fmt, ...) {
    char buf[0x200];
    __builtin_va_list ap;
    char *p;
    s32 i = 0;
    s32 c;
    __builtin_stdarg_start(ap, fmt);
    func_00360E78(buf, fmt, ap);
    c = buf[0];
    if (c != 0) {
        do {
            i++;
            func_0033CE08(arg0, c, arg0->unk16);
            p = buf + i;
            c = *p;
        } while (c != 0);
    }
    return i;
}

void func_0033D040(DevConsState *arg0, s16 arg1, s16 arg2) {
    arg0->unk10 = arg1;
    arg0->unk12 = arg2;
}

void func_0033D050(DevConsState *arg0, u8 arg1) {
    arg0->unk16 = arg1;
}

u8 func_0033D058(DevConsState *arg0) {
    return arg0->unk14;
}

void func_0033D060(DevConsState *arg0, u8 arg1) {
    arg0->unk14 = arg1;
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D068);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D0D8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D1A8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D2D8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D480);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D568);
