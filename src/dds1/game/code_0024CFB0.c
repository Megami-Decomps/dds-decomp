#include "common.h"

extern s32 D_003BC410;

extern s32 D_003BC408;

extern u8 D_003BC40C;

extern s8 D_003BC40D;

extern s8 D_003BC414;

extern s8 D_003BC415;

extern s32 D_003BAA00;

typedef struct EvtActiveFlagTable {
    s32 unk0;
    s32 unk4;
    s8 flags[0];
} EvtActiveFlagTable;

typedef struct {
    u8 count;
    u8 pad;
    u16 indices[0];
} ActiveList;

extern EvtActiveFlagTable D_003BD8A0;

extern u32 func_002EB028(u32, u32 *, u32);

extern s64 func_0024DC08(void);

extern s32 func_00101A70();

extern u32 D_003D8100[];

extern void func_0024A2D8(s32 arg0);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern void func_0024DD78(void);

void func_0024CFB0(s32 arg0) {
    func_0024DBC8();
    func_0024D9D8(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024CFD8);

s32 func_0024D220(void) {
    s32 *state = (s32 *)func_00101A70();

    func_00249DD0(state);
    func_0024B358(0, state);
    return 1;
}

u32 func_0024D260(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D268);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

s64 func_0024D300(s32 request) {
    s32 state = func_00101A70();
    func_0024A2D8(state);
    return menuRunPanel(state, 1, request);
}

s64 func_0024D350(s32 request) {
    s32 state = func_00101A70();
    func_0024DD78();
    return menuRunPanel(state, 2, request);
}

u32 func_0024D398(void) {
    s32 state;

    state = func_00101A70();
    if (*(s32 *)(state + 0xdc) == 0) {
        evtClearActiveFlag();
        func_0024DEF8(0, 1);
        func_0024DEF8(1, 0);
    }
    else {
        kwlnFadeInStart(0, 0, 0, 0xf);
    }
    return 1;
}

s32 func_0024D400(void) {
    s32 state = func_00101A70();

    func_0024B358(1, state);
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D440);

s64 func_0024D500(s32 request) {
    s32 state = func_00101A70();
    func_0024A2D8(state);
    return menuRunPanel(state, 1, request);
}

s64 func_0024D550(s32 request) {
    s32 context = func_00101A70();

    return menuRunPanel(context, 2, request);
}

u32 func_0024D588(void) {
    s32 state;

    state = func_00101A70();
    *(u32 *)(state + 0x90) = 0;
    return 1;
}

s32 func_0024D5B0(void) {
    s32 *state = (s32 *)func_00101A70();

    mnuReleaseWorkResources(state);
    func_00249498(state);
    return 1;
}

u32 func_0024D5F0(void) {
    return 0;
}

u32 func_0024D5F8(void) {
    return 0;
}

u32 func_0024D600(void) {
    return 0;
}

u32 func_0024D608(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D610);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D670);

void func_0024D6E0(void) {
    kwlnDrawSetOffsetTransition(0, 0, 0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    func_0018F3B0();
    func_0018F438();
    func_0018F750();
    func_0018F4F0();
    func_0018F6E8();
}

void func_0024D738(void) {
    evtCommandShutdownStage();
    func_0024D6E0();
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D758);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D778);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D7B8);

void evtCollectActiveGameIndices(ActiveList *list) {
    s32 i;
    list->count = 0;
    for (i = 1; i < 0xC0; i++) {
        if (*(u8 *)(i + D_003BAA00 + 0x12A0) != 0) {
            s32 count = list->count++;
            list->indices[count] = i;
        }
    }
}

s32 evtCompareByteValues(u8 *left, u8 *right) {
    u8 leftValue = *left;
    u8 rightValue = *right;

    if (rightValue < leftValue) {
        return 1;
    }
    return (leftValue < rightValue) ? -1 : 0;
}

s32 evtCompactFilteredBytes(u8 *buffer, s32 length, u8 excluded) {
    s32 i;
    s32 count = 0;
    for (i = 0; i < length; i++) {
        if (buffer[i] != excluded) {
            u8 value = buffer[i];
            buffer[i] = 0;
            buffer[count++] = value;
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D8F8);

void evtLoadResourcePair(u32 resourceId, u32 *record) {
    u32 handle;

    handle = func_002EB028(resourceId, record + 1, 0);
    *record = handle;
}

void evtReleaseResourcePairHandle(u32 *record) {
    func_002D0918(*record);
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D9D8);

s32 func_0024DA20(s32 arg0) {
    if (D_003BC408 < 0) {
        return 0;
    }
    func_0019C968(D_003BC408, 0, arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DA58);

s32 func_0024DAB8(s32 arg0) {
    if (D_003BC408 < 0) {
        return 0;
    }
    D_003BC410 = arg0;
    D_003BC415 = sndGetActiveMode();
    return 1;
}

void func_0024DAE8(s32 arg0) {
    if (D_003BC408 >= 0) {
        D_003BC414 = arg0;
    }
}

s8 func_0024DB00(void) {
    return D_003BC414;
}

s32 sndGetActiveMode(void) {
    if (D_003BC408 < 0) {
        return -1;
    }
    return itfPanelGetPairSecond(D_003BC408);
}

s8 func_0024DB40(void) {
    return D_003BC415;
}

u32 func_0024DB48(s32 notify) {
    u32 result;

    result = 0;
    if (-1 < D_003BC408) {
        itfPanelSetStatus(D_003BC408, 0);
        if (notify != 0) {
            func_0019B4A0(D_003BC408);
        }
        itfMesCleanupWindow(D_003BC408, 0);
        func_0024DDC0(1);
        D_003BC40C = 0;
        result = 1;
    }
    return result;
}

void func_0024DBB0(void) {
    func_0024DB48(1);
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DBC8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DC08);

s32 sndUpdateActiveMode(void) {
    if (D_003BC408 < 0) {
        return 0;
    }
    if (itfPanelGetPairFirst(D_003BC408) < 0) {
        return 0;
    }
    D_003BC415 = sndGetActiveMode();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DC98);

void func_0024DD78(void) {
    func_0024DC98(1);
}

void func_0024DD90(s32 arg0, s32 arg1) {
    func_0019C838(D_003BC408, arg0, arg1);
}

s8 func_0024DDB8(void) {
    return D_003BC40D;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DDC0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DE30);

s32 evtIsTaskInActiveStates(s32 task) {
    if (kwlnTaskGetRegisteredState(task) == 1) {
        return 1;
    }
    if (kwlnTaskGetRegisteredState(task) == 2) {
        return 1;
    }
    return kwlnTaskGetRegisteredState(task) == 3;
}

void evtClearActiveFlag(s32 flagIndex) {
    D_003BD8A0.flags[flagIndex] = 0;
}

s32 evtIsActiveFlagSet(s32 flagIndex) {
    return D_003BD8A0.flags[flagIndex] != 0;
}

s32 func_0024DEF8(s32 index, s32 value) {
    if (index >= 0x10) {
        return 0;
    }
    D_003D8100[index] = value;
    return 1;
}

u32 func_0024DF20(s32 index) {
    index = (index < 0x10) ? index : 0xf;
    return D_003D8100[index];
}

s32 evtSetCurrentActiveFlag(void) {
    s32 flagIndex = func_0010D428(0);

    D_003BD8A0.flags[flagIndex] = 1;
    return 1;
}

s32 evtActivateCurrentFlag(void) {
    s32 index = func_0010D428(0);
    if (index >= 16) {
        index = 15;
    }
    func_0010D5F0(D_003D8100[index]);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DFC0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E010);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E100);

void func_0024E198(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_002C0DD8(arg0, arg1, 0, arg2, arg3, 0x30303040, 0x53);
}

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC408);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC40C);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC40D);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC410);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC414);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC415);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC418);

