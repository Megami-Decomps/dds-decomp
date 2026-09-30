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

/* Unlink a node from the list threaded through +0x4. */
void fileUnlinkNode(FileWork *list, FileNode *node) {
    FileNode **link = &list->head;
    FileNode *cur;

    while ((cur = *link) != node) {
        link = &cur->next;
    }
    *link = node->next;
}

/* Work area behind the fileMan task. */
typedef struct FileManWork {
    u8 pad00[8];
    void *unk8;  /* 0x08 */
    u8 pad0C[0xC];
    u32 unk18;   /* 0x18 */
} FileManWork;

extern FileManWork D_00457F28;

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

u32 fileGetResourceHandle(FileWork *work) {
    return work->resourceHandle;
}

u32 func_002C8110(FileWork *work) {
    return work->unk24;
}

u32 fileGetResourceSize(FileWork *work) {
    return work->size;
}

u32 func_002C8120(FileWork *work) {
    return work->unk10;
}

extern s32 func_002C8128(FileRequest *file);

s32 func_002C8128(FileRequest *file) {
    s32 result;

    if (file->pad00 == 1) {
        result = 0;
        if (file->unk68 != 0) {
            result = file->state == 6;
        }
        return result;
    }
    return file->state == 6;
}

s32 fileRequestIsReady(FileRequest *file) {
    s32 result = 0;
    if (file->unk68 != 0) {
        result = file->state == 6;
    }
    return result;
}

/* Keep the device scheduler and file manager running while a request finishes. */
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

/* Spin until the file manager has no work left. */
void fileWaitIdle(void) {
    FileManWork *work = &D_00457F28;

    while (work->unk8 != 0 || work->unk18 != 0) {
        fileManUpdate();
    }
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C8238);

void func_002C82C8(a, b, c)
s32 a;
s32 b;
s32 c;
{
    func_002C8238(a, b, c, 0, 0);
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C82E8);
