#include "common.h"

extern u32 itfLoadTextureFromAsset(u32);

extern u32 kwlnTaskGetUserValue(void);

extern u32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234C18);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234CA8);

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

