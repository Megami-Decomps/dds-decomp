#include "common.h"

extern u64 func_0010D650(u64);

extern u64 func_0011A318(u64);

typedef struct EEF0Node EEF0Node;

typedef struct {
    u8 pad00[0x4];              /* 0x00 */
    void (*notify)(EEF0Node *); /* 0x04 called with the node being destroyed */
} EEF0Owner;

/* Transform node (0xD0 bytes). The links at 0x10/0x20/0x24 tie a node into
 * its owner's list; inner points at a child node whose vectors live in VU
 * registers between calls (loaded with lqc2, stored with sqc2).
 */
struct EEF0Node {
    u8 pad00[0x10];   /* 0x00 */
    EEF0Owner *owner; /* 0x10 */
    u8 pad14[0x8];    /* 0x14 */
    EEF0Node *inner;  /* 0x1C */
    EEF0Node *prev;   /* 0x20 */
    EEF0Node *next;   /* 0x24 */
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
    f32 unkC4;        /* 0xC4 */
    u32 unkC8;        /* 0xC8 */
};

u32 func_0010F118(void) {
    u64 id;

    id = func_0010D650(0);
    func_0011A328(id);
    return 1;
}

u32 func_0010F140(void) {
    func_0011A700();
    return 1;
}

u32 func_0010F160(void) {
    u64 id;

    id = func_0010D650(0);
    id = func_0011A318(id);
    func_0010D818(id);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F190);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F490);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F518);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F640);

INCLUDE_ASM(const s32, "game/code_0010F118", effObjNodeDestroy);

INCLUDE_ASM(const s32, "game/code_0010F118", effObjInnerCreate);

void effObjFreeInner(EEF0Node *node) {
    EEF0Node *inner;

    if (node != 0) {
        inner = node->inner;
        if (inner != 0) {
            func_00328E48(inner);
            node->inner = 0;
        }
    }
}

void effObjSetNodeFlags(EEF0Node *node, u32 flags) {
    node->flags = node->flags | flags;
}

void effObjClearNodeFlags(EEF0Node *node, u32 flags) {
    node->flags = node->flags & ~flags;
}

u8 effObjTestNodeFlags(EEF0Node *node, u32 flags) {
    return (node->flags & flags) != 0;
}

void effObjInnerVecInit(EEF0Node *arg0) {
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

INCLUDE_ASM(const s32, "game/code_0010F118", effObjInnerVecBackup);

void effObjSetInnerFloat(EEF0Node *arg0, f32 fparg0) {
    arg0->inner->unkC4 = fparg0;
}

f32 effObjGetInnerFloat(EEF0Node *arg0) {
    return arg0->inner->unkC4;
}

INCLUDE_ASM(const s32, "game/code_0010F118", effObjSetInnerFirstVec);

INCLUDE_ASM(const s32, "game/code_0010F118", effObjSetInnerSecondVec);

INCLUDE_ASM(const s32, "game/code_0010F118", effObjSetInnerThirdVec);

void effObjFetchInnerFirstVec(EEF0Node *arg0) {
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

INCLUDE_ASM(const s32, "game/code_0010F118", effObjFetchInnerSecondVecNorm);

void effObjFetchInnerThirdVec(EEF0Node *arg0) {
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

INCLUDE_ASM(const s32, "game/code_0010F118", effObjAddInnerFirstVec);

INCLUDE_ASM(const s32, "game/code_0010F118", effObjQuatMulInnerSecondVec);

INCLUDE_ASM(const s32, "game/code_0010F118", effObjMulInnerThirdVec);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D70);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D78);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D80);

