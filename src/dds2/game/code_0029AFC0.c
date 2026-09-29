#include "common.h"

extern s32 func_00101958();

extern void func_0026C900(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_0029AA48(s32);

extern void func_0029AC20(s32, s32);

extern s32 func_002C7168(void);

extern s64 func_0026C768(void);

extern void func_002C42C0(s32 *, char *);

extern char D_003D64C8[];

typedef struct MenuSumBytes {
    u8 pad00[0x16];
    s8 values[5];
} MenuSumBytes;

typedef struct MenuSumTable {
    u8 pad00[0x3F4];
    s32 values[5];
} MenuSumTable;

s32 mnuCheckTableSums(MenuSumBytes *bytes, MenuSumTable *table) {
    s32 *tableValues = table->values;
    s8 *byteValues = bytes->values;
    s32 index = 0;

    do {
        s32 total = *byteValues + *tableValues;

        byteValues++;
        tableValues++;
        if (total < 0x63) {
            return 0;
        }
        index++;
    } while (index < 5);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B008);

void func_0029B320(s32 request) {
    s32 context = func_00101958();

    func_0029AA48(context);
    func_0029AC20(context, 1);
    func_002C4038(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_0029B378(s32 request) {
    s32 context = func_00101958();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, request);
}

u32 func_0029B3C0(void) {
    s32 context;

    context = func_00101958();
    func_002C0CF8(*(u32 *)(context + 0xad34), 0xffffffffffffffff);
    func_0026C5B8(0x17);
    func_0026C648(0);
    func_0026C618(0xa3);
    return 1;
}

u32 func_0029B410(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B418);

void func_0029B600(s32 request) {
    s32 context = func_00101958();

    func_0029AA48(context);
    func_0029AC20(context, 1);
    func_002C4038(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_0029B658(s32 request) {
    s32 context = func_00101958();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B6A0);

u32 func_0029B778(void) {
    return 1;
}

s64 func_0029B780(u64 request) {
    s32 context = func_00101958();
    s32 *state = (s32 *)(context + 0x54);
    s64 result;

    func_002C7168();
    result = func_002C4038(context + 8, state, 0, request);
    if (result == 0) {
        if ((*state == 0) && (result = func_0026C768(), result == 0)) {
            func_002C42C0(state, D_003D64C8);
        }
        result = 0;
    }
    return result;
}

void func_0029B810(s32 request) {
    s32 context = func_00101958();

    func_0029AA48(context);
    func_0029AC20(context, 0);
    func_002C4038(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_0029B868(s32 request) {
    s32 context = func_00101958();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B8B0);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B950);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029BB28);

u32 func_0029BBC0(void) {
    func_0026C710();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029BBE0);

INCLUDE_SDATA(const s32, "game/code_0029AFC0", D_004379B8);

