#include "common.h"

extern s32 func_00101958(void);

typedef struct ScriptCommandBuffer {
    u8 pad0[0x1C];
    s32 count;
    u8 kinds[0x1C];
    u32 values[0x20];
} ScriptCommandBuffer;

void func_0010C250(u32 unused, u32 value) {
    s32 context;

    context = func_00101958();
    *(u32 *)(context + 0xf0) = value;
}

u32 func_0010C278(void) {
    s32 context;

    context = func_00101958();
    return *(u32 *)(context + 0xf0);
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

u32 scrPushNextInstructionValue(u32 address) {
    s32 instruction;
    s32 context;

    context = (s32)address;
    instruction = *(s32 *)(context + 0x18) + 1;
    *(s32 *)(context + 0x18) = instruction;
    scrPushInteger(address, *(u32 *)(instruction * 4 + *(s32 *)(context + 0xbc)));
    *(s32 *)(context + 0x18) = *(s32 *)(context + 0x18) + 1;
    return 1;
}
