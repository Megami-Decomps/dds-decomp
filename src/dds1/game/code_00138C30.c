#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "sdf_gs_header.h"
#include "sdf_pac_reloc.h"
#include "fld_script_resource.h"
#include "sdf_packet_header.h"
#include "eff.h"
#include "fld_player_scene_state.h"
#include "fld_marker_packet.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "sdf_vu0.h"
#include "fld_stop_entry.h"
#include "sdf_resource_list_release_api.h"
#include "kwln_background_fade_api.h"
#include "fld_scene_ready_api.h"
#include "fld_scene_record_api.h"
#include "fld_light_set.h"
#include "fld_sky_buffer.h"
#include "fld_sky_gradient_record.h"
#include "fld_quad_state.h"
#include "kwln_frame_draw_buffer.h"
#include "kwln_draw_buffer.h"
#include "kwln_fade.h"
#include "mnu_title_stream.h"
#include "fld_bill_config.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "fld_npc_motion.h"
#include "fld_npc_palette.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "fld_resource_resolver.h"
#include "fld_clear_menu_entries_api.h"
#include "fld_play_pending_sounds_api.h"
#include "fld_release_texture_slots_api.h"
#include "fld_transfer_chunk.h"
#include "fld_camera_follow_row.h"
#include "fld.h"
#include "fld_encounter_runtime_api.h"
#include "fld_collision_geometry.h"
#include "fld_scene_request.h"
#include "fld_spawn_event.h"
#include "sdf_resource.h"
#include "dds3obj.h"
#include "dds3_world_value.h"
#include "sdf_texture_file.h"
#include "sdf_texture_primary_buffer_api.h"
#include "evt_action_object.h"
#include "sdf_texture_offset_list.h"
#include "evt_script_model.h"
#include "sdf_model.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "sdf_vu_matrix.h"
#include "dds3_world_secondary.h"
#include "eff_light.h"
#include "fld_area_work.h"
#include "fld_spawn_collision.h"
#include "evt_world.h"
#include "evt_world_source_transform.h"
#include "sdf_chip.h"
#include "sdf_column_packet_api.h"
#include "sdf_packet_append.h"
#include "sdf_packet_builders.h"
#include "fr_font.h"
#include "sdf_packet_list.h"
#include "sdf_dev_state.h"
#include "fld_waypoint.h"
#include "fld_inf.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "file_pac.h"
#include "fld_packed_resource_kind.h"
#include "fld_title_banner_menu.h"
#include "field_stage.h"
#include "sdf_pac_work.h"
#include "sdf_primitive_packet_api.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "mdl.h"
#include "scr.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "dat_state.h"
#include "dds3_world_primary.h"
#include "sdf_packet_alloc.h"
#include "fld_object_slot_api.h"
#include "fld_placement_data.h"
#include "fld_resource_name.h"
#include "eff_object.h"
#include "dds3_path.h"
#include "fld_label_cache_api.h"
#include "fld_sprite_vertex.h"
#include "fld_room_state.h"
#include "fld_trigger_shape.h"
#include "fld_action_row.h"
#include "fld_actor_row.h"
#include "mdl_world_object_source_vector_api.h"
#include "btl.h"
#include "fld_index_pair.h"
/* Declared before first use (were implicit int). */
extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);
extern void fldMarkStageExplorationBitmapRegion(s32, s32, s32, s32, s32, s32);
extern void fldSetFlagBit(s32 area, s32 floor, s32 bit);

extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);
extern void datAddCurrencyClamped(s32 delta);
extern void scrDestroyAllNamedProcesses(void);
extern void fldSetDeferredFieldCommand(u32 command, u32 parameter);
extern void evtSubmitEventRequestImmediate(s32 eventId);

struct dds3Basic_t;


extern char D_003BAC40[];



extern FldInfTable D_00332E30;

extern void fldPlayFieldSeVolumePan(s32);

extern void kwlnFadeSetRGB(s32 red, s32 green, s32 blue);

extern void fldReleaseActorTasksById(s32);

/* Reset by fldResetActorSlots and scanned by fldReleaseActorTasksById. */
extern FldActorRow fldActorSlots[];

extern s32 D_0032E530[];

extern FldNpcMotion D_00336A60[];

extern char *D_003BAE44;

extern char *D_003BAE48;

static inline s32 fldTestBits(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

/* vu0 routine: normalize vf10 and return its original XYZ length. */
static inline f32 fldNormalizeProbeVector(void) {
    f32 length;

    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    return length;
}

extern s32 D_0032E400[];

extern u8 fldEncountTestFlag(u32);

extern s32 D_003BAD24;

extern s32 D_003BAD28;

extern s32 D_0032E3C0[];

extern s8 D_0032C9A0[];

extern const char *fldSelectCurrentActorOnNextFloor(void);

extern s32 D_003BAE68;

extern FldWallMeshRecord *fldItemBoxObjBuf;

extern FldValueRecord *fldMatterTable;

extern struct sdfMemHandle_t *fldAuxRecordResource;

extern struct sdfMemHandle_t *fldValueRecordResource;

extern u32 gFilterAlpha;

extern u32 gFilterAnime;

extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern sdfTexHandle_t *fldRainTextureReference;

extern dds3Basic_t *fldPlayerObject;

extern u8 D_003BAB3C;

extern u32 D_003BAD34;

extern u32 D_003BAD38;

extern u32 D_003BAD3C;

extern u32 D_003BAD40;

extern s32 D_003BAD1C;

extern u32 D_003BACF8;

extern s32 fldDrwZBuf;

extern u32 rsetFrameMaskOtOffset;

extern u32 gFldFileResetMode;

extern u32 fldPendingArea;

extern u32 fldPendingFloor;

extern u32 D_003BAD98;

extern u32 D_003BAD9C;

extern f32 D_003BADA0;

extern u32 D_003BADA4;

extern u32 D_003BADC8;

extern u32 D_003BADD8;

extern u32 D_003BAE28;

extern s32 gFldInf_HitIdx;

extern u32 fldSelectedActorEntryIndex;

extern u32 fldRunDanteGunFlag[];

extern s32 D_003BAE38;

extern u32 D_003BACD4;

extern u32 D_003BACEC;

extern sdfTexHandle_t *fldMarkerTexture;

extern FldFileResource *fldDisplayValues[];

extern s16 D_003C9518[];


extern f32 D_003BAD20;

extern char fldEncounterTaskName[];

extern struct FilePacRequest *pFldNextFileDataF2;

extern u8 D_003BACE0[];

extern u8 D_003BACE8[];

extern s32 fldEncounterRuntimeState;

extern u32 D_003BACFC;

extern dds3ProcessID_t *fldRoomTaskHandles[];


extern s32 fldUpdateTitleBannerTask(dds3ProcessID_t *task);

extern s32 fldValueRecordCount;

extern s32 D_003BADF8;

extern s32 D_003BADEC;


extern struct sdfMemHandle_t *fldFieldPackData;

extern struct FileRequest *gFldFileF1;

extern u8 D_003BAC90[];

extern s32 D_003BAE30;

extern s32 gFldEveRngIdx_N;

extern s32 fldEveRngCount;

extern s16 D_003C9510[];

extern u32 D_003BAE4C;

extern s32 D_003BAE50;

extern void fldResetZoneRecordsAndActorSlots();

extern FldWaypointBlock fldActorWaypointRows;

extern FldFileResource *fldRoomResources[];

extern void fldDrawPos(const f32 position[3]);

extern FldIndexPair D_0032EF18[];

extern FldIndexPair D_0032EFE0[];

extern s32 D_003BAE54;

extern s32 D_003BAE5C;

extern s32 D_003BAE60;

extern char D_003BAC40[];

extern s32 fldRoomEventPlacementCount;

extern dds3Basic_t *dds3AddEventBasic(s32 key, const char *resourcePath, const char *name);

extern u32 fldEveHit_AddEveData(FldFileResource *, dds3Basic_t *);

extern s32 fldRegisterRoomObject(FldFileResource *, dds3Basic_t *);

extern struct DevRequest *fldRoomSoundResources;

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *, u32);



extern s32 D_003BAC80;

extern const u8 *fldCachedRoomResourceData, *fldCachedRoomF1ResourceData, *fldCachedRoomF2ResourceData,
    *fldCachedRoomKF2ResourceData;

extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;

extern void fldGetDoorEntry(FldWaypointBlock *source);

extern void fldInfLoadFromMem(const FldInfTable *source);

extern s32 fldRoomEventPlacementCount;

extern u32 gFldFileResetMode;

extern s32 gFldInf_WpExit;

extern FldTransferChunk *fldRoomF1Chunk;

extern u32 fldRoomF1Data;

extern FldTransferChunk *fldRoomF2Chunk;

extern u32 fldRoomF2Data;

extern FldTransferChunk *fldRoomKF2Chunk;

extern u32 fldRoomKF2Data;

extern struct FilePacRequest *D_003BAC38;

extern struct sdfMemHandle_t *D_003BAC4C;

extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;

extern struct sdfMemHandle_t *fldFieldPackData;

extern void fldInitializeActorMotionSlots(void);

extern char D_003BAC98[];

extern char D_003BACA0[];

extern char D_003BACA8[];

extern s32 fldRoomEventPlacementCount;

extern FldTransferChunk *fldRoomF1Chunk;

extern u32 fldRoomF1Data;

extern FldTransferChunk *fldRoomF2Chunk;

extern u32 fldRoomF2Data;

extern FldTransferChunk *fldRoomKF2Chunk;

extern u32 fldRoomKF2Data;

extern struct FilePacRequest *D_003BAC38;

extern struct sdfMemHandle_t *D_003BAC4C;

extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;



void fldDbgPrint2D(s32 x, s32 y, const char *format);

extern char D_003BACB0[];

extern char D_003BACB8[];

extern char D_003BACC0[];

extern char D_003BACB0[];

extern char D_003BACB8[];

extern char D_003BACC0[];

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern f32 EulerAngleAxisY(void);

extern f32 D_003BAD10, D_003BAD14, D_003BAD18;

extern f32 EulerAngleAxisY(void);

extern s32 D_003BAD44;

extern s32 D_003BAD2C;

extern s32 D_003BAD30;

extern s32 D_003BAD48;

extern s32 D_003BAD4C;

extern f32 D_003BAD50;

extern s32 D_003BAD54;

extern void fldDisableCameraObjectFlag(void);

extern FldCameraSetting *fldCameraSettings;

extern void *D_003BAD60;

extern FldSkyGradientRecord *D_003BAD74;

extern s32 D_003BAD88, D_003BAD8C;

extern f32 D_003BAD90, D_003BAD94;

extern s32 D_003BAD58;

extern u32 D_003BADA8;

extern s32 D_0032E3C0[];

extern s32 D_003BAD58;

/* The resolver's selected-light transition cache is stored in these existing
 * D1 sdata words; the light record itself is the complete 0xE0-byte owner. */
extern u32 D_003BADA8;

extern u32 D_003BADAC;

extern f32 D_003BADB0;

extern s32 D_003BADB4;

extern s32 D_003BADB8, D_003BADBC;

extern f32 D_003BADC0, D_003BADC4;

extern u32 D_003BADCC;

extern f32 D_003BADD0;

extern u32 D_003BADD4;

extern u32 D_003BADDC;

extern s32 fldCameraColorFade;

extern s32 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit);

void fldApplyPendingCameraHeading(void);

/* Build a normalized plane from three vertices spaced four floats apart. * Snap near-zero normal components before deriving D from the third vertex. * Degenerate triangles retain the native divide-by-zero behavior. */ void fldMakeEveHitPlanes(const f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant);

void fldClearCameraFacingPointRequest(void);

void fldClearCameraHeadingRequest(void);

void fldKanjiDraw(s32 x, s32 y, u32 colors, const u8 *text);

void fldEnableCameraObjectFlag(void);

/* All 25 code words match. Check_unit rodata range extends beyond selector strings into the separately-owned next string. */ void fldMakeFilePath(char *path, s32 field, s32 unused);

/* Suppress the world object's current entry when it is already selected. */ dds3Basic_t *fldCamNowEveCamera(void);

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z);

/* Camera facing requests share the field-work block. Both request states * advance from 1 to 2 when their new angle is installed. */ void fldQueueCameraXYOverride(f32 targetX, f32 targetZ);

s32 fldRequestAreaResource(s32 area, s32 room);

void rsetOtOfs(u32 displayRow);

/* Write the selected blend equation to GS ALPHA_1. */ void rsetAlpha(s32 mode);

void ooyTestLine3D(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w);

void fldUpdateCameraViewFromPlayer(void);

void fldUpdateFieldCameraViewState(void);

void evtMessageExtraActionNoOp(void);

typedef struct FldRoomPlanes {
    f32 plane[6][4];
    f32 limit[6];
    u8 pad78[0x140 - 0x78];
} FldRoomPlanes;

extern FldRoomPlanes D_003C9470[];

extern f32 fldVecDot(const f32 *, const f32 *);

extern FldRoomState fldEveHitTable[];

extern void fldResetActorSlots(void);

extern s32 fldActorTaskRoom;

extern s32 fldEventSetRoom;

extern s32 fldLabelTaskRoom;

extern s32 fldLabelTaskSlot;

extern s32 gFldEveRngIdx;

extern s32 gFldEveRngIdx_N;

extern s32 gFldEveRngIdx_T;

extern s32 fldEveHitState;

extern s32 fldActionRowCount;

/* Reset room records and selection sentinels, then apply actor-slot defaults. */
void fldResetZoneRecordsAndActorSlots(void) {
    s32 roomIndex;
    s32 vectorIndex;

    fldEveRngCount = 0;
    D_003BAE28 = 0;
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
        fldEveHitTable[roomIndex].mode = 0;
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

extern s32 gFldEveRngIdx;

extern s32 gFldEveRngIdx_T;

extern s32 fldEveHitState;

/* Clear slot handles and destroy named tasks reached through linked display values. */
void fldEveHit_End(void) {
    s32 slotIndex;
    void *world;
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
                task = dds3EventBasicGetResource((dds3Basic_t *)dds3SearchSceneBasic(world, fldDisplayValues[taskInfo->slot]->id, 0xD));
                if (scrSearchScriptName(task) != 0) {
                    dds3SceneBasicEndScript(dds3GetActiveScene(), task);
                }
            }
        }
    }
}

/* Append a file resource for an event task and return its index. */
u32 fldEveHit_AddEveData(FldFileResource *resource, dds3Basic_t *object) {
    s32 index = D_003BAE28;

    fldDisplayValues[index] = resource;
    D_003BAE28 = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "game/code_00138C30", fldRegisterRoomObject);

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
        if (fldVecDot(probe, D_003C9470[roomIndex].plane[i + 1]) - D_003C9470[roomIndex].limit[i + 1] < 0.0f) {
            return -1;
        }
    }
    return 0;
}

s32 fldRoomContainsPoint(f32 *point, s32 room) {
    FldRoomPlanes *planes = &D_003C9470[room];
    f32 *limit = planes->limit;
    f32 *plane = planes->plane[0];
    s32 i;

    for (i = 0; i < 6; i++) {
        if (fldVecDot(point, plane) - *limit > 0.0f) {
            return 0;
        }
        plane += 4;
        limit++;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00138C30", fldStartRoomEventTask);

s32 fldEveHit_ScriptEnd(char *flag, u32 slot) {
    fldRoomTaskHandles[slot] = NULL;
    if (scrSearchScriptName(flag) != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), flag);
        return 0;
    }
    return -1;
}

u32 fldEveHit_ScriptEnd2(u32 slot) {
    dds3ProcessID_t **task = &fldRoomTaskHandles[slot];

    if (dds3ExistProcess(*task) != 0) {
        dds3KillProcess(*task, 0);
    }
    *task = NULL;
    return 0;
}

s32 fldEveHit_ChkEveHitAfter(void) {
    const char *object;
    u32 state = D_003BAB3C;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && D_0032C9A0[0] != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), (const char *)D_0032C9A0);
    }
    D_0032C9A0[0] = 0;
    D_003BAB3C = 0;
    object = fldSelectCurrentActorOnNextFloor();
    if (scrSearchScriptName(object) == 0) {
        evtStartSceneResourceTask(dds3GetActiveScene(), object);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00138C30", fldUpdateRoomTriggers);

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
            if (dds3ExistProcess(*taskSlot) == 0) {
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

extern s32 fldEveRngCount;

/* Return the room ID stored for the matching task; -1 denotes a miss. */
s32 fldFindRoomIdByTask(dds3ProcessID_t *task) {
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

extern char D_00330760[];

extern char fldHitProgramName[];

extern dds3Basic_t *dds3AddEventBasic(s32, const char *, const char *);

extern char D_00330780[];

void fldSpawnRoomTrigger(s32 roomId, f32 x, f32 y, f32 z, f32 radius) {
    s32 index = fldActionRowCount;
    FldActionTransform *transform;
    s32 worldKey = dds3AllocObjectId();
    dds3Basic_t *object;
    s32 taskIndex;

    object = dds3AddEventBasic(worldKey, D_00330780, D_00330760);
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
    fldActionRows[index].childResource.name = (const char *)D_00330760;
    object = dds3AddPointBasic(fldActionRows[index].childResource.id,
                                 (EvtWorldSourceTransformPrefix *)fldActionRows[index].childResource.transform,
                                 fldHitProgramName);
    taskIndex = fldRegisterRoomObject(&fldActionRows[index].childResource, object);
    fldEveHitTable[taskIndex].roomId = roomId;
    fldEveHitTable[taskIndex].mode = 2;
    fldActionRowCount++;
    fldEveHitTable[taskIndex].entryIndex = -1;
}

INCLUDE_ASM(const s32, "game/code_00138C30", fldSpawnNpcTrigger);

extern char D_00330790[];

extern char D_003307A0[];

void fldSpawnRoomTriggerVariant(s32 kind, s32 roomValue,
                                f32 x, f32 y, f32 z, f32 radius) {
    s32 index = fldActionRowCount;
    FldActionTransform *transform;
    s32 worldKey = dds3AllocObjectId();
    dds3Basic_t *object;
    s32 taskIndex;

    if (kind == 0) {
        object = dds3AddEventBasic(worldKey, D_003307A0, D_00330760);
    } else {
        object = dds3AddEventBasic(worldKey, D_00330790, D_00330760);
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
    fldActionRows[index].childResource.name = (const char *)D_00330760;
    object = dds3AddPointBasic(fldActionRows[index].childResource.id,
                                 (EvtWorldSourceTransformPrefix *)fldActionRows[index].childResource.transform,
                                 fldHitProgramName);
    taskIndex = fldRegisterRoomObject(&fldActionRows[index].childResource, object);
    if (kind == 0) {
        fldEveHitTable[taskIndex].mode = 3;
        fldEveHitTable[taskIndex].savePointValue = roomValue;
    } else {
        fldEveHitTable[taskIndex].mode = 4;
    }
    fldActionRowCount++;
    fldEveHitTable[taskIndex].entryIndex = -1;
}

s32 fldCheckEntryActive(s32 entry) {
    s32 entryIdx = gFldEveRngIdx_N;
    s16 *slot;

    if (D_003BAE30 != 0) {
        return 1;
    }
    if (entryIdx == -1) {
        return 0;
    }
    slot = D_003C9510 + entryIdx * 0xA0;
    if (slot[3] == 1 && slot[4] == entry) {
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
    s32 slot = gFldInf_HitIdx;

    if (slot < 0) {
        return -1;
    }
    return D_003C9518[slot * 160];
}

extern s32 D_003BAE38;

extern const char D_00332DF0[];


const char *fldCheckNpcTalk(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (D_00332E30.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (D_00332E30.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, D_00332E30.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = D_00332E30.packs[packOrSetIndex].setIndex;
                    if (D_00332E30.sets[D_003BAE38].kindArea.packed == 1) {
                        D_003BAE38 = candidateSet;
                        return D_00332DF0;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (D_00332E30.sets[packOrSetIndex].kindArea.packed == 1 &&
            D_00332E30.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, D_00332E30.sets[packOrSetIndex].eventName) == 0) {
            D_003BAE38 = packOrSetIndex;
            return D_00332DF0;
        }
    }
    return 0;
}

const char *fldCheckNpcTalk2(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (D_00332E30.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (D_00332E30.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, D_00332E30.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = D_00332E30.packs[packOrSetIndex].setIndex;
                    if (D_00332E30.sets[D_003BAE38].kindArea.packed == 0) {
                        D_003BAE38 = candidateSet;
                        return D_00332DF0;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (D_00332E30.sets[packOrSetIndex].kindArea.packed == 0 &&
            D_00332E30.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, D_00332E30.sets[packOrSetIndex].eventName) == 0) {
            D_003BAE38 = packOrSetIndex;
            return D_00332DF0;
        }
    }
    return 0;
}

const char *fldInf_FindTypeEveNpc(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (D_00332E30.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (D_00332E30.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, D_00332E30.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    D_003BAE38 = D_00332E30.packs[packOrSetIndex].setIndex;
                    return D_00332DF0;
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if ((u16)D_00332E30.sets[packOrSetIndex].kindArea.packed < 2 &&
            D_00332E30.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, D_00332E30.sets[packOrSetIndex].eventName) == 0) {
            D_003BAE38 = packOrSetIndex;
            return D_00332DF0;
        }
    }
    return 0;
}

extern s32 fldGetActorStat0(s32 attribute);

/* Follow the INF flag/message graph to its terminal event or actor action.
 * Linked selectors recurse in tail position; the native routine loops without
 * growing the stack. Targets and eventHit are signed serialized bytes. */
extern const char *fldFindActorEntryByName(const char *);

INCLUDE_RODATA(const s32, "game/code_00138C30", D_003A0150);

s32 fldResolveEventSelectorTarget(s32 selector) {
    s32 target;
    s32 index;

    if (selector < 10) {
        index = selector - 1;
        if (D_00332E30.sets[D_003BAE38].flags[index].flag == 0) {
            target = (s8)D_00332E30.sets[D_003BAE38].flags[index].offTarget;
        } else if (evtBitCheck(D_00332E30.sets[D_003BAE38].flags[index].flag)) {
            target = (s8)D_00332E30.sets[D_003BAE38].flags[index].onTarget;
        } else {
            target = (s8)D_00332E30.sets[D_003BAE38].flags[index].offTarget;
        }
    } else {
        index = selector - 10;
        target = (s8)D_00332E30.sets[D_003BAE38].messages[index].targets[0];
    }
    switch (target) {
    case 0:
        return 0;
    case 100:
        fldInf_FindTypeEveNpc(fldRoomResources[gFldInf_HitIdx]->name);
        switch ((s8)D_00332E30.sets[D_003BAE38].eventHit) {
        case 0: return 199;
        case 4: return 104;
        case 5: return 105;
        case 6: return 106;
        case 7: return 107;
        }
        fldFindActorEntryByName(fldRoomResources[gFldInf_HitIdx]->name);
        switch (D_003BAE68) {
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
        if (D_00332E30.sets[D_003BAE38].messages[index].kind == -1) {
            return fldResolveEventSelectorTarget(target);
        }
        if ((u16)D_00332E30.sets[D_003BAE38].messages[index].kind < 2) {
            switch ((s8)D_00332E30.sets[D_003BAE38].eventHit) {
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
        if (D_00332E30.sets[D_003BAE38].flags[row].flag == 0) {
            target = (s8)D_00332E30.sets[D_003BAE38].flags[row].offTarget;
        } else if (evtBitCheck(D_00332E30.sets[D_003BAE38].flags[row].flag) != 0) {
            target = (s8)D_00332E30.sets[D_003BAE38].flags[row].onTarget;
        } else {
            target = (s8)D_00332E30.sets[D_003BAE38].flags[row].offTarget;
        }
    } else {
        row = messageId - 10;
        target = (s8)D_00332E30.sets[D_003BAE38].messages[row].targets[slot];
    }
    if (target >= 10) {
        if (D_00332E30.sets[D_003BAE38].messages[target - 10].flags[0] != 0) {
            evtBitOff(D_00332E30.sets[D_003BAE38].messages[target - 10].flags[0]);
        }
        if (D_00332E30.sets[D_003BAE38].messages[target - 10].flags[1] != 0) {
            evtBitOn(D_00332E30.sets[D_003BAE38].messages[target - 10].flags[1]);
        }
    }
    world = dds3GetActiveScene();
    if (target >= 10) {
        extraAction = (s8)D_00332E30.sets[D_003BAE38].messages[target - 10].extraAction;
    }
    if (extraAction != 0) {
        if ((s16)D_00332E30.extraActions[extraAction].action == 1) {
            evtMessageExtraActionNoOp();
        } else if ((s16)D_00332E30.extraActions[extraAction].action == 2) {
            ptyAdjustItemQuantity(D_00332E30.extraActions[extraAction].parameter0,
                                  D_00332E30.extraActions[extraAction].parameter1);
        } else if ((s16)D_00332E30.extraActions[extraAction].action == 3) {
            datAddCurrencyClamped(D_00332E30.extraActions[extraAction].parameter0);
        }
    }
    if (target >= 10) {
        viewIndex = D_00332E30.sets[D_003BAE38].messages[target - 10].view & 0x7F;
    }
    if (viewIndex != 0) {
        unit = ((EffectObjectData *)fldPlayerObject->data)->transitionWork;
        if (unit != NULL) {
            if (D_00332E30.views[viewIndex].player != 0) {
                fldDisableCameraObjectFlag();
            } else {
                fldEnableCameraObjectFlag();
            }
        }
        if (D_00332E30.views[viewIndex].positionName[0] != 0) {
            node = dds3SceneBasicSearchName(world, (const u8 *)D_00332E30.views[viewIndex].positionName);
            if (node != NULL) {
                evtSetUnitPos(fldPlayerObject->key, node->key);
            }
        }
        if (D_00332E30.views[viewIndex].cameraName[0] != 0) {
            node = dds3SceneBasicSearchName(world, (const u8 *)D_00332E30.views[viewIndex].cameraName);
            if (node != NULL) {
                camera = dds3SearchSceneBasic(world, node->key, 4);
                if (camera != NULL) {
                    dds3SetActiveCameraSceneBasic(world, camera);
                }
            }
        } else if (fldCamNowEveCamera() != 0) {
            dds3SetActiveCameraSceneBasic(world, fldGetPlayerSceneState());
            switch (D_0032E400[0]) {
            case 0:
            case 3:
                fldUpdateFieldCameraViewState();
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
        if ((s8)D_00332E30.sets[D_003BAE38].messages[row].extraAction != 0) {
            if ((s16)D_00332E30.extraActions[(s8)D_00332E30.sets[D_003BAE38].messages[row].extraAction].action == 4) {
                s32 command;
                s32 flag;

                scrDestroyAllNamedProcesses();
                command = D_00332E30.extraActions[(s8)D_00332E30.sets[D_003BAE38].messages[row].extraAction].parameter0;
                flag = D_00332E30.extraActions[(s8)D_00332E30.sets[D_003BAE38].messages[row].extraAction].parameter1;
                fldSetDeferredFieldCommand(fldGlobalBase.area, command);
                evtSubmitEventRequestImmediate(command);
                if (flag == 1) {
                    fldGlobalBase.titleFade = flag;
                }
                return command + 200;
            }
        }
        /* The action callbacks can replace the player's transition work. */
        unit = ((EffectObjectData *)fldPlayerObject->data)->transitionWork;
        if (unit != NULL) {
            unit->owner->flags &= ~1;
        }
        if (fldCamNowEveCamera() != 0) {
            switch (D_0032E400[0]) {
            case 0:
            case 3:
                fldUpdateFieldCameraViewState();
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

s32 fldGetActorSlotAttribute(s32 actorId, FldInfMessageAttribute attribute) {
    s32 index = actorId - 10;
    s32 attributeIndex = (s32)attribute;
    s32 value = 0;

    switch (attributeIndex) {
    case FLD_INF_MESSAGE_ATTRIBUTE_KIND:
        value = D_00332E30.sets[D_003BAE38].messages[index].kind;
        break;
    case FLD_INF_MESSAGE_ATTRIBUTE_MESSAGE:
        value = D_00332E30.sets[D_003BAE38].messages[index].message;
        break;
    case FLD_INF_MESSAGE_ATTRIBUTE_VIEWED:
        if (D_00332E30.sets[D_003BAE38].messages[index].view & 0x80) {
            value = 1;
        }
        break;
    }
    return value;
}


extern char D_003A01F8[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */

void fldInfLoad(s32 field) {
    char path[64];
    char directory[32];
    sdfFileCmdHandle_t *command;
    if (field < 200) {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, D_003A01F8, directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &D_00332E30, sizeof(D_00332E30));
        sdfFileClose(command);
    }
}

void fldInfLoadFromMem(const FldInfTable *source) {
    memcpy(&D_00332E30, source, sizeof(D_00332E30));
}

extern s32 gFldInf_WpExit;

extern const char *fldFindActorEntryByName(const char *);

void fldInfToWAP(void) {
    s32 slot;
    const char *actor;

    gFldInf_WpExit = 1;
    slot = gFldInf_HitIdx;
    actor = fldFindActorEntryByName(fldRoomResources[slot]->name);
    if (scrSearchScriptName(actor) != NULL) {
        return;
    }
    fldRoomTaskHandles[slot] = evtStartSceneResourceTask(dds3GetActiveScene(), actor);
}

extern const char D_003369B0[];

extern const char D_003369C0[];

extern const char D_003369D0[];

extern const char D_003369E0[];

extern const char D_00336A00[];

extern const char D_00336A10[];

extern const char D_00336A30[];

extern const char D_00336A40[];

extern const char D_00336A50[];

extern s32 fldPickedActorTemplateKind;

static inline FldActorEntry *fldGetActorRows(void) {
    return fldActorWaypointRows.actors;
}

static inline u8 fldGetActorControlFlags(const FldActorEntry *entry) {
    return entry->flags31;
}

INCLUDE_RODATA(const s32, "game/code_00138C30", D_003A01F8);

INCLUDE_RODATA(const s32, "game/code_00138C30", D_003A0200);

const char *fldPickActorTemplateByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = &fldGetActorRows()[i];
        flag = entry->requiredFlag;
        if ((flag == 0 || evtBitCheck(flag) != 0)
            && (fldGlobalBase.specialEventActive == 0 || !(fldGetActorControlFlags(entry) & 4))
            && entry->floor == fldGlobalBase.floor + 1
            && strcmp(name, entry->name) == 0) {
            switch (entry->kind) {
            case 1:
                if (entry->variantMode == 0) {
                    sub = entry->variant;
                    if (sub != 0) {
                        if (sub == 1) {
                            fldPickedActorTemplateKind = 6;
                            return D_00336A10;
                        }
                    }
                }
                fldPickedActorTemplateKind = 0;
                return D_003369B0;
            case 2:
                fldPickedActorTemplateKind = 1;
                return D_003369C0;
            case 3:
                fldPickedActorTemplateKind = 2;
                return D_003369D0;
            case 4:
                fldPickedActorTemplateKind = 3;
                return D_003369E0;
            case 5:
                fldPickedActorTemplateKind = 5;
                return D_00336A00;
            case 10:
                fldPickedActorTemplateKind = 8;
                return D_00336A30;
            case 11:
                fldPickedActorTemplateKind = 9;
                return D_00336A40;
            case 12:
                fldPickedActorTemplateKind = 10;
                return D_00336A50;
            }
        }
        i++;
    } while (i < 0x100);
    fldPickedActorTemplateKind = -1;
    return 0;
}

const char *fldFindActorEntryByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = &fldGetActorRows()[i];
        flag = entry->requiredFlag;
        if ((flag == 0 || evtBitCheck(flag) != 0)
            && (fldGlobalBase.specialEventActive == 0 || !(fldGetActorControlFlags(entry) & 4))
            && entry->floor == fldGlobalBase.floor + 1
            && strcmp(name, entry->name) == 0) {
            fldSelectedActorEntryIndex = i;
            switch (entry->kind) {
            case 1:
                if (entry->variantMode == 0) {
                    sub = entry->variant;
                    if (sub != 0) {
                        if (sub == 1) {
                            D_003BAE68 = 6;
                            return D_00336A10;
                        }
                    }
                }
                D_003BAE68 = 0;
                return D_003369B0;
            case 2:
                D_003BAE68 = 1;
                return D_003369C0;
            case 3:
                D_003BAE68 = 2;
                return D_003369D0;
            case 4:
                D_003BAE68 = 3;
                return D_003369E0;
            case 5:
                D_003BAE68 = 5;
                return D_00336A00;
            case 10:
                D_003BAE68 = 0;
                return D_00336A30;
            case 11:
                D_003BAE68 = 0;
                return D_00336A40;
            case 12:
                D_003BAE68 = 0;
                return D_00336A50;
            }
        }
        i++;
    } while (i < 0x100);
    return 0;
}

extern s32 D_003BAB40;

extern s32 D_0032E3C4[];

extern const char D_00336A30[];

const char *fldSelectCurrentActorOnNextFloor(void) {
    s32 index = D_003BAB40;
    FldActorEntry *entry = &fldActorWaypointRows.actors[index];
    if (entry->floor == D_0032E3C4[0] + 1 && entry->kind == 10) {
        fldSelectedActorEntryIndex = index;
        D_003BAE68 = 8;
        return D_00336A30;
    }
    return 0;
}

s32 fldGetSelectedActorMotionId(void) {
    return fldActorWaypointRows.actors[fldSelectedActorEntryIndex].motion;
}

s32 fldQuerySelectedActorMotionState(s32 query) {
    FldActorEntry *entry = &fldActorWaypointRows.actors[fldSelectedActorEntryIndex];

    if (query == 0 && entry->kind == 1) {
        if (entry->motion == 5 || entry->secondaryMotion == 5 || entry->motion == 6
            || entry->secondaryMotion == 6 || entry->motion == 7 || entry->secondaryMotion == 7
            || entry->motion == 8 || entry->secondaryMotion == 8) {
            return 0x28;
        }
        return 0x14;
    }
    if (query == 1) {
        return fldTestBits((u8)entry->flags54, 8);
    }
    return 0;
}

/* Kinds 10, 11 and 12 have no case body, and that is deliberate: retail tests
   them in exactly this order (10, 11, 12, then 4), so moving any of them
   changes the branch layout and stops matching. */
void fldApplyActorEntryTrigger(s32 checkTaskRecord) {
    s32 index;
    s32 kind;
    const char *record;
    FldActorEntry *entry;

    if (checkTaskRecord != 0) {
        record = fldEveHitResName_Find(scrGetScriptHandle()->task);
        if (record == 0) {
            return;
        }
        if (fldFindActorEntryByName(record) == 0) {
            return;
        }
    }
    index = fldSelectedActorEntryIndex;
    entry = &fldGetActorRows()[index];
    kind = entry->kind;
    if (kind == 1) {
        if (entry->floor == D_0032E3C4[0] + 1) {
            fldPlayFieldSeVolumePan(entry->sound);
            fldReleaseActorTasksById(fldActorSlots[index].actorId);
            return;
        }
    } else if (kind == 2) {
        if (entry->floor == fldGlobalBase.floor + 1) {
            s32 motion = entry->motion;

            fldGlobalBase.modeOneCameraHeadingTarget = fldActorSlots[index].firstValues[0];
            fldGlobalBase.positionMode = 1;
            if (motion == 1) {
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
            D_0032E530[0] = 0x64;
        } else {
            D_0032E530[0] = -0x64;
        }
    } else if (kind == 10) {
    } else if (kind == 11) {
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
        entry = &fldActorWaypointRows.actors[i];
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
                    camera = (dds3Basic_t *)dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 4,
                                                                                  entry->linkName);
                    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
                    fldEnableCameraObjectFlag();
                    return;
                }
            }
        }
    }
}

u8 fldDoor_ChkWarp(void) {
    return D_003BAE68 == 8;
}

void fldApplySceneRoomSelection(FldActorEntry *entry) {
    if (entry->selectedRoom != 0) {
        fldGlobalBase.selectedRoomIndex = entry->selectedRoom - 1;
    }
    fldGlobalBase.rowIdx = entry->rowIndex;
}

extern s32 D_003BAE58;
extern char D_003A0270[], D_003A0280[];
extern void dds3WorkClear(void);
extern void fldBgmFadeOut(void);
extern void fldSetSceneParam4(fldSceneParam_t *record, s32 stage, s32 kind,
                                             const char *name, s32 code, s32 link,
                                             const char *subname);
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
            i = D_003BAB40;
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
                fldSetFlagBit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                entry->motion);
            }
        } else if (entry->kind == 10) {
            if (entry->secondaryMotion != 0) {
                fldSetFlagBit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                                entry->secondaryMotion);
            }
        } else if (entry->kind == 11) {
            if (entry->secondaryMotion != 0) {
                fldSetFlagBit(fldGlobalBase.area, fldGlobalBase.floor + 1,
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
            fldSetFlagBit(area[0], floor, entry->floorFlagBit);
        }
        flags = entry->flags64;
        if (flags & 2) {
            fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor,
                entry->explorationColumn, entry->explorationRow,
                entry->explorationColumnCount, entry->explorationRowCount);
        }
        if (entry->variantMode == 2) {
            fldSetDeferredFieldCommand(0, 0);
            fldSetFlagBit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                            entry->sequenceValue);
            D_003BAE54 = fldGlobalBase.area;
            D_003BAE58 = fldGlobalBase.floor + 1;
            D_003BAE5C = entry->variant;
            D_003BAE60 = entry->sequenceKind;
            dds3WorkClear();
            switch (entry->variant) {
            case 0:
                fldBgmFadeOut();
                event = entry->sequenceKind;
                dds3AdminiCallSequence(0x14, &event, sizeof(event), 0);
                return;
            case 3:
                fldBgmFadeOut();
                vr[0] = 0;
                vr[1] = entry->sequenceKind;
                if (D_0032EF18[vr[1]].requiredModelFlag != 0) {
                    evtBitOn(D_0032EF18[vr[1]].requiredModelFlag);
                }
                DDS3_GBWK->vr.previewX = vr[0];
                DDS3_GBWK->vr.previewY = vr[1];
                dds3AdminiCallSequence(0x15, vr, sizeof(vr), 0);
                return;
            case 4:
                vr[0] = 1;
                vr[1] = entry->sequenceKind;
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
            if (entry->sequenceKind == 0) {
                floor = fldGlobalBase.floor + 1;
            } else {
                floor = entry->sequenceKind;
            }
            if (fldGlobalBase.area != area[0]) {
                fldBgmFadeOut();
            }
            fldApplySceneRoomSelection(entry);
            if (entry->flags54 != 0) {
                D_003BAB3C = entry->flags54;
                strcpy((char *)D_0032C9A0, entry->taskName);
                D_003BAB40 = i + 1;
            }
            if (entry->flags31 & 1) {
                fldGlobalBase.commandEnabled = 1;
            } else {
                fldGlobalBase.commandEnabled = 0;
            }
            if (entry->flags31 & 2) {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_REQUEST;
            } else if (entry->flags31 & 4) {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_SUPPRESS;
            } else {
                fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_NONE;
            }
            if (entry->kind == 3 || entry->kind == 4) {
                fldGlobalBase.unk17C = 15;
                dds3FadeOut(0, 0, 0, 0);
                if (entry->linkKind == 1 || entry->linkKind == 2) {
                    fldSetSceneParam4(&sequence, area[0], floor, entry->sequenceName, entry->sequenceCode,
                                                     entry->linkKind, entry->linkName);
                } else {
                    fldSetSceneParam3(&sequence, area[0], floor, entry->sequenceName);
                }
            } else {
                fldSetSceneParam4(&sequence, area[0], floor, entry->sequenceName, entry->sequenceCode,
                                                 entry->linkKind, entry->linkName);
            }
            dds3AdminiCallSequence(5, &sequence, sizeof(sequence), 0);
            return;
        }
        if (entry->variantMode == 1) {
            row = entry->variant - 1;
            headerFloor = entry->sequenceKind;
            currentArea = fldGlobalBase.area;
            area[0] = currentArea;
            floor = fldActorWaypointRows.headers[row].area;
            D_003BAE4C = row;
            D_003BAE50 = headerFloor;
            fldGlobalBase.commandEnabled = 0;
            fldSetFlagBit(area[0], fldGlobalBase.floor + 1,
                            fldActorWaypointRows.headers[row].payload[headerFloor + 8]);
            entryName = D_003A0270;
            name = D_003A0280;
            fldSetSceneParam5(&sequence, area[0], floor, entryName, 0, name);
            dds3AdminiCallSequence(5, &sequence, sizeof(sequence), 0);
            return;
        }
        if (entry->variantMode == 3) {
            command = entry->variant;
            fldSetDeferredFieldCommand(fldGlobalBase.area, command);
            fldBgmFadeOut();
            if (entry->sequenceKind == 0 && entry->sequenceValue != 0) {
                fldSetDeferredFieldCommand(entry->sequenceValue, command);
            }
            dds3AdminiCallSequence(6, &command, sizeof(command), 0);
            return;
        }
    }
}

extern s32 D_003BAE58;
extern s32 fldTriggeredSavePointValue;
extern char D_003A0270[], D_003A0280[];
extern void dds3WorkClear(void);
extern void fldBgmFadeOut(void);
extern void fldSetSceneParam4(fldSceneParam_t *record, s32 stage, s32 kind,
                                             const char *name, s32 code, s32 link,
                                             const char *subname);
extern void fldSetSceneParam3(fldSceneParam_t *, s32, s32, const char *);
extern void fldSetSceneParam5(fldSceneParam_t *, s32, s32, const char *, s32, const char *);

void fldDispatchSceneAction(u32 action) {
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
        D_003BAE54 = fldGlobalBase.area;
        preview[0] = 2;
        preview[1] = 0;
        D_003BAE58 = fldGlobalBase.floor + 1;
        D_003BAE5C = 5;
        D_003BAE60 = 0;
        DDS3_GBWK->vr.previewX = preview[0];
        DDS3_GBWK->vr.previewY = preview[1];
        dds3AdminiCallSequence(0x15, preview, 8, 0);
        return;
    }
    if (action == 0x385) {
        dds3WorkClear();
        fldSetDeferredFieldCommand(0, 0);
        D_003BAE54 = fldGlobalBase.area;
        preview[0] = 1;
        preview[1] = fldTriggeredSavePointValue;
        D_003BAE58 = fldGlobalBase.floor + 1;
        D_003BAE5C = 4;
        D_003BAE60 = fldTriggeredSavePointValue;
        DDS3_GBWK->vr.previewX = preview[0];
        DDS3_GBWK->vr.previewY = preview[1];
        dds3AdminiCallSequence(0x15, preview, 8, 0);
        return;
    }
    for (index = 0; index < 256; index++) {
        entry = &fldActorWaypointRows.actors[index];
        if (entry->requiredFlag != 0 && !evtBitCheck(entry->requiredFlag)) continue;
        if (entry->kind != 9 || entry->motion != action) continue;
        floorFlags = entry->flags64;
        if (floorFlags & 1) {
            if (entry->explorationArea == 0) area[0] = fldGlobalBase.area;
            else area[0] = entry->explorationArea;
            room = entry->flags64 >> 2;
            if (room == 0) room = fldGlobalBase.floor + 1;
            fldSetFlagBit(area[0], room, entry->floorFlagBit);
        }
        flags = entry->flags64;
        if (flags & 2) {
            fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor,
                entry->explorationColumn, entry->explorationRow,
                entry->explorationColumnCount, entry->explorationRowCount);
        }
        if (entry->variantMode == 2) {
            dds3FadeIn2(15);
            fldSetFlagBit(fldGlobalBase.area, fldGlobalBase.floor + 1, entry->sequenceValue);
            D_003BAE54 = fldGlobalBase.area;
            D_003BAE58 = fldGlobalBase.floor + 1;
            D_003BAE5C = entry->variant;
            D_003BAE60 = entry->sequenceKind;
            dds3WorkClear();
            switch (entry->variant) {
            case 0:
                fldBgmFadeOut();
                event = entry->sequenceKind;
                dds3AdminiCallSequence(0x14, &event, 4, 0);
                return;
            case 3:
                fldBgmFadeOut();
                preview[0] = 0;
                preview[1] = entry->sequenceKind;
                if (D_0032EF18[preview[1]].requiredModelFlag != 0) {
                    evtBitOn(D_0032EF18[preview[1]].requiredModelFlag);
                }
                DDS3_GBWK->vr.previewX = preview[0];
                DDS3_GBWK->vr.previewY = preview[1];
                dds3AdminiCallSequence(0x15, preview, 8, 0);
                return;
            case 4:
                preview[0] = 1;
                preview[1] = entry->sequenceKind;
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
            if (entry->sequenceKind == 0) room = fldGlobalBase.floor + 1;
            else room = entry->sequenceKind;
            if (fldGlobalBase.area != area[0]) fldBgmFadeOut();
            fldApplySceneRoomSelection(entry);
            if (entry->flags54 != 0) {
                D_003BAB3C = entry->flags54;
                strcpy((char *)D_0032C9A0, entry->taskName);
                D_003BAB40 = index + 1;
            }
            {
                s32 commandFlag = fldGetActorControlFlags(entry) & 1;

                flags = fldGetActorControlFlags(entry);
                if (commandFlag) fldGlobalBase.commandEnabled = 1;
                else fldGlobalBase.commandEnabled = 0;
            }
            if (flags & 2) fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_REQUEST;
            else if (flags & 4) fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_SUPPRESS;
            else fldGlobalBase.pendingEntryDialogDisposition = FLD_ENTRY_DIALOG_NONE;
            if ((u32)((u8)entry->kind - 3) < 2U) {
                fldGlobalBase.unk17C = 15;
                if (entry->linkKind == 1) {
                    fldSetSceneParam4(&sequence, area[0], room, entry->sequenceName,
                        entry->sequenceCode, 1, entry->linkName);
                } else {
                    fldSetSceneParam3(&sequence, area[0], room, entry->sequenceName);
                }
            } else {
                fldSetSceneParam4(&sequence, area[0], room, entry->sequenceName,
                    entry->sequenceCode, entry->linkKind, entry->linkName);
            }
            dds3AdminiCallSequence(5, &sequence, 0xA0, 0);
            return;
        } else if (entry->variantMode == 1) {
            headerIndex = entry->variant - 1;
            headerFloor = entry->sequenceKind;
            currentArea = fldGlobalBase.area;
            room = fldActorWaypointRows.headers[headerIndex].area;
            area[0] = currentArea;
            D_003BAE4C = headerIndex;
            D_003BAE50 = headerFloor;
            fldSetFlagBit(area[0], fldGlobalBase.floor + 1,
                fldActorWaypointRows.headers[headerIndex].body.data[headerFloor + 6]);
            fldSetSceneParam5(&sequence, area[0], room, D_003A0270, 0, D_003A0280);
            dds3AdminiCallSequence(5, &sequence, 0xA0, 0);
            return;
        } else if (entry->variantMode == 3) {
            warp = entry->variant;
            fldSetDeferredFieldCommand(fldGlobalBase.area, warp);
            fldBgmFadeOut();
            if (entry->sequenceKind == 0 && entry->sequenceValue != 0) {
                fldSetDeferredFieldCommand(entry->sequenceValue, warp);
            }
            dds3AdminiCallSequence(6, &warp, 4, 0);
            return;
        }
    }
}

void fldSelectActorFromSceneIndexTables(s32 mode, s32 index) {
    switch (mode) {
    case 0:
        D_003BAE5C = 3;
        D_003BAE54 = D_0032EF18[index].unk0;
        D_003BAE60 = index;
        break;
    case 1:
        D_003BAE60 = index;
        D_003BAE54 = D_0032EFE0[index].unk0;
        break;
    }
}

extern void fldDoorLoad(s32);

/* Run the actor transition selected by fldSelectActorFromSceneIndexTables:
 * find the first enabled kind-7 actor for the pending field, motion and
 * index, mark its floor and exploration bits, then submit its sequence and
 * clear the pending selection. DDS1 also publishes the command flag. */
void fldSubmitSelectedActorWaypointTransition(void) {
    s32 stagePayload[1]; /* One-word payload for the field mode request. */
    fldSceneParam_t record;
    s32 room;
    s32 extra;
    s32 motion;
    s32 index;
    u32 floorFlags;
    u32 flags;
    s32 i;
    FldActorEntry *entry;

    if (D_003BAE54 == 0) {
        return;
    }
    extra = D_003BAE58;
    motion = D_003BAE5C;
    index = D_003BAE60;
    fldDoorLoad(D_003BAE54);
    for (i = 0; i < 0x100; i++) {
        entry = &fldActorWaypointRows.actors[i];
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
            if (entry->explorationArea == 0) {
                stagePayload[0] = fldGlobalBase.area;
            } else {
                stagePayload[0] = entry->explorationArea;
            }
            room = floorFlags >> 2;
            if (room == 0) {
                room = fldGlobalBase.floor + 1;
            }
            fldSetFlagBit(stagePayload[0], room, entry->floorFlagBit);
        }
        flags = entry->flags64;
        if (flags & 2) {
            fldMarkStageExplorationBitmapRegion(entry->explorationArea, entry->explorationFloor,
                                                entry->explorationColumn, entry->explorationRow,
                                                entry->explorationColumnCount, entry->explorationRowCount);
        }
        if (entry->variant == 1) {
            fldBgmFadeOut();
            stagePayload[0] = fldGlobalBase.area;
            dds3AdminiCallSequence(0x1C, stagePayload, sizeof(stagePayload), 0);
            return;
        }
        if (entry->variant == 0) {
            stagePayload[0] = D_003BAE54;
        } else {
            stagePayload[0] = entry->variant;
        }
        room = entry->sequenceKind;
        if (room == 0) {
            room = D_003BAE58;
        }
        if (fldGlobalBase.area != stagePayload[0]) {
            fldBgmFadeOut();
        }
        fldApplySceneRoomSelection(entry);
        if (entry->flags54 != 0) {
            D_003BAB3C = entry->flags54;
            {
                char *name = entry->taskName;

                strcpy((char *)D_0032C9A0, name);
            }
            D_003BAB40 = i + 1;
        }
        if (entry->flags31 & 1) {
            fldGlobalBase.commandEnabled = 1;
        } else {
            fldGlobalBase.commandEnabled = 0;
        }
        fldSetSceneParam4(&record, stagePayload[0], room, entry->sequenceName, entry->sequenceCode, entry->linkKind, entry->linkName);
        if ((u32)(motion - 3) < 2) {
            record.mode = 5;
        } else {
            record.mode = 4;
        }
        D_003BAE54 = 0;
        D_003BAE58 = 0;
        D_003BAE5C = 0;
        D_003BAE60 = 0;
        dds3AdminiCallSequence(5, &record, 0xA0, 0);
        return;
    }
}

extern void fldSetSceneParamLoad(fldSceneParam_t *, s32, s32, const char *, s32, s32, const char *);

void fldInitializeActorLinkedSequence(s32 mode, s32 index, fldSceneParam_t *sequence) {
    s32 field;
    s32 motion;
    s32 kind;
    s32 i;
    FldActorEntry *entry;

    if (mode == 0) {
        field = D_0032EF18[index].unk0;
        motion = 3;
    } else {
        motion = 4;
        field = D_0032EFE0[index].unk0;
    }
    if (field == 0) {
        return;
    }
    fldDoorLoad(field);
    for (i = 0; i < 0x100; i++) {
        entry = &fldActorWaypointRows.actors[i];
        if (entry->requiredFlag != 0 && !evtBitCheck(entry->requiredFlag)) {
            continue;
        }
        if (entry->kind != 7 || motion != entry->motion ||
            index != entry->secondaryMotion || entry->variantMode != 0) {
            continue;
        }
        fldGlobalBase.rowIdx = entry->rowIndex;
        if (entry->flags54 != 0) {
            D_003BAB3C = entry->flags54;
            strcpy((char *)D_0032C9A0, entry->taskName);
            D_003BAB40 = i + 1;
        }
        if (entry->flags31 & 1) {
            fldGlobalBase.commandEnabled = 1;
        } else {
            fldGlobalBase.commandEnabled = 0;
        }
        if (entry->variant != 0) {
            field = entry->variant;
        }
        kind = 1;
        if (entry->sequenceKind != 0) {
            kind = entry->sequenceKind;
        }
        if (field != 0 && kind != 0) {
            fldSetSceneParamLoad(sequence, field, kind, entry->sequenceName,
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

INCLUDE_ASM(const s32, "game/code_00138C30", func_0013F100);

void fldDoor_WarpCash(const char *name) {
    FldActorEntry *entry;
    s32 area;
    s32 room;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldActorWaypointRows.actors[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 1 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 2 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 3 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 4 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldRequestAreaResource(area, room);
                return;
            }
        } else if (entry->kind == 5 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
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
    FldActorEntry *actor = &fldActorWaypointRows.actors[fldSelectedActorEntryIndex];
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
INCLUDE_RODATA(const s32, "game/code_00138C30", D_003A0270);

INCLUDE_RODATA(const s32, "game/code_00138C30", D_003A0280);

s32 fldGetActorStat1(u32 attribute) {
    FldActorEntry *actor = &fldActorWaypointRows.actors[fldSelectedActorEntryIndex];
    dds3Basic_t *objectNode;

    switch (attribute) {
    case 0:
        switch (actor->motion) {
        case 0:
            return 12;
        case 1:
            return 15;
        case 2:
            return 16;
        case 3:
            return 13;
        case 4:
            return 14;
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
    }
    return 0;
}

/* Look up a motion-table attribute indexed by this actor's state; named
 * object lookups fall through to the next attribute if absent. */
s32 fldGetActorMotionEntry(u32 attribute) {
    FldActorEntry *actor = &fldActorWaypointRows.actors[fldSelectedActorEntryIndex];
    s16 motionIndex = actor->motion;
    dds3Basic_t *objectNode;
    s32 flags;

    switch (attribute) {
    case 0:
        return D_00336A60[motionIndex].defaultMotionId;
    case 1:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), D_00336A60[motionIndex].motionObjectName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 2:
        objectNode = dds3SceneBasicSearchName(dds3GetActiveScene(), D_00336A60[motionIndex].secondaryMotionObjectName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 3:
        D_003BAE44 = D_00336A60[motionIndex].roomModeGroupOneName;
        return 0;
    case 4:
        D_003BAE48 = D_00336A60[motionIndex].sceneBitsBarrierRoomName;
        return 0;
    case 5:
        return D_00336A60[motionIndex].attribute5Value;
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
    s32 slot = D_003BAE4C;

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
        return fldActorWaypointRows.headers[slot].floorCount - D_003BAE50 - 1;
    }
    return 0;
}

s32 fldDoor_ElvTakasaToFloor(s32 index) {
    s32 slot = D_003BAE4C;
    s32 count = fldActorWaypointRows.headers[slot].floorCount;

    if (count - 1 < index) {
        return count - D_003BAE50 - 1;
    }
    index = count - index - 1;
    return fldActorWaypointRows.headers[slot].body.data[index];
}

/* The retail code reaches the waypoint table through separate +4 and +6 address
 * constants (a 16-halfword row stride), so the two views are distinct symbols. */
extern s16 D_00337C64[];
extern s16 D_00337C66[];

void fldDispatchSceneRoomSelection(s32 index) {
    fldSceneParam_t sequence;
    s32 command;
    s32 sourceRoom;
    s32 count;
    s32 remaining;
    s32 inRange;
    s32 i;
    s32 area;
    s32 floor;
    FldActorEntry *entry;

    sourceRoom = D_003BAE50;
    count = D_00337C66[D_003BAE4C * 16];
    inRange = index < count;
    remaining = count - index;
    if (inRange) {
        sourceRoom = remaining;
        sourceRoom--;
    }
    for (i = 0; i < FIELD_ACTOR_SLOT_COUNT; i++) {
        entry = &fldActorWaypointRows.actors[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 6 && entry->motion - 1 == D_003BAE4C &&
            entry->secondaryMotion == sourceRoom && entry->variantMode == 0) {
            if (entry->variant == 0) {
                area = fldGlobalBase.area;
            } else {
                area = entry->variant;
            }
            if (entry->sequenceKind == 0) {
                floor = fldGlobalBase.floor + 1;
            } else {
                floor = entry->sequenceKind;
            }
            if (fldGlobalBase.area != area) {
                fldBgmFadeOut();
            }
            fldApplySceneRoomSelection(entry);
            if (entry->flags54 != 0) {
                D_003BAB3C = entry->flags54;
                strcpy((char *)D_0032C9A0, entry->taskName);
                D_003BAB40 = i + 1;
            }
            if (entry->flags31 & 1) {
                fldGlobalBase.commandEnabled = 1;
            } else {
                fldGlobalBase.commandEnabled = 0;
            }
            fldSetFlagBit(area, floor, D_00337C64[D_003BAE4C * 16 + sourceRoom + 8]);
            fldSetSceneParam4(&sequence, area, floor, entry->sequenceName, entry->sequenceCode, entry->linkKind, entry->linkName);
            dds3AdminiCallSequence(5, &sequence, sizeof(sequence), 0);
            return;
        } else if (entry->kind == 6 && entry->motion - 1 == D_003BAE4C &&
                   entry->secondaryMotion == sourceRoom && entry->variantMode == 3) {
            command = entry->variant;
            fldSetDeferredFieldCommand(fldGlobalBase.area, command);
            fldBgmFadeOut();
            if (entry->sequenceKind == 0 && entry->sequenceValue != 0) {
                fldSetDeferredFieldCommand(entry->sequenceValue, command);
            }
            dds3AdminiCallSequence(6, &command, sizeof(command), 0);
            return;
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
        memset(&fldActorWaypointRows, 0, 0x6CA0);
    } else {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, "%sF%03d.WAP", directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &fldActorWaypointRows, 0x6CA0);
        sdfFileClose(command);
    }
}

void fldGetDoorEntry(FldWaypointBlock *source) {
    memcpy(&fldActorWaypointRows, source, 0x6CA0);
}

extern f32 EulerAngleAxisY(void);

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
        actor = &fldActorWaypointRows.actors[actorIndex];
        row = &fldActorSlots[actorIndex];
        if (actor->kind == 2) {
            if (actor->floor == D_0032E3C4[0] + 1) {
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
            if (actor->floor != D_0032E3C4[0] + 1) {
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

void fldReleaseActorTasksById(s32 id) {
    s32 i;
    FldActorRow *task;
    FldActorEntry *actor;

    if (id != 0) {
        if (id != -1) {
            for (i = 0; i < 256; i++) {
                actor = &fldActorWaypointRows.actors[i];
                task = &fldActorSlots[i];
                if (task->kind == 1 && task->actorId == id) {
                    task->kind = 2;
                    task->firstFrame = 0;
                    task->secondFrame = 0;
                    task->transitionFrame = 0;
                    if (actor->motion == 5 || actor->secondaryMotion == 5 || actor->motion == 6
                        || actor->secondaryMotion == 6 || actor->motion == 7 || actor->secondaryMotion == 7
                        || actor->motion == 8 || actor->secondaryMotion == 8) {
                        task->frameCount = 0x28;
                    } else {
                        task->frameCount = 0x14;
                    }
                    task->firstValues[0] = 0.0f;
                    task->firstValues[1] = 0;
                    task->firstValues[2] = 0;
                    task->secondValues[0] = 0;
                    task->secondValues[1] = 0;
                    task->secondValues[2] = 0;
                    fldApplyPendingCameraHeading();
                }
            }
        }
    }
}

#include "sdf_debug_api.h"

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value);

/* Advance an actor slot's first/second interpolation (ease-weighted for 40-frame moves) or start its door
 * transition, returning the current values for the key. */
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
    void *node;

    if (D_0032E3C0[0] >= 200) {
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

void func_00140F68(void) {
}

extern u8 fldProcCampChk(void), fldProcAmChk(void);
extern s32 fileMenuTaskExists(void);

extern u8 fldHasKiretaLabelProcess(void), fldProcKiretaChk(void), fldProcHirakenaiChk(void);

extern u8 fldIsEventPhaseAtLeastTwo(void);

extern s32 frFontMeasureTextAdvance(const u8 *text, u8 fontIndex, u8 mode);

extern void fldDrawGaugeBar(s32);

extern void fldDrawInputCursor(s32, s32);

extern u8 D_0033E900[];

s32 fldUpdateTitleBannerTask(dds3ProcessID_t *task) {
    FldTitleBannerMenu *menu;
    u8 *label;
    s32 width;
    s32 x;

    if (fldProcCampChk() != 0) {
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
        label = D_0033E900 + menu->choice * 32;
        width = frFontMeasureTextAdvance(label, 1, 0x13);
        x = 0xF6 - (width >> 1);
        fldDrawGaugeBar(width);
        fldDrawInputCursor(x, 0x131);
        fldKanjiDraw(x + 0x14, 0x98, 0xA09DC380, D_0033E900 + menu->choice * 32);
        menu->position = menu->position + 1;
        menu->pending = 0;
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

INCLUDE_SDATA(const s32, "game/code_00138C30", fldActorTaskRoom);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldEventSetRoom);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldLabelTaskRoom);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldLabelTaskSlot);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldEveRngCount);

INCLUDE_SDATA(const s32, "game/code_00138C30", gFldEveRngIdx);

INCLUDE_SDATA(const s32, "game/code_00138C30", gFldEveRngIdx_N);

INCLUDE_SDATA(const s32, "game/code_00138C30", gFldEveRngIdx_T);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldTriggeredSavePointValue);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE28);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldEveHitState);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE30);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldActionRowCount);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE38);

INCLUDE_SDATA(const s32, "game/code_00138C30", gFldInf_HitIdx);

INCLUDE_SDATA(const s32, "game/code_00138C30", gFldInf_WpExit);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE44);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE48);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE4C);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE50);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE54);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE58);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE5C);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE60);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldSelectedActorEntryIndex);

INCLUDE_SDATA(const s32, "game/code_00138C30", D_003BAE68);

INCLUDE_SDATA(const s32, "game/code_00138C30", fldPickedActorTemplateKind);

INCLUDE_SDATA(const s32, "game/code_00138C30", addressprocess_id);
