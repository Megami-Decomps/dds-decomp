#include "common.h"

typedef struct MenuSearchState {
    u8 pad0[5];
    s8 selectedIndex;
    s16 targetId;
} MenuSearchState;

typedef struct MenuSearchNode {
    u8 pad0[0x58];
    struct MenuSearchNode *next;
    u8 pad5C[0x14];
    u8 *value;
} MenuSearchNode;

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E350);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E568);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E638);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427218);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427258);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E858);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EB38);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427278);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004272C8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EF50);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F128);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004272F8);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427330);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427340);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427360);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F570);

s32 mnuSelectMatchingNode(s32 object, MenuSearchState *state) {
    MenuSearchNode *current = *(MenuSearchNode **)(*(s32 *)(object + 4) + 0x10);
    s32 index = 0;
    while (current != 0) {
        if (*(u16 *)(current->value + 4) == state->targetId) {
            state->selectedIndex = index;
            return 0;
        }
        current = current->next;
        index++;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427428);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F7B8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F8A8);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427488);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F9A0);

u32 func_0028FD08(void) {
    return 0;
}

void func_0028FD10(u32 arg0) {
    func_0028FD30(arg0, 0xffffffffffffffff, 0xffffffffffffffff);
}

