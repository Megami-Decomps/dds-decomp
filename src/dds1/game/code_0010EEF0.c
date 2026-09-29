#include "common.h"

extern u64 func_00119AF8(u64);
extern u64 func_0010D428(u64);

extern void *func_002CFEB8(s32 size);
extern void func_002CFF98(void *p);
extern void effMiscNormalizeVU(void);
extern void effMiscQuatMultiplyVU(void);


typedef struct EffTransformNode EffTransformNode;

typedef struct {
    u8 pad00[0x4];              /* 0x00 */
    void (*notify)(EffTransformNode *); /* 0x04 called with the node being destroyed */
} EffTransformOwner;

/* Transform node (0xD0 bytes). The links at 0x10/0x20/0x24 tie a node into
 * its owner's list; inner points at a child node whose vectors live in VU
 * registers between calls (loaded with lqc2, stored with sqc2).
 */
struct EffTransformNode {
    u8 pad00[0x10];   /* 0x00 */
    EffTransformOwner *owner; /* 0x10 */
    u8 pad14[0x8];    /* 0x14 */
    EffTransformNode *inner;  /* 0x1C */
    EffTransformNode *prev;   /* 0x20 */
    EffTransformNode *next;   /* 0x24 */
    u8 pad28[0x18];   /* 0x28 */
    u128 vec40;       /* 0x40 */
    u128 vec50;       /* 0x50 */
    u128 vec60;       /* 0x60 */
    u8 pad70[0x10];   /* 0x70 */
    u128 vec80;       /* 0x80 copy of vec40 */
    u128 vec90;       /* 0x90 copy of vec50 */
    u128 vecA0;       /* 0xA0 copy of vec60 */
    u8 vecB0[0x10];   /* 0xB0 cleared on alloc */
    u32 flags;        /* 0xC0 bit0 set, bit1 cleared on vector write */
    f32 scalar;       /* 0xC4 */
    u32 unkC8;        /* 0xC8 */
};

/* 128-bit vector copy for the inner vectors. The scratch register is named
 * explicitly: a plain-C u128 copy lets gcc 2.96 fold the destination into
 * an sq offset and hoist the lq above the flag update, while the retail
 * code computes both addresses with addiu and keeps each copy together.
 * The destination is the first operand so its address is computed first.
 */
#define EEF0_COPY128(dst, src) __asm__ volatile ( \
    ".set noreorder \n" \
    "lq $2, 0(%1)   \n" \
    "sq $2, 0(%0)   \n" \
    ".set reorder" \
    : : "r" (dst), "r" (src) : "memory", "$2")

/* Store vf10 to base+off, recomputing the address. The tied output (no
 * early clobber) lets gcc reuse the base register for the address. The
 * `addiu` is part of the asm because the C form (`dst = base + off;` then a
 * bare sqc2) was tried and picks other registers (retail addiu's into the
 * base's register, effObjAddInnerFirstVec and effObjMulInnerThirdVec differ).
 */
#define EEF0_STORE_V10(dst, base, off) __asm__ volatile ( \
    ".set noreorder    \n" \
    "addiu %0, %1, %2  \n" \
    "sqc2 vf10, 0(%0)  \n" \
    ".set reorder" \
    : "=r" (dst) : "r" (base), "i" (off) : "memory")

u32 func_0010EEF0(void) {
    u64 context;

    context = func_0010D428(0);
    func_00119B08(context);
    return 1;
}

u32 func_0010EF18(void) {
    ptyRecoverAllUnits();
    return 1;
}

u32 func_0010EF38(void) {
    u64 context;

    context = func_0010D428(0);
    context = func_00119AF8(context);
    func_0010D5F0(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010EF68);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F268);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F2F0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F418);

/* Notify the owner before unlinking and freeing this transform node. */
void effObjNodeDestroy(EffTransformNode *node) {
    EffTransformOwner *owner;
    EffTransformNode *next;
    EffTransformNode *prev;

    if (node != NULL) {
        owner = node->owner;
        if (owner != NULL) {
            if (owner->notify != NULL) {
                owner->notify(node);
            }
        }
        next = node->next;
        if (next != NULL) {
            next->prev = node->prev;
        }
        prev = node->prev;
        if (prev != NULL) {
            prev->next = node->next;
        }
        func_002CFF98(node);
    }
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", effObjInnerCreate);

void effObjFreeInner(EffTransformNode *node) {
    EffTransformNode *inner;

    if (node != NULL) {
        inner = node->inner;
        if (inner != NULL) {
            func_002CFF98(inner);
            node->inner = NULL;
        }
    }
}

void effObjSetNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags |= flags;
}

void effObjClearNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags &= ~flags;
}

u8 effObjTestNodeFlags(EffTransformNode *node, u32 flags) {
    return (node->flags & flags) != 0;
}

void effObjInnerVecInit(EffTransformNode *arg0) {
    u8 *p40 = (u8 *)arg0 + 0x40;
    u8 *p50;
    u8 *p60;

    __asm__ volatile (
        ".set noreorder      \n"
        "sqc2 vf0, 0(%0)     \n"
        ".set reorder"
        :
        : "r" (p40)
        : "memory"
    );
    p50 = (u8 *)arg0 + 0x50;
    __asm__ volatile (
        ".set noreorder      \n"
        "sqc2 vf0, 0(%0)     \n"
        ".set reorder"
        :
        : "r" (p50)
        : "memory"
    );
    __asm__ volatile (
        ".set noreorder            \n"
        "vaddw.xyz vf10, vf0, vf0w \n"
        "vmulx.w vf10, vf0, vf0x   \n"
        ".set reorder"
        :
        :
        : "memory"
    );
    p60 = (u8 *)arg0 + 0x60;
    __asm__ volatile (
        ".set noreorder       \n"
        "sqc2 vf10, 0(%0)     \n"
        ".set reorder"
        :
        : "r" (p60)
        : "memory"
    );
}

void effObjInnerVecBackup(EffTransformNode *arg0) {
    EEF0_COPY128((u128 *)((u8 *)arg0 + 0xA0), (u128 *)((u8 *)arg0 + 0x60));
    EEF0_COPY128((u128 *)((u8 *)arg0 + 0x90), (u128 *)((u8 *)arg0 + 0x50));
    EEF0_COPY128((u128 *)((u8 *)arg0 + 0x80), (u128 *)((u8 *)arg0 + 0x40));
}

void effObjSetInnerFloat(EffTransformNode *arg0, f32 fparg0) {
    arg0->inner->scalar = fparg0;
}

f32 effObjGetInnerFloat(EffTransformNode *arg0) {
    return arg0->inner->scalar;
}

void effObjSetInnerFirstVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;
    u128 *dst = (u128 *)((u8 *)inner + 0x40);

    inner->flags = (inner->flags | 1) & ~2;
    EEF0_COPY128(dst, arg1);
}

void effObjSetInnerSecondVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;
    u128 *dst = (u128 *)((u8 *)inner + 0x50);

    inner->flags = (inner->flags | 1) & ~2;
    EEF0_COPY128(dst, arg1);
}

void effObjSetInnerThirdVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;
    u128 *dst = (u128 *)((u8 *)inner + 0x60);

    inner->flags = (inner->flags | 1) & ~2;
    EEF0_COPY128(dst, arg1);
}

void effObjFetchInnerFirstVec(EffTransformNode *arg0) {
    u8 *p = (u8 *)arg0->inner + 0x40;

    __asm__ volatile (
        ".set noreorder       \n"
        "lqc2 vf10, 0(%0)     \n"
        "vmove.w vf10, vf0    \n"
        ".set reorder"
        :
        : "r" (p)
        : "memory"
    );
}

void effObjFetchInnerSecondVecNorm(EffTransformNode *arg0) {
    u8 *p = (u8 *)arg0->inner + 0x50;

    __asm__ volatile (
        ".set noreorder      \n"
        "lqc2 vf10, 0(%0)    \n"
        ".set reorder"
        :
        : "r" (p)
        : "memory"
    );
    effMiscNormalizeVU();
}

void effObjFetchInnerThirdVec(EffTransformNode *arg0) {
    u8 *p = (u8 *)arg0->inner + 0x60;

    __asm__ volatile (
        ".set noreorder      \n"
        "lqc2 vf10, 0(%0)    \n"
        ".set reorder"
        :
        : "r" (p)
        : "memory"
    );
}

void effObjAddInnerFirstVec(EffTransformNode *arg0, void *arg1) {
    EffTransformNode *inner = arg0->inner;
    u8 *src = (u8 *)inner + 0x40;
    u8 *dst;

    inner->flags = (inner->flags | 1) & ~2;
    __asm__ volatile (
        ".set noreorder             \n"
        "lqc2 vf10, 0(%0)           \n"
        "lqc2 vf11, 0(%1)           \n"
        "vadd.xyzw vf10, vf10, vf11 \n"
        ".set reorder"
        :
        : "r" (src), "r" (arg1)
        : "memory"
    );
    EEF0_STORE_V10(dst, inner, 0x40);
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", effObjQuatMulInnerSecondVec);

void effObjMulInnerThirdVec(EffTransformNode *arg0, void *arg1) {
    EffTransformNode *inner = arg0->inner;
    u8 *src = (u8 *)inner + 0x60;
    u8 *dst;

    inner->flags = (inner->flags | 1) & ~2;
    __asm__ volatile (
        ".set noreorder             \n"
        "lqc2 vf10, 0(%0)           \n"
        "lqc2 vf11, 0(%1)           \n"
        "vmul.xyzw vf10, vf10, vf11 \n"
        ".set reorder"
        :
        : "r" (src), "r" (arg1)
        : "memory"
    );
    EEF0_STORE_V10(dst, inner, 0x60);
}

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A0);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A8);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9B0);

