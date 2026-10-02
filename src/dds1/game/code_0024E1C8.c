#include "common.h"

extern s32 func_002CB3B8(u32, u32);
extern void func_0024F6F0(s32, s32);

extern u8 *datGameState;
extern s32 scrGetSelectedOperandIndex(void *);
extern u32 ptyGetProfileRecordValue(void *, u16);
extern u32 prfGetCapValue(u16);

typedef struct MnuProfileProgress {
    void *unit;
    s32 profileId;
    u32 value;
    u32 cap;
} MnuProfileProgress;


extern u8 mnuResourceTaskName[];

extern u32 mnuSceneResourceContext;
extern s32 D_0036C698[];
extern u8 D_0036C648[];

typedef s16 MnuSpritePlacement[4];

typedef struct MnuSpriteWork {
    u8 pad00[0x24];
    f32 rotation;
    u8 pad28[0x78];
} MnuSpriteWork;

typedef struct MnuSpriteResource {
    u8 pad00[0x18];
    MnuSpriteWork *sprites;
} MnuSpriteResource;

typedef union MnuVariantSpritePlacement {
    struct {
        u8 pad00[4];
        s16 x;
        s16 y;
        s16 spriteGroups;
        u16 flags;
    } fields;
    s16 values[6];
} MnuVariantSpritePlacement;

enum {
    MNU_VARIANT_X_OFFSET = 2,
    MNU_VARIANT_Y_OFFSET,
};

enum {
    MNU_SPRITE_RESOURCE_INDEX,
    MNU_SPRITE_INDEX,
    MNU_SPRITE_X_OFFSET,
    MNU_SPRITE_Y_OFFSET,
};

extern MnuSpritePlacement D_0036B510[];
extern MnuVariantSpritePlacement D_0036B7F0[];
extern s32 D_0036C6AC[];
extern s32 func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);

s32 func_0024E1C8(s32 x, s32 y, s32 z, s32 alpha, s32 sprite, s32 placementIndex,
                  s32 flags, s32 context) {
    return func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         flags,
                         sprite,
                         D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

s32 func_0024E260(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context) {
    return func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         0,
                         D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

s32 func_0024E310(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 mode,
                  s32 context) {
    return func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         0,
                         D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         mode,
                         context);
}

s32 func_0024E3C0(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 placementIndex,
                  s32 context) {
    return func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         flags,
                         D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void func_0024E470(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 placementIndex,
                   s32 context, f32 rotation) {
    ((MnuSpriteResource *)D_0036C698[
        D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]])
        ->sprites[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].rotation = rotation;
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                  (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                  context);
    ((MnuSpriteResource *)D_0036C698[
        D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]])
        ->sprites[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].rotation = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E5A0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E728);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E8D0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EA50);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EC08);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EDC0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EF68);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F0D0);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF720);

void func_0024F210(s32 x, s32 y, s32 z, s32 alpha, s32 groupPlacementIndex,
                   s32 coordinatePlacementIndex, s32 selector, s32 flags,
                   f32 scaleX, f32 scaleY, s32 context) {
    s8 spriteMap[12] = { -1, 12, 26, 27, 13, 14, 15, 16, 20, 21, 23, 24 };
    s32 sprite;

    selector += 2;
    sprite = spriteMap[((groupPlacementIndex + D_0036B7F0)->fields.spriteGroups >>
                        (selector * 4)) & 0xF];
    if (sprite == -1) {
        return;
    }
    func_002BF4E0((s32)((f32)(x + D_0036B7F0[coordinatePlacementIndex]
                                              .values[MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + D_0036B7F0[coordinatePlacementIndex]
                                              .values[MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C6AC[0],
                  sprite,
                  context);
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F338);

extern u8 D_0036C568[];
extern void effRequestResourceByMode(char *, void *, s32, void *);

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
    mnuSceneResourceContext = sdfCreateTaskWorker(mnuResourceTaskName, 0x402, 0x2B12, D_0036C648, func_0024F6F0, data);
}

s32 mnuCheckResourceTask(void) {
    if (kwlnTaskExists(mnuResourceTaskName) != 0) {
        return 1;
    }
    mnuSceneResourceContext = 0;
    return 0;
}

void mnuStopResourceTask(void) {
    sdfDestroyTaskWorkerTasks(mnuSceneResourceContext);
    mnuSceneResourceContext = 0;
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

void mnuInitializeProfileProgress(u16 index, MnuProfileProgress *progress) {
    s32 offset = index * 0x1A4;
    void *unit = datGameState + offset + 0xA60;

    progress->unit = unit;
    progress->profileId = scrGetSelectedOperandIndex(unit);
    progress->value = ptyGetProfileRecordValue(datGameState + offset + 0xA60,
                                              progress->profileId);
    progress->cap = prfGetCapValue(progress->profileId);
}

/* Resource-task -> list -> selection chain used by the mantra display. */
typedef struct MnuResourceSelectionNode {
    u8 pad00[0x70];
    u32 selectionAddress;
} MnuResourceSelectionNode;

typedef struct MnuResourceList {
    u8 pad00[0x1C];
    MnuResourceSelectionNode *selectionNode;
    u8 pad20[0xC];
    void (*drawCallback)();
    s32 *drawValues;
} MnuResourceList;

typedef struct MnuResourceTask {
    u8 pad00[0xC];
    MnuResourceList *menuList;
} MnuResourceTask;

typedef struct MnuPartyRecord {
    u16 flags;
    u8 pad02[2];
    u16 unitId;
    u8 pad06[0x19E];
} MnuPartyRecord;

extern MnuResourceList *mnuCreateListState(s32, s32, s32);
extern MnuResourceSelectionNode *mnuListAppendNode(MnuResourceList *, s32);
extern void *sdfAllocSizeClassBlock(s32);
extern void func_00254C68();
extern void *memset(void *, s32, u32);

void mnuBuildMantraPartyList(MnuResourceTask *task) {
    u16 partyOrder[32];
    s32 i = 0;
    u16 *order;
    MnuResourceList *list = mnuCreateListState(0, 6, 0x1A);

    list->drawCallback = func_00254C68;
    list->drawValues = sdfAllocSizeClassBlock(8);
    memset(list->drawValues, 0, 8);
    memset(partyOrder, 0, sizeof(partyOrder));
    do {
        s32 offset = i * sizeof(MnuPartyRecord) + 0xA60;
        MnuPartyRecord *party = (MnuPartyRecord *)(datGameState + offset);
        u16 active = party->flags & 1;
        if (active != 0) {
            partyOrder[party->unitId] = i + 1;
        }
        i++;
    } while (i < 5);
    order = partyOrder;
    i = 31;
    do {
        if (*order != 0) {
            MnuResourceSelectionNode *node = mnuListAppendNode(list, 0);
            MnuProfileProgress *progress = sdfAllocSizeClassBlock(sizeof(MnuProfileProgress));
            node->selectionAddress = (u32)progress;
            mnuInitializeProfileProgress(*order - 1, progress);
        }
        order++;
    } while (--i >= 0);
    task->menuList = list;
}


/* Return the selection record address used by labels and transition IDs. */
u32 mnuGetSelectedNodeValue(void) {
    MnuResourceTask *taskObject;

    taskObject = (MnuResourceTask *)func_002CB3B8(mnuSceneResourceContext, 0);
    return taskObject->menuList->selectionNode->selectionAddress;
}

void mnuStopResourceAnimation(void) {
    s32 object = func_002CB3B8(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)object)->menuList);
    mnuRetreatListCursorDefault(((MnuResourceTask *)object)->menuList);
}

void mnuResetResourceAnimation(void) {
    s32 object = func_002CB3B8(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)object)->menuList);
    mnuAdvanceListCursorDefault(((MnuResourceTask *)object)->menuList);
}

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfMemoryGetBlockAddress(s32);

u32 *mnuAllocateEmptyResourceListState(void) {
    s32 handle = sdfAllocGeneralBlock(0x10);
    u32 *block = sdfMemoryGetBlockAddress(handle);

    memset(block, 0, 0x10);
    block[0] = handle;
    mnuBuildMantraPartyList((MnuResourceTask *)block);
    block[1] = 0;
    block[2] = 0;
    return block;
}

extern void sdfReleaseChipBlock(void *);
extern void mnuDestroyListState(void *);
extern void mnuReleaseMenuVisualWorkResources(s32);
extern void sdfReleaseResourceAllocation(s32);

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
    u8 *record = (u8 *)func_002CB3B8(mnuSceneResourceContext, -1);

    while (node != NULL) {
        sdfReleaseChipBlock(node->resource);
        node = node->next;
    }
    sdfReleaseChipBlock(owner->resource);
    mnuDestroyListState(owner);
    mnuReleaseMenuVisualWorkResources(*(s32 *)(record + 0x24));
    sdfReleaseResourceAllocation(taskData[0]);
}

INCLUDE_RODATA(const s32, "game/code_0024E1C8", mnuResourceTaskName);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024FBB8);

extern void mnuDrawDisplaySpriteAndPanelMarks(s32, s32, s32);
extern void func_002546D8(s32, s32);
extern void func_00254758(s32, s32, s32, s32, s32);
extern void func_00254778(s32, s32, s32, s32, s32);
extern void mnuDrawDisplayModeSprites(s32, s32, s32, s32, s32, s32);
extern void func_00254B30(s32, s32, s32, s32, s32, s32);
extern void itfDspInitSelectedWindow(s32, s32, s32, s32, s32, s32);
extern void func_00254810(s32, s32, s32, s32, s32, s32);

/* Scene draw state: fade phase 1..5 and its frame counter. */
typedef struct MenuFadeWork {
    u8 pad00[4];
    s32 phase;      /* 0x04 */
    s32 timer;      /* 0x08 */
} MenuFadeWork;

/* Per-phase window draw: a sprite/panel fade driven by the frame counter (phases 1/5 fade in, 2 fades out, 3 fades in reversed, 4 holds). */
s32 func_002501E0(s32 unused, MenuFadeWork *work) {
    f32 shade;
    f32 elapsed;
    f32 ratio;
    s32 sel = func_002CB3B8(mnuSceneResourceContext, -1);
    s32 amount;
    s32 limit;

    switch (work->phase) {
    case 1:
        ratio = (f32)work->timer / 10.0f;
        shade = ratio + ratio;
        if (shade > 1.0f) {
            shade = 1.0f;
        }
        amount = (s32)(ratio * 128.0f);
        mnuDrawDisplaySpriteAndPanelMarks(sel, amount, 0x52);
        func_002546D8(amount, 0x52);
        func_00254758(0, 0, 1, amount, 0x53);
        func_00254778(0, 0, 1, amount, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, amount, (s32)work, 0x53);
        func_00254B30(0, (s32)((1.0f - shade) * -24.0f), 1, amount, (s32)work, 0x53);
        if (ratio < 0.5f) {
            shade = 0.0f;
        } else {
            shade = (ratio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shade * 128.0f), (s32)work, 0x53);
        func_00254810(0, 0, 1, (s32)(ratio * 128.0f), (s32)work, 0x53);
        return 0;
    case 5:
        ratio = (f32)work->timer / 10.0f;
        shade = ratio + ratio;
        if (shade > 1.0f) {
            shade = 1.0f;
        }
        mnuDrawDisplaySpriteAndPanelMarks(sel, 0x80, 0x52);
        amount = (s32)(ratio * 128.0f);
        func_002546D8(amount, 0x52);
        func_00254758(0, 0, 1, 0x80, 0x53);
        func_00254778(0, 0, 1, amount, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, amount, (s32)work, 0x53);
        func_00254B30(0, (s32)((1.0f - shade) * -24.0f), 1, amount, (s32)work, 0x53);
        if (ratio < 0.5f) {
            shade = 0.0f;
        } else {
            shade = (ratio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shade * 128.0f), (s32)work, 0x53);
        func_00254810(0, 0, 1, (s32)(ratio * 128.0f), (s32)work, 0x53);
        return 0;
    case 2:
        limit = work->timer;
        ratio = (f32)limit / 10.0f;
        if (limit < 4) {
            shade = (f32)limit * 0.25f;
        } else {
            shade = 1.0f;
        }
        mnuDrawDisplaySpriteAndPanelMarks(sel, 0x80, 0x52);
        amount = (s32)((1.0f - ratio) * 128.0f);
        func_002546D8(amount, 0x52);
        func_00254758(0, 0, 1, 0x80, 0x53);
        func_00254778(0, 0, 1, amount, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, amount, (s32)work, 0x53);
        func_00254B30(0, (s32)(ratio * 36.0f), 1, amount, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)((1.0f - shade) * 128.0f), (s32)work, 0x53);
        func_00254810(0, 0, 1, amount, (s32)work, 0x53);
        return 0;
    case 3:
        limit = work->timer;
        elapsed = limit;
        ratio = elapsed / 10.0f;
        if (limit < 4) {
            shade = elapsed * 0.25f;
            shade = 1.0f - shade;
        } else {
            shade = 0.0f;
        }
        ratio = 1.0f - ratio;
        amount = (s32)(ratio * 128.0f);
        mnuDrawDisplaySpriteAndPanelMarks(sel, amount, 0x52);
        func_002546D8(amount, 0x52);
        func_00254758(0, 0, 1, amount, 0x53);
        func_00254778(0, 0, 1, amount, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, amount, (s32)work, 0x53);
        func_00254B30(0, (s32)((1.0f - ratio) * 36.0f), 1, amount, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shade * 128.0f), (s32)work, 0x53);
        func_00254810(0, 0, 1, amount, (s32)work, 0x53);
        return 0;
    case 4:
        mnuDrawDisplaySpriteAndPanelMarks(sel, 0x80, 0x52);
        func_002546D8(0x80, 0x52);
        func_00254758(0, 0, 1, 0x80, 0x53);
        func_00254778(0, 0, 1, 0x80, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, 0x80, (s32)work, 0x53);
        func_00254B30(0, 0, 1, 0x80, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, 0x80, (s32)work, 0x53);
        func_00254810(0, 0, 1, 0x80, (s32)work, 0x53);
        break;
    case 0:
    case 6:
    case 7:
        break;
    }
    return 0;
}

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

void mnuInitializeMantraSelectionGrid(s32 context) {
    MnuSceneContext *scene = (MnuSceneContext *)context;
    s32 coordinates[2];
    s32 record;
    s32 field;

    scene->grid = func_002CB9C0(0xF, 0x11, 0x40, 0x43, 4, 4,
                                (u8 *)scene + 4, 0);
    sdfSetShortPairValues(scene->grid, 1, 1);
    scene->grid->freeTaskData = mnuFreeTaskData;
    scene->grid->callback = func_0025A680;
    record = func_002CB3B8(mnuSceneResourceContext, 0);
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
