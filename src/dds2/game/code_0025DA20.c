#include "common.h"

extern s8 D_003CD8D8[34];

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
extern void func_002C1B70(s32, s32);
extern void func_002C1B68(s32, s32);
extern s32 func_0026C768(void);
extern u8 D_003CD8D0[];
extern s32 effMiscRand(s32);
extern u8 D_003CDA8C[];
extern u8 D_003CD8F8[];

extern s32 func_00243330(void);
extern u8 D_003CD8DD[];



INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DA20);

void func_0025DAB0(void) {
    s64 temp_v0;

    temp_v0 = evtFindTaskById();
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
        return;
    }
}

void func_0025DAE8(void) {
    s64 temp_v0;

    while (temp_v0 = func_00101820(0x3ec), temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DB20);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DB98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DD68);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DE08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DFE8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E048);


INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E240);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E288);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E338);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E390);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E460);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E6F0);

u32 func_0025E7B0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E7B8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E7D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E858);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E8D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E980);

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
    u8 pad2410[0x34];
    s32 idCount; /* 0x2444 */
    s32 registeredIds[20]; /* 0x2448 */
} CampScene;

void func_0025EBC8(CampScene *scene) {
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

void func_0025EE00(s32 arg0) {
    func_0025EC00();
    if (*(s32 *)(arg0 + 0x23cc) == 1) {
        func_00137888();
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EE40);

void func_0025EE90(s32 arg0) {
    func_0019C5B0(*(u32 *)(arg0 + 0x2410));
    *(u32 *)(arg0 + 0x2410) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EEC0);

void func_0025EEE8(s32 arg0) {
    if ((*(s32 *)(arg0 + 0x2430) == 0) || (*(s32 *)(arg0 + 0x2430) == 5)) {
        *(u32 *)(arg0 + 0x2430) = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EF10);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025EFD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F0B0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F130);

void func_0025F2B0(s32 arg0) {
    func_0025F130(*(u32 *)(arg0 + 0x2438));
}

void func_0025F2C8(void) {
}

void func_0025F2D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffffc) | (arg1 & 3);
}

u32 func_0025F2F0(s32 arg0) {
    return *(u32 *)(arg0 + 0x243c) & 3;
}

void func_0025F300(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffff3) | ((arg1 & 3) << 2);
}

u32 func_0025F320(s32 arg0) {
    return (*(u32 *)(arg0 + 0x243c) & 0xc) >> 2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F330);

void shopSavePrimaryTransform(u8 *scene) {
    extern f32 D_00453CB0[];
    extern f32 D_00453C90[];
    extern s32 D_004377F0;
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    for (i = 0; i < 4; i++) {
        D_00453CB0[i] = coordinates[i + 8];
        D_00453C90[i] = coordinates[i];
    }
    D_004377F0 = 0;
}

void shopSaveFullTransform(u8 *scene) {
    extern f32 D_00453CB0[];
    extern f32 D_00453CA0[];
    extern f32 D_00453C90[];
    extern s32 D_004377F0;
    s32 i;
    f32 *coordinates = (f32 *)(scene + 0x2330);
    for (i = 0; i < 4; i++) {
        D_00453CB0[i] = coordinates[i + 8];
        D_00453CA0[i] = coordinates[i + 4];
        D_00453C90[i] = coordinates[i];
    }
    D_004377F0 = 1;
}

void shopRestoreTransform(u8 *scene) {
    extern f32 D_00453CB0[];
    extern f32 D_00453CA0[];
    extern f32 D_00453C90[];
    extern s32 D_004377F0;
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

void func_0025F568(CampScene *scene, s32 id) {
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

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F5D0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F708);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F7F0);

INCLUDE_ASM(const s32, "game/code_0025DA20", shopReleaseSceneObjects);

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
    destroyPackedEffectBatch(*(u32 *)(arg0 + 0x3c));
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FA28);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FC08);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FCD8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FD78);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FE70);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FF18);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260020);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260138);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002601D8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260250);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260380);

u32 func_00260458(void) {
    return 0;
}

u32 func_00260460(void) {
    return 0;
}

s32 func_00260468(void) {
    u16 temp_v0;
    u16 *puVar2;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v1 = 4;
    puVar2 = (u16 *)(D_00435DD0 + 0xa60);
    do {
        temp_v0 = *puVar2;
        puVar2 = puVar2 + 0xe2;
        temp_v1 = temp_v1 - 1;
        temp_v2 = temp_v2 + (temp_v0 & 1);
    } while (-1 < temp_v1);
    return temp_v2;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002604A0);

void func_00260538(void) {
    u16 temp_v0;
    s8 *pcVar2;
    u32 temp_v1;

    temp_v1 = 0;
    pcVar2 = D_003CD8D8;
    do {
        temp_v0 = *(u16 *)pcVar2;
        pcVar2 = (s8 *)((s32)pcVar2 + 8);
        temp_v1 = temp_v1 + 1;
        *(u8 *)((u32)temp_v0 + D_00435DD0 + 0x1340) = 0;
    } while (temp_v1 < 3);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260570);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260620);

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

void func_00260808(void) {
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

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260880);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002608E0);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260918);

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

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260B90);

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
        if (func_00243330() == 8) {
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

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261040);


s32 func_002610C0(s32 row, s32 column) {
    return *(s32 *)(D_003CD8F8 + row * 0x44 + column * 8);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002610E8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261290);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261310);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002613C8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261480);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261538);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261670);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261850);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_002619A8);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261B98);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261D78);

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

