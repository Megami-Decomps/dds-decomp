#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "fld_item_box_object_api.h"
#include "fld_projected_sprite.h"
#include "sdf_model_packet_callback.h"
#include "sdf_model_release_api.h"
#include "dsp_entry_api.h"
#include "dsp_channel.h"
#include "evt_message_window_api.h"
#include "fld_area_work.h"
#include "fld_automap_entry_discovery_state.h"
#include "fld_label_row.h"
#include "fld_coordinate_lookup_api.h"
#include "fld_room_effect_entry.h"
#include "fld_object_slot.h"
#include "fld_activation_entry.h"
#include "fld_npc_palette.h"
#include "file_pac.h"
#include "evt_world.h"
#include "common.h"
#include "fld_scene_selection_index.h"
#include "fld_spatial_sound_id.h"
#include "pty_status_query_api.h"
#include "fld_camera_model_motion_api.h"
#include "sdf_delayed_resource_check_api.h"
#include "fld_archive_load_state.h"
#include "dds3_object_flags.h"
#include "sdf_vu0.h"
#include "snd_track_playback_api.h"
#include "fld_scene_ready_api.h"
#include "fld_camp_check_state_api.h"
#include "fld_scene_transition_gate_api.h"
#include "fld_clear_menu_entries_api.h"
#include "fld_play_pending_sounds_api.h"
#include "fld_scene_record_api.h"
#include "fld_object_slot_api.h"
#include "fld_release_texture_slots_api.h"
#include "fld_spark_object_entry.h"
#include "fld_spark_slot.h"
#include "fld_spark_controller_state.h"
#include "sdf_math_vector4.h"
#include "sdf_vu_matrix.h"
#include "kwln_fade.h"
#include "mnu_title_stream.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_textured_rect.h"
#include "sdf_motion.h"
#include "mdl.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_value.h"
#include "sdf_dev_state.h"
#include "fld_transfer_chunk.h"
#include "sdf_resource.h"
#include "eff_transform.h"
#include "sdf_model.h"
#include "fld_scene_record.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_create_node_from_parameter.h"
#include "eff_node_descriptor.h"
#include "kwln.h"
#include "kwln_task_runner.h"
#include "kwln_task_create.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "fr_font.h"
#include "itf.h"
#include "btl.h"
#include "evt_unit_color_transition.h"
#include "evt_unit.h"
/* Declared before first use (were implicit int). */
extern void dds3WorkClear(void);
extern void sdfDrawSync(void);
extern void p2sdSendStopCmd(u32 controlWord);

extern void p2sdStopAllBgm(void);
extern u8 dds3PadButtonState[2][2][16];
extern u8 D_003BA878[2][2];
extern f32 aViewTarget[4];
extern f32 sdfViewTargetVector[4];
extern f32 aViewDown[4];
extern void fldAutoMapGetBBox(f32 *, f32 *, f32 *, f32 *);






extern char fldTitleTaskName[]; /* "fldTitle" */

extern char D_003A0828[]; /* "fldTitleMini" */

extern struct sdfMemHandle_t *fldDamEffectResource;

extern u32 fldDamEffectData;

extern EffNode *fldDamEffectNode;

extern u32 fldDamEffectPositioned;

extern struct sdfMemHandle_t *fldYukEffectResource;

extern u32 fldYukEffectData;

extern EffNode *fldYukEffectNode;

extern u32 fldYukEffectPositioned;

extern sdfTexHandle_t *fldWeatherLimitTexture;

extern s32 D_003BAFBC;

extern sdfTexHandle_t *D_003BAFC4;

extern s32 D_003BAFB8;

extern s32 D_003BAFC0;

extern sdfTexHandle_t *D_003BAFB4;

extern s32 D_003BAFA0;

extern s32 D_003BAFA8;
extern s32 D_003BAF9C;
extern s32 D_003BAFAC;

extern u32 D_003BAFB0;


extern u32 AutoMapSeq;

extern FldArchiveLoadPhase fldFixedArchiveLoadPhase;

extern u32 fldArchiveLoadPending;

extern u32 fldSeLastBgm;

extern dds3ProcessID_t *addressprocess_id;

extern s32 gFldSe3DNum;

extern s32 fldAutoMapField;

extern s32 D_003BAEB4;



extern s32 fldPrimaryEffectPositionPending;

extern s32 fldSecondaryEffectPositionPending;

extern s32 D_0032E474[];

extern s32 D_0033EB78[];

extern void p2sdStopBGM(u32 trackHandle);




extern void fldPreparePlayerSceneCameraTarget(void);

extern void fldReleaseCameraModel(u32 enabled);

extern void evtSetSolarOverlayFullyVisible(void);

struct FileRequest;

struct sdfMemHandle_t;
extern s8 D_00324530[];
extern s32 D_0032E4C0[];
extern s32 D_0032E4E8[];
extern struct FileRequest *D_003BAFE8;
extern struct sdfMemHandle_t *D_003BAFEC;
extern u32 D_003BAFF0;
extern void mnuAdvanceTitleStateUnderSemaphore(void);
struct SoundMixer;
extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(struct SoundMixer *, const void *, const void *);
extern void dds3EffectObjectBasicStatOff(dds3Basic_t *, u32);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *, u32);
extern void dds3StartBEDEffectObjectBasic(dds3Basic_t *, u32);
extern void fldSetCameraNodeModeWithTen(void);
extern u8 D_003A0978[];
extern void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *);

extern s32 D_0032E5C4[];


extern u8 D_0034D8F0[];


extern void fldResetPlayerSceneObjectState(void);

extern s32 D_0032E478[];

extern f32 fldPrimaryQueuedEffectPosition[];

extern f32 fldSecondaryQueuedEffectPosition[];

extern s32 D_003D62C8[];

extern void *D_003D62A4[];

extern s32 D_003D62A8[];

extern s32 fldCurrentBgmId[];

extern void p2sdStopSe(u32 soundId);



extern KwlnTaskUpdateResult fldFieldTaskUpdate(dds3ProcessID_t *task);

extern s32 D_0032E3C0[];

extern s32 fldSceneSoundBase;

extern void dds3DeleteUnitObjectBasic(dds3Basic_t *node);

extern FldSparkObjectEntry fldSparkObjectEntries[];

/* Each DDS1 resource row owns sixteen 16-byte spark sequences. */
typedef struct FldSparkSequence {
    s32 count;
    s8 slots[12];
} FldSparkSequence;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 entryCount;            /* 0x04 */
    s32 duration;              /* 0x08 */
    s16 encounterSelection;
    s16 reservedModel;         /* 0x0E: model for reserved object slot 16. */
    FldSparkSequence sequences[16]; /* 0x10 */
} FldEnt110; /* 0x110 bytes */

typedef char FldSparkSequenceSizeCheck[(sizeof(FldSparkSequence) == 0x10) ? 1 : -1];
typedef char FldEnt110SizeCheck[(sizeof(FldEnt110) == 0x110) ? 1 : -1];
typedef char FldEnt110_encounterSelection_offset_must_be_0x0C[
    ((u32)&((FldEnt110 *)0)->encounterSelection == 0x0C) ? 1 : -1];

extern FldEnt110 *D_003BAA48;

typedef struct {
    s32 *unk0;
    u8 pad4[0x4C];
} FldTbl50; /* 0x50 bytes */

extern FldTbl50 D_003D46C0[];


extern u8 D_003BAE78[];


typedef struct {
    s32 flags;
    FldSpatialSoundId soundId;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FldClear18; /* 0x18 bytes */

extern FldClear18 fldSe3DTable[];


extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, s32, sdfTexHandle_t *);

#include "fld_title_banner_menu.h"

extern dds3ProcessID_t *inputpanel_id;
extern u32 fldRunDanteGunFlag[];
extern s32 gFldInf_WpExit;
extern s32 fileMenuTaskExists(void);
extern u8 fldHasKiretaLabelProcess(void);
extern u8 fldProcKiretaChk(void);
extern u8 fldProcHirakenaiChk(void);
extern u8 fldIsEventPhaseAtLeastTwo(void);
extern void fldApplyCameraFacingPoint(void);

FldTitleBannerResult fldResolvePendingTitleBanner(void) {
    FldTitleBannerMenu *menu;
    FldAreaWork *area;

    if (fldProcCampChk() != FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET) {
        return FLD_TITLE_BANNER_UNRESOLVED;
    }
    if (fldProcAmChk() != FLD_SCENE_TRANSITION_GATE_PROCEED) {
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
    if (menu->drawRequestState == FLD_TITLE_BANNER_DRAW_NOT_REQUESTED) {
        return FLD_TITLE_BANNER_NO_PENDING;
    }

    if (fldGlobalBase.titleFade != 0 || gFldInf_WpExit == 1 || (s8)dds3PadButtonState[1][0][1] < 0) {
        if ((s8)dds3PadButtonState[1][0][1] < 0) {
            fldApplyCameraFacingPoint();
        }
        menu->drawRequestState = FLD_TITLE_BANNER_DRAW_NOT_REQUESTED;
        area = &fldGlobalBase;
        if (area->titleFade != 0) {
            dds3FadeIn2(8);
            area->titleFade = 0;
        }
        return FLD_TITLE_BANNER_RESOLVED;
    }
    return FLD_TITLE_BANNER_UNRESOLVED;
}

extern u8 fldTitleIsActive(void);

extern void fldDrawAnimatedFieldBanner(s32, s32, s32);

extern void fldDrawMapLabelBanner(s32);

extern s32 gFldElvFloorNow;

KwlnTaskUpdateResult fldFieldTaskUpdate(dds3ProcessID_t *task) {
    if (fldProcCampChk() != FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (fldProcAmChk() != FLD_SCENE_TRANSITION_GATE_PROCEED) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (fileMenuTaskExists() != 0) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (fldTitleIsActive() != 0) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    if (gFldElvFloorNow != -999) {
        fldDrawAnimatedFieldBanner(0x80, 0, 0);
    } else {
        fldDrawMapLabelBanner(0);
    }
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

KwlnTaskUpdateResult fldAddressProcess1(dds3ProcessID_t *task) {
    s32 *work;

    work = sdfChipAllocDirect(0x10);
    work[0] = 0;
    work[1] = 0;
    work[2] = 0;
    work[3] = 0;
    dds3SetProcessData(task, (u32)work);
    return (KwlnTaskUpdateResult)(u32)fldFieldTaskUpdate;
}

void fldAddressShutdown(dds3ProcessID_t *task) {
    void *work;

    work = (void *)dds3GetProcessData(task);
    sdfChipFreeDirect(work);
    addressprocess_id = NULL;
}

void fldInitAddressProcess(void) {
    if (addressprocess_id == NULL) {
        addressprocess_id = dds3StartProcess(D_003BAE78, 0x2B0B, 1, 1, fldAddressProcess1, fldAddressShutdown, 0);
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
        entry->x = 0;
        entry->soundId = 0;
        entry += 1;
    } while (remaining >= 0);
    gFldSe3DNum = 0;
}

void fldAddSe3D(FldSpatialSoundId soundId, f32 radius, f32 sourceX, f32 sourceY, f32 sourceZ) {
    s32 i = gFldSe3DNum;

    fldSe3DTable[i].flags = 0;
    fldSe3DTable[i].soundId = soundId;
    fldSe3DTable[i].x = radius;
    fldSe3DTable[i].y = sourceX;
    fldSe3DTable[i].z = sourceY;
    fldSe3DTable[i].w = sourceZ;
    gFldSe3DNum = i + 1;
}

extern s32 fldStageSoundBaseTable[];


void fldPlayPendingSounds(void) {
    s32 stage;
    s32 i;

    stage = D_0032E3C0[0];
    if (stage == 11) {
        stage = evtBitCheck(0x28) != 0 ? 2 : stage;
    }
    for (i = 0; i < gFldSe3DNum; i++) {
        if (fldSe3DTable[i].flags & 1) {
            fldSe3DTable[i].flags &= ~1;
            p2sdStopSe(fldStageSoundBaseTable[stage] + fldSe3DTable[i].soundId);
        }
    }
}

extern f32 fldCamTarget[];
extern s32 D_003BAE90;
extern s32 D_003BAE94;
extern f32 fldLookAtNearPoint[];
typedef struct SoundPosition {
    f32 v[4];
} SoundPosition;
extern const SoundPosition D_003A0470;
extern void p2sdPlaySe3DEx(s32, s32, f32, f32, f32);
extern void p2sdSetStandPos(f32, f32, f32, f32, f32, f32);
void func_001415F8(void) {
    SoundPosition position = D_003A0470;
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

    tick = D_003BAE94 + 1;
    nearby = 0;
    D_003BAE94 = tick;
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
        dx = listenerX - fldSe3DTable[i].y;
        dy = listenerY - fldSe3DTable[i].z;
        dz = listenerZ - fldSe3DTable[i].w;
        distance = fsqrtf(dx * dx + dy * dy + dz * dz);
        if (distance < fldSe3DTable[i].x + extraRadius) {
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
        stage = evtBitCheck(0x28) != 0 ? 2 : stage;
    }
    for (i = 0; i < gFldSe3DNum; i++) {
        entry = &fldSe3DTable[i];
        sourceX = fldSe3DTable[i].y;
        sourceY = fldSe3DTable[i].z;
        sourceZ = fldSe3DTable[i].w;
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
            if (distance < entry->x + extraRadius) {
                position.v[0] = sourceX;
                position.v[1] = sourceY;
                position.v[2] = sourceZ;
                entry->flags = flags | 1;
                p2sdPlaySe3DEx(fldStageSoundBaseTable[stage] + entry->soundId,
                    (s32)(entry->x / 100.0f), position.v[0], position.v[1], position.v[2]);
            }
        } else if (distance < entry->x + extraRadius) {
            position.v[0] = sourceX;
            position.v[1] = sourceY;
            position.v[2] = sourceZ;
        } else {
            entry->flags = flags & ~1;
            p2sdStopSe(fldStageSoundBaseTable[stage] + entry->soundId);
        }
    }
    D_003BAE90++;
}

extern s32 D_0033EB7C[];

extern s32 D_0033EB80[];

extern s32 D_0033EB84[];

extern s32 D_0033EB88[];

extern s32 D_0033EB90[];

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0470);

s32 fldResolveSpecialBgmTrack(s32 id) {
    s32 result;

    switch (id) {
    case 0x80:
        result = D_0033EB78[0] + 1;
        break;
    case 0x81:
        result = D_0033EB7C[0] + 1;
        break;
    case 0x82:
        result = D_0033EB80[0] + 1;
        break;
    case 0x83:
        result = D_0033EB7C[0] + 2;
        break;
    case 0x84:
        result = D_0033EB84[0] + 1;
        break;
    case 0x85:
        result = D_0033EB84[0] + 2;
        break;
    case 0x86:
        result = D_0033EB88[0] + 1;
        break;
    case 0x87:
        result = D_0033EB90[0] + 1;
        break;
    case 0x88:
        result = D_0033EB90[0] + 2;
        break;
    case 0x89:
        result = D_0033EB90[0] + 3;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

typedef struct {
    s16 id;
    s16 flag;
    s8 data[0x40];
} FldEnt44; /* 0x44 bytes */

extern FldEnt44 D_0032E5C8[32];

s8 fldFindSceneEntryData(s32 id, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_0032E5C8[i].id == id && (D_0032E5C8[i].flag == 0 || evtBitCheck(D_0032E5C8[i].flag) != 0)) {
            return D_0032E5C8[i].data[idx];
        }
    }
    return 0;
}

void fldStartSceneBgm(void) {
    s32 handle = fldResolveSpecialBgmTrack(fldGlobalBase.sequenceCode);
    s32 stage;
    s32 idx;

    if (handle != -1 || fldGlobalBase.area < 0x32) {
        if (handle == -1) {
            stage = fldGlobalBase.area;
            if (stage == 11) {
                stage = evtBitCheck(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, fldGlobalBase.floor + 1);
            fldSceneSoundBase = fldStageSoundBaseTable[stage];
            fldGlobalBase.sequenceCode = idx;
            handle = fldSceneSoundBase + idx;
        }
        if (fldGlobalBase.sceneBgmSuppressed != FLD_SCENE_BGM_SUPPRESSED) {
            if (fldSeLastBgm != handle) {
                p2sdStopBGM(fldSeLastBgm);
            }
            fldSeLastBgm = handle;
            p2sdPlayBGM(handle);
        }
    }
}

void fldPlayFieldBgm(void) {
    if (fldCurrentBgmId[0] == 0x80) {
        p2sdPlayBGM(D_0033EB78[0] + 1);
    }
}

extern s32 fldResolveSpecialBgmTrack(s32);

extern u64 p2sdResumeBgm(s32);

void fldStartSceneBgmAlternate(void) {
    s32 handle = fldResolveSpecialBgmTrack(fldGlobalBase.sequenceCode);
    s32 stage;
    s32 idx;

    if (handle != -1 || fldGlobalBase.area < 0x32) {
        if (handle == -1) {
            stage = fldGlobalBase.area;
            if (stage == 11) {
                stage = evtBitCheck(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, fldGlobalBase.floor + 1);
            fldSceneSoundBase = fldStageSoundBaseTable[stage];
            fldGlobalBase.sequenceCode = idx;
            handle = fldSceneSoundBase + idx;
        }
        if (fldGlobalBase.sceneBgmSuppressed != FLD_SCENE_BGM_SUPPRESSED) {
            fldSeLastBgm = handle;
            p2sdResumeBgm(handle);
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

void fldPlayFieldSeVolumePan(s32 soundId) {
    if (D_0032E3C0[0] < 50 && soundId >= 16) {
        if (soundId == 0x80) {
            p2sdPlaySE(0x6C0060, 0x7F, 0x3F);
        } else if (soundId == 0x81) {
            p2sdPlaySE(0x6C0061, 0x7F, 0x3F);
        } else if (soundId == 0x82) {
            p2sdPlaySE(0x6C0064, 0x7F, 0x3F);
        } else {
            p2sdPlaySE(fldSceneSoundBase + soundId, 0x7F, 0x3F);
        }
    }
}

void fldPlayFieldSe(s32 soundId) {
    if (D_0032E3C0[0] < 50) {
        if (soundId == 0x100) {
            p2sdStopSe(0x6C0060);
        } else if (soundId == 0x101) {
            p2sdStopSe(0x6C0061);
        } else if (soundId == 0x102) {
            p2sdStopSe(0x6C0064);
        } else {
            p2sdStopSe(fldSceneSoundBase + soundId);
        }
    }
}

void fldSetSequenceVolume(s32 category, s32 volume) {
    if (D_0032E3C0[0] < 0x32) {
        p2sdPlaySE(fldSceneSoundBase + category, volume, 0x3F);
    }
}

extern s32 fldCurrentBgmMode;

extern s32 D_0033EAE8[];

extern void p2sdStopBGM(u32 trackHandle);

void fldSetBgmMode(s32 mode) {
    switch (mode) {
    case -1:
        if (fldCurrentBgmMode != mode) {
            p2sdStopSe(fldCurrentBgmMode);
            fldCurrentBgmMode = mode;
        }
        break;
    case FLD_MAP_BANNER_FADE_HIDDEN:
        mode = -1;
        if (fldCurrentBgmMode != mode) {
            p2sdStopBGM(fldCurrentBgmMode);
            fldCurrentBgmMode = mode;
        }
        break;
    case 1:
        mode = D_0033EB78[0] + 1;
        p2sdPlayBGM(mode);
        fldCurrentBgmMode = mode;
        break;
    case FLD_MAP_BANNER_FADE_OUT:
        mode = D_0033EAE8[0] + 1;
        p2sdPlayBGM(mode);
        fldCurrentBgmMode = mode;
        break;
    }
}

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 MidiNo;

extern FldArchiveLoadPhase fldBgmReloadStep;

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
            stage = evtBitCheck(0x28) != 0 ? 2 : stage;
        }
        idx = fldFindSceneEntryData(stage, fldGlobalBase.floor + 1);
        fldSceneSoundBase = fldStageSoundBaseTable[stage];
        fldGlobalBase.sequenceCode = idx;
        handle = fldSceneSoundBase + idx;
    }
    MidiNo = handle;
    fldBgmReloadStep = FLD_ARCHIVE_LOAD_PHASE_CHECK;
}

extern SndPackedTrackLoadStatus sndFindPackedTrackLoadStatus(s32);

extern void p2sdLoadSMGFile(s32);

FldArchiveLoadResult fldStepSceneBgmArchive(void) {
    switch (fldBgmReloadStep) {
    case FLD_ARCHIVE_LOAD_PHASE_CHECK:
        if (sndFindPackedTrackLoadStatus(MidiNo) == SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
            p2sdLoadSMGFile(MidiNo);
            fldBgmReloadStep = fldBgmReloadStep + 1;
        } else {
            fldBgmReloadStep = fldBgmReloadStep + 2;
        }
        break;
    case FLD_ARCHIVE_LOAD_PHASE_WAIT:
        if (sndFindPackedTrackLoadStatus(MidiNo) == SND_PACKED_TRACK_LOAD_RESIDENT) {
            fldBgmReloadStep = fldBgmReloadStep + 1;
        }
        break;
    default:
        fldBgmReloadStep = FLD_ARCHIVE_LOAD_PHASE_REPORTED;
        return FLD_ARCHIVE_LOAD_COMPLETE;
    }
    return FLD_ARCHIVE_LOAD_PENDING;
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
        if (sndFindPackedTrackLoadStatus(0x670000) == SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
            p2sdLoadSMGFile(0x670000);
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        } else {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 2;
        }
        break;
    case FLD_ARCHIVE_LOAD_PHASE_WAIT:
        if (sndFindPackedTrackLoadStatus(0x670000) == SND_PACKED_TRACK_LOAD_RESIDENT) {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        }
        break;
    default:
        /* The resident and reported phases both complete the one-shot load. */
        fldFixedArchiveLoadPhase = FLD_ARCHIVE_LOAD_PHASE_REPORTED;
        return FLD_ARCHIVE_LOAD_COMPLETE;
    }
    return FLD_ARCHIVE_LOAD_PENDING;
}

u32 fldGetCurrentBgmHandle(void) {
    return fldSeLastBgm;
}

extern FldLabelRow28 D_0033F068[], D_00342868[], D_00346068[];

extern char D_0033EC90[][0x18];

extern s16 D_003D43B0[], D_003D4408[], D_003D4460[];

extern s16 D_003BD7D0;

#include "fld_label_cache_api.h"

void fldCacheMapLabelLengths(s32 mapId) {
    s32 i;

    for (i = 0; i < 41; i++) {
        D_003D43B0[i] = strlen((const char *)D_0033F068[fldFindMapLabelIndexByCoordinate(mapId, i)].name);
    }
    for (i = 0; i < 24; i++) {
        D_003D4460[i] = strlen((const char *)D_00342868[fldFindSecondaryMapLabelIndexByCoordinate(mapId, i)].name);
    }
    for (i = 0; i < 41; i++) {
        D_003D4408[i] = strlen((const char *)D_00346068[fldFindMapCoordinateIndex(mapId, i)].name);
    }
    i = 0;
    if (D_0032E3C0[0] < 100) {
        i = D_0032E3C0[0];
    }
    D_003BD7D0 = strlen(D_0033EC90[i]);
}


/* The base format occupies the first 16-byte-aligned AMB format record. */
const char D_003A04B0[16] __attribute__((aligned(16))) = "%sf%03d.amb";

void fldLoadSceneRecordTable(void) {
    char path[128];
    char directory[64];
    u32 resourceAddress;
    s32 transferStart;
    s32 area;
    s32 room;
    FldSceneRecordTableHeader *header;
    const FldSceneRecord *rows;
    s32 count;

    fldMakeFilePath(directory, fldGlobalBase.area, 1);
    area = fldGlobalBase.area;
    if (area == 0x1C) {
        if (evtBitCheck(0x5E1)) {
            sprintf(path, D_003A04B0, directory, fldGlobalBase.area);
        } else if (evtBitCheck(0x5E2)) {
            sprintf(path, "%sf%03da.amb", directory, fldGlobalBase.area);
        } else if (evtBitCheck(0x5E3)) {
            sprintf(path, "%sf%03db.amb", directory, fldGlobalBase.area);
        } else if (evtBitCheck(0x5E4)) {
            sprintf(path, "%sf%03dc.amb", directory, fldGlobalBase.area);
        } else {
            sprintf(path, "%sf%03dd.amb", directory, fldGlobalBase.area);
        }
    } else if (area == 0x1B) {
        room = fldGlobalBase.floor;
        switch (room) {
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            sprintf(path, "%sf%03da.amb", directory, fldGlobalBase.area);
            break;
        case 7:
            sprintf(path, "%sf%03db.amb", directory, fldGlobalBase.area);
            break;
        case 22:
        case 23:
        case 27:
            if (evtBitCheck(0x58D)) {
                sprintf(path, "%sf%03dd.amb", directory, fldGlobalBase.area);
            } else {
                sprintf(path, "%sf%03dc.amb", directory, fldGlobalBase.area);
            }
            break;
        case 24:
        case 25:
        case 26:
        default:
            sprintf(path, D_003A04B0, directory, fldGlobalBase.area);
            break;
        }
    } else {
        sprintf(path, D_003A04B0, directory, fldGlobalBase.area);
    }

    fldAutoMapBuf = sdfDevLoadFileEx(path, &resourceAddress, 0);
    transferStart = resourceAddress + 8;
    fldAddressResolver((u8 *)resourceAddress, (const FldTransferChunk *)transferStart);
    header = (FldSceneRecordTableHeader *)fldGetInfoHeader((const FldTransferChunk *)transferStart);
    rows = header->rows;
    count = header->count;
    fldAutoMapData = count;
    fldAutoMapHeader = rows;
}

void fldAdoptSceneRecordChunk(u8 *chunk, struct sdfMemHandle_t *resourceHandle) {
    if (D_0032E3C0[0] < 0xC8) {
        u8 *transferStart = chunk + 8;

        fldAutoMapBuf = resourceHandle;
        fldAddressResolver(chunk, (const FldTransferChunk *)transferStart);
        {
            FldSceneRecordTableHeader *header =
                (FldSceneRecordTableHeader *)fldGetInfoHeader((const FldTransferChunk *)transferStart);
            const FldSceneRecord *rows = header->rows;
            s32 count = header->count;

            fldAutoMapData = count;
            fldAutoMapHeader = rows;
        }
    }
}

void fldInitSceneMapLabels(void) {
    s32 sceneId = D_0032E3C0[0];

    if (sceneId < 0xC8) {
        fldCacheMapLabelLengths(sceneId % 100);
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


extern sdfTexHandle_t *D_003D40A0[4];
extern sdfModelRootNode_t *D_00348F30[];
/* Native scene rows copy XYZ floats into this twelve-byte position cache. */
extern FldPoint D_003D40B0[];

void fldResetSceneResourceTables(void) {
    s32 i;

    fldGlobalBase.primaryMapLabelSelection = -1;
    fldGlobalBase.secondaryMapLabelSelection = -1;
    fldAutoMapBuf = 0;
    fldAutoMapHeader = 0;
    fldAutoMapData = 0;
    D_003D40A0[0] = 0;
    D_003D40A0[1] = 0;
    D_003D40A0[2] = 0;
    D_003D40A0[3] = 0;
    for (i = 0; i < 64; i++) {
        D_00348F30[i] = 0;
        D_003D40B0[i].x = 0.0f;
        D_003D40B0[i].y = 0.0f;
        D_003D40B0[i].z = 0.0f;
    }
}


extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];


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

extern FldProjectedSprite D_0033EBA0[];
extern sdfTexHandle_t *D_003D40A0[4];

void fldDrawProjectedMapIcon(f32 x, f32 y, s32 index, u32 color) {
    FldProjectedSprite *sprite = &D_0033EBA0[index];
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
                        D_003D40A0[D_0033EBA0[index].textureSlot]);
}

#ifdef NON_MATCHING
#include "fld_projected_sprite.h"
#include "dsp_entry_api.h"
#include "dsp_channel.h"
#include "evt_message_window_api.h"
#include "fld_area_work.h"
#include "fld_room_effect_entry.h"
#include "fld_object_slot.h"
#include "fld_npc_palette.h"
#include "file_pac.h"
#include "evt_world.h"
#include "common.h"
#include "snd_track_playback_api.h"
#include "fld_scene_ready_api.h"
#include "fld_clear_menu_entries_api.h"
#include "fld_play_pending_sounds_api.h"
#include "fld_scene_record_api.h"
#include "fld_object_slot_api.h"
#include "fld_release_texture_slots_api.h"
#include "fld_spark_object_entry.h"
#include "fld_spark_slot.h"
#include "sdf_math_vector4.h"
#include "kwln_fade.h"
#include "mnu_title_stream.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_textured_rect.h"
#include "sdf_motion.h"
#include "mdl.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_value.h"
#include "sdf_dev_state.h"
#include "fld_transfer_chunk.h"
#include "sdf_resource.h"
#include "eff_transform.h"
#include "sdf_model.h"
#include "fld_scene_record.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_node_descriptor.h"
#include "kwln.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "fr_font.h"
#include "itf.h"
#include "btl.h"
#include "evt_unit.h"
#include "fld_title_banner_menu.h"
#include "fld_label_cache_api.h"
#include "sdf_packet_alloc.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_vu_matrix.h"

extern FldProjectedSprite D_0033EBA0[];

extern f32 D_003BAEEC;
extern sdfTexHandle_t *D_003BAF38;
extern SdfPoolNode *fldDrawGetOt(void);
/* Draw a minimap icon from the icon table: its textured quad rotated about its
 * anchor, squashed to the map's tilt and placed at the map position, the
 * player's (index 1) pulsing in alpha. Upstream: INCLUDE_ASM in
 * game/code_001411F0; written from the retail routine. It differs from retail
 * only in register allocation and scheduling. */
void fldAutoMapDrawIconOfsRF(s32 index, f32 x, f32 y, f32 rotation) {
    f32 x0, x1, z0, z1;
    s32 textureU, textureV, textureWidth, textureHeight;
    s32 textureURight, textureVBottom;
    f32 corner0[4] __attribute__((aligned(16)));
    f32 corner1[4] __attribute__((aligned(16)));
    f32 corner2[4] __attribute__((aligned(16)));
    f32 corner3[4] __attribute__((aligned(16)));
    f32 xOffset, yOffset;
    u32 color;
    sdfDrawTag_t *packetList;
    SdfPoolNode *appendTarget;
    SdfPoolNode *drawSurface;

    if (index == 1) {
        color = (u32)(s32)(sdfSin(D_003BAEEC) * 64.0f) << 24;
        color += 0x80000000u;
        D_003BAEEC += 0.2f;
    } else {
        color = 0x80000000;
    }

    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdf2DLinkTexPacket(packetList, D_003BAF38, 0);

    textureU = (s32)D_0033EBA0[index].u * 16;
    textureV = (s32)D_0033EBA0[index].v * 16;
    textureWidth = (s32)D_0033EBA0[index].width * 16;
    textureHeight = (s32)D_0033EBA0[index].height * 16;
    textureURight = textureU + textureWidth;
    textureVBottom = textureV + textureHeight;
    x0 = (f32)-((s32)D_0033EBA0[index].anchorX * 16);
    z0 = (f32)-((s32)D_0033EBA0[index].anchorY * 16);
    x1 = x0 + (f32)textureWidth;
    z1 = z0 + (f32)textureHeight;

    corner0[0] = x0; corner0[1] = 0.0f; corner0[2] = z0;
    corner3[0] = x1; corner3[1] = 0.0f; corner3[2] = z1;
    corner1[0] = corner3[0]; corner1[1] = 0.0f; corner1[2] = corner0[2];
    corner2[0] = corner0[0]; corner2[1] = 0.0f; corner2[2] = corner3[2];

    SetRotateMatrixY((rotation + 180.0f) * 3.14f / 180.0f);
    VU0_LOAD_VF_MEMORY(vf10, corner0);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner0);
    VU0_LOAD_VF_MEMORY(vf10, corner1);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner1);
    VU0_LOAD_VF_MEMORY(vf10, corner2);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner2);
    VU0_LOAD_VF_MEMORY(vf10, corner3);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner3);

    xOffset = x * 16.0f;
    yOffset = y * 16.0f;
    corner0[0] += xOffset; corner0[2] = corner0[2] * 0.5f + yOffset;
    corner1[2] = corner1[2] * 0.5f + yOffset; corner1[0] += xOffset;
    corner2[0] += xOffset; corner2[2] = corner2[2] * 0.5f + yOffset;
    corner3[0] += xOffset; corner3[2] = corner3[2] * 0.5f + yOffset;

    sdfPrimLinkFt4(packetList, color | 0x808080, 0x44,
                         (s32)corner0[0], (s32)corner0[2],
                         textureU, textureV,
                         (s32)corner1[0], (s32)corner1[2],
                         textureURight, textureV,
                         (s32)corner2[0], (s32)corner2[2],
                         textureU, textureVBottom,
                         (s32)corner3[0], (s32)corner3[2],
                         textureURight, textureVBottom,
                         0, NULL);

    appendTarget = fldDrawGetOt();
    drawSurface = fldDrawGetOt();
    drawSurface->append(appendTarget, packetList);
}
#else
INCLUDE_ASM(const s32, "game/code_001411F0", fldAutoMapDrawIconOfsRF);
#endif

s32 fldAutoMap_GetFloor(s32 scene, u32 index) {
    s32 result = 1;
    const FldSceneRecord *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    const FldItem *item;

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


/* Model request buffers contain the root followed by indexed draw nodes. */
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
    const FldSceneRecord *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    const FldItem *item;

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
typedef struct _FRQ _FRQ;
extern _FRQ *D_003BD7CC;
extern s32 D_003BAEAC, D_003BAEB8, D_003BAEBC, D_003BAEC0;
extern s32 D_003BAEF0, D_003BAEF4, D_003BAF00, D_003BAF04, D_003BAF08;
extern f32 D_003BAEF8, D_003BAEFC, D_003BAF0C, D_003BAF10, D_003BAF14;
extern u8 dds3PadButtonState[2][2][16];
extern Matrix4 D_00329790, sdfViewMatrix;
extern f32 D_00324A20[4] __attribute__((aligned(16)));
extern sdfModelRootNode_t *D_00348F30[];
extern void frSetFontDrawOT(u32);
extern void InversMatrix(void);
extern s32 fldAutoMapGetFloorNext(s32), fldAutoMapGetFloorPrev(s32);
extern void aDrawBoxF(s32, s32, s32, s32, u32, u32, u32, u32);
extern void eglSetOt(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void eglLinkPoly4(f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,u32,u32,u32,u32);
extern u8 fldIsStageExplorationCellMarked(s32 floor, f32 x, f32 z);
extern void fldSubmitPrimaryFramePacket(void), fldSubmitAlternateFramePacket(void);
extern void fldAllIconDel(s32 index, f32 *x, f32 *z);
extern void fldPoint3To2H(f32 *, f32 *, f32, f32, f32);
extern void fldAutoMapDrawIconOfsRF(s32, f32, f32, f32);
extern void fldAutoMapGetBBox(f32 *, f32 *, f32 *, f32 *);
extern void frSetPriority(_FRQ *, u32);
extern s32 frFontDrawGlyphWithSharedFlags(_FRQ *, s8);
extern s32 frDestroyFontRequestAfter(_FRQ *);


void fldDrawAutomapScene(void) {
    s32 proposed;
    s32 selectedFloor;
    s32 itemVisible;
    s32 i, j, k;
    const FldSceneRecord *record;
    const FldItem *item;
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
    VU0_LOAD_MATRIX(&D_00329790);
    InversMatrix();
    D_003BAEF0 = D_003BAEB8;
    if ((s8)dds3PadButtonState[1][0][8] < 0 && D_003BAEF0 < proposed) {
        proposed = D_003BAEF0 + 1;
        if (!D_003BAEAC) proposed = fldAutoMapGetFloorNext(D_003BAEF0);
        if (D_003BAEF0 != proposed) {
            p2sdPlaySE(2, 127, 63);
            D_003BAEF4 = D_003BAEF0;
            D_003BAEF0 = proposed;
            D_003BAEF8 = -2500.0f;
            D_003BAF04 = -24;
            D_003BAEFC = 0.0f;
            D_003BAF00 = 0;
            D_003BAF08 = 0;
        }
    }
    if ((s8)dds3PadButtonState[1][0][10] < 0 && D_003BAEF0 >= 2) {
        proposed = D_003BAEF0 - 1;
        if (!D_003BAEAC) proposed = fldAutoMapGetFloorPrev(D_003BAEF0);
        if (D_003BAEF0 != proposed) {
            p2sdPlaySE(2, 127, 63);
            D_003BAEF4 = D_003BAEF0;
            D_003BAEF0 = proposed;
            D_003BAEF8 = 2500.0f;
            D_003BAF04 = 24;
            D_003BAEFC = 0.0f;
            D_003BAF00 = 0;
            D_003BAF08 = 0;
        }
    }
    if (D_003BAF00 < 128) D_003BAF00 += 16;
    if (D_003BAF04 < 0) { D_003BAF04 += 4; D_003BAF08 += 4; }
    if (D_003BAF04 > 0) { D_003BAF04 -= 4; D_003BAF08 -= 4; }
    if (D_003BAEF8 < 1000.0f && D_003BAEF8 > -1000.0f) {
        D_003BAEF8 = 0.0f;
    } else if (D_003BAEF8 > 0.0f) {
        D_003BAEF8 -= 500.0f; D_003BAEFC -= 500.0f;
    } else if (D_003BAEF8 < 0.0f) {
        D_003BAEF8 += 500.0f; D_003BAEFC += 500.0f;
    } else { D_003BAEF8 = 0.0f; }
    D_003BAEB8 = D_003BAEF0;
    sdf3DSetCamera(&dds3ScreenParam, &D_00329790);
    rsetOtOfs(0x56);
    rsetAlpha(0);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);
    rsetOtOfs(0x57);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    rsetOtOfs(0x56);
    rsetTest(1, 1, 128, 1, 0, 0, 1, 1);
    stage = fldFindStageCoordinateRecord(fldGlobalBase.area, D_003BAEF0);
    if (stage != NULL) {
        s32 run;
        f32 step = (f32)stage->cellSize;
        u32 color;
        eglSetOt(0x56);
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        color = D_003BAEAC ? 0x80000080 : 0x80000000;
        for (j = 0, sceneZ = stage->originZ; j < stage->rows;
             j++, sceneZ -= step) {
            if (sceneZ > (f32)D_003BAEC0 + 8400.0f) continue;
            if (sceneZ < (f32)D_003BAEC0 - 8400.0f) break;
            run = 0;
            for (i = 0, sceneX = stage->originX; i < stage->cols;
                 i++, sceneX += step) {
                if (sceneX < (f32)D_003BAEBC - 11400.0f) continue;
                if (sceneX > (f32)D_003BAEBC + 11400.0f) break;
                if (fldIsStageExplorationCellMarked(D_003BAEF0, sceneX, sceneZ)) {
                    if (run == 0) { left = sceneX - 50.0f; top = sceneZ + 50.0f; }
                    bottom = sceneZ - step - 50.0f;
                    right = sceneX + step + 50.0f;
                    run++;
                } else {
                    if (run > 0) eglLinkPoly4(left, D_003BAEF8 - 10.0f, top,
                    right, D_003BAEF8 - 10.0f, top,
                    left, D_003BAEF8 - 10.0f, bottom,
                    right, D_003BAEF8 - 10.0f, bottom, color, color, 0x80000000, 0x80000000);
                    run = 0;
                }
            }
            if (run > 0) eglLinkPoly4(left, D_003BAEF8 - 10.0f, top,
                    right, D_003BAEF8 - 10.0f, top,
                    left, D_003BAEF8 - 10.0f, bottom,
                    right, D_003BAEF8 - 10.0f, bottom, color, color, 0x80000000, 0x80000000);
            run = 0;
        }
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 1, 1, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(0);
    }


    if (D_003BAEAC != 0) {
        rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    }

    selectedFloor = D_003BAEF0 - 1;
    record = fldAutoMapHeader;
    for (i = 0; i < fldAutoMapData; i++, record++) {
        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            itemVisible = 0;
            if (selectedFloor == item->floor) {
                if (fldGetFloorFlag(fldAutoMapField, i, j) != FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED) {
                    itemVisible = 1;
                }
                if (D_003BAEAC != 0) {
                    itemVisible = 1;
                }
            }
            fldAutoMapNodeSet(D_00348F30[i], item->nodeIndex, itemVisible);
        }
    }
    for (i = 0; i < fldAutoMapData; i++) {
        D_00348F30[i]->color = 0x80808080;
        fldAutoMapDrawMdl1(D_00348F30[i], 0.0f, D_003BAEF8, 0.0f);
    }

    rsetOtOfs(0x56);
    rsetAlpha(0);
    fldSubmitPrimaryFramePacket();
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);

    record = fldAutoMapHeader;
    selectedFloor = D_003BAEF0 - 1;
    for (i = 0; i < fldAutoMapData; i++, record++) {
        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            itemVisible = 0;
            if (selectedFloor == item->floor) {
                if (fldGetFloorFlag(fldAutoMapField, i, j) != FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED) {
                    itemVisible = 1;
                }
                if (D_003BAEAC != 0) {
                    itemVisible = 1;
                }
            }
            fldAutoMapNodeSet(D_00348F30[i], item->nodeIndex, itemVisible);
        }
        D_00348F30[i]->color = 0x80808080;
        fldAutoMapDrawMdl1(D_00348F30[i], -50.0f, D_003BAEF8, 50.0f);
        fldAutoMapDrawMdl1(D_00348F30[i], 50.0f, D_003BAEF8, 50.0f);
        fldAutoMapDrawMdl1(D_00348F30[i], -50.0f, D_003BAEF8, -50.0f);
        fldAutoMapDrawMdl1(D_00348F30[i], 50.0f, D_003BAEF8, -50.0f);

        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            fldAutoMapNodeSet(D_00348F30[i], item->nodeIndex,
                          selectedFloor == item->floor);
        }
        D_00348F30[i]->color = 0x10101010;
        fldAutoMapDrawMdl1(D_00348F30[i], 0.0f, D_003BAEF8, 0.0f);
        D_00348F30[i]->color = 0x80808080;
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
            if ((f32)D_003BAEC0 + 8400.0f < sceneZ) {
                continue;
            }
            if (sceneZ < (f32)D_003BAEC0 - 8400.0f) {
                break;
            }
            run = 0;
            sceneX = stage->originX;
            for (i = 0; i < stage->cols; i++, sceneX += step) {
                if (sceneX < (f32)D_003BAEBC - 11400.0f) {
                    continue;
                }
                if ((f32)D_003BAEBC + 11400.0f < sceneX) {
                    break;
                }
                if (fldIsStageExplorationCellMarked(D_003BAEF0, sceneX, sceneZ) != 0) {
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
                            left, D_003BAEF8 - 10.0f, top,
                            right, D_003BAEF8 - 10.0f, top,
                            left, D_003BAEF8 - 10.0f, bottom,
                            right, D_003BAEF8 - 10.0f, bottom,
                            0x80867173, 0x80867173, 0x80867173, 0x80867173);
                    }
                    run = 0;
                }
            }
            if (run > 0) {
                eglLinkPoly4(
                    left, D_003BAEF8 - 10.0f, top,
                    right, D_003BAEF8 - 10.0f, top,
                    left, D_003BAEF8 - 10.0f, bottom,
                    right, D_003BAEF8 - 10.0f, bottom,
                    0x80867173, 0x80867173, 0x80867173, 0x80867173);
            }
            run = 0;
        }
        evtSubmitPrimaryAlphaBlendMode(0);
    }

    rsetAlpha(0);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    D_003BAF10 += 0.4f;
    D_003BAF14 += 0.2f;
    PCP_COPY_VECTOR_F32(focus, D_00324A20);

    if (D_003BAF00 >= 128) {
        selectedFloor = D_003BAEF0 - 1;
        record = fldAutoMapHeader;
        for (i = 0; i < fldAutoMapData; i++, record++) {
            sceneX = 0.0f;
            sceneZ = 0.0f;
            item = record->items;
            for (j = 0; j < record->count; j++, item++) {
                itemVisible = 0;
                if (selectedFloor == item->floor) {
                    if (fldGetFloorFlag(fldAutoMapField, i, j) != FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED) {
                    itemVisible = 1;
                }
                    if (D_003BAEAC != 0) {
                        itemVisible = 1;
                    }
                }
                if (itemVisible != 0) {
                    const FldIcon *icon = item->icons;

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
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 8;
                                    iconVisible = 1;
                                }
                                break;
                            case 2:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 9;
                                    iconVisible = 1;
                                }
                                break;
                            case 3:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 10;
                                    iconVisible = 1;
                                }
                                break;
                            case 4:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 5;
                                    iconVisible = 1;
                                }
                                break;
                            case 5:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 2;
                                    iconVisible = 1;
                                }
                                break;
                            case 6:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 4;
                                    iconVisible = 1;
                                }
                                break;
                            case 7:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 6;
                                    iconVisible = 1;
                                }
                                break;
                            case 8:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 7;
                                    iconVisible = 1;
                                }
                                break;
                            case 9:
                                spriteIndex = 3;
                                iconVisible = 1;
                                break;
                            case 10:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 12;
                                    iconVisible = 1;
                                }
                                break;
                            case 11:
                                spriteIndex = 11;
                                iconVisible = 1;
                                offsetZ = sdfSin(D_003BAF14 + (i + k) * 3.3f) * 30.0f;
                                break;
                            case 12:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 13;
                                    iconVisible = 1;
                                }
                                break;
                            case 13:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAEF0,
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
                                                            x, D_003BAEF8, z);
                                    fldDrawProjectedMapIcon(projectedX, projectedY,
                                                  spriteIndex, 0x80808080);
                                }
                            }
                        }
                    }
                }
            }
        }
        /* The fade gate deliberately stays open for the player-marker tail. */

/* Source-only reconstruction of fldDrawAutomapScene, starting at 0x00143EC0.
 * Continues inside the D_003BAF00 >= 128 block after the icon loops.
 * Function-scope locals and declarations are supplied by the prefix. */

        if (D_003BAEF0 == fldGlobalBase.flagStage) {
            f32 worldX;
            f32 worldZ;
            f32 distanceX;
            f32 distanceZ;

            fldAllIconDel(D_003BAEB4, &sceneX, &sceneZ);
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
                                    worldX, D_003BAEF8, worldZ);
            if (distanceX < 80000.0f && distanceZ < 80000.0f) {
                rsetAlpha(1);
                fldAutoMapDrawIconOfsRF(1, playerX, playerY, -fldGlobalBase.negatedAngle);
                rsetAlpha(0);
                fldAutoMapDrawIconOfsRF(0, playerX, playerY, fldGlobalBase.angle);
            }
        }
    }

    rsetOtOfs(0x5E);
    rsetAlpha(0);
    rsetAlpha(1);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    DrwSprite2D(0, 0, 512, 448, 32, 59, 22, 22,
                        0x40808080, D_003D40A0[1]);
    rsetAlpha(0);
    rsetAlpha(0);
    DrwSprite2D(332, 410, 156, 27, 2, 83, 156, 27,
                        0x80808080, D_003D40A0[0]);

    top = sdfSin(D_003BAF0C);
    i = (s32)(top * 32.0f) + 96;
    D_003BAF0C += 0.2f;
    i = i + (i << 8) + (i << 16)
                 + 0x80000000U;
    fldAutoMapGetBBox(&minX, &maxZ, &maxX, &minZ);
    minX = (s32)((minX - 600.0f) / 600.0f) * 600;
    maxX = (s32)((maxX + 600.0f) / 600.0f) * 600;
    maxZ = (s32)((maxZ - 600.0f) / 600.0f) * 600;
    minZ = (s32)((minZ + 600.0f) / 600.0f) * 600;
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    rsetAlpha(0);

    DrwSprite2D(242, 63, 28, 27, 172, 61, 28, 27,
                        0x80808080, D_003D40A0[0]);
    DrwSprite2D(20, 222, 28, 27, 201, 89, 28, 27,
                        0x80808080, D_003D40A0[0]);
    DrwSprite2D(242, 386, 28, 27, 172, 89, 28, 27,
                        0x80808080, D_003D40A0[0]);
    DrwSprite2D(464, 222, 28, 27, 201, 61, 28, 27,
                        0x80808080, D_003D40A0[0]);

    if ((f32)D_003BAEC0 < maxZ) {
        DrwSprite2D(242, 53, 28, 40, 2, 2, 28, 40,
                            i, D_003D40A0[1]);
    }
    if (minZ < (f32)D_003BAEC0) {
        DrwSprite2D(242, 383, 28, 40, 2, 43, 28, 40,
                            i, D_003D40A0[1]);
    }
    if (minX < (f32)D_003BAEBC) {
        DrwSprite2D(11, 222, 39, 27, 31, 30, 39, 27,
                            i, D_003D40A0[1]);
    }
    if ((f32)D_003BAEBC < maxX) {
        DrwSprite2D(462, 222, 39, 27, 31, 2, 39, 27,
                            i, D_003D40A0[1]);
    }
    DrwSprite2D(173, 0, 166, 80, 2, 2, 166, 80,
                        0x80808080, D_003D40A0[0]);
    DrwSprite2D(339, 0, 190, 80, 167, 2, 1, 80,
                        0x80808080, D_003D40A0[0]);

    {
        const u8 *title = (const u8 *)D_0033EC90[fldGlobalBase.area % 100];

        D_003BD7CC = itfCreateConvertedTextGlyph(
            (450 - D_003BD7D0 * 6) << 4, 112, 0, 0xA09DC380,
            title, 0);
        frSetPriority(D_003BD7CC, 0xFFFFFF);
        frFontDrawGlyphInDefaultMode(D_003BD7CC);
        frDestroyFontRequestAfter(D_003BD7CC);
    }

    rsetOtOfs(0x5E);
    rsetAlpha(0);
    rsetTest(1, 0, 128, 2, 0, 0, 1, 2);
    rsetTest(1, 5, 128, 3, 0, 0, 1, 2);

    if (D_003BAF04 != 0 && (u32)D_003BAEF4 - 1U < 40U
        && D_003D4408[D_003BAEF4] >= 2) {
        FldMapCoordinateRecordIndex labelIndex;

        labelAlpha = D_003BAF08;
        if (labelAlpha < 0) {
            labelAlpha = -labelAlpha;
        }
        labelAlpha = 128 - (labelAlpha << 3);
        if (labelAlpha < 0) {
            labelAlpha = 0;
        }
        labelColor = labelAlpha | 0x78759B00;
        labelIndex = fldFindMapCoordinateIndex(fldGlobalBase.area % 100, D_003BAEF4);
        D_003BD7CC = itfCreateConvertedTextGlyph(
            (450 - (D_003D4408[D_003BAEF4] / 2) * 12) << 4,
            (D_003BAF08 + 42) << 3, 0, labelColor,
            D_00346068[labelIndex].name, 0);
        frSetPriority(D_003BD7CC, 0xFFFFFF);
        frFontDrawGlyphWithSharedFlags(D_003BD7CC, 1);
        frDestroyFontRequestAfter(D_003BD7CC);
    }

    if ((u32)D_003BAEF0 - 1U < 40U && D_003D4408[D_003BAEF0] >= 2) {
        FldMapCoordinateRecordIndex labelIndex;

        labelAlpha = D_003BAF04;
        if (labelAlpha < 0) {
            labelAlpha = -labelAlpha;
        }
        labelAlpha = 128 - (labelAlpha << 3);
        if (labelAlpha < 0) {
            labelAlpha = 0;
        }
        labelColor = labelAlpha | 0x78759B00;
        labelIndex = fldFindMapCoordinateIndex(fldGlobalBase.area % 100, D_003BAEF0);
        D_003BD7CC = itfCreateConvertedTextGlyph(
            (450 - (D_003D4408[D_003BAEF0] / 2) * 12) << 4,
            (D_003BAF04 << 3) + 332, 0, labelColor,
            D_00346068[labelIndex].name, 0);
        frSetPriority(D_003BD7CC, 0xFFFFFF);
        frFontDrawGlyphWithSharedFlags(D_003BD7CC, 1);
        frDestroyFontRequestAfter(D_003BD7CC);
    }

    VU0_LOAD_MATRIX(&sdfViewMatrix);
}

extern f32 D_003BAF18;
extern Matrix4 D_00329790;
extern Matrix4 sdfViewMatrix;
extern void InversMatrix(void);
extern void aDrawBoxF(s32, s32, s32, s32, u32, u32, u32, u32);

void fldDrawFieldBackdropFrame(void) {
    s32 i;
    s32 x;
    s32 y;
    s32 alpha;
    u32 color;

    /* The stored single-precision phase step is just below 0.1. */
    D_003BAF18 += 0x1.999998p-4f;
    VU0_LOAD_MATRIX(&D_00329790);
    InversMatrix();
    sdf3DSetCamera(&dds3ScreenParam, &D_00329790);

    rsetOtOfs(0x56);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 0x80);
    DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x40, 0x40,
                        0x80404040, D_003D40A0[2]);
    rsetAlpha(1);

    alpha = 0x5C - (s32)(sdfSin(D_003BAF18 + 3.0f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    DrwSprite2D(0x0E, 6, 0x100, 0x100, 0, 0, 0x100, 0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSin(D_003BAF18 + 1.5f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    DrwSprite2D(0x25, 0x1D, 0x100, 0x100, 0, 0, 0x100, 0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSin(D_003BAF18) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    DrwSprite2D(0x3C, 0x34, 0x100, 0x100, 0, 0, 0x100, 0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSin(D_003BAF18) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    DrwSprite2D(0xC4, 0x8C, 0x100, 0x100, 0x100, 0x100, -0x100, -0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSin(D_003BAF18 + 2.0f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    DrwSprite2D(0xDB, 0xA3, 0x100, 0x100, 0x100, 0x100, -0x100, -0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSin(D_003BAF18 + 3.0f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    DrwSprite2D(0xF2, 0xBA, 0x100, 0x100, 0x100, 0x100, -0x100, -0x100,
                        color, D_003D40A0[3]);
    rsetAlpha(1);

    for (i = 0; i < 9; i++) {
        x = 0x0C + i * 0x17;
        y = -(i * 0x32);
        DrwSprite2D(x, y, 5, 0x1A3, 3, 0x56, 5, 0x1C,
                            0x20808080, D_003D40A0[1]);
    }
    for (i = 0; i < 19; i++) {
        x = -(i * 0x0A);
        y = 4 + i * 0x17;
        DrwSprite2D(x, y, 0xC3, 5, 0x0B, 0x56, 0x19, 5,
                            0x20808080, D_003D40A0[1]);
    }
    for (i = 0; i < 9; i++) {
        x = 0x1EF - i * 0x17;
        y = 0x1D + i * 0x32;
        DrwSprite2D(x, y, 5, 0x1A3, 3, 0x72, 5, -0x1C,
                            0x20808080, D_003D40A0[1]);
    }
    for (i = 0; i < 19; i++) {
        x = 0x13D + i * 0x0A;
        y = 0x1B7 - i * 0x17;
        DrwSprite2D(x, y, 0xC3, 5, 0x24, 0x56, -0x19, 5,
                            0x20808080, D_003D40A0[1]);
    }

    rsetAlpha(0);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    VU0_LOAD_MATRIX(&sdfViewMatrix);
    rsetOtOfs(0x5E);
    rsetTest(1, 5, 0x81, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}


#ifdef NON_MATCHING
#include "dsp_entry_api.h"
#include "dsp_channel.h"
#include "evt_message_window_api.h"
#include "fld_area_work.h"
#include "fld_room_effect_entry.h"
#include "fld_object_slot.h"
#include "fld_npc_palette.h"
#include "file_pac.h"
#include "evt_world.h"
#include "common.h"
#include "snd_track_playback_api.h"
#include "fld_scene_ready_api.h"
#include "fld_clear_menu_entries_api.h"
#include "fld_play_pending_sounds_api.h"
#include "fld_scene_record_api.h"
#include "fld_object_slot_api.h"
#include "fld_release_texture_slots_api.h"
#include "fld_spark_object_entry.h"
#include "fld_spark_slot.h"
#include "sdf_math_vector4.h"
#include "kwln_fade.h"
#include "mnu_title_stream.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_textured_rect.h"
#include "sdf_motion.h"
#include "mdl.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_value.h"
#include "sdf_dev_state.h"
#include "fld_transfer_chunk.h"
#include "sdf_resource.h"
#include "eff_transform.h"
#include "sdf_model.h"
#include "fld_scene_record.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_node_descriptor.h"
#include "kwln.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "fr_font.h"
#include "itf.h"
#include "btl.h"
#include "evt_unit.h"
#include "fld_title_banner_menu.h"
#include "fld_label_cache_api.h"
#include "field_stage.h"
#include "fld_automap_entry_discovery_state.h"

extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);
extern s32 fldAutoMapGetFloorMax(void);
extern void fldDrawProjectedMapIcon(f32 x, f32 y, s32 index, u32 color);
extern void fldAutoMapDrawMdl1(sdfModelRootNode_t *emitter, f32 x, f32 y, f32 z);
extern void fldAutoMapNodeSet(sdfModelRootNode_t *model, s32 index, s32 clearFlag);
extern f32 D_00324A20[4] __attribute__((aligned(16)));
extern s32 D_003BAEAC, D_003BAEB8;

extern s32 fldAutoMapField;
extern s32 D_003BAEB4;
extern void rsetOtOfs(u32);
extern void rsetAlpha(s32);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, s32, sdfTexHandle_t *);
extern sdfModelRootNode_t *D_00348F30[];

extern void InversMatrix(void);
extern void aDrawBoxF(s32, s32, s32, s32, u32, u32, u32, u32);
extern void eglSetOt(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void eglLinkPoly4(f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,u32,u32,u32,u32);

extern void fldSubmitPrimaryFramePacket(void), fldSubmitAlternateFramePacket(void);
extern void fldAllIconDel(s32 index, f32 *x, f32 *z);
extern void fldPoint3To2H(f32 *, f32 *, f32, f32, f32);
extern void fldAutoMapDrawIconOfsRF(s32, f32, f32, f32);



extern s32 D_003BAF1C, D_003BAF20;
extern f32 D_003BAF24, D_003BAF28;
extern sdfTexHandle_t *D_003BAF38;
/* Draw the field minimap: the scenes and items around the player on the
 * selected floor, projected through the map matrix, with the player's marker.
 * Upstream: INCLUDE_ASM in game/code_001411F0; written from the retail routine.
 * It differs from retail in register copies, the jump table's label and the
 * placement of two float literals (13 words). */
void func_00144D30(void) {
    s32 i, j, k;
    s32 selectedFloor;
    s32 itemVisible;
    FldSceneRecord *record;
    FldItem *item;
    FieldStageCoordinate *stage;
    f32 sceneX, sceneZ;
    f32 projectedX, projectedY;
    f32 playerX, playerY;
    f32 playerWorldX, playerWorldZ;
    f32 focus[4] __attribute__((aligned(16)));
    Matrix4 *mapMatrix;

    if (fldGlobalBase.unk138 > 0) {
        fldGlobalBase.unk138--;
        return;
    }

    mapMatrix = &D_00329790;
    fldAllIconDel(D_003BAEB4, &sceneX, &sceneZ);
    playerWorldX = fldGlobalBase.x + sceneX;
    playerWorldZ = fldGlobalBase.z + sceneZ;
    fldAutoMapGetFloorMax();
    VU0_LOAD_MATRIX(mapMatrix);
    InversMatrix();
    D_003BAF1C = D_003BAEB8;
    D_003BAF20 = 128;
    sdf3DSetCamera(&dds3ScreenParam, mapMatrix);
    rsetOtOfs(0x56);
    rsetAlpha(0);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);
    rsetOtOfs(0x57);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    rsetOtOfs(0x56);
    rsetTest(1, 1, 128, 1, 0, 0, 1, 1);

    if (D_003BAEAC == 0) {
        stage = fldFindStageCoordinateRecord(fldGlobalBase.area, D_003BAF1C);
        if (stage != NULL) {
            f32 step = (f32)stage->cellSize;

            eglSetOt(0x56);
            evtSubmitPrimaryGsTest(1, 0, 128, 1, 0, 0, 1, 1);
            evtSubmitPrimaryAlphaBlendMode(1);
            for (i = 0, sceneX = stage->originX; i < stage->cols;
                 i++, sceneX += step) {
                if (sceneX > playerWorldX + 3000.0f) {
                    continue;
                }
                if (sceneX < playerWorldX - 3000.0f) {
                    continue;
                }
                for (j = 0, sceneZ = stage->originZ; j < stage->rows;
                     j++, sceneZ -= step) {
                    if (sceneZ > playerWorldZ + 3600.0f) {
                        continue;
                    }
                    if (sceneZ < playerWorldZ - 3600.0f) {
                        continue;
                    }
                    if (fldIsStageExplorationCellMarked(D_003BAF1C, sceneX, sceneZ) != 0) {
                        eglLinkPoly4(
                            sceneX - 50.0f, -10.0f, sceneZ + 50.0f,
                            sceneX + step + 50.0f, -10.0f, sceneZ + 50.0f,
                            sceneX - 50.0f, -10.0f, sceneZ - step - 50.0f,
                            sceneX + step + 50.0f, -10.0f, sceneZ - step - 50.0f,
                            0x80000000, 0x80000000, 0x80000000, 0x80000000);
                    }
                }
            }
            evtSubmitPrimaryGsTest(1, 0, 128, 1, 1, 1, 1, 1);
            evtSubmitPrimaryAlphaBlendMode(0);
        }
    }

    eglSetOt(0x56);
    evtSubmitPrimaryAlphaBlendMode(1);
    evtSubmitDefaultDepthGradientRect(0, 0, 512, 290, 0, 0, 0, 0);
    evtSubmitDefaultDepthGradientRect(0, 290, 404, 143, 0, 0, 0, 0);
    evtSubmitDefaultDepthGradientRect(499, 290, 14, 143, 0, 0, 0, 0);
    evtSubmitDefaultDepthGradientRect(0, 433, 512, 16, 0, 0, 0, 0);
    evtSubmitPrimaryAlphaBlendMode(0);

    record = fldAutoMapHeader;
    selectedFloor = D_003BAF1C - 1;
    for (i = 0; i < (s32)fldAutoMapData; i++, record++) {
        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            itemVisible = 0;
            if (selectedFloor == item->floor) {
                if (fldGetFloorFlag(fldAutoMapField, i, j) != 0) {
                    itemVisible = 1;
                }
                if (D_003BAEAC != 0) {
                    itemVisible = 1;
                }
            }
            fldAutoMapNodeSet(D_00348F30[i], item->nodeIndex, itemVisible);
        }
    }

    for (i = 0; i < (s32)fldAutoMapData; i++) {
        sdfModelRootNode_t *model;

        model = D_00348F30[i];
        model->color = 0x80808080;
        fldAutoMapDrawMdl1(model, 0.0f, 0.0f, 0.0f);
    }

    rsetOtOfs(0x56);
    rsetAlpha(0);
    fldSubmitPrimaryFramePacket();
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);
    rsetOtOfs(0x5A);
    fldSubmitAlternateFramePacket();
    rsetAlpha(0);
    rsetTest(1, 0, 128, 1, 1, 1, 1, 1);
    aDrawBoxF(0, 0, 512, 224, 128, 128, 162, 128);
    rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    D_003BAF24 += 0.4f;
    D_003BAF28 += 0.2f;
    PCP_COPY_VECTOR_F32(focus, D_00324A20);

    if (D_003BAF20 >= 128) {
        rsetAlpha(1);
        aDrawBoxF(0, 0, 512, 224, 0, 0, 0, 0);
        aDrawBoxF(404, 145, 95, 71, 0, 0, 0, 128);
        rsetAlpha(0);
        rsetTest(1, 0, 128, 3, 1, 1, 1, 1);

        record = fldAutoMapHeader;
        selectedFloor = D_003BAF1C - 1;
        for (i = 0; i < (s32)fldAutoMapData; i++, record++) {
            sceneX = 0.0f;
            sceneZ = 0.0f;
            item = record->items;
            for (j = 0; j < record->count; j++, item++) {
                itemVisible = 0;
                if (selectedFloor == item->floor) {
                    if (fldGetFloorFlag(fldAutoMapField, i, j) != 0) {
                        itemVisible = 1;
                    }
                    if (D_003BAEAC != 0) {
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

                            iconVisible = 0;

                            switch (icon->kind) {
                            case 1:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 8;
                                    iconVisible = 1;
                                }
                                break;
                            case 2:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 9;
                                    iconVisible = 1;
                                }
                                break;
                            case 3:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 10;
                                    iconVisible = 1;
                                }
                                break;
                            case 4:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 5;
                                    iconVisible = 1;
                                }
                                break;
                            case 5:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 2;
                                    iconVisible = 1;
                                }
                                break;
                            case 6:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 4;
                                    iconVisible = 1;
                                }
                                break;
                            case 7:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 6;
                                    iconVisible = 1;
                                }
                                break;
                            case 8:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 7;
                                    iconVisible = 1;
                                }
                                break;
                            case 10:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 12;
                                    iconVisible = 1;
                                }
                                break;
                            case 11:
                                spriteIndex = 11;
                                iconVisible = 1;
                                offsetZ = sdfSin(D_003BAF28 + (i + k) * 3.3f) * 30.0f;
                                break;
                            case 12:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 13;
                                    iconVisible = 1;
                                }
                                break;
                            case 13:
                                if (D_003BAEAC != 0 ||
                                    fldIsStageExplorationCellMarked(D_003BAF1C,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 14;
                                    iconVisible = 1;
                                }
                                break;
                            default:
                                spriteIndex = 11;
                                iconVisible = 0;
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
                                    fldPoint3To2H(&projectedX, &projectedY, x, 0.0f, z);
                                    fldDrawProjectedMapIcon(projectedX, projectedY, spriteIndex, 0x80808080);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (D_003BAF1C == fldGlobalBase.flagStage) {
            f32 worldX;
            f32 worldZ;
            f32 distanceX;
            f32 distanceZ;

            rsetTest(1, 0, 128, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            DrwSprite2D(413, 320, 77, 84, 34, 2, 28, 32,
                                0x80808080, D_003BAF38);
            DrwSprite2D(404, 432, 95, 2, 2, 124, 95, 2,
                                0x80808080, D_003BAF38);
            fldAllIconDel(D_003BAEB4, &sceneX, &sceneZ);
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
            fldPoint3To2H(&playerX, &playerY, worldX, 0.0f, worldZ);
            if (distanceX < 80000.0f && distanceZ < 80000.0f) {
                rsetAlpha(1);
                fldAutoMapDrawIconOfsRF(1, playerX, playerY, -fldGlobalBase.negatedAngle);
                rsetAlpha(0);
                fldAutoMapDrawIconOfsRF(0, playerX, playerY, fldGlobalBase.angle);
            }
        }
        rsetTest(1, 0, 128, 1, 0, 0, 1, 1);
    }
    rsetOtOfs(0x5E);
    rsetAlpha(0);
    VU0_LOAD_MATRIX(&sdfViewMatrix);
}
#else
INCLUDE_ASM(const s32, "game/code_001411F0", func_00144D30);
#endif

extern void fldAllIconDel(s32 index, f32 *x, f32 *z);

extern KwlnDrawVectorParams kwlnOverlayFrustumParameters;



extern u128 kwlnOverlayEyeVector;

extern u128 kwlnOverlayUpVector;

extern s32 D_003BAEB8;

extern s32 D_003BAEBC;

extern s32 D_003BAEC0;

extern s32 D_003BAEC4;

extern s32 D_003BAEC8;

extern s32 D_003BAED0;

extern s32 D_003BAECC;

extern s32 D_003BAED4;

extern u8 fldAutoMapSeqTestFlag(u32 mask);
extern void fldAutoMapSeqSetFlag(u32);
extern void evtDisableMoonDraw(void);
extern KwlnDrawVectorParams kwlnDrawVector;
extern SdfMathVector4 D_003A05D8[];

/* Enter the field scene once: hide the world object, load the scene's two
 * map textures, centre the camera on the floor's entry point and set the
 * scene fog and camera offsets. */
void func_00145B18(void) {
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
    D_003D40A0[2] = sdfTexSet(cam->mapResources[2].resourceData);
    D_003D40A0[3] = sdfTexSet(cam->mapResources[3].resourceData);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->floor;
    AutoMapSeq = FLD_SCENE_READY;
    D_003BAEB8 = cam->flagStage;
    D_003BAED0 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    D_003BAECC = 0;
    ix = cx + entry[0];
    iz = cz + entry[1];
    D_003BAEC4 = ix;
    D_003BAEC8 = iz;
    D_003BAEC0 = iz;
    D_003BAEBC = ix;
    fldAutoMapSeqSetFlag(1);
    fldResetPlayerSceneObjectState();
    evtDisableMoonDraw();
    memcpy(&up, &D_003A05D8[0], sizeof(up));
    focus[1] = 0.0f;
    focus[0] = D_003BAEBC;
    focus[2] = D_003BAEC0;
    eye[0] = D_003BAEBC;
    eye[1] = -24000.0f;
    eye[2] = D_003BAEC0;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_00324A20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, &up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x108010;
    D_00324980.camera.offsetX = 2048.0f;
    D_00324980.camera.offsetY = 2048.0f;
    kwlnDrawVector.fogNear = 255.0f;
    kwlnDrawVector.depthNear = 1000.0f;
    kwlnDrawVector.fogFar = 255.0f;
    kwlnDrawVector.depthFar = 20000.0f;
    kwlnDrawVector.fogColor = 0x108010;
}
extern void frSetFontDrawOT(u32);

void fldReleaseTitleSlots(void) {
    if (D_003D40A0[2] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[2]);
    }
    if (D_003D40A0[3] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[3]);
    }
    D_003D40A0[2] = 0;
    D_003D40A0[3] = 0;
    while (sdfDelayedResrcCheck() != 0) {
        sdfDrawSync();
    }
    AutoMapSeq = FLD_SCENE_NOT_READY;
    frSetFontDrawOT(0x54);
}

extern sdfModelRootNode_t *D_00348F30[];

extern FldPoint D_003D40B0[];


void fldReleaseMenuSlots(void) {
    s32 i;

    for (i = 0; i < fldAutoMapData; i++) {
        if (D_00348F30[i] != 0) {
            sdfModelRootFree(D_00348F30[i],
                             SDF_MODEL_ROOT_FREE_DEFERRED,
                             SDF_MODEL_MATERIAL_RESOURCES_RELEASE_DEFERRED);
        }
    }
    for (i = 0; i < 64; i++) {
        D_00348F30[i] = 0;
        D_003D40B0[i].x = 0.0f;
        D_003D40B0[i].y = 0.0f;
        D_003D40B0[i].z = 0.0f;
    }
    if (D_003D40A0[0] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[0]);
    }
    D_003D40A0[0] = 0;
    if (D_003D40A0[1] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[1]);
    }
    D_003D40A0[1] = 0;
    while (sdfDelayedResrcCheck() != 0) {
        sdfDrawSync();
    }
    AutoMapSeq = FLD_SCENE_NOT_READY;
}

extern void fldAutoMapSeqClearFlag(s32);


extern SdfMathVector4 D_003A05D8[]; /* default camera up vectors (3 copies), the first read by func_00145B18 */

extern void fldApplySkyLightSetToPlayerVU(void);


/* Enters the field camera state for a fresh scene: releases the title slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldAreaWork *cam = &fldGlobalBase;
    f32 focus[4];
    f32 eye[4];
    SdfMathVector4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 cx;
    s32 cz;

    sdfDrawSync();
    sdfDrawSync();
    fldReleaseTitleSlots();
    AutoMapSeq = FLD_SCENE_NOT_READY;
    fldAutoMapSeqClearFlag(1);
    fldPreparePlayerSceneCameraTarget();
    evtSetSolarOverlayFullyVisible();
    fldApplySkyLightSetToPlayerVU();
    cam->sceneTransitionCooldown = 4;
    frSetFontDrawOT(0x54);
    dds3VisibleSceneBasic((dds3Basic_t *)(u32)dds3GetActiveScene(), 1);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->floor;
    D_003BAEB8 = cam->flagStage;
    D_003BAED0 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    iz = cz + entry[1];
    ix = cx + entry[0];
    memcpy(&up, &D_003A05D8[1], sizeof(up));
    D_003BAECC = 0;
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_00324A20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, &up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x108010;
    D_00324980.camera.offsetX = 2244.0f;
    D_00324980.camera.offsetY = 2118.0f;
}

/* Loads the scene's models into the menu slots, then centers the camera on the
 * scene's entry point. */
void fldLoadSceneModelsAndCamera(void) {
    FldAreaWork *cam;
    const FldSceneRecord *rec;
    f32 focus[4];
    f32 eye[4];
    SdfMathVector4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 cx;
    s32 cz;
    s32 i;

    rec = fldAutoMapHeader;
    for (i = 0; i < fldAutoMapData; i++, rec++) {
        D_00348F30[i] = sdfModelMakeEarly(0, rec->model);
        D_003D40B0[i].x = rec->pos->x;
        D_003D40B0[i].y = rec->pos->y;
        D_003D40B0[i].z = rec->pos->z;
    }
    cam = &fldGlobalBase;
    D_003D40A0[0] = sdfTexSet(cam->mapResources[0].resourceData);
    D_003D40A0[1] = sdfTexSet(cam->mapResources[1].resourceData);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->floor;
    D_003BAEB8 = cam->flagStage;
    D_003BAED0 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    iz = cz + entry[1];
    ix = cx + entry[0];
    memcpy(&up, &D_003A05D8[2], sizeof(up));
    D_003BAECC = 0;
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_00324A20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, &up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x108010;
    D_00324980.camera.offsetX = 2244.0f;
    D_00324980.camera.offsetY = 2118.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfDrawSync();
    sdfDrawSync();
    fldReleaseMenuSlots();
}

FldSceneReadyState fldGetSceneReadyFlag(void) {
    return AutoMapSeq;
}

/* Handle automap input, clamp the grid position, and refresh its camera. */
INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A05D8);

void fldUpdateAutomapInput(void) {
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 minX, maxZ, maxX, minZ;
    s32 closeRequested = 0;

    if (fldGetSceneReadyFlag() == FLD_SCENE_NOT_READY) {
        return;
    }
    if (D_0032E5C4[0] != 0) {
        if ((s8)dds3PadButtonState[1][0][2] != 0) {
            closeRequested = 1;
        }
    } else if ((s8)dds3PadButtonState[1][0][2] < 0) {
        closeRequested = 1;
    }

    /* A close request does not advance the input-delay counter. */
    if (closeRequested == 0) {
        D_003BAECC++;
    }
    if (closeRequested != 0 ||
        ((s8)dds3PadButtonState[1][0][3] < 0 && D_003BAED4 == 0)) {
        fldEnterSceneCamera();
        dds3FadeOut2(0);
        dds3FadeIn2(8);
        return;
    }
    if ((s8)dds3PadButtonState[1][0][3] < 0) {
        p2sdPlaySE(10, 127, 63);
        D_003BAEBC = D_003BAEC4;
        D_003BAEC0 = D_003BAEC8;
        D_003BAEB8 = D_003BAED0;
        D_003BAED4 = 0;
    }

    if (D_003BAECC >= 11) {
        if ((s8)D_003BA878[0][1] != 0) {
            if (D_003BA878[0][1] & 2) {
                if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                    (s8)dds3PadButtonState[1][0][7] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 -= 600;
                } else if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                           (s8)dds3PadButtonState[1][0][6] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 += 600;
                } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                           (s8)dds3PadButtonState[1][0][7] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 -= 600;
                } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                           (s8)dds3PadButtonState[1][0][6] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 += 600;
                } else if ((s8)dds3PadButtonState[1][0][5] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                } else if ((s8)dds3PadButtonState[1][0][4] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                } else if ((s8)dds3PadButtonState[1][0][7] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEC0 -= 600;
                } else if ((s8)dds3PadButtonState[1][0][6] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEC0 += 600;
                }
            }
        } else {
            /* A held diagonal consumes the direction choice even when its
             * Z-repeat bit is absent; do not fall back to a cardinal step. */
            if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                (s8)dds3PadButtonState[1][0][7] != 0) {
                if (dds3PadButtonState[1][0][7] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 -= 600;
                }
            } else if ((s8)dds3PadButtonState[1][0][5] != 0 &&
                       (s8)dds3PadButtonState[1][0][6] != 0) {
                if (dds3PadButtonState[1][0][6] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 += 600;
                }
            } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                       (s8)dds3PadButtonState[1][0][7] != 0) {
                if (dds3PadButtonState[1][0][7] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 -= 600;
                }
            } else if ((s8)dds3PadButtonState[1][0][4] != 0 &&
                       (s8)dds3PadButtonState[1][0][6] != 0) {
                if (dds3PadButtonState[1][0][6] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 += 600;
                }
            } else if (dds3PadButtonState[1][0][5] & 2) {
                D_003BAED4 = 1;
                D_003BAEBC += 600;
            } else if (dds3PadButtonState[1][0][4] & 2) {
                D_003BAED4 = 1;
                D_003BAEBC -= 600;
            } else if (dds3PadButtonState[1][0][7] & 2) {
                D_003BAED4 = 1;
                D_003BAEC0 -= 600;
            } else if (dds3PadButtonState[1][0][6] & 2) {
                D_003BAED4 = 1;
                D_003BAEC0 += 600;
            }
        }
    }

    fldAutoMapGetBBox(&minX, &maxZ, &maxX, &minZ);
    /* Quantize the two X endpoints, then the two Z endpoints. */
    minX = (s32)((minX - 600.0f) / 600.0f) * 600;
    maxX = (s32)((maxX + 600.0f) / 600.0f) * 600;
    maxZ = (s32)((maxZ - 600.0f) / 600.0f) * 600;
    minZ = (s32)((minZ + 600.0f) / 600.0f) * 600;

    if ((f32)D_003BAEBC < minX) {
        D_003BAEBC = (s32)minX;
    }
    if (maxX < (f32)D_003BAEBC) {
        D_003BAEBC = (s32)maxX;
    }
    if ((f32)D_003BAEC0 < minZ) {
        D_003BAEC0 = (s32)minZ;
    }
    if (maxZ < (f32)D_003BAEC0) {
        D_003BAEC0 = (s32)maxZ;
    }

    focus[0] = D_003BAEBC;
    focus[1] = 0.0f;
    focus[2] = D_003BAEC0;
    eye[0] = D_003BAEBC;
    eye[1] = -24000.0f;
    eye[2] = D_003BAEC0;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_00324A20, focus);
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

    D_003BAEB4 = cam->floor;
    D_003BAEB8 = cam->flagStage;
    D_003BAED0 = cam->flagStage;
    fldAllIconDel(cam->floor, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    SDF_VU0_COPYVECTOR(&kwlnOverlayEyeVector, eye);
    SDF_VU0_COPYVECTOR(&D_00324A20, focus);
    SDF_VU0_COPYVECTOR(&kwlnOverlayUpVector, up);
    kwlnOverlayFrustumParameters.fogNear = 255.0f;
    kwlnOverlayFrustumParameters.depthNear = 1000.0f;
    kwlnOverlayFrustumParameters.fogFar = 255.0f;
    kwlnOverlayFrustumParameters.depthFar = 20000.0f;
    kwlnOverlayFrustumParameters.fogColor = 0x808080;
}

/* Stores the area, floor/room and third location selection, and selects the
 * area's flag table. The third selection's interpretation is left unknown. */
void fldSetSceneLocation(s32 area, s32 floor, s32 stage) {
    fldGlobalBase.area = area;
    fldGlobalBase.unk18 = stage;
    D_003BAEB4 = fldGlobalBase.floor = floor;
    fldAutoMapField = area % 100;
}


extern s32 D_0032C900[];

/* The area's last two digits map to a save-table row; scene-record and entry
 * indexes are zero-based. */
FldAutoMapEntryDiscoveryState fldGetFloorFlag(
    s32 area, FldAutoMapSceneRecordIndex floor,
    FldAutoMapEntryIndex bit) {
    s32 areaIndex = D_0032C900[area % 100];
    if (areaIndex == -1) return FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED;
    return (DDS3_GBWK->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetFlagAndFindRecord(s32 flagNumber) {
    s32 areaIndex;
    s32 bit;

    if (flagNumber > 0) {
        areaIndex = D_0032C900[fldAutoMapField % 100];
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
void fldSetFlagBit(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_0032C900[area % 100];
    if (areaIndex != -1) {
        DDS3_GBWK->areaFlags[areaIndex][floor] |= 1ULL << bit;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_0032C900[area % 100];
    if (areaIndex != -1) {
        DDS3_GBWK->areaFlags[areaIndex][floor] &= ~(1ULL << bit);
    }
}

void fldAutoMapNameU(s32 labelIndex) {
    if (labelIndex >= 0x40) {
        D_0032E474[0] = -1;
    } else {
        D_0032E474[0] = labelIndex;
    }
}

void fldAutoMapNameD(s32 labelIndex) {
    if (labelIndex >= 0x40) {
        D_0032E478[0] = -1;
    } else {
        D_0032E478[0] = labelIndex;
    }
}


extern s32 fldAutoMap_GetFloor(s32 scene, u32 index);

s32 fldAutoMapGetFloorPrev(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = -1;
    const FldSceneRecord *entry = fldAutoMapHeader;

    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldGetFloorFlag(fldAutoMapField, i, j) != FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED) {
                item = fldAutoMap_GetFloor(i, j);
                if (item < limit && best < item) {
                    best = item;
                }
            }
        }
    }
    if (best == -1) {
        return limit;
    }
    return best;
}

s32 fldAutoMapGetFloorNext(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = 999;
    const FldSceneRecord *entry = fldAutoMapHeader;

    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldGetFloorFlag(fldAutoMapField, i, j) != FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED) {
                item = fldAutoMap_GetFloor(i, j);
                if (limit < item && item < best) {
                    best = item;
                }
            }
        }
    }
    if (best == 999) {
        return limit;
    }
    return best;
}

void fldAllIconDel(s32 index, f32 *x, f32 *z) {
    s32 i;
    const FldSceneRecord *entry = fldAutoMapHeader;
    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        if (i == index) {
            const FldPoint *position = entry->pos;
            *x = position->x;
            *z = position->z;
            return;
        }
    }
    *x = 0.0f;
    *z = 0.0f;
}

extern s32 D_003BAEAC;

void fldAutoMapGetBBox(f32 *minX, f32 *maxZ, f32 *maxX, f32 *minZ) {
    s32 room;
    u32 i;
    const FldSceneRecord *rec;
    const FldItem *item;
    const FldPoint *a;
    s32 visible;
    const FldPoint *b;

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
            if (fldGetFloorFlag(fldAutoMapField, room, i) != FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED) {
                visible = 1;
            }
            if (D_003BAEAC != 0) {
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
    if (D_0032E3C0[0] >= 200) {
        return;
    }
    if (AutoMapSeq == FLD_SCENE_READY) {
        fldUpdateAutomapInput();
    }
}

void fldDrawAutomapWhenReady(void) {
    if (AutoMapSeq == FLD_SCENE_READY) {
        fldDrawFieldBackdropFrame();
        fldDrawAutomapScene();
    }
}

void fldResetCameraAndSceneView(void) {
    fldCenterCameraOnEntry();
    func_00144D30();
}

void fldClearAllAreaFloorFlags(void) {
    u64 *words;
    s32 remaining;
    DatGameState *block;
    s32 index;

    index = 0;
    block = DDS3_GBWK;
    do {
        words = block->areaFlags[index];
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        index = index + 1;

    } while (index < 0xd);
}

extern f32 D_003BAEE8;
extern s32 frFontMeasureTextAdvance(const u8 *, u8, u8);
extern void fldKanjiDraw(s32 x, s32 y, u32 colors, const u8 *text);
void fldDrawMapLabelBanner(s32 unused) {
    FldAreaWork *work;
    s32 mode;
    s32 map;
    FldSecondaryMapLabelRecordIndex secondaryRecord;
    FldMapLabelRecordIndex primaryRecord;
    s32 lookupIndex;
    s32 textWidth;
    s32 x;
    s32 y;
    f32 alpha;
    f32 positionX, positionY;
    s32 xOffset;
    s32 yOffset;

    if (fldGlobalBase.playerModelAlternate == FLD_PLAYER_MODEL_ALTERNATE_ENABLED) {
        return;
    }
    if (fldGlobalBase.area >= 200) {
        return;
    }

    mode = fldGlobalBase.mapBannerFadeMode;
    switch (mode) {
    case 0:
        alpha = 0.0f;
        D_003BAEE8 = alpha;
        break;
    case 2:
        alpha = D_003BAEE8;
        if (alpha > 0.0f) {
            alpha -= 0.1f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
                /* Retail 0x00147228 bc1tl .L00147280: the clamp store has one entry
                 * and falls into interpolate. */
                goto storeClampedAlpha;
            }
            D_003BAEE8 = alpha;
        }
        break;
    case FLD_MAP_BANNER_FADE_IN:
        alpha = D_003BAEE8;
        if (!(alpha < 1.0f)) {
            break;
        }
        alpha += 0.1f;
        if (!(1.0f < alpha)) {
            D_003BAEE8 = alpha;
            break;
        }
        /* A completed fade-in uses the fully visible state. */
    case FLD_MAP_BANNER_FADE_VISIBLE:
        alpha = 1.0f;
        D_003BAEE8 = alpha;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    default:
        alpha = D_003BAEE8;
        break;
    }

    /* Retail 0x00147278 b .L00147284: interpolate is shared by eight
     * branches and the store block fall-through. */
    goto interpolate;
storeClampedAlpha:
    D_003BAEE8 = alpha;
interpolate:
    positionX = alpha * 0.0f + (1.0f - alpha) * 250.0f;
    positionY = alpha * 0.0f + (1.0f - alpha) * 0.0f;
    xOffset = (s32)positionX;
    yOffset = (s32)positionY;

    work = &fldGlobalBase;
    map = work->secondaryMapLabelSelection;
    if (map != -1) {
        if (D_003D4460[map] < 2) {
            return;
        }
        {
            s32 queryY = map;
            s32 queryX = work->area % 100;
            secondaryRecord = fldFindSecondaryMapLabelIndexByCoordinate(queryX, queryY);
        }
        textWidth = frFontMeasureTextAdvance(D_00342868[secondaryRecord].name, 1, 0);
        fldDrawAnimatedFieldBanner(textWidth, xOffset, yOffset);
        if (textWidth >= 0x9E) {
            x = 0x155;
        } else {
            x = 0x1A3 - textWidth / 2;
        }
        y = (yOffset + 0x12) / 2;
        fldKanjiDraw(xOffset + x, y, 0xF0DC6480, D_00342868[secondaryRecord].name);
    } else {
        lookupIndex = work->primaryMapLabelSelection;
        if (lookupIndex == map || D_003D43B0[lookupIndex] < 2) {
            return;
        }
        {
            s32 queryY = lookupIndex;
            s32 queryX = work->area % 100;
            primaryRecord = fldFindMapLabelIndexByCoordinate(queryX, queryY);
        }
        textWidth = frFontMeasureTextAdvance(D_0033F068[primaryRecord].name, 1, 0);
        fldDrawAnimatedFieldBanner(textWidth, xOffset, yOffset);
        if (textWidth >= 0x9E) {
            x = 0x155;
        } else {
            x = 0x1A3 - textWidth / 2;
        }
        y = (yOffset + 0x12) / 2;
        fldKanjiDraw(xOffset + x, y, 0xA09DC380, D_0033F068[primaryRecord].name);
    }
}

extern struct sdfMemHandle_t *fldEffectTextureLoadHandles[];

extern s32 fldEffectTextureData[];

extern EffNode *fldEffectTextureNodes[];

extern s32 D_0034C8A0[];


extern char *D_003A06B0[4]; /* {"/fld/f/bin/KUT_3Z.EPL", "ASI_2MZ", "ASI_2HZ", "MAN_2Z"} */

void fldLoadFieldEffectTextureSlots(void) {
    char *names[4];
    s32 i;

    memcpy(names, D_003A06B0, sizeof(names));
    for (i = 0; i < 4; i++) {
        fldEffectTextureLoadHandles[i] = sdfDevLoadFileEx(names[i], (u32 *)&fldEffectTextureData[i], 0);
        fldEffectTextureNodes[i] = effCreateNodeFromParameter(fldEffectTextureData[i]);
        D_0034C8A0[i] = 0;
    }
}

extern struct sdfMemHandle_t *fldEffectTextureLoadHandles[4];

extern s32 fldEffectTextureData[4];

extern EffNode *fldEffectTextureNodes[4];

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

extern dds3Basic_t *fldPlayerObject;

extern f32 EulerAngleAxisY(void);

extern void SetRotateMatrixY(f32 angle);

void fldUpdateEffectTextureTransforms(void) {
    f32 vec[4];
    u8 mat[64];
    f32 angle;
    s32 i;

    memset(vec, 0, sizeof(vec));
    vec[3] = 1.0f;
    dds3GetRotPoseSet(fldPlayerObject);
    angle = EulerAngleAxisY();
    for (i = 0; i < 4; i++) {
        if (fldEffectTextureNodes[i] != 0) {
            vec[0] = fldGlobalBase.x;
            vec[2] = fldGlobalBase.z;
            vec[1] = fldGlobalBase.y;
            if (i == 3) {
                vec[1] = fldGlobalBase.y - 100.0f;
            }
            effCopyVectorToNodeInstance(fldEffectTextureNodes[i], vec);
            SetRotateMatrixY(-angle);
            VU0_STORE_MATRIX_UNCLOBBERED(mat);
            effApplyNodeTransformMatrix(fldEffectTextureNodes[i], mat);
            dds3EffectDispatchUpdate(fldEffectTextureNodes[i]);
        }
    }
}

extern EffNode *mnuPositionedResourceNodes[];

extern s32 mnuPositionedResourceActive[];

extern s32 D_003BAF88;

void mnuInitializeResourceEntries(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        mnuPositionedResourceNodes[i] = effCreateNodeFromParameter(D_003BAF88);
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
    s32 index;
    for (index = 0; index < 4; index++) {
        if (mnuPositionedResourceNodes[index] != 0 && mnuPositionedResourceActive[index] != 0) {
            dds3EffectDispatchUpdate(mnuPositionedResourceNodes[index]);
        }
    }
}




extern s32 D_003BD7EC, D_003BD7F0, D_003BAF68, D_003BAF6C, D_003BAF50, D_003BAF54;

extern s32 D_003BAF74, D_003BAF78, D_003BAF5C, D_003BAF60, D_003BAF84, D_003BAF88;

extern s32 D_003BAF44, D_003BAF48, D_003BD7D4, D_003BD7D8, D_003BD7DC, D_003BD7E0;

extern s32 D_003BD7E4, D_003BD7E8;

/* Retains fldmix.LB node values and their memory blocks in field work.
 * Nodes 10..13 supply the map resources later acquired by the scene camera. */
INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0650);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0668);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0680);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0698);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A06B0);

void fldParseMixLb(void) {
    FilePacRequest *lb;
    PacWork *node;
    u32 index;
    struct sdfMemHandle_t *allocation;

    index = 0;
    lb = (FilePacRequest *)pacLoad("/fld/f/bin/fldmix.LB");
    pacSync((struct FileRequest *)lb);
    for (node = lb->packet.queueHead; node != NULL; node = node->next, index++) {
        switch (index) {
        case 0:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BD7EC = (s32)(u32)allocation;
            D_003BD7F0 = sdfMemGetPtr(allocation);
            break;
        case 1:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BAF68 = (s32)(u32)allocation;
            D_003BAF6C = sdfMemGetPtr(allocation);
            break;
        case 2:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BAF50 = (s32)(u32)allocation;
            D_003BAF54 = sdfMemGetPtr(allocation);
            break;
        case 3:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BAF74 = (s32)(u32)allocation;
            D_003BAF78 = sdfMemGetPtr(allocation);
            break;
        case 4:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BAF5C = (s32)(u32)allocation;
            D_003BAF60 = sdfMemGetPtr(allocation);
            break;
        case 5:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BAF84 = (s32)(u32)allocation;
            D_003BAF88 = sdfMemGetPtr(allocation);
            break;
        case 6:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BAF44 = (s32)(u32)allocation;
            D_003BAF48 = sdfMemGetPtr(allocation);
            break;
        case 7:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BD7D4 = (s32)(u32)allocation;
            D_003BD7D8 = sdfMemGetPtr(allocation);
            break;
        case 8:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BD7DC = (s32)(u32)allocation;
            D_003BD7E0 = sdfMemGetPtr(allocation);
            break;
        case 9:
            allocation = PAC_WORK_ALLOCATION(node);
            D_003BD7E4 = (s32)(u32)allocation;
            D_003BD7E8 = sdfMemGetPtr(allocation);
            break;
        case 10:
            allocation = PAC_WORK_ALLOCATION(node);
            fldGlobalBase.mapResources[0].allocation = allocation;
            fldGlobalBase.mapResources[0].resourceData =
                (const struct SdfTextureFileHeader *)sdfMemGetPtr(allocation);
            break;
        case 11:
            allocation = PAC_WORK_ALLOCATION(node);
            fldGlobalBase.mapResources[1].allocation = allocation;
            fldGlobalBase.mapResources[1].resourceData =
                (const struct SdfTextureFileHeader *)sdfMemGetPtr(allocation);
            break;
        case 12:
            allocation = PAC_WORK_ALLOCATION(node);
            fldGlobalBase.mapResources[2].allocation = allocation;
            fldGlobalBase.mapResources[2].resourceData =
                (const struct SdfTextureFileHeader *)sdfMemGetPtr(allocation);
            break;
        case 13:
            allocation = PAC_WORK_ALLOCATION(node);
            fldGlobalBase.mapResources[3].allocation = allocation;
            fldGlobalBase.mapResources[3].resourceData =
                (const struct SdfTextureFileHeader *)sdfMemGetPtr(allocation);
            break;
        }
    }
    pacFileHandleFree_(lb);
}

extern s32 D_003BAF48, D_003BAF40;

extern EffNode *D_003BAF4C, *D_003BAF3C;

extern sdfTexHandle_t *D_003BAF30, *D_003BAF34, *D_003BAF38;

extern s32 D_003BD7F0, D_003BD7D8, D_003BD7E0, D_003BD7E8;

void fldInitializeMenuResources(void) {
    if (D_0032E3C0[0] < 200) {
        D_003BAF4C = dds3EffectLoad((EffNodeDescriptor *)(u32)D_003BAF48);
        D_003BAF3C = dds3EffectLoad((EffNodeDescriptor *)(u32)D_003BD7F0);
        D_003BAF40 = 0;
        D_003BAF30 = sdfTexSet((SdfTextureFileHeader *)(D_003BD7D8));
        D_003BAF34 = sdfTexSet((SdfTextureFileHeader *)(D_003BD7E0));
        D_003BAF38 = sdfTexSet((SdfTextureFileHeader *)(D_003BD7E8));
    }
}

extern EffNode *D_003BAF58;

extern EffNode *D_003BAF64;

void fldReleaseResourceHandles(void) {
    if (D_003BAF30 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF30);
        D_003BAF30 = 0;
    }
    if (D_003BAF34 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF34);
        D_003BAF34 = 0;
    }
    if (D_003BAF38 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF38);
        D_003BAF38 = 0;
    }
    if (D_003BAF4C != 0) {
        dds3EffectDelete(D_003BAF4C);
        D_003BAF4C = 0;
    }
    if (D_003BAF58 != 0) {
        dds3EffectDelete(D_003BAF58);
        D_003BAF58 = 0;
    }
    if (D_003BAF64 != 0) {
        dds3EffectDelete(D_003BAF64);
        D_003BAF64 = 0;
    }
    if (D_003BAF3C != 0) {
        dds3EffectDelete(D_003BAF3C);
        D_003BAF3C = 0;
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
        fldModelSlotTable[i].lodIndex = 0;
    }
    fldNpcModelTable = 0;
}

extern EffNode *D_003BAF70;

extern EffNode *D_003BAF7C;


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
    D_003BAF70 = 0;
    D_003BAF7C = 0;
}

extern void sdfModelSetFaceDivParm(struct sdfModelRootNode_t *model, f32 first, f32 second);
extern s32 D_0032E3C4[];
extern FldNpcPalette D_003D44A0;
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
    index = D_0032E3C4[0] + 1;
    if (index >= 64) {
        index = 63;
    }
    row = &D_003D44A0.entries[index];
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

extern FldNpcPlacementRule D_00349030[400];
extern void FromRotation(f32 angle);
extern dds3Basic_t *dds3AddUnitObjectBasic(s32 value, u128 *position, u128 *rotation);
extern void dds3RebuildObjectModelContext(dds3Basic_t *node, s32 kind, s32 resource);
extern void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value);
extern u32 sdfGetLodChunkValue(sdfModelRootNode_t *model);
extern void fldSpawnNpcTrigger(f32 x, f32 y, f32 z, f32 height, const char *name, s32 arg1, s32 room, s32 index);

void func_001480D0(u32 key, f32 *orientation, f32 x, f32 y, f32 z,
                   s32 room, const char *name, s32 argument) {
    f32 position[4];
    f32 rotation[4];
    s32 outputIndex;
    s32 kind, resource, motion, lod;
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
        outputIndex = fldNpcModelTable;
        fldModelSlotTable[outputIndex].x = x;
        fldModelSlotTable[outputIndex].y = y;
        fldModelSlotTable[outputIndex].z = z;
        fldModelSlotTable[outputIndex].orientation[0] = orientation[0];
        fldModelSlotTable[outputIndex].orientation[1] = orientation[1];
        fldModelSlotTable[outputIndex].orientation[2] = orientation[2];
        fldModelSlotTable[outputIndex].placementId = key;
        fldModelSlotTable[outputIndex].orientation[3] = orientation[3];
        fldModelSlotTable[outputIndex].unk2C = 0.0f;
        fldModelSlotTable[outputIndex].unk30 = 0;
        fldModelSlotTable[outputIndex].unk3C = 0;
        fldModelSlotTable[outputIndex].object = NULL;
        fldModelSlotTable[outputIndex].unk34 = 0;
        fldModelSlotTable[outputIndex].room = room;
        strcpy(fldModelSlotTable[outputIndex].name, name);
        for (i = 0; i < 400; i++) {
            if (D_00349030[i].area == fldGlobalBase.area &&
                D_00349030[i].floor == fldGlobalBase.floor + 1 &&
                strcmp(D_00349030[i].name, name) == 0) {
                if (D_00349030[i].requiredFlagA != 0 && !evtBitCheck(D_00349030[i].requiredFlagA)) return;
                if (D_00349030[i].requiredFlagB != 0 && !evtBitCheck(D_00349030[i].requiredFlagB)) return;
                if (D_00349030[i].requiredFlagC != 0 && !evtBitCheck(D_00349030[i].requiredFlagC)) return;
                behavior = D_00349030[i].behavior;
                kind = D_00349030[i].kind;
                resource = D_00349030[i].resource;
                motion = D_00349030[i].motion;
                lod = D_00349030[i].lod;
                if (D_00349030[i].alternateFlag != 0 && evtBitCheck(D_00349030[i].alternateFlag)) {
                    resource = D_00349030[i].alternateResource;
                    motion = D_00349030[i].alternateMotion;
                    lod = D_00349030[i].alternateLod;
                }
                break;
            }
        }
        fldModelSlotTable[outputIndex].kind = kind;
        fldModelSlotTable[outputIndex].kindId = resource;
        fldModelSlotTable[outputIndex].lodIndex = lod;
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
        dds3SetSclPoseSet(object, (u128 *)scale);
        if (kind == 3 && resource == 8) dds3GetRadiusPoseSet(object, 50.0f);
        model = dds3ObjectBaseGetModelHandle(object);
        model->first->frameStep = 1.0f;
        mdlLoopMotion(model, 0, motion);
        inner = model->inner;
        sdfGetLodChunkValue(inner);
        inner->lodIndex = lod;
        dds3ObjectBaseOnFlag(object, 0x400);
        fldInitializeHuntSparkUnit(evtGetUnitHandle2(object));
        fldAddItemBoxObject(x, y, z, radius, hitHeight, room);
        fldSpawnNpcTrigger(x, y, z, height, name, argument, room, fldNpcModelTable);
        fldNpcModelTable++;
    }
}

extern FldActivationEntry D_0034C8F0[256];
extern u8 fldTestObjectActivationFlag(u32 flagIndex);
extern void fldSpawnRoomTrigger(s32 roomId, f32 x, f32 y, f32 z, f32 radius);

void func_001486D0(u32 id, f32 *rotation, f32 x, f32 y, f32 z) {
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
        switch (D_0034C8F0[id].kind) {
        case 0:
        case 3:
            if (D_003BAF70 == 0) {
                D_003BAF70 = dds3EffectLoad((EffNodeDescriptor *)(u32)D_003BAF6C);
                fldTakaraSlotTable[fldTakaraCount].effectNode = D_003BAF70;
            } else {
                fldTakaraSlotTable[fldTakaraCount].effectNode =
                    dds3EffectCopy(D_003BAF70);
            }
            modelId = 21;
            break;
        case 1:
            if (D_003BAF7C == 0) {
                D_003BAF7C = dds3EffectLoad((EffNodeDescriptor *)(u32)D_003BAF78);
                fldTakaraSlotTable[fldTakaraCount].effectNode = D_003BAF7C;
            } else {
                fldTakaraSlotTable[fldTakaraCount].effectNode =
                    dds3EffectCopy(D_003BAF7C);
            }
            modelId = 22;
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
        fldTakaraSlotTable[fldTakaraCount].activationKind = D_0034C8F0[id].kind;
        fldTakaraSlotTable[fldTakaraCount].effectKindIndex = id;
        fldTakaraCount++;
        fldAddItemBoxObject(x, y, z, 20.0f, 100.0f, -1);
        floorIndex = D_0032E3C4[0] + 1;
        if (floorIndex >= 64) {
            floorIndex = 63;
        }
        row = &D_003D44A0.entries[floorIndex];
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

extern u8 fldEncountTestFlag(u32 mask);

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
                switch (fldTakaraSlotTable[i].activationKind) {
                case 0:
                case 3:
                    if (D_003BAF58 != 0) {
                        dds3EffectDelete(D_003BAF58);
                        D_003BAF58 = 0;
                    }
                    D_003BAF58 = dds3EffectLoad((EffNodeDescriptor *)(u32)D_003BAF54);
                    dds3EffectReset(D_003BAF4C);
                    break;
                case 1:
                    if (D_003BAF64 != 0) {
                        dds3EffectDelete(D_003BAF64);
                        D_003BAF64 = 0;
                    }
                    D_003BAF64 = dds3EffectLoad((EffNodeDescriptor *)(u32)D_003BAF60);
                    dds3EffectReset(D_003BAF4C);
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
                if (D_003BAF58 != 0) {
                    dds3EffectDelete(D_003BAF58);
                    D_003BAF58 = 0;
                }
                if (D_003BAF64 != 0) {
                    dds3EffectDelete(D_003BAF64);
                    D_003BAF64 = 0;
                }
            }
            break;
        }
    }
}

extern void fldSpawnRoomTriggerVariant(s32 kind, s32 roomValue,
                                       f32 x, f32 y, f32 z, f32 radius);

void func_00148FF0(s32 kind, u32 id, f32 *rotation, f32 x, f32 y, f32 z) {
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
            dds3RebuildObjectModelContext(object, 2, 3);
        } else {
            dds3SetUnitObjectId(object, "HEAL_UNIT");
            dds3RebuildObjectModelContext(object, 2, 4);
        }
        dds3SetSclPoseSet(object, (u128 *)scale);
        dds3ObjectBaseOnFlag(object, 0x100);
        resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(object);
        mdlLoopMotion(resource, 0, 0);
        fldAddItemBoxObject(x, y, z, 28.0f, 400.0f, -1);
        floorIndex = D_0032E3C4[0] + 1;
        if (floorIndex >= 64) {
            floorIndex = 63;
        }
        row = &D_003D44A0.entries[floorIndex];
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


extern s32 D_0032E4C8[];

extern f32 fldBannerColorPhase;

extern void DrwSprite2DC(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, sdfTexHandle_t *);

void fldDrawAnimatedFieldBanner(s32 alpha, s32 x, s32 y) {
    u32 color;

    if (D_0032E4C8[0] != 1) {
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(x + 0x123, y, 0x7D, 0x34, 1, 0x31, 0x7D, 0x34, 0x80808080, D_003BAF30);
        DrwSprite2D(x + 0x1A0, y, 0x62, 0x34, 0x7D, 0x31, 1, 0x34, 0x80808080, D_003BAF30);
        if (ptyAnyLivingUnitHasStatus(0x5D0, PTY_PARTY_STATUS_SCOPE_ALL_LIVING) != 0) {
            color = 0x808080;
            DrwSprite2D(x + 0x17F, y + 0x28, 0x73, 0x1A, 1, 0x66, 0x73, 0x1A, 0x80808080, D_003BAF30);
            if (fldBannerColorPhase < 45.0f) {
                color = (s32)(sdfSin(fldBannerColorPhase * 4.0f * 3.14f / 180.0f) * 128.0f) + 0x80;
                color |= (color << 8) | (color << 16);
            }
            rsetAlpha(1);
            DrwSprite2DC(x + 0x17F, y + 0x28, 0x73, 0x1A, 1, 0x66, 0x73, 0x1A, color, color | 0x5A000000, color | 0x5A000000, color, D_003BAF30);
            fldBannerColorPhase += 1.0f;
            if (fldBannerColorPhase > 90.0f) {
                fldBannerColorPhase = 0.0f;
            }
        }
        rsetAlpha(0);
    }
}

void fldDrawGaugeBar(s32 width) {
    s32 x;

    if (D_0032E3C0[0] < 200) {
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        x = 0x9D - (width >> 1);
        DrwSprite2D(x, 0x123, 0x63, 0x2E, 2, 1, 0x63, 0x2E, 0x80808080, D_003BAF30);
        DrwSprite2D(x + 0x63, 0x123, width, 0x2E, 0x64, 1, 1, 0x2E, 0x80808080, D_003BAF30);
        x += width;
        DrwSprite2D(x + 0x63, 0x123, 0x63, 0x2E, 0x65, 1, -0x63, 0x2E, 0x80808080, D_003BAF30);
        rsetAlpha(0);
    }
}

extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

void fldDrawInputCursor(s32 x, s32 y) {
    if (D_0032E3C0[0] < 200) {
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(x, y, 0x10, 0x11, 0x68, 1, 0x10, 0x11, 0x80808080, D_003BAF30);
        rsetAlpha(0);
    }
}

extern FldSparkDialogPhase fldGetSparkDialogPhase(void);
extern const s16 *fldFindLocationCoordinateRecord(s32 area, s32 locationIndex);
extern void fldSubmitGsGradientQuad(s32, s32, s32, s32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

void fldDrawLocationPanel(void) {
    FldAreaWork *work;
    u32 color;

    if (fldGetSparkDialogPhase() != FLD_SPARK_DIALOG_PHASE_IDLE) {
        return;
    }
    work = &fldGlobalBase;
    if (work->playerModelAlternate == FLD_PLAYER_MODEL_ALTERNATE_ENABLED ||
        work->deferredExit != FLD_DEFERRED_EXIT_IDLE) {
        return;
    }
    if (fldFindLocationCoordinateRecord(work->area, work->floor + 1)[2] <= 0) {
        return;
    }
    switch (work->overlayMode) {
    case FLD_SOLAR_OVERLAY_MODE_HIDE_MOON:
    case FLD_SOLAR_OVERLAY_MODE_MOON_FADE_OUT:
    case FLD_SOLAR_OVERLAY_MODE_PANEL_HIDDEN:
    case FLD_SOLAR_OVERLAY_MODE_PANEL_FADE_OUT:
        break;
    case FLD_SOLAR_OVERLAY_MODE_SHOW_MOON:
    case FLD_SOLAR_OVERLAY_MODE_MOON_FADE_IN:
    case FLD_SOLAR_OVERLAY_MODE_PANEL_VISIBLE:
    case FLD_SOLAR_OVERLAY_MODE_PANEL_FADE_IN:
        color = 0x80808080;
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitGsGradientQuad(0x194, 0x91, 0x5F, 0x47, 0, 0, 0, 0x26,
            0, 0, 0, 0x26, 0, 0, 0, 0x26, 0, 0, 0, 0x26);
        DrwSprite2D(0x188, 0x117, 0x78, 0x80, 1, 0, 0x78, 0x80, color, D_003BAF34);
        DrwSprite2D(0x188, 0x197, 0x78, 0x1C, 1, 0x7F, 0x78, 1, color, D_003BAF34);
        rsetAlpha(0);
        fldSubmitGsGradientQuad(0x190, 0x8F, 0x70, 0x49, 0, 0xD, 0xA2, 0x18,
            0, 0xD, 0xA2, 0, 0, 0xD, 0xA2, 0, 0, 0xD, 0xA2, 0);
        fldSubmitGsGradientQuad(0x193, 0x90, 0x6D, 0x4E, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x18);
        DrwSprite2D(0x1B4, 0x112, 0x1F, 0x10, 1, 1, 0x1F, 0x10, color, D_003BAF38);
        rsetAlpha(0);
        fldResetCameraAndSceneView();
        break;
    }
}

extern u8 fldAutoMapSeqTestFlag(u32 mask);

void fldFlushQueuedEffectPositions(void) {
    if (fldGlobalBase.area < 0xC8) {
        if (fldAutoMapSeqTestFlag(1) != 0) {
            return;
        }
        if (fldProcCampChk() != FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET) {
            return;
        }
        if (fldProcAmChk() != FLD_SCENE_TRANSITION_GATE_PROCEED) {
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

#ifdef NON_MATCHING
#include "dsp_entry_api.h"
#include "dsp_channel.h"
#include "evt_message_window_api.h"
#include "fld_area_work.h"
#include "fld_room_effect_entry.h"
#include "fld_object_slot.h"
#include "fld_npc_palette.h"
#include "file_pac.h"
#include "evt_world.h"
#include "common.h"
#include "snd_track_playback_api.h"
#include "fld_scene_ready_api.h"
#include "fld_clear_menu_entries_api.h"
#include "fld_play_pending_sounds_api.h"
#include "fld_scene_record_api.h"
#include "fld_object_slot_api.h"
#include "fld_release_texture_slots_api.h"
#include "fld_spark_object_entry.h"
#include "fld_spark_slot.h"
#include "sdf_math_vector4.h"
#include "kwln_fade.h"
#include "mnu_title_stream.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_textured_rect.h"
#include "sdf_motion.h"
#include "mdl.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_value.h"
#include "sdf_dev_state.h"
#include "fld_transfer_chunk.h"
#include "sdf_resource.h"
#include "eff_transform.h"
#include "sdf_model.h"
#include "fld_scene_record.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_node_descriptor.h"
#include "kwln.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "fr_font.h"
#include "itf.h"
#include "btl.h"
#include "evt_unit.h"
#include "fld_title_banner_menu.h"
#include "fld_label_cache_api.h"
#include "field_stage.h"
#include "sdf_chunk.h"
#include "sdk_libc.h"
#include "dds3_object_flags.h"
#include "sdf_map_position_api.h"


extern void fldDrawLocationPanel(void);
extern void fldUpdateEffectTextureTransforms(void);
extern void fldUpdateMenuResourceEffects(void);

extern f32 sdfViewTargetVector[4];
extern u8 fldAutoMapSeqTestFlag(u32 mask);
extern s32 D_003BAF48, D_003BAF40;





extern evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *);


/* Move an effect node to a position and update it. */
static void placeEffectNode(s32 node, f32 *position) {
    effCopyVectorToNodeInstance((EffNode *)(u32)node, position);
    dds3EffectDispatchUpdate((EffNode *)(u32)node);
}

/* Each field frame outside battle areas: move the field objects' effects to
 * their map records' positions, hide the room's placed NPC models that are
 * switched off or close to the camera's target (by their size), update the
 * pending effect node, and draw the menu resource effects, texture
 * transforms and location panel. Upstream: INCLUDE_ASM in game/code_001411F0;
 * written from the retail routine, on the shared FldObjectSlot and
 * FldEffectKind types. Its instructions match retail's apart from registers,
 * address forms (base plus offset), the float branches' sense and branch-likely
 * scheduling (291 aligned words); this body states the activation states and
 * distance limits directly. */
void func_00149B68(void) {
    f32 position[4];
    FldAreaWork *area = &fldGlobalBase;
    s32 i;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    if (fldGlobalBase.area >= 200 || fldAutoMapSeqTestFlag(1) != 0) {
        return;
    }

    for (i = 0; i < fldTakaraCount; i++) {
        FldObjectSlot *slot = &fldTakaraSlotTable[i];
        evtUnitHandle_t *transition;
        s32 kind;

        if (slot->activationKind < 0 || slot->activationKind >= 4) {
            continue;
        }
        if (slot->activationState == 0) {
            if (slot->effectNode == 0) {
                continue;
            }
            transition = dds3GetUnitHandleObjectBasic(slot->unitObject);
            if (sdfModelGetMarkPos(transition->owner->inner, 2) != 0) {
                VU0_STORE_VF(vf10, position);
            }
            placeEffectNode(slot->effectNode, position);
        } else if (slot->activationState == 1 || slot->activationState == 2) {
            transition = dds3GetUnitHandleObjectBasic(slot->unitObject);
            if (sdfModelGetMarkPos(transition->owner->inner, 1) != 0) {
                VU0_STORE_VF(vf10, position);
            }
            kind = D_0034C8F0[slot->effectKindIndex].kind;
            if (kind == 2 || kind == 3) {
                if (effInvokeNodeConditionOrAcceptDefault((EffNode *)(u32)D_003BAF4C) != 0) {
                    placeEffectNode(D_003BAF4C, position);
                }
                continue;
            }
            if (D_003BAF58 != 0) {
                placeEffectNode(D_003BAF58, position);
            }
            if (D_003BAF64 != 0) {
                placeEffectNode(D_003BAF64, position);
            }
        }
    }

    for (i = 0; i < fldNpcModelTable; i++) {
        FldRoomEffectEntry *entry = &fldModelSlotTable[i];
        f32 target[4];
        f32 height;
        f32 dx;
        f32 dz;
        f32 distance;
        s32 near;

        if (entry->room != 0 && fldTestMapSlotAuxiliaryFlag(area->area, area->floor + 1, entry->room) != 0) {
            if (entry->object != 0) {
                dds3ObjectBaseOnFlag((void *)entry->object, 1);
            }
            continue;
        }
        if (entry->placementId == 999) {
            continue;
        }
        SDF_VU0_COPYVECTOR(target, sdfViewTargetVector);
        if (entry->object == 0) {
            continue;
        }
        dds3ObjectBaseOffFlag((void *)entry->object, 1);
        dds3GetPosPoseSet((dds3Basic_t *)(u32)entry->object);
        VU0_STORE_VF(vf10, position);

        height = target[1] - position[1];
        if (height < 0.0f) {
            height = -height;
        }
        if (height >= 250.0f) {
            continue;
        }
        dx = target[0] - position[0];
        dz = target[2] - position[2];
        distance = fsqrtf(dx * dx + dz * dz);
        if (entry->size >= 79.0f) {
            near = distance < 95.0f;
        } else if (entry->size >= 29.0f) {
            near = distance < 75.0f;
        } else {
            near = distance < 60.0f;
        }
        if (near) {
            dds3ObjectBaseOnFlag((void *)entry->object, 1);
        }
        if (entry->kind == 6 && entry->kindId == 0x49 && distance < 110.0f) {
            dds3ObjectBaseOnFlag((void *)entry->object, 1);
        }
    }

    if (D_003BAF40 != 0) {
        if (effInvokeNodeConditionOrAcceptDefault((EffNode *)(u32)D_003BAF3C) != 0) {
            dds3EffectDispatchUpdate((EffNode *)(u32)D_003BAF3C);
        } else {
            D_003BAF40 = 0;
        }
    }
    fldUpdateMenuResourceEffects();
    fldUpdateEffectTextureTransforms();
    fldDrawLocationPanel();
}
#else
INCLUDE_ASM(const s32, "game/code_001411F0", func_00149B68);
#endif

extern u8 fldTestMapSlotAuxiliaryFlag(
    s32 mapId, u32 slotIndex, FldMapSlotAuxiliaryFlagSelector bit);


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

extern FldNpcPalette D_003D44A0;






void fldLoadNpcPalette(s32 field) {
    char path[64];
    char directory[32];
    sdfFileCmdHandle_t *command;

    if (field < 100) {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, "%sF%03d.NPL", directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &D_003D44A0, 0x200);
        sdfFileClose(command);
    }
}

void fldSetNpcPalette(const FldNpcPalette *src) {
    memcpy(&D_003D44A0, src, 0x200);
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
    FldSceneSelectionIndex index = fldGetCurrentSceneSelectionId();

    if (index < 0) {
        return 0;
    }
    return D_003D46C0[index].unk0[1];
}

void fldSetTitleDrawMode(u32 value) {
    D_003BAFB0 = value;
}

u8 fldTitleIsActive(void) {
    return dds3SearchProcessName(fldTitleTaskName) != 0;
}

void nbPanelAnalyzeStart(void) {
    D_003BAFA8 = 0;
    D_003BAFA0 = 1;
}

KwlnTaskUpdateResult fldTitle(dds3ProcessID_t *task) {
    s32 nextFrame;
    s32 alpha;
    s32 nextPhase;

    if (D_003BAFB0 == 0) {
        rsetOtOfs(0x53);
    } else {
        rsetOtOfs(0x5E);
    }
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 1);
    rsetAlpha(0);
    /* The next phase is staged before the dispatch: the scheduler issues the
     * constant-1 pseudo ahead of the phase load (see docs/idioms.md,
     * "Trace-justified staging: fldTitle next phase"). */
    nextPhase = 1;
    switch (D_003BAFA0) {
    case 0:
        if (D_003BAFA8 > 0) {
            D_003BAFA8--;
        }
        if (D_003BAFA8 == 0) {
            D_003BAFA0 = nextPhase;
            D_003BAF9C = 0;
        }
        break;
    case 1:
        if (D_003BAF9C >= 20) {
            alpha = (D_003BAF9C - 19) * 128 / 15;
            if (alpha > 0x80) {
                alpha = 0x80;
            }
            fldSubmitGsGradientQuad(0x175, 0x74, 0x8B, 0x45, 0, 0, 0, 0, 0, 0, 0, alpha * 12 / 128, 0, 0, 0,
                                    alpha / 64, 0, 0, 0, alpha / 2);
            DrwSprite2D(0x100, 0x12C, 0xFE, 0x45, 1, 1, 0xFE, 0x45, (alpha << 24) | 0x808080, D_003BAFB4);
        }
        if (D_003BAF9C >= 20 && D_003BAF9C < 30) {
            rsetAlpha(1);
            alpha = (D_003BAF9C - 20) * 128 / 10;
            if (alpha > 0x80) {
                alpha = 0x80;
            }
            if (alpha < 0) {
                alpha = 0;
            }
            fldSubmitGsGradientQuad(0x175, 0x74, 0x8B, 0x45, 0, 0, 0, 0, 0, 0, 0, alpha * 12 / 128, 0, 0, 0,
                                    alpha / 64, 0, 0, 0, alpha / 2);
            DrwSprite2D(0x100, 0x12C, 0xFE, 0x45, 1, 1, 0xFE, 0x45, (alpha << 24) | 0x807060, D_003BAFB4);
            rsetAlpha(0);
        }
        if (D_003BAF9C >= 30) {
            rsetAlpha(1);
            alpha = (0x30 - D_003BAF9C) * 128 / 18;
            if (alpha > 0x80) {
                alpha = 0x80;
            }
            if (alpha < 0) {
                alpha = 0;
            }
            fldSubmitGsGradientQuad(0x175, 0x74, 0x8B, 0x45, 0, 0, 0, 0, 0, 0, 0, alpha * 12 / 128, 0, 0, 0,
                                    alpha / 64, 0, 0, 0, alpha / 2);
            DrwSprite2D(0x100, 0x12C, 0xFE, 0x45, 1, 1, 0xFE, 0x45, (alpha << 24) | 0x807060, D_003BAFB4);
            rsetAlpha(0);
        }
        nextFrame = D_003BAF9C + 1;
        if (nextFrame > 60) {
            D_003BAF9C = 0;
            D_003BAFA0++;
        } else {
            D_003BAF9C = nextFrame;
        }
        break;
    case 2:
        fldSubmitGsGradientQuad(0x175, 0x74, 0x8B, 0x45, 0, 0, 0, 0, 0, 0, 0, 0xC, 0, 0, 0, 2, 0, 0, 0, 0x40);
        DrwSprite2D(0x100, 0x12C, 0xFE, 0x45, 1, 1, 0xFE, 0x45, 0x80808080, D_003BAFB4);
        nextFrame = D_003BAF9C + 1;
        if (nextFrame > 60) {
            D_003BAF9C = 0;
            D_003BAFA0++;
        } else {
            D_003BAF9C = nextFrame;
        }
        break;
    case 3:
        alpha = (20 - D_003BAF9C) * 128 / 20;
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        fldSubmitGsGradientQuad(0x175, 0x74, 0x8B, 0x45, 0, 0, 0, 0, 0, 0, 0, alpha * 12 / 128, 0, 0, 0,
                                alpha / 64, 0, 0, 0, alpha / 2);
        DrwSprite2D(0x100, 0x12C, 0xFE, 0x45, 1, 1, 0xFE, 0x45, (alpha << 24) | 0x808080, D_003BAFB4);
        nextFrame = D_003BAF9C + 1;
        if (nextFrame > D_003BAFAC) {
            D_003BAF9C = 0;
            D_003BAFA0++;
        } else {
            D_003BAF9C = nextFrame;
        }
        break;
    default:
        return KWLN_TASK_UPDATE_FINISH_CALLBACK;
    }
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

void fldReleaseTitleTextureReference(dds3ProcessID_t *task) {
    if (D_003BAFB4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFB4);
        D_003BAFB4 = 0;
    }
}

extern KwlnTaskUpdateResult fldTitle(dds3ProcessID_t *task);


extern u32 D_003BAFA4;




extern char D_003A0810[]; /* "/fld/f/pnl/df%03d.tmx" */

void fldStartTitle(s32 field, s32 mode, s32 option) {
    char path[32];
    s32 resourceAddress;
    struct sdfMemHandle_t *allocation;

    D_003BAFA8 = mode;
    D_003BAFAC = option;
    D_003BAFA4 = field;
    D_003BAF9C = 0;
    D_003BAFA0 = 0;
    D_003BAFB0 = 0;
    sprintf(path, D_003A0810, field);
    allocation = sdfDevLoadFileEx(path, (u32 *)&resourceAddress, 0);
    D_003BAFB4 = sdfTexSet((SdfTextureFileHeader *)(resourceAddress));
    sdfMemFreeDirect(allocation);
    if (fldTitleIsActive() == 0) {
        dds3StartProcess(fldTitleTaskName, 0x2B0A, 0, 1, fldTitle, fldReleaseTitleTextureReference, 0);
    }
}

void fldDestroyTitleTask(void) {
    if (fldTitleIsActive() != 0) {
        dds3KillProcessName(fldTitleTaskName, 1);
    }
}

u8 fldTitleMiniIsActive(void) {
    return dds3SearchProcessName(D_003A0828) != 0;
}

/* Animate the unlock title's split caption, highlight and expanding fade. */
s32 fldTitleMini(dds3ProcessID_t *task) {
    s32 alpha;
    u32 color;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 nextFrame;
    u32 highlightRgb;

    if (fldProcCampChk() == FLD_CAMP_CHECK_STATE_MENU_ACTIVE) {
        return 0;
    }
    rsetOtOfs(0x53);
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    switch (D_003BAFBC) {
    case 0:
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 3) {
            D_003BAFBC = 1;
            D_003BAFB8 = 0;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    case 1:
        if (D_003BAFB8 >= 8) {
            alpha = (D_003BAFB8 - 8) * 16;
            fldSubmitGsGradientQuad(0x4C, 0xB1, 0xB4, 2, 0, 0, 0, 0,
                0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128);
            fldSubmitGsGradientQuad(0x100, 0xB1, 0xB4, 2, 0, 0, 0, alpha * 0x33 / 128,
                0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0);
        }
        alpha = D_003BAFB8 * 128 / 10;
        if (alpha > 128) {
            alpha = 128;
        }
        color = (alpha << 24) | 0x808080;
        x = 218 - D_003BAFB8 * 6;
        DrwSprite2D(x < 128 ? 128 : x, 0x152, 0x80, 0x1C,
            0, 0x24, 0x80, 0x1C, color, D_003BAFC4);
        x = 166 + D_003BAFB8 * 6;
        DrwSprite2D(x > 256 ? 256 : x, 0x152, 0x80, 0x1C,
            0x80, 0x24, 0x80, 0x1C, color, D_003BAFC4);
        if (D_003BAFB8 >= 10) {
            alpha = (D_003BAFB8 - 10) * 25;
            if (alpha > 128) {
                alpha = 128;
            }
            rsetAlpha(1);
            DrwSprite2D(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        if (D_003BAFB8 >= 8) {
            alpha = (D_003BAFB8 - 8) * 16;
            rsetAlpha(0);
            DrwSprite2D(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 15) {
            D_003BAFB8 = 0;
            D_003BAFBC++;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    case 2:
        alpha = 128;
        fldSubmitGsGradientQuad(0x4C, 0xB1, 0xB4, 2, 0, 0, 0, 0,
            0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128);
        fldSubmitGsGradientQuad(0x100, 0xB1, 0xB4, 2, 0, 0, 0, alpha * 0x33 / 128,
            0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0);
        alpha -= D_003BAFB8 * 8;
        if (alpha > 0) {
            rsetAlpha(1);
            DrwSprite2D(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        rsetAlpha(0);
        DrwSprite2D(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
            0x80808080, D_003BAFC4);
        rsetAlpha(0);
        DrwSprite2D(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
            0x80808080, D_003BAFC4);
        highlightRgb = 0x808080;
        if (D_003BAFB8 < 4) {
            alpha = D_003BAFB8 * 40 / 3;
            rsetAlpha(1);
            DrwSprite2D(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
                (alpha << 24) | highlightRgb, D_003BAFC4);
        } else {
            alpha = 56 - D_003BAFB8 * 4;
            if (alpha > 0) {
                rsetAlpha(1);
                DrwSprite2D(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
                    (alpha << 24) | highlightRgb, D_003BAFC4);
            }
        }
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 30) {
            D_003BAFB8 = 0;
            D_003BAFBC++;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    case 3:
        alpha = 128;
        if (D_003BAFB8 >= 5) {
            alpha = 128 - (D_003BAFB8 - 5) * 12;
        }
        if (alpha > 0) {
            fldSubmitGsGradientQuad(0x4C, 0xB1, 0xB4, 2, 0, 0, 0, 0,
                0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128);
            fldSubmitGsGradientQuad(0x100, 0xB1, 0xB4, 2, 0, 0, 0, alpha * 0x33 / 128,
                0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0);
        }
        if (D_003BAFB8 < 10) {
            alpha = 128 - D_003BAFB8 * 16;
            if (alpha > 0) {
                rsetAlpha(0);
                DrwSprite2D(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
                    (alpha << 24) | 0x808080, D_003BAFC4);
            }
        }
        alpha = 128;
        if (D_003BAFB8 >= 5) {
            alpha = 128 - (D_003BAFB8 - 5) * 16;
        }
        if (D_003BAFB8 < 10) {
            x = 128;
            width = 256;
            y = 338;
            height = 36;
        } else {
            width = (D_003BAFB8 - 10) * 128 / 15;
            x = 128 - width / 2;
            width = 256 + width;
            height = (D_003BAFB8 - 10) * 32 / 15;
            y = 338 + height / 2;
            height = 36 - height;
        }
        if (alpha > 0) {
            rsetAlpha(0);
            DrwSprite2D(x, y, width, height, 0, 0, 0x100, 0x24,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 30) {
            D_003BAFB8 = 0;
            D_003BAFBC++;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    default:
        return -1;
    }
    return 0;
}


void fldReleaseTitleMiniTexture(dds3ProcessID_t *task) {
    if (D_003BAFC4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFC4);
        D_003BAFC4 = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001411F0", fldTitleTaskName);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0810);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0824);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0828);

void fldStartMiniTitleForUnlock(s32 id) {
    char path[0x20];
    s32 data;
    struct sdfMemHandle_t *allocation;

    switch (id) {
    case 22:
        if (evtBitCheck(0x440) == 0) {
            return;
        }
        break;
    case 23:
        if (evtBitCheck(0x480) == 0) {
            return;
        }
        break;
    case 24:
        if (evtBitCheck(0x4C0) == 0) {
            return;
        }
        break;
    case 25:
        if (evtBitCheck(0x500) == 0) {
            return;
        }
        break;
    case 26:
        if (evtBitCheck(0x560) == 0) {
            return;
        }
        break;
    case 27:
        if (evtBitCheck(0x580) == 0) {
            return;
        }
        break;
    case 28:
        if (evtBitCheck(0x5A0) == 0) {
            return;
        }
        break;
    case 29:
        if (evtBitCheck(0x600) == 0) {
            return;
        }
        break;
    case 30:
        if (evtBitCheck(0x620) == 0) {
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
    D_003BAFC0 = id;
    D_003BAFB8 = 0;
    D_003BAFBC = 0;
    sprintf(path, "/fld/f/pnl/ds%03d.tmx", id);
    allocation = sdfDevLoadFileEx(path, (u32 *)&data, 0);
    D_003BAFC4 = sdfTexSet((SdfTextureFileHeader *)(data));
    sdfMemFreeDirect(allocation);
    if (fldTitleIsActive() == 0) {
        dds3StartProcess(D_003A0828, 0x2B0A, 0, 1, fldTitleMini, fldReleaseTitleMiniTexture, 0);
    }
}

static const char s_fieldWeatherLimitPath[] __attribute__((aligned(16))) = "/fld/f/bin/limit_00.tmx";

void fldSetSnowActive(void) {
    if (D_003BAFBC == 0) {
        D_003BAFBC = 1;
    }
}


void fldLoadWeatherEffects(void) {
    struct sdfMemHandle_t *allocation;
    s32 resourceAddress;

    allocation = sdfDevLoadFileEx(s_fieldWeatherLimitPath, (u32 *)&resourceAddress, 0);
    fldWeatherLimitTexture = sdfTexSet((SdfTextureFileHeader *)(resourceAddress));
    sdfMemFreeDelayedDirect(allocation);
    fldDamEffectResource = sdfDevLoadFileEx("/fld/f/bin/FH_DAM_2.EPL", &fldDamEffectData, 0);
    fldDamEffectNode = effCreateNodeFromParameter(fldDamEffectData);
    fldDamEffectPositioned = 0;
    fldYukEffectResource = sdfDevLoadFileEx("/fld/f/bin/YUK_2.EPL", &fldYukEffectData, 0);
    fldYukEffectNode = effCreateNodeFromParameter(fldYukEffectData);
    fldYukEffectPositioned = 0;
}

void fldReleaseWeatherEffects(void) {
    if (fldWeatherLimitTexture != 0) {
        sdfTexReleaseReferenceViaHandler(fldWeatherLimitTexture);
        fldWeatherLimitTexture = 0;
    }
    dds3EffectDelete(fldDamEffectNode);
    fldDamEffectNode = NULL;
    fldDamEffectPositioned = 0;
    sdfMemFreeDelayedDirect(fldDamEffectResource);
    fldDamEffectResource = NULL;
    fldDamEffectData = 0;
    dds3EffectDelete(fldYukEffectNode);
    fldYukEffectNode = NULL;
    fldYukEffectPositioned = 0;
    sdfMemFreeDelayedDirect(fldYukEffectResource);
    fldYukEffectResource = NULL;
    fldYukEffectData = 0;
}

void fldSetWeatherEffectPos(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (fldDamEffectNode != NULL) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3EffectReset(fldDamEffectNode);
        effCopyVectorToNodeInstance(fldDamEffectNode, pos);
        fldDamEffectPositioned = 1;
    }
    if (fldYukEffectNode != NULL) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3EffectReset(fldYukEffectNode);
        effCopyVectorToNodeInstance(fldYukEffectNode, pos);
        fldYukEffectPositioned = 1;
    }
}

void fldUpdateWeatherEffectNodes(void) {
    if (fldDamEffectNode != NULL && fldDamEffectPositioned != 0) {
        dds3EffectDispatchUpdate(fldDamEffectNode);
    }
    if (fldYukEffectNode != NULL && fldYukEffectPositioned != 0) {
        dds3EffectDispatchUpdate(fldYukEffectNode);
    }
}

void *fldFindFieldEntryByKeyPair(s32 key0, s32 key1) {
    FldEnt110 *entry = D_003BAA48;
    s32 index = 0;

    while (index < 8) {
        if (entry->unk0 == key0) {
            if (entry->unk2 == key1) {
                return entry;
            }
        }
        index++;
        entry++;
    }
    return NULL;
}

typedef struct FldSparkController {
    void *object;              /* 0x00 */
    FldEnt110 *entry;           /* 0x04 */
    FldSparkControllerState state; /* 0x08 */
    s32 cursor;                /* 0x3C */
} FldSparkController;

typedef char FldSparkControllerStateOffsetCheck[
    ((u32)&((FldSparkController *)0)->state == 0x08) ? 1 : -1];
typedef char FldSparkControllerSizeCheck[(sizeof(FldSparkController) == 0x40) ? 1 : -1];

extern FldSparkController fldSparkControlState;

typedef struct FldSparkEntryInfo {
    u8 pad00[0xE];
    s16 modelId;
} FldSparkEntryInfo;

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 value, u128 *position, u128 *rotation);
extern void dds3RebuildObjectModelContext(dds3Basic_t *node, s32 kind, s32 resource);
extern void dds3SetSclPoseSet(dds3Basic_t *node, u128 *vector);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08E8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08F8);

const char D_003A0908[] = "HUNT_MDL_UNIT";

extern SdfMathVector4 D_003A08E8;
extern SdfMathVector4 D_003A08F8;

void fldCreateSparkSlotObjects(void) {
    SdfMathVector4 position = D_003A08E8;
    SdfMathVector4 rotation;
    dds3ModelHandle_t *resource;
    s32 i;

    memset(&rotation, 0, sizeof(rotation));
    {
        SdfMathVector4 scale = D_003A08F8;

        for (i = 0; i < 17; i++) {
            fldSparkObjectEntries[i].objectHandle = NULL;
            fldSparkObjectEntries[i].sparkSlotIndex = -1;
            *(s32 *)fldSparkObjectEntries[i].pad8 = 0;
        }
        for (i = 0; i < 17; i++) {
            fldSparkObjectEntries[i].objectHandle = dds3AddUnitObjectBasic(dds3AllocObjectId(), (u128 *)&position, (u128 *)&rotation);
            dds3SetUnitObjectId(fldSparkObjectEntries[i].objectHandle, D_003A0908);
            if (i == 16) {
                dds3RebuildObjectModelContext(fldSparkObjectEntries[i].objectHandle, 1, ((FldSparkEntryInfo *)fldSparkControlState.entry)->modelId);
            } else {
                dds3RebuildObjectModelContext(fldSparkObjectEntries[i].objectHandle, 1, 0x146);
            }
            dds3SetSclPoseSet(fldSparkObjectEntries[i].objectHandle, (u128 *)&scale);
            resource = (dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(fldSparkObjectEntries[i].objectHandle);
            resource->first->frameStep = 1.0f;
            mdlLoopMotion(resource, 0, 0);
            resource->flags |= 1;
            dds3ObjectBaseOnFlag(fldSparkObjectEntries[i].objectHandle, 0x400);
            fldInitializeHuntSparkUnit(evtGetUnitHandle2(fldSparkObjectEntries[i].objectHandle));
        }
    }
}

void fldClearObjectEntryHandles(void) {
    FldSparkObjectEntry *entry = fldSparkObjectEntries;
    s32 i = 0x10;

    do {
        dds3Basic_t *objectHandle = entry->objectHandle;

        i--;
        if (objectHandle != NULL) {
            dds3DeleteUnitObjectBasic(objectHandle);
            entry->objectHandle = NULL;
        }
        entry++;
    } while (i >= 0);
}

extern FldSparkSlot fldSparkSlots[];


extern s16 D_0032E4B4[];

void fldInitSparkTable(void) {
    s32 i;

    if (D_0032E4B4[0] == 0) {
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

/* Complete 0x40-byte field spark controller, including the weather timer. */
extern s32 D_003D62DC[];
struct dds3Basic_t;


s32 fldBindSparkSlotObject(s32 index, s32 reserved) {
    s32 slot;
    s32 i;
    s32 candidate;
    s32 *flags;
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
    D_003D62DC[0] = (slot + 1) % 16;
    if (fldSparkSlots[index].hasVectors == 1) {
        SDF_VU0_COPYVECTOR(&position, fldSparkSlots[index].position);
        dds3SetPosPoseSet(fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].position);
        dds3SetRotPoseSet(fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].rotation);
        flags = (s32 *)dds3ObjectBaseGetModelHandle(fldSparkObjectEntries[slot].objectHandle);
        *flags &= ~1;
        fldSparkSlots[index].objectSlot = slot;
        fldSparkObjectEntries[slot].sparkSlotIndex = index;
        return 1;
    }
    return 0;
}


extern SdfMathVector4 D_003A0918;

void fldFreeSparkSlot(s32 index) {
    SdfMathVector4 vec;
    dds3Basic_t *obj;
    s32 *flags;
    s16 slot;

    vec = D_003A0918;
    if (fldSparkSlots[index].hasVectors == 1 && fldSparkSlots[index].active != 0 && fldSparkSlots[index].objectSlot != -1) {
        vec.x = fldSparkSlots[index].position[0];
        vec.z = fldSparkSlots[index].position[2];
        dds3SetPosPoseSet(fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle, (u128 *)&vec);
        obj = fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle;
        flags = (s32 *)dds3ObjectBaseGetModelHandle(obj);
        *flags |= 1;
        dds3ObjectBaseOffFlag(obj, 0x400);
        slot = fldSparkSlots[index].objectSlot;
        fldSparkSlots[index].objectSlot = -1;
        fldSparkObjectEntries[slot].sparkSlotIndex = -1;
    }
}

extern FldSparkController fldSparkControlState;

extern void fldCreateSparkSlotObjects();

extern s32 fldBindSparkSlotObject(s32, s32);

void fldUpdateSparkSlots(void) {
    s32 i;

    fldCreateSparkSlotObjects();
    for (i = 0; i < 64 && i < fldSparkControlState.state.entryCount; i++) {
        if (fldSparkSlots[i].hasVectors != 0 && fldSparkSlots[i].active != 0) {
            fldBindSparkSlotObject(i, fldSparkSlots[i].reservedObjectSlot);
        }
    }
}

void fldUpdateSparkSequence(void) {
    FldSparkSequence *sequence;
    s32 i;
    s32 slot;
    SdfMathVector4 position __attribute__((aligned(16)));
    f32 dx;
    f32 dy;
    f32 dz;

    if (fldSparkControlState.state.sequenceRemaining == 0) {
        i = fldSparkControlState.state.sequenceIndex;
        sequence = &fldSparkControlState.entry->sequences[i];
        fldSparkControlState.state.sequenceRemaining = sequence->count;
        if (sequence->count == -1) {
            fldSparkControlState.state.terminated = 1;
            return;
        }
        if (sequence->count == 0) {
            slot = sequence->slots[0];
            fldBindSparkSlotObject(slot, 1);
            fldSparkSlots[slot].reservedObjectSlot = 1;
            fldSparkSlots[slot].active = 1;
            fldSparkSlots[slot].tickCounter = 0;
            fldSparkControlState.state.sequenceRemaining = 1;
            p2sdPlaySE(0x670015, 0x7F, 0x3F);
        } else {
            for (i = 0; i < sequence->count; i++) {
                slot = sequence->slots[i];
                fldBindSparkSlotObject(slot, 0);
                fldSparkSlots[slot].reservedObjectSlot = 0;
                fldSparkSlots[slot].active = 1;
                fldSparkSlots[slot].tickCounter = 0;
            }
            p2sdPlaySE(0x670014, 0x7F, 0x3F);
        }
        fldSparkControlState.state.sequenceIndex++;
    }
    for (i = 0; i < 64 && i < fldSparkControlState.state.entryCount; i++) {
        switch (fldSparkSlots[i].active) {
        case 0:
            break;
        case 1:
            fldSparkSlots[i].tickCounter++;
            if (fldSparkSlots[i].objectSlot != -1 &&
                fldSparkControlState.state.animationState == FLD_SPARK_ANIMATION_RUNNING &&
                fldSparkControlState.state.pulse != 0 && (u32)(fldSparkControlState.state.modeCountdown - 10) < 6) {
                SDF_VU0_COPYVECTOR(&position, fldSparkSlots[i].position);
                dx = fldGlobalBase.x - position.x;
                dy = fldGlobalBase.y - position.y;
                dz = fldGlobalBase.z - position.z;
                if (fsqrtf(dx * dx + dy * dy + dz * dz) < 200.0f) {
                    fldSetWeatherEffectPos(position.x, position.y, position.z);
                    fldGlobalBase.collectedCount++;
                    fldGlobalBase.score += 0x32;
                    fldSparkControlState.state.sequenceRemaining--;
                    p2sdPlaySE(0x670013, 0x7F, 0x3F);
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

extern FldSparkController fldSparkControlState;

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

/* Ten atlas origins for digits 0-9, each stored as a U/V atlas coordinate pair. */
extern const s32 D_003A0928[10][2];
extern void eglSetOt(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);

void fldDrawWeatherLimitTimer(void) {
    s32 digitOrigins[10][2];
    s32 remainingFrames;
    s32 remainingSeconds;
    s32 digit;
    u32 vertexColor;

    memcpy(digitOrigins, D_003A0928, sizeof(digitOrigins));
    remainingFrames = fldSparkControlState.state.countdown;
    vertexColor = 0x80808080;
    eglSetOt(0x53);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitTexturedGradientQuad(20, 20, 113, 31, 1, 0, 113, 31,
                  vertexColor, vertexColor, vertexColor, vertexColor, fldWeatherLimitTexture);
    remainingSeconds = remainingFrames / 30;
    if (remainingSeconds >= 100) {
        remainingSeconds = 99;
    }
    digit = remainingSeconds / 10;
    if (digit > 0) {
        evtSubmitTexturedGradientQuad(92, 37, 15, 16, digitOrigins[digit][0], digitOrigins[digit][1], 15, 16,
                      vertexColor, vertexColor, vertexColor, vertexColor, fldWeatherLimitTexture);
    }
    digit = remainingSeconds % 10;
    evtSubmitTexturedGradientQuad(105, 37, 15, 16, digitOrigins[digit][0], digitOrigins[digit][1], 15, 16,
                  vertexColor, vertexColor, vertexColor, vertexColor, fldWeatherLimitTexture);
}

extern FldSparkController fldSparkControlState;




/* Reset the spark controller for the field entry of the current area and floor
 * (or take the early dialog path when flag 0x819 is set). unk10C is an f32:
 * its alias set keeps that clear independent of the entry loads in sched1
 * (cc1probe sched1: priority 2, no dependence chain to the entry loads). */
void func_0014C468(void) {
    FldEnt110 *entry;

    if (D_0032E5C4[0] != 0) {
        return;
    }
    if (evtBitCheck(0x819) != 0) {
        fclInitMessage((struct ItfMesSub *)D_0034D8F0);
        fclStartMessage(6);
        D_0032E4B4[0] = 1;
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_UNAVAILABLE_MESSAGE;
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_IDLE;
        fldSparkControlState.state.dialogTimer = 0;
        fldResetPlayerSceneObjectState();
        return;
    }
    fclInitMessage((struct ItfMesSub *)D_0034D8F0);
    if (evtBitCheck(0x81A) != 0) {
        fclStartMessage(1);
    } else {
        fclStartMessage(0);
    }
    evtStoreValueAndCaptureWindowPanelValue(2);
    evtSetMessageWindowOptionWhenOpen(0);
    evtBitOn(0x81A);
    entry = (FldEnt110 *)fldFindFieldEntryByKeyPair(fldGlobalBase.area, fldGlobalBase.floor + 1);
    fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_IDLE;
    fldGlobalBase.score = 0;
    fldSparkControlState.state.countdown = entry->duration;
    fldGlobalBase.specialEventActive = FLD_SPECIAL_EVENT_ACTIVE;
    fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_ENTRY_PROMPT;
    fldSparkControlState.state.terminated = 0;
    fldSparkControlState.state.animationState = FLD_SPARK_ANIMATION_READY;
    fldSparkControlState.state.modeCountdown = 0;
    fldSparkControlState.state.pendingMode = 0;
    fldSparkControlState.state.pulse = 0;
    fldSparkControlState.state.dialogTimer = 0;
    fldSparkControlState.state.sequenceIndex = 0;
    fldSparkControlState.state.sequenceRemaining = 0;
    fldSparkControlState.entry = entry;
    fldSparkControlState.state.entryCount = entry->entryCount;
    fldSparkControlState.cursor = 0;
    fldGlobalBase.collectedCount = 0;
    fldGlobalBase.unk10C = 0.0f;
    fldResetSparkTable();
    fldResetPlayerSceneObjectState();
}

extern s32 D_0032E5C4[];

extern FldSparkController fldSparkControlState;

extern void fldTogglePlayerModel(void);

extern void fldClearObjectEntryHandles(void);

extern void fldReleaseWeatherEffects(void);

extern void fldStartSceneBgmAlternate(void);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void evtSetSolarOverlayFullyVisible(void);

/* Restores field presentation once an active event finishes, then clears the
 * event latch. An already-cleared latch leaves field resources untouched. */
void fldFinishEventFieldState(void) {
    FldAreaWork *state = &fldGlobalBase;

    if (state->specialEventActive != FLD_SPECIAL_EVENT_INACTIVE) {
        D_0032E5C4[0] = 0;
        fldTogglePlayerModel();
        fldClearObjectEntryHandles();
        fldReleaseWeatherEffects();
        fldStartSceneBgmAlternate();
        fldPreparePlayerSceneCameraTarget();
        state->specialEventActive = FLD_SPECIAL_EVENT_INACTIVE;
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_IDLE;
        fldSparkControlState.state.countdown = 0;
        evtSetSolarOverlayFullyVisible();
    }
}

extern void dspRequestMessageWindowModeOne(void);
extern s8 evtGetCapturedWindowPanelValue(void);
extern void fldAddCameraModelEntry(s32);
extern void fldCreateSparkSlotObjects(void);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern void evtDisableMoonDraw(void);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0918);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0928);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0978);

void fldUpdateSparkMessageSequence(void) {
    f32 position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    f32 rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    s32 choice;
    void *node;

    switch (fldSparkControlState.state.dialogPhase) {
    case FLD_SPARK_DIALOG_PHASE_UNAVAILABLE_MESSAGE:
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            evtFinishMessageWindowAndNotify();
            fclDestroyMessage();
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_CANCELLED;
            fldGlobalBase.specialEventActive = FLD_SPECIAL_EVENT_INACTIVE;
            evtSetSolarOverlayFullyVisible();
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_ENTRY_PROMPT:
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
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
                fldGlobalBase.specialEventActive = FLD_SPECIAL_EVENT_INACTIVE;
                evtSetSolarOverlayFullyVisible();
            } else {
                evtFinishMessageWindowAndNotify();
                fclDestroyMessage();
                fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_CONFIRM_PROMPT;
                fldSparkControlState.state.dialogTimer = 0;
                fclInitMessage((struct ItfMesSub *)D_0034D8F0);
                fclStartMessage(3);
                evtStoreValueAndCaptureWindowPanelValue(2);
                evtSetMessageWindowOptionWhenOpen(0);
            }
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_CONFIRM_PROMPT:
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            evtFinishMessageWindowAndNotify();
            fclDestroyMessage();
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_ENTRY_PROMPT;
            fldSparkControlState.state.dialogTimer = 0;
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_START_ARCHIVE_LOAD:
        dds3WorkClear();
        fldBgmFadeOut();
        D_003BAFE8 = fileLoad((const char *)D_003A0978);
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_WAIT_ARCHIVE_FILE;
        fldSparkControlState.state.dialogTimer = 0;
        break;
    case FLD_SPARK_DIALOG_PHASE_WAIT_ARCHIVE_FILE:
        if (pacFileCheckEnd(D_003BAFE8) != 0) {
            D_003BAFF0 = (u32)fileGetLoadPtr(D_003BAFE8);
            D_003BAFEC = fileGetMemHandle(D_003BAFE8);
            position[0] = fldGlobalBase.x;
            position[1] = fldGlobalBase.y;
            position[2] = fldGlobalBase.z;
            node = dds3AddBEDEffectObjectBasic2((struct SoundMixer *)D_003BAFF0, position, rotation);
            fldSparkControlState.object = node;
            dds3EffectObjectBasicStatOff(node, 1);
            fileRequestFreeIfComplete(D_003BAFE8);
            fldAddCameraModelEntry(0xD);
            D_0032E5C4[0] = 1;
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
        if (mnuPollTitleStreamStateLocked() == MNU_STREAM_LOAD_COPIED) {
            p2sdPlaySE(0x670010, 0x7F, 0x3F);
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
        fldDrawWeatherLimitTimer();
        if (++fldSparkControlState.state.dialogTimer > 10) {
            dds3EffectObjectBasicStatOff(fldSparkControlState.object, 1);
            dds3DeleteSceneBasicData(fldSparkControlState.object);
            sdfMemFreeDelayedDirect(D_003BAFEC);
            fldSparkControlState.state.dialogTimer = 0;
            fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_ENTER_ENCOUNTER;
            mnuTitleStreamUpdateAndLogBgm();
        }
        break;
    case FLD_SPARK_DIALOG_PHASE_ENTER_ENCOUNTER:
        fldDrawWeatherLimitTimer();
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_IDLE;
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_ENCOUNTER_ACTIVE;
        fldPreparePlayerSceneCameraTarget();
        break;
    case FLD_SPARK_DIALOG_PHASE_CANCELLED:
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_ENCOUNTER_ACTIVE;
        fldSparkControlState.state.dialogPhase = FLD_SPARK_DIALOG_PHASE_IDLE;
        fldPreparePlayerSceneCameraTarget();
        break;
    }
}

FldSparkDialogPhase fldGetSparkDialogPhase(void) {
    return D_003D62C8[0];
}

void fldUpdateSparkEncounterScene(void) {
    f32 firstVector[4];
    f32 secondVector[4];
    FldSparkControllerPhase phase;
    s32 value;
    void *node;
    FldAreaWork *area;

    memset(firstVector, 0, sizeof(firstVector));
    firstVector[3] = 1.0f;
    memset(secondVector, 0, sizeof(secondVector));
    secondVector[3] = 1.0f;

    phase = fldSparkControlState.state.phase;
    if (phase == FLD_SPARK_CONTROLLER_PHASE_ENCOUNTER_ACTIVE) {
        if (fldSparkControlState.state.animationState == FLD_SPARK_ANIMATION_READY) {
            if (D_00324530[0] < 0) {
                fldSetCameraModelMotionRequest(6, 20);
                p2sdPlaySE(0x670012, 0x7F, 0x3F);
                fldSparkControlState.state.animationState = FLD_SPARK_ANIMATION_RUNNING;
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
                    fldSparkControlState.state.animationState = pendingMode;
                    fldSparkControlState.state.modeCountdown = 0;
                    fldSparkControlState.state.pendingMode = 0;
                }
            } else if (fldSparkControlState.state.animationState == FLD_SPARK_ANIMATION_RUNNING &&
                       fldSparkControlState.state.pendingMode == 0 && value < 14 && D_00324530[0] < 0) {
                fldSetCameraModelMotionRequest(6, 20);
                p2sdPlaySE(0x670012, 0x7F, 0x3F);
                fldSparkControlState.state.animationState = FLD_SPARK_ANIMATION_RUNNING;
                fldSparkControlState.state.pulse = 1;
                fldSparkControlState.state.modeCountdown = 20;
            }
        }

        if (fldGetSparkDialogPhase() == FLD_SPARK_DIALOG_PHASE_IDLE && fldEncountTestFlag(0x40) != 0) {
            if (fldSparkControlState.state.countdown == 300) {
                p2sdPlaySE(0x670017, 0x7F, 0x3F);
            }
            if (fldSparkControlState.state.countdown > 0) {
                fldSparkControlState.state.countdown--;
            }
        }

        fldDrawWeatherLimitTimer();
        if (fldSparkControlState.state.countdown < 31) {
            fldSparkControlState.state.countdown = 0;
            p2sdStopSe(0x670017);
            p2sdPlaySE(0x670016, 0x7F, 0x3F);
            fldReleaseCameraModel(0);
            fldSetCameraModelMotionRequest(2, 0);
            fldSetCameraNodeModeWithTen();
            fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_TIMEOUT_MESSAGE;
            fclInitMessage((struct ItfMesSub *)D_0034D8F0);
            fclStartMessage(4);
            fldResetPlayerSceneObjectState();
            fldClearObjectEntryHandles();
            fldReleaseWeatherEffects();
        } else {
            fldUpdateSparkSequence();
        }

        if (fldSparkControlState.state.terminated != 0) {
            p2sdStopSe(0x670017);
            mnuMarkTitleStreamResetPending();
            D_0032E4C0[0] = 1;
            fldResetPlayerSceneObjectState();
            fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_FADE_TO_EVENT;
            evtBitOn(0x819);
        }
        return;
    } else if (phase == FLD_SPARK_CONTROLLER_PHASE_TIMEOUT_MESSAGE) {
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() != EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            return;
        }
        evtFinishMessageWindowAndNotify();
        fclDestroyMessage();
        fldSparkControlState.state.frame = 0;
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_POST_MESSAGE_HANDOFF;
        mnuAdvanceTitleStateUnderSemaphore();
        D_003BAFE8 = fileLoad(D_003A0978);
    } else if (phase == FLD_SPARK_CONTROLLER_PHASE_POST_MESSAGE_HANDOFF) {
        if (pacFileCheckEnd(D_003BAFE8) == 0) {
            return;
        }
        D_003BAFF0 = (u32)fileGetLoadPtr(D_003BAFE8);
        D_003BAFEC = fileGetMemHandle(D_003BAFE8);
        area = &fldGlobalBase;
        firstVector[0] = area->x;
        firstVector[1] = area->y;
        firstVector[2] = area->z;
        node = dds3AddBEDEffectObjectBasic2((struct SoundMixer *)D_003BAFF0, firstVector, secondVector);
        fldSparkControlState.object = node;
        dds3EffectObjectBasicStatOff(node, 1);
        fileRequestFreeIfComplete(D_003BAFE8);
        fldSetCameraModelMotionRequest(5, 0);
        dds3EffectObjectBasicStatOn(fldSparkControlState.object, 1);
        dds3StartBEDEffectObjectBasic(fldSparkControlState.object, 0);
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_FADE_TO_EVENT;
    } else if (phase == FLD_SPARK_CONTROLLER_PHASE_FADE_TO_EVENT) {
        fldSparkControlState.state.frame++;
        if (fldSparkControlState.state.frame == 40) {
            dds3FadeOut(255, 255, 255, 4);
        }
        if (fldSparkControlState.state.frame < 45) {
            return;
        }
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_ENTER_EVENT;
    } else if (phase == FLD_SPARK_CONTROLLER_PHASE_ENTER_EVENT) {
        p2sdPlaySE(0x670011, 0x7F, 0x3F);
        fldTogglePlayerModel();
        fldStartSceneBgmAlternate();
        evtSetSolarOverlayFullyVisible();
        D_0032E4E8[0] = 1;
        fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_RETURN_TO_FIELD;
        fldSparkControlState.state.frame = 0;
    } else if (phase == FLD_SPARK_CONTROLLER_PHASE_RETURN_TO_FIELD) {
        fldSparkControlState.state.frame++;
        if (fldSparkControlState.state.frame == 1) {
            dds3FadeIn(255, 255, 255, 4);
        }
        if (fldSparkControlState.state.frame == 6) {
            node = fldSparkControlState.object;
            dds3EffectObjectBasicStatOff(node, 1);
            dds3DeleteSceneBasicData(fldSparkControlState.object);
            sdfMemFreeDelayedDirect(D_003BAFEC);
        }
        if (fldSparkControlState.state.frame >= 11) {
            D_0032E5C4[0] = 0;
            fldPreparePlayerSceneCameraTarget();
            fldSparkControlState.state.phase = FLD_SPARK_CONTROLLER_PHASE_IDLE;
            D_0032E4B4[0] = 0;
        }
    } else {
        return;
    }
}

/* Releases scene presentation and camera resources before clearing the event
 * latch and installing the existing field-state values. */
void fldResetEventSceneState(void) {
    fldReleaseCameraModel(0);
    D_0032E5C4[0] = 0;
    fldGlobalBase.playerModelAlternate = FLD_PLAYER_MODEL_ALTERNATE_DISABLED;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldPreparePlayerSceneCameraTarget();
    fldGlobalBase.deferredExit = FLD_DEFERRED_EXIT_REQUESTED;
    fldGlobalBase.specialEventActive = FLD_SPECIAL_EVENT_INACTIVE;
    D_003D62A8[0] = 0;
    fldGlobalBase.unk138 = 1;
    evtSetSolarOverlayFullyVisible();
}

void fldStartDeferredFieldExit(void) {
    fclInitMessage((struct ItfMesSub *)D_0034D8F0);
    fclStartMessage(5);
    fldResetPlayerSceneObjectState();
    fldGlobalBase.deferredExit = FLD_DEFERRED_EXIT_MESSAGE_ACTIVE;
}

/* Completes the message-active deferred exit after message control ends, then
 * closes its display channel and restores the camera target. */
void fldFinishDeferredExit(void) {
    if (fldGlobalBase.deferredExit == FLD_DEFERRED_EXIT_MESSAGE_ACTIVE) {
        dspRequestMessageWindowModeOne();
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            evtFinishMessageWindowAndNotify();
            fclDestroyMessage();
            fldPreparePlayerSceneCameraTarget();
            fldGlobalBase.deferredExit = FLD_DEFERRED_EXIT_IDLE;
        }
    }
}

u8 fldIsEventPhaseAtLeastTwo(void) {
    if (D_003D62A8[0] < 2) {
        return 0;
    }
    return 1;
}

void fldTickWeatherEffectNodes(void) {
    fldUpdateWeatherEffectNodes();
}


s32 fldGetEncounterSelection(void) {
    return ((FldEnt110 *)D_003D62A4[0])->encounterSelection;
}
INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE78);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSeLastBgm);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneSoundBase);

INCLUDE_SDATA(const s32, "game/code_001411F0", gFldSe3DNum);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE90);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE94);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldCurrentBgmMode);

INCLUDE_SDATA(const s32, "game/code_001411F0", MidiNo);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldBgmReloadStep);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldArchiveLoadPending);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldFixedArchiveLoadPhase);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEAC);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldAutoMapField);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEB4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEB8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEBC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAECC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAED0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAED4);

INCLUDE_SDATA(const s32, "game/code_001411F0", AutoMapSeq);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldAutoMapHeader);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldAutoMapData);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldAutoMapBuf);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEE8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEEC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEFC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF00);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF04);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF08);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF0C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF10);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF14);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF18);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF1C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF20);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF24);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF28);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldNpcModelTable);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF30);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF34);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF38);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", D_003BAF3C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF40);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF44);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF48);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", D_003BAF4C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF50);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF54);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", D_003BAF58);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF5C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF60);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", D_003BAF64);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF68);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF6C);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", D_003BAF70);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF74);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF78);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", D_003BAF7C);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldTakaraCount);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF84);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF88);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldPrimaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSecondaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001411F0", mnuPositionedResourceCursor);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldBannerColorPhase);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF9C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFAC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFBC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFC0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFC4);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldDamEffectResource);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldDamEffectData);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", fldDamEffectNode);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldDamEffectPositioned);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldYukEffectResource);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldYukEffectData);

INCLUDE_SDATA(EffNode *, "game/code_001411F0", fldYukEffectNode);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldYukEffectPositioned);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFE8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFEC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFF0);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldWeatherLimitTexture);
