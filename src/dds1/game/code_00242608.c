#include "common.h"

extern s32 func_002D03F8(s32);
extern u8 *sdfResourceRetainAddress(s32);
extern void func_00285490(u8 *);
extern void evtLoadResourcePair(const char *, u8 *);
extern s32 func_0024D9D8(s32);
extern s32 func_00244848();
extern s32 D_003BC520;
extern s32 func_0019D208(s32, s32);
extern void mnuUnpackNibbleFields();

extern u8 D_00368C40[];

extern s32 D_003BAA00;

extern s8 D_003BC39C;

extern s32 kwlnTaskFindByPriority(u32);

extern s64 evtFindTaskById(void);

s32 effDestroyResourceSlotSet(u32 sprite);

extern s32 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

extern char D_003BC3A0[]; /* "camp" */

extern char D_003AF418[]; /* "camp_draw" */

extern char D_003AF428[]; /* "camp_update" */

extern void evtFormatTaskName(s32 taskId, void *name);
extern void *func_002CFEB8(s32 size);
extern void *memset(void *dst, s32 c, u32 n);
extern s32 kwlnTaskCreate(void *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);
extern void func_002424B0(void);
extern void evtReleaseEventPackResources(void);
extern f32 D_003D80F0[];
extern f32 D_003D80E0[];
extern f32 D_003D80D0[];
extern s32 D_003BC380;
extern s32 func_00241A50(s32, s32);
extern u8 *effCreateStatusBatch(s32 kind);
extern s32 effDestroyPackedBatch(s32);
extern s32 D_0036AA60[];
extern s32 effLoadIndexedResource(const char *, s32, s32);

#define CAMP_TASK_PRIORITY 0x3EC

typedef struct CampTaskData {
    s32 taskId;
    s32 unk4;
    u8 pad08[0x40];
} CampTaskData;

/* Schedule the camp task only if no task currently owns this event ID. */
void mnuCampCreateTask(s32 taskId) {
    char name[0x20];
    CampTaskData *data;

    if (evtFindTaskById() == 0) {
        evtFormatTaskName(taskId, name);
        data = func_002CFEB8(0x48);
        memset(data, 0, 0x48);
        data->taskId = taskId;
        data->unk4 = 0;
        kwlnTaskCreate(name, CAMP_TASK_PRIORITY, 1, 1, func_002424B0, evtReleaseEventPackResources, data);
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

    while (task = kwlnTaskFindByPriority(CAMP_TASK_PRIORITY), task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
}

typedef struct ScrollNode {
    u16 pos;                  /* 0x0 */
    u8 unk2[0x2E];            /* 0x2 */
    struct ScrollNode *next;  /* 0x30 */
} ScrollNode;

typedef struct ScrollList {
    u8 unk0[0x1C];
    s16 base;                 /* 0x1C */
    u8 unk1E[0x36];
    ScrollNode *nodes;        /* 0x54 */
} ScrollList;

typedef struct ScrollOwner {
    u8 unk0[0xC];
    s32 limit;                /* 0xC */
    u8 unk10[0x22F8];
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
    u8 pad0C[0x24];
    struct FxChild *next; /* 0x30 */
} FxChild;

typedef struct FxNode {
    s32 kind;             /* 0x00 */
    u8 pad04[0x18];
    s16 base;             /* 0x1C */
    u8 pad1E[0x36];
    FxChild *children;    /* 0x54 */
    u8 pad58[0x24];
    struct FxNode *next;  /* 0x7C */
} FxNode;

typedef struct FxWorld {
    u8 pad00[0xC];
    union {
        s32 whole;
        u16 low;
    } limitv;             /* 0x0C */
    u8 pad10[4];
    s32 unk14;            /* 0x14 */
    s32 unk18;            /* 0x18 */
    s32 unk1C;            /* 0x1C */
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
    if (world->unk14 + delta < 0) {
        world->unk14 = 10;
    } else {
        world->unk14 += delta;
    }
    if (world->limitv.whole + delta < 0) {
        world->limitv.whole = 10;
    } else {
        world->limitv.whole += delta;
    }
    if (world->unk18 > world->limitv.whole) {
        world->unk18 = world->limitv.whole;
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
    func_002300B8(world, delta, threshold, base, offset, ubase, node);
    evtViewerDispatchFlagMode(world);
}

INCLUDE_ASM(const s32, "game/code_00242608", mnuFxWorldDropOutOfRange);

INCLUDE_ASM(const s32, "game/code_00242608", func_002429F0);

void func_00242BD0(f32 *a, f32 *b, f32 *c, f32 *d, f32 *e) {
    a[0] = 0.7f;
    a[1] = 0.7f;
    a[2] = 0.7f;
    a[3] = 0.0f;
    b[0] = 0.65f;
    b[1] = 0.39f;
    b[2] = 0.65f;
    b[3] = 0.0f;
    c[0] = 0.2f;
    c[1] = 0.2f;
    c[2] = 0.2f;
    c[3] = 1.0f;
    d[0] = 7.0f;
    *e = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242C30);

extern s32 strcmp(const char *a, const char *b);

typedef struct CampDisplayDefaults {
    s32 width;
    s32 height;
    s8 color[4];
    f32 scaleX;
    f32 scaleY;
    s32 enabled;
    s32 variant;
} CampDisplayDefaults;

void mnuCampInitDisplayDefaults(CampDisplayDefaults *display) {
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
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242E70);

typedef struct CampWindowDefaults {
    s32 unk0;
    s32 unk4;
    s32 unk8;
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
} CampWindowDefaults;

void func_00242F20(CampWindowDefaults *w) {
    w->unk0 = 0x96;
    w->unk4 = 0x96;
    w->unk10 = 0x50;
    w->unk8 = 0x96;
    w->unkC = 0x1E;
    w->unk14 = 1;
    w->unk20 = 7;
    w->unk24 = 4;
    w->unk28 = 0xA;
    w->unk2C = 0x20;
    w->unk30 = 0x10;
    w->unk34 = 0x10;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243048);

INCLUDE_ASM(const s32, "game/code_00242608", func_002432D0);

typedef struct CampOwner {
    u8 pad00[0x104];
    s32 handle; /* 0x104 */
} CampOwner;

typedef struct CampWorld {
    u8 pad00[8];
    CampOwner *owner; /* 0x08 */
    u8 pad0C[0x2028];
    FxNode *entries;  /* 0x2034 */
} CampWorld;

s32 campAnyPackedFlagSet(CampWorld *scene) {
    FxNode *node;
    FxChild *child;
    s32 low;
    s32 high;

    for (node = scene->entries; node != NULL; node = node->next) {
        if (node->kind == 4) {
            for (child = node->children; child != NULL; child = child->next) {
                mnuUnpackNibbleFields(child, &low, &high);
                if (func_0019D208(scene->owner->handle, low) == 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

typedef struct CampPacked {
    u8 pad00[8];
    union {
        u32 whole;
        u16 packed;
    } value;
} CampPacked;

void mnuUnpackNibbleFields(CampPacked *src, s32 *low, s32 *high) {
    *low = src->value.packed & 0xFFF;
    *high = src->value.packed >> 12;
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

typedef struct {
    u8 pad00[0x2034];
    CampEntryNode *entries; /* 0x2034 */
    u8 pad2038[0x394];
    s32 dispatchMode; /* 0x23CC: checked after func_00243818 */
    u8 pad23D0[0x10];
    s32 pendingValue; /* 0x23E0 */
    u8 pad23E4[0x28];
    u32 state; /* 0x240C */
    u32 fontResource; /* 0x2410: returned by func_001951C8 */
    u8 pad2414[0x14];
    s32 descriptorResource; /* 0x2428 */
    u8 pad242C[4];
    s32 menuState; /* 0x2430 */
    u8 pad2434[4];
    u32 auxResource; /* 0x2438 */
    u32 options; /* 0x243C: two two-bit fields */
    u8 pad2440[4];
    s32 registeredCount; /* 0x2444 */
    s32 registeredIds[10]; /* 0x2448 */
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

INCLUDE_ASM(const s32, "game/code_00242608", func_00243608);

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

INCLUDE_ASM(const s32, "game/code_00242608", func_00243818);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243928);

void func_00243A18(CampScene *scene) {
    func_00243818();
    if (scene->dispatchMode == 1) {
        func_00134CD8();
        return;
    }
}

extern s32 D_00368BD8[];
extern s32 func_001951C8(s32 *resources, s32, s32, s32, s32);
extern void func_00195450(s32 resource, s32 width, s32 height);

void mnuCampInitFontResource(CampScene *scene) {
    s32 resource;
    scene->fontResource = 0;
    resource = func_001951C8(D_00368BD8, 0, 0, 0, 0);
    scene->fontResource = resource;
    func_00195450(resource, 0x960, 0x70);
}

void mnuCampLinkFontGlyph(CampScene *scene) {
    func_00194920(scene->fontResource);
    scene->fontResource = 0;
}

extern void func_002D0B50(s32 *);

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void) {
    s32 info[8];
    s32 quotient;

    func_002D0B50(info);
    quotient = 1 / info[0];
}

void func_00243B00(CampScene *scene) {
    if ((scene->menuState == 0) || (scene->menuState == 5)) {
        scene->menuState = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243B28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243BF0);

extern s32 D_003BA8F8;
extern s32 sdfAllocatePacketList();
extern void sdfCreateDescriptorPacket();

typedef struct BufferDescriptor {
    u8 unk0[0x10];
    void (*open)(struct BufferDescriptor *, s32);
} BufferDescriptor;

extern BufferDescriptor D_00325708;

void mnuShopSubmitDescriptor(CampScene *scene) {
    s32 packet;

    if (scene->descriptorResource != 0) {
        packet = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(packet, *(s32 *)(D_003BA8F8 + 0x10), 0, 0, 0x200, 0xE0, scene->descriptorResource, 0);
        D_00325708.open(&D_00325708, packet);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243D48);

void func_00243EC8(CampScene *scene) {
    func_00243D48(scene->auxResource);
}

void func_00243EE0(void) {
}

void mnuCampSetPrimaryOption(CampScene *scene, u32 value) {
    scene->options = (scene->options & 0xfffffffc) | (value & 3);
}

u32 mnuCampGetPrimaryOption(CampScene *scene) {
    return scene->options & 3;
}

void mnuCampSetSecondaryOption(CampScene *scene, u32 value) {
    scene->options = (scene->options & 0xfffffff3) | ((value & 3) << 2);
}

u32 mnuCampGetSecondaryOption(CampScene *scene) {
    return (scene->options & 0xc) >> 2;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243F48);

void mnuShopSavePrimaryTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    for (i = 0; i < 4; i++) {
        D_003D80F0[i] = coordinates[i + 8];
        D_003D80D0[i] = coordinates[i];
    }
    D_003BC380 = 0;
}

void mnuShopSaveFullTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    for (i = 0; i < 4; i++) {
        D_003D80F0[i] = coordinates[i + 8];
        D_003D80E0[i] = coordinates[i + 4];
        D_003D80D0[i] = coordinates[i];
    }
    D_003BC380 = 1;
}

void mnuShopRestoreTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    s32 useMiddle = D_003BC380;
    for (i = 0; i < 4; i++) {
        coordinates[i + 8] = D_003D80F0[i];
        if (useMiddle != 0) {
            coordinates[i + 4] = D_003D80E0[i];
        }
        coordinates[i] = D_003D80D0[i];
    }
}

void mnuShopRegisterSceneObject(CampScene *scene, s32 identifier) {
    s32 count = scene->registeredCount;
    s32 i = 0;
    if (count > 0) {
        s32 *entry = scene->registeredIds;
        do {
            if (*entry == identifier) {
                return;
            }
            entry++;
            i++;
        } while (i < count);
    }
    if (count < 10) {
        scene->registeredIds[count] = identifier;
        scene->registeredCount++;
    }
}

void func_002441E8(CampScene *scene) {
    s32 count = 0;
    if (scene->registeredCount > 0) {
        s32 *entry = scene->registeredIds;
        do {
            s32 identifier = *entry++;
            count++;
            func_00241A50(*(s32 *)(*(u8 **)((u8 *)scene + 8) + 0x10c), identifier);
        } while (count < scene->registeredCount);
    }
    scene->registeredCount = 0;
}

typedef struct ShopScene {
    s32 resourceHandle; /* 0x00 */
    u8 pad04[0x58];
    u8 resourcePair[4]; /* 0x5C */
    s32 pairedHandle; /* 0x60 */
    u32 spriteResource; /* 0x64 */
    s32 batchState; /* 0x68 */
    u8 *sprite; /* 0x6C */
    s32 window; /* 0x70 */
    u8 *batches[2]; /* 0x74, 0x78 */
    s32 initialSelection; /* 0x7C */
    u8 pad80[0xC];
    s32 count8C; /* 0x8C: func_00244898 */
    u8 pad90[8];
    s32 count98; /* 0x98: func_00244848 */
} ShopScene;

typedef struct ShopBatchGraphics {
    u8 pad00[0x20];
    s32 *params; /* 0x20 */
} ShopBatchGraphics;

typedef struct ShopBatch {
    u8 pad00[8];
    ShopBatchGraphics *graphics; /* 0x08 */
} ShopBatch;

void func_00244258(ShopScene *scene) {
    ShopBatch *object;
    ShopBatchGraphics *graphics;
    s32 *params;
    s32 defaultValue = 15;
    scene->batchState = 0;
    object = (ShopBatch *)effCreateStatusBatch(6);
    graphics = object->graphics;
    scene->batches[0] = (u8 *)object;
    params = graphics->params;
    params[0] = defaultValue;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    params[4] = 0;
    object = (ShopBatch *)effCreateStatusBatch(1);
    graphics = object->graphics;
    scene->batches[1] = (u8 *)object;
    params = graphics->params;
    params[0] = defaultValue;
    params[1] = 0;
}

s32 mnuShopReleaseSceneObjects(ShopScene *scene) {
    s32 *objects = (s32 *)scene->batches;
    s32 result;
    u32 i;
    for (i = 0; i < 2; i++) {
        result = effDestroyPackedBatch(*objects++);
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF3D0);

void mnuShopLoadSpriteAssets(ShopScene *scene) {
    u32 *resource = &scene->spriteResource;
    *resource = effLoadIndexedResource("/facility/spr/shop/", D_0036AA60[0], 0);
}

s64 func_00244360(ShopScene *scene) {
    return effDestroyResourceSlotSet(scene->spriteResource);
}

extern s32 D_003BAA00;
extern u8 *D_003BAA68;

s32 mnuShopHasPendingFlag(void) {
    u8 *flags = (u8 *)(D_003BAA00 + 0x12A0);
    u8 *entry = D_003BAA68;
    s32 found = 0;
    s32 i;

    for (i = 0; i < 0xC0; flags++, i++) {
        if ((u32)(i - 0xA0) >= 0x20 && *flags != 0) {
            if ((*entry & 3) != 0) {
                found = 1;
                break;
            }
            if ((u32)(i - 0x60) < 0x20) {
                found = 1;
                break;
            }
        }
        entry += 8;
    }
    return found;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002443F8);

void func_002444D0(s32 *record) {
    record[27] = func_002443F8(D_00368C40, 3, record);
}

typedef struct CampFlagRow {
    s16 flag[8]; /* 0x00 */
    u8 value[9]; /* 0x10: [0] default, [i + 1] for flag[i] */
    u8 pad19;
} CampFlagRow;

extern CampFlagRow D_00368C50[];

s32 campFlagRowValue(s32 row) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_00368C50[row].flag[i] > 0 && mdlFlagTest(D_00368C50[row].flag[i])) {
            return D_00368C50[row].value[i + 1];
        }
    }
    return D_00368C50[row].value[0];
}

extern u8 D_00369A88[];

s32 mnuCampFindActiveSlot(void) {
    s32 i;
    u8 *base = D_00369A88;
    s16 *p = (s16 *)(base + 0x410);
    for (i = 4; i >= 0; i--, p = (s16 *)((u8 *)p - 0x104)) {
        if (*p > 0 && mdlFlagTest(*p)) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244658);

typedef struct ShopBuf {
    u8 unk0[0x30];
    void *buffer;    /* 0x30 */
} ShopBuf;

typedef struct ShopSprite {
    u8 unk0[0x14];
    ShopBuf *data;   /* 0x14 */
} ShopSprite;

extern void mnuDestroyWindowContainer();
extern void func_002CFF98();

void mnuShopReleaseSprites(ShopScene *scene) {
    ShopSprite **slot = (ShopSprite **)&scene->sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        ShopSprite *sprite = *slot;

        if (sprite->data->buffer != NULL) {
            func_002CFF98(sprite->data->buffer);
            sprite = *slot;
            sprite->data->buffer = NULL;
        }
        mnuDestroyWindowContainer(sprite);
        slot++;
    }
    if (scene->window != 0) {
        mnuDestroyWindowContainer(scene->window);
    }
}

s32 mnuCampGetProgressStage(void) {
    s32 result = 0;
    if (mdlFlagTest(0x970)) {
        result = 1;
    }
    if (mdlFlagTest(0x971)) {
        result = 2;
    }
    if (mdlFlagTest(0x972)) {
        result = 3;
    }
    if (mdlFlagTest(0x973)) {
        result = 4;
    }
    if (mdlFlagTest(0x974)) {
        result = 5;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244848);

s32 func_00244898(void) {
    u16 flags;
    u16 *entry;
    s32 remaining;
    s32 count;

    count = 0;
    remaining = 4;
    entry = (u16 *)(D_003BAA00 + 0xa60);
    do {
        flags = *entry;
        entry += 0xd2;
        remaining--;
        count += flags & 1;
    } while (remaining >= 0);
    return count;
}

ShopScene *mnuShopCreateScene(void) {
    s32 handle;
    ShopScene *obj;

    handle = func_002D03F8(0xB4);
    obj = (ShopScene *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0xB4);
    obj->resourceHandle = handle;
    func_00285490((u8 *)obj + 8);
    mnuShopLoadSpriteAssets(obj);
    func_00244258(obj);
    evtLoadResourcePair("/facility/msg/shop/mes_data.bmd", obj->resourcePair);
    func_0024D9D8(obj->pairedHandle);
    D_003BC520 = obj->spriteResource;
    obj->count98 = func_00244848();
    obj->count8C = func_00244898();
    return obj;
}

extern s32 func_00101A70();
extern void func_00285600();
extern void dspCloseChannel();
extern void evtReleaseResourcePairHandle();
extern void func_002D0918();

void mnuShopDestroyScene(s32 arg) {
    ShopScene *scene = (ShopScene *)func_00101A70();

    if (scene != NULL) {
        mnuShopReleaseSprites(scene);
        func_00244360(scene);
        mnuShopReleaseSceneObjects(scene);
        func_00285600((u8 *)scene + 8, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(scene->resourcePair);
        func_002D0918(scene->resourceHandle);
        D_003BC39C = 2;
    }
}

extern s64 mnuCampRunPanel0(u64 request);
extern s64 mnuCampRunPanel1(u64 request);
extern s64 mnuCampRunPanel2(u64 request);

/* Create the camp context and its three scheduler tasks (main, draw, update).
 * Optionally seed the initial selection from the caller. */

s32 func_002449F0(s32 *initialSelection) {
    ShopScene *ctx = mnuShopCreateScene();
    s32 result;

    if (initialSelection != 0) {
        ctx->initialSelection = *initialSelection;
    }
    kwlnTaskCreate(D_003BC3A0, 0x402, 1, 1, mnuCampRunPanel0, 0, ctx);
    kwlnTaskCreate(D_003AF418, 0x2B12, 1, 1, mnuCampRunPanel1, 0, ctx);
    result = kwlnTaskCreate(D_003AF428, 0x520E, 1, 1, mnuCampRunPanel2, mnuShopDestroyScene, ctx);
    D_003BC39C = 1;
    return result;
}

void mnuCampDestroyPanelTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC3A0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF418, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF428, 0);
}

s32 mnuPollTaskState(void) {
    s32 state = D_003BC39C;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC39C = 0;
    }
    return 0;
}

extern void func_002858E8(s32 *, void *);
extern u8 D_0036AB48[];

s64 mnuCampRunPanel0(u64 request) {
    s32 state = func_00101A70();
    s32 *panel = (s32 *)(state + 0x54);
    func_002858E8(panel, D_0036AB48);
    return func_00285670(state + 8, panel, 0, request);
}

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

s64 mnuCampRunPanel1(u64 request) {
    s32 state = func_00101A70();
    return menuRunPanel(state, 1, request);
}

s64 mnuCampRunPanel2(u64 request) {
    s32 state = func_00101A70();
    return menuRunPanel(state, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244C00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244D10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244E08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244FA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245068);

extern s32 func_00244E08(void *scene);

typedef struct CampCounterState {
    u8 pad00[0x80];
    s32 counter; /* 0x80 */
    u8 pad84[0x2F];
    u8 atLimit; /* 0xB3 */
} CampCounterState;

s32 mnuCampClampSceneCounter(s32 delta, CampCounterState *scene) {
    s32 limit = func_00244E08(scene);
    s32 sum = scene->counter + delta;
    s32 current;
    scene->counter = sum;
    if (sum <= 0) {
        scene->counter = 1;
    }
    current = scene->counter;
    if (current >= limit) {
        scene->atLimit = 1;
        scene->counter = limit;
        current = limit;
    } else {
        scene->atLimit = 0;
    }
    return current;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00245208);

INCLUDE_ASM(const s32, "game/code_00242608", func_002453C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245628);

INCLUDE_ASM(const s32, "game/code_00242608", func_002457E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245A40);

extern s32 func_00197C40(s32, s32, s32, s32, s32, s32);

extern void frFontSetChildColors(s32, u32);

extern void func_001958A0(s32, s32, s32);

extern void func_00194920(s32);

void func_00245C00(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    s32 handle;

    if (a1 != 0) {
        handle = func_00197C40(0x970, 0xB58, 1, (u16)a0, a1, a4);
        frFontSetChildColors(handle, 0x80808040);
        func_001958A0(handle, 0, a5);
        func_00194920(handle);
    }
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF428);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC380);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC388);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC390);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC398);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC39C);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A0);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A8);

