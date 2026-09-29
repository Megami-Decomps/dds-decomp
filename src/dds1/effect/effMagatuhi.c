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
    void *resource;     /* 0x34 released by effMagatuhiReleaseResource */
} EffMagatuhiWork; /* 0x38 */

extern void *effGetHandlerArg(void *arg);


/* The two resource slots select different owners during each variant's teardown. */
typedef struct {
    u8   pad_0x000[0x120]; /* 0x000 */
    void *firstResource;   /* 0x120 */
    void *secondResource;  /* 0x124 */
    void *buffer;          /* 0x128 */
    void *extraBuffer;     /* 0x12C */
} EffMagatuhiMidWork; /* 0x130 */

/* The first and second teardown paths use different offsets for the trio. */
typedef struct {
    u8 pad00[0x180];
    void *mathResource;    /* 0x180 */
    u8 pad184[8];
    void *managedResource; /* 0x18C */
    void *buffer;          /* 0x190 */
} EffMagatuhiWideFirst;

typedef struct {
    u8 pad00[0x184];
    void *mathResource;    /* 0x184 */
    void *managedResource; /* 0x188 */
    void *buffer;          /* 0x18C */
} EffMagatuhiWideSecond;

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

void effMagatuhiReleaseResource(EffMagatuhiWork *work) {
    func_002D0918(work->resource);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001893D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189818);

void effMagatuhiSetValue(EffMagatuhiWork *work, s32 index, u32 value) {
    work->values[index] = value;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189BA8);

void func_00189C80(void) {
    func_001891C0();
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_00189C98);

void func_00189E60(EffMagatuhiWideFirst *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseSceneResource(work->managedResource);
    func_002D0918(work->buffer);
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

void func_0018A800(EffMagatuhiWideSecond *work) {
    effMathReleaseWorkResource(work->mathResource);
    effReleaseSceneResource(work->managedResource);
    func_002D0918(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018A838);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AB40);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AD58);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018ADD8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018AFE8);

void func_0018B1D0(EffMagatuhiMidWork *work) {
    effReleaseSceneResource(work->firstResource);
    func_002D0918(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B200);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B348);

void func_0018B600(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effMagatuhiSetSecondResource(EffMagatuhiMidWork *work, void *value) {
    work->secondResource = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B648);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018B850);

void func_0018BA00(EffMagatuhiMidWork *work) {
    effReleaseSceneResource(work->secondResource);
    func_002D0918(work->extraBuffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BA30);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BB88);

void func_0018BE50(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void func_0018BE68(EffMagatuhiMidWork *work, void *value) {
    work->buffer = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs2(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018BE98);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C0C0);

void func_0018C2A8(EffMagatuhiMidWork *work) {
    effReleaseSceneResource(work->secondResource);
    func_002D0918(work->buffer);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C2D8);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C4C8);

void func_0018C7A8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void effMagatuhiSetFirstResource(EffMagatuhiMidWork *work, void *value) {
    work->firstResource = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagatuhiCopyVecs3(EffMagatuhiMidWork *dst, EffMagatuhiMidWork *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018C7F0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_0018CA30);
