#include "common.h"
#include "pcp_vu0.h"

typedef struct UiQuadColor {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
} UiQuadColor;

typedef struct UiSurface {
    u8 pad00[0x10];
    void (*submit)(struct UiSurface *, s32);
    u8 pad14[0xC];
} UiSurface;

extern UiSurface kwlnDrawSurfaces[];
extern s32 D_003BD824;
extern UiQuadColor D_00358390;
extern f32 sdfSinPoly(f32);
extern s32 sdfCreateResetPacketList(void);
extern void sdfAppendPacket(s32, u64 *);
extern u64 *func_001A0910(s32, s32, s32, s32, s32, u32, u32);

extern void btlBossDebugPrintf(const char *format, ...);

extern s32 datComputeSkillBoostedMaxHp();

extern s32 datComputeSkillBoostedMaxMp();

extern u64 btlAdvanceRuntimeSequenceCounter();

extern void btlUpdateScene(void);

extern s32 mdlFlagTest(u32);

extern s32 btlGetRuntime(void);

extern s32 datGameState;

extern s32 btlRuntime;

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

void itfDrawPulsingTestOverlay(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    u8 *component;
    s32 list;
    UiSurface *surface;
    f32 phase;

    phase = (f32)(D_003BD824 % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSinPoly(phase) + 1.0f) * 0.5f * D_00358390.alpha);
    component = (u8 *)&D_00358390;
    for (i = 0; i != 3; i++, component += 4) {
        color |= *component << (i * 8);
    }
    color |= (u32)alpha << 24;
    list = sdfCreateResetPacketList();
    sdfAppendPacket(list, btlCreateGsTestRegisterPacket(0x33001, 0));
    sdfAppendPacket(list, btlCreateGsAlphaRegisterPacket(6, 0));
    sdfAppendPacket(list, func_001A0910(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->submit(surface, list);
}

void btlResetRuntimeSequenceCounter(void) {
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
    s32 state = btlRuntime;
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
        ++*(s32 *)(btlRuntime + 0x1F0);
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

s32 btlUpdateBattleFieldPresentation(void) {
    s32 state = btlRuntime;
    if (state == 0) {
        return 0;
    }
    if ((*(u32 *)(state + 0x1F4) & 1) != 0) {
        btlTickFieldSwayAndTint();
        btlDispatchLinkedEffectWhenBattleGatesClear();
        btlSweepFloorModelLists();
        btlUpdateActorModelColorAndLinks();
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
    s32 state = btlRuntime;
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
        kwlnTaskDestroyWithHierarchy(*(u32 *)(btlRuntime + 0x29c), 1);
        return;
    }
}

extern s32 D_003BB2E0;

extern u16 mnuMovieTaskState;

typedef struct ItfMesTable ItfMesTable;

typedef struct ItfMesEntry {
    u32 itemList;
    ItfMesTable *table;
} ItfMesEntry;

/* Variable-length message data consumed by itfMesCreateWindow. */
typedef struct ItfMesSub {
    u8 unk00[0x18];
    u32 entryCount;
    u8 unk1C[4];
    ItfMesEntry entries[1];
} ItfMesSub;

typedef struct BattleInitState {
    u8 pad000[0x1F0];
    u32 tick;
    u8 pad1F4[0x0C];
    u32 unk200;
    u8 pad204[0x20];
    u32 listHeads[6];
    u8 pad23C[0x1C];
    u8 endCode;
    u8 pad259[0x2F];
    s16 backgroundA;
    s16 backgroundB;
    u32 unk28C;
    u8 pad290[0x200];
    u8 unk490;
    u8 pad491[3];
    f32 unk494;
    s32 messageWindows[3];
    u8 pad4A4[0xE0];
    u8 unk584;
    u8 pad585[3];
    u32 unk588;
} BattleInitState;

extern ItfMesSub *D_003BAAA4;

extern ItfMesSub D_0032A520;

extern ItfMesSub D_0032A210;

extern s8 effSharedRandomState[];

extern s32 sdfAllocGeneralBlock(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern void effMiscSeedRandom(s8 *, s32);

extern u32 func_00100510(void);

extern s32 itfMesCreateWindow(ItfMesSub *);

extern void btlResetActorEntryState(void);

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

void func_001A0F10(void) {
    btlResetRuntimeSequenceCounter();
    D_003BB2E0 = sdfAllocGeneralBlock(0xE10);
    btlRuntime = (s32)sdfResourceRetainAddress(D_003BB2E0);
    memset((void *)btlRuntime, 0, 0xE10);

    ((BattleInitState *)btlRuntime)->tick = 0;
    ((BattleInitState *)btlRuntime)->unk200 = 0;
    ((BattleInitState *)btlRuntime)->listHeads[0] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[1] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[2] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[3] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[4] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[5] = 0;
    mnuMovieTaskState = 2;
    ((BattleInitState *)btlRuntime)->unk490 = 0x1E;
    {
        BattleInitState *state = (BattleInitState *)btlRuntime;
        state->endCode = 0;
        state->unk494 = 1.0f;
    }
    ((BattleInitState *)btlRuntime)->unk584 = 0;
    ((BattleInitState *)btlRuntime)->unk588 = 0x80808080;
    ((BattleInitState *)btlRuntime)->backgroundA = 0xC9;
    ((BattleInitState *)btlRuntime)->backgroundB = 1;
    ((BattleInitState *)btlRuntime)->unk28C = 0;

    effMiscSeedRandom(effSharedRandomState, func_00100510());
    btlResetActorEntryState();
    ((BattleInitState *)btlRuntime)->messageWindows[0] =
        itfMesCreateWindow(D_003BAAA4);
    ((BattleInitState *)btlRuntime)->messageWindows[1] = itfMesCreateWindow(&D_0032A520);
    ((BattleInitState *)btlRuntime)->messageWindows[2] = itfMesCreateWindow(&D_0032A210);
    sndResetTransition();
    func_001F4430();
    btlResetDeferredTaskQueue();
    btlResetToInitialScene();
    fldClearSceneSlotsAndGroups();
    btlResetFieldColorAndSweepFlags();
    btlRefreshSoundEntries();
    btlRetainButtonTexture();
    func_001C45F0();
    func_001FB098();
    btlClearModelFlagRange();
}

extern char D_003A15A8[];

extern char D_003A15C8[];

extern void itfMesDestroyWindowIfPresent(s32);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A1598);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A15A8);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A15C8);

s32 btlExitWhenAudioAndTasksIdle(void) {
    if (btlRuntime == 0) {
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
    btlGetCurrentSceneRecordValue();
    btlFreeFieldBlocks();
    btlStopRainSoundTransition();
    btlClearTintAndEnableCamera();
    fldDestroySceneTasksAndBuffers();
    btlReleaseButtonTexture();
    sndFreeBattleSoundEntries();
    sndClearList();
    brsTaskTryDestroy();
    btlClearSoundAndModelResources();
    btlDestroyDrawTaskAtPriorityWhenPresent();
    itfMesDestroyWindowIfPresent(*(s32 *)(btlRuntime + 0x4A0));
    itfMesDestroyWindowIfPresent(*(s32 *)(btlRuntime + 0x49C));
    itfMesDestroyWindowIfPresent(*(s32 *)(btlRuntime + 0x498));
    btlAdvanceTitleStateWithAudioCleanup();
    kwlnCancelConfiguredFadeFrames();
    evtDestroySelectionState();
    effResetSlots();
    evtSetSolarOverlayFullyTransparent();
    itfMesClearFlags(1);
    sdfReleaseResourceAllocation(D_003BB2E0);
    D_003BB2E0 = 0;
    btlRuntime = 0;
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
    return btlRuntime != 0;
}

s32 btlIsCurrentActorFullyMarked(void) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    return (*(s32 *)(btlRuntime + 0x1f4) & 0x6000000) == 0x6000000;
}

s32 btlHasPendingRuntimeActivity(void) {
    s32 state;
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    state = btlRuntime;
    if (*(s32 *)(state + 0x224) != 0) {
        return 1;
    }
    if ((*(s32 *)(state + 0x1C8) & 2) != 0) {
        return 1;
    }
    return *(u32 *)(state + 0x694) != 0;
}

void btlResetActorEntryState(void) {
    s32 context = btlRuntime;
    s32 *entries = (s32 *)(datGameState + 0xBF8);
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
    memset((void *)(btlRuntime + 0x2B4), 0, 12);
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

s32 btlGetRuntime(void) {
    return btlRuntime;
}

/* Shared party/enemy entry prefix consumed by the HP/MP/status accessors. */
typedef struct BtlEntry {
    u16 flags;
    u8 pad2[4];
    u16 hp;     /* 0x06 */
    u16 maxHp;  /* 0x08: cached skill-adjusted maximum */
    u16 mp;     /* 0x0A */
    u16 maxMp;  /* 0x0C: cached skill-adjusted maximum */
    u16 status; /* 0x0E */
} BtlEntry;

#define BTL_ENTRY_STATUS_MASK 0x7FFF

/* Read current HP from a unit-entry address. */
u16 btlReadCurrentUnitHp(s32 entryAddress) {
    return ((BtlEntry *)entryAddress)->hp;
}

/* Read current MP from a unit-entry address. */
u16 btlReadCurrentUnitMp(s32 entryAddress) {
    return ((BtlEntry *)entryAddress)->mp;
}

void btlComputeProfileMaxHp(void) {
    ptyComputeMaxHp();
}

void btlComputeProfileMaxMp(void) {
    ptyComputeMaxMp();
}

u32 btlComputeSkillAdjustedMaxHp(s32 object) {
    return datComputeSkillBoostedMaxHp(object);
}

u32 btlComputeSkillAdjustedMaxMp(s32 object) {
    return datComputeSkillBoostedMaxMp(object);
}

void btlAdjustUnitHp(u8 *object, s32 value) {
    datAdjustCurrentHp(object, value);
}

void btlAdjustUnitMp(u8 *object, s32 value) {
    datAdjustCurrentMp(object, value);
}

/* Cache the skill-adjusted maximum and return current HP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumHpAndClampCurrentHp(s32 entryAddress) {
    u16 currentHp = btlReadCurrentUnitHp(entryAddress);
    u32 maxHp = btlComputeSkillAdjustedMaxHp(entryAddress);
    ((BtlEntry *)entryAddress)->maxHp = maxHp;
    if (maxHp < currentHp) {
        ((BtlEntry *)entryAddress)->hp = maxHp;
    }
    return ((BtlEntry *)entryAddress)->hp;
}

/* Cache the skill-adjusted maximum and return current MP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumMpAndClampCurrentMp(s32 entryAddress) {
    u16 currentMp = btlReadCurrentUnitMp(entryAddress);
    u32 maxMp = btlComputeSkillAdjustedMaxMp(entryAddress);
    ((BtlEntry *)entryAddress)->maxMp = maxMp;
    if (maxMp < currentMp) {
        ((BtlEntry *)entryAddress)->mp = maxMp;
    }
    return ((BtlEntry *)entryAddress)->mp;
}

/* Return the low 15 status bits; do not expose the stored high bit. */
u16 btlReadUnitStatusMask(s32 entryAddress) {
    return ((BtlEntry *)entryAddress)->status & BTL_ENTRY_STATUS_MASK;
}

void func_001A1948() {
    sdfRaisePackedChannelValue();
}
