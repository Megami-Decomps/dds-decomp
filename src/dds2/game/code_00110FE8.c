#include "common.h"

typedef struct WorldObjectPointer {
    u8 pad00[0x18];
    u32 *value;
} WorldObjectPointer;

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00110FE8);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111050);

void dds3SetWorldObjectValue(WorldObjectPointer *object, u32 value) {
    if (object != NULL) {
        *object->value = value;
    }
}

s32 dds3GetWorldObjectValue(WorldObjectPointer *object) {
    if (object == NULL) {
        return -1;
    }
    return *object->value;
}

INCLUDE_ASM(const s32, "game/code_00110FE8", func_001110F8);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_001111A8);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111218);

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111278);

u32 func_001112F8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00110FE8", func_00111300);
