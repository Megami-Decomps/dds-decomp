#include "common.h"

extern s32 func_002C8128(u32);

/* Intrusive list node threaded through +0x4. */
typedef struct FileNode {
    u32 unk0;              /* 0x0 */
    struct FileNode *next; /* 0x4 */
} FileNode;

/* Work record behind the fileManager getters below. */
typedef struct FileWork {
    u8 unk0[0x10];   /* 0x0 */
    u32 unk10;       /* 0x10 */
    u32 size;        /* 0x14: loaded resource size */
    FileNode *head;  /* 0x18 */
    u8 unk1C[4];     /* 0x1C */
    u32 resourceHandle; /* 0x20 */
    u32 unk24;       /* 0x24 */
} FileWork;

INCLUDE_ASM(const s32, "file/fileManager", func_002C7D00);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7D78);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7E28);

void filePrependNode(FileWork *list, FileNode *node) {
    node->next = list->head;
    list->head = node;
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C7EF8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7F38);

void func_002C7FF0(u32 arg0) {
    func_002C7F38(arg0, 0, 0, 0, 0);
}

void func_002C8018(u32 arg0) {
    func_002C7F38(arg0, 1, 0, 0, 0);
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C8040);

void func_002C80C8(u32 arg0) {
    func_002C8040(arg0, 0, 0, 0);
}

void func_002C80E8(u32 arg0) {
    func_002C8040(arg0, 1, 0, 0);
}

u32 func_002C8108(FileWork *work) {
    return work->resourceHandle;
}

u32 func_002C8110(FileWork *work) {
    return work->unk24;
}

u32 func_002C8118(FileWork *work) {
    return work->size;
}

u32 func_002C8120(FileWork *work) {
    return work->unk10;
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C8128);

INCLUDE_ASM(const s32, "file/fileManager", func_002C8168);

void fileWaitReady(u32 arg0) {
    s64 temp_v0;

    while (temp_v0 = func_002C8128(arg0), temp_v0 == 0) {
        sdfRestoreDeviceThreadPriority();
        fileManUpdate();
    }
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C81D0);

INCLUDE_ASM(const s32, "file/fileManager", func_002C81E8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C8238);

INCLUDE_ASM(const s32, "file/fileManager", func_002C82C8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C82E8);
