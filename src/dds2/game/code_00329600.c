#include "common.h"

extern u64 func_00329930(void);

typedef struct SdfAllocation {
    u8 pad00[8];
    u32 address;
    u8 pad0C[2];
    s16 referenceCount;
} SdfAllocation;

INCLUDE_ASM(const s32, "game/code_00329600", func_00329600);

void func_003297B0(u8 *node) {
    u8 *next = *(u8 **)(*(u8 **)(node + 4) + 4);
    *(u8 **)next = node;
    *(u8 **)(node + 4) = next;
}

INCLUDE_ASM(const s32, "game/code_00329600", func_003297C8);

void func_00329868(void) {
    u64 resource;

    resource = func_00329930();
    func_003297C8(resource);
}

void sdfReleaseMemorySlot(s32 *slot) {
    s32 resource;

    resource = *slot;
    if (resource != 0) {
        *slot = 0;
        func_003297C8(resource);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00329600", func_003298C0);

u32 sdfResourceRetainAddress(SdfAllocation *allocation) {
    allocation->referenceCount = allocation->referenceCount + 1;
    return allocation->address;
}
void func_00329910(u8 *work) {
    u16 value = *(u16 *)(work + 0xE);
    if (value != 0) {
        *(u16 *)(work + 0xE) = value - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00329600", func_00329930);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329A00);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329AA8);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329B18);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329C20);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329CE0);
