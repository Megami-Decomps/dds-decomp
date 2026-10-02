#include "common.h"
#include "mnu.h"
#include "sdf.h"
#include "kwln.h"

extern void func_00101968(KwlnTask *, KwlnTask *);
extern s64 mnuPreparePopupAndDispatchSelection(s32);
extern s64 mnuAdvanceCampPopup(s32);
extern s64 mnuFinishCampPopup(s32);

extern s32 sdfAllocGeneralBlock(s32);
extern u8 *sdfResourceRetainAddress(s32);
extern void mnuClearPanelTransitionState(u8 *);
extern void mnuInitializeShopStatusBatches(u8 *);
extern void func_002945B8(u8 *);
extern void mnuResetGradientFadeColor(u8 *, s32);
extern void sndEnsureMidiBankResident(s32);
extern void sndStartTrackExtended(s32);
extern s32 func_00261198();
extern s32 func_002613C8();
extern s32 ptyCountBulletItem(s32);

extern s8 mnuCampListedItems[34];

extern void func_00246950();

typedef struct CampFlagRow {
    s32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

extern CampFlagRow D_003C9A40[];

extern s32 mdlFlagTest(u32);

extern void mnuSetPopupEntry(s32 *, void *);

extern u8 D_003CE658[];

extern s32 kwlnHeldTextureReference;

/* Camp reads the currency word, byte-sized inventory counts and a tier input. */
typedef struct CampSaveState {
    u8 pad00[0x3C];
    s32 money;
    u8 pad40[0x1300];
    u8 counts[0x100]; /* 0x1340 */
    u8 pad1440[0x1D210];
    u32 unk1E650; /* Compared against the camp tier thresholds. */
} CampSaveState;

extern s32 sdfAllocatePacketList();

extern void sdfCreateDescriptorPacket();

extern s32 evtQueueValidatedBgmSoundCode(s32, s32);

extern s32 strcmp(const char *a, const char *b);

extern s32 func_0019CE78(s32 *, s32, s32, s32, s32);

extern void frFontSetContextPair(s32, s32, s32);

extern s32 D_003C99B8[];

extern void evtViewerCleanupMessageWindow();

extern void evtViewerDispatchFlagMode();

extern s64 evtFindTaskById(void);

extern s32 func_00101820(u32);

extern s32 datGameState;

extern s32 func_00261B98(s32);

extern void func_0025FD78(s32);

extern char D_00437838[]; /* "camp" */

extern char D_00424BC0[]; /* "camp_draw" */

extern char D_00424BD0[]; /* "camp_update" */

extern s8 mnuPanelTaskCompletionState;

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 kwlnTaskGetUserValue();

extern void mnuShopReleaseWindowSprites();

extern void sdfReleaseChipBlock();

extern void mnuDestroyWindowContainer();

extern void mnuShopReleaseWindowAndEffectResources();

extern void mnuReleaseWindowTextures();

extern void effDestroyResourceSlotSet();

extern s32 mnuShopReleaseSceneObjects(u8 *);

extern void mnuDrainPanelTransitions(s32, s32);

extern void dspCloseChannel(void);

extern void evtReleaseResourcePairHandle();

extern void sdfReleaseResourceAllocation(s32);

extern void mnuDrawAndStepGradientFade(s32, s32);

extern void func_002C1B68(s32, s32);

extern s32 evtGetMessageWindowControlState(void);

extern u8 D_003CD8D0[];

extern s32 effMiscRand(s32);

extern u8 D_003CDA8C[];

extern u8 mnuCampCompactEntries[];

extern s32 evtGetMirroredSolarPhase(void);

extern u8 D_003CD8DD[];

extern void evtFormatTaskName(s32 taskId, void *name);

extern void *sdfAllocSizeClassBlock(s32 size);

extern void *memset(void *dst, s32 c, u32 n);

extern KwlnTask *kwlnTaskCreate();

extern void evtTickPackLoad(void);

extern s32 func_002C54B0(s32);
extern s32 mnuIsBulletItemId(s32);
extern s32 func_002C5498(s32);
extern u8 *datItemSkillRecords;

extern u8 D_003CDA88[];

extern s32 func_0019FC38(s32, s32, u64, u64, u64, u64);

extern void frFontSetChildColors(s32, u32);

extern s32 func_0019D550(s32, s32, u32);

extern void evtReleaseEventPackResources(void);
extern f32 mnuShopSavedLastTransformVector[];
extern f32 mnuShopSavedMiddleTransformVector[];
extern f32 mnuShopSavedFirstTransformVector[];
extern s32 mnuShopRestoreMiddleVector;
extern u8 *effCreateStatusBatch(s32 kind);
extern s32 effDestroyPackedBatch(s32);

typedef struct CampTaskData {
    s32 taskId;
    s32 unk4;
    u8 pad08[0x40];
} CampTaskData;

extern u8 D_003CBB70[];

/* Schedule the camp task only if no task currently owns this event ID. */
void mnuCampCreateTask(s32 taskId) {
    char name[0x20];
    CampTaskData *data;

    if (evtFindTaskById() == 0) {
        evtFormatTaskName(taskId, name);
        data = sdfAllocSizeClassBlock(0x48);
        memset(data, 0, 0x48);
        data->taskId = taskId;
        data->unk4 = 0;
        kwlnTaskCreate(name, CAMP_TASK_PRIORITY, 1, 1, evtTickPackLoad, evtReleaseEventPackResources, data);
    }
}

void mnuCampDestroyTaskById(void) {
    s64 task;

    task = evtFindTaskById();
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
        return;
    }
}

/* Drain every camp task at the scheduler priority used during creation. */
void mnuCampDestroyAllTasks(void) {
    s64 task;

    while (task = func_00101820(CAMP_TASK_PRIORITY), task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
}

typedef struct ScrollNode {
    u16 pos;                  /* 0x0 */
    u8 pad02[0x2E];           /* 0x2 */
    struct ScrollNode *next;  /* 0x30 */
} ScrollNode;

typedef struct ScrollList {
    u8 pad00[0x1C];
    s16 base;                 /* 0x1C */
    u8 pad1E[0x36];
    ScrollNode *nodes;        /* 0x54 */
} ScrollList;

typedef struct ScrollOwner {
    u8 pad00[0xC];
    s32 limit;                /* 0xC */
    u8 pad10[0x22F8];
    ScrollList *list;         /* 0x2308 */
} ScrollOwner;

void mnuShopScrollList(ScrollOwner *owner, s32 delta) {
    ScrollList *list = owner->list;
    ScrollNode *node;
    s32 next;

    if (list == NULL) {
        return;
    }
    for (node = list->nodes; node != NULL; node = node->next) {
        next = node->pos + list->base + delta;
        if (next < list->base) {
            node->pos = 0;
        } else if (owner->limit < next) {
            node->pos = (u16)owner->limit - (u16)list->base - 1;
        } else {
            node->pos = node->pos + delta;
        }
    }
}

typedef struct FxChild {
    u16 offset;           /* 0x00 */
    u8 pad02[6];
    s16 fadeA;            /* 0x08 */
    s16 fadeB;            /* 0x0A */
    s16 cond;             /* 0x0C */
    u8 pad0E[0x22];
    struct FxChild *next; /* 0x30 */
    struct FxChild *link; /* 0x34 */
} FxChild;

typedef struct FxNode {
    s32 kind;             /* 0x00 */
    u8 pad04[0x18];
    s16 base;             /* 0x1C */
    u8 pad1E[0x36];
    FxChild *children;    /* 0x54 */
    FxChild *fallback;    /* 0x58 */
    u8 pad5C[0x20];
    struct FxNode *next;  /* 0x7C */
} FxNode;

typedef struct FxWorld {
    u8 pad00[0xC];
    union {
        s32 whole;
        u16 low;
    } limitv;             /* 0x0C */
    u8 pad10[4];
    s32 scrollOffset;     /* 0x14: shifted by delta, wraps to 10 below zero */
    s32 clampedOffset;    /* 0x18: cannot exceed the current limit */
    s32 unk1C;            /* 0x1C: decremented on each scroll; other uses unknown */
    u8 pad20[0x2010];
    s32 count;            /* 0x2030 */
    FxNode *nodes;        /* 0x2034 */
} FxWorld;

void mnuFxWorldScrollDelta(FxWorld *world, s32 delta, s32 threshold, s32 base, s32 offset, s32 ubase) {
    FxNode *node;
    FxChild *child;
    s32 total;

    if (world->count <= 0) {
        return;
    }
    if (world->scrollOffset + delta < 0) {
        world->scrollOffset = 10;
    } else {
        world->scrollOffset += delta;
    }
    if (world->limitv.whole + delta < 0) {
        world->limitv.whole = 10;
    } else {
        world->limitv.whole += delta;
    }
    if (world->clampedOffset > world->limitv.whole) {
        world->clampedOffset = world->limitv.whole;
    }
    node = world->nodes;
    while (node != NULL) {
        for (child = node->children; child != NULL; child = child->next) {
            offset = child->offset;
            base = node->base;
            ubase = (u16)node->base;
            total = offset + base;
            if (total < threshold) {
                continue;
            }
            total += delta;
            if (total < base) {
                child->offset = 0;
            } else if (world->limitv.whole < total) {
                child->offset = world->limitv.low - ubase - 1;
            } else {
                child->offset = offset + delta;
            }
            switch (node->kind) {
            case 0x12:
                if (child->fadeA != 0) {
                    child->fadeA += delta;
                    if (child->fadeA < 0) {
                        child->fadeA = 0;
                    }
                    if (world->limitv.whole < child->fadeA + delta) {
                        child->fadeA = world->limitv.whole - 1;
                    }
                }
                break;
            case 3:
            case 0x14:
            case 0x15:
            case 0x1A:
                if (child->fadeB != 0) {
                    child->fadeB += delta;
                    if (child->fadeB < 0) {
                        child->fadeB = 0;
                    }
                    if (world->limitv.whole < child->fadeB + delta) {
                        child->fadeB = world->limitv.whole - 1;
                    }
                }
                break;
            }
        }
        node = node->next;
    }
    world->unk1C -= 1;
    evtViewerCleanupMessageWindow(world, delta, threshold, base, offset, ubase, node);
    evtViewerDispatchFlagMode(world);
}

void mnuFxWorldDropOutOfRange(FxWorld *world, s32 threshold) {
    FxNode *node;
    FxChild *child;

    if (world->count <= 0) {
        return;
    }
    for (node = world->nodes; node != NULL; node = node->next) {
        child = node->children;
        while (child != NULL) {
            if (child->offset + node->base < threshold) {
                child = child->next;
            } else {
                func_00246950(world, node, child);
                child = node->children;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DE08);

/* Default three-vector slot contents; the trailing two scalars have unknown roles. */
void mnuInitializeCampPanelVisualDefaults(f32 *firstVector, f32 *secondVector, f32 *thirdVector, f32 *scalarA, f32 *scalarB) {
    firstVector[0] = 0.7f;
    firstVector[1] = 0.7f;
    firstVector[2] = 0.7f;
    firstVector[3] = 0.0f;
    secondVector[0] = 0.65f;
    secondVector[1] = 0.39f;
    secondVector[2] = 0.65f;
    secondVector[3] = 0.0f;
    thirdVector[0] = 0.2f;
    thirdVector[1] = 0.2f;
    thirdVector[2] = 0.2f;
    thirdVector[3] = 1.0f;
    *scalarA = 7.0f;
    *scalarB = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E048);

typedef struct CampDisplayDefaults {
    s32 width;
    s32 height;
    s8 color[4];
    f32 scaleX;
    f32 scaleY;
    s32 enabled;
    s32 variant;
    s32 unk1C;
} CampDisplayDefaults;

void mnuCampInitializeDisplayDefaults(CampDisplayDefaults *display) {
    display->width = 0x100;
    display->height = 0xE0;
    display->color[0] = -0x80;
    display->color[1] = -0x80;
    display->color[2] = -0x80;
    display->color[3] = -0x80;
    display->scaleY = 1.0f;
    display->scaleX = 1.0f;
    display->enabled = 1;
    display->variant = 0;
    display->unk1C = 0;
}

typedef struct CampKeyNode {
    u16 frame;
    u8 pad02[0xA];
    s16 condition;             /* 0xC */
    u8 pad0E[0x22];
    struct CampKeyNode *next;  /* 0x30 */
    struct CampKeyNode *alt;   /* 0x34 */
} CampKeyNode;

typedef struct CampKeyTrack {
    s32 type;
    u8 pad04[0x18];
    s16 base;                  /* 0x1C */
    u8 pad1E[0x36];
    CampKeyNode *first;        /* 0x54 */
    CampKeyNode *fallback;     /* 0x58 */
} CampKeyTrack;

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E288);

typedef struct CampListLayout {
    s32 width0;
    s32 width1;
    s32 width2;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[8];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
} CampListLayout;

void mnuInitializeCampListLayoutDefaults(CampListLayout *layout) {
    layout->width0 = 150;
    layout->width1 = 150;
    layout->unk10 = 80;
    layout->width2 = 150;
    layout->unkC = 30;
    layout->unk14 = 1;
    layout->unk20 = 7;
    layout->unk24 = 4;
    layout->unk28 = 10;
    layout->unk2C = 32;
    layout->unk30 = 16;
    layout->unk34 = 16;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E390);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E460);

void mnuFindCampKeyTrackNeighbors(CampKeyTrack *track, s32 value, CampKeyNode **out1, CampKeyNode **out2) {
    s32 base;

    *out1 = 0;
    *out2 = 0;
    if (track == 0) {
        return;
    }
    base = track->base;
    *out2 = track->first;
    while (*out2 != 0) {
        if (value < (*out2)->frame + base) {
            break;
        }
        *out2 = (*out2)->next;
    }
    if (*out2 != 0) {
        *out1 = (*out2)->alt;
    } else {
        *out1 = track->fallback;
    }
    if (track->type == 2) {
        while (*out1 != 0 && evtViewerTestIndexedCondition((*out1)->condition) != 1) {
            *out1 = (*out1)->alt;
        }
    }
}

u32 func_0025E7B0(void) {
    return 0;
}

typedef struct PackedPair {
    u8 pad00[8];
    union {
        u32 word;
        u16 half;
    } packed;
} PackedPair;

void mnuUnpackNibbleFields(PackedPair *pair, s32 *low, s32 *high) {
    *low = pair->packed.half & 0xFFF;
    *high = pair->packed.half >> 12;
}

typedef struct CampEntryNode {
    u8 pad00[8];
    s32 nameIndex; /* 0x08: 32-byte name in the owning scene */
    u8 pad0C[0x18];
    s32 value; /* 0x24 */
    u32 status; /* 0x28 */
    u8 pad2C[0x50];
    struct CampEntryNode *next; /* 0x7C */
} CampEntryNode;

typedef struct CampOwner {
    u8 pad00[0x104];
    s32 handle; /* 0x104 */
    u8 pad108[4];
    s32 bgmId; /* 0x10C: validated event BGM ID used with registered variations */
} CampOwner;

typedef struct {
    u8 pad00[8];
    CampOwner *owner; /* 0x08 */
    u8 pad0C[0x2028];
    CampEntryNode *entries; /* 0x2034 */
    u8 pad2038[0x2F8];
    f32 transform[12]; /* 0x2330: three four-component vectors saved by shop */
    u8 pad2360[0x6C];
    s32 sceneMode; /* 0x23CC */
    u8 pad23D0[0x10];
    s32 pendingValue; /* 0x23E0 */
    u8 pad23E4[0x28];
    u32 state; /* 0x240C */
    u32 fontDrawHandle; /* 0x2410: font draw handle created by func_0019CE78 */
    u8 pad2414[0x14];
    s32 descriptorHandle; /* 0x2428: submitted to the drawing packet */
    u8 pad242C[4];
    u32 menuState; /* 0x2430 */
    s32 shopFlag;  /* 0x2434: 1 once the shop descriptor was submitted */
    u32 linkedHandle; /* 0x2438: passed to func_0025F130 */
    u32 optionFlags; /* 0x243C */
    u8 pad2440[4];
    s32 idCount; /* 0x2444 */
    s32 registeredIds[20]; /* 0x2448 */
} CampScene;

typedef struct CampNameLookup {
    u8 pad00[0x7C];
    u8 *nameTable; /* 0x7C: 32-byte names indexed by nameIndex */
} CampNameLookup;

s32 mnuCampFindMatchingEntryIndex(CampNameLookup *entry, CampScene *scene, s32 nameIndex) {
    CampEntryNode *node = scene->entries;
    while (node != NULL) {
        if (strcmp((char *)scene + (node->nameIndex << 5) + 0x24,
                   (char *)entry->nameTable + (nameIndex << 5)) == 0) {
            return node->nameIndex;
        }
        node = node->next;
    }
    return -1;
}

void *mnuCampFindEntryByName(CampScene *scene, const char *name) {
    CampEntryNode *node = scene->entries;
    while (node != NULL) {
        if (strcmp((char *)scene + (node->nameIndex << 5) + 0x24, name) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

typedef struct CampCue {
    u8 pad00[0x10];
    s16 kind;             /* 0x10 */
    u8 pad12[0x22];
    struct CampCue *link; /* 0x34 */
} CampCue;

void campResolvePendingValue(CampScene *scene, CampCue *cue) {
    CampCue *next;
    s32 kind;
    u16 id;

    if (cue == NULL) {
        return;
    }
    kind = cue->kind;
    id = cue->kind;
    if (kind == 1) {
        scene->pendingValue = 0;
        return;
    }
    if (kind == 0) {
        next = cue->link;
        scene->pendingValue = 0;
        for (; ; next = next->link) {
            s32 nextKind;

            if (next == NULL) {
                return;
            }
            nextKind = next->kind;
            if (nextKind != 0) {
                if (nextKind == 1) {
                    scene->pendingValue = 0;
                    return;
                }
                scene->pendingValue = ((CampEntryNode *)mnuCampFindEntryByName(scene, (char *)scene + (nextKind << 5) - 0x1C))->value;
                return;
            }
        }
    } else {
        scene->pendingValue = ((CampEntryNode *)mnuCampFindEntryByName(scene, (char *)scene + ((s16)id << 5) - 0x1C))->value;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E980);

void fldResetCampSceneEntries(CampScene *scene) {
    CampEntryNode *node;

    node = scene->entries;
    if (node != 0) {
        node->status = 0;
        while (node = node->next, node != 0) {
            node->status = 0;
        }
    }
    scene->state = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EC00);

extern void fldCopyCameraSetting(void *setting);
extern void fldUpdateCameraColorEffect(void *setting);

void fldApplyCameraColorKeyWords(CampScene *scene, const s32 *colorSettings) {
    s32 cameraSettings[0x54 / sizeof(s32)];
    u8 *cameraColorA;
    u8 *sourceColorA;
    u8 *cameraColorB;
    s32 sourceOffset;
    s32 cameraOffset;
    s32 i;

    fldCopyCameraSetting(cameraSettings);
    cameraSettings[0x20 / sizeof(s32)] = colorSettings[0x10 / sizeof(s32)];
    cameraSettings[0x10 / sizeof(s32)] = colorSettings[0];
    cameraSettings[0x14 / sizeof(s32)] = colorSettings[1];
    cameraSettings[0x18 / sizeof(s32)] = colorSettings[2];
    cameraSettings[0x1C / sizeof(s32)] = colorSettings[3];
    cameraSettings[0x0C / sizeof(s32)] = colorSettings[0x14 / sizeof(s32)];
    cameraColorA = (u8 *)cameraSettings + 8;
    sourceColorA = (u8 *)colorSettings + 0x0C;
    cameraColorB = (u8 *)cameraSettings + 0x0C;
    sourceOffset = 0x20;
    cameraOffset = 0x20;
    /* Copy the three color rows into the camera setting's 0x10-byte slots. */
    for (i = 2; i >= 0; i--) {
        *(s32 *)(cameraColorA + cameraOffset) =
            *(s32 *)(sourceColorA + sourceOffset);
        *(s32 *)(cameraColorB + cameraOffset) =
            *(s32 *)((u8 *)colorSettings + sourceOffset);
        sourceOffset += 4;
        cameraOffset += 0x10;
    }
    fldUpdateCameraColorEffect(cameraSettings);
    if (colorSettings[0x0C / sizeof(s32)] == 0 &&
        colorSettings[0x2C / sizeof(s32)] == 0 &&
        colorSettings[0x30 / sizeof(s32)] == 0 &&
        colorSettings[0x34 / sizeof(s32)] == 0) {
        scene->sceneMode = 0;
    } else {
        scene->sceneMode = 1;
    }
}

void func_0025EE00(CampScene *scene) {
    func_0025EC00();
    if (scene->sceneMode == 1) {
        func_00137888();
        return;
    }
}

void mnuCampInitFontResource(CampScene *scene) {
    s32 fontHandle;
    scene->fontDrawHandle = 0;
    fontHandle = func_0019CE78(D_003C99B8, 0, 0, 0, 0);
    scene->fontDrawHandle = fontHandle;
    frFontSetContextPair(fontHandle, 0x960, 0x70);
}

void mnuCampLinkFontGlyph(CampScene *scene) {
    frFontQueueGlyphInSelectedSlot(scene->fontDrawHandle);
    scene->fontDrawHandle = 0;
}

extern void sdfGetGeneralHeapStats(s32 *);

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void) {
    s32 info[8];
    s32 quotient;

    sdfGetGeneralHeapStats(info);
    quotient = 1 / info[0];
}

void mnuEnterCampSceneMenuState(CampScene *scene) {
    if ((scene->menuState == 0) || (scene->menuState == 5)) {
        scene->menuState = 1;
    }
}

extern void kwlnCreateHeldTextureBuffer(s32, s32, f32);
extern void func_0025EFD8(CampScene *scene);
extern void mnuShopSubmitDescriptor(u8 *work);
extern void func_0025F2B0(CampScene *scene);

/* Camp scene opening steps 1..5, each falling into the next; stages 0 and 2 do nothing, other values advance by one. */
void mnuAdvanceShopMenuState(CampScene *scene) {
    switch (scene->menuState) {
    case 1:
        if (kwlnHeldTextureReference == 0) {
            kwlnCreateHeldTextureBuffer(0x200, 0xE0, 100.75f);
        }
        scene->menuState = scene->menuState + 1;
    case 3:
        func_0025EFD8(scene);
        scene->menuState = scene->menuState + 1;
    case 4:
        mnuShopSubmitDescriptor((u8 *)scene);
        scene->menuState = scene->menuState + 1;
    case 5:
        if (scene->shopFlag == 1) {
            func_0025F2B0(scene);
        }
    case 0:
        break;
    default:
        scene->menuState = scene->menuState + 1;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EFD8);

typedef struct BufferDescriptor {
    u8 pad00[0x10];
    void (*open)(struct BufferDescriptor *, s32);
} BufferDescriptor;

extern BufferDescriptor D_00380708;

void mnuShopSubmitDescriptor(u8 *work) {
    s32 packet;

    if (((CampScene *)work)->descriptorHandle != 0) {
        packet = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(packet, (s32)((SdfTex *)kwlnHeldTextureReference)->primaryResource, 0, 0, 0x200, 0xE0, ((CampScene *)work)->descriptorHandle, 0);
        D_00380708.open(&D_00380708, packet);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F130);

void func_0025F2B0(CampScene *scene) {
    func_0025F130(scene->linkedHandle);
}

void func_0025F2C8(void) {
}

void mnuCampSetPrimaryOption(CampScene *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & 0xfffffffc) | (option & 3);
}

u32 mnuCampGetPrimaryOption(CampScene *scene) {
    return scene->optionFlags & 3;
}

void mnuCampSetSecondaryOption(CampScene *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & 0xfffffff3) | ((option & 3) << 2);
}

u32 mnuCampGetSecondaryOption(CampScene *scene) {
    return (scene->optionFlags & 0xc) >> 2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F330);

void mnuShopSavePrimaryTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = ((CampScene *)scene)->transform;
    for (i = 0; i < 4; i++) {
        mnuShopSavedLastTransformVector[i] = coordinates[i + 8];
        mnuShopSavedFirstTransformVector[i] = coordinates[i];
    }
    mnuShopRestoreMiddleVector = 0;
}

void mnuShopSaveFullTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = ((CampScene *)scene)->transform;
    for (i = 0; i < 4; i++) {
        mnuShopSavedLastTransformVector[i] = coordinates[i + 8];
        mnuShopSavedMiddleTransformVector[i] = coordinates[i + 4];
        mnuShopSavedFirstTransformVector[i] = coordinates[i];
    }
    mnuShopRestoreMiddleVector = 1;
}

void mnuShopRestoreTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = ((CampScene *)scene)->transform;
    s32 useMiddle = mnuShopRestoreMiddleVector;
    for (i = 0; i < 4; i++) {
        coordinates[i + 8] = mnuShopSavedLastTransformVector[i];
        if (useMiddle != 0) {
            coordinates[i + 4] = mnuShopSavedMiddleTransformVector[i];
        }
        coordinates[i] = mnuShopSavedFirstTransformVector[i];
    }
}

void fldRegisterCampSceneId(CampScene *scene, s32 id) {
    s32 count = scene->idCount;
    s32 i = 0;
    if (count > 0) {
        s32 *entry = scene->registeredIds;
        s32 value = *entry;
        do {
            entry++;
            if (value == id) {
                return;
            }
            i++;
            if (i >= count) {
                break;
            }
            value = *entry;
        } while (1);
    }
    if (count < 20) {
        scene->registeredIds[count] = id;
        ++scene->idCount;
    }
}

/* Queue the scene's BGM ID with each registered variation, then clear the list. */
void mnuReleaseCampSceneRegisteredIds(CampScene *scene) {
    s32 count = 0;
    if (scene->idCount > 0) {
        s32 *entry = scene->registeredIds;
        do {
            s32 identifier = *entry++;
            count++;
            evtQueueValidatedBgmSoundCode(scene->owner->bgmId, identifier);
        } while (count < scene->idCount);
    }
    scene->idCount = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F708);

typedef struct ShopEffectGraphics {
    u8 pad00[0x20];
    s32 *params; /* 0x20 */
} ShopEffectGraphics;

typedef struct ShopEffectObject {
    u8 pad00[8];
    ShopEffectGraphics *graphics; /* 0x08 */
} ShopEffectObject;

typedef struct ShopEffectScene {
    s32 resourceHandle; /* 0x00 */
    u8 pad04[0x74];
    s32 state;          /* 0x78 */
    u8 pad7C[8];
    u8 *objects[2];    /* 0x84 */
    u8 pad8C[0x10];
    s32 availableCount; /* 0x9C */
    u16 unkA0;
    u16 unkA2;
    u16 unkA4;
    u8 padA6[6];
    u32 options;        /* 0xAC */
    u8 padB0[0x34];
    s32 mode;           /* 0xE4 */
} ShopEffectScene;

void mnuInitializeShopStatusBatches(u8 *scene) {
    u8 *object;
    u8 *graphics;
    s32 *params;
    s32 defaultValue = 15;
    ((ShopEffectScene *)scene)->state = 0;
    object = effCreateStatusBatch(6);
    graphics = (u8 *)((ShopEffectObject *)object)->graphics;
    ((ShopEffectScene *)scene)->objects[0] = object;
    params = ((ShopEffectGraphics *)graphics)->params;
    params[0] = defaultValue;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    params[4] = 0;
    object = effCreateStatusBatch(1);
    graphics = (u8 *)((ShopEffectObject *)object)->graphics;
    ((ShopEffectScene *)scene)->objects[1] = object;
    params = ((ShopEffectGraphics *)graphics)->params;
    params[0] = defaultValue;
    params[1] = 0;
}

s32 mnuShopReleaseSceneObjects(u8 *scene) {
    s32 *objects = (s32 *)((ShopEffectScene *)scene)->objects;
    s32 result;
    u32 i;
    for (i = 0; i < 2; i++) {
        result = effDestroyPackedBatch(*objects++);
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A00);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A10);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A20);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A30);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424AC0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F8B8);

/* The background-effect packet owns the packed animation created for it. */
typedef struct MenuEffectResources {
    u8 pad00[0x3C];
    u32 animationHandle;
} MenuEffectResources;

void mnuShopDestroyNestedEffectBatch(s32 object) {
    effDestroyPackedBatch(((MenuEffectResources *)object)->animationHandle);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FA28);

/* DDS2 shop scene fields used while choosing the message resource. */
typedef struct ShopMessageScene {
    u8 pad00[8];
    s32 messageSet;     /* 0x08 */
    u8 pad0C[0x54];
    u8 resourcePair[4]; /* 0x60 */
    s32 pairedHandle;   /* 0x64 */
    u8 pad68[0x24];
    s32 shopRow;        /* 0x8C */
} ShopMessageScene;

extern void evtLoadResourcePair();
extern void evtCreateMessageWindowIfMissing();

void func_0025FC08(ShopMessageScene *scene) {
    scene->messageSet = D_003C9A40[scene->shopRow].messageSet;
    switch (scene->messageSet) {
    case 0:
        evtLoadResourcePair("/facility/msg/shop/mes_01.bmd", scene->resourcePair);
        break;
    case 1:
        evtLoadResourcePair("/facility/msg/shop/mes_02.bmd", scene->resourcePair);
        break;
    case 2:
        evtLoadResourcePair("/facility/msg/shop/mes_03.bmd", scene->resourcePair);
        break;
    case 3:
        evtLoadResourcePair("/facility/msg/shop/mes_04.bmd", scene->resourcePair);
        break;
    }
    evtCreateMessageWindowIfMissing(scene->pairedHandle);
}

typedef struct ShopSceneCleanup {
    u8 pad00[8];
    s32 mode;             /* 0x08 */
    u8 pad0C[0x5C];
    s32 handles[4];       /* 0x68 through 0x74 */
    u8 pad78[0x300];
    s32 resourceHandle;   /* 0x378 */
} ShopSceneCleanup;

void mnuShopReleaseWindowAndEffectResources(s32 scene) {
    s32 i;
    s32 *slot = ((ShopSceneCleanup *)scene)->handles;

    mnuReleaseWindowTextures(((ShopSceneCleanup *)scene)->resourceHandle);
    for (i = 1; i >= 0; i--) {
        effDestroyResourceSlotSet(*slot++);
    }
    effDestroyResourceSlotSet(((ShopSceneCleanup *)scene)->handles[2]);
    effDestroyResourceSlotSet(((ShopSceneCleanup *)scene)->handles[3]);
    switch (((ShopSceneCleanup *)scene)->mode) {
    case 1:
    case 3:
        mnuShopDestroyNestedEffectBatch(scene + 0x210);
        break;
    case 0:
    case 2:
        break;
    default:
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FD78);

s32 mnuCampHasEligibleOwnedItems(void) {
    s32 result = 0;
    s32 i;

    for (i = 0; i < 0x100; i++) {
        if (mnuIsBulletItemId(i) != 0) {
            continue;
        }
        if (func_002C54B0(i) != 0) {
            continue;
        }
        if (((CampSaveState *)datGameState)->counts[i] == 0) {
            continue;
        }
        if ((datItemSkillRecords[i * 8] & 3) != 0) {
            result = 1;
            break;
        }
        if (func_002C5498(i) != 0) {
            result = 1;
            break;
        }
    }
    return result;
}

typedef struct CampWindowContainer CampWindowContainer;

/* The first payload word is list-specific (entry index or displayed value). */
typedef struct CampWindowParams {
    s32 value;
    s32 id;
    s32 price; /* Base price used for affordability and quantity-dependent discounts. */
    s32 mode;
} CampWindowParams;

typedef struct CampWindowNode {
    u8 pad00[0x48];
    u32 flags;
    u8 pad4C[0xC];
    struct CampWindowNode *next; /* 0x58 */
    u8 pad5C[4];
    CampWindowParams params; /* 0x60 */
} CampWindowNode;

typedef struct CampWindowBuffer {
    u8 pad00[0xC];
    u16 unk0C;
    u16 unk0E;
    u16 unk10;
    u8 pad12[2];
} CampWindowBuffer;

typedef struct CampWindowListData {
    u8 pad00[0x10];
    CampWindowNode *first;
    u8 pad14[8];
    CampWindowNode *cursor; /* 0x1C */
    s32 count;
    u8 pad24[8];
    void (*callback)(void);
    void *buffer;
} CampWindowListData;

struct CampWindowContainer {
    u8 pad00[0x18];
    CampWindowListData *list;
};

extern void func_00295400(void);

s32 mnuCreateEnabledCampEntryWindow(s32 count, s32 *enabled, u8 *settings) {
    CampWindowContainer *window;
    CampWindowBuffer *storage;
    s32 i;

    window = (CampWindowContainer *)mnuCreateWindowContainer(0, 0x260, 0x10, count, 0x16);
    mnuSetWindowEntryParameters(0, window, 0, 8, 0xA);
    for (i = 0; i < count; i++) {
        if (enabled[i] != 0) {
            ((CampWindowNode *)mnuAppendWindowListNode(window, 0))->params.value = i;
        }
    }
    window->list->callback = func_00295400;
    storage = sdfAllocSizeClassBlock(0x14);
    memset(storage, 0, 0x14);
    window->list->buffer = storage;
    storage->unk0C = ((ShopEffectScene *)settings)->unkA0;
    storage->unk0E = ((ShopEffectScene *)settings)->unkA2;
    storage->unk10 = ((ShopEffectScene *)settings)->unkA4;
    return (s32)window;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260020);

s32 mnuCampResolveFlagRowValue(s32 row) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_003C9A40[row].flag[i] > 0 && mdlFlagTest(D_003C9A40[row].flag[i])) {
            return D_003C9A40[row].value[i + 1];
        }
    }
    return D_003C9A40[row].value[0];
}

/* Scan the twenty-one flag IDs in descending slot order. */
s32 mnuCampFindActiveSlot(void) {
    s32 i;
    u8 *base = D_003CBB70;
    s16 *flagId = (s16 *)(base + 0x1450);
    for (i = 0x14; i >= 0; i--, flagId = (s16 *)((u8 *)flagId - 0x104)) {
        if (*flagId > 0 && mdlFlagTest(*flagId)) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260250);

typedef struct ShopScene {
    u8 pad00[0x7C];
    CampWindowContainer *sprites[1]; /* 0x7C */
    CampWindowContainer *extra;      /* 0x80 */
    u8 pad84[8];
    s32 stockGroup;         /* 0x8C */
    s32 quantity;           /* 0x90: clamped to [1, maximum] */
    u8 pad94[0x33];
    u8 atLimit;             /* 0xC7 */
} ShopScene;

void mnuShopReleaseWindowSprites(s32 keepExtra, ShopScene *scene) {
    CampWindowContainer **slot = scene->sprites;
    CampWindowContainer *sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        sprite = *slot;
        if (sprite != NULL) {
            if (sprite->list->buffer != NULL) {
                sdfReleaseChipBlock(sprite->list->buffer);
                sprite = *slot;
                sprite->list->buffer = NULL;
            }
            mnuDestroyWindowContainer(sprite);
        }
        slot++;
    }
    if (keepExtra == 0) {
        if (scene->extra != NULL) {
            if (scene->extra->list->buffer != NULL) {
                sdfReleaseChipBlock(scene->extra->list->buffer);
                scene->extra->list->buffer = NULL;
            }
            mnuDestroyWindowContainer(scene->extra);
        }
    }
}

u32 func_00260458(void) {
    return 0;
}

u32 func_00260460(void) {
    return 0;
}

s32 mnuCountActivePartyEntries(void) {
    u16 entryFlags;
    u16 *entry;
    s32 remaining;
    s32 enabledCount;

    enabledCount = 0;
    remaining = 4;
    entry = (u16 *)(datGameState + 0xa60);
    do {
        entryFlags = *entry;
        entry = entry + 0xe2;
        remaining = remaining - 1;
        enabledCount = enabledCount + (entryFlags & 1);
    } while (-1 < remaining);
    return enabledCount;
}

typedef struct CampTier {
    u32 threshold;
    s32 value;
} CampTier;

extern CampTier D_003CE408[];

s32 mnuCampResolveProgressTierValue(void) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (i + 1 < 3) {
            if (D_003CE408[i].threshold > ((CampSaveState *)datGameState)->unk1E650) {
                break;
            }
        } else if (D_003CE408[i].threshold <= ((CampSaveState *)datGameState)->unk1E650) {
            break;
        }
    }
    return D_003CE408[i].value;
}

void mnuCampClearListedItemCounts(void) {
    u16 entryId;
    s8 *entry;
    u32 index;

    index = 0;
    entry = mnuCampListedItems;
    do {
        entryId = *(u16 *)entry;
        entry = (s8 *)((s32)entry + 8);
        index = index + 1;
        ((CampSaveState *)datGameState)->counts[(u32)entryId] = 0;
    } while (index < 3);
}

u8 *mnuTerminalCreateContext(void) {
    s32 handle;
    u8 *obj;

    handle = sdfAllocGeneralBlock(0x38C);
    obj = sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x38C);
    ((ShopEffectScene *)obj)->resourceHandle = handle;
    mnuClearPanelTransitionState(obj + 0xC);
    mnuInitializeShopStatusBatches(obj);
    ((ShopEffectScene *)obj)->options = func_00260460();
    ((ShopEffectScene *)obj)->availableCount = mnuCountActivePartyEntries();
    ((ShopEffectScene *)obj)->mode = 0xF;
    func_002945B8(obj);
    mnuResetGradientFadeColor(obj + 0x37C, 0x60);
    mnuCampClearListedItemCounts();
    sndEnsureMidiBankResident(0x300000);
    sndStartTrackExtended(0x300000);
    return obj;
}

void mnuTerminalReleaseContextAndResources(s32 arg) {
    s32 scene = kwlnTaskGetUserValue();

    if (scene != 0) {
        mnuShopReleaseWindowSprites(0, scene);
        mnuShopReleaseWindowAndEffectResources(scene);
        mnuShopReleaseSceneObjects((u8 *)scene);
        mnuDrainPanelTransitions(scene + 0xC, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(scene + 0x60);
        sdfReleaseResourceAllocation(*(s32 *)scene);
        mnuPanelTaskCompletionState = 2;
    }
}

s32 mnuTerminalSyncMessageWindowControl(void) {
    s32 state = kwlnTaskGetUserValue() + 0x37C;
    mnuDrawAndStepGradientFade(state, 0x53);
    if (evtGetMessageWindowControlState() != 0) {
        func_002C1B68(state, 1);
    } else {
        func_002C1B68(state, 0);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BD0);

void func_00260708(const s32 *stockGroup) {
    ShopScene *scene = (ShopScene *)mnuTerminalCreateContext();
    KwlnTask *drawTask;
    KwlnTask *fadeTask;

    if (stockGroup != NULL) {
        scene->stockGroup = *stockGroup;
    }
    kwlnTaskCreate(D_00437838, 0x402, 1, 1,
        mnuPreparePopupAndDispatchSelection, NULL, (u32)scene);
    drawTask = kwlnTaskCreate(D_00424BC0, 0x2B12, 1, 1,
        mnuAdvanceCampPopup, NULL, (u32)scene);
    kwlnTaskCreate(D_00424BD0, 0x520E, 1, 1,
        mnuFinishCampPopup, mnuTerminalReleaseContextAndResources, (u32)scene);
    fadeTask = kwlnTaskCreate("shop_fade", 0x2B13, 1, 0,
        mnuTerminalSyncMessageWindowControl, NULL, (u32)scene);
    func_00101968(drawTask, fadeTask);
    mnuPanelTaskCompletionState = 1;
}

void mnuCampDestroyPanelTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437838, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BC0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BD0, 0);
}

s32 mnuCampConsumePanelTaskCompletion(void) {
    s32 state = mnuPanelTaskCompletionState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        mnuPanelTaskCompletionState = 0;
    }
    return 0;
}

static inline s64 campSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), mode, callback);
}

s64 mnuPreparePopupAndDispatchSelection(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    mnuSetPopupEntry((s32 *)(context + 0x58), D_003CE658);
    return campSetHandler(context, 0, callback);
}

s64 mnuAdvanceCampPopup(s32 callback) {
    return campSetHandler(kwlnTaskGetUserValue(), 1, callback);
}

s64 mnuFinishCampPopup(s32 callback) {
    return campSetHandler(kwlnTaskGetUserValue(), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260950);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260A58);

s32 mnuCampFindListedItemIndex(s32 id) {
    u16 *entry = (u16 *)mnuCampListedItems;
    u32 index = 0;
    do {
        if (id == *entry) {
            return index;
        }
        entry += 4;
        index++;
    } while (index < 3);
    return -1;
}

typedef struct EffItemSlot44 {
    u32 id;
    u8 pad_04[0x40];
} EffItemSlot44;

extern EffItemSlot44 D_003CD8F0[];
extern void mdlFlagSet(u32);

s32 itmClaimFreeSlot(s32 row) {
    s32 first = row * 2;
    EffItemSlot44 *slot = &D_003CD8F0[first];
    s32 i = 0;

    do {
        u32 id = slot->id;

        slot++;
        if (id != 0) {
            if (mdlFlagTest(id) == 0) {
                mdlFlagSet(id);
                return i;
            }
        }
        i++;
    } while (i < 2);
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260C28);

s32 func_00260C38(s32 slot, s32 row) {
    return row * 2 + slot;
}

u32 mnuCampChooseSolarWeightedOption(s32 row) {
    s32 chance = effMiscRand(0) & 0xFF;
    u8 *item = D_003CD8DD + row * 8;
    s32 total = 0;
    u32 index = 0;
    do {
        if (evtGetMirroredSolarPhase() == 8) {
            total += item[-3];
        } else {
            total += item[0];
        }
        if (total >= chance) {
            return index;
        }
        index++;
        item++;
    } while (index < 3);
    return 0;
}

u8 mnuCampChooseWeightedTableValue(void) {
    s32 chance = effMiscRand(0) & 0xFF;
    s32 total = 0;
    u8 *item = D_003CD8D0;
    u32 index = 0;
    do {
        total += item[1];
        if (total >= chance) {
            return item[0];
        }
        index++;
        item += 2;
    } while (index < 3);
    return 1;
}

s32 mnuCampFindFreeWideEntryIndex(s32 row) {
    s32 *entry = (s32 *)(D_003CDA8C + row * 0xC0);
    s32 index = 0;
    do {
        s32 value = *entry;
        entry += 3;
        if (value == 0) {
            return index;
        }
        index++;
    } while (index < 16);
    return 15;
}

s32 mnuCampFindFreeCompactEntryIndex(s32 unused, s32 row) {
    s32 *entry = (s32 *)(mnuCampCompactEntries + row * 0x44);
    s32 index = 0;
    do {
        s32 value = *entry;
        entry += 2;
        if (value == 0) {
            return index;
        }
        index++;
    } while (index < 8);
    return 7;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260DF0);

extern u8 D_003CDA89[];

u8 func_00260FE8(s32 row, s32 column) {
    return D_003CDA89[column * 0xC + row * 0xC0];
}

extern u8 D_003CD8F4[];

u8 func_00261018(s32 row, s32 column) {
    return D_003CD8F4[column * 8 + row * 0x44];
}

s32 mnuCampResolveOwnedItemVariant(s32 row, s32 column) {
    u8 *entry = D_003CDA88 + column * 0xC + row * 0xC0;
    s32 id = *(s32 *)(D_003CDA88 + column * 0xC + row * 0xC0 + 4);

    if (entry[1] == 0 && func_002C54B0(id) != 0 && ((CampSaveState *)datGameState)->counts[id] != 0) {
        id = *(u16 *)(entry + 8);
    }
    return id;
}

s32 mnuCampGetCompactEntryId(s32 row, s32 column) {
    return *(s32 *)(mnuCampCompactEntries + row * 0x44 + column * 8);
}

s32 mnuCampCountRemainingUses(s32 mode, s32 id, s32 record) {
    s32 value = ((ShopEffectScene *)record)->availableCount;

    if (mode == 1) {
        value -= ptyCountBulletItem(id);
    } else if (mode == 3) {
        value = 1 - ((CampSaveState *)datGameState)->counts[id];
    } else if (mode == 2) {
        value = 1 - ((CampSaveState *)datGameState)->counts[id];
    } else {
        value = 99 - ((CampSaveState *)datGameState)->counts[id];
    }
    if (mnuCampFindListedItemIndex(id) >= 0) {
        if (value >= 2) {
            value = ((CampSaveState *)datGameState)->counts[id] == 0;
        }
    }
    return value < 0 ? 0 : value;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261290);

/* Mark rows unavailable when neither currency nor inventory capacity permits a use. */
void func_00261310(u8 *scene) {
    CampWindowNode *node;
    CampWindowParams *item;
    s32 i;
    s32 count;
    s32 remaining;

    node = ((ShopScene *)scene)->extra->list->first;
    for (i = 0; i < ((ShopScene *)scene)->extra->list->count; i++) {
        item = &node->params;
        /* Keep the unchecked division: retail traps when the row price is zero. */
        count = ((CampSaveState *)datGameState)->money / item->price;
        remaining = mnuCampCountRemainingUses(item->mode, item->id, (s32)scene);
        if (remaining < count) {
            count = remaining;
        }
        if (count == 0) {
            node->flags = 1;
        }
        node = node->next;
        if (node == NULL) {
            break;
        }
    }
}

typedef struct CampPriceTier {
    u8 limit;
    u8 percent;
} CampPriceTier;

extern CampPriceTier D_003CE3E8[];

typedef struct CampRateRow {
    u16 percent;
    u8 pad02[10];
} CampRateRow;

extern CampRateRow D_003CE150[];

s32 func_002613C8(s32 level, s32 price) {
    u32 i;

    if (level != 0) {
        for (i = 0; i < 8; i++) {
            if (D_003CE3E8[i].limit != 0 && D_003CE3E8[i].limit >= level) {
                price = price * D_003CE3E8[i].percent / 100;
                break;
            }
        }
    } else {
        price = price * D_003CE150[*(s32 *)(datGameState + 0x1E658)].percent / 100;
    }
    return price;
}

s32 mnuCampAdvanceCounter(s32 delta, u8 *scene) {
    s32 max = func_00261198(scene);
    CampWindowParams *slot = &((ShopScene *)scene)->extra->list->cursor->params;
    s32 sum = ((ShopScene *)scene)->quantity + delta;
    s32 cur;

    ((ShopScene *)scene)->quantity = sum;
    if (sum <= 0) {
        ((ShopScene *)scene)->quantity = 1;
    }
    cur = ((ShopScene *)scene)->quantity;
    if (cur >= max) {
        ((ShopScene *)scene)->atLimit = 1;
        ((ShopScene *)scene)->quantity = max;
        cur = max;
    } else {
        ((ShopScene *)scene)->atLimit = 0;
    }
    if (((ShopScene *)scene)->sprites[0]->list->cursor->params.value == 3) {
        slot->value = func_002613C8(cur, slot->price);
        cur = ((ShopScene *)scene)->quantity;
    }
    return cur;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261538);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261670);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261850);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002619A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261B98);

void mnuQueueCampTextGlyphWithChildColor(s32 fontValue, s32 enabled, s32 unused2, s32 unused3, s32 fontArg, s32 flags) {
    s32 handle;

    if (enabled != 0) {
        handle = func_0019FC38(0x970, 0xB58, 1, (u16)fontValue, enabled, fontArg);
        frFontSetChildColors(handle, 0x80808040);
        func_0019D550(handle, 0, flags);
        frFontQueueGlyphInSelectedSlot(handle);
    }
}

INCLUDE_SDATA(const s32, "game/code_0025DA20", mnuShopRestoreMiddleVector);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_004377F8);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437800);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437808);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437810);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437818);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437820);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437828);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437830);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437838);

