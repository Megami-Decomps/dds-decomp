#include "common.h"

extern void func_0025FD78(s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_00101958();

extern void func_0026C900(void);

extern s32 kwlnFadeIsActive(void);

extern void func_002C42C0(s32 *, char *);

extern char D_003CE6AC[];

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265AD8);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265E78);

s64 func_00265EE8(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

u32 func_00265F30(void) {
    func_0026C948(1);
    func_0026C5B8(0xd);
    return 1;
}

u32 func_00265F58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265F60);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265FE8);

s64 func_00266038(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266080);

u32 func_002660D8(void) {
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266108);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266188);

INCLUDE_ASM(const s32, "game/code_00265AD8", evtDispatchSync);

u32 func_00266210(void) {
    return 1;
}

u32 func_00266218(void) {
    return 1;
}

u32 func_00266220(void) {
    return 0;
}

u32 func_00266228(void) {
    return 0;
}

u32 func_00266230(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266238);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266320);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_002663D8);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D50);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D60);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D70);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D80);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D90);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DA0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DB0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DC0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DD0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DE0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424E10);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266460);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424E48);

