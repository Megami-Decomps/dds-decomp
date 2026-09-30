#include "common.h"
#include "pcp_vu0.h"

extern u64 scrReadIntParameter(u64);

extern u64 func_0011A318(u64);

extern void func_00328E48(void *p);

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

extern void effMiscNormalizeVU(void);
extern void effMiscQuatMultiplyVU(void);

u32 func_0010F118(void) {
    u64 id;

    id = scrReadIntParameter(0);
    func_0011A328(id);
    return 1;
}

u32 func_0010F140(void) {
    ptyRecoverAllUnits();
    return 1;
}

u32 func_0010F160(void) {
    u64 id;

    id = scrReadIntParameter(0);
    id = func_0011A318(id);
    func_0010D818(id);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F190);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F490);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F518);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F640);

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
        func_00328E48(node);
    }
}

INCLUDE_ASM(const s32, "game/code_0010F118", effObjInnerCreate);

void effObjFreeInner(EffTransformNode *node) {
    EffTransformNode *inner;

    if (node != 0) {
        inner = node->inner;
        if (inner != 0) {
            func_00328E48(inner);
            node->inner = 0;
        }
    }
}

void effObjSetNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags = node->flags | flags;
}

void effObjClearNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags = node->flags & ~flags;
}

u8 effObjTestNodeFlags(EffTransformNode *node, u32 flags) {
    return (node->flags & flags) != 0;
}

void effObjInnerVecInit(EffTransformNode *arg0) {
    u8 *p40 = (u8 *)arg0 + 0x40;
    u8 *p50;
    u8 *p60;

    VU0_STORE_VF(vf0, p40);
    p50 = (u8 *)arg0 + 0x50;
    VU0_STORE_VF(vf0, p50);
    VU0_SET_ONES_XYZ(vf10);
    p60 = (u8 *)arg0 + 0x60;
    VU0_STORE_VF(vf10, p60);
}

void effObjInnerVecBackup(EffTransformNode *arg0) {
    PCP_COPY_VECTOR(&arg0->vecA0, &arg0->vec60);
    PCP_COPY_VECTOR(&arg0->vec90, &arg0->vec50);
    PCP_COPY_VECTOR(&arg0->vec80, &arg0->vec40);
}

void effObjSetInnerFloat(EffTransformNode *arg0, f32 fparg0) {
    arg0->inner->scalar = fparg0;
}

f32 effObjGetInnerFloat(EffTransformNode *arg0) {
    return arg0->inner->scalar;
}

void effObjSetInnerFirstVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->vec40, arg1);
}

void effObjSetInnerSecondVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->vec50, arg1);
}

void effObjSetInnerThirdVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->vec60, arg1);
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

void effObjAddInnerFirstVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->vec40);
    VU0_LOAD_VF($vf11, arg1);
    __asm__ volatile (
        ".set noreorder\n"
        "vadd.xyzw $vf10, $vf10, $vf11\n"
        ".set reorder\n"
        : : : "memory");
    VU0_STORE_VF($vf10, &inner->vec40);
}

void effObjQuatMulInnerSecondVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->vec50);
    VU0_LOAD_VF($vf11, arg1);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, &inner->vec50);
}

void effObjMulInnerThirdVec(EffTransformNode *arg0, u128 *arg1) {
    EffTransformNode *inner = arg0->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->vec60);
    VU0_LOAD_VF($vf11, arg1);
    __asm__ volatile (
        ".set noreorder\n"
        "vmul.xyzw $vf10, $vf10, $vf11\n"
        ".set reorder\n"
        : : : "memory");
    VU0_STORE_VF($vf10, &inner->vec60);
}

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D70);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D78);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D80);

