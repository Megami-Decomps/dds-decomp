#include "common.h"

extern s32 kwlnFadeIsActive(void);

extern s32 func_0024A6C0(s32);

extern s8 D_003BC3E0;

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern const char D_003AF658[];

extern const char D_003AF668[];

extern const char D_003AF678[];

extern s32 D_003BC3E4;

INCLUDE_ASM(const s32, "game/code_00249FA8", func_00249FA8);

void func_0024A058(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003AF658, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF668, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF678, 0);
    D_003BC3E4 = 0;
}

s32 fldPollSceneState(void) {
    s32 state = D_003BC3E0;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC3E0 = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A0D8);

void func_0024A138(s32 value) {
    s32 context = func_00101A70();

    func_00285670(context + 8, context + 0x54, 1, value);
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A170);

s32 func_0024A1A8(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return func_0024DC08() == 0;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A1D8);

typedef struct {
    u8 pad00[6];
    u16 previousA; /* 0x06 */
    u16 currentA;  /* 0x08 */
    u16 previousB; /* 0x0A */
    u16 currentB;  /* 0x0C */
    u16 flags;     /* 0x0E */
} SceneOptionRecord;

void func_0024A2B8(SceneOptionRecord *option) {
    u16 flags = option->flags;
    u16 currentA = option->currentA;
    u16 currentB = option->currentB;
    u16 retainedFlags = flags & 0xfa2f;

    option->previousA = currentA;
    option->previousB = currentB;
    option->flags = retainedFlags;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A2D8);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A340);

s32 fldClassifyRemainingFrames(s32 timer) {
    s32 frames = *(s32 *)(timer + 0x9C);
    if (frames == 0) {
        return 0;
    }
    return frames >= 60 ? 2 : 1;
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A4A0);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A570);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A610);

extern s32 func_0024A6C0(s32);

s32 func_0024A6C0(s32 object) {
    switch (*(s32 *)(object + 0xdc)) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

typedef struct {
    u8 pad00[0x14];
    u8 unk14;
    u8 pad15[0x8B];
} SceneFrameRecord;

typedef struct {
    u8 pad00[0x18];
    SceneFrameRecord *records;
} SceneFrameTable;

typedef struct {
    u8 pad00[0x64];
    SceneFrameTable *frameTable;
} SceneFrameOwner;

u8 func_0024A6E8(SceneFrameOwner *scene) {
    s32 index;

    index = func_0024A6C0((s32)scene);
    return scene->frameTable->records[index].unk14;
}

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF658);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF668);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF678);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF688);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF6A0);

INCLUDE_SDATA(const s32, "game/code_00249FA8", D_003BC400);
