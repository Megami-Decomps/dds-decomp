#include "common.h"

extern s32 func_00101958(void);

typedef struct ScriptCommandBuffer {
    u8 pad0[0x18];
    s32 pc; /* 0x18 */
    s32 count; /* 0x1C */
    u8 kinds[0x1C];
    u32 values[0x20];
    u32 *instructions; /* 0xBC */
    u8 padC0[0x30];
    void *actor; /* 0xF0: battle actor bound to the active script */
} ScriptCommandBuffer;

void func_0010C250(u32 unused, u32 actor) {
    ScriptCommandBuffer *context;

    context = (ScriptCommandBuffer *)func_00101958();
    context->actor = (void *)actor;
}

u32 func_0010C278(void) {
    ScriptCommandBuffer *context;

    context = (ScriptCommandBuffer *)func_00101958();
    return (u32)context->actor;
}

void func_0010C298(u32 task) {
    s64 context;

    context = func_00101958();
    if (context != 0) {
        func_0010BF48(context);
    }
    func_00101950(task, 0);
}

void func_0010C2D8(void) {
    func_0010D5A8();
}

INCLUDE_ASM(const s32, "game/code_0010C250", func_0010C2F0);

void scrPushInteger(ScriptCommandBuffer *buffer, u32 value) {
    buffer->kinds[buffer->count] = 0;
    buffer->values[buffer->count] = value;
    buffer->count = buffer->count + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C250", func_0010C378);

void scrPushString(ScriptCommandBuffer *buffer, u32 value) {
    buffer->kinds[buffer->count] = 5;
    buffer->values[buffer->count] = value;
    buffer->count = buffer->count + 1;
}

void scrPushTypeFourValue(ScriptCommandBuffer *buffer, u32 value) {
    buffer->kinds[buffer->count] = 4;
    buffer->values[buffer->count] = value;
    buffer->count = buffer->count + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C250", func_0010C408);

INCLUDE_ASM(const s32, "game/code_0010C250", func_0010C4E0);

u32 scrPushNextInstructionValue(ScriptCommandBuffer *script) {
    s32 nextPc;
    ScriptCommandBuffer *context;

    context = script;
    nextPc = context->pc + 1;
    context->pc = nextPc;
    scrPushInteger(script, context->instructions[nextPc]);
    context->pc = context->pc + 1;
    return 1;
}
