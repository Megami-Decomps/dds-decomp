#include "sdk_ps2.h"
#include "fld_area_work.h"
#include "fld_title_banner_menu.h"
#include "fld_activation_entry.h"
#include "fld_scene_record.h"
#include "common.h"
#include "fld_scene_status_api.h"
#include "evt_solar.h"
#include "eff_random_mod.h"
#include "sdf_trig.h"
#include "kwln_task_create.h"
#include "eff_manager_task.h"
#include "eff_random.h"
#include "dds3_path.h"
#include "dds3_slot1_handler_api.h"
extern void fldApplyCurrentTaskScene(const char *name);
#include "pcp_vu0.h"
#include "fpu.h"
#include "dds3obj.h"
#include "dds3_object_mode.h"
#include "eff_object.h"
#include "fld_player_scene_state.h"
#include "scr.h"
#include "scr_string_parameter_api.h"

/* The setter returns the context it set the value in (scrTraceCode.c). */
extern scrProcessWork_s *scrSetIntReturnValue(s32 retVal);

struct evtUnitHandle_t;
struct sdfMemHandle_t;

struct sdfTexHandle_t;


extern void fldSetRoomObjectModeFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);


extern s32 D_004362D4;

extern u32 D_004362D8;

extern s32 D_004362E0;

extern s32 D_004362E4;

extern s32 D_004362EC;

extern s32 D_004362F0;

extern s32 D_004362F8;

extern s32 D_004362FC;

extern s32 D_00436304;

extern s32 D_00436308;

extern s32 D_00436310;

extern s32 D_00436314;

extern s32 D_00436318;

extern s32 D_0043631C;

extern s32 D_00436338;

extern s32 D_0043633C;

extern s32 D_004363C4;

extern struct dds3Basic_t *fldPlayerObject;


extern s32 fldNpcModelTable;

extern u32 D_00436330[];

extern s32 fldTakaraCount;

extern s32 fldSceneSoundBase;

extern f32 fldCamTarget[];

extern s32 D_00436228;

extern u32 D_004363C8;

extern u32 D_004363CC;

extern u32 D_004363D0;

extern u32 D_004363D4;

extern struct sdfMemHandle_t *D_00436380;

extern u32 D_00436384;

extern u32 fldDamEffectNode;

extern u32 fldDamEffectPositioned;

extern struct sdfMemHandle_t *D_00436390;

extern u32 D_00436394;

extern u32 fldYukEffectNode;

extern u32 fldYukEffectPositioned;

extern struct sdfTexHandle_t *D_004363AC;

extern struct sdfTexHandle_t *D_004363B0;

extern struct sdfTexHandle_t *D_004363B4;

extern s32 D_00436370;

extern s32 D_00436374;

extern struct sdfTexHandle_t *D_00436368;

extern struct sdfTexHandle_t *D_0043636C;

extern struct sdfMemHandle_t *fldIndexedResourceHandle;

extern u32 fldIndexedResourceData;

extern s32 fldIndexedResourceEffect;

extern u32 D_0043623C;

extern u32 fldSeLastBgm;

extern u32 fldArchiveLoadPending;

extern u32 fldFixedArchiveLoadPhase;

extern u32 AutoMapSeq;

extern u32 D_00436364;

extern u32 D_00436354;

extern u32 D_0043635C;

extern s32 D_00436350;

extern s32 D_00436358;

extern s32 D_00436360;

extern struct sdfTexHandle_t *D_0043637C;

extern s32 fldFindEveRngIndex(u32);

extern s32 fldElvCheckEnd(void);


extern s32 fldQuerySelectedActorMotionState(s32 value);

extern s32 fldDispatchInteractionAction(s32 param0, s32 param1);

extern s32 fldGetActorSlotAttribute(s32 actorId, s32 attribute);

extern void fldInfToWAP(void);

extern void fldPauseBgm(void);

extern void fldBgmStop(void);

extern void fldBgmFadeOut(void);

extern void fldPlayMenuSound(s32 soundId);

extern void fldPlayFieldSe(s32 soundId);

extern s32 fldPollArchiveLoad(s32 archiveId);

extern void fldSetArchiveSoundVolumePan(s32 archiveId, s32 soundId);

extern void fldPlayArchiveSound(s32 archiveId, s32 soundId);

extern void fldStartTitle(s32 field, s32 mode, s32 option);

extern const char *fldEveHitResName_Find(u32 key);

extern void fldDoor_ExitCamera(const char *entry);


extern FldActivationEntry D_003A8EB0[];

extern void fldInitializeHuntSparkUnit(struct evtUnitHandle_t *unit);

extern struct evtUnitHandle_t *evtGetUnitHandle(s32 param);


extern s32 fldFindEffectByName(const char *name);

extern s32 fldGetCurrentSceneSelectionResource(void);

extern s32 p2sdGetPcmState(void);

extern u8 D_00436208[];

extern s32 gFldSe3DNum;



extern s32 fldCurrentBgmId[];

extern s32 D_00389780[];

extern s32 fldAutoMapField;

extern s32 D_00436248;

extern s32 fldPrimaryEffectPositionPending;

extern s32 fldSecondaryEffectPositionPending;

extern void fldResetPlayerSceneObjectState(void);

extern s32 D_003897C0[];
#include "dds3_world_secondary.h"
#include "sdf_debug_api.h"

extern s32 D_003897C8[];

extern s32 fldRunDanteGunFlag[];


extern s32 fldGetActorStat0(s32);

extern s32 fldGetMappedActorStateAttribute(s32);

extern s32 fldGetActorMotionEntry(s32);

extern s32 fldDoor_GetElvTbl(s32);

extern s32 fldGetSelectedActorMotionId(void);

extern void fldPreparePlayerSceneCameraTarget(void);

extern s32 D_0043633C;

extern s32 D_00389784[];

extern s32 fldSetMapTargetFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern s32 fldSetAlternateMapTargetFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern void fldClearFloorFlag(s32 area, s32 floor, s32 flagNumber);

extern void fldSetFloorFlag(s32 area, s32 floor, s32 flagNumber);

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 scrGetIntPara(s32);

extern s32 D_0038984C[];

extern s32 fldDoor_ChkWarp(void);

extern void fldMarkStageExplorationBitmapRegion(s32, s32, s32, s32, s32, s32);



extern s32 fldMirroredSolarThresholds[];

extern char fldRoomNameSentinel[];


extern void fldSetFadeTarget(s32 area, s32 value, s32 duration);

extern void fldSetFilterAnime(u32 mode);

extern s32 MidiNo;

extern s32 fldBgmReloadStep;

extern s32 D_00436300;

extern s32 D_0043630C;

#include "dds3_world_primary.h"




extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                       f32 *targetPositionOut);

extern void fldUpdateCameraProjectionEndpoints(void);

extern f32 D_0038BAD0[];

extern f32 D_0038BAE0[];

extern s32 D_004360B4;

extern s32 D_004360B8;

extern void fldSetRoomSceneFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);


extern char D_004363F0[]; /* "BARIA" */

extern char *D_004361D8;

extern void fldSetMapSlotValueFlag(s32 area, u32 slotIndex, s32 bit, s32 enabled);

extern s32 fldResolveEventSelectorTarget(s32 index);

extern s32 D_0043624C;

extern s32 D_00436250;

extern s32 D_00436254;

extern s32 D_00436258;

extern s32 D_0043625C;

extern s32 D_00436264;

extern s32 D_00436260;

extern s32 D_00436268;

extern s32 D_00436240;

extern s32 mnuPositionedResourceCursor;

extern u32 D_004362D8;

extern u32 D_004362DC;

extern u32 fldAreaDamageEffect;

extern s32 fldAreaDamageEffectPlaced;

extern u32 D_004362DC;

extern u32 D_004362E8;

extern u32 D_004362F4;

extern u32 fldAreaDamageEffect;

extern s32 fldAreaDamageEffectPlaced;


extern s32 D_00436378;

extern s16 D_004363BC;

extern s16 D_004363BE;

extern s16 D_004363C0;

extern s16 D_004363C2;

extern s32 D_004363DC;

extern f32 D_004363E0;

extern f32 D_004363E4;

void fldNpcHojiRefresh(void);

void fldResetAfterEvent(void);

void fldSetTargetGuideEnabled(s32 active);

void fldStartSceneBgm(void);

void fldStartSceneBgmAlternate(void);

extern s32 fldTestRoomProbeFacingAndRange(dds3Basic_t *actor, dds3Basic_t *entry);

extern s32 fldTestRoomProbeFacing(dds3Basic_t *actor, dds3Basic_t *entry);

extern s32 fldEveHitResID_Find(u32 task);
extern s32 fldResolvePendingTitleBanner(void);

s32 fldCmdQueryActorEntrySceneStatus(void) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *unit = fldPlayerObject;
    dds3Basic_t *entry;
    s32 result;

    if (unit == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    entry = dds3SearchSceneBasic(world, fldEveHitResID_Find((u32)scrGetScriptHandle()->task), 0x11);
    if (entry == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacingAndRange(unit, entry) == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(1);
    result = fldResolvePendingTitleBanner();
    switch (result) {
    case 1:
        scrSetIntReturnValue(1);
        return 1;
    case -1:
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCmdQueryAlternateActorEntrySceneStatus(void) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *unit = fldPlayerObject;
    dds3Basic_t *entry;
    s32 result;

    if (unit == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    entry = dds3SearchSceneBasic(world, fldEveHitResID_Find((u32)scrGetScriptHandle()->task), 0x11);
    if (entry == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacing(unit, entry) == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(1);
    result = fldResolvePendingTitleBanner();
    switch (result) {
    case 1:
        scrSetIntReturnValue(1);
        return 1;
    case -1:
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_ACT_WIN_DSP3(void) {
    s32 result;

    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(1);
    result = fldResolvePendingTitleBanner();
    if (result == -1) {
        scrSetIntReturnValue(-1);
        return 1;
    }
    if (result == 1) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCmdTestActorEntryCondition(void) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *unit = fldPlayerObject;
    dds3Basic_t *entry;

    if (unit == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    entry = dds3SearchSceneBasic(world, fldEveHitResID_Find((u32)scrGetScriptHandle()->task), 0x11);
    if (entry == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacingAndRange(unit, entry) == 0) {
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(1);
    }
    return 1;
}

extern void fldSetCameraNodeModeWithTen(void);

s32 fldCommand_PAD_DISABLE(void) {
    fldResetPlayerSceneObjectState();
    if (fldPlayerObject == 0) {
        return 1;
    }
    fldSetCameraNodeModeWithTen();
    return 1;
}

extern s32 D_003898B0[];

s32 fldCommand_PAD_ENABLE(void) {
    if (fldPlayerObject == 0) {
        return 1;
    }
    dds3ObjectBaseSetPrimaryControl(fldPlayerObject, 0);
    fldSetCameraNodeModeWithTen();
    fldPreparePlayerSceneCameraTarget();
    D_003898B0[0] = 0;
    return 1;
}

s32 fldCmdForceTargetGuideUpdate(void) {
    D_003898B0[0] = 1;
    return 1;
}

extern void fldEncountSetFlag(u32 mask);

u32 fldCommand_ENCOUNT_ON(void) {
    fldEncountSetFlag(0x10);
    return 1;
}

extern void fldEncountClearFlag(u32 mask);

u32 fldCommand_ENCOUNT_OFF(void) {
    fldEncountClearFlag(0x10);
    return 1;
}


extern void fldUpdateCameraViewFromPlayer(void);

/* The kind-4 lookup returns a camera with a separately owned world transform. */
s32 fldCmdFocusCameraOnObject(void) {
    FldAreaWork *work;
    dds3Basic_t *obj;
    dds3Basic_t *world = dds3GetControlScene();

    obj = dds3SearchSceneBasic(world, scrGetIntPara(0), 4);
    if (obj == NULL) {
        return 1;
    }
    work = &fldGlobalBase;
    work->mode = 1;
    work->focusPos[0] = obj->inner->position[0];
    work->focusPos[1] = obj->inner->position[1];
    work->focusPos[2] = obj->inner->position[2];
    fldUpdateCameraViewFromPlayer();
    return 1;
}

extern f32 fldLookAtNearPoint[];



extern s32 fldSynchronizePlayerPositionAndAreaState();

extern void fldClearCameraObjectHighlightFlag();

extern void fldInitializeCameraViewFromPlayer();

s32 fldUpdateLookAtSegment(void) {
    FldAreaWork *cam = &fldGlobalBase;
    FldPoint near;
    FldPoint far;

    cam->mode = 0;
    cam->negatedAngle = -cam->angle;
    near.x = cam->x - sdfSin((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + sdfCos((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSin(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + sdfCos(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    fldLookAtNearPoint[0] = near.x;
    fldLookAtNearPoint[1] = near.y;
    fldLookAtNearPoint[2] = near.z;
    fldCamTarget[0] = far.x;
    fldCamTarget[1] = far.y;
    fldCamTarget[2] = far.z;
    fldSynchronizePlayerPositionAndAreaState();
    fldClearCameraObjectHighlightFlag();
    fldInitializeCameraViewFromPlayer();
    return 1;
}

s32 fldCmdSetScenePhaseThree(void) {
    D_003897C0[0] = 3;
    return 1;
}

s32 fldCmdSetLookAtHeading(void) {
    FldAreaWork *cam;
    FldPoint near;
    FldPoint far;

    if (scrGetIntPara(0) < 0) {
        fldGlobalBase.negatedAngle = -fldGlobalBase.angle;
    } else {
        fldGlobalBase.negatedAngle = -45 * scrGetIntPara(0);
    }
    cam = &fldGlobalBase;
    near.x = cam->x - sdfSin((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + sdfCos((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSin(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + sdfCos(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    fldLookAtNearPoint[0] = near.x;
    fldLookAtNearPoint[1] = near.y;
    fldLookAtNearPoint[2] = near.z;
    fldCamTarget[0] = far.x;
    fldCamTarget[1] = far.y;
    fldCamTarget[2] = far.z;
    fldInitializeCameraViewFromPlayer();
    return 1;
}

s32 fldCommand_CAM_SEL_PHOKAN(void) {
    f32 pos[4];
    f32 rot[4];
    dds3Basic_t *target;
    dds3Basic_t *object;
    dds3Basic_t *world;

    if (scrGetIntPara(0) == -1) {
        target = fldGetPlayerSceneState();
        if (target == NULL) {
            return 1;
        }
        object = dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), target);
        if (object == NULL) {
            return 1;
        }
        dds3CameraObjectBasicGetTgtCam(object, pos, rot);
        fldUpdateCameraProjectionEndpoints();
        D_0038BAD0[0] = pos[0];
        D_0038BAD0[1] = pos[1];
        D_0038BAD0[2] = pos[2];
        D_0038BAE0[0] = rot[0];
        D_0038BAE0[1] = rot[1];
        D_0038BAE0[2] = rot[2];
        D_004360B4 = 0;
        D_004360B8 = scrGetIntPara(1);
        D_003897C0[0] = 4;
    } else {
        world = dds3GetActiveScene();
        target = dds3SearchSceneBasic(world, scrGetIntPara(0), 4);
        if (target == NULL) {
            return 1;
        }
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), target);
    }
    return 1;
}


s32 fldCommand_SET_NPC_CAMERA(void) {
    char *name;

    name = scrGetStrPara(0);
    fldGlobalBase.mode = 5;
    strcpy(fldGlobalBase.requestedSceneName, name);
    return 1;
}

extern void evtDisableMoonDraw(void);

extern void evtDisableMoonDraw2(void);

extern void evtEnableMoonDraw2(void);

s32 fldCmdSetSolarOverlayMode(void) {
    s32 mode = scrGetIntPara(0);

    fldGlobalBase.overlayMode = mode;
    fldGlobalBase.overlayCounter = 0;
    if (mode < 4) {
        fldGlobalBase.unkD4 = mode;
    }
    switch (fldGlobalBase.overlayMode) {
    case 0:
        evtDisableMoonDraw();
        break;
    case 1:
        evtSetSolarOverlayFullyVisible();
        fldGlobalBase.unk138 = 1;
        break;
    case 2:
        evtDisableMoonDraw2();
        fldGlobalBase.overlayCounter = 15;
        break;
    case 3:
        evtEnableMoonDraw2();
        fldGlobalBase.overlayCounter = 0;
        fldGlobalBase.unk138 = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    return 1;
}

void fldNoopWithMode(s32 mode) {
}

u32 func_00154F20(void) {
    return 1;
}

extern void fldFreeNextFile(void);

u32 fldCmdFreeDisplayObjects(void) {
    fldFreeNextFile();
    return 1;
}

s32 fldCommand_SEARCH_ID(const char *name) {
    dds3Basic_t *entry = dds3SceneBasicSearchName(dds3GetControlScene(), name);
    if (entry != 0) {
        return entry->key;
    }
    printf("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

s32 fldCommand_GetHojiID(char *name) {
    s32 index;
    if (strstr(name, fldRoomNameSentinel) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (name[index] == '\0') {
            if (index < 4) return 0;
            return (name[index - 2] - '0') * 10 + (name[index - 1] - '0');
        }
    }
    return 0;
}

s32 fldCommand_SET_CAMTBL(void) {
    s32 value;

    value = scrGetIntPara(0);
    D_003897C8[0] = value;
    return 1;
}

extern void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

s32 fldCommand_FLD_KANKYO(void) {
    s32 value;

    fldRunDanteGunFlag[11] = scrGetIntPara(0);
    value = scrGetIntPara(1);
    fldApplySkyLightSet(fldRunDanteGunFlag[11], value);
    return 1;
}

s32 fldCommand_FLD_FILTER(void) {
    s32 second;
    s32 third;
    fldRunDanteGunFlag[13] = scrGetIntPara(0) & 0xFF;
    second = scrGetIntPara(1);
    third = scrGetIntPara(2);
    fldSetFadeTarget(fldRunDanteGunFlag[13], second, third);
    fldSetFilterAnime(scrGetIntPara(3));
    return 1;
}

extern void fldSetRoomModeFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);



#include "dds3_object_payload.h"

s32 fldCommand_FLD_MODEL_ON(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    WorldIndexNode *list;
    dds3Basic_t *node;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    mode = scrGetIntPara(3);
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetRoomModeFlag(world, stage, room, 0);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetRoomModeFlag(world, stage, room, 0);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        list = dds3SceneBasicGetList(dds3GetControlScene(), 6);
        if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)list) != 0) {
            dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list);
            do {
                node = (dds3Basic_t *)(u32)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
                if (((EffectTransformData *)node->data)->roomNumber == (u32)room) {
                    switch (mode) {
                    case 0:
                        dds3MatterObjectBasicSetAlpMode(node, 1);
                        break;
                    case 1:
                        dds3MatterObjectBasicSetAlpMode(node, 5);
                        break;
                    case 2:
                        dds3MatterObjectBasicSetAlpMode(node, 7);
                        break;
                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);
            dds3FreeBasicArrayHandle(list);
        }
    }
    return 1;
}

extern char D_00414090[];


/* Numeric form of fldCommand_FLD_MODEL_ON: the room number parameter is
 * formatted to a name and parsed like the string command's argument. */
s32 fldCmdApplyNumericRoomModeGroupZero(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char buffer[32];
    char *name;
    s32 room;
    WorldIndexNode *list;
    dds3Basic_t *node;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    mode = scrGetIntPara(3);
    sprintf(buffer, D_00414090, scrGetIntPara(2));
    name = buffer;
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetRoomModeFlag(world, stage, room, 0);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetRoomModeFlag(world, stage, room, 0);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        list = dds3SceneBasicGetList(dds3GetControlScene(), 6);
        if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)list) != 0) {
            dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list);
            do {
                node = (dds3Basic_t *)(u32)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
                if (((EffectTransformData *)node->data)->roomNumber == (u32)room) {
                    switch (mode) {
                    case 0:
                        dds3MatterObjectBasicSetAlpMode(node, 1);
                        break;
                    case 1:
                        dds3MatterObjectBasicSetAlpMode(node, 5);
                        break;
                    case 2:
                        dds3MatterObjectBasicSetAlpMode(node, 7);
                        break;
                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);
            dds3FreeBasicArrayHandle(list);
        }
    }
    return 1;
}

extern char *D_004361D4;

s32 fldCmdApplyRoomModeGroupOne(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    WorldIndexNode *list;
    dds3Basic_t *node;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    mode = scrGetIntPara(3);
    name = scrGetStrPara(2);
    if (strcmp(name, D_004363F0) == 0) {
        name = D_004361D4;
    }
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetRoomModeFlag(world, stage, room, 1);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetRoomModeFlag(world, stage, room, 1);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        list = dds3SceneBasicGetList(dds3GetControlScene(), 6);
        if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)list) != 0) {
            dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list);
            do {
                node = (dds3Basic_t *)(u32)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
                if (((EffectTransformData *)node->data)->roomNumber == (u32)room) {
                    switch (mode) {
                    case 0:
                        dds3MatterObjectBasicSetAlpMode(node, 2);
                        break;
                    case 1:
                        dds3MatterObjectBasicSetAlpMode(node, 6);
                        break;
                    case 2:
                        dds3MatterObjectBasicSetAlpMode(node, 8);
                        break;
                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);
            dds3FreeBasicArrayHandle(list);
        }
    }
    return 1;
}

/* Numeric form of fldCmdApplyRoomModeGroupOne (without its alias-name remap). */
s32 fldCmdApplyNumericRoomModeGroupOne(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char buffer[32];
    char *name;
    s32 room;
    WorldIndexNode *list;
    dds3Basic_t *node;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    mode = scrGetIntPara(3);
    sprintf(buffer, D_00414090, scrGetIntPara(2));
    name = buffer;
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetRoomModeFlag(world, stage, room, 1);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetRoomModeFlag(world, stage, room, 1);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        list = dds3SceneBasicGetList(dds3GetControlScene(), 6);
        if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)list) != 0) {
            dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list);
            do {
                node = (dds3Basic_t *)(u32)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
                if (((EffectTransformData *)node->data)->roomNumber == (u32)room) {
                    switch (mode) {
                    case 0:
                        dds3MatterObjectBasicSetAlpMode(node, 2);
                        break;
                    case 1:
                        dds3MatterObjectBasicSetAlpMode(node, 6);
                        break;
                    case 2:
                        dds3MatterObjectBasicSetAlpMode(node, 8);
                        break;
                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);
            dds3FreeBasicArrayHandle(list);
        }
    }
    return 1;
}

s32 fldCommand_FLD_ANIME_ON(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;
    s32 searchId;
    void *obj;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    if (world == fldGlobalBase.area) {
        if (stage == fldGlobalBase.floor + 1) {
            searchId = fldCommand_SEARCH_ID(name);
            if (searchId != -1) {
                obj = dds3SearchSceneBasic(dds3GetControlScene(), searchId, 6);
                switch (scrGetIntPara(3)) {
                case 0:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 0);
                    break;
                case 1:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 1);
                    break;
                case 2:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 2);
                    break;
                case 3:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 6);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldApplyRoomObjectModeZero(s32 world, s32 stage, char *roomName, s32 mode) {
    s32 i;
    s32 searchId;
    void *obj;

    if (world == 0) {
        world = fldGlobalBase.area;
    }
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    if (roomName == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldCommand_GetHojiID(roomName), 0);
    }
    if (world == fldGlobalBase.area) {
        if (stage == fldGlobalBase.floor + 1) {
            searchId = fldCommand_SEARCH_ID(roomName);
            if (searchId != -1) {
                obj = dds3SearchSceneBasic(dds3GetControlScene(), searchId, 6);
                switch (mode) {
                case 0:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 0);
                    break;
                case 1:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 1);
                    break;
                case 2:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 2);
                    break;
                case 3:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 6);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;
    s32 searchId;
    void *obj;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    if (world == fldGlobalBase.area) {
        if (stage == fldGlobalBase.floor + 1) {
            searchId = fldCommand_SEARCH_ID(name);
            if (searchId != -1) {
                obj = dds3SearchSceneBasic(dds3GetControlScene(), searchId, 6);
                switch (scrGetIntPara(3)) {
                case 0:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 3);
                    break;
                case 1:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 4);
                    break;
                case 2:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 5);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldApplyRoomObjectModeOne(s32 world, s32 stage, char *roomName, s32 mode) {
    s32 i;
    s32 searchId;
    void *obj;

    if (world == 0) {
        world = fldGlobalBase.area;
    }
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    if (roomName == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldCommand_GetHojiID(roomName), 1);
    }
    if (world == fldGlobalBase.area) {
        if (stage == fldGlobalBase.floor + 1) {
            searchId = fldCommand_SEARCH_ID(roomName);
            if (searchId != -1) {
                obj = dds3SearchSceneBasic(dds3GetControlScene(), searchId, 6);
                switch (mode) {
                case 0:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 3);
                    break;
                case 1:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 4);
                    break;
                case 2:
                    dds3ObjectBaseMatterAnimeSpeed(obj, 5);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldCommand_FLD_ATARI_ON(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomSceneFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomSceneFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    return 1;
}

s32 fldCmdSetSceneBitsBarrierRoom(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = scrGetStrPara(2);
    if (strcmp(name, D_004363F0) == 0) {
        name = D_004361D8;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomSceneFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomSceneFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    return 1;
}

s32 fldCommand_FLD_MODEL_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_MODEL_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 1);
    return 1;
}

s32 fldCommand_FLD_ANIME_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetAlternateMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetAlternateMapTargetFlag(area, floor, target, 1);
    return 1;
}

extern void fldSetMapSlotByte(s32 area, s32 slotIndex, s32 valueIndex, s32 value);

s32 fldCommand_FLD_ANIME_ID2(void) {
    s32 world;
    s32 stage;
    s32 slot;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    slot = scrGetIntPara(2);
    if (slot == 0) {
        return 1;
    }
    fldSetMapSlotByte(world, stage, slot, scrGetIntPara(3));
    return 1;
}

s32 fldCommand_FLD_MAP_ON(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = D_00389780[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldSetFloorFlag(area, floor, flagNumber);
    return 1;
}

s32 fldCommand_FLD_MAP_OFF(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = D_00389780[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldClearFloorFlag(area, floor, flagNumber);
    return 1;
}

extern void fldSetMapSlotAuxiliaryFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

s32 fldOpClearRoomStateFlags(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetMapSlotAuxiliaryFlag(world, stage, i, 0);
        }
    } else {
        fldSetMapSlotAuxiliaryFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    if (world == fldGlobalBase.area) {
        if (stage == fldGlobalBase.floor + 1) {
            fldNpcHojiRefresh();
        }
    }
    return 1;
}

s32 fldOpSetRoomStateFlags(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetMapSlotAuxiliaryFlag(world, stage, i, 1);
        }
    } else {
        fldSetMapSlotAuxiliaryFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    if (world == fldGlobalBase.area) {
        if (stage == fldGlobalBase.floor + 1) {
            fldNpcHojiRefresh();
        }
    }
    return 1;
}

s32 fldCmdSetSceneBitsValue(void) {
    s32 world;
    s32 stage;
    s32 room;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    room = scrGetIntPara(2);
    if (room == 0) {
        for (; room < 16; room++) {
            fldSetMapSlotValueFlag(world, stage, room, scrGetIntPara(3));
        }
    } else {
        fldSetMapSlotValueFlag(world, stage, room, scrGetIntPara(3));
    }
    return 1;
}

s32 fldCommand_FLD_CHK_BTL(void) {
    if (fldGetRestoreMode() == 1) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_ANA(void) {
    if (fldGetRestoreMode() == 3) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_SISETU(void) {
    if (fldGetRestoreMode() == 4) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_EVE(void) {
    if (fldGetRestoreMode() == 2) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00154558", D_00414090);

s32 fldCmdPushSceneParam(void) {
    s32 room;

    room = fldFindEveRngIndex((u32)scrGetScriptHandle()->task);
    switch (scrGetIntPara(0)) {
    case 0:
        scrSetIntReturnValue(D_003A8EB0[room].selectedValue0);
        break;
    case 1:
        scrSetIntReturnValue(D_003A8EB0[room].selectedValue1);
        break;
    case 2:
        scrSetIntReturnValue(D_003A8EB0[room].selectedValue2);
        break;
    case 3:
        scrSetIntReturnValue(D_003A8EB0[room].selectedValue3);
        break;
    case 4:
        scrSetIntReturnValue(D_003A8EB0[room].selectedValue4);
        break;
    }
    return 1;
}

extern void fldActivateFlaggedObject(u32 flagIndex);

u32 fldCommand_FLD_SET_TAKARA_OPEN(void) {
    scrProcessWork_s *script;
    s32 room;

    script = scrGetScriptHandle();
    room = fldFindEveRngIndex((u32)script->task);
    fldActivateFlaggedObject(room);
    return 1;
}

extern u8 fldTestObjectActivationFlag(u32 flagIndex);

u32 fldCommand_FLD_CHK_TAKARA_OPEN(void) {
    if (fldTestObjectActivationFlag(fldFindEveRngIndex((u32)scrGetScriptHandle()->task)) != 0) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

u32 fldCmdSetCurrentTaskScene(void) {
    const char *scene;

    if (fldDoor_ChkWarp()) {
        fldApplyCurrentTaskScene(0);
        return 1;
    }
    scene = fldEveHitResName_Find((u32)scrGetScriptHandle()->task);
    if (scene) {
        fldApplyCurrentTaskScene(scene);
    }
    return 1;
}

extern void func_001411F8(s32 action);

u32 fldCmdExecuteSceneAction(void) {
    s32 sceneAction;

    sceneAction = scrGetIntPara(0);
    func_001411F8(sceneAction);
    return 1;
}

u32 func_001569E0(void) {
    return 1;
}

u32 fldCmdGetActorStat0(void) {
    s32 actorStat;

    actorStat = scrGetIntPara(0);
    actorStat = fldGetActorStat0(actorStat);
    scrSetIntReturnValue(actorStat);
    return 1;
}

u32 fldCmdGetActorStat1(void) {
    s32 actorStat;

    actorStat = scrGetIntPara(0);
    actorStat = fldGetMappedActorStateAttribute(actorStat);
    scrSetIntReturnValue(actorStat);
    return 1;
}

u32 fldCommand_FLD_WARP_HASIGO_GETTBL(void) {
    s32 motionEntry;

    motionEntry = scrGetIntPara(0);
    motionEntry = fldGetActorMotionEntry(motionEntry);
    scrSetIntReturnValue(motionEntry);
    return 1;
}

u32 fldCommand_FLD_WARP_ELV_GETTBL(void) {
    s32 rowValue;

    rowValue = scrGetIntPara(0);
    rowValue = fldDoor_GetElvTbl(rowValue);
    scrSetIntReturnValue(rowValue);
    return 1;
}

extern void fldDispatchSceneRoomSelection(s32 index);

u32 fldCommand_FLD_WARP_ELV_EXIT(void) {
    s32 value;

    value = scrGetIntPara(0);
    fldDispatchSceneRoomSelection(value);
    return 1;
}

extern void fldElvSetFloor(s32 start, s32 end);

u32 fldCommand_FLD_ELV_FLOORSTART(void) {
    s32 first;
    s32 second;

    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    fldElvSetFloor(first, second);
    return 1;
}

extern void fldResetCameraMoveTracking(void);

u32 fldCommand_FLD_ELV_FLOOREND(void) {
    fldResetCameraMoveTracking();
    return 1;
}

s32 fldCommand_FLD_ELV_ENDCHK(void) {
    scrSetIntReturnValue(fldElvCheckEnd());
    return 1;
}

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
s32 fldCommand_FLD_WARP_GET_DOOR(void) {
    scrSetIntReturnValue(fldQuerySelectedActorMotionState(scrGetIntPara(0)));
    return 1;
}

extern void fldApplyActorEntryTrigger(s32 useTaskRecord);

u32 fldCommand_FLD_WARP_RUN_START(void) {
    s32 scene;
    if (fldDoor_ChkWarp()) {
        scene = 0;
    } else {
        scene = fldEveHitResID_Find((u32)scrGetScriptHandle()->task);
    }
    fldApplyActorEntryTrigger(scene);
    return 1;
}

void fldCmdResetAfterEvent(void) {
    fldResetAfterEvent();
}

s32 fldCommand_FLD_INF_NEXT(void) {
    s32 param0 = scrGetIntPara(0);
    s32 param1 = scrGetIntPara(1);

    scrSetIntReturnValue(fldDispatchInteractionAction(param0, param1));
    return 1;
}

s32 fldCommand_FLD_INF_GET(void) {
    s32 actorId = scrGetIntPara(0);
    s32 attribute = scrGetIntPara(1);

    scrSetIntReturnValue(fldGetActorSlotAttribute(actorId, attribute));
    return 1;
}

s32 fldCommand_FLD_INF_GETPANEL(void) {
    return (s32)scrSetIntReturnValue(fldResolveEventSelectorTarget(1));
}

s32 fldCommand_FLD_INF_GOWAP(void) {
    fldInfToWAP();
    return 1;
}

s32 fldCmdStartSceneBgm(void) {
    fldCurrentBgmId[0] = scrGetIntPara(0);
    fldStartSceneBgm();
    return 1;
}

s32 fldCmdStartSceneBgmAlternate(void) {
    fldCurrentBgmId[0] = scrGetIntPara(0);
    fldStartSceneBgmAlternate();
    return 1;
}

s32 fldCmdStopCurrentBgm(void) {
    fldPauseBgm();
    return 1;
}

s32 fldCmdPlayCurrentBgmSound(void) {
    fldBgmStop();
    return 1;
}

s32 fldCmdReleaseCurrentBgm(void) {
    fldBgmFadeOut();
    return 1;
}

s32 fldCommandPlaySeVolumePan(void) {
    fldPlayMenuSound(scrGetIntPara(0));
    return 1;
}

s32 fldCommandPlaySe(void) {
    fldPlayFieldSe(scrGetIntPara(0));
    return 1;
}

u8 fldCommandLoadArchive(void) {
    return fldPollArchiveLoad(scrGetIntPara(0)) != 0;
}

s32 fldCommand_FLD_MSE_PLAY2(void) {
    s32 sequenceGroup = scrGetIntPara(0);
    s32 sequenceIndex = scrGetIntPara(1);

    fldSetArchiveSoundVolumePan(sequenceGroup, sequenceIndex);
    return 1;
}

s32 fldCommand_FLD_MSE_STOP2(void) {
    s32 sequenceGroup = scrGetIntPara(0);
    s32 sequenceIndex = scrGetIntPara(1);

    fldPlayArchiveSound(sequenceGroup, sequenceIndex);
    return 1;
}

s32 fldCommandStartTitle(void) {
    s32 fieldId = scrGetIntPara(0);
    s32 titleArg = scrGetIntPara(1);

    fldStartTitle(fieldId, titleArg, 0x3c);
    return 1;
}

s32 fldCommand_FLD_SET_ENCOUNT(void) {
    s32 value;

    value = scrGetIntPara(0);
    D_0038984C[0] = value;
    return 1;
}

s32 fldCommand_FLD_EXIT_CAMERA(void) {
    scrProcessWork_s *work = scrGetScriptHandle();
    const char *entry = fldEveHitResName_Find((u32)work->task);

    if (entry != NULL) {
        fldDoor_ExitCamera(entry);
    }
    return 1;
}

s32 fldCommand_FLD_WARP_GET_WARPEFF(void) {
    scrSetIntReturnValue(fldGetSelectedActorMotionId());
    return 1;
}

s32 fldCmdInitializeHuntSparkUnit(void) {
    fldInitializeHuntSparkUnit(evtGetUnitHandle(scrGetIntPara(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 fldCommandFindEffectByName(void) {
    char *param = scrGetStrPara(0);

    scrSetIntReturnValue(fldFindEffectByName(param));
    return 1;
}

s32 fldCmdGetCurrentSceneSelectionResource(void) {
    scrSetIntReturnValue(fldGetCurrentSceneSelectionResource());
    return 1;
}

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommand_FLD_GET_KIRETA(void) {
    s32 changed = 0;
    switch (scrGetIntPara(0)) {
    case 0:
        if (fldGlobalBase.consumedFlags & 1) {
            fldGlobalBase.consumedFlags &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (fldGlobalBase.consumedFlags & 2) {
            fldGlobalBase.consumedFlags &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (fldGlobalBase.consumedFlags & 4) {
            fldGlobalBase.consumedFlags &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (fldGlobalBase.consumedFlags & 8) {
            fldGlobalBase.consumedFlags &= ~8;
            changed = 1;
        }
        break;
    }
    scrSetIntReturnValue(changed);
    return 1;
}

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void) {
    s32 solarPhaseThreshold = fldMirroredSolarThresholds[evtGetMirroredSolarPhase()];
    s32 roll = (s32)dds3GetRandIntA(0, 100);
    if (solarPhaseThreshold >= roll) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

extern s32 p2sdLoadPcmFile(const char *name, u8 channel);
extern void p2sdSyncSound(void);

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void) {
    s32 commandName;
    commandName = scrGetStrPara(0);
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    p2sdLoadPcmFile(commandName, 0x7f);
    return 1;
}

u32 fldCommandSendSoundControl(void) {
    p2sdSyncSound();
    return 1;
}

/* Return sound-command busy state to the field script interpreter. */
s32 fldCommandIsSoundBusy(void) {
    scrSetIntReturnValue(p2sdGetPcmState());
    return 1;
}

/* Pass five field-script arguments to the underlying handler. */
s32 fldCmdMarkBitmapRegion(void) {
    s32 first = scrGetIntPara(0);
    s32 second = scrGetIntPara(1);
    s32 third = scrGetIntPara(2);
    s32 fourth = scrGetIntPara(3);
    fldMarkStageExplorationBitmapRegion(D_00389780[0], first, second, third, fourth, scrGetIntPara(4));
    return 1;
}

extern s32 D_003898A4[];

s32 fldCmdSetStateFlag(void) {
    D_003898A4[0] = 1;
    return 1;
}

typedef struct FldSceneParam {
    s32 unk0;
    s32 value;
} FldSceneParam;

extern FldSceneParam *D_00435F1C;

s32 fldCommand_FLD_MODEL_DANTE(void) {
    if (D_00435F1C == NULL) {
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(D_00435F1C->value);
    return 1;
}

s32 fldCmdSetTargetGuideEnabled(void) {
    if (scrGetIntPara(0) == 2) {
        fldSetTargetGuideEnabled(0);
        D_003898B0[0] = 0;
    } else if (scrGetIntPara(0) == 0) {
        fldSetTargetGuideEnabled(0);
    } else {
        fldSetTargetGuideEnabled(1);
    }
    return 1;
}

extern void fldSetTargetGuideMode(s32);

s32 fldCmdSetTargetGuideMode(void) {
    fldSetTargetGuideMode(scrGetIntPara(0));
    return 1;
}


extern void effLoadCommonTexturesAndResetRenderFlags(void);

extern void effInitializeBillResourceOwners(void);

extern void dds3PolygonManagerInit(void);

extern void dds3ParticleManagerInit(void);

extern void effFieldTaskStartupHook(void);

extern void Eve_EffectInit(void);

extern void effBTLFieldColorResetFlags(void);

extern EffRandState effDefaultRandomState[];

extern char D_004363F8[];

void fldCreateFieldEffectTask(void) {
    sdfRandomizeByTime((EffRandState *)effDefaultRandomState);
    effLoadCommonTexturesAndResetRenderFlags();
    effInitializeBillResourceOwners();
    dds3PolygonManagerInit();
    dds3ParticleManagerInit();
    dds3StartProcess("effect_f", 0x2B04, 0, 0, effSubmitFieldBlendStateTask, NULL, 0);
    dds3StartProcess(D_004363F8, 0x2B18, 0, 0, dds3EffectManagerRun, effManagerInitializeSubsystems, 0);
    effFieldTaskStartupHook();
    Eve_EffectInit();
    effBTLFieldColorResetFlags();
}

INCLUDE_SDATA(const s32, "game/code_00154558", fldRoomNameSentinel);

INCLUDE_SDATA(const s32, "game/code_00154558", D_004363F0);

INCLUDE_SDATA(const s32, "game/code_00154558", D_004363F8);
