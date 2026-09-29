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
    u32 size;        /* 0x14: loaded resource size */
    FileNode *head;  /* 0x18 */
    u8 unk1C[4];     /* 0x1C */
    u32 resourceHandle; /* 0x20: released with func_002D0918 */
    u32 unk24;       /* 0x24 */
} FileWork;

extern s32 func_00288BA8(u32);

INCLUDE_ASM(const s32, "file/fileManager", func_002887A0);

INCLUDE_ASM(const s32, "file/fileManager", func_00288818);

INCLUDE_ASM(const s32, "file/fileManager", func_002888C8);

void filePrependNode(FileWork *list, FileNode *node) {
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

u32 fileGetResourceHandle(FileWork *work) {
    return work->resourceHandle;
}

u32 func_00288B90(FileWork *work) {
    return work->unk24;
}

u32 fileGetResourceSize(FileWork *work) {
    return work->size;
}

u32 func_00288BA0(FileWork *work) {
    return work->unk10;
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288BA8);

s32 fileRequestIsReady(u8 *file) {
    s32 result = 0;
    if (*(u16 *)(file + 0x68) != 0) {
        result = *(u8 *)(file + 1) == 6;
    }
    return result;
}

void fileWaitReady(u32 id) {
    while (func_00288BA8(id) == 0) {
        sdfRestoreDeviceThreadPriority();
        fileManUpdate();
    }
}

void func_00288C50(u32 id) {
    fileWaitReady(id);
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288C68);

INCLUDE_ASM(const s32, "file/fileManager", func_00288CB8);

void func_00288D48(a, b, c)
s32 a;
s32 b;
s32 c;
{
    func_00288CB8(a, b, c, 0, 0);
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288D68);
