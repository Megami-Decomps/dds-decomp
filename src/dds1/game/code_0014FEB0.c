#include "common.h"

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_0014FEB0);

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_0014FF28);

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_00150040);

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_001500F0);
INCLUDE_ASM(const s32, "game/code_0014FEB0", func_00150148);

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
void func_00150260(TexRecord *entry) {
    entry->refCount--;
    if (entry->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(entry->texture);
        func_002D0918(entry->allocation);
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

