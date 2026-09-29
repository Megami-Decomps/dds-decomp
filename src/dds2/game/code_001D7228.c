#include "common.h"

typedef struct BattleVisualState {
    u8 pad00[0x110];
    u32 flags;
} BattleVisualState;

typedef struct BattleVisualObject {
    u8 pad00[0x18];
    BattleVisualState *visual;
} BattleVisualObject;

void func_001D7228(void) {
}

INCLUDE_ASM(const s32, "game/code_001D7228", func_001D7230);

void func_001D8C78(void) {
}

INCLUDE_ASM(const s32, "game/code_001D7228", func_001D8C80);

/* Four callbacks mark the linked visual state; bit 0x4000's meaning is unconfirmed. */
void func_001DA1F8(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

INCLUDE_ASM(const s32, "game/code_001D7228", func_001DA210);

void func_001DA728(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

INCLUDE_ASM(const s32, "game/code_001D7228", func_001DA740);

void func_001DABC0(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

INCLUDE_ASM(const s32, "game/code_001D7228", func_001DABD8);

void func_001DACE0(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

INCLUDE_RODATA(const s32, "game/code_001D7228", D_004174A8);

