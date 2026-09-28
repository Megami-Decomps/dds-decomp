#include "common.h"

extern u32 func_001986E0(u32);

extern u32 func_00101A70(void);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234C18);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234CA8);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234DA8);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234F18);

typedef struct {
    u32 flags; /* 0x00 */
    u32 value; /* 0x04 */
} EventContext;

void func_00235088(void) {
    EventContext *context;

    context = (EventContext *)func_00101A70();
    context->flags = context->flags | 1;
}

void func_002350B0(void) {
    EventContext *context;

    context = (EventContext *)func_00101A70();
    context->flags = context->flags & 0xfffffffe;
}

void func_002350E0(u32 arg0) {
    kwlnTaskDestroyWithHierarchy(arg0, 1);
}

INCLUDE_ASM(const s32, "game/code_00234C18", func_002350F8);

void func_00235120(EventContext *context, u32 value) {
    u32 result;

    result = func_001986E0(value);
    context->value = result;
}

INCLUDE_SDATA(const s32, "game/code_00234C18", D_003BBF78);

