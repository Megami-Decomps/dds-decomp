#include "common.h"
#include "evt_world.h"
#include "evt_unit.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "sdf_draw.h"
#include "scr.h"



extern EvtUnitVectorSlot D_003D7BD8[7];


extern void sdfBuildLightingPacket(void *, SdfLightSources, f32 *);

typedef struct {
    u8 pad00[0x10];     /* 0x00 */
    f32 unk10;          /* 0x10 */
    u8 pad14[0x04];     /* 0x14 */
    f32 unk18;          /* 0x18 */
    f32 unk1C;          /* 0x1C */
    u8 pad20[0x250];    /* 0x20 */
} Entry270;

extern Entry270 *D_003BAA20;

extern void *dds3GetWorldObject(void);
extern void effObjSetInnerThirdVec(void *object, void *vector);

extern u8 evtTestUnitStatusFlags(EvtUnit *unit);

extern EvtUnit *evtGetWorldUnitNestedValue(s32 idx);
extern void evtInitializeUnitColorTransition(EvtUnit *unit, s32 arg, u32 color1, u32 color2);
extern void evtSetUnitRgbTransition(EvtUnit *unit, s32 arg, u32 color);
extern void evtSetUnitAlphaTransition(EvtUnit *unit, s32 arg, u32 color);

extern u32 evtWindowMotionUnit;
extern s32 D_003BBDB0;

/* Event lip-sync registry: world -> root -> list -> unit links. */
typedef struct EvtLipsModel {
    u8 pad00[0x18];
    void *chunk;        /* 0x18 */
} EvtLipsModel;

typedef struct EvtLipsMh {
    u8 pad00[0x0C];
    EvtLipsModel *model; /* 0x0C */
} EvtLipsMh;

typedef struct EvtLipsLink {
    u8 pad00[0x08];
    void *unit;         /* 0x08 */
    EvtLipsMh *mh;      /* 0x0C */
} EvtLipsLink;

typedef struct EvtLipsNode {
    u8 pad00[0x18];
    EvtLipsLink *link;  /* 0x18 */
    u8 pad1C[0x04];
    struct EvtLipsNode *next; /* 0x20 */
} EvtLipsNode;


extern u32 sdfGetUniqueChunkValue();

extern s32 scrReadIntParameter(s32 idx);
extern s32 mdlSpawnLinkedCameraSlotViewerObject(s32 arg0, s32 arg1);


extern void *dds3FindWorldObjectNodeByKey(void *world, s32 objectId, s32 kind);
extern EffWorldNode *dds3GetWorldPlayerObject(EffWorldNode *world);
extern s32 evtIsUnitMotionIdleOrTimedMode(EvtUnit *unit);
extern void effObjDispatchReadyState(void *arg0);
extern void dds3RemoveWorldObjectNode(void *arg0);
extern void *dds3GetWorldSecondaryObject(void);
extern void evtBeginUnitVectorTransition(EvtUnit *work, s32 mode, s128 *vector, s32 unused, s32 frames, s32 valueB6, s32 value94, s32 unusedLast);
extern void dds3FreePathObject(s32);
extern s32 dds3CreatePathCurveWork(void *);
extern void dds3InterpolatePathVectorVU(s32);
extern f32 evtMeasurePathTrajectoryLength(s32);
extern void evtScaleValueByMultiplier(s32, f32);
extern void sdfSetFloatCounterDirection(s32, s32);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU();
extern void effObjSetInnerSecondVec(void *, void *);

extern void dds3SetObjectFlags(void *object, s32 flags);
extern void dds3ClearObjectFlags(void *object, s32 flags);
extern void evtResetObjectPendingValue(EffWorldNode *object);
extern void evtArmEffectObjectPendingValue(EffWorldNode *object, s32 value);
extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern s32 evtCreateModelFromPackResource(s32 eventId, s32 resourceId);
extern s32 evtCreateMotionSeTask(s32 arg0, s32 arg1, s32 arg2);
extern s32 evtFindTaskById(s32 taskId);
extern void func_00101A80(s32 arg0, s32 arg1);
extern void evtPrepareUnitMotionState(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void evtConfigureUnitMotionSlot(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 func_003003F0();
extern u8 D_003AC480[];
extern void evtSetUnitValueTransition(EvtUnit *unit, EffWorldNode *target, s32 duration);
extern s32 mdlCheckNodeByte30(u32 *arg0, s32 arg1);
extern void *memset(void *dst, s32 c, u32 n);
extern void effObjReplaceActiveEventNode(void *arg0, u32 arg1);
extern void effObjSetInnerFirstVec(void *object, void *vector);
extern f32 bfWaitReadArgFloat(s32 idx);

/* World object views used by the model-parameter opcodes. */
typedef struct EvtModelHeader {
    u8 pad00[0x04];
    u32 flags;          /* 0x04: bit 2 selects the header transform */
    u8 pad08[0x08];
    f32 positionX;      /* 0x10: script-supplied translation */
    f32 positionY;      /* 0x14 */
    f32 positionZ;      /* 0x18 */
} EvtModelHeader;

typedef struct EvtSourceVec {
    f32 positionX;      /* 0x00: copied into model position */
    f32 positionY;      /* 0x04 */
    f32 positionZ;      /* 0x08 */
    u8 pad0C[0x04];
    f32 rotationX;      /* 0x10: copied into model rotation */
    f32 rotationY;      /* 0x14 */
    f32 rotationZ;      /* 0x18 */
    f32 rotationW;      /* 0x1C */
} EvtSourceVec;

typedef struct EvtSourceObj {
    u8 pad00[0x18];
    EvtSourceVec *vec;  /* 0x18 */
} EvtSourceObj;
extern char D_003AC588[];
extern u8 D_003AC2A0[];
extern u8 D_003AC550[];
extern u8 D_003AC5B0[];
extern u8 D_003AC600[];

typedef struct EvtLodRoot {
    u8 pad00[0x98];
    s8 lodIndex;        /* 0x98 */
} EvtLodRoot;

typedef struct EvtLodMh {
    u8 pad00[0x18];
    EvtLodRoot *root;   /* 0x18 */
} EvtLodMh;

typedef struct EvtLodWork {
    u8 pad00[0x0C];
    EvtLodMh *mh;       /* 0x0C */
} EvtLodWork;

typedef struct EvtLodModel {
    u8 pad00[0x0C];
    EvtLodWork *workbase; /* 0x0C */
} EvtLodModel;

typedef struct EvtLodUnit {
    u8 pad00[0x18];
    EvtLodModel *model;   /* 0x18 */
} EvtLodUnit;

extern s32 sdfGetLodChunkValue();
extern s32 scrGetWindow(void);
extern void itfMesSetWindowCallbackAddress(s32 window, void (*callback)(void));
extern void evtStoreUnitMotionSlotSelection(EvtUnit *unit, s32 arg1, s32 arg2);
extern s32 scrReadStringParameter(s32 idx);
extern void *effObjCreateKindFromResource(s32 arg0, s32 arg1);
extern void effObjSetFlags(void *object, s32 flags);
extern void *effObjSpawnLoadedResourceEffect(s32 arg0, void *arg1, void *arg2);
extern u8 D_003AC520[];
extern void *effObjCreateFromResolvedResource(s32 arg0, void *arg1, void *arg2);
extern s32 scrSetIntegerReturnValue(s32 arg0);
extern void mdlAttachWorldObjectToSourceVector(s32 arg0, s32 arg1);
extern void evtSetUnitStatusFlags(EvtUnit *unit);
extern void evtConfigureUnitTransition(EvtUnit *unit, s32 arg1);
extern void evtEndUnitValueTransition(EvtUnit *unit, s32 arg1);
extern void evtActivateStoredUnitMotionSlot(u32 arg0);

typedef struct EvtWorldUnitRef {
    u8 pad00[0x18];
    s128 *transform; /* 0x18: first aligned vector */
} EvtWorldUnitRef;

void evtBeginVectorTransition(EvtUnit *work, s128 *vector, s32 frames) {
    if (frames > 0 && frames <= 100) {
        work->linkedUnit = NULL;
        work->motionState = EVT_UNIT_MOTION_STATE_VECTOR;
        PCP_COPY_VECTOR(work->targetVector, vector);
        work->motionParameter = frames;
        work->directionOffset = 0;
        work->unk94 = 0;
        work->motionTicks = 0;
    }
}

void evtAttachSecondaryWorldUnit(EvtUnit *work, s32 objectId, s32 frames) {
    EvtWorldUnitRef *worldUnit;

    worldUnit = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (worldUnit != NULL) {
        evtBeginVectorTransition(work, worldUnit->transform + 1, frames);
        work->linkedUnit = worldUnit;
    }
}

void evtBeginUnitVectorTransition(EvtUnit *work, s32 mode, s128 *vector, s32 unused, s32 frames, s32 valueB6, s32 value94, s32 unusedLast) {
    work->motionSubmode = mode;
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = 0;
    work->linkedUnit = NULL;
    PCP_COPY_VECTOR(work->targetVector, vector);
    work->motionParameter = frames;
    work->directionOffset = valueB6;
    work->unk94 = value94;
    work->motionTicks = 0;
}

void evtBeginUnitTransitionTowardWorldObject(EvtUnit *work, s32 mode, s32 objectId, s32 unused, s32 frames, s32 valueB6, s32 value94, s32 unusedLast) {
    EvtWorldUnitRef *worldUnit;

    worldUnit = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (worldUnit != NULL) {
        evtBeginUnitVectorTransition(work, mode, worldUnit->transform, unused, frames, valueB6, value94, unusedLast);
        work->transitionSourceKind = 1;
        work->linkedUnit = worldUnit;
    }
}

void evtSetUnitPathFollow(EvtUnit *work, s32 objectId, s32 frames, s32 valueB6, s32 mode, s32 dirFlag, s32 sideMode) {
    void *pathSource;
    s32 path;

    pathSource = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), objectId, 0x10);
    if (pathSource == NULL) {
        return;
    }
    if (work->pathHandle != 0) {
        dds3FreePathObject(work->pathHandle);
    }
    path = dds3CreatePathCurveWork(pathSource);
    work->pathHandle = path;
    work->pathSpeed = 40.0f / evtMeasurePathTrajectoryLength(path);
    if (dirFlag == 0) {
        evtScaleValueByMultiplier(path, 0.0f);
        sdfSetFloatCounterDirection(path, 0);
    } else {
        evtScaleValueByMultiplier(path, 1.0f);
        sdfSetFloatCounterDirection(path, 1);
        work->pathSpeed = -work->pathSpeed;
    }
    switch (mode) {
    case 0:
        work->motionSubmode = 0;
        work->flags &= ~2;
        break;
    case 1:
        work->motionSubmode = 3;
        work->flags |= 2;
        break;
    }
    switch (dirFlag) {
    case 0:
        work->flags &= ~EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    case 1:
        work->flags |= EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    }
    switch (sideMode) {
    case 0:
        work->flags &= ~8;
        work->flags &= ~0x10;
        break;
    case 1:
        work->flags |= 8;
        work->flags &= ~0x10;
        break;
    case 2:
        work->flags &= ~8;
        work->flags |= 0x10;
        break;
    }
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = 2;
    work->linkedUnit = pathSource;
    dds3InterpolatePathVectorVU(path);
    VU0_STORE_VF($vf10, work->targetVector);
    work->motionParameter = frames;
    work->directionOffset = valueB6;
    work->unk94 = 0;
    work->motionTicks = 0;
}

s32 evtStartUnitModeWithValue(EvtUnit *work, s32 value) {
    s32 ret = 0;

    if (value != 0) {
        work->unk94 = value;
        work->motionTicks = 0;
        work->motionState = EVT_UNIT_MOTION_STATE_VALUE;
        ret = 1;
    }
    return ret;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222ED8);

extern void mnuInitializeCampPanelVisualDefaults(f32 *, f32 *, f32 *, f32 *, f32 *);

void evtResetUnitVectorSlots(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        mnuInitializeCampPanelVisualDefaults(&D_003D7BD8[i].vec[0], &D_003D7BD8[i].vec[4], &D_003D7BD8[i].vec[8], &D_003D7BD8[i].vec[12], &D_003D7BD8[i].vec[13]);
        D_003D7BD8[i].state = EVT_UNIT_VECTOR_SLOT_EMPTY;
        D_003D7BD8[i].id = 0;
    }
}

void evtSetSlotVectors(s32 slotIndex, s32 slotState, s32 unitId, f32 *firstEndpoint, f32 *secondEndpoint, f32 *color) {
    if (slotIndex < 7) {
        D_003D7BD8[slotIndex].vec[0] = firstEndpoint[0];
        D_003D7BD8[slotIndex].state = slotState;
        D_003D7BD8[slotIndex].vec[1] = firstEndpoint[1];
        D_003D7BD8[slotIndex].vec[2] = firstEndpoint[2];
        D_003D7BD8[slotIndex].vec[3] = 0;
        D_003D7BD8[slotIndex].vec[4] = secondEndpoint[0];
        D_003D7BD8[slotIndex].vec[5] = secondEndpoint[1];
        D_003D7BD8[slotIndex].vec[6] = secondEndpoint[2];
        D_003D7BD8[slotIndex].vec[7] = secondEndpoint[3];
        D_003D7BD8[slotIndex].vec[8] = color[0];
        D_003D7BD8[slotIndex].vec[9] = color[1];
        D_003D7BD8[slotIndex].vec[10] = color[2];
        D_003D7BD8[slotIndex].vec[11] = 1.0f;
        if (slotState == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND) {
            D_003D7BD8[slotIndex].id = unitId;
        } else {
            D_003D7BD8[slotIndex].id = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00222AC0", evtSetSlotVector);

/* Copy the second vector of the slot bound to `id` (else the first state-2 slot). */
s32 func_00223718(s32 id, f32 *out) {
    s32 found = -1;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && D_003D7BD8[i].id == id) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 7; i++) {
            if (D_003D7BD8[i].state == EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return 0;
        }
    }
    out[0] = D_003D7BD8[found].vec[4];
    out[1] = D_003D7BD8[found].vec[5];
    out[2] = D_003D7BD8[found].vec[6];
    return 1;
}

/* Select a matching slot and write its endpoint render work. */
void evtApplyMatchingUnitSlotEndpoints(EvtUnit *unit) {
    f32 ends[4][4];
    f32 color[4];
    SdfLightSources desc = { ends, 0, 0 };
    s32 found = -1;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && D_003D7BD8[i].id == (s32)unit) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 7; i++) {
            if (D_003D7BD8[i].state == EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return;
        }
    }
    ends[0][0] = D_003D7BD8[found].vec[0];
    ends[0][1] = D_003D7BD8[found].vec[1];
    ends[0][2] = D_003D7BD8[found].vec[2];
    ends[0][3] = 0;
    ends[1][0] = D_003D7BD8[found].vec[4];
    ends[1][1] = D_003D7BD8[found].vec[5];
    ends[1][2] = D_003D7BD8[found].vec[6];
    ends[1][3] = 0;
    color[0] = D_003D7BD8[found].vec[8];
    color[1] = D_003D7BD8[found].vec[9];
    color[2] = D_003D7BD8[found].vec[10];
    color[3] = 1.0f;
    for (i = 0; i < 3; i++) {
        if (color[i] > 1.0f) {
            color[i] = 1.0f;
        }
    }
    sdfBuildLightingPacket(unit->endpointWork, desc, color);
    unit->value = (u32)unit->endpointWork;
}

/* Find auxiliary coordinates for the unit-bound slot, else the shared fallback. */
s32 evtFindUnitSlotAuxCoordinates(EvtUnit *unit, f32 *outX, f32 *outY) {
    s32 i;

    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == EVT_UNIT_VECTOR_SLOT_UNIT_BOUND && D_003D7BD8[i].id == (s32)unit) {
            *outX = D_003D7BD8[i].vec[12];
            *outY = D_003D7BD8[i].vec[13];
            return 1;
        }
    }
    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK) {
            *outX = D_003D7BD8[i].vec[12];
            *outY = D_003D7BD8[i].vec[13];
            return 1;
        }
    }
    return 0;
}

void *evtFindWorldObjectByIdAndKind(s32 kind, s32 id) {
    void *world;

    world = dds3GetWorldObject();
    return dds3FindWorldObjectNodeByKey(world, id, kind);
}

u32 evtGetWorldObjectId(void) {
    void *world;
    EffWorldNode *object;
    s32 id;

    world = dds3GetWorldObject();
    object = dds3GetWorldPlayerObject(world);
    id = -1;
    if (object != NULL) {
        id = object->key;
    }
    scrSetIntegerReturnValue(id);
    return 1;
}

u32 evtOpCreateLinkedCameraViewer(void) {
    s32 param0;
    s32 param1;
    s32 value;

    param0 = scrReadIntParameter(0);
    param1 = scrReadIntParameter(1);
    value = mdlSpawnLinkedCameraSlotViewerObject(param0, param1);
    scrSetIntegerReturnValue(value);
    return 1;
}

u32 evtOpBindMotionSoundToModel(void) {
    s32 param0;
    s32 rid;
    s32 model;
    s32 ret;

    if (scrGetCurrentContext() == 0) {
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    model = evtCreateModelFromPackResource(param0, rid);
    if (model < 0) {
        evtPrintDeveloperConsoleMessage("MODEL_BE not fount RID = %d!\n", scrReadIntParameter(1));
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    ret = evtCreateMotionSeTask(model, param0, rid);
    if (ret != 0) {
        func_00101A80(evtFindTaskById(scrReadIntParameter(0)), ret);
    }
    return scrSetIntegerReturnValue(model);
}

u32 evtOpUseSourceVectorForWorldObject(void) {
    s32 param0;
    s32 param1;

    param0 = scrReadIntParameter(0);
    param1 = scrReadIntParameter(1);
    mdlAttachWorldObjectToSourceVector(param0, param1);
    return 1;
}

u32 evtScriptSetWorldUnitFlagMask(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3SetObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 evtScriptClearWorldUnitFlagMask(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3SetObjectFlags(unit, 0x200);
    return 1;
}

u32 evtOpClearWorldObjectStateFlags(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 evtOpQueueWorldObjectPendingValue(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    evtArmEffectObjectPendingValue(unit, scrReadIntParameter(1));
    return 1;
}

u32 evtOpClearWorldObjectPendingValue(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    evtResetObjectPendingValue(unit);
    return 1;
}

u32 evtOpModelLodChg(void) {
    s32 lod;
    void *world;
    EvtLodUnit *unit;
    EvtLodRoot *root;
    s32 max;

    lod = scrReadIntParameter(1);
    func_003003F0("call: MODEL_LOD_CHG(int,int)\n");
    world = dds3GetWorldObject();
    unit = dds3FindWorldObjectNodeByKey(world, scrReadIntParameter(0), 5);
    if (unit == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) unit pointer null\n");
        return 1;
    }
    if (unit->model->workbase == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) workbase pointer null\n");
        return 1;
    }
    if (unit->model->workbase->mh == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) mh pointer null\n");
        return 1;
    }
    root = unit->model->workbase->mh->root;
    if (root == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) root pointer null\n");
        return 1;
    }
    max = sdfGetLodChunkValue(root);
    if (max < lod) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) lodno over!! max=%d setval=%d\n", max, lod);
        return 1;
    }
    root->lodIndex = lod;
    func_003003F0("success: MODEL_LOD_CHG(int,int)\n");
    return 1;
}

u32 evtSetWorldUnitFirstVector(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = bfWaitReadArgFloat(1);
    v[1] = bfWaitReadArgFloat(2);
    v[2] = bfWaitReadArgFloat(3);
    effObjSetInnerFirstVec(unit, v);
    return 1;
}

u32 evtOpSetWorldUnitRotationFromAngles(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    toRad = 0.017453293f;
    x = bfWaitReadArgFloat(1) * toRad;
    y = bfWaitReadArgFloat(2) * toRad;
    func_002E7F20(x, y, 0.0f);
    VU0_MOVE_VF(vf11, vf10);
    func_002E7F20(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, v);
    effObjSetInnerSecondVec(unit, v);
    return 1;
}

u32 evtSetWorldUnitThirdVector(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = bfWaitReadArgFloat(1);
    v[1] = bfWaitReadArgFloat(2);
    v[2] = bfWaitReadArgFloat(3);
    effObjSetInnerThirdVec(unit, v);
    return 1;
}

u32 func_002241D0(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    evtPrepareUnitMotionState(unit, scrReadIntParameter(1), scrReadIntParameter(2), scrReadIntParameter(3), 0);
    func_003003F0(D_003AC2A0);
    return 1;
}

u32 evtOpSetUnitParams5(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 scriptParam1 = scrReadIntParameter(1);
        s32 scriptParam2 = scrReadIntParameter(2);
        s32 scriptParam3 = scrReadIntParameter(3);
        s32 scriptParam4 = scrReadIntParameter(4);
        evtPrepareUnitMotionState(unit, scriptParam1, scriptParam2, scriptParam3, scriptParam4);
    }
    return 1;
}

extern void evtConfigureUnitMotionSlot(EvtUnit *, s32, s32, s32, s32, s32);

u32 evtOpSetUnitParams6(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 scriptParam1 = scrReadIntParameter(1);
        s32 scriptParam2 = scrReadIntParameter(2);
        s32 scriptParam3 = scrReadIntParameter(3);
        s32 scriptParam4 = scrReadIntParameter(4);
        s32 scriptParam5 = scrReadIntParameter(5);
        evtConfigureUnitMotionSlot(unit, scriptParam1, scriptParam2, scriptParam3, scriptParam4, scriptParam5);
    }
    return 1;
}

void evtInvokeStoredWindowMotion(void) {
    evtActivateStoredUnitMotionSlot(evtWindowMotionUnit);
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC2A0);

void evtLipsExecFunction(s32 id, s32 motion) {
    void *unit = NULL;
    EvtLipsModel *model = NULL;
    EvtLipsNode *node;

    if (id == 0) {
        return;
    }
    for (node = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head; node != NULL; node = node->next) {
        model = node->link->mh->model;
        if (sdfGetUniqueChunkValue(model->chunk) == id) {
            unit = node->link->unit;
            break;
        }
    }
    if (unit == NULL) {
        func_003003F0("warning: call evtLipsExecFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (motion >= mdlGetNodeRefHalf((MdlCtx *)model, 2)) {
        func_003003F0("warning: call evtLipsExecFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    evtConfigureUnitMotionSlot(unit, 2, motion, 0, 5, 1);
    D_003BBDB0 = id;
    func_003003F0("<lips %d %d> \n", id, motion);
}

void evtLipsStopFunction(void) {
    void *unit = NULL;
    EvtLipsModel *model = NULL;
    EvtLipsNode *node;

    if (D_003BBDB0 == 0) {
        return;
    }
    for (node = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head; node != NULL; node = node->next) {
        model = node->link->mh->model;
        if (sdfGetUniqueChunkValue(model->chunk) == D_003BBDB0) {
            unit = node->link->unit;
            break;
        }
    }
    if (unit == NULL) {
        func_003003F0("warning: call evtLipsStopFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (mdlGetNodeRefHalf((MdlCtx *)model, 2) == 0) {
        func_003003F0("warning: call evtLipsStopFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    evtConfigureUnitMotionSlot(unit, 2, 0, 0, 3, 2);
    func_003003F0("<lips_stop> stopunitid = %d\n", D_003BBDB0);
    D_003BBDB0 = 0;
}

u32 evtOpBeginWindowCallback(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 scriptParam1 = scrReadIntParameter(1);
        s32 scriptParam2 = scrReadIntParameter(2);
        evtStoreUnitMotionSlotSelection(unit, scriptParam1, scriptParam2);
        evtWindowMotionUnit = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        itfMesSetWindowCallbackAddress(window, evtInvokeStoredWindowMotion);
    }
    return 1;
}

u32 evtOpActivateUnitMotionOnWindowEvent(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 scriptParam1 = scrReadIntParameter(1);
        s32 scriptParam2 = scrReadIntParameter(2);
        evtStoreUnitMotionSlotSelection(unit, scriptParam1, scriptParam2);
        evtActivateStoredUnitMotionSlot((u32)unit);
        evtWindowMotionUnit = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        itfMesSetWindowCallbackAddress(window, evtInvokeStoredWindowMotion);
    }
    return 1;
}

u32 evtOpIsUnitMotionIdleOrTimed(void) {
    s32 id;
    EvtUnit *unit;
    u32 ret = 1;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit == NULL) {
        return ret;
    }
    return evtIsUnitMotionIdleOrTimedMode(unit) != 0;
}

u32 evtOpTestUnitMotionNodeFlag(void) {
    s32 id;
    EvtUnit *unit;
    s32 offset;
    u32 result = 1;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit == NULL) {
        return result;
    }
    offset = scrReadIntParameter(1);
    if (((unit->slotFlags[offset] & 1) & 0xFF) == 0) {
        return result;
    }
    return mdlCheckNodeByte30(unit->owner, scrReadIntParameter(1)) != 0;
}

u32 evtCmdDestroySelectedWorldUnit(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = dds3FindWorldObjectNodeByKey(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3RemoveWorldObjectNode(unit);
    return 1;
}

extern void evtUnitPrepareVerticalMoveSteps(EvtUnit *unit);

u32 evtOpStartUnitTransitionTowardWorldObject(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 mode = scrReadIntParameter(2);
        s32 objectId = scrReadIntParameter(1);
        s32 frames = scrReadIntParameter(3);
        s32 valueB6 = scrReadIntParameter(4);
        evtBeginUnitTransitionTowardWorldObject(unit, mode, objectId, -1, frames, valueB6, 0, 0);
    }
    if (scrReadIntParameter(2) == 1) {
        evtUnitPrepareVerticalMoveSteps(unit);
    }
    return 1;
}

u32 evtOpStartUnitPathFollow(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 objectId = scrReadIntParameter(1);
        s32 frames = scrReadIntParameter(5);
        s32 valueB6 = scrReadIntParameter(6);
        s32 mode = scrReadIntParameter(2);
        s32 dirFlag = scrReadIntParameter(4);
        s32 sideMode = scrReadIntParameter(3);
        evtSetUnitPathFollow(unit, objectId, frames, valueB6, mode, dirFlag, sideMode);
    }
    return 1;
}

u32 evtOpSetUnitTableEntry(void) {
    EvtUnit *unit;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 index = scrReadIntParameter(1);
        s32 value = scrReadIntParameter(2);
        unit->tableValues[index] = value;
    }
    return 1;
}

/* Store the script-supplied value for the selected event unit. */
u32 evtCommandSetUnitValue(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBC = scrReadIntParameter(1);
    return 1;
}

/* Store the two script-supplied world-unit state values. */
u32 evtCmdSetWorldUnitStatePair(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBE = scrReadIntParameter(1);
    unit->unkC0 = scrReadIntParameter(2);
    return 1;
}

u32 evtCmdPrepareUnitMotionAndLogState(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit == NULL) {
        return 1;
    }
    evtPrepareUnitMotionState(unit, scrReadIntParameter(1), 5, 7, 1);
    func_003003F0(D_003AC480);
    return 1;
}

u32 evtOpAttachUnitToWorldObject(void) {
    s32 id;
    EvtUnit *unit;
    s32 objectId;
    s32 frames;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    objectId = scrReadIntParameter(1);
    frames = scrReadIntParameter(2);
    evtAttachSecondaryWorldUnit(unit, objectId, frames);
    return 1;
}

f32 evtGetShortestAngleDelta(f32 fromDegrees, f32 toDegrees) {
    f32 difference;

    if (fromDegrees < 0.0f || toDegrees < 0.0f) {
        fromDegrees += 360.0f;
        toDegrees += 360.0f;
    }
    fromDegrees = (s32)fromDegrees % 360;
    toDegrees = (s32)toDegrees % 360;
    difference = fromDegrees - toDegrees;
    if (difference > 180.0f || difference < -180.0f) {
        if (fromDegrees < toDegrees) {
            fromDegrees += 360.0f;
        } else {
            toDegrees += 360.0f;
        }
    }
    return toDegrees - fromDegrees;
}

extern void effObjFetchInnerFirstVec(EffWorldNode *object);
extern void effObjFetchInnerSecondVecNorm(EffWorldNode *object);
extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);
extern f32 sdfAtan2(f32 y, f32 x);

u32 func_00224CD8(void) {
    EvtUnit *unit;
    EffWorldNode *actor;
    EffWorldNode *source;
    f32 *sourceVector;
    f32 rotation[4] __attribute__((aligned(16)));
    f32 actorPosition[4] __attribute__((aligned(16)));
    f32 sourcePosition[4] __attribute__((aligned(16)));
    f32 referenceAngle;
    f32 targetAngle;
    f32 angleDelta;
    s32 frames;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;

    actor = (EffWorldNode *)dds3FindWorldObjectNodeByKey(
        dds3GetWorldObject(), scrReadIntParameter(0), 5);
    if (actor == NULL) {
        return 1;
    }
    source = (EffWorldNode *)dds3FindWorldObjectNodeByKey(
        dds3GetWorldObject(), scrReadIntParameter(1), 0x11);
    if (source == NULL) {
        return 1;
    }

    sourceVector = (f32 *)source->data;
    effObjFetchInnerFirstVec(actor);
    VU0_STORE_VF(vf10, actorPosition);
    PCP_COPY_VECTOR_F32(sourcePosition, sourceVector);

    if ((unit->unkD8Flags & 1) == 0) {
        effObjFetchInnerSecondVecNorm(actor);
        referenceAngle = effMiscComputeQuaternionRotatedReferenceAngle();
        unit->unkD8Flags |= 1;
        unit->unkDC = -(referenceAngle * 57.29577637f);
    }

    targetAngle = sdfAtan2(actorPosition[0] - sourcePosition[0],
                           actorPosition[2] - sourcePosition[2]) * 57.32484055f;
    angleDelta = evtGetShortestAngleDelta(unit->unkDC, targetAngle);
    if (angleDelta < -135.0f) {
        targetAngle -= angleDelta + 135.0f;
        angleDelta = -135.0f;
    } else if (angleDelta > 135.0f) {
        targetAngle -= angleDelta - 135.0f;
        angleDelta = 135.0f;
    }
    targetAngle -= (angleDelta + angleDelta) / 3.0f;

    func_002E7F20(0.0f, targetAngle * 0.017453293f, 0.0f);
    /* First write to this output vector; the SDK store touches only it. */
    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);

    frames = scrReadIntParameter(2);
    if (frames >= 101) {
        frames = 100;
    }
    angleDelta = evtGetShortestAngleDelta(unit->unkDC, targetAngle);
    if (angleDelta < 0.0f) {
        angleDelta = -angleDelta;
    }
    if (angleDelta > 90.0f) {
        angleDelta = 90.0f;
    }
    frames = (frames * (s32)angleDelta) / 90;
    if (frames <= 0) {
        frames = 1;
    }
    if (frames > 100) {
        frames = 100;
    }

    evtBeginVectorTransition(unit, (s128 *)rotation, frames);
    evtArmEffectObjectPendingValue(actor, source->key);
    return 1;
}


INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224F48);

u32 evtUnitClearFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit != NULL) {
        unit->owner->flags &= ~1;
    }
    return 1;
}

u32 evtUnitSetFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    if (unit != NULL) {
        unit->owner->flags |= 1;
    }
    return 1;
}

u32 evtCommandFlagSelectedUnitStatus(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    evtSetUnitStatusFlags(unit);
    return 1;
}

u32 evtCommandBeginSelectedUnitTransition(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    param1 = scrReadIntParameter(1);
    evtConfigureUnitTransition(unit, param1);
    return 1;
}

u8 evtUnitHasNoStatusFlags(void) {
    s32 id;
    EvtUnit *unit;
    u8 active;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    active = evtTestUnitStatusFlags(unit);
    return active == 0;
}

u32 evtOpSetUnitGradientColors(void) {
    EvtUnit *unit;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;
    u32 scale;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(4), "vaddx.z vf10, vf0, vf2x");
    VU0_CLEAR_W(vf10);
    scale = 0x43000000;
    EE_MMI_RGBA_PACK_UNIT(packed1, scale);
    color1[0] = packed1;
    VU0_SCALAR_OP(bfWaitReadArgFloat(5), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(6), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(7), "vaddx.z vf10, vf0, vf2x");
    VU0_SET_W_ONE(vf10);
    EE_MMI_RGBA_PACK_UNIT(packed2, scale);
    color2[0] = packed2;
    evtInitializeUnitColorTransition(unit, scrReadIntParameter(1), packed1, packed2);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC480);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225330);

u32 evtOpSetUnitPackedRgbColor(void) {
    EvtUnit *unit;
    s32 color[4];
    u32 packed;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SET_AXIS_CLEAR_W(bfWaitReadArgFloat(4), z);
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    evtSetUnitRgbTransition(unit, scrReadIntParameter(1), packed);
    return 1;
}

u32 evtOpSetUnitPackedAlpha(void) {
    EvtUnit *unit;
    s32 color[4];
    u32 packed;

    unit = evtGetWorldUnitNestedValue(scrReadIntParameter(0));
    VU0_MOVE_VF(vf10, vf0);
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vmulx.w vf10, vf0, vf2x");
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    evtSetUnitAlphaTransition(unit, scrReadIntParameter(1), packed);
    return 1;
}

u32 evtOpSetUnitValueTransitionTarget(void) {
    s32 id;
    EvtUnit *unit;
    EffWorldNode *target;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    target = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(2));
    if (target == NULL) {
        return 1;
    }
    evtSetUnitValueTransition(unit, target, scrReadIntParameter(1));
    return 1;
}

u32 evtOpEndUnitValueTransition(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = evtGetWorldUnitNestedValue(id);
    param1 = scrReadIntParameter(1);
    evtEndUnitValueTransition(unit, param1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225620);

u32 evtOpResolveAndFlagObjectFromName(void) {
    u8 buf1[16];
    u8 buf2[16];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    param0 = scrReadStringParameter(0);
    unit = effObjSpawnLoadedResourceEffect(param0, buf1, buf2);
    if (unit == NULL) {
        func_003003F0(D_003AC520, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

u32 evtOpCreateAndFlagObjectFromResourceName(void) {
    f32 buf1[4];
    f32 buf2[4];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    buf2[3] = 1.0f;
    param0 = scrReadStringParameter(0);
    unit = effObjCreateFromResolvedResource(param0, buf1, buf2);
    if (unit == NULL) {
        func_003003F0(D_003AC520, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC508);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC520);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC550);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225880);

u32 func_00225958(void) {
    return 1;
}

u32 evtUnitCheckModelCut(void) {
    s32 id;
    void *unit;
    u32 param1;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        param1 = scrReadIntParameter(1);
        if (param1 >= 3U) {
            evtPrintDeveloperConsoleMessage(D_003AC588, param1);
            return 1;
        } else {
            effObjReplaceActiveEventNode(unit, param1);
        }
    }
    return 1;
}

f32 evtComputeClampedModelScale(s32 index) {
    f32 scale = (D_003BAA20[index].unk18 + D_003BAA20[index].unk1C * 0.5f) * 0.5f * D_003BAA20[index].unk10 * (1.0f / 70.0f);

    if (scale > 2.0f) {
        scale = 2.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

extern f32 evtComputeClampedModelScale(s32);
extern void effEventSetScale(void *, f32);

u32 evtOpSetModelCutAndScale(void) {
    void *unit;

    unit = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (unit != NULL) {
        s32 mode = scrReadIntParameter(1);
        s32 index;
        if ((u32)mode >= 3) {
            evtPrintDeveloperConsoleMessage(D_003AC588, mode);
            return 1;
        }
        effObjReplaceActiveEventNode(unit, mode);
        index = scrReadIntParameter(2);
        if (index >= 0) {
            f32 value = evtComputeClampedModelScale(index);
            void *target = *(void **)(*(u8 **)((u8 *)unit + 0x18) + 0x2C);
            if (target != NULL) {
                effEventSetScale(target, value);
            }
        }
    }
    return 1;
}

u32 evtCmdCreateEffectObjectFromResource(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = effObjCreateKindFromResource(1, param0);
    if (unit == NULL) {
        func_003003F0(D_003AC5B0, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC588);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC5B0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225BA0);

u32 evtScriptCreateEffectObjectFromResource(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = effObjCreateKindFromResource(2, param0);
    if (unit == NULL) {
        func_003003F0(D_003AC600, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        scrSetIntegerReturnValue(0);
    } else {
        effObjSetFlags(unit, 1);
        scrSetIntegerReturnValue(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC600);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225CD8);

u32 func_00225D80(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        effObjDispatchReadyState(obj);
    }
    return 1;
}

u32 func_00225DC0(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        effObjDispatchReadyState(obj);
    }
    return 1;
}

u32 evtScriptDestroyWorldEffectObject(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        dds3RemoveWorldObjectNode(obj);
    }
    return 1;
}

u32 evtOpSetModelObjectPosition(void) {
    EffWorldNode *obj;
    EvtModelHeader *header;

    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    header = (EvtModelHeader *)obj->data;
    if (header->flags & 4) {
        header->positionX = bfWaitReadArgFloat(1);
        header->positionY = bfWaitReadArgFloat(2);
        header->positionZ = bfWaitReadArgFloat(3);
    } else {
        obj->inner->position[0] = bfWaitReadArgFloat(1);
        obj->inner->position[1] = bfWaitReadArgFloat(2);
        obj->inner->position[2] = bfWaitReadArgFloat(3);
        obj->inner->flags = (obj->inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    }
    return 1;
}

u32 evtOpSetModelObjectRotationFromAngles(void) {
    f32 v[4];
    EffWorldNode *obj;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (!(((EvtModelHeader *)obj->data)->flags & 4)) {
        toRad = 0.017453293f;
        x = bfWaitReadArgFloat(1) * toRad;
        y = bfWaitReadArgFloat(2) * toRad;
        func_002E7F20(x, y, 0.0f);
        VU0_MOVE_VF(vf11, vf10);
        func_002E7F20(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(obj, v);
    }
    return 1;
}

u32 evtOpCopyModelTransformFromSource(void) {
    EffWorldNode *obj;
    EvtSourceObj *source;
    EvtSourceVec *vec;
    ObjectTransform *params;

    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (obj == NULL) {
        return 1;
    }
    source = evtFindWorldObjectByIdAndKind(0x11, scrReadIntParameter(1));
    if (source == NULL) {
        return 1;
    }
    vec = source->vec;
    if (!(((EvtModelHeader *)obj->data)->flags & 4)) {
        params = obj->inner;
        params->position[0] = vec->positionX;
        params->position[1] = vec->positionY;
        params->position[2] = vec->positionZ;
        params->rotation[0] = vec->rotationX;
        params->rotation[1] = vec->rotationY;
        params->rotation[2] = vec->rotationZ;
        params->rotation[3] = vec->rotationW;
    }
    obj->inner->flags = (obj->inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00222AC0", evtWindowMotionUnit);

INCLUDE_SDATA(const s32, "game/code_00222AC0", D_003BBDB0);

