#include "common.h"

extern u32 func_00101958(void);

extern u32 func_001A0710(u32);

extern u32 func_00328D68(u32);

typedef struct {
    u32 flags; /* 0x00 */
    u32 value; /* 0x04 */
} EventContext;

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024F9B8);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FA48);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FB48);

INCLUDE_ASM(const s32, "game/code_0024F9B8", func_0024FCB8);

void func_0024FE28(void) {
    EventContext *context;

    context = (EventContext *)func_00101958();
    context->flags = context->flags | 1;
}

void func_0024FE50(void) {
    EventContext *context;

    context = (EventContext *)func_00101958();
    context->flags = context->flags & 0xfffffffe;
}

void func_0024FE80(u32 task) {
    kwlnTaskDestroyWithHierarchy(task, 1);
}

EventContext *func_0024FE98(s32 *owner) {
    EventContext *work = (EventContext *)func_00328D68(8);
    work->flags = 0;
    work->value = 0;
    return work;
}

void func_0024FEC0(EventContext *context, u32 value) {
    u32 result;

    result = func_001A0710(value);
    context->value = result;
}

INCLUDE_SDATA(const s32, "game/code_0024F9B8", D_004373B8);

