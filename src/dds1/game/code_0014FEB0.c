#include "common.h"

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_0014FEB0);

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_0014FF28);

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_00150040);

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_001500F0);
INCLUDE_ASM(const s32, "game/code_0014FEB0", func_00150148);

void func_00150260(void *arg) {
    struct {
        void *field_0x00;
        u32 field_0x04;
        s32 field_0x08;
        u8 pad_0x0C[0x38];
        void *field_0x44;
    } *entry = arg;

    entry->field_0x08--;
    if (entry->field_0x08 == 0) {
        sdfTexReleaseReferenceViaHandler(entry->field_0x00);
        func_002D0918(entry->field_0x44);
    }
}

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AA8);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AC0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AD8);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AF0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B08);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B20);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B38);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B50);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B68);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B80);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B98);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BB0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BC8);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BE0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BF8);

