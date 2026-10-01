#include "common.h"

/* Intrusive list node threaded through +0x4. */
typedef struct FileNode {
    u32 unk0;              /* 0x0 */
    struct FileNode *next; /* 0x4 */
    void *unk8;            /* 0x08: duplicated string */
    u32 unk10;             /* 0x10 */
    u32 unk14;             /* 0x14 */
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

typedef struct FileRequest {
    u8 mode; /* 0x00: mode 1 uses the conditional readiness path */
    u8 state; /* 0x01: ready when 6 */
    u8 pad02[0xA];
    u32 handle; /* 0x0C */
    s32 size; /* 0x10 */
    u8 pad14[0x54];
    u16 stateRequired; /* 0x68: gate state == 6 readiness checks */
    u16 slot; /* 0x6A */
} FileRequest;

/* Work area behind the fileMan task (see game/code_00288E70). */
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

extern FileManWork D_003DC658;


extern s32 fileIsRequestReadyInCurrentMode(FileRequest *file);

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

INCLUDE_ASM(const s32, "file/fileManager", func_00288818);

INCLUDE_ASM(const s32, "file/fileManager", func_002888C8);

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

extern void func_002EDBD8(void *, u32);
extern void func_002EDC40(void *);
extern void func_00288818(void *, s32, u32, u32, u32);

void *func_002889D8(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    void *work = sdfAllocAndClearQuadwords(0x70);
    void *data = (u8 *)work + 0x30;

    func_002EDBD8(data, arg2);
    if (arg1 != 0) {
        func_002EDC40(data);
    }
    func_00288818(work, 1, arg0, arg3, arg4);
    return work;
}

void func_00288A80(u32 request) {
    func_002889D8(request, 0, 0, 0, 0);
}

void func_00288AA8(u32 request) {
    func_002889D8(request, 1, 0, 0, 0);
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288AD0);

void func_00288B48(u32 request) {
    func_00288AD0(request, 0, 0, 0);
}

void func_00288B68(u32 request) {
    func_00288AD0(request, 1, 0, 0);
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

/* A request is ready once its state byte reaches 6, but only for mode 1. */
s32 fileIsRequestReadyInCurrentMode(FileRequest *file) {
    s32 result;

    if (file->mode == 1) {
        result = 0;
        if (file->stateRequired != 0) {
            result = file->state == 6;
        }
        return result;
    }
    return file->state == 6;
}

s32 fileRequestIsReady(FileRequest *file) {
    s32 result = 0;
    if (file->stateRequired != 0) {
        result = file->state == 6;
    }
    return result;
}

/* Keep the device scheduler and file manager running while a request finishes. */
void fileWaitReady(u32 request) {
    while (fileIsRequestReadyInCurrentMode(request) == 0) {
        sdfRestoreDeviceThreadPriority();
        fileManUpdate();
    }
}

void func_00288C50(u32 id) {
    fileWaitReady(id);
}


/* Spin until the file manager has no work left. */
void fileWaitIdle(void) {
    FileManWork *work = &D_003DC658;
    while (work->unk8 != 0 || work->unk18 != 0) {
        fileManUpdate();
    }
}

typedef struct FileWindowSlot {
    u8 pad00[0x10];
    s32 secondValueCopy; /* 0x10 */
    s32 secondValue;     /* 0x14 */
    u8 pad18[0xC];
    s32 firstValue;      /* 0x24 */
    s32 firstValueCopy;  /* 0x28 */
    u8 pad2C[4];
} FileWindowSlot; /* 0x30 */

FileWindowSlot *func_00288CB8(s32 id, s32 firstValue, s32 secondValue, s32 left, s32 right) {
    FileWindowSlot *slot = sdfAllocAndClearQuadwords(0x30);

    slot->firstValue = firstValue;
    slot->firstValueCopy = firstValue;
    slot->secondValue = secondValue;
    slot->secondValueCopy = secondValue;
    func_00288818(slot, 2, id, left, right);
    return slot;
}

void func_00288D48(a, b, c)
s32 a;
s32 b;
s32 c;
{
    func_00288CB8(a, b, c, 0, 0);
}

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void sdfDevQueueRead(u32 handle, u32 buffer, u32 size);

/* Claim the next of four read slots for a pending request and start its device read. */
void fileQueuePendingRequestInFreeSlot(FileRequest *request) {
    FileManWork *work = &D_003DC658;
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
