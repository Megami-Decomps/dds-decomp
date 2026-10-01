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

extern EffectHandler D_003AA770[];
extern void *func_00328D68(s32);

void *effCloneSourceWithTypeHandler(EffectSource *source) {
    EffectSource *copy = (EffectSource *)func_00328D68(0x10);
    u32 argument = source->argument;

    copy->type = source->type;
    copy->field4 = source->field4;
    copy->argument = D_003AA770[source->type].handler(argument);
    return copy;
}

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

extern s32 D_00438EFC;
extern u32 D_00451EE0[];
extern void billDispatchByKind(u32);

void effBillDispatchAll(void) {
    u32 i;

    D_00438EFC = 0;
    for (i = 0; i < 15; i++) {
        billDispatchByKind(D_00451EE0[i]);
    }
}

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
void effReleaseSharedTextureRecord(TexRecord *entry) {
    entry->refCount--;
    if (entry->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(entry->texture);
        func_003297C8(entry->allocation);
    }
}
