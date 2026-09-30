#include "common.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct ParListNode ParListNode;
typedef struct ParCellNode ParCellNode;

extern ParCellNode *D_003BB014;

extern ParListNode *D_003BB010;

/* Particle object (layout mirrors effect/parManager.c ParObj, which owns
 * the type; only the fields this TU touches are named here). */
typedef struct ParObj {
    u8 pad00[0x10];    /* 0x00 */
    f32 unk10;          /* 0x10 */
    f32 unk14;          /* 0x14 */
    u8 pad18[0x10];    /* 0x18 */
    s32 unk28;          /* 0x28 */
    s16 unk2C;          /* 0x2C */
    u8 pad2E[0x5E];    /* 0x2E */
    f32 scale8C;       /* 0x8C scaled by effParScaleComponent */
    u8 pad90[0x14];    /* 0x90 */
    u32 unkA4;         /* 0xA4 */
    u8 padA8[0x48];    /* 0xA8 */
    u32 valueF0;       /* 0xF0 settable param */
    s32 billId;         /* 0xF4 */
    u8 padF8[4];       /* 0xF8 */
    void *unkFC;       /* 0xFC */
    u8 pad100[0x40];   /* 0x100 */
    u16 dispatchIndex; /* 0x140: particle dispatch table index */
    u16 restartFlag;   /* 0x142 set to 1 after mode changes */
    u8 pad144[0x0C];   /* 0x144 */
    u8 mode150;        /* 0x150 mode byte for some kinds */
    u8 mode151;        /* 0x151 mode byte for the other kinds */
    u8 pad152[0x22];   /* 0x152 */
    void *child;       /* 0x174 */
} ParObj;

/* Particle dispatch entry (0xC bytes, mirrors effect/parManager.c). */
typedef struct ParDispatch {
    void *(*func)(); /* 0x0 */
    u32 unk4;        /* 0x4 */
    u32 unk8;        /* 0x8 */
} ParDispatch; /* 0xC */

/* Kind resource owner: release flag and handles at +0x10/+0x40. */
typedef struct ParReleaseRecord {
    u16 released;       /* 0x00 */
    u8 pad02[0x0E];
    u32 allocation;     /* 0x10 */
    u8 pad14[0x2C];
    u32 asset;          /* 0x40 */
} ParReleaseRecord;

/* 20-byte cell initialized by parCellInit (grey plus zeros). */
typedef struct ParCell {
    u128 *history;   /* 0x00 */
    void *vertices;  /* 0x04 */
    s32 vertexCount; /* 0x08: processed in groups of three */
    s32 unk0C;       /* 0x0C cleared */
    u32 color;       /* 0x10 set to grey 0x80808080 */
} ParCell; /* 0x14 */

/* Free-list links mirror the DDS2 particle unit. */
struct ParListNode {
    u8 pad00[0x54];
    ParListNode *next;
};

struct ParCellNode {
    u8 pad00[0x24];
    ParCellNode *next;
};

extern ParDispatch D_0034E250[];

extern s32 func_00151E60(s32);

extern void func_00152000(s32, f32, f32);

extern void billSetBillboardMode(s32, s16);

extern void func_001523B0(s32);

extern ParDispatch D_0034E258[];

extern void (*D_0034E5E0[])(void *, void *, void *);

extern void *memset(void *dst, s32 c, u32 n);
extern void *memcpy(void *dst, void *src, u32 n);
extern void *func_002CFEB8(s32);
extern void func_0015DA10(void *);

/* Emitter descriptor copied into a fresh allocation by func_0015D910. */
typedef struct ParEmitDesc {
    u8 pad00[0x10];
    s32 count;          /* 0x10 */
    u8 pad14[0x04];
    s32 headerSize;     /* 0x18 */
    u8 pad1C[0xAC];
    u16 unkC8;          /* 0xC8 */
    u8 padCA[0x0A];
    s32 unkD4;          /* 0xD4 */
    s32 unkD8;          /* 0xD8 */
    s32 unkDC;          /* 0xDC */
    void *unkE0;        /* 0xE0 */
} ParEmitDesc;

extern void parControlInit();

extern void func_002E84A0(void *arg);

extern u8 D_003D64B0[];

extern u8 D_003D64C0[];

extern s32 parGetRestartFlag();

extern void parCellInit();

typedef struct ParSystem {
    s16 kind;            /* 0x00 */
    s16 unk2;            /* 0x02 */
    s32 cellCount;       /* 0x04 */
    s32 vertexWordCount; /* 0x08 */
    s32 unkC;            /* 0x0C */
    s32 handle;          /* 0x10 */
    ParCell *cells;      /* 0x14 */
    void *vertices;      /* 0x18 */
    void *colors;        /* 0x1C */
    s32 object;          /* 0x20 */
    s32 unk24;           /* 0x24 */
    s32 unk28;           /* 0x28 */
} ParSystem;

extern s32 func_002DA730();

extern void func_002DA420(s32, f32);

typedef struct ParScaleObj {
    u16 kind;
    u8 pad2[6];
    f32 scale; /* 0x8 */
} ParScaleObj;

extern f32 D_003D6490[];

extern f32 D_003D64A0[];

extern f32 D_0034E590[];

extern f32 D_0034E5B0[];

extern f32 D_0034E5C0[];

extern f32 D_0034E5D0[];

extern u8 D_00324680[];

extern u8 D_00324690[];

void func_0015A758(ParObj *work, u32 value) {
    work->valueF0 = value;
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

s32 parObjGetMode(ParObj *work) {
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
    if (work->unk28 == -1) {
        s32 transform = func_00151E60(work->billId);
        func_00152000(transform, particle->unk10, particle->unk14);
        billSetBillboardMode(transform, particle->unk2C);
        func_001523B0(transform);
        particle->billId = transform;
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
    work->scale8C *= scale;
}

s64 func_0015AD48(void) {
    return parGetRestartFlag();
}

void parCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0015AD78(void) {
    func_0015A6F8();
}

void func_0015AD90(ParObj *work, u32 value) {
    work->valueF0 = value;
}

void func_0015AD98(ParObj *work, u8 mode) {
    parObjSetMode(work, mode);
}

s64 func_0015ADB0(ParObj *work) {
    return parObjGetMode(work);
}

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void func_0015ADD0(ParScaleObj *obj) {
    f32 scale;

    switch (obj->kind) {
    case 0:
    case 1:
        return;
    case 2:
        scale = obj->scale;
        D_003D64A0[0] = D_003D64A0[1] = D_003D64A0[2] = scale;
        break;
    case 3:
        scale = obj->scale;
        D_003D64A0[0] = D_003D64A0[1] = D_003D64A0[2] = scale;
        break;
    case 4:
        scale = obj->scale;
        D_003D64A0[0] = D_003D64A0[1] = D_003D64A0[2] = scale;
        break;
    default:
        return;
    }
    VU0_LOAD_VF($vf10, D_00324680);
    VU0_LOAD_VF($vf11, D_00324690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    VU0_STORE_VF($vf10, D_003D6490);
}

extern void func_00159CF0(s32);

extern void func_0015AF70(s32, s32, u32);

extern void func_0015B058(s32, s32, u32);

extern void func_0015B148(s32, s32, u32);

void parDispatchKindUpdate(ParSystem *work, s32 index, u32 color) {
    switch ((u16)work->kind) {
    case 1:
        func_00159CF0(work->vertexWordCount);
        return;
    case 2:
        func_0015AF70(work->handle, index, color);
        return;
    case 3:
        func_0015B058((s32)work->cells, index, color);
        return;
    case 4:
        func_0015B148((s32)work->cells, index, color);
        break;
    }
}

extern void parClearSlotFlag(s32);

extern void func_00188510(s32);

void parDispatchKindInit(ParSystem *work, s32 index) {
    switch ((u16)work->kind) {
    case 1:
        parClearSlotFlag(work->vertexWordCount);
        return;
    case 2:
        parCellInit((void *)work->handle, index);
        return;
    case 3:
        parCellInit((void *)work->cells, index);
        return;
    case 4:
        func_00188510((s32)work->cells);
        break;
    }
}

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

void func_0015B3D8(ParReleaseRecord *record) {
    record->released = 1;
    sdfQueueAssetRelease(record->asset);
    func_002D0918(record->allocation);
}

void func_0015B410(ParListNode *node) {
    node->next = D_003BB010;
    D_003BB010 = node;
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015B420);

void func_0015B648(ParReleaseRecord *record) {
    func_002DA438(record->asset);
}

void parControlInit(void) {
    memset(D_003D64C0, 0, 0x2C);
    *(u16 *)(D_003D64C0 + 4) = 0x4000;
}

ParSystem *func_0015B6A0(s32 count, s32 perCell, s32 groupDivisor, u32 kind) {
    s32 total;
    s32 handle;
    s32 base;
    s32 cellsSize;
    ParSystem *system;
    s32 i;

    if (kind == 4) {
        perCell = perCell * 5 + 5;
    } else if (kind == 3) {
        perCell = perCell * 4 + 4;
    } else if (kind == 2) {
        perCell = perCell * 6 + 6;
    } else {
        perCell = perCell * 2;
        if (groupDivisor != 0) {
            if (perCell % groupDivisor != 0) {
                perCell = perCell / groupDivisor + 2;
            } else {
                perCell = perCell / groupDivisor;
            }
        }
        perCell &= ~1;
        perCell += 2;
        if (kind == 1) {
            perCell += perCell >> 1;
        }
    }
    total = count * perCell;
    cellsSize = (total + count) * 0x14;
    handle = func_002D03F8(cellsSize + 0x2C);
    base = sdfResourceRetainAddress(handle);
    system = (ParSystem *)(base + cellsSize);
    memset(system, 0, 0x2C);
    system->vertices = (void *)base;
    base += total * 0x10;
    system->colors = (void *)base;
    base += total * 4;
    system->cells = (ParCell *)base;
    for (i = 0; i < count; i++) {
        ParCell *cell = (ParCell *)(i * sizeof(ParCell) + (s32)system->cells);
        cell->history = (u128 *)((u8 *)system->vertices + i * perCell * 0x10);
        cell->vertices = (u8 *)system->colors + i * perCell * 4;
        parCellInit(system, i);
    }
    system->object = func_002DA730();
    func_002DA420(system->object, 1.0f);
    system->kind = kind;
    system->cellCount = count;
    system->unk2 = 2;
    system->vertexWordCount = perCell;
    system->unkC = groupDivisor;
    system->handle = handle;
    system->unk24 = 0;
    system->unk28 = 0;
    return system;
}

void func_0015B8B8(ParSystem *system) {
    sdfQueueAssetRelease(system->object);
    func_002D0918(system->handle);
}

void parCellInit(ParSystem *system, s32 index) {
    ParCell *cell = (ParCell *)(index * 20 + (u32)system->cells);

    cell->color = 0x80808080;
    cell->unk0C = 0;
    cell->vertexCount = 0;
}

void func_0015B918(ParCellNode *node) {
    node->next = D_003BB014;
    D_003BB014 = node;
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
    s32 count = cell->vertexCount / 3;
    u8 *vertex = (u8 *)cell->history;
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

void func_0015BB90(s32 particle, s32 index) {
    ParSystem *system = (ParSystem *)particle;
    u32 count = system->cells[index].vertexCount >> 1;
    u32 *vertex = system->cells[index].vertices;
    u32 word = vertex[0];
    u32 i;
    s32 alpha[4];
    s32 start[4];
    s32 out[4];
    u32 packed;
    if (count >= 2) {
        alpha[0] = word & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, D_0034E590);
        VU0_LERP_VF10(1.0f / count);
        VU0_MOVE_VF(vf12, vf10);
        start[0] = word;
        EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
        for (i = 0; i < count; i++) {
            VU0_MOVE_VF(vf11, vf10);
            EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
            out[0] = packed;
            vertex[0] = packed;
            vertex[1] = packed;
            VU0_SUB(vf11, vf11, vf12);
            VU0_MOVE_VF(vf10, vf11);
            vertex += 2;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015BCE8);

void parFillVertexPairs(ParSystem *system, s32 firstWord, s32 secondWord) {
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
                    vertex[0] = firstWord;
                    vertex[1] = secondWord;
                    vertex += 2;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void func_0015BFD8(ParSystem *system, u32 color) {
    u32 perCell = system->vertexWordCount >> 1;
    u32 count = system->cellCount;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 out[4];
    u32 packed;
    alpha[0] = color & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_0034E5B0);
    VU0_LERP_VF10(1.0f / perCell);
    VU0_MOVE_VF(vf12, vf10);
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            start[0] = color;
            EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            for (j = 0; j < perCell; j++) {
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                out[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                VU0_SUB(vf11, vf11, vf12);
                VU0_MOVE_VF(vf10, vf11);
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void func_0015C128(ParSystem *system, u32 color) {
    u32 words = system->vertexWordCount;
    u32 count = system->cellCount;
    u32 quarter = words >> 2;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 rise[4];
    s32 fall[4];
    u32 base;
    u32 odd;
    u32 packed;
    alpha[0] = color & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_0034E5C0);
    VU0_LERP_VF10(1.0f / quarter);
    VU0_MOVE_VF(vf12, vf10);
    i = 0;
    if (count != 0) {
        base = color & 0xFFFFFF;
        odd = (words >> 1) & 1;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            start[0] = base;
            EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            for (j = 0; j < quarter; j++) {
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                rise[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                VU0_ADD(vf11, vf11, vf12);
                VU0_MOVE_VF(vf10, vf11);
                vertex += 2;
            }
            if (odd) {
                vertex[0] = color;
                vertex[1] = color;
                vertex += 2;
            }
            for (j = 0; j < quarter; j++) {
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                fall[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                VU0_SUB(vf11, vf11, vf12);
                VU0_MOVE_VF(vf10, vf11);
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void func_0015C2F0(ParSystem *system, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 3;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    *(s32 *)(vertex + 4) = middleWord;
                    *(s32 *)(vertex + 8) = edgeWord;
                    *(s32 *)(vertex + 0) = edgeWord;
                    vertex += 0xC;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void func_0015C360(ParSystem *system, u32 middleWord, u32 edgeWord) {
    f32 cur0[4];
    f32 cur1[4];
    f32 step0[4];
    f32 step1[4];
    s32 alpha0[4];
    s32 alpha1[4];
    s32 col0[4];
    s32 col1[4];
    s32 out0[4];
    s32 out1[4];
    s32 groups = 3;
    u32 perCell = system->vertexWordCount / groups;
    u32 count = system->cellCount;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    u32 packedMiddle;
    u32 packedEdge;
    alpha0[0] = middleWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_0034E5D0);
    VU0_LERP_VF10(1.0f / perCell);
    VU0_STORE_VF(vf10, step0);
    alpha1[0] = edgeWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_0034E5D0);
    VU0_LERP_VF10(1.0f / perCell);
    VU0_STORE_VF(vf10, step1);
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            col0[0] = middleWord;
            EE_MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
            VU0_STORE_VF(vf10, cur0);
            col1[0] = edgeWord;
            EE_MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
            VU0_STORE_VF(vf10, cur1);
            for (j = 0; j < perCell; j++) {
                VU0_LOAD_VF(vf10, cur0);
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packedMiddle, 128.0f);
                out0[0] = packedMiddle;
                vertex[1] = packedMiddle;
                VU0_LOAD_VF(vf12, step0);
                VU0_SUB(vf11, vf11, vf12);
                VU0_STORE_VF(vf11, cur0);
                VU0_LOAD_VF(vf10, cur1);
                VU0_MOVE_VF(vf11, vf10);
                EE_MMI_RGBA_PACK_UNIT(packedEdge, 128.0f);
                out1[0] = packedEdge;
                vertex[0] = packedEdge;
                vertex[2] = packedEdge;
                VU0_LOAD_VF(vf12, step1);
                VU0_SUB(vf11, vf11, vf12);
                VU0_STORE_VF(vf11, cur1);
                vertex += 3;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C618);

void func_0015C728(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 6;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    *(s32 *)(vertex + 0x0) = edgeWord;
                    *(s32 *)(vertex + 0x4) = middleWord;
                    *(s32 *)(vertex + 0x8) = centerWord;
                    *(s32 *)(vertex + 0xC) = centerWord;
                    *(s32 *)(vertex + 0x10) = middleWord;
                    *(s32 *)(vertex + 0x14) = edgeWord;
                    vertex += 0x18;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C7A0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015C8C0);

void parFillCellVertexQuads(ParSystem *system, s32 middleWord, s32 edgeWord) {
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
                    *(s32 *)(vertex + 0x8) = middleWord;
                    *(s32 *)(vertex + 0x4) = middleWord;
                    *(s32 *)(vertex + 0xC) = edgeWord;
                    *(s32 *)(vertex + 0x0) = edgeWord;
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

void func_0015CC58(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 5;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    *(s32 *)(vertex + 0x0) = edgeWord;
                    *(s32 *)(vertex + 0x4) = middleWord;
                    *(s32 *)(vertex + 0x8) = centerWord;
                    *(s32 *)(vertex + 0xC) = middleWord;
                    *(s32 *)(vertex + 0x10) = edgeWord;
                    vertex += 0x14;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CCD0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CDF0);

INCLUDE_ASM(const s32, "game/code_0015A758", func_0015CEF8);

void func_0015D078(s32 recordAddress, u16 value) {
    *(u16 *)(recordAddress + 2) = value;
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

void func_0015D7B8(ParBlock *block) {
    sdfQueueAssetRelease(block->object);
    func_002D0918(block->handle);
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

ParEmitDesc *func_0015D910(ParEmitDesc *src) {
    ParEmitDesc *desc = func_002CFEB8(src->count * 4 + 0xF0);

    memset(desc, 0, 0xF0);
    memcpy(desc, src, src->headerSize);
    memcpy((u8 *)desc + 0xC0, (u8 *)src + src->headerSize, 0x30);
    desc->headerSize = 0xC0;
    desc->unkE0 = (u8 *)desc + 0xF0;
    if (desc->unkC8 < 3) {
        desc->unkC8 = 3;
    }
    desc->unkDC = func_0015B6A0(desc->count, desc->unkC8, 1, 0);
    parDispatchSub(desc->unkDC, 0, desc->unkD4, desc->unkD8);
    func_0015DA10(desc);
    return desc;
}

INCLUDE_SDATA(const s32, "game/code_0015A758", D_003BB010);

INCLUDE_SDATA(const s32, "game/code_0015A758", D_003BB014);

