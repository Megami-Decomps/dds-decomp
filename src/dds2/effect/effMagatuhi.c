#include "common.h"

/* Small work area: type id and count, an id block, a result table plus an
 * object released on cleanup. */
typedef struct {
    s32   type;         /* 0x00 effect type (== 3 in func_0018CA30) */
    u16   count04;      /* 0x04 loop count */
    u8    pad06[2];     /* 0x06 */
    void *ptr08;        /* 0x08 id block / source block */
    u8    pad0C[0x0C];  /* 0x0C */
    u32  *out18;        /* 0x18 result table */
    u8    pad1C[4];     /* 0x1C */
    u32  *unk20;        /* 0x20 table written by func_00189B90 */
    u8    pad24[0x10];  /* 0x24 */
    void *unk34;        /* 0x34 released by func_001893C0 */
} EffMagatuhiWork; /* 0x38 */

extern void *effGetHandlerArg(void *arg);

/* Float source block read by func_0018A610. */
typedef struct EffMagatuhiSrc {
    f32 f00, f04, f08;
    u8 pad0C[4];
    f32 f10, f14, f18;
    u8 pad1C[4];
    f32 f20, f24;
} EffMagatuhiSrc; /* 0x28 */

/* Float destination block written by func_0018A610. */
typedef struct EffMagatuhiDst {
    f32 f00, f04, f08;
    u8 pad0C[4];
    f32 f10, f14, f18;
    u8 pad1C[0x18];
    f32 f34;
    u8 pad38[0x18];
    f32 f50;
} EffMagatuhiDst; /* 0x54 */

void effMagatuhiReleaseResource(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x34));
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191010);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191450);

void func_001917C8(s32 arg0, s32 arg1, u32 arg2) {
    *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 0x20)) = arg2;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001917E0);

void func_001918B8(void) {
    func_00190DF8();
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001918D0);

void func_00191A98(s32 arg0) {
    effMathReleaseWorkResource(*(u32 *)(arg0 + 0x180));
    func_00190DB0(*(u32 *)(arg0 + 0x18c));
    func_003297C8(*(u32 *)(arg0 + 400));
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191AD0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191CD0);

void effMagatuhiCopyFloatBlock(EffMagatuhiWork *work, EffMagatuhiSrc *src) {
    EffMagatuhiDst *dst = effGetHandlerArg(work->ptr08);

    dst->f00 = src->f00;
    dst->f04 = src->f04;
    dst->f08 = src->f08;
    dst->f34 = src->f20;
    dst->f10 = src->f10;
    dst->f14 = src->f14;
    dst->f18 = src->f18;
    dst->f50 = src->f24;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001922B0);

void func_00192438(s32 arg0) {
    effMathReleaseWorkResource(*(u32 *)(arg0 + 0x184));
    func_00190DB0(*(u32 *)(arg0 + 0x188));
    func_003297C8(*(u32 *)(arg0 + 0x18c));
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192470);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192778);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192990);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192A10);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192C20);

void func_00192E08(s32 arg0) {
    func_00190DB0(*(u32 *)(arg0 + 0x120));
    func_003297C8(*(u32 *)(arg0 + 0x128));
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192E38);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192F80);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193238);

void func_00193250(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x124) = arg1;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagatuhiCopyVecs);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193280);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193488);

void func_00193638(s32 arg0) {
    func_00190DB0(*(u32 *)(arg0 + 0x124));
    func_003297C8(*(u32 *)(arg0 + 300));
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193668);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001937C0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193A88);

void func_00193AA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x128) = arg1;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagatuhiCopyVecs2);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193AD0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193CF8);

void func_00193EE0(s32 arg0) {
    func_00190DB0(*(u32 *)(arg0 + 0x124));
    func_003297C8(*(u32 *)(arg0 + 0x128));
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193F10);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194100);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001943E0);

void func_001943F8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x120) = arg1;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagatuhiCopyVecs3);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194428);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194668);
