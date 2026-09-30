#include "common.h"

#include "eff.h"

#include "pcp_vu0.h"

/* Particle dispatch object; see the matching DDS1 game unit. */
typedef struct ParObj {
    u8 pad0[0x8C];
    f32 scale8C;
    u8 pad90[0x60];
    u32 valueF0; /* 0xF0 settable param */
    u8 padF4[0x4C];
    u16 dispatchIndex; /* 0x140: particle dispatch table index */
    u16 restartFlag;   /* 0x142: set after mode changes */
    u8 pad144[0x0C];
    u8 mode150;
    u8 mode151;
} ParObj;

typedef struct ParListNode {
    u8 pad00[0x54];
    struct ParListNode *next;
} ParListNode;

typedef struct ParCellNode {
    u8 pad00[0x24];
    struct ParCellNode *next;
} ParCellNode;

typedef struct ParScaleObj {
    u16 kind;
    u8 pad2[6];
    f32 scale; /* 0x8 */
} ParScaleObj;

extern f32 D_00451F30[];

extern f32 D_00451F40[];

extern u8 D_0037F680[];

extern u8 D_0037F690[];

extern ParListNode *D_00436400;

extern ParCellNode *D_00436404;

extern void (*D_003AAF10[])(void *, void *, void *);

extern BillDispatch D_003AAB88[];

extern s32 parGetRestartFlag();

extern u8 parObjGetMode();

extern void func_001618E0(s32);

extern void func_00162B60(s32);

extern void func_00162C48(s32);

extern void func_00162D38(s32);

extern void parClearSlotFlag(s32);

extern void func_00190148(s32);

typedef struct ParBlock {
    s32 count;       /* 0x00 */
    u32 color;       /* 0x04 */
    s32 base;        /* 0x08 */
    u8 *vertices;    /* 0x0C */
    s32 object;      /* 0x10 */
    s32 handle;      /* 0x14 */
} ParBlock;

extern s32 func_003335E0();

extern void func_003332D0(s32, f32);

void func_00162348(ParObj *work, u32 value) {
    work->valueF0 = value;
}

void parObjSetMode(ParObj *object, u8 mode) {
    switch (object->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        object->mode150 = mode;
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
        object->mode151 = mode;
        break;
    }
    object->restartFlag = 1;
}

u8 parObjGetMode(ParObj *object) {
    switch (object->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        return object->mode150;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        return object->mode151;
    default:
        return 0;
    }
}

typedef struct ParKindObj {
    u8 pad00[0x10];
    f32 unk10;
    f32 unk14;
    u8 pad18[0x10];
    s32 unk28;
    s16 unk2C;
    u8 pad2E[0xC6];
    s32 billId; /* 0xF4 */
    u8 padF8[0x48];
    u16 kind;   /* 0x140 */
} ParKindObj;

extern BillDispatch D_003AAB80[];

extern s32 func_00159A50(s32 id);

extern void func_00159BF0(s32 id, f32 a, f32 b);

extern void billSetBillboardMode(s32 id, s16 mode);

extern void func_00159FA0(s32 id);

INCLUDE_ASM(const s32, "game/code_00162348", func_001623D0);

ParKindObj *parInstantiateKind(ParKindObj *src) {
    ParKindObj *obj;
    s32 bill;

    obj = D_003AAB80[src->kind].func();
    obj->kind = src->kind;
    if (src->unk28 == -1) {
        bill = func_00159A50(src->billId);
        func_00159BF0(bill, obj->unk10, obj->unk14);
        billSetBillboardMode(bill, obj->unk2C);
        func_00159FA0(bill);
        obj->billId = bill;
    }
    return obj;
}

void parObjDispatch(ParObj *object) {
    D_003AAB88[object->dispatchIndex].func();
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162590);

void func_001628E0(void) {
    parRestartKind();
}

void effParScaleComponent(float scale, ParObj *work) {
    func_00162248();
    work->scale8C = work->scale8C * scale;
}

s64 func_00162938(void) {
    return parGetRestartFlag();
}

void parCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00162968(void) {
    func_001622E8();
}

void func_00162980(ParObj *work, u32 value) {
    work->valueF0 = value;
}

void func_00162988(u32 arg0, u8 arg1) {
    parObjSetMode(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001629A0);

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void func_001629C0(ParScaleObj *obj) {
    f32 scale;

    switch (obj->kind) {
    case 0:
    case 1:
        return;
    case 2:
        scale = obj->scale;
        D_00451F40[0] = D_00451F40[1] = D_00451F40[2] = scale;
        break;
    case 3:
        scale = obj->scale;
        D_00451F40[0] = D_00451F40[1] = D_00451F40[2] = scale;
        break;
    case 4:
        scale = obj->scale;
        D_00451F40[0] = D_00451F40[1] = D_00451F40[2] = scale;
        break;
    default:
        return;
    }
    VU0_LOAD_VF($vf10, D_0037F680);
    VU0_LOAD_VF($vf11, D_0037F690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    VU0_STORE_VF($vf10, D_00451F30);
}

void parDispatchKindUpdate(void *work) {
    switch (*(u16 *)work) {
    case 1:
        func_001618E0(*(s32 *)((u8 *)work + 8));
        return;
    case 2:
        func_00162B60(*(s32 *)((u8 *)work + 0x10));
        return;
    case 3:
        func_00162C48(*(s32 *)((u8 *)work + 0x14));
        return;
    case 4:
        func_00162D38(*(s32 *)((u8 *)work + 0x14));
        break;
    }
}

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
        func_00190148(*(s32 *)((u8 *)work + 0x14));
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162B60);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162C48);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162D38);

u32 func_00162E10(void) {
    return 0;
}

extern u8 D_00451F50[];

extern void func_00341348();

void parSysReset(void) {
    D_00436400 = 0;
    parControlInit();
    func_00341348(D_00451F50);
}

void func_00162E40(void) {
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162E48);

void func_00162FC8(u16 *arg0) {
    *arg0 = 1;
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 8));
}

void func_00163000(ParListNode *node) {
    node->next = D_00436400;
    D_00436400 = node;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163010);

void func_00163238(s32 arg0) {
    func_003332E8(*(u32 *)(arg0 + 0x40));
}

/* Draw parameter block filled per strip by func_00164CB0. */
typedef struct ParDrawState {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    void *unkC;
    s32 unk10;
    u8 pad14[0xC];
    s32 unk20;
    u8 pad24[8];
} ParDrawState;

extern ParDrawState D_00451F60;

void parControlInit(void) {
    memset(&D_00451F60, 0, 0x2C);
    D_00451F60.unk4 = 0x4000;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163290);

void func_001634A8(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 0x10));
}

/* 20-byte cell initialized by parCellInit (grey plus zeros). */
typedef struct ParCell {
    u128 *history;   /* 0x00 */
    void *vertices;  /* 0x04 */
    s32 vertexCount; /* 0x08: processed in groups of three */
    s32 unk0C;       /* 0x0C cleared */
    u32 color;       /* 0x10 set to grey 0x80808080 */
} ParCell; /* 0x14 */

typedef struct ParSystem {
    u8 pad00[4];
    s32 cellCount;       /* 0x04 */
    s32 vertexWordCount; /* 0x08 */
    u8 pad0C[8];
    ParCell *cells;      /* 0x14 */
} ParSystem;

void parCellInit(ParSystem *system, s32 index) {
    ParCell *cell = (ParCell *)(index * sizeof(ParCell) + (s32)system->cells);

    cell->color = 0x80808080;
    cell->unk0C = 0;
    cell->vertexCount = 0;
}

void func_00163508(ParCellNode *node) {
    node->next = D_00436404;
    D_00436404 = node;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163518);

void func_001635D0(ParSystem *system, s32 index, void *delta) {
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

INCLUDE_ASM(const s32, "game/code_00162348", func_00163628);

void func_001636F0(ParSystem *system, s32 index, void *delta) {
    ParCell *cell = system->cells + index;
    s32 count = cell->vertexCount / 3;
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

INCLUDE_ASM(const s32, "game/code_00162348", func_00163780);

INCLUDE_ASM(const s32, "game/code_00162348", func_001638D8);

void parFillVertexPairs(ParSystem *system, s32 arg1, s32 arg2) {
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

INCLUDE_ASM(const s32, "game/code_00162348", func_00163BC8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163D18);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163EE0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163F50);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164208);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164318);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164390);

INCLUDE_ASM(const s32, "game/code_00162348", func_001644B0);

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

INCLUDE_ASM(const s32, "game/code_00162348", func_00164690);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164748);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164848);

INCLUDE_ASM(const s32, "game/code_00162348", func_001648C0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001649E0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164AE8);

void func_00164C68(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
}

void parDispatchSub(void *work, s32 sub, void *a2, void *a3) {
    u16 id = *(u16 *)work;

    D_003AAF10[id * 3 + sub](work, a2, a3);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00164CB0);

ParBlock *func_00165300(s32 count) {
    s32 points = count * 3;
    s32 colorBytes = points * 4;
    s32 handle = func_003292A8((colorBytes + points) * 4 + 0x18);
    s32 base = sdfResourceRetainAddress(handle);
    u8 *vertices = (u8 *)base + points * 16;
    ParBlock *block = (ParBlock *)(vertices + colorBytes);
    block->color = 0x80808080;
    block->count = count;
    block->vertices = vertices;
    block->handle = handle;
    block->base = base;
    block->object = func_003335E0();
    func_003332D0(block->object, 1.0f);
    return block;
}

void func_001653A8(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x10));
    func_003297C8(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001653D8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00165500);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436400);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436404);

