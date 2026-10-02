#include "common.h"

typedef struct EffectSource {
    u32 type;
    u32 field4;
    u32 argument;
} EffectSource;

typedef struct EffectHandler {
    u32 (*handler)(u32);
    u8 pad04[0x2C];
} EffectHandler;

extern EffectHandler D_0034DE40[];
extern void *func_002CFEB8(s32);

void *effCloneSourceWithTypeHandler(EffectSource *source) {
    EffectSource *copy = (EffectSource *)func_002CFEB8(0x10);
    u32 argument = source->argument;

    copy->type = source->type;
    copy->field4 = source->field4;
    copy->argument = D_0034DE40[source->type].handler(argument);
    return copy;
}

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_0014FF28);

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_00150040);

extern s32 D_003BD7F4;
extern u32 effBillResourceOwners[];
extern void billDispatchByKind(u32);

void effBillDispatchAll(void) {
    u32 i;

    D_003BD7F4 = 0;
    for (i = 0; i < 15; i++) {
        billDispatchByKind(effBillResourceOwners[i]);
    }
}

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
void effReleaseSharedTextureRecord(TexRecord *entry) {
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

