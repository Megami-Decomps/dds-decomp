#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"

typedef struct FldCamPose {
    u8 pad0[0x10];
    s32 world; /* 0x10 */
    s32 stage;
    u8 pad18[0x18];
    f32 focusPos[3]; /* 0x30 */
    u8 pad3C[4];
    char requestedSceneName[0x10];
    s32 focusActive; /* 0x50 */
    u8 pad54[0x10];
    f32 negatedAngle;
    u8 pad68[8];
    s32 unk70;
    u8 pad74[0x4C];
    s32 unkC0;
    u8 padC4[0x66];
    s16 unk12A;
    u8 pad12C[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad14C[0x18];
    f32 angle;
    u8 pad168[0x34];
    struct {
        s32 value; /* fldmix.LB node value */
        s32 block; /* sdfMemoryGetBlockAddress(value) */
    } fldmix[4]; /* 0x19C */
} FldCamPose;

typedef struct FldVec3 {
    f32 x;
    f32 y;
    f32 z;
} FldVec3;

extern u64 fldEvaluateCameraMoveTracking(void);

extern s32 fldGetRowValue(s32);

extern s32 fldGetActorMotionEntry(s32);

extern s32 fldGetActorStat1(s32);

extern s32 fldGetActorStat0(s32);

extern s32 scrReadIntParameter(s32);

extern s32 scrGetCurrentContext(void);

extern s32 fldFindRoomByTask(u32);

extern u32 fldDamEffectResource;

extern u32 fldDamEffectData;

extern u32 fldDamEffectNode;

extern u32 fldDamEffectPositioned;

extern u32 fldYukEffectResource;

extern u32 fldYukEffectData;

extern u32 fldYukEffectNode;

extern u32 fldYukEffectPositioned;

extern s32 fldWeatherLimitTexture;

extern s32 D_003BAFBC;

extern s32 D_003BAFC4;

extern s32 D_003BAFB4;

extern u32 D_003BAFA0;

extern u32 D_003BAFA8;

extern u32 D_003BAFB0;

extern u32 fldSceneReady;

extern u32 fldSceneRecords;

extern s32 fldSceneRecordCount;

extern s32 fldSceneRecordResource;

extern u32 fldFixedArchiveLoadPhase;

extern u32 fldArchiveLoadPending;

extern u32 fldCurrentBgmHandle;

extern s32 fldPendingSoundCount;

extern s32 fldAreaFlagIndex;

extern s32 D_003BAEB4;

extern s32 fldAreaState[];

extern s32 fldPrimaryEffectPositionPending;

extern s32 fldSecondaryEffectPositionPending;

extern s32 dds3GetWorldObject(void);

extern s32 dds3GetWorldPlayerObject(s32 arg0);

extern char D_003BB000[]; /* "BARIA" */

extern char *D_003BAE44;

extern void *dds3FindWorldObjectNodeByKey(u64, s32, s32);

extern s32 fldTestRoomProbeFacingAndRange(s32, void *);

extern s32 fldTestRoomProbeFacing(s32, void *);

extern void fldSetCameraNodeModeWithTen(void);

extern void dds3InvokeSlot1Handler(s32 arg0, s32 arg1);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern void fldResetPlayerSceneObjectState(void);

extern char *scrReadStringParameter(s32 idx);

extern s32 D_0032E400[];

extern s32 D_0032E408[];

extern s32 D_0032E3C0[];

extern s32 fldSceneSoundBase;

extern s32 D_0032E570[];

extern u8 D_003BAE78[];

extern u64 dds3GetWorldSecondaryObject(void);

extern u32 *dds3FindObjectChainNodeByName(u64 world, const char *name);

extern void func_003003F0(const char *fmt, ...);

extern s32 fldCurrentBgmMode;

extern s32 fldSceneBgmArchiveTrack;

extern s32 fldSceneBgmArchivePhase;

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

extern s32 D_003BAF48, D_003BAF4C, D_003BAF40, D_003BAF3C;

extern s32 D_003BAF30, D_003BAF34, D_003BAF38;

extern s32 D_003BAF30;

extern s32 D_003BAF34;

extern s32 D_003BAF38;

extern s32 D_003BAF3C;

extern s32 D_003BAF4C;

extern s32 D_003BAF58;

extern s32 D_003BAF64;

extern s32 fldRoomEffectEntryCount;

extern s32 D_003BAF70;

extern s32 D_003BAF7C;

extern s32 fldObjectSlotCount;

extern f32 fldBannerColorPhase;

extern f32 sdfSinPoly(f32);

extern s32 fldRoomEffectEntryCount;

extern int strcmp(const char *, const char *);

extern u32 D_003BAFA4;

extern u32 D_003BAFAC;

extern u32 D_003BAF9C;

extern void fldPreparePlayerSceneCameraTarget(void);

extern void evtSetSolarOverlayFullyVisible(void);

/* Script execution state retains the task id used by the room lookup. */
typedef struct FldScriptTask {
    u8 pad00[0xE4];
    s32 taskId; /* 0xE4 */
} FldScriptTask;

void fldFireRoomEffects(void);

s32 fldCmdQueryActorEntrySceneStatus(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = dds3GetWorldPlayerObject(world);
    void *entry;
    s32 result;

    if (unit == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    entry = dds3FindWorldObjectNodeByKey(world, fldFindTaskRecordId(((FldScriptTask *)scrGetCurrentContext())->taskId), 0x11);
    if (entry == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacingAndRange(unit, entry) == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    func_001411C0(scrReadIntParameter(0));
    func_00141190(1);
    result = func_001411F0();
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
    s32 unit = dds3GetWorldPlayerObject(world);
    void *entry;
    s32 result;

    if (unit == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    entry = dds3FindWorldObjectNodeByKey(world, fldFindTaskRecordId(((FldScriptTask *)scrGetCurrentContext())->taskId), 0x11);
    if (entry == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacing(unit, entry) == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    func_001411C0(scrReadIntParameter(0));
    func_00141190(1);
    result = func_001411F0();
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

s32 fldCmdReadSceneStatus(void) {
    s32 result;

    func_001411C0(scrReadIntParameter(0));
    func_00141190(1);
    result = func_001411F0();
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
    s32 unit = dds3GetWorldPlayerObject(world);
    void *entry;

    if (unit == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    entry = dds3FindWorldObjectNodeByKey(world, fldFindTaskRecordId(((FldScriptTask *)scrGetCurrentContext())->taskId), 0x11);
    if (entry == 0) {
        scrSetIntegerReturnValue(0);
        return 1;
    }
    if (fldTestRoomProbeFacingAndRange(unit, entry) == 0) {
        scrSetIntegerReturnValue(0);
    } else {
        scrSetIntegerReturnValue(1);
    }
    return 1;
}

s32 fldCmdRestoreCameraNodeMode(void) {
    s32 object;

    fldResetPlayerSceneObjectState();
    object = dds3GetWorldPlayerObject(dds3GetWorldObject());
    if (object == 0) {
        return 1;
    }
    fldSetCameraNodeModeWithTen();
    return 1;
}

s32 fldCmdReleaseCurrentObject(void) {
    s32 object;

    object = dds3GetWorldPlayerObject(dds3GetWorldObject());
    if (object == 0) {
        return 1;
    }
    dds3InvokeSlot1Handler(object, 0);
    fldSetCameraNodeModeWithTen();
    fldPreparePlayerSceneCameraTarget();
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


extern void func_0012DB70(void);

/* The kind-4 lookup returns a camera with a separately owned world transform. */
s32 fldCmdFocusCameraOnObject(void) {
    FldCamPose *work;
    CameraObject *obj;
    u64 world = dds3GetWorldSecondaryObject();

    obj = dds3FindWorldObjectNodeByKey(world, scrReadIntParameter(0), 4);
    if (obj == NULL) {
        return 1;
    }
    work = (FldCamPose *)fldAreaState;
    work->focusActive = 1;
    work->focusPos[0] = obj->inner->position[0];
    work->focusPos[1] = obj->inner->position[1];
    work->focusPos[2] = obj->inner->position[2];
    func_0012DB70();
    return 1;
}

extern FldVec3 fldLookAtNearPoint;

extern FldVec3 fldLookAtFarPoint;

extern f32 sdfSinPoly(f32);

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern s32 func_001312D8();

extern void fldClearCameraObjectHighlightFlag();

extern void func_0012CB48();

s32 fldUpdateLookAtSegment(void) {
    FldCamPose *cam = (FldCamPose *)fldAreaState;
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
    fldLookAtNearPoint.x = near.x;
    fldLookAtNearPoint.y = near.y;
    fldLookAtNearPoint.z = near.z;
    fldLookAtFarPoint.x = far.x;
    fldLookAtFarPoint.y = far.y;
    fldLookAtFarPoint.z = far.z;
    func_001312D8();
    fldClearCameraObjectHighlightFlag();
    func_0012CB48();
    return 1;
}

s32 fldCmdSetScenePhaseThree(void) {
    D_0032E400[0] = 3;
    return 1;
}

s32 fldCmdSetLookAtHeading(void) {
    FldCamPose *cam;
    FldVec3 near;
    FldVec3 far;

    if (scrReadIntParameter(0) < 0) {
        ((FldCamPose *)fldAreaState)->negatedAngle = -((FldCamPose *)fldAreaState)->angle;
    } else {
        ((FldCamPose *)fldAreaState)->negatedAngle = -45 * scrReadIntParameter(0);
    }
    cam = (FldCamPose *)fldAreaState;
    near.x = cam->x - sdfSinPoly((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + sdfEvaluateCosineViaSinePhaseShift((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + sdfEvaluateCosineViaSinePhaseShift(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    fldLookAtNearPoint.x = near.x;
    fldLookAtNearPoint.y = near.y;
    fldLookAtNearPoint.z = near.z;
    fldLookAtFarPoint.x = far.x;
    fldLookAtFarPoint.y = far.y;
    fldLookAtFarPoint.z = far.z;
    func_0012CB48();
    return 1;
}

extern s32 fldGetPlayerSceneState(void);

extern s32 dds3SetWorldCameraObject(s32, s32);

extern void dds3TransformCameraVectorsByInnerRotation(s32, f32 *, f32 *);

extern void fldUpdateCameraProjectionEndpoints(void);

extern f32 D_00330630[];

extern f32 D_00330640[];

extern s32 D_003BAD24;

extern s32 D_003BAD28;

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
        D_00330630[0] = pos[0];
        D_00330630[1] = pos[1];
        D_00330630[2] = pos[2];
        D_00330640[0] = rot[0];
        D_00330640[1] = rot[1];
        D_00330640[2] = rot[2];
        D_003BAD24 = 0;
        D_003BAD28 = scrReadIntParameter(1);
        D_0032E400[0] = 4;
    } else {
        world = dds3GetWorldObject();
        handle = dds3FindWorldObjectNodeByKey(world, scrReadIntParameter(0), 4);
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
    strcpy(((FldCamPose *)fldAreaState)->requestedSceneName, name);
    return 1;
}

extern void evtSetSolarOverlayFullyTransparent(void);

extern void evtDisableSolarOverlayAlpha(void);

extern void evtEnableSolarOverlayAlpha(void);

s32 fldCmdSetSolarOverlayMode(void) {
    s32 mode = scrReadIntParameter(0);

    fldAreaState[51] = mode;
    fldAreaState[52] = 0;
    if (mode < 4) {
        fldAreaState[53] = mode;
    }
    switch (fldAreaState[51]) {
    case 0:
        evtSetSolarOverlayFullyTransparent();
        break;
    case 1:
        evtSetSolarOverlayFullyVisible();
        fldAreaState[78] = 1;
        break;
    case 2:
        evtDisableSolarOverlayAlpha();
        fldAreaState[52] = 0xF;
        break;
    case 3:
        evtEnableSolarOverlayAlpha();
        fldAreaState[52] = 0;
        fldAreaState[78] = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    return 1;
}

void func_0014DAF0(void) {
}

u32 func_0014DAF8(void) {
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
    func_003003F0("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

extern char fldRoomNameSentinel[];

extern s32 func_00302290(char *, const char *);

s32 fldParseRoomNumberFromName(char *name) {
    s32 index;
    if (func_00302290(name, fldRoomNameSentinel) == 0) return 0;
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
    D_0032E408[0] = value;
    return 1;
}

s32 fldCmdSetFieldParameterPair(void) {
    s32 value;

    D_0032E570[11] = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    func_00132FD0(D_0032E570[11], value);
    return 1;
}

extern void fldSetFadeTarget(s32, s32, s32);

extern void fldSetSwayMode(s32);

s32 fldCmdSetFadeAndSway(void) {
    s32 second;
    s32 third;
    D_0032E570[13] = scrReadIntParameter(0) & 0xFF;
    second = scrReadIntParameter(1);
    third = scrReadIntParameter(2);
    fldSetFadeTarget(D_0032E570[13], second, third);
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
    if (strcmp(name, D_003BB000) == 0) {
        name = D_003BAE44;
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

extern void fldSetRoomObjectModeFlag(s32, s32, s32, s32);

extern void dds3SetObjectModeAndDefaultWeight(void *, s32);

s32 fldCmdApplyRoomObjectModeZero(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
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
            fldSetRoomObjectModeFlag(world, stage, i, 0);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    if (world == fldAreaState[4] && stage == fldAreaState[5] + 1) {
        id = fldFindSearchId(name);
        if (id != -1) {
            object = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), id, 6);
            switch (scrReadIntParameter(3)) {
            case 0:
                dds3SetObjectModeAndDefaultWeight(object, 0);
                break;
            case 1:
                dds3SetObjectModeAndDefaultWeight(object, 1);
                break;
            case 2:
                dds3SetObjectModeAndDefaultWeight(object, 2);
                break;
            case 3:
                dds3SetObjectModeAndDefaultWeight(object, 6);
                break;
            }
        }
    }
    return 1;
}

s32 fldCmdApplyRoomObjectModeOne(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
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
            fldSetRoomObjectModeFlag(world, stage, i, 1);
        }
    } else {
        fldSetRoomObjectModeFlag(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    if (world == fldAreaState[4] && stage == fldAreaState[5] + 1) {
        id = fldFindSearchId(name);
        if (id != -1) {
            object = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), id, 6);
            switch (scrReadIntParameter(3)) {
            case 0:
                dds3SetObjectModeAndDefaultWeight(object, 3);
                break;
            case 1:
                dds3SetObjectModeAndDefaultWeight(object, 4);
                break;
            case 2:
                dds3SetObjectModeAndDefaultWeight(object, 5);
                break;
            }
        }
    }
    return 1;
}

extern s32 D_0032E3C4[];

extern void fldSetRoomSceneFlag(s32, s32, s32, s32);

s32 fldCmdSetSceneBits(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
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

extern char *D_003BAE48;

s32 fldCmdSetSceneBitsBarrierRoom(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    name = scrReadStringParameter(2);
    if (strcmp(name, D_003BB000) == 0) {
        name = D_003BAE48;
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

extern s32 D_0032E3C4[];

extern s32 fldSetMapTargetFlag(s32, s32, s32, s32);

extern s32 fldSetAlternateMapTargetFlag(s32, s32, s32, s32);

s32 fldOpClearMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldOpSetMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetMapTargetFlag(area, floor, target, 1);
    return 1;
}

s32 fldOpClearAlternateMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetAlternateMapTargetFlag(area, floor, target, 0);
    return 1;
}

s32 fldOpSetAlternateMapTargetFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
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
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    slot = scrReadIntParameter(2);
    if (slot == 0) {
        return 1;
    }
    fldSetMapSlotByte(world, stage, slot, scrReadIntParameter(3));
    return 1;
}

extern void fldSetFlagBit(s32, s32, s32);

s32 fldCmdSetFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetFlagBit(area, floor, target);
    return 1;
}

extern void fldClearFloorFlag(s32, s32, s32);

s32 fldCmdClearFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
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

extern void fldSetMapSlotValueFlag(s32, s32, s32, s32);

s32 fldCmdSetSceneBitsValue(void) {
    s32 world;
    s32 stage;
    s32 room;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
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

typedef struct FldSceneParam {
    s32 unk_0;
    s16 unk_4;
    s16 unk_6;
    s32 unk_8;
    s32 unk_c;
} FldSceneParam;

extern FldSceneParam D_0034C8F0[];

s32 fldCmdPushSceneParam(void) {
    s32 room = fldFindRoomByTask(((FldScriptTask *)scrGetCurrentContext())->taskId);

    switch (scrReadIntParameter(0)) {
    case 0:
        scrSetIntegerReturnValue(D_0034C8F0[room].unk_0);
        break;
    case 1:
        scrSetIntegerReturnValue(D_0034C8F0[room].unk_4);
        break;
    case 2:
        scrSetIntegerReturnValue(D_0034C8F0[room].unk_6);
        break;
    case 3:
        scrSetIntegerReturnValue(D_0034C8F0[room].unk_8);
        break;
    case 4:
        scrSetIntegerReturnValue(D_0034C8F0[room].unk_c);
        break;
    }
    return 1;
}

/* Script task's scene-record key, shared by the adjacent field commands. */
typedef struct FldTaskWork {
    u8 pad00[0xE4];
    s32 recordKey; /* 0xE4 */
} FldTaskWork;

u32 fldCmdActivateTaskRoomObject(void) {
    s32 task;
    u64 room;

    task = scrGetCurrentContext();
    room = fldFindRoomByTask(((FldTaskWork *)task)->recordKey);
    fldActivateFlaggedObject(room);
    return 1;
}

u32 fldCmdTestTaskRoomObjectActive(void) {
    if (fldTestObjectActivationFlag(fldFindRoomByTask(((FldTaskWork *)scrGetCurrentContext())->recordKey)) != 0) {
        scrSetIntegerReturnValue(1);
    } else {
        scrSetIntegerReturnValue(0);
    }
    return 1;
}

u32 fldCmdSetCurrentTaskScene(void) {
    s32 scene;

    if (fldIsSceneStateEight()) {
        func_0013DF60(0);
        return 1;
    }
    scene = fldGetTaskRecordValue(((FldTaskWork *)scrGetCurrentContext())->recordKey);
    if (scene) {
        func_0013DF60(scene);
    }
    return 1;
}

u32 func_0014EEF8(void) {
    s32 value;

    value = scrReadIntParameter(0);
    func_0013E5A8(value);
    return 1;
}

u32 func_0014EF20(void) {
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
    actorStat = fldGetActorStat1(actorStat);
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

u32 func_0014EFE8(void) {
    s32 value;

    value = scrReadIntParameter(0);
    func_0013FA40(value);
    return 1;
}

u32 func_0014F010(void) {
    s32 first;
    s32 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_0011E810(first, second);
    return 1;
}

u32 fldCmdResetCameraMoveTracking(void) {
    fldResetCameraMoveTracking();
    return 1;
}

u32 fldCmdQueryCameraMoveTracking(void) {
    u64 result;

    result = fldEvaluateCameraMoveTracking();
    scrSetIntegerReturnValue(result);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_0014D110", fldRoomNameSentinel);

INCLUDE_SDATA(const s32, "game/code_0014D110", D_003BB000);

