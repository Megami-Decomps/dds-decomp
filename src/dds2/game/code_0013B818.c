#include "sdk_ps2.h"
#include "fld_area_work.h"
#include "fld_camp_check_state_api.h"
#include "fld_deferred_transition.h"
#include "common.h"
#include "sdf_vu0.h"
#include "fld_stop_entry.h"
#include "sdf_textured_rect.h"
#include "sdf_chip.h"
#include "sdf_dev_state.h"
#include "kwln.h"
#include "kwln_process_registration.h"
#include "sdf_resource.h"
#include "fld_waypoint.h"
#include "fld_npc_motion.h"
#include "fld_inf.h"
#include "fld_title_banner_menu.h"
#include "kwln_task_lifecycle.h"
#include "sdf_vu_matrix.h"
#include "kwln_fade.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "evt_world_source_transform.h"
#include "fld.h"
#include "fld_resource_resolver.h"
#include "fld_collision_geometry.h"
#include "evt_world.h"
#include "scr.h"
#include "sdf.h"
#include "sdf_texture_file.h"
#include "dds3_world_primary.h"
#include "mdl_flag_api.h"
#include "dds3_world_secondary.h"
#include "fld_trigger_shape.h"
#include "fld_room_state.h"
#include "fld_action_row.h"
#include "fld_actor_row.h"
#include "eff_object.h"
#include "evt_unit.h"
#include "mdl.h"
#include "fld_player_scene_state.h"
#include "mdl_world_object_source_vector_api.h"
#include "dds3_world_counter.h"
#include "fld_index_pair.h"
/* Declared before first use (were implicit int). */
extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);
extern void fldMarkStageExplorationBitmapRegion(s32, s32, s32, s32, s32, s32);
extern s32 fldRegisterRoomObject(FldFileResource *, dds3Basic_t *);
/* itfSetTextDrawLimit is void in the font TU; preserve C89 call scope here.
 * An early void declaration moves fldUpdateTitleBannerTask's position update
 * at +0x11C/+0x124/+0x12C from retail $3 to $2. */

extern struct dds3Basic_t *fldPlayerObject;
extern s32 D_003897C0[];
extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);
extern void datAddCurrencyClamped(s32 delta);
extern void scrDestroyAllNamedProcesses(void);
extern void evtSubmitEventRequestImmediate(s32 eventId);
extern dds3Basic_t *fldCamNowEveCamera(void);
extern void fldShowSceneModels(void);
extern void fldHideSceneModelsAndResetCamera(void);
extern void fldInitializeCameraViewFromPlayer(void);
extern void fldUpdateCameraViewFromPlayer(void);

extern FldInfTable D_0038E2D0;

/* Field command providers: declarations agree with their owning definitions. */
extern void fldDoorLoad(s32 field);

extern void fldSetDeferredFieldCommand(u32 command, u32 parameter);

extern void fldBgmFadeOut(void);

extern void fldSetFloorFlag(s32 area, s32 floor, s32 bit);

extern void fldSetSceneParam4(fldSceneParam_t *record,
    s32 stage, s32 kind, const char *name, s32 code, s32 link, const char *subname);

/* vu0 routine: normalize vf10 and return its original XYZ length. */
static inline f32 fldNormalizeProbeVector(void) {
    f32 length;

    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    return length;
}


extern void eglSetOt(u32 surfaceIndex);

extern void evtSubmitPrimaryGsTest();

extern void evtSubmitPrimaryAlphaBlendMode();

extern FldWallMeshRecord *fldItemBoxObjBuf;

extern FldValueRecord *fldMatterTable;

extern u32 fldAuxRecordResource;

extern u32 fldValueRecordResource;

extern s32 D_004361F8;


extern dds3ProcessID_t *fldRoomTaskHandles[];


extern s32 fldEveRngCount;

extern FldFileResource *fldRoomResources[];

extern void fldDrawPos(const f32 position[3]);

extern s32 gFldInf_HitIdx;

extern s32 gFldInf_WpExit;

extern s32 D_004361B8;

extern s32 fldActionRowCount;

extern s32 fldActorTaskRoom;

extern s32 fldEventSetRoom;

extern s32 fldLabelTaskRoom;

extern s32 fldLabelTaskSlot;

extern void fldResetActorSlots(void);

extern const char *fldFindActorEntryByName(const char *);

extern void fldPlayMenuSound(s32);

extern void kwlnFadeSetRGB(s32 red, s32 green, s32 blue);

extern void fldApplyPendingCameraHeading(void);

extern s32 fldApplyRoomObjectModeZero(s32 world, s32 stage, char *roomName, s32 mode);

extern void fldBeginNpcInteractionById(s32);

extern s32 D_003898FC[];

extern s32 D_00389780[];

extern s32 D_00399F60[];

extern u8 D_00399EA0[][16];


extern s32 fldProcAmChk(void);

extern s32 fileMenuTaskExists(void);

extern s32 fldHasKiretaLabelProcess(void);

extern s32 fldProcKiretaChk(void);

extern s32 fldProcHirakenaiChk(void);

extern u8 fldIsEventPhaseAtLeastTwo(void);

extern void fldKanjiDraw(s32 x, s32 y, u32 colors, const u8 *text);

extern s16 D_00444C68[];

extern u32 fldSelectedActorEntryIndex;

extern FldIndexPair D_0038A3B8[];

extern FldIndexPair D_0038A480[];

extern s32 D_004361E4;

extern s32 D_004361EC;

extern s32 D_004361F0;

extern s32 D_00435F28;

extern s32 D_00389784[];

extern const char D_00391E50[];

extern const char D_00391E6C[];

extern const char D_00391E88[];

extern const char D_00391EA4[];

extern const char D_00391EDC[];

extern const char D_00391EF8[];

extern const char D_00391F30[];

extern const char D_00391F4C[];

extern const char D_00391F68[];

extern const char D_00391F84[];

extern s32 fldPickedActorTemplateKind;


extern void fldMakeFilePath(char *, s32, s32);

extern s32 fldValueRecordCount;

extern s32 D_00436188;

extern s32 D_0043617C;

extern FldFileResource *fldDisplayValues[];

extern s32 gFldEveRngIdx_N;

extern f32 EulerAngleAxisY(void);

extern s32 gFldEveRngIdx;

extern s32 gFldEveRngIdx_T;

extern s32 fldTriggeredSavePointValue;

extern s32 fldEveHitState;

extern f32 fldVecDot(const f32 *, const f32 *);

extern FldRoomState fldEveHitTable[];

extern char D_00413448[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */

extern FldNpcMotion D_00391FA0[];

extern char *D_004361D4;

extern char *D_004361D8;

extern u32 D_004361DC;

extern s32 D_004361E0;

extern FldWaypointBlock fldActorWaypointRows;

extern FldActorEntry fldDoorTable[256];

/* Initialized by fldInitializeActorMotionSlots. */
extern FldActorRow fldActorSlots[];

extern u8 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit);

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value);

/* Build a normalized plane from three vertices spaced four floats apart. * Snap near-zero normal components before deriving D from the third vertex. * Degenerate triangles retain the native divide-by-zero behavior. */ void fldMakeEveHitPlanes(const f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant);

void evtMessageExtraActionNoOp(void);

/* Reset room records and selection sentinels, then apply actor-slot defaults. */
void fldResetZoneRecordsAndActorSlots(void) {
    s32 roomIndex;
    s32 vectorIndex;

    fldEveRngCount = 0;
    D_004361B8 = 0;
    fldEveHitState = 0;
    for (roomIndex = 0; roomIndex < FIELD_ROOM_RECORD_COUNT; roomIndex++) {
        for (vectorIndex = 0; vectorIndex < FIELD_ROOM_CORNER_COUNT; vectorIndex++) {
            fldEveHitTable[roomIndex].corner[vectorIndex][0] = 0.0f;
            fldEveHitTable[roomIndex].corner[vectorIndex][1] = 0.0f;
            fldEveHitTable[roomIndex].corner[vectorIndex][2] = 0.0f;
            fldEveHitTable[roomIndex].corner[vectorIndex][3] = 1.0f;
        }
        fldEveHitTable[roomIndex].center[0] = 0.0f;
        fldEveHitTable[roomIndex].center[1] = 0.0f;
        fldEveHitTable[roomIndex].center[2] = 0.0f;
        fldEveHitTable[roomIndex].center[3] = 1.0f;
        for (vectorIndex = 0; vectorIndex < FIELD_ROOM_PLANE_COUNT; vectorIndex++) {
            fldEveHitTable[roomIndex].plane[vectorIndex][0] = 0.0f;
            fldEveHitTable[roomIndex].plane[vectorIndex][1] = 0.0f;
            fldEveHitTable[roomIndex].plane[vectorIndex][2] = 0.0f;
            fldEveHitTable[roomIndex].plane[vectorIndex][3] = 1.0f;
            fldEveHitTable[roomIndex].limit[vectorIndex] = 0.0f;
        }
        /* Retain the native word clear in this register-sensitive reset loop. */
        *(u32 *)&fldRoomTaskHandles[roomIndex] = 0;
        fldEveHitTable[roomIndex].worldObjectKey = 0;
        fldEveHitTable[roomIndex].sizeX = 0.0f;
        fldEveHitTable[roomIndex].sizeY = 0.0f;
        fldEveHitTable[roomIndex].sizeZ = 0.0f;
        fldEveHitTable[roomIndex].unk118 = 0.0f;
        fldEveHitTable[roomIndex].unk11C = 0.0f;
        fldEveHitTable[roomIndex].unk130 = 0;
        fldEveHitTable[roomIndex].roomId = -1;
        fldEveHitTable[roomIndex].mapSlot = -1;
        fldEveHitTable[roomIndex].mode = FLD_ROOM_INTERACTION_NONE;
        fldEveHitTable[roomIndex].entryIndex = -1;
        fldEveHitTable[roomIndex].savePointValue = -1;
        fldEveHitTable[roomIndex].axisMode = 0;
        fldEveHitTable[roomIndex].crossingDirection = 0;
    }
    fldActorTaskRoom = -1;
    fldEventSetRoom = -1;
    fldLabelTaskRoom = -1;
    fldLabelTaskSlot = -1;
    gFldEveRngIdx = -1;
    gFldEveRngIdx_N = -1;
    gFldEveRngIdx_T = -1;
    fldActionRowCount = 0;
    fldResetActorSlots();
}

/* Clear slot handles and destroy named tasks reached through linked display values. */
void fldEveHit_End(void) {
    s32 slotIndex;
    dds3Basic_t *world;
    const char *task;
    FldTaskInfo *taskInfo;

    fldEveHitState = 1;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        fldRoomTaskHandles[slotIndex] = NULL;
    }
    gFldEveRngIdx = -1;
    gFldEveRngIdx_N = -1;
    gFldEveRngIdx_T = -1;
    world = dds3GetControlScene();
    if (world != 0) {
        for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
            taskInfo = (FldTaskInfo *)fldRoomResources[slotIndex]->data;
            if (taskInfo->slot >= 0) {
                task = dds3EventBasicGetResource(dds3SearchSceneBasic(world, fldDisplayValues[taskInfo->slot]->id, 0xD));
                if (scrSearchScriptName(task) != 0) {
                    dds3SceneBasicEndScript(dds3GetActiveScene(), task);
                }
            }
        }
    }
}

#include "fld_event_display_resource_api.h"

/* Append a file resource for an event task and return its index. */
FldEventDisplayResourceIndex fldEveHit_AddEveData(
    FldFileResource *resource, dds3Basic_t *unusedObject) {
    s32 index = D_004361B8;
    fldDisplayValues[index] = resource;
    D_004361B8 = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "game/code_0013B818", fldRegisterRoomObject);

/* vu0 routine: actor-facing probe for the world kind-0x11 position payload. */
s32 fldTestRoomProbeFacingAndRange(dds3Basic_t *actor, dds3Basic_t *entry) {
    f32 dir[4];
    f32 position[4];
    f32 length;
    f32 dot;
    f32 angle;
    EvtWorldSourceTransformPrefix *source;
    s32 i;
    u32 kind;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    for (i = 0; i < fldEveRngCount; i++) {
        if (fldEveHitTable[i].worldObjectKey == entry->key) {
            kind = ((FldTaskInfo *)fldRoomResources[i]->data)->shape->kind;
            switch (kind) {
            case 0:
                source = entry->data;
                position[0] = source->position.x;
                position[1] = source->position.y;
                position[2] = source->position.z;
                SDF_VU0_LOADVECTOR(actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_STORE_ROW2(dir);
                dir[1] = 0.0f;
                SDF_VU0_LOADVECTOR(dir);
                SDF_VU0_NORMALIZE();
                VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_LOADVECTOR_B(actor->inner->position);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(dir);
                dir[1] = 0.0f;
                SDF_VU0_LOADVECTOR(dir);
                length = fldNormalizeProbeVector();
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_INNERPRODUCT(dot);
                angle = dot * 180.0f / 3.14f;
                if (angle < 0.0f) {
                    return 0;
                }
                if (200.0f < length) {
                    return 0;
                }
                return 1;
            case 1:
                return fldEveHitTable[i].crossingDirection == kind;
            case 2:
                SDF_VU0_LOADVECTOR(actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_STORE_ROW2(dir);
                dir[1] = 0.0f;
                SDF_VU0_LOADVECTOR(dir);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(dir);
                dot = fldVecDot(dir, fldEveHitTable[i].center);
                if (dot < 0.5f) {
                    return 0;
                }
                return 1;
            }
        }
    }
    return 0;
}

/* vu0 routine: the alternate entry probe only constrains facing, not range. */
s32 fldTestRoomProbeFacing(dds3Basic_t *actor, dds3Basic_t *entry) {
    f32 dir[4];
    f32 position[4];
    EvtWorldSourceTransformPrefix *source;
    f32 dot;
    f32 angle;
    s32 i;
    u32 kind;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    for (i = 0; i < fldEveRngCount; i++) {
        if (fldEveHitTable[i].worldObjectKey == entry->key) {
            kind = ((FldTaskInfo *)fldRoomResources[i]->data)->shape->kind;
            switch (kind) {
            case 0:
                source = entry->data;
                SDF_VU0_COPYVECTOR(position, &source->position);
                SDF_VU0_LOADVECTOR(actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_STORE_ROW2(dir);
                dir[1] = 0.0f;
                SDF_VU0_LOADVECTOR(dir);
                SDF_VU0_NORMALIZE();
                VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_LOADVECTOR_B(actor->inner->position);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(dir);
                dir[1] = 0.0f;
                SDF_VU0_LOADVECTOR(dir);
                fldNormalizeProbeVector();
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_INNERPRODUCT(dot);
                angle = dot * 180.0f / 3.14f;
                if (angle < 0.0f) {
                    return 0;
                }
                return 1;
            case 1:
                return fldEveHitTable[i].crossingDirection == kind;
            case 2:
                SDF_VU0_LOADVECTOR(actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_STORE_ROW2(dir);
                dir[1] = 0.0f;
                SDF_VU0_LOADVECTOR(dir);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(dir);
                dot = fldVecDot(dir, fldEveHitTable[i].center);
                if (dot < 0.5f) {
                    return 0;
                }
                return 1;
            }
        }
    }
    return 0;
}

s32 fldTestActorRoomProbeCondition(s32 roomIndex, dds3Basic_t *actor, f32 *position) {
    f32 dir[4];
    f32 length;
    f32 dot;
    f32 angle;
    u32 kind;

    kind = ((FldTaskInfo *)fldRoomResources[roomIndex]->data)->shape->kind;
    switch (kind) {
    case 0:
        SDF_VU0_LOADVECTOR(actor->inner->rotation);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_STORE_ROW2(dir);
        dir[1] = 0.0f;
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_NORMALIZE();
        VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_LOADVECTOR_B(actor->inner->position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(dir);
        dir[1] = 0.0f;
        SDF_VU0_LOADVECTOR(dir);
        length = fldNormalizeProbeVector();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_INNERPRODUCT(dot);
        angle = dot * 180.0f / 3.14f;
        if (angle < 0.0f) {
            return 0;
        }
        if (200.0f < length) {
            return 0;
        }
        return 1;
    case 1:
        return fldEveHitTable[roomIndex].crossingDirection == kind;
    case 2:
        SDF_VU0_LOADVECTOR(actor->inner->rotation);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_STORE_ROW2(dir);
        dir[1] = 0.0f;
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(dir);
        dot = fldVecDot(dir, fldEveHitTable[roomIndex].center);
        if (dot < 0.5f) {
            return 0;
        }
        return 1;
    }
    return 0;
}

s32 fldProbeRoomPlanes(f32 *direction, s32 roomIndex) {
    f32 probe[3];
    f32 planar[2];
    s32 i;

    if (fldEveHitTable[roomIndex].axisMode == 0) {
        probe[0] = 0.0f;
        probe[1] = direction[1];
        probe[2] = direction[2];
        planar[0] = direction[1];
        planar[1] = direction[2];
    } else {
        probe[0] = direction[0];
        probe[1] = direction[1];
        probe[2] = 0.0f;
        planar[0] = direction[0];
        planar[1] = direction[1];
    }
    for (i = 0; i < 4; i++) {
        if (fldVecDot(probe, fldEveHitTable[roomIndex].plane[i + 1]) - fldEveHitTable[roomIndex].limit[i + 1] < 0.0f) {
            return -1;
        }
    }
    return 0;
}

s32 fldRoomContainsPoint(f32 *direction, s32 index) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (fldVecDot(direction, fldEveHitTable[index].plane[i]) - fldEveHitTable[index].limit[i] > 0.0f) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0013B818", fldStartRoomEventTask);

s32 fldEveHit_ScriptEnd(char *flag, u32 slot) {
    fldRoomTaskHandles[slot] = NULL;
    if (scrSearchScriptName(flag) != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), flag);
        return 0;
    }
    return -1;
}

u32 fldEveHit_ScriptEnd2(u32 index) {
    dds3ProcessID_t **taskSlot = &fldRoomTaskHandles[index];

    if (dds3ExistProcess(*taskSlot) != KWLN_PROCESS_NOT_REGISTERED) {
        dds3KillProcess(*taskSlot, 0);
    }
    *taskSlot = NULL;
    return 0;
}

extern s8 D_00387D60[];

extern u8 D_00435F24;

extern const char *fldSelectCurrentActorOnNextFloor(void);

s32 fldEveHit_ChkEveHitAfter(void) {
    const char *object;
    u32 state = D_00435F24;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && D_00387D60[0] != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), (const char *)D_00387D60);
    }
    D_00387D60[0] = 0;
    D_00435F24 = 0;
    object = fldSelectCurrentActorOnNextFloor();
    if (scrSearchScriptName(object) == 0) {
        evtStartSceneResourceTask(dds3GetActiveScene(), object);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0013B818", fldUpdateRoomTriggers);

void fldDrawTaskMarkers(void) {
    s32 count = fldEveRngCount;
    s32 i = 0;
    if (count > 0) {
        FldFileResource **entry = fldRoomResources;
        do {
            fldDrawPos((*entry)->transform);
            i++;
            entry++;
        } while (i < fldEveRngCount);
    }
}

/* Clear unregistered task handles; retain the native global-count loop bound. */
void fldPurgeEveRngProcesses(void) {
    s32 count = fldEveRngCount;
    s32 slotIndex = 0;
    if (count > 0) {
        dds3ProcessID_t **taskSlot = fldRoomTaskHandles;
        do {
            if (dds3ExistProcess(*taskSlot) == KWLN_PROCESS_NOT_REGISTERED) {
                *taskSlot = NULL;
            }
            slotIndex++;
            taskSlot++;
        } while (slotIndex < fldEveRngCount);
    }
}

/* Return the first matching task slot's record ID; -1 denotes a miss. */
s32 fldEveHitResID_Find(dds3ProcessID_t *task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (fldRoomTaskHandles[slotIndex] == task) {
            return fldRoomResources[slotIndex]->id;
        }
    }
    return -1;
}

/* Return the room ID stored for the matching task; -1 denotes a miss. */
s16 fldFindRoomIdByTask(dds3ProcessID_t *task) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (fldRoomTaskHandles[slotIndex] == task) {
            return fldEveHitTable[slotIndex].roomId;
        }
    }
    return -1;
}

/* Return the first matching task slot's record name; NULL on miss. */
const char *fldEveHitResName_Find(dds3ProcessID_t *task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (fldRoomTaskHandles[slotIndex] == task) {
            return fldRoomResources[slotIndex]->name;
        }
    }
    return 0;
}


extern FldActionRow fldActionRows[];

extern const char D_0038BC00[];

extern char fldHitProgramName[];

extern const char D_0038BC20[];

static inline void fldInitializeActionTransform(FldActionTransformFields *transform, f32 x, f32 y, f32 z) {
    transform->position[3] = 1.0f;
    transform->scale[0] = 1.0f;
    transform->scale[1] = 1.0f;
    transform->scale[2] = 1.0f;
    transform->scale[3] = 1.0f;
    transform->position[0] = x;
    transform->position[1] = y;
    transform->position[2] = z;
    transform->rotation[0] = 0.0f;
    transform->rotation[1] = 0.0f;
    transform->rotation[2] = 0.0f;
    transform->rotation[3] = 0.0f;
}

extern dds3Basic_t *dds3AddEventBasic(s32 key, const char *resourcePath, const char *name);

void fldSpawnRoomTrigger(s32 roomId, f32 x, f32 y, f32 z, f32 triggerRadius) {
    FldActionTransformFields *transform;
    dds3Basic_t *object;
    dds3Basic_t *child;
    s32 index;
    s32 roomIndex;
    u32 counter;
    s32 slot;

    index = fldActionRowCount;
    counter = dds3AllocObjectId();
    object = dds3AddEventBasic(counter, D_0038BC20, D_0038BC00);
    fldActionRows[index].displayResource.id = counter;
    slot = fldEveHit_AddEveData(&fldActionRows[index].displayResource, object);
    fldActionRows[index].prefix.task.shape.radius = triggerRadius;
    fldActionRows[index].prefix.shape.kind = 0;
    fldActionRows[index].prefix.task.info.unk0 = 1;
    fldActionRows[index].prefix.task.info.slot = slot;
    fldActionRows[index].prefix.task.info.unk08 = 1;
    fldActionRows[index].prefix.task.info.shape = &fldActionRows[index].prefix.shape;
    transform = &fldActionRows[index].transform.fields;
    fldInitializeActionTransform(transform, x, y, z);
    fldActionRows[index].childResource.transform = transform->position;
    fldActionRows[index].childResource.data = &fldActionRows[index].prefix.task.info;
    fldActionRows[index].childResource.id = dds3AllocObjectId();
    fldActionRows[index].childResource.name = D_0038BC00;
    child = dds3AddPointBasic(fldActionRows[index].childResource.id, (EvtWorldSourceTransformPrefix *)fldActionRows[index].childResource.transform, fldHitProgramName);
    roomIndex = fldRegisterRoomObject(&fldActionRows[index].childResource, child);
    fldEveHitTable[roomIndex].roomId = roomId;
    fldEveHitTable[roomIndex].mode = FLD_ROOM_INTERACTION_ITEM_BOX;
    fldEveHitTable[roomIndex].entryIndex = -1;
    fldActionRowCount++;
}

INCLUDE_ASM(const s32, "game/code_0013B818", fldSpawnNpcTrigger);

extern const char D_0038BC30[];

extern const char D_0038BC40[];

void fldSpawnRoomTriggerVariant(s32 kind, s32 roomValue,
                                f32 x, f32 y, f32 z, f32 radius) {
    s32 index = fldActionRowCount;
    FldActionTransform *transform;
    s32 worldKey = dds3AllocObjectId();
    dds3Basic_t *object;
    s32 taskIndex;

    if (kind == 0) {
        object = dds3AddEventBasic(worldKey, D_0038BC40, D_0038BC00);
    } else {
        object = dds3AddEventBasic(worldKey, D_0038BC30, D_0038BC00);
    }
    fldActionRows[index].displayResource.id = worldKey;
    taskIndex = fldEveHit_AddEveData(
        &fldActionRows[index].displayResource, object);
    fldActionRows[index].prefix.task.shape.radius = radius;
    fldActionRows[index].prefix.task.shape.kind = 0;
    fldActionRows[index].prefix.task.info.unk0 = 1;
    fldActionRows[index].prefix.task.info.slot = taskIndex;
    fldActionRows[index].prefix.task.info.unk08 = 1;
    fldActionRows[index].prefix.task.info.shape = (FldTriggerShape *)&fldActionRows[index].prefix.task.shape;
    fldActionRows[index].childResource.data = &fldActionRows[index].prefix.task.info;
    transform = &fldActionRows[index].transform;
    transform->position[3] = 1.0f;
    transform->scale[0] = 1.0f;
    transform->scale[1] = 1.0f;
    transform->scale[2] = 1.0f;
    transform->scale[3] = 1.0f;
    transform->position[0] = x;
    transform->position[1] = y;
    transform->position[2] = z;
    fldActionRows[index].childResource.transform = transform->position;
    transform->rotation[1] = 0.0f;
    transform->rotation[2] = 0.0f;
    transform->rotation[3] = 0.0f;
    transform->rotation[0] = 0.0f;
    fldActionRows[index].childResource.id = dds3AllocObjectId();
    fldActionRows[index].childResource.name = D_0038BC00;
    object = dds3AddPointBasic(fldActionRows[index].childResource.id,
                                 (EvtWorldSourceTransformPrefix *)fldActionRows[index].childResource.transform,
                                 fldHitProgramName);
    taskIndex = fldRegisterRoomObject(&fldActionRows[index].childResource, object);
    if (kind == 0) {
        fldEveHitTable[taskIndex].mode = FLD_ROOM_INTERACTION_SAVE_POINT;
        fldEveHitTable[taskIndex].savePointValue = roomValue;
    } else {
        fldEveHitTable[taskIndex].mode = FLD_ROOM_INTERACTION_HEAL_POINT;
    }
    fldActionRowCount++;
    fldEveHitTable[taskIndex].entryIndex = -1;
}

extern s32 D_004361C0;

s32 fldCheckEntryActive(s32 entry) {
    s32 blocked = D_004361C0;
    s32 index = gFldEveRngIdx_N;

    if (blocked != 0) {
        return 1;
    }
    if (index == -1) {
        return 0;
    }
    if (fldEveHitTable[index].mode == FLD_ROOM_INTERACTION_NPC &&
        fldEveHitTable[index].entryIndex == entry) {
        return 1;
    }
    return 0;
}

/* Test for any nonzero slot handle, without consulting task registration. */
s32 fldCheckEveRngEncount(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (fldRoomTaskHandles[slotIndex] != NULL) {
            return 1;
        }
    }
    return 0;
}

s32 fldGetCurrentSceneSelectionId(void) {
    s32 index = gFldInf_HitIdx;

    if (index < 0) {
        return -1;
    }
    return D_00444C68[index * 160];
}

extern s32 D_004361C8;

extern const char D_0038E290[];


const char *fldCheckNpcTalk(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (D_0038E2D0.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (D_0038E2D0.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, D_0038E2D0.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = D_0038E2D0.packs[packOrSetIndex].setIndex;
                    if (D_0038E2D0.sets[D_004361C8].kindArea.packed == 1) {
                        D_004361C8 = candidateSet;
                        return D_0038E290;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (D_0038E2D0.sets[packOrSetIndex].kindArea.packed == 1 &&
            D_0038E2D0.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, D_0038E2D0.sets[packOrSetIndex].eventName) == 0) {
            D_004361C8 = packOrSetIndex;
            return D_0038E290;
        }
    }
    return 0;
}

const char *fldCheckNpcTalk2(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (D_0038E2D0.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (D_0038E2D0.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, D_0038E2D0.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = D_0038E2D0.packs[packOrSetIndex].setIndex;
                    if (D_0038E2D0.sets[D_004361C8].kindArea.packed == 0) {
                        D_004361C8 = candidateSet;
                        return D_0038E290;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (D_0038E2D0.sets[packOrSetIndex].kindArea.packed == 0 &&
            D_0038E2D0.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, D_0038E2D0.sets[packOrSetIndex].eventName) == 0) {
            D_004361C8 = packOrSetIndex;
            return D_0038E290;
        }
    }
    return 0;
}

const char *fldInf_FindTypeEveNpc(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (D_0038E2D0.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (D_0038E2D0.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, D_0038E2D0.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    D_004361C8 = D_0038E2D0.packs[packOrSetIndex].setIndex;
                    return D_0038E290;
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if ((u16)D_0038E2D0.sets[packOrSetIndex].kindArea.packed < 2 &&
            D_0038E2D0.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, D_0038E2D0.sets[packOrSetIndex].eventName) == 0) {
            D_004361C8 = packOrSetIndex;
            return D_0038E290;
        }
    }
    return 0;
}

extern s32 fldGetActorStat0(s32 attribute);

/* Follow the INF flag/message graph to its terminal event or actor action.
 * Linked selectors recurse in tail position; the native routine loops without
 * growing the stack. Targets and eventHit are signed serialized bytes. */
INCLUDE_RODATA(const s32, "game/code_0013B818", D_004133A0);

s32 fldResolveEventSelectorTarget(s32 selector) {
    s32 target;
    s32 index;

    if (selector < 10) {
        index = selector - 1;
        if (D_0038E2D0.sets[D_004361C8].flags[index].flag == 0) {
            target = (s8)D_0038E2D0.sets[D_004361C8].flags[index].offTarget;
        } else if (evtBitCheck(D_0038E2D0.sets[D_004361C8].flags[index].flag)) {
            target = (s8)D_0038E2D0.sets[D_004361C8].flags[index].onTarget;
        } else {
            target = (s8)D_0038E2D0.sets[D_004361C8].flags[index].offTarget;
        }
    } else {
        index = selector - 10;
        target = (s8)D_0038E2D0.sets[D_004361C8].messages[index].targets[0];
    }
    switch (target) {
    case 0:
        return 0;
    case 100:
        fldInf_FindTypeEveNpc(fldRoomResources[gFldInf_HitIdx]->name);
        switch ((s8)D_0038E2D0.sets[D_004361C8].eventHit) {
        case 0: return 199;
        case 4: return 104;
        case 5: return 105;
        case 6: return 106;
        case 7: return 107;
        }
        fldFindActorEntryByName(fldRoomResources[gFldInf_HitIdx]->name);
        switch (D_004361F8) {
        case 0:
        case 1:
        case 2:
        case 4: return 100;
        case 3: return 102;
        case 5:
            if (fldGetActorStat0(0) == 0) {
                return 104;
            } else {
                return 105;
            }
        case 6: return 106;
        case 7: return 107;
        }
        return target;
    case 101: return target;
    case 102: return target;
    case 103: return target;
    case 104: return target;
    case 105: return target;
    default:
        if (target < 10) {
            return fldResolveEventSelectorTarget(target);
        }
        index = target - 10;
        if (D_0038E2D0.sets[D_004361C8].messages[index].kind == -1) {
            return fldResolveEventSelectorTarget(target);
        }
        if ((u16)D_0038E2D0.sets[D_004361C8].messages[index].kind < 2) {
            switch ((s8)D_0038E2D0.sets[D_004361C8].eventHit) {
            case 0: return 199;
            case 1: return 2;
            case 2: return 100;
            case 3: return 1;
            case 4: return 104;
            case 5: return 105;
            case 6: return 106;
            case 7: return 107;
            }
        }
        return 0;
    }
}

/* Apply a selected INF graph branch and its view and extra actions. */
s32 fldDispatchInteractionAction(s32 messageId, s32 slot) {
    s32 row;
    s8 target;
    s8 extraAction = 0;
    s32 viewIndex = 0;
    dds3Basic_t *world;
    dds3Basic_t *node;
    dds3Basic_t *camera;
    evtUnitHandle_t *unit;

    if (messageId < 10) {
        row = messageId - 1;
        if (D_0038E2D0.sets[D_004361C8].flags[row].flag == 0) {
            target = (s8)D_0038E2D0.sets[D_004361C8].flags[row].offTarget;
        } else if (evtBitCheck(D_0038E2D0.sets[D_004361C8].flags[row].flag) != 0) {
            target = (s8)D_0038E2D0.sets[D_004361C8].flags[row].onTarget;
        } else {
            target = (s8)D_0038E2D0.sets[D_004361C8].flags[row].offTarget;
        }
    } else {
        row = messageId - 10;
        target = (s8)D_0038E2D0.sets[D_004361C8].messages[row].targets[slot];
    }
    if (target >= 10) {
        if (D_0038E2D0.sets[D_004361C8].messages[target - 10].flags[0] != 0) {
            evtBitOff(D_0038E2D0.sets[D_004361C8].messages[target - 10].flags[0]);
        }
        if (D_0038E2D0.sets[D_004361C8].messages[target - 10].flags[1] != 0) {
            evtBitOn(D_0038E2D0.sets[D_004361C8].messages[target - 10].flags[1]);
        }
    }
    world = dds3GetActiveScene();
    if (target >= 10) {
        extraAction = (s8)D_0038E2D0.sets[D_004361C8].messages[target - 10].extraAction;
    }
    if (extraAction != 0) {
        if ((s16)D_0038E2D0.extraActions[extraAction].action == 1) {
            evtMessageExtraActionNoOp();
        } else if ((s16)D_0038E2D0.extraActions[extraAction].action == 2) {
            ptyAdjustItemQuantity(D_0038E2D0.extraActions[extraAction].parameter0,
                                  D_0038E2D0.extraActions[extraAction].parameter1);
        } else if ((s16)D_0038E2D0.extraActions[extraAction].action == 3) {
            datAddCurrencyClamped(D_0038E2D0.extraActions[extraAction].parameter0);
        }
    }
    if (target >= 10) {
        viewIndex = D_0038E2D0.sets[D_004361C8].messages[target - 10].view & 0x7F;
    }
    if (viewIndex != 0) {
        unit = ((EffectObjectData *)(fldPlayerObject)->data)->transitionWork;
        if (unit != NULL) {
            if (D_0038E2D0.views[viewIndex].player != 0) {
                fldShowSceneModels();
            } else {
                fldHideSceneModelsAndResetCamera();
            }
        }
        if (D_0038E2D0.views[viewIndex].positionName[0] != 0) {
            node = dds3SceneBasicSearchName(world, (const u8 *)D_0038E2D0.views[viewIndex].positionName);
            if (node != NULL) {
                evtSetUnitPos((fldPlayerObject)->key, node->key);
            }
        }
        if (D_0038E2D0.views[viewIndex].cameraName[0] != 0) {
            node = dds3SceneBasicSearchName(world, (const u8 *)D_0038E2D0.views[viewIndex].cameraName);
            if (node != NULL) {
                camera = dds3SearchSceneBasic(world, node->key, 4);
                if (camera != NULL) {
                    dds3SetActiveCameraSceneBasic(world, camera);
                }
            }
        } else if (fldCamNowEveCamera() != 0) {
            dds3SetActiveCameraSceneBasic(world, fldGetPlayerSceneState());
            switch (D_003897C0[0]) {
            case 0:
            case 3:
                fldInitializeCameraViewFromPlayer();
                break;
            case 1:
                fldUpdateCameraViewFromPlayer();
                break;
            case 2:
            case 4:
                break;
            }
        }
    }
    if (target == 0) {
        if ((s8)D_0038E2D0.sets[D_004361C8].messages[row].extraAction != 0) {
            if ((s16)D_0038E2D0.extraActions[(s8)D_0038E2D0.sets[D_004361C8].messages[row].extraAction].action == 4) {
                s32 command;
                s32 flag;

                scrDestroyAllNamedProcesses();
                command = D_0038E2D0.extraActions[(s8)D_0038E2D0.sets[D_004361C8].messages[row].extraAction].parameter0;
                flag = D_0038E2D0.extraActions[(s8)D_0038E2D0.sets[D_004361C8].messages[row].extraAction].parameter1;
                fldSetDeferredFieldCommand(fldGlobalBase.area, command);
                evtSubmitEventRequestImmediate(command);
                if (flag == 1) {
                    fldGlobalBase.titleFade = flag;
                }
                return command + 200;
            }
        }
        /* The action callbacks can replace the player's transition work. */
        unit = ((EffectObjectData *)(fldPlayerObject)->data)->transitionWork;
        if (unit != NULL) {
            unit->owner->flags &= ~1;
        }
        if (fldCamNowEveCamera() != 0) {
            switch (D_003897C0[0]) {
            case 0:
            case 3:
                fldInitializeCameraViewFromPlayer();
                break;
            case 1:
                fldUpdateCameraViewFromPlayer();
                break;
            case 2:
            case 4:
                break;
            }
        }
        return target;
    }
    return target;
}

extern s32 D_004361C8;

s32 fldGetActorSlotAttribute(s32 actorId, FldInfMessageAttribute attribute) {
    s32 index = actorId - 10;
    s32 attributeIndex = (s32)attribute;
    s32 value = 0;

    switch (attributeIndex) {
    case FLD_INF_MESSAGE_ATTRIBUTE_KIND:
        value = D_0038E2D0.sets[D_004361C8].messages[index].kind;
        break;
    case FLD_INF_MESSAGE_ATTRIBUTE_MESSAGE:
        value = D_0038E2D0.sets[D_004361C8].messages[index].message;
        break;
    case FLD_INF_MESSAGE_ATTRIBUTE_VIEWED:
        if (D_0038E2D0.sets[D_004361C8].messages[index].view & 0x80) {
            value = 1;
        }
        break;
    }
    return value;
}

void fldInfLoad(s32 field) {
    char path[64];
    char directory[32];
    sdfFileCmdHandle_t *command;
    if (field < 200) {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, D_00413448, directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &D_0038E2D0, sizeof(D_0038E2D0));
        sdfFileClose(command);
    }
}

void fldInfLoadFromMem(const FldInfTable *source) {
    D_0038E2D0 = *source;
}

void fldInfToWAP(void) {
    s32 slot = gFldInf_HitIdx;
    const char *actor;

    gFldInf_WpExit = 1;
    actor = fldFindActorEntryByName(fldRoomResources[slot]->name);
    if (scrSearchScriptName(actor) == NULL) {
        fldRoomTaskHandles[slot] = evtStartSceneResourceTask(dds3GetActiveScene(), actor);
    }
}

/* Resolve a named actor in the current area to its template; the selected
 * template kind is also recorded for the caller in fldPickedActorTemplateKind. */
INCLUDE_RODATA(const s32, "game/code_0013B818", D_00413448);

const char *fldPickActorTemplateByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = &fldDoorTable[i];
        flag = entry->requiredFlag;
        if ((flag == 0 || evtBitCheck(flag) != 0)
            && entry->floor == fldGlobalBase.floor + 1
            && strcmp(name, entry->name) == 0) {
            switch (entry->kind) {
            case 1:
                if (fldGlobalBase.specialEventActive != 0 && (entry->flags31 & 4)) {
                    fldPickedActorTemplateKind = 0xB;
                    return D_00391F84;
                }
                if (entry->variantMode == 0) {
                    sub = entry->variant;
                    if (sub != 0) {
                        if (sub == 1) {
                            fldPickedActorTemplateKind = 6;
                            return D_00391EF8;
                        }
                    }
                }
                fldPickedActorTemplateKind = 0;
                return D_00391E50;
            case 2:
                fldPickedActorTemplateKind = 1;
                return D_00391E6C;
            case 3:
                fldPickedActorTemplateKind = 2;
                return D_00391E88;
            case 4:
                fldPickedActorTemplateKind = 3;
                return D_00391EA4;
            case 5:
                fldPickedActorTemplateKind = 5;
                return D_00391EDC;
            case 10:
                fldPickedActorTemplateKind = 8;
                return D_00391F30;
            case 11:
                fldPickedActorTemplateKind = 9;
                return D_00391F4C;
            case 12:
                fldPickedActorTemplateKind = 0xA;
                return D_00391F68;
            }
        }
        i++;
    } while (i < 0x100);
    fldPickedActorTemplateKind = -1;
    return 0;
}

/* Like the template lookup, but also records the matching actor slot and
 * the kind-specific scene state before returning its template. */
const char *fldFindActorEntryByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = &fldDoorTable[i];
        flag = entry->requiredFlag;
        if ((flag == 0 || evtBitCheck(flag) != 0)
            && entry->floor == fldGlobalBase.floor + 1
            && strcmp(name, entry->name) == 0) {
            fldSelectedActorEntryIndex = i;
            switch (entry->kind) {
            case 1:
                if (fldGlobalBase.specialEventActive != 0 && (entry->flags31 & 4)) {
                    fldPickedActorTemplateKind = 0xB;
                    return D_00391F84;
                }
                if (entry->variantMode == 0) {
                    sub = entry->variant;
                    if (sub != 0) {
                        if (sub == 1) {
                            D_004361F8 = 6;
                            return D_00391EF8;
                        }
                    }
                }
                D_004361F8 = 0;
                return D_00391E50;
            case 2:
                D_004361F8 = 1;
                return D_00391E6C;
            case 3:
                D_004361F8 = 2;
                return D_00391E88;
            case 4:
                D_004361F8 = 3;
                return D_00391EA4;
            case 5:
                D_004361F8 = 5;
                return D_00391EDC;
            case 10:
                D_004361F8 = 0;
                return D_00391F30;
            case 11:
                D_004361F8 = 0;
                return D_00391F4C;
            case 12:
                D_004361F8 = 0;
                return D_00391F68;
            }
        }
        i++;
    } while (i < 0x100);
    return 0;
}

/* Use the selected actor slot only when it belongs to this area and kind 10. */
const char *fldSelectCurrentActorOnNextFloor(void) {
    s32 index = D_00435F28;
    FldActorEntry *entry = &fldDoorTable[index];
    if (entry->floor == D_00389784[0] + 1 && entry->kind == 10) {
        fldSelectedActorEntryIndex = index;
        D_004361F8 = 8;
        return D_00391F30;
    }
    return 0;
}

s32 fldGetSelectedActorMotionId(void) {
    return fldDoorTable[fldSelectedActorEntryIndex].motion;
}

/* Inspect two independent properties of the selected actor, depending on mode:
 * mode 0 derives a size from the first two states; mode 1 tests a flag. */
s32 fldQuerySelectedActorMotionState(s32 mode) {
    FldActorEntry *entry = &fldDoorTable[fldSelectedActorEntryIndex];
    s16 a;
    u32 result;
    if (mode == 0 && entry->kind == 1) {
        a = entry->motion;
        if (a == 5 || entry->secondaryMotion == 5 || a == 6 || entry->secondaryMotion == 6 || a == 7 ||
            entry->secondaryMotion == 7 || a == 8 || entry->secondaryMotion == 8) {
            return 0x28;
        }
        return 0x14;
    }
    if (mode == 1) {
        result = (u8)entry->flags54 & 8;
        return result != 0;
    }
    return 0;
}

void fldApplyActorEntryTrigger(s32 useTaskRecord) {
    s32 index;
    s32 kind;
    const char *record;
    FldActorEntry *entry;

    if (useTaskRecord != 0) {
        record = fldEveHitResName_Find(scrGetScriptHandle()->task);
        if (record == 0) {
            return;
        }
        if (fldFindActorEntryByName(record) == 0) {
            return;
        }
    }
    index = fldSelectedActorEntryIndex;
    entry = &fldDoorTable[index];
    kind = entry->kind;
    if (kind == 1) {
        if (entry->floor == D_00389784[0] + 1) {
            fldPlayMenuSound(entry->sound);
            fldBeginNpcInteractionById(fldActorSlots[index].actorId);
            return;
        }
    } else if (kind == 2) {
        if (entry->floor == fldGlobalBase.floor + 1) {
            fldGlobalBase.positionMode = 1;
            fldGlobalBase.modeOneCameraHeadingTarget = fldActorSlots[index].firstValues[0];
            if (entry->motion == 1) {
                dds3FadeOut(0xC0, 0xC0, 0xC0, 0xF);
                return;
            }
            dds3FadeOut(0, 0, 0, 0xF);
            return;
        }
    } else if (kind == 3) {
        kwlnFadeSetRGB(0, 0, 0);
        return;
    } else if (kind == 5) {
        if (entry->motion == 0) {
            D_003898FC[0] = 0x64;
        } else {
            D_003898FC[0] = -0x64;
        }
    } else if (kind == 10) {
    } else if (kind == 11) {
        if (entry->floor == D_00389784[0] + 1) {
            if (entry->motion == 3) {
                fldApplyRoomObjectModeZero(0, 0, (char *)entry->otherName, 0);
                return;
            }
        }
    } else if (kind == 12) {
    } else if (kind == 4) {
        fldApplyPendingCameraHeading();
    }
}

void fldDoor_ExitCamera(const char *name) {
    FldActorEntry *entry;
    char *entryName;
    dds3Basic_t *camera;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 0) {
            continue;
        }
        entryName = entry->name;
        if (entry->floor == fldGlobalBase.floor + 1) {
            if (strcmp(name, entryName) == 0) {
                if (entry->variantMode == 2) {
                    return;
                }
                if (entry->variantMode == 0 && entry->linkKind == 3) {
                    camera = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 4, entry->linkName);
                    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
                    fldHideSceneModelsAndResetCamera();
                    return;
                }
            }
        }
    }
}

u8 fldDoor_ChkWarp(void) {
    return D_004361F8 == 8;
}

void fldApplySceneRoomSelection(FldActorEntry *actorEntry) {
    if (actorEntry->selectedRoom != 0) {
        fldGlobalBase.selectedRoomIndex = actorEntry->selectedRoom - 1;
    }
    fldGlobalBase.rowIdx = actorEntry->rowIndex;
}

#include "dat_state.h"
extern s32 D_004361E8;
extern char D_004134C0[];
extern char D_004134D0[];
extern void dds3WorkClear(void);
extern void fldSetSceneParam3(fldSceneParam_t *, s32, s32, const char *);
extern void fldSetSceneParam5(fldSceneParam_t *, s32, s32, const char *, s32, const char *);

void fldApplyCurrentTaskScene(const char *name) {
    fldSceneParam_t sequence;
    s32 vr[2];
    s32 event;
    s32 area[1];
    s32 command;
    s32 floor;
    s32 row;
    s32 headerFloor;
    s32 currentArea;
    s32 i;
    FldActorEntry *entry;
    const char *entryName;
    s32 match;
    u32 floorFlags;
    u32 flags;

    if (name == NULL && !fldDoor_ChkWarp()) {
        return;
    }
    for (i = 0; i < FIELD_ACTOR_SLOT_COUNT; i++) {
        if (fldDoor_ChkWarp()) {
            i = D_00435F28;
        }
        entry = &fldActorWaypointRows.actors[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 0) {
            continue;
        }
        entryName = entry->name;
        if (entry->floor != fldGlobalBase.floor + 1) {
            continue;
        }
        match = 0;
        if (fldDoor_ChkWarp()) {
            match = 1;
        }
        if (!match && strcmp(name, entryName) == 0) {
            match = 1;
        }
        if (!match) {
            continue;
        }
        if (entry->kind == 3) {
            if (entry->motion != 0) {
                fldSetFloorFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                entry->motion);
            }
        } else if (entry->kind == 10) {
            if (entry->secondaryMotion != 0) {
                fldSetFloorFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                entry->secondaryMotion);
            }
        } else if (entry->kind == 11) {
            if (entry->secondaryMotion != 0) {
                fldSetFloorFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                entry->secondaryMotion);
            }
        }
        floorFlags = entry->flags64;
        if (floorFlags & 1) {
            if (entry->explorationArea != 0) {
                area[0] = entry->explorationArea;
            } else {
                area[0] = fldGlobalBase.area;
            }
            floor = floorFlags >> 2;
            if (floor == 0) {
                floor = fldGlobalBase.floor + 1;
            }
            fldSetFloorFlag(area[0], floor, entry->floorFlagBit);
        }
        flags = entry->flags64;
        if (flags & 2) {
            fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor,
                entry->explorationColumn, entry->explorationRow,
                entry->explorationColumnCount, entry->explorationRowCount);
        }
        if (entry->variantMode == 2) {
            fldSetDeferredFieldCommand(0, 0);
            fldSetFloorFlag(fldGlobalBase.area, fldGlobalBase.floor + 1,
                            entry->warpEntry2);
            D_004361E4 = fldGlobalBase.area;
            D_004361E8 = fldGlobalBase.floor + 1;
            D_004361EC = entry->variant;
            D_004361F0 = entry->warpEntry;
            dds3WorkClear();
            switch (entry->variant) {
            case 0:
                fldBgmFadeOut();
                event = entry->warpEntry;
                dds3AdminiCallSequence(0x14, &event, sizeof(event), 0);
                return;
            case 3:
                fldBgmFadeOut();
                vr[0] = 0;
                vr[1] = entry->warpEntry;
                if (D_0038A3B8[vr[1]].requiredModelFlag != 0) {
                    evtBitOn(D_0038A3B8[vr[1]].requiredModelFlag);
                }
                DDS3_GBWK->vr.previewX = vr[0];
                DDS3_GBWK->vr.previewY = vr[1];
                dds3AdminiCallSequence(0x15, vr, sizeof(vr), 0);
                return;
            case 4:
                vr[0] = 1;
                vr[1] = entry->warpEntry;
                DDS3_GBWK->vr.previewX = vr[0];
                DDS3_GBWK->vr.previewY = vr[1];
                dds3AdminiCallSequence(0x15, vr, sizeof(vr), 0);
                return;
            }
            return;
        }
        if (entry->variantMode == 0) {
            if (entry->variant == 1) {
                dds3WorkClear();
                fldBgmFadeOut();
                area[0] = fldGlobalBase.area;
                dds3AdminiCallSequence(0x1C, &area[0], sizeof(area[0]), 0);
                return;
            }
            if (entry->variant == 0) {
                area[0] = fldGlobalBase.area;
            } else {
                area[0] = entry->variant;
            }
            if (entry->warpEntry == 0) {
                floor = fldGlobalBase.floor + 1;
            } else {
                floor = entry->warpEntry;
            }
            if (fldGlobalBase.area != area[0]) {
                fldBgmFadeOut();
            }
            fldApplySceneRoomSelection(entry);
            if (entry->flags54 != 0) {
                D_00435F24 = entry->flags54;
                strcpy((char *)D_00387D60, entry->taskName);
                D_00435F28 = i + 1;
            }
            if (entry->flags31 & 2) {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_REQUEST;
            } else if (entry->flags31 & 4) {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_SUPPRESS;
            } else {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_NONE;
            }
            if (entry->kind == 3 || entry->kind == 4) {
                fldGlobalBase.unk188 = 15;
                dds3FadeOut(0, 0, 0, 0);
                if (entry->linkKind == 1 || entry->linkKind == 2) {
                    fldSetSceneParam4(&sequence, area[0], floor, entry->warpName, entry->sequenceCode,
                                                     entry->linkKind, entry->linkName);
                } else {
                    fldSetSceneParam3(&sequence, area[0], floor, entry->warpName);
                }
            } else {
                fldSetSceneParam4(&sequence, area[0], floor, entry->warpName, entry->sequenceCode,
                                                 entry->linkKind, entry->linkName);
            }
            dds3AdminiCallSequence(5, &sequence, sizeof(sequence), 0);
            return;
        }
        if (entry->variantMode == 1) {
            row = entry->variant - 1;
            headerFloor = entry->warpEntry;
            currentArea = fldGlobalBase.area;
            floor = fldActorWaypointRows.headers[row].area;
            area[0] = currentArea;
            D_004361DC = row;
            D_004361E0 = headerFloor;
            fldSetFloorFlag(area[0], fldGlobalBase.floor + 1,
                            fldActorWaypointRows.headers[row].payload[headerFloor + 8]);
            entryName = D_004134C0;
            name = D_004134D0;
            fldSetSceneParam5(&sequence, area[0], floor, entryName, 0, name);
            dds3AdminiCallSequence(5, &sequence, sizeof(sequence), 0);
            return;
        }
        if (entry->variantMode == 3) {
            command = entry->variant;
            fldSetDeferredFieldCommand(fldGlobalBase.area, command);
            fldBgmFadeOut();
            if (entry->warpEntry == 0 && entry->warpEntry2 != 0) {
                fldSetDeferredFieldCommand(entry->warpEntry2, command);
            }
            dds3AdminiCallSequence(6, &command, sizeof(command), 0);
            return;
        }
    }
}

/* Dispatch the first enabled field actor matching this action.
 * Elevator block IDs occupy the second six-halfword bank of body.data,
 * the same canonical header view used by fldDoor_GetElvTbl below. */
void fldDispatchSceneAction(s32 action) {
    fldSceneParam_t sequence;
    s32 preview[2];
    s32 event;
    s32 area[1];
    s32 currentArea;
    s32 warp;
    s32 room;
    s32 index;
    s32 headerIndex;
    s32 headerFloor;
    u32 floorFlags;
    u32 flags;
    FldActorEntry *entry;

    if (action == 0) return;
    if (action == 0x384) {
        dds3WorkClear();
        fldSetDeferredFieldCommand(0, 0);
        D_004361E4 = fldGlobalBase.area;
        preview[0] = 2;
        preview[1] = 0;
        D_004361E8 = fldGlobalBase.floor + 1;
        D_004361EC = 5;
        D_004361F0 = 0;
        DDS3_GBWK->vr.previewX = preview[0];
        DDS3_GBWK->vr.previewY = preview[1];
        dds3AdminiCallSequence(0x15, preview, 8, 0);
        return;
    }
    if (action == 0x385) {
        dds3WorkClear();
        fldSetDeferredFieldCommand(0, 0);
        D_004361E4 = fldGlobalBase.area;
        preview[0] = 1;
        preview[1] = fldTriggeredSavePointValue;
        D_004361E8 = fldGlobalBase.floor + 1;
        D_004361EC = 4;
        D_004361F0 = fldTriggeredSavePointValue;
        DDS3_GBWK->vr.previewX = preview[0];
        DDS3_GBWK->vr.previewY = preview[1];
        dds3AdminiCallSequence(0x15, preview, 8, 0);
        return;
    }
    for (index = 0; index < 256; index++) {
        entry = &fldDoorTable[index];
        if (entry->requiredFlag != 0 && !evtBitCheck(entry->requiredFlag)) continue;
        if (entry->kind != 9 || entry->motion != action) continue;
        floorFlags = entry->flags64;
        if (floorFlags & 1) {
            if (entry->explorationArea == 0) area[0] = fldGlobalBase.area;
            else area[0] = entry->explorationArea;
            room = entry->flags64 >> 2;
            if (room == 0) room = fldGlobalBase.floor + 1;
            fldSetFloorFlag(area[0], room, entry->floorFlagBit);
        }
        flags = entry->flags64;
        if (flags & 2) {
            fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor,
                entry->explorationColumn, entry->explorationRow,
                entry->explorationColumnCount, entry->explorationRowCount);
        }
        if (entry->variantMode == 2) {
            fldSetFloorFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, entry->warpEntry2);
            D_004361E4 = fldGlobalBase.area;
            D_004361E8 = fldGlobalBase.floor + 1;
            D_004361EC = entry->variant;
            D_004361F0 = entry->warpEntry;
            dds3WorkClear();
            switch (entry->variant) {
            case 0:
                fldBgmFadeOut();
                event = entry->warpEntry;
                dds3AdminiCallSequence(0x14, &event, 4, 0);
                return;
            case 3:
                fldBgmFadeOut();
                preview[0] = 0;
                preview[1] = entry->warpEntry;
                if (D_0038A3B8[preview[1]].requiredModelFlag != 0) {
                    evtBitOn(D_0038A3B8[preview[1]].requiredModelFlag);
                }
                DDS3_GBWK->vr.previewX = preview[0];
                DDS3_GBWK->vr.previewY = preview[1];
                dds3AdminiCallSequence(0x15, preview, 8, 0);
                return;
            case 4:
                preview[0] = 1;
                preview[1] = entry->warpEntry;
                DDS3_GBWK->vr.previewX = preview[0];
                DDS3_GBWK->vr.previewY = preview[1];
                dds3AdminiCallSequence(0x15, preview, 8, 0);
                return;
            }
            return;
        } else if (entry->variantMode == 0) {
            if (entry->variant == 1) {
                fldBgmFadeOut();
                area[0] = fldGlobalBase.area;
                dds3AdminiCallSequence(0x1C, &area[0], 4, 0);
                return;
            }
            if (entry->variant == 0) area[0] = fldGlobalBase.area;
            else area[0] = entry->variant;
            if (entry->warpEntry == 0) room = fldGlobalBase.floor + 1;
            else room = entry->warpEntry;
            if (fldGlobalBase.area != area[0]) fldBgmFadeOut();
            fldApplySceneRoomSelection(entry);
            if (entry->flags54 != 0) {
                D_00435F24 = entry->flags54;
                strcpy((char *)D_00387D60, entry->taskName);
                D_00435F28 = index + 1;
            }
            if (entry->flags31 & 2) fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_REQUEST;
            else if (entry->flags31 & 4) fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_SUPPRESS;
            else fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_NONE;
            if ((u32)((u8)entry->kind - 3) < 2U) {
                fldGlobalBase.unk188 = 15;
                if (entry->linkKind == 1) {
                    fldSetSceneParam4(&sequence, area[0], room, entry->warpName,
                        entry->sequenceCode, 1, entry->linkName);
                } else {
                    fldSetSceneParam3(&sequence, area[0], room, entry->warpName);
                }
            } else {
                fldSetSceneParam4(&sequence, area[0], room, entry->warpName,
                    entry->sequenceCode, entry->linkKind, entry->linkName);
            }
            dds3AdminiCallSequence(5, &sequence, 0xA0, 0);
            return;
        } else if (entry->variantMode == 1) {
            headerIndex = entry->variant - 1;
            headerFloor = entry->warpEntry;
            currentArea = fldGlobalBase.area;
            room = fldActorWaypointRows.headers[headerIndex].area;
            area[0] = currentArea;
            D_004361DC = headerIndex;
            D_004361E0 = headerFloor;
            fldSetFloorFlag(area[0], fldGlobalBase.floor + 1,
                fldActorWaypointRows.headers[headerIndex].body.data[headerFloor + 6]);
            fldSetSceneParam5(&sequence, area[0], room, D_004134C0, 0, D_004134D0);
            dds3AdminiCallSequence(5, &sequence, 0xA0, 0);
            return;
        } else if (entry->variantMode == 3) {
            warp = entry->variant;
            fldSetDeferredFieldCommand(fldGlobalBase.area, warp);
            fldBgmFadeOut();
            if (entry->warpEntry == 0 && entry->warpEntry2 != 0) {
                fldSetDeferredFieldCommand(entry->warpEntry2, warp);
            }
            dds3AdminiCallSequence(6, &warp, 4, 0);
            return;
        }
    }
}


void fldSelectActorFromSceneIndexTables(s32 mode, s32 index) {
    switch (mode) {
    case 0:
        D_004361EC = 3;
        D_004361E4 = D_0038A3B8[index].unk0;
        D_004361F0 = index;
        break;
    case 1:
        D_004361F0 = index;
        D_004361E4 = D_0038A480[index].unk0;
        break;
    }
}

/* Run the actor transition selected by fldSelectActorFromSceneIndexTables:
 * find the first enabled kind-7 actor for the pending field, motion and
 * index, mark its floor and exploration bits, then submit its sequence and
 * clear the pending selection. */
void fldSubmitSelectedActorWaypointTransition(void) {
    fldSceneParam_t record;
    s32 stage;
    s32 room;
    s32 extra;
    s32 motion;
    s32 index;
    u32 floorFlags;
    u32 flags;
    s32 i;
    FldActorEntry *entry;

    if (D_004361E4 == 0) {
        return;
    }
    extra = D_004361E8;
    motion = D_004361EC;
    index = D_004361F0;
    fldDoorLoad(D_004361E4);
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && !evtBitCheck(entry->requiredFlag)) {
            continue;
        }
        if (entry->kind != 7) {
            continue;
        }
        if ((u32)(motion - 3) >= 2 && extra != entry->floor) {
            continue;
        }
        if (motion != entry->motion || entry->variantMode != 0) {
            continue;
        }
        if (motion != 5 && index != entry->secondaryMotion) {
            continue;
        }
        floorFlags = entry->flags64;
        if (floorFlags & 1) {
            stage = entry->explorationArea != 0 ? entry->explorationArea : fldGlobalBase.area;
            room = floorFlags >> 2;
            if (room == 0) {
                room = fldGlobalBase.floor + 1;
            }
            fldSetFloorFlag(stage, room, entry->floorFlagBit);
        }
        flags = entry->flags64;
        if (flags & 2) {
            fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor, entry->explorationColumn, entry->explorationRow,
                                                entry->explorationColumnCount, entry->explorationRowCount);
        }
        if (entry->variant == 1) {
            fldBgmFadeOut();
            stage = fldGlobalBase.area;
            dds3AdminiCallSequence(0x1C, &stage, 4, 0);
            return;
        }
        if (entry->variant == 0) {
            stage = D_004361E4;
        } else {
            stage = entry->variant;
        }
        room = entry->warpEntry;
        if (room == 0) {
            room = D_004361E8;
        }
        if (fldGlobalBase.area != stage) {
            fldBgmFadeOut();
        }
        fldApplySceneRoomSelection(entry);
        if (entry->flags54 != 0) {
            D_00435F24 = entry->flags54;
            strcpy((char *)D_00387D60, entry->taskName);
            D_00435F28 = i + 1;
        }
        fldSetSceneParam4(&record, stage, room, entry->warpName, entry->sequenceCode, entry->linkKind, entry->linkName);
        if ((u32)(motion - 3) < 2) {
            record.mode = 5;
        } else {
            record.mode = 4;
        }
        D_004361E4 = 0;
        D_004361E8 = 0;
        D_004361EC = 0;
        D_004361F0 = 0;
        dds3AdminiCallSequence(5, &record, 0xA0, 0);
        return;
    }
}

extern void fldSetSceneParamLoad(fldSceneParam_t *, s32, s32, const char *, s32, s32, const char *);

/* Start the linked sequence of the first enabled waypoint actor that matches this field transition. */
void fldInitializeActorLinkedSequence(s32 mode, s32 index, fldSceneParam_t *sequence) {
    s32 field;
    s32 motion;
    s32 kind;
    s32 i;
    FldActorEntry *entry;

    if (mode == 0) {
        field = D_0038A3B8[index].unk0;
        motion = 3;
    } else {
        motion = 4;
        field = D_0038A480[index].unk0;
    }
    if (field == 0) {
        return;
    }
    fldDoorLoad(field);
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && !evtBitCheck(entry->requiredFlag)) {
            continue;
        }
        if (entry->kind != 7 || motion != entry->motion ||
            index != entry->secondaryMotion || entry->variantMode != 0) {
            continue;
        }
        fldGlobalBase.rowIdx = entry->rowIndex;
        if (entry->flags54 != 0) {
            D_00435F24 = entry->flags54;
            strcpy((char *)D_00387D60, entry->taskName);
            D_00435F28 = i + 1;
        }
        if (entry->variant != 0) {
            field = entry->variant;
        }
        if (entry->warpEntry != 0) {
            kind = entry->warpEntry;
        } else {
            kind = 1;
        }
        if (field != 0 && kind != 0) {
            fldSetSceneParamLoad(sequence, field, kind, entry->warpName,
                                        entry->sequenceCode, entry->linkKind, entry->linkName);
            sequence->mode = 5;
            if (fldGlobalBase.sequenceCode != entry->sequenceCode && entry->sequenceCode != 0) {
                fldGlobalBase.sequenceCode = entry->sequenceCode;
            }
            if (entry->selectedRoom != 0) {
                fldGlobalBase.selectedRoomIndex = entry->selectedRoom - 1;
            }
        }
        return;
    }
}

FldDeferredCommandDisposition fldStartDeferredActorTransition(s32 command, u32 parameter) {
    fldSceneParam_t record;
    s32 warp;
    union {
        s32 value;
        u8 bytes[sizeof(s32)]; /* The field mode request copies this word. */
    } stagePayload;
    s32 room;
    u32 floorFlags;
    u32 flags;
    s32 i;
    FldActorEntry *entry;
    char *taskName;

    fldDoorLoad(command);
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && !evtBitCheck(entry->requiredFlag)) {
            continue;
        }
        if (entry->kind != 8 || entry->motion != parameter) {
            continue;
        }
        if (entry->variantMode == 3) {
            warp = entry->variant;
            fldSetDeferredFieldCommand(command, warp);
            fldBgmFadeOut();
            if (entry->warpEntry == 0 && entry->warpEntry2 != 0) {
                fldSetDeferredFieldCommand(entry->warpEntry2, warp);
            }
            dds3AdminiCallSequence(6, &warp, 4, 0);
            return FLD_DEFERRED_COMMAND_PRESERVE_PENDING;
        }
        if (entry->variantMode != 0) {
            continue;
        }
        floorFlags = entry->flags64;
        if (floorFlags & 1) {
            if (entry->explorationArea == 0) {
                stagePayload.value = fldGlobalBase.area;
            } else {
                stagePayload.value = entry->explorationArea;
            }
            room = floorFlags >> 2;
            if (room == 0) {
                room = fldGlobalBase.floor + 1;
            }
            fldSetFloorFlag(stagePayload.value, room, entry->floorFlagBit);
        }
        flags = entry->flags64;
        if (flags & 2) {
            fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor, entry->explorationColumn, entry->explorationRow,
                                                entry->explorationColumnCount, entry->explorationRowCount);
        }
        if (entry->variant == 1) {
            fldBgmFadeOut();
            stagePayload.value = fldGlobalBase.area;
            dds3AdminiCallSequence(0x1C, stagePayload.bytes, sizeof(stagePayload.bytes), 0);
            return FLD_DEFERRED_COMMAND_CLEAR_PENDING;
        }
        if (entry->variant == 0) {
            stagePayload.value = command;
        } else {
            stagePayload.value = entry->variant;
        }
        room = entry->warpEntry;
        if (fldGlobalBase.area != stagePayload.value) {
            fldBgmFadeOut();
        }
        fldApplySceneRoomSelection(entry);
        if (entry->flags54 != 0) {
            D_00435F24 = entry->flags54;
            taskName = entry->taskName;
            strcpy((char *)D_00387D60, taskName);
            D_00435F28 = i + 1;
        }
        fldSetSceneParam4(&record, stagePayload.value, room, entry->warpName, entry->sequenceCode, entry->linkKind, entry->linkName);
        record.mode = 6;
        dds3AdminiCallSequence(5, &record, 0xA0, 0);
        return FLD_DEFERRED_COMMAND_CLEAR_PENDING;
    }
    return FLD_DEFERRED_COMMAND_CLEAR_PENDING;
}

extern s32 fldRequestAreaResource(s32 area, s32 room);

/* Each warp kind keeps its own arm; retail compiles them separately and merges the tails. */
void fldDoor_WarpCash(const char *name) {
    FldActorEntry *entry;
    s32 area;
    s32 room;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 1 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->warpEntry;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 2 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->warpEntry;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 3 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->warpEntry;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 4 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->warpEntry;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 5 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->warpEntry;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        }
    }
}

/* Read a selected actor state or a named world-object value; cases 1 and 2
 * deliberately fall through when no named object is found. */
s32 fldGetActorStat0(s32 attribute) {
    FldActorEntry *actor = &fldDoorTable[fldSelectedActorEntryIndex];
    dds3Basic_t *objectNode;
    s32 secondaryMotion;

    switch (attribute) {
    case 0:
        return actor->motion;
    case 1:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), actor->motionName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 2:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), actor->otherName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 3:
        secondaryMotion = (u16)actor->secondaryMotion;
        if (secondaryMotion & 1) {
            return 1;
        }
        return 0;
    }
    return 0;
}


/* Read a selected actor attribute; selector zero maps its motion code.
 * Missing named objects fall through to subsequent attribute cases. */
INCLUDE_RODATA(const s32, "game/code_0013B818", D_004134C0);

INCLUDE_RODATA(const s32, "game/code_0013B818", D_004134D0);

s32 fldGetMappedActorStateAttribute(u32 attribute) {
    FldActorEntry *actor = &fldDoorTable[fldSelectedActorEntryIndex];
    dds3Basic_t *objectNode;

    switch (attribute) {
    case 0:
        switch (actor->motion) {
        case 0:
            return 0xC;
        case 1:
            return 0xF;
        case 2:
            return 0x10;
        case 3:
            return 0xD;
        case 4:
            return 0xE;
        }
        return 0;
    case 1:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), actor->motionName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 2:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), actor->otherName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
        return actor->secondaryMotion;
    case 3:
        return actor->secondaryMotion;
    case 4:
        return actor->sound;
    case 5:
        return D_003897C0[0] != 1;
    }
    return 0;
}

/* Look up a motion-table attribute indexed by this actor's state; named
 * object lookups fall through to the next attribute if absent. */
s32 fldGetActorMotionEntry(u32 attribute) {
    FldActorEntry *actor = &fldDoorTable[fldSelectedActorEntryIndex];
    s16 motionIndex = actor->motion;
    dds3Basic_t *objectNode;
    s32 flags;

    switch (attribute) {
    case 0:
        return D_00391FA0[motionIndex].defaultMotionId;
    case 1:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), D_00391FA0[motionIndex].motionObjectName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 2:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), D_00391FA0[motionIndex].secondaryMotionObjectName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 3:
        D_004361D4 = D_00391FA0[motionIndex].roomModeGroupOneName;
        return 0;
    case 4:
        D_004361D8 = D_00391FA0[motionIndex].sceneBitsBarrierRoomName;
        return 0;
    case 5:
        return D_00391FA0[motionIndex].attribute5Value;
    case 6:
        flags = actor->flags64;
        if (flags & 1) {
            return actor->floorFlagBit;
        }
        return -1;
    }
    return 0;
}

s32 fldDoor_GetElvTbl(u32 kind) {
    s32 slot = D_004361DC;

    switch (kind) {
    case 0:
        return fldActorWaypointRows.headers[slot].floorCount;
    case 1:
        return fldActorWaypointRows.headers[slot].sound;
    case 2:
        return fldActorWaypointRows.headers[slot].body.data[0];
    case 3:
        return fldActorWaypointRows.headers[slot].body.data[1];
    case 4:
        return fldActorWaypointRows.headers[slot].body.data[2];
    case 5:
        return fldActorWaypointRows.headers[slot].body.data[3];
    case 6:
        return fldActorWaypointRows.headers[slot].body.data[4];
    case 7:
        return fldActorWaypointRows.headers[slot].body.data[5];
    case 8:
        return fldActorWaypointRows.headers[slot].body.data[6];
    case 9:
        return fldActorWaypointRows.headers[slot].body.data[7];
    case 10:
        return fldActorWaypointRows.headers[slot].body.data[8];
    case 11:
        return fldActorWaypointRows.headers[slot].body.data[9];
    case 12:
        return fldActorWaypointRows.headers[slot].body.data[10];
    case 13:
        return fldActorWaypointRows.headers[slot].body.data[11];
    case 14:
        return fldActorWaypointRows.headers[slot].floorCount - D_004361E0 - 1;
    }
    return 0;
}

s32 fldDoor_ElvTakasaToFloor(s32 index) {
    s32 slot = D_004361DC;
    s32 count = fldActorWaypointRows.headers[slot].floorCount;

    if (count - 1 < index) {
        return count - D_004361E0 - 1;
    }
    index = count - index - 1;
    return fldActorWaypointRows.headers[slot].body.data[index];
}

/* The retail code reaches the waypoint table through separate +4 and +6 address
 * constants (a 16-halfword row stride), so the two views are distinct symbols. */
extern s16 D_003931A4[];
extern s16 D_003931A6[];

void fldDispatchSceneRoomSelection(s32 index) {
    fldSceneParam_t sequence;
    s32 command;
    s32 sourceRoom;
    s32 count;
    s32 i;
    s32 area;
    s32 floor;
    FldActorEntry *entry;
    s32 remaining;
    s32 inRange;
    u32 floorFlags;
    u32 flags;

    sourceRoom = D_004361E0;
    count = D_003931A6[D_004361DC * 16];
    inRange = index < count;
    remaining = count - index;
    if (inRange) {
        sourceRoom = remaining;
        sourceRoom--;
    }
    for (i = 0; i < FIELD_ACTOR_SLOT_COUNT; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 6 && entry->motion - 1 == D_004361DC &&
            entry->secondaryMotion == sourceRoom && entry->variantMode == 0) {
            if (entry->variant == 0) {
                area = fldGlobalBase.area;
            } else {
                area = entry->variant;
            }
            if (entry->warpEntry == 0) {
                floor = fldGlobalBase.floor + 1;
            } else {
                floor = entry->warpEntry;
            }
            if (fldGlobalBase.area != area) {
                fldBgmFadeOut();
            }
            fldApplySceneRoomSelection(entry);
            floorFlags = entry->flags64;
            if (floorFlags & 1) {
                if (entry->explorationArea != 0) {
                    area = entry->explorationArea;
                } else {
                    area = fldGlobalBase.area;
                }
                floor = floorFlags >> 2;
                if (floor == 0) {
                    floor = fldGlobalBase.floor + 1;
                }
                fldSetFloorFlag(area, floor, entry->floorFlagBit);
            }
            flags = entry->flags64;
            if (flags & 2) {
                fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor, entry->explorationColumn, entry->explorationRow,
                                                    entry->explorationColumnCount, entry->explorationRowCount);
            }
            if (entry->flags54 != 0) {
                D_00435F24 = entry->flags54;
                strcpy((char *)D_00387D60, entry->taskName);
                D_00435F28 = i + 1;
            }
            if (entry->flags31 & 2U) {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_REQUEST;
            } else if (entry->flags31 & 4U) {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_SUPPRESS;
            } else {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_NONE;
            }
            fldSetFloorFlag(area, floor, D_003931A4[D_004361DC * 16 + sourceRoom + 8]);
            fldSetSceneParam4(&sequence, area, floor, entry->warpName, entry->sequenceCode, entry->linkKind, entry->linkName);
            dds3AdminiCallSequence(5, &sequence, sizeof(sequence), 0);
            break;
        } else if (entry->kind == 6 && entry->motion - 1 == D_004361DC &&
                   entry->secondaryMotion == sourceRoom && entry->variantMode == 3) {
            command = entry->variant;
            fldSetDeferredFieldCommand(fldGlobalBase.area, command);
            fldBgmFadeOut();
            if (entry->warpEntry == 0 && entry->warpEntry2 != 0) {
                fldSetDeferredFieldCommand(entry->warpEntry2, command);
            }
            dds3AdminiCallSequence(6, &command, sizeof(command), 0);
            break;
        }
    }
}

/* Apply native actor-slot defaults; fields not written here remain untouched. */
void fldResetActorSlots(void) {
    s32 actorIndex;

    for (actorIndex = 0; actorIndex < FIELD_ACTOR_SLOT_COUNT; actorIndex++) {
        fldActorSlots[actorIndex].kind = 0;
        fldActorSlots[actorIndex].actorId = 0;
        fldActorSlots[actorIndex].unk08 = 0;
        fldActorSlots[actorIndex].firstKey = -1;
        fldActorSlots[actorIndex].secondaryMotion = 0;
        fldActorSlots[actorIndex].secondKey = -1;
        fldActorSlots[actorIndex].transitionKey = -1;
        fldActorSlots[actorIndex].firstFrame = 0;
        fldActorSlots[actorIndex].secondFrame = 0;
        fldActorSlots[actorIndex].transitionFrame = 0;
        fldActorSlots[actorIndex].firstValues[0] = 0.0f;
        fldActorSlots[actorIndex].firstValues[1] = 0;
        fldActorSlots[actorIndex].firstValues[2] = 0;
        fldActorSlots[actorIndex].firstStep[0] = 0;
        fldActorSlots[actorIndex].firstStep[1] = 0;
        fldActorSlots[actorIndex].firstStep[2] = 0;
        fldActorSlots[actorIndex].secondValues[0] = 0;
        fldActorSlots[actorIndex].secondValues[1] = 0;
        fldActorSlots[actorIndex].secondValues[2] = 0;
        fldActorSlots[actorIndex].secondStep[0] = 0;
        fldActorSlots[actorIndex].secondStep[1] = 0;
        fldActorSlots[actorIndex].secondStep[2] = 0;
    }
    fldSelectedActorEntryIndex = -1;
}

void fldDoorLoad(s32 field) {
    char path[64];
    char directory[32];
    sdfFileCmdHandle_t *command;
    if (field >= 100) {
        memset(&fldActorWaypointRows, 0, 0x6D00);
    } else {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, "%sF%03d.WAP", directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &fldActorWaypointRows, 0x6D00);
        sdfFileClose(command);
    }
}

void fldGetDoorEntry(FldWaypointBlock *src) {
    fldActorWaypointRows = *src;
}

void fldInitializeActorMotionSlots(void) {
    f32 rotation[4];
    f32 angle;
    dds3Basic_t *world;
    s32 firstDirection = 0;
    s32 secondDirection = 0;
    s32 actorIndex;
    s32 slotIndex;
    s32 primaryMotion;
    FldActorEntry *actor;
    FldActorRow *row;
    dds3Basic_t *node;

    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;
    world = (dds3Basic_t *)dds3GetControlScene();
    for (actorIndex = 0; actorIndex < FIELD_ACTOR_SLOT_COUNT; actorIndex++) {
        actor = &fldDoorTable[actorIndex];
        row = &fldActorSlots[actorIndex];
        if (actor->kind == 2) {
            if (actor->floor == D_00389784[0] + 1) {
                for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
                    if (fldRoomResources[slotIndex]->name != NULL &&
                        strcmp(fldRoomResources[slotIndex]->name, actor->name) == 0) {
                        FldFileResource *resource = fldRoomResources[slotIndex];
                        const f32 *transform = resource->transform;
                        row->actorId = resource->id;
                        rotation[0] = transform[4];
                        rotation[1] = transform[5];
                        rotation[2] = transform[6];
                        rotation[3] = transform[7];
                        SDF_VU0_LOADVECTOR(rotation);
                        angle = EulerAngleAxisY() * 180.0f / 3.14f;
                        while (angle < 0.0f) {
                            angle += 360.0f;
                        }
                        while (angle > 360.0f) {
                            angle -= 360.0f;
                        }
                        row->firstValues[0] = angle;
                    }
                }
                row->kind = 0;
                row->firstFrame = 0;
                row->secondFrame = 0;
                row->firstValues[1] = 0.0f;
                row->firstValues[2] = 0.0f;
                row->secondValues[0] = 0.0f;
                row->secondValues[1] = 0.0f;
                row->secondValues[2] = 0.0f;
            }
        } else if (actor->kind == 1) {
            if (actor->floor != D_00389784[0] + 1) {
                continue;
            }
            row->firstKey = -1;
            if (actor->motion != 0) {
                node = dds3SceneBasicSearchNameAndType(world, 6, (const u8 *)actor->motionName);
                if (node != NULL) {
                    row->firstKey = node->key;
                    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
                        if (fldRoomResources[slotIndex]->name != NULL &&
                            strcmp(fldRoomResources[slotIndex]->name, actor->name) == 0) {
                            FldFileResource *resource = fldRoomResources[slotIndex];
                            const f32 *transform = resource->transform;
                            row->actorId = resource->id;
                            rotation[0] = transform[4];
                            rotation[1] = transform[5];
                            rotation[2] = transform[6];
                            rotation[3] = transform[7];
                            SDF_VU0_LOADVECTOR(rotation);
                            angle = EulerAngleAxisY() * 180.0f / 3.14f;
                            while (angle < 0.0f) {
                                angle += 360.0f;
                            }
                            while (angle > 360.0f) {
                                angle -= 360.0f;
                            }
                            if (45.0f <= angle && angle <= 135.0f) {
                                firstDirection = 1;
                            } else if (135.0f <= angle && angle <= 225.0f) {
                                firstDirection = 0;
                            } else if (225.0f <= angle && angle <= 315.0f) {
                                firstDirection = 3;
                            } else {
                                firstDirection = 2;
                            }
                        }
                    }
                }
            }
            row->secondKey = -1;
            if (actor->secondaryMotion != 0) {
                node = dds3SceneBasicSearchNameAndType(world, 6, (const u8 *)actor->otherName);
                if (node != NULL) {
                    row->secondKey = node->key;
                    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
                        if (fldRoomResources[slotIndex]->name != NULL &&
                            strcmp(fldRoomResources[slotIndex]->name, actor->name) == 0) {
                            FldFileResource *resource = fldRoomResources[slotIndex];
                            const f32 *transform = resource->transform;
                            row->actorId = resource->id;
                            rotation[0] = transform[4];
                            rotation[1] = transform[5];
                            rotation[2] = transform[6];
                            rotation[3] = transform[7];
                            SDF_VU0_LOADVECTOR(rotation);
                            angle = EulerAngleAxisY() * 180.0f / 3.14f;
                            while (angle < 0.0f) {
                                angle += 360.0f;
                            }
                            while (angle > 360.0f) {
                                angle -= 360.0f;
                            }
                            if (45.0f <= angle && angle <= 135.0f) {
                                secondDirection = 1;
                            } else if (135.0f <= angle && angle <= 225.0f) {
                                secondDirection = 0;
                            } else if (225.0f <= angle && angle <= 315.0f) {
                                secondDirection = 3;
                            } else {
                                secondDirection = 2;
                            }
                        }
                    }
                }
            }
            row->transitionKey = -1;
            if ((actor->flags54 & 2) == 0 && actor->taskName[0] != '\0') {
                node = dds3SceneBasicSearchNameAndType(world, 6, (const u8 *)actor->taskName);
                if (node != NULL) {
                    row->transitionKey = node->key;
                }
            }
            primaryMotion = actor->motion;
            row->kind = 1;
            row->secondaryMotion = actor->secondaryMotion;
            row->unk08 = primaryMotion;
            row->firstFrame = 0;
            row->secondFrame = 0;
            row->transitionFrame = 0;
            row->firstValues[0] = 0.0f;
            row->firstValues[1] = 0.0f;
            row->firstValues[2] = 0.0f;
            row->secondValues[0] = 0.0f;
            row->secondValues[1] = 0.0f;
            row->secondValues[2] = 0.0f;
            if (row->firstKey != (u32)-1) {
                switch (primaryMotion) {
                case 1:
                    switch (firstDirection) {
                    case 0:
                        row->firstStep[0] = -7.50f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 1:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 7.50f;
                        break;
                    case 2:
                        row->firstStep[0] = 7.50f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 3:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = -7.50f;
                        break;
                    }
                    break;
                case 2:
                    switch (firstDirection) {
                    case 0:
                        row->firstStep[0] = 7.50f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 1:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = -7.50f;
                        break;
                    case 2:
                        row->firstStep[0] = -7.50f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 3:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 7.50f;
                        break;
                    }
                    break;
                case 3:
                    row->firstStep[0] = 0.00f;
                    row->firstStep[1] = -7.50f;
                    row->firstStep[2] = 0.00f;
                    break;
                case 4:
                    row->firstStep[0] = 0.00f;
                    row->firstStep[1] = 7.50f;
                    row->firstStep[2] = 0.00f;
                    break;
                case 5:
                    switch (firstDirection) {
                    case 0:
                        row->firstStep[0] = -3.75f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 1:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 3.75f;
                        break;
                    case 2:
                        row->firstStep[0] = 3.75f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 3:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = -3.75f;
                        break;
                    }
                    break;
                case 6:
                    switch (firstDirection) {
                    case 0:
                        row->firstStep[0] = 3.75f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 1:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = -3.75f;
                        break;
                    case 2:
                        row->firstStep[0] = -3.75f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 0.00f;
                        break;
                    case 3:
                        row->firstStep[0] = 0.00f;
                        row->firstStep[1] = 0.00f;
                        row->firstStep[2] = 3.75f;
                        break;
                    }
                    break;
                case 7:
                    row->firstStep[0] = 0.00f;
                    row->firstStep[1] = -3.75f;
                    row->firstStep[2] = 0.00f;
                    break;
                case 8:
                    row->firstStep[0] = 0.00f;
                    row->firstStep[1] = 3.75f;
                    row->firstStep[2] = 0.00f;
                    break;
                default:
                    row->firstStep[0] = 0.0f;
                    row->firstStep[1] = 0.0f;
                    row->firstStep[2] = 0.0f;
                    break;
                }
            } else {
                row->firstStep[0] = 0.0f;
                row->firstStep[1] = 0.0f;
                row->firstStep[2] = 0.0f;
            }
            if (row->secondKey != (u32)-1) {
                switch (row->secondaryMotion) {
                case 1:
                    switch (secondDirection) {
                    case 0:
                        row->secondStep[0] = -7.50f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 1:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 7.50f;
                        break;
                    case 2:
                        row->secondStep[0] = 7.50f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 3:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = -7.50f;
                        break;
                    }
                    break;
                case 2:
                    switch (secondDirection) {
                    case 0:
                        row->secondStep[0] = 7.50f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 1:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = -7.50f;
                        break;
                    case 2:
                        row->secondStep[0] = -7.50f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 3:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 7.50f;
                        break;
                    }
                    break;
                case 3:
                    row->secondStep[0] = 0.00f;
                    row->secondStep[1] = -7.50f;
                    row->secondStep[2] = 0.00f;
                    break;
                case 4:
                    row->secondStep[0] = 0.00f;
                    row->secondStep[1] = 7.50f;
                    row->secondStep[2] = 0.00f;
                    break;
                case 5:
                    switch (secondDirection) {
                    case 0:
                        row->secondStep[0] = -3.75f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 1:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 3.75f;
                        break;
                    case 2:
                        row->secondStep[0] = 3.75f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 3:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = -3.75f;
                        break;
                    }
                    break;
                case 6:
                    switch (secondDirection) {
                    case 0:
                        row->secondStep[0] = 3.75f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 1:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = -3.75f;
                        break;
                    case 2:
                        row->secondStep[0] = -3.75f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 0.00f;
                        break;
                    case 3:
                        row->secondStep[0] = 0.00f;
                        row->secondStep[1] = 0.00f;
                        row->secondStep[2] = 3.75f;
                        break;
                    }
                    break;
                case 7:
                    row->secondStep[0] = 0.00f;
                    row->secondStep[1] = -3.75f;
                    row->secondStep[2] = 0.00f;
                    break;
                case 8:
                    row->secondStep[0] = 0.00f;
                    row->secondStep[1] = 3.75f;
                    row->secondStep[2] = 0.00f;
                    break;
                default:
                    row->secondStep[0] = 0.0f;
                    row->secondStep[1] = 0.0f;
                    row->secondStep[2] = 0.0f;
                    break;
                }
            } else {
                row->secondStep[0] = 0.0f;
                row->secondStep[1] = 0.0f;
                row->secondStep[2] = 0.0f;
            }
        }
    }
}

void fldBeginNpcInteractionById(s32 id) {
    s32 i;
    FldActorRow *npc;
    FldActorEntry *actor;

    if (id == 0) {
        return;
    }
    if (id == -1) {
        return;
    }
    if (D_00389780[0] == 0x1D || D_00389780[0] == 0x1E) {
        for (i = 0; i < 0x100; i++) {
            actor = &fldDoorTable[i];
            npc = &fldActorSlots[i];
            if (npc->kind == 1 && npc->actorId == id) {
                fldApplyRoomObjectModeZero(0, 0, (char *)actor->motionName, 0);
                return;
            }
        }
    } else {
        for (i = 0; i < 256; i++) {
            actor = &fldDoorTable[i];
            npc = &fldActorSlots[i];
            if (npc->kind == 1 && npc->actorId == id) {
                npc->kind = 2;
                npc->firstFrame = 0;
                npc->secondFrame = 0;
                npc->transitionFrame = 0;
                if (actor->motion == 5 || actor->secondaryMotion == 5 || actor->motion == 6 || actor->secondaryMotion == 6
                    || actor->motion == 7 || actor->secondaryMotion == 7 || actor->motion == 8 || actor->secondaryMotion == 8) {
                    npc->frameCount = 0x28;
                } else {
                    npc->frameCount = 0x14;
                }
                npc->firstValues[0] = 0;
                npc->firstValues[1] = 0;
                npc->firstValues[2] = 0;
                npc->secondValues[0] = 0;
                npc->secondValues[1] = 0;
                npc->secondValues[2] = 0;
                fldApplyPendingCameraHeading();
            }
        }
    }
}

#include "sdf_debug_api.h"

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value);

s32 fldStepActorMotionByKey(u32 key, f32 *x, f32 *y, f32 *z) {
    f32 weights[50] = {
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f
    };
    s32 i;
    FldActorRow *row;
    dds3Basic_t *node;

    if (D_00389780[0] >= 200) {
        return 0;
    }
    for (i = 0; i < FIELD_ACTOR_SLOT_COUNT; i++) {
        row = &fldActorSlots[i];
        if (row->firstKey == key) {
            if (row->kind != 2) {
                continue;
            }
            if (row->firstFrame < row->frameCount) {
                if (row->frameCount == 40) {
                    row->firstValues[0] += row->firstStep[0] * weights[row->firstFrame];
                    row->firstValues[1] += row->firstStep[1] * weights[row->firstFrame];
                    row->firstValues[2] += row->firstStep[2] * weights[row->firstFrame];
                } else {
                    row->firstValues[0] += row->firstStep[0];
                    row->firstValues[1] += row->firstStep[1];
                    row->firstValues[2] += row->firstStep[2];
                }
                row->firstFrame++;
            }
            *x = row->firstValues[0];
            *y = row->firstValues[1];
            *z = row->firstValues[2];
            return 1;
        } else if (row->secondKey == key) {
            if (row->kind != 2) {
                continue;
            }
            if (row->secondFrame < row->frameCount) {
                if (row->frameCount == 40) {
                    row->secondValues[0] += row->secondStep[0] * weights[row->secondFrame];
                    row->secondValues[1] += row->secondStep[1] * weights[row->secondFrame];
                    row->secondValues[2] += row->secondStep[2] * weights[row->secondFrame];
                } else {
                    row->secondValues[0] += row->secondStep[0];
                    row->secondValues[1] += row->secondStep[1];
                    row->secondValues[2] += row->secondStep[2];
                }
                row->secondFrame++;
            }
            *x = row->secondValues[0];
            *y = row->secondValues[1];
            *z = row->secondValues[2];
            return 1;
        } else if (row->transitionKey == key) {
            if (row->kind != 2) {
                continue;
            }
            if (row->transitionFrame == 0) {
                node = dds3SearchSceneBasic(dds3GetControlScene(), key, 6);
                if (node != NULL) {
                    printf("DOOR SISETU FADE 6\n");
                    dds3MatterObjectBasicSetAlpMode(node, 9);
                }
            }
            row->transitionFrame++;
            *x = 0.0f;
            *y = 0.0f;
            *z = 0.0f;
            return 1;
        }
    }
    return 0;
}

void func_00143C90(void) {
}

extern s32 fldApplyRoomObjectModeOne(s32 world, s32 stage, char *roomName, s32 mode);

void fldApplyCurrentAreaActorEntries(void) {
    FldActorEntry *entry;
    s32 i;

    for (i = 0; i < 256; i++) {
        entry = &fldDoorTable[i];
        if (entry->kind == 1 && entry->floor == fldGlobalBase.floor + 1 && entry->motion != 0) {
            fldApplyRoomObjectModeOne(0, 0, (char *)entry->motionName, 0);
        }
        if (entry->kind == 11 && entry->floor == fldGlobalBase.floor + 1 && entry->motion == 3) {
            fldApplyRoomObjectModeOne(0, 0, (char *)entry->otherName, 0);
        }
    }
}

extern s32 D_00399F90[];

void fldDrawTitleBannerFrame(s32 mode) {
    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSubmitTexturedGradientQuad(0x97, 0x128, 0x3A, 0x24, 1, 2, 0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldGlobalBase.fieldTextures[1].texture);
    evtSubmitTexturedGradientQuad(0xD1, 0x128, 0x5E, 0x24, 0x3A, 2, 1, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldGlobalBase.fieldTextures[1].texture);
    evtSubmitTexturedGradientQuad(0x12F, 0x128, 0x3A, 0x24, 0x3B, 2, -0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldGlobalBase.fieldTextures[1].texture);
    if (mode < 0x18) {
        evtSubmitPrimaryAlphaBlendMode(1);
        evtSubmitTexturedGradientQuad(0xA0, 0x12B, 0x22, 0x24, 1, 0x21, 0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], fldGlobalBase.fieldTextures[1].texture);
        evtSubmitTexturedGradientQuad(0x13E, 0x12B, 0x22, 0x24, 0x23, 0x21, -0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], fldGlobalBase.fieldTextures[1].texture);
        evtSubmitPrimaryAlphaBlendMode(0);
    }
}

extern s32 D_00389978[];

void fldDrawTitleBannerCursor(s32 x, s32 y) {
    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSubmitTexturedGradientQuad(x, y, 0x12, 0x13, 1, 0x25, 0x12, 0x13, 0x80808080, 0x80808080, 0x80808080, 0x80808080, (struct sdfTexHandle_t *)(u32)(D_00389978[0]));
    evtSubmitPrimaryAlphaBlendMode(0);
}

s32 fldUpdateTitleBannerTask(dds3ProcessID_t *task) {
    FldTitleBannerMenu *menu;

    if (D_00389780[0] >= 200) {
        return 0;
    }
    if (fldProcCampChk() != FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET) {
        return 0;
    }
    if (fldProcAmChk() != 0) {
        return 0;
    }
    if (fileMenuTaskExists() != 0) {
        return 0;
    }
    if (fldHasKiretaLabelProcess() != 0) {
        return 0;
    }
    if (fldProcKiretaChk() != 0) {
        return 0;
    }
    if (fldProcHirakenaiChk() != 0) {
        return 0;
    }
    if (fldIsEventPhaseAtLeastTwo() != 0) {
        return 0;
    }
    menu = (FldTitleBannerMenu *)dds3GetProcessData(task);
    if (menu->pending != 0) {
        fldDrawTitleBannerFrame(menu->position);
        fldDrawTitleBannerCursor(D_00399F60[menu->choice] - 9, 0x130);
        itfSetTextDrawLimit(0x13);
        fldKanjiDraw(D_00399F60[menu->choice] + 10, 0x98, 0xA09DC380, D_00399EA0[menu->choice]);
        itfSetTextDrawLimit(-1);
        menu->position = menu->position + 1;
        menu->pending = 0;
    } else {
        menu->position = 0;
    }
    return 0;
}

s32 fldInputPanelProcess1(dds3ProcessID_t *task) {
    FldTitleBannerMenu *menu = sdfChipAllocDirect(sizeof(FldTitleBannerMenu));
    menu->choice = 1;
    menu->position = 0;
    menu->pending = 0;
    menu->reserved = 0;
    dds3SetProcessData(task, (u32)menu);
    return (s32)fldUpdateTitleBannerTask;
}

INCLUDE_SDATA(const s32, "game/code_0013B818", fldActorTaskRoom);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldEventSetRoom);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldLabelTaskRoom);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldLabelTaskSlot);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldEveRngCount);

INCLUDE_SDATA(const s32, "game/code_0013B818", gFldEveRngIdx);

INCLUDE_SDATA(const s32, "game/code_0013B818", gFldEveRngIdx_N);

INCLUDE_SDATA(const s32, "game/code_0013B818", gFldEveRngIdx_T);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldTriggeredSavePointValue);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361B8);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldEveHitState);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361C0);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldActionRowCount);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361C8);

INCLUDE_SDATA(const s32, "game/code_0013B818", gFldInf_HitIdx);

INCLUDE_SDATA(const s32, "game/code_0013B818", gFldInf_WpExit);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361D4);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361D8);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361DC);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361E0);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361E4);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361E8);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361EC);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361F0);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldSelectedActorEntryIndex);

INCLUDE_SDATA(const s32, "game/code_0013B818", D_004361F8);

INCLUDE_SDATA(const s32, "game/code_0013B818", fldPickedActorTemplateKind);

INCLUDE_SDATA(const s32, "game/code_0013B818", addressprocess_id);
