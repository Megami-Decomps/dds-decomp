#include "common.h"

#define MNU_SCENE_WORK_SIZE 0x5B0

extern void func_002512F0(s32, s32);

extern void mnuReleaseListNodes(s32);

extern void func_00250978(s32);

extern void mnuCopySceneCoordinates(s32);

extern void sdfDestroyGridWork(s32);

extern void mnuReleaseDisplayListNodes(s32);

extern void func_002D0918(s32);

extern void mnuResetWorkFloats(void);

extern s32 func_002CB3B8(u32, u32);

extern u32 D_003BC4CC;

typedef struct {
    u32 unk0;
    u16 entryX;
    u16 entryY;
    u32 unk8;
} SceneEntry;

extern SceneEntry D_0036BE38[];

/* Fields initialized and released around the scene's 0x5B0-byte work block. */
typedef struct MenuSceneWork {
    s32 allocationHandle; /* 0x000 */
    u8 pad004[0x480];
    s32 gridHandle;       /* 0x484 */
    u8 pad488[0xB8];
    s32 coordinateA;      /* 0x540 */
    s32 coordinateB;      /* 0x544 */
} MenuSceneWork;

typedef struct MenuSceneMetadata {
    u8 pad00[0x23C];
    s32 displayedCurrency;
} MenuSceneMetadata;

typedef struct DatGameCounters {
    u8 pad00[0x3C];
    s32 currency;
} DatGameCounters;

extern void sdfReleaseChipBlock(void *);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250E88);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250F60);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251260);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002512F0);

void mnuFreeTaskData(s32 unused, void *data) {
    if (data != NULL) {
        sdfReleaseChipBlock(data);
    }
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_002515F0);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002517C0);

extern s32 func_002D03F8(s32);
extern s32 sdfMemoryGetBlockAddress(s32);
extern void *memset(void *, s32, u32);
extern u8 *D_003BAA00;

/* Allocate and clear scene work before registering its grid and coordinates. */
s32 mnuCreateSceneWork(void) {
    s32 handle = func_002D03F8(MNU_SCENE_WORK_SIZE);
    u8 *work = (u8 *)sdfMemoryGetBlockAddress(handle);

    memset(work, 0, MNU_SCENE_WORK_SIZE);
    ((MenuSceneWork *)work)->allocationHandle = handle;
    func_00250978((s32)work);
    ((MenuSceneWork *)work)->coordinateA = 0;
    ((MenuSceneWork *)work)->coordinateB = 0;
    ((MenuSceneMetadata *)func_002CB3B8(D_003BC4CC, -1))->displayedCurrency = ((DatGameCounters *)D_003BAA00)->currency;
    mnuCopySceneCoordinates((s32)work);
    return (s32)work;
}

void mnuReleaseSceneContext(s32 unused, s32 context) {
    func_002CB3B8(D_003BC4CC, -1);
    sdfDestroyGridWork(((MenuSceneWork *)context)->gridHandle);
    mnuReleaseDisplayListNodes(context + 0x584);
    func_002D0918(((MenuSceneWork *)context)->allocationHandle);
    mnuResetWorkFloats();
}

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF850);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251A38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252CE8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252F88);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253018);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002530D8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253208);

void mnuReinitializeSceneGrid(s32 context) {
    sdfDestroyGridWork(((MenuSceneWork *)context)->gridHandle);
    func_00250978(context);
    mnuCopySceneCoordinates(context);
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253558);

s32 fldGetSceneMetadataNode(void) {
    s32 context = func_002CB3B8(D_003BC4CC, 1);

    if (context == 0) {
        return 0;
    }
    return *(s32 *)(*(s32 *)(*(s32 *)(context + 0x484) + 8) + 4);
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253640);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253778);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253830);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253AD0);

typedef struct SceneMetadataNode {
    u8 pad00[0xC];
    u16 entryIndex; /* 0x0C */
} SceneMetadataNode;

typedef struct {
    u8 pad00[4];
    SceneMetadataNode *node; /* 0x04 */
} SceneMetadataSlot;

typedef struct {
    u8 pad00[8];
    SceneMetadataSlot *slot; /* 0x08 */
} SceneMetadataGrid;

typedef struct {
    u8 pad00[0x484];
    SceneMetadataGrid *grid; /* 0x484 */
    u8 pad488[0x114];
    u16 entryX; /* 0x59C */
    u16 entryY; /* 0x59E */
} SceneMetadataContext;

/* Copy the active scene entry coordinates selected by the grid metadata. */
void fldUpdateSceneEntryMetadata(s32 context) {
    SceneMetadataNode *node = ((SceneMetadataContext *)context)->grid->slot->node;
    u16 index = node->entryIndex;
    ((SceneMetadataContext *)context)->entryX = D_0036BE38[index].entryX;
    index = node->entryIndex;
    ((SceneMetadataContext *)context)->entryY = D_0036BE38[index].entryY;
}

s32 fldResetSceneState(void) {
    fldUpdateSceneEntryMetadata(func_002CB3B8(D_003BC4CC, 1));
    return 0;
}

void mnuCopySceneCoordinatesAndReleaseNodeList(void) {
    s32 context = func_002CB3B8(D_003BC4CC, 1);
    func_002512F0(context, 1);
    mnuCopySceneCoordinates(context);
    mnuReleaseListNodes(context + 0x590);
}

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC420);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC424);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC428);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC430);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC438);

