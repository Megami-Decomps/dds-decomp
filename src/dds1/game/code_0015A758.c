#include "common.h"
#include "pcp_vu0.h"

extern s32 D_003BB014;

extern s32 D_003BB010;

/* Particle object (layout mirrors effect/parManager.c ParObj, which owns
 * the type; only the fields this TU touches are named here). */
typedef struct ParObj {
    u8 pad00[0x8C];   /* 0x00 */
    f32 unk8C;        /* 0x8C scaled by func_0015AD08 */
    u8 pad90[0x14];   /* 0x90 */
    u32 unkA4;        /* 0xA4 */
    u8 padA8[0x48];   /* 0xA8 */
    u32 unkF0;        /* 0xF0 settable param */
    u8 padF4[0x08];   /* 0xF4 */
    void *unkFC;      /* 0xFC */
    u8 pad100[0x40];  /* 0x100 */
    u16 dispatchIndex; /* 0x140: particle dispatch table index */
    u16 restartFlag;   /* 0x142 set to 1 after mode changes */
    u8 pad144[0x0C];  /* 0x144 */
    u8 mode150;       /* 0x150 mode byte for some kinds */
    u8 mode151;       /* 0x151 mode byte for the other kinds */
    u8 pad152[0x22];  /* 0x152 */
    void *child;       /* 0x174 */
} ParObj;

/* Particle dispatch entry (0xC bytes, mirrors effect/parManager.c). */
typedef struct ParDispatch {
    void *(*func)(); /* 0x0 */
    u32 unk4;        /* 0x4 */
    u32 unk8;        /* 0x8 */
} ParDispatch; /* 0xC */

/* 20-byte cell initialized by parCellInit (grey plus zeros). */
typedef struct ParCell {
    u8 pad00[8];   /* 0x00 */
    u32 unk08;     /* 0x08 cleared */
    u32 unk0C;     /* 0x0C cleared */
    u32 color10;   /* 0x10 set to grey 0x80808080 */
} ParCell; /* 0x14 */

extern ParDispatch D_0034E250[];
extern s32 func_00151E60(s32);
extern void func_00152000(s32, f32, f32);
extern void func_00152050(s32, s16);
extern void func_001523B0(s32);
extern ParDispatch D_0034E258[];

extern void (*D_0034E5E0[])(void *, void *, void *);

extern void *memset(void *dst, s32 c, u32 n);

extern void parControlInit();

extern void func_002E84A0(void *arg);

extern u8 D_003D64B0[];

extern u8 D_003D64C0[];

extern s32 func_0015A6E0();

void func_0015A758(ParObj *work, u32 value) {
    work->unkF0 = value;
}

void parObjSetMode(ParObj *work, s32 value) {
    value &= 0xFF;
    switch (work->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        work->mode150 = value;
        break;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        work->mode151 = value;
        break;
    case 9:
        break;
    }
    work->restartFlag = 1;
}

u32 parObjGetMode(ParObj *work) {
    switch (work->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        return work->mode150;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        return work->mode151;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A7E0);

ParObj *parInstantiateKind(ParObj *work) {
    ParObj *particle = D_0034E250[work->dispatchIndex].func();
    particle->dispatchIndex = work->dispatchIndex;
    if (*(s32 *)((u8 *)work + 0x28) == -1) {
        s32 transform = func_00151E60(*(s32 *)((u8 *)work + 0xF4));
        func_00152000(transform, *(f32 *)((u8 *)particle + 0x10), *(f32 *)((u8 *)particle + 0x14));
        func_00152050(transform, *(s16 *)((u8 *)particle + 0x2C));
        func_001523B0(transform);
        *(s32 *)((u8 *)particle + 0xF4) = transform;
    }
    return particle;
}

void parObjDispatch(ParObj *work) {
    D_0034E258[work->dispatchIndex].func(work);
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015A9A0);

void func_0015ACF0(void) {
    parRestartKind();
}

void func_0015AD08(float scale, ParObj *work) {
    func_0015A658();
    work->unk8C *= scale;
}

s64 func_0015AD48(void) {
    return func_0015A6E0();
}

void func_0015AD68(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0015AD78(void) {
    func_0015A6F8();
}

void func_0015AD90(ParObj *work, u32 value) {
    work->unkF0 = value;
}

void func_0015AD98(ParObj *work, u8 value) {
    parObjSetMode(work, value);
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015ADB0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015ADD0);

extern void func_00159CF0(s32);
extern void func_0015AF70(s32);
extern void func_0015B058(s32);
extern void func_0015B148(s32);

void parDispatchKindUpdate(void *work) {
    switch (*(u16 *)work) {
    case 1:
        func_00159CF0(*(s32 *)((u8 *)work + 8));
        return;
    case 2:
        func_0015AF70(*(s32 *)((u8 *)work + 0x10));
        return;
    case 3:
        func_0015B058(*(s32 *)((u8 *)work + 0x14));
        return;
    case 4:
        func_0015B148(*(s32 *)((u8 *)work + 0x14));
        break;
    }
}

extern void parClearSlotFlag(s32);
extern void func_00188510(s32);

void parDispatchKindInit(void *work, s32 index) {
    switch (*(u16 *)work) {
    case 1:
        parClearSlotFlag(*(s32 *)((u8 *)work + 8));
        return;
    case 2:
        parCellInit((void *)*(s32 *)((u8 *)work + 0x10), index);
        return;
    case 3:
        parCellInit((void *)*(s32 *)((u8 *)work + 0x14), index);
        return;
    case 4:
        func_00188510(*(s32 *)((u8 *)work + 0x14));
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015AF70);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B058);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B148);

u32 func_0015B220(void) {
    return 0;
}

void parSysReset(void) {
    D_003BB010 = 0;
    parControlInit();
    func_002E84A0(&D_003D64B0);
}

void func_0015B250(void) {
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B258);

void func_0015B3D8(u16 *arg0) {
    *arg0 = 1;
    func_002DAA68(*(u32 *)(arg0 + 0x20));
    func_002D0918(*(u32 *)(arg0 + 8));
}

void func_0015B410(s32 arg0) {
    *(s32 *)(arg0 + 0x54) = D_003BB010;
    D_003BB010 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B420);

void func_0015B648(s32 arg0) {
    func_002DA438(*(u32 *)(arg0 + 0x40));
}

void parControlInit(void) {
    memset(D_003D64C0, 0, 0x2C);
    *(u16 *)(D_003D64C0 + 4) = 0x4000;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B6A0);

void func_0015B8B8(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x20));
    func_002D0918(*(u32 *)(arg0 + 0x10));
}

void parCellInit(void *work, s32 index) {
    ParCell *cell = (ParCell *)(index * 20 + *(u32 *)((u8 *)work + 0x14));

    cell->color10 = 0x80808080;
    cell->unk0C = 0;
    cell->unk08 = 0;
}

void func_0015B918(s32 arg0) {
    *(s32 *)(arg0 + 0x24) = D_003BB014;
    D_003BB014 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B928);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B9E0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BA38);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BB00);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BB90);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BCE8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BF78);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BFD8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C128);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C2F0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C360);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C618);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C728);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C7A0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C8C0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CA40);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CAA0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CB58);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CC58);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CCD0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CDF0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CEF8);

void func_0015D078(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
}

void parDispatchSub(void *work, s32 sub, void *a2, void *a3) {
    u16 id = *(u16 *)work;

    D_0034E5E0[id * 3 + sub](work, a2, a3);
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D0C0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D710);

void func_0015D7B8(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x10));
    func_002D0918(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D7E8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D910);

INCLUDE_SDATA(const s32, "game/code_0015A758", D_003BB010);

INCLUDE_SDATA(const s32, "game/code_0015A758", D_003BB014);

