#include "sdk_ps2.h"
#include "fld_spark_reward_profile.h"
#include "snd_packed_track_load_status.h"
#include "fld_item_box_object_api.h"
#include "fld_projected_sprite.h"
#include "sdf_model_packet_callback.h"
#include "fld_label_rows.h"
#include "dsp_channel.h"
#include "evt_message_window_api.h"
#include "dsp_entry_api.h"
#include "fld_area_work.h"
#include "fld_room_effect_entry.h"
#include "fld_object_slot.h"
#include "fld_activation_entry.h"
#include "fld_npc_palette.h"
#include "file_pac.h"
#include "evt_world.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "common.h"
#include "fld_archive_load_state.h"
#include "dds3_object_flags.h"
#include "sdf_vu0.h"
#include "snd_track_playback_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_texture_draw_packet.h"
#include "fld_scene_ready_api.h"
#include "fld_clear_menu_entries_api.h"
#include "fld_play_pending_sounds_api.h"
#include "fld_scene_record_api.h"
#include "fld_object_slot_api.h"
#include "fld_release_texture_slots_api.h"
#include "fld_spark_object_entry.h"
#include "fld_spark_slot.h"
#include "fld_spark_controller_state.h"
#include "sdf_math_vector4.h"
#include "mnu_title_stream.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_model_scalars.h"
#include "sdf_motion.h"
#include "sdf_chip.h"
#include "sdf_dev_state.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "sdf_model.h"
#include "fld_scene_record.h"
#include "fld_transfer_chunk.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "mdl.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_create_node_from_parameter.h"
#include "eff_node_descriptor.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"
#include "sdf_textured_rect.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "kwln_fade.h"
#include "fr_font.h"
#include "itf.h"
#include "sdf_packet_list.h"
#include "sdf_packet_alloc.h"

extern f32 fabsf(f32);
extern s32 fldTitleIsActive(void);
extern void p2sdSendStopCmd(u32 controlWord);
extern void p2sdStopAllBgm(void);
extern s32 sdfDelayedResrcCheck(void);
extern void func_00148A98(void);
extern void dds3WorkClear(void);
extern void fldAddCameraModelEntry(s32 motionIndex);

extern void fldSetCameraNodeModeWithTen(void);


extern void FromRotation(f32 angle);
extern dds3Basic_t *dds3AddUnitObjectBasic(s32 value, u128 *position, u128 *rotation);
extern void dds3SetUnitObjectId(dds3Basic_t *node, const char *objectName);
extern void dds3RebuildObjectModelContext(dds3Basic_t *node, s32 kind, s32 resource);
extern void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value);

extern void fldApplyLightSetToChaseCameraUnit(void);
extern void func_001526B8(void);
extern void fldInitializeSceneGuideMarkers(void);
extern dds3Basic_t *D_00435F1C;
extern dds3ModelHandle_t *D_00435F20;

extern void fldUpdateGuideMarkerPulse(void);
extern void fldUpdateTargetGuideMovementState(void);
extern void fldDispatchTargetGuideMotionUpdate(void);
extern void fldUpdateTargetGuideEffects(void);
extern void func_00153068(void);
extern s32 fldFindNearestOccupiedMapGridCell(f32 x, f32 z, s16 *gridX, s16 *gridY);


extern s32 D_003899E0[];
extern f32 aViewTarget[4];
extern f32 sdfViewTargetVector[4];
extern u8 D_00435C48[2][2];
extern f32 aViewDown[4];
extern void fldAutoMapGetBBox(f32 *, f32 *, f32 *, f32 *);
typedef struct EffNode EffNode;

extern u8 fldEncountTestFlag(u32);

extern f32 fldAngleDifference(f32 fromAngle, f32 toAngle);









extern sdfModelRootNode_t *D_003A5470[];

/* Native scene rows copy XYZ floats into this twelve-byte position cache. */
extern FldPoint D_0044F818[];

extern s32 D_004362D4;

extern u32 D_004362D8;

extern s32 D_004362E0;

extern EffNodeDescriptor *D_004362E4;

extern s32 D_004362EC;

extern EffNodeDescriptor *D_004362F0;

extern s32 D_004362F8;

extern EffNodeDescriptor *D_004362FC;

extern s32 D_00436304;

extern s32 D_00436308;

extern s32 D_00436310;

extern s32 D_00436314;

extern s32 D_00436318;

extern s32 D_0043631C;

extern s32 D_00436338;

extern s32 D_0043633C;

extern s32 D_00438EDC;

extern s32 D_00438EE0;

extern s32 D_00438EE4;

extern s32 D_00438EE8;

extern s32 D_00438EEC;

extern s32 D_00438EF0;


extern struct sdfMemHandle_t *D_004363C4;

extern void mnuAdvanceTitleStateUnderSemaphore(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern u32 D_00436330[];



extern s32 fldSceneSoundBase;


extern f32 fldCamTarget[];

extern s32 D_003899D8[];

extern s32 D_00436228;

extern s32 D_0039A0B8[];

extern s32 D_0039A028[];


extern char D_00413F20[];



extern sdfTexHandle_t *D_0044F7F0[];
extern u32 D_004363C8;

extern u32 D_004363CC;

extern u32 D_004363D0;

extern u32 D_004363D4;

extern struct sdfMemHandle_t *D_00436380;

extern u32 D_00436384;

extern EffNode *fldDamEffectNode;

extern u32 fldDamEffectPositioned;

extern struct sdfMemHandle_t *D_00436390;

extern u32 D_00436394;

extern EffNode *fldYukEffectNode;

extern u32 fldYukEffectPositioned;

extern sdfTexHandle_t *D_004363AC;

extern sdfTexHandle_t *D_004363B0;

extern sdfTexHandle_t *D_004363B4;

extern s32 D_00436370;

extern s32 D_00436374;

extern sdfTexHandle_t *D_00436368;

extern sdfTexHandle_t *D_0043636C;

extern struct sdfMemHandle_t *fldIndexedResourceHandle;

extern EffNodeDescriptor *fldIndexedResourceData;

extern EffNode *fldIndexedResourceEffect;


extern u32 D_0043623C;

extern dds3ProcessID_t *addressprocess_id;


extern u32 fldSeLastBgm;

extern u32 fldArchiveLoadPending;

extern u32 fldFixedArchiveLoadPhase;

extern u32 AutoMapSeq;

extern u32 D_00436364;

extern s32 D_00436354;

extern s32 D_0043635C;

extern s32 D_00436350;

extern s32 D_00436358;

extern s32 D_00436360;


extern s32 fldTitleTaskUpdate(dds3ProcessID_t *task);

extern sdfTexHandle_t *D_0043637C;

extern void fldPauseBgm(void);

extern void fldBgmStop(void);

extern void fldBgmFadeOut(void);

extern void fldPlayMenuSound(s32 soundId);

extern void fldPlayFieldSe(s32 soundId);

extern u8 fldPollArchiveLoad(s32 archiveId);

extern void fldSetArchiveSoundVolumePan(s32 archiveId, s32 soundId);

extern void fldPlayArchiveSound(s32 archiveId, s32 soundId);

extern void fldStartTitle(s32 field, s32 mode, s32 option);

extern void fldInitializeHuntSparkUnit(evtUnitHandle_t *unit);

extern s32 fldFindEffectByName(const char *name);

extern s32 fldGetCurrentSceneSelectionResource(void);



extern s32 fldFieldTaskUpdate(dds3ProcessID_t *task);

extern u8 D_00436208[];

extern s32 gFldSe3DNum;

typedef struct {
    s32 flags;
    s32 soundId;
    f32 radius;
    f32 x;
    f32 y;
    f32 z;
} FldClear18; /* 0x18 bytes */

extern FldClear18 fldSe3DTable[];



extern void p2sdStopBGM(u32 trackHandle);

extern s32 fldCurrentBgmId[];

extern void p2sdStopSe(u32);

extern void fldAddressResolver(u8 *buffer, FldTransferChunk *chunk);

extern s32 D_00389780[];

extern s32 fldAutoMapField;

extern s32 D_00436248;

extern s32 D_00389834[];

extern s32 D_00389838[];

extern s32 fldPrimaryEffectPositionPending;

extern f32 fldPrimaryQueuedEffectPosition[];

extern s32 fldSecondaryEffectPositionPending;

extern f32 fldSecondaryQueuedEffectPosition[];

extern char fldTitleTaskName[]; /* "fldTitle" */


extern char D_00413C80[]; /* "fldTitleMini" */


extern void dds3DeleteUnitObjectBasic(dds3Basic_t *node);

extern FldSparkObjectEntry fldSparkObjectEntries[];

extern s32 D_00389884[];

extern u8 D_003A9EB0[];



extern void fldResetPlayerSceneObjectState(void);


#include "sdf_debug_api.h"

extern void fldPreparePlayerSceneCameraTarget(void);

extern EffNode *mnuPositionedResourceNodes[];

extern s32 mnuPositionedResourceActive[];

extern s32 D_0043633C;


extern void fldClearFloorFlag(s32, s32, s32);

extern void fldSetFloorFlag(s32, s32, s32);

extern void sdfDrawSync(void);

extern void fldReleaseSceneDevSlotsAndTextures(void);


extern s32 fldGetCurrentSceneSelectionId(void);

typedef struct {
    s32 *resource;
    u8 pad[0x4C];
} FieldResourceSlot;

extern FieldResourceSlot D_0044FFB0[];
#include "fld_label_cache_api.h"

extern void fldLoadSceneRecordTable(void);

extern s32 fldStageSoundBaseTable[];


typedef struct {
    s16 id;
    s16 flag;
    s8 data[0x40];
} FldEnt44; /* 0x44 bytes */

extern FldEnt44 D_00389A70[32];

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 MidiNo;

extern s32 fldBgmReloadStep;

extern SndPackedTrackLoadStatus sndFindPackedTrackLoadStatus(s32);

extern void p2sdLoadSMGFile(s32);

extern s32 fldAutoMap_GetFloor(s32 scene, u32 index);

extern struct sdfMemHandle_t *fldEffectTextureLoadHandles[];

extern s32 fldEffectTextureData[];

extern EffNode *fldEffectTextureNodes[];

extern struct sdfMemHandle_t *fldEffectTextureLoadHandles[4];

extern s32 fldEffectTextureData[4];

extern EffNode *fldEffectTextureNodes[4];

extern s32 D_00436300;

extern s32 D_0043630C;

extern s32 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit);




extern FldSparkSlot fldSparkSlots[];




extern SdfMathVector4 D_00413DE8;

struct FieldResourceRecord;

/* DDS2 stores the selected reward row in its inserted +8 word; sequence and
 * pickup bookkeeping grow the controller to 0x4C. */
typedef struct FldSparkController {
    void *object;
    struct FieldResourceRecord *entry;
    FldSparkRewardProfileIndex rewardProfileIndex;
    FldSparkControllerState state;
    s32 cursor;
    s32 pickupDelay;
    s32 scheduledSounds;
} FldSparkController;

typedef char FldSparkControllerSizeCheck[(sizeof(FldSparkController) == 0x4C) ? 1 : -1];
typedef char FldSparkControllerStateOffsetCheck[
    ((u32)&((FldSparkController *)0)->state == 0x0C) ? 1 : -1];
typedef char FldSparkControllerRewardProfileIndexOffsetCheck[
    ((u32)&((FldSparkController *)0)->rewardProfileIndex == 0x08) ? 1 : -1];

extern FldSparkController fldSparkControlState;

extern void fldCreateSparkSlotObjects();

extern s32 fldAttachSparkSlotToWorldObject(s32, s32);







extern u8 fldProcCampChk(void);
extern u8 fldProcAmChk(void);
extern s32 fileMenuTaskExists(void);
extern u8 fldHasKiretaLabelProcess(void);
extern u8 fldProcKiretaChk(void);
extern u8 fldProcHirakenaiChk(void);
extern u8 fldIsEventPhaseAtLeastTwo(void);
extern dds3ProcessID_t *inputpanel_id;
extern void fldUpdateCameraHeadingFromXY(void);
extern u32 fldRunDanteGunFlag[];
extern u8 dds3PadButtonState[2][2][16];
extern s32 gFldInf_WpExit;

#include "fld_title_banner_menu.h"

extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];


FldTitleBannerResult fldResolvePendingTitleBanner(void) {
    FldTitleBannerMenu *menu;
    FldAreaWork *area;

    if (fldProcCampChk() != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fldProcAmChk() != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fldRunDanteGunFlag[0] != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fileMenuTaskExists() != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fldHasKiretaLabelProcess() != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fldProcKiretaChk() != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fldProcHirakenaiChk() != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fldIsEventPhaseAtLeastTwo() != 0) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }

    menu = (FldTitleBannerMenu *)dds3GetProcessData(inputpanel_id);
    if (menu->pending == 0) {
        return FLD_TITLE_BANNER_NO_PENDING;
    }
    if (fldGlobalBase.titleFade != 0 ||
        gFldInf_WpExit == 1 || (s8)dds3PadButtonState[1][0][1] < 0) {
        if ((s8)dds3PadButtonState[1][0][1] < 0) {
            fldUpdateCameraHeadingFromXY();
        }
        area = &fldGlobalBase;
        menu->pending = 0;
        if (area->titleFade != 0) {
            dds3FadeIn2(8);
            area->titleFade = 0;
        }
        return FLD_TITLE_BANNER_RESOLVED;
    }
    return FLD_TITLE_BANNER_UNRESOLVED;
}

extern s32 gFldElvFloorNow;

extern void fldDrawAnimatedFieldBanner(s32 alpha, s32 x, s32 y);

void func_0014B0F8(); /* no prototype: called here with arguments */

s32 fldFieldTaskUpdate(dds3ProcessID_t *task) {
    if (fldProcCampChk() != 0) {
        return 0;
    }
    if (fldProcAmChk() != 0) {
        return 0;
    }
    if (fileMenuTaskExists() != 0) {
        return 0;
    }
    if (fldTitleIsActive() != 0) {
        return 0;
    }
    if (D_00389884[0] == 1) {
        return 0;
    }
    if (gFldElvFloorNow != -0x3e7) {
        fldDrawAnimatedFieldBanner(0x80, 0, 0);
    } else {
        func_0014B0F8(0);
    }
    return 0;
}

s32 fldAddressProcess1(dds3ProcessID_t *task) {
    s32 *work;

    work = sdfChipAllocDirect(0x10);
    work[0] = 0;
    work[1] = 0;
    work[2] = 0;
    work[3] = 0;
    dds3SetProcessData(task, (u32)work);
    return (s32)(u32)fldFieldTaskUpdate;
}

void fldAddressShutdown(dds3ProcessID_t *task) {
    void *work;

    work = (void *)dds3GetProcessData(task);
    sdfChipFreeDirect(work);
    addressprocess_id = NULL;
}

void fldInitAddressProcess(void) {
    if (addressprocess_id == NULL) {
        addressprocess_id = dds3StartProcess(D_00436208, 0x2B0B, 1, 1, fldAddressProcess1, fldAddressShutdown, 0);
    }
}

void fldFreeAddressProcess(void) {
    if (addressprocess_id != NULL) {
        dds3KillProcess(addressprocess_id, 1);
    }
}

void fldSe_Init(void) {
    FldClear18 *entry = fldSe3DTable;
    s32 remaining = 7;

    do {
        remaining -= 1;
        entry->flags = 0;
        entry->radius = 0;
        entry->soundId = 0;
        entry += 1;
    } while (remaining >= 0);
    gFldSe3DNum = 0;
}

void fldAppendPendingSoundForScene(s32 soundId, f32 radius, f32 sourceX, f32 sourceY, f32 sourceZ) {
    if (fldGlobalBase.area == 0x1A && evtBitCheck(0x1F) != 0) {
        return;
    }
    if (fldGlobalBase.area == 0x17 && fldGlobalBase.floor == 7 && (evtBitCheck(0x4C5) == 0 || evtBitCheck(0x1C) != 0)) {
        return;
    }
    fldSe3DTable[gFldSe3DNum].flags = 0;
    fldSe3DTable[gFldSe3DNum].soundId = soundId;
    fldSe3DTable[gFldSe3DNum].radius = radius;
    fldSe3DTable[gFldSe3DNum].x = sourceX;
    fldSe3DTable[gFldSe3DNum].y = sourceY;
    fldSe3DTable[gFldSe3DNum].z = sourceZ;
    gFldSe3DNum++;
}

void fldPlayPendingSounds(void) {
    s32 stage;
    s32 i;

    stage = D_00389780[0];
    if (stage == 11) {
        stage = evtBitCheck(0x13) != 0 ? 2 : stage;
    }
    for (i = 0; i < gFldSe3DNum; i++) {
        if (fldSe3DTable[i].flags & 1) {
            fldSe3DTable[i].flags &= ~1;
            p2sdStopSe(fldStageSoundBaseTable[stage] + fldSe3DTable[i].soundId);
        }
    }
}

extern s32 D_00436220;
extern s32 D_00436224;
extern f32 fldLookAtNearPoint[];
typedef struct SoundPosition {
    f32 v[4];
} SoundPosition;
extern const SoundPosition D_004136C0;
extern void p2sdPlaySe3DEx(s32, s32, f32, f32, f32);
extern void p2sdSetStandPos(f32, f32, f32, f32, f32, f32);
void func_001447A0(void) {
    SoundPosition position = D_004136C0;
    f32 listenerX, listenerY, listenerZ;
    f32 extraRadius;
    f32 dx, dy, dz, distance, vertical;
    f32 sourceX, sourceY, sourceZ;
    s32 tick;
    s32 nearby;
    s32 flags;
    s32 stage;
    s32 i;
    FldClear18 *entry;

    tick = D_00436224 + 1;
    nearby = 0;
    D_00436224 = tick;
    if (fldGlobalBase.mode == FLD_CAMERA_MODE_PLAYER_FOLLOW) {
        listenerX = fldCamTarget[0] - fldLookAtNearPoint[0];
        extraRadius = 500.0f;
        listenerY = fldLookAtNearPoint[1];
        listenerZ = fldCamTarget[2] - fldLookAtNearPoint[2];
        distance = fsqrtf(listenerX * listenerX + listenerZ * listenerZ);
        if (distance > 200.0f) {
            listenerX = fldLookAtNearPoint[0] + (listenerX / distance) * 200.0f;
            listenerZ = fldLookAtNearPoint[2] + (listenerZ / distance) * 200.0f;
        } else {
            listenerX = fldCamTarget[0];
            listenerZ = fldCamTarget[2];
        }
    } else {
        extraRadius = 0.0f;
        listenerX = fldGlobalBase.x;
        listenerY = fldGlobalBase.y;
        listenerZ = fldGlobalBase.z;
    }
    for (i = 0; i < gFldSe3DNum; i++) {
        dx = listenerX - fldSe3DTable[i].x;
        dy = listenerY - fldSe3DTable[i].y;
        dz = listenerZ - fldSe3DTable[i].z;
        distance = fsqrtf(dx * dx + dy * dy + dz * dz);
        if (distance < fldSe3DTable[i].radius + extraRadius) {
            distance = fsqrtf(dy * dy);
            if (distance < 500.0f) {
                nearby = 1;
            }
        }
    }
    if (nearby != 0 && (tick & 1)) {
        if (fldGlobalBase.mode == FLD_CAMERA_MODE_PLAYER_FOLLOW) {
            p2sdSetStandPos(listenerX, listenerY, listenerZ,
                fldLookAtNearPoint[0], fldLookAtNearPoint[1], fldLookAtNearPoint[2]);
        } else {
            p2sdSetStandPos(fldCamTarget[0], fldCamTarget[1], fldCamTarget[2],
                fldLookAtNearPoint[0], fldLookAtNearPoint[1], fldLookAtNearPoint[2]);
        }
    }
    stage = fldGlobalBase.area;
    if (stage == 11) {
        stage = evtBitCheck(0x13) != 0 ? 2 : stage;
    }
    for (i = 0; i < gFldSe3DNum; i++) {
        entry = &fldSe3DTable[i];
        sourceX = fldSe3DTable[i].x;
        sourceY = fldSe3DTable[i].y;
        sourceZ = fldSe3DTable[i].z;
        dx = listenerX - sourceX;
        dy = listenerY - sourceY;
        dz = listenerZ - sourceZ;
        vertical = fsqrtf(dy * dy);
        distance = fsqrtf(dx * dx + dy * dy + dz * dz);
        if (vertical > 500.0f) {
            distance = 99999.0f;
        }
        flags = entry->flags;
        if (!(flags & 1)) {
            if (distance < entry->radius + extraRadius) {
                position.v[0] = sourceX;
                position.v[1] = sourceY;
                position.v[2] = sourceZ;
                entry->flags = flags | 1;
                p2sdPlaySe3DEx(fldStageSoundBaseTable[stage] + entry->soundId,
                    (s32)(entry->radius / 100.0f), position.v[0], position.v[1], position.v[2]);
            }
        } else if (distance < entry->radius + extraRadius) {
            position.v[0] = sourceX;
            position.v[1] = sourceY;
            position.v[2] = sourceZ;
        } else {
            entry->flags = flags & ~1;
            p2sdStopSe(fldStageSoundBaseTable[stage] + entry->soundId);
        }
    }
    D_00436220++;
}

extern s32 D_0039A0BC[];

extern s32 D_0039A0C0[];

extern s32 D_0039A0C4[];

extern s32 D_0039A0C8[];

extern s32 D_0039A0D0[];

extern const char D_00413700[];

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004136C0);

s32 fldResolveSpecialBgmTrack(s32 id) {
    s32 result;

    switch (id) {
    case 0x80:
        result = D_0039A0B8[0] + 1;
        break;
    case 0x81:
        result = D_0039A0BC[0] + 1;
        break;
    case 0x82:
        result = D_0039A0C0[0] + 1;
        break;
    case 0x83:
        result = D_0039A0BC[0] + 2;
        break;
    case 0x84:
        result = D_0039A0C4[0] + 1;
        break;
    case 0x85:
        result = D_0039A0C4[0] + 2;
        break;
    case 0x86:
        result = D_0039A0C8[0] + 1;
        break;
    case 0x87:
        result = D_0039A0D0[0] + 1;
        break;
    case 0x88:
        result = D_0039A0D0[0] + 2;
        break;
    case 0x89:
        result = D_0039A0D0[0] + 3;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

s8 fldFindSceneEntryData(s32 id, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_00389A70[i].id == id && (D_00389A70[i].flag == 0 || evtBitCheck(D_00389A70[i].flag) != 0)) {
            return D_00389A70[i].data[idx];
        }
    }
    return 0;
}

void fldStartSceneBgm(void) {
    s32 handle;
    s32 stage;
    s32 idx;

    if (D_003899D8[0] != 0) {
        handle = fldResolveSpecialBgmTrack(fldGlobalBase.sequenceCode);
        if (handle != -1 || fldGlobalBase.area < 0x32) {
            if (handle == -1) {
                stage = fldGlobalBase.area;
                if (stage == 11) {
                    stage = evtBitCheck(0x13) != 0 ? 2 : stage;
                }
                idx = fldFindSceneEntryData(stage, fldGlobalBase.floor + 1);
                fldSceneSoundBase = fldStageSoundBaseTable[stage];
                fldGlobalBase.sequenceCode = idx;
                handle = fldSceneSoundBase + idx;
            }
            if (fldGlobalBase.sceneBgmSuppressed != 1) {
                if (fldSeLastBgm != handle) {
                    p2sdStopBGM(fldSeLastBgm);
                }
                fldSeLastBgm = handle;
                p2sdPlayBGM(handle);
            }
        }
    }
}

s32 fldPlayFieldBgm(void) {
    if (D_003899D8[0] != 0) {
        if (fldCurrentBgmId[0] == 0x80) {
            p2sdPlayBGM(D_0039A0B8[0] + 1);
        }
    }
}

extern void p2sdResumeBgm(s32, s32);

void fldStartSceneBgmAlternate(void) {
    s32 handle;
    s32 stage;
    s32 idx;

    if (D_003899D8[0] != 0) {
        handle = fldResolveSpecialBgmTrack(fldGlobalBase.sequenceCode);
        if (handle != -1 || fldGlobalBase.area < 0x32) {
            if (handle == -1) {
                stage = fldGlobalBase.area;
                if (stage == 11) {
                    stage = evtBitCheck(0x13) != 0 ? 2 : stage;
                }
                idx = fldFindSceneEntryData(stage, fldGlobalBase.floor + 1);
                fldSceneSoundBase = fldStageSoundBaseTable[stage];
                fldGlobalBase.sequenceCode = idx;
                handle = fldSceneSoundBase + idx;
            }
            if (fldGlobalBase.sceneBgmSuppressed != 1) {
                fldSeLastBgm = handle;
                p2sdResumeBgm(handle, handle);
            }
        }
    }
}

void fldPauseBgm(void) {
    p2sdSendStopCmd(fldSeLastBgm);
    fldSeLastBgm = 0;
}

void fldBgmFadeOut(void) {
    p2sdStopBGM(fldSeLastBgm);
    if (fldGlobalBase.sequenceCode >= 0x80) {
        fldGlobalBase.sequenceCode = 1;
    }
    fldSeLastBgm = 0;
}

void fldBgmStopAll(void) {
    p2sdStopAllBgm();
}

void fldBgmStop(void) {
    p2sdStopSe(fldSeLastBgm);
    fldCurrentBgmId[0] = 0;
}

void fldPlayMenuSound(s32 soundId) {
    if (fldGlobalBase.area < 0x32) {
        if (soundId >= 0x10) {
            if (soundId == 0x80) {
                p2sdPlaySE(0x6d0060, 0x7f, 0x3f);
            } else if (soundId == 0x81) {
                p2sdPlaySE(0x6d0061, 0x7f, 0x3f);
            } else if (soundId == 0x82) {
                p2sdPlaySE(0x6d0064, 0x7f, 0x3f);
            } else if (soundId >= 0x200) {
                p2sdPlaySE(0x68fe00 + soundId, 0x7f, 0x3f);
            } else {
                p2sdPlaySE(fldSceneSoundBase + soundId, 0x7f, 0x3f);
            }
        }
    }
}

void fldPlayFieldSe(s32 soundId) {
    if (D_00389780[0] < 50) {
        if (soundId == 0x100) {
            p2sdStopSe(0x6D0060);
        } else if (soundId == 0x101) {
            p2sdStopSe(0x6D0061);
        } else if (soundId == 0x102) {
            p2sdStopSe(0x6D0064);
        } else {
            p2sdStopSe(fldSceneSoundBase + soundId);
        }
    }
}

void fldSetSequenceVolume(s32 category, s32 volume) {
    if (D_00389780[0] < 50) {
        p2sdPlaySE(fldSceneSoundBase + category, volume, 0x3F);
    }
}

void fldSelectBgmMode(s32 selection) {
    s32 volume;
    if (D_003899D8[0] == 0) {
        return;
    }
    switch (selection) {
    case -1:
        if (D_00436228 != selection) {
            p2sdStopSe(D_00436228);
            D_00436228 = selection;
        }
        break;
    case 0:
        if (D_00436228 != -1) {
            p2sdStopBGM(D_00436228);
            D_00436228 = -1;
        }
        break;
    case 1:
        volume = D_0039A0B8[0] + 1;
        p2sdPlayBGM(volume);
        D_00436228 = volume;
        break;
    case 2:
        volume = D_0039A028[0] + 1;
        p2sdPlayBGM(volume);
        D_00436228 = volume;
        break;
    }
}

void fldPrepareSceneBgmArchive(void) {
    s32 handle = fldResolveSpecialBgmTrack(fldGlobalBase.sequenceCode);
    s32 stage;
    s32 idx;

    if (handle == -1) {
        if (fldGlobalBase.area >= 0x32) {
            return;
        }
        stage = fldGlobalBase.area;
        if (stage == 11) {
            stage = evtBitCheck(0x13) != 0 ? 2 : stage;
        }
        idx = fldFindSceneEntryData(stage, fldGlobalBase.floor + 1);
        fldSceneSoundBase = fldStageSoundBaseTable[stage];
        fldGlobalBase.sequenceCode = idx;
        handle = fldSceneSoundBase + idx;
    }
    MidiNo = handle;
    fldBgmReloadStep = 0;
}

s32 fldStepSceneBgmArchive(void) {
    switch (fldBgmReloadStep) {
    case 0:
        if (sndFindPackedTrackLoadStatus(MidiNo) == SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
            p2sdLoadSMGFile(MidiNo);
            fldBgmReloadStep = fldBgmReloadStep + 1;
        } else {
            fldBgmReloadStep = fldBgmReloadStep + 2;
        }
        break;
    case 1:
        if (sndFindPackedTrackLoadStatus(MidiNo) == SND_PACKED_TRACK_LOAD_RESIDENT) {
            fldBgmReloadStep = fldBgmReloadStep + 1;
        }
        break;
    default:
        fldBgmReloadStep = -1;
        return 1;
    }
    return 0;
}

FldArchivePendingState fldGetArchiveLoadPending(void) {
    return fldArchiveLoadPending;
}

u8 fldPollArchiveLoad(s32 archiveId) {
    s32 name = 0x30000000 + (archiveId << 16);
    SndPackedTrackLoadStatus result = sndFindPackedTrackLoadStatus(name);
    if (result == SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
        p2sdLoadSMGFile(name);
        fldArchiveLoadPending = FLD_ARCHIVE_PENDING_ACTIVE;
        return 0;
    }
    if (result == SND_PACKED_TRACK_LOAD_RESIDENT) {
        fldArchiveLoadPending = FLD_ARCHIVE_PENDING_NONE;
        return 1;
    }
    return 0;
}

void fldSetArchiveSoundVolumePan(s32 archiveId, s32 soundId) {
    p2sdPlaySE(archiveId * 0x10000 + soundId + 0x30000000, 0x7f, 0x3f);
}

void fldPlayArchiveSound(s32 archiveId, s32 soundId) {
    p2sdStopSe(archiveId * 0x10000 + soundId + 0x30000000);
}

void fldResetArchiveLoadPhase(void) {
    fldFixedArchiveLoadPhase = FLD_ARCHIVE_LOAD_PHASE_CHECK;
}

FldArchiveLoadResult fldStepArchiveLoad(void) {
    switch (fldFixedArchiveLoadPhase) {
    case FLD_ARCHIVE_LOAD_PHASE_CHECK:
        if (sndFindPackedTrackLoadStatus(0x680000) == SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
            p2sdLoadSMGFile(0x680000);
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        } else {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 2;
        }
        break;
    case FLD_ARCHIVE_LOAD_PHASE_WAIT:
        if (sndFindPackedTrackLoadStatus(0x680000) == SND_PACKED_TRACK_LOAD_RESIDENT) {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        }
        break;
    default:
        /* Phase 2 and other non-pending phases report completion. */
        fldFixedArchiveLoadPhase = FLD_ARCHIVE_LOAD_PHASE_REPORTED;
        return FLD_ARCHIVE_LOAD_COMPLETE;
    }
    return FLD_ARCHIVE_LOAD_PENDING;
}

void fldResetSecondaryArchiveLoadPhase(void) {
    D_0043623C = 0;
}

s32 fldStepSecondaryArchiveLoad(void) {
    switch (D_0043623C) {
    case 0:
        if (sndFindPackedTrackLoadStatus(0x690000) == SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
            p2sdLoadSMGFile(0x690000);
            D_0043623C = D_0043623C + 1;
        } else {
            D_0043623C = D_0043623C + 2;
        }
        break;
    case 1:
        if (sndFindPackedTrackLoadStatus(0x690000) == SND_PACKED_TRACK_LOAD_RESIDENT) {
            D_0043623C = D_0043623C + 1;
        }
        break;
    default:
        D_0043623C = -1;
        return 1;
    }
    return 0;
}

u32 fldGetCurrentBgmHandle(void) {
    return fldSeLastBgm;
}



typedef struct FldName28 { char s[0x1C]; } FldName28;

typedef struct FldName24 { char s[0x18]; } FldName24;

extern s16 D_0044FC98[];

extern s16 D_0044FD48[];

extern s16 D_0044FCF0[];

extern s16 D_00438ED8;



extern FldName28 D_003A25AC[];

extern FldName24 D_0039A1D0[];


extern s32 fldFindMapLabelIndexByCoordinate(s32 mapId, s32 floorIndex);

extern s32 fldFindSecondaryMapLabelIndexByCoordinate(s32 mapId, s32 floorIndex);

extern s32 fldFindMapCoordinateIndex(s32 mapId, s32 floorIndex);

void fldCacheMapLabelLengths(s32 mapId) {
    s32 i;

    for (i = 0; i < 41; i++) {
        D_0044FC98[i] = strlen((const char *)D_0039A5A8[fldFindMapLabelIndexByCoordinate(mapId, i)].name);
    }
    for (i = 0; i < 24; i++) {
        D_0044FD48[i] = strlen((const char *)D_0039E1A8[fldFindSecondaryMapLabelIndexByCoordinate(mapId, i)].name);
    }
    for (i = 0; i < 41; i++) {
        D_0044FCF0[i] = strlen(D_003A25AC[fldFindMapCoordinateIndex(mapId, i)].s);
    }
    i = 0;
    if (D_00389780[0] < 100) {
        i = D_00389780[0];
    }
    D_00438ED8 = strlen(D_0039A1D0[i].s);
}

void fldLoadSceneRecordTable(void) {
    char path[0x80];
    char directory[0x40];
    s32 resourceHandle;
    FldTransferChunk *transferStart;
    FldSceneRecordTableHeader *header;

    fldMakeFilePath(directory, fldGlobalBase.area, 1);
    sprintf(path, (const char *)D_00413700, directory, fldGlobalBase.area);
    fldAutoMapBuf = sdfDevLoadFileEx(path, &resourceHandle, 0);
    transferStart = (FldTransferChunk *)(resourceHandle + 8);
    fldAddressResolver((u8 *)resourceHandle, transferStart);
    header = (FldSceneRecordTableHeader *)fldGetInfoHeader(transferStart);
    fldAutoMapHeader = header->rows;
    fldAutoMapData = header->count;
}

void fldSetSceneRecordChunk(u8 *chunk, struct sdfMemHandle_t *resourceHandle) {
    if (D_00389780[0] < 0xC8) {
        FldTransferChunk *transferStart = (FldTransferChunk *)(chunk + 8);

        fldAutoMapBuf = resourceHandle;
        fldAddressResolver(chunk, transferStart);
        {
            FldSceneRecordTableHeader *header =
                (FldSceneRecordTableHeader *)fldGetInfoHeader(transferStart);
            FldSceneRecord *rows = header->rows;
            s32 count = header->count;

            fldAutoMapData = count;
            fldAutoMapHeader = rows;
        }
    }
}

void fldInitSceneMapLabels(void) {
    if (D_00389780[0] < 200) {
        fldCacheMapLabelLengths(D_00389780[0] % 100);
        fldLoadSceneRecordTable();
    }
}

void fldReleaseSceneRecordChunk(void) {
    if (fldAutoMapBuf != 0) {
        sdfMemFreeDelayedDirect(fldAutoMapBuf);
    }
    fldAutoMapBuf = 0;
    fldAutoMapHeader = 0;
    fldAutoMapData = 0;
}

void fldResetSceneResourceTables(void) {
    s32 i;

    fldGlobalBase.primaryMapLabelSelection = -1;
    fldGlobalBase.secondaryMapLabelSelection = -1;
    fldAutoMapBuf = 0;
    fldAutoMapHeader = 0;
    fldAutoMapData = 0;
    D_0044F7F0[0] = 0;
    D_0044F7F0[1] = 0;
    D_0044F7F0[2] = 0;
    D_0044F7F0[3] = 0;
    D_0044F7F0[4] = 0;
    D_0044F7F0[5] = 0;
    D_0044F7F0[6] = 0;
    D_0044F7F0[7] = 0;
    D_0044F7F0[8] = 0;
    for (i = 0; i < 96; i++) {
        D_003A5470[i] = 0;
        D_0044F818[i].x = 0.0f;
        D_0044F818[i].y = 0.0f;
        D_0044F818[i].z = 0.0f;
    }
}

void fldAutoMapDrawMdl1(sdfModelRootNode_t *emitter, f32 x, f32 y, f32 z) {
    f32 pos[4];
    sdfModelNode_t *handle;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    handle = *(sdfModelNode_t **)emitter->list->buffer;
    SDF_VU0_LOADVECTOR_BARRIER(pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(emitter->matrix[3]);
    sdfModelMakeLocalTm(handle);
    sdfModelEvalTm(emitter);
    sdfBuildModelPacketsVia(sdfModelBuildNodeResourcePackets, kwlnSceneSurfaceSets[11], emitter);
}

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32,
                                s32, sdfTexHandle_t *);

extern FldProjectedSprite D_0039A0E0[];

/* Convert projected GS coordinates to the field's centered sprite origin. */
void fldDrawProjectedMapIcon(f32 x, f32 y, s32 index, u32 color) {
    FldProjectedSprite *sprite = &D_0039A0E0[index];
    s32 anchorY = sprite->anchorY;
    s16 u = sprite->u;
    s16 v = sprite->v;

    y -= 2048.0f;
    x -= 2048.0f;
    y += y;
    y += 224.0f;
    x += 256.0f;
    DrwSprite2D((s32)(x - sprite->anchorX), (s32)(y - anchorY),
                        sprite->width, sprite->height, u, v,
                        sprite->width, sprite->height, color,
                        D_0044F7F0[D_0039A0E0[index].textureSlot]);
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145A08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145D48);

s32 fldAutoMap_GetFloor(s32 scene, u32 index) {
    s32 result = 1;
    FldSceneRecord *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)fldAutoMapData; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (i == scene && j == index) {
                result = item->floor + 1;
            }
        }
    }
    return result;
}

void fldAutoMapNodeSet(sdfModelRootNode_t *model, s32 index, s32 clearFlag) {
    sdfModelNode_t *parent = ((sdfModelNode_t **)model->list->buffer)[index + 1];
    sdfModelNode_t *first;
    sdfModelNode_t *node;
    sdfModelNode_t *next;
    sdfModelNode_t *headNext;
    u16 flags;

    if (parent->children == NULL) {
        return;
    }
    parent->children->flags &= ~SDF_DRAW_NODE_FLAG_SKIP_RENDER;
    if (clearFlag == 0) {
        parent->children->flags |= SDF_DRAW_NODE_FLAG_SKIP_RENDER;
    }

    first = parent->children;
    headNext = first->next;
    if (headNext == NULL) {
        return;
    }
    node = headNext;
    if (parent != node->parent || first == node) {
        return;
    }
    for (;;) {
        flags = node->flags & ~SDF_DRAW_NODE_FLAG_SKIP_RENDER;
        node->flags = flags;
        if (clearFlag == 0) {
            node->flags = flags | SDF_DRAW_NODE_FLAG_SKIP_RENDER;
        }
        next = node->next;
        if (next == NULL) {
            return;
        }
        node = next;
        if (parent != node->parent) {
            return;
        }
        if (first != node) {
            continue;
        }
        return;
    }
}

s32 fldAutoMapGetFloorMax(void) {
    s32 max = 0;
    FldSceneRecord *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)fldAutoMapData; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (max < item->floor) {
                max = item->floor;
            }
        }
    }
    return max + 1;
}

#include "field_stage.h"

extern s32 D_00436240, D_0043624C, D_00436250, D_00436254;
extern s32 D_00436288, D_0043628C, D_00436298, D_0043629C, D_004362A0;
extern f32 D_00436290, D_00436294, D_004362A4, D_004362A8, D_004362AC;
extern Matrix4 D_00384790, sdfViewMatrix;
extern f32 D_0037FA20[4] __attribute__((aligned(16)));
extern void frSetFontDrawOT(u32);
extern void InversMatrix(void);
extern s32 fldAutoMapGetFloorNext(s32), fldAutoMapGetFloorPrev(s32);
extern void rsetOtOfs(u32);
extern void rsetAlpha(s32);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void aDrawBoxF(s32, s32, s32, s32, u32, u32, u32, u32);
extern void eglSetOt(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void eglLinkPoly4(f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,u32,u32,u32,u32);
extern s32 fldIsStageExplorationCellMarked(s32 floor, f32 x, f32 z);
extern s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit);
extern void fldSubmitPrimaryFramePacket(void), fldSubmitAlternateFramePacket(void);
typedef struct _FRQ _FRQ;
extern _FRQ *D_00438ED4;
extern s32 D_00436240;
extern s32 D_00436248;
extern s32 D_00436250, D_00436254;
extern s32 D_00436288, D_0043628C, D_00436298, D_0043629C, D_004362A0;
extern f32 D_00436290, D_004362A4, D_004362A8, D_004362AC;

extern Matrix4 sdfViewMatrix;
extern void rsetAlpha(s32);
extern void rsetOtOfs(u32);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void aDrawBoxF(s32, s32, s32, s32, u32, u32, u32, u32);
extern s32 fldIsStageExplorationCellMarked(s32 floor, f32 x, f32 z);
extern s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit);
extern void fldPoint3To2H(f32 *, f32 *, f32, f32, f32);
extern void func_00145A08(s32, f32, f32, f32);
extern void fldAllIconDel(s32 index, f32 *x, f32 *z);
extern void fldAutoMapGetBBox(f32 *, f32 *, f32 *, f32 *);
extern s32 fldFindMapCoordinateIndex(s32 mapId, s32 floorIndex);
extern void frSetPriority(_FRQ *, u32);
extern s32 frFontDrawGlyphWithSharedFlags(_FRQ *, s8);
extern s32 frDestroyFontRequestAfter(_FRQ *);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413700);

void fldDrawAutomapScene(void) {
    s32 proposed;
    s32 selectedFloor;
    s32 itemVisible;
    s32 i, j, k;
    FldSceneRecord *record;
    FldItem *item;
    FieldStageCoordinate *stage;
    f32 left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
    f32 sceneX, sceneZ;
    f32 playerX, playerY;
    f32 minX, maxZ, maxX, minZ;
    f32 focus[4] __attribute__((aligned(16)));
    s32 labelAlpha;
    u32 labelColor;

    proposed = fldAutoMapGetFloorMax();
    frSetFontDrawOT(0x5E);
    VU0_LOAD_MATRIX(&D_00384790);
    InversMatrix();
    D_00436288 = D_0043624C;
    if ((s8)dds3PadButtonState[1][0][8] < 0 && D_00436288 < proposed) {
        proposed = D_00436288 + 1;
        if (!D_00436240) proposed = fldAutoMapGetFloorNext(D_00436288);
        if (D_00436288 != proposed) {
            p2sdPlaySE(2, 127, 63);
            D_0043628C = D_00436288;
            D_00436288 = proposed;
            D_00436290 = -2500.0f;
            D_0043629C = -24;
            D_00436294 = 0.0f;
            D_00436298 = 0;
            D_004362A0 = 0;
        }
    }
    if ((s8)dds3PadButtonState[1][0][10] < 0 && D_00436288 >= 2) {
        proposed = D_00436288 - 1;
        if (!D_00436240) proposed = fldAutoMapGetFloorPrev(D_00436288);
        if (D_00436288 != proposed) {
            p2sdPlaySE(2, 127, 63);
            D_0043628C = D_00436288;
            D_00436288 = proposed;
            D_00436290 = 2500.0f;
            D_0043629C = 24;
            D_00436294 = 0.0f;
            D_00436298 = 0;
            D_004362A0 = 0;
        }
    }
    if (D_00436298 < 128) D_00436298 += 16;
    if (D_0043629C < 0) { D_0043629C += 4; D_004362A0 += 4; }
    if (D_0043629C > 0) { D_0043629C -= 4; D_004362A0 -= 4; }
    if (D_00436290 < 1000.0f && D_00436290 > -1000.0f) {
        D_00436290 = 0.0f;
    } else if (D_00436290 > 0.0f) {
        D_00436290 -= 500.0f; D_00436294 -= 500.0f;
    } else if (D_00436290 < 0.0f) {
        D_00436290 += 500.0f; D_00436294 += 500.0f;
    } else { D_00436290 = 0.0f; }
    D_0043624C = D_00436288;
    sdf3DSetCamera(&dds3ScreenParam, &D_00384790);
    rsetOtOfs(0x56);
    rsetAlpha(0);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);
    rsetOtOfs(0x57);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    rsetOtOfs(0x56);
    rsetTest(1, 1, 128, 1, 0, 0, 1, 1);
    stage = fldFindStageCoordinateRecord(fldGlobalBase.area, D_00436288);
    if (stage != NULL) {
        s32 run;
        f32 step = (f32)stage->cellSize;
        u32 color;
        eglSetOt(0x56);
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        color = D_00436240 ? 0x80000080 : 0x80000000;
        for (j = 0, sceneZ = stage->originZ; j < stage->rows;
             j++, sceneZ -= step) {
            if (sceneZ > (f32)D_00436254 + 8400.0f) continue;
            if (sceneZ < (f32)D_00436254 - 8400.0f) break;
            run = 0;
            for (i = 0, sceneX = stage->originX; i < stage->cols;
                 i++, sceneX += step) {
                if (sceneX < (f32)D_00436250 - 11400.0f) continue;
                if (sceneX > (f32)D_00436250 + 11400.0f) break;
                if (fldIsStageExplorationCellMarked(D_00436288, sceneX, sceneZ)) {
                    if (run == 0) { left = sceneX - 50.0f; top = sceneZ + 50.0f; }
                    bottom = sceneZ - step - 50.0f;
                    right = sceneX + step + 50.0f;
                    run++;
                } else {
                    if (run > 0) eglLinkPoly4(left, D_00436290 - 10.0f, top,
                    right, D_00436290 - 10.0f, top,
                    left, D_00436290 - 10.0f, bottom,
                    right, D_00436290 - 10.0f, bottom, color, color, 0x80000000, 0x80000000);
                    run = 0;
                }
            }
            if (run > 0) eglLinkPoly4(left, D_00436290 - 10.0f, top,
                    right, D_00436290 - 10.0f, top,
                    left, D_00436290 - 10.0f, bottom,
                    right, D_00436290 - 10.0f, bottom, color, color, 0x80000000, 0x80000000);
            run = 0;
        }
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 1, 1, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(0);
    }


    /* Scene visibility and outline passes, then the DDS2 ground tint. */
    if (D_00436240 != 0) {
        rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    }

    selectedFloor = D_00436288 - 1;
    record = fldAutoMapHeader;
    for (i = 0; i < fldAutoMapData; i++, record++) {
        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            itemVisible = 0;
            if (selectedFloor == item->floor) {
                if (fldGetFloorFlag(fldAutoMapField, i, j) != 0) {
                    itemVisible = 1;
                }
                if (D_00436240 != 0) {
                    itemVisible = 1;
                }
            }
            fldAutoMapNodeSet(D_003A5470[i], item->nodeIndex, itemVisible);
        }
    }
    for (i = 0; i < fldAutoMapData; i++) {
        D_003A5470[i]->color = 0x80808080;
        fldAutoMapDrawMdl1(D_003A5470[i], 0.0f, D_00436290, 0.0f);
    }

    rsetOtOfs(0x56);
    rsetAlpha(0);
    fldSubmitPrimaryFramePacket();
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);

    record = fldAutoMapHeader;
    selectedFloor = D_00436288 - 1;
    for (i = 0; i < fldAutoMapData; i++, record++) {
        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            itemVisible = 0;
            if (selectedFloor == item->floor) {
                if (fldGetFloorFlag(fldAutoMapField, i, j) != 0) {
                    itemVisible = 1;
                }
                if (D_00436240 != 0) {
                    itemVisible = 1;
                }
            }
            fldAutoMapNodeSet(D_003A5470[i], item->nodeIndex, itemVisible);
        }
        D_003A5470[i]->color = 0x80808080;
        fldAutoMapDrawMdl1(D_003A5470[i], -50.0f, D_00436290, 50.0f);
        fldAutoMapDrawMdl1(D_003A5470[i], 50.0f, D_00436290, 50.0f);
        fldAutoMapDrawMdl1(D_003A5470[i], -50.0f, D_00436290, -50.0f);
        fldAutoMapDrawMdl1(D_003A5470[i], 50.0f, D_00436290, -50.0f);

        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            fldAutoMapNodeSet(D_003A5470[i], item->nodeIndex,
                          selectedFloor == item->floor);
        }
        D_003A5470[i]->color = 0x10101010;
        fldAutoMapDrawMdl1(D_003A5470[i], 0.0f, D_00436290, 0.0f);
        D_003A5470[i]->color = 0x80808080;
    }

    rsetOtOfs(0x5A);
    fldSubmitAlternateFramePacket();
    if (stage != NULL) {
        f32 step;
        s32 run;

        eglSetOt(0x5A);
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 1, 1, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(0);
        step = stage->cellSize;
        sceneZ = stage->originZ;
        for (j = 0; j < stage->rows; j++, sceneZ -= step) {
            if ((f32)D_00436254 + 8400.0f < sceneZ) {
                continue;
            }
            if (sceneZ < (f32)D_00436254 - 8400.0f) {
                break;
            }
            run = 0;
            sceneX = stage->originX;
            for (i = 0; i < stage->cols; i++, sceneX += step) {
                if (sceneX < (f32)D_00436250 - 11400.0f) {
                    continue;
                }
                if ((f32)D_00436250 + 11400.0f < sceneX) {
                    break;
                }
                if (fldIsStageExplorationCellMarked(D_00436288, sceneX, sceneZ) != 0) {
                    if (run == 0) {
                        left = sceneX - 50.0f;
                        top = sceneZ + 50.0f;
                    }
                    bottom = sceneZ - step - 50.0f;
                    right = sceneX + step + 50.0f;
                    run++;
                } else {
                    if (run > 0) {
                        eglLinkPoly4(
                            left, D_00436290 - 10.0f, top,
                            right, D_00436290 - 10.0f, top,
                            left, D_00436290 - 10.0f, bottom,
                            right, D_00436290 - 10.0f, bottom,
                            0x8059656E, 0x8059656E, 0x8059656E, 0x8059656E);
                    }
                    run = 0;
                }
            }
            if (run > 0) {
                eglLinkPoly4(
                    left, D_00436290 - 10.0f, top,
                    right, D_00436290 - 10.0f, top,
                    left, D_00436290 - 10.0f, bottom,
                    right, D_00436290 - 10.0f, bottom,
                    0x8059656E, 0x8059656E, 0x8059656E, 0x8059656E);
            }
            run = 0;
        }
        evtSubmitPrimaryAlphaBlendMode(0);
    }




    rsetAlpha(0);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    D_004362A8 += 0.4f;
    D_004362AC += 0.2f;
    PCP_COPY_VECTOR_F32(focus, D_0037FA20);

    if (D_00436298 >= 128) {
        selectedFloor = D_00436288 - 1;
        record = fldAutoMapHeader;
        for (i = 0; i < fldAutoMapData; i++, record++) {
            sceneX = 0.0f;
            sceneZ = 0.0f;
            item = record->items;
            for (j = 0; j < record->count; j++, item++) {
                itemVisible = 0;
                if (selectedFloor == item->floor) {
                    if (fldGetFloorFlag(fldAutoMapField, i, j) != 0) {
                    itemVisible = 1;
                }
                    if (D_00436240 != 0) {
                        itemVisible = 1;
                    }
                }
                if (itemVisible != 0) {
                    FldIcon *icon = item->icons;

                    if (icon != NULL) {
                        for (k = 0; k < item->iconCount; k++, icon++) {
                            s32 spriteIndex = 0;
                            s32 iconVisible;
                            f32 offsetX = 0.0f;
                            f32 offsetZ = 0.0f;
                            f32 projectedX;
                            f32 projectedY;

                            iconVisible = 0;
                            switch (icon->kind) {
                            case 1:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 8;
                                    iconVisible = 1;
                                }
                                break;
                            case 2:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 9;
                                    iconVisible = 1;
                                }
                                break;
                            case 3:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 10;
                                    iconVisible = 1;
                                }
                                break;
                            case 4:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 5;
                                    iconVisible = 1;
                                }
                                break;
                            case 5:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 2;
                                    iconVisible = 1;
                                }
                                break;
                            case 6:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 4;
                                    iconVisible = 1;
                                }
                                break;
                            case 7:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 6;
                                    iconVisible = 1;
                                }
                                break;
                            case 8:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 7;
                                    iconVisible = 1;
                                }
                                break;
                            case 9:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 3;
                                    iconVisible = 1;
                                }
                                break;
                            case 10:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 12;
                                    iconVisible = 1;
                                }
                                break;
                            case 11:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 11;
                                    iconVisible = 1;
                                }
                                break;
                            case 12:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 13;
                                    iconVisible = 1;
                                }
                                break;
                            case 13:
                                if (D_00436240 != 0 ||
                                    fldIsStageExplorationCellMarked(D_00436288,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 14;
                                    iconVisible = 1;
                                }
                                break;
                            default:
                                spriteIndex = 11;
                                break;
                            }
                            if (iconVisible != 0) {
                                f32 x;
                                f32 z;

                                x = icon->position->x + sceneX + offsetX;
                                offsetX = focus[0] - x;
                                if (offsetX < 0.0f) {
                                    offsetX = -offsetX;
                                }
                                z = icon->position->z + sceneZ + offsetZ;
                                offsetZ = focus[2] - z;
                                if (offsetZ < 0.0f) {
                                    offsetZ = -offsetZ;
                                }
                                if (offsetX < 80000.0f && offsetZ < 80000.0f) {
                                    fldPoint3To2H(&projectedX, &projectedY,
                                                            x, D_00436290, z);
                                    fldDrawProjectedMapIcon(projectedX, projectedY,
                                                  spriteIndex, 0x80808080);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (D_00436288 == fldGlobalBase.flagStage) {
            f32 worldX;
            f32 worldZ;
            f32 distanceX;
            f32 distanceZ;

            fldAllIconDel(D_00436248, &sceneX, &sceneZ);
            rsetTest(1, 0, 128, 3, 0, 0, 1, 1);
            rsetAlpha(1);
            worldX = fldGlobalBase.x + sceneX;
            distanceX = focus[0] - worldX;
            if (distanceX < 0.0f) {
                distanceX = -distanceX;
            }
            worldZ = fldGlobalBase.z + sceneZ;
            distanceZ = focus[2] - worldZ;
            if (distanceZ < 0.0f) {
                distanceZ = -distanceZ;
            }
            fldPoint3To2H(&playerX, &playerY,
                                    worldX, D_00436290, worldZ);
            if (distanceX < 80000.0f && distanceZ < 80000.0f) {
                rsetAlpha(1);
                func_00145A08(1, playerX, playerY, -fldGlobalBase.negatedAngle);
                rsetAlpha(0);
                func_00145A08(0, playerX, playerY, fldGlobalBase.angle);
            }
        }
    }

    /* DDS2's location-flag overlay also runs during the floor fade. */
    if (D_00436288 == fldGlobalBase.flagStage && fldGlobalBase.commandEnabled != 0
        && fldGlobalBase.sceneCommand == 0) {
        s32 maskX;
        s32 maskY;
        s32 spriteX;
        s32 spriteY;

        fldAllIconDel(D_00436248, &sceneX, &sceneZ);
        fldPoint3To2H(&playerX, &playerY,
                                fldGlobalBase.x + sceneX, D_00436290,
                                fldGlobalBase.z + sceneZ);
        maskX = (s32)(playerX - 2048.0f);
        maskY = (s32)(playerY - 2048.0f);
        rsetOtOfs(0x5E);
        rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
        rsetAlpha(1);
        aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);
        aDrawBoxF(maskX + 219, maskY + 93,
                              72, 37, 0, 0, 0, 128);
        rsetTest(1, 0, 128, 1, 1, 0, 1, 1);
        rsetAlpha(0);
        aDrawBoxF(0, 0, 512, 224, 26, 17, 8, 128);
        rsetTest(1, 0, 128, 3, 0, 0, 1, 1);
        spriteX = (s32)(playerX - 2048.0f) + 219;
        spriteY = (s32)(playerY - 2048.0f) * 2 + 224;
        DrwSprite2D(spriteX, spriteY - 39, 74, 39,
                            2, 2, 74, 39, 0x80808080, D_0044F7F0[9]);
        DrwSprite2D(spriteX, spriteY, 74, 39,
                            2, 41, 74, -39, 0x80808080, D_0044F7F0[9]);
        rsetAlpha(1);
        DrwSprite2D(-27, -23, 560, 180, 1, 36, 62, 27,
                            0x80808080, D_0044F7F0[8]);
        rsetAlpha(0);
    }

    rsetOtOfs(0x5E);
    rsetAlpha(0);
    rsetAlpha(0);
    DrwSprite2D(341, 403, 22, 22, 48, 2, -22, 22,
                        0x80808080, D_0044F7F0[6]);
    DrwSprite2D(363, 403, 112, 22, 26, 2, 1, 22,
                        0x80808080, D_0044F7F0[6]);
    DrwSprite2D(475, 403, 22, 22, 26, 2, 22, 22,
                        0x80808080, D_0044F7F0[6]);
    DrwSprite2D(454, 401, 58, 44, 1, 1, 58, 44,
                        0x80808080, fldGlobalBase.fieldTextures[3].texture);
    DrwSprite2D(354, 406, 61, 16, 2, 2, 61, 16,
                        0x80808080, D_0044F7F0[5]);
    DrwSprite2D(424, 406, 61, 16, 64, 2, 61, 16,
                        0x80808080, D_0044F7F0[5]);

    top = sdfSin(D_004362A4);
    i = (s32)(top * 32.0f) + 96;
    D_004362A4 += 0.2f;
    i = i + (i << 8) + (i << 16)
                 + 0x80000000U;
    fldAutoMapGetBBox(&minX, &maxZ, &maxX, &minZ);
    minX = (s32)((minX - 600.0f) / 600.0f) * 600;
    maxX = (s32)((maxX + 600.0f) / 600.0f) * 600;
    maxZ = (s32)((maxZ - 600.0f) / 600.0f) * 600;
    minZ = (s32)((minZ + 600.0f) / 600.0f) * 600;
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    rsetAlpha(0);

    if ((f32)D_00436254 < maxZ) {
        DrwSprite2D(241, 6, 29, 41, 41, 20, 29, 41,
                            i, D_0044F7F0[5]);
    } else {
        DrwSprite2D(243, 15, 25, 25, 2, 26, 25, 25,
                            0x80808080, D_0044F7F0[6]);
    }
    if (minZ < (f32)D_00436254) {
        DrwSprite2D(241, 401, 29, 41, 41, 61, 29, -41,
                            i, D_0044F7F0[5]);
    } else {
        DrwSprite2D(243, 408, 25, 25, 2, 51, 25, -25,
                            0x80808080, D_0044F7F0[6]);
    }
    if ((f32)D_00436250 < maxX) {
        DrwSprite2D(465, 209, 41, 29, 73, 20, 41, 29,
                            i, D_0044F7F0[5]);
    } else {
        DrwSprite2D(472, 211, 25, 25, 29, 26, 25, 25,
                            0x80808080, D_0044F7F0[6]);
    }
    if (minX < (f32)D_00436250) {
        DrwSprite2D(6, 209, 41, 29, 114, 20, -41, 29,
                            i, D_0044F7F0[5]);
    } else {
        DrwSprite2D(15, 211, 25, 25, 54, 26, -25, 25,
                            0x80808080, D_0044F7F0[6]);
    }
    DrwSprite2D(247, 38, 18, 18, 2, 20, 18, 18,
                        0x80808080, D_0044F7F0[5]);
    DrwSprite2D(247, 393, 18, 18, 21, 20, 18, 18,
                        0x80808080, D_0044F7F0[5]);
    DrwSprite2D(457, 215, 18, 18, 2, 39, 18, 18,
                        0x80808080, D_0044F7F0[5]);
    DrwSprite2D(38, 215, 18, 18, 21, 39, 18, 18,
                        0x80808080, D_0044F7F0[5]);
    DrwSprite2D(280, 16, 18, 25, 38, 3, 18, 25,
                        0x80808080, fldGlobalBase.fieldTextures[0].texture);
    DrwSprite2D(298, 16, 182, 25, 54, 3, 1, 25,
                        0x80808080, fldGlobalBase.fieldTextures[0].texture);
    DrwSprite2D(480, -1, 32, 57, 1, 2, 32, 57,
                        0x80808080, fldGlobalBase.fieldTextures[0].texture);
    DrwSprite2D(292, 42, 22, 22, 2, 2, 22, 22,
                        0x80808080, D_0044F7F0[6]);
    DrwSprite2D(314, 42, 171, 22, 23, 2, 1, 22,
                        0x80808080, D_0044F7F0[6]);
    DrwSprite2D(485, 42, 22, 22, 26, 2, 22, 22,
                        0x80808080, D_0044F7F0[6]);

    {
        const u8 *title = (const u8 *)D_0039A1D0[fldGlobalBase.area % 100].s;

        D_00438ED4 = itfCreateConvertedTextGlyph(
            (450 - D_00438ED8 * 6) << 4, 128, 0, 0xA09DC380,
            title, 0);
        frSetPriority(D_00438ED4, 0xFFFFFF);
        frFontDrawGlyphInDefaultMode(D_00438ED4);
        frDestroyFontRequestAfter(D_00438ED4);
    }

    rsetOtOfs(0x5E);
    rsetAlpha(0);
    rsetTest(1, 5, 128, 3, 0, 0, 1, 2);

    if (D_0043629C != 0 && (u32)D_0043628C - 1U < 40U
        && D_0044FCF0[D_0043628C] >= 2) {
        s32 labelIndex;

        labelAlpha = D_004362A0;
        if (labelAlpha < 0) {
            labelAlpha = -labelAlpha;
        }
        labelAlpha = 128 - (labelAlpha << 3);
        if (labelAlpha < 0) {
            labelAlpha = 0;
        }
        labelColor = labelAlpha | 0x78759B00;
        labelIndex = fldFindMapCoordinateIndex(fldGlobalBase.area % 100, D_0043628C);
        D_00438ED4 = itfCreateConvertedTextGlyph(
            (450 - (D_0044FCF0[D_0043628C] / 2) * 12) << 4,
            (D_004362A0 + 44) << 3, 0, labelColor,
            (const u8 *)D_003A25AC[labelIndex].s, 0);
        frSetPriority(D_00438ED4, 0xFFFFFF);
        frFontDrawGlyphWithSharedFlags(D_00438ED4, 1);
        frDestroyFontRequestAfter(D_00438ED4);
    }

    if ((u32)D_00436288 - 1U < 40U && D_0044FCF0[D_00436288] >= 2) {
        s32 labelIndex;

        labelAlpha = D_0043629C;
        if (labelAlpha < 0) {
            labelAlpha = -labelAlpha;
        }
        labelAlpha = 128 - (labelAlpha << 3);
        if (labelAlpha < 0) {
            labelAlpha = 0;
        }
        labelColor = labelAlpha | 0x78759B00;
        labelIndex = fldFindMapCoordinateIndex(fldGlobalBase.area % 100, D_00436288);
        D_00438ED4 = itfCreateConvertedTextGlyph(
            (450 - (D_0044FCF0[D_00436288] / 2) * 12) << 4,
            (D_0043629C << 3) + 348, 0, labelColor,
            (const u8 *)D_003A25AC[labelIndex].s, 0);
        frSetPriority(D_00438ED4, 0xFFFFFF);
        frFontDrawGlyphWithSharedFlags(D_00438ED4, 1);
        frDestroyFontRequestAfter(D_00438ED4);
    }

    VU0_LOAD_MATRIX(&sdfViewMatrix);
}

extern s32 sdfGrAllocPacketBuff(s32);
extern void sdfDrawTagInit(sdfDrawTag_t *);
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *, sdfTexHandle_t *, s32);
extern void SetRotateMatrixY(f32);
extern SdfPoolNode *fldDrawGetOt(void);
extern sdfTexHandle_t *D_0044F7F0[];

extern void sdfPrimLinkFt4(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 depth, s32 (*alloc)(s32));

/* Rotate the map quad and project its X/Z plane into screen coordinates. */
void func_00148188(f32 centerX, f32 centerY, f32 angle) {
    f32 corners[4][4];
    sdfDrawTag_t *packet;
    s32 u0 = 0;
    s32 v0 = 0;
    s32 u3 = 0x800;
    s32 v3 = 0x800;
    s32 u1 = 0x800;
    s32 v1 = 0;
    s32 u2 = 0;
    s32 v2 = 0x800;

    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    sdf2DLinkTexPacket(packet, D_0044F7F0[1], SDF_GS_CONTEXT_1);

    corners[0][0] = -4720.0f;
    corners[0][1] = 0.0f;
    corners[0][2] = -4080.0f;
    corners[3][0] = 4720.0f;
    corners[3][1] = 0.0f;
    corners[3][2] = 4080.0f;
    corners[1][0] = 4720.0f;
    corners[1][1] = 0.0f;
    corners[1][2] = -4080.0f;
    corners[2][0] = -4720.0f;
    corners[2][1] = 0.0f;
    corners[2][2] = 4080.0f;

    SetRotateMatrixY((angle + 180.0f) * 3.14f / 180.0f);
    SDF_VU0_LOADVECTOR(corners[0]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(corners[0]);
    SDF_VU0_LOADVECTOR(corners[1]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(corners[1]);
    SDF_VU0_LOADVECTOR(corners[2]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(corners[2]);
    SDF_VU0_LOADVECTOR(corners[3]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(corners[3]);

    corners[0][2] *= 0.5f;
    corners[0][0] += centerX * 16.0f;
    corners[0][2] += centerY * 8.0f;

    corners[1][2] *= 0.5f;
    corners[1][0] += centerX * 16.0f;
    corners[1][2] += centerY * 8.0f;

    corners[2][2] *= 0.5f;
    corners[2][0] += centerX * 16.0f;
    corners[2][2] += centerY * 8.0f;

    corners[3][2] *= 0.5f;
    corners[3][0] += centerX * 16.0f;
    corners[3][2] += centerY * 8.0f;

    sdfPrimLinkFt4(
        packet, 0x80808080, 0x44,
        (s32)(corners[0][0] + 28672.0f), (s32)(corners[0][2] + 30976.0f), u0, v0,
        (s32)(corners[1][0] + 28672.0f), (s32)(corners[1][2] + 30976.0f), u1, v1,
        (s32)(corners[2][0] + 28672.0f), (s32)(corners[2][2] + 30976.0f), u2, v2,
        (s32)(corners[3][0] + 28672.0f), (s32)(corners[3][2] + 30976.0f), u3, v3,
        0, 0);
    fldDrawGetOt()->append(fldDrawGetOt(), packet);
}

extern f32 D_004362B0;
extern f32 D_004362B4;
extern Matrix4 D_00384790;
extern Matrix4 sdfViewMatrix;
extern sdfTexHandle_t *D_0044F7F8[8];
extern void InversMatrix(void);
extern void rsetOtOfs(u32 buffer);
extern void rsetTest(s32 flag0, s32 flag1, s32 field4, s32 field12, s32 flag14, s32 flag15, s32 unused, s32 field17);
extern void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture);
extern void fldSubmitGsGradientQuad(s32 x, s32 y, s32 w, s32 h,
                                    u32 r0, u32 g0, u32 b0, u32 a0,
                                    u32 r1, u32 g1, u32 b1, u32 a1,
                                    u32 r2, u32 g2, u32 b2, u32 a2,
                                    u32 r3, u32 g3, u32 b3, u32 a3);
extern void fldSubmitPrimaryFramePacket(void);
extern void fldSubmitAlternateFramePacket(void);
extern void rsetAlpha(s32 mode);

void fldDrawScrollingLayerOverlay(void) {
    s32 textureRow;
    s32 panelOffset;

    D_004362B0 += 0.04f;
    VU0_LOAD_MATRIX(&D_00384790);
    InversMatrix();
    sdf3DSetCamera(&dds3ScreenParam, &D_00384790);

    rsetOtOfs(0x56);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0x40, 0, 0, 0x80);
    DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x80, 0x80, 0x80808080, D_0044F7F8[0]);

    D_004362B4 -= 0.5f;
    if (D_004362B4 < 0.0f) {
        D_004362B4 += 360.0f;
    }
    func_00148188(256.0f, 224.0f, D_004362B4);
    panelOffset = (s32)(sdfSin(D_004362B0) * 50.0f - 50.0f);

    fldSubmitPrimaryFramePacket();
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 0x80);
    rsetTest(1, 5, 0x10, 0, 0, 0, 1, 1);
    rsetAlpha(0);

    for (textureRow = 0; textureRow < 7; textureRow++) {
        s32 y = textureRow * 0x40 + 7;
        DrwSprite2D(0, y, 0x100, 0x40, 0, 0, 0x100, 0x40, 0x80808080, D_0044F7F0[3]);
        DrwSprite2D(0x100, y, 0x100, 0x40, 0, 0, 0x100, 0x40, 0x80808080, D_0044F7F0[3]);
    }
    fldSubmitAlternateFramePacket();
    rsetTest(1, 0, 0x80, 1, 1, 0, 1, 1);
    rsetAlpha(0);

    fldSubmitGsGradientQuad(panelOffset, 0, 0x64, 0x1C0,
                            0, 0, 0, 0xC,
                            0, 0, 0, 0,
                            0, 0, 0, 0xC,
                            0, 0, 0, 0);
    fldSubmitGsGradientQuad(0x19C - panelOffset, 0, 0x64, 0x1C0,
                            0, 0, 0, 0,
                            0, 0, 0, 0xC,
                            0, 0, 0, 0,
                            0, 0, 0, 0xC);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    fldSubmitPrimaryFramePacket();
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 0x80);
    rsetTest(1, 5, 0x10, 0, 0, 0, 1, 1);
    rsetAlpha(0);

    for (textureRow = 0; textureRow < 7; textureRow++) {
        s32 y = textureRow * 0x40;

        DrwSprite2D(0, y, 0x100, 0x40, 0, 0, 0x100, 0x40, 0x80808080, D_0044F7F0[3]);
        DrwSprite2D(0x100, y, 0x100, 0x40, 0, 0, 0x100, 0x40, 0x80808080, D_0044F7F0[3]);
    }
    fldSubmitAlternateFramePacket();
    rsetTest(1, 0, 0x80, 1, 1, 0, 1, 1);
    rsetAlpha(1);

    fldSubmitGsGradientQuad(panelOffset, 0, 0x96, 0x1C0,
                            0x80, 0x80, 0x80, 7,
                            0x80, 0x80, 0x80, 0,
                            0x80, 0x80, 0x80, 7,
                            0x80, 0x80, 0x80, 0);
    fldSubmitGsGradientQuad(0x16A - panelOffset, 0, 0x96, 0x1C0,
                            0x80, 0x80, 0x80, 0,
                            0x80, 0x80, 0x80, 7,
                            0x80, 0x80, 0x80, 0,
                            0x80, 0x80, 0x80, 7);

    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    rsetAlpha(0);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    VU0_LOAD_MATRIX(&sdfViewMatrix);
    rsetOtOfs(0x5E);
    rsetTest(1, 5, 0x81, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148A98);

extern u8 fldAutoMapSeqTestFlag(u32);
extern void fldAutoMapSeqSetFlag(u32);
extern void evtDisableMoonDraw(void);
extern KwlnDrawVectorParams kwlnDrawVector;
extern SdfMathVector4 D_00413788[];
extern s32 D_00436268;
extern s32 D_00436264;
extern s32 D_00436260;
extern s32 D_0043625C;
extern s32 D_00436258;
extern u128 kwlnOverlayEyeVector;
extern u128 kwlnOverlayUpVector;
extern KwlnDrawVectorParams kwlnOverlayFrustumParameters;

/* Enter the field scene once: hide the world object, load the scene's map
 * textures, centre the camera on the floor's entry point and set the
 * scene fog and camera offsets. DDS2 loads four map textures. */
void func_00149A00(void) {
    FldAreaWork *cam;
    f32 focus[4];
    f32 eye[4];
    SdfMathVector4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 cx;
    s32 cz;

    if (fldAutoMapSeqTestFlag(1) == 1) {
        return;
    }
    dds3VisibleSceneBasic((dds3Basic_t *)(u32)dds3GetActiveScene(), 0);
    p2sdPlaySE(0xE, 0x7F, 0x3F);
    cam = &fldGlobalBase;
    D_0044F7F0[1] = sdfTexSet(cam->mapResources[0].resourceData);
    D_0044F7F0[2] = sdfTexSet(cam->mapResources[1].resourceData);
    D_0044F7F0[3] = sdfTexSet(cam->mapResources[2].resourceData);
    D_0044F7F0[6] = sdfTexSet(cam->mapResources[4].resourceData);
    D_00436268 = 0;
    D_00436248 = cam->floor;
    AutoMapSeq = 1;
    D_0043624C = cam->flagStage;
    D_00436264 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    D_00436260 = 0;
    ix = cx + entry[0];
    iz = cz + entry[1];
    D_00436258 = ix;
    D_0043625C = iz;
    D_00436254 = iz;
    D_00436250 = ix;
    fldAutoMapSeqSetFlag(1);
    fldResetPlayerSceneObjectState();
    evtDisableMoonDraw();
    memcpy(&up, &D_00413788[0], sizeof(up));
    focus[1] = 0.0f;
    focus[0] = D_00436250;
    focus[2] = D_00436254;
    eye[0] = D_00436250;
    eye[1] = -24000.0f;
    eye[2] = D_00436254;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_0037FA20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, &up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x108010;
    D_0037F980.camera.offsetX = 2048.0f;
    D_0037F980.camera.offsetY = 2048.0f;
    kwlnDrawVector.fogNear = 255.0f;
    kwlnDrawVector.depthNear = 1000.0f;
    kwlnDrawVector.fogFar = 255.0f;
    kwlnDrawVector.depthFar = 20000.0f;
    kwlnDrawVector.fogColor = 0x108010;
}

void fldReleaseResourceSlots(void) {
    if (D_0044F7F0[1] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[1]);
    }
    if (D_0044F7F0[2] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[2]);
    }
    if (D_0044F7F0[3] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[3]);
    }
    if (D_0044F7F0[6] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[6]);
    }
    D_0044F7F0[1] = 0;
    D_0044F7F0[2] = 0;
    D_0044F7F0[3] = 0;
    D_0044F7F0[6] = 0;
    while (sdfDelayedResrcCheck() != 0) {
        sdfDrawSync();
    }
    AutoMapSeq = 0;
    frSetFontDrawOT(0x54);
}

extern void sdfModelRootFree(sdfModelRootNode_t *slot, s32 recycle, s32 release);

void fldReleaseSceneDevSlotsAndTextures(void) {
    s32 i;

    for (i = 0; i < fldAutoMapData; i++) {
        if (D_003A5470[i] != 0) {
            sdfModelRootFree(D_003A5470[i], 1, 1);
        }
    }
    for (i = 0; i < 96; i++) {
        D_003A5470[i] = 0;
        D_0044F818[i].x = 0.0f;
        D_0044F818[i].y = 0.0f;
        D_0044F818[i].z = 0.0f;
    }
    if (D_0044F7F0[5] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[5]);
    }
    if (D_0044F7F0[7] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[7]);
    }
    if (D_0044F7F0[8] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[8]);
    }
    if (D_0044F7F0[9] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[9]);
    }
    D_0044F7F0[5] = 0;
    D_0044F7F0[7] = 0;
    D_0044F7F0[8] = 0;
    D_0044F7F0[9] = 0;
    while (sdfDelayedResrcCheck() != 0) {
        sdfDrawSync();
    }
    AutoMapSeq = 0;
}

extern void fldAllIconDel(s32 index, f32 *x, f32 *z);

extern KwlnDrawVectorParams kwlnOverlayFrustumParameters;



extern u128 kwlnOverlayEyeVector;

extern u128 kwlnOverlayUpVector;

extern s32 D_0043624C;

extern s32 D_00436250;

extern s32 D_00436254;

extern s32 D_00436258;

extern s32 D_0043625C;

extern s32 D_00436264;

extern void fldAutoMapSeqClearFlag(s32);


extern SdfMathVector4 D_00413788[]; /* default camera up vectors (3 copies), the first read by func_00149A00 */

extern void fldApplySkyLightSetToPlayerVU(void);


extern s32 D_00436260;

extern s32 D_00436268;

/* Enters the field camera state for a fresh scene: releases the resource slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldAreaWork *cam = &fldGlobalBase;
    f32 focus[4];
    f32 eye[4];
    SdfMathVector4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;

    sdfDrawSync();
    sdfDrawSync();
    fldReleaseResourceSlots();
    AutoMapSeq = 0;
    fldAutoMapSeqClearFlag(1);
    fldPreparePlayerSceneCameraTarget();
    evtSetSolarOverlayFullyVisible();
    fldApplySkyLightSetToPlayerVU();
    cam->sceneMode = 4;
    frSetFontDrawOT(0x54);
    dds3VisibleSceneBasic((dds3Basic_t *)(u32)dds3GetActiveScene(), 1);
    D_00436268 = 0;
    D_00436248 = cam->floor;
    D_0043624C = cam->flagStage;
    D_00436264 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    memcpy(&up, &D_00413788[1], sizeof(up));
    D_00436260 = 0;
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_0037FA20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, &up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x108010;
    D_0037F980.camera.offsetX = 2244.0f;
    D_0037F980.camera.offsetY = 2118.0f;
}

/* Loads the scene's models into the resource slots, then centers the camera on the
 * scene's entry point. */
void fldLoadSceneModelsAndCamera(void) {
    FldAreaWork *cam;
    FldSceneRecord *rec;
    f32 focus[4];
    f32 eye[4];
    SdfMathVector4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 i;

    rec = fldAutoMapHeader;
    for (i = 0; i < fldAutoMapData; i++, rec++) {
        D_003A5470[i] = sdfModelMakeEarly(0, rec->model);
        D_0044F818[i].x = rec->pos->x;
        D_0044F818[i].y = rec->pos->y;
        D_0044F818[i].z = rec->pos->z;
    }
    cam = &fldGlobalBase;
    D_0044F7F0[5] = sdfTexSet(cam->mapResources[3].resourceData);
    D_0044F7F0[7] = sdfTexSet(cam->mapResources[5].resourceData);
    D_0044F7F0[8] = sdfTexSet(cam->mapResources[6].resourceData);
    D_0044F7F0[9] = sdfTexSet(cam->mapResources[7].resourceData);
    D_00436268 = 0;
    D_00436248 = cam->floor;
    D_0043624C = cam->flagStage;
    D_00436264 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    memcpy(&up, &D_00413788[2], sizeof(up));
    D_00436260 = 0;
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_0037FA20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, &up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x108010;
    D_0037F980.camera.offsetX = 2241.0f;
    D_0037F980.camera.offsetY = 2113.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfDrawSync();
    sdfDrawSync();
    fldReleaseSceneDevSlotsAndTextures();
}

u32 fldGetSceneReadyFlag(void) {
    return AutoMapSeq;
}

/* Handle automap input, clamp the grid position, and refresh its camera. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413788);

void fldUpdateAutomapInput(void) {
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 minX, maxZ, maxX, minZ;
    s32 closeRequested = 0;

    if (fldGetSceneReadyFlag() == 0) {
        return;
    }
    if (D_003899E0[0] != 0) {
        if ((s8)dds3PadButtonState[1][0][2] != 0) {
            closeRequested = 1;
        }
    } else if ((s8)dds3PadButtonState[1][0][2] < 0) {
        closeRequested = 1;
    }

    /* A close request does not advance the input-delay counter. */
    if (closeRequested == 0) {
        D_00436260++;
    }
    if (closeRequested != 0 ||
        ((s8)dds3PadButtonState[1][0][3] < 0 && D_00436268 == 0)) {
        fldEnterSceneCamera();
        dds3FadeOut2(0);
        dds3FadeIn2(8);
        return;
    }
    if ((s8)dds3PadButtonState[1][0][3] < 0) {
        p2sdPlaySE(10, 127, 63);
        D_00436250 = D_00436258;
        D_00436254 = D_0043625C;
        D_0043624C = D_00436264;
        D_00436268 = 0;
    }

    if (D_00436260 >= 11) {
        if ((s8)D_00435C48[0][1] != 0) {
            if (D_00435C48[0][1] & 2) {
                if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                    (s8)dds3PadButtonState[1][0][7] != 0) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 -= 600;
                } else if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                           (s8)dds3PadButtonState[1][0][6] != 0) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 += 600;
                } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                           (s8)dds3PadButtonState[1][0][7] != 0) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 -= 600;
                } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                           (s8)dds3PadButtonState[1][0][6] != 0) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 += 600;
                } else if ((s8)dds3PadButtonState[1][0][5] != 0) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                } else if ((s8)dds3PadButtonState[1][0][4] != 0) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                } else if ((s8)dds3PadButtonState[1][0][7] != 0) {
                    D_00436268 = 1;
                    D_00436254 -= 600;
                } else if ((s8)dds3PadButtonState[1][0][6] != 0) {
                    D_00436268 = 1;
                    D_00436254 += 600;
                }
            }
        } else {
            /* A held diagonal consumes the direction choice even when its
             * Z-repeat bit is absent; do not fall back to a cardinal step. */
            if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                (s8)dds3PadButtonState[1][0][7] != 0) {
                if (dds3PadButtonState[1][0][7] & 2) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 -= 600;
                }
            } else if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                       (s8)dds3PadButtonState[1][0][6] != 0) {
                if (dds3PadButtonState[1][0][6] & 2) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 += 600;
                }
            } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                       (s8)dds3PadButtonState[1][0][7] != 0) {
                if (dds3PadButtonState[1][0][7] & 2) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 -= 600;
                }
            } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                       (s8)dds3PadButtonState[1][0][6] != 0) {
                if (dds3PadButtonState[1][0][6] & 2) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 += 600;
                }
            } else if (dds3PadButtonState[1][0][5] & 2) {
                D_00436268 = 1;
                D_00436250 += 600;
            } else if (dds3PadButtonState[1][0][4] & 2) {
                D_00436268 = 1;
                D_00436250 -= 600;
            } else if (dds3PadButtonState[1][0][7] & 2) {
                D_00436268 = 1;
                D_00436254 -= 600;
            } else if (dds3PadButtonState[1][0][6] & 2) {
                D_00436268 = 1;
                D_00436254 += 600;
            }
        }
    }

    fldAutoMapGetBBox(&minX, &maxZ, &maxX, &minZ);
    /* Quantize the two X endpoints, then the two Z endpoints. */
    minX = (s32)((minX - 600.0f) / 600.0f) * 600;
    maxX = (s32)((maxX + 600.0f) / 600.0f) * 600;
    maxZ = (s32)((maxZ - 600.0f) / 600.0f) * 600;
    minZ = (s32)((minZ + 600.0f) / 600.0f) * 600;

    if ((f32)D_00436250 < minX) {
        D_00436250 = (s32)minX;
    }
    if (maxX < (f32)D_00436250) {
        D_00436250 = (s32)maxX;
    }
    if ((f32)D_00436254 < minZ) {
        D_00436254 = (s32)minZ;
    }
    if (maxZ < (f32)D_00436254) {
        D_00436254 = (s32)maxZ;
    }

    focus[0] = D_00436250;
    focus[1] = 0.0f;
    focus[2] = D_00436254;
    eye[0] = D_00436250;
    eye[1] = -24000.0f;
    eye[2] = D_00436254;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_0037FA20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x808080;
    SDF_VU0_COPYVECTOR(sdfViewTargetVector, eye);
    SDF_VU0_COPYVECTOR(aViewTarget, focus);
    SDF_VU0_COPYVECTOR(aViewDown, up);
}

/* Re-centers the scene camera on the current scene's entry point. */
void fldCenterCameraOnEntry(void) {
    FldAreaWork *cam = &fldGlobalBase;
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 entry[2];
    s32 ix;
    s32 iz;

    D_00436248 = cam->floor;
    D_0043624C = cam->flagStage;
    D_00436264 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_0037FA20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x808080;
}

/* Stores the area, room and third location selection, and selects the area's
 * flag table. The third selection's interpretation is left unknown. */
void fldSetSceneLocation(s32 area, s32 floor, s32 stage) {
    fldGlobalBase.area = area;
    fldGlobalBase.unk18 = stage;
    D_00436248 = fldGlobalBase.floor = floor;
    fldAutoMapField = area % 100;
}

extern s32 D_00387CE0[];

/* The area number's last two digits map to a save-table row; floor and bit are zero-based. */
s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];

    if (areaIndex == -1) {
        return 0;
    }
    return (DDS3_GBWK->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetCurrentFloorFlag(s32 flagNumber) {
    s32 areaIndex;
    s32 bit;

    if (flagNumber > 0) {
        areaIndex = D_00387CE0[fldAutoMapField % 100];
        if (areaIndex != -1) {
            bit = flagNumber - 1;
            fldGlobalBase.unk18 = bit;
            fldGlobalBase.flagNumber = flagNumber;
            DDS3_GBWK->areaFlags[areaIndex][fldGlobalBase.floor] |= 1ULL << bit;
            fldGlobalBase.flagStage = fldAutoMap_GetFloor(fldGlobalBase.floor, bit);
        }
    }
}

/* The public setters take one-based floor and flag numbers. */
void fldSetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_00387CE0[area % 100];
    if (areaIndex != -1) {
        DDS3_GBWK->areaFlags[areaIndex][floor] |= 1ULL << bit;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_00387CE0[area % 100];
    if (areaIndex != -1) {
        DDS3_GBWK->areaFlags[areaIndex][floor] &= ~(1ULL << bit);
    }
}

void fldAutoMapNameU(s32 labelIndex) {
    if (labelIndex >= 0x40) {
        D_00389834[0] = -1;
    } else {
        D_00389834[0] = labelIndex;
    }
}

void fldAutoMapNameD(s32 labelIndex) {
    if (labelIndex >= 0x40) {
        D_00389838[0] = -1;
    } else {
        D_00389838[0] = labelIndex;
    }
}

s32 fldAutoMapGetFloorPrev(s32 limit) {
    FldSceneRecord *group = fldAutoMapHeader;
    s32 best = -1;
    s32 groupIndex = 0;

    for (groupIndex = 0; groupIndex < (s32)fldAutoMapData; groupIndex++, group++) {
        u32 entryCount = group->count;
        u32 entryIndex;

        for (entryIndex = 0; entryIndex < entryCount;) {
            s32 marked = fldGetFloorFlag(fldAutoMapField, groupIndex, entryIndex);
            if (marked) {
                s32 value = fldAutoMap_GetFloor(groupIndex, entryIndex);
                if (value < limit && value > best) {
                    best = value;
                }
            }
            entryIndex++;
            entryCount = group->count;
        }
    }
    if (best == -1) {
        return limit;
    }
    return best;
}

s32 fldAutoMapGetFloorNext(s32 limit) {
    FldSceneRecord *group = fldAutoMapHeader;
    s32 best = 999;
    s32 groupIndex = 0;

    for (groupIndex = 0; groupIndex < (s32)fldAutoMapData; groupIndex++, group++) {
        u32 entryCount = group->count;
        u32 entryIndex;

        for (entryIndex = 0; entryIndex < entryCount;) {
            s32 marked = fldGetFloorFlag(fldAutoMapField, groupIndex, entryIndex);
            if (marked) {
                s32 value = fldAutoMap_GetFloor(groupIndex, entryIndex);
                if (value > limit && value < best) {
                    best = value;
                }
            }
            entryIndex++;
            entryCount = group->count;
        }
    }
    if (best == 999) {
        return limit;
    }
    return best;
}

void fldAllIconDel(s32 index, f32 *x, f32 *z) {
    s32 i;
    FldSceneRecord *entry = fldAutoMapHeader;
    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        if (i == index) {
            FldPoint *position = entry->pos;
            *x = position->x;
            *z = position->z;
            return;
        }
    }
    *x = 0.0f;
    *z = 0.0f;
}

extern s32 D_00436240;

void fldAutoMapGetBBox(f32 *minX, f32 *maxZ, f32 *maxX, f32 *minZ) {
    s32 room;
    u32 i;
    FldSceneRecord *rec;
    FldItem *item;
    FldPoint *a;
    s32 visible;
    FldPoint *b;

    room = 0;
    *minX = 0.0f;
    *maxZ = 0.0f;
    *maxX = 0.0f;
    *minZ = 0.0f;
    rec = fldAutoMapHeader;
    for (; room < fldAutoMapData; room++, rec++) {
        item = rec->items;
        for (i = 0; i < rec->count; i++, item++) {
            visible = 0;
            if (fldGetFloorFlag(fldAutoMapField, room, i) != 0) {
                visible = 1;
            }
            if (D_00436240 != 0) {
                visible = 1;
            }
            if (visible != 0) {
                a = item->pointA;
                if (a != NULL) {
                    if (*minX > a->x) {
                        *minX = a->x;
                    }
                    b = item->pointB;
                    if (*minX > b->x) {
                        *minX = b->x;
                    }
                    if (*maxX < a->x) {
                        *maxX = a->x;
                    }
                    if (*maxX < b->x) {
                        *maxX = b->x;
                    }
                    if (*maxZ < a->z) {
                        *maxZ = a->z;
                    }
                    if (*maxZ < b->z) {
                        *maxZ = b->z;
                    }
                    if (a->z < *minZ) {
                        *minZ = a->z;
                    }
                    if (b->z < *minZ) {
                        *minZ = b->z;
                    }
                }
            }
        }
    }
}

void fldCheckSceneReady(void) {
    if (D_00389780[0] >= 200) {
        return;
    }
    if (AutoMapSeq == 1) {
        fldUpdateAutomapInput();
    }
}

void fldDrawAutomapWhenReady(void) {
    if (AutoMapSeq == 1) {
        fldDrawScrollingLayerOverlay();
        fldDrawAutomapScene();
    }
}

void fldResetCameraAndSceneView(void) {
    fldCenterCameraOnEntry();
    func_00148A98();
}

void fldClearAllAreaFloorFlags(void) {
    u64 *words;
    s32 remaining;
    DatGameState *block;
    s32 blockIndex;

    blockIndex = 0;
    block = DDS3_GBWK;
    do {
        words = block->areaFlags[blockIndex];
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        blockIndex = blockIndex + 1;
    } while (blockIndex < 10);
}

extern f32 D_0043627C;
extern void itfSetTextDrawLimit(s32 value);
extern s32 frFontMeasureTextAdvance(const u8 *text, u8 fontIndex, u8 mode);
extern void fldKanjiDraw(s32, s32, u32, const u8 *);

/* Both banner coordinates are interpolated, including the fixed zero Y endpoint. */
static inline f32 fldBannerCoordinate(f32 progress, f32 hidden, f32 shown) {
    return progress * shown + (1.0f - progress) * hidden;
}

void func_0014B0F8(void) {
    f32 progress;
    s32 x, y;
    s32 labelIndex;
    s32 width;
    s32 labelX;
    if (fldGlobalBase.playerModelAlternate == 1) return;
    if (fldGlobalBase.area >= 200) return;
    switch (fldGlobalBase.mapBannerFadeMode) {
    case 0:
        progress = 0.0f;
        D_0043627C = progress;
        break;
    case 2:
        progress = D_0043627C;
        if (progress > 0.0f) {
            progress -= 0.1f;
            if (progress < 0.0f) {
                progress = 0.0f;
                /* Retail 0x0014B198 bc1tl .L0014B1F0: the clamp store has one entry
                 * and falls into interpolate. */
                goto storeClampedProgress;
            }
            D_0043627C = progress;
        }
        break;
    case 3:
        progress = D_0043627C;
        if (!(progress < 1.0f)) {
            break;
        }
        progress += 0.1f;
        if (!(1.0f < progress)) {
            D_0043627C = progress;
            break;
        }
        /* A completed fade-in uses the fully visible state. */
    case 1:
        progress = 1.0f;
        D_0043627C = progress;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    default:
        progress = D_0043627C;
        break;
    }
    /* Retail 0x0014B1A0 b .L0014B1F4: interpolate is shared by six
     * branches and the store block fall-through. */
    goto interpolate;
storeClampedProgress:
    D_0043627C = progress;
interpolate:
    x = (s32)fldBannerCoordinate(progress, 250.0f, 0.0f);
    y = (s32)fldBannerCoordinate(progress, 0.0f, 0.0f);
    itfSetTextDrawLimit(0x13);
    if (fldGlobalBase.secondaryMapLabelSelection != -1) {
        if (D_0044FD48[fldGlobalBase.secondaryMapLabelSelection] < 2) return;
        labelIndex = fldFindSecondaryMapLabelIndexByCoordinate(fldGlobalBase.area % 100, fldGlobalBase.secondaryMapLabelSelection);
        width = frFontMeasureTextAdvance(D_0039E1A8[labelIndex].name, 1, 0);
        fldDrawAnimatedFieldBanner(width, x, y);
        if (width >= 0x9E) labelX = 0x14A; else labelX = 0x198 - width / 2;
        fldKanjiDraw(x + labelX, (y + 0x12) / 2, 0xF0DC6480, D_0039E1A8[labelIndex].name);
    } else if (fldGlobalBase.primaryMapLabelSelection != fldGlobalBase.secondaryMapLabelSelection) {
        if (D_0044FC98[fldGlobalBase.primaryMapLabelSelection] < 2) return;
        labelIndex = fldFindMapLabelIndexByCoordinate(fldGlobalBase.area % 100, fldGlobalBase.primaryMapLabelSelection);
        width = frFontMeasureTextAdvance(D_0039A5A8[labelIndex].name, 1, 0);
        fldDrawAnimatedFieldBanner(width, x, y);
        if (width >= 0x9E) labelX = 0x14A; else labelX = 0x198 - width / 2;
        fldKanjiDraw(x + labelX, (y + 0x12) / 2, 0xA09DC380, D_0039A5A8[labelIndex].name);
    }
    itfSetTextDrawLimit(-1);
}

extern s32 D_003A8E60[];

extern char *D_00413860[4];

void fldLoadFieldEffectTextureSlots(void) {
    char *names[4];
    s32 i;

    memcpy(names, D_00413860, sizeof(names));
    for (i = 0; i < 4; i++) {
        fldEffectTextureLoadHandles[i] = sdfDevLoadFileEx(names[i], &fldEffectTextureData[i], 0);
        fldEffectTextureNodes[i] = effCreateNodeFromParameter(fldEffectTextureData[i]);
        D_003A8E60[i] = 0;
    }
}

void fldReleaseTextureSlots(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (fldEffectTextureNodes[i] != 0) {
            dds3EffectDelete(fldEffectTextureNodes[i]);
            fldEffectTextureNodes[i] = 0;
            sdfMemFreeDelayedDirect(fldEffectTextureLoadHandles[i]);
            fldEffectTextureLoadHandles[i] = NULL;
            fldEffectTextureData[i] = 0;
        }
    }
}

extern struct dds3Basic_t *fldPlayerObject;

extern f32 EulerAngleAxisY(void);
void fldUpdatePositionedTextureNodes(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 matrix[4][4] __attribute__((aligned(16)));
    f32 angle;
    s32 i;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    dds3GetRotPoseSet(fldPlayerObject);
    angle = EulerAngleAxisY();
    for (i = 0; i < 4; i++) {
        if (fldEffectTextureNodes[i] != 0) {
            position[0] = fldGlobalBase.x;
            position[1] = fldGlobalBase.y;
            position[2] = fldGlobalBase.z;
            if (i == 3) {
                position[1] -= 100.0f;
            }
            effCopyVectorToNodeInstance(fldEffectTextureNodes[i], position);
            SetRotateMatrixY(-angle);
            VU0_STORE_MATRIX_UNCLOBBERED(matrix);
            effApplyNodeTransformMatrix(fldEffectTextureNodes[i], matrix);
            dds3EffectDraw(fldEffectTextureNodes[i]);
        }
    }
}


void fldLoadResourceByIndex(s32 index) {
    s32 ids[2];

    memcpy(ids, D_00436330, sizeof(ids));
    fldIndexedResourceHandle = sdfDevLoadFileEx((const char *)(u32)ids[index], (u32 *)&fldIndexedResourceData, 0);
    fldIndexedResourceEffect = dds3EffectLoad(fldIndexedResourceData);
}

void fldReleaseIndexedResourceEffect(void) {
    if (fldIndexedResourceEffect != 0) {
        dds3EffectDelete(fldIndexedResourceEffect);
        fldIndexedResourceEffect = 0;
        sdfMemFreeDelayedDirect(fldIndexedResourceHandle);
        fldIndexedResourceHandle = NULL;
        fldIndexedResourceData = NULL;
    }
}



void fldUpdateIndexedResourceEffectPosition(void) {
    f32 position[4];
    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    if (fldIndexedResourceEffect != 0) {
        FldAreaWork *state = &fldGlobalBase;
        position[0] = state->x;
        position[1] = state->y;
        position[2] = state->z;
        effCopyVectorToNodeInstance(fldIndexedResourceEffect, position);
        dds3EffectDraw(fldIndexedResourceEffect);
    }
}

void mnuInitializeResourceEntries(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        mnuPositionedResourceNodes[i] = effCreateNodeFromParameter(D_0043633C);
        mnuPositionedResourceActive[i] = 0;
    }
}

extern s32 mnuPositionedResourceCursor;

void mnuSpawnResourceAtPosition(f32 x, f32 y, f32 z) {
    f32 pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    EffNode *handle;

    handle = mnuPositionedResourceNodes[mnuPositionedResourceCursor];
    if (handle != NULL) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3EffectReset(handle);
        effCopyVectorToNodeInstance(mnuPositionedResourceNodes[mnuPositionedResourceCursor], pos);
        mnuPositionedResourceActive[mnuPositionedResourceCursor] = 1;
        mnuPositionedResourceCursor = (mnuPositionedResourceCursor + 1) % 4;
    }
}

void fldQueuePrimaryEffectPosition(f32 x, f32 y, f32 z) {
    fldPrimaryEffectPositionPending = 1;
    fldPrimaryQueuedEffectPosition[0] = x;
    fldPrimaryQueuedEffectPosition[1] = y;
    fldPrimaryQueuedEffectPosition[2] = z;
}

void fldQueueSecondaryEffectPosition(f32 x, f32 y, f32 z) {
    fldSecondaryEffectPositionPending = 1;
    fldSecondaryQueuedEffectPosition[0] = x;
    fldSecondaryQueuedEffectPosition[1] = y;
    fldSecondaryQueuedEffectPosition[2] = z;
}

void mnuReleaseResourceEntries(void) {
    s32 i;
    fldPrimaryEffectPositionPending = 0;
    fldSecondaryEffectPositionPending = 0;
    for (i = 0; i < 4; i++) {
        if (mnuPositionedResourceNodes[i] != 0) {
            dds3EffectDelete(mnuPositionedResourceNodes[i]);
            mnuPositionedResourceNodes[i] = 0;
        }
    }
}

void fldUpdateMenuResourceEffects(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (mnuPositionedResourceNodes[i] != 0 && mnuPositionedResourceActive[i] != 0) {
            dds3EffectDraw(mnuPositionedResourceNodes[i]);
        }
    }
}




/* Retains field/automap TMX blocks and field-model resources. Unlike DDS1,
 * these map blocks come from separate files; fldmix.LB supplies other nodes. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413800);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413818);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413830);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413848);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413860);

void fldParseMixLb(void) {
    FilePacRequest *lb;
    PacWork *node;
    u32 index;

    fldGlobalBase.fieldTextures[0].allocation = sdfDevLoadFileEx("/fld/f/bin/d2_fild1.tmx", (u32 *)&fldGlobalBase.fieldTextures[0].resourceData, 0);
    fldGlobalBase.fieldTextures[1].allocation = sdfDevLoadFileEx("/fld/f/bin/d2_fild2.tmx", (u32 *)&fldGlobalBase.fieldTextures[1].resourceData, 0);
    fldGlobalBase.fieldTextures[2].allocation = sdfDevLoadFileEx("/fld/f/bin/d2_fild3.tmx", (u32 *)&fldGlobalBase.fieldTextures[2].resourceData, 0);
    fldGlobalBase.fieldTextures[3].allocation = sdfDevLoadFileEx("/fld/f/bin/d2_fild4.tmx", (u32 *)&fldGlobalBase.fieldTextures[3].resourceData, 0);
    fldGlobalBase.mapResources[0].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_1.tmx", (u32 *)&fldGlobalBase.mapResources[0].resourceData, 0);
    fldGlobalBase.mapResources[1].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_2.tmx", (u32 *)&fldGlobalBase.mapResources[1].resourceData, 0);
    fldGlobalBase.mapResources[2].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_3.tmx", (u32 *)&fldGlobalBase.mapResources[2].resourceData, 0);
    fldGlobalBase.mapResources[3].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_5.tmx", (u32 *)&fldGlobalBase.mapResources[3].resourceData, 0);
    fldGlobalBase.mapResources[4].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_6.tmx", (u32 *)&fldGlobalBase.mapResources[4].resourceData, 0);
    fldGlobalBase.mapResources[5].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_7.tmx", (u32 *)&fldGlobalBase.mapResources[5].resourceData, 0);
    fldGlobalBase.mapResources[6].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_8.tmx", (u32 *)&fldGlobalBase.mapResources[6].resourceData, 0);
    fldGlobalBase.mapResources[7].allocation = sdfDevLoadFileEx("/fld/f/bin/autmap_9.tmx", (u32 *)&fldGlobalBase.mapResources[7].resourceData, 0);
    index = 0;
    D_004362E0 = sdfDevLoadFileEx("/fld/f/bin/TOPEN.D3P", &D_004362E4, 0);
    D_00436304 = sdfDevLoadFileEx("/fld/f/bin/TAKARA2.D3P", &D_00436308, 0);
    D_004362EC = sdfDevLoadFileEx("/fld/f/bin/TOPEN2.D3P", &D_004362F0, 0);
    D_00436310 = sdfDevLoadFileEx("/fld/f/bin/TAKARA3.D3P", &D_00436314, 0);
    D_004362F8 = sdfDevLoadFileEx("/fld/f/bin/TOPEN3.D3P", &D_004362FC, 0);
    D_00436318 = sdfDevLoadFileEx("/fld/f/bin/TAKARA4.D3P", &D_0043631C, 0);
    D_00438EDC = sdfDevLoadFileEx("/fld/f/bin/DAMAGE_1.D3P", &D_00438EE0, 0);
    D_00438EE4 = sdfDevLoadFileEx("/fld/f/bin/DAMAGE_2.D3P", &D_00438EE8, 0);
    D_00438EEC = sdfDevLoadFileEx("/fld/f/bin/DAMAGE_3.D3P", &D_00438EF0, 0);
    lb = (FilePacRequest *)pacLoad("/fld/f/bin/fldmix.LB");
    pacSync((struct FileRequest *)lb);
    for (node = lb->packet.queueHead; node != NULL; node = node->next, index++) {
        switch (index) {
        case 5:
            D_00436338 = (s32)(u32)PAC_WORK_ALLOCATION(node);
            D_0043633C = sdfMemGetPtr(PAC_WORK_ALLOCATION(node));
            break;
        case 6:
            D_004362D4 = (s32)(u32)PAC_WORK_ALLOCATION(node);
            D_004362D8 = sdfMemGetPtr(PAC_WORK_ALLOCATION(node));
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            break;
        }
    }
    pacFileHandleFree_(lb);
}

extern s32 D_00438EE0;

extern s32 D_00438EE8;

extern s32 D_00438EF0;

extern u32 D_004362D8;

extern EffNode *D_004362DC;

extern struct EffNode *fldAreaDamageEffect;

extern s32 fldAreaDamageEffectPlaced;

/* For regular field areas, creates menu effects and acquires texture references
 * from the four retained d2_fild TMX blocks. */
void fldInitializeMenuResources(void) {
    if (fldGlobalBase.area < 200) {
        D_004362DC = dds3EffectLoad((EffNodeDescriptor *)D_004362D8);
        if (fldGlobalBase.area == 26) {
            fldAreaDamageEffect = dds3EffectLoad((EffNodeDescriptor *)(u32)D_00438EE0);
        } else if (fldGlobalBase.area == 29) {
            fldAreaDamageEffect = dds3EffectLoad((EffNodeDescriptor *)(u32)D_00438EE8);
        } else if (fldGlobalBase.area == 30) {
            fldAreaDamageEffect = dds3EffectLoad((EffNodeDescriptor *)(u32)D_00438EF0);
        } else {
            fldAreaDamageEffect = 0;
        }
        fldAreaDamageEffectPlaced = 0;
        fldGlobalBase.fieldTextures[0].texture = sdfTexSet(fldGlobalBase.fieldTextures[0].resourceData);
        fldGlobalBase.fieldTextures[1].texture = sdfTexSet(fldGlobalBase.fieldTextures[1].resourceData);
        fldGlobalBase.fieldTextures[2].texture = sdfTexSet(fldGlobalBase.fieldTextures[2].resourceData);
        fldGlobalBase.fieldTextures[3].texture = sdfTexSet(fldGlobalBase.fieldTextures[3].resourceData);
    }
}

extern EffNode *D_004362DC;

extern EffNode *D_004362E8;

extern EffNode *D_004362F4;

extern struct EffNode *fldAreaDamageEffect;

/* Releases retained field texture references and menu effects. Each reference
 * is cleared immediately so repeated cleanup does not release it twice. */
void fldFreeSceneResources(void) {
    if (fldGlobalBase.fieldTextures[0].texture != 0) {
        sdfTexReleaseReferenceViaHandler(fldGlobalBase.fieldTextures[0].texture);
        fldGlobalBase.fieldTextures[0].texture = 0;
    }
    if (fldGlobalBase.fieldTextures[1].texture != 0) {
        sdfTexReleaseReferenceViaHandler(fldGlobalBase.fieldTextures[1].texture);
        fldGlobalBase.fieldTextures[1].texture = 0;
    }
    if (fldGlobalBase.fieldTextures[2].texture != 0) {
        sdfTexReleaseReferenceViaHandler(fldGlobalBase.fieldTextures[2].texture);
        fldGlobalBase.fieldTextures[2].texture = 0;
    }
    if (fldGlobalBase.fieldTextures[3].texture != 0) {
        sdfTexReleaseReferenceViaHandler(fldGlobalBase.fieldTextures[3].texture);
        fldGlobalBase.fieldTextures[3].texture = 0;
    }
    if (D_004362DC != 0) {
        dds3EffectDelete(D_004362DC);
        D_004362DC = 0;
    }
    if (D_004362E8 != 0) {
        dds3EffectDelete(D_004362E8);
        D_004362E8 = 0;
    }
    if (D_004362F4 != 0) {
        dds3EffectDelete(D_004362F4);
        D_004362F4 = 0;
    }
    if (fldAreaDamageEffect != 0) {
        dds3EffectDelete(fldAreaDamageEffect);
        fldAreaDamageEffect = 0;
    }
}

void fldClearMenuEntries(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        fldModelSlotTable[i].placementId = 0;
        fldModelSlotTable[i].x = 0.0f;
        fldModelSlotTable[i].y = 0.0f;
        fldModelSlotTable[i].z = 0.0f;
        fldModelSlotTable[i].object = 0;
        fldModelSlotTable[i].unk2C = 0;
        fldModelSlotTable[i].unk30 = 0;
        fldModelSlotTable[i].unk34 = 0;
        fldModelSlotTable[i].room = 0;
        fldModelSlotTable[i].unk3C = 0;
        fldModelSlotTable[i].unk3E = 0;
    }
    fldNpcModelTable = 0;
}

void fldResetObjectSlots(void) {
    s32 i;

    fldTakaraCount = 0;
    for (i = 0; i < 32; i++) {
        fldTakaraSlotTable[i].id = -1;
        fldTakaraSlotTable[i].unitObject = NULL;
        fldTakaraSlotTable[i].activationState = FLD_OBJECT_ACTIVATION_INACTIVE;
        fldTakaraSlotTable[i].activationRequested = FLD_OBJECT_ACTIVATION_INACTIVE;
        if (fldTakaraSlotTable[i].effectNode != 0) {
            dds3EffectDelete(fldTakaraSlotTable[i].effectNode);
        }
        fldTakaraSlotTable[i].effectNode = 0;
    }
    D_00436300 = 0;
    D_0043630C = 0;
}

extern s32 D_00389784[];
extern FldNpcPalette D_0044FD90;
extern f32 sdfCos(f32);

void fldInitializeHuntSparkUnit(evtUnitHandle_t *unit) {
    dds3Basic_t *object;
    dds3ModelHandle_t *resource;
    FldNpcPaletteEntry *row;
    s32 index;
    f32 angle, height, radians;
    f32 direction[4];
    u32 colorA, colorB;
    s32 red, green, blue;

    object = unit->effObj;
    resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(object);
    SDF_VU0_LOADVECTOR(object->inner->rotation);
    index = D_00389784[0] + 1;
    if (index >= 64) {
        index = 63;
    }
    row = &D_0044FD90.entries[index];
    height = 0.0f;
    angle = -EulerAngleAxisY();
    if (row->phase >= 20) {
        angle = (f32)((row->phase - 20) % 20);
        angle = angle * 360.0f * 0.0625f;
        angle += 180.0f;
        if (row->phase >= 60) {
            height = -0.6f;
        } else if (row->phase >= 40) {
            height = 0.6f;
        }
    } else {
        if (row->phase == 0) {
            angle = angle * 180.0f / 3.14f;
            angle += 70.0f;
            height = 0.6f;
        } else if (row->phase == 1) {
            angle = angle * 180.0f / 3.14f;
            angle -= 70.0f;
            height = 0.6f;
        } else if (row->phase == 2) {
            angle = angle * 180.0f / 3.14f;
            angle -= 70.0f;
            height = -0.6f;
        } else if (row->phase == 3) {
            angle = angle * 180.0f / 3.14f;
            angle += 70.0f;
            height = -0.6f;
        } else {
            angle = angle * 180.0f / 3.14f;
            angle += 70.0f;
            height = 0.6f;
        }
    }
    evtUnitLightOn(unit);
    radians = angle * 3.14f;
    dds3ObjectBaseOnFlag(object, 0x100);
    sdfModelSetFaceDivParm(resource->inner, (f32)(row->textPair >> 4), (f32)(row->textPair & 15));
    radians /= 180.0f;
    direction[0] = sdfSin(radians);
    direction[1] = height;
    direction[2] = sdfCos(radians);
    direction[3] = 0.0f;
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    evtSetUnitLightDir(unit, 0);
    red = row->colorA[0];
    green = row->colorA[1];
    blue = row->colorA[2];
    colorA = red | (blue << 16) | (green << 8) | 0x80000000;
    red = row->colorB[0];
    green = row->colorB[1];
    blue = row->colorB[2];
    colorB = red | (blue << 16) | (green << 8) | 0x80000000;
    evtSetUnitLightAndAmbient(unit, 0, colorA, colorB);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    evtSetUnitLightDir(unit, 0);
    evtSetUnitLightAndAmbient(unit, 0, colorA, colorB);
}

/* Area/floor/name rules select an NPC model, animation and LOD. */
typedef struct FldNpcPlacementRule {
    s16 area, floor;
    char name[16];
    s8 kind;
    u8 resource;
    s8 lod, motion;
    s16 alternateFlag;
    u8 alternateResource;
    s8 alternateLod, alternateMotion, behavior;
    s16 requiredFlagA, requiredFlagB, requiredFlagC;
} FldNpcPlacementRule;
typedef char FldNpcPlacementRule_size_check[
    (sizeof(FldNpcPlacementRule) == 0x24) ? 1 : -1];

extern FldNpcPlacementRule D_003A55F0[400];
extern u32 sdfGetLodChunkValue(sdfModelRootNode_t *model);
extern void fldSpawnNpcTrigger(f32 x, f32 y, f32 z, f32 height, char *name, s32 arg1, s32 room, s32 index);

void func_0014C2B8(s32 key, f32 *orientation, s32 room, char *name, s32 argument,
                   f32 x, f32 y, f32 z) {
    f32 position[4];
    f32 rotation[4];
    s32 outputIndex;
    s32 kind, resource, motion, lod, visible;
    s32 behavior = 0;
    s32 i;
    f32 radius, height, hitHeight, size;
    dds3Basic_t *object;
    dds3ModelHandle_t *model;
    sdfModelRootNode_t *inner;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(rotation, 0, sizeof(rotation));
    {
        f32 scale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        kind = 3;
        resource = 9;
        motion = 0;
        lod = 0;
        visible = 0;
        outputIndex = fldNpcModelTable;
        fldModelSlotTable[outputIndex].x = x;
        fldModelSlotTable[outputIndex].y = y;
        fldModelSlotTable[outputIndex].z = z;
        fldModelSlotTable[outputIndex].orientation[0] = orientation[0];
        fldModelSlotTable[outputIndex].orientation[1] = orientation[1];
        fldModelSlotTable[outputIndex].orientation[2] = orientation[2];
        fldModelSlotTable[outputIndex].placementId = key;
        fldModelSlotTable[outputIndex].orientation[3] = orientation[3];
        fldModelSlotTable[outputIndex].unk2C = 0;
        fldModelSlotTable[outputIndex].unk30 = 0;
        fldModelSlotTable[outputIndex].unk3C = 0;
        fldModelSlotTable[outputIndex].object = NULL;
        fldModelSlotTable[outputIndex].unk34 = 0;
        fldModelSlotTable[outputIndex].room = room;
        strcpy(fldModelSlotTable[outputIndex].name, name);
        for (i = 0; i < 400; i++) {
            if (D_003A55F0[i].area == fldGlobalBase.area &&
                D_003A55F0[i].floor == fldGlobalBase.floor + 1 &&
                strcmp(D_003A55F0[i].name, name) == 0) {
                if (D_003A55F0[i].requiredFlagA != 0 && !evtBitCheck(D_003A55F0[i].requiredFlagA)) return;
                if (D_003A55F0[i].requiredFlagB != 0 && !evtBitCheck(D_003A55F0[i].requiredFlagB)) return;
                if (D_003A55F0[i].requiredFlagC != 0 && !evtBitCheck(D_003A55F0[i].requiredFlagC)) return;
                kind = D_003A55F0[i].kind;
                resource = D_003A55F0[i].resource;
                motion = D_003A55F0[i].motion;
                lod = D_003A55F0[i].lod;
                if (D_003A55F0[i].behavior >= 10) visible = 1;
                else visible = 0;
                behavior = D_003A55F0[i].behavior % 10;
                if (D_003A55F0[i].alternateFlag != 0 && evtBitCheck(D_003A55F0[i].alternateFlag)) {
                    resource = D_003A55F0[i].alternateResource;
                    motion = D_003A55F0[i].alternateMotion;
                    lod = D_003A55F0[i].alternateLod;
                }
                break;
            }
        }
        fldModelSlotTable[outputIndex].kind = kind;
        fldModelSlotTable[outputIndex].kindId = resource;
        fldModelSlotTable[outputIndex].unk3E = lod;
        if (behavior == 1) {
            radius = 50.0f; height = 180.0f; hitHeight = 100.0f;
            size = 30.0f;
        } else if (behavior == 2) {
            radius = 100.0f; height = 230.0f; hitHeight = 100.0f;
            size = 80.0f;
        } else {
            radius = 20.0f; height = 160.0f; hitHeight = 100.0f;
            size = 0.0f;
        }
        fldModelSlotTable[outputIndex].size = size;
        if (dds3SceneBasicSearchName(dds3GetControlScene(), "TEX_UNIT") == NULL) {
            object = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position, (u128 *)rotation);
            dds3SetUnitObjectId(object, "TEX_UNIT");
            dds3RebuildObjectModelContext(object, 3, 0x60);
            dds3ObjectBaseOffFlag(object, 0x400);
            model = dds3ObjectBaseGetModelHandle(object);
            model->flags |= 1;
        }
        if (fldGlobalBase.area == 15 && dds3SceneBasicSearchName(dds3GetControlScene(), "TEX_UNIT015") == NULL) {
            object = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position, (u128 *)rotation);
            dds3SetUnitObjectId(object, "TEX_UNIT015");
            dds3RebuildObjectModelContext(object, 3, 0xBF);
            dds3ObjectBaseOffFlag(object, 0x400);
            model = dds3ObjectBaseGetModelHandle(object);
            model->flags |= 1;
        }
        position[0] = x;
        position[1] = y;
        position[2] = z;
        SDF_VU0_LOADVECTOR(axis);
        FromRotation(3.14159265f);
        SDF_VU0_LOADVECTOR_B(orientation);
        effMiscQuatMultiplyVU();
        /* Audited SDK-style store: the complete output goes directly to opaque
         * constructor calls, with no scalar C readback or loop reuse. */
        SDF_VU0_STOREVECTOR_NOMEM(rotation);
        object = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position, (u128 *)rotation);
        dds3SetUnitObjectId(object, "NPC_MDL_UNIT");
        fldModelSlotTable[outputIndex].object = object;
        dds3RebuildObjectModelContext(object, kind, resource);
        dds3SetSclPoseSet(object, scale);
        if (kind == 3 && resource == 8) dds3GetRadiusPoseSet(object, 50.0f);
        model = dds3ObjectBaseGetModelHandle(object);
        model->first->frameStep = 1.0f;
        mdlLoopMotion(model, 0, motion);
        inner = model->inner;
        sdfGetLodChunkValue(inner);
        inner->lodIndex = lod;
        if (!visible) dds3ObjectBaseOnFlag(object, 0x400);
        else dds3ObjectBaseOffFlag(object, 0x400);
        fldInitializeHuntSparkUnit(evtGetUnitHandle2(object));
        fldAddItemBoxObject(x, y, z, radius, hitHeight, room);
        fldSpawnNpcTrigger(x, y, z, height, name, argument, room, fldNpcModelTable);
        fldNpcModelTable++;
    }
}

extern FldActivationEntry D_003A8EB0[256];
extern u8 fldTestObjectActivationFlag(u32 flagIndex);
extern void fldSpawnRoomTrigger(s32 roomId, f32 x, f32 y, f32 z, f32 radius);

void func_0014C9B0(u32 id, f32 *rotation, f32 x, f32 y, f32 z) {
    f32 position[4];
    f32 resultRotation[4];
    s32 modelId = 21;
    s32 entryIndex, floorIndex;
    dds3Basic_t *object;
    dds3ModelHandle_t *resource;
    evtUnitHandle_t *unit;
    FldNpcPaletteEntry *row;
    f32 heading, lightAngle;
    s32 red, green, blue;
    u32 colorA, colorB;
    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(resultRotation, 0, sizeof(resultRotation));
    {
        f32 scale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        f32 direction[4];
        entryIndex = fldNpcModelTable;
        fldNpcModelTable = entryIndex + 1;
        fldModelSlotTable[entryIndex].placementId = 999;
        fldModelSlotTable[entryIndex].x = x;
        fldModelSlotTable[entryIndex].y = y;
        fldModelSlotTable[entryIndex].z = z;
        fldModelSlotTable[entryIndex].orientation[0] = rotation[0];
        fldModelSlotTable[entryIndex].orientation[1] = rotation[1];
        fldModelSlotTable[entryIndex].orientation[2] = rotation[2];
        fldModelSlotTable[entryIndex].orientation[3] = rotation[3];
        fldModelSlotTable[entryIndex].unk2C = 0.0f;
        fldModelSlotTable[entryIndex].unk30 = 0;
        fldModelSlotTable[entryIndex].unk34 = 0;
        fldModelSlotTable[entryIndex].room = 0;
        fldModelSlotTable[entryIndex].unk3C = id;
        position[0] = x;
        position[1] = y;
        position[2] = z;
        switch (D_003A8EB0[id].kind) {
        case 0:
        case 3:
            modelId = 21;
            if (fldGlobalBase.area == 29 || fldGlobalBase.area == 30) {
                if (D_00436300 == 0) {
                    D_00436300 = (s32)dds3EffectLoad((EffNodeDescriptor *)(u32)D_00436314);
                    fldTakaraSlotTable[fldTakaraCount].effectNode = (EffNode *)(u32)D_00436300;
                } else {
                    fldTakaraSlotTable[fldTakaraCount].effectNode =
                        dds3EffectCopy((EffNode *)(u32)D_00436300);
                }
                modelId = 23;
            }
            break;
        case 1:
            if (fldGlobalBase.area == 29 || fldGlobalBase.area == 30) {
                if (D_0043630C == 0) {
                    D_0043630C = (s32)dds3EffectLoad((EffNodeDescriptor *)(u32)D_0043631C);
                    fldTakaraSlotTable[fldTakaraCount].effectNode = (EffNode *)(u32)D_0043630C;
                } else {
                    fldTakaraSlotTable[fldTakaraCount].effectNode =
                        dds3EffectCopy((EffNode *)(u32)D_0043630C);
                }
                modelId = 17;
            } else {
                if (D_0043630C == 0) {
                    D_0043630C = (s32)dds3EffectLoad((EffNodeDescriptor *)(u32)D_00436308);
                    fldTakaraSlotTable[fldTakaraCount].effectNode = (EffNode *)(u32)D_0043630C;
                } else {
                    fldTakaraSlotTable[fldTakaraCount].effectNode =
                        dds3EffectCopy((EffNode *)(u32)D_0043630C);
                }
                modelId = 22;
            }
            break;
        case 2:
            modelId = 23;
            break;
        }
        {
            f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
            SDF_VU0_LOADVECTOR(axis);
            FromRotation(3.1415926f);
        }
        SDF_VU0_LOADVECTOR_B(rotation);
        effMiscQuatMultiplyVU();
        SDF_VU0_STOREVECTOR_NOMEM(resultRotation);
        object = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position, (u128 *)resultRotation);
        dds3SetUnitObjectId(object, "TAKARA_UNIT");
        dds3RebuildObjectModelContext(object, 2, modelId);
        dds3SetSclPoseSet(object, (u128 *)scale);
        dds3ObjectBaseOnFlag(object, 0x100);
        resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(object);
        if (fldTestObjectActivationFlag(id)) {
            mdlLoopMotion(resource, 0, 2);
            fldTakaraSlotTable[fldTakaraCount].activationState = FLD_OBJECT_ACTIVATION_ACTIVE;
        } else {
            mdlSingleMotion(resource, 0, 0);
            fldTakaraSlotTable[fldTakaraCount].activationState = FLD_OBJECT_ACTIVATION_INACTIVE;
        }
        fldTakaraSlotTable[fldTakaraCount].id = id;
        fldTakaraSlotTable[fldTakaraCount].unitObject = object;
        fldTakaraSlotTable[fldTakaraCount].x = x;
        fldTakaraSlotTable[fldTakaraCount].y = y;
        fldTakaraSlotTable[fldTakaraCount].z = z;
        dds3GetRotPoseSet(object);
        heading = EulerAngleAxisY();
        fldTakaraSlotTable[fldTakaraCount].yaw = heading * 180.0f / 3.14f;
        fldTakaraSlotTable[fldTakaraCount].effectVariant = D_003A8EB0[id].kind;
        fldTakaraSlotTable[fldTakaraCount].effectKindIndex = id;
        fldTakaraCount++;
        fldAddItemBoxObject(x, y, z, 20.0f, 100.0f, -1);
        floorIndex = fldGlobalBase.floor + 1;
        if (floorIndex >= 64) {
            floorIndex = 63;
        }
        row = &D_0044FD90.entries[floorIndex];
        SDF_VU0_LOADVECTOR(resultRotation);
        lightAngle = EulerAngleAxisY();
        lightAngle = lightAngle * 180.0f;
        lightAngle = lightAngle / 3.14f;
        lightAngle += 80.0f;
        unit = evtGetUnitHandle2(object);
        evtUnitLightOn(unit);
        red = row->colorA[0];
        green = row->colorA[1];
        blue = row->colorA[2];
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = row->colorB[0];
        green = row->colorB[1];
        blue = row->colorB[2];
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        evtSetUnitLightAndAmbient(unit, 0, colorA, colorB);
        direction[0] = sdfSin(lightAngle * 3.14f / 180.0f);
        direction[1] = 0.5f;
        direction[2] = sdfCos(lightAngle * 3.14f / 180.0f);
        direction[3] = 0.0f;
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_NORMALIZE();
        evtSetUnitLightDir(unit, 0);
        fldSpawnRoomTrigger(id, position[0], position[1], position[2], 160.0f);
    }
}


void fldFindTakaraSlot(s32 id) {
    s32 i;

    for (i = 0; i < fldTakaraCount; i++) {
        if (fldTakaraSlotTable[i].id == id &&
            fldTakaraSlotTable[i].activationState == FLD_OBJECT_ACTIVATION_INACTIVE) {
            fldTakaraSlotTable[i].activationRequested = FLD_OBJECT_ACTIVATION_ANIMATING;
        }
    }
}

void fldReleaseObjectSlots(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (fldTakaraSlotTable[i].effectNode != 0) {
            dds3EffectDelete(fldTakaraSlotTable[i].effectNode);
            fldTakaraSlotTable[i].effectNode = 0;
        }
    }
    fldTakaraCount = 0;
    for (i = 0; i < 32; i++) {
        fldTakaraSlotTable[i].id = -1;
        fldTakaraSlotTable[i].unitObject = NULL;
        fldTakaraSlotTable[i].activationState = FLD_OBJECT_ACTIVATION_INACTIVE;
        fldTakaraSlotTable[i].activationRequested = FLD_OBJECT_ACTIVATION_INACTIVE;
    }
}

void fldUpdateObjectSlotActivation(void) {
    s32 i;
    dds3ModelHandle_t *resource;

    for (i = 0; i < fldTakaraCount; i++) {
        switch (fldTakaraSlotTable[i].activationState) {
        case FLD_OBJECT_ACTIVATION_INACTIVE:
            if (fldTakaraSlotTable[i].activationRequested == FLD_OBJECT_ACTIVATION_ANIMATING) {
                resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].unitObject);
                mdlSingleMotion(resource, 0, 1);
                fldTakaraSlotTable[i].activationState = fldTakaraSlotTable[i].activationRequested;
                switch (fldTakaraSlotTable[i].effectVariant) {
                case 0:
                case 3:
                    if (D_004362E8 != 0) {
                        dds3EffectDelete(D_004362E8);
                        D_004362E8 = 0;
                    }
                    if (fldGlobalBase.area == 29 || fldGlobalBase.area == 30) {
                        D_004362E8 = dds3EffectLoad(D_004362FC);
                        dds3EffectReset(D_004362DC);
                    } else {
                        D_004362E8 = dds3EffectLoad(D_004362E4);
                        dds3EffectReset(D_004362DC);
                    }
                    break;
                case 1:
                    if (D_004362F4 != 0) {
                        dds3EffectDelete(D_004362F4);
                        D_004362F4 = 0;
                    }
                    D_004362F4 = dds3EffectLoad(D_004362F0);
                    dds3EffectReset(D_004362DC);
                    break;
                }
            } else {
                resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].unitObject);
                if (resource->first->state == SDF_MOTION_STATE_TERMINAL) {
                    mdlSingleMotion(resource, 0, 0);
                }
            }
            break;
        case FLD_OBJECT_ACTIVATION_ANIMATING:
            resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].unitObject);
            if (resource->first->state == SDF_MOTION_STATE_TERMINAL) {
                mdlLoopMotion(resource, 0, 2);
                fldTakaraSlotTable[i].activationRequested = FLD_OBJECT_ACTIVATION_WAITING_FOR_ENCOUNTER;
                fldTakaraSlotTable[i].activationState = FLD_OBJECT_ACTIVATION_WAITING_FOR_ENCOUNTER;
            }
            break;
        case FLD_OBJECT_ACTIVATION_WAITING_FOR_ENCOUNTER:
            if (fldEncountTestFlag(0x40)) {
                resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].unitObject);
                mdlLoopMotion(resource, 0, 3);
                fldTakaraSlotTable[i].activationRequested = FLD_OBJECT_ACTIVATION_ACTIVE;
                fldTakaraSlotTable[i].activationState = FLD_OBJECT_ACTIVATION_ACTIVE;
                if (D_004362E8 != 0) {
                    dds3EffectDelete(D_004362E8);
                    D_004362E8 = 0;
                }
                if (D_004362F4 != 0) {
                    dds3EffectDelete(D_004362F4);
                    D_004362F4 = 0;
                }
            }
            break;
        }
    }
}

extern void fldSpawnRoomTriggerVariant(s32 kind, s32 roomValue,
                                       f32 x, f32 y, f32 z, f32 radius);

void func_0014D380(s32 kind, u32 id, f32 *rotation, f32 x, f32 y, f32 z) {
    f32 position[4];
    f32 resultRotation[4];
    s32 entryIndex, floorIndex;
    dds3Basic_t *object;
    dds3ModelHandle_t *resource;
    evtUnitHandle_t *unit;
    FldNpcPaletteEntry *row;
    f32 lightAngle;
    s32 red, green, blue;
    u32 colorA, colorB;
    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(resultRotation, 0, sizeof(resultRotation));
    {
        f32 scale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        f32 direction[4];
        entryIndex = fldNpcModelTable;
        fldNpcModelTable = entryIndex + 1;
        fldModelSlotTable[entryIndex].placementId = 999;
        fldModelSlotTable[entryIndex].x = x;
        fldModelSlotTable[entryIndex].y = y;
        fldModelSlotTable[entryIndex].z = z;
        fldModelSlotTable[entryIndex].orientation[0] = rotation[0];
        fldModelSlotTable[entryIndex].orientation[1] = rotation[1];
        fldModelSlotTable[entryIndex].orientation[2] = rotation[2];
        fldModelSlotTable[entryIndex].orientation[3] = rotation[3];
        fldModelSlotTable[entryIndex].unk2C = 0.0f;
        fldModelSlotTable[entryIndex].unk30 = 0;
        fldModelSlotTable[entryIndex].unk34 = 0;
        fldModelSlotTable[entryIndex].room = 0;
        fldModelSlotTable[entryIndex].unk3C = 0;
        position[0] = x;
        position[1] = y;
        position[2] = z;
        SDF_VU0_LOADVECTOR(axis);
        FromRotation(3.1415926f);
        SDF_VU0_LOADVECTOR_B(rotation);
        effMiscQuatMultiplyVU();
        SDF_VU0_STOREVECTOR_NOMEM(resultRotation);
        object = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position, (u128 *)resultRotation);
        if (kind == 0) {
            dds3SetUnitObjectId(object, "SAVE_UNIT");
            if (fldGlobalBase.area == 29 || fldGlobalBase.area == 30) {
                dds3RebuildObjectModelContext(object, 2, 15);
            } else {
                dds3RebuildObjectModelContext(object, 2, 3);
            }
        } else {
            dds3SetUnitObjectId(object, "HEAL_UNIT");
            if (fldGlobalBase.area == 29 || fldGlobalBase.area == 30) {
                dds3RebuildObjectModelContext(object, 2, 18);
            } else {
                dds3RebuildObjectModelContext(object, 2, 4);
            }
        }
        dds3SetSclPoseSet(object, (u128 *)scale);
        dds3ObjectBaseOnFlag(object, 0x100);
        resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(object);
        mdlLoopMotion(resource, 0, 0);
        fldAddItemBoxObject(x, y, z, 60.0f, 400.0f, -1);
        floorIndex = fldGlobalBase.floor + 1;
        if (floorIndex >= 64) {
            floorIndex = 63;
        }
        row = &D_0044FD90.entries[floorIndex];
        SDF_VU0_LOADVECTOR(resultRotation);
        lightAngle = EulerAngleAxisY();
        lightAngle = lightAngle * 180.0f;
        lightAngle = lightAngle / 3.14f;
        lightAngle += 80.0f;
        unit = evtGetUnitHandle2(object);
        evtUnitLightOn(unit);
        red = row->colorA[0];
        green = row->colorA[1];
        blue = row->colorA[2];
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = row->colorB[0];
        green = row->colorB[1];
        blue = row->colorB[2];
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        evtSetUnitLightAndAmbient(unit, 0, colorA, colorB);
        direction[0] = sdfSin(lightAngle * 3.14f / 180.0f);
        direction[1] = 0.5f;
        direction[2] = sdfCos(lightAngle * 3.14f / 180.0f);
        direction[3] = 0.0f;
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_NORMALIZE();
        evtSetUnitLightDir(unit, 0);
        fldSpawnRoomTriggerVariant(kind, id, position[0], position[1], position[2], 160.0f);
    }
}


extern s32 fldAreaDamageEffectPlaced;

s32 fldPlaceAreaDamageEffect(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = 1.0f;
    dds3EffectReset(fldAreaDamageEffect);
    effCopyVectorToNodeInstance(fldAreaDamageEffect, pos);
    fldAreaDamageEffectPlaced = 1;
    return 1;
}

extern void rsetOtOfs(u32);
extern void rsetAlpha(s32);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 ptyAnyLivingUnitHasStatus(u32 statusMask, s32 frontlineOnly);
extern void DrwSprite2DC(s32, s32, s32, s32, s32, s32, s32, s32, u32,
                          u32, u32, u32, sdfTexHandle_t *);
extern f32 D_0043634C;

/* Draws the field banner at (x, y), using field textures 0 and 2, unless either
 * field-state gate is 1. alpha is unused; the optional party mark animates. */
void fldDrawAnimatedFieldBanner(s32 alpha, s32 x, s32 y) {
    u32 color;

    if (fldGlobalBase.playerModelAlternate != 1 && fldGlobalBase.deferredExit != 1) {
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(x + 0x140, y + 0x10, 0x12, 0x19,
                            0x26, 3, 0x12, 0x19, 0x80808080,
                            fldGlobalBase.fieldTextures[0].texture);
        DrwSprite2D(x + 0x152, y + 0x10, 0x90, 0x19,
                            0x36, 3, 1, 0x19, 0x80808080,
                            fldGlobalBase.fieldTextures[0].texture);
        DrwSprite2D(x + 0x1E2, y - 1, 0x20, 0x39,
                            1, 2, 0x20, 0x39, 0x80808080,
                            fldGlobalBase.fieldTextures[0].texture);
        if (ptyAnyLivingUnitHasStatus(0x5D0, 0) != 0) {
            DrwSprite2D(x + 0x1B6, y + 0x2E, 0x23, 0xB,
                                0x3B, 0x25, 0x23, 0xB, 0x80808080,
                                fldGlobalBase.fieldTextures[2].texture);
            DrwSprite2D(x + 0x1D8, y + 0x26, 0x1B, 0x21,
                                0x64, 2, 0x1B, 0x21, 0x80808080,
                                fldGlobalBase.fieldTextures[2].texture);
            color = 0x808080;
            if (D_0043634C < 45.0f) {
                color = (s32)(sdfSin(D_0043634C * 4.0f * 3.14f / 180.0f) *
                              128.0f) + 0x80;
                color |= (color << 8) | (color << 16);
            }
            rsetAlpha(1);
            DrwSprite2DC(x + 0x1B6, y + 0x2E, 0x23, 0xB,
                          0x3B, 0x25, 0x23, 0xB,
                          color | 0x30000000, color | 0x30000000,
                          color | 0x30000000, color | 0x30000000,
                          fldGlobalBase.fieldTextures[2].texture);
            DrwSprite2DC(x + 0x1D8, y + 0x26, 0x1B, 0x21,
                          0x64, 2, 0x1B, 0x21,
                          color | 0x5A000000, color | 0x5A000000,
                          color | 0x5A000000, color | 0x5A000000,
                          fldGlobalBase.fieldTextures[2].texture);
            D_0043634C += 1.0f;
            if (D_0043634C > 90.0f) {
                D_0043634C = 0.0f;
            }
        }
        rsetAlpha(0);
    }
}

extern s32 fldGetSparkDialogPhase(void);
extern s16 *fldFindLocationCoordinateRecord(s32 area, s32 locationIndex);

void fldDrawLocationPanel(void) {
    u32 color;

    if (fldGetSparkDialogPhase() != 0) {
        return;
    }
    if (fldGlobalBase.playerModelAlternate == 1 || fldGlobalBase.deferredExit != 0) {
        return;
    }
    if (fldFindLocationCoordinateRecord(fldGlobalBase.area, fldGlobalBase.floor + 1)[2] <= 0) {
        return;
    }
    switch (fldGlobalBase.overlayMode) {
    case 0:
    case 2:
    case 4:
    case 6:
        break;
    case 1:
    case 3:
    case 5:
    case 7:
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        if (fldGlobalBase.overlayFadeInFrames > 0) {
            color = ((u32)((20 - fldGlobalBase.overlayFadeInFrames) * 128 / 20) << 24) | 0x808080;
        } else {
            color = 0x80808080;
        }
        DrwSprite2D(0x191, 0x11C, 0x60, 0x10, 1, 2, 0x60, 0x10,
                            color, fldGlobalBase.fieldTextures[2].texture);
        DrwSprite2D(0x191, 0x12C, 0x60, 0x70, 1, 0x11, 0x60, 1,
                            color, fldGlobalBase.fieldTextures[2].texture);
        DrwSprite2D(0x191, 0x19C, 0x60, 0x10, 1, 0x14, 0x60, 0x10,
                            color, fldGlobalBase.fieldTextures[2].texture);
        DrwSprite2D(0x1A4, 0x113, 0x3A, 0x11, 1, 0x2E, 0x3A, 0x11,
                            color, fldGlobalBase.fieldTextures[3].texture);
        fldResetCameraAndSceneView();
        rsetOtOfs(0x5B);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(0x191, 0x11D, 0x1C, 0x16, 0x24, 0x22, 0x1C, 0x16,
                            color, fldGlobalBase.fieldTextures[0].texture);
        DrwSprite2D(0x1C6, 0x191, 0x3A, 0x2C, 1, 1, 0x3A, 0x2C,
                            color, fldGlobalBase.fieldTextures[3].texture);
        break;
    }
}

extern u8 fldAutoMapSeqTestFlag(u32);

void fldFlushQueuedEffectPositions(void) {
    if (fldGlobalBase.area < 0xC8) {
        if (fldAutoMapSeqTestFlag(1) != 0) {
            return;
        }
        if (fldProcCampChk() != 0) {
            return;
        }
        if (fldProcAmChk() != 0) {
            return;
        }
        if (fldSecondaryEffectPositionPending != 0) {
            if (fldGlobalBase.colorEffectSuppressed == 0) {
                mnuSpawnResourceAtPosition(fldSecondaryQueuedEffectPosition[0], fldSecondaryQueuedEffectPosition[1], fldSecondaryQueuedEffectPosition[2]);
            }
            fldSecondaryEffectPositionPending = 0;
        }
        if (fldPrimaryEffectPositionPending != 0) {
            if (fldGlobalBase.colorEffectSuppressed == 0) {
                mnuSpawnResourceAtPosition(fldPrimaryQueuedEffectPosition[0], fldPrimaryQueuedEffectPosition[1], fldPrimaryQueuedEffectPosition[2]);
            }
            fldPrimaryEffectPositionPending = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DEA8);

void fldNpcHojiRefresh(void) {
    s32 i;

    for (i = 0; i < fldNpcModelTable; i++) {
        s32 room = fldModelSlotTable[i].room;
        if (room != 0 && fldTestMapSlotAuxiliaryFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, room) != 0) {
            if (fldModelSlotTable[i].object != 0) {
                dds3ObjectBaseOnFlag(fldModelSlotTable[i].object, 1);
            }
        }
    }
}




extern FldNpcPalette D_0044FD90;

void fldLoadNpcPalette(s32 field) {
    char path[64];
    char directory[32];
    sdfFileCmdHandle_t *command;

    if (field < 100) {
        fldMakeFilePath(directory, field, 1);
        if (field == 23 && evtBitCheck(25) != 0) {
            sprintf(path, "%sF033.NPL", directory);
        } else if (field == 24 && evtBitCheck(25) != 0) {
            sprintf(path, "%sF034.NPL", directory);
        } else if (field == 27 && evtBitCheck(25) != 0) {
            sprintf(path, "%sF037.NPL", directory);
        } else {
            sprintf(path, "%sF%03d.NPL", directory, field);
        }
        command = sdfFileOpen(path);
        sdfFileRead(command, &D_0044FD90, 0x200);
        sdfFileClose(command);
    }
}

void fldSetNpcPalette(const FldNpcPalette *src) {
    D_0044FD90 = *src;
}


s32 fldFindEffectByName(const char *name) {
    s32 i;

    for (i = 0; i < fldNpcModelTable; i++) {
        if (fldModelSlotTable[i].placementId != 999 && strcmp(fldModelSlotTable[i].name, name) == 0) {
            return (s32)fldModelSlotTable[i].object->key;
        }
    }
    return 0;
}

s32 fldGetCurrentSceneSelectionResource(void) {
    s32 index = fldGetCurrentSceneSelectionId();
    if (index >= 0) {
        return D_0044FFB0[index].resource[1];
    }
    return 0;
}

void fldSetTitleDrawMode(u32 value) {
    D_00436364 = value;
}

s32 fldTitleIsActive(void) {
    return dds3SearchProcessName(fldTitleTaskName) != 0;
}

void nbPanelAnalyzeStart(void) {
    D_0043635C = 0;
    D_00436354 = 1;
}

/* Render the field title through delay, fade-in, hold and fade-out phases. */
s32 fldTitleTaskUpdate(dds3ProcessID_t *task) {
    s32 alpha;
    u32 color;
    s32 nextFrame;

    if (D_00436364 == 0) {
        rsetOtOfs(0x53);
    } else {
        rsetOtOfs(0x5E);
    }
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 1);
    rsetAlpha(0);
    switch (D_00436354) {
    case 0:
        if (D_0043635C > 0) {
            D_0043635C--;
        }
        if (D_0043635C == 0) {
            D_00436354 = 1;
            D_00436350 = 0;
        }
        return 0;
    case 1:
        if (D_00436350 >= 20) {
            alpha = (D_00436350 - 19) * 128 / 15;
            if (alpha > 128) {
                alpha = 128;
            }
            color = (alpha << 24) | 0x808080;
            DrwSprite2D(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color, D_00436368);
            if (D_00389780[0] == 12) {
                DrwSprite2D(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color, D_0043636C);
            }
        }
        if (D_00436350 >= 20 && D_00436350 < 30) {
            rsetAlpha(1);
            alpha = (D_00436350 - 20) * 128 / 10;
            if (alpha > 128) {
                alpha = 128;
            }
            if (alpha < 0) {
                alpha = 0;
            }
            color = alpha << 24;
            DrwSprite2D(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color | 0x807060, D_00436368);
            if (D_00389780[0] == 12) {
                DrwSprite2D(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color | 0x808080, D_0043636C);
            }
            rsetAlpha(0);
        }
        if (D_00436350 >= 30) {
            rsetAlpha(1);
            alpha = (48 - D_00436350) * 128 / 18;
            if (alpha > 128) {
                alpha = 128;
            }
            if (alpha < 0) {
                alpha = 0;
            }
            color = alpha << 24;
            DrwSprite2D(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color | 0x807060, D_00436368);
            if (D_00389780[0] == 12) {
                DrwSprite2D(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color | 0x808080, D_0043636C);
            }
            rsetAlpha(0);
        }
        nextFrame = D_00436350 + 1;
        if (nextFrame > 60) {
            D_00436350 = 0;
            D_00436354++;
        } else {
            D_00436350 = nextFrame;
        }
        break;
    case 2:
        DrwSprite2D(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, 0x80808080, D_00436368);
        if (D_00389780[0] == 12) {
            DrwSprite2D(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, 0x80808080, D_0043636C);
        }
        nextFrame = D_00436350 + 1;
        if (nextFrame > 60) {
            D_00436350 = 0;
            D_00436354++;
        } else {
            D_00436350 = nextFrame;
        }
        break;
    case 3:
        alpha = (20 - D_00436350) * 128 / 20;
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha > 128) {
            alpha = 128;
        }
        color = (alpha << 24) | 0x808080;
        DrwSprite2D(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color, D_00436368);
        if (D_00389780[0] == 12) {
            DrwSprite2D(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color, D_0043636C);
        }
        nextFrame = D_00436350 + 1;
        if (nextFrame > D_00436360) {
            D_00436350 = 0;
            D_00436354++;
        } else {
            D_00436350 = nextFrame;
        }
        break;
    default:
        return -1;
    }
    return 0;
}

void fldReleaseTitleTextures(dds3ProcessID_t *task) {
    if (D_00436368 != 0) {
        sdfTexReleaseReferenceViaHandler(D_00436368);
        D_00436368 = 0;
    }
    if (D_0043636C != 0) {
        sdfTexReleaseReferenceViaHandler(D_0043636C);
        D_0043636C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", fldTitleTaskName);

void fldStartTitle(s32 field, s32 mode, s32 option) {
    char path[32];
    s32 resourceAddress;
    struct sdfMemHandle_t *allocation;

    D_00436350 = 0;
    D_00436354 = 0;
    D_00436358 = field;
    D_0043635C = mode;
    D_00436360 = option;
    D_00436364 = 0;
    if ((u32)(field - 8) < 2) {
        sprintf(path, "/fld/f/pnl/df%03d_a.tmx", field);
    } else if (field == 0xC) {
        sprintf(path, "/fld/f/pnl/df%03d_b.tmx", 0xC);
    } else {
        sprintf(path, "/fld/f/pnl/df%03d.tmx", field);
    }
    allocation = sdfDevLoadFileEx(path, &resourceAddress, 0);
    D_00436368 = sdfTexSet((SdfTextureFileHeader *)(resourceAddress));
    sdfMemFreeDirect(allocation);
    if (field == 0xC) {
        sprintf(path, "/fld/f/pnl/df_b.tmx");
        allocation = sdfDevLoadFileEx(path, &resourceAddress, 0);
        D_0043636C = sdfTexSet((SdfTextureFileHeader *)(resourceAddress));
        sdfMemFreeDirect(allocation);
    }
    if (fldTitleIsActive() == 0) {
        dds3StartProcess(fldTitleTaskName, 0x2B0A, 0, 1, fldTitleTaskUpdate, fldReleaseTitleTextures, 0);
    }
}

void fldDestroyTitleTask(void) {
    if (fldTitleIsActive()) {
        dds3KillProcessName(fldTitleTaskName, 1);
    }
}

s32 fldTitleMiniIsActive(void) {
    return dds3SearchProcessName(D_00413C80) != 0;
}

s32 fldTitleMini(dds3ProcessID_t *task) {
    s32 alpha;
    u32 color;
    s32 slide;
    s32 nextFrame;

    if (fldProcCampChk() == 1) {
        return 0;
    }
    rsetOtOfs(0x53);
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    switch (D_00436374) {
    case 0:
        nextFrame = D_00436370 + 1;
        if (nextFrame > 3) {
            D_00436374 = 1;
            D_00436370 = 0;
        } else {
            D_00436370 = nextFrame;
        }
        break;
    case 1:
        if (D_00436370 > 0) {
            if (D_00436370 - 1 < 20) {
                alpha = (D_00436370 - 1) * 128 / 20;
            } else {
                alpha = 128;
            }
            DrwSprite2D(0, 0x170, 0x200, 0x50,
                                0xD1, 1, 0x2D, 0x3D, ((u32)alpha << 24) | 0x808080, D_0043637C);
        }
        if (D_00436370 < 10) {
            slide = (10 - D_00436370) * 2;
        } else {
            slide = 0;
        }
        if (D_00436370 < 20) {
            alpha = D_00436370 * 128 / 20;
        } else {
            alpha = 128;
        }
        DrwSprite2D(0x132 + slide, 0x199, 0xCE, 0x16,
                            1, 0x28, 0xCE, 0x16, ((u32)alpha << 24) | 0x808080, D_0043637C);
        if (D_00436370 >= 8) {
            if (D_00436370 - 8 < 30) {
                alpha = (D_00436370 - 8) * 128 / 30;
            } else {
                alpha = 128;
            }
            DrwSprite2D(0x143, 0x18A, 0xBD, 0x24,
                                1, 2, 0xBD, 0x24, ((u32)alpha << 24) | 0x808080, D_0043637C);
        }
        nextFrame = D_00436370 + 1;
        if (nextFrame > 60) {
            D_00436370 = 0;
            D_00436374++;
        } else {
            D_00436370 = nextFrame;
        }
        break;
    case 2:
        if (D_00436370 >= 9 && fldGlobalBase.overlayFadeInFrames > 0) {
            fldGlobalBase.overlayFadeInFrames--;
        }
        if (D_00436370 < 30) {
            alpha = (30 - D_00436370) * 128 / 30;
        } else {
            alpha = 0;
        }
        color = ((u32)alpha << 24) | 0x808080;
        DrwSprite2D(0, 0x170, 0x200, 0x50,
                            0xD1, 1, 0x2D, 0x3D, color, D_0043637C);
        DrwSprite2D(0x132, 0x199, 0xCE, 0x16,
                            1, 0x28, 0xCE, 0x16, color, D_0043637C);
        DrwSprite2D(0x143, 0x18A, 0xBD, 0x24,
                            1, 2, 0xBD, 0x24, color, D_0043637C);
        nextFrame = D_00436370 + 1;
        if (nextFrame > 30) {
            D_00436370 = 0;
            D_00436374++;
        } else {
            D_00436370 = nextFrame;
        }
        break;
    default:
        return -1;
    }
    return 0;
}

void fldReleaseTitleMiniTexture(dds3ProcessID_t *task) {
    if (D_0043637C != 0) {
        sdfTexReleaseReferenceViaHandler(D_0043637C);
        D_0043637C = 0;
    }
}

extern s32 D_003898B4[];

extern s32 D_00436378;

extern s32 fldTitleMini(dds3ProcessID_t *task);

extern void fldReleaseTitleMiniTexture(dds3ProcessID_t *task);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C80);

void fldStartMiniTitleForUnlock(s32 id) {
    char path[0x20];
    s32 data;
    struct sdfMemHandle_t *allocation;

    switch (id) {
    case 4:
        if (evtBitCheck(0x420) == 0) {
            return;
        }
        break;
    case 5:
        if (evtBitCheck(0x424) == 0) {
            return;
        }
        break;
    case 7:
    case 8:
    case 9:
    case 11:
    case 13:
    case 14:
    case 15:
    case 30:
        break;
    case 10:
        if (evtBitCheck(0x421) == 0) {
            return;
        }
        break;
    case 12:
        if (evtBitCheck(0x422) == 0) {
            return;
        }
        break;
    case 21:
        if (evtBitCheck(0x440) == 0) {
            return;
        }
        break;
    case 22:
        if (evtBitCheck(0x480) == 0) {
            return;
        }
        break;
    case 23:
        if (evtBitCheck(0x4C0) == 0) {
            return;
        }
        break;
    case 24:
        if (evtBitCheck(0x500) == 0) {
            return;
        }
        break;
    case 25:
        if (evtBitCheck(0x540) == 0) {
            return;
        }
        break;
    case 26:
        if (evtBitCheck(0x580) == 0) {
            return;
        }
        break;
    case 27:
        if (evtBitCheck(0x5C0) == 0) {
            return;
        }
        break;
    case 28:
        if (evtBitCheck(0x600) == 0) {
            return;
        }
        break;
    case 29:
        if (evtBitCheck(0x640) == 0) {
            return;
        }
        break;
    case 31:
        if (evtBitCheck(0x6E0) == 0) {
            return;
        }
        break;
    case 32:
        if (evtBitCheck(0x6E1) == 0) {
            return;
        }
        break;
    case 33:
        if (evtBitCheck(0x6E2) == 0) {
            return;
        }
        break;
    case 34:
        if (evtBitCheck(0x6E3) == 0) {
            return;
        }
        break;
    case 35:
        if (evtBitCheck(0x6E4) == 0) {
            return;
        }
        break;
    case 38:
        if (evtBitCheck(0x6E5) == 0) {
            return;
        }
        break;
    default:
        return;
    }
    D_003898B4[0] = 20;
    D_00436378 = id;
    D_00436370 = 0;
    D_00436374 = 0;
    sprintf(path, "/fld/f/pnl/ds_%03d.tmx", id);
    allocation = sdfDevLoadFileEx(path, &data, 0);
    D_0043637C = sdfTexSet((SdfTextureFileHeader *)(data));
    sdfMemFreeDirect(allocation);
    if (fldTitleIsActive() == 0) {
        dds3StartProcess(D_00413C80, 0x2B0A, 0, 1, fldTitleMini, fldReleaseTitleMiniTexture, 0);
    }
}

void fldSetSnowActive(void) {
    if (D_00436374 == 0) {
        D_00436370 = 0;
        D_00436374 = 1;
    }
}

void fldLoadWeatherEffects(void) {
    s32 data;
    struct sdfMemHandle_t *allocation;

    allocation = sdfDevLoadFileEx("/fld/f/bin/d2_hunt1.tmx", &data, 0);
    D_004363AC = sdfTexSet((SdfTextureFileHeader *)(data));
    sdfMemFreeDelayedDirect(allocation);
    allocation = sdfDevLoadFileEx("/fld/f/bin/d2_hunt2.tmx", &data, 0);
    D_004363B0 = sdfTexSet((SdfTextureFileHeader *)(data));
    sdfMemFreeDelayedDirect(allocation);
    allocation = sdfDevLoadFileEx("/fld/f/bin/d2_hunt3.tmx", &data, 0);
    D_004363B4 = sdfTexSet((SdfTextureFileHeader *)(data));
    sdfMemFreeDelayedDirect(allocation);
    D_00436380 = sdfDevLoadFileEx("/fld/f/bin/FH_DAM_2.EPL", &D_00436384, 0);
    fldDamEffectNode = effCreateNodeFromParameter(D_00436384);
    fldDamEffectPositioned = 0;
    D_00436390 = sdfDevLoadFileEx("/fld/f/bin/YUK_2.EPL", &D_00436394, 0);
    fldYukEffectNode = effCreateNodeFromParameter(D_00436394);
    fldYukEffectPositioned = 0;
}

void fldReleaseWeatherEffects(void) {
    if (D_004363AC != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363AC);
        D_004363AC = 0;
    }
    if (D_004363B0 != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363B0);
        D_004363B0 = 0;
    }
    if (D_004363B4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363B4);
        D_004363B4 = 0;
    }
    dds3EffectDelete(fldDamEffectNode);
    fldDamEffectNode = 0;
    fldDamEffectPositioned = 0;
    sdfMemFreeDelayedDirect(D_00436380);
    D_00436380 = NULL;
    D_00436384 = 0;
    dds3EffectDelete(fldYukEffectNode);
    fldYukEffectNode = 0;
    fldYukEffectPositioned = 0;
    sdfMemFreeDelayedDirect(D_00436390);
    D_00436390 = NULL;
    D_00436394 = 0;
}

void fldSetWeatherEffectPos(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (fldDamEffectNode != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3EffectReset(fldDamEffectNode);
        effCopyVectorToNodeInstance(fldDamEffectNode, pos);
        fldDamEffectPositioned = 1;
    }
    if (fldYukEffectNode != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3EffectReset(fldYukEffectNode);
        effCopyVectorToNodeInstance(fldYukEffectNode, pos);
        fldYukEffectPositioned = 1;
    }
}

void fldUpdateWeatherEffectNodes(void) {
    if (fldDamEffectNode != 0 && fldDamEffectPositioned != 0) {
        dds3EffectDraw(fldDamEffectNode);
    }
    if (fldYukEffectNode != 0 && fldYukEffectPositioned != 0) {
        dds3EffectDraw(fldYukEffectNode);
    }
}

/* The 0xE0 resource row owns sixteen 12-byte signed-ID sequences at +0x20. */
typedef struct FldSparkSequence {
    s32 count;
    s8 slots[8];
} FldSparkSequence;

typedef struct FieldResourceRecord {
    s16 category;
    s16 id;
    s16 entryCount;
    s16 encounterSelection;
    s16 duration;
    u8 pad0A[0x16];
    FldSparkSequence sequences[16];
} FieldResourceRecord;

typedef char FieldResourceRecordSizeCheck[(sizeof(FieldResourceRecord) == 0xE0) ? 1 : -1];
typedef char FieldResourceRecord_encounterSelection_offset_must_be_0x06[
    ((u32)&((FieldResourceRecord *)0)->encounterSelection == 0x06) ? 1 : -1];

extern FieldResourceRecord *D_00435E18;

FldSparkRewardProfileIndex fldFindResourceRecordIndex(s32 category, s32 id) {
    FieldResourceRecord *record = D_00435E18;
    FldSparkRewardProfileIndex index;
    for (index = 0; index < 8; index++, record++) {
        if (record->category == category && record->id == id) {
            return index;
        }
    }
    return 0;
}

extern SdfMathVector4 D_00413DB8;
extern SdfMathVector4 D_00413DC8;
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DB8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DC8);

const char D_00413DD8[] = "HUNT_MDL_UNIT";

/* Create the sixteen (plus one) hunt-unit objects used by the field spark controller. */
void fldCreateSparkSlotObjects(void) {
    SdfMathVector4 position = D_00413DB8;
    SdfMathVector4 rotation;
    FldSparkObjectEntry *entry;
    dds3ModelHandle_t *model;
    s32 i;

    memset(&rotation, 0, sizeof(rotation));
    {
        SdfMathVector4 scale = D_00413DC8;

        for (i = 0; i < 17; i++) {
            fldSparkObjectEntries[i].objectHandle = 0;
            fldSparkObjectEntries[i].sparkSlotIndex = -1;
            *(s32 *)fldSparkObjectEntries[i].pad8 = 0;
        }
        for (i = 0; i < 17; i++) {
            entry = &fldSparkObjectEntries[i];
            entry->objectHandle = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)&position, (u128 *)&rotation);
            dds3SetUnitObjectId(entry->objectHandle, D_00413DD8);
            dds3RebuildObjectModelContext(entry->objectHandle, 1, 0x130);
            dds3SetSclPoseSet(entry->objectHandle, (u128 *)&scale);
            model = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(entry->objectHandle);
            model->first->frameStep = 1.0f;
            mdlLoopMotion(model, 0, 0);
            model->flags |= 1;
            dds3ObjectBaseOnFlag(entry->objectHandle, 0x400);
            fldInitializeHuntSparkUnit(evtGetUnitHandle2(entry->objectHandle));
        }
    }
}

void fldClearObjectEntryHandles(void) {
    FldSparkObjectEntry *entry = fldSparkObjectEntries;
    s32 i = 0x10;

    do {
        s32 objectHandle = entry->objectHandle;

        i--;
        if (objectHandle != 0) {
            dds3DeleteUnitObjectBasic((dds3Basic_t *)objectHandle);
            entry->objectHandle = 0;
        }
        entry++;
    } while (i >= 0);
}

void fldInitSparkTable(void) {
    s32 i;

    if (fldGlobalBase.specialEventActive == 0) {
        for (i = 0; i < 64; i++) {
            fldSparkSlots[i].hasVectors = 0;
            fldSparkSlots[i].active = 0;
            fldSparkSlots[i].reservedObjectSlot = 0;
            fldSparkSlots[i].objectSlot = -1;
            fldSparkSlots[i].tickCounter = dds3GetRandA(0) % 60 + 15;
        }
    } else {
        for (i = 0; i < 64; i++) {
            fldSparkSlots[i].hasVectors = 0;
            fldSparkSlots[i].objectSlot = -1;
        }
    }
}

void fldResetSparkTable(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        fldSparkSlots[i].active = 0;
        fldSparkSlots[i].reservedObjectSlot = 0;
        fldSparkSlots[i].objectSlot = -1;
        fldSparkSlots[i].tickCounter = dds3GetRandA(0) % 60 + 15;
    }
}

s32 fldSetSparkVectors(s32 index, const u128 *position, const u128 *rotation) {
    SDF_VU0_COPYVECTOR(fldSparkSlots[index].position, position);
    SDF_VU0_COPYVECTOR(fldSparkSlots[index].rotation, rotation);
    fldSparkSlots[index].hasVectors = 1;
    return 1;
}

s32 fldAttachSparkSlotToWorldObject(s32 index, s32 reserved) {
    s32 slot;
    s32 i;
    s32 candidate;
    dds3ModelHandle_t *model;
    u128 position;

    if (reserved == 0) {
        slot = -1;
        for (i = 0; i < 16; i++) {
            candidate = (fldSparkControlState.cursor + i) % 16;
            if (fldSparkObjectEntries[candidate].sparkSlotIndex == -1) {
                slot = candidate;
                break;
            }
        }
    } else {
        slot = 16;
    }
    if (slot < 0) {
        return 0;
    }
    fldSparkControlState.cursor = (slot + 1) % 16;
    if (fldSparkSlots[index].hasVectors == 1) {
        SDF_VU0_COPYVECTOR(&position, fldSparkSlots[index].position);
        dds3SetPosPoseSet((dds3Basic_t *)fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].position);
        dds3SetRotPoseSet((dds3Basic_t *)fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].rotation);
        model = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle((dds3Basic_t *)(u32)fldSparkObjectEntries[slot].objectHandle);
        model->flags &= ~MDL_SKIP_TRANSFORMS;
        fldSparkSlots[index].objectSlot = slot;
        fldSparkObjectEntries[slot].sparkSlotIndex = index;
        return 1;
    }
    return 0;
}

void fldFreeSparkSlot(s32 index) {
    SdfMathVector4 vec;
    s32 obj;
    s32 *flags;
    s16 slot;

    vec = D_00413DE8;
    if (fldSparkSlots[index].hasVectors == 1 && fldSparkSlots[index].active != 0 && fldSparkSlots[index].objectSlot != -1) {
        vec.x = fldSparkSlots[index].position[0];
        vec.z = fldSparkSlots[index].position[2];
        dds3SetPosPoseSet((dds3Basic_t *)fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle, (u128 *)&vec);
        obj = fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle;
        flags = (s32 *)dds3ObjectBaseGetModelHandle((dds3Basic_t *)(u32)obj);
        *flags |= 1;
        dds3ObjectBaseOffFlag(obj, 0x400);
        slot = fldSparkSlots[index].objectSlot;
        fldSparkSlots[index].objectSlot = -1;
        fldSparkObjectEntries[slot].sparkSlotIndex = -1;
    }
}

void fldUpdateSparkSlots(void) {
    s32 i;

    fldCreateSparkSlotObjects();
    for (i = 0; i < 64 && i < fldSparkControlState.state.entryCount; i++) {
        if (fldSparkSlots[i].hasVectors != 0 && fldSparkSlots[i].active != 0) {
            fldAttachSparkSlotToWorldObject(i, fldSparkSlots[i].reservedObjectSlot);
        }
    }
}

extern s32 D_004363B8;
extern void fldReleaseCameraModel(u32);

void fldUpdateSparkSequence(void) {
    FldSparkSequence *sequence;
    s32 i;
    s32 slot;
    SdfMathVector4 position __attribute__((aligned(16)));
    f32 dx;
    f32 dy;
    f32 dz;

    D_004363B8++;
    if (D_004363B8 % 5 == 0) {
        if (fldSparkControlState.scheduledSounds > 0) {
            fldSparkControlState.scheduledSounds--;
            p2sdPlaySE(0x680014, 0x7F, 0x3F);
        }
    }
    if (fldSparkControlState.state.sequenceRemaining == 0) {
        i = fldSparkControlState.state.sequenceIndex;
        sequence = &fldSparkControlState.entry->sequences[i];
        fldSparkControlState.state.sequenceRemaining = sequence->count;
        if (sequence->count == -1) {
            fldSparkControlState.state.sequenceIndex = 0;
            sequence = &fldSparkControlState.entry->sequences[0];
            fldSparkControlState.state.sequenceRemaining = sequence->count;
        }
        for (i = 0; i < sequence->count; i++) {
            slot = sequence->slots[i];
            fldAttachSparkSlotToWorldObject(slot, 0);
            fldSparkSlots[slot].reservedObjectSlot = 0;
            fldSparkSlots[slot].active = 1;
            fldSparkControlState.scheduledSounds++;
            fldSparkSlots[slot].tickCounter = 0;
        }
        fldSparkControlState.state.sequenceIndex++;
    }
    for (i = 0; i < 64 && i < fldSparkControlState.state.entryCount; i++) {
        switch (fldSparkSlots[i].active) {
        case 0:
            break;
        case 1:
            fldSparkSlots[i].tickCounter++;
            if (fldSparkSlots[i].objectSlot != -1 && fldSparkControlState.state.mode == 1 &&
                fldSparkControlState.state.pulse != 0 && (u32)(fldSparkControlState.state.modeCountdown - 10) < 6) {
                SDF_VU0_COPYVECTOR(&position, fldSparkSlots[i].position);
                dx = fldGlobalBase.x - position.x;
                dy = fldGlobalBase.y - position.y;
                dz = fldGlobalBase.z - position.z;
                if (fsqrtf(dx * dx + dy * dy + dz * dz) < 200.0f) {
                    fldSetWeatherEffectPos(position.x, position.y, position.z);
                    fldGlobalBase.collectedCount++;
                    fldGlobalBase.score += 0x32;
                    fldSparkControlState.pickupDelay = 4;
                    fldSparkControlState.state.sequenceRemaining--;
                    p2sdPlaySE(0x680013, 0x7F, 0x3F);
                    fldReleaseCameraModel(3);
                    fldSparkControlState.state.modeCountdown += 3;
                    fldFreeSparkSlot(i);
                    fldSparkSlots[i].reservedObjectSlot = 0;
                    fldSparkSlots[i].active = 0;
                    fldSparkSlots[i].tickCounter = dds3GetRandA(0) % 270 + 30;
                    fldSparkControlState.state.pulse = 0;
                }
            }

            break;
        }
    }
}

u8 fldIsNearSpark(f32 x, f32 y, f32 z) {
    s32 i;

    for (i = 0; i < 64 && i < fldSparkControlState.state.entryCount; i++) {
        if (fldSparkSlots[i].active == 1 && fldSparkSlots[i].objectSlot != -1) {
            f32 dx = x - fldSparkSlots[i].position[0];
            f32 dy = y - fldSparkSlots[i].position[1];
            f32 dz = z - fldSparkSlots[i].position[2];

            if (fsqrtf(dx * dx + dy * dy + dz * dz) < 50.0f) {
                return 1;
            }
        }
    }
    return 0;
}

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void eglLinkTri(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Draw the countdown, pickup total, and expanding pickup pulse. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DE8);

void fldDrawSparkStatusOverlay(void) {
    s32 timerOrigins[10][2] = {
        {2, 2}, {21, 2}, {40, 2}, {59, 2}, {78, 2}, {97, 2},
        {2, 21}, {21, 21}, {40, 21}, {59, 21}
    };
    s32 pickupOrigins[10][2] = {
        {2, 2}, {26, 2}, {50, 2}, {74, 2}, {98, 2},
        {2, 25}, {26, 25}, {50, 25}, {74, 25}, {98, 25}
    };
    /* Each layer row contains radius, packed vertex color, and copy count. */
    s32 pickupEffects[5][3] = {
        {1, 0x18808080, 1}, {1, 0x30808080, 2}, {1, 0x60808080, 3},
        {2, 0x60808080, 2}, {3, 0x60808080, 1}
    };
    s32 remainingFrames;
    s32 remaining;
    s32 digit;
    s32 pickupTens;
    s32 pickupOnes;
    s32 i;
    s32 radius;
    s32 color;
    s32 diameter;
    u32 vertexColor;

    remainingFrames = fldSparkControlState.state.countdown;
    vertexColor = 0x80808080;
    eglSetOt(0x53);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(1);
    eglLinkTri(0, 0, 180, 0, 0, 140,
                              0x1A063564, 0x01063564, 0x01063564);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitTexturedGradientQuad(15, 19, 74, 13, 2, 41, 74, 13,
        vertexColor, vertexColor, vertexColor, vertexColor, D_004363AC);
    evtSubmitTexturedGradientQuad(24, 26, 122, 48, 2, 2, 122, 48,
        vertexColor, vertexColor, vertexColor, vertexColor, D_004363B4);

    remaining = remainingFrames / 30;
    if (remaining >= 100) {
        remaining = 99;
    }
    digit = remaining / 10;
    if (digit > 0) {
        evtSubmitTexturedGradientQuad(89, 15, 18, 18,
            timerOrigins[digit][0], timerOrigins[digit][1], 18, 18,
            vertexColor, vertexColor, vertexColor, vertexColor, D_004363AC);
    }
    digit = remaining % 10;
    evtSubmitTexturedGradientQuad(99, 15, 18, 18,
        timerOrigins[digit][0], timerOrigins[digit][1], 18, 18,
        vertexColor, vertexColor, vertexColor, vertexColor, D_004363AC);

    remaining = (s16)fldGlobalBase.collectedCount;
    if (remaining >= 100) {
        remaining = 99;
    }
    pickupTens = remaining / 10;
    pickupOnes = remaining % 10;
    if (pickupTens > 0) {
        evtSubmitTexturedGradientQuad(33, 37, 22, 21,
            pickupOrigins[pickupTens][0], pickupOrigins[pickupTens][1], 22, 21,
            vertexColor, vertexColor, vertexColor, vertexColor, D_004363B0);
    }
    evtSubmitTexturedGradientQuad(50, 37, 22, 21,
        pickupOrigins[pickupOnes][0], pickupOrigins[pickupOnes][1], 22, 21,
        vertexColor, vertexColor, vertexColor, vertexColor, D_004363B0);

    if (fldSparkControlState.pickupDelay > 0) {
        evtSubmitPrimaryAlphaBlendMode(1);
        for (i = 0; i < pickupEffects[fldSparkControlState.pickupDelay][2]; i++) {
            radius = pickupEffects[fldSparkControlState.pickupDelay + i][0];
            color = pickupEffects[fldSparkControlState.pickupDelay][1] / (i + 1);
            diameter = radius * 2;
            if (pickupTens > 0) {
                evtSubmitTexturedGradientQuad(33 - radius, 37 - radius,
                    22 + diameter, 21 + diameter,
                    pickupOrigins[pickupTens][0], pickupOrigins[pickupTens][1], 22, 21,
                    color, color, color, color, D_004363B0);
            }
            evtSubmitTexturedGradientQuad(50 - radius, 37 - radius,
                22 + diameter, 21 + diameter,
                pickupOrigins[pickupOnes][0], pickupOrigins[pickupOnes][1], 22, 21,
                color, color, color, color, D_004363B0);
        }
        evtSubmitPrimaryAlphaBlendMode(0);
        if (fldSparkControlState.pickupDelay > 0) {
            fldSparkControlState.pickupDelay--;
        }
    }
}

extern s16 D_00389874[];
void func_00150800(void) {
    FieldResourceRecord *record;
    s32 duration;
    s32 entryCount;
    FldSparkRewardProfileIndex recordIndex;
    FldSparkController *control;

    if (D_003899E0[0] != 0) {
        return;
    }
    if (evtBitCheck(0x13) && !evtBitCheck(0x25)) {
        fclInitMessage(D_003A9EB0);
        fclStartMessage(6);
        D_00389874[0] = 1;
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_UNAVAILABLE_MESSAGE;
        fldSparkControlState.state.phase = 0;
        fldSparkControlState.state.dialogTimer = 0;
        fldResetPlayerSceneObjectState();
        return;
    }
    if (evtBitCheck(0x802)) {
        fclInitMessage(D_003A9EB0);
        fclStartMessage(5);
        D_00389874[0] = 1;
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_UNAVAILABLE_MESSAGE;
        fldSparkControlState.state.phase = 0;
        fldSparkControlState.state.dialogTimer = 0;
        fldResetPlayerSceneObjectState();
        return;
    }
    fclInitMessage(D_003A9EB0);
    if (evtBitCheck(0x803)) {
        fclStartMessage(1);
    } else {
        fclStartMessage(0);
    }
    evtStoreValueAndCaptureWindowPanelValue(2);
    evtSetMessageWindowOptionWhenOpen(0);
    evtBitOn(0x803);
    recordIndex = fldFindResourceRecordIndex(fldGlobalBase.area, fldGlobalBase.floor + 1);
    control = &fldSparkControlState;
    record = &D_00435E18[recordIndex];
    fldGlobalBase.specialEventActive = 1;
    fldGlobalBase.collectedCount = 0;
    entryCount = record->entryCount;
    duration = record->duration;
    control->state.dialogPhase = FLD_SPARK_DIALOG_PHASE_ENTRY_PROMPT;
    control->state.countdown = duration;
    control->state.entryCount = entryCount;
    do {
    control->scheduledSounds = 0;
    control->rewardProfileIndex = recordIndex;
    fldGlobalBase.unk10C = 0.0f;
    fldGlobalBase.score = 0;
    } while (0);
    control->entry = record;
    do {
    control->state.phase = 0;
    control->state.terminated = 0;
    control->state.mode = 0;
    control->state.modeCountdown = 0;
    control->state.pendingMode = 0;
    control->state.pulse = 0;
    control->state.dialogTimer = 0;
    control->state.sequenceIndex = 0;
    control->state.sequenceRemaining = 0;
    } while (0);
    control->cursor = 0;
    control->pickupDelay = 0;
    fldResetSparkTable();
    fldResetPlayerSceneObjectState();
}


extern void fldTogglePlayerModel(void);

/* Restores field presentation once an active event finishes, then clears the
 * event latch. An already-cleared latch leaves field resources untouched. */
void fldFinishEventFieldState(void) {
    FldAreaWork *work = &fldGlobalBase;

    if (work->specialEventActive != 0) {
        D_003899E0[0] = 0;
        fldTogglePlayerModel();
        fldClearObjectEntryHandles();
        fldReleaseWeatherEffects();
        fldStartSceneBgmAlternate();
        fldPreparePlayerSceneCameraTarget();
        work->specialEventActive = 0;
        fldSparkControlState.state.phase = 0;
        fldSparkControlState.state.countdown = 0;
        evtSetSolarOverlayFullyVisible();
    }
}

extern struct FileRequest *D_004363A0;
extern struct sdfMemHandle_t *D_004363A4;
extern u32 D_004363A8;
extern void dspRequestMessageWindowModeOne(void);
extern s8 evtGetCapturedWindowPanelValue(void);
struct SoundMixer;
extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(struct SoundMixer *, const void *, const void *);
extern void dds3EffectObjectBasicStatOff(dds3Basic_t *, u32);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *, u32);
extern void dds3StartBEDEffectObjectBasic(dds3Basic_t *, u32);
extern void fldSetCameraModelMotionRequest(u32, u32);
extern void fldDrawSparkStatusOverlay(void);
extern void mnuResetTitleStreamAfterFileIdle(void);

void fldUpdateDevilizeEventSequence(void) {
    f32 position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    f32 rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    s32 choice;
    void *node;

    switch (fldSparkControlState.state.dialogPhase) {
    case FLD_SPARK_DIALOG_PHASE_UNAVAILABLE_MESSAGE:
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == 0) {
            evtFinishMessageWindowAndNotify();
            fclDestroyMessage();
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_CANCELLED;
            fldGlobalBase.specialEventActive = 0;
            evtSetSolarOverlayFullyVisible();
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_ENTRY_PROMPT:
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == 0) {
            choice = evtGetCapturedWindowPanelValue();
            if (choice == 0) {
                evtFinishMessageWindowAndNotify();
                fclDestroyMessage();
                fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_START_ARCHIVE_LOAD;
                fldSparkControlState.state.dialogTimer = 0;
            } else if (choice == 1) {
                evtFinishMessageWindowAndNotify();
                fclDestroyMessage();
                fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_CANCELLED;
                fldGlobalBase.specialEventActive = 0;
                evtSetSolarOverlayFullyVisible();
            } else {
                evtFinishMessageWindowAndNotify();
                fclDestroyMessage();
                fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_CONFIRM_PROMPT;
                fldSparkControlState.state.dialogTimer = 0;
                fclInitMessage(D_003A9EB0);
                fclStartMessage(3);
                evtStoreValueAndCaptureWindowPanelValue(2);
                evtSetMessageWindowOptionWhenOpen(0);
            }
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_CONFIRM_PROMPT:
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == 0) {
            evtFinishMessageWindowAndNotify();
            fclDestroyMessage();
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_ENTRY_PROMPT;
            fldSparkControlState.state.dialogTimer = 0;
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_START_ARCHIVE_LOAD:
        dds3WorkClear();
        fldBgmFadeOut();
        D_004363A0 = fileLoad("/fld/f/bin/DVILIZE.BED");
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_WAIT_ARCHIVE_FILE;
        fldSparkControlState.state.dialogTimer = 0;
        break;
    case FLD_SPARK_DIALOG_PHASE_WAIT_ARCHIVE_FILE:
        if (pacFileCheckEnd(D_004363A0) != 0) {
            D_004363A8 = (u32)fileGetLoadPtr(D_004363A0);
            D_004363A4 = fileGetMemHandle(D_004363A0);
            position[0] = fldGlobalBase.x;
            position[1] = fldGlobalBase.y;
            position[2] = fldGlobalBase.z;
            node = dds3AddBEDEffectObjectBasic2((struct SoundMixer *)D_004363A8, position, rotation);
            fldSparkControlState.object = node;
            dds3EffectObjectBasicStatOff(node, 1);
            fileRequestFreeIfComplete(D_004363A0);
            fldAddCameraModelEntry(0xD);
            D_003899E0[0] = 1;
            fldResetArchiveLoadPhase();
            fldSparkControlState.state.dialogTimer = 0;
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_ARCHIVE_LOADING;
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_ARCHIVE_LOADING:
        if (fldSparkControlState.state.dialogTimer == 20) {
            dds3EffectObjectBasicStatOn(fldSparkControlState.object, 1);
            dds3StartBEDEffectObjectBasic(fldSparkControlState.object, 0);
        }
        fldSparkControlState.state.dialogTimer++;
        if (fldStepArchiveLoad() != FLD_ARCHIVE_LOAD_PENDING) {
            mnuQueueTitleSoundStreamEntry(0x12);
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_TITLE_STREAM_WAIT;
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_TITLE_STREAM_WAIT:
        if (fldSparkControlState.state.dialogTimer == 20) {
            dds3EffectObjectBasicStatOn(fldSparkControlState.object, 1);
            dds3StartBEDEffectObjectBasic(fldSparkControlState.object, 0);
        }
        fldSparkControlState.state.dialogTimer++;
        if (mnuPollTitleStreamStateLocked() == 2) {
            p2sdPlaySE(0x680010, 0x7F, 0x3F);
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_PLAY_EVENT;
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_PLAY_EVENT:
        if (fldSparkControlState.state.dialogTimer == 46) {
            dds3FadeOut(0xFF, 0xFF, 0xFF, 4);
        }
        if (fldSparkControlState.state.dialogTimer == 20) {
            dds3EffectObjectBasicStatOn(fldSparkControlState.object, 1);
            dds3StartBEDEffectObjectBasic(fldSparkControlState.object, 0);
        }
        if (++fldSparkControlState.state.dialogTimer > 50) {
            fldSparkControlState.state.dialogTimer = 0;
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_RESTORE_FIELD;
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_RESTORE_FIELD:
        fldCreateSparkSlotObjects();
        fldLoadWeatherEffects();
        fldTogglePlayerModel();
        fldSetCameraModelMotionRequest(0x66, 0);
        fldSparkControlState.state.dialogTimer = 0;
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_WAIT_FIELD_FADE;
        evtDisableMoonDraw();
        dds3FadeIn(0xFF, 0xFF, 0xFF, 4);
        break;
    case FLD_SPARK_DIALOG_PHASE_WAIT_FIELD_FADE:
        fldDrawSparkStatusOverlay();
        if (++fldSparkControlState.state.dialogTimer > 10) {
            dds3EffectObjectBasicStatOff(fldSparkControlState.object, 1);
            dds3DeleteSceneBasicData(fldSparkControlState.object);
            sdfMemFreeDelayedDirect(D_004363A4);
            fldSparkControlState.state.dialogTimer = 0;
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_ENTER_ENCOUNTER;
            mnuResetTitleStreamAfterFileIdle();
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_ENTER_ENCOUNTER:
        fldDrawSparkStatusOverlay();
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_IDLE;
        fldSparkControlState.state.phase = 1;
        fldPreparePlayerSceneCameraTarget();
        break;
    case FLD_SPARK_DIALOG_PHASE_CANCELLED:
        fldSparkControlState.state.phase = 1;
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_IDLE;
        fldPreparePlayerSceneCameraTarget();
        break;
    }
}


s32 fldGetSparkDialogPhase(void) {
    return fldSparkControlState.state.dialogPhase;
}

/* Update the spark encounter, its timeout dialog, and the return-to-field fade. */
void fldUpdateSparkEncounterScene(void) {
    s32 phase;
    s32 value;

    phase = fldSparkControlState.state.phase;
    if (phase == 1) {
        if (fldSparkControlState.state.mode == 0) {
            if ((s8)dds3PadButtonState[1][0][0] < 0) {
                fldSetCameraModelMotionRequest(6, 20);
                p2sdPlaySE(0x680012, 0x7F, 0x3F);
                fldSparkControlState.state.mode = 1;
                fldSparkControlState.state.pulse = 1;
                fldSparkControlState.state.modeCountdown = 20;
                fldSparkControlState.state.pendingMode = 0;
            }
        } else if (fldSparkControlState.state.modeCountdown > 0) {
            value = fldSparkControlState.state.modeCountdown - 1;
            fldSparkControlState.state.modeCountdown = value;
            if (value == 0) {
                if (fldSparkControlState.state.pendingMode == 0) {
                    s32 pendingMode;
                    fldSetCameraModelMotionRequest(1, 0);
                    pendingMode = fldSparkControlState.state.pendingMode;
                    fldSparkControlState.state.mode = pendingMode;
                    fldSparkControlState.state.modeCountdown = 0;
                    fldSparkControlState.state.pendingMode = 0;
                }
            } else if (fldSparkControlState.state.mode == 1 &&
                       fldSparkControlState.state.pendingMode == 0 && value < 14 &&
                       (s8)dds3PadButtonState[1][0][0] < 0) {
                fldSetCameraModelMotionRequest(6, 20);
                p2sdPlaySE(0x680012, 0x7F, 0x3F);
                fldSparkControlState.state.mode = 1;
                fldSparkControlState.state.pulse = 1;
                fldSparkControlState.state.modeCountdown = 20;
            }
        }

        if (fldGetSparkDialogPhase() == 0 && fldEncountTestFlag(0x40) != 0) {
            if (fldSparkControlState.state.countdown == 300) {
                p2sdPlaySE(0x680017, 0x7F, 0x3F);
            }
            if (fldSparkControlState.state.countdown > 0) {
                fldSparkControlState.state.countdown--;
            }
        }

        fldDrawSparkStatusOverlay();
        if (fldSparkControlState.state.countdown < 31) {
            p2sdStopSe(0x680017);
            fldSparkControlState.state.countdown = 0;
            fldReleaseCameraModel(0);
            fldSetCameraModelMotionRequest(2, 0);
            fldSetCameraNodeModeWithTen();
            fldSparkControlState.state.phase = 3;
            fclInitMessage(D_003A9EB0);
            fclStartMessage(7);
            fldResetPlayerSceneObjectState();
        } else {
            fldUpdateSparkSequence();
        }
        return;
    } else if (phase == 3) {
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == 0) {
            evtFinishMessageWindowAndNotify();
            fclDestroyMessage();
            fldSparkControlState.state.frame = 0;
            fldSparkControlState.state.phase = 6;
            fldSparkControlState.state.terminated = 1;
        }
        if (fldSparkControlState.state.terminated != 0) {
            mnuMarkTitleStreamResetPending();
            fldGlobalBase.pendingSceneRequest = 1;
            fldResetPlayerSceneObjectState();
            fldSparkControlState.state.phase = 7;
            evtBitOn(0x802);
            return;
        }
    } else if (phase == 6) {
        return;
    } else if (phase == 7) {
        fldSparkControlState.state.frame++;
        if (fldSparkControlState.state.frame == 40) {
            dds3FadeOut(255, 255, 255, 4);
        }
        if (fldSparkControlState.state.frame < 45) {
            return;
        }
        fldSparkControlState.state.phase = 8;
    } else if (phase == 8) {
        p2sdPlaySE(0x680011, 0x7F, 0x3F);
        fldTogglePlayerModel();
        fldStartSceneBgmAlternate();
        evtSetSolarOverlayFullyVisible();
        fldGlobalBase.unk138 = 1;
        fldSparkControlState.state.phase = 9;
        fldSparkControlState.state.frame = 0;
    } else if (phase == 9) {
        fldSparkControlState.state.frame++;
        if (fldSparkControlState.state.frame == 1) {
            dds3FadeIn(255, 255, 255, 4);
        }
        if (fldSparkControlState.state.frame == 6) {
            dds3EffectObjectBasicStatOff(fldSparkControlState.object, 1);
            dds3DeleteSceneBasicData(fldSparkControlState.object);
            sdfMemFreeDelayedDirect(D_004363A4);
        }
        if (fldSparkControlState.state.frame >= 11) {
            D_003899E0[0] = 0;
            fldPreparePlayerSceneCameraTarget();
            fldSparkControlState.state.phase = 0;
            fldGlobalBase.specialEventActive = 0;
        }
    } else {
        return;
    }
}


void fldRunDanteClear(void) {
    fldSparkControlState.rewardProfileIndex = FLD_SPARK_REWARD_PROFILE_INDEX_NONE;
}



/* Releases scene presentation and camera resources before clearing the event
 * latch and installing the existing field-state values. */
void fldResetEventSceneState(void) {
    fldReleaseCameraModel(0);
    D_003899E0[0] = 0;
    fldGlobalBase.playerModelAlternate = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldPreparePlayerSceneCameraTarget();
    fldGlobalBase.deferredExit = 1;
    fldGlobalBase.specialEventActive = 0;
    fldSparkControlState.state.phase = 0;
    fldGlobalBase.unk138 = 1;
}

/* Restores title/menu and field presentation after an event, then clears the
 * event latch and field gates while retaining the existing reset values. */
void fldResetAfterEvent(void) {
    p2sdStopSe(0x680017);
    mnuAdvanceTitleStateUnderSemaphore();
    D_003899E0[0] = 0;
    fldGlobalBase.playerModelAlternate = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldStartSceneBgmAlternate();
    fldGlobalBase.deferredExit = 0;
    fldGlobalBase.specialEventActive = 0;
    fldSparkControlState.state.phase = 0;
    fldGlobalBase.unk138 = 1;
    evtSetSolarOverlayFullyVisible();
}

void fldStartDeferredFieldExit(void) {
    fclInitMessage(D_003A9EB0);
    fclStartMessage(4);
    fldResetPlayerSceneObjectState();
    D_00389884[0] = 2;
}

/* Completes deferred exit state 2 only after the message window stops being
 * controlled, then closes its display channel and restores the camera target. */
void fldFinishDeferredExit(void) {
    if (fldGlobalBase.deferredExit == 2) {
        dspRequestMessageWindowModeOne();
        if (!fclChkMessage()) {
            evtFinishMessageWindowAndNotify();
            fclDestroyMessage();
            fldPreparePlayerSceneCameraTarget();
            fldGlobalBase.deferredExit = 0;
        }
    }
}

u8 fldIsEventPhaseAtLeastTwo(void) {
    if (fldSparkControlState.state.phase < 2) {
        return 0;
    }
    return 1;
}

void fldTickWeatherEffectNodes(void) {
    fldUpdateWeatherEffectNodes();
}

/* The serialized resource row retains a signed parameter at +6. */
s32 fldGetSparkEncounterSelection(void) {
    return fldSparkControlState.entry->encounterSelection;
}

FldSparkRewardProfileIndex fldGetSparkRewardProfileIndex(void) {
    return fldSparkControlState.rewardProfileIndex;
}


s16 fldGetSparkPickupCount(void) {
    return fldGlobalBase.collectedCount;
}

typedef struct FieldGuidePoint {
    u8 x;
    u8 y;
    u16 bearing;
} FieldGuidePoint;

typedef struct FieldGridCoordPair {
    s16 x;
    s16 y;
} FieldGridCoordPair;

/* Motion states consumed by fldDispatchTargetGuideMotionUpdate. */
#define FLD_TARGET_GUIDE_MOTION_STOPPED 0
#define FLD_TARGET_GUIDE_MOTION_WAIT_FOR_PLAYER 1
#define FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE 2
#define FLD_TARGET_GUIDE_MOTION_FAST_ROUTE 3
#define FLD_TARGET_GUIDE_MOTION_ARRIVAL_PAUSE 4
#define FLD_TARGET_GUIDE_MOTION_CYCLE_ROUTE 5
#define FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_6 6
#define FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_7 7
#define FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_8 8

typedef struct FieldTargetGuideState {
    s32 motionMode;
    s32 disabled;
    f32 position[3];
    f32 yaw;
    f32 previousModelFrame; /* 0x18 */
    f32 currentModelFrame; /* 0x1C */
    s16 gridX;
    s16 gridY;
    s16 targetGridX;
    s16 targetGridY;
    s16 unk28;
    s16 unk2A;
    struct FieldCoordinateRecord *routeRecord;
    FieldGuidePoint *route;
    FieldGridCoordPair *cycleRoute;
    s32 routeIndex;
    s32 routeEnd;
    s32 moveTimer;
    f32 targetYaw;
    f32 stepDistance;
    s32 age;
    s32 updateFlags;
    s32 guideMotionIndex; /* 0x54: animation index submitted to the guide model. */
    f32 targetDistance; /* 0x58 */
    f32 targetBearing;  /* 0x5C */
    f32 viewAngleError; /* 0x60 */
    s32 guideMode;
    s16 previousGridX;
    s16 previousGridY;
    s32 unk6C;
    s32 cycleIndex;
} FieldTargetGuideState;

typedef char FieldTargetGuideStateGuideMotionIndexOffsetCheck[
    ((u32)&((FieldTargetGuideState *)0)->guideMotionIndex == 0x54) ? 1 : -1];
typedef char FieldTargetGuideStateSizeCheck[sizeof(FieldTargetGuideState) == 0x74 ? 1 : -1];

extern FieldTargetGuideState fldTargetGuideState;

void fldResetViewState(void) {
    fldTargetGuideState.unk6C = -1;
    fldTargetGuideState.guideMotionIndex = 0;
    fldTargetGuideState.updateFlags = 0;
    fldTargetGuideState.guideMode = 0;
    fldTargetGuideState.motionMode = FLD_TARGET_GUIDE_MOTION_STOPPED;
    fldTargetGuideState.cycleIndex = 0;
    fldTargetGuideState.cycleRoute = NULL;
    fldTargetGuideState.disabled = 0;
}

void fldLoadTargetGuideResource(void) {
    typedef struct {
        u32 firstOffset;
        u32 unk04;
        u32 secondOffset;
    } FieldCoordinateResource;
    char directory[0x20];
    char path[0x40];
    FieldCoordinateResource *resource;
    u32 base;
    u32 first;
    u32 second;
    u32 third;

    fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
    sprintf(path, D_00413F20, directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
    D_004363C4 = sdfDevLoadFileEx(path, &resource, 0);
    base = (u32)resource;
    first = base + resource->firstOffset;
    second = base + resource->secondOffset;
    third = base + *(u32 *)(second + 4);
    D_004363C8 = base;
    D_004363CC = first;
    D_004363D0 = second;
    D_004363D4 = third;
}


void fldFreeRunMapData(void) {
    if (D_004363C4 != 0) {
        sdfMemFreeDelayedDirect(D_004363C4);
        D_004363C4 = 0;
        D_004363C8 = 0;
        D_004363CC = 0;
        D_004363D0 = 0;
        D_004363D4 = 0;
    }
}

extern s16 D_004363BC;
extern s16 D_004363BE;
extern s16 D_004363C0;
extern s16 D_004363C2;

typedef struct FieldCellGrid {
    u16 width;
    u16 height;
} FieldCellGrid;

/* Find the occupied grid cell nearest to a world position; returns -1 when it is cell (0, 0). */
s32 fldFindNearestOccupiedMapGridCell(f32 x, f32 z, s16 *outX, s16 *outY) {
    f32 originX = D_004363BC;
    f32 originY = D_004363BE;
    f32 nearest = 10000.0f;
    s16 bestCol = 0;
    s16 bestRow = 0;
    s16 row;
    s16 col;

    if (D_004363D0 == 0) {
        *outX = 0;
        *outY = 0;
        return 0;
    }
    for (row = 0; row < ((FieldCellGrid *)D_004363D0)->height; row++) {
        for (col = 0; col < ((FieldCellGrid *)D_004363D0)->width; col++) {
            s32 offset = (((FieldCellGrid *)D_004363D0)->width * row + col) * 2;

            if (*(u8 *)(D_004363D4 + offset) != 0) {
                f32 dx = x - (originX + (f32)D_004363C0 * (f32)col);
                f32 dz = z - (originY + (f32)-D_004363C2 * (f32)row);
                f32 distance = fsqrtf(dx * dx + dz * dz);

                if (distance < nearest) {
                    nearest = distance;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
    }
    *outX = bestCol;
    *outY = bestRow;
    if (bestCol != 0 || bestRow != 0) {
        return 0;
    }
    return -1;
}

void fldMapIndexToPos(s16 x, s16 y, f32 *outX, f32 *outY) {
    f32 scaleX = (f32)D_004363C0;
    f32 originX = (f32)D_004363BC;
    f32 originY = (f32)D_004363BE;
    f32 sourceX = (f32)x;
    f32 sourceY = (f32)y;
    *outX = originX + scaleX * sourceX;
    *outY = originY + (f32)(-D_004363C2) * sourceY;
}

typedef struct FieldCoordinateRecord {
    u8 sourceGridX;      /* 0x00: route lookup key */
    u8 sourceGridY;      /* 0x01: route lookup key */
    u8 destinationGridX; /* 0x02: route lookup key */
    u8 destinationGridY; /* 0x03: route lookup key */
    u32 flags;           /* 0x04: bit 0 is checked when a route finishes */
    u32 routePointOffset; /* 0x08: points relative to the coordinate resource */
    s32 routePointCount; /* 0x0C: signed count used to form the final point index */
} FieldCoordinateRecord;

typedef char FieldCoordinateRecordLayoutCheck[
    (sizeof(FieldCoordinateRecord) == 0x10 &&
     (u32)&((FieldCoordinateRecord *)0)->sourceGridX == 0x00 &&
     (u32)&((FieldCoordinateRecord *)0)->sourceGridY == 0x01 &&
     (u32)&((FieldCoordinateRecord *)0)->destinationGridX == 0x02 &&
     (u32)&((FieldCoordinateRecord *)0)->destinationGridY == 0x03 &&
     (u32)&((FieldCoordinateRecord *)0)->flags == 0x04 &&
     (u32)&((FieldCoordinateRecord *)0)->routePointOffset == 0x08 &&
     (u32)&((FieldCoordinateRecord *)0)->routePointCount == 0x0C)
        ? 1 : -1];

typedef struct {
    s32 unk00;
    u32 count;
} FieldCoordinateList;

FieldCoordinateRecord *fldFindMapCell(s16 x, s16 y, s16 z, s16 w) {
    FieldCoordinateList *list = (FieldCoordinateList *)D_004363C8;
    FieldCoordinateRecord *record = (FieldCoordinateRecord *)D_004363CC;
    u32 index;
    for (index = 0; index < list->count; index++, record++) {
        if (record->sourceGridX == (u8)x && record->sourceGridY == (u8)y &&
            record->destinationGridX == (u8)z && record->destinationGridY == (u8)w) {
            return record;
        }
    }
    return NULL;
}

void fldCalcTargetDistanceYaw(f32 *distance, f32 *angle) {
    FldAreaWork *player = &fldGlobalBase;
    FieldTargetGuideState *target = &fldTargetGuideState;
    f32 dx = player->x - target->position[0];
    f32 dz = player->z - target->position[2];
    f32 dist = fsqrtf(dx * dx + dz * dz);
    f32 yaw = 0.0f;
    if (!(dist < 1.0f)) {
        yaw = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    *distance = dist;
    *angle = yaw;
}

f32 fldTurnToward(f32 cur, f32 target, f32 speed, f32 minStep) {
    f32 delta = fldAngleDifference(cur, target);
    f32 step = delta / speed;

    if (step < 0.0f) {
        step = -step;
    }
    if (step < minStep) {
        step = minStep;
    }
    if ((delta >= 0.0f && delta <= step) || (delta <= 0.0f && -step <= delta)) {
        delta = target;
    } else if (delta < 0.0f) {
        delta = cur - step;
    } else {
        delta = cur + step;
    }
    return delta;
}

extern FieldGridCoordPair D_00438EF8;

/* Change the guide's model motion index, flagging the change. */
static inline void fldSetTargetGuideUpdateMode(s32 value) {
    if (fldTargetGuideState.guideMotionIndex != value) {
        fldTargetGuideState.guideMotionIndex = value;
        fldTargetGuideState.updateFlags |= 2;
    }
}

/* Route the target guide from one grid cell to another and start its point
 * walk, or idle when no route record connects them. A macro because the
 * target cell is re-read after the record lookup. */
#define FLD_ROUTE_TARGET_GUIDE(state, fromX, fromY, toX, toY, step, timer)                \
    {                                                                                     \
        FieldCoordinateRecord *record = fldFindMapCell(fromX, fromY, toX, toY); \
                                                                                          \
        (state)->routeRecord = record;                                                    \
        (state)->targetGridX = (toX);                                                     \
        (state)->targetGridY = (toY);                                                     \
        if (record != NULL) {                                                             \
            FieldGuidePoint *route = (FieldGuidePoint *)(D_004363C8 + record->routePointOffset); \
                                                                                          \
            (state)->routeIndex = 0;                                                      \
            (state)->stepDistance = (step);                                               \
            (state)->routeEnd = record->routePointCount - 1;                              \
            (state)->moveTimer = (timer);                                                 \
            (state)->route = route;                                                       \
            (state)->targetYaw = (f32)route->bearing;                                     \
        } else {                                                                          \
            (state)->stepDistance = 0.0f;                                                 \
            (state)->route = NULL;                                                        \
            (state)->routeIndex = 0;                                                      \
            (state)->routeEnd = 0;                                                        \
            (state)->targetYaw = 0.0f;                                                    \
            (state)->moveTimer = 0x14;                                                    \
        }                                                                                 \
    }

/* Hold the guide in place under a timed mode. */
static inline void fldSetTargetGuideIdleMode(FieldTargetGuideState *state, s32 motionMode) {
    state->motionMode = motionMode;
    state->moveTimer = 0x1E;
    state->updateFlags |= 2;
    state->unk2A = 0;
    state->stepDistance = 0.0f;
    state->guideMotionIndex = 0;
}

/* Switch the field target guide's motion mode, routing it toward the player
 * or along its cycle route as the mode requires. */
void fldSetTargetGuideMotionMode(s32 motionMode) {
    FieldTargetGuideState *state = &fldTargetGuideState;

    state->age = 0;
    state->unk28 = 0;
    if (motionMode == FLD_TARGET_GUIDE_MOTION_STOPPED) {
        state->motionMode = FLD_TARGET_GUIDE_MOTION_STOPPED;
        state->unk2A = 0;
        state->moveTimer = 0;
        state->stepDistance = 0.0f;
    } else if (motionMode == FLD_TARGET_GUIDE_MOTION_WAIT_FOR_PLAYER) {
        fldSetTargetGuideIdleMode(state, motionMode);
    } else if (motionMode == FLD_TARGET_GUIDE_MOTION_FAST_ROUTE) {
        state->motionMode = motionMode;
        state->unk2A = 0;
        fldFindNearestOccupiedMapGridCell(state->position[0], state->position[2], &state->gridX, &state->gridY);
        FLD_ROUTE_TARGET_GUIDE(state, state->gridX, state->gridY, D_00438EF8.x, D_00438EF8.y, 20.0f, 0x14);
        fldSetTargetGuideUpdateMode(0x11);
    } else if (motionMode == FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE) {
        state->motionMode = motionMode;
        state->unk2A = 1;
        fldFindNearestOccupiedMapGridCell(state->position[0], state->position[2], &state->gridX, &state->gridY);
        fldFindNearestOccupiedMapGridCell(fldGlobalBase.x, fldGlobalBase.z, &D_00438EF8.x, &D_00438EF8.y);
        FLD_ROUTE_TARGET_GUIDE(state, state->gridX, state->gridY, D_00438EF8.x, D_00438EF8.y, 5.0f, 0x50);
        fldSetTargetGuideUpdateMode(0x10);
    } else {
        if (motionMode == FLD_TARGET_GUIDE_MOTION_ARRIVAL_PAUSE) {
            state->moveTimer = 0x3C;
            state->guideMotionIndex = 0x14;
            state->updateFlags |= 2;
            state->motionMode = motionMode;
            state->unk2A = 0;
            state->stepDistance = 0.0f;
        }
        if (motionMode == FLD_TARGET_GUIDE_MOTION_CYCLE_ROUTE) {
            s32 index = state->cycleIndex;
            s32 next;

            state->unk2A = 1;
            next = index + 1;
            if (state->cycleRoute[index + 1].x == -1) {
                next = 0;
            }
            state->motionMode = motionMode;
            fldFindNearestOccupiedMapGridCell(state->position[0], state->position[2], &state->gridX, &state->gridY);
            fldFindNearestOccupiedMapGridCell(fldGlobalBase.x, fldGlobalBase.z, &D_00438EF8.x, &D_00438EF8.y);
            FLD_ROUTE_TARGET_GUIDE(state, state->cycleRoute[index].x, state->cycleRoute[index].y,
                                   state->cycleRoute[next].x, state->cycleRoute[next].y, 5.0f, 0x50);
            fldSetTargetGuideUpdateMode(0x12);
        } else if (motionMode == FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_6) {
            fldSetTargetGuideIdleMode(state, motionMode);
        } else if (motionMode == FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_7) {
            fldSetTargetGuideIdleMode(state, motionMode);
        } else if (motionMode == FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_8) {
            fldSetTargetGuideIdleMode(state, motionMode);
        }
    }
}

void func_00151DD8(void) {
}

void fldTickTargetGuideCounter(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}

extern void fldSetTargetGuideMotionMode(s32);

void fldUpdateTargetGuideFastRoute(void) {
    f32 distance, angle;
    FieldTargetGuideState *state = &fldTargetGuideState;
    FieldGuidePoint *point;
    f32 originX = D_004363BC;
    f32 originY = D_004363BE;

    if (state->age < 300) {
        state->age++;
    }
    if (state->moveTimer <= 0 || --state->moveTimer <= 0) {
        if (state->routeIndex < state->routeEnd) {
            state->routeIndex++;
            state->moveTimer = 20;
        }
        point = state->route;
        if (point != NULL) {
            point += state->routeIndex;
            state->gridX = point->x;
            state->gridY = point->y;
            state->position[0] = originX + (f32)point->x * (f32)D_004363C0;
            state->position[2] = originY + (f32)point->y * (f32)-D_004363C2;
        }
        if (D_00438EF8.x == state->gridX && D_00438EF8.y == state->gridY) {
            state->routeRecord = NULL;
            state->route = NULL;
            state->routeIndex = 0;
            state->routeEnd = 0;
            state->moveTimer = 20;
            state->stepDistance = 0.0f;
        } else {
            fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_FAST_ROUTE);
        }
    } else {
        state->position[0] += sdfSin(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->position[2] += sdfCos(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->yaw = fldTurnToward(state->yaw, 180.0f - state->targetYaw, 10.0f, 1.0f);
    }
    fldCalcTargetDistanceYaw(&distance, &angle);
}

void fldUpdateTargetGuideSlowRoute(void) {
    f32 distance, angle;
    FieldTargetGuideState *state = &fldTargetGuideState;
    FieldGuidePoint *point;
    f32 originX = D_004363BC;
    f32 originY = D_004363BE;

    if (state->age < 300) {
        state->age++;
    }
    if (state->moveTimer <= 0 || --state->moveTimer <= 0) {
        if (state->routeIndex < state->routeEnd) {
            state->routeIndex++;
            state->moveTimer = 80;
        }
        point = state->route;
        if (point != NULL) {
            point += state->routeIndex;
            state->gridX = point->x;
            state->gridY = point->y;
            state->position[0] = originX + (f32)point->x * (f32)D_004363C0;
            state->position[2] = originY + (f32)point->y * (f32)-D_004363C2;
        }
        if (D_00438EF8.x == state->gridX && D_00438EF8.y == state->gridY) {
            state->routeRecord = NULL;
            state->route = NULL;
            state->routeIndex = 0;
            state->routeEnd = 0;
            state->moveTimer = 20;
            state->stepDistance = 0.0f;
        } else {
            fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        }
    } else {
        state->position[0] += sdfSin(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->position[2] += sdfCos(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->yaw = fldTurnToward(state->yaw, 180.0f - state->targetYaw, 15.0f, 1.0f);
    }
    fldCalcTargetDistanceYaw(&distance, &angle);
}


void fldTickTargetGuideAndNotify(void) {
    s32 count = fldTargetGuideState.moveTimer;
    s32 remaining = count - 1;
    if (count > 0) {
        fldTargetGuideState.moveTimer = remaining;
        count = remaining;
    }
    if (count == 0) {
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_WAIT_FOR_PLAYER);
    }
}


void fldUpdateTargetGuideCycleMotion(void) {
    f32 distance, angle;
    FieldTargetGuideState *state = &fldTargetGuideState;

    if (state->age < 300) {
        state->age++;
    }
    if (state->moveTimer <= 0 || --state->moveTimer <= 0) {
        if (state->routeIndex < state->routeEnd) {
            state->routeIndex++;
            state->moveTimer = 80;
        }
        if (state->cycleRoute[state->cycleIndex + 1].x == -1) {
            state->cycleIndex = 0;
        } else {
            state->cycleIndex++;
        }
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_CYCLE_ROUTE);
    } else {
        state->position[0] += sdfSin(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->position[2] += sdfCos(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->yaw = fldTurnToward(state->yaw, 180.0f - state->targetYaw, 15.0f, 1.0f);
    }
    fldCalcTargetDistanceYaw(&distance, &angle);
}

void func_00152390(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}

void func_001523B0(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}

void func_001523D0(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}


void fldUpdateTargetGuideMovementState(void) {
    FieldTargetGuideState *state = &fldTargetGuideState;
    SdfMathVector4 guidePosition;
    SdfMathVector4 areaPosition;
    f32 dx;
    f32 dz;
    f32 magnitude;
    f32 viewAngle;
    f32 guideX;
    f32 guideZ;
    f32 areaX;
    f32 areaZ;

    state->updateFlags = 0;
    fldCalcTargetDistanceYaw(&state->targetDistance, &state->targetBearing);

    dx = aViewTarget[0] - sdfViewTargetVector[0];
    dz = aViewTarget[2] - sdfViewTargetVector[2];
    magnitude = fsqrtf(dx * dx + dz * dz);
    viewAngle = 0.0f;
    if (!(magnitude < 1.0f)) {
        viewAngle = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    magnitude = fldAngleDifference(180.0f - viewAngle, state->yaw);
    if (magnitude < 0.0f) {
        magnitude = -magnitude;
    }
    guideX = state->position[0];
    guideZ = state->position[2];
    areaX = fldGlobalBase.x;
    areaZ = fldGlobalBase.z;
    state->viewAngleError = magnitude;

    guidePosition.x = guideX;
    guidePosition.y = 0.0f;
    guidePosition.z = guideZ;
    guidePosition.w = 1.0f;
    areaPosition.x = areaX;
    areaPosition.y = 0.0f;
    areaPosition.z = areaZ;
    areaPosition.w = 1.0f;

    if (state->motionMode == FLD_TARGET_GUIDE_MOTION_STOPPED) {
        return;
    }
    if (state->motionMode == FLD_TARGET_GUIDE_MOTION_WAIT_FOR_PLAYER) {
        if (state->moveTimer > 0) {
            return;
        }
        if (state->gridX != D_00438EF8.x || state->gridY != D_00438EF8.y) {
            fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        }
    }
    state = &fldTargetGuideState;
    if (state->motionMode == FLD_TARGET_GUIDE_MOTION_ARRIVAL_PAUSE) {
        if (state->moveTimer > 0) {
            return;
        }
    }
    if (state->motionMode == FLD_TARGET_GUIDE_MOTION_FAST_ROUTE) {
        if (state->gridX == D_00438EF8.x && state->gridY == D_00438EF8.y) {
            fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_ARRIVAL_PAUSE);
            state->updateFlags |= 0x80;
            return;
        }
    }
    state = &fldTargetGuideState;
    if (state->motionMode == FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE) {
        if (state->routeRecord != NULL && (state->routeRecord->flags & 1) != 0) {
            fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_FAST_ROUTE);
            state->updateFlags |= 0x80;
        }
        return;
    } else {
        return;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F20);

void fldDispatchTargetGuideMotionUpdate(void) {
    s32 motionMode = fldTargetGuideState.motionMode;

    switch (motionMode) {
    case FLD_TARGET_GUIDE_MOTION_STOPPED:
        func_00151DD8();
        return;
    case FLD_TARGET_GUIDE_MOTION_WAIT_FOR_PLAYER:
        fldTickTargetGuideCounter();
        return;
    case FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE:
        fldUpdateTargetGuideSlowRoute();
        return;
    case FLD_TARGET_GUIDE_MOTION_ARRIVAL_PAUSE:
        fldTickTargetGuideAndNotify();
        return;
    case FLD_TARGET_GUIDE_MOTION_CYCLE_ROUTE:
        fldUpdateTargetGuideCycleMotion();
        return;
    case FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_6:
        func_00152390();
        return;
    case FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_7:
        func_001523B0();
        return;
    case FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_8:
        func_001523D0();
        return;
    }
    fldUpdateTargetGuideFastRoute();
    /* Retire the mode after the fast-route phase; see docs/idioms.md,
     * "Post-call guide mode cleanup" for the late-deletion trace. */
    motionMode = FLD_TARGET_GUIDE_MOTION_STOPPED;
}

extern void fldSetTargetGuideMode(s32 mode);
extern s32 fldCanUseTargetGuideInCurrentScene(s32 area, s32 room);
extern FieldGridCoordPair D_003AA530[];
extern FieldGridCoordPair D_003AA558[];
extern FieldGridCoordPair D_003AA620[];
extern FieldGridCoordPair D_003AA6A8[10];
extern FieldGridCoordPair D_003AA6D0[10];
extern FieldGridCoordPair D_003AA6F8[10];

void func_001526B8(void) {
    FieldTargetGuideState *state;
    FieldGridCoordPair *point;
    u32 index;
    f32 playerX;
    f32 playerZ;
    if (fldCanUseTargetGuideInCurrentScene(fldGlobalBase.area, fldGlobalBase.floor + 1) == 0) {
        return;
    }
    D_004363C0 = 0x190;
    D_004363C2 = 0x190;
    switch (fldGlobalBase.floor + 1) {
    case 11: {
        FieldTargetGuideState *state;
        fldLoadTargetGuideResource();
        playerZ = fldGlobalBase.z;
        playerX = fldGlobalBase.x;
        D_004363BC = -0x960;
        D_004363BE = 0xAF0;
        state = &fldTargetGuideState;
        state->gridX = 0;
        state->gridY = 0;
        fldFindNearestOccupiedMapGridCell(playerX, playerZ, &D_00438EF8.x, &D_00438EF8.y);
        if (evtBitCheck(0x4D0) != 0) {
            u16 encodedX = (u16)D_003AA530[0].x;
            u16 encodedY = (u16)D_003AA530[0].y;
            state->cycleRoute = D_003AA530;
            state->gridX = encodedX;
            state->gridY = encodedY;
            state->position[0] = (f32)D_004363BC + (f32)(s16)encodedX * (f32)D_004363C0;
            state->position[2] = (f32)D_004363BE + (f32)(s16)encodedY * (f32)-D_004363C2;
            fldSetTargetGuideMode(1);
            return;
        }
        for (index = 0; index < 5; index++) {
            point = &D_003AA6A8[index * 2];
            if (point[0].x == D_00438EF8.x && point[0].y == D_00438EF8.y) {
                fldTargetGuideState.gridX = point[1].x;
                fldTargetGuideState.gridY = point[1].y;
            }
        }
        fldSetTargetGuideMode(0);
        break;
    }
    case 14: {
        FieldTargetGuideState *state;
        fldLoadTargetGuideResource();
        playerZ = fldGlobalBase.z;
        playerX = fldGlobalBase.x;
        D_004363BC = -0x960;
        D_004363BE = 0x960;
        state = &fldTargetGuideState;
        state->gridX = 0;
        state->gridY = 0;
        fldFindNearestOccupiedMapGridCell(playerX, playerZ, &D_00438EF8.x, &D_00438EF8.y);
        if (evtBitCheck(0x4D0) != 0) {
            u16 encodedX = (u16)D_003AA558[0].x;
            u16 encodedY = (u16)D_003AA558[0].y;
            state->cycleRoute = D_003AA558;
            state->gridX = encodedX;
            state->gridY = encodedY;
            state->position[0] = (f32)D_004363BC + (f32)(s16)encodedX * (f32)D_004363C0;
            state->position[2] = (f32)D_004363BE + (f32)(s16)encodedY * (f32)-D_004363C2;
            fldSetTargetGuideMode(1);
            return;
        }
        for (index = 0; index < 5; index++) {
            point = &D_003AA6D0[index * 2];
            if (point[0].x == D_00438EF8.x && point[0].y == D_00438EF8.y) {
                fldTargetGuideState.gridX = point[1].x;
                fldTargetGuideState.gridY = point[1].y;
            }
        }
        fldSetTargetGuideMode(0);
        break;
    }
    case 16: {
        FieldTargetGuideState *state;
        fldLoadTargetGuideResource();
        playerZ = fldGlobalBase.z;
        playerX = fldGlobalBase.x;
        D_004363BC = -0x960;
        D_004363BE = 0x960;
        state = &fldTargetGuideState;
        state->gridX = 0;
        state->gridY = 0;
        fldFindNearestOccupiedMapGridCell(playerX, playerZ, &D_00438EF8.x, &D_00438EF8.y);
        if (evtBitCheck(0x4D0) != 0) {
            u16 encodedX = (u16)D_003AA620[0].x;
            u16 encodedY = (u16)D_003AA620[0].y;
            state->cycleRoute = D_003AA620;
            state->gridX = encodedX;
            state->gridY = encodedY;
            state->position[0] = (f32)D_004363BC + (f32)(s16)encodedX * (f32)D_004363C0;
            state->position[2] = (f32)D_004363BE + (f32)(s16)encodedY * (f32)-D_004363C2;
            fldSetTargetGuideMode(1);
            break;
        }
        for (index = 0; index < 5; index++) {
            point = &D_003AA6F8[index * 2];
            if (point[0].x == D_00438EF8.x && point[0].y == D_00438EF8.y) {
                fldTargetGuideState.gridX = point[1].x;
                fldTargetGuideState.gridY = point[1].y;
            }
        }
        fldSetTargetGuideMode(0);
        break;
    }
    case 18:
        fldSetTargetGuideMode(14);
        break;
    }
    state = &fldTargetGuideState;
    state->moveTimer = -20;
    state->routeIndex = 0;
    state->routeEnd = 0;
    state->position[1] = 0.0f;
    state->targetYaw = 0.0f;
    state->stepDistance = 0.0f;
    state->updateFlags = 0;
    state->position[0] = (f32)D_004363BC + (f32)D_004363C0 * (f32)state->gridX;
    state->position[2] = (f32)D_004363BE + (f32)-D_004363C2 * (f32)state->gridY;
    if (state->guideMode != 14) {
        state->guideMode = 0;
    }
    state->viewAngleError = 0.0f;
    state->unk2A = 0;
    state->disabled = 0;
    fldUpdateTargetGuideEffects();
}

extern void fldFreeRunMapData(void);

extern void fldResetViewState(void);

extern void p2sdStopSe(u32);

void fldResetTargetViewAndSound(void) {
    fldFreeRunMapData();
    fldResetViewState();
    p2sdStopSe(0x690061);
}

extern void fldCalcTargetDistanceYaw(f32 *, f32 *);

void fldUpdateViewAngle(void) {
    f32 distance;
    f32 angle;
    FieldTargetGuideState *state;

    fldCalcTargetDistanceYaw(&distance, &angle);
    state = &fldTargetGuideState;
    state->targetYaw = angle;
    state->yaw = 180.0f - angle;
}

extern const f32 D_00413F68[4];
extern f32 D_00451D4C[];
extern void dds3OnPoseSetFlag(dds3PoseSet_t *inner, u32 flags);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *inner, u32 flags);

void fldUpdateTargetGuideEffects(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 smoothedPosition[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    dds3ModelHandle_t *model = D_00435F20;
    s32 animation;
    s32 changeAnimation;
    f32 angle;
    f32 previousFrame;
    f32 currentFrame;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(smoothedPosition, 0, sizeof(smoothedPosition));
    smoothedPosition[3] = 1.0f;
    memcpy(axis, D_00413F68, sizeof(axis));
    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;

    position[0] = fldTargetGuideState.position[0];
    position[1] = 0.0f;
    position[2] = fldTargetGuideState.position[2];
    dds3SetPosPoseSet(D_00435F1C, (u128 *)position);

    SDF_VU0_LOADVECTOR(axis);
    angle = (D_00451D4C[0] * 3.14f) / 180.0f;
    FromRotation(angle);
    /* SDK store: the quaternion is consumed by the following vector setter. */
    SDF_VU0_STOREVECTOR_NOMEM(rotation);
    dds3SetRotPoseSet(D_00435F1C, (u128 *)rotation);

    if (fldTargetGuideState.updateFlags & 0x40) {
        dds3OnPoseSetFlag(D_00435F1C->inner, 8);
        PCP_COPY_VECTOR_F32(smoothedPosition, D_00435F1C->inner->smoothedPosition);
        if (fabsf(smoothedPosition[0] - position[0]) < 1.0f) {
            D_00435F1C->inner->smoothedPosition[0] = position[0];
        } else {
            D_00435F1C->inner->smoothedPosition[0] =
                smoothedPosition[0] + (position[0] - smoothedPosition[0]) / 3.0f;
        }
        if (fabsf(smoothedPosition[1] - position[1]) < 1.0f) {
            D_00435F1C->inner->smoothedPosition[1] = position[1];
        } else {
            D_00435F1C->inner->smoothedPosition[1] =
                smoothedPosition[1] + (position[1] - smoothedPosition[1]) / 3.0f;
        }
        if (fabsf(smoothedPosition[2] - position[2]) < 1.0f) {
            D_00435F1C->inner->smoothedPosition[2] = position[2];
        } else {
            D_00435F1C->inner->smoothedPosition[2] =
                smoothedPosition[2] + (position[2] - smoothedPosition[2]) / 3.0f;
        }
        D_00435F1C->inner->smoothedPosition[3] = 1.0f;
    } else {
        dds3OffPoseSetFlag(D_00435F1C->inner, 8);
    }

    if (fldTargetGuideState.updateFlags & 0x20) {
        mdlLoopMotion(model, 0, fldTargetGuideState.guideMotionIndex);
    } else if (fldTargetGuideState.updateFlags & 2) {
        animation = fldTargetGuideState.guideMotionIndex;
        /* Only zero and the 16..18 family suppress a repeated animation. */
        changeAnimation = 1;
        switch (animation) {
        case 0:
        case 16:
        case 17:
        case 18:
            changeAnimation = animation != (s32)model->current.h.motionIndex;
            break;
        }
        if (changeAnimation) {
            model->first->frameStep = 1.0f;
            mdlSmoothLoopMotion(model, 0, fldTargetGuideState.guideMotionIndex, 6.0f, 6.0f);
        }
    }

    previousFrame = fldTargetGuideState.currentModelFrame;
    currentFrame = model->first->currentFrame;
    fldTargetGuideState.currentModelFrame = currentFrame;
    fldTargetGuideState.previousModelFrame = previousFrame;
    if (fldTargetGuideState.updateFlags & 2) {
        fldTargetGuideState.previousModelFrame = currentFrame;
    }
}

static inline s32 scaleToVolume(s32 dist, s32 max, s32 range) {
    return (max - dist) * 127 / range;
}

s32 fldCalcDistanceVolume(f32 x, f32 y, f32 z) {
    f32 dx = fldCamTarget[0] - x;
    f32 dy = fldCamTarget[1] - y;
    f32 dz = fldCamTarget[2] - z;
    f32 dist = fsqrtf(dx * dx + dy * dy + dz * dz) - 600.0f;
    s32 volume;
    if (4800.0f < dist) {
        dist = 4800.0f;
    }
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    volume = scaleToVolume((s32)dist, 4800, 4800);
    if (volume > 127) {
        volume = 127;
    }
    if (volume < 0) {
        volume = 0;
    }
    return volume;
}

#include "kwln_fade_timing.h"
extern s32 D_004363D8;
extern char D_00413F78[];
void func_00153068(void) {
    f32 previousFrame;
    f32 currentFrame;
    s32 playStep;
    s32 percent;
    s32 volume;
    s32 skipFade;

    playStep = 0;
    percent = 0;
    skipFade = 0;
    if (fldTargetGuideState.guideMode == 0xE) {
        volume = 0x6F;
        previousFrame = fldTargetGuideState.currentModelFrame;
        currentFrame = D_00435F20->first->currentFrame;
        fldTargetGuideState.guideMotionIndex = D_00435F20->current.h.motionIndex;
        fldTargetGuideState.currentModelFrame = currentFrame;
        fldTargetGuideState.previousModelFrame = previousFrame;
        fldTargetGuideState.targetDistance = 1300.0f;
    } else {
        volume = fldCalcDistanceVolume(fldTargetGuideState.position[0], fldTargetGuideState.position[1], fldTargetGuideState.position[2]);
    }
    currentFrame = fldTargetGuideState.currentModelFrame;
    previousFrame = fldTargetGuideState.previousModelFrame;
    switch (fldTargetGuideState.guideMotionIndex) {
    case 17:
        if (previousFrame < 2.0f && currentFrame >= 2.0f) {
            playStep = 1;
        }
        if (previousFrame < 12.0f && currentFrame >= 12.0f) {
            playStep = 1;
        }
        percent = 80;
        break;
    case 16:
        if (previousFrame < 23.0f && currentFrame >= 23.0f) {
            playStep = 1;
        }
        if (previousFrame < 49.0f && currentFrame >= 49.0f) {
            playStep = 1;
        }
        if (previousFrame < 72.0f && currentFrame >= 72.0f) {
            playStep = 1;
        }
        if (previousFrame < 98.0f && currentFrame >= 98.0f) {
            playStep = 1;
        }
        percent = 40;
        break;
    case 18:
        if (previousFrame < 1.0f && currentFrame >= 1.0f) {
            playStep = 1;
            skipFade = 1;
        }
        if (previousFrame < 23.0f && currentFrame >= 23.0f) {
            playStep = 1;
        }
        break;
    }
    if (playStep != 0) {
        p2sdPlaySE(D_004363D8 + 0x690010, volume, 0x3F);
        D_004363D8 = (D_004363D8 + (s32)((dds3GetRandA(NULL) >> 4) & 1) + 1) % 4;
        if ((skipFade == 0) && !(fldTargetGuideState.targetDistance > 2000.0f)) {
            if (fldTargetGuideState.targetDistance > 1600.0f) {
                dds3SetQuake(4, 0x14);
            } else if (fldTargetGuideState.targetDistance > 1200.0f) {
                dds3SetQuake(5, 0x28);
            } else if (fldTargetGuideState.targetDistance > 800.0f) {
                dds3SetQuake(6, 0x3C);
            } else {
                dds3SetQuake(7, 0x46);
            }
        }
    }
    if (fldTargetGuideState.updateFlags & 0x80) {
        p2sdPlaySE(0x690060, volume, 0x3F);
    }
    /* The scaled volume is never used; retail keeps only the divide-by-zero check. */
    if (percent > 0) {
        volume = (volume * percent) / 100;
        printf(D_00413F78);
    } else {
        printf(D_00413F78);
    }
}

extern char D_00413F78[];

extern s32 D_00451D3C[];

s32 fldReportCampVolumeError(void) {
    if (D_00451D3C[0] != 1) {
        if (fldProcCampChk() != 0) {
            printf(D_00413F78);
        }
    }
}

void fldUpdateTargetGuideController(void) {
    if (fldTargetGuideState.disabled != 1) {
        if (fldGlobalBase.targetGuideActive == 0 && fldEncountTestFlag(0x40) == 0) {
            if (fldTargetGuideState.guideMode == 14) {
                func_00153068();
                return;
            }
            if (fldProcAmChk() != 0) {
                printf(D_00413F78);
                return;
            }
        } else {
            fldUpdateGuideMarkerPulse();
            fldUpdateTargetGuideMovementState();
            fldDispatchTargetGuideMotionUpdate();
            fldUpdateTargetGuideEffects();
            func_00153068();
            fldFindNearestOccupiedMapGridCell(fldGlobalBase.x, fldGlobalBase.z, &D_00438EF8.x, &D_00438EF8.y);
            fldTargetGuideState.previousGridX = fldTargetGuideState.gridX;
            fldTargetGuideState.previousGridY = fldTargetGuideState.gridY;
        }
    }
}

s32 fldIsTargetWithinInteractionRange(void) {
    f32 distance;
    f32 angle;
    s32 inRange = 1;
    fldCalcTargetDistanceYaw(&distance, &angle);
    if (!(distance < 200.0f)) {
        inRange = 0;
    }
    return inRange;
}

s32 fldCanUseTargetGuideInCurrentScene(s32 unusedArea, s32 unusedRoom) {
    if (fldGlobalBase.area == 23) {
        if (fldGlobalBase.floor == 10 && evtBitCheck(1254)) {
            return 0;
        }
        if (fldGlobalBase.floor == 13 && evtBitCheck(1255)) {
            return 0;
        }
        if (fldGlobalBase.floor == 15 && evtBitCheck(9)) {
            return 0;
        }
        if (fldGlobalBase.floor == 17 && !evtBitCheck(1232)) {
            return 0;
        }
        if (fldGlobalBase.floor == 17 && evtBitCheck(9)) {
            return 0;
        }
    }
    if (evtBitCheck(1222) && fldGlobalBase.area == 23 &&
        fldGlobalBase.floor == 10) {
        return 1;
    }
    if (fldGlobalBase.area == 23) {
        if (fldGlobalBase.floor == 13) {
            return 1;
        }
        if (fldGlobalBase.floor == 15) {
            return 1;
        }
        if (fldGlobalBase.floor == 17) {
            return 1;
        }
    }
    return 0;
}

extern FieldGridCoordPair D_003AA6A8[10];
extern FieldGridCoordPair D_003AA6D0[10];
extern FieldGridCoordPair D_003AA6F8[10];

/* Select the target guide's route endpoint, motion state and model visibility. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F78);

void fldSetTargetGuideMode(s32 mode) {
    s32 previousGuideMode;

    if (fldCanUseTargetGuideInCurrentScene(fldGlobalBase.area, fldGlobalBase.floor + 1) == 0) {
        return;
    }
    previousGuideMode = fldTargetGuideState.guideMode;
    fldTargetGuideState.updateFlags = 0x40;
    fldTargetGuideState.guideMode = mode;

    switch (mode) {
    case 0:
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        fldUpdateTargetGuideEffects();
        p2sdStopSe(0x690061);
        p2sdPlaySE(0x690061, 0, 0x3F);
        break;
    case 1:
        fldUpdateViewAngle();
        fldTargetGuideState.cycleIndex = 0;
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_CYCLE_ROUTE);
        fldUpdateTargetGuideEffects();
        break;
    case 2:
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 0.0f;
        fldTargetGuideState.yaw = 180.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_6);
        fldUpdateTargetGuideEffects();
        break;
    case 3:
        fldTargetGuideState.gridX = D_003AA6A8[1].x;
        fldTargetGuideState.gridY = D_003AA6A8[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        p2sdStopSe(0x690061);
        p2sdPlaySE(0x690061, 0, 0x3F);
        break;
    case 4:
        fldTargetGuideState.gridX = D_003AA6A8[3].x;
        fldTargetGuideState.gridY = D_003AA6A8[3].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 90.0f;
        fldTargetGuideState.yaw = 180.0f - fldTargetGuideState.targetYaw;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        p2sdStopSe(0x690061);
        p2sdPlaySE(0x690061, 0, 0x3F);
        break;
    case 5:
        fldTargetGuideState.gridX = D_003AA6A8[5].x;
        fldTargetGuideState.gridY = D_003AA6A8[5].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        p2sdStopSe(0x690061);
        p2sdPlaySE(0x690061, 0, 0x3F);
        break;
    case 6:
        fldTargetGuideState.gridX = D_003AA6D0[1].x;
        fldTargetGuideState.gridY = D_003AA6D0[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        D_00435F20->flags &= ~1;
        p2sdStopSe(0x690061);
        p2sdPlaySE(0x690061, 0, 0x3F);
        break;
    case 7:
        fldTargetGuideState.gridX = D_003AA6D0[3].x;
        fldTargetGuideState.gridY = D_003AA6D0[3].y;
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_6);
        fldUpdateTargetGuideEffects();
        p2sdStopSe(0x690061);
        break;
    case 8:
        if (previousGuideMode == 7) {
            fldTargetGuideState.gridX = D_003AA6D0[3].x;
            fldTargetGuideState.gridY = D_003AA6D0[3].y;
            fldTargetGuideState.targetYaw = 180.0f;
            fldTargetGuideState.yaw = 0.0f;
            fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
            fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_FAST_ROUTE);
            fldTargetGuideState.updateFlags |= 2;
            fldUpdateTargetGuideEffects();
            p2sdStopSe(0x690061);
            p2sdPlaySE(0x690061, 0, 0x3F);
        }
        break;
    case 9:
        fldTargetGuideState.gridX = D_003AA6F8[1].x;
        fldTargetGuideState.gridY = D_003AA6F8[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        D_00435F20->flags &= ~1;
        p2sdStopSe(0x690061);
        p2sdPlaySE(0x690061, 0, 0x3F);
        break;
    case 10:
        fldTargetGuideState.gridX = D_003AA6F8[3].x;
        fldTargetGuideState.gridY = D_003AA6F8[3].y;
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_6);
        fldUpdateTargetGuideEffects();
        p2sdStopSe(0x690061);
        break;
    case 11:
        if (previousGuideMode == 10) {
            fldTargetGuideState.gridX = D_003AA6F8[3].x;
            fldTargetGuideState.gridY = D_003AA6F8[3].y;
            fldTargetGuideState.yaw = 180.0f;
            fldTargetGuideState.targetYaw = 0.0f;
            fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
            fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_FAST_ROUTE);
            fldTargetGuideState.updateFlags |= 2;
            fldUpdateTargetGuideEffects();
            p2sdStopSe(0x690061);
            p2sdPlaySE(0x690061, 0, 0x3F);
        }
        break;
    case 12:
        fldTargetGuideState.gridX = D_003AA6D0[1].x;
        fldTargetGuideState.gridY = D_003AA6D0[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_7);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        D_00435F20->flags |= 1;
        p2sdStopSe(0x690061);
        break;
    case 13:
        fldTargetGuideState.gridX = D_003AA6F8[1].x;
        fldTargetGuideState.gridY = D_003AA6F8[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_7);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        D_00435F20->flags |= 1;
        p2sdStopSe(0x690061);
        break;
    case 14:
        fldTargetGuideState.gridX = 0;
        fldTargetGuideState.gridY = 0;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 0.0f;
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.position[0] = fldTargetGuideState.targetYaw;
        fldTargetGuideState.position[2] = fldTargetGuideState.targetYaw;
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_TIMED_IDLE_8);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        D_00435F20->flags |= 1;
        break;
    case 15:
        fldTargetGuideState.gridX = D_003AA6F8[5].x;
        fldTargetGuideState.gridY = D_003AA6F8[5].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapIndexToPos(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        fldSetTargetGuideMotionMode(FLD_TARGET_GUIDE_MOTION_SLOW_ROUTE);
        fldTargetGuideState.updateFlags |= 2;
        fldUpdateTargetGuideEffects();
        p2sdStopSe(0x690061);
        p2sdPlaySE(0x690061, 0, 0x3F);
        break;
    }
}


void fldSetTargetGuideEnabled(s32 active) {
    if (active == 0) {
        D_00451D3C[0] = 1;
        return;
    }
    D_00451D3C[0] = 0;
}

/* Five-word rows are shared by the object producer and the guide update. */
typedef struct FieldGuideObjectSlot {
    dds3Basic_t *object;
    dds3ModelHandle_t *model;
    u32 active;
    s16 gridX;
    s16 gridY;
    f32 progress;
} FieldGuideObjectSlot;

typedef char FieldGuideObjectSlot_size_must_be_0x14[
    (sizeof(FieldGuideObjectSlot) == 0x14) ? 1 : -1];

extern FieldGuideObjectSlot D_00451DB0[15];

void fldCreateGrippedUnitMarker(s32 gridX, s32 gridY, s32 slot) {
    f32 position[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    f32 stepX;
    f32 stepY;
    s16 stepScaleX;
    s16 stepScaleY;
    dds3Basic_t *object;
    dds3ModelHandle_t *model;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;
    {
        f32 scale[4] __attribute__((aligned(16))) = {1.2f, 1.2f, 1.2f, 1.0f};
        stepX = (f32)D_004363BC;
        stepY = (f32)D_004363BE;
        object = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position,
                                        (u128 *)rotation);
        dds3SetUnitObjectId(object, "TUKAMI_UNIT");
        dds3SetActiveUnitSceneBasic(dds3GetControlScene(), object);
        dds3RebuildObjectModelContext(object, 2, 0x10);
        dds3GetRadiusPoseSet(object, 180.0f);

        stepScaleX = D_004363C0;
        stepScaleY = D_004363C2;
        stepX += (f32)stepScaleX * (f32)gridX;
        stepY += (f32)-stepScaleY * (f32)gridY;
        position[0] = stepX;
        position[1] = 0.0f;
        position[2] = stepY;
        scale[0] = 1.0f;
        scale[1] = 0.5f;
        scale[2] = 1.0f;
        PCP_COPY_VECTOR_F32(object->inner->position, position);
        PCP_COPY_VECTOR_F32(object->inner->smoothedPosition, position);
        PCP_COPY_VECTOR_F32(object->inner->rotation, rotation);
        dds3SetPosPoseSet(object, (u128 *)position);
        dds3SetRotPoseSet(object, (u128 *)rotation);
        dds3SetSclPoseSet(object, (u128 *)scale);

        model = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(object);
        mdlLoopMotion(model, 0, 0);
        sdfModelSetFaceDivParm(model->inner, 15.0f, 0.0f);
        D_00451DB0[slot].model = model;
        D_00451DB0[slot].object = object;
    }
}

typedef struct FieldGuideObjectPreset {
    FieldGridCoordPair coordinate;
    f32 phase;
} FieldGuideObjectPreset;
extern FieldGuideObjectPreset D_003AA458[];
extern FieldGuideObjectPreset D_003AA460[];
extern FieldGuideObjectPreset D_003AA4B8[];

void fldInitializeSceneGuideMarkers(void) {
    FieldGuideObjectPreset *presets = NULL;
    s32 slot;
    s32 packedCoordinates;

    for (slot = 0; slot < 15; slot++) {
        D_00451DB0[slot].active = 0;
    }
    if (fldGlobalBase.area == 23 && fldGlobalBase.floor == 13 && evtBitCheck(0x4C9) == 0) {
        return;
    }
    if (fldGlobalBase.area == 23) {
        if (fldGlobalBase.floor == 10) {
            presets = D_003AA458;
        }
        if (fldGlobalBase.floor == 13) {
            presets = D_003AA460;
        }
        if (fldGlobalBase.floor == 15) {
            presets = D_003AA4B8;
        }
    }
    if (presets != NULL) {
        for (slot = 0; slot < 15; slot++) {
            /* The terminal record encodes both coordinates as 0xFFFF. */
            memcpy(&packedCoordinates, &presets[slot].coordinate, sizeof(packedCoordinates));
            if (packedCoordinates == -1) {
                return;
            }
            D_00451DB0[slot].active = 1;
            D_00451DB0[slot].gridX = presets[slot].coordinate.x;
            D_00451DB0[slot].gridY = presets[slot].coordinate.y;
            fldCreateGrippedUnitMarker(D_00451DB0[slot].gridX, D_00451DB0[slot].gridY, slot);
            D_00451DB0[slot].progress = presets[slot].phase;
        }
    }
}


void fldCreateChaseCameraObject(void) {
    f32 position[4] __attribute__((aligned(16))) = {0, 0, 0, 1.0f};
    f32 rotation[4] __attribute__((aligned(16))) = {0, 0, 0, 1.0f};
    f32 axis[4] __attribute__((aligned(16))) = {0, 1.0f, 0, 1.0f};
    f32 scale[4] __attribute__((aligned(16))) = {1.2f, 1.2f, 1.2f, 1.0f};
    dds3PoseSet_t *inner;
    dds3Basic_t *object;

    SDF_VU0_LOADVECTOR(axis);
    FromRotation(3.14159265f);
    SDF_VU0_LOADVECTOR_B(rotation);
    effMiscQuatMultiplyVU();
    /* The SDK store is followed by opaque consumers, with no scalar readback. */
    SDF_VU0_STOREVECTOR_NOMEM(rotation);
    D_00435F1C = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)position, (u128 *)rotation);
    dds3SetUnitObjectId(D_00435F1C, "OIKAKE_UNIT");
    dds3SetActiveUnitSceneBasic(dds3GetControlScene(), D_00435F1C);
    dds3RebuildObjectModelContext(D_00435F1C, 1, 0x103);
    dds3GetRadiusPoseSet(D_00435F1C, 180.0f);
    dds3ObjectBaseOnFlag(D_00435F1C, 0x400);
    position[0] = 800.0f;
    position[1] = 0.0f;
    position[2] = 800.0f;
    object = D_00435F1C;
    inner = object->inner;
    PCP_COPY_VECTOR_F32(inner->position, position);
    PCP_COPY_VECTOR_F32(inner->smoothedPosition, position);
    PCP_COPY_VECTOR_F32(inner->rotation, rotation);
    dds3SetPosPoseSet(object, (u128 *)position);
    dds3SetRotPoseSet(D_00435F1C, (u128 *)rotation);
    dds3SetSclPoseSet(D_00435F1C, (u128 *)scale);
    D_00435F20 = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(D_00435F1C);
    mdlLoopMotion(D_00435F20, 0, 0x11);
    sdfModelSetFaceDivParm(D_00435F20->inner, 15.0f, 0.0f);
    fldApplyLightSetToChaseCameraUnit();
    func_001526B8();
    fldInitializeSceneGuideMarkers();
}

extern s32 D_004363DC;
extern f32 D_004363E0, D_004363E4;

void fldUpdateGuideMarkerPulse(void) {
    FieldGuideObjectSlot *marker;
    s32 i;
    s32 visible = 0;
    f32 elapsed;
    f32 factor;

    for (i = 0; i < 15; i++) {
        if (D_00451DB0[i].active) {
            D_00451DB0[i].progress += 1.0f;
            if (D_00451DB0[i].progress > 150.0f) {
                D_00451DB0[i].progress = 0.0f;
            }
            if (D_00451DB0[i].progress > 0.0f && D_00451DB0[i].progress < 15.5f) {
                factor = D_00451DB0[i].progress - 0.0f;
                factor *= 80.0f;
                factor /= 15.0f;
                mdlSetColor(D_00451DB0[i].model,
                    ((s32)factor << 24) | 0x808080);
            } else if (D_00451DB0[i].progress > 15.0f &&
                       D_00451DB0[i].progress < 75.5f) {
                mdlSetColor(D_00451DB0[i].model, 0x80808080);
            } else if ((elapsed = D_00451DB0[i].progress) > 75.0f &&
                       elapsed < 90.5f) {
                elapsed -= 75.0f;
                factor = 15.0f - elapsed;
                factor *= 80.0f;
                factor /= 15.0f;
                mdlSetColor(D_00451DB0[i].model,
                    ((s32)factor << 24) | 0x808080);
            } else {
                mdlSetColor(D_00451DB0[i].model, 0x808080);
            }
        }
    }
    for (i = 0; i < 15; i++) {
        marker = &D_00451DB0[i];
        if (marker->active && marker->gridX == D_00438EF8.x &&
            marker->gridY == D_00438EF8.y && marker->progress > 15.0f &&
            marker->progress < 75.0f) {
            visible = 1;
            fldMapIndexToPos(marker->gridX, marker->gridY,
                                      &D_004363E0, &D_004363E4);
            break;
        }
    }
    D_004363DC = visible;
}

extern s32 D_003898AC[];

extern s32 D_004363DC;

s32 fldIsCurrentGridGuideMarkerActive(void) {
    if (D_003898AC[0] != 0) {
        return D_004363DC;
    }
    return 0;
}

extern f32 D_004363E0;

extern f32 D_004363E4;

void fldGetFieldFloatPair(f32 *x, f32 *y) {
    *x = D_004363E0;
    *y = D_004363E4;
}
INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436208);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSeLastBgm);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneSoundBase);

INCLUDE_SDATA(const s32, "game/code_001442D0", gFldSe3DNum);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436220);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436224);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436228);

INCLUDE_SDATA(const s32, "game/code_001442D0", MidiNo);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldBgmReloadStep);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldArchiveLoadPending);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldFixedArchiveLoadPhase);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043623C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436240);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAutoMapField);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436248);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043624C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436250);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436254);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436258);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043625C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436260);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436264);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436268);

INCLUDE_SDATA(const s32, "game/code_001442D0", AutoMapSeq);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAutoMapHeader);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAutoMapData);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAutoMapBuf);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043627C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436280);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436284);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436288);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043628C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436290);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436294);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436298);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043629C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldNpcModelTable);

INCLUDE_SDATA(struct EffNode * const, "game/code_001442D0", fldAreaDamageEffect);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAreaDamageEffectPlaced);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E4);

INCLUDE_SDATA(EffNode *, "game/code_001442D0", D_004362E8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362EC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F0);

INCLUDE_SDATA(EffNode *, "game/code_001442D0", D_004362F4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362FC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436300);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436304);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436308);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043630C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436310);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436314);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436318);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043631C);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldTakaraCount);

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_001442D0", fldIndexedResourceHandle);

INCLUDE_SDATA(EffNodeDescriptor *, "game/code_001442D0", fldIndexedResourceData);

INCLUDE_SDATA(EffNode *, "game/code_001442D0", fldIndexedResourceEffect);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436330);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436338);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043633C);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldPrimaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSecondaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001442D0", mnuPositionedResourceCursor);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043634C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436350);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436354);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436358);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043635C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436360);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436364);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436368);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043636C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436370);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436374);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436378);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043637C);

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_001442D0", D_00436380);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436384);

INCLUDE_SDATA(EffNode *, "game/code_001442D0", fldDamEffectNode);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldDamEffectPositioned);

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_001442D0", D_00436390);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436394);

INCLUDE_SDATA(EffNode *, "game/code_001442D0", fldYukEffectNode);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldYukEffectPositioned);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BE);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C2);

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_001442D0", D_004363C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363CC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E4);
