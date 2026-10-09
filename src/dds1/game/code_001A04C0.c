#include "common.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "btl_state.h"
#include "mnu_result.h"
#include "sdf.h"
#include "kwln_task_lifecycle.h"
#include "itf_mes_resource.h"
#include "sdf_sif_command.h"

/* Named and indexed views of the same four signed 32-bit color channels. */
typedef union UiQuadColor {
    struct {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    };
    s32 channels[4];
} UiQuadColor;
typedef char UiQuadColorSizeCheck[sizeof(UiQuadColor) == 0x10 ? 1 : -1];


extern SdfPoolNode kwlnDrawSurfaces[];
extern s32 D_003BD824;
extern UiQuadColor D_00358390;
extern f32 sdfSinPoly(f32);
extern u64 *itfCreateVerticalGradientQuadPacket(u32, u32, u32, u32, u32, u32, u32);

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 datComputeSkillBoostedMaxHp(DatPartyRecord *);

extern u32 datComputeSkillBoostedMaxMp(DatPartyRecord *);

extern u64 btlAdvanceRuntimeSequenceCounter();

extern void btlUpdateScene(void);

extern s32 mdlFlagTest(u32);

extern s32 btlGetRuntime(void);


extern s32 btlRuntime;

extern s32 kwlnTaskFindByPriority(u32);

extern u64 D_003BB2E8;

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern s32 sdfAllocPacketAligned(s32 size);

/* Option IDs index the same signed-byte bank used by the named controls. */
typedef struct SndPad {
    u8 pad00[0x20];
    union {
        s8 buttons[0x20];
        struct {
            u8 pad20;
            s8 confirm;
            s8 edge22;
            s8 edge23;
            u8 pad24[2];
            s8 prev;
            s8 next;
            u8 pad28[9];
            s8 unk31;
            u8 unk32;
            s8 cancel;
            s8 coarseDown;
            s8 coarseUp;
            s8 unk36;
            s8 unk37;
            s8 fineDown;
            u8 pad39;
            s8 fineUp;
            u8 pad3B[5]; /* Complete two-port current/edge byte storage. */
        };
    };
} SndPad;


typedef struct ItfColorPanelWork {
    s16 editing;
    s16 selection;
    u32 blink;
    u8 pad08[8];
} ItfColorPanelWork;

typedef struct ItfColorPanelRow {
    const char *title;
    s32 highlight;
} ItfColorPanelRow;
typedef char ItfColorPanelWorkSizeCheck[sizeof(ItfColorPanelWork) == 0x10 ? 1 : -1];
typedef char ItfColorPanelRowSizeCheck[sizeof(ItfColorPanelRow) == 8 ? 1 : -1];

extern ItfColorPanelWork D_003D73E0;
extern ItfColorPanelRow D_00358360[6];
extern SndPad D_00324510;
extern SdfPoolNode kwlnPositionedTextSurface;
extern s32 D_003BD828;
extern u8 D_003BB2B4;
extern char D_003BB2B8[]; /* "FILTER:" */
extern char D_003BB2C0[]; /* ">" */
extern char D_003BB2C8[]; /* " %s" */
extern char D_003BB2D0[]; /* "%3d" */
extern char D_003BB2D8[]; /* "%4d" */
extern s32 func_0011D3E8(s32, s32, s32, s32, s32, u32, u32);
void itfDrawPulsingTestOverlay(s32 surfaceIndex);

/* Draw the four color channels and handle selection and value editing. */
/* Retail 0x001A0680 keeps the row < 4 select inside the four-row loop. */
s32 func_001A04C0(void) {
    SifCommand packet;
    SdfListHead *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = sdfCreateResetPacketList();
    sdfAppendPacket(list,
        func_0011D3E8(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_003BB2B8));
    if (D_003D73E0.editing == 0) {
        sdfPktInit(&packet, 0x8980, D_003D73E0.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        D_003D73E0.blink++;
        if (D_003D73E0.blink < 32 || (D_003D73E0.blink & 31) < 18) {
            sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_003BB2C0));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfPktInit(&packet, 0x8980, y, 0xFF0000, D_00358360[row].highlight);
        sdfAppendPacket(list,
            (u32)sdfFormatSifPacket(&packet, D_003BB2C8, D_00358360[row].title));
        highlight = 0;
        if (D_003D73E0.editing != 0 && row == D_003D73E0.selection) {
            highlight = 6;
        }
        sdfPktInit(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_003BB2D0, D_00358390.channels[row]));
        } else {
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_003BB2D8, D_003BD828));
        }
        y += 0x60;
    }
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, list);
    D_003BD824 += D_003BD828;
    itfDrawPulsingTestOverlay(0x52);
    if ((s8)D_00324510.unk32 < 0) {
        D_003BB2B4 ^= 1;
    }
    if (D_003D73E0.editing == 0) {
        previousSelection = D_003D73E0.selection;
        if ((u8)D_00324510.unk36 & 2) {
            D_003D73E0.selection--;
            if (D_003D73E0.selection < 0) {
                D_003D73E0.selection = 3;
            }
        }
        if ((u8)D_00324510.unk37 & 2) {
            D_003D73E0.selection++;
            if (D_003D73E0.selection >= 4) {
                D_003D73E0.selection = 0;
            }
        }
        if (previousSelection != D_003D73E0.selection) {
            D_003D73E0.blink = 0;
        }
        if (D_00324510.unk31 < 0) {
            D_003D73E0.editing ^= 1;
            D_003D73E0.blink = 0;
        }
        if ((u8)D_00324510.cancel & 2) {
            return -1;
        }
    } else {
        if (D_003D73E0.selection < 4) {
            if ((u8)D_00324510.coarseDown & 2) {
                D_00358390.channels[D_003D73E0.selection] -= 10;
                if (D_00358390.channels[D_003D73E0.selection] < 0) {
                    D_00358390.channels[D_003D73E0.selection] = 0;
                }
            }
            if ((u8)D_00324510.coarseUp & 2) {
                D_00358390.channels[D_003D73E0.selection] += 10;
                if (D_00358390.channels[D_003D73E0.selection] > 255) {
                    D_00358390.channels[D_003D73E0.selection] = 255;
                }
            }
        } else {
            previousRate = D_003BD828;
            if ((u8)D_00324510.coarseDown & 2) {
                D_003BD828 -= 0x100;
                if (D_003BD828 < 0) {
                    D_003BD828 = 0;
                }
            }
            if ((u8)D_00324510.coarseUp & 2) {
                D_003BD828 += 0x100;
                if (D_003BD828 > 0x800) {
                    D_003BD828 = 0x800;
                }
            }
            if (previousRate != D_003BD828) {
                D_003BD824 = 0;
            }
        }
        if (D_00324510.cancel < 0) {
            D_003D73E0.editing ^= 1;
            D_003D73E0.blink = 0;
        }
    }
    return 0;
}


u64 *itfCreateVerticalGradientQuadPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
    u64 *packet = (u64 *)sdfAllocPacketAligned(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = 0x14D;
    packet[5] = color0 | ((u64)0xFE00 << 46);
    packet[7] = color0 | ((u64)0xFE00 << 46);
    packet[9] = color1 | ((u64)0xFE00 << 46);
    packet[11] = color1 | ((u64)0xFE00 << 46);
    packet[6] = (u64)((x & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[8] = (u64)(((x + width) & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[10] = (u64)(((x + width) & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[12] = (u64)((x & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[13] = packet[14] = packet[15] = 0;
    return packet;
}

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
    SdfPoolNode *surface;
    f32 phase;

    phase = (f32)(D_003BD824 % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSinPoly(phase) + 1.0f) * 0.5f * D_00358390.alpha);
    component = (u8 *)&D_00358390;
    for (i = 0; i != 3; i++, component += 4) {
        color |= *component << (i * 8);
    }
    color |= (u32)alpha << 24;
    list = (s32)sdfCreateResetPacketList();
    sdfAppendPacket(list, (u32)btlCreateGsTestRegisterPacket(0x33001, 0));
    sdfAppendPacket(list, (u32)btlCreateGsAlphaRegisterPacket(6, 0));
    sdfAppendPacket(list, (u32)itfCreateVerticalGradientQuadPacket(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->append(surface, (SdfListHead *)list);
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

/* Clear the battle model flags; the exclusive upper limit differs by title. */
void btlClearModelFlagRange(void) {
    s32 nextFlagIndex;
    s32 flagIndex;

    flagIndex = 0xBE0;
    do {
        nextFlagIndex = flagIndex + 1;
        mdlFlagClear(flagIndex);
        flagIndex = nextFlagIndex;
    } while (nextFlagIndex < 0xBFF);
}

s32 btlUpdateActiveBattleFrame(void) {
    BtlState *battle = (BtlState *)btlRuntime;
    if (battle == NULL) {
        return 0;
    }
    if ((battle->battleFlags & 1) != 0) {
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
        /* The callbacks above may replace the runtime; advance its current frame. */
        ++((BtlState *)btlRuntime)->frame;
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

s32 btlUpdateBattleFieldPresentation(void) {
    BtlState *battle = (BtlState *)btlRuntime;
    if (battle == NULL) {
        return 0;
    }
    if ((battle->battleFlags & 1) != 0) {
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
        kwlnTaskDestroyWithHierarchy((KwlnTask *)*(u32 *)(btlRuntime + 0x29c), 1);
        return;
    }
}

extern s32 D_003BB2E0;

extern u16 mnuMovieTaskState;

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
    D_003BB2E0 = (u32)sdfAllocGeneralBlock(0xE10);
    btlRuntime = (s32)sdfResourceRetainAddress((struct SdfMemBlock *)(D_003BB2E0));
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
    ((BtlState *)btlRuntime)->timingRate = 0x1E;
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
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(D_003BB2E0));
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
    return (((BtlState *)btlRuntime)->battleFlags & 0x6000000) == 0x6000000;
}

s32 btlHasPendingRuntimeActivity(void) {
    BtlState *battle;
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    battle = (BtlState *)btlRuntime;
    if (battle->tasks != NULL) {
        return 1;
    }
    if ((battle->eventFlags & 2) != 0) {
        return 1;
    }
    return battle->effect != NULL;
}

void btlResetActorEntryState(void) {
    s32 context = btlRuntime;
    DatPartyRecord *entries = datGameState->party;
    u32 i;
    *(s32 *)(context + 0x2C0) = 0;
    *(s32 *)(context + 0x2C4) = 0;
    *(s32 *)(context + 0x2C8) = 0;
    *(s32 *)(context + 0x2CC) = 0;
    *(s32 *)(context + 0x2D0) = 0;
    for (i = 0; i < 5; i++) {
        entries[i].huntExp = 0;

    }
    memset((void *)(btlRuntime + 0x2B4), 0, 12);
}

extern const char D_003A1698[];
extern const char D_003A16B8[];
extern const char D_003A16C8[];
extern const char D_003A16D8[];
extern const char D_003A16E8[];

/* Snapshot battle rewards for the result screen. */
u32 btlSnapshotResultRewards(BrsRewardSummary *rewards) {
    BtlState *runtime;
    u32 i;

    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    runtime = (BtlState *)btlRuntime;
    rewards->macca = *(u32 *)runtime->pad2C4;
    rewards->totalExp = runtime->experienceEarned;
    rewards->totalAp = *(s32 *)runtime->pad2D0;
    btlBossDebugPrintf(D_003A1698);
    btlBossDebugPrintf(D_003A16B8, rewards->macca);
    btlBossDebugPrintf(D_003A16C8, rewards->totalExp);
    btlBossDebugPrintf(D_003A16D8, rewards->totalAp);
    for (i = 0; i < 5; i++) {
        rewards->unitApBonus[i] = datGameState->party[i].huntExp;
        btlBossDebugPrintf(D_003A16E8, rewards->unitApBonus[i], datGameState->party[i].unitId);
    }
    btlBossDebugPrintf(D_003A1698);
    for (i = 0; i < 3; i++) {
        rewards->icons[i].id = ((BtlState *)btlRuntime)->itemDrops[i].id;
        rewards->icons[i].param = ((BtlState *)btlRuntime)->itemDrops[i].count;
    }
    return 1;
}


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


#define BTL_ENTRY_STATUS_MASK 0x7FFF

/* Read current HP from a unit-entry address. */
s32 btlReadCurrentUnitHp(DatPartyRecord *entryAddress) {
    return entryAddress->hp;
}

/* Read current MP from a unit-entry address. */
u16 btlReadCurrentUnitMp(DatPartyRecord *entryAddress) {
    return entryAddress->mp;
}

extern s32 ptyComputeMaxHp(DatPartyRecord *);
extern s32 ptyComputeMaxMp(DatPartyRecord *);

void btlComputeProfileMaxHp(DatPartyRecord *unit) {
    ptyComputeMaxHp(unit);
}

void btlComputeProfileMaxMp(DatPartyRecord *unit) {
    ptyComputeMaxMp(unit);
}

u32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *object) {
    return datComputeSkillBoostedMaxHp(object);
}

u32 btlComputeSkillAdjustedMaxMp(DatPartyRecord *object) {
    return datComputeSkillBoostedMaxMp(object);
}

extern void datAdjustCurrentHp(DatPartyRecord *, s32);
extern void datAdjustCurrentMp(DatPartyRecord *, s32);

void btlAdjustUnitHp(DatPartyRecord *object, s32 value) {
    datAdjustCurrentHp(object, value);
}

void btlAdjustUnitMp(DatPartyRecord *object, s32 value) {
    datAdjustCurrentMp(object, value);
}

/* Cache the skill-adjusted maximum and return current HP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumHpAndClampCurrentHp(DatPartyRecord *entryAddress) {
    DatPartyRecord *entry = entryAddress;
    u32 currentHp = btlReadCurrentUnitHp(entry);
    u32 maxHp = btlComputeSkillAdjustedMaxHp(entryAddress);
    entry->maxHp = maxHp;
    if (maxHp < currentHp) {
        entry->hp = maxHp;
    }
    return entry->hp;
}

/* Cache the skill-adjusted maximum and return current MP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumMpAndClampCurrentMp(DatPartyRecord *entryAddress) {
    u16 currentMp = btlReadCurrentUnitMp(entryAddress);
    u32 maxMp = btlComputeSkillAdjustedMaxMp(entryAddress);
    entryAddress->maxMp = maxMp;
    if (maxMp < currentMp) {
        entryAddress->mp = maxMp;
    }
    return entryAddress->mp;
}

/* Return the low 15 status bits; do not expose the stored high bit. */
u16 btlReadUnitStatusMask(DatPartyRecord *entry) {
    return entry->status & BTL_ENTRY_STATUS_MASK;
}

void func_001A1948() {
    sdfRaisePackedChannelValue();
}

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A1698);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A16B8);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A16C8);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A16D8);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A16E8);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2B4);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2B8);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2C0);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2C8);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2D0);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2D8);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2E0);

INCLUDE_SDATA(const s32, "game/code_001A04C0", btlRuntime);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2E8);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2F0);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB2F8);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB300);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB308);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB310);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB318);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB320);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB328);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB330);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB338);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB340);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB348);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB350);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB358);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB360);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB368);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB370);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB378);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB380);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB388);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB390);

INCLUDE_SDATA(const s32, "game/code_001A04C0", D_003BB398);

