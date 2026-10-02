#include "common.h"

extern u32 itfLoadTextureFromAsset(u32);

extern u32 kwlnTaskGetUserValue(void);

extern u32 sdfAllocSizeClassBlock(u32);

typedef struct EvtMovieResourceSpec {
    u32 unk00;
    u32 unk04;
    char format[4];
    u32 unk0C;
    u32 unk10;
    s32 type;
    u32 unk18;
    u32 unk1C;
} EvtMovieResourceSpec;

extern char D_003BBF78[];
extern s32 sdfAllocGeneralBlock(s32 size);
extern u32 *sdfResourceRetainAddress(s32 handle);
extern void *memset(void *dst, s32 value, u32 size);
extern void *copyResourceBytes(void *dst, const void *src, u32 size) __asm__("memcpy");

s32 func_00234C18(void **address) {
    EvtMovieResourceSpec spec;
    s32 handle;
    u32 *resource;

    memset(&spec, 0, sizeof(spec));
    memcpy(spec.format, D_003BBF78, sizeof(spec.format));
    spec.type = 9;
    handle = sdfAllocGeneralBlock(sizeof(spec));
    resource = sdfResourceRetainAddress(handle);
    copyResourceBytes(resource, &spec, sizeof(spec));
    *address = resource;
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
extern EvtViewerWork *evtPolygonMovieAllocWork(void);
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

typedef struct {
    u32 flags; /* 0x00: bit 0 toggled without disturbing the other bits */
    u32 value; /* 0x04 */
} EventContext;

void evtSetContextFlag(void) {
    EventContext *context;

    context = (EventContext *)kwlnTaskGetUserValue();
    context->flags = context->flags | 1;
}

void evtClearContextFlag(void) {
    EventContext *context;

    context = (EventContext *)kwlnTaskGetUserValue();
    context->flags = context->flags & 0xfffffffe;
}

void evtDestroyTaskHierarchy(u32 task) {
    kwlnTaskDestroyWithHierarchy(task, 1);
}

/* The owner argument is part of the allocator callback signature; the
 * returned context starts with both words clear. */
EventContext *evtAllocateContext(s32 *owner) {
    EventContext *context = (EventContext *)sdfAllocSizeClassBlock(8);
    context->flags = 0;
    context->value = 0;
    return context;
}

void evtSetConvertedContextValue(EventContext *context, u32 value) {
    u32 result;

    result = itfLoadTextureFromAsset(value);
    context->value = result;
}

INCLUDE_RODATA(const s32, "game/code_00234C18", D_003ADDB0);

INCLUDE_SDATA(const s32, "game/code_00234C18", D_003BBF78);

