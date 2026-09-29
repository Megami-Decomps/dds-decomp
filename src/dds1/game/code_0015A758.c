#include "common.h"
#include "pcp_vu0.h"

extern s32 D_003BB014;

extern s32 D_003BB010;

/* Particle object (layout mirrors effect/parManager.c ParObj, which owns
 * the type; only the fields this TU touches are named here). */
typedef struct ParObj {
    u8 pad00[0x8C];   /* 0x00 */
    f32 unk8C;        /* 0x8C scaled by effParScaleComponent */
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
    u128 *vertices; /* 0x00 */
    u8 pad04[4];   /* 0x04 */
    s32 unk08;     /* 0x08 cleared */
    s32 unk0C;     /* 0x0C cleared */
    u32 color10;   /* 0x10 set to grey 0x80808080 */
} ParCell; /* 0x14 */

extern ParDispatch D_0034E250[];
extern s32 func_00151E60(s32);
extern void func_00152000(s32, f32, f32);
extern void effBillSetMode(s32, s16);
extern void func_001523B0(s32);
extern ParDispatch D_0034E258[];

extern void (*D_0034E5E0[])(void *, void *, void *);

extern void *memset(void *dst, s32 c, u32 n);

extern void parControlInit();

extern void func_002E84A0(void *arg);

extern u8 D_003D64B0[];

extern u8 D_003D64C0[];

extern s32 parGetRestartFlag();

typedef struct ParSystem {
    u8 pad00[4];
    s32 cellCount;       /* 0x04 */
    s32 vertexWordCount; /* 0x08 */
    u8 pad0C[8];
    ParCell *cells;      /* 0x14 */
} ParSystem;

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
        effBillSetMode(transform, *(s16 *)((u8 *)particle + 0x2C));
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

void effParScaleComponent(float scale, ParObj *work) {
    func_0015A658();
    work->unk8C *= scale;
}

s64 func_0015AD48(void) {
    return parGetRestartFlag();
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
extern void func_0015AF70(s32, s32, u32);
extern void func_0015B058(s32, s32, u32);
extern void func_0015B148(s32, s32, u32);

void parDispatchKindUpdate(void *work, s32 index, u32 color) {
    switch (*(u16 *)work) {
    case 1:
        func_00159CF0(*(s32 *)((u8 *)work + 8));
        return;
    case 2:
        func_0015AF70(*(s32 *)((u8 *)work + 0x10), index, color);
        return;
    case 3:
        func_0015B058(*(s32 *)((u8 *)work + 0x14), index, color);
        return;
    case 4:
        func_0015B148(*(s32 *)((u8 *)work + 0x14), index, color);
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

extern u8 D_003D6490[];
extern u8 D_003D64A0[];
extern void effBillSetEntryValue(s32, s32, u32);
extern void func_0015BB90(s32, s32);

void func_0015AF70(s32 particle, s32 index, u32 color) {
    u128 axis[2];
    __asm__ volatile ("vmove.xyzw vf11, vf12\n\tvsub.xyzw vf10, vf10, vf11");
    __asm__ volatile ("lqc2 vf11, 0(%0)" : : "r"(D_003D6490));
    __asm__ volatile (
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q");
    __asm__ volatile ("lqc2 vf11, 0(%0)" : : "r"(D_003D64A0));
    __asm__ volatile (
        "vmul.xyzw vf10, vf10, vf11\n\t"
        "vmove.xyzw vf2, vf10\n\t"
        "vmove.xyzw vf10, vf12\n\t"
        "vmove.xyzw vf12, vf2\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        "vadd.xyzw vf10, vf10, vf12");
    __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(&axis[0]) : "memory");
    __asm__ volatile ("vmove.xyzw vf10, vf12\n\tvsub.xyzw vf11, vf11, vf10");
    __asm__ volatile ("sqc2 vf11, 0(%0)" : : "r"(&axis[1]) : "memory");
    func_0015B928(particle, index, axis);
    func_0015BB90(particle, index);
    effBillSetEntryValue(particle, index, (color & 0xFF000000) | 0x808080);
}


extern u8 D_003D6490[];
extern u8 D_003D64A0[];
extern void effBillSetEntryValue(s32, s32, u32);
extern void func_0015BCE8(s32, s32);

void func_0015B058(s32 particle, s32 index, u32 color) {
    u128 axis[3];
    __asm__ volatile ("vmove.xyzw vf11, vf12\n\tvsub.xyzw vf10, vf10, vf11");
    __asm__ volatile ("lqc2 vf11, 0(%0)" : : "r"(D_003D6490));
    __asm__ volatile (
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q");
    __asm__ volatile ("lqc2 vf11, 0(%0)" : : "r"(D_003D64A0));
    __asm__ volatile (
        "vmul.xyzw vf10, vf10, vf11\n\t"
        "vmove.xyzw vf2, vf10\n\t"
        "vmove.xyzw vf10, vf12\n\t"
        "vmove.xyzw vf12, vf2");
    __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(&axis[1]) : "memory");
    __asm__ volatile ("vmove.xyzw vf11, vf10\n\tvadd.xyzw vf10, vf10, vf12");
    __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(&axis[0]) : "memory");
    __asm__ volatile ("vmove.xyzw vf10, vf12\n\tvsub.xyzw vf11, vf11, vf10");
    __asm__ volatile ("sqc2 vf11, 0(%0)" : : "r"(&axis[2]) : "memory");
    func_0015BA38(particle, index, axis);
    func_0015BCE8(particle, index);
    effBillSetEntryValue(particle, index, (color & 0xFF000000) | 0x808080);
}


extern u8 D_003D6490[];
extern u8 D_003D64A0[];
extern void func_001884E8(s32, s32, void *);
extern void func_00188538(s32, s32, u32);

void func_0015B148(s32 particle, s32 index, u32 color) {
    u128 axis[2];
    __asm__ volatile ("vmove.xyzw vf11, vf12\n\tvsub.xyzw vf10, vf10, vf11");
    __asm__ volatile ("lqc2 vf11, 0(%0)" : : "r"(D_003D6490));
    __asm__ volatile (
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q");
    __asm__ volatile ("lqc2 vf11, 0(%0)" : : "r"(D_003D64A0));
    __asm__ volatile (
        "vmul.xyzw vf10, vf10, vf11\n\t"
        "vmove.xyzw vf2, vf10\n\t"
        "vmove.xyzw vf10, vf12\n\t"
        "vmove.xyzw vf12, vf2\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        "vadd.xyzw vf10, vf10, vf12");
    __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(&axis[0]) : "memory");
    __asm__ volatile ("vmove.xyzw vf10, vf12\n\tvsub.xyzw vf11, vf11, vf10");
    __asm__ volatile ("sqc2 vf11, 0(%0)" : : "r"(&axis[1]) : "memory");
    func_001884E8(particle, index, axis);
    func_00188538(particle, index, (color & 0xFF000000) | 0x808080);
}


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
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x20));
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
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x20));
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

void func_0015B9E0(ParSystem *system, s32 index, void *delta) {
    ParCell *cell = system->cells + index;
    s32 count = system->vertexWordCount;
    u8 *vertex = *(u8 **)cell;
    s32 i;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(delta) : "memory");
    if (count > 0) {
        i = count;
        do {
            __asm__ volatile ("lqc2 vf10, 0(%0)" : : "r"(vertex));
            __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
            __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(vertex) : "memory");
            i--;
            vertex += 0x10;
        } while (i != 0);
    }
}


INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BA38);

void func_0015BB00(ParSystem *system, s32 index, void *delta) {
    ParCell *cell = system->cells + index;
    s32 count = cell->unk08 / 3;
    u8 *vertex = *(u8 **)cell;
    s32 i;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(delta));
    if (count > 0) {
        i = count;
        do {
            __asm__ volatile ("lqc2 vf10, 0(%0)" : : "r"(vertex));
            __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
            __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(vertex) : "memory");
            __asm__ volatile ("lqc2 vf10, 0(%0)" : : "r"(vertex + 0x10));
            __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
            __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(vertex + 0x10) : "memory");
            __asm__ volatile ("lqc2 vf10, 0(%0)" : : "r"(vertex + 0x20));
            __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
            __asm__ volatile ("sqc2 vf10, 0(%0)" : : "r"(vertex + 0x20) : "memory");
            i--;
            vertex += 0x30;
        } while (i != 0);
    }
}


INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BB90);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BCE8);

void func_0015BF78(ParSystem *system, s32 arg1, s32 arg2) {
    s32 count = system->cellCount;
    s32 perCell = system->vertexWordCount >> 1;
    s32 i;
    s32 j;
    u8 *cell;
    s32 *vertex;
    if (count > 0) {
        i = count;
        /* Required to match: advance a byte cursor based at ParCell.vertices. */
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(s32 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    vertex[0] = arg1;
                    vertex[1] = arg2;
                    vertex += 2;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BFD8);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C128);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C2F0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C360);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C618);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C728);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C7A0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C8C0);

void parFillCellVertexQuads(ParSystem *system, s32 arg1, s32 arg2) {
    s32 count = system->cellCount;
    s32 perCell = system->vertexWordCount >> 2;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        /* Required to match: use the same offset-four cell cursor as the paired fill. */
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    *(s32 *)(vertex + 0x8) = arg1;
                    *(s32 *)(vertex + 0x4) = arg1;
                    *(s32 *)(vertex + 0xC) = arg2;
                    *(s32 *)(vertex + 0x0) = arg2;
                    vertex += 0x10;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

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

typedef struct ParBlock {
    s32 count;       /* 0x00 */
    u32 color;       /* 0x04 */
    s32 base;        /* 0x08 */
    u8 *vertices;    /* 0x0C */
    s32 object;      /* 0x10 */
    s32 handle;      /* 0x14 */
} ParBlock;

extern s32 func_002DA730();
extern void func_002DA420(s32, f32);

ParBlock *func_0015D710(s32 count) {
    s32 points = count * 3;
    s32 colorBytes = points * 4;
    s32 handle = func_002D03F8((colorBytes + points) * 4 + 0x18);
    s32 base = sdfResourceRetainAddress(handle);
    u8 *vertices = (u8 *)base + points * 16;
    ParBlock *block = (ParBlock *)(vertices + colorBytes);
    block->color = 0x80808080;
    block->count = count;
    block->vertices = vertices;
    block->handle = handle;
    block->base = base;
    block->object = func_002DA730();
    func_002DA420(block->object, 1.0f);
    return block;
}


void func_0015D7B8(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x10));
    func_002D0918(*(u32 *)(arg0 + 0x14));
}

typedef struct ParDrawState {
    u16 width;      /* 0x00 */
    u16 height;     /* 0x02 */
    u16 flags;      /* 0x04 */
    u8 pad06[2];
    s32 unk08;      /* 0x08 */
    u8 pad0C[4];
    s32 unk10;      /* 0x10 */
    u8 pad14[0xC];
    s32 unk20;      /* 0x20 */
    u8 pad24[8];
} ParDrawState; /* 0x2C */

typedef struct ParDrawCmd {
    s32 count;      /* 0x00 */
    s32 unk4;
    s32 unk8;
    s32 unkC;
    void (*finish)(void *, s32); /* 0x10 */
} ParDrawCmd;

extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(s32);
extern void sdfConsAppendClearPacket(s32, s32);
extern void sdfAppendPacket(s32, s32);
extern s32 func_0015FE20(ParDrawState *);

void func_0015D7E8(ParDrawCmd *emitter, ParDrawCmd *cmd) {
    s32 list = sdfAllocPacketAligned(0x20);
    ParDrawState state;
    s32 remaining;
    sdfInitPacketList(list);
    sdfConsAppendClearPacket(list, 0);
    remaining = cmd->count * 3;
    memset(&state, 0, sizeof(state));
    state.width = 0x10;
    state.height = 0x30;
    state.flags = 0x4000;
    state.unk10 = cmd->unk8;
    state.unk20 = cmd->unkC;
    state.unk08 = cmd->unk4;
    while (remaining >= 0x30) {
        remaining -= 0x30;
        sdfAppendPacket(list, func_0015FE20(&state));
    }
    if (remaining > 0) {
        u16 *counts = (u16 *)D_003D64C0;
        counts[0] = remaining / 3;
        counts[1] = remaining;
        sdfAppendPacket(list, func_0015FE20(&state));
    }
    emitter->finish(emitter, list);
}


INCLUDE_ASM(const s32, "game/code_0015A758", func_0015D910);

INCLUDE_SDATA(const s32, "game/code_0015A758", D_003BB010);

INCLUDE_SDATA(const s32, "game/code_0015A758", D_003BB014);

