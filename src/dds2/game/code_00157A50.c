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

/* Texture record: +0x00 is the handle the reference is dropped from, +0x08 the
 * reference count and +0x44 the allocation released when it reaches zero. */
typedef struct TexRecord {
    void *texture;     /* 0x00 */
    u32 flags;         /* 0x04 */
    s32 refCount;      /* 0x08 */
    u8 pad0C[0x38];
    void *allocation;  /* 0x44 */
} TexRecord;

/* Drop one reference; the last one releases the texture and its allocation. */
void func_00157E50(TexRecord *entry) {
    entry->refCount--;
    if (entry->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(entry->texture);
        func_003297C8(entry->allocation);
    }
}
