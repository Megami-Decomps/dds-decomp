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
    u8 pad02[0xA];
    u32 handle; /* 0x0C */
    s32 size; /* 0x10 */
    u8 pad14[0x54];
    u16 unk68;
    u16 slot; /* 0x6A */
} FileRequest;

typedef struct FileCleanup {
    u8 kind;
    u8 state;
    u8 pad02[6];
    void *resource;
    u32 handle;
} FileCleanup;

extern s32 btlDestroyStageTask(void *);
extern void sdfDevQueueReleaseState(u32);
extern void sdfReleaseChipBlock(void *);
extern void func_0035B6E0(const char *, ...);
extern void *sdfAllocAndClearQuadwords(s32);
extern void func_00346A80(void *, void *);
extern void func_00346AE8(void *);
extern void func_002C7D78(void *, s32, u32, s32, s32);

s32 filePollEntryCleanup(FileCleanup *entry) {
    if (entry->kind == 1) {
        return btlDestroyStageTask(entry);
    }
    if (entry->state == 6) {
        if (entry->handle != 0) {
            sdfDevQueueReleaseState(entry->handle);
        }
        sdfReleaseChipBlock(entry->resource);
        sdfReleaseChipBlock(entry);
        return 0;
    }
    return 1;
}

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
typedef struct FileManSlot {
    FileRequest *request; /* 0x24 + 8 * slot */
    u32 unk4;
} FileManSlot;

typedef struct FileManWork {
    s32 sema;    /* 0x00 */
    u8 pad04;
    u8 nextSlot; /* 0x05 */
    u8 pad06;
    u8 freeSlots; /* 0x07 */
    void *unk8;  /* 0x08 */
    u8 pad0C[0xC];
    u32 unk18;   /* 0x18 */
    u32 buffer;  /* 0x1C */
    u8 pad20[4];
    FileManSlot slots[4]; /* 0x24 */
} FileManWork;

extern FileManWork D_00457F28;

void *func_002C7F38(u32 request, s32 flags, void *dispatch, s32 arg4, s32 arg5) {
    u8 *work;
    u8 *packet;

    func_0035B6E0("pac load %s\n", request);
    work = sdfAllocAndClearQuadwords(0x70);
    packet = work + 0x30;
    func_00346A80(packet, dispatch);
    if (flags != 0) {
        func_00346AE8(packet);
    }
    func_002C7D78(work, 1, request, arg4, arg5);
    return work;
}

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

extern s32 fileIsRequestReadyInCurrentMode(FileRequest *file);

s32 fileIsRequestReadyInCurrentMode(FileRequest *file) {
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

    while (status = fileIsRequestReadyInCurrentMode(request), status == 0) {
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

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void sdfDevQueueRead(u32 handle, u32 buffer, u32 size);

/* Claim the next of four read slots for a pending request and start its device read. */
void fileQueuePendingRequestInFreeSlot(FileRequest *request) {
    FileManWork *work = &D_00457F28;
    u8 slot;
    s32 size;

    WaitSema(work->sema);
    if (work->freeSlots == 0) {
        SignalSema(work->sema);
        return;
    }
    if (request->state != 3) {
        SignalSema(work->sema);
        return;
    }
    request->state = 4;
    slot = work->nextSlot;
    if (slot == 3) {
        work->nextSlot = 0;
    } else {
        work->nextSlot = slot + 1;
    }
    request->slot = slot;
    work->freeSlots--;
    work->slots[slot].request = request;
    size = request->size;
    if (size > 0x10000) {
        size = 0x10000;
    }
    SignalSema(work->sema);
    sdfDevQueueRead(request->handle, work->buffer + (slot << 16), size);
}
