#include "common.h"

typedef struct {
    u8 pad[0xA4];    /* 0x0 */
    u32 unkA4;       /* 0xA4 copied to unkFC by func_0015A608 */
    u8 padA8[0x54];  /* 0xA8 */
    void *unkFC;     /* 0xFC */
    u8 pad100[0x40]; /* 0x100 */
    u16 unk140;      /* 0x140 dispatch index for D_0034E250/D_0034E258/D_0034E2F0 */
    u16 unk142;      /* 0x142 read by func_0015A6E0, set to 1 by func_0015A608 */
    u8 pad144[0x30]; /* 0x144 */
    void *unk174;    /* 0x174 child released by func_00158F68 */
} ParObj;

typedef struct {
    u8 pad[0xC]; /* 0x0 */
    void *unkC;  /* 0xC released by func_00159CD8 */
} ParNode;

typedef struct {
    u8 pad[4];   /* 0x0 */
    u16 unk4;    /* 0x4 cleared by func_00159F30 */
    u8 pad6[10]; /* 0x6 */
} ParSlot; /* 0x10 bytes */

typedef struct {
    u8 pad[4];     /* 0x0 */
    ParSlot *unk4; /* 0x4 */
} ParTable;

typedef struct {
    u8 pad[0x142]; /* 0x0 */
    u16 unk142;    /* 0x142 read by func_0015A6E0 */
} ParState;

/* Particle dispatch entry (0xC bytes): command func selected by the
   u16 at +0x140. */
typedef struct {
    void *(*func)(); /* 0x0 */
    u32 unk4;        /* 0x4 */
    u32 unk8;        /* 0x8 */
} ParDispatch; /* 0xC bytes */

extern ParDispatch D_0034E250[];
extern ParDispatch D_0034E258[];
extern void (*D_0034E2F0[])();

void func_002D0918(void *arg);
void func_00153920(void *arg);
void func_002CFF98(void *arg);

void func_00158F68(ParObj *obj) {
    if (obj->unk174 != NULL) {
        func_002D0918(obj->unk174);
    }
    func_00153920(obj);
    func_002CFF98(obj);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00158FA8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159308);

INCLUDE_ASM(const s32, "effect/parManager", func_001596E8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159AB8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159C08);

void func_00159CD8(ParNode *node) {
    func_002D0918(node->unkC);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159CF0);

INCLUDE_ASM(const s32, "effect/parManager", func_00159D68);

INCLUDE_ASM(const s32, "effect/parManager", func_00159E20);

void func_00159F30(ParTable *table, s32 index) {
    table->unk4[index].unk4 = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159F48);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A118);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A2F8);

void func_0015A378(s32 index, void *arg) {
    ParObj *newobj;

    newobj = D_0034E250[index].func(arg);
    newobj->unk140 = index;
}

void func_0015A3C0(ParObj *obj) {
    D_0034E258[obj->unk140].func();
}

INCLUDE_ASM(const s32, "effect/parManager", func_0015A3F8);

void func_0015A5B8(ParObj *obj) {
    ParObj *newobj;

    newobj = D_0034E250[obj->unk140].func();
    newobj->unk140 = obj->unk140;
}

void func_0015A608(ParObj *obj) {
    D_0034E2F0[obj->unk140]();
    obj->unkFC = (void *)obj->unkA4;
    obj->unk142 = 1;
}

INCLUDE_ASM(const s32, "effect/parManager", func_0015A658);

u16 func_0015A6E0(ParState *obj) {
    return obj->unk142;
}

INCLUDE_ASM(const s32, "effect/parManager", func_0015A6E8);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A6F8);
