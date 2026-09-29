#include "common.h"

extern s32 D_0043789C;

extern s32 D_00437898;

extern s32 func_0026CD50(u32);

extern s64 func_0026C768(void);

extern u32 func_00343ED0(u32, u32 *, u32);

extern s32 D_00437880;

extern u8 D_00437884;

extern void func_0026C900(void);

extern s32 D_00437888;

extern s8 D_0043788D;

extern s8 D_0043788C;

extern s8 D_00437885;

extern u32 D_00453CC0[];

void func_003297C8(u32 sprite);

typedef struct EvtResourcePair {
    u32 handle;
    u32 input;
} EvtResourcePair;

extern s32 D_00435DD0;

typedef struct {
    u8 count;
    u8 pad;
    u16 indices[0];
} ActiveList;

extern u64 dds3GetWorldSecondaryObject(void);
extern s32 dds3GetWorldObjectValue(u64);
extern void func_0023AA30(s32, s32);
/* Updates the secondary world selector only when the packed pair changes. */
void func_0026C1D0(s32 first, s32 second) {
    s32 packed = (first << 16) + second;

    if (dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) != packed) {
        func_0023AA30(first, second);
    }
}

void func_0026C240(void) {
    kwlnDrawSetOffsetTransition(0, 0, 0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    func_00196FE8();
    func_00197070();
    func_00197388();
    func_00197128();
    func_00197320();
}

void func_0026C298(void) {
    evtCommandShutdownStage();
    func_0026C240();
}

void func_0026C2B8(u32 a, u32 b, u32 c, s16 d, s16 e, u32 f, u32 *dst) {
    dst[0] = a;
    dst[1] = b;
    dst[2] = c;
    *(s16 *)&dst[3] = d;
    *((s16 *)&dst[3] + 1) = e;
    dst[4] = f;
}

s32 func_0026C2D8(s32 task) {
    if (task != 0) {
        if (kwlnTaskGetRegisteredState(task) != 0) {
            kwlnTaskDestroyWithHierarchy(task, 0);
        }
    }
}

extern s32 scrCreateTaskForProcessId();
s32 func_0026C318(s32 first, s32 second, s32 *taskSlot) {
    s32 task;

    if (taskSlot != NULL) {
        func_0026C2D8(*taskSlot);
    }
    task = scrCreateTaskForProcessId(0x7D0, first, second);
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
        if (*(u8 *)(index + D_00435DD0 + 0x1340) != 0) {
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

    value = func_00343ED0(resource, &record->input, 0);
    record->handle = value;
}

void evtReleaseResourcePairHandle(EvtResourcePair *record) {
    func_003297C8(record->handle);
}

extern s32 func_001A38D8(void);
s32 func_0026C538(void) {
    if (D_00437880 < 0) {
        D_00437880 = func_001A38D8();
        func_001A4988(D_00437880, 2, 0);
        return 1;
    }
    return 0;
}

s32 func_0026C580(s32 arg0) {
    if (D_00437880 < 0) {
        return 0;
    }
    func_001A4988(D_00437880, 0, arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026C5B8);

s32 func_0026C618(s32 arg0) {
    if (D_00437880 < 0) {
        return 0;
    }
    D_00437888 = arg0;
    D_0043788D = sndGetActiveMode();
    return 1;
}

void func_0026C648(s32 arg0) {
    if (D_00437880 >= 0) {
        D_0043788C = arg0;
    }
}

s8 func_0026C660(void) {
    return D_0043788C;
}

s32 sndGetActiveMode(void) {
    if (D_00437880 < 0) {
        return -1;
    }
    return itfPanelGetPairSecond(D_00437880);
}

s8 func_0026C6A0(void) {
    return D_0043788D;
}

u32 func_0026C6A8(s32 notify) {
    u32 result;

    result = 0;
    if (-1 < D_00437880) {
        itfPanelSetStatus(D_00437880, 0);
        if (notify != 0) {
            func_001A34D0(D_00437880);
        }
        itfMesCleanupWindow(D_00437880, 0);
        func_0026C948(1);
        D_00437884 = 0;
        result = 1;
    }
    return result;
}

void func_0026C710(void) {
    func_0026C6A8(1);
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026C728);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026C768);

s32 sndUpdateActiveMode(void) {
    if (D_00437880 < 0) {
        return 0;
    }
    if (itfPanelGetPairFirst(D_00437880) < 0) {
        return 0;
    }
    D_0043788D = sndGetActiveMode();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026C7F8);

void func_0026C8E8(u32 arg0) {
    func_0026C7F8(arg0, 1);
}

void func_0026C900(void) {
    func_0026C8E8(1);
}

void func_0026C918(s32 arg0, s32 arg1) {
    func_001A4858(D_00437880, arg0, arg1);
}

s8 func_0026C940(void) {
    return D_00437885;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026C948);

void func_0026C9B8(s32 x, s32 y) {
    itfMesBlk24MoveTo(D_00437880, x * 16, y * 8);
    itfPanelEmitRecord(D_00437880, -((0x15F - y) * 8));
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

INCLUDE_ASM(const s32, "game/code_0026C1D0", evtClearActiveFlag);

INCLUDE_ASM(const s32, "game/code_0026C1D0", evtIsActiveFlagSet);

s32 func_0026CA80(s32 arg0, s32 arg1) {
    if (arg0 < 0x10) {
    } else {
        return 0;
    }
    D_00453CC0[arg0] = arg1;
    return 1;
}

u32 func_0026CAA8(s32 arg0) {
    arg0 = (arg0 < 0x10) ? arg0 : 0xf;
    return D_00453CC0[arg0];
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", evtSetCurrentActiveFlag);

s32 evtActivateCurrentFlag(void) {
    s32 index = scrReadIntParameter(0);
    if (index >= 16) {
        index = 15;
    }
    func_0010D818(D_00453CC0[index]);
    return 1;
}

extern s32 func_0032C138(u32);
s32 func_0026CB48(u32 resource) {
    u32 buffer[2];
    u32 handle;
    s32 result;

    handle = func_00343ED0(resource, &buffer[0], (u32)&buffer[1]);
    result = func_0032C138(buffer[0]);
    func_003297C8(handle);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CB98);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CC88);

void func_0026CD20(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_00308808(arg0, arg1, 0, arg2, arg3, 0x30303040, 0x53);
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CD50);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CE90);

void func_0026CF08(u32 arg0) {
    if (D_00437898 != 0) {
        func_0026CF48();
    }
    D_00437898 = func_0026CD50(arg0);
}

void func_0026CF48(void) {
    func_003297C8(*(u32 *)D_00437898);
    D_00437898 = 0;
}

s32 func_0026CF70(s32 arg0) {
    return *(s32 *)(D_00437898 + 4) + ((arg0 << 0x10) >> 0xb);
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CF88);

u32 func_0026D020(void) {
    return *(u32 *)(D_00437898 + 8);
}

void func_0026D030(u32 arg0) {
    if (D_0043789C != 0) {
        func_0026D070();
    }
    D_0043789C = func_0026CD50(arg0);
}

void func_0026D070(void) {
    func_003297C8(*(u32 *)D_0043789C);
    D_0043789C = 0;
}

s32 func_0026D098(s32 arg0) {
    return *(s32 *)(D_0043789C + 4) + ((arg0 << 0x10) >> 0xb);
}

extern u32 func_003292A8(u32);
extern void *sdfMemoryGetBlockAddress(u32);
extern void func_0026D168(void *, s32, s32);
typedef struct EvtMantraWork {
    u32 allocation;
    u32 capacity;
    void *entries;
    u8 data[0x160];
} EvtMantraWork; /* 0x16C bytes */
EvtMantraWork *func_0026D0B0(s32 arg0, s32 arg1) {
    u32 allocation = func_003292A8(0x16C);
    EvtMantraWork *work = sdfMemoryGetBlockAddress(allocation);

    memset(work, 0, 0x16C);
    work->allocation = allocation;
    work->capacity = 0xB0;
    work->entries = work->data;
    if (arg0 != 0) {
        func_0026D168(work, arg0, arg1);
    }
    return work;
}

s64 func_0026D148(u32 *p) {
    if (p != NULL) {
        func_003297C8((void *)*p);
    }
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D168);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D4C8);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D590);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D710);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D7E8);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D988);

extern s32 func_00315248(s32, s32);
void func_0026DA90(s32 arg0, s32 *record) {
    s32 i;
    s32 value;

    for (i = 0; i < 5; i++) {
        value = func_00315248(arg0 & 0xFFFF, i);
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

s64 func_0026DB28(u32 context, u32 entry) {
    return scrClearEntryFlag(context, entry & 0xFF, 0xF);
}

void func_0026DB48(u32 arg0, u8 arg1) {
    scrTestEntryFlag(arg0, arg1, 0xf);
}

void func_0026DB68(void) {
}

s64 func_0026DB70(u32 context) {
    return scrClearEntryFlag(context, 0, 0);
}

void func_0026DB90(u32 arg0) {
    scrTestEntryFlag(arg0, 0, 0);
}

void func_0026DBB0(void) {
}

s64 func_0026DBB8(u32 context) {
    return scrClearEntryFlag(context, 0, 1);
}

void func_0026DBD8(u32 arg0) {
    scrTestEntryFlag(arg0, 0, 1);
}

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437880);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437884);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437885);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437888);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_0043788C);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_0043788D);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437890);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437898);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_0043789C);

