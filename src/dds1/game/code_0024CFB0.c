#include "mnu.h"

extern s32 D_003BC410;

extern s32 dspWindowHandle;

extern u8 dspWindowControlState;

extern s8 dspWindowStateGate;

extern s8 D_003BC414;

extern s8 dspCapturedSoundMode;

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

extern EvtActiveFlagTable evtActiveEntryFlags;

extern u32 func_002EB028(u32, u32 *, u32 *);

extern s32 kwlnTaskGetUserValue();

extern u32 effMiscRand(void *);

extern u32 evtDisplayValues[];

extern void func_0024A2D8(s32 arg0);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern void func_0024DD78(void);
extern void func_0024DD90(s32, s32);
extern void dspSetActive();
extern void itfMesSetWindowHighFlags(s32, s32);
extern void itfMesClearWindowHighFlags(s32, s32);
extern void itfPanelSetStatus(s32, s32);
extern void itfPanelSetPairFirst(s32, s32);
extern void itfMesStartEntry(s32, s32, s32);
extern void mnuSetPopupEntryFlagged(s32 *, char *);
extern char D_0036ACF8[];
extern void itfMesDestroyWindowIfPresent(s32);
extern s32 mdlFlagTest(s32);
extern void mdlFlagSet(s32);
extern s32 D_003BAA70;
extern s32 D_003BAA74;
extern s32 D_003BAA78;
extern s32 D_003BAA84;

typedef struct SceneFlagEntry {
    s32 needFlag;    /* 0x00 */
    s32 doneFlag;    /* 0x04 */
    u16 areaIndex;   /* 0x08 */
    u8 nameIndex;    /* 0x0A */
    u8 pad0B;
    u16 dialogIndex; /* 0x0C */
    u16 pad0E;
} SceneFlagEntry;

typedef struct PartyFlagPair {
    s32 needFlag;
    s32 doneFlag;
} PartyFlagPair;

typedef struct PartySlotHeader {
    u16 flags;
    u16 pad02;
    u16 id;
} PartySlotHeader;

extern SceneFlagEntry D_0036ABB8[4];
extern PartyFlagPair D_0036ABF8[];

void evtCloseDisplayChannelAndEnsureMessageWindow(s32 arg0) {
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(*(u32 *)(arg0 + 0x60));
}

s32 dspStartFlagEvent(s32 context) {
    s32 kind = *(s32 *)(context + 0x7C);
    s32 i;
    PartySlotHeader *slot;

    if (kind < 2) {
        if (kind >= 0) {
            if (mdlFlagTest(0x902) != 0 && mdlFlagTest(0x907) == 0) {
                dspSetActive(1);
                dspStartEntry(5);
                mdlFlagSet(0x907);
                return 1;
            }
            for (i = 0; i < sizeof(D_0036ABB8) / sizeof(D_0036ABB8[0]); i++) {
                if (mdlFlagTest(D_0036ABB8[i].needFlag) != 0 && mdlFlagTest(D_0036ABB8[i].doneFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(context);
                    dspSetActive(1);
                    func_0024DD90(0, D_003BAA84 + D_0036ABB8[i].areaIndex * 0x19);
                    func_0024DD90(1, D_003BAA78 + D_0036ABB8[i].nameIndex * 0x13);
                    func_0024DD90(2, D_003BAA74 + D_0036ABB8[i].dialogIndex * 0x11);
                    dspStartEntry(3);
                    mdlFlagSet(D_0036ABB8[i].doneFlag);
                    return 1;
                }
            }
            for (i = 0; i < 5; i++) {
                slot = (PartySlotHeader *)(D_003BAA00 + i * 0x1A4 + 0xA60);
                if ((slot->flags & 1) != 0 && mdlFlagTest(D_0036ABF8[slot->id].needFlag) != 0
                    && mdlFlagTest(D_0036ABF8[slot->id].doneFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(context);
                    dspSetActive(1);
                    func_0024DD90(0, D_003BAA70 + slot->id * 0x11);
                    dspStartEntry(4);
                    mdlFlagSet(D_0036ABF8[slot->id].doneFlag);
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 mnuPrepareTerminalPanelState(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();

    func_00249DD0(state);
    mnuApplyFadeTrackMode(0, state);
    return 1;
}

u32 func_0024D260(void) {
    return 1;
}

s64 dspUpdateFlagEvent(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panel = (s32 *)(state + 0x54);
    s64 result = func_00285670(state + 8, panel, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panel == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (dspStartFlagEvent(state) == 0) {
                mnuSetPopupEntryFlagged(panel, D_0036ACF8);
            }
        }
    }
    return 0;
}

s64 mnuStartPanelDispatch(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    return menuRunPanel(state, 1, request);
}

s64 mnuStartPanelExit(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    func_0024DD78();
    return menuRunPanel(state, 2, request);
}

u32 mnuApplyTerminalExitFadeOrReset(void) {
    s32 state;

    state = kwlnTaskGetUserValue();
    if (*(s32 *)(state + 0xdc) == 0) {
        evtClearActiveFlag();
        evtSetBoundedDisplayValue(0, 1);
        evtSetBoundedDisplayValue(1, 0);
    }
    else {
        kwlnFadeInStart(0, 0, 0, 0xf);
    }
    return 1;
}

s32 mnuStartTerminalPanelFadeOut(void) {
    s32 state = kwlnTaskGetUserValue();

    mnuApplyFadeTrackMode(1, state);
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D440);

s64 func_0024D500(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    return menuRunPanel(state, 1, request);
}

s64 func_0024D550(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, request);
}

u32 mnuClearTerminalPanelSelection(void) {
    s32 state;

    state = kwlnTaskGetUserValue();
    *(u32 *)(state + 0x90) = 0;
    return 1;
}

s32 mnuRebuildTerminalMenuResources(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();

    mnuReleaseWorkResources(state);
    mnuTerminalBuildMenus(state);
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

s32 evtCopyWorldObjectEntryValue(s32 id, s32 dst) {
    s32 src = func_00110ED0(dds3GetWorldSecondaryObject(), 9, id);
    if (src != 0) {
        *(s32 *)(*(s32 *)(dst + 0x18) + 0x80) = *(s32 *)(*(s32 *)(src + 0x18) + 0x78);
        return 1;
    }
    return 0;
}

void evtSwitchWorldValueIfChanged(s32 high, s32 low) {
    s32 key = (high << 16) + low;
    if (dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) != key) {
        func_0021FEC0(high, low);
    }
}

void evtResetDrawTransitions(void) {
    kwlnDrawSetOffsetTransition(0, 0, 0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    func_0018F3B0();
    func_0018F438();
    func_0018F750();
    func_0018F4F0();
    func_0018F6E8();
}

void evtShutdownStageAndResetDrawTransitions(void) {
    evtCommandShutdownStage();
    evtResetDrawTransitions();
}

typedef struct QuadRecord {
    s32 a;
    s32 b;
    s32 c;
    s16 d;
    s16 e;
    s32 f;
} QuadRecord;

void evtFillQuadRecordFields(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, QuadRecord *dst) {
    dst->a = a;
    dst->b = b;
    dst->c = c;
    dst->d = d;
    dst->e = e;
    dst->f = f;
}

s32 evtDestroyRegisteredTaskIfPresent(s32 task) {
    if (task != 0) {
        if (kwlnTaskGetRegisteredState(task)) {
            kwlnTaskDestroyWithHierarchy(task, 0);
        }
    }
}

s32 evtReplaceScriptProcessTask(s32 arg0, s32 arg1, s32 *slot) {
    s32 task;

    if (slot != 0) {
        evtDestroyRegisteredTaskIfPresent(*slot);
    }
    task = scrCreateTaskForProcessId(0x7D0, arg0, arg1);
    evtClearActiveFlag(0);
    if (slot != 0) {
        *slot = task;
    }
    return task;
}

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

s32 evtCompareBytesAscending(u8 *left, u8 *right) {
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

void evtRandomSwapBytes(u8 *bytes, u32 length, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        u8 *first = bytes + effMiscRand(0) % length;
        u8 *second = bytes + effMiscRand(0) % length;
        u8 tmp = *first;
        *first = *second;
        *second = tmp;
    }
}

void evtLoadResourcePair(u32 resourceId, u32 *record) {
    u32 handle;

    handle = func_002EB028(resourceId, record + 1, 0);
    *record = handle;
}

void evtReleaseResourcePairHandle(u32 *record) {
    func_002D0918(*record);
}

s32 evtCreateMessageWindowIfMissing(s32 unused) {
    if (dspWindowHandle < 0) {
        dspWindowHandle = itfMesCreateWindow();
        func_0019C968(dspWindowHandle, 2, 0);
        return 1;
    }
    return 0;
}

s32 func_0024DA20(s32 arg0) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    func_0019C968(dspWindowHandle, 0, arg0);
    return 1;
}

s32 dspStartEntry(s32 entry) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    itfMesSetWindowHighFlags(dspWindowHandle, 0x200000);
    itfMesStartEntry(dspWindowHandle, entry, 0);
    itfPanelSetPairFirst(dspWindowHandle, -1);
    dspWindowControlState = 1;
    return 1;
}

s32 evtCaptureMessageWindowSoundMode(s32 arg0) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    D_003BC410 = arg0;
    dspCapturedSoundMode = sndGetActiveMode();
    return 1;
}

void evtSetMessageWindowOptionWhenOpen(s32 arg0) {
    if (dspWindowHandle >= 0) {
        D_003BC414 = arg0;
    }
}

s8 func_0024DB00(void) {
    return D_003BC414;
}

s32 sndGetActiveMode(void) {
    if (dspWindowHandle < 0) {
        return -1;
    }
    return itfPanelGetPairSecond(dspWindowHandle);
}

s8 evtGetCapturedMessageWindowSoundMode(void) {
    return dspCapturedSoundMode;
}

u32 evtCleanupMessageWindow(s32 notify) {
    u32 result;

    result = 0;
    if (-1 < dspWindowHandle) {
        itfPanelSetStatus(dspWindowHandle, 0);
        if (notify != 0) {
            itfMesFinishWindowAndClearStatus(dspWindowHandle);
        }
        itfMesCleanupWindow(dspWindowHandle, 0);
        dspSetActive(1);
        dspWindowControlState = 0;
        result = 1;
    }
    return result;
}

void evtFinishMessageWindowAndNotify(void) {
    evtCleanupMessageWindow(1);
}

s32 dspCloseChannel(void) {
    s32 channel = dspWindowHandle;
    if (channel < 0) {
        return 0;
    }
    itfMesDestroyWindowIfPresent(channel);
    dspWindowHandle = -1;
    dspWindowControlState = 0;
    dspWindowStateGate = 0;
    return 1;
}

s32 evtGetMessageWindowControlState(void) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    if (dspWindowStateGate != 0) {
        if ((s8)dspWindowControlState == 2) {
            return 0;
        }
    }
    return (s8)dspWindowControlState;
}

s32 sndUpdateActiveMode(void) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    if (itfPanelGetPairFirst(dspWindowHandle) < 0) {
        return 0;
    }
    dspCapturedSoundMode = sndGetActiveMode();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DC98);

void func_0024DD78(void) {
    func_0024DC98(1);
}

void func_0024DD90(s32 arg0, s32 arg1) {
    func_0019C838(dspWindowHandle, arg0, arg1);
}

s8 func_0024DDB8(void) {
    return dspWindowStateGate;
}

void dspSetActive(s32 enable) {
    if (enable) {
        itfMesClearWindowHighFlags(dspWindowHandle, 0x800000);
        itfMesClearWindowHighFlags(dspWindowHandle, 0x100000);
        dspWindowStateGate = 0;
        itfPanelSetStatus(dspWindowHandle, 1);
        dspWindowControlState = 1;
    } else {
        itfMesSetWindowHighFlags(dspWindowHandle, 0x800000);
        itfMesSetWindowHighFlags(dspWindowHandle, 0x100000);
        dspWindowStateGate = 1;
    }
}

void evtMoveMessageWindowWithPanelOffset(s32 x, s32 y) {
    itfMesBlk24MoveTo(dspWindowHandle, x << 4, y << 3);
    itfPanelEmitRecord(dspWindowHandle, -((0x15F - y) << 3));
}

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
    evtActiveEntryFlags.flags[flagIndex] = 0;
}

s32 evtIsActiveFlagSet(s32 flagIndex) {
    return evtActiveEntryFlags.flags[flagIndex] != 0;
}

s32 evtSetBoundedDisplayValue(s32 index, s32 value) {
    if (index >= 0x10) {
        return 0;
    }
    evtDisplayValues[index] = value;
    return 1;
}

u32 evtGetBoundedDisplayValue(s32 index) {
    index = (index < 0x10) ? index : 0xf;
    return evtDisplayValues[index];
}

s32 evtSetCurrentActiveFlag(void) {
    s32 flagIndex = scrReadIntParameter(0);

    evtActiveEntryFlags.flags[flagIndex] = 1;
    return 1;
}

s32 evtActivateCurrentFlag(void) {
    s32 index = scrReadIntParameter(0);
    if (index >= 16) {
        index = 15;
    }
    scrSetIntegerReturnValue(evtDisplayValues[index]);
    return 1;
}

u32 evtLoadTextureFromResourcePath(u32 path) {
    u32 info[2];
    u32 allocation = func_002EB028(path, info, &info[1]);
    u32 texture = func_002D3288(info[0]);

    func_002D0918(allocation);
    return texture;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E010);

void evtDrawListViewportPanel(s32 x, s32 y, s32 width, s32 record) {
    s32 height = mnuGetListViewportHeight(*(s32 *)(record + 0x14)) + 0x80;
    func_002C0DD8(x, y, 0, width, height, 0x30303040, 0x53);
    func_0024E010(x + width - 0xA0, y, y + height, 8, record);
}

void evtDrawPlainPanel(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_002C0DD8(arg0, arg1, 0, arg2, arg3, 0x30303040, 0x53);
}

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspWindowHandle);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspWindowControlState);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspWindowStateGate);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC410);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC414);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspCapturedSoundMode);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC418);

