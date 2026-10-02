#include "common.h"

extern u32 itfLoadTextureFromAsset(u32);

extern u32 kwlnTaskGetUserValue(void);

extern u32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234C18);

s32 func_003014F0(char *output, const char *format, ...);

s32 func_00234CA8(s32 event, s32 id, char *path1, char *path2, char *path3) {
    func_003014F0(path1, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM1",
                  event / 10 * 10, event, event, id, event, id);
    func_003014F0(path2, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2",
                  event / 10 * 10, event, event, id, event, id);
    return func_003014F0(path3, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM3",
                         event / 10 * 10, event, event, id, event, id);
}

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234DA8);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234F18);

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
    EventContext *context = (EventContext *)func_002CFEB8(8);
    context->flags = 0;
    context->value = 0;
    return context;
}

void evtSetConvertedContextValue(EventContext *context, u32 value) {
    u32 result;

    result = itfLoadTextureFromAsset(value);
    context->value = result;
}

INCLUDE_SDATA(const s32, "game/code_00234C18", D_003BBF78);

