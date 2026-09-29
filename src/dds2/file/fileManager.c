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

typedef struct FileRequest {
    u8 pad00;
    u8 state; /* 0x01: ready when 6 */
    u8 pad02[0x66];
    u16 unk68;
} FileRequest;

INCLUDE_ASM(const s32, "file/fileManager", func_002C7D00);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7D78);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7E28);

void filePrependNode(FileWork *list, FileNode *node) {
    node->next = list->head;
    list->head = node;
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C7EF8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7F38);

void func_002C7FF0(u32 request) {
    func_002C7F38(request, 0, 0, 0, 0);
}

void func_002C8018(u32 request) {
    func_002C7F38(request, 1, 0, 0, 0);
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C8040);

void func_002C80C8(u32 request) {
    func_002C8040(request, 0, 0, 0);
}

void func_002C80E8(u32 request) {
    func_002C8040(request, 1, 0, 0);
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

s32 func_002C8168(FileRequest *file) {
    s32 result = 0;
    if (file->unk68 != 0) {
        result = file->state == 6;
    }
    return result;
}

void fileWaitReady(u32 request) {
    s64 status;

    while (status = func_002C8128(request), status == 0) {
        sdfRestoreDeviceThreadPriority();
        fileManUpdate();
    }
}

void func_002C81D0(u32 id) {
    fileWaitReady(id);
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C81E8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C8238);

INCLUDE_ASM(const s32, "file/fileManager", func_002C82C8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C82E8);
