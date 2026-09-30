#include "common.h"

extern s32 func_002CB3B8(u32, u32);
extern void func_0024F6F0(s32, s32);


extern u8 D_003AF7A8[];

extern u32 D_003BC4CC;
extern s32 D_0036C698[];
extern u8 D_0036C648[];


INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E1C8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E260);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E310);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E3C0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E470);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E5A0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E728);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E8D0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EA50);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EC08);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EDC0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EF68);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F0D0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F210);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F338);

extern u8 D_0036C568[];
extern void effRequestResourceByMode(char *, void *, s32, void *);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF720);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF730);

void mnuRequestMantraResources(void) {
    s32 i;

    for (i = 0; i < 14; i++) {
        if (D_0036C698[i] == 0) {
            effRequestResourceByMode("/facility/spr/mantra/", &D_0036C568[i * 0x10], 0, &D_0036C698[i]);
        }
    }
}

s32 mnuAreResourceSlotsOccupied(void) {
    s32 i;
    for (i = 0; i < 14; ++i) {
        if (D_0036C698[i] == 0) {
            return 0;
        }
    }
    return 1;
}

void mnuReleaseResourceSlots(void) {
    s32 i;
    for (i = 0; i < 14; ++i) {
        if (D_0036C698[i] != 0) {
            func_002BDD60(D_0036C698[i]);
            D_0036C698[i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F608);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F6F0);

/* The task handle is shared by the existence probe and explicit stop;
 * both clear it when the resource group is no longer active. */
void mnuCreateResourceTask(void) {
    s32 data = func_0024F608();
    D_003BC4CC = func_002CB1C8(D_003AF7A8, 0x402, 0x2B12, D_0036C648, func_0024F6F0, data);
}

s32 mnuCheckResourceTask(void) {
    if (kwlnTaskExists(D_003AF7A8) != 0) {
        return 1;
    }
    D_003BC4CC = 0;
    return 0;
}

void mnuStopResourceTask(void) {
    sdfDestroyTaskWorkerTasks(D_003BC4CC);
    D_003BC4CC = 0;
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F800);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F858);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F8D8);

/* The current resource task stores this value three pointers below the task object. */
u32 func_0024FA18(void) {
    s32 taskObject;

    taskObject = func_002CB3B8(D_003BC4CC, 0);
    return *(u32 *)(*(s32 *)(*(s32 *)(taskObject + 0xc) + 0x1c) + 0x70);
}

void mnuStopResourceAnimation(void) {
    s32 object = func_002CB3B8(D_003BC4CC, 0);
    mnuClearListFlagsOneAndTwo(*(s32 *)(object + 0xC));
    func_0027BEB0(*(s32 *)(object + 0xC));
}

void mnuResetResourceAnimation(void) {
    s32 object = func_002CB3B8(D_003BC4CC, 0);
    mnuClearListFlagsOneAndTwo(*(s32 *)(object + 0xC));
    func_0027BE90(*(s32 *)(object + 0xC));
}

extern s32 func_002D03F8(s32);
extern void *sdfMemoryGetBlockAddress(s32);
extern void *memset(void *, s32, u32);
extern void func_0024F8D8(void *);

u32 *func_0024FAC8(void) {
    s32 handle = func_002D03F8(0x10);
    u32 *block = sdfMemoryGetBlockAddress(handle);

    memset(block, 0, 0x10);
    block[0] = handle;
    func_0024F8D8(block);
    block[1] = 0;
    block[2] = 0;
    return block;
}

extern void func_002CFF98(void *);
extern void func_0027B368(void *);
extern void func_00249930(s32);
extern void func_002D0918(s32);

typedef struct MenuCleanupNode {
    u8 pad00[0x58];
    struct MenuCleanupNode *next;
    u8 pad5C[0x14];
    void *resource;
} MenuCleanupNode;

typedef struct MenuCleanupOwner {
    u8 pad00[0x1C];
    MenuCleanupNode *first;
    u8 pad20[0x10];
    void *resource;
} MenuCleanupOwner;

/* Tear down the linked resource nodes and release the task's allocation. */
void mnuReleaseResourceTaskData(s32 unused, s32 *taskData) {
    MenuCleanupOwner *owner = (MenuCleanupOwner *)taskData[3];
    MenuCleanupNode *node = owner->first;
    u8 *record = (u8 *)func_002CB3B8(D_003BC4CC, -1);

    while (node != NULL) {
        func_002CFF98(node->resource);
        node = node->next;
    }
    func_002CFF98(owner->resource);
    func_0027B368(owner);
    func_00249930(*(s32 *)(record + 0x24));
    func_002D0918(taskData[0]);
}

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF7A8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024FBB8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_002501E0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250758);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250820);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_002508D8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250978);

