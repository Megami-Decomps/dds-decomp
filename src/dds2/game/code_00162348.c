#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"

/* Particle dispatch object; see the matching DDS1 game unit. */
typedef struct ParObj {
    u8 pad0[0x8C];
    f32 scale8C;
    u8 pad90[0x60];
    u32 unkF0;
} ParObj;

typedef struct ParListNode {
    u8 pad00[0x54];
    struct ParListNode *next;
} ParListNode;

typedef struct ParCellNode {
    u8 pad00[0x24];
    struct ParCellNode *next;
} ParCellNode;

extern ParListNode *D_00436400;

extern ParCellNode *D_00436404;

extern void (*D_003AAF10[])(void *, void *, void *);

extern BillDispatch D_003AAB88[];

extern void parGetRestartFlag();

extern u8 parObjGetMode();

void func_00162348(ParObj *work, u32 value) {
    work->unkF0 = value;
}

void parObjSetMode(u8 *object, u8 mode) {
    switch (*(u16 *)(object + 0x140)) {
    case 1:
    case 5:
    case 11:
        object[0x150] = mode;
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
        object[0x151] = mode;
        break;
    }
    *(u16 *)(object + 0x142) = 1;
}

u8 parObjGetMode(u8 *object) {
    switch (*(u16 *)(object + 0x140)) {
    case 1:
    case 5:
    case 11:
        return object[0x150];
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        return object[0x151];
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001623D0);

INCLUDE_ASM(const s32, "game/code_00162348", parInstantiateKind);

void parObjDispatch(u8 *object) {
    D_003AAB88[*(u16 *)(object + 0x140)].func();
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162590);

void func_001628E0(void) {
    parRestartKind();
}

void effParScaleComponent(float scale, ParObj *work) {
    func_00162248();
    work->scale8C = work->scale8C * scale;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162938);

void func_00162958(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00162968(void) {
    func_001622E8();
}

void func_00162980(ParObj *work, u32 value) {
    work->unkF0 = value;
}

void func_00162988(u32 arg0, u8 arg1) {
    parObjSetMode(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001629A0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001629C0);

INCLUDE_ASM(const s32, "game/code_00162348", parDispatchKindUpdate);

INCLUDE_ASM(const s32, "game/code_00162348", parDispatchKindInit);

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

INCLUDE_ASM(const s32, "game/code_00162348", parCellInit);

void func_00163508(ParCellNode *node) {
    node->next = D_00436404;
    D_00436404 = node;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163518);

INCLUDE_ASM(const s32, "game/code_00162348", func_001635D0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163628);

INCLUDE_ASM(const s32, "game/code_00162348", func_001636F0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163780);

INCLUDE_ASM(const s32, "game/code_00162348", func_001638D8);

typedef struct ParCell {
    u8 pad00[4];
    void *vertices; /* 0x04 */
    u8 pad08[0xC];
} ParCell;

typedef struct ParSystem {
    u8 pad00[4];
    s32 cellCount;       /* 0x04 */
    s32 vertexWordCount; /* 0x08 */
    u8 pad0C[8];
    ParCell *cells;      /* 0x14 */
} ParSystem;

void func_00163B68(ParSystem *system, s32 arg1, s32 arg2) {
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

INCLUDE_ASM(const s32, "game/code_00162348", func_00165300);

void func_001653A8(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x10));
    func_003297C8(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001653D8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00165500);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436400);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436404);

