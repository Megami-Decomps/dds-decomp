#include "common.h"

extern u8 D_00368C40[];

extern s32 D_003BAA00;

extern s8 D_003BC39C;

extern s32 kwlnTaskFindByPriority(u32);

extern s64 evtFindTaskById(void);

s32 func_002BDD60(u32 sprite);

extern s32 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

extern char D_003BC3A0[]; /* "camp" */

extern char D_003AF418[]; /* "camp_draw" */

extern char D_003AF428[]; /* "camp_update" */

extern void evtFormatTaskName(s32 arg0, void *arg1);
extern void *func_002CFEB8(s32 size);
extern void *memset(void *dst, s32 c, u32 n);
extern s32 kwlnTaskCreate(void *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);
extern void func_002424B0(void);
extern void evtReleaseEventPackResources(void);

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

void campDestroyTaskById(void) {
    s64 task;

    task = evtFindTaskById();
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
        return;
    }
}

/* Drain every camp task at the scheduler priority used during creation. */
void campDestroyAllTasks(void) {
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

void func_00242780(FxWorld *world, s32 delta, s32 threshold, s32 base, s32 offset, s32 ubase) {
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
    func_0022FDE8(world);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242950);

INCLUDE_ASM(const s32, "game/code_00242608", func_002429F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242BD0);

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

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243048);

INCLUDE_ASM(const s32, "game/code_00242608", func_002432D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243390);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243440);

typedef struct CampEntryNode {
    u8 pad00[8];
    s32 nameIndex; /* 0x08: 32-byte name in the owning scene */
    u8 pad0C[0x1C];
    u32 status; /* 0x28 */
    u8 pad2C[0x50];
    struct CampEntryNode *next; /* 0x7C */
} CampEntryNode;

typedef struct {
    u8 pad00[0x2034];
    CampEntryNode *entries; /* 0x2034 */
    u8 pad2038[0x3D4];
    u32 state; /* 0x240C */
    u32 fontResource; /* 0x2410: returned by func_001951C8 */
    u8 pad2414[0x1C];
    s32 menuState; /* 0x2430 */
    u8 pad2434[4];
    u32 auxResource; /* 0x2438 */
    u32 options; /* 0x243C: two two-bit fields */
    u8 pad2440[4];
    s32 registeredCount; /* 0x2444 */
    s32 registeredIds[10]; /* 0x2448 */
} CampScene;

s32 campFindMatchingEntryIndex(u8 *entry, CampScene *scene, s32 nameIndex) {
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

INCLUDE_ASM(const s32, "game/code_00242608", func_00243558);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243608);

void campResetEntryStatus(CampScene *scene) {
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

void func_00243A18(s32 arg0) {
    func_00243818();
    if (*(s32 *)(arg0 + 0x23cc) == 1) {
        func_00134CD8();
        return;
    }
}

extern s32 D_00368BD8[];
extern s32 func_001951C8(s32 *resources, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
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

INCLUDE_ASM(const s32, "game/code_00242608", func_00243AD8);

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

void mnuShopSubmitDescriptor(u8 *work) {
    s32 packet;

    if (*(s32 *)(work + 0x2428) != 0) {
        packet = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(packet, *(s32 *)(D_003BA8F8 + 0x10), 0, 0, 0x200, 0xE0, *(s32 *)(work + 0x2428), 0);
        D_00325708.open(&D_00325708, packet);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243D48);

void func_00243EC8(CampScene *scene) {
    func_00243D48(scene->auxResource);
}

void func_00243EE0(void) {
}

void func_00243EE8(CampScene *scene, u32 value) {
    scene->options = (scene->options & 0xfffffffc) | (value & 3);
}

u32 func_00243F08(CampScene *scene) {
    return scene->options & 3;
}

void func_00243F18(CampScene *scene, u32 value) {
    scene->options = (scene->options & 0xfffffff3) | ((value & 3) << 2);
}

u32 func_00243F38(CampScene *scene) {
    return (scene->options & 0xc) >> 2;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243F48);

void mnuShopSavePrimaryTransform(u8 *scene) {
    extern f32 D_003D80F0[];
    extern f32 D_003D80D0[];
    extern s32 D_003BC380;
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    for (i = 0; i < 4; i++) {
        D_003D80F0[i] = coordinates[i + 8];
        D_003D80D0[i] = coordinates[i];
    }
    D_003BC380 = 0;
}

void mnuShopSaveFullTransform(u8 *scene) {
    extern f32 D_003D80F0[];
    extern f32 D_003D80E0[];
    extern f32 D_003D80D0[];
    extern s32 D_003BC380;
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
    extern f32 D_003D80F0[];
    extern f32 D_003D80E0[];
    extern f32 D_003D80D0[];
    extern s32 D_003BC380;
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
    extern s32 func_00241A50(s32, s32);
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

void func_00244258(u8 *scene) {
    extern u8 *func_002BD258(s32 kind);
    u8 *object;
    u8 *graphics;
    s32 *params;
    s32 defaultValue = 15;
    *(s32 *)(scene + 0x68) = 0;
    object = func_002BD258(6);
    graphics = *(u8 **)(object + 8);
    *(u8 **)(scene + 0x74) = object;
    params = *(s32 **)(graphics + 0x20);
    params[0] = defaultValue;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    params[4] = 0;
    object = func_002BD258(1);
    graphics = *(u8 **)(object + 8);
    *(u8 **)(scene + 0x78) = object;
    params = *(s32 **)(graphics + 0x20);
    params[0] = defaultValue;
    params[1] = 0;
}

s32 mnuShopReleaseSceneObjects(u8 *scene) {
    extern s32 effDestroyPackedBatch(s32);
    s32 *objects = (s32 *)(scene + 0x74);
    s32 result;
    u32 i;
    for (i = 0; i < 2; i++) {
        result = effDestroyPackedBatch(*objects++);
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF3D0);

void mnuShopLoadSpriteAssets(u8 *scene) {
    extern s32 D_0036AA60[];
    extern s32 effLoadIndexedResource(const char *, s32, s32);
    s32 *resource = (s32 *)(scene + 0x64);
    *resource = effLoadIndexedResource("/facility/spr/shop/", D_0036AA60[0], 0);
}

s64 func_00244360(u8 *work) {
    return func_002BDD60(*(u32 *)(work + 0x64));
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

void func_002444D0(s32 *arg0) {
    arg0[27] = func_002443F8(D_00368C40, 3, arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244508);

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

extern void func_0027C430();
extern void func_002CFF98();

void mnuShopReleaseSprites(u8 *scene) {
    ShopSprite **slot = (ShopSprite **)(scene + 0x6C);
    u32 i;

    for (i = 0; i < 1; i++) {
        ShopSprite *sprite = *slot;

        if (sprite->data->buffer != NULL) {
            func_002CFF98(sprite->data->buffer);
            sprite = *slot;
            sprite->data->buffer = NULL;
        }
        func_0027C430(sprite);
        slot++;
    }
    if (*(s32 *)(scene + 0x70) != 0) {
        func_0027C430(*(s32 *)(scene + 0x70));
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

INCLUDE_ASM(const s32, "game/code_00242608", func_002448D0);

extern s32 func_00101A70();
extern void func_00285600();
extern void func_0024DBC8();
extern void evtReleaseResourcePairHandle();
extern void func_002D0918();

void mnuShopDestroyScene(s32 arg) {
    u8 *scene = (u8 *)func_00101A70();

    if (scene != NULL) {
        mnuShopReleaseSprites(scene);
        func_00244360(scene);
        mnuShopReleaseSceneObjects(scene);
        func_00285600(scene + 8, arg);
        func_0024DBC8();
        evtReleaseResourcePairHandle(scene + 0x5C);
        func_002D0918(*(s32 *)scene);
        D_003BC39C = 2;
    }
}

extern u8 *func_002448D0(void);
extern s64 mnuCampRunPanel0(u64 request);
extern s64 mnuCampRunPanel1(u64 request);
extern s64 mnuCampRunPanel2(u64 request);

/* Create the camp context and its three scheduler tasks (main, draw, update).
 * Optionally seed the initial selection from the caller. */
typedef struct MnuCampStartContext {
    u8 pad00[0x7C];
    s32 initialSelection; /* 0x7C */
} MnuCampStartContext;

s32 func_002449F0(s32 *initialSelection) {
    u8 *ctx = func_002448D0();
    s32 result;

    if (initialSelection != 0) {
        ((MnuCampStartContext *)ctx)->initialSelection = *initialSelection;
    }
    kwlnTaskCreate(D_003BC3A0, 0x402, 1, 1, mnuCampRunPanel0, 0, ctx);
    kwlnTaskCreate(D_003AF418, 0x2B12, 1, 1, mnuCampRunPanel1, 0, ctx);
    result = kwlnTaskCreate(D_003AF428, 0x520E, 1, 1, mnuCampRunPanel2, mnuShopDestroyScene, ctx);
    D_003BC39C = 1;
    return result;
}

void func_00244AB8(void) {
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

extern s32 func_00244E08(void *arg0);

s32 mnuCampClampSceneCounter(s32 arg0, u8 *arg1) {
    s32 v = func_00244E08(arg1);
    s32 sum = *(s32 *)(arg1 + 0x80) + arg0;
    s32 cur;
    *(s32 *)(arg1 + 0x80) = sum;
    if (sum <= 0) {
        *(s32 *)(arg1 + 0x80) = 1;
    }
    cur = *(s32 *)(arg1 + 0x80);
    if (cur >= v) {
        arg1[0xB3] = 1;
        *(s32 *)(arg1 + 0x80) = v;
        cur = v;
    } else {
        arg1[0xB3] = 0;
    }
    return cur;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00245208);

INCLUDE_ASM(const s32, "game/code_00242608", func_002453C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245628);

INCLUDE_ASM(const s32, "game/code_00242608", func_002457E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245A40);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245C00);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF428);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC380);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC388);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC390);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC398);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC39C);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A0);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A8);

