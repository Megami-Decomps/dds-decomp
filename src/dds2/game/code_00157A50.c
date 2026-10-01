#include "common.h"

INCLUDE_ASM(const s32, "game/code_00157A50", func_00157A50);

INCLUDE_ASM(const s32, "game/code_00157A50", func_00157AC8);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414148);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414160);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414178);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414190);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141A8);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141C0);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141D8);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141F0);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414208);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414220);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414238);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414250);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414268);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414280);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414298);

INCLUDE_ASM(const s32, "game/code_00157A50", func_00157BE0);

INCLUDE_ASM(const s32, "game/code_00157A50", func_00157CE0);

INCLUDE_ASM(const s32, "game/code_00157A50", func_00157D38);

void func_00157E50(void *arg) {
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
        func_003297C8(entry->field_0x44);
    }
}
