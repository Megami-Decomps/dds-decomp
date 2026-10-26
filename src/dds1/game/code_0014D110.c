#include "sdk_ps2.h"
#include "fld_area_work.h"
#include "fld_camera_move_tracking.h"
#include "fld_title_banner_menu.h"
#include "common.h"
#include "fld_scene_status_api.h"
#include "sdf_trig.h"
#include "fld_scene_record.h"
#include "dds3_path.h"
#include "dds3_slot1_handler_api.h"
struct dds3ProcessID_t;
extern const char *fldEveHitResName_Find(struct dds3ProcessID_t *task);
extern void fldApplyCurrentTaskScene(const char *name);
#include "sdf.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "dds3_object_mode.h"
#include "eff_object.h"
#include "fld_player_scene_state.h"
#include "scr.h"
#include "scr_string_parameter_api.h"
#include "scr_integer_return_value_api.h"
#include "fld_scene_object_key_result.h"
#include "fld_activation_entry.h"



extern s32 fldDoor_GetElvTbl(s32);

extern s32 fldGetActorMotionEntry(s32);

extern s32 fldGetActorStat1(s32);

extern s32 fldGetActorStat0(s32);

extern s32 scrGetIntPara(s32);
extern void fldDispatchSceneAction(u32 action);


extern s16 fldFindRoomIdByTask(struct dds3ProcessID_t *task);

extern u32 fldDamEffectResource;

extern u32 fldDamEffectData;


extern u32 fldDamEffectPositioned;

extern u32 fldYukEffectResource;

extern u32 fldYukEffectData;


extern u32 fldYukEffectPositioned;

extern sdfTexHandle_t *fldWeatherLimitTexture;

extern s32 D_003BAFBC;

extern sdfTexHandle_t *D_003BAFC4;

extern sdfTexHandle_t *D_003BAFB4;

extern s32 D_003BAFA0;

extern s32 D_003BAFA8;

extern u32 D_003BAFB0;

extern u32 AutoMapSeq;

extern u32 fldFixedArchiveLoadPhase;

extern u32 fldArchiveLoadPending;

extern u32 fldSeLastBgm;

extern s32 gFldSe3DNum;

extern s32 fldAutoMapField;

extern s32 D_003BAEB4;



extern s32 fldPrimaryEffectPositionPending;

extern s32 fldSecondaryEffectPositionPending;

#include "dds3_world_primary.h"


extern char D_003BB000[]; /* "BARIA" */

extern char *D_003BAE44;


extern s32 fldTestRoomProbeFacingAndRange(dds3Basic_t *actor, dds3Basic_t *entry);

extern s32 fldTestRoomProbeFacing(dds3Basic_t *actor, dds3Basic_t *entry);

extern void fldSetCameraNodeModeWithTen(void);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern void fldResetPlayerSceneObjectState(void);


extern s32 D_0032E400[];

extern s32 D_0032E408[];

extern s32 D_0032E3C0[];

extern s32 fldSceneSoundBase;

extern s32 fldRunDanteGunFlag[];

extern u8 D_003BAE78[];
#include "dds3_world_secondary.h"
#include "sdf_debug_api.h"

extern s32 fldCurrentBgmMode;

extern s32 MidiNo;

extern s32 fldBgmReloadStep;

extern s32 D_003BAEB8;

extern s32 D_003BAEBC;

extern s32 D_003BAEC0;

extern s32 D_003BAEC4;

extern s32 D_003BAEC8;

extern s32 D_003BAED0;

extern s32 D_003BAECC;

extern s32 D_003BAED4;

extern s32 D_003BAEAC;

extern s32 D_003BAF88;

extern s32 mnuPositionedResourceCursor;

extern s32 D_003BD7EC, D_003BD7F0, D_003BAF68, D_003BAF6C, D_003BAF50, D_003BAF54;

extern s32 D_003BAF74, D_003BAF78, D_003BAF5C, D_003BAF60, D_003BAF84, D_003BAF88;

extern s32 D_003BAF48, D_003BAF40;

extern struct EffNode *D_003BAF4C, *D_003BAF3C, *D_003BAF70, *D_003BAF7C;

extern sdfTexHandle_t *D_003BAF30, *D_003BAF34, *D_003BAF38;

extern s32 fldNpcModelTable;

extern s32 fldTakaraCount;

extern f32 fldBannerColorPhase;


extern s32 fldNpcModelTable;


extern u32 D_003BAFA4;

extern s32 D_003BAFAC;

extern s32 D_003BAF9C;

extern void fldPreparePlayerSceneCameraTarget(void);

extern void evtSetSolarOverlayFullyVisible(void);


void fldNpcHojiRefresh(void);

extern s32 fldEveHitResID_Find(struct dds3ProcessID_t *task);
s32 fldCmdQueryActorEntrySceneStatus(void) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *unit = dds3GetActiveUnitSceneBasic(world);
    dds3Basic_t *entry;
    FldTitleBannerResult result;

    if (unit == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    entry = dds3SearchSceneBasic(world, fldEveHitResID_Find(scrGetScriptHandle()->task), 0x11);
    if (entry == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacingAndRange(unit, entry) == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(FLD_TITLE_BANNER_DRAW_REQUESTED);
    result = fldResolvePendingTitleBanner();
    switch (result) {
    case FLD_TITLE_BANNER_RESOLVED:
        scrSetIntReturnValue(1);
        return 1;
    case FLD_TITLE_BANNER_NO_PENDING:
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCmdQueryAlternateActorEntrySceneStatus(void) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *unit = dds3GetActiveUnitSceneBasic(world);
    dds3Basic_t *entry;
    FldTitleBannerResult result;

    if (unit == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    entry = dds3SearchSceneBasic(world, fldEveHitResID_Find(scrGetScriptHandle()->task), 0x11);
    if (entry == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacing(unit, entry) == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(FLD_TITLE_BANNER_DRAW_REQUESTED);
    result = fldResolvePendingTitleBanner();
    switch (result) {
    case FLD_TITLE_BANNER_RESOLVED:
        scrSetIntReturnValue(1);
        return 1;
    case FLD_TITLE_BANNER_NO_PENDING:
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_ACT_WIN_DSP3(void) {
    FldTitleBannerResult result;

    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(FLD_TITLE_BANNER_DRAW_REQUESTED);
    result = fldResolvePendingTitleBanner();
    if (result == FLD_TITLE_BANNER_NO_PENDING) {
        scrSetIntReturnValue(-1);
        return 1;
    }
    if (result == FLD_TITLE_BANNER_RESOLVED) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCmdTestActorEntryCondition(void) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *unit = dds3GetActiveUnitSceneBasic(world);
    dds3Basic_t *entry;

    if (unit == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    entry = dds3SearchSceneBasic(world, fldEveHitResID_Find(scrGetScriptHandle()->task), 0x11);
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

s32 fldCommand_PAD_DISABLE(void) {
    dds3Basic_t *object;

    fldResetPlayerSceneObjectState();
    object = dds3GetActiveUnitSceneBasic((dds3Basic_t *)dds3GetActiveScene());
    if (object == 0) {
        return 1;
    }
    fldSetCameraNodeModeWithTen();
    return 1;
}

s32 fldCommand_PAD_ENABLE(void) {
    dds3Basic_t *object;

    object = dds3GetActiveUnitSceneBasic((dds3Basic_t *)dds3GetActiveScene());
    if (object == 0) {
        return 1;
    }
    dds3ObjectBaseSetPrimaryControl(object, 0);
    fldSetCameraNodeModeWithTen();
    fldPreparePlayerSceneCameraTarget();
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
    work->mode = FLD_CAMERA_MODE_OBJECT_FOCUS;
    work->focusPos[0] = obj->inner->position[0];
    work->focusPos[1] = obj->inner->position[1];
    work->focusPos[2] = obj->inner->position[2];
    fldUpdateCameraViewFromPlayer();
    return 1;
}

extern FldPoint fldLookAtNearPoint;

extern FldPoint fldCamTarget;



extern s32 fldUpdateCameraObjectPose();

extern void fldClearCameraObjectHighlightFlag();

extern void fldUpdateFieldCameraViewState();

s32 fldUpdateLookAtSegment(void) {
    FldAreaWork *cam = &fldGlobalBase;
    FldPoint near;
    FldPoint far;

    cam->mode = FLD_CAMERA_MODE_PLAYER_FOLLOW;
    cam->negatedAngle = -cam->angle;
    near.x = cam->x - sdfSin((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + sdfCos((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSin(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + sdfCos(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    fldLookAtNearPoint.x = near.x;
    fldLookAtNearPoint.y = near.y;
    fldLookAtNearPoint.z = near.z;
    fldCamTarget.x = far.x;
    fldCamTarget.y = far.y;
    fldCamTarget.z = far.z;
    fldUpdateCameraObjectPose();
    fldClearCameraObjectHighlightFlag();
    fldUpdateFieldCameraViewState();
    return 1;
}

s32 fldCmdSetScenePhaseThree(void) {
    D_0032E400[0] = 3;
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
    fldLookAtNearPoint.x = near.x;
    fldLookAtNearPoint.y = near.y;
    fldLookAtNearPoint.z = near.z;
    fldCamTarget.x = far.x;
    fldCamTarget.y = far.y;
    fldCamTarget.z = far.z;
    fldUpdateFieldCameraViewState();
    return 1;
}




extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                       f32 *targetPositionOut);

extern void fldUpdateCameraProjectionEndpoints(void);

extern f32 D_00330630[];

extern f32 D_00330640[];

extern s32 D_003BAD24;

extern s32 D_003BAD28;

s32 fldCommand_CAM_SEL_PHOKAN(void) {
    f32 pos[4];
    f32 rot[4];
    dds3Basic_t *target;
    dds3Basic_t *previousCamera;
    dds3Basic_t *world;

    if (scrGetIntPara(0) == -1) {
        target = fldGetPlayerSceneState();
        if (target == NULL) {
            return 1;
        }
        previousCamera = dds3ReplaceActiveCameraSceneBasic(dds3GetActiveScene(), target);
        if (previousCamera == NULL) {
            return 1;
        }
        dds3CameraObjectBasicGetTgtCam(previousCamera, pos, rot);
        fldUpdateCameraProjectionEndpoints();
        D_00330630[0] = pos[0];
        D_00330630[1] = pos[1];
        D_00330630[2] = pos[2];
        D_00330640[0] = rot[0];
        D_00330640[1] = rot[1];
        D_00330640[2] = rot[2];
        D_003BAD24 = 0;
        D_003BAD28 = scrGetIntPara(1);
        D_0032E400[0] = 4;
    } else {
        world = dds3GetActiveScene();
        target = dds3SearchSceneBasic(world, scrGetIntPara(0), 4);
        if (target == NULL) {
            return 1;
        }
        dds3ReplaceActiveCameraSceneBasic(dds3GetActiveScene(), target);
    }
    return 1;
}


s32 fldCommand_SET_NPC_CAMERA(void) {
    const char *name;

    name = scrGetStrPara(0);
    fldGlobalBase.mode = FLD_CAMERA_MODE_NPC_REQUEST;
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
        fldGlobalBase.mapBannerFadeMode = mode;
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
        fldGlobalBase.overlayCounter = 0xF;
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

u32 func_0014DAF8(void) {
    return 1;
}

extern void fldFreeNextFile(void);

u32 fldCmdFreeDisplayObjects(void) {
    fldFreeNextFile();
    return 1;
}

FldSceneObjectKeyResult fldCommand_SEARCH_ID(const char *name) {
    dds3Basic_t *entry = dds3SceneBasicSearchName(dds3GetControlScene(), name);
    if (entry != 0) {
        return entry->key;
    }
    printf("field SEARCH_ID NotFound:[%s]\n", name);
    return FLD_SCENE_OBJECT_KEY_NOT_FOUND;
}

extern char fldRoomNameSentinel[];


s32 fldCommand_GetHojiID(const char *name) {
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
    D_0032E408[0] = value;
    return 1;
}

extern void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

s32 fldCommand_FLD_KANKYO(void) {
    s32 durationMode;

    fldRunDanteGunFlag[11] = scrGetIntPara(0);
    durationMode = scrGetIntPara(1);
    fldApplySkyLightSet(fldRunDanteGunFlag[11], durationMode);
    return 1;
}

extern void fldSetFadeTarget(s32 area, s32 value, s32 duration);

extern void fldSetFilterAnime(u32 mode);

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
extern u8 fldTestObjectActivationFlag(u32 flagIndex);
extern u8 fldDoor_ChkWarp(void);

s32 fldCommand_FLD_MODEL_ON(void) {
    s32 world;
    s32 stage;
    s32 mode;
    const char *name;
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
                node = dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
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

s32 fldCmdApplyRoomModeGroupOne(void) {
    s32 world;
    s32 stage;
    s32 mode;
    const char *name;
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
    if (strcmp(name, D_003BB000) == 0) {
        name = D_003BAE44;
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
                node = dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
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

extern void fldSetRoomObjectModeFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);


s32 fldCommand_FLD_ANIME_ON(void) {
    s32 world;
    s32 stage;
    const char *name;
    void *object;
    FldSceneObjectKeyResult id;
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
            fldSetRoomObjectModeFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        id = fldCommand_SEARCH_ID(name);
        if (id != FLD_SCENE_OBJECT_KEY_NOT_FOUND) {
            object = dds3SearchSceneBasic(dds3GetControlScene(), id, 6);
            switch (scrGetIntPara(3)) {
            case 0:
                dds3ObjectBaseMatterAnimeSpeed(object, 0);
                break;
            case 1:
                dds3ObjectBaseMatterAnimeSpeed(object, 1);
                break;
            case 2:
                dds3ObjectBaseMatterAnimeSpeed(object, 2);
                break;
            case 3:
                dds3ObjectBaseMatterAnimeSpeed(object, 6);
                break;
            }
        }
    }
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF(void) {
    s32 world;
    s32 stage;
    const char *name;
    void *object;
    FldSceneObjectKeyResult id;
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
            fldSetRoomObjectModeFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        id = fldCommand_SEARCH_ID(name);
        if (id != FLD_SCENE_OBJECT_KEY_NOT_FOUND) {
            object = dds3SearchSceneBasic(dds3GetControlScene(), id, 6);
            switch (scrGetIntPara(3)) {
            case 0:
                dds3ObjectBaseMatterAnimeSpeed(object, 3);
                break;
            case 1:
                dds3ObjectBaseMatterAnimeSpeed(object, 4);
                break;
            case 2:
                dds3ObjectBaseMatterAnimeSpeed(object, 5);
                break;
            }
        }
    }
    return 1;
}

extern s32 D_0032E3C4[];

extern void fldSetRoomSceneFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

s32 fldCommand_FLD_ATARI_ON(void) {
    s32 world;
    s32 stage;
    const char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
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

extern char *D_003BAE48;

s32 fldCmdSetSceneBitsBarrierRoom(void) {
    s32 world;
    s32 stage;
    const char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    name = scrGetStrPara(2);
    if (strcmp(name, D_003BB000) == 0) {
        name = D_003BAE48;
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

extern s32 D_0032E3C4[];

extern void fldSetMapTargetFlag(s32 area, u32 slotIndex, s32 bit, s32 enabled);

extern void fldSetAlternateMapTargetFlag(s32 area, u32 slotIndex, s32 bit, s32 enabled);

s32 fldCommand_FLD_MODEL_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_MODEL_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 1);
    return 1;
}

s32 fldCommand_FLD_ANIME_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetAlternateMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
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
        world = D_0032E3C0[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    slot = scrGetIntPara(2);
    if (slot == 0) {
        return 1;
    }
    fldSetMapSlotByte(world, stage, slot, scrGetIntPara(3));
    return 1;
}

extern void fldSetFlagBit(s32 area, s32 floor, s32 flagNumber);

s32 fldCommand_FLD_MAP_ON(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldSetFlagBit(area, floor, flagNumber);
    return 1;
}

extern void fldClearFloorFlag(s32 area, s32 floor, s32 flagNumber);

s32 fldCommand_FLD_MAP_OFF(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldClearFloorFlag(area, floor, flagNumber);
    return 1;
}

extern void fldSetMapSlotAuxiliaryFlag(s32 area, u32 slotIndex, s32 bit, s32 enabled);

s32 fldOpClearRoomStateFlags(void) {
    s32 world;
    s32 stage;
    const char *name;
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
    const char *name;
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

extern void fldSetMapSlotValueFlag(s32 area, u32 slotIndex, s32 bit, s32 enabled);

s32 fldCmdSetSceneBitsValue(void) {
    s32 world;
    s32 stage;
    s32 room;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
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
    if (fldGetRestoreMode() == FLD_SCENE_STATUS_BATTLE) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_ANA(void) {
    if (fldGetRestoreMode() == FLD_SCENE_STATUS_ANA) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_SISETU(void) {
    if (fldGetRestoreMode() == FLD_SCENE_STATUS_SISETU) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_EVE(void) {
    if (fldGetRestoreMode() == FLD_SCENE_STATUS_EVENT) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

extern FldActivationEntry D_0034C8F0[];

s32 fldCmdPushSceneParam(void) {
    s32 room = fldFindRoomIdByTask(scrGetScriptHandle()->task);

    switch (scrGetIntPara(0)) {
    case 0:
        scrSetIntReturnValue(D_0034C8F0[room].kind);
        break;
    case 1:
        scrSetIntReturnValue(D_0034C8F0[room].parameter);
        break;
    case 2:
        scrSetIntReturnValue(D_0034C8F0[room].selectedValue2);
        break;
    case 3:
        scrSetIntReturnValue(D_0034C8F0[room].selectedValue3);
        break;
    case 4:
        scrSetIntReturnValue(D_0034C8F0[room].selectedValue4);
        break;
    }
    return 1;
}


extern void fldActivateFlaggedObject(u32 flagIndex);

u32 fldCommand_FLD_SET_TAKARA_OPEN(void) {
    scrProcessWork_s *script;
    s32 room;

    script = scrGetScriptHandle();
    room = fldFindRoomIdByTask(script->task);
    fldActivateFlaggedObject(room);
    return 1;
}

u32 fldCommand_FLD_CHK_TAKARA_OPEN(void) {
    if (fldTestObjectActivationFlag(fldFindRoomIdByTask(scrGetScriptHandle()->task)) != 0) {
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
    scene = fldEveHitResName_Find(scrGetScriptHandle()->task);
    if (scene) {
        fldApplyCurrentTaskScene(scene);
    }
    return 1;
}

u32 fldCmdDispatchSceneAction(void) {
    s32 sceneAction;

    sceneAction = scrGetIntPara(0);
    fldDispatchSceneAction(sceneAction);
    return 1;
}

u32 func_0014EF20(void) {
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
    actorStat = fldGetActorStat1(actorStat);
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
    s32 sceneRoomSelection;

    sceneRoomSelection = scrGetIntPara(0);
    fldDispatchSceneRoomSelection(sceneRoomSelection);
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

u32 fldCommand_FLD_ELV_ENDCHK(void) {
    FldCameraTrackingResult result;

    result = fldEvaluateCameraMoveTracking();
    scrSetIntReturnValue(result);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_0014D110", fldRoomNameSentinel);

INCLUDE_SDATA(const s32, "game/code_0014D110", D_003BB000);
