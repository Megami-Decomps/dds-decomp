#include "common.h"

extern u64 func_002D0A80(void);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0750);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0900);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0918);

void func_002D09B8(void) {
    u64 handle;

    handle = func_002D0A80();
    func_002D0918(handle);
}

void sdfReleaseMemorySlot(s32 *slot) {
    s32 handle;

    handle = *slot;
    if (handle != 0) {
        *slot = 0;
        func_002D0918(handle);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0A10);

typedef struct {
    u8 pad00[8];
    u32 address;     /* 0x08 */
    u8 pad0C[2];
    s16 referenceCount; /* 0x0E */
} SdfResource;

u32 sdfResourceRetainAddress(SdfResource *resource) {
    resource->referenceCount = resource->referenceCount + 1;
    return resource->address;
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0A60);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0A80);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0B50);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0BF8);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0C68);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0D70);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0E30);
