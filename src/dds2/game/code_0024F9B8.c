#include "common.h"

extern u32 kwlnTaskGetUserValue(void);

extern u32 itfLoadTextureFromAsset(u32);

extern u32 func_00328D68(u32);

typedef struct {
    u32 flags; /* 0x00: bit 0 toggled without disturbing the other bits */
    u32 value; /* 0x04 */
} EventContext;

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024F9B8);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FA48);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FB48);

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

extern u32 D_00435CBC;
extern s32 func_0024FA48(s32 event, s32 id, char *path1, char *path2, char *path3);
extern s32 func_003292A8(s32 size);
extern u32 *sdfResourceRetainAddress(s32 handle);
extern void *memset(void *dst, s32 value, u32 size);
extern EvtViewerWork *evtPolygonMovieAllocWork(void);
extern s32 func_002C80C8(char *path);
extern s32 sdfPathExists(char *path);
extern s32 kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern void evtViewerStartUpdate(void);
extern void func_0024DAC0(void);
extern void func_0024DAE0(void *viewer);
extern char D_00423360[]; /* "(ZikkiPlayMode)EventViewer" */

/* Create the event viewer task `taskId` for event `event`/scene `id` and request its movie files. */
s32 func_0024FCB8(s32 taskId, s32 event, s32 id) {
    char path0[0x40];
    char path1[0x40];
    char path2[0x40];
    s32 viewerHandle;
    u32 *viewer;
    EvtViewerWork *work;
    s32 task;

    D_00435CBC = 0x80000000;
    viewerHandle = func_003292A8(0x24BC);
    viewer = sdfResourceRetainAddress(viewerHandle);
    memset(viewer, 0, 0x24BC);
    *viewer = viewerHandle;
    func_0024FA48(event, id, path0, path1, path2);
    work = evtPolygonMovieAllocWork();
    work->event = event;
    work->id = id;
    work->first.request = func_002C80C8(path0);
    work->flags |= 2;
    work->second.request = func_002C80C8(path1);
    work->flags |= 4;
    if (sdfPathExists(path2) != 0) {
        work->third.request = func_002C80C8(path2);
        work->flags |= 0x10;
    } else {
        work->third.request = 0;
        work->third.address = 0;
    }
    task = kwlnTaskCreate(D_00423360, taskId, 1, 1, evtViewerStartUpdate, func_0024DAC0, viewer);
    viewer[2] = (u32)work;
    work->task = task;
    func_0024DAE0(viewer);
    return task;
}

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
    EventContext *context = (EventContext *)func_00328D68(8);
    context->flags = 0;
    context->value = 0;
    return context;
}

void evtSetConvertedContextValue(EventContext *context, u32 value) {
    u32 result;

    result = itfLoadTextureFromAsset(value);
    context->value = result;
}

INCLUDE_RODATA(const s32, "game/code_0024F9B8", D_00423360);

INCLUDE_SDATA(const s32, "game/code_0024F9B8", D_004373B8);

