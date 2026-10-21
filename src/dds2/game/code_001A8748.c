#include "sdk_ps2.h"
#include "fld_area_work.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "btl_actor_status_table_api.h"
#include "itf_debug_panel_work.h"
#include "itf_color_panel_row.h"
#include "btl_boss_data_api.h"
#include "btl_event_asset_api.h"
#include "btl_skill_max_api.h"
#include "kwln_task_add_child.h"
#include "btl_special_ability.h"
#include "dat_unit_has_skill_api.h"
#include "btl_command_partner_output.h"
#include "btl_enc_bg_entry.h"
#include "btl_animation_mode_entry.h"
#include "btl_effect_position.h"
#include "evt_solar.h"
#include "eff_random_mod.h"
#include "eff_random.h"
#include "sdf_trig.h"
#include "snd_pad.h"
#include "btl_item_drop.h"
#include "itf_mes_window.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "kwln_task_priority_query.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "eff.h"
#include "itf.h"
#include "itf_panel_draw.h"
#include "itf_panel_api.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "btl_action.h"
#include "scr.h"
#include "dat_state.h"
#include "btl_resolve_unit_value_api.h"
#include "btl_select_transform_parameters_api.h"
#include "mnu_result.h"
#include "dat_command.h"
#include "sdf_sif_command.h"
#include "mdl_resource_table.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"
#include "btl_target_result.h"

extern s32 btlGetCommandUseError(BtlUnit *, s32);

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

extern s32 dds3FindEntryIndex();

extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);


extern const char *D_003B4CF8[];

extern u32 sndTestMessageResourceIndex;

extern SdfTex *sndTestMessageTexture;


extern u64 D_004366E8;

extern s32 btlRuntime;

extern u8 *D_00435DE0;

extern u8 *D_00435DF0;

extern u8 *D_00435DE4;

extern u8 *D_00435DF8;
extern s8 D_0037F530[];

extern u8 btlSharedActorTransformParameters[];

extern u8 *D_00435E00;

extern void *btlPartyUnitGunTypeNames[];

extern EncBgRow *D_00435E14;

extern s32 fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter);

typedef struct BattleController {
    u8 pad_000[0x214];
    s32 frame;
    u32 flags;
    u32 flags21C;
    u8 pad_220[0x28];
    ActionStateLink *taskHead; /* 0x248 */
    BtlUnit *actors;
    u8 pad_250[0x1E];
    u8 mode; /* 0x26E: mode 3 scales defeat experience in DDS2. */
    u8 pad26F;
    u16 variant; /* 0x270 */
    u8 pad_272[0x22];
    s32 adjustmentRecordIndex; /* 0x294 */
    s32 adjustmentGroupIndex;  /* 0x298 */
    s32 adjustmentEntryIndex;  /* 0x29C */
    u8 pad_2A0[0x24];
    KwlnTask *drawTask;
    u8 pad2C8[0x14];
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2E8 */
    u32 rewardMacca; /* 0x2EC */
    s32 experienceEarned; /* 0x2F0 */
    s32 epEarned; /* 0x2F4 */
    s32 totalAp; /* 0x2F8: defeat EP accumulated for the result screen. */
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103. */
    u8 pad2FE[0x3DE];
    s32 (*commandRangeOverride)(BtlUnit *, s32);
} BattleController;
typedef char BattleControllerTotalApOffsetCheck[
    ((u32)&((BattleController *)0)->totalAp == 0x2F8) ? 1 : -1];
typedef char BattleControllerDrawTaskOffsetCheck[
    ((u32)&((BattleController *)0)->drawTask == 0x2C4) ? 1 : -1];

extern s32 datCalculateCommandBaseValue(DatPartyRecord *, s32);

extern DatEnemyRecord *datEnemyRecords;

extern char D_00415158[];

extern char D_004150B0[]; /* "btl:hunt ep=%d[id=%X]\n" */

extern void btlBossDebugPrintf(const char *, ...);

extern EffRandState effSharedRandomState[];

extern void *btlCommandAttributeNames[];

extern f32 btlActiveGroupCountMultipliers[];



extern u16 mnuGetPartyEntryCurrentId(DatPartyRecord *);

extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);

extern s32 evtCheckValueThreshold(s32 itemId, s32 minimumQuantity);


extern s32 sndUpdateTestMsgTask(KwlnTask *task);

extern SdfPoolNode D_003805A8;

extern u8 D_003B4D08[];

extern u8 D_003B4D18[];

extern u8 D_003B4D28[];

#include "sdf_packet_alloc.h"

extern void itfSendTablePacket(SdfListHead *list, s32 context, s32 mode);

extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);

/* Option IDs index the same signed-byte bank used by the named controls. */
extern SndPad D_0037F510;

extern SdfPoolNode kwlnDrawSurfaces[];

#define BTL_ENTRY_STATUS_MASK 0x7FFF

extern u32 datComputeSkillBoostedMaxHp(DatPartyRecord *);

extern u32 datComputeSkillBoostedMaxMp(DatPartyRecord *);

extern SdfPoolNode D_00380708;


extern s32 D_00435CBC;


extern void itfPrintTestMessageCallback();

extern s32 itfUpdateTestMessageResourceInput(KwlnTask *task);

extern u8 D_003B4A28[];

extern s32 scrCreateTaskForProcessId();

void sndCycleTestMessageResource(void);

void sndCreateTestMsgTasks(void) {
    D_00435CBC = 0x80FFFFFF;
    sndCycleTestMessageResource();
    itfMesSetWindowCallbackAddress(((ScrData *)dds3GetProcessData((KwlnTask *)scrCreateTaskForProcessId(0x3E8, D_003B4A28, 0)))->resourceIndex, itfPrintTestMessageCallback);
    dds3StartProcess("TestMsgMngC", 0x3EF, 0, 0, itfUpdateTestMessageResourceInput, 0, 0);
    dds3StartProcess("TestMsgMngD", 0x2AFE, 0, 0, sndUpdateTestMsgTask, 0, 0);
}

void sndCycleTestMessageResource(void) {
    if (sndTestMessageTexture != 0) {
        sdfTexReleaseReferenceViaHandler(sndTestMessageTexture);
        sndTestMessageTexture = 0;
    }
    sndTestMessageResourceIndex = (sndTestMessageResourceIndex + 1) & 3;
    if (sndTestMessageResourceIndex != 3) {
        sndTestMessageTexture = itfLoadTextureFromAsset(D_003B4CF8[sndTestMessageResourceIndex]);
    }
}

s32 itfUpdateTestMessageResourceInput(KwlnTask *task) {
    if (D_0037F530[0] < 0) {
        sndCycleTestMessageResource();
    }
    return 0;
}

s32 sndUpdateTestMsgTask(KwlnTask *task) {
    SdfListHead *mem;
    if ((sndTestMessageTexture != 0) && (sndTestMessageResourceIndex != 3)) {
        mem = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(mem);
        itfSendTablePacket(mem, 0, 0);
        itfQueueTextureBoundQuadPacket(D_003B4D08, D_003B4D18, D_003B4D28, 0xFFF, sndTestMessageTexture, 0, mem);
        D_003805A8.append(&D_003805A8, mem);
        return 0;
    }
    return 0;
}

#include "sdf_debug_api.h"

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414EB0);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414EC8);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414EE0);

void itfPrintTestMessageCallback(void) {
    sdfDebugPrintf("********* AAAA ********\n");
}

typedef struct ItfFovPanelWork {
    u8 pad00[8];
    f32 degrees;
    u8 pad0C[4];
} ItfFovPanelWork;

typedef char ItfFovPanelWorkSizeCheck[sizeof(ItfFovPanelWork) == 0x10 ? 1 : -1];

extern ItfFovPanelWork D_00452E70;

extern s8 D_0037F543[];

extern u64 *sdfCreateBorderedRectanglePacket(s32, s32, s32, s32, s32, u32, u32);

extern s32 itfStepFloatWithPad(f32 *, f32, f32, f32, f32);

s32 btlEditCameraFieldOfView(void) {
    SifCommand packet;
    SdfListHead *list;
    f32 radiansToDegrees = 57.2957795f;

    list = sdfCreateResetPacketList();
    sdfAppendPacket(list,
                    (u32)sdfCreateBorderedRectanglePacket(0x8500, 0x79C0, 0xFEFFFF,
                                       0xA80, 0x120, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x85C0, 0x7A20, 0xFF0000, 0);
    sdfAppendPacket(list,
                    (u32)sdfFormatSifPacket(&packet, "FOVY: %6.2f",
                        sdfSceneProjectionParameters.camera.fov * radiansToDegrees));
    D_00380708.append(&D_00380708, list);
    if (D_0037F543[0] < 0) {
        return -1;
    }
    D_00452E70.degrees = sdfSceneProjectionParameters.camera.fov * radiansToDegrees;
    if (itfStepFloatWithPad(&D_00452E70.degrees, 1.0f, 89.0f, 0.1f, 1.0f) != 0) {
        sdfSceneProjectionParameters.camera.fov = D_00452E70.degrees * 0.0174532925f;
    }
    return 0;
}

s32 itfStepFloatWithPad(f32 *angleDegrees, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep) {
    f32 current = *angleDegrees;
    s32 changed = 0;

    if (D_0037F510.coarseUp < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (D_0037F510.coarseUp & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if (D_0037F510.coarseDown < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (D_0037F510.coarseDown & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if (D_0037F510.fineUp < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (D_0037F510.fineUp & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if (D_0037F510.fineDown < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (D_0037F510.fineDown & 2) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        *angleDegrees = current;
        return 1;
    }
    return 0;
}

extern ItfDebugPanelWork D_00452E80;

extern const char *D_003B4D38[6];

extern u8 kwlnDrawOverlayEnabled;

extern s16 kwlnDrawOverlayAlpha;

extern s16 kwlnDrawOverlayScale;

extern s32 D_00435CE0[2];

extern char D_00436660[];

extern char D_00436668[];

extern char D_00436670[];

extern char D_00436678[];

extern char D_00436680[];

extern char D_00436688[];

extern SdfPoolNode kwlnPositionedTextSurface;

/* Draw the blur settings and handle selection, editing and cancellation. */
s32 kwlnUpdateBlurDebugMenu(void) {
    SifCommand packet;
    SdfListHead *list;
    s32 row;
    s32 y;
    s16 previous;
    s32 highlight;

    list = sdfCreateResetPacketList();
    sdfAppendPacket(list,
        (u32)sdfCreateBorderedRectanglePacket(0x8290, 0x79A8, 0xFEFFFF, 0xC60, 0x1B0, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x82C0, 0x79C0, 0xFF0000, 0);
    sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet,
        "BLUR:        %s", kwlnDrawOverlayEnabled ? D_00436660 : D_00436668));

    y = 0x7A20;
    if (D_00452E80.editing == 0) {
        sdfPktInit(&packet, 0x82C0, D_00452E80.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        D_00452E80.blink++;
        if (D_00452E80.blink < 32 || (D_00452E80.blink & 31) < 18) {
            sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_00436670));
        }
    }

    for (row = 0; row != 3; row++) {
        sdfPktInit(&packet, 0x82C0, y, 0xFF0000, 0);
        sdfAppendPacket(list,
            (u32)sdfFormatSifPacket(&packet, D_00436678, D_003B4D38[row]));
        highlight = 0;
        if (D_00452E80.editing != 0 && row == D_00452E80.selection) {
            highlight = 6;
        }
        sdfPktInit(&packet, 0x8800, y, 0xFF0000, highlight);
        switch (row) {
        case 0:
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_00436680, kwlnDrawOverlayAlpha));
            break;
        case 1:
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_00436680, kwlnDrawOverlayScale));
            break;
        case 2:
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_00436688, D_00435CE0[0], D_00435CE0[1]));
            break;
        }
        y += 0x60;
    }
    sdfAppendPacket(list,
        (u32)sdfCreateBorderedRectanglePacket((D_00435CE0[0] * 16) + 0x7FC0, (D_00435CE0[1] * 8) + 0x7FE0,
            0xFF0000, 0x80, 0x40, 0x80008080, 0x80000000));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);

    if ((s8)D_0037F510.secondaryTriangle < 0) {
        kwlnDrawOverlayEnabled ^= 1;
    }
    if (D_00452E80.editing == 0) {
        previous = D_00452E80.selection;
        if ((u8)D_0037F510.secondaryUp & 2) {
            D_00452E80.selection--;
            if (D_00452E80.selection < 0) {
                D_00452E80.selection = 2;
            }
        }
        if ((u8)D_0037F510.secondaryDown & 2) {
            D_00452E80.selection++;
            if (D_00452E80.selection >= 3) {
                D_00452E80.selection = 0;
            }
        }
        if (previous != D_00452E80.selection) {
            D_00452E80.blink = 0;
        }
        if (D_0037F510.secondaryCross < 0) {
            D_00452E80.editing ^= 1;
            D_00452E80.blink = 0;
        }
        if ((u8)D_0037F510.secondaryCancel & 2) {
            return -1;
        }
    } else {
        if (D_00452E80.selection == 2) {
            if (D_0037F510.secondaryUp != 0) {
                D_00435CE0[1] -= 16;
                if (D_00435CE0[1] < -224) {
                    D_00435CE0[1] = -224;
                }
            }
            if (D_0037F510.secondaryDown != 0) {
                D_00435CE0[1] += 16;
                if (D_00435CE0[1] > 224) {
                    D_00435CE0[1] = 224;
                }
            }
            if (D_0037F510.coarseDown != 0) {
                D_00435CE0[0] -= 16;
                if (D_00435CE0[0] < -256) {
                    D_00435CE0[0] = -256;
                }
            }
            if (D_0037F510.coarseUp != 0) {
                D_00435CE0[0] += 16;
                if (D_00435CE0[0] > 256) {
                    D_00435CE0[0] = 256;
                }
            }
        } else if (D_00452E80.selection == 0) {
            if ((u8)D_0037F510.coarseDown & 2) {
                kwlnDrawOverlayAlpha -= 10;
                if (kwlnDrawOverlayAlpha < 0) {
                    kwlnDrawOverlayAlpha = 0;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                kwlnDrawOverlayAlpha += 10;
                if (kwlnDrawOverlayAlpha > 255) {
                    kwlnDrawOverlayAlpha = 255;
                }
            }
        } else {
            if ((u8)D_0037F510.coarseDown & 2) {
                kwlnDrawOverlayScale -= 10;
                if (kwlnDrawOverlayScale < -255) {
                    kwlnDrawOverlayScale = -255;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                kwlnDrawOverlayScale += 10;
                if (kwlnDrawOverlayScale > 255) {
                    kwlnDrawOverlayScale = 255;
                }
            }
        }
        if (D_0037F510.secondaryCancel < 0) {
            D_00452E80.editing ^= 1;
            D_00452E80.blink = 0;
        }
    }
    return 0;
}

extern ItfDebugPanelWork itfOverlayColorEditorWork;

extern ItfColorPanelRow itfOverlayColorEditorRows[6];

extern s32 itfTestOverlayPhase;

extern UiQuadColor itfTestOverlayColor;

extern s32 itfTestOverlayPhaseStep;

extern u8 D_004366B4;

extern char D_004366B8[]; /* "FILTER:" */

extern char D_004366C0[]; /* ">" */

extern char D_004366C8[]; /* " %s" */

extern char D_004366D0[]; /* "%3d" */

extern char D_004366D8[]; /* "%4d" */

void itfDrawPulsingTestOverlay(s32 surfaceIndex);

/* Draw the four color channels and handle selection and value editing. */
/* Retail 0x001A92F0 keeps the row < 4 select inside the four-row loop. */
s32 itfEditOverlayColor(void) {
    SifCommand packet;
    SdfListHead *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = sdfCreateResetPacketList();
    sdfAppendPacket(list,
        (u32)sdfCreateBorderedRectanglePacket(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_004366B8));
    if (itfOverlayColorEditorWork.editing == 0) {
        sdfPktInit(&packet, 0x8980, itfOverlayColorEditorWork.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        itfOverlayColorEditorWork.blink++;
        if (itfOverlayColorEditorWork.blink < 32 || (itfOverlayColorEditorWork.blink & 31) < 18) {
            sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_004366C0));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfPktInit(&packet, 0x8980, y, 0xFF0000, itfOverlayColorEditorRows[row].highlight);
        sdfAppendPacket(list,
            (u32)sdfFormatSifPacket(&packet, D_004366C8, itfOverlayColorEditorRows[row].title));
        highlight = 0;
        if (itfOverlayColorEditorWork.editing != 0 && row == itfOverlayColorEditorWork.selection) {
            highlight = 6;
        }
        sdfPktInit(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_004366D0, itfTestOverlayColor.channels[row]));
        } else {
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_004366D8, itfTestOverlayPhaseStep));
        }
        y += 0x60;
    }
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    itfTestOverlayPhase += itfTestOverlayPhaseStep;
    itfDrawPulsingTestOverlay(0x52);
    if ((s8)D_0037F510.secondaryTriangle < 0) {
        D_004366B4 ^= 1;
    }
    if (itfOverlayColorEditorWork.editing == 0) {
        previousSelection = itfOverlayColorEditorWork.selection;
        if ((u8)D_0037F510.secondaryUp & 2) {
            itfOverlayColorEditorWork.selection--;
            if (itfOverlayColorEditorWork.selection < 0) {
                itfOverlayColorEditorWork.selection = 3;
            }
        }
        if ((u8)D_0037F510.secondaryDown & 2) {
            itfOverlayColorEditorWork.selection++;
            if (itfOverlayColorEditorWork.selection >= 4) {
                itfOverlayColorEditorWork.selection = 0;
            }
        }
        if (previousSelection != itfOverlayColorEditorWork.selection) {
            itfOverlayColorEditorWork.blink = 0;
        }
        if (D_0037F510.secondaryCross < 0) {
            itfOverlayColorEditorWork.editing ^= 1;
            itfOverlayColorEditorWork.blink = 0;
        }
        if ((u8)D_0037F510.secondaryCancel & 2) {
            return -1;
        }
    } else {
        if (itfOverlayColorEditorWork.selection < 4) {
            if ((u8)D_0037F510.coarseDown & 2) {
                itfTestOverlayColor.channels[itfOverlayColorEditorWork.selection] -= 10;
                if (itfTestOverlayColor.channels[itfOverlayColorEditorWork.selection] < 0) {
                    itfTestOverlayColor.channels[itfOverlayColorEditorWork.selection] = 0;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                itfTestOverlayColor.channels[itfOverlayColorEditorWork.selection] += 10;
                if (itfTestOverlayColor.channels[itfOverlayColorEditorWork.selection] > 255) {
                    itfTestOverlayColor.channels[itfOverlayColorEditorWork.selection] = 255;
                }
            }
        } else {
            previousRate = itfTestOverlayPhaseStep;
            if ((u8)D_0037F510.coarseDown & 2) {
                itfTestOverlayPhaseStep -= 0x100;
                if (itfTestOverlayPhaseStep < 0) {
                    itfTestOverlayPhaseStep = 0;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                itfTestOverlayPhaseStep += 0x100;
                if (itfTestOverlayPhaseStep > 0x800) {
                    itfTestOverlayPhaseStep = 0x800;
                }
            }
            if (previousRate != itfTestOverlayPhaseStep) {
                itfTestOverlayPhase = 0;
            }
        }
        if (D_0037F510.secondaryCancel < 0) {
            itfOverlayColorEditorWork.editing ^= 1;
            itfOverlayColorEditorWork.blink = 0;
        }
    }
    return 0;
}

u64 *sdfCreateGradientQuadPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
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

extern u32 sdfConsFinalizePacketHeader(u32 packetAddress, s32 packetBytes);

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


extern u64 *sdfCreateGradientQuadPacket(u32, u32, u32, u32, u32, u32, u32);

void itfDrawPulsingTestOverlay(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    u8 *component;
    SdfListHead *list;
    SdfPoolNode *surface;
    f32 phase;

    phase = (f32)(itfTestOverlayPhase % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSinPoly(phase) + 1.0f) * 0.5f * itfTestOverlayColor.alpha);
    component = (u8 *)&itfTestOverlayColor;
    for (i = 0; i != 3; i++, component += 4) {
        color |= *component << (i * 8);
    }
    color |= alpha << 24;
    list = sdfCreateResetPacketList();
    sdfAppendPacket(list, (u32)btlCreateGsTestRegisterPacket(0x33001, 0));
    sdfAppendPacket(list, (u32)btlCreateGsAlphaRegisterPacket(6, 0));
    sdfAppendPacket(list, (u32)sdfCreateGradientQuadPacket(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->append(surface, list);
}

void btlResetRuntimeSequenceCounter(void) {
    D_004366E8 = 1;
}

u64 btlAdvanceRuntimeSequenceCounter(void) {
    s64 value = D_004366E8 + 1;

    if (value < 0) {
        value = 1;
        D_004366E8 = value;
    } else {
        D_004366E8 = value;
    }
    return value;
}

/* Clear the battle model flags; the exclusive upper limit differs by title. */
void btlClearModelFlagRange(void) {
    s32 flagIndex;

    for (flagIndex = 0xBE0; flagIndex < 0xC00; flagIndex++) {
        mdlFlagClear(flagIndex);
    }
}

extern s32 btlUpdateFadeColor(void);

extern s32 btlUpdateAutoMusic(void);

extern s32 btlUpdateTintAndWorldLight(void);

extern void btlUpdateMotionSoundLoading(void);

extern s32 btlUpdateScene(void);

extern s32 fldAdvanceSceneGroupScheduler(void);

extern s32 btlUpdateActionSeqs(void);

extern s32 btlUpdateActiveActorModelState(void);

extern void btlUpdateActiveFrameNoOp(void);

extern s32 btlSweepFinishedTasks(void);

extern void btlUpdateBattleCameraFrame(void);

extern s32 btlExitWhenAudioAndTasksIdle(void);

s32 btlUpdateActiveBattleFrame(KwlnTask *task) {
    if (btlRuntime == 0) {
        return 0;
    }
    if (((BattleController *)btlRuntime)->flags & 1) {
        btlUpdateFadeColor();
        btlUpdateAutoMusic();
        btlUpdateTintAndWorldLight();
        btlUpdateMotionSoundLoading();
        btlUpdateScene();
        fldAdvanceSceneGroupScheduler();
        btlUpdateActionSeqs();
        btlUpdateActiveActorModelState();
        btlUpdateActiveFrameNoOp();
        btlSweepFinishedTasks();
        btlUpdateBattleCameraFrame();
        ((BattleController *)btlRuntime)->frame = ((BattleController *)btlRuntime)->frame + 1;
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

extern s32 btlTickFieldSwayAndTint(void);

extern s32 btlReleaseRainSoundTransition(void);

extern s32 btlSweepFloorModelLists(void);

extern s32 btlUpdateActorModelColorAndLinks(void);

extern void btlUpdateEventPlayback(void);

extern void btlUpdateFieldPresentationNoOp(void);

extern s32 btlUpdateSceneFlow(void);

extern s32 btlClearDeferredTasks(void);

s32 btlUpdateBattleFieldPresentation(KwlnTask *task) {
    BattleController *battle = (BattleController *)btlRuntime;
    if (battle == 0) {
        return 0;
    }
    if (battle->flags & 1) {
        btlTickFieldSwayAndTint();
        btlReleaseRainSoundTransition();
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
void btlReleaseActiveBattleFrameTask(KwlnTask *task) {
}

extern char D_004366F0[];

extern void btlReleaseActiveBattleFrameTask(KwlnTask *task);

extern s32 btlUpdateBattleFieldPresentation(KwlnTask *task);

extern s32 btlUpdateActiveBattleFrame(KwlnTask *task);

void btlCreateDrawTasks(void) {
    BattleController *battle = (BattleController *)btlRuntime;
    KwlnTask *draw;
    draw = dds3StartProcess(D_004366F0, 0x3F9, 0, 0, btlUpdateActiveBattleFrame, btlReleaseActiveBattleFrameTask, 0);
    battle->drawTask = draw;
    dds3LinkProcess(draw, dds3StartProcess("battle_draw", 0x2B0E, 0, 0, btlUpdateBattleFieldPresentation, 0, 0));
}

void btlDestroyDrawTaskAtPriorityWhenPresent(void) {
    KwlnTask *task;

    task = dds3SearchProcessPriority(0x3f9);
    if (task != 0) {
        BattleController *battle = (BattleController *)btlRuntime;
        dds3KillProcess(battle->drawTask, 1);
        return;
    }
}

typedef struct BattleInitState {
    u8 pad000[0x214];
    u32 tick;
    u8 pad218[4];
    u32 commandRestrictFlags;
    u8 pad220[4];
    u32 unk224;
    u8 pad228[0x20];
    u32 listHeads[6];
    u8 pad260[0x1C];
    u8 endCode;
    u8 pad27D[0x2F];
    u16 backgroundA;
    u16 backgroundB;
    u32 unk2B0;
    u8 unk2B4;
    u8 pad2B5[0x20F];
    u8 timingRate;
    u8 pad4C5[3];
    f32 modelFrameScale;
    s32 messageWindows[3];
    u8 pad4D8[0xE0];
    u8 unk5B8;
    u8 pad5B9[3];
    u32 unk5BC;
} BattleInitState;

extern SdfMemBlock *D_004366E0;

extern ItfMesSub *D_00435E78;

extern ItfMesSub D_003858D8;

extern ItfMesSub D_00385228;

extern u16 mnuMovieTaskState;

extern u32 kwlnGetFrameCounter(void);

extern u32 btlIsCurrentDarkZone(void);

extern void sdfRandomize(void *, u32);

extern s32 itfMesCreateWindow(ItfMesSub *);

extern void btlResetActorEntryState(void);

extern char D_004366F8[];

extern char D_003B4D90[];

extern char D_003B4DA8[];

extern void btlAllocatePanelTaskWork(void);
extern void btlInitializeRuntimeNoOp(void);
extern void btlRefreshSoundEntries(void);
extern void btlResetDeferredTaskQueue(void);
extern void btlResetFieldColorAndSweepFlags(void);
extern void btlResetMotionSoundFrameCounters(void);
extern void btlResetToInitialScene(void);
extern void btlRetainButtonTexture(void);
extern void fldClearSceneSlotsAndGroups(void);
extern void sndResetTransition(void);

/* Allocate/reset battle work, initialize windows and queue shared resources. */
void btlInitializeBattleRuntime(void) {
    SdfMemBlock *allocation;
    u32 seed;
    u8 *runtime;

    btlResetRuntimeSequenceCounter();
    allocation = sdfMemAllocDirect(0xFD4);
    D_004366E0 = allocation;
    runtime = (u8 *)sdfMemLock(allocation);
    btlRuntime = (s32)runtime;
    memset(runtime, 0, 0xFD4);

    ((BattleInitState *)btlRuntime)->tick = 0;
    ((BattleInitState *)btlRuntime)->unk224 = 0;
    ((BattleInitState *)btlRuntime)->listHeads[0] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[1] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[2] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[3] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[4] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[5] = 0;
    mnuMovieTaskState = 2;
    ((BattleInitState *)btlRuntime)->timingRate = 0x1E;
    {
        BattleInitState *state = (BattleInitState *)btlRuntime;
        state->endCode = 0;
        state->modelFrameScale = 1.0f;
    }
    ((BattleInitState *)btlRuntime)->unk5B8 = 0;
    ((BattleInitState *)btlRuntime)->unk5BC = 0x80808080;
    ((BattleInitState *)btlRuntime)->backgroundA = 0xC9;
    ((BattleInitState *)btlRuntime)->backgroundB = 1;
    ((BattleInitState *)btlRuntime)->unk2B0 = 0;
    ((BattleInitState *)btlRuntime)->unk2B4 = btlIsCurrentDarkZone();
    seed = kwlnGetFrameCounter();
    sdfRandomize(effSharedRandomState, seed);
    btlResetActorEntryState();

    ((BattleInitState *)btlRuntime)->messageWindows[0] =
        itfMesCreateWindow(D_00435E78);
    ((BattleInitState *)btlRuntime)->messageWindows[1] =
        itfMesCreateWindow(&D_003858D8);
    ((BattleInitState *)btlRuntime)->messageWindows[2] =
        itfMesCreateWindow(&D_00385228);
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
    mdlFlagClear(0x82B);

    if (mdlFlagTest(0x290) == 0) {
        mdlSetViewerSlotResourceHandles(0, 1, 0, D_003B4D90, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4D90);
    } else {
        mdlSetViewerSlotResourceHandles(0, 1, 0, D_003B4DA8, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4DA8);
    }
}

extern void itfMesDestroyWindowIfPresent(s32);

extern char D_00414F48[];

extern char D_00414F68[];

extern char D_00414F80[];

extern char D_00414F98[];

extern char D_00414FB0[];

extern s32 sndHasOccupiedNodeSlots(void);

extern s32 btlCountRegisteredTasks(void);

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
extern void evtSetSolarOverlayFullyTransparent(void);
extern void fldDestroySceneTasksAndBuffers(void);
extern void itfMesClearFlags(u32 mask);
extern void kwlnCancelConfiguredFadeFrames(void);
extern void sndClearList(void);
extern void sndFreeBattleSoundEntries(void);

s32 btlExitWhenAudioAndTasksIdle(void) {
    BattleInitState *state;

    if (btlRuntime == 0) {
        btlBossDebugPrintf(D_00414F48);
        return 0;
    }
    if (sndHasOccupiedNodeSlots() != 0) {
        btlBossDebugPrintf(D_00414F68);
        return -1;
    }
    if (btlCountRegisteredTasks() != 0) {
        btlBossDebugPrintf(D_00414F80);
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
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[2]);
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[1]);
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[0]);
    btlAdvanceTitleStateWithAudioCleanup();
    kwlnCancelConfiguredFadeFrames();
    evtDestroySelectionState();
    effResetSlots();
    evtSetSolarOverlayFullyTransparent();
    itfMesClearFlags(1);

    state = (BattleInitState *)btlRuntime;
    if (state->commandRestrictFlags & 0x8000) {
        state->commandRestrictFlags &= ~0x8000;
        sdfSceneProjectionParameters.camera.farZ = 65536.0f;
        btlBossDebugPrintf(D_00414F98);
    }

    sdfMemFreeDirect(D_004366E0);
    D_004366E0 = 0;
    btlRuntime = 0;
    btlBossDebugPrintf(D_00414FB0);
    return 0;
}

extern void evtPrintDeveloperConsoleMessage(const char *, ...);

extern void btlSetScene(s32);

extern void itfMesSetFlags(u32);

extern s32 btlResolveQueuedSceneRequestParameters(s32 *, s32 *);

/* Start the requested battle mode with its encounter and background parameters. */
INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414F48);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414F68);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414F80);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414F98);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00414FB0);

void btlStart(s32 mode, s32 pack, s32 encounter) {
    struct { s32 code; s32 parameter; } req;
    u16 paramA;
    u16 paramB;

    btlInitializeBattleRuntime();
    btlCreateDrawTasks();
    switch (mode) {
    case 0:
        ((BtlState *)btlRuntime)->effectLayer = pack;
        ((BtlState *)btlRuntime)->adjustmentGroupIndex = (u8)(encounter >> 24);
        ((BtlState *)btlRuntime)->adjustmentEntryIndex = (u8)(encounter >> 16);
        ((BtlState *)btlRuntime)->battleMode = (u16)encounter;
        btlBossDebugPrintf("enc[pack:%X,no:%X,index:%X,encNo:%X]\n", pack, (u8)(encounter >> 24), (u8)(encounter >> 16), (u16)encounter);
        if (pack > 0) {
            ((BtlState *)btlRuntime)->background.ids.major = D_00435E0C[pack].encounterParamA;
            ((BtlState *)btlRuntime)->background.ids.minor = D_00435E0C[pack].encounterParamB;
            if (((BtlState *)btlRuntime)->battleMode == 0x3A0 && mdlFlagTest(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xDF;
                ((BtlState *)btlRuntime)->background.ids.minor = 5;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x3A1 && mdlFlagTest(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE0;
                ((BtlState *)btlRuntime)->background.ids.minor = 6;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x33B && mdlFlagTest(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE2;
                ((BtlState *)btlRuntime)->background.ids.minor = 1;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x340 && mdlFlagTest(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE4;
                ((BtlState *)btlRuntime)->background.ids.minor = 2;
            }
            evtPrintDeveloperConsoleMessage("Pack:0x%X Maj:%d Min:%d\n", pack, ((BtlState *)btlRuntime)->background.ids.major, ((BtlState *)btlRuntime)->background.ids.minor);
            if (((BtlState *)btlRuntime)->background.ids.major == 0 && ((BtlState *)btlRuntime)->background.ids.minor == 0) {
                if (btlResolveQueuedSceneRequestParameters(&req.code, &req.parameter) != 0) {
                    evtPrintDeveloperConsoleMessage("GetFldBG Maj:%d Min:%d\n", req.code, req.parameter);
                    paramA = req.code;
                    paramB = req.parameter;
                    ((BtlState *)btlRuntime)->background.ids.major = paramA;
                    ((BtlState *)btlRuntime)->background.ids.minor = paramB;
                }
            }
        } else {
            ((BtlState *)btlRuntime)->background.ids.major = datBattleSceneRecords[((BtlState *)btlRuntime)->battleMode].backgroundMajorId;
            ((BtlState *)btlRuntime)->background.ids.minor = datBattleSceneRecords[((BtlState *)btlRuntime)->battleMode].backgroundMinorId;
            if (((BtlState *)btlRuntime)->battleMode == 0x3A0 && mdlFlagTest(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xDF;
                ((BtlState *)btlRuntime)->background.ids.minor = 5;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x3A1 && mdlFlagTest(0x80D) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE0;
                ((BtlState *)btlRuntime)->background.ids.minor = 6;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x33B && mdlFlagTest(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE2;
                ((BtlState *)btlRuntime)->background.ids.minor = 1;
            } else if (((BtlState *)btlRuntime)->battleMode == 0x340 && mdlFlagTest(0x818) != 0) {
                ((BtlState *)btlRuntime)->background.ids.major = 0xE4;
                ((BtlState *)btlRuntime)->background.ids.minor = 2;
            }
            if (((BtlState *)btlRuntime)->background.ids.major == 0 && ((BtlState *)btlRuntime)->background.ids.minor == 0) {
                if (btlResolveQueuedSceneRequestParameters(&req.code, &req.parameter) != 0) {
                    ((BtlState *)btlRuntime)->background.ids.major = req.code;
                    ((BtlState *)btlRuntime)->background.ids.minor = req.parameter;
                } else {
                    ((BtlState *)btlRuntime)->background.ids.major = 0xC9;
                    ((BtlState *)btlRuntime)->background.ids.minor = 1;
                }
            }
        }
        btlSetScene(2);
        break;
    case 1:
        btlSetScene(0xC);
        break;
    }
    if (((BtlState *)btlRuntime)->background.ids.major == 0 && ((BtlState *)btlRuntime)->background.ids.minor == 0) {
        ((BtlState *)btlRuntime)->background.ids.major = 0xC9;
        ((BtlState *)btlRuntime)->background.ids.minor = 1;
    }
    itfMesSetFlags(1);
    mdlFlagSet(0xC0F);
    ((BtlState *)btlRuntime)->battleFlags |= 0x701C1;
    ((BtlState *)btlRuntime)->commandRestrictFlags |= 2;
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
    return btlRuntime != 0;
}

s32 btlIsCurrentActorFullyMarked(void) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    return (((BattleController *)btlRuntime)->flags & 0x06000000) == 0x06000000;
}

s32 btlHasPendingRuntimeActivity(void) {
    s32 state;
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    state = btlRuntime;
    if (*(s32 *)(state + 0x248) != 0) {
        return 1;
    }
    if ((*(s32 *)(state + 0x1EC) & 2) != 0) {
        return 1;
    }
    return *(u32 *)(state + 0x718) != 0;
}

void btlResetActorEntryState(void) {
    s32 context = btlRuntime;
    DatPartyRecord *party = datGameState->party;
    u32 i;
    *(s32 *)(context + 0x2E8) = 0;
    *(s32 *)(context + 0x2EC) = 0;
    *(s32 *)(context + 0x2F0) = 0;
    *(s32 *)(context + 0x2F4) = 0;
    *(s32 *)(context + 0x2F8) = 0;
    *(u16 *)(context + 0x2FC) = 0;
    for (i = 0; i < 5; i++) {
        party[i].huntExp = 0;
    }
    memset((void *)(btlRuntime + 0x2DC), 0, 12);
}

s32 btlCaptureRewardSummary(BrsRewardSummary *rewards) {
    u32 i;

    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    rewards->macca = mdlFlagTest(0x290) ? ((BattleController *)btlRuntime)->rewardMacca : 0;
    rewards->mitama = ((BattleController *)btlRuntime)->specialEnemyDefeats;
    rewards->totalExp = ((BattleController *)btlRuntime)->experienceEarned;
    rewards->totalAp = ((BattleController *)btlRuntime)->totalAp;
    btlBossDebugPrintf("btl:----------------------\n");
    btlBossDebugPrintf("btl:money =%d\n", rewards->macca);
    btlBossDebugPrintf("btl:exp   =%d\n", rewards->totalExp);
    btlBossDebugPrintf("btl:ep    =%d\n", rewards->totalAp);
    btlBossDebugPrintf("btl:mitama=%d\n", rewards->mitama);
    for (i = 0; i < 5; i++) {
        rewards->unitApBonus[i] = datGameState->party[i].huntExp;
        btlBossDebugPrintf(D_004150B0, rewards->unitApBonus[i], datGameState->party[i].unitId);
    }
    btlBossDebugPrintf("btl:----------------------\n");
    for (i = 0; i < 3; i++) {
        rewards->icons[i].id = ((BattleController *)btlRuntime)->itemDrops[i].id;
        rewards->icons[i].param = ((BattleController *)btlRuntime)->itemDrops[i].count;
    }
    return 1;
}

s32 btlResolveQueuedSceneRequestParameters(s32 *outCode, s32 *outParameter) {
    s32 buffer[2];
    s16 i;

    if (fldConsumeNextSceneRequest(&buffer[0], &buffer[1]) == 0) {
        return 0;
    }
    for (i = 0; i < 0x10; i++) {
        if (D_00435E14[i].mapIdBase + 0xC8 != buffer[0]) {
            continue;
        }
        *outCode = D_00435E14[i].mapIdBase + 0xC8;
        if (D_00435E14[i].entries[buffer[1]].flagB != -1 &&
            mdlFlagTest(D_00435E14[i].entries[buffer[1]].flagB) != 0) {
            *outParameter = D_00435E14[i].entries[buffer[1]].parameterB;
            return 1;
        }
        if (D_00435E14[i].entries[buffer[1]].flagA != -1 &&
            mdlFlagTest(D_00435E14[i].entries[buffer[1]].flagA) != 0) {
            *outParameter = D_00435E14[i].entries[buffer[1]].parameterA;
            return 1;
        }
        *outParameter = D_00435E14[i].entries[buffer[1]].fallbackParameter;
        return 1;
    }
    return 0;
}

void *btlGetRuntime(void) {
    return (void *)(u32)btlRuntime;
}

/* Read current HP from a unit-entry address. */
s32 btlReadCurrentUnitHp(DatPartyRecord *entry) {
    return entry->hp;
}

/* Word-return getter; native callers choose whether to truncate MP to 16 bits. */
s32 btlReadCurrentUnitMp(DatPartyRecord *entry) {
    return entry->mp;
}

extern s32 ptyComputeMaxHp(DatPartyRecord *);

extern s32 ptyComputeMaxMp(DatPartyRecord *);

void btlComputeProfileMaxHp(DatPartyRecord *unit) {
    ptyComputeMaxHp(unit);
}

void btlComputeProfileMaxMp(DatPartyRecord *unit) {
    ptyComputeMaxMp(unit);
}

u32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *stats) {
    return datComputeSkillBoostedMaxHp(stats);
}

u32 btlComputeSkillAdjustedMaxMp(DatPartyRecord *stats) {
    return datComputeSkillBoostedMaxMp(stats);
}

extern s32 datAdjustCurrentHp(DatPartyRecord *, s32);

extern s32 datAdjustCurrentMp(DatPartyRecord *, s32);

s32 btlAdjustUnitHp(DatPartyRecord *object, s32 delta) {
    return datAdjustCurrentHp(object, delta);
}

s32 btlAdjustUnitMp(DatPartyRecord *object, s32 delta) {
    return datAdjustCurrentMp(object, delta);
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
    u32 currentMp = btlReadCurrentUnitMp(entryAddress);
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

extern void sdfRaisePackedChannelValue(DatPartyRecord *item, u32 minimumValue);

/* Forward the record and value to the packed low-15-bit status update. */
void btlRaisePartyRecordStatusValue(DatPartyRecord *item, u32 minimumValue) {
    sdfRaisePackedChannelValue(item, minimumValue);
}

extern void datClearUnitStatusBits(DatPartyRecord *unit, s32 mask);

/* Forward the legacy record/mask arguments to the party-record status clear. */
void btlClearPartyRecordStatusBits(DatPartyRecord *record, s32 mask) {
    datClearUnitStatusBits(record, mask);
}

/* Set the actor's selected entry index. */
void btlSetActorSelectedEntryIndex(BtlUnit *actor, u32 index) {
    actor->selectedEntryIndex = index;
}

/* No selected entry is represented by -1. */
void btlClearActorSelectedEntryIndex(BtlUnit *actor) {
    actor->selectedEntryIndex = -1;
}

/* Initialize enemy vitals/stats and pack its nonzero skill IDs into the party record. */
void btlInitializePartyRecordFromEnemy(DatPartyRecord *entry, s32 index) {
    u32 count;
    u32 i;

    memset(entry, 0, sizeof(*entry));
    entry->flags = 1;
    entry->affinityTableIndex = 0;
    entry->unitId = index;
    entry->level = datEnemyRecords[index].level;
    entry->totalExp = 0;
    for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
        entry->baseStats[i] = datEnemyRecords[index].baseStats[i];
    }
    count = 0;
    for (i = 0; i < 8; i++) {
        if (datEnemyRecords[index].skills[i] != 0) {
            entry->effectData[count++] = datEnemyRecords[index].skills[i];
        }
    }
    for (i = count; i < 24; i++) {
        entry->effectData[i] = 0;
    }
    entry->effectDataCount = count;
    entry->maxHp = datEnemyRecords[index].maxHp;
    entry->hp = datEnemyRecords[index].hp;
    entry->maxMp = datEnemyRecords[index].maxMp;
    entry->mp = datEnemyRecords[index].mp;
}

DatPartyRecord *btlGetActorEntryData(BtlUnit *actor) {
    DatPartyRecord *entry;

    if ((actor->status.flags & 0x400) == 0) {
        entry = btlGetIndexedPartyEntryRecord(actor->partyIndex);
        return entry;
    }
    return &actor->partyRecord;
}

s32 btlGetCurrentPartyEntryRecord(void) {
    s32 temp_v0;

    temp_v0 = dds3FindEntryIndex();
    return (s32)&datGameState->party[temp_v0];
}

DatPartyRecord *btlGetIndexedPartyEntryRecord(s32 index) {
    return &datGameState->party[index];
}

INCLUDE_RODATA(const s32, "game/code_001A8748", D_004150B0);

void btlSyncPlayerWork(BtlUnit *actor) {
    DatPartyRecord *src = &actor->partyRecord;
    DatPartyRecord *dst = btlGetIndexedPartyEntryRecord(actor->partyIndex);
    s32 maxHp;
    s32 maxMp;
    if (src->flags & 0x1000) {
        dst->flags |= 0x1000;
    } else {
        dst->flags &= ~0x1000;
    }
    if (src->flags & 0x4000) {
        dst->flags |= 0x4000;
    } else {
        dst->flags &= ~0x4000;
    }
    dst->level = src->level;
    maxHp = datComputeSkillBoostedMaxHp(dst);
    maxMp = datComputeSkillBoostedMaxMp(dst);
    dst->hp = src->hp < maxHp ? src->hp : maxHp;
    dst->mp = src->mp < maxMp ? src->mp : maxMp;
    memcpy(dst->baseStats, src->baseStats, sizeof(dst->baseStats));
    dst->status = src->status & 0x7FFF;
    dst->savedCommandPhase = src->savedCommandPhase;
    dst->unk1AE = src->unk1AE;
    dst->actionSlot = (u16)src->actionSlot;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

s32 btlFindPartyEntryIndexForActor(BtlUnit *object) {
    return dds3FindEntryIndex(object->partyRecord.unitId);
}

/* Retail no-op after party actor insertion, removal or roster swap. */
void btlOnPartyRosterChange(void) {
}

BtlUnit *btlFindActiveActorByKind(s32 kind) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    u32 flags;
    for (unit = battle->units; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (kind == unit->partyIndex) {
                    return unit;
                }
            }
        }
    }
    return 0;
}

extern void btlCopyUnitStats(BtlUnit *, DatPartyRecord *);

/* Insert a complete party record before actors with a larger priority. */
void btlInsertPartyActorByPriority(BtlUnit *unit, u8 sourceIndex, u8 priority) {
    DatPartyRecord saved;
    s32 insertion = 0;
    s32 i;

    if (datGameState->party[0].flags & 2) {
        do {
            if ((u16)(datGameState->party[insertion].flags & 1) == 0) {
                break;
            }
            if (priority < btlFindActiveActorByKind(insertion)->lookupId) {
                break;
            }
            insertion++;
            if (insertion >= 5) {
                break;
            }
        } while (datGameState->party[insertion].flags & 2);
    }

    memcpy(&saved, &datGameState->party[sourceIndex], sizeof(saved));
    for (i = sourceIndex; i < 4; i++) {
        memcpy(&datGameState->party[i], &datGameState->party[i + 1], sizeof(saved));
        if (datGameState->party[i + 1].flags & 2) {
            btlFindActiveActorByKind(i + 1)->partyIndex = i;
        }
    }

    for (i = 4; i > insertion; i--) {
        memcpy(&datGameState->party[i], &datGameState->party[i - 1], sizeof(saved));
        if (datGameState->party[i - 1].flags & 2) {
            btlFindActiveActorByKind(i - 1)->partyIndex = i;
        }
    }

    memcpy(&datGameState->party[i], &saved, sizeof(saved));
    btlCopyUnitStats(unit, &saved);
    unit->partyRecord.flags |= 2;
    datGameState->party[i].flags |= 2;
    unit->partyIndex = i;
    btlOnPartyRosterChange();
    btlBossDebugPrintf("btl:party in %d->%d[%d]\n", sourceIndex, i, saved.unitId);
}

void btlMoveActorOutOfActiveParty(BtlUnit *unit) {
    DatPartyRecord saved;
    DatGameState *scanState = datGameState;
    s32 originalIndex;
    s32 index;

    originalIndex = unit->partyIndex;
    memcpy(&saved, &datGameState->party[originalIndex], sizeof(saved));
    index = originalIndex;
    if (index < 4 && (u16)(scanState->party[index + 1].flags & 1)) {
        do {
            memcpy(&datGameState->party[index], &datGameState->party[index + 1], sizeof(saved));
            if (datGameState->party[index + 1].flags & 2) {
                btlFindActiveActorByKind(index + 1)->partyIndex = index;
            }
            index++;
            if (index >= 4) {
                break;
            }
            scanState = datGameState;
        } while ((u16)(scanState->party[index + 1].flags & 1));
    }

    memcpy(&datGameState->party[index], &saved, sizeof(saved));
    unit->partyRecord.flags &= ~2;
    datGameState->party[index].flags &= ~2;
    unit->partyIndex = 6;
    btlOnPartyRosterChange();
    btlBossDebugPrintf("btl:party out %d->%d[%d]\n", originalIndex, index, saved.unitId);
}

extern const char D_00415130[];

/* Swap the actor's roster entry, refresh its stats, and mark the active entry. */
void btlSynchronizePartyActorRecords(BtlUnit *actor, u8 targetIndex) {
    DatPartyRecord previous;

    if (actor->partyIndex != targetIndex) {
        memcpy(&previous, &datGameState->party[actor->partyIndex], sizeof(previous));
        memcpy(&datGameState->party[actor->partyIndex], &datGameState->party[targetIndex], sizeof(previous));
        memcpy(&datGameState->party[targetIndex], &previous, sizeof(previous));

        btlCopyUnitStats(actor, &datGameState->party[actor->partyIndex]);
        actor->partyRecord.flags |= 2;
        datGameState->party[actor->partyIndex].flags |= 2;
        datGameState->party[targetIndex].flags &= ~2;
        btlOnPartyRosterChange();
        btlBossDebugPrintf(D_00415130,
                           actor->partyIndex, targetIndex, previous.unitId,
                           actor->partyRecord.unitId);
    }
}

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record) {
    if ((record->flags & 4) != 0) {
        return 0;
    }
    return datEnemyRecords[record->unitId].flags;
}

extern s32 datGetClampedProfileAdjustedStat(DatPartyRecord *, s32);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

/* Native tail jump forwards the clamped profile-adjusted stat in v0;
 * retain the existing void C surface for matching. */
void btlQueryClampedProfileAdjustedStat(DatPartyRecord *unit, s32 statIndex) {
    datGetClampedProfileAdjustedStat(unit, statIndex);
}

/* Return the requested party-record stat with its native status override. */
s32 btlGetStatWithStatusOverride(DatPartyRecord *unit, s32 statIndex) {
    return datGetStatWithStatusOverride(unit, statIndex);
}

s32 btlApplyCommandAbilityMultiplier(DatPartyRecord *battler, s32 command) {
    u32 value = datCalculateCommandBaseValue(battler, command);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (datCommandRecords[command].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if (btlCheckSpecialAbility(battler, 0x254)) {
            scale = datAbilityParameters[0x254 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if (btlCheckSpecialAbility(battler, 0x255)) {
            scale = datAbilityParameters[0x255 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    value = (s32)((f32)value * scale);
    return value == 0 ? 1 : value;
}

s32 btlGetSlotValueAdjustedForSpecialAbility(BtlUnit *battler, s32 slot) {
    s32 value = datAffinityRecords[slot - DAT_AFFINITY_FIRST_COMMAND].slotCost;
    if (btlDoesEnabledStatusMatchCurrentId(&battler->partyRecord, 0xe4) && (u32)value >= 2) {
        value--;
    }
    return value;
}

s32 btlCheckCommandVitalCost(BtlUnit *unit, s32 command) {
    s32 result = 0;
    u32 cost = btlApplyCommandAbilityMultiplier(&unit->partyRecord, command);

    switch (datCommandRecords[command].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if ((datCommandRecords[command].flags & 8) == 0) {
            if (unit->partyRecord.hp <= cost) {
                result = 1;
            }
        } else if (unit->partyRecord.hp < cost) {
            result = 1;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if (unit->partyRecord.mp < cost) {
            result = 2;
        }
        break;
    }
    return result;
}

extern s32 fldCountSceneSlots(void);

s32 btlGetCommandUseError(BtlUnit *unit, s32 command) {
    u32 availableSlots;

    if (command == 0) {
        return 0;
    }
    if ((unit->partyRecord.flags & 0x10) && (unit->status.flags & 0x200)) {
        if ((datCommandRecords[command].effectType != 0 &&
             datCommandRecords[command].effectType != 2) ||
            datCommandRecords[command].costMode >= 2) {
            return 7;
        }
    }
    if ((datCommandRecords[command].effectType == 1 ||
         datCommandRecords[command].costMode == DAT_COMMAND_COST_MODE_MP) &&
        (unit->partyRecord.status & 0x7FFF) == 0x10) {
        return 3;
    }
    if (datCommandSelectors[command].kind == 2 &&
        (unit->partyRecord.status & 0x7FFF) == 0x40) {
        return 5;
    }
    if (datCommandSelectors[command].kind == 1) {
        if ((unit->partyRecord.status & 0x7FFF) == 0x1000) {
            return 4;
        }
        availableSlots = fldCountSceneSlots();
        if (availableSlots < btlGetSlotValueAdjustedForSpecialAbility(unit, command)) {
            return 6;
        }
    }
    if (datCommandRecords[command].flags & 4) {
        return 0;
    }
    return btlCheckCommandVitalCost(unit, command);
}

s32 btlGetCombinedPartyCommandPower(DatPartyRecord *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    DatPartyRecord snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;
    s32 average;

    if (first != NULL) {
        totalMaxHp = first->partyRecord.maxHp;
        count = 1;
    }
    if (second != NULL) {
        totalMaxHp += second->partyRecord.maxHp;
        count++;
    }
    if (third != NULL) {
        totalMaxHp += third->partyRecord.maxHp;
        count++;
    }
    average = totalMaxHp / count;
    snapshot.maxHp = average;
    snapshot.hp = average;
    return btlApplyCommandAbilityMultiplier(&snapshot, command);
}

s32 btlGetPartnerCommandUseError(BtlUnit *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    BtlUnit snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;

    if (first != NULL) {
        if ((first->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((first->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        totalMaxHp = first->partyRecord.maxHp;
        count = 1;
    }
    if (second != NULL) {
        if ((second->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((second->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += second->partyRecord.maxHp;
    }
    if (third != NULL) {
        if ((third->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((third->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += third->partyRecord.maxHp;
    }
    snapshot.partyRecord.maxHp = totalMaxHp / count;
    return btlGetCommandUseError(&snapshot, command);
}

s8 btlGetActorIndexedSignedValue(BtlUnit *object, s32 index) {
    if (index == 0 && (object->status.flags & 0x400) != 0) {
        return datEnemyRecords[object->partyRecord.unitId].indexedValueOverride;
    }
    return datCommandSelectors[index].stat;
}

s32 btlGetUnitElementAffinity(BtlUnit *battler, s32 element) {
    return btlResolveUnitValueWithOverride(&battler->partyRecord, element);
}

extern s32 datGetEffectiveAffinity(DatPartyRecord *, s32);

s32 btlResolveUnitValueWithOverride(DatPartyRecord *record, s32 element) {
    s32 (*hook)(s32, s32) = *(s32 (**)(s32, s32))((u8 *)btlGetRuntime() + 0x6B8);
    if (hook != 0) {
        s32 value = hook((s32)record, element);
        if (value != -1) {
            return value;
        }
    }
    return datGetEffectiveAffinity(record, element);
}

void *btlGetSideIndexedActorStatusTable(s32 side, s32 index) {
    if (side == 0) {
        return (void *)(u32)(D_00435DE0 + index * 0x270);
    }
    return (void *)(u32)(D_00435DF0 + index * 0x270);
}

BtlActorTransformParameters *btlSelectSharedOrIndexedTransformParameters(s32 side, s32 index) {
    if (side == 0) {
        return (BtlActorTransformParameters *)btlSharedActorTransformParameters;
    }
    return (BtlActorTransformParameters *)(D_00435E00 + index * 24);
}

s32 btlSelectSideIndexedActorParameterTable(s32 category, s32 actorId) {
    if (category == 0) {
        return D_00435DE4 + actorId * 0x74;
    }
    return D_00435DF8 + actorId * 0x74;
}

s32 btlGetLoggedIndexedCommandItem(s32 index) {
    u16 item = datItemSkillRecords[index].commandIndex;
    btlBossDebugPrintf(D_00415158, index, item);
    return item;
}

/* Record IDs are halfwords; the battle operand API returns a word. */
s32 btlGetActorBedAssetIdFromIndex(s32 itemIndex) {
    return datItemSkillRecords[itemIndex].commandIndex;
}

extern const char D_00415178[];

s32 btlBuildUnitIndexList(void *actionArg, BtlIndexList *targets,
                  s32 area, s32 rule, s32 excludedStatus) {
    ActionStateLink *action = (ActionStateLink *)actionArg;
    BtlUnit *collected[16];
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 alliedArea;
    s32 opposingArea;
    s32 areaMask;
    BtlUnit *unit;
    s32 count;
    s32 selected;
    s32 i;

    if ((action->unit->status.flags & 0x200) != 0) {
        alliedArea = 0x200;
        opposingArea = 0x400;
    } else {
        alliedArea = 0x400;
        opposingArea = 0x200;
    }
    areaMask = 0;
    if ((area & 1) != 0) {
        areaMask = alliedArea;
    }
    if ((area & 2) != 0) {
        areaMask |= opposingArea;
    }
    btlClearIndexList(targets);
    if ((rule & 1) != 0) {
        btlAppendIndexListEntry(targets, action->unit);
        return 1;
    }
    unit = battle->units;
    count = 0;
    selected = -1;
    for (; unit != NULL; unit = unit->nextActor) {
        s32 flags = unit->status.flags;

        if ((flags & 0x100) == 0) {
            continue;
        }
        if ((flags & 0xE0) != 0) {
            if (excludedStatus == 0) {
                continue;
            }
            if ((excludedStatus & 0x4000) != 0 && excludedStatus != -1) {
                continue;
            }
        }
        if ((flags & areaMask) == 0) {
            continue;
        }
        if ((rule & 2) != 0 && (unit->status.flags & 1) != 0) {
            if (action->unit == unit) {
                continue;
            }
            collected[count++] = unit;
        } else if ((rule & 8) != 0) {
            if ((unit->status.flags & 1) != 0) {
                continue;
            }
            collected[count] = unit;
            selected = count++;
        } else {
            if ((rule & 4) == 0) {
                if ((unit->status.flags & 1) == 0) {
                    continue;
                }
            } else {
                if ((unit->status.flags & 1) == 0) {
                    selected = count;
                }
            }
            if ((unit->status.flags & 1) != 0) {
                if (excludedStatus != 0) {
                    if (excludedStatus != -1) {
                        if (unit->partyRecord.status == 0) {
                            continue;
                        }
                        if ((excludedStatus & unit->partyRecord.status) != 0) {
                            continue;
                        }
                    }
                }
            }
            collected[count++] = unit;
        }
    }
    if (count > 0) {
        if (selected < 0) {
            selected = count - 1;
        }
        for (i = 0; i < count; i++) {
            s32 index = selected - i;

            if (index < 0) {
                index += count;
            } else {
                index %= count;
            }
            btlAppendIndexListEntry(targets, collected[index]);
        }
    }
    btlBossDebugPrintf(D_00415178, count, area, rule, excludedStatus);
    return count != 0;
}


extern s32 btlBuildUnitIndexList(void *, BtlIndexList *, s32, s32, s32);

extern u8 *datRosterDetails;
INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415130);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415158);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415178);

u32 btlSelectCommandTargetPolicy(ActionStateLink *task, BtlIndexList *destination, s32 unused) {
    s32 category = 3;
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 mode = 0;
    s32 flags = 0;
    s32 mask = 0;
    s32 value = task->indexWork.phase;

    switch (value) {
    case 1: {
        BtlUnit *actor = task->unit;
        if ((btlUnitStatusPair(actor) & 0x1400) || (actor->partyRecord.flags & 0x10)) {
            if (btlCheckSpecialAbility(&actor->partyRecord, 0x23E) != 0) {
                category = 1;
                task->unit->pendingAbilityId = 0x23E;
            } else {
                category = 0;
            }
            mode = 2;
            flags = 0;
            mask = 0;
        } else {
            value = actor->partyRecord.unitId;
            category = ((EventRosterStat *)datRosterDetails)[value].targetType;
            mode = ((EventRosterStat *)datRosterDetails)[value].options;
            flags = ((EventRosterStat *)datRosterDetails)[value].targetFlags;
            mask = ((EventRosterStat *)datRosterDetails)[value].restriction;
        }
        break;
    }
    case 2:
    case 3:
    case 4:
    case 7:
    case 8: {
        if (value == 4) {
            value = btlGetLoggedIndexedCommandItem(task->indexWork.reference);
        } else {
            value = task->indexWork.skillId;
        }
        category = datCommandRecords[value].targetType;
        mode = datCommandRecords[value].options;
        flags = datCommandRecords[value].targetFlags;
        mask = datCommandRecords[value].restriction;
        if ((datCommandRecords[value].attribute.bits & 0x400000FF) == 0x40000002 && mask == 0) {
            mask = -1;
        }
        break;
    }
    case 9:
        category = 0;
        mode = 1;
        flags = 4;
        mask = -1;
        break;
    }
    if (battle->unk220 & BTL_STATE_FLAG_BYPASS_COMMAND_BLOCKS) {
        mode = 3;
        flags &= ~0xB;
        mask = 0;
    }
    if (destination != NULL) {
        btlBuildUnitIndexList(task, destination, mode, flags, mask);
    }
    return category;
}

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 actionTask, BtlIndexList *targets);

s32 btlFindEligibleTargetForMultiActorCommand(s32 actionTask, BtlIndexList *targets) {
    ActionStateLink *action = (ActionStateLink *)actionTask;
    u32 count;
    u32 i;
    s32 command;
    s32 index;

    if (action == NULL || targets == NULL) {
        return 0;
    }
    count = btlGetIndexListCount(targets);
    if (count < 2) {
        return 0;
    }
    command = action->indexWork.phase;
    switch (command) {
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
        if (command == 4) {
            index = btlGetLoggedIndexedCommandItem(action->indexWork.reference);
        } else {
            index = action->indexWork.skillId;
        }
        if (datCommandRecords[index].targetType == 0 &&
            datCommandRecords[index].attribute.parts.kind == DAT_COMMAND_ATTRIBUTE_KIND_FLAG_MASK &&
            datCommandRecords[index].attribute.parts.valueMask != 0) {
            for (i = 0; i < count; i++) {
                if ((datCommandRecords[index].attribute.parts.valueMask &
                     ((BtlUnit *)btlGetIndexListEntry(targets, i))->partyRecord.status) != 0) {
                    return i;
                }
            }
        }
        break;
    }
    return 0;
}

s32 btlMarkInactiveActorCandidates(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *eligible[16];
    BtlUnit *unit;
    BtlUnit **read;
    s32 count;
    s32 remaining;
    s32 lowestId;

    unit = battle->units;
    count = 0;
    if (unit != NULL) {
        do {
            s32 flags = unit->status.flags;
            if ((flags & 0x200) != 0) {
                if ((flags & 1) == 0) {
                    eligible[count] = unit;
                    unit->status.flags = flags & ~0x100;
                    count++;
                }
            }
            unit = unit->nextActor;
        } while (unit != NULL);
    }
    if (count == 0) {
        return 0;
    }
    if (battle->unk268 == 3 && count == 2) {
        eligible[0]->status.flags |= 0x100;
        eligible[1]->status.flags |= 0x100;
    } else {
        lowestId = 4;
        unit = NULL;
        if (count > 0) {
            remaining = count;
            read = eligible;
            do {
                BtlUnit *candidate = *read++;
                s32 lookupId = candidate->lookupId;
                if (lookupId < lowestId) {
                    unit = candidate;
                    lowestId = lookupId;
                }
            } while (--remaining != 0);
        }
        unit->status.flags |= 0x100;
    }
    return 1;
}

#include "btl_roster_pair.h"

extern u8 btlLinkageParticipantOrders[];
extern DatSkillList *D_00435E28;

/* Build distinct LINKAGE skill/partner rows for actor; return the roster count. */
s32 btlBuildActorLinkageSkillList(BtlUnit *actor, BattleRosterTable *table) {
    BtlUnit *actors[3];
    BtlState *battle;
    BtlUnit *unit;
    DatAffinityRecord *record;
    s32 actorCount;
    u32 count;
    u32 affinity;
    u32 order;
    s32 command;
    u32 participant;
    u32 skillIndex;
    u32 previous;
    s32 allRequired;
    s32 requirement;
    s32 value;
    u32 matched;
    s32 duplicate;

    battle = (BtlState *)btlGetRuntime();
    table->count = 0;
    if (actor->partyRecord.unitId == 9) {
        return 0;
    }
    actors[0] = actor;
    actors[1] = NULL;
    actors[2] = NULL;
    actorCount = 1;
    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
        if ((unit->status.flags & 0x200) == 0) {
            continue;
        }
        if (unit == actor) {
            continue;
        }
        if ((unit->status.flags & 1) == 0) {
            continue;
        }
        if ((unit->status.flags & 0xC0) != 0) {
            continue;
        }
        if ((unit->status.flags & 0x20) != 0) {
            continue;
        }
        if (unit->partyRecord.unitId == 9) {
            continue;
        }
        actors[actorCount++] = unit;
    }
    if (actorCount == 1) {
        return 0;
    }

    count = 0;
    record = datAffinityRecords;
    for (command = DAT_AFFINITY_FIRST_COMMAND, affinity = 0;
         affinity < 117; affinity++, command++, record++) {
        allRequired = 1;
        for (order = 0; order < 6; order++) {
            matched = 0;
            for (participant = 0; participant < 3; participant++) {
                requirement = record->requirements[btlLinkageParticipantOrders[participant + order * 3]];
                if ((battle->battleFlags & 0x1000000) == 0) {
                    if (requirement == -1) {
                        allRequired = 0;
                        continue;
                    }
                } else {
                    if (requirement == -1) {
                        allRequired = 0;
                    } else {
                        matched |= 1 << participant;
                    }
                    continue;
                }
                if (actors[participant] == NULL) {
                    continue;
                }
                if ((record->flags & 1) != 0 &&
                    (actors[participant]->status.flags & 0x200) == 0) {
                    continue;
                }
                if ((record->flags & 2) != 0 &&
                    (actors[participant]->status.flags & 0x400) == 0 &&
                    actors[participant]->partyRecord.unitId < 0x100) {
                    continue;
                }
                switch (requirement & 0xF0000000) {
                case 0:
                    if ((actors[participant]->status.flags & 0x1000) == 0) {
                        continue;
                    }
                    if (requirement == 0 ||
                        btlCheckSpecialAbility(&actors[participant]->partyRecord, requirement) != 0) {
                        matched |= 1 << participant;
                    }
                    break;
                case 0x10000000:
                    if ((actors[participant]->status.flags & 0x1000) == 0) {
                        continue;
                    }
                    value = requirement & 0x0FFFFFFF;
                    for (skillIndex = 0; skillIndex < 24; skillIndex++) {
                        if (actors[participant]->partyRecord.effectData[skillIndex] != 0 &&
                            (1 << datCommandSelectors[actors[participant]->partyRecord.effectData[skillIndex]].stat) == value) {
                            matched |= 1 << participant;
                            break;
                        }
                    }
                    continue;
                case 0x20000000:
                    if ((actors[participant]->status.flags & 0x1000) != 0) {
                        continue;
                    }
                    value = requirement & 0x0FFFFFFF;
                    if (value != 0 && value != (1 << actors[participant]->partyRecord.unitId)) {
                        continue;
                    }
                    matched |= 1 << participant;
                    break;
                case 0x40000000:
                    if ((actors[participant]->status.flags & 0x1000) == 0) {
                        continue;
                    }
                    value = requirement & 0x0FFFFFFF;
                    for (skillIndex = 0; skillIndex < 24; skillIndex++) {
                        s16 skill = D_00435E28[value].skills[skillIndex];
                        if (skill >= 0 && btlCheckSpecialAbility(&actors[participant]->partyRecord, skill)) {
                            matched |= 1 << participant;
                            break;
                        }
                    }
                    continue;
                default:
                    continue;
                }
            }
            if ((matched & 1) != 0 && (matched & 6) != 0) {
                duplicate = 0;
                if (matched == 7) {
                    table->entries[count].skill = command;
                    table->entries[count].first = actors[1];
                    table->entries[count].second = actors[2];
                } else if (!allRequired) {
                    if (matched == 3) {
                        table->entries[count].skill = command;
                        table->entries[count].first = actors[1];
                        table->entries[count].second = NULL;
                    } else if (matched == 5) {
                        table->entries[count].skill = command;
                        table->entries[count].first = actors[2];
                        table->entries[count].second = NULL;
                    }
                } else {
                    duplicate = 1;
                }
                if (!duplicate) {
                    for (previous = 0; previous < count; previous++) {
                        if (table->entries[count].skill == table->entries[previous].skill &&
                            ((table->entries[count].first == table->entries[previous].first &&
                              table->entries[count].second == table->entries[previous].second) ||
                             (table->entries[count].first == table->entries[previous].second &&
                              table->entries[count].second == table->entries[previous].first))) {
                            duplicate = 1;
                            break;
                        }
                    }
                }
                if (!duplicate) {
                    count++;
                }
            }
        }
    }
    table->count = count;
    return count;
}


extern ActionStateLink *fldGetSceneGroupEntry(s32 entryIndex);
extern s32 btlFindCommandPartnersByAffinity(BtlUnit *unit, s32 command, BtlUnit **first, BtlUnit **second);

s32 btlResolveCommandAffinityPartners(BtlUnit *unit, s32 command, BtlCommandPartnerOutput *output) {
    BtlState *controller = (BtlState *)btlGetRuntime();
    s32 *requirementCursor;
    u32 requiredCount;
    u32 i;
    s32 result;

    if (datCommandSelectors[command].kind != 1) {
        return 0;
    }
    if ((u32)(command - DAT_AFFINITY_FIRST_COMMAND) >= 0x75) {
        return 0;
    }
    requiredCount = 0;
    requirementCursor = datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].requirements;
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount == 0) {
        return 0;
    }
    if ((datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].flags & 2) == 0 &&
        (datBattleSceneRecords[controller->battleMode].flags & 1) == 0) {
        output->commandId = command;
        result = btlFindCommandPartnersByAffinity(unit, command, &output->firstPartner, &output->secondPartner);
    } else {
        output->commandId = command;
        output->firstPartner = NULL;
        output->secondPartner = NULL;
        if (requiredCount == 3) {
            for (i = 1; i < requiredCount; i++) {
                ActionStateLink *entry = fldGetSceneGroupEntry(i);
                BtlUnit *partner;

                if (entry == NULL) {
                    return 0;
                }
                partner = entry->unit;
                if ((partner->status.flags & 0x400) == 0) {
                    return 0;
                }
                if ((partner->status.flags & 0xE0) != 0) {
                    return 0;
                }
                if ((partner->partyRecord.status & 0x2A0E) != 0) {
                    return 0;
                }
                switch (i) {
                case 1:
                    output->firstPartner = partner;
                    break;
                case 2:
                    output->secondPartner = partner;
                    break;
                }
            }
            return 1;
        }
        if (requiredCount == 2) {
            ActionStateLink *entry = fldGetSceneGroupEntry(2);

            if (entry != NULL && (entry->unit->partyRecord.status & 0x2A0E) == 0 &&
                (btlUnitStatusPair(entry->unit) & 0x4E0) == 0x400) {
                output->firstPartner = entry->unit;
            } else {
                BtlUnit *partner;

                entry = fldGetSceneGroupEntry(1);
                if (entry == NULL) {
                    return 0;
                }
                partner = entry->unit;
                if ((partner->status.flags & 0x400) == 0) {
                    return 0;
                }
                if ((partner->status.flags & 0xE0) != 0) {
                    return 0;
                }
                if ((partner->partyRecord.status & 0x2A0E) == 0) {
                    output->firstPartner = partner;
                } else {
                    return 0;
                }
            }
        } else {
            return 0;
        }
        result = 1;
    }
    return result;
}


u32 btlEncodeActorIndexAsSelectionMask(u32 id) {
    u32 mask;
    switch (id) {
    case 0xFFFFFFFF: mask = 0x1; break;
    case 0: mask = 0x2; break;
    case 1: mask = 0x4; break;
    case 2: mask = 0x8; break;
    case 3: mask = 0x10; break;
    case 4: mask = 0x20; break;
    case 5: mask = 0x40; break;
    case 6: mask = 0x80; break;
    case 7: mask = 0x100; break;
    case 8: mask = 0x200; break;
    case 9: mask = 0x400; break;
    case 10: mask = 0x800; break;
    case 11: mask = 0x1000; break;
    case 12: mask = 0x2000; break;
    case 13: mask = 0x4000; break;
    case 14: mask = 0x8000; break;
    case 15: mask = 0x10000; break;
    case 16: mask = 0x20000; break;
    case 17: mask = 0x40000; break;
    case 18: mask = 0x80000; break;
    default: mask = 0; break;
    }
    return mask;
}

/* Decode an element flag through the shared native flag-to-index table. */
s32 btlFlagToElementIndex(s32 flag) {
    return datFlagToElementIndex(flag);
}

s32 btlCheckSpecialAbility(DatPartyRecord *record, s32 ability) {
    if (datUnitHasSkill(record, ability) == 0) {
        return 0;
    }
    switch (ability) {
    case 0x227:
        return evtGetMirroredSolarPhase() == 8;
    case 0x228:
        return evtGetMirroredSolarPhase() == 0;
    default:
        return 1;
    }
}

typedef struct DatSkillOwner {
    u16 flags;
    u16 unk2;
    u16 enemyRecordIndex;
    u8 unk6[0x1C];
    u16 skills[0x18];
} DatSkillOwner;

/* Search the first eight owner/enemy skill slots; return whether the ID occurs. */
s32 btlHasSkillInFirstEightSlots(DatSkillOwner *unit, s32 skill) {
    s32 i;

    if (!(unit->flags & 0x20)) {
        for (i = 0; i < 8; i++) {
            if (unit->skills[i] == skill) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (datEnemyRecords[unit->enemyRecordIndex].skills[i] == skill) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *partyRecord, u32 entryId) {
    if (partyRecord->flags & 0x20) {
        return 0;
    }
    return mnuGetPartyEntryCurrentId(partyRecord) == entryId;
}

u32 btlSelectEnemyDropItem(BtlUnit *enemyUnit, s32 mode);

s32 btlSelectActorAction(s32 state) {
    s32 selection = btlSelectEnemyDropItem(state, 1);
    if (selection == 0) {
        selection = (sdfGetRand((EffRandState *)effSharedRandomState) & 1) ? 2 : 9;
    }
    return selection;
}

s32 btlIsEventThresholdSatisfiedForEntry(s32 entryValue) {
    s32 eventId = entryValue & 0xFFFF;
    switch (eventId) {
    case 0x8A:
        if (evtCheckValueThreshold(0x8A, 1) != 0 || evtCheckValueThreshold(0x93, 1) != 0) {
            return 1;
        }
        break;
    case 0x8B:
        if (evtCheckValueThreshold(0x8B, 1) != 0 || evtCheckValueThreshold(0x94, 1) != 0) {
            return 1;
        }
        break;
    }
    if ((u32)((eventId + 0xFF80) & 0xFFFF) < 0x20 && evtCheckValueThreshold(eventId, 1) != 0) {
        return 1;
    }
    return 0;
}

extern s32 btlRollAiBucket(void);


/* Select an enemy item drop; keep DDS2's party ability bonus and event exclusions. */
u32 btlSelectEnemyDropItem(BtlUnit *enemyUnit, s32 mode) {
    u16 count = 0;
    u16 bonus = 0;
    u32 selected = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    DatEnemyRecord *enemy = &datEnemyRecords[enemyUnit->partyRecord.unitId];
    BtlUnit *unit;
    u16 total;
    u16 index;
    u16 roll;

    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    if (btlCheckSpecialAbility(&unit->partyRecord, 0x27B)) {
                        count++;
                    }
                }
            }
        }
    }
    if (count != 0) {
        switch (count) {
        case 1:
            bonus = datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterA;
            break;
        case 2:
            bonus = datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterA;
            bonus += (u16)datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterB;
            break;
        default:
            bonus = datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterA;
            bonus += datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterB * 2;
            break;
        }
    }
    if (mode != 1 && enemy->overrideAction != 0 && enemy->overrideFlag != 0 &&
        mdlFlagTest(enemy->overrideFlag)) {
        if ((u8)btlRollAiBucket() < enemy->overrideChance) {
            selected = enemy->overrideAction;
        }
    }
    if (selected == 0) {
        total = 0;
        for (index = 0; index < 2; index++) {
            if (enemy->additionalDropItemIds[index] != 0) {
                if (btlIsEventThresholdSatisfiedForEntry(enemy->additionalDropItemIds[index])) {
                    btlBossDebugPrintf("btl:event item check[%X]\n", enemy->additionalDropItemIds[index]);
                } else {
                    total += enemy->actionChances[index];
                }
            }
        }
        if (total != 0 && btlRollAiBucket() < total + bonus) {
            roll = sdfGetRandInt(0, total);
            total = 0;
            for (index = 0; index < 2; index++) {
                if (enemy->additionalDropItemIds[index] != 0) {
                    if (btlIsEventThresholdSatisfiedForEntry(enemy->additionalDropItemIds[index])) {
                        btlBossDebugPrintf("btl:event item check[%X]\n", enemy->additionalDropItemIds[index]);
                    } else {
                        total += enemy->actionChances[index];
                        if (roll < total) {
                            selected = enemy->additionalDropItemIds[index];
                            break;
                        }
                    }
                }
            }
        }
    }
    return selected;
}

/* Accumulate one item drop; new IDs are discarded if all three slots are occupied. */
void btlAddItemDrop(u16 item) {
    BattleController *controller = (BattleController *)btlGetRuntime();

    if (item != 0) {
        s32 found = 0;
        u16 i;

        for (i = 0; i < 3; i++) {
            if (controller->itemDrops[i].id == item) {
                found = 1;
                controller->itemDrops[i].count++;
                break;
            }
        }
        if (!found) {
            for (i = 0; i < 3; i++) {
                if (controller->itemDrops[i].id == 0) {
                    controller->itemDrops[i].id = item;
                    controller->itemDrops[i].count = 1;
                    break;
                }
            }
        }
    }
}

extern s32 btlAverageLevelsForMaskExcludingStatus20(u32);

extern s32 btlCalculateEnemyExperienceReward(BtlUnit *, BtlUnit *);

extern s32 btlGetEnemyMoney(BtlUnit *, BtlUnit *);

extern char D_004151E8[];

extern char D_00415208[];

extern char D_00415220[];

extern char D_00415238[];

extern char D_00415250[];

void btlAccumulateEnemyDefeatRewards(BtlUnit *enemy) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    DatEnemyRecord *record = &datEnemyRecords[enemy->partyRecord.unitId];
    s32 level = btlAverageLevelsForMaskExcludingStatus20(4);
    s32 enemyLevel = record->level;
    s32 allowance = datBattleParameters->rewardLevelAllowance;
    s32 reward;
    s32 amount;
    s32 item;
    s32 kind;

    if (level > 62) {
        level = 62;
    }
    if (level >= enemyLevel + allowance && datBattleParameters->rewardDivisor != 0.0f) {
        reward = (s32)(record->battleExperienceReward / datBattleParameters->rewardDivisor);
        btlBossDebugPrintf(D_004151E8, reward, level, enemyLevel, allowance,
                          datBattleParameters->rewardDivisor);
    } else {
        reward = record->battleExperienceReward;
    }
    if (record->flags & 0x2000) {
        reward *= 100;
    }
    if (controller->mode == 3) {
        reward = (s32)(reward * datBattleParameters->majinRewardScale);
        btlBossDebugPrintf(D_00415208, reward, datBattleParameters->majinRewardScale);
    }
    controller->experienceEarned += reward;
    if ((btlUnitStatusPair(enemy) & 0x200800000ULL) == 0) {
        amount = btlCalculateEnemyExperienceReward(NULL, enemy);
        controller->epEarned += amount;
        btlBossDebugPrintf(D_00415220, controller->epEarned, amount);
    }
    if ((enemy->status.stateFlags & 0x400) == 0) {
        amount = btlGetEnemyMoney(NULL, enemy);
        controller->moneyEarned += amount;
        btlBossDebugPrintf(D_00415238, controller->moneyEarned, amount);
    }
    item = btlSelectEnemyDropItem((s32)enemy, 0);
    if (item != 0) {
        btlAddItemDrop(item);
    }
    kind = enemy->partyRecord.unitId;
    if (kind < 104) {
        if (kind >= 100) {
            controller->specialEnemyDefeats++;
        }
    }
    enemy->status.stateFlags |= 1;
    btlBossDebugPrintf(D_00415250, enemy);
}

/* Readiness requires each active actor to have cleared transient action flags. */
s32 btlAllActiveUnitsReady(void) {
    BtlUnit *unit;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 0xC0) {
                    return 0;
                }
                if (flags & 0x20) {
                    if (!(unit->status.stateFlags & 1)) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

/* Return the battle-parameter scale for the selected encounter's clamped adjustment. */
f32 btlGetEncounterAdjustmentScale(void) {
    BattleController *runtime = (BattleController *)btlGetRuntime();
    s32 adjustment = D_00435E0C[runtime->adjustmentRecordIndex]
                         .groups[runtime->adjustmentGroupIndex]
                         .entries[runtime->adjustmentEntryIndex]
                         .value;

    if (adjustment < -3) {
        adjustment = -3;
    } else if (adjustment > 3) {
        adjustment = 3;
    }
    return datBattleParameters->adjustmentScale[adjustment + 3];
}

/* Read the counter incremented by the escape-attempt result callback. */
u32 btlGetEscapeAttemptCount(void) {
    BtlState *controller;

    controller = btlGetRuntime();
    return controller->escapeAttemptCount;
}


/* Return a random eligible actor task, or 0 if no candidate is available. */
s32 btlChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    ActionStateLink *node;
    BtlUnit *unit;
    u32 flags;
    for (node = ((BattleController *)btlGetRuntime())->taskHead; node != 0; node = node->next) {
        if (!(node->pendingFlags & 8)) {
            continue;
        }
        unit = node->unit;
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (flags & 2) {
                    if (!(flags & 0xE0)) {
                        candidates[count] = (s32)node;
                        count++;
                    }
                }
            }
        }
    }
    if (count == 0) {
        return 0;
    }
    return candidates[sdfGetRandInt(0, count)];
}

s32 btlCountAvailableUnits(void) {
    BtlUnit *unit;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    count++;
                }
            }
        }
    }
    return count;
}

/* Count ready scene actors plus eligible party entries stored in script state. */
s32 btlCountAvailableParticipants(void) {
    BtlUnit *unit;
    DatPartyRecord *entry;
    s32 count = 0;
    s32 i;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    count++;
                }
            }
        }
    }
    entry = datGameState->party;
    for (i = 4; i >= 0; i--) {
        if (entry->flags & 1) {
            if (!(entry->flags & 2)) {
                if (!(entry->status & 0x4000)) {
                    count++;
                }
            }
        }
        entry++;
    }
    return count;
}

f32 btlGetActorStateScale(ActionStateLink *task) {
    if (task == NULL) {
        return 1.0f;
    }
    if (((BtlState *)btlGetRuntime())->battleFlags & 0x8000) {
        if ((btlUnitStatusPair(task->unit) & 0x1200) != 0x200
            || (task->unit->partyRecord.flags & 0x10)) {
            return 3.0f;
        }
    }
    return 1.0f;
}

void btlClearActorEntrySlot(BtlUnit *unit, s32 index);

void btlClearAllActorEntrySlots(BtlUnit *unit) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        btlClearActorEntrySlot(unit, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

s32 btlActorEntryIsExpired(BtlUnit *unit, s32 index) {
    if (unit->entrySlots[index].code == 0) {
        return 0;
    }
    return unit->entrySlots[index].countdown < 1;
}

typedef struct EntryPair {
    s16 first;
    s16 second;
    s16 initialValue;
    s16 countdown;
} EntryPair;

extern EntryPair btlActorEntryCodeDefaults[];

s32 btlMatchActorEntryCode(BtlUnit *unit, s32 index) {
    s16 value = unit->entrySlots[index].code;
    if (btlActorEntryCodeDefaults[index].first != 0) {
        if (btlActorEntryCodeDefaults[index].first == value) {
            return 1;
        }
    }
    if (btlActorEntryCodeDefaults[index].second != 0) {
        if (btlActorEntryCodeDefaults[index].second == value) {
            return 2;
        }
    }
    return 0;
}

/* Clamp a signed entry-code delta and refresh timing only when the result is nonzero. */
void btlAdjustActorEntryCode(BtlUnit *unit, s32 index, s16 delta) {
    s16 code = unit->entrySlots[index].code;
    code += delta;

    if (code > btlActorEntryCodeDefaults[index].first) {
        code = btlActorEntryCodeDefaults[index].first;
    }
    if (code < btlActorEntryCodeDefaults[index].second) {
        code = btlActorEntryCodeDefaults[index].second;
    }
    if (code != 0) {
        unit->entrySlots[index].unk02 = btlActorEntryCodeDefaults[index].initialValue;
        unit->entrySlots[index].countdown = btlActorEntryCodeDefaults[index].countdown;
    }
    unit->entrySlots[index].code = code;
}

void btlSetActorEntryCode(BtlUnit *unit, s32 index, u16 code) {
    unit->entrySlots[index].code = code;
}

void btlClearActorEntrySlot(BtlUnit *unit, s32 index) {
    unit->entrySlots[index].code = 0;
    unit->entrySlots[index].unk02 = -1;
    unit->entrySlots[index].countdown = -1;
}

#include "btl_actor_entry_code_api.h"

s16 btlGetActorEntryCode(const BtlUnit *unit, s32 index) {
    return unit->entrySlots[index].code;
}

INCLUDE_RODATA(const s32, "game/code_001A8748", D_004151E8);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415208);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415220);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415238);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415250);

f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge) {
    f32 factor;
    s32 stage;

    stage = btlGetActorEntryCode(unit, index);
    factor = 1.0f;
    switch (index) {
    case 3:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleA + 3)[-stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleA + 3)[-stage];
        }
        break;
    case 2:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleB + 3)[-stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleB + 3)[-stage];
        }
        break;
    case 0:
    case 1:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleA + 3)[stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleA + 3)[stage];
        }
        break;
    case 4:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleB + 3)[stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleB + 3)[stage];
        }
        break;
    }
    if (index == 0 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 5) > 0) {
        if (btlActorEntryIsExpired(unit, 5) != 0) {
            factor *= 2.25f;
            btlBossDebugPrintf("btl:BUTURIx2\n");
        }
    }
    if (index == 1 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 6) > 0) {
        if (btlActorEntryIsExpired(unit, 6) != 0) {
            factor *= 2.25f;
            btlBossDebugPrintf("btl:MAGICx2\n");
        }
    }
    return factor;
}

/* Apply mask-selected signed entry deltas, then sign-specific clears. */
void btlApplyMaskedActorEntryAdjustments(BtlUnit *unit, u32 flags, s16 delta) {
    if (flags == 0) {
        return;
    }
    if (flags & 1) {
        btlAdjustActorEntryCode(unit, 0, delta);
    }
    if (flags & 2) {
        btlAdjustActorEntryCode(unit, 0, -delta);
    }
    if (flags & 4) {
        btlAdjustActorEntryCode(unit, 1, delta);
    }
    if (flags & 8) {
        btlAdjustActorEntryCode(unit, 1, -delta);
    }
    if (flags & 0x10) {
        btlAdjustActorEntryCode(unit, 2, delta);
    }
    if (flags & 0x20) {
        btlAdjustActorEntryCode(unit, 2, -delta);
    }
    if (flags & 0x40) {
        btlAdjustActorEntryCode(unit, 3, delta);
    }
    if (flags & 0x80) {
        btlAdjustActorEntryCode(unit, 3, -delta);
    }
    if (flags & 0x100) {
        btlAdjustActorEntryCode(unit, 4, delta);
    }
    if (flags & 0x200) {
        btlAdjustActorEntryCode(unit, 4, -delta);
    }
    if (flags & 0x400) {
        btlAdjustActorEntryCode(unit, 5, delta);
    }
    if (flags & 0x2000) {
        btlAdjustActorEntryCode(unit, 6, delta);
    }
    if (flags & 0x800) {
        if (btlGetActorEntryCode(unit, 0) > 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) > 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) > 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) > 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) > 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
    if (flags & 0x1000) {
        if (btlGetActorEntryCode(unit, 0) < 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) < 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) < 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) < 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) < 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
}

/* Index of the lowest set bit pair in the mask (bits 10 and 13 are single), or -1. */
s32 btlLowestSetPairIndex(u32 mask) {
    s32 index = 0;
    s32 step = 1;

    if (mask == 0)
        return -step;
    if (mask & 0x0001)
        return index;
    if (mask & 0x0002)
        return index;
    index += step;
    if (mask & 0x0004)
        return index;
    if (mask & 0x0008)
        return index;
    index += step;
    if (mask & 0x0010)
        return index;
    if (mask & 0x0020)
        return index;
    index += step;
    if (mask & 0x0040)
        return index;
    if (mask & 0x0080)
        return index;
    index += step;
    if (mask & 0x0100)
        return index;
    if (mask & 0x0200)
        return index;
    index += step;
    if (mask & 0x0400)
        return index;
    index += step;
    if (mask & 0x2000)
        return index;
    return -step;
}

void btlTickActorEntryCountdowns(BtlUnit *unit) {
    s16 *entry = &unit->entrySlots[0].countdown;
    u32 i;
    for (i = 0; i < 7; i++) {
        if (*entry >= 0) {
            if (*entry == 0) {
                *entry = -1;
            }
            *entry = *entry - 1;
        }
        entry += 3;
    }
}

/* Return the element-ability scale, including native party-unit default factors. */
f32 btlGetElementAbilityMultiplier(BtlUnit *unit, s32 attr) {
    f32 scale = 1.0f;

    switch (attr) {
    case 2:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x22D)) {
            scale *= datAbilityParameters[0x22D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x232)) {
            scale *= datAbilityParameters[0x232 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 3) {
                scale *= datBattleParameters->defaultElementAbilityScale;
            }
        }
        break;
    case 3:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x22E)) {
            scale *= datAbilityParameters[0x22E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x233)) {
            scale *= datAbilityParameters[0x233 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 1) {
                scale *= datBattleParameters->defaultElementAbilityScale;
            }
            if (unit->partyRecord.unitId == 2) {
                scale *= datBattleParameters->defaultElementAbilityScale;
            }
        }
        break;
    case 4:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x22F)) {
            scale *= datAbilityParameters[0x22F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x234)) {
            scale *= datAbilityParameters[0x234 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 6) {
                scale *= datBattleParameters->defaultElementAbilityScale;
            }
            if (unit->partyRecord.unitId == 7) {
                scale *= datBattleParameters->defaultElementAbilityScale;
            }
        }
        break;
    case 5:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x230)) {
            scale *= datAbilityParameters[0x230 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x235)) {
            scale *= datAbilityParameters[0x235 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 5) {
                scale *= datBattleParameters->defaultElementAbilityScale;
            }
        }
        break;
    case 6:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x231)) {
            scale *= datAbilityParameters[0x231 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x236)) {
            scale *= datAbilityParameters[0x236 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 4) {
                scale *= datBattleParameters->defaultElementAbilityScale;
            }
        }
        break;
    }
    return scale;
}

/* Return the ability-adjusted attribute percentage; the suppression flag leaves it at 100. */
s32 btlGetAbilityAttributeMultiplierPercent(BtlUnit *actor, s32 attr) {
    u32 value = 100;

    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return value;
    }
    switch (attr) {
    case 0:
    case 1:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25C)) {
            value = (u32)(datAbilityParameters[0x25C - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 2:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25D)) {
            value = (u32)(datAbilityParameters[0x25D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 3:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25E)) {
            value = (u32)(datAbilityParameters[0x25E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 4:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25F)) {
            value = (u32)(datAbilityParameters[0x25F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 5:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x260)) {
            value = (u32)(datAbilityParameters[0x260 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 6:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x261)) {
            value = (u32)(datAbilityParameters[0x261 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 8:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x262)) {
            value = (u32)(datAbilityParameters[0x262 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 9:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x263)) {
            value = (u32)(datAbilityParameters[0x263 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    }
    return value;
}

s32 btlHasMappedSpecialAbilityForSlot(BtlUnit *unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot < 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x26F)) {
        return 1;
    }
    if (slot == 8 && btlCheckSpecialAbility(&unit->partyRecord, 0x271)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(&unit->partyRecord, 0x272)) {
        return 1;
    }
    if (slot == 10 && btlCheckSpecialAbility(&unit->partyRecord, 0x264)) {
        return 1;
    }
    if (slot == 11 && btlCheckSpecialAbility(&unit->partyRecord, 0x265)) {
        return 1;
    }
    if (slot == 12 && btlCheckSpecialAbility(&unit->partyRecord, 0x266)) {
        return 1;
    }
    if (slot == 13 && btlCheckSpecialAbility(&unit->partyRecord, 0x267)) {
        return 1;
    }
    if (slot == 14 && btlCheckSpecialAbility(&unit->partyRecord, 0x268)) {
        return 1;
    }
    if (slot < 15) {
        if (slot >= 10 && btlCheckSpecialAbility(&unit->partyRecord, 0x273)) {
            return 1;
        }
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x277)) {
            return 1;
        }
    }
    if (slot != 7) {
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x269)) {
            return 1;
        }
        if (btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF7)) {
            return 1;
        }
    }
    if (slot == 3 && btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF2)) {
        return 1;
    }
    return 0;
}

s32 btlHasSpecialAbility274(BtlUnit *unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot < 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x274)) {
        return 1;
    }
    return 0;
}

s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot == 8 && btlCheckSpecialAbility(&unit->partyRecord, 0x275)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(&unit->partyRecord, 0x276)) {
        return 1;
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x278)) {
            return 1;
        }
    }
    if (slot == 8 && btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF0)) {
        return 1;
    }
    if (slot == 9 && btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF1)) {
        return 1;
    }
    return 0;
}

/* Return the enabled low-HP ability's multiplier from the native HP-percent thresholds. */
f32 btlGetLowHpAbilityMultiplier(BtlUnit *unit) {
    DatPartyRecord *entry = &unit->partyRecord;
    s32 maximum;
    s32 percentage;

    if (btlCheckSpecialAbility(entry, 0x27A) == 0) {
        return 1.0f;
    }
    maximum = btlComputeSkillAdjustedMaxHp(entry);
    percentage = (s32)((f32)btlReadCurrentUnitHp(entry) / (f32)maximum * 100.0f);
    if (percentage < 6) {
        return 3.0f;
    }
    if (percentage < 11) {
        return 2.2f;
    }
    if (percentage < 16) {
        return 1.7f;
    }
    if (percentage < 21) {
        return 1.4f;
    }
    if (percentage < 26) {
        return 1.2f;
    }
    if (percentage < 31) {
        return 1.1f;
    }
    return 1.0f;
}

f32 btlGetClampedBattleTableValue(BtlUnit *source) {
    BtlState *state = (BtlState *)btlGetRuntime();
    u16 index = state->activeGroupCount;
    if (index > 4) {
        index = 4;
    }
    return btlActiveGroupCountMultipliers[index];
}

s32 btlGetCommandAttributeName(s32 commandIndex) {
    s8 resource = datCommandSelectors[commandIndex].stat;
    if (resource < 0) {
        return 0;
    }
    return (s32)btlCommandAttributeNames[resource];
}

/* The label resource index is the copied party record's unit ID. */
void *btlGetIndexedUiResource(BtlUnit *unit) {
    return btlPartyUnitGunTypeNames[unit->partyRecord.unitId];
}

typedef struct BattleSavedActorState {
    BtlUnitEntrySlot entrySlots[7];
    u8 pad2A[2];
    s32 flags;
    u8 pad30[4];
    u16 status;
    u16 unitId;
} BattleSavedActorState;

typedef char BattleSavedActorStateSize[(sizeof(BattleSavedActorState) == 0x38) ? 1 : -1];

extern BattleSavedActorState btlSavedPartyUnitStates[3];

INCLUDE_RODATA(const s32, "game/code_001A8748", D_004152F8);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415308);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415318);

void btlPushSavedPartyUnitStates(void) {
    s32 count = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;

    memset(btlSavedPartyUnitStates, 0, sizeof(btlSavedPartyUnitStates));
    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                btlSavedPartyUnitStates[count].flags = unit->status.flags;
                btlSavedPartyUnitStates[count].unitId = unit->partyRecord.unitId;
                memcpy(btlSavedPartyUnitStates[count].entrySlots, unit->entrySlots,
                       sizeof(btlSavedPartyUnitStates[count].entrySlots));
                btlSavedPartyUnitStates[count].status = unit->partyRecord.status & 0xCFF9;
                count++;
                sdfDebugPrintf("btl:state push[%d:%X]\n", count,
                              unit->partyRecord.unitId);
            }
        }
    }
}

extern const char D_00415340[];

/* Restore saved party entries/status and the matching actors' transformation/death state. */
void btlRestorePartySlotSnapshots(void) {
    u32 index;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;

    for (index = 0; index < 3; index++) {
        if (btlSavedPartyUnitStates[index].unitId == 0) {
            continue;
        }
        for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x200) {
                    if (unit->partyRecord.unitId == btlSavedPartyUnitStates[index].unitId) {
                        if (btlSavedPartyUnitStates[index].flags & 0x1000) {
                            unit->status.flags |= 0x1000;
                            unit->partyRecord.flags |= 0x1000;
                        } else {
                            unit->status.flags &= ~0x1000;
                            unit->partyRecord.flags &= ~0x1000;
                        }
                        memcpy(unit->entrySlots, btlSavedPartyUnitStates[index].entrySlots,
                               sizeof(unit->entrySlots));
                        unit->partyRecord.status = btlSavedPartyUnitStates[index].status;
                        if ((unit->partyRecord.status & 0x7FFF) == 0x4000) {
                            unit->partyRecord.hp = 0;
                            unit->status.flags |= 0x20;
                        }
                        sdfDebugPrintf(D_00415340, index,
                                      unit->partyRecord.unitId);
                        break;
                    }
                }
            }
        }
    }
}

void btlSyncModelFlagFromEventThresholds(void) {
    if (evtCheckValueThreshold(0x53, 1) || evtCheckValueThreshold(0x54, 1)) {
        mdlFlagSet(0xa20);
    } else {
        mdlFlagClear(0xa20);
    }
}

extern u32 btlGetAdjustedSlotAffinity(BtlUnit *, s32, s32);

extern s32 evtRunContext(s32, s32, s32, s32, u16);

INCLUDE_RODATA(const s32, "game/code_001A8748", D_00415340);

s32 btlDetermineActionPressResult(BtlUnit *attacker, BtlUnit *defender, u32 command) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 element;
    s32 mask;
    s32 damage;
    s32 criticalAbility = -1;
    s32 automaticAbility = -1;
    s32 critical;
    s32 fixed;
    s32 automatic;
    f32 scale;
    s32 condition;

    element = btlGetActorIndexedSignedValue(attacker, command);
    mask = btlEncodeActorIndexAsSelectionMask(element);
    if (mask & 0xE0001) {
        return 1;
    }
    damage = btlGetAdjustedSlotAffinity(defender, command, element);
    if (!(mask & 4) && damage < 0) {
        return 4;
    }
    if (!(mask & 0x10006)) {
        if (!(mask & 0x100) || datCommandRecords[command].effectType != 0) {
            return 1;
        }
    }
    critical = 0;
    fixed = 0;
    scale = 1.0f;
    automatic = 0;
    switch (command) {
    case 0:
    case 0x94:
    case 0x95:
    case 0x96:
        if (btlCheckSpecialAbility(&attacker->partyRecord, 0x226)) {
            criticalAbility = 0x226;
            scale = datAbilityParameters[0x226 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&attacker->partyRecord, 0x227)) {
            automaticAbility = 0x227;
            automatic = (s32)(datAbilityParameters[0x227 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * 100.0f);
        }
        if (btlCheckSpecialAbility(&attacker->partyRecord, 0x228)) {
            automaticAbility = 0x228;
            automatic = (s32)(datAbilityParameters[0x228 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * 100.0f);
        }
        break;
    }
    condition = defender->partyRecord.status & 0x7FFF;
    switch (condition) {
    case 8:
    case 0x2000:
        fixed = 60;
        break;
    case 2:
    case 4:
        fixed = 100;
        break;
    }
    if (btlCheckSpecialAbility(&defender->partyRecord, 0x24F)) {
        scale *= datAbilityParameters[0x24F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    if (datCommandRecords[command].criticalPercent == 100) {
        critical = 100;
        fixed = 100;
    }
    if (datCommandRecords[command].effectType == 0) {
        if (battle->unk220 & 0x1000) {
            return 2;
        }
        if (critical == 0) {
            critical = evtRunContext(4, (s32)&attacker->partyRecord, (s32)&defender->partyRecord, command, 0);
        }
        if (mdlFlagTest(0x80E) && (attacker->status.flags & 0x400) && (defender->status.flags & 0x200)) {
            scale *= datBattleParameters->criticalScale;
        }
        critical = (s32)((f32)critical * scale);
        if (critical >= 36) {
            critical = 35;
        }
        if ((attacker->partyRecord.flags & 0x10) && (attacker->status.flags & 0x200)) {
            critical += datBattleParameters->criticalPartyAttackerBias;
        }
        if ((defender->partyRecord.flags & 0x10) && (defender->status.flags & 0x200)) {
            critical += datBattleParameters->criticalPartyDefenderBias;
        }
        btlBossDebugPrintf("btl:critical=%d%%[raito=%.3f,fix=%d,auto=%d]\n", critical, scale, fixed, automatic);
        if (btlRollAiBucket() < fixed) {
            return 2;
        }
        if (btlRollAiBucket() < automatic) {
            if (automaticAbility != -1) {
                attacker->pendingAbilityId = automaticAbility;
            }
            return 2;
        }
        if (btlRollAiBucket() < critical) {
            if (criticalAbility != -1) {
                attacker->pendingAbilityId = criticalAbility;
            }
            return 2;
        }
    }
    if ((mask & 4) && damage < 0) {
        return 4;
    }
    return 1;
}
