#include "common.h"

/* Intrusive list node threaded through +0x4. */
typedef struct FileNode {
    u32 unk0;              /* 0x0 */
    struct FileNode *next; /* 0x4 */
} FileNode;

/* Work record behind the fileManager getters below. */
typedef struct FileWork {
    u8 unk0[0x10];   /* 0x0 */
    u32 unk10;       /* 0x10 */
    u32 unk14;       /* 0x14 */
    FileNode *head;  /* 0x18 */
    u8 unk1C[4];     /* 0x1C */
    u32 unk20;       /* 0x20 */
    u32 unk24;       /* 0x24 */
} FileWork;

extern s32 func_00288BA8(u32);

INCLUDE_ASM(const s32, "file/fileManager", func_002887A0);

INCLUDE_ASM(const s32, "file/fileManager", func_00288818);

INCLUDE_ASM(const s32, "file/fileManager", func_002888C8);

void func_00288988(FileWork *list, FileNode *node) {
    node->next = list->head;
    list->head = node;
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288998);

INCLUDE_ASM(const s32, "file/fileManager", func_002889D8);

void func_00288A80(u32 arg0) {
    func_002889D8(arg0, 0, 0, 0, 0);
}

void func_00288AA8(u32 arg0) {
    func_002889D8(arg0, 1, 0, 0, 0);
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288AD0);

void func_00288B48(u32 arg0) {
    func_00288AD0(arg0, 0, 0, 0);
}

void func_00288B68(u32 arg0) {
    func_00288AD0(arg0, 1, 0, 0);
}

u32 func_00288B88(FileWork *work) {
    return work->unk20;
}

u32 func_00288B90(FileWork *work) {
    return work->unk24;
}

u32 func_00288B98(FileWork *work) {
    return work->unk14;
}

u32 func_00288BA0(FileWork *work) {
    return work->unk10;
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288BA8);

INCLUDE_ASM(const s32, "file/fileManager", func_00288BE8);

void fileWaitReady(u32 arg0) {
    while (func_00288BA8(arg0) == 0) {
        func_002E7098();
        func_002897A0();
    }
}

void func_00288C50(u32 id) {
    fileWaitReady(id);
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288C68);

INCLUDE_ASM(const s32, "file/fileManager", func_00288CB8);

INCLUDE_ASM(const s32, "file/fileManager", func_00288D48);

INCLUDE_ASM(const s32, "file/fileManager", func_00288D68);
