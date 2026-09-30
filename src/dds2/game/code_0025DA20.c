#include "common.h"

extern s32 func_0024B040(u32);

extern s32 func_003292A8(s32);
extern u8 *sdfResourceRetainAddress(s32);
extern void func_002C3E58(u8 *);
extern void func_0025F7F0(u8 *);
extern void func_002945B8(u8 *);
extern void func_002C1B58(u8 *, s32);
extern void func_003421E8(s32);
extern void sndStartTrackExtended(s32);
extern s32 func_00261198();
extern s32 func_002613C8();
extern s32 func_002C54C8(s32);

extern s8 D_003CD8D8[34];

extern void func_00246950();

typedef struct CampFlagRow {
    u8 pad00[4];
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

extern CampFlagRow D_003C9A40[];

extern s32 mdlFlagTest(u32);

extern void func_002C42B0(s32 *, void *);

extern u8 D_003CE658[];

extern s32 D_00435CC8;

extern s32 sdfAllocatePacketList();

extern void sdfCreateDescriptorPacket();

extern s32 func_0025CE68(s32, s32);

extern s32 strcmp(const char *a, const char *b);

extern s32 func_0019CE78(s32 *, s32, s32, s32, s32);

extern void func_0019D100(s32, s32, s32);

extern s32 D_003C99B8[];

extern void func_0024ACC0();

extern void evtViewerDispatchFlagMode();

extern s64 evtFindTaskById(void);

extern s32 func_00101820(u32);

extern s32 D_00435DD0;

extern s32 func_00261B98(s32);

extern void func_0025FD78(s32);

extern char D_00437838[]; /* "camp" */

extern char D_00424BC0[]; /* "camp_draw" */

extern char D_00424BD0[]; /* "camp_update" */

extern s8 D_00437837;

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_00101958();

extern void func_00260380();

extern void func_00328E48();

extern void mnuDestroyWindowContainer();

extern void func_0025FCD8();

extern void func_002B99D8();

extern void func_003054E8();

extern s32 mnuShopReleaseSceneObjects(u8 *);

extern void func_002C3FC8(s32, s32);

extern void func_0026C728(void);

extern void evtReleaseResourcePairHandle();

extern void func_003297C8(s32);

extern void func_002C1B70(s32, s32);

extern void func_002C1B68(s32, s32);

extern s32 func_0026C768(void);

extern u8 D_003CD8D0[];

extern s32 effMiscRand(s32);

extern u8 D_003CDA8C[];

extern u8 D_003CD8F8[];

extern s32 evtGetMirroredSolarPhase(void);

extern u8 D_003CD8DD[];

extern void evtFormatTaskName(s32 arg0, void *arg1);

extern void *func_00328D68(s32 size);

extern void *memset(void *dst, s32 c, u32 n);

extern void kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern void func_0025D8C8(void);

extern s32 func_002C54B0(s32);
extern s32 func_002C5480(s32);
extern s32 func_002C5498(s32);
extern u8 *D_00435E38;

extern u8 D_003CDA88[];

extern s32 func_0019FC38(s32, s32, u64, u64, u64, u64);

extern void frFontSetChildColors(s32, u32);

extern s32 func_0019D550(s32, s32, u32);

extern void evtReleaseEventPackResources(void);
extern f32 D_00453CB0[];
extern f32 D_00453CA0[];
extern f32 D_00453C90[];
extern s32 D_004377F0;
extern u8 *func_00304998(s32 kind);
extern s32 effDestroyPackedBatch(s32);

#define CAMP_TASK_PRIORITY 0x3EC

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
        data = func_00328D68(0x48);
        memset(data, 0, 0x48);
        data->taskId = taskId;
        data->unk4 = 0;
        kwlnTaskCreate(name, CAMP_TASK_PRIORITY, 1, 1, func_0025D8C8, evtReleaseEventPackResources, data);
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
    func_0024ACC0(world, delta, threshold, base, offset, ubase, node);
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

void func_0025DFE8(f32 *a, f32 *b, f32 *c, f32 *d, f32 *e) {
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
    *d = 7.0f;
    *e = 0.0f;
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

void func_0025E240(CampDisplayDefaults *display) {
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

void func_0025E338(CampListLayout *layout) {
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

void func_0025E6F0(CampKeyTrack *track, s32 value, CampKeyNode **out1, CampKeyNode **out2) {
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
        while (*out1 != 0 && func_0024B040((*out1)->condition) != 1) {
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

typedef struct {
    u8 pad00[0x2034];
    CampEntryNode *entries; /* 0x2034 */
    u8 pad2038[0x394];
    s32 sceneMode; /* 0x23CC */
    u8 pad23D0[0x10];
    s32 pendingValue; /* 0x23E0 */
    u8 pad23E4[0x28];
    u32 state; /* 0x240C */
    u32 effectHandle; /* 0x2410 */
    u8 pad2414[0x14];
    s32 descriptorHandle; /* 0x2428: submitted to the drawing packet */
    u8 pad242C[4];
    u32 menuState; /* 0x2430 */
    u8 pad2434[8];
    u32 optionFlags; /* 0x243C */
    u8 pad2440[4];
    s32 idCount; /* 0x2444 */
    s32 registeredIds[20]; /* 0x2448 */
} CampScene;

s32 mnuCampFindMatchingEntryIndex(u8 *entry, CampScene *scene, s32 nameIndex) {
    CampEntryNode *node = scene->entries;
    while (node != NULL) {
        if (strcmp((char *)scene + (node->nameIndex << 5) + 0x24,
                   (char *)*(u8 **)(entry + 0x7c) + (nameIndex << 5)) == 0) {
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

void func_0025E8D0(CampScene *scene, CampCue *cue) {
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

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025ED10);

void func_0025EE00(CampScene *scene) {
    func_0025EC00();
    if (scene->sceneMode == 1) {
        func_00137888();
        return;
    }
}

void mnuCampInitFontResource(CampScene *scene) {
    s32 resource;
    scene->effectHandle = 0;
    resource = func_0019CE78(D_003C99B8, 0, 0, 0, 0);
    scene->effectHandle = resource;
    func_0019D100(resource, 0x960, 0x70);
}

void mnuCampLinkFontGlyph(CampScene *scene) {
    func_0019C5B0(scene->effectHandle);
    scene->effectHandle = 0;
}

extern void func_00329A00(s32 *);

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void func_0025EEC0(void) {
    s32 info[8];
    s32 quotient;

    func_00329A00(info);
    quotient = 1 / info[0];
}

void func_0025EEE8(CampScene *scene) {
    if ((scene->menuState == 0) || (scene->menuState == 5)) {
        scene->menuState = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EF10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EFD8);

typedef struct BufferDescriptor {
    u8 unk0[0x10];
    void (*open)(struct BufferDescriptor *, s32);
} BufferDescriptor;

extern BufferDescriptor D_00380708;

void mnuShopSubmitDescriptor(u8 *work) {
    s32 packet;

    if (((CampScene *)work)->descriptorHandle != 0) {
        packet = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(packet, *(s32 *)(D_00435CC8 + 0x10), 0, 0, 0x200, 0xE0, ((CampScene *)work)->descriptorHandle, 0);
        D_00380708.open(&D_00380708, packet);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F130);

void func_0025F2B0(s32 arg0) {
    func_0025F130(*(u32 *)(arg0 + 0x2438));
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
    f32 *coordinates = (f32 *)(scene + 0x2330);
    for (i = 0; i < 4; i++) {
        D_00453CB0[i] = coordinates[i + 8];
        D_00453C90[i] = coordinates[i];
    }
    D_004377F0 = 0;
}

void mnuShopSaveFullTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    for (i = 0; i < 4; i++) {
        D_00453CB0[i] = coordinates[i + 8];
        D_00453CA0[i] = coordinates[i + 4];
        D_00453C90[i] = coordinates[i];
    }
    D_004377F0 = 1;
}

void mnuShopRestoreTransform(u8 *scene) {
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    s32 useMiddle = D_004377F0;
    for (i = 0; i < 4; i++) {
        coordinates[i + 8] = D_00453CB0[i];
        if (useMiddle != 0) {
            coordinates[i + 4] = D_00453CA0[i];
        }
        coordinates[i] = D_00453C90[i];
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

void func_0025F5D0(CampScene *scene) {
    s32 count = 0;
    if (scene->idCount > 0) {
        s32 *entry = scene->registeredIds;
        do {
            s32 identifier = *entry++;
            count++;
            func_0025CE68(*(s32 *)(*(u8 **)((u8 *)scene + 8) + 0x10c), identifier);
        } while (count < scene->idCount);
    }
    scene->idCount = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F708);

void func_0025F7F0(u8 *scene) {
    u8 *object;
    u8 *graphics;
    s32 *params;
    s32 defaultValue = 15;
    *(s32 *)(scene + 0x78) = 0;
    object = func_00304998(6);
    graphics = *(u8 **)(object + 8);
    *(u8 **)(scene + 0x84) = object;
    params = *(s32 **)(graphics + 0x20);
    params[0] = defaultValue;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    params[4] = 0;
    object = func_00304998(1);
    graphics = *(u8 **)(object + 8);
    *(u8 **)(scene + 0x88) = object;
    params = *(s32 **)(graphics + 0x20);
    params[0] = defaultValue;
    params[1] = 0;
}

s32 mnuShopReleaseSceneObjects(u8 *scene) {
    s32 *objects = (s32 *)(scene + 0x84);
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

void func_0025FA10(s32 arg0) {
    effDestroyPackedBatch(*(u32 *)(arg0 + 0x3c));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FA28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FC08);

void func_0025FCD8(s32 scene) {
    s32 i;
    s32 *slot = (s32 *)(scene + 0x68);

    func_002B99D8(*(s32 *)(scene + 0x378));
    for (i = 1; i >= 0; i--) {
        func_003054E8(*slot++);
    }
    func_003054E8(*(s32 *)(scene + 0x70));
    func_003054E8(*(s32 *)(scene + 0x74));
    switch (*(s32 *)(scene + 8)) {
    case 1:
    case 3:
        func_0025FA10(scene + 0x210);
        break;
    case 0:
    case 2:
        break;
    default:
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FD78);

s32 func_0025FE70(void) {
    s32 result = 0;
    s32 i;

    for (i = 0; i < 0x100; i++) {
        if (func_002C5480(i) != 0) {
            continue;
        }
        if (func_002C54B0(i) != 0) {
            continue;
        }
        if (*(u8 *)(i + D_00435DD0 + 0x1340) == 0) {
            continue;
        }
        if ((D_00435E38[i * 8] & 3) != 0) {
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

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FF18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260020);

s32 func_00260138(s32 row) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_003C9A40[row].flag[i] > 0 && mdlFlagTest(D_003C9A40[row].flag[i])) {
            return D_003C9A40[row].value[i + 1];
        }
    }
    return D_003C9A40[row].value[0];
}

s32 mnuCampFindActiveSlot(void) {
    s32 i;
    u8 *base = D_003CBB70;
    s16 *p = (s16 *)(base + 0x1450);
    for (i = 0x14; i >= 0; i--, p = (s16 *)((u8 *)p - 0x104)) {
        if (*p > 0 && mdlFlagTest(*p)) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260250);

typedef struct ShopBuf {
    u8 unk0[0x30];
    void *buffer;    /* 0x30 */
} ShopBuf;

typedef struct ShopSprite {
    u8 unk0[0x18];
    ShopBuf *data;   /* 0x18 */
} ShopSprite;

typedef struct ShopScene {
    u8 unk0[0x7C];
    ShopSprite *sprites[1]; /* 0x7C */
    ShopSprite *extra;      /* 0x80 */
    u8 pad84[0xC];
    s32 quantity;           /* 0x90: clamped to [1, maximum] */
    u8 pad94[0x33];
    u8 atLimit;             /* 0xC7 */
} ShopScene;

void func_00260380(s32 keepExtra, ShopScene *scene) {
    ShopSprite **slot = scene->sprites;
    ShopSprite *sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        sprite = *slot;
        if (sprite != NULL) {
            if (sprite->data->buffer != NULL) {
                func_00328E48(sprite->data->buffer);
                sprite = *slot;
                sprite->data->buffer = NULL;
            }
            mnuDestroyWindowContainer(sprite);
        }
        slot++;
    }
    if (keepExtra == 0) {
        if (scene->extra != NULL) {
            if (scene->extra->data->buffer != NULL) {
                func_00328E48(scene->extra->data->buffer);
                scene->extra->data->buffer = NULL;
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

s32 func_00260468(void) {
    u16 entryFlags;
    u16 *entry;
    s32 remaining;
    s32 enabledCount;

    enabledCount = 0;
    remaining = 4;
    entry = (u16 *)(D_00435DD0 + 0xa60);
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

s32 func_002604A0(void) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (i + 1 < 3) {
            if (D_003CE408[i].threshold > *(u32 *)(D_00435DD0 + 0x1E650)) {
                break;
            }
        } else if (D_003CE408[i].threshold <= *(u32 *)(D_00435DD0 + 0x1E650)) {
            break;
        }
    }
    return D_003CE408[i].value;
}

void func_00260538(void) {
    u16 entryId;
    s8 *entry;
    u32 index;

    index = 0;
    entry = D_003CD8D8;
    do {
        entryId = *(u16 *)entry;
        entry = (s8 *)((s32)entry + 8);
        index = index + 1;
        *(u8 *)((u32)entryId + D_00435DD0 + 0x1340) = 0;
    } while (index < 3);
}

u8 *func_00260570(void) {
    s32 handle;
    u8 *obj;

    handle = func_003292A8(0x38C);
    obj = sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x38C);
    *(s32 *)obj = handle;
    func_002C3E58(obj + 0xC);
    func_0025F7F0(obj);
    *(u32 *)(obj + 0xAC) = func_00260460();
    *(s32 *)(obj + 0x9C) = func_00260468();
    *(s32 *)(obj + 0xE4) = 0xF;
    func_002945B8(obj);
    func_002C1B58(obj + 0x37C, 0x60);
    func_00260538();
    func_003421E8(0x300000);
    sndStartTrackExtended(0x300000);
    return obj;
}

void func_00260620(s32 arg) {
    s32 scene = func_00101958();

    if (scene != 0) {
        func_00260380(0, scene);
        func_0025FCD8(scene);
        mnuShopReleaseSceneObjects((u8 *)scene);
        func_002C3FC8(scene + 0xC, arg);
        func_0026C728();
        evtReleaseResourcePairHandle(scene + 0x60);
        func_003297C8(*(s32 *)scene);
        D_00437837 = 2;
    }
}

s32 func_002606A0(void) {
    s32 state = func_00101958() + 0x37C;
    func_002C1B70(state, 0x53);
    if (func_0026C768() != 0) {
        func_002C1B68(state, 1);
    } else {
        func_002C1B68(state, 0);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BD0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260708);

void mnuCampDestroyPanelTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437838, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BC0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BD0, 0);
}

s32 func_00260848(void) {
    s32 state = D_00437837;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437837 = 0;
    }
    return 0;
}

static inline s64 campSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), mode, callback);
}

s64 func_00260880(s32 callback) {
    s32 context = func_00101958();
    func_002C42B0((s32 *)(context + 0x58), D_003CE658);
    return campSetHandler(context, 0, callback);
}

s64 func_002608E0(s32 callback) {
    return campSetHandler(func_00101958(), 1, callback);
}

s64 func_00260918(s32 callback) {
    return campSetHandler(func_00101958(), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260950);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260A58);

s32 func_00260B50(s32 id) {
    u16 *entry = (u16 *)D_003CD8D8;
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

s32 func_00260C38(s32 arg0, s32 arg1) {
    return arg1 * 2 + arg0;
}

u32 func_00260C48(s32 row) {
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

u8 func_00260CE8(void) {
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

s32 func_00260D50(s32 row) {
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

s32 func_00260DA0(s32 unused, s32 row) {
    s32 *entry = (s32 *)(D_003CD8F8 + row * 0x44);
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

s32 func_00261040(s32 row, s32 column) {
    u8 *entry = D_003CDA88 + column * 0xC + row * 0xC0;
    s32 id = *(s32 *)(D_003CDA88 + column * 0xC + row * 0xC0 + 4);

    if (entry[1] == 0 && func_002C54B0(id) != 0 && *(u8 *)(id + D_00435DD0 + 0x1340) != 0) {
        id = *(u16 *)(entry + 8);
    }
    return id;
}

s32 func_002610C0(s32 row, s32 column) {
    return *(s32 *)(D_003CD8F8 + row * 0x44 + column * 8);
}

s32 func_002610E8(s32 mode, s32 id, s32 record) {
    s32 value = *(s32 *)(record + 0x9C);

    if (mode == 1) {
        value -= func_002C54C8(id);
    } else if (mode == 3) {
        value = 1 - *(u8 *)(id + D_00435DD0 + 0x1340);
    } else if (mode == 2) {
        value = 1 - *(u8 *)(id + D_00435DD0 + 0x1340);
    } else {
        value = 99 - *(u8 *)(id + D_00435DD0 + 0x1340);
    }
    if (func_00260B50(id) >= 0) {
        if (value >= 2) {
            value = *(u8 *)(id + D_00435DD0 + 0x1340) == 0;
        }
    }
    return value < 0 ? 0 : value;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261290);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261310);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002613C8);

typedef struct CampCounterSlot {
    s32 value;  /* 0x00 */
    u8 pad04[4];
    s32 limit;  /* 0x08 */
} CampCounterSlot;

s32 func_00261480(s32 delta, u8 *scene) {
    s32 max = func_00261198(scene);
    CampCounterSlot *slot = (CampCounterSlot *)(*(u8 **)(*(u8 **)(*(u8 **)(scene + 0x80) + 0x18) + 0x1C) + 0x60);
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
    if (*(s32 *)(*(u8 **)(*(u8 **)(*(u8 **)(scene + 0x7C) + 0x18) + 0x1C) + 0x60) == 3) {
        slot->value = func_002613C8(cur, slot->limit);
        cur = ((ShopScene *)scene)->quantity;
    }
    return cur;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261538);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261670);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261850);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002619A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261B98);

void func_00261D78(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    s32 handle;

    if (a1 != 0) {
        handle = func_0019FC38(0x970, 0xB58, 1, (u16)a0, a1, a4);
        frFontSetChildColors(handle, 0x80808040);
        func_0019D550(handle, 0, a5);
        func_0019C5B0(handle);
    }
}

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_004377F0);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_004377F8);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437800);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437808);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437810);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437818);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437820);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437828);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437830);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437838);

