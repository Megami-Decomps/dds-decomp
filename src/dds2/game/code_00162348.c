#include "common.h"

#include "eff.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

/* Particle object layout mirrors the matching DDS1 unit and parManager. */
typedef struct ParObj {
    u8 pad00[0x10];    /* 0x00 */
    f32 unk10;          /* 0x10 */
    f32 unk14;          /* 0x14 */
    u8 pad18[0x10];    /* 0x18 */
    s32 unk28;          /* 0x28 */
    s16 unk2C;          /* 0x2C */
    u8 pad2E[0x5E];    /* 0x2E */
    f32 scale8C;       /* 0x8C */
    u8 pad90[0x14];    /* 0x90 */
    u32 unkA4;         /* 0xA4 */
    u8 padA8[0x48];    /* 0xA8 */
    u32 valueF0;       /* 0xF0 */
    s32 billId;        /* 0xF4 */
    u8 padF8[4];       /* 0xF8 */
    void *unkFC;       /* 0xFC */
    u8 pad100[0x40];   /* 0x100 */
    u16 dispatchIndex; /* 0x140: particle dispatch table index */
    u16 restartFlag;   /* 0x142: set after mode changes */
    u8 pad144[0x0C];
    u8 mode150;
    u8 mode151;
    u8 pad152[0x22];
    void *child;       /* 0x174 */
} ParObj;

typedef struct ParListNode {
    u8 pad00[0x54];
    struct ParListNode *next;
} ParListNode;

typedef struct ParCellNode {
    u8 pad00[0x24];
    struct ParCellNode *next;
} ParCellNode;

/* Kind resource owner: release flag and handles at +0x10/+0x40. */
typedef struct ParReleaseRecord {
    u16 released;       /* 0x00 */
    u8 pad02[0x0E];
    u32 allocation;     /* 0x10 */
    u8 pad14[0x2C];
    u32 asset;          /* 0x40 */
} ParReleaseRecord;

typedef struct ParScaleObj {
    u16 kind;
    u8 pad2[6];
    f32 scale; /* 0x8 */
} ParScaleObj;

extern f32 D_00451F30[];

extern f32 D_00451F40[];

extern u8 D_0037F680[];

extern u8 D_0037F690[];

extern f32 D_003AAEC0[];

extern f32 D_003AAEE0[];

extern f32 D_003AAEF0[];

extern f32 D_003AAF00[];

extern ParListNode *D_00436400;

extern ParCellNode *D_00436404;

extern void (*D_003AAF10[])(void *, void *, void *);

extern BillDispatch D_003AAB88[];

extern s32 parGetRestartFlag();

extern void parCellInit();

/* 20-byte cell initialized by parCellInit (grey plus zeros). */
typedef struct ParCell {
    u128 *history;   /* 0x00 */
    void *vertices;  /* 0x04 */
    s32 vertexCount; /* 0x08: processed in groups of three */
    s32 unk0C;       /* 0x0C cleared */
    u32 color;       /* 0x10 set to grey 0x80808080 */
} ParCell; /* 0x14 */

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

extern s32 parObjGetMode();

extern void func_001618E0(s32);

extern void parUpdateBillboardCrossStrip();

extern void parUpdateBillboardCrossTriangle();

extern void parUpdateTrackPolygonCrossAxes();

extern void effBillSetEntryValue(s32, s32, u32);

extern void func_00190120(s32, s32, void *);

extern void effTrackPolySetIndexedColor(s32, s32, u32);

extern void parFadeAlphaCell(s32, s32);

extern void func_001638D8(s32, s32);

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

extern s32 func_00167A10();

extern void *memcpy(void *dst, void *src, u32 n);
extern void *func_00328D68(s32);
extern void func_00165600(void *);

/* Emitter descriptor copied into a fresh allocation by parCloneEmitterAndInitCells. */
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

extern void parClearSlotFlag(s32);

extern void effTrackPolyResetIndexedWork(s32);

typedef struct ParBlock {
    s32 count;       /* 0x00 */
    u32 color;       /* 0x04 */
    s32 base;        /* 0x08 */
    u8 *vertices;    /* 0x0C */
    s32 object;      /* 0x10 */
    s32 handle;      /* 0x14 */
} ParBlock;

extern s32 sdfCreateAssetWithDrawEntries();

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

s32 parObjGetMode(ParObj *object) {
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

extern BillDispatch D_003AAB80[];

extern s32 billCloneObjectRetainingSharedData(s32 id);

extern void billSetChildScaleComponents(s32 id, f32 a, f32 b);

extern void billSetBillboardMode(s32 id, s16 mode);

extern void billMarkKindOneFlag(s32 id);

INCLUDE_ASM(const s32, "game/code_00162348", func_001623D0);

ParObj *parInstantiateKind(ParObj *src) {
    ParObj *obj;
    s32 bill;

    obj = D_003AAB80[src->dispatchIndex].func();
    obj->dispatchIndex = src->dispatchIndex;
    if (src->unk28 == -1) {
        bill = billCloneObjectRetainingSharedData(src->billId);
        billSetChildScaleComponents(bill, obj->unk10, obj->unk14);
        billSetBillboardMode(bill, obj->unk2C);
        billMarkKindOneFlag(bill);
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
    parScaleAndRestartKind();
    work->scale8C = work->scale8C * scale;
}

s64 func_00162938(void) {
    return parGetRestartFlag();
}

void parCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00162968(void) {
    parComposeEffectTransformMatrices();
}

void func_00162980(ParObj *work, u32 value) {
    work->valueF0 = value;
}

void func_00162988(ParObj *work, u8 mode) {
    parObjSetMode(work, mode);
}

s64 func_001629A0(ParObj *work) {
    return parObjGetMode(work);
}

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void parUpdateSharedScaleAndDelta(ParScaleObj *obj) {
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

void parDispatchKindUpdate(ParSystem *work) {
    switch ((u16)work->kind) {
    case 1:
        func_001618E0(work->vertexWordCount);
        return;
    case 2:
        parUpdateBillboardCrossStrip(work->handle);
        return;
    case 3:
        parUpdateBillboardCrossTriangle((s32)work->cells);
        return;
    case 4:
        parUpdateTrackPolygonCrossAxes((s32)work->cells);
        break;
    }
}

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
        effTrackPolyResetIndexedWork((s32)work->cells);
        break;
    }
}

void parUpdateBillboardCrossStrip(s32 particle, s32 index, u32 color) {
    u128 axis[2];
    VU0_MOVE_VF(vf11, vf12);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00451F30);
;
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, D_00451F40);
;
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf12, vf2);
    VU0_MOVE_VF(vf11, vf10);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, &axis[0]);
;
    VU0_MOVE_VF(vf10, vf12);
    VU0_SUB(vf11, vf11, vf10);
    VU0_STORE_VF(vf11, &axis[1]);
;
    func_00163518(particle, index, axis);
    parFadeAlphaCell(particle, index);
    effBillSetEntryValue(particle, index, (color & 0xFF000000) | 0x808080);
}

void parUpdateBillboardCrossTriangle(s32 particle, s32 index, u32 color) {
    u128 axis[3];
    VU0_MOVE_VF(vf11, vf12);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00451F30);
;
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, D_00451F40);
;
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf12, vf2);
    VU0_STORE_VF(vf10, &axis[1]);
;
    VU0_MOVE_VF(vf11, vf10);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, &axis[0]);
;
    VU0_MOVE_VF(vf10, vf12);
    VU0_SUB(vf11, vf11, vf10);
    VU0_STORE_VF(vf11, &axis[2]);
;
    func_00163628(particle, index, axis);
    func_001638D8(particle, index);
    effBillSetEntryValue(particle, index, (color & 0xFF000000) | 0x808080);
}

void parUpdateTrackPolygonCrossAxes(s32 particle, s32 index, u32 color) {
    u128 axis[2];
    VU0_MOVE_VF(vf11, vf12);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00451F30);
;
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, D_00451F40);
;
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf12, vf2);
    VU0_MOVE_VF(vf11, vf10);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, &axis[0]);
;
    VU0_MOVE_VF(vf10, vf12);
    VU0_SUB(vf11, vf11, vf10);
    VU0_STORE_VF(vf11, &axis[1]);
;
    func_00190120(particle, index, axis);
    effTrackPolySetIndexedColor(particle, index, (color & 0xFF000000) | 0x808080);
}

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

void parReleaseAssetRecord(ParReleaseRecord *record) {
    record->released = 1;
    sdfQueueAssetRelease(record->asset);
    func_003297C8(record->allocation);
}

void parPrependRecordListNode(ParListNode *node) {
    node->next = D_00436400;
    D_00436400 = node;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163010);

void func_00163238(ParReleaseRecord *record) {
    func_003332E8(record->asset);
}

/* Draw parameter block filled per strip by func_00164CB0. */
typedef struct ParDrawState {
    s16 width;    /* 0x00 */
    s16 height;   /* 0x02 */
    s16 flags;    /* 0x04 */
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
    D_00451F60.flags = 0x4000;
}

ParSystem *parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind) {
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
    handle = func_003292A8(cellsSize + 0x2C);
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
    system->object = sdfCreateAssetWithDrawEntries();
    func_003332D0(system->object, 1.0f);
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

void parReleaseCellSystem(ParSystem *system) {
    sdfQueueAssetRelease(system->object);
    func_003297C8(system->handle);
}

void parCellInit(ParSystem *system, s32 index) {
    ParCell *cell = (ParCell *)(index * sizeof(ParCell) + (s32)system->cells);

    cell->color = 0x80808080;
    cell->unk0C = 0;
    cell->vertexCount = 0;
}

void parPrependCellNode(ParCellNode *node) {
    node->next = D_00436404;
    D_00436404 = node;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163518);

void parTranslateCellVertices(ParSystem *system, s32 index, void *delta) {
    ParCell *cell = system->cells + index;
    s32 count = system->vertexWordCount;
    u8 *vertex = *(u8 **)cell;
    s32 i;
    VU0_LOAD_VF_MEMORY(vf11, delta);
;
    if (count > 0) {
        i = count;
        do {
            VU0_LOAD_VF(vf10, vertex);
;
            VU0_ADD(vf10, vf10, vf11);
;
            VU0_STORE_VF(vf10, vertex);
;
            i--;
            vertex += 0x10;
        } while (i != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163628);

void parTranslateCellTriangleVertices(ParSystem *system, s32 index, void *delta) {
    ParCell *cell = system->cells + index;
    s32 count = cell->vertexCount / 3;
    u8 *vertex = (u8 *)cell->history;
    s32 i;
    VU0_LOAD_VF(vf11, delta);
;
    if (count > 0) {
        i = count;
        do {
            VU0_LOAD_VF(vf10, vertex);
;
            VU0_ADD(vf10, vf10, vf11);
;
            VU0_STORE_VF(vf10, vertex);
;
            VU0_LOAD_VF(vf10, vertex + 0x10);
;
            VU0_ADD(vf10, vf10, vf11);
;
            VU0_STORE_VF(vf10, vertex + 0x10);
;
            VU0_LOAD_VF(vf10, vertex + 0x20);
;
            VU0_ADD(vf10, vf10, vf11);
;
            VU0_STORE_VF(vf10, vertex + 0x20);
;
            i--;
            vertex += 0x30;
        } while (i != 0);
    }
}

void parFadeAlphaCell(s32 particle, s32 index) {
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
        VU0_LOAD_VF(vf10, D_003AAEC0);
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

INCLUDE_ASM(const s32, "game/code_00162348", func_001638D8);

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

void parFadeAlphaAllCells(ParSystem *system, u32 color) {
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
    VU0_LOAD_VF(vf10, D_003AAEE0);
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

void parFadeAlphaUpDownAllCells(ParSystem *system, u32 color) {
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
    VU0_LOAD_VF(vf10, D_003AAEF0);
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

void parFillTriangleCellColors(ParSystem *system, s32 middleWord, s32 edgeWord) {
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

void parFadeAlphaTriangleAllCells(ParSystem *system, u32 middleWord, u32 edgeWord) {
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
    VU0_LOAD_VF(vf10, D_003AAF00);
    VU0_LERP_VF10(1.0f / perCell);
    VU0_STORE_VF(vf10, step0);
    alpha1[0] = edgeWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003AAF00);
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

INCLUDE_ASM(const s32, "game/code_00162348", func_00164208);

/* Ordered packed color words for the three particle vertex layouts. */
typedef struct ParStripVertexColors {
    s32 edge0;
    s32 middle0;
    s32 center0;
    s32 center1;
    s32 middle1;
    s32 edge1;
} ParStripVertexColors;

typedef struct ParQuadVertexColors {
    s32 edge0;
    s32 middle0;
    s32 middle1;
    s32 edge1;
} ParQuadVertexColors;

typedef struct ParSymmetricVertexColors {
    s32 edge0;
    s32 middle0;
    s32 center;
    s32 middle1;
    s32 edge1;
} ParSymmetricVertexColors;

void parFillStripCellColors(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
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
                    ((ParStripVertexColors *)vertex)->edge0 = edgeWord;
                    ((ParStripVertexColors *)vertex)->middle0 = middleWord;
                    ((ParStripVertexColors *)vertex)->center0 = centerWord;
                    ((ParStripVertexColors *)vertex)->center1 = centerWord;
                    ((ParStripVertexColors *)vertex)->middle1 = middleWord;
                    ((ParStripVertexColors *)vertex)->edge1 = edgeWord;
                    vertex += 0x18;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00164390);

INCLUDE_ASM(const s32, "game/code_00162348", func_001644B0);

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
                    ((ParQuadVertexColors *)vertex)->middle1 = middleWord;
                    ((ParQuadVertexColors *)vertex)->middle0 = middleWord;
                    ((ParQuadVertexColors *)vertex)->edge1 = edgeWord;
                    ((ParQuadVertexColors *)vertex)->edge0 = edgeWord;
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

void parFillSymmetricCellColors(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
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
                    ((ParSymmetricVertexColors *)vertex)->edge0 = edgeWord;
                    ((ParSymmetricVertexColors *)vertex)->middle0 = middleWord;
                    ((ParSymmetricVertexColors *)vertex)->center = centerWord;
                    ((ParSymmetricVertexColors *)vertex)->middle1 = middleWord;
                    ((ParSymmetricVertexColors *)vertex)->edge1 = edgeWord;
                    vertex += 0x14;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001648C0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001649E0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164AE8);

void func_00164C68(s32 recordAddress, u16 value) {
    *(u16 *)(recordAddress + 2) = value;
}

void parDispatchSub(void *work, s32 sub, void *a2, void *a3) {
    u16 id = *(u16 *)work;

    D_003AAF10[id * 3 + sub](work, a2, a3);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00164CB0);

ParBlock *parAllocateDrawBlock(s32 count) {
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
    block->object = sdfCreateAssetWithDrawEntries();
    func_003332D0(block->object, 1.0f);
    return block;
}

void parReleaseDrawBlock(ParBlock *block) {
    sdfQueueAssetRelease(block->object);
    func_003297C8(block->handle);
}

void parSubmitCellDrawPackets(ParDrawCmd *emitter, ParDrawCmd *cmd) {
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
    state.unk8 = cmd->unk4;
    while (remaining >= 0x30) {
        remaining -= 0x30;
        sdfAppendPacket(list, func_00167A10(&state));
    }
    if (remaining > 0) {
        u16 *counts = (u16 *)&D_00451F60;
        counts[0] = remaining / 3;
        counts[1] = remaining;
        sdfAppendPacket(list, func_00167A10(&state));
    }
    emitter->finish(emitter, list);
}

ParEmitDesc *parCloneEmitterAndInitCells(ParEmitDesc *src) {
    ParEmitDesc *desc = func_00328D68(src->count * 4 + 0xF0);

    memset(desc, 0, 0xF0);
    memcpy(desc, src, src->headerSize);
    memcpy((u8 *)desc + 0xC0, (u8 *)src + src->headerSize, 0x30);
    desc->headerSize = 0xC0;
    desc->unkE0 = (u8 *)desc + 0xF0;
    if (desc->unkC8 < 3) {
        desc->unkC8 = 3;
    }
    desc->unkDC = parAllocateCellSystem(desc->count, desc->unkC8, 1, 0);
    parDispatchSub(desc->unkDC, 0, desc->unkD4, desc->unkD8);
    func_00165600(desc);
    return desc;
}

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436400);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436404);

