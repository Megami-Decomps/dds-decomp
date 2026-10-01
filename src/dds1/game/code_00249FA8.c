#include "common.h"

extern s32 kwlnFadeIsActive(void);

extern s8 D_003BC3E0;

extern s64 evtGetMessageWindowControlState(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 kwlnTaskGetUserValue();

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern const char D_003AF658[];

extern const char D_003AF668[];

extern const char D_003AF678[];

extern s32 D_003BC3E4;

typedef struct MenuSlotState {
    u8 pad00[0x64];
    s32 batch;     /* 0x64 */
    u8 pad68[0x40];
    s32 effect[5]; /* 0xA8: pairs at +0x08/+0x0C select the two effect slots */
    s32 cur;       /* 0xBC */
    s32 prev;      /* 0xC0 */
    u8 padC4[0x20];
    s32 mode;      /* 0xE4 */
} MenuSlotState;

extern void effConfigureWithDefaultSetting(s32, s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_00249FA8);

void fldStopSceneTasks(void) {
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

extern char D_0036ADF4[];

extern void mnuSetPopupEntry(s32 *, char *);

void mnuPreparePopupAndDispatchSelection(s32 value) {
    s32 context = kwlnTaskGetUserValue();
    s32 *state = (s32 *)(context + 0x54);

    mnuSetPopupEntry(state, D_0036ADF4);
    func_00285670(context + 8, state, 0, value);
}

void func_0024A138(s32 value) {
    s32 context = kwlnTaskGetUserValue();

    func_00285670(context + 8, context + 0x54, 1, value);
}

void func_0024A170(s32 value) {
    s32 context = kwlnTaskGetUserValue();

    func_00285670(context + 8, context + 0x54, 2, value);
}

s32 evtIsFadeCompleteAndMessageWindowIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
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

void fldSaveSceneOptionsAndClearFlags(SceneOptionRecord *option) {
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

/* View of the frame countdown; the preceding scene state is not known here. */
typedef struct {
    u8 pad00[0x9C];
    s32 remainingFrames; /* 0x9C */
} SceneTimerView;

s32 fldClassifyRemainingFrames(SceneTimerView *timer) {
    s32 frames = timer->remainingFrames;
    if (frames == 0) {
        return 0;
    }
    return frames >= 60 ? 2 : 1;
}

void mnuTerminalConfigureEffects(u32 mode, MenuSlotState *state) {
    s32 *slot = &state->cur;

    if (*slot < 0) {
        return;
    }
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[2], 0, 5, 2);
        break;
    case 2:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[3], 0, 0, 2);
        break;
    case 3:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[2], 0, 0, 2);
        if (slot[1] >= 0) {
            effConfigureWithDefaultSetting(state->batch, slot[1], state->effect[3], 0, 0, 2);
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A570);

INCLUDE_ASM(const s32, "game/code_00249FA8", func_0024A610);

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
    SceneFrameTable *frameTable; /* 0x64 */
    u8 pad68[0x74];
    s32 mode; /* 0xDC */
} SceneFrameOwner;

extern s32 fldGetModeFrameRecordIndex(SceneFrameOwner *);

/* Scene modes 1 and 2 select different entries from the same frame table. */
s32 fldGetModeFrameRecordIndex(SceneFrameOwner *scene) {
    switch (scene->mode) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_0024A6E8(SceneFrameOwner *scene) {
    s32 index;

    index = fldGetModeFrameRecordIndex(scene);
    return scene->frameTable->records[index].unk14;
}

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF658);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF668);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF678);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF688);

INCLUDE_RODATA(const s32, "game/code_00249FA8", D_003AF6A0);

INCLUDE_SDATA(const s32, "game/code_00249FA8", D_003BC400);

