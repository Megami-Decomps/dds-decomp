#include "common.h"

#define MNU_MANTRA_GRID_ROW_COUNT 0x11
#define MNU_MANTRA_GRID_COLUMN_COUNT 15
#define MNU_MANTRA_GRID_SLOT_BYTES 8
#define MNU_MANTRA_PROFILE_MATCH_FLAG 1
#define MNU_MANTRA_PROFILE_CAP_FLAG 2
#define MNU_MANTRA_ENTRY_STATE_PAIR_MASK 0xC
#define MNU_MANTRA_SELECTED_DRAW_CODE 0x34
#define MNU_MANTRA_CAP_DRAW_CODE 0x32
#define MNU_MANTRA_STATE_DRAW_CODE 0x33
#define MNU_MANTRA_CAP_OVERLAY_CODE 0x2F
#define MNU_MANTRA_SELECTED_OVERLAY_CODE 0x30
#define MNU_MANTRA_COST_ICON_X_OFFSET 0x19
#define MNU_MANTRA_COST_ICON_Y_OFFSET 0x5C

extern s32 sdfAllocSizeClassBlock(u32);

void effDestroyResourceSlotSet(u32);

typedef struct DspListNode {
    u8 pad00[0x10];
    struct DspListNode *next; /* 0x10 */
} DspListNode;

typedef struct {
    u32 pad00[2];
    DspListNode *first; /* 0x08 */
} DspListHead;

typedef struct {
    u32 allocation;
    u8 pad04[0xC];
    u32 userData;
    u32 callback14;
    void (*onDestroy)(s32, u32);
} SdfTaskHeader;

typedef struct {
    s32 allocation;
    SdfTaskHeader *tasks[10];
    s32 activeCount;
    s32 spawnCountdown;
} MovieResourceGroup;

extern void *sdfCreateTaskHeader(u32);
extern f32 effMiscRandUnitFloat(s32);
extern void mnuReleaseOptionalDrawAllocation(void *, void *);




void mnuDestroyMantraDrawPool(MovieResourceGroup *resources) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (resources->tasks[i] != 0) {
            sdfDestroyTaskWork(resources->tasks[i]);
        }
    }
    sdfReleaseResourceAllocation(resources->allocation);
}

extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern s32 D_0036C698[];

void mnuDrawIconAlpha(s32 x, s32 y, s32 z, s32 alpha, s32 param) {
    func_002BF4E0(x << 4, y << 3, z, (u32)((f32)(alpha << 8) * 0.0078125f), 0, D_0036C698[0], 0x25, param);
}

typedef struct MovieCueNode {
    u8 pad00[8];
    s32 framesLeft;       /* 0x08 */
    s32 duration;         /* 0x0C */
    u8 pad10;
    s8 cueIndex;          /* 0x11 */
    u8 enabled;           /* 0x12 */
} MovieCueNode;

extern void func_0025BA20(s32, s32, u8 *, s8);
extern u8 *sdfListRemoveNode(s32, u8 *);

s32 mnuTickResourceGroup(s32 owner, s32 group) {
    u8 *list = *(u8 **)(group + 8);
    MovieCueNode *node;

    if (list == NULL) {
        sdfDestroyTaskWork(group);
        return 0;
    }
    do {
        node = *(MovieCueNode **)(list + 0x10);
        node->framesLeft = node->framesLeft - 1;
        /* Fire an enabled cue five frames before its node expires. */
        if (node->framesLeft == node->duration - 5 && node->enabled != 0) {
            func_0025BA20(owner, group, (u8 *)node, node->cueIndex);
        }
        if (node->framesLeft == 0) {
            list = sdfListRemoveNode(group, list);
        } else {
            list = *(u8 **)(list + 8);
        }
    } while (list != NULL);
    return group;
}
void func_0025BDD0(MovieResourceGroup *resources) {
    SdfTaskHeader **slot;
    s32 i;
    SdfTaskHeader *group;

    if (resources->spawnCountdown == 0) {
        if (resources->activeCount < 10) {
            group = sdfCreateTaskHeader(0);
            group->callback14 = (u32)mnuReleaseOptionalDrawAllocation;
            func_0025BA20((s32)resources, (s32)group, NULL, 0);
            for (i = 0; i < 10; i++) {
                if (resources->tasks[i] == 0) {
                    resources->tasks[i] = group;
                    resources->activeCount++;
                    break;
                }
            }
        }
        resources->spawnCountdown = (s32)(effMiscRandUnitFloat(0) * 20.0f + 1.0f);
    } else {
        resources->spawnCountdown--;
    }
    slot = (SdfTaskHeader **)resources->tasks;
    for (i = 0; i < 10; i++, slot++) {
        group = *slot;
        if (group != NULL) {
            *slot = (SdfTaskHeader *)mnuTickResourceGroup((s32)resources, (s32)group);
            if (*slot == NULL) {
                resources->activeCount--;
            }
        }
    }
}


INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BF18);

typedef struct {
    u8 pad00[0x6C];
    u32 resourceHandle; /* 0x6C */
} MenuResourceWork;

u32 mnuRequestEffectResource(u32 ctx, u32 config) {
    MenuResourceWork *work = (MenuResourceWork *)sdfAllocSizeClassBlock(0x70);
    memset(work, 0, 0x70);
    effRequestResourceByMode(ctx, config, 0, (u32)&work->resourceHandle);
    return (u32)work;
}

typedef struct MenuListNode {
    u8 pad00[0x10];
    struct MenuListNode *next; /* 0x10 */
} MenuListNode;

typedef struct {
    u32 pad00[2];
    MenuListNode *first; /* 0x08 */
} MenuListHead;

u8 mnuHasEffectResourceHandle(MenuResourceWork *work) {
    return work->resourceHandle != 0;
}

void mnuReleaseEffectResource(MenuResourceWork *work) {
    effDestroyResourceSlotSet(work->resourceHandle);
    sdfReleaseChipBlock(work);
}
INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C0D8);


typedef s16 MnuSpritePlacement[4];

extern MnuSpritePlacement D_0036C268[];

void func_0025C1C8(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                  s32 flags, s32 context) {
    func_002BF4E0((x + D_0036C268[placementIndex][2]) << 4,
                  (y + D_0036C268[placementIndex][3]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036C268[placementIndex][0]],
                  D_0036C268[placementIndex][1],
                  context);
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C278);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C350);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C418);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C4C8);

void mnuAdvanceLoopingFrame(s32 *frame) {
    s32 oldFrame;

    oldFrame = *frame;
    *frame = oldFrame + 1;
    if (0x3c < oldFrame + 1) {
        *frame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C588);

extern u32 mnuGetMantraDisplayFlags(void *, s32);

void mnuAdvanceGridSlotAnimation(s32 animationContext, s32 unusedGrid, u8 *slot) {
    s32 *counter = *(s32 **)(slot + 4);
    s32 value = *counter + 1;

    *counter = value;
    if ((f32)value > 60.0f) {
        *counter = 0;
    }
    mnuGetMantraDisplayFlags(counter, animationContext);
}

typedef struct MenuAnimationSlot {
    s32 unk00;
    s32 counterAddress; /* 0x04: address of the frame counter */
} MenuAnimationSlot;

/* Advance only occupied animation slots across the fixed mantra grid. */
void mnuAdvanceActiveGridSlotAnimations(s32 animationContext, s32 gridOwner) {
    u8 *grid = *(u8 **)(gridOwner + 0x484);
    s32 row;
    s32 column;

    for (row = 0; row < MNU_MANTRA_GRID_ROW_COUNT; row++) {
        MenuAnimationSlot *slot = (MenuAnimationSlot *)(*(s32 *)(grid + 4) + row * *(s32 *)(grid + 0x14) * MNU_MANTRA_GRID_SLOT_BYTES);
        for (column = 0; column < MNU_MANTRA_GRID_COLUMN_COUNT; column++) {
            if (slot[column].counterAddress != 0) {
                mnuAdvanceGridSlotAnimation(animationContext, (s32)grid, (u8 *)&slot[column]);
            }
        }
    }
}

extern void func_0025C278(s32, s32, s32, s32, s32, s32, s32, s32);

/* Choose draw codes from profile-match, cap and entry-state flags; cap overlay is half-strength. */
void func_0025C8D0(s32 x, s32 y, s32 depth, s32 amount, s32 profileAddress, u8 *entry, s32 drawArg) {
    u32 flags = mnuGetMantraDisplayFlags(entry, profileAddress);

    if (flags & MNU_MANTRA_PROFILE_MATCH_FLAG) {
        func_0025C278(x, y, depth, amount, *(u16 *)(entry + 0xC), MNU_MANTRA_SELECTED_DRAW_CODE, 0, drawArg);
    } else if (flags & MNU_MANTRA_PROFILE_CAP_FLAG) {
        func_0025C278(x, y, depth, amount, *(u16 *)(entry + 0xC), MNU_MANTRA_CAP_DRAW_CODE, 0, drawArg);
    } else if (flags & MNU_MANTRA_ENTRY_STATE_PAIR_MASK) {
        func_0025C278(x, y, depth, amount, *(u16 *)(entry + 0xC), MNU_MANTRA_STATE_DRAW_CODE, 0, drawArg);
    }
    if (flags & MNU_MANTRA_PROFILE_CAP_FLAG) {
        func_0025C278(x, y, depth, (s32)((f32)amount * 0.5f),
                      *(u16 *)(entry + 0xC), MNU_MANTRA_CAP_OVERLAY_CODE, 0, drawArg);
    }
    if (flags & MNU_MANTRA_PROFILE_MATCH_FLAG) {
        func_0025C278(x, y, depth, amount, *(u16 *)(entry + 0xC), MNU_MANTRA_SELECTED_OVERLAY_CODE, 0, drawArg);
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CA50);

typedef struct MnuProfileIdList {
    s8 ids[22];
} MnuProfileIdList;

typedef struct MnuProfileOwner {
    u32 unit;
} MnuProfileOwner;

extern const MnuProfileIdList D_003AFA00;
extern u32 prfGetCapValue(u16);
extern u32 ptyGetProfileRecordValue(u32, u16);
extern void func_0025C1C8(s32, s32, s32, s32, s32, s32, s32);

/* Draw cap markers, with a second marker for the first four profiles. */
void mnuDrawCappedProfileMarkers(s32 x, s32 y, s32 depth, s32 alpha,
                   MnuProfileOwner *owner, s32 context) {
    MnuProfileIdList profiles = D_003AFA00;
    s32 i;
    u32 cap;

    for (i = 0; i < 22; i++) {
        cap = prfGetCapValue(profiles.ids[i]);
        if (cap == ptyGetProfileRecordValue(owner->unit, profiles.ids[i])) {
            func_0025C1C8(x, y, depth, alpha, i + 0x10, 0, context);
            if (i < 4) {
                func_0025C1C8(x, y, depth, alpha, i + 0x2B, 0x20, context);
            }
        }
    }
}

extern void func_0025CA50(s32, s32, s32, s32, s32,
                          MnuProfileOwner *, u8 *, s32);
extern void func_0025C588(s32, s32, s32, s32, s32, s32);

/* Draw the grid background and profile markers, then animation and status layers in separate passes. */
void func_0025D100(s32 x, s32 y, s32 z, s32 alpha,
                   MnuProfileOwner *profileOwner, s32 gridOwner,
                   s32 context) {
    u8 *grid = *(u8 **)(gridOwner + 0x484);
    u8 *entry;
    u8 *counter;
    s32 row;
    s32 column;

    func_0025C1C8(x, y, z, alpha, 0xF, 0x20, context);
    mnuDrawCappedProfileMarkers(x, y, z,
                                (s32)((f32)alpha * 0.5f),
                                profileOwner, context);
    for (row = 0; row < MNU_MANTRA_GRID_ROW_COUNT; row++) {
        entry = (u8 *)(*(s32 *)(grid + 4) +
                       row * *(s32 *)(grid + 0x14) * MNU_MANTRA_GRID_SLOT_BYTES);
        for (column = 0; column < MNU_MANTRA_GRID_COLUMN_COUNT; column++, entry += MNU_MANTRA_GRID_SLOT_BYTES) {
            counter = (u8 *)*(s32 *)(entry + 4);
            if (counter != NULL) {
                func_0025CA50(x, y, z,
                              (s32)((f32)alpha * 0.5f),
                              gridOwner, profileOwner, counter, context);
            }
        }
    }
    for (row = 0; row < MNU_MANTRA_GRID_ROW_COUNT; row++) {
        entry = (u8 *)(*(s32 *)(grid + 4) +
                       row * *(s32 *)(grid + 0x14) * MNU_MANTRA_GRID_SLOT_BYTES);
        for (column = 0; column < MNU_MANTRA_GRID_COLUMN_COUNT; column++, entry += MNU_MANTRA_GRID_SLOT_BYTES) {
            counter = (u8 *)*(s32 *)(entry + 4);
            if (counter != NULL) {
                func_0025C8D0(x, y, z, alpha,
                              (s32)profileOwner, counter, context);
            }
        }
    }
    func_0025C588(x, y, z, alpha, gridOwner, context);
}

extern void mnuAdvanceActiveGridSlotAnimations(s32, s32);

/* Update the owner's grid slots and its independent looping-frame counter. */
void mnuAdvanceDisplayGridAndLoopingFrame(s32 gridOwner, s32 animationContext) {
    mnuAdvanceActiveGridSlotAnimations(animationContext, gridOwner);
    mnuAdvanceLoopingFrame((s32 *)(gridOwner + 0x490));
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D2F8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D628);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BC4E8[];
extern void func_0024E260(s32, s32, s32, s32, s32, s32);

/* Select the cost marker from the record's icon index and draw at the fixed badge offset. */
void mnuDrawMantraCostIcon(s32 x, s32 y, s32 depth, s32 recordAddress, s32 amount, s32 drawArg) {
    Bytes7 table = D_003BC4E8[0];

    func_0024E260(x - MNU_MANTRA_COST_ICON_X_OFFSET, y + MNU_MANTRA_COST_ICON_Y_OFFSET, depth, amount, table.b[*(u16 *)(recordAddress + 4)], drawArg);
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D7F8);

MenuListNode *mnuAllocateMenuListNode(void) {
    MenuListNode *node = (MenuListNode *)sdfAllocSizeClassBlock(0x14);

    memset(node, 0, 0x14);
    return node;
}

DspListNode *mnuAppendNodeToDisplayList(DspListHead *head) {
    DspListNode *node = head->first;
    if (node == NULL) {
        node = mnuAllocateMenuListNode();
        head->first = node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = mnuAllocateMenuListNode();
        node = node->next;
    }
    return node;
}

MenuListNode *mnuFreeMenuListNodeAndGetNext(MenuListNode *node) {
    MenuListNode *next;

    next = node->next;
    sdfReleaseChipBlock();
    return next;
}

void mnuReleaseListNodes(MenuListHead *head) {
    MenuListNode *node = head->first;

    while (node != NULL) {
        node = mnuFreeMenuListNodeAndGetNext(node);
    }
}

extern f32 sdfSinPoly(f32);

s32 mnuDrawMantraSineFade(s32 frame, s32 amount, s32 drawArg) {
    f32 wave = (f32)frame / 60.0f;
    s32 shown;
    s32 halfShown;

    wave = sdfSinPoly(wave * 3.14159265f);
    shown = (f32)amount * (wave * 0.5f + 0.5f);
    func_0024E260(-27, 52, 0, shown, 0x16, drawArg);
    func_0024E260(-27, 52, 0, shown, 0x17, drawArg);
    halfShown = (f32)amount * 0.5f;
    func_0024E260(-27, 52, 0, halfShown, 0x18, drawArg);
    func_0024E260(-27, 52, 0, halfShown, 0x19, drawArg);
    if (frame < 60) {
        return 0;
    }
    return 1;
}

extern s32 func_0025D7F8(MenuListNode *, s32, s32);

s32 mnuAdvanceDisplayList(s32 arg0, s32 arg1, s32 arg2) {
    s32 *counter = (s32 *)arg0;
    MenuListHead *head = (MenuListHead *)arg0;
    MenuListNode *node = head->first;
    s32 index = 0;

    if (mnuDrawMantraSineFade(*counter, arg1, arg2) != 0) {
        *counter = 0;
    } else {
        *counter = *counter + 1;
    }
    if (node == NULL) {
        return 1;
    }
    do {
        s32 hit = func_0025D7F8(node, index, arg2);

        index++;
        if (hit != 0) {
            node = mnuFreeMenuListNodeAndGetNext(node);
            head->first = node;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 0;
}

extern s32 mnuAdvanceDisplayList(s32, s32, s32);
extern void mnuDrawMantraCostIcon(s32, s32, s32, s32, s32, s32);

/* Draw the selected record's cost marker once the animated list reports completion. */
void mnuDrawMantraCostAfterListAdvance(s32 recordAddress, s32 listAddress, s32 amount, s32 drawArg) {
    if (mnuAdvanceDisplayList(listAddress, amount, drawArg) != 0) {
        mnuDrawMantraCostIcon(0, 0, 0, recordAddress, amount, drawArg);
    }
}

extern u16 D_0036C6D0[];
extern s32 mdlFlagTest(s32);

void mnuCollectFlagArray(u8 *flags) {
    u32 i;

    for (i = 0; i < 0x29; i++, flags++) {
        if (mdlFlagTest(D_0036C6D0[i]) != 0) {
            *flags = 1;
        }
    }
}

extern u16 D_0036C6D0[];
extern void mdlFlagSet(s32);

void mnuApplyFlagArray(u8 *flags) {
    u32 i;

    for (i = 0; i < 0x29; i++) {
        if (*flags++ != 0) {
            mdlFlagSet(D_0036C6D0[i]);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA00);

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA18);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D8);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E8);

