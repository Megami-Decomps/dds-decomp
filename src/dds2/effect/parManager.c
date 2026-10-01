#include "common.h"

#include "ee_mmi.h"

#include "pcp_vu0.h"

#include "eff.h"

typedef struct {
    u8 pad[0x30];    /* 0x0 */
    u16 kind;        /* 0x30 */
    u8 pad32[6];     /* 0x32 */
    f32 scale;       /* 0x38 */
    u8 pad3C[0x68];  /* 0x3C */
    u32 unkA4;       /* 0xA4 copied to unkFC by parRestartKind */
    u8 padA8[0x54];  /* 0xA8 */
    void *unkFC;     /* 0xFC */
    u8 pad100[0x40]; /* 0x100 */
    u16 dispatchIndex; /* 0x140 selects D_0034E250/D_0034E258/D_0034E2F0 */
    u16 restartFlag; /* 0x142 read by parGetRestartFlag, set to 1 by parRestartKind */
    u8 pad144[0x30]; /* 0x144 */
    void *child;      /* 0x174 released by parReleaseObject */
} ParObj;

typedef struct {
    u8 pad[0xC];
    void *resource; /* 0xC released by effParReleaseNodeResource */
} ParNode;

typedef struct {
    u8 pad[4];
    u16 flag;       /* 0x4 */
    u8 pad6[10];
} ParSlot; /* 0x10 bytes */

typedef struct {
    u8 pad[4];
    ParSlot *slots;
} ParTable;

extern void (*D_003AAC20[])();

/* Particle dispatch entry (0xC bytes): command func selected by the
   u16 at +0x140. */
typedef struct {
    void *(*func)(); /* 0x0 */
    u32 unk4;        /* 0x4 */
    u32 unk8;        /* 0x8 */
} ParDispatch; /* 0xC bytes */

extern ParDispatch D_003AAB80[];

extern ParDispatch D_003AAB88[];

void parReleaseObject(ParObj *obj) {
    s32 child;

    child = (s32)obj->child;
    if (child != 0) {
        func_003297C8(child);
    }
    effDestroyResources(obj);
    sdfReleaseChipBlock(obj);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00160B98);

INCLUDE_ASM(const s32, "effect/parManager", func_00160EF8);

INCLUDE_ASM(const s32, "effect/parManager", func_001612D8);

INCLUDE_ASM(const s32, "effect/parManager", func_001616A8);

INCLUDE_ASM(const s32, "effect/parManager", func_001617F8);

void effParReleaseNodeResource(ParNode *node) {
    func_003297C8(node->resource);
}

INCLUDE_ASM(const s32, "effect/parManager", func_001618E0);

INCLUDE_ASM(const s32, "effect/parManager", func_00161958);

INCLUDE_ASM(const s32, "effect/parManager", func_00161A10);

void parClearSlotFlag(ParTable *table, s32 index) {
    table->slots[index].flag = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00161B38);

INCLUDE_ASM(const s32, "effect/parManager", func_00161D08);

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 effParModulateColors(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

void parCreateIndexed(s32 index, void *arg) {
    ParObj *newobj;

    newobj = D_003AAB80[index].func(arg);
    newobj->dispatchIndex = index;
}

void parDispatchByKind(ParObj *obj) {
    D_003AAB88[obj->dispatchIndex].func();
}

INCLUDE_ASM(const s32, "effect/parManager", func_00161FE8);

void parCloneKind(ParObj *obj) {
    ParObj *newobj;

    newobj = D_003AAB80[obj->dispatchIndex].func();
    newobj->dispatchIndex = obj->dispatchIndex;
}

/* Reissues the dispatch callback and arms the restart flag. */
void parRestartKind(ParObj *obj) {
    D_003AAC20[obj->dispatchIndex]();
    obj->unkFC = (void *)obj->unkA4;
    obj->restartFlag = 1;
}

extern void (*D_003AAC58[])(ParObj *, f32);

/* Reissue the dispatch callback, scale the kind-specific value for kinds 2..4, then restart. */
void parScaleAndRestartKind(ParObj *obj, f32 factor) {
    D_003AAC58[obj->dispatchIndex](obj, factor);
    switch (obj->kind) {
    case 2:
        obj->scale *= factor;
        break;
    case 3:
        obj->scale *= factor;
        break;
    case 4:
        obj->scale *= factor;
        break;
    }
    parRestartKind(obj);
}

u16 parGetRestartFlag(ParObj *obj) {
    return obj->restartFlag;
}

void func_001622D8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* vu0 routine: effect+0xB0 = matrix * effect+0x100 via sdfComposeVuMatrixFromRegisters */
void parComposeEffectTransformMatrices(u8 *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect + 0x100);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(effect + 0xB0);
}
