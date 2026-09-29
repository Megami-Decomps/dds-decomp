#include "common.h"
#include "pcp_vu0.h"

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
    u32  *values;       /* 0x20: indexed value table */
    u8    pad24[0x10];  /* 0x24 */
    void *resource;   /* 0x34: freed during cleanup */
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

typedef struct EffMagatuhiResourceSet {
    u8 pad00[0x120];
    u32 firstResource;   /* 0x120 */
    u32 secondResource;  /* 0x124 */
    u32 buffer;          /* 0x128 */
    u32 extraBuffer;     /* 0x12C */
} EffMagatuhiResourceSet;

typedef struct EffMagatuhiWideFirst {
    u8 pad00[0x180];
    u32 mathResource; /* 0x180 */
    u8 pad184[8];
    u32 managedResource; /* 0x18C */
    u32 buffer; /* 0x190 */
} EffMagatuhiWideFirst;

typedef struct EffMagatuhiWideSecond {
    u8 pad00[0x184];
    u32 mathResource; /* 0x184 */
    u32 managedResource; /* 0x188 */
    u32 buffer; /* 0x18C */
} EffMagatuhiWideSecond;

void effMagatuhiReleaseResource(EffMagatuhiWork *work) {
    func_003297C8(work->resource);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191010);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00191450);

void effMagatuhiSetValue(EffMagatuhiWork *work, s32 index, u32 value) {
    work->values[index] = value;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001917E0);

void func_001918B8(void) {
    func_00190DF8();
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001918D0);

void func_00191A98(EffMagatuhiWideFirst *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    func_003297C8(work->buffer);
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

void func_00192438(EffMagatuhiWideSecond *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseMagatuhiOwner(work->managedResource);
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192470);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192778);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192990);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192A10);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192C20);

void func_00192E08(EffMagatuhiResourceSet *work) {
    effReleaseMagatuhiOwner(work->firstResource);
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192E38);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00192F80);

void func_00193238(u8 *work, void *src) {
    PCP_COPY_VECTOR(work + 0x40, src);
}

void effMagatuhiSetSecondResource(EffMagatuhiResourceSet *work, u32 value) {
    work->secondResource = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193280);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193488);

void func_00193638(EffMagatuhiResourceSet *work) {
    effReleaseMagatuhiOwner(work->secondResource);
    func_003297C8(work->extraBuffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193668);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001937C0);

void func_00193A88(u8 *work, void *src) {
    PCP_COPY_VECTOR(work + 0x40, src);
}

void func_00193AA0(EffMagatuhiResourceSet *work, u32 value) {
    work->buffer = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs2(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193AD0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193CF8);

void func_00193EE0(EffMagatuhiResourceSet *work) {
    effReleaseMagatuhiOwner(work->secondResource);
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00193F10);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194100);

void func_001943E0(u8 *work, void *src) {
    PCP_COPY_VECTOR(work + 0x40, src);
}

void effMagatuhiSetFirstResource(EffMagatuhiResourceSet *work, u32 value) {
    work->firstResource = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs3(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194428);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00194668);
