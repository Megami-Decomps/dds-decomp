#include "common.h"

extern s32 mnuMantraPanelPositionTable;

extern s32 mnuMantraNodePositionTable;

extern s32 func_0026CD50(u32);

extern s32 evtGetMessageWindowControlState(void);

extern u32 sdfReadNamedResource(u32, u32 *, u32);

extern s32 dspWindowHandle;

extern s8 dspWindowControlState;

extern void func_0026C900(void);

extern s32 D_00437888;

extern s8 dspCapturedSoundMode;

extern s8 evtMessageWindowOption;

extern s8 dspWindowStateGate;

extern u32 evtDisplayValues[];

void sdfReleaseResourceAllocation(u32 sprite);

typedef struct EvtResourcePair {
    u32 handle;
    u32 input;
} EvtResourcePair;

extern s32 datGameState;

typedef struct {
    u8 pad0[0x1340];
    u8 active[0x100]; /* Active game-entry flags indexed from 1 to 255. */
} EvtGameEntries;

typedef struct {
    u8 count;
    u8 pad;
    u16 indices[0];
} ActiveList;

extern u64 dds3GetWorldSecondaryObject(void);
extern s32 dds3GetWorldObjectValue(u64);
extern void evtCreateWorldObjectForKey(s32, s32);
/* Updates the secondary world selector only when the packed pair changes. */
void evtSwitchWorldValueIfChanged(s32 first, s32 second) {
    s32 packed = (first << 16) + second;

    if (dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) != packed) {
        evtCreateWorldObjectForKey(first, second);
    }
}

void evtResetDrawTransitions(void) {
    kwlnDrawSetOffsetTransition(0, 0, 0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    effDisableRectangleBlur();
    effDisableTexturedBlur();
    effDisableTexturedSquare();
    effDisableFilterBlur();
    effDisableColorRectangle();
}

void evtShutdownStageAndResetDrawTransitions(void) {
    evtCommandShutdownStage();
    evtResetDrawTransitions();
}

void evtFillQuadRecordFields(u32 a, u32 b, u32 c, s16 d, s16 e, u32 f, u32 *dst) {
    dst[0] = a;
    dst[1] = b;
    dst[2] = c;
    *(s16 *)&dst[3] = d;
    *((s16 *)&dst[3] + 1) = e;
    dst[4] = f;
}

s32 evtDestroyRegisteredTaskIfPresent(s32 task) {
    if (task != 0) {
        if (kwlnTaskGetRegisteredState(task) != 0) {
            kwlnTaskDestroyWithHierarchy(task, 0);
        }
    }
}

extern s32 scrCreateTaskForProcessId();
s32 evtReplaceScriptProcessTask(s32 processId, s32 value, s32 *taskSlot) {
    s32 task;

    if (taskSlot != NULL) {
        evtDestroyRegisteredTaskIfPresent(*taskSlot);
    }
    task = scrCreateTaskForProcessId(0x7D0, processId, value);
    evtClearActiveFlag(0);
    if (taskSlot != NULL) {
        *taskSlot = task;
    }
    return task;
}

/* Collects indexes of the active entries into the caller's list. */
void evtCollectActiveGameIndices(ActiveList *list) {
    s32 index;
    list->count = 0;
    for (index = 1; index < 0x100; index++) {
        if (((EvtGameEntries *)datGameState)->active[index] != 0) {
            s32 count = list->count++;
            list->indices[count] = index;
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

extern u32 effMiscRand();
/* Swaps randomly selected elements the requested number of times (not a Fisher-Yates shuffle). */
void evtRandomSwapBytes(u8 *buffer, u32 length, s32 count) {
    u8 *first;
    u8 *second;
    u8 value;

    if (count > 0) {
        s32 remaining = count;
        do {
            remaining--;
            first = buffer + effMiscRand(0) % length;
            second = buffer + effMiscRand(0) % length;
            value = *first;
            *first = *second;
            *second = value;
        } while (remaining != 0);
    }
}

void evtLoadResourcePair(u32 resource, EvtResourcePair *record) {
    u32 value;

    value = sdfReadNamedResource(resource, &record->input, 0);
    record->handle = value;
}

void evtReleaseResourcePairHandle(EvtResourcePair *record) {
    sdfReleaseResourceAllocation(record->handle);
}

extern s32 itfMesCreateWindow(void);
s32 evtCreateMessageWindowIfMissing(void) {
    if (dspWindowHandle < 0) {
        dspWindowHandle = itfMesCreateWindow();
        itfMesSetWindowPageAndRefresh(dspWindowHandle, 2, 0);
        return 1;
    }
    return 0;
}

s32 func_0026C580(s32 value) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    itfMesSetWindowPageAndRefresh(dspWindowHandle, 0, value);
    return 1;
}

extern void itfMesSetWindowHighFlags(s32, u32);
extern void itfMesStartEntry(s32, s32, s32);
extern void itfPanelSetPairFirst(s32, s32);
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

s32 evtCaptureMessageWindowSoundMode(s32 value) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    D_00437888 = value;
    dspCapturedSoundMode = sndGetActiveMode();
    return 1;
}

void evtSetMessageWindowOptionWhenOpen(s32 value) {
    if (dspWindowHandle >= 0) {
        evtMessageWindowOption = value;
    }
}

s8 evtGetMessageWindowOption(void) {
    return evtMessageWindowOption;
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

extern void itfMesDestroyWindowIfPresent(s32);
s32 dspCloseChannel(void) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    itfMesDestroyWindowIfPresent(dspWindowHandle);
    dspWindowHandle = -1;
    dspWindowControlState = 0;
    dspWindowStateGate = 0;
    return 1;
}

s32 evtGetMessageWindowControlState(void) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    if (dspWindowStateGate != 0 && dspWindowControlState == 2) {
        return 0;
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

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026C7F8);

void func_0026C8E8(u32 value) {
    func_0026C7F8(value, 1);
}

void func_0026C900(void) {
    func_0026C8E8(1);
}

void func_0026C918(s32 first, s32 second) {
    itfMesCopyStringToWindowTableSlot(dspWindowHandle, first, second);
}

s8 dspGetWindowStateGate(void) {
    return dspWindowStateGate;
}

extern void itfMesClearWindowHighFlags(s32, u32);
extern void itfPanelSetStatus(s32, s32);
void dspSetActive(s32 enabled) {
    if (enabled != 0) {
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
    itfMesBlk24MoveTo(dspWindowHandle, x * 16, y * 8);
    itfPanelEmitRecord(dspWindowHandle, -((0x15F - y) * 8));
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

extern s8 evtActiveEntryFlags[8];

void evtClearActiveFlag(s32 index) {
    evtActiveEntryFlags[8 + index] = 0;
}

s32 evtIsActiveFlagSet(s32 index) {
    return evtActiveEntryFlags[8 + index] != 0;
}

s32 evtSetBoundedDisplayValue(s32 index, s32 value) {
    if (index < 0x10) {
    } else {
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
    s32 index = scrReadIntParameter(0);
    evtActiveEntryFlags[8 + index] = 1;
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

extern s32 sdfTexAcquireResourceTexture(u32);
s32 evtLoadTextureFromResourcePath(u32 resource) {
    u32 buffer[2];
    u32 handle;
    s32 result;

    handle = sdfReadNamedResource(resource, &buffer[0], (u32)&buffer[1]);
    result = sdfTexAcquireResourceTexture(buffer[0]);
    sdfReleaseResourceAllocation(handle);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CB98);

extern s32 mnuGetListViewportHeight(s32);

extern void func_00308808(s32, s32, s32, s32, s32, u32, s32);

extern void func_0026CB98(s32, s32, s32, s32, s32);

typedef struct {
    u8 pad0[0x18];
    s32 heightSource; /* 0x18: passed to mnuGetListViewportHeight for the panel height */
} EvtPanelRecord;

typedef struct {
    u32 handle; /* 0x0: released by sdfReleaseResourceAllocation */
    s32 base;   /* 0x4: origin of a 32-byte-stride lookup */
    u32 unk8;   /* 0x8: exposed by func_0026D020 */
} EvtLoadedRecord;

void evtDrawListViewportPanel(s32 x, s32 y, s32 width, EvtPanelRecord *record) {
    s32 height = mnuGetListViewportHeight(record->heightSource) + 0x80;

    func_00308808(x, y, 0, width, height, 0x30303040, 0x53);
    func_0026CB98(x + width - 0xA0, y, y + height, 8, (s32)record);
}

void evtDrawPlainPanel(u32 x, u32 y, u32 width, u32 height) {
    func_00308808(x, y, 0, width, height, 0x30303040, 0x53);
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CD50);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CE90);

void mnuLoadMantraNodePositionTable(u32 resource) {
    if (mnuMantraNodePositionTable != 0) {
        mnuReleaseMantraNodePositionTable();
    }
    mnuMantraNodePositionTable = func_0026CD50(resource);
}

void mnuReleaseMantraNodePositionTable(void) {
    sdfReleaseResourceAllocation(((EvtLoadedRecord *)mnuMantraNodePositionTable)->handle);
    mnuMantraNodePositionTable = 0;
}

s32 mnuGetMantraNodePositionRecord(s32 index) {
    return ((EvtLoadedRecord *)mnuMantraNodePositionTable)->base + ((index << 0x10) >> 0xb);
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CF88);

u32 func_0026D020(void) {
    return ((EvtLoadedRecord *)mnuMantraNodePositionTable)->unk8;
}

void mnuLoadMantraPanelPositionTable(u32 resource) {
    if (mnuMantraPanelPositionTable != 0) {
        mnuReleaseMantraPanelPositionTable();
    }
    mnuMantraPanelPositionTable = func_0026CD50(resource);
}

void mnuReleaseMantraPanelPositionTable(void) {
    sdfReleaseResourceAllocation(((EvtLoadedRecord *)mnuMantraPanelPositionTable)->handle);
    mnuMantraPanelPositionTable = 0;
}

s32 mnuGetMantraPanelPositionRecord(s32 index) {
    return ((EvtLoadedRecord *)mnuMantraPanelPositionTable)->base + ((index << 0x10) >> 0xb);
}

extern u32 sdfAllocGeneralBlock(u32);
extern void *sdfMemoryGetBlockAddress(u32);
extern void func_0026D168(void *, s32, s32);
typedef struct EvtMantraWork {
    u32 allocation;
    u32 capacity;
    void *entries;
    u8 data[0x160];
} EvtMantraWork; /* 0x16C bytes */
EvtMantraWork *evtAllocateMantraSelectionWork(s32 initialValue, s32 mode) {
    u32 allocation = sdfAllocGeneralBlock(0x16C);
    EvtMantraWork *work = sdfMemoryGetBlockAddress(allocation);

    memset(work, 0, 0x16C);
    work->allocation = allocation;
    work->capacity = 0xB0;
    work->entries = work->data;
    if (initialValue != 0) {
        func_0026D168(work, initialValue, mode);
    }
    return work;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", evtReleaseMantraSelectionWork);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D168);

typedef struct PartySlotHeader {
    u16 flags;
    u16 pad02;
    u16 unitId;
    u8 pad06[0x1BE];
} PartySlotHeader;

typedef struct PartyGameState {
    u8 pad00[0xA60];
    PartySlotHeader party[5];
} PartyGameState;

extern u32 ptyGetProfileRecordCap(u16 scriptId);
extern u32 ptyGetProfileRecordValue(u32 work, u16 scriptId);

/* 1 when some active party member other than unit `skipId` has `scriptId` at its profile record cap. */
s32 ptyAnyActivePartyMemberAtProfileCap(u16 scriptId, u16 skipId) {
    s32 i;

    for (i = 0; i < 5; i++) {
        PartySlotHeader *slot;

        if (((PartyGameState *)datGameState)->party[i].flags & 1) {
            slot = &((PartyGameState *)datGameState)->party[i];
            if (slot->unitId != skipId) {
                if (ptyGetProfileRecordCap(scriptId) == ptyGetProfileRecordValue((u32)slot, scriptId)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D590);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D710);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D7E8);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D988);

extern s32 prfGetIndexedProfileByte(s32, s32);
void prfSummarizeNonzeroEntryAttributes(s32 entry, s32 *record) {
    s32 i;
    s32 value;

    for (i = 0; i < 5; i++) {
        value = prfGetIndexedProfileByte(entry & 0xFFFF, i);
        if (value != 0) {
            record[1] = value;
            if (record[0] == 0) {
                record[0] = i;
            } else {
                record[0] = 5;
                return;
            }
        }
    }
}

void func_0026DB20(void) {
}

extern s32 scrClearEntryFlag();

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026DB28);

void func_0026DB48(u32 context, u8 entry) {
    scrTestEntryFlag(context, entry, 0xf);
}

void func_0026DB68(void) {
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026DB70);

void func_0026DB90(u32 context) {
    scrTestEntryFlag(context, 0, 0);
}

void func_0026DBB0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026DBB8);

void func_0026DBD8(u32 context) {
    scrTestEntryFlag(context, 0, 1);
}

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowHandle);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowControlState);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowStateGate);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437888);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", evtMessageWindowOption);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspCapturedSoundMode);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437890);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", mnuMantraNodePositionTable);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", mnuMantraPanelPositionTable);

