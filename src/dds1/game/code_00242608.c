#include "common.h"

extern u8 D_00368C40[];

extern s32 D_003BAA00;

extern s8 D_003BC39C;

extern s32 kwlnTaskFindByPriority(u32);

extern s64 evtFindTaskById(void);

s32 func_002BDD60(u32 sprite);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

extern char D_003BC3A0[]; /* "camp" */

extern char D_003AF418[]; /* "camp_draw" */

extern char D_003AF428[]; /* "camp_update" */

extern void evtFormatTaskName(s32 arg0, void *arg1);
extern void *func_002CFEB8(s32 size);
extern void *memset(void *dst, s32 c, u32 n);
extern void kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern void func_002424B0(void);
extern void func_00242510(void);

typedef struct CampTaskData {
    s32 taskId;
    s32 unk4;
    u8 pad08[0x40];
} CampTaskData;

void mnuCampCreateTask(s32 arg0) {
    char name[0x20];
    CampTaskData *data;

    if (evtFindTaskById() == 0) {
        evtFormatTaskName(arg0, name);
        data = func_002CFEB8(0x48);
        memset(data, 0, 0x48);
        data->taskId = arg0;
        data->unk4 = 0;
        kwlnTaskCreate(name, 0x3EC, 1, 1, func_002424B0, func_00242510, data);
    }
}

void campDestroyTaskById(void) {
    s64 temp_v0;

    temp_v0 = evtFindTaskById();
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
        return;
    }
}

void campDestroyAllTasks(void) {
    s64 temp_v0;

    while (temp_v0 = kwlnTaskFindByPriority(0x3ec), temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242708);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242780);

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

void func_00243A58(CampScene *scene) {
    s32 resource;
    scene->fontResource = 0;
    resource = func_001951C8(D_00368BD8, 0, 0, 0, 0);
    scene->fontResource = resource;
    func_00195450(resource, 0x960, 0x70);
}

void func_00243AA8(CampScene *scene) {
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

INCLUDE_ASM(const s32, "game/code_00242608", func_00243CC8);

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

INCLUDE_ASM(const s32, "game/code_00242608", func_00244380);

INCLUDE_ASM(const s32, "game/code_00242608", func_002443F8);

void func_002444D0(s32 *arg0) {
    arg0[27] = func_002443F8(D_00368C40, 3, arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244508);

INCLUDE_ASM(const s32, "game/code_00242608", func_002445E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244658);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244740);

INCLUDE_ASM(const s32, "game/code_00242608", func_002447D8);

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

INCLUDE_ASM(const s32, "game/code_00242608", func_00244970);

INCLUDE_ASM(const s32, "game/code_00242608", func_002449F0);

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

INCLUDE_ASM(const s32, "game/code_00242608", func_00244B30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244B90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244BC8);

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

