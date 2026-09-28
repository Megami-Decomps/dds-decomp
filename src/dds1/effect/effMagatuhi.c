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
    void *resource;     /* 0x34 released by func_001893C0 */
} EffMagatuhiWork; /* 0x38 */

extern void *effGetHandlerArg(void *arg);

/* 64-byte vector copy via COP2 (plain C cannot emit lqc2/sqc2). Wrapped in
 * .set noreorder so ee-as keeps the block verbatim. Shared by the three
 * byte-identical copy functions below. */
#define EFF_COPY64(dst, src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf28, 0(%1)\n\t" \
    "lqc2 vf29, 16(%1)\n\t" \
    "lqc2 vf30, 32(%1)\n\t" \
    "lqc2 vf31, 48(%1)\n\t" \
    "sqc2 vf28, 0(%0)\n\t" \
    "sqc2 vf29, 16(%0)\n\t" \
    "sqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst), "r" (src) : "memory")

/* Mid-size variant holding the pairs freed by func_0018B1D0/func_0018C2A8. */
typedef struct {
    u8   pad_0x000[0x120]; /* 0x000 */
    void *unk120;          /* 0x120 */
    void *unk124;          /* 0x124 */
    void *unk128;          /* 0x128 */
    void *unk12C;          /* 0x12C */
} EffMagatuhiMidWork; /* 0x130 */

/* Large variant holding the triple freed by func_00189E60/func_0018A800. */
typedef struct {
    u8   pad_0x000[0x180]; /* 0x000 */
    void *unk180;          /* 0x180 */
    void *unk184;          /* 0x184 */
    void *unk188;          /* 0x188 */
    void *unk18C;          /* 0x18C */
    void *unk190;          /* 0x190 */
} EffMagatuhiBigWork; /* 0x194 */

/* Float source block read by effMagatuhiCopyFloatBlock. */
typedef struct EffMagatuhiSrc {
    f32 f00, f04, f08;
    u8 pad0C[4];
    f32 f10, f14, f18;
    u8 pad1C[4];
    f32 f20, f24;
} EffMagatuhiSrc; /* 0x28 */

/* Float destination block written by effMagatuhiCopyFloatBlock. */
typedef struct EffMagatuhiDst {
    f32 f00, f04, f08;
    u8 pad0C[4];
    f32 f10, f14, f18;
    u8 pad1C[0x18];
    f32 f34;
    u8 pad38[0x18];
    f32 f50;
} EffMagatuhiDst; /* 0x54 */

void func_001893C0(EffMagatuhiWork *work) {
    func_002D0918(work->resource);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001893D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189818);

void func_00189B90(EffMagatuhiWork *work, s32 index, u32 value) {
    work->unk20[index] = value;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189BA8);

void func_00189C80(void) {
    func_001891C0();
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189C98);

void func_00189E60(EffMagatuhiBigWork *work) {
    func_0018DF90(work->unk180);
    func_00189178(work->unk18C);
    func_002D0918(work->unk190);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189E98);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A098);

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

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A678);

void func_0018A800(EffMagatuhiBigWork *work) {
    func_0018DF90(work->unk184);
    func_00189178(work->unk188);
    func_002D0918(work->unk18C);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A838);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AB40);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AD58);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018ADD8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AFE8);

void func_0018B1D0(EffMagatuhiMidWork *work) {
    func_00189178(work->unk120);
    func_002D0918(work->unk128);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B200);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B348);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B600);

void func_0018B618(EffMagatuhiMidWork *work, void *value) {
    work->unk124 = value;
}

void effMagatuhiCopyVecs(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    EFF_COPY64(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B648);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B850);

void func_0018BA00(EffMagatuhiMidWork *work) {
    func_00189178(work->unk124);
    func_002D0918(work->unk12C);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BA30);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BB88);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BE50);

void func_0018BE68(EffMagatuhiMidWork *work, void *value) {
    work->unk128 = value;
}

void effMagatuhiCopyVecs2(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    EFF_COPY64(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BE98);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C0C0);

void func_0018C2A8(EffMagatuhiMidWork *work) {
    func_00189178(work->unk124);
    func_002D0918(work->unk128);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C2D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C4C8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C7A8);

void func_0018C7C0(EffMagatuhiMidWork *work, void *value) {
    work->unk120 = value;
}

void effMagatuhiCopyVecs3(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    EFF_COPY64(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C7F0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018CA30);
