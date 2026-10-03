#include "common.h"
#include "pcp_vu0.h"
#include "fpu.h"

extern s32 fldGetSceneStatusCode(void);

extern void func_001442A0(s32);

extern void func_00144270(s32);

/* Field work area (fldAreaState) fields reached through a pointer. */
typedef struct FldWorkView {
    u8 unk00[0x14];
    s32 stage;            /* 0x14 */
    u8 unk18[0x18];
    f32 focusPos[3];      /* 0x30 */
    u8 unk3C[0x14];
    s32 focusActive;      /* 0x50 */
    u8 unk54[0x10];
    f32 negatedAngle;     /* 0x64 */
    u8 unk68[8];
    s32 unk70;            /* 0x70 */
    u8 unk74[0x4C];
    s32 unkC0;            /* 0xC0 */
    u8 unkC4[0x40];
    s16 eventActive;      /* 0x104 */
    u8 unk106[0x24];
    s16 unk12A;           /* 0x12A */
    u8 unk12C[0x20];
    f32 x;                /* 0x14C */
    f32 y;
    f32 z;
    u8 unk158[0x18];
    f32 angle;            /* 0x170 */
    u8 unk174[0x4C];
    struct {
        s32 value;
        s32 block;
    } fldmix[5];          /* 0x1C0 */
} FldWorkView;

extern void fldSetRoomObjectModeFlag(s32, s32, s32, s32);

extern void dds3SetObjectModeAndDefaultWeight(void *, s32);

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

extern u32 fldPlayerObject;

extern void evtSetSolarOverlayFullyVisible(void);

extern s32 fldRoomEffectEntryCount;

extern u32 D_00436330[];

extern s32 fldObjectSlotCount;

extern s32 fldSceneSoundBase;

extern f32 fldLookAtFarPoint[];

extern s32 D_00436228;

extern u32 D_004363C8;

extern u32 D_004363CC;

extern u32 D_004363D0;

extern u32 D_004363D4;

extern u32 D_00436380;

extern u32 D_00436384;

extern u32 fldDamEffectNode;

extern u32 fldDamEffectPositioned;

extern u32 D_00436390;

extern u32 D_00436394;

extern u32 fldYukEffectNode;

extern u32 fldYukEffectPositioned;

extern s32 D_004363AC;

extern s32 D_004363B0;

extern s32 D_004363B4;

extern u32 D_00436370;

extern s32 D_00436374;

extern s32 D_00436368;

extern s32 D_0043636C;

extern u32 fldIndexedResourceHandle;

extern u32 fldIndexedResourceData;

extern s32 fldIndexedResourceEffect;

extern u32 D_0043623C;

extern u32 fldCurrentBgmHandle;

extern u32 fldArchiveLoadPending;

extern u32 fldFixedArchiveLoadPhase;

extern u32 fldSceneRecords;

extern s32 fldSceneRecordResource;

extern u32 fldSceneReady;

extern u32 D_00436364;

extern u32 D_00436354;

extern u32 D_0043635C;

extern s32 D_00436350;

extern s32 D_00436358;

extern s32 D_00436360;

extern s32 D_0043637C;

extern s32 fldFindRoomByTask(u32);

extern s32 fldEvaluateCameraMoveTracking(void);

extern void scrSetIntegerReturnValue(s32 value);

extern s32 fldQuerySelectedActorMotionState(s32 value);

extern s32 func_0013FA98(s32 param0, s32 param1);

extern s32 fldGetActorSlotAttribute(s32 param0, s32 param1);

extern void func_00140238(void);

extern void fldStopCurrentBgm(void);

extern void fldPlayCurrentBgmSound(void);

extern void fldReleaseCurrentBgm(void);

extern void fldPlayMenuSound(s32 param);

extern void fldPlayFieldSe(s32 param);

extern s32 fldPollArchiveLoad(s32 param);

extern void fldSetArchiveSoundVolumePan(s32 param0, s32 param1);

extern void fldPlayArchiveSound(s32 param0, s32 param1);

extern void fldStartTitle(s32 param0, s32 param1, s32 param2);

extern s32 fldGetTaskRecordValue(u32 key);

extern void func_00140A58(void *entry);

/* Work object queried by fldReleaseWeatherEffects; +0xE4 holds the key for func_0013BEE8. */
typedef struct {
    u8 unk00[0xE4]; /* 0x00 */
    u32 key;        /* 0xE4 */
} EffCmdWork;

typedef struct FldSceneParamRow {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} FldSceneParamRow; /* 0x10 bytes */

extern FldSceneParamRow D_003A8EB0[];

extern void func_0014BF98(s32 handle);

extern s32 evtGetWorldUnitNestedValue(s32 param);

extern char *scrReadStringParameter(s32 idx);

extern s32 fldFindEffectByName(char *str);

extern s32 fldGetCurrentSceneSelectionResource(void);

extern s32 sdfSoundIsCommandBusy(void);

extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern u8 D_00436208[];

extern s32 fldPendingSoundCount;

extern s32 fldAreaState[];

extern s32 fldCurrentBgmId[];

extern s32 D_00389780[];

extern s32 fldAreaFlagIndex;

extern s32 D_00436248;

extern s32 fldPrimaryEffectPositionPending;

extern s32 fldSecondaryEffectPositionPending;

extern void fldResetPlayerSceneObjectState(void);

extern s32 D_003897C0[];

extern u64 dds3GetWorldSecondaryObject(void);

extern u32 *dds3FindObjectChainNodeByName(u64 world, const char *name);

extern void func_0035B6E0(const char *fmt, ...);

extern s32 D_003897C8[];

extern s32 D_00389988[];

extern s32 scrGetCurrentContext(void);

extern s32 fldGetActorStat0(s32);

extern s32 fldGetMappedActorStateAttribute(s32);

extern s32 fldGetActorMotionEntry(s32);

extern s32 fldGetRowValue(s32);

extern u32 fldGetSelectedActorMotionId(void);

extern void fldPreparePlayerSceneCameraTarget(void);

extern s32 D_0043633C;

extern s32 D_00389784[];

extern s32 fldSetMapTargetFlag(s32, s32, s32, s32);

extern s32 fldSetAlternateMapTargetFlag(s32, s32, s32, s32);

extern void fldClearFloorFlag(s32, s32, s32);

extern void fldSetFloorFlag(s32, s32, s32);

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 scrReadIntParameter(s32);

extern s32 D_0038984C[];

extern s32 fldIsSceneStateEight(void);

extern void func_00123DE8(s32, s32, s32, s32, s32, s32);

extern s32 effMiscRandMod(s32, s32);

extern s32 evtGetMirroredSolarPhase(void);

extern s32 fldMirroredSolarThresholds[];

extern char fldRoomNameSentinel[];

extern s32 func_0035D600(char *, const char *);

extern void fldSetFadeTarget(s32, s32, s32);

extern void fldSetSwayMode(s32);

extern s32 fldSceneBgmArchiveTrack;

extern s32 fldSceneBgmArchivePhase;

extern s32 fldSceneRecordCount;

extern s32 D_00436300;

extern s32 D_0043630C;

extern s32 dds3GetWorldObject(void);

extern void *dds3FindWorldObjectNodeByKey(u64, s32, s32);

extern s32 fldGetPlayerSceneState(void);

extern s32 dds3SetWorldCameraObject(s32, s32);

extern void dds3TransformCameraVectorsByInnerRotation(s32, f32 *, f32 *);

extern void fldUpdateCameraProjectionEndpoints(void);

extern f32 D_0038BAD0[];

extern f32 D_0038BAE0[];

extern s32 D_004360B4;

extern s32 D_004360B8;

extern void fldSetRoomSceneFlag(s32, s32, s32, s32);

extern int strcmp(const char *, const char *);

extern char D_004363F0[]; /* "BARIA" */

extern char *D_004361D8;

extern void fldSetMapSlotValueFlag(s32, s32, s32, s32);

extern s32 func_0013F790(s32 index);

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

extern s32 strcmp(const char *, const char *);

extern s32 D_00436378;

extern s16 D_004363BC;

extern s16 D_004363BE;

extern s16 D_004363C0;

extern s16 D_004363C2;

extern s32 D_004363DC;

extern f32 D_004363E0;

extern f32 D_004363E4;

void fldFireRoomEffects(void);

void fldResetAfterEvent(void);

void fldSetTargetGuideEnabled(s32 active);

void fldStartSceneBgm(void);

void fldStartSceneBgmAlternate(void);

extern s32 func_0013D308(s32, void *);

extern s32 func_0013D598(s32, void *);

s32 fldCmdQueryActorEntrySceneStatus(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = fldPlayerObject;
    void *entry;
    s32 result;

    if (unit == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    entry = dds3FindWorldObjectNodeByKey(world, fldFindTaskRecordId(((EffCmdWork *)scrGetCurrentContext())->key), 0x11);
    if (entry == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    if (func_0013D308(unit, entry) == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    func_001442A0(scrReadIntParameter(0));
    func_00144270(1);
    result = func_001442D0();
    switch (result) {
    case 1:
        scrSetIntegerReturnValue(1);
        return 1;
    case -1:
        scrSetIntegerReturnValue(-1);
        return 1;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

s32 fldCmdQueryAlternateActorEntrySceneStatus(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = fldPlayerObject;
    void *entry;
    s32 result;

    if (unit == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    entry = dds3FindWorldObjectNodeByKey(world, fldFindTaskRecordId(((EffCmdWork *)scrGetCurrentContext())->key), 0x11);
    if (entry == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    if (func_0013D598(unit, entry) == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    func_001442A0(scrReadIntParameter(0));
    func_00144270(1);
    result = func_001442D0();
    switch (result) {
    case 1:
        scrSetIntegerReturnValue(1);
        return 1;
    case -1:
        scrSetIntegerReturnValue(-1);
        return 1;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

extern void func_001442A0(s32);

extern void func_00144270(s32);

s32 fldCmdReadSceneStatus(void) {
    s32 result;

    func_001442A0(scrReadIntParameter(0));
    func_00144270(1);
    result = func_001442D0();
    if (result == -1) {
        scrSetIntegerReturnValue(-1);
        return 1;
    }
    if (result == 1) {
        scrSetIntegerReturnValue(1);
        return 1;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

s32 fldCmdTestActorEntryCondition(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = fldPlayerObject;
    void *entry;

    if (unit == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    entry = dds3FindWorldObjectNodeByKey(world, fldFindTaskRecordId(((EffCmdWork *)scrGetCurrentContext())->key), 0x11);
    if (entry == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    if (func_0013D308(unit, entry) == 0) {
        scrSetIntegerReturnValue(0);
    } else {
        scrSetIntegerReturnValue(1);
    }
    return 1;
}

extern void fldSetCameraNodeModeWithTen(void);

s32 fldCmdRestoreCameraNodeMode(void) {
    fldResetPlayerSceneObjectState();
    if (fldPlayerObject == 0) {
        return 1;
    }
    fldSetCameraNodeModeWithTen();
    return 1;
}

extern s32 D_003898B0[];

extern void dds3InvokeSlot1Handler(s32, s32);

s32 fldCmdReleaseCurrentObject(void) {
    if (fldPlayerObject == 0) {
        return 1;
    }
    dds3InvokeSlot1Handler(fldPlayerObject, 0);
    fldSetCameraNodeModeWithTen();
    fldPreparePlayerSceneCameraTarget();
    D_003898B0[0] = 0;
    return 1;
}

s32 func_00154870(void) {
    D_003898B0[0] = 1;
    return 1;
}

u32 fldCmdSetSceneControlFlag(void) {
    fldSetSceneControlFlags(0x10);
    return 1;
}

u32 fldCmdClearSceneControlFlag(void) {
    fldClearSceneControlFlags(0x10);
    return 1;
}

typedef struct FldObjModel {
    u8 unk00[0x40];
    f32 pos[3]; /* 0x40 */
} FldObjModel;

typedef struct FldObj {
    u8 unk00[0x1C];
    FldObjModel *model; /* 0x1C */
} FldObj;

extern void func_001300A0(void);

s32 fldCmdFocusCameraOnObject(void) {
    FldWorkView *work;
    FldObj *obj;
    u64 world = dds3GetWorldSecondaryObject();

    obj = dds3FindWorldObjectNodeByKey(world, scrReadIntParameter(0), 4);
    if (obj == NULL) {
        return 1;
    }
    work = (FldWorkView *)fldAreaState;
    work->focusActive = 1;
    work->focusPos[0] = obj->model->pos[0];
    work->focusPos[1] = obj->model->pos[1];
    work->focusPos[2] = obj->model->pos[2];
    func_001300A0();
    return 1;
}

typedef struct FldVec3 {
    f32 x;
    f32 y;
    f32 z;
} FldVec3;

extern f32 fldLookAtNearPoint[];

extern f32 sdfSinPoly(f32);

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern s32 func_00133B10();

extern void fldClearCameraObjectHighlightFlag();

extern void func_0012F078();

s32 fldUpdateLookAtSegment(void) {
    FldWorkView *cam = (FldWorkView *)fldAreaState;
    FldVec3 near;
    FldVec3 far;

    cam->focusActive = 0;
    cam->negatedAngle = -cam->angle;
    near.x = cam->x - sdfSinPoly((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + sdfEvaluateCosineViaSinePhaseShift((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + sdfEvaluateCosineViaSinePhaseShift(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    fldLookAtNearPoint[0] = near.x;
    fldLookAtNearPoint[1] = near.y;
    fldLookAtNearPoint[2] = near.z;
    fldLookAtFarPoint[0] = far.x;
    fldLookAtFarPoint[1] = far.y;
    fldLookAtFarPoint[2] = far.z;
    func_00133B10();
    fldClearCameraObjectHighlightFlag();
    func_0012F078();
    return 1;
}

s32 fldCmdSetScenePhaseThree(void) {
    D_003897C0[0] = 3;
    return 1;
}

s32 fldCmdSetLookAtHeading(void) {
    FldWorkView *cam;
    FldVec3 near;
    FldVec3 far;

    if (scrReadIntParameter(0) < 0) {
        ((FldWorkView *)fldAreaState)->negatedAngle = -((FldWorkView *)fldAreaState)->angle;
    } else {
        ((FldWorkView *)fldAreaState)->negatedAngle = -45 * scrReadIntParameter(0);
    }
    cam = (FldWorkView *)fldAreaState;
    near.x = cam->x - sdfSinPoly((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + sdfEvaluateCosineViaSinePhaseShift((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + sdfEvaluateCosineViaSinePhaseShift(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    fldLookAtNearPoint[0] = near.x;
    fldLookAtNearPoint[1] = near.y;
    fldLookAtNearPoint[2] = near.z;
    fldLookAtFarPoint[0] = far.x;
    fldLookAtFarPoint[1] = far.y;
    fldLookAtFarPoint[2] = far.z;
    func_0012F078();
    return 1;
}

s32 fldCmdCaptureObjectPose(void) {
    f32 pos[4];
    f32 rot[4];
    s32 handle;
    s32 object;
    s32 world;

    if (scrReadIntParameter(0) == -1) {
        handle = fldGetPlayerSceneState();
        if (handle == 0) {
            return 1;
        }
        object = dds3SetWorldCameraObject(dds3GetWorldObject(), handle);
        if (object == 0) {
            return 1;
        }
        dds3TransformCameraVectorsByInnerRotation(object, pos, rot);
        fldUpdateCameraProjectionEndpoints();
        D_0038BAD0[0] = pos[0];
        D_0038BAD0[1] = pos[1];
        D_0038BAD0[2] = pos[2];
        D_0038BAE0[0] = rot[0];
        D_0038BAE0[1] = rot[1];
        D_0038BAE0[2] = rot[2];
        D_004360B4 = 0;
        D_004360B8 = scrReadIntParameter(1);
        D_003897C0[0] = 4;
    } else {
        world = dds3GetWorldObject();
        handle = (s32)dds3FindWorldObjectNodeByKey(world, scrReadIntParameter(0), 4);
        if (handle == 0) {
            return 1;
        }
        dds3SetWorldCameraObject(dds3GetWorldObject(), handle);
    }
    return 1;
}

s32 fldCmdSetRequestedSceneName(void) {
    char *name;

    name = scrReadStringParameter(0);
    fldAreaState[0x14] = 5;
    strcpy((char *)fldAreaState + 0x40, name);
    return 1;
}

extern void evtSetSolarOverlayFullyTransparent(void);

extern void evtDisableSolarOverlayAlpha(void);

extern void evtEnableSolarOverlayAlpha(void);

s32 fldCmdSetSolarOverlayMode(void) {
    s32 mode = scrReadIntParameter(0);

    fldAreaState[0xCC / 4] = mode;
    fldAreaState[0xD0 / 4] = 0;
    if (mode < 4) {
        fldAreaState[0xD4 / 4] = mode;
    }
    switch (fldAreaState[0xCC / 4]) {
    case 0:
        evtSetSolarOverlayFullyTransparent();
        break;
    case 1:
        evtSetSolarOverlayFullyVisible();
        fldAreaState[0x138 / 4] = 1;
        break;
    case 2:
        evtDisableSolarOverlayAlpha();
        fldAreaState[0xD0 / 4] = 15;
        break;
    case 3:
        evtEnableSolarOverlayAlpha();
        fldAreaState[0xD0 / 4] = 0;
        fldAreaState[0x138 / 4] = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    return 1;
}

void func_00154F18(void) {
}

u32 func_00154F20(void) {
    return 1;
}

u32 fldCmdFreeDisplayObjects(void) {
    fldFreeDisplayObjects();
    return 1;
}

s32 fldFindSearchId(const char *name) {
    u32 *entry = dds3FindObjectChainNodeByName(dds3GetWorldSecondaryObject(), name);
    if (entry != 0) {
        return entry[1];
    }
    func_0035B6E0("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

s32 fldParseRoomNumberFromName(char *name) {
    s32 index;
    if (func_0035D600(name, fldRoomNameSentinel) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (name[index] == '\0') {
            if (index < 4) return 0;
            return (name[index - 2] - '0') * 10 + (name[index - 1] - '0');
        }
    }
    return 0;
}

s32 fldCmdSetSceneControlValue(void) {
    s32 value;

    value = scrReadIntParameter(0);
    D_003897C8[0] = value;
    return 1;
}

s32 fldCmdSetFieldParameterPair(void) {
    s32 value;

    D_00389988[11] = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    func_00135A68(D_00389988[11], value);
    return 1;
}

s32 fldCmdSetFadeAndSway(void) {
    s32 second;
    s32 third;
    D_00389988[13] = scrReadIntParameter(0) & 0xFF;
    second = scrReadIntParameter(1);
    third = scrReadIntParameter(2);
    fldSetFadeTarget(D_00389988[13], second, third);
    fldSetSwayMode(scrReadIntParameter(3));
    return 1;
}

extern void fldSetRoomModeFlag(s32, s32, s32, s32);

extern s64 dds3GetWorldValueCount(u64);

extern s64 dds3AdvanceObjectValueCursor(u64);

extern u64 dds3CopyWorldListToValueChain(u64, u64);

extern void dds3DestroyWorldIndexNode(u64);

extern s32 dds3ResetObjectValueCursor(u64);

typedef struct FldWorldItem {
    u8 pad0[0x18];
    s32 *data;
} FldWorldItem;

extern FldWorldItem *dds3ReadIndexedWorldObjectWord(u64);

extern void evtSetObjectTransitionWork(FldWorldItem *, s32);

s32 fldCmdApplyRoomModeGroupZero(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    u64 list;
    FldWorldItem *item;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = fldAreaState[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    mode = scrReadIntParameter(3);
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetRoomModeFlag(world, stage, room, 0);
        }
    } else {
        room = fldParseRoomNumberFromName(name);
        fldSetRoomModeFlag(world, stage, room, 0);
    }
    if (world == fldAreaState[4] && stage == fldAreaState[5] + 1) {
        list = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 6);
        if (dds3GetWorldValueCount(list) != 0) {
            dds3ResetObjectValueCursor(list);
            do {
                item = dds3ReadIndexedWorldObjectWord(list);
                if (item->data[1] == room) {
                    switch (mode) {
                    case 0:
                        evtSetObjectTransitionWork(item, 1);
                        break;
                    case 1:
                        evtSetObjectTransitionWork(item, 5);
                        break;
                    case 2:
                        evtSetObjectTransitionWork(item, 7);
                        break;
                    }
                }
            } while (dds3AdvanceObjectValueCursor(list) != 0);
            dds3DestroyWorldIndexNode(list);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00154558", func_001552E0);

extern char *D_004361D4;

s32 fldCmdApplyRoomModeGroupOne(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    u64 list;
    FldWorldItem *item;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = fldAreaState[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    mode = scrReadIntParameter(3);
    name = scrReadStringParameter(2);
    if (strcmp(name, D_004363F0) == 0) {
        name = D_004361D4;
    }
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetRoomModeFlag(world, stage, room, 1);
        }
    } else {
        room = fldParseRoomNumberFromName(name);
        fldSetRoomModeFlag(world, stage, room, 1);
    }
    if (world == fldAreaState[4] && stage == fldAreaState[5] + 1) {
        list = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 6);
        if (dds3GetWorldValueCount(list) != 0) {
            dds3ResetObjectValueCursor(list);
            do {
                item = dds3ReadIndexedWorldObjectWord(list);
                if (item->data[1] == room) {
                    switch (mode) {
                    case 0:
                        evtSetObjectTransitionWork(item, 2);
                        break;
                    case 1:
                        evtSetObjectTransitionWork(item, 6);
                        break;
                    case 2:
                        evtSetObjectTransitionWork(item, 8);
                        break;
                    }
                }
            } while (dds3AdvanceObjectValueCursor(list) != 0);
            dds3DestroyWorldIndexNode(list);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00154558", func_00155690);

s32 fldCmdApplyRoomObjectModeZero(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;
    s32 searchId;
    void *obj;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = fldAreaState[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    if (world == fldAreaState[4]) {
        if (stage == fldAreaState[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (scrReadIntParameter(3)) {
                case 0:
                    dds3SetObjectModeAndDefaultWeight(obj, 0);
                    break;
                case 1:
                    dds3SetObjectModeAndDefaultWeight(obj, 1);
                    break;
                case 2:
                    dds3SetObjectModeAndDefaultWeight(obj, 2);
                    break;
                case 3:
                    dds3SetObjectModeAndDefaultWeight(obj, 6);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldApplyRoomObjectModeZero(s32 world, s32 stage, char *name, s32 mode) {
    s32 i;
    s32 searchId;
    void *obj;

    if (world == 0) {
        world = fldAreaState[4];
    }
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    if (world == fldAreaState[4]) {
        if (stage == fldAreaState[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (mode) {
                case 0:
                    dds3SetObjectModeAndDefaultWeight(obj, 0);
                    break;
                case 1:
                    dds3SetObjectModeAndDefaultWeight(obj, 1);
                    break;
                case 2:
                    dds3SetObjectModeAndDefaultWeight(obj, 2);
                    break;
                case 3:
                    dds3SetObjectModeAndDefaultWeight(obj, 6);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldCmdApplyRoomObjectModeOne(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;
    s32 searchId;
    void *obj;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = fldAreaState[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    if (world == fldAreaState[4]) {
        if (stage == fldAreaState[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (scrReadIntParameter(3)) {
                case 0:
                    dds3SetObjectModeAndDefaultWeight(obj, 3);
                    break;
                case 1:
                    dds3SetObjectModeAndDefaultWeight(obj, 4);
                    break;
                case 2:
                    dds3SetObjectModeAndDefaultWeight(obj, 5);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldApplyRoomObjectModeOne(s32 world, s32 stage, char *name, s32 mode) {
    s32 i;
    s32 searchId;
    void *obj;

    if (world == 0) {
        world = fldAreaState[4];
    }
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomObjectModeFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    if (world == fldAreaState[4]) {
        if (stage == fldAreaState[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (mode) {
                case 0:
                    dds3SetObjectModeAndDefaultWeight(obj, 3);
                    break;
                case 1:
                    dds3SetObjectModeAndDefaultWeight(obj, 4);
                    break;
                case 2:
                    dds3SetObjectModeAndDefaultWeight(obj, 5);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldCmdSetSceneBits(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomSceneFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomSceneFlag(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    return 1;
}

s32 fldCmdSetSceneBitsBarrierRoom(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = scrReadStringParameter(2);
    if (strcmp(name, D_004363F0) == 0) {
        name = D_004361D8;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetRoomSceneFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomSceneFlag(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    return 1;
}

s32 fldOpClearMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldOpSetMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 1);
    return 1;
}

s32 fldOpClearAlternateMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetAlternateMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldOpSetAlternateMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetAlternateMapTargetFlag(area, floor, target, 1);
    return 1;
}

extern void fldSetMapSlotByte(s32, s32, s32, s32);

s32 fldCmdSetMapSlotByte(void) {
    s32 world;
    s32 stage;
    s32 slot;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    slot = scrReadIntParameter(2);
    if (slot == 0) {
        return 1;
    }
    fldSetMapSlotByte(world, stage, slot, scrReadIntParameter(3));
    return 1;
}

s32 fldCmdClearFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetFloorFlag(area, floor, target);
    return 1;
}

s32 fldCmdSetFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldClearFloorFlag(area, floor, target);
    return 1;
}

extern void fldSetMapSlotAuxiliaryFlag(s32, s32, s32, s32);

s32 fldOpClearRoomStateFlags(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = fldAreaState[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetMapSlotAuxiliaryFlag(world, stage, i, 0);
        }
    } else {
        fldSetMapSlotAuxiliaryFlag(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    if (world == fldAreaState[4]) {
        if (stage == fldAreaState[5] + 1) {
            fldFireRoomEffects();
        }
    }
    return 1;
}

s32 fldOpSetRoomStateFlags(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = fldAreaState[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = fldAreaState[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetMapSlotAuxiliaryFlag(world, stage, i, 1);
        }
    } else {
        fldSetMapSlotAuxiliaryFlag(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    if (world == fldAreaState[4]) {
        if (stage == fldAreaState[5] + 1) {
            fldFireRoomEffects();
        }
    }
    return 1;
}

s32 fldCmdSetSceneBitsValue(void) {
    s32 world;
    s32 stage;
    s32 room;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    room = scrReadIntParameter(2);
    if (room == 0) {
        for (; room < 16; room++) {
            fldSetMapSlotValueFlag(world, stage, room, scrReadIntParameter(3));
        }
    } else {
        fldSetMapSlotValueFlag(world, stage, room, scrReadIntParameter(3));
    }
    return 1;
}

s32 fldSetFlagFromWorld1(void) {
    if (fldGetSceneStatusCode() == 1) {
        scrSetIntegerReturnValue(1);
        return 1;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

s32 fldSetFlagFromWorld3(void) {
    if (fldGetSceneStatusCode() == 3) {
        scrSetIntegerReturnValue(1);
        return 1;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

s32 fldSetFlagFromWorld4(void) {
    if (fldGetSceneStatusCode() == 4) {
        scrSetIntegerReturnValue(1);
        return 1;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

s32 fldSetFlagFromWorld2(void) {
    if (fldGetSceneStatusCode() == 2) {
        scrSetIntegerReturnValue(1);
        return 1;
    }
    scrSetIntegerReturnValue(0);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00154558", D_00414090);

s32 fldCmdPushSceneParam(void) {
    s32 room;

    room = fldFindRoomByTask(((EffCmdWork *)scrGetCurrentContext())->key);
    switch (scrReadIntParameter(0)) {
    case 0:
        scrSetIntegerReturnValue(D_003A8EB0[room].unk0);
        break;
    case 1:
        scrSetIntegerReturnValue(D_003A8EB0[room].unk4);
        break;
    case 2:
        scrSetIntegerReturnValue(D_003A8EB0[room].unk6);
        break;
    case 3:
        scrSetIntegerReturnValue(D_003A8EB0[room].unk8);
        break;
    case 4:
        scrSetIntegerReturnValue(D_003A8EB0[room].unkC);
        break;
    }
    return 1;
}

u32 fldCmdActivateTaskRoomObject(void) {
    s32 task;
    u64 room;

    task = scrGetCurrentContext();
    room = fldFindRoomByTask(((EffCmdWork *)task)->key);
    fldActivateFlaggedObject(room);
    return 1;
}

u32 fldCmdTestTaskRoomObjectActive(void) {
    if (fldTestObjectActivationFlag(fldFindRoomByTask(((EffCmdWork *)scrGetCurrentContext())->key)) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 fldCmdSetCurrentTaskScene(void) {
    s32 scene;

    if (fldIsSceneStateEight()) {
        func_00140BC8(0);
        return 1;
    }
    scene = fldGetTaskRecordValue(((EffCmdWork *)scrGetCurrentContext())->key);
    if (scene) {
        func_00140BC8(scene);
    }
    return 1;
}

u32 func_001569B8(void) {
    u64 value;

    value = scrReadIntParameter(0);
    func_001411F8(value);
    return 1;
}

u32 func_001569E0(void) {
    return 1;
}

u32 fldCmdGetActorStat0(void) {
    s32 actorStat;

    actorStat = scrReadIntParameter(0);
    actorStat = fldGetActorStat0(actorStat);
    scrSetIntegerReturnValue(actorStat);
    return 1;
}

u32 fldCmdGetActorStat1(void) {
    s32 actorStat;

    actorStat = scrReadIntParameter(0);
    actorStat = fldGetMappedActorStateAttribute(actorStat);
    scrSetIntegerReturnValue(actorStat);
    return 1;
}

u32 fldCmdGetActorMotionEntry(void) {
    s32 motionEntry;

    motionEntry = scrReadIntParameter(0);
    motionEntry = fldGetActorMotionEntry(motionEntry);
    scrSetIntegerReturnValue(motionEntry);
    return 1;
}

u32 fldCmdGetRowValue(void) {
    s32 rowValue;

    rowValue = scrReadIntParameter(0);
    rowValue = fldGetRowValue(rowValue);
    scrSetIntegerReturnValue(rowValue);
    return 1;
}

u32 func_00156AA8(void) {
    u64 value;

    value = scrReadIntParameter(0);
    func_00142670(value);
    return 1;
}

u32 func_00156AD0(void) {
    u64 first;
    u64 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_001206D0(first, second);
    return 1;
}

u32 fldCmdResetCameraMoveTracking(void) {
    fldResetCameraMoveTracking();
    return 1;
}

s32 fldCmdQueryCameraMoveTracking(void) {
    scrSetIntegerReturnValue(fldEvaluateCameraMoveTracking());
    return 1;
}

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
s32 fldCmdQuerySceneValue(void) {
    scrSetIntegerReturnValue(fldQuerySelectedActorMotionState(scrReadIntParameter(0)));
    return 1;
}

u32 fldCmdUpdateTaskRecordScene(void) {
    s32 scene;
    if (fldIsSceneStateEight()) {
        scene = 0;
    } else {
        scene = fldFindTaskRecordId(((EffCmdWork *)scrGetCurrentContext())->key);
    }
    fldApplyActorEntryTrigger(scene);
    return 1;
}

void fldCmdResetAfterEvent(void) {
    fldResetAfterEvent();
}

s32 func_00156BE8(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    scrSetIntegerReturnValue(func_0013FA98(param0, param1));
    return 1;
}

s32 fldCmdGetActorSlotAttribute(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    scrSetIntegerReturnValue(fldGetActorSlotAttribute(param0, param1));
    return 1;
}

void func_00156C78(void) {
    scrSetIntegerReturnValue(func_0013F790(1));
}

s32 func_00156C98(void) {
    func_00140238();
    return 1;
}

s32 fldCmdStartSceneBgm(void) {
    fldCurrentBgmId[0] = scrReadIntParameter(0);
    fldStartSceneBgm();
    return 1;
}

s32 fldCmdStartSceneBgmAlternate(void) {
    fldCurrentBgmId[0] = scrReadIntParameter(0);
    fldStartSceneBgmAlternate();
    return 1;
}

s32 fldCmdStopCurrentBgm(void) {
    fldStopCurrentBgm();
    return 1;
}

s32 fldCmdPlayCurrentBgmSound(void) {
    fldPlayCurrentBgmSound();
    return 1;
}

s32 fldCmdReleaseCurrentBgm(void) {
    fldReleaseCurrentBgm();
    return 1;
}

s32 fldCommandPlaySeVolumePan(void) {
    fldPlayMenuSound(scrReadIntParameter(0));
    return 1;
}

s32 fldCommandPlaySe(void) {
    fldPlayFieldSe(scrReadIntParameter(0));
    return 1;
}

u8 fldCommandLoadArchive(void) {
    return fldPollArchiveLoad(scrReadIntParameter(0)) != 0;
}

s32 fldCmdSetArchiveSoundVolumePan(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    fldSetArchiveSoundVolumePan(param0, param1);
    return 1;
}

s32 fldCmdPlayArchiveSound(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    fldPlayArchiveSound(param0, param1);
    return 1;
}

s32 fldCommandStartTitle(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    fldStartTitle(param0, param1, 0x3c);
    return 1;
}

s32 fldCmdSetDefaultEncounterId(void) {
    s32 value;

    value = scrReadIntParameter(0);
    D_0038984C[0] = value;
    return 1;
}

s32 fldCmdApplyTaskRecordEntry(void) {
    EffCmdWork *work = scrGetCurrentContext();
    void *entry = fldGetTaskRecordValue(work->key);

    if (entry != NULL) {
        func_00140A58(entry);
    }
    return 1;
}

s32 fldScriptReturnSelectedActorMotionId(void) {
    scrSetIntegerReturnValue(fldGetSelectedActorMotionId());
    return 1;
}

s32 func_00156F40(void) {
    func_0014BF98(evtGetWorldUnitNestedValue(scrReadIntParameter(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 fldCommandFindEffectByName(void) {
    char *param = scrReadStringParameter(0);

    scrSetIntegerReturnValue(fldFindEffectByName(param));
    return 1;
}

s32 fldCmdGetCurrentSceneSelectionResource(void) {
    scrSetIntegerReturnValue(fldGetCurrentSceneSelectionResource());
    return 1;
}

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommandClearSelectedFlag(void) {
    s32 changed = 0;
    switch (scrReadIntParameter(0)) {
    case 0:
        if (fldAreaState[3] & 1) {
            fldAreaState[3] &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (fldAreaState[3] & 2) {
            fldAreaState[3] &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (fldAreaState[3] & 4) {
            fldAreaState[3] &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (fldAreaState[3] & 8) {
            fldAreaState[3] &= ~8;
            changed = 1;
        }
        break;
    }
    scrSetIntegerReturnValue(changed);
    return 1;
}

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void) {
    s32 solarPhaseThreshold = fldMirroredSolarThresholds[evtGetMirroredSolarPhase()];
    if (solarPhaseThreshold >= effMiscRandMod(0, 100)) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void) {
    s32 commandName;
    commandName = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(commandName, 0x7f);
    return 1;
}

u32 fldCommandSendSoundControl(void) {
    sdfSoundStopNamedPlayback();
    return 1;
}

/* Return sound-command busy state to the field script interpreter. */
s32 fldCommandIsSoundBusy(void) {
    scrSetIntegerReturnValue(sdfSoundIsCommandBusy());
    return 1;
}

/* Pass five field-script arguments to the underlying handler. */
s32 fldCmdMarkBitmapRegion(void) {
    s32 first = scrReadIntParameter(0);
    s32 second = scrReadIntParameter(1);
    s32 third = scrReadIntParameter(2);
    s32 fourth = scrReadIntParameter(3);
    func_00123DE8(D_00389780[0], first, second, third, fourth, scrReadIntParameter(4));
    return 1;
}

extern s32 D_003898A4[];

s32 func_00157258(void) {
    D_003898A4[0] = 1;
    return 1;
}

typedef struct FldSceneParam {
    s32 unk0;
    s32 value;
} FldSceneParam;

extern FldSceneParam *D_00435F1C;

s32 fldCmdGetPendingSceneParam(void) {
    if (D_00435F1C == NULL) {
        scrSetIntegerReturnValue(-1);
        return 1;
    }
    scrSetIntegerReturnValue(D_00435F1C->value);
    return 1;
}

s32 fldCmdSetTargetGuideMode(void) {
    if (scrReadIntParameter(0) == 2) {
        fldSetTargetGuideEnabled(0);
        D_003898B0[0] = 0;
    } else if (scrReadIntParameter(0) == 0) {
        fldSetTargetGuideEnabled(0);
    } else {
        fldSetTargetGuideEnabled(1);
    }
    return 1;
}

extern void func_001536B8(s32);

s32 func_00157308(void) {
    func_001536B8(scrReadIntParameter(0));
    return 1;
}

extern void effMiscSeedRandomFromClock(void *);

extern void effLoadCommonTexturesAndResetRenderFlags(void);

extern void effInitializeBillResourceOwners(void);

extern void parSysReset(void);

extern void func_0015B270(void);

extern void func_00194A70(void);

extern void effInitWorks(void);

extern void effBTLFieldColorResetFlags(void);

extern void func_00157400(void);

extern void effManagerUpdateAndDispatch(void);

extern void effManagerInitializeSubsystems(void);

extern u8 D_003AA868[];

extern char D_004363F8[];

void fldCreateFieldEffectTask(void) {
    effMiscSeedRandomFromClock(D_003AA868);
    effLoadCommonTexturesAndResetRenderFlags();
    effInitializeBillResourceOwners();
    parSysReset();
    func_0015B270();
    kwlnTaskCreate("effect_f", 0x2B04, 0, 0, func_00157400, NULL, 0);
    kwlnTaskCreate(D_004363F8, 0x2B18, 0, 0, effManagerUpdateAndDispatch, effManagerInitializeSubsystems, 0);
    func_00194A70();
    effInitWorks();
    effBTLFieldColorResetFlags();
}

INCLUDE_SDATA(const s32, "game/code_00154558", fldRoomNameSentinel);

INCLUDE_SDATA(const s32, "game/code_00154558", D_004363F0);

INCLUDE_SDATA(const s32, "game/code_00154558", D_004363F8);

