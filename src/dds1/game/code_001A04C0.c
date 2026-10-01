#include "common.h"
#include "pcp_vu0.h"

extern s32 func_001190B0();

extern s32 func_001191B0();

extern u64 btlAdvanceRuntimeSequenceCounter();

extern void btlUpdateScene(void);

extern s32 mdlFlagTest(u32);

extern s32 func_001A17F0(void);

extern s32 D_003BAA00;

extern s32 D_003BB2E4;

extern s32 kwlnTaskFindByPriority(u32);

extern u64 D_003BB2E8;

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern s32 sdfAllocPacketAligned(s32 size);

INCLUDE_ASM(const s32, "game/code_001A04C0", func_001A04C0);

INCLUDE_ASM(const s32, "game/code_001A04C0", func_001A0910);

u64 *btlCreateGsTestRegisterPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdfConsFinalizePacketHeader(sdfAllocPacketAligned(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x48 : 0x47;
    return entry;
}

u64 *btlCreateGsAlphaRegisterPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdfConsFinalizePacketHeader(sdfAllocPacketAligned(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x43 : 0x42;
    return entry;
}

INCLUDE_ASM(const s32, "game/code_001A04C0", func_001A0B28);

void func_001A0CA0(void) {
    D_003BB2E8 = 1;
}

u64 btlAdvanceRuntimeSequenceCounter(void) {
    s64 value = D_003BB2E8 + 1;

    if (value < 0) {
        value = 1;
        D_003BB2E8 = value;
    } else {
        D_003BB2E8 = value;
    }
    return value;
}

void btlClearModelFlagRange(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0xbe0;
    do {
        temp_v0 = temp_v1 + 1;
        mdlFlagClear(temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 0xbff);
}

s32 btlUpdateActiveBattleFrame(void) {
    s32 state = D_003BB2E4;
    if (state == 0) {
        return 0;
    }
    if ((*(u32 *)(state + 0x1F4) & 1) != 0) {
        btlUpdateFadeColor();
        btlUpdateAutoMusic();
        btlUpdateTintAndWorldLight();
        func_001F44C0();
        btlUpdateScene();
        func_001C8330();
        btlUpdateActionSeqs();
        func_001DA468();
        func_001FB088();
        btlSweepFinishedTasks();
        func_001DBE68();
        ++*(s32 *)(D_003BB2E4 + 0x1F0);
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

s32 btlUpdateBattleFieldPresentation(void) {
    s32 state = D_003BB2E4;
    if (state == 0) {
        return 0;
    }
    if ((*(u32 *)(state + 0x1F4) & 1) != 0) {
        btlTickFieldSwayAndTint();
        btlDispatchLinkedEffectWhenBattleGatesClear();
        btlSweepFloorModelLists();
        func_001DA780();
        func_0020FC48();
        func_001FB090();
        fldInitializeBattleSceneFlow();
        btlClearDeferredTasks();
    }
    return 0;
}

void func_001A0E38(void) {
}

extern u8 D_003BB2F0[];

extern char D_003A1598[]; /* "battle_draw" */

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

void btlCreateDrawTasks(void) {
    s32 state = D_003BB2E4;
    u32 mainTask;
    u32 drawTask;
    mainTask = kwlnTaskCreate((u32)D_003BB2F0, 0x3F9, 0, 0, btlUpdateActiveBattleFrame, func_001A0E38, 0);
    *(u32 *)(state + 0x29C) = mainTask;
    drawTask = kwlnTaskCreate((u32)D_003A1598, 0x2B0E, 0, 0, btlUpdateBattleFieldPresentation, 0, 0);
    func_00101A80(mainTask, drawTask);
}

void btlDestroyDrawTaskAtPriorityWhenPresent(void) {
    s64 temp_v0;

    temp_v0 = kwlnTaskFindByPriority(0x3f9);
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(*(u32 *)(D_003BB2E4 + 0x29c), 1);
        return;
    }
}

extern s32 D_003BB2E0;

extern u16 D_003BA72C;

extern s32 D_003BAAA4;

extern s32 D_0032A520;

extern s32 D_0032A210;

extern s8 D_00324550[];

extern s32 func_002D03F8(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern void func_002E8430(s8 *, s32);

extern u32 func_00100510(void);

extern s32 itfMesCreateWindow(s32);

extern void sndResetTransition(void);

extern void func_001F4430(void);

extern void btlResetDeferredTaskQueue(void);

extern void btlResetToInitialScene(void);

extern void fldClearSceneSlotsAndGroups(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

extern void btlRetainButtonTexture(void);

extern void func_001C45F0(void);

extern void func_001FB098(void);

INCLUDE_ASM(const s32, "game/code_001A04C0", func_001A0F10);

extern char D_003A15A8[];

extern char D_003A15C8[];

extern void itfMesDestroyWindowIfPresent(s32);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A1598);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A15A8);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A15C8);

s32 btlExitWhenAudioAndTasksIdle(void) {
    if (D_003BB2E4 == 0) {
        btlBossDebugPrintf(D_003A15A8);
        return 0;
    }
    if (sndHasOccupiedNodeSlots() != 0) {
        btlBossDebugPrintf(D_003A15C8);
        return -1;
    }
    if (btlCountRegisteredTasks() != 0) {
        btlBossDebugPrintf("btl:wait Packet\n");
        btlFlagTasksForUpdate();
        return -1;
    }
    btlReleaseBossData();
    btlClearTaskLists();
    btlDestroyAllActionSeqs();
    btlDestroyAllUnits();
    btlClearPendingSoundList();
    btlReleaseEventAssets();
    func_001C7360();
    btlFreeFieldBlocks();
    btlStopRainSoundTransition();
    btlClearTintAndEnableCamera();
    func_001C4658();
    btlReleaseButtonTexture();
    sndFreeBattleSoundEntries();
    sndClearList();
    brsTaskTryDestroy();
    btlClearSoundAndModelResources();
    btlDestroyDrawTaskAtPriorityWhenPresent();
    itfMesDestroyWindowIfPresent(*(s32 *)(D_003BB2E4 + 0x4A0));
    itfMesDestroyWindowIfPresent(*(s32 *)(D_003BB2E4 + 0x49C));
    itfMesDestroyWindowIfPresent(*(s32 *)(D_003BB2E4 + 0x498));
    btlAdvanceTitleStateWithAudioCleanup();
    func_00105888();
    evtDestroySelectionState();
    effResetSlots();
    evtSetSolarOverlayFullyTransparent();
    itfMesClearFlags(1);
    func_002D0918(D_003BB2E0);
    D_003BB2E0 = 0;
    D_003BB2E4 = 0;
    btlBossDebugPrintf("** btlExit ***************\n");
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A04C0", func_001A11F0);

void btlLoadInputIconsAndSystemSounds(void) {
    btlOpenButtonIconResource();
    func_001C44F8();
    sndLoadSysEffLb();
}

u8 btlIsRuntimeAllocated(void) {
    return D_003BB2E4 != 0;
}

s32 btlIsCurrentActorFullyMarked(void) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    return (*(s32 *)(D_003BB2E4 + 0x1f4) & 0x6000000) == 0x6000000;
}

s32 btlHasPendingRuntimeActivity(void) {
    s32 state;
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    state = D_003BB2E4;
    if (*(s32 *)(state + 0x224) != 0) {
        return 1;
    }
    if ((*(s32 *)(state + 0x1C8) & 2) != 0) {
        return 1;
    }
    return *(u32 *)(state + 0x694) != 0;
}

void btlResetActorEntryState(void) {
    s32 context = D_003BB2E4;
    s32 *entries = (s32 *)(D_003BAA00 + 0xBF8);
    u32 i;
    *(s32 *)(context + 0x2C0) = 0;
    *(s32 *)(context + 0x2C4) = 0;
    *(s32 *)(context + 0x2C8) = 0;
    *(s32 *)(context + 0x2CC) = 0;
    *(s32 *)(context + 0x2D0) = 0;
    for (i = 0; i < 5; i++) {
        *entries = 0;
        entries = (s32 *)((u8 *)entries + 0x1A4);
    }
    memset((void *)(D_003BB2E4 + 0x2B4), 0, 12);
}

INCLUDE_ASM(const s32, "game/code_001A04C0", func_001A1530);

typedef struct EncBgEntry {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
} EncBgEntry;

typedef struct EncBgRow {
    s32 mapIdBase;
    EncBgEntry entries[64];
} EncBgRow;

extern EncBgRow *D_003BAA44;

extern s32 fldConsumeNextSceneRequest(s32 *, s32 *);

s32 btlResolveQueuedSceneRequestParameters(s32 *outCode, s32 *outParameter) {
    s32 buffer[2];
    s16 i;

    if (fldConsumeNextSceneRequest(&buffer[0], &buffer[1]) == 0) {
        return 0;
    }
    for (i = 0; i < 0x10; i++) {
        if (D_003BAA44[i].mapIdBase + 0xC8 != buffer[0]) {
            continue;
        }
        *outCode = D_003BAA44[i].mapIdBase + 0xC8;
        if (D_003BAA44[i].entries[buffer[1]].unk08 != -1 &&
            mdlFlagTest(D_003BAA44[i].entries[buffer[1]].unk08) != 0) {
            *outParameter = D_003BAA44[i].entries[buffer[1]].unk0A;
            return 1;
        }
        if (D_003BAA44[i].entries[buffer[1]].unk04 != -1 &&
            mdlFlagTest(D_003BAA44[i].entries[buffer[1]].unk04) != 0) {
            *outParameter = D_003BAA44[i].entries[buffer[1]].unk06;
            return 1;
        }
        *outParameter = D_003BAA44[i].entries[buffer[1]].unk00;
        return 1;
    }
    return 0;
}

s32 func_001A17F0(void) {
    return D_003BB2E4;
}

u16 func_001A17F8(s32 arg0) {
    return *(u16 *)(arg0 + 6);
}

u16 func_001A1800(s32 arg0) {
    return *(u16 *)(arg0 + 10);
}

void func_001A1808(void) {
    ptyComputeMaxHp();
}

void func_001A1820(void) {
    ptyComputeMaxMp();
}

u32 func_001A1838(s32 object) {
    return func_001190B0(object);
}

u32 func_001A1850(s32 object) {
    return func_001191B0(object);
}

void func_001A1868(u8 *object, s32 value) {
    datMoveCursorX(object, value);
}

void func_001A1880(u8 *object, s32 value) {
    datMoveCursorY(object, value);
}

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(s32 object) {
    u16 maximum = func_001A17F8(object);
    u32 value = func_001A1838(object);
    *(u16 *)(object + 8) = value;
    if (value < maximum) {
        *(u16 *)(object + 6) = value;
    }
    return *(u16 *)(object + 6);
}

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(s32 object) {
    u16 maximum = func_001A1800(object);
    u32 value = func_001A1850(object);
    *(u16 *)(object + 12) = value;
    if (value < maximum) {
        *(u16 *)(object + 10) = value;
    }
    return *(u16 *)(object + 10);
}

u16 func_001A1938(s32 arg0) {
    return *(u16 *)(arg0 + 0xe) & 0x7fff;
}

void func_001A1948() {
    func_00119018();
}
