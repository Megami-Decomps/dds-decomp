#include "sdk_ps2.h"
#include "eff_random.h"
#include "common.h"
#include "itf_mes_window_index.h"
#include "fld_next_scene_request_api.h"
#include "btl_boss_data_api.h"
#include "btl_event_asset_api.h"
#include "btl_skill_max_api.h"
#include "btl_raise_party_record_status_value.h"
#include "kwln_task_add_child.h"
#include "sdf_trig.h"
#include "snd_pad.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "kwln_task_priority_query.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "btl_state.h"
#include "btl_runtime_activity_status_api.h"
#include "mnu_result.h"
#include "sdf.h"
#include "kwln_task_lifecycle.h"
#include "itf_mes_resource.h"
#include "sdf_sif_command.h"
#include "mdl_flag_api.h"
#include "btl_enc_bg_entry.h"
#include "itf_color_panel_row.h"

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


extern SdfPoolNode sdfGrOtTable[];
extern s32 pbDebugPanel3State;
extern UiQuadColor itfTestOverlayColor;
extern u64 *pbAllocPrimPacket(u32, u32, u32, u32, u32, u32, u32);

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 datComputeSkillBoostedMaxHp(datUnitWork_t *);

extern u32 datComputeSkillBoostedMaxMp(datUnitWork_t *);

extern u64 btlAdvanceRuntimeSequenceCounter(void);

extern void btlUpdateScene(void);
extern void btlUpdateBattleCameraFrame(void);
extern BtlState *btlRuntime;


extern u64 D_003BB2E8;
#include "sdf_packet_alloc.h"

/* Option IDs index the same signed-byte bank used by the named controls. */
typedef struct ItfColorPanelWork {
    s16 editing;
    s16 selection;
    u32 blink;
    u8 pad08[8];
} ItfColorPanelWork;

typedef char ItfColorPanelWorkSizeCheck[sizeof(ItfColorPanelWork) == 0x10 ? 1 : -1];

extern ItfColorPanelWork pbDebugPanel3X;
extern ItfColorPanelRow itfOverlayColorEditorRows[6];
extern SndPad dds3PadButtonState;
extern SdfPoolNode kwlnPositionedTextSurface;
extern s32 itfTestOverlayPhaseStep;
extern u8 D_003BB2B4;
extern char D_003BB2B8[]; /* "FILTER:" */
extern char D_003BB2C0[]; /* ">" */
extern char D_003BB2C8[]; /* " %s" */
extern char D_003BB2D0[]; /* "%3d" */
extern char D_003BB2D8[]; /* "%4d" */
extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
void pbDrawRotatedPrim(s32 surfaceIndex);

/* Draw the four color channels and handle selection and value editing. */
/* Retail 0x001A0680 keeps the row < 4 select inside the four-row loop. */
s32 pbDebugPrintPanel3(void) {
    SifCommand packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_003BB2B8));
    if (pbDebugPanel3X.editing == 0) {
        sdfPktInit(&packet, 0x8980, pbDebugPanel3X.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        pbDebugPanel3X.blink++;
        if (pbDebugPanel3X.blink < 32 || (pbDebugPanel3X.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_003BB2C0));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfPktInit(&packet, 0x8980, y, 0xFF0000, itfOverlayColorEditorRows[row].highlight);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_003BB2C8, itfOverlayColorEditorRows[row].title));
        highlight = 0;
        if (pbDebugPanel3X.editing != 0 && row == pbDebugPanel3X.selection) {
            highlight = 6;
        }
        sdfPktInit(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB2D0, itfTestOverlayColor.channels[row]));
        } else {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB2D8, itfTestOverlayPhaseStep));
        }
        y += 0x60;
    }
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    pbDebugPanel3State += itfTestOverlayPhaseStep;
    pbDrawRotatedPrim(0x52);
    if ((s8)dds3PadButtonState.secondaryTriangle < 0) {
        D_003BB2B4 ^= 1;
    }
    if (pbDebugPanel3X.editing == 0) {
        previousSelection = pbDebugPanel3X.selection;
        if ((u8)dds3PadButtonState.secondaryUp & 2) {
            pbDebugPanel3X.selection--;
            if (pbDebugPanel3X.selection < 0) {
                pbDebugPanel3X.selection = 3;
            }
        }
        if ((u8)dds3PadButtonState.secondaryDown & 2) {
            pbDebugPanel3X.selection++;
            if (pbDebugPanel3X.selection >= 4) {
                pbDebugPanel3X.selection = 0;
            }
        }
        if (previousSelection != pbDebugPanel3X.selection) {
            pbDebugPanel3X.blink = 0;
        }
        if (dds3PadButtonState.secondaryCross < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
        if ((u8)dds3PadButtonState.secondaryCancel & 2) {
            return -1;
        }
    } else {
        if (pbDebugPanel3X.selection < 4) {
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                itfTestOverlayColor.channels[pbDebugPanel3X.selection] -= 10;
                if (itfTestOverlayColor.channels[pbDebugPanel3X.selection] < 0) {
                    itfTestOverlayColor.channels[pbDebugPanel3X.selection] = 0;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                itfTestOverlayColor.channels[pbDebugPanel3X.selection] += 10;
                if (itfTestOverlayColor.channels[pbDebugPanel3X.selection] > 255) {
                    itfTestOverlayColor.channels[pbDebugPanel3X.selection] = 255;
                }
            }
        } else {
            previousRate = itfTestOverlayPhaseStep;
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                itfTestOverlayPhaseStep -= 0x100;
                if (itfTestOverlayPhaseStep < 0) {
                    itfTestOverlayPhaseStep = 0;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                itfTestOverlayPhaseStep += 0x100;
                if (itfTestOverlayPhaseStep > 0x800) {
                    itfTestOverlayPhaseStep = 0x800;
                }
            }
            if (previousRate != itfTestOverlayPhaseStep) {
                pbDebugPanel3State = 0;
            }
        }
        if (dds3PadButtonState.secondaryCancel < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
    }
    return 0;
}


u64 *pbAllocPrimPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x80);

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

extern void *sdf2DMakeStdPacket(void *packet, s32 packetBytes);

u64 *pbMakeSpritePacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdf2DMakeStdPacket((void *)sdfGrAllocPacketBuff(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x48 : 0x47;
    return entry;
}

u64 *pbMakePrimPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdf2DMakeStdPacket((void *)sdfGrAllocPacketBuff(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x43 : 0x42;
    return entry;
}

void pbDrawRotatedPrim(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    u8 *component;
    sdfDrawTag_t *list;
    SdfPoolNode *surface;
    f32 phase;

    phase = (f32)(pbDebugPanel3State % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSin(phase) + 1.0f) * 0.5f * itfTestOverlayColor.alpha);
    component = (u8 *)&itfTestOverlayColor;
    for (i = 0; i != 3; i++, component += 4) {
        color |= *component << (i * 8);
    }
    color |= (u32)alpha << 24;
    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list, (u32)pbMakeSpritePacket(0x33001, 0));
    sdfDrawTagLink(list, (u32)pbMakePrimPacket(6, 0));
    sdfDrawTagLink(list, (u32)pbAllocPrimPacket(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->append(surface, list);
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
        evtBitOff(flagIndex);
        flagIndex = nextFlagIndex;
    } while (nextFlagIndex < 0xBFF);
}

extern s32 btlExitWhenAudioAndTasksIdle(void);
extern void btlPollMotionSoundResources(void);
extern void btlSweepFinishedTasks(void);
extern void btlUpdateActionSeqs(void);
extern void btlUpdateActiveActorModelState(void);
extern void btlUpdateActiveFrameNoOp(void);
extern void btlUpdateAutoMusic(void);
extern void btlUpdateFadeColor(void);
extern void btlUpdateTintAndWorldLight(void);
extern void fldAdvanceSceneGroupScheduler(void);

s32 btlUpdateActiveBattleFrame(dds3ProcessID_t *task) {
    BtlState *battle = btlRuntime;
    if (battle == NULL) {
        return 0;
    }
    if ((battle->battleFlags & 1) != 0) {
        btlUpdateFadeColor();
        btlUpdateAutoMusic();
        btlUpdateTintAndWorldLight();
        btlPollMotionSoundResources();
        btlUpdateScene();
        fldAdvanceSceneGroupScheduler();
        btlUpdateActionSeqs();
        btlUpdateActiveActorModelState();
        btlUpdateActiveFrameNoOp();
        btlSweepFinishedTasks();
        btlUpdateBattleCameraFrame();
        /* The callbacks above may replace the runtime; advance its current frame. */
        ++btlRuntime->frame;
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

extern void btlClearDeferredTasks(void);
extern void btlDispatchLinkedEffectWhenBattleGatesClear(void);
extern void btlSweepFloorModelLists(void);
extern void btlTickFieldSwayAndTint(void);
extern void btlUpdateActorModelColorAndLinks(void);
extern void btlUpdateEventPlayback(void);
extern void btlUpdateFieldPresentationNoOp(void);
extern void btlUpdateSceneFlow(void);

s32 btlUpdateBattleFieldPresentation(dds3ProcessID_t *task) {
    BtlState *battle = btlRuntime;
    if (battle == NULL) {
        return 0;
    }
    if ((battle->battleFlags & 1) != 0) {
        btlTickFieldSwayAndTint();
        btlDispatchLinkedEffectWhenBattleGatesClear();
        btlSweepFloorModelLists();
        btlUpdateActorModelColorAndLinks();
        btlUpdateEventPlayback();
        btlUpdateFieldPresentationNoOp();
        btlUpdateSceneFlow();
        btlClearDeferredTasks();
    }
    return 0;
}

/* Empty destructor callback for the active battle-frame task. */
void btlReleaseActiveBattleFrameTask(dds3ProcessID_t *task) {
}

extern char D_003BB2F0[];

extern char D_003A1598[]; /* "battle_draw" */


void btlCreateDrawTasks(void) {
    BtlState *battle = btlRuntime;
    dds3ProcessID_t *mainTask;
    dds3ProcessID_t *drawTask;
    mainTask = dds3StartProcess(D_003BB2F0, 0x3F9, 0, 0, btlUpdateActiveBattleFrame, btlReleaseActiveBattleFrameTask, 0);
    battle->scriptOwner = mainTask;
    drawTask = dds3StartProcess(D_003A1598, 0x2B0E, 0, 0, btlUpdateBattleFieldPresentation, 0, 0);
    dds3LinkProcess(mainTask, drawTask);
}

void btlDestroyDrawTaskAtPriorityWhenPresent(void) {
    dds3ProcessID_t *task;

    task = dds3SearchProcessPriority(0x3f9);
    if (task != NULL) {
        dds3KillProcess(btlRuntime->scriptOwner, 1);
        return;
    }
}

extern struct sdfMemHandle_t *D_003BB2E0;

extern u16 dds3VblanksPerFrame;

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

extern ItfMesSub *datStatusHelpMessages;

extern ItfMesSub D_0032A520;

extern ItfMesSub D_0032A210;

extern EffRandState effSharedRandomState[];



extern void sdfRandomize(s8 *, s32);

extern u32 dds3GetKernelCount(void);

extern ItfMesWindowIndex itfMesMngInitialize(ItfMesSub *);

extern void btlResetActorEntryState(void);

extern void sndResetTransition(void);

extern void btlResetMotionSoundFrameCounters(void);

extern void btlResetDeferredTaskQueue(void);

extern void btlResetToInitialScene(void);

extern void fldClearSceneSlotsAndGroups(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

extern void btlRetainButtonTexture(void);

extern void btlAllocatePanelTaskWork(void);

extern void btlInitializeRuntimeNoOp(void);


/* Allocate/reset battle work, initialize windows and queue shared resources. */
void btlInitializeBattleRuntime(void) {
    btlResetRuntimeSequenceCounter();
    D_003BB2E0 = sdfMemAllocDirect(0xE10);
    btlRuntime = (BtlState *)sdfMemLock(D_003BB2E0);
    memset(btlRuntime, 0, 0xE10);

    ((BattleInitState *)btlRuntime)->tick = 0;
    ((BattleInitState *)btlRuntime)->unk200 = 0;
    ((BattleInitState *)btlRuntime)->listHeads[0] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[1] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[2] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[3] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[4] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[5] = 0;
    dds3VblanksPerFrame = 2;
    btlRuntime->timingRate = 0x1E;
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

    sdfRandomize(effSharedRandomState, dds3GetKernelCount());
    btlResetActorEntryState();
    ((BattleInitState *)btlRuntime)->messageWindows[0] =
        itfMesMngInitialize(datStatusHelpMessages);
    ((BattleInitState *)btlRuntime)->messageWindows[1] = itfMesMngInitialize(&D_0032A520);
    ((BattleInitState *)btlRuntime)->messageWindows[2] = itfMesMngInitialize(&D_0032A210);
    sndResetTransition();
    btlResetMotionSoundFrameCounters();
    btlResetDeferredTaskQueue();
    btlResetToInitialScene();
    fldClearSceneSlotsAndGroups();
    btlResetFieldColorAndSweepFlags();
    btlRefreshSoundEntries();
    btlRetainButtonTexture();
    btlAllocatePanelTaskWork();
    btlInitializeRuntimeNoOp();
    btlClearModelFlagRange();
}

extern char D_003A15A8[];

extern char D_003A15C8[];

extern void itfMesMngRelease(ItfMesWindowIndex window);

extern s32 btlCountRegisteredTasks(void);
extern s32 sndHasOccupiedNodeSlots(void);
extern u32 brsTaskTryDestroy(void);
extern void btlAdvanceTitleStateWithAudioCleanup(void);
extern void btlClearPendingSoundList(void);
extern void btlClearSoundAndModelResources(void);
extern void btlClearTaskLists(void);
extern void btlClearTintAndEnableCamera(void);
extern void btlDestroyAllActionSeqs(void);
extern void btlDestroyAllUnits(void);
extern void btlFlagTasksForUpdate(void);
extern void btlFreeFieldBlocks(void);
extern void btlGetCurrentSceneRecordValue(void);
extern void btlReleaseButtonTexture(void);
extern void btlStopRainSoundTransition(void);
extern void effResetSlots(void);
extern void evtDestroySelectionState(void);
extern void evtDisableMoonDraw(void);
extern void fldDestroySceneTasksAndBuffers(void);
extern void itfMesClearFlags(u32 mask);
extern void dds3StopQuake(void);
extern void sndClearList(void);
extern void sndFreeBattleSoundEntries(void);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A1598);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A15A8);

INCLUDE_RODATA(const s32, "game/code_001A04C0", D_003A15C8);

s32 btlExitWhenAudioAndTasksIdle(void) {
    if (btlRuntime == NULL) {
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
    itfMesMngRelease(((BattleInitState *)btlRuntime)->messageWindows[2]);
    itfMesMngRelease(((BattleInitState *)btlRuntime)->messageWindows[1]);
    itfMesMngRelease(((BattleInitState *)btlRuntime)->messageWindows[0]);
    btlAdvanceTitleStateWithAudioCleanup();
    dds3StopQuake();
    evtDestroySelectionState();
    effResetSlots();
    evtDisableMoonDraw();
    itfMesClearFlags(1);
    sdfMemFreeDirect(D_003BB2E0);
    D_003BB2E0 = NULL;
    btlRuntime = NULL;
    btlBossDebugPrintf("** btlExit ***************\n");
    return 0;
}

extern void evtPrintDeveloperConsoleMessage(const char *format, ...);
extern void btlSetScene(s32 sceneId);
extern void itfMesSetFlags(s32 flags);

extern s32 btlResolveQueuedSceneRequestParameters(s32 *outCode, s32 *outParameter);

/* Start a battle: pick the scene parameters for the encounter and enter the battle scene. */
void btlStart(s32 mode, s32 pack, s32 encounter) {
    struct { s32 code; s32 parameter; } req;
    u16 paramA;
    u16 paramB;

    btlInitializeBattleRuntime();
    btlCreateDrawTasks();
    switch (mode) {
    case 0:
        btlRuntime->encounterPack = pack;
        btlRuntime->adjustmentGroupIndex = (u8)(encounter >> 24);
        btlRuntime->adjustmentEntryIndex = (u8)(encounter >> 16);
        btlRuntime->battleMode = (u16)encounter;
        btlBossDebugPrintf("enc[pack:%X,no:%X,index:%X,encNo:%X]\n", pack, (u8)(encounter >> 24), (u8)(encounter >> 16), (u16)encounter);
        if (pack > 0) {
            paramA = datBattleAdjustmentRecords[pack].encounterParamA;
            btlRuntime->encounterParamA = paramA;
            paramB = datBattleAdjustmentRecords[pack].encounterParamB;
            btlRuntime->encounterParamB = paramB;
            evtPrintDeveloperConsoleMessage("Pack:0x%X Maj:%d Min:%d\n", pack, (s16)paramA, (s16)paramB);
            if (btlRuntime->encounterParamA == 0 && btlRuntime->encounterParamB == 0) {
                if (btlResolveQueuedSceneRequestParameters(&req.code, &req.parameter) != 0) {
                    evtPrintDeveloperConsoleMessage("GetFldBG Maj:%d Min:%d\n", req.code, req.parameter);
                    paramA = req.code;
                    paramB = req.parameter;
                    btlRuntime->encounterParamA = paramA;
                    btlRuntime->encounterParamB = paramB;
                }
            }
        } else {
            btlRuntime->encounterParamA = datBattleSceneRecords[btlRuntime->battleMode].backgroundMajorId;
            btlRuntime->encounterParamB = datBattleSceneRecords[btlRuntime->battleMode].backgroundMinorId;
            if (btlRuntime->encounterParamA == 0 && btlRuntime->encounterParamB == 0) {
                if (btlResolveQueuedSceneRequestParameters(&req.code, &req.parameter) != 0) {
                    btlRuntime->encounterParamA = req.code;
                    btlRuntime->encounterParamB = req.parameter;
                } else {
                    btlRuntime->encounterParamA = 0xC9;
                    btlRuntime->encounterParamB = 1;
                }
            }
        }
        btlSetScene(2);
        break;
    case 1:
        btlSetScene(0xC);
        break;
    }
    if (btlRuntime->encounterParamA == 0 && btlRuntime->encounterParamB == 0) {
        btlRuntime->encounterParamA = 0xC9;
        btlRuntime->encounterParamB = 1;
    }
    itfMesSetFlags(1);
    evtBitOn(0xC0F);
    btlRuntime->battleFlags |= 0x701C1;
    btlRuntime->commandRestrictFlags |= 2;
    btlBossDebugPrintf("** btlStart **************\n");
}

extern void btlLoadStartupPanelResources(void);
extern void btlOpenButtonIconResource(void);
extern void sndLoadSysEffLb(void);

void btlLoadInputIconsAndSystemSounds(void) {
    btlOpenButtonIconResource();
    btlLoadStartupPanelResources();
    sndLoadSysEffLb();
}

u8 btlIsRuntimeAllocated(void) {
    return btlRuntime != NULL;
}

u8 btlIsCurrentActorFullyMarked(void) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    return (btlRuntime->battleFlags & 0x6000000) == 0x6000000;
}

BtlRuntimeActivityStatus btlHasPendingRuntimeActivity(void) {
    BtlState *battle;
    if (btlIsRuntimeAllocated() == 0) {
        return BTL_RUNTIME_ACTIVITY_NONE;
    }
    battle = btlRuntime;
    if (battle->tasks != NULL) {
        return BTL_RUNTIME_ACTIVITY_PENDING;
    }
    if ((battle->eventFlags & 2) != 0) {
        return BTL_RUNTIME_ACTIVITY_PENDING;
    }
    return battle->effect != NULL ? BTL_RUNTIME_ACTIVITY_PENDING
                                  : BTL_RUNTIME_ACTIVITY_NONE;
}

void btlResetActorEntryState(void) {
    BtlState *context = btlRuntime;
    datUnitWork_t *entries = DDS3_GBWK->party;
    u32 i;
    context->moneyEarned = 0;
    context->moneyTotal = 0;
    context->experienceEarned = 0;
    context->epEarned = 0;
    context->totalAp = 0;
    for (i = 0; i < 5; i++) {
        entries[i].huntExp = 0;

    }
    memset(btlRuntime->itemDrops, 0, sizeof(btlRuntime->itemDrops));
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
    runtime = btlRuntime;
    rewards->macca = (u32)runtime->moneyTotal;
    rewards->totalExp = runtime->experienceEarned;
    rewards->totalAp = runtime->totalAp;
    btlBossDebugPrintf(D_003A1698);
    btlBossDebugPrintf(D_003A16B8, rewards->macca);
    btlBossDebugPrintf(D_003A16C8, rewards->totalExp);
    btlBossDebugPrintf(D_003A16D8, rewards->totalAp);
    for (i = 0; i < 5; i++) {
        rewards->unitApBonus[i] = DDS3_GBWK->party[i].huntExp;
        btlBossDebugPrintf(D_003A16E8, rewards->unitApBonus[i], DDS3_GBWK->party[i].unitId);
    }
    btlBossDebugPrintf(D_003A1698);
    for (i = 0; i < 3; i++) {
        rewards->icons[i].id = btlRuntime->itemDrops[i].id;
        rewards->icons[i].param = btlRuntime->itemDrops[i].count;
    }
    return 1;
}


extern EncBgRow *D_003BAA44;

s32 btlResolveQueuedSceneRequestParameters(s32 *outCode, s32 *outParameter) {
    FldNextSceneRequestMapCode mapCode;
    FldNextSceneRequestEntryIndex entryIndex;
    s16 i;

    if (fldConsumeNextSceneRequest(&mapCode, &entryIndex) == FLD_NEXT_SCENE_REQUEST_NO_REQUEST) {
        return 0;
    }
    for (i = 0; i < 0x10; i++) {
        if (D_003BAA44[i].mapIdBase + 0xC8 != mapCode) {
            continue;
        }
        *outCode = D_003BAA44[i].mapIdBase + 0xC8;
        if (D_003BAA44[i].entries[entryIndex].flagB != -1 &&
            evtBitCheck(D_003BAA44[i].entries[entryIndex].flagB) != 0) {
            *outParameter = D_003BAA44[i].entries[entryIndex].parameterB;
            return 1;
        }
        if (D_003BAA44[i].entries[entryIndex].flagA != -1 &&
            evtBitCheck(D_003BAA44[i].entries[entryIndex].flagA) != 0) {
            *outParameter = D_003BAA44[i].entries[entryIndex].parameterA;
            return 1;
        }
        *outParameter = D_003BAA44[i].entries[entryIndex].fallbackParameter;
        return 1;
    }
    return 0;
}

void *btlGetRuntime(void) {
    return btlRuntime;
}


#define BTL_ENTRY_STATUS_MASK 0x7FFF

/* Read current HP from a unit-entry address. */
s32 btlReadCurrentUnitHp(datUnitWork_t *entryAddress) {
    return entryAddress->hp;
}

/* Word-return getter; native callers choose whether to truncate MP to 16 bits. */
s32 btlReadCurrentUnitMp(datUnitWork_t *entryAddress) {
    return entryAddress->mp;
}

extern s32 ptyComputeMaxHp(datUnitWork_t *);
extern s32 ptyComputeMaxMp(datUnitWork_t *);

void btlComputeProfileMaxHp(datUnitWork_t *unit) {
    ptyComputeMaxHp(unit);
}

void btlComputeProfileMaxMp(datUnitWork_t *unit) {
    ptyComputeMaxMp(unit);
}

u32 btlComputeSkillAdjustedMaxHp(datUnitWork_t *object) {
    return datComputeSkillBoostedMaxHp(object);
}

u32 btlComputeSkillAdjustedMaxMp(datUnitWork_t *object) {
    return datComputeSkillBoostedMaxMp(object);
}

extern s32 datAddHp(datUnitWork_t *, s32);
extern s32 datAddMp(datUnitWork_t *, s32);

s32 btlAdjustUnitHp(datUnitWork_t *object, s32 delta) {
    return datAddHp(object, delta);
}

s32 btlAdjustUnitMp(datUnitWork_t *object, s32 delta) {
    return datAddMp(object, delta);
}

/* Cache the skill-adjusted maximum and return current HP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumHpAndClampCurrentHp(datUnitWork_t *entryAddress) {
    datUnitWork_t *entry = entryAddress;
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
u16 btlRefreshUnitMaximumMpAndClampCurrentMp(datUnitWork_t *entryAddress) {
    u32 currentMp = btlReadCurrentUnitMp(entryAddress);
    u32 maxMp = btlComputeSkillAdjustedMaxMp(entryAddress);
    entryAddress->maxMp = maxMp;
    if (maxMp < currentMp) {
        entryAddress->mp = maxMp;
    }
    return entryAddress->mp;
}

/* Return the low 15 status bits; do not expose the stored high bit. */
u16 btlReadUnitStatusMask(datUnitWork_t *entry) {
    return entry->status & BTL_ENTRY_STATUS_MASK;
}

extern void sdfRaisePackedChannelValue(datUnitWork_t *item, u32 minimumValue);

/* Forward the record and value to the packed low-15-bit status update. */
void btlRaisePartyRecordStatusValue(datUnitWork_t *item, u32 minimumValue) {
    sdfRaisePackedChannelValue(item, minimumValue);
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
