#include "common.h"

/* The object owns a pointer to its current world value, not the value itself. */
typedef struct WorldObjectPointer {
    u8 pad00[0x18];
    u32 *value;
} WorldObjectPointer;

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110DC0);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110E28);

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

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110ED0);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110F80);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110FF0);

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00111050);

u32 func_001110D0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00110DC0", func_001110D8);
