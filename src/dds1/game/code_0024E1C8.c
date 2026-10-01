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
            effDestroyResourceSlotSet(D_0036C698[i]);
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
    D_003BC4CC = sdfCreateTaskWorker(D_003AF7A8, 0x402, 0x2B12, D_0036C648, func_0024F6F0, data);
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

/* One of the two 0x14-byte slots of a mantra source entry. */
typedef struct {
    s32 value;      /* 0x00 */
    u8 pad04[8];
    u32 slotFlags;      /* 0x0C: bit 5 marks this mantra source slot active */
    u8 pad10[4];
} MnuSourceSlot;

/* Mantra source entry behind prfReqGetEntryRecord (0x54 bytes). */
typedef struct {
    u8 pad00[0x2C];
    u32 flags;             /* 0x2C: bit 5 selects slot 0 */
    u8 pad30[4];
    MnuSourceSlot slot[2]; /* 0x34 */
} MnuSourceEntry;

extern MnuSourceEntry *prfReqGetEntryRecord(u16 index);

/* Pick the value of the first active slot, preferring slot 0. */
s32 mnuGetMantraSourceValue(u16 index) {
    s32 result = 0;
    MnuSourceEntry *entry = prfReqGetEntryRecord(index);
    s32 slot = 0;

    if (entry->flags & 0x20) {
        slot = 0;
    } else if (entry->slot[0].slotFlags & 0x20) {
        slot = 1;
    } else {
        return result;
    }
    return entry->slot[slot].value;
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F858);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F8D8);

/* Resource-task -> list -> selection chain used by the mantra display. */
typedef struct MnuResourceSelectionNode {
    u8 pad00[0x70];
    u32 selectionAddress;
} MnuResourceSelectionNode;

typedef struct MnuResourceList {
    u8 pad00[0x1C];
    MnuResourceSelectionNode *selectionNode;
} MnuResourceList;

typedef struct MnuResourceTask {
    u8 pad00[0xC];
    MnuResourceList *menuList;
} MnuResourceTask;

/* Return the selection record address used by labels and transition IDs. */
u32 mnuGetSelectedNodeValue(void) {
    MnuResourceTask *taskObject;

    taskObject = (MnuResourceTask *)func_002CB3B8(D_003BC4CC, 0);
    return taskObject->menuList->selectionNode->selectionAddress;
}

void mnuStopResourceAnimation(void) {
    s32 object = func_002CB3B8(D_003BC4CC, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)object)->menuList);
    mnuRetreatListCursorDefault(((MnuResourceTask *)object)->menuList);
}

void mnuResetResourceAnimation(void) {
    s32 object = func_002CB3B8(D_003BC4CC, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)object)->menuList);
    mnuAdvanceListCursorDefault(((MnuResourceTask *)object)->menuList);
}

extern s32 func_002D03F8(s32);
extern void *sdfMemoryGetBlockAddress(s32);
extern void *memset(void *, s32, u32);
extern void func_0024F8D8(void *);

u32 *mnuAllocateEmptyResourceListState(void) {
    s32 handle = func_002D03F8(0x10);
    u32 *block = sdfMemoryGetBlockAddress(handle);

    memset(block, 0, 0x10);
    block[0] = handle;
    func_0024F8D8(block);
    block[1] = 0;
    block[2] = 0;
    return block;
}

extern void sdfReleaseChipBlock(void *);
extern void mnuDestroyListState(void *);
extern void mnuReleaseMenuVisualWorkResources(s32);
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
        sdfReleaseChipBlock(node->resource);
        node = node->next;
    }
    sdfReleaseChipBlock(owner->resource);
    mnuDestroyListState(owner);
    mnuReleaseMenuVisualWorkResources(*(s32 *)(record + 0x24));
    func_002D0918(taskData[0]);
}

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF7A8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024FBB8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_002501E0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250758);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250820);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_002508D8);

typedef struct MnuSceneGridWork {
    u8 pad00[0x18];
    void (*callback)(void);
    void (*freeTaskData)(s32, void *);
} MnuSceneGridWork;

typedef struct MnuSceneContext {
    u8 pad00[0x484];
    MnuSceneGridWork *grid;
    u8 pad488[0x11C];
    u16 cursorX;
    u16 cursorY;
} MnuSceneContext;

extern MnuSceneGridWork *func_002CB9C0(s32, s32, s32, s32, s32, s32, void *, s32);
extern void sdfSetShortPairValues(MnuSceneGridWork *, s32, s32);
extern void mnuFreeTaskData(s32, void *);
extern void func_0025A680(void);
extern void func_002CC0D0(MnuSceneGridWork *);
extern void func_00253208(s32, s32, s32 *, s32 *);
extern void *sdfGridSelectFilledCell(MnuSceneGridWork *, s32, s32);
extern void func_002512F0(s32, s32);

void func_00250978(s32 context) {
    MnuSceneContext *scene = (MnuSceneContext *)context;
    s32 coordinates[2];
    s32 record;
    s32 field;

    scene->grid = func_002CB9C0(0xF, 0x11, 0x40, 0x43, 4, 4,
                                (u8 *)scene + 4, 0);
    sdfSetShortPairValues(scene->grid, 1, 1);
    scene->grid->freeTaskData = mnuFreeTaskData;
    scene->grid->callback = func_0025A680;
    record = func_002CB3B8(D_003BC4CC, 0);
    field = *(s32 *)(*(s32 *)(record + 0xC) + 0x1C);
    func_00253208(context, *(s32 *)(field + 0x70),
                  &coordinates[0], &coordinates[1]);
    if (sdfGridSelectFilledCell(scene->grid, coordinates[0], coordinates[1]) == NULL) {
        func_002CC0D0(scene->grid);
    }
    scene->cursorX = 0;
    scene->cursorY = 0;
    func_002512F0(context, 1);
}

