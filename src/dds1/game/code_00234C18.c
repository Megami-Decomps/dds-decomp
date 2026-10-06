#include "common.h"
#include "kwln.h"
#include "sdf.h"
#include "evt_world.h"

extern SdfTex *itfLoadTextureFromAsset(const char *);

extern u32 kwlnTaskGetUserValue(KwlnTask *);

extern u32 sdfAllocSizeClassBlock(u32);

extern char D_003BBF78[];

/* Build the 0x20-byte PMD3 resource header, copy it into a fresh allocation and
 * hand the retained address back through `out`. */
s32 func_00234C18(u8 **out) {
    u8 buffer[0x20];
    s32 size = 0x20;
    s32 handle;
    u8 *address;

    memset(buffer, 0, size);
    memcpy(buffer + 8, D_003BBF78, 4);
    *(s32 *)(buffer + 0x14) = 9;
    handle = sdfAllocGeneralBlock(size);
    address = (u8 *)sdfResourceRetainAddress(handle);
    memcpy(address, buffer, size);
    *out = address;
    return handle;
}

s32 func_003014F0(char *output, const char *format, ...);

s32 evtFormatPolygonMoviePaths(s32 event, s32 id, char *path1, char *path2, char *path3) {
    func_003014F0(path1, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM1",
                  event / 10 * 10, event, event, id, event, id);
    func_003014F0(path2, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2",
                  event / 10 * 10, event, event, id, event, id);
    return func_003014F0(path3, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM3",
                         event / 10 * 10, event, event, id, event, id);
}

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234DA8);

typedef struct EvtViewerSlot {
    s32 request;  /* 0x0 */
    s32 resource; /* 0x4 */
    u32 *address; /* 0x8 */
} EvtViewerSlot;

typedef struct EvtViewerWork {
    u32 flags;             /* 0x000: 2/4/0x10 = slot request pending */
    EvtViewerSlot first;   /* 0x004 */
    u8 pad10[0x4C];
    EvtViewerSlot second;  /* 0x05C */
    EvtViewerSlot third;   /* 0x068 */
    u8 pad74[0x94];
    s32 task;              /* 0x108 */
    s32 event;             /* 0x10C */
    s32 id;                /* 0x110 */
} EvtViewerWork;

extern u32 D_003BA8EC;
extern s32 sdfAllocGeneralBlock(s32 size);
extern u32 *sdfResourceRetainAddress(s32 handle);
extern void *memset(void *dst, s32 value, u32 size);
extern void *evtPolygonMovieAllocWork(void);
extern s32 fileQueueDefaultCallbackRequest(char *path);
extern s32 sdfPathExists(char *path);
extern s32 kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern void evtViewerStartUpdate(void);
extern void func_00232D28(void);
extern void func_00232D48(void *viewer);
extern char D_003ADDB0[]; /* "(ZikkiPlayMode)EventViewer" */

/* Create the event viewer task `taskId` for event `event`/scene `id` and request its movie files. */
s32 evtViewerCreateTask(s32 taskId, s32 event, s32 id) {
    char path0[0x40];
    char path1[0x40];
    char path2[0x40];
    s32 viewerHandle;
    u32 *viewer;
    EvtViewerWork *work;
    s32 task;

    D_003BA8EC = 0x80000000;
    viewerHandle = sdfAllocGeneralBlock(0x2490);
    viewer = sdfResourceRetainAddress(viewerHandle);
    memset(viewer, 0, 0x2490);
    *viewer = viewerHandle;
    evtFormatPolygonMoviePaths(event, id, path0, path1, path2);
    work = evtPolygonMovieAllocWork();
    work->event = event;
    work->id = id;
    work->first.request = fileQueueDefaultCallbackRequest(path0);
    work->flags |= 2;
    work->second.request = fileQueueDefaultCallbackRequest(path1);
    work->flags |= 4;
    if (sdfPathExists(path2) != 0) {
        work->third.request = fileQueueDefaultCallbackRequest(path2);
        work->flags |= 0x10;
    } else {
        work->third.request = 0;
        work->third.address = 0;
    }
    task = kwlnTaskCreate(D_003ADDB0, taskId, 1, 1, evtViewerStartUpdate, func_00232D28, viewer);
    viewer[2] = (u32)work;
    work->task = task;
    func_00232D48(viewer);
    return task;
}


/* Flag operations act on the scheduler task's user-value context. */
void evtSetContextFlag(KwlnTask *task) {
    EvtPictureWork *context;

    context = (EvtPictureWork *)kwlnTaskGetUserValue(task);
    context->flags = context->flags | 1;
}

void evtClearContextFlag(KwlnTask *task) {
    EvtPictureWork *context;

    context = (EvtPictureWork *)kwlnTaskGetUserValue(task);
    context->flags = context->flags & 0xfffffffe;
}

void evtDestroyTaskHierarchy(u32 task) {
    kwlnTaskDestroyWithHierarchy(task, 1);
}

/* Allocate the picture task's flag and texture state. */
EvtPictureWork *evtAllocateContext(void) {
    EvtPictureWork *context = (EvtPictureWork *)sdfAllocSizeClassBlock(8);
    context->flags = 0;
    context->texture = NULL;
    return context;
}

void evtSetConvertedContextValue(EvtPictureWork *context, const char *path) {
    context->texture = itfLoadTextureFromAsset(path);
}

INCLUDE_RODATA(const s32, "game/code_00234C18", D_003ADDB0);

INCLUDE_SDATA(const s32, "game/code_00234C18", D_003BBF78);

