#include "sdf_gs_header.h"
#include "sdf_packet_header.h"
#include "eff.h"
#include "fld_marker_packet.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "fld_light_set.h"
#include "fld_sky_gradient_record.h"
#include "fld_quad_state.h"
#include "kwln_frame_draw_buffer.h"
#include "kwln_fade.h"
#include "mnu_title_stream.h"
#include "fld_bill_config.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "fld_npc_motion.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "fld_resource_resolver.h"
#include "fld_transfer_chunk.h"
#include "fld_camera_follow_row.h"
#include "fld.h"
#include "fld_collision_geometry.h"
#include "fld_scene_request.h"
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



extern FldFileResource *D_003BD7B8;
extern u32 D_003BD7BC;
#include "dds3_world_secondary.h"
struct EffWorldNode;
extern struct EffWorldNode *dds3SpawnInnerVecObj6(s32, f32 *, void *);
extern char D_0039FD50[]; /* "FLD_DMY_MATTER" */
extern s32 fldGetRecordValueById(s32 id);
extern void fldSetRecordValueById(s32 id, s32 value);
extern s32 strcmp(const char *left, const char *right);

struct EffWorldNode *fldCreateDummyMatter(void) {
    u32 args[8];
    struct EffWorldNode *matter;
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = 0;
    args[5] = 0;
    args[6] = 0;
    args[7] = 0;
    matter = dds3SpawnInnerVecObj6(dds3AdvanceWorldCounter(), (f32 *)args, args + 4);
    dds3SetWorldNodeValue(matter, (u32)D_0039FD50);
    return matter;
}

EffWorldNode *fldResolveWorldObjectByResourceId(u32 id) {
    u32 i = 0;
    EffWorldNode *world = dds3GetWorldSecondaryObject();
    FldFileResource *resource = D_003BD7B8;

    for (; i < D_003BD7BC; i++, resource++) {
        if (id == resource->id) {
            if (resource->names != NULL) {
                FldFileNameEntry *entry = resource->names->entries;
                u32 j = 0;

                for (; j < resource->names->count; j++, entry++) {
                    EffWorldNode *object = dds3FindIndexedObjectChainNodeByName(world, 6, (const u8 *)entry->name);
                    if (object != NULL) {
                        return object;
                    }
                }
            } else {
                EffWorldNode *object = (EffWorldNode *)(u32)fldGetRecordValueById(resource->id);
                if (object == NULL) {
                    object = fldCreateDummyMatter();
                    fldSetRecordValueById(resource->id, (s32)object);
                }
                return object;
            }
        }
    }
    return NULL;
}

EffWorldNode *fldResolveWorldObjectByResourceEntryName(const char *name) {
    u32 i = 0;
    EffWorldNode *world = dds3GetWorldSecondaryObject();
    FldFileResource *resource = D_003BD7B8;

    for (; i < D_003BD7BC; i++, resource++) {
        if (resource->names != NULL) {
            FldFileNameEntry *entry = resource->names->entries;
            u32 j = 0;

            for (; j < resource->names->count; j++, entry++) {
                if (strcmp(name, entry->name) == 0) {
                    EffWorldNode *object = dds3FindIndexedObjectChainNodeByName(world, 11, (const u8 *)resource->name);
                    if (object != NULL) {
                        return object;
                    }
                }
            }
        }
    }
    return NULL;
}

#include "eff_light.h"
#include "fld_area_work.h"
#include "fld_spawn_collision.h"

struct EffNodeDescriptor;


/* Complete serialized descriptors shared with their existing consumers. */

typedef char FldSpawnBatchSize[(sizeof(FldSpawnBatch) == 12) ? 1 : -1];

extern FldFileResource *D_003BD7B0;
extern u32 D_003BD7B4;
extern u32 D_003C91D0[];
extern u32 D_003C91F0[];
extern char D_003BAC40[];
extern void *memset(void *, int, unsigned int);
extern s32 sprintf(char *, const char *, ...);
extern EffWorldNode *effObjCreateResourceKindZero(const char *, void *, s32);
extern EffWorldNode *effObjCreateResourceKindOne(const char *, void *, s32);
extern EffWorldNode *effObjSpawnSharedBillNodeClone(EffWorldNode *, void *, s32);
extern EffWorldNode *effObjSpawnSharedBillClone(EffWorldNode *, void *, s32);
extern EffWorldNode *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *, void *, void *);
extern EffWorldNode *effObjCreateNamedScaledEffect(const char *, void *, void *, f32);
extern void effObjSetFlags(EffWorldNode *, s32);
extern void effObjClearFlags(EffWorldNode *, s32);
extern void billCopySourceVectorAndSetConfig(EffWorldNode *, BillConfig *);
extern EffWorldNode *evtSpawnActionObj9(s32);
extern EffWorldNode *evtSpawnActionObj10(s32, void *, const char *);
extern EffWorldNode *evtSpawnActionObjB(s32, void *, f32 *, const char *);
extern void effObjSetInnerFloat(EffWorldNode *, f32);
extern void dds3LoadWorldTransformSetup(EffWorldNode *, WorldTransformSetup *);
extern void fldBuildCollisionRecordsFromGeometry(void *, f32 *, u32, s32, s32);
extern s32 fldParseRoomNumberFromName(const char *);
extern void dds3SetPathStateValue(EffWorldNode *, u32);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_0039FD50);

void fldSpawnSceneResourceBatches(FldSpawnBatch *batch, u32 batchCount) {
    f32 position[4];
    f32 rotation[4];
    char path[64];
    char directory[32];
    s8 loaded[4];
    void *cached[4];
    u32 cacheCount = sizeof(loaded);
    u32 batchIndex;
    u32 count;
    u32 i;
    FldFileResource *resource;
    void *object;
    u32 pathIndex;

    memset(loaded, 0, cacheCount);
    memset(cached, 0, cacheCount * sizeof(*cached));
    D_003BD7B0 = NULL;
    D_003BD7B4 = 0;
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, batch++) {
        resource = batch->entries;
        count = batch->count;
        switch (batch->kind) {
        case 0:
        case 1:
            break;
        case 2:
            D_003BD7B0 = resource;
            D_003BD7B4 = count;
            break;
        case 11:
            for (i = 0; i < count; i++, resource++) {
                BillConfig *config;
                u32 *resourceKeys;
                u32 cacheByteOffset;
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                config = resource->data;
                switch (config->kind) {
                case 0:
                case 3:
                    resourceKeys = D_003C91D0;
                    cacheByteOffset = config->resourceIndex * sizeof(*cached);
                    if (loaded[config->resourceIndex] == 0) {
                        fldFormatAreaDirectory(directory, fldAreaState.area, fldAreaState.floor);
                        sprintf(path, D_003BAC40, directory,
                                     resourceKeys[((BillConfig *)resource->data)->resourceIndex]);
                        object = effObjCreateResourceKindZero(path, position, (s32)rotation);
                        effObjSetFlags(object, 1);
                        billCopySourceVectorAndSetConfig(object, resource->data);
                        loaded[((BillConfig *)resource->data)->resourceIndex] = 1;
                        cached[((BillConfig *)resource->data)->resourceIndex] = object;
                    } else {
                        object = effObjSpawnSharedBillNodeClone(*(void **)((u8 *)&cached + cacheByteOffset), position, (s32)rotation);
                        effObjSetFlags(object, 1);
                        billCopySourceVectorAndSetConfig(object, resource->data);
                    }
                    break;
                case 1:
                    resourceKeys = D_003C91D0;
                    cacheByteOffset = config->resourceIndex * sizeof(*cached);
                    if (loaded[config->resourceIndex] == 0) {
                        fldFormatAreaDirectory(directory, fldAreaState.area, fldAreaState.floor);
                        sprintf(path, D_003BAC40, directory,
                                     resourceKeys[((BillConfig *)resource->data)->resourceIndex]);
                        object = effObjCreateResourceKindOne(path, position, (s32)rotation);
                        effObjSetFlags(object, 1);
                        loaded[((BillConfig *)resource->data)->resourceIndex] = 1;
                        cached[((BillConfig *)resource->data)->resourceIndex] = object;
                        billCopySourceVectorAndSetConfig(object, resource->data);
                    } else {
                        object = effObjSpawnSharedBillClone(*(void **)((u8 *)&cached + cacheByteOffset), position, (s32)rotation);
                        effObjSetFlags(object, 1);
                        billCopySourceVectorAndSetConfig(object, resource->data);
                    }
                    break;
                case 2:
                    if (D_003C91F0[config->resourceIndex] != NULL) {
                        object = effObjSpawnDescriptorBoundEffect((struct EffNodeDescriptor *)D_003C91F0[config->resourceIndex], position, rotation);
                        effObjSetFlags(object, 1);
                        billCopySourceVectorAndSetConfig(object, resource->data);
                    }
                    break;
                case 100:
                    object = effObjCreateNamedScaledEffect(resource->name, position, rotation, config->width * 0.5f);
                    effObjClearFlags(object, 1);
                    break;
                }
            }
            break;
        case 12:
            for (i = 0; i < count; i++, resource++) {
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                object = evtSpawnActionObj9(resource->id);
                dds3SetWorldNodeValue(object, (u32)resource->name);
                effObjSetInnerPosition(object, (u128 *)position);
                /* Type-12 serialization uses this shared slot for outer radius. */
                effObjSetInnerFloat(object, ((WorldTransformSetup *)resource->data)->transform.rotation[1]);
                dds3LoadWorldTransformSetup(object, resource->data);
                if ((u32)fldAreaState.area - 200U < 300U) {
                    ((EffLightData *)((EffWorldNode *)object)->data)->flags |= 8;
                    dds3SetWorldNodeValue(object, (u32)"BTL_DEF_LIGHT");
                }
            }
            break;
        case 9:
            for (i = 0; i < count; i++, resource++) {
                evtSpawnActionObj10(resource->id, resource->data, resource->name);
            }
            break;
        case 3:
            for (i = 0, pathIndex = 1; i < count; i++, resource++, pathIndex++) {
                resource->id |= 0x20000;
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                if (((FldSpawnCollision *)resource->data)->kind == 0) {
                    s32 room = fldParseRoomNumberFromName(resource->name);
                    fldBuildCollisionRecordsFromGeometry(((FldSpawnCollision *)resource->data)->header, position,
                                  resource->id, room, -1);
                    object = evtSpawnActionObjB(resource->id, resource->data, position, resource->name);
                    dds3SetPathStateValue(object, pathIndex);
                }
            }
            break;
        }
    }
}


#include "fld_area_work.h"
#include "evt_world.h"
#include "evt_world_source_transform.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_append.h"
#include "sdf_packet_builders.h"
#include "fr_font.h"
#include "sdf_packet_list.h"
#include "sdf_dev_state.h"
#include "sdf_resource.h"
#include "fld_waypoint.h"
#include "fld_inf.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "file_pac.h"
#include "fld_packed_resource_kind.h"
#include "field_stage.h"
#include "sdf_pac_work.h"
extern FldInfTable D_00332E30;
#include "sdf_primitive.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "fld.h"
#include "mdl.h"
#include "scr.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "dat_state.h"

/* Fixed allocation sizes and native room/actor table dimensions. */
enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256,
    FIELD_CAMERA_SETTING_COUNT = 8
};

typedef struct SdfDrawPacket SdfDrawPacket;
typedef struct DmaPacketHeader DmaPacketHeader;
/* Packet addresses are 32-bit handles; GS and GIF payload words remain 64-bit. */

extern void fldPlayFieldSeVolumePan(s32);
extern void kwlnFadeSetRGB(s32, s32, s32);
extern void fldReleaseActorTasksById(s32);
/* Per-actor slot in the field's actor table; reset by fldResetActorSlots and
   scanned by fldReleaseActorTasksById. 0x5C is the stride retail uses. Three transition keys, signed frame
   counters and two float triples with per-frame increments (same layout as DDS2). */
typedef struct {
    s32 kind;
    u32 actorId;
    u32 unk08;
    u32 firstKey;
    u32 unk10;
    u32 secondKey;
    u32 transitionKey;
    s32 firstFrame;
    s32 secondFrame;
    s32 transitionFrame;
    s32 frameCount;
    f32 firstValues[3];
    f32 firstStep[3];
    f32 secondValues[3];
    f32 secondStep[3];
} FldActorRow; /* 0x5C bytes */

extern FldActorRow fldActorSlots[];
extern s32 D_0032E530[];

extern FldNpcMotion D_00336A60[];
extern void *D_003BAE44;
extern void *D_003BAE48;

static inline s32 fldTestBits(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}
/* vu0 routine: normalize vf10 and return its original XYZ length. */
static inline f32 fldNormalizeProbeVector(void) {
    f32 length;

    VU0_LENGTH_VF10(length);
    VU0_NORMALIZE_VF10();
    return length;
}

typedef struct FldActionSpawn {
    s32 unk0;
    s32 secondValue;
    s32 firstValue;
} FldActionSpawn;
extern void fldSpawnActionObjects(FldActionSpawn *, u32);
extern s32 fldGetSceneReadyFlag(void);
extern s32 D_0032E400[];
extern void fldClearCameraObjectHighlightFlag(void);
extern void fldSetCameraObjectHighlightFlag(void);
extern s32 func_0012FC20(void);
extern void fldUpdateModeOneCameraPose(void);
extern void fldDispatchCameraModelFrameEvents(s32, s32);
extern s32 func_0012FF48(void);
extern void fldUpdateCameraModelMotion(s32, s32, f32);
extern s32 *fldGetPlayerSceneStateAddress();
extern void dds3SetCameraFieldOfView(s32, f32);
extern void fldToggleWorldNodeState(s32);
extern void fldProbeCameraSupportSurface(void);
extern void fldRestoreCameraModelColor(void);
extern void func_0012D528(void);
extern void fldInterpolateCameraEndpoints(void);
extern void func_0012DD70(void);
extern void fldUpdateCameraMoveOscillation(void);
extern u8 fldTestSceneControlFlags(u32);
extern f32 fldPointDistance(f32, f32, f32, f32, f32, f32);
extern void fldClearCameraModelColor(void);
extern void fldSetCameraObjectTransitionFlag(void);
extern void fldClearCameraObjectTransitionFlags(void);
extern FldCamRow fldCameraFollowRows[];
extern f32 D_00330630[];
extern s32 D_003BAD24;
extern s32 D_003BAD28;

extern f32 D_00330650[];
extern f32 D_00330660[];
extern void btlActivateRuntime(s32 mode);
extern u32 sdfConsFinalizePacketHeader(u32, s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern s32 sdfConsMeasurePacketWithHeader(s32);
extern void *sdfConsAllocateColumnPacket(s32);

extern s32 D_0032E3C0[];
extern s8 D_0032C9A0[];
extern void fldReleaseSkyResources(void);
extern void fldCreatePlayerObject(void);
extern u8 *fldSelectCurrentActorOnNextFloor(void);
extern void fldSpawnPlacementObjectsFromBatches(u32, u32, s32);

extern s32 D_003BAE68;

extern u32 fldAuxRecordBuffer;
extern u32 fldValueRecords;
extern u32 fldAuxRecordResource;
extern u32 fldValueRecordResource;

extern u32 fldSkyDrawState;

extern u32 fldSwayMode;
extern f32 fldSwayPhase;
extern s32 fldSwayOffset;

extern SdfFlagListWork *fldCameraColorEffect;
extern u32 fldCameraColorEnabled;
extern SdfTex *fldRainTextureReference;
extern s32 fldRainTextureResource;

extern MdlCtx *fldPlayerModelContext;

extern u32 fldPlayerObject;
extern void evtEndObjectValueTransition(EffWorldNode *object);
extern u8 D_003BAB3C;

extern u32 D_003BAD34;
extern u32 D_003BAD38;
extern u32 D_003BAD3C;
extern u32 D_003BAD40;

extern u32 D_003BAD1C;

#include "dds3_world_primary.h"
extern s64 fldGetPlayerSceneState(void);


extern u32 D_003BACF8;

#include "sdf_packet_alloc.h"


extern s32 fldBackgroundBuffer;

extern u32 fldDisplayRow;

extern u32 D_003BAC30;

extern u32 fldPendingArea;
extern u32 fldPendingFloor;
extern u32 D_003BAD98;
extern u32 D_003BAD9C;
extern f32 D_003BADA0;
extern u32 D_003BADA4;
extern u32 D_003BADC8;
extern u32 D_003BADD8;
extern u32 D_003BAE28;
extern s32 D_003BAE3C;
extern u32 fldSelectedActorEntryIndex;
extern u32 D_0032E428[];

extern u32 D_0032E538[];
extern u32 D_0032E544[];
extern u32 D_0032E570[];
extern s32 D_003BAE38;
extern u32 D_0032E59C[];
extern u32 D_0032E5A8[];
extern SdfPoolNode kwlnDrawSurfaces[];
extern u8 sdfViewMatrix[];
extern u32 D_003BACD4;
extern u32 D_003BACEC;
extern SdfTex *fldMarkerTexture;
extern u32 D_003BD7C0;
extern u8 D_0032F260[];
extern SdfAsset *sdfCreateAssetWithDrawEntries(void);
extern void fldResetRecordState(void);
extern u8 sdfProjectionMatrix[];
extern u8 D_00324660[];
extern char D_003C9200[];
extern u32 D_003C92E0[];
extern s16 D_003C9518[];
extern void fldFormatAreaResourceName(char *arg0);
extern void fldApplySkyLightSet(u32 arg0, s32 arg1);
extern s32 strcmp(const char *a, const char *b);
extern f32 D_003BAD20;
extern char fldEncounterTaskName[];
extern f32 D_003C9230[];
extern f32 D_003C9220[];
extern s32 fldEncProc(KwlnTask *task);
extern void fldResetEncounterAsyncState(KwlnTask *task);
extern s32 btlUpdateRuntimeFadeState(void);
extern void btlClearRuntimeState(void);
extern void dds3TransformCameraVectorsByInnerRotation(EffWorldNode *camera, f32 *worldEyeOut,
                                                      f32 *targetPositionOut);
extern f32 sdfAtan2(f32 arg0, f32 arg1);
extern struct FileRequest *fldAreaLoadRequest;
extern u8 D_003BACE0[];
extern u8 D_003BACE8[];
extern s32 fldEncounterRuntimeState;
extern u32 D_003BACFC;
extern u32 D_00330738[];
extern u32 D_003306B0[];
extern f32 D_003306C0[];
extern f32 D_00330670[];
extern u32 D_003308B0[];

extern void *memset(void *s, s32 c, u32 n);
extern s32 fldUpdateTitleBannerTask(KwlnTask *task);
extern s32 btlGetRuntimeResult(void);
extern s32 fldValueRecordCount;
extern s32 D_003BADF8;
extern s32 D_003BADEC;
extern u8 D_003BA734;
extern ScrData *scrFindNamedProcessNode(char *name);
extern u32 fldAreaCachedResource;
extern struct FileRequest *fldAreaPackedArchive;
extern u8 D_003BAC90[];
extern s32 D_0032E4C8[];
extern s32 D_003BAE30;
extern s32 D_003BAE1C;
extern s32 fldTaskSlotCount;
extern s16 D_003C9510[];
extern u32 D_003BAE4C;
extern s32 D_003BAE50;
extern void fldClearMenuEntries();
extern void fldDestroyTitleTask();
extern void fldPlayPendingSounds();
extern void fldReleaseObjectSlots();
extern void fldReleaseTitleSlots();
extern void fldReleaseResourceHandles();
extern void fldReleaseTextureSlots();
extern void fldResetObjectSlots();
extern s32 fldTitleMiniIsActive();
extern void fldResetZoneRecordsAndActorSlots();
extern void fldResetPendingSounds();
extern void fldReleaseSceneRecordChunk();
extern void fldReleaseMenuSlotsAfterWait();
extern void mnuReleaseResourceEntries();
extern void sdfResourceListRelease();
extern void fldReleaseBackgroundBuffer(void);

extern FldWaypointBlock fldActorWaypointRows;
extern FldFileResource *D_003307B0[];
extern void fldDrawMarkerQuad(f32 *pos);

typedef struct {
    s16 unk0;
    s16 unk2;
} FldIndexPair;
extern FldIndexPair D_0032EF18[];
extern FldIndexPair D_0032EFE0[];
extern s32 D_003BAE54;
extern s32 D_003BAE5C;
extern s32 D_003BAE60;

extern u32 D_003C91D0[], D_003C91E0[], D_003C91F0[];
extern char D_003BAC40[];
extern void fldSpawnSceneResourceBatches(FldSpawnBatch *, u32);

/* Relocated collision headers contain their eight-word geometry header. */

typedef struct FldSpawnEvent {
    u32 flags;
    const char *label;
    u32 reserved[2];
} FldSpawnEvent;

#include "fld_placement_data.h"

typedef struct FldSpecialPoint {
    u32 kind;
    u32 id;
} FldSpecialPoint;

/* A relocated batch consists of a kind, count, and complete resource rows. */

extern FldFileResource *D_003BD7B8;
extern u32 D_003BD7BC;
extern s32 D_003BAC10;
extern s16 D_0032E4B4[];
extern s32 fldParseRoomNumberFromName(const char *);
extern void fldBuildCollisionRecordsFromGeometry(void *, f32 *, u32, s32, s32);
extern EffWorldNode *evtSpawnActionObjB(s32, void *, f32 *, const char *);
extern void dds3SetPathStateValue(EffWorldNode *, u32);
extern EffWorldNode *evtSpawnActionObjD(s32, void *, s32);
extern EffWorldNode *evtSpawnActionObj10(s32, void *, const char *);
extern u32 fldPushDisplayValue(u32, EffWorldNode *);
extern s32 func_00138ED0(FldFileResource *, EffWorldNode *);
extern void func_00148FF0(s32, u32, f32 *, f32, f32, f32);
extern void func_001480D0(u32, f32 *, f32, f32, f32, s32, const char *, s32);
extern void func_001486D0(u32, f32 *, f32, f32, f32);
extern s32 fldSetSparkVectors(s32, const u128 *, const u128 *);
extern void fldAppendClearEntry(s32, f32, f32, f32, f32);
extern EffWorldNode *dds3CreateCameraObject(s32, void *, void *);

/* Serialized transforms leave the position W component zero. */
static inline void fldCopySpawnTransform(f32 *position, f32 *rotation, const f32 *source) {
    if (source != NULL) {
        position[0] = source[0];
        position[1] = source[1];
        position[2] = source[2];
        position[3] = 0.0f;
        rotation[0] = source[4];
        rotation[1] = source[5];
        rotation[2] = source[6];
        rotation[3] = source[7];
    } else {
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;
        position[3] = 0.0f;
        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = 0.0f;
        rotation[3] = 0.0f;
    }
}

void fldSpawnPlacementObjectsFromBatches(u32 batchAddress, u32 batchCount, s32 appended) {
    f32 position[4];
    f32 rotation[4];
    f32 *transform;
    FldSpawnBatch *batch = (FldSpawnBatch *)batchAddress;
    FldFileResource *resource;
    u32 count;
    u32 i;
    u32 batchIndex;
    u32 pathState;
    FldSpawnEvent *event;
    u32 *parameters;
    FldSpecialPoint *point;
    EffWorldNode *object;
    s32 room;
    u32 id;

    if (appended == 0) {
        D_003BD7B8 = NULL;
        D_003BD7BC = 0;
    }
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, batch++) {
        resource = batch->entries;
        count = batch->count;
        switch (batch->kind) {
        case 0:
        case 1:
        case 2:
        case 5:
        case 8:
        case 11:
            break;
        case 3:
            if (appended != 0) {
                break;
            }
            D_003BD7B8 = resource;
            D_003BD7BC = count;
            pathState = 1;
            for (i = 0; i < count; i++, resource++, pathState++) {
                resource->id |= 0x10000;
                fldCopySpawnTransform(position, rotation, resource->transform);
                if (((FldSpawnCollision *)resource->data)->kind == 0) {
                    room = fldParseRoomNumberFromName(resource->name);
                    fldBuildCollisionRecordsFromGeometry(((FldSpawnCollision *)resource->data)->header,
                                 position, resource->id, room, -1);
                    object = evtSpawnActionObjB(resource->id, resource->data, position, resource->name);
                    dds3SetPathStateValue(object, pathState);
                }
            }
            break;
        case 6:
            for (i = 0; i < count; i++, resource++) {
                if (appended != 0) {
                    resource->id |= 0x800000;
                } else {
                    D_003BAC10++;
                }
                event = resource->data;
                object = evtSpawnActionObjD(resource->id, (void *)event->label, (s32)resource->name);
                fldPushDisplayValue((u32)resource, object);
            }
            break;
        case 9:
            for (i = 0; i < count; i++, resource++) {
                if (appended != 0) {
                    resource->id |= 0x800000;
                }
                evtSpawnActionObj10(resource->id, resource->data, resource->name);
            }
            break;
        case 10:
            for (i = 0; i < count; i++, resource++) {
                if (appended != 0) {
                    resource->id |= 0x800000;
                }
                transform = resource->transform;
                fldCopySpawnTransform(position, rotation, transform);
                switch (((FldPlacementData *)resource->data)->kind) {
                case 8:
                    point = ((FldPlacementData *)resource->data)->parameters;
                    switch (point->kind) {
                    case 1:
                        func_00148FF0(0, point->id, rotation, position[0], position[1], position[2]);
                        if (appended == 0) {
                            D_003BAC10++;
                        }
                        break;
                    case 2:
                        func_00148FF0(1, point->id, rotation, position[0], position[1], position[2]);
                        if (appended == 0) {
                            D_003BAC10++;
                        }
                        break;
                    case 3:
                        fldSetSparkVectors(point->id, (const u128 *)position, (const u128 *)rotation);
                        break;
                    }
                    break;
                case 0:
                    evtSpawnActionObj11(resource->id, (EvtWorldSourceTransformPrefix *)transform, (s32)resource->name);
                    break;
                case 1:
                    object = evtSpawnActionObj11(resource->id, (EvtWorldSourceTransformPrefix *)transform, (s32)resource->name);
                    func_00138ED0(resource, object);
                    break;
                case 2:
                    if (D_0032E4B4[0] != 0) {
                        break;
                    }
                    parameters = ((FldPlacementData *)resource->data)->parameters;
                    id = parameters[0];
                    room = fldParseRoomNumberFromName(resource->name);
                    if (appended != 0) {
                        FldPlacementData *placement = resource->data;
                        func_001480D0(id, rotation, position[0], position[1], position[2],
                                      room, resource->name, placement->event + D_003BAC10);
                    } else {
                        FldPlacementData *placement = resource->data;
                        func_001480D0(id, rotation, position[0], position[1], position[2],
                                      room, resource->name, placement->event);
                    }
                    break;
                case 3:
                    parameters = ((FldPlacementData *)resource->data)->parameters;
                    func_001486D0(parameters[0], rotation, position[0], position[1], position[2]);
                    if (appended == 0) {
                        D_003BAC10++;
                    }
                    break;
                case 7:
                    parameters = ((FldPlacementData *)resource->data)->parameters;
                    fldAppendClearEntry(parameters[1], *(f32 *)parameters,
                                         position[0], position[1], position[2]);
                    break;
                }
            }
            break;
        case 4:
            for (i = 0; i < count; i++, resource++) {
                if (appended != 0) {
                    resource->id |= 0x800000;
                }
                fldCopySpawnTransform(position, rotation, resource->transform);
                object = dds3CreateCameraObject(resource->id, position, rotation);
                ((CameraData *)object->data)->fieldOfView = *(f32 *)resource->data;
                dds3SetWorldNodeValue(object, (u32)resource->name);
            }
            break;
        case 7:
            for (;;) {
            }
            break;
        }
    }
}


void fldSpawnActionObjects(FldActionSpawn *list, u32 count) {
    u32 i;

    dds3GetWorldSecondaryObject();
    for (i = 0; i < count; i++) {
        struct EffWorldNode *node = evtSpawnActionObj2(list->firstValue, list->secondValue);
        list++;
        if (i == 0) {
            fldAreaState.taskHandle = (s32)node;
        }
    }
}

typedef struct FldScriptResource {
    u8 pad00[0x10];
    SdfItemListRef itemList;
    struct MotionTable *motionTable;
} FldScriptResource;

typedef char FldScriptResource_itemList_at_10[
    ((u32)&((FldScriptResource *)0)->itemList == 0x10) ? 1 : -1];
typedef char FldScriptResource_motionTable_at_20[
    ((u32)&((FldScriptResource *)0)->motionTable == 0x20) ? 1 : -1];

typedef struct FldResourceName {
    u32 unk00;
    const char *name;
} FldResourceName;

extern FldFileResource *D_003BD7B0;
extern u32 D_003BD7B4;
extern FldFileResource *D_003BD7B8;
extern u32 D_003BD7BC;
extern struct DevRequest *D_003BAC14;
extern EffWorldNode *dds3SpawnInnerVecObj6(s32, f32 *, void *);
extern void effObjSetActiveId(EffWorldNode *, s32);
extern s32 fldParseRoomNumberFromName(const char *);
extern void effObjSetRoomNumber(EffWorldNode *, u32);
extern void dds3CreateObjectModelAndMotion(void *);
extern void fldSetRecordValueById(s32, s32);
extern void dds3RegisterObjectInHandlerIndex(void *);

void fldCreateResourceScriptObjects(void) {
    f32 position[4];
    f32 rotation[4];
    FldFileResource *resource = D_003BD7B0;
    u32 count = D_003BD7B4;
    void *world;
    EffWorldNode *object;
    FldFileResource *binding;
    FldFileNameEntry *name;
    FldScriptResource *script;
    FldResourceName *linkedName;
    s32 i;
    s32 j;
    u32 k;

    if (resource == NULL) {
        return;
    }
    world = dds3GetWorldSecondaryObject();
    for (i = 0; i < count; i++, resource++) {
        script = resource->data;
        evtCreateScriptObjectWithResource(resource->id, &script->itemList,
                                         script->motionTable, D_003BAC14, resource->name);
        /* Retail fills both 16-byte stack vectors before creating the object. */
        if (resource->transform != NULL) {
            position[0] = resource->transform[0];
            position[1] = resource->transform[1];
            position[2] = resource->transform[2];
            position[3] = 0.0f;
            rotation[0] = resource->transform[4];
            rotation[1] = resource->transform[5];
            rotation[2] = resource->transform[6];
            rotation[3] = resource->transform[7];
        } else {
            position[0] = 0.0f;
            position[1] = 0.0f;
            position[2] = 0.0f;
            position[3] = 0.0f;
            rotation[0] = 0.0f;
            rotation[1] = 0.0f;
            rotation[2] = 0.0f;
            rotation[3] = 0.0f;
        }
        object = dds3SpawnInnerVecObj6(resource->id, position, rotation);
        dds3SetWorldNodeValue(object, (u32)resource->name);
        if (fldAreaState.area >= 200 && fldAreaState.area < 500) {
            if (fldAreaState.area == 230 && fldAreaState.floor == 6 && i == 2) {
                effObjSetActiveId(object, 6);
            } else {
                switch (i) {
                    case 0:
                        effObjSetActiveId(object, 2);
                        break;
                    case 1:
                        effObjSetActiveId(object, 3);
                        break;
                    case 2:
                        effObjSetActiveId(object, 4);
                        break;
                    default:
                        effObjSetActiveId(object, 5);
                        break;
                }
            }
        } else {
            effObjSetActiveId(object, 7);
        }
        effObjSetRoomNumber(object, fldParseRoomNumberFromName(resource->name));
        dds3SetSlotByKind(object, dds3FindWorldObjectNodeByKey(world, resource->id, 10));
        dds3CreateObjectModelAndMotion(object);
        binding = D_003BD7B8;
        for (j = 0; j < D_003BD7BC; j++, binding++) {
            if (binding->names != NULL) {
                name = binding->names->entries;
                k = 0;
                for (; k < binding->names->count; k++, name++) {
                    if (strcmp(resource->name, name->name) == 0) {
                        fldSetRecordValueById(binding->id, (s32)object);
                        break;
                    }
                }
            }
        }
        /* Retail fetches the link descriptor even when the object is NULL. */
        linkedName = (FldResourceName *)resource->word14;
        if (object != NULL) {
            dds3SetSlotByKind(object, dds3FindIndexedObjectChainNodeByName(world, 2, linkedName->name));
            dds3RegisterObjectInHandlerIndex(object);
        }
    }
}

#include "eff_object.h"
#include "dds3_path.h"
#include "fld_resource_resolver.h"

extern s32 fldTestMapSlotValueFlag(s32, u32, s32);
extern s32 fldTestRoomModeFlag(s32, u32, s32);
extern s32 fldFindSearchId(const char *);
extern void dds3EnsureSlotData(void *);
extern void dds3ReplaceObjectResource(EffWorldNode *);
extern void sdfSetFloatCounterDirection(u32 *, u32);
extern void evtScaleSlotByClampedMultiplier(void *, f32);
extern void dds3InvokeMoverUpdate(EffWorldNode *);
extern void dds3SetObjectPayloadWord8(EffWorldNode *, u32);
extern s32 sprintf(char *, const char *, ...);
extern char D_0039FE08[], D_0039FE18[], D_0039FE28[];

/* Restore the room's named objects and enable their retained path controllers. */
void fldActivateRoomFlagObjects(void) {
    char modelName[16];
    char collisionName[16];
    char pathName[16];
    void *world = dds3GetWorldSecondaryObject();
    s32 index;

    for (index = 1; index < 16; index++) {
        if (fldTestMapSlotValueFlag(fldAreaState.area, fldAreaState.floor + 1, index)) {
            s32 modelKey, collisionKey;
            EffWorldNode *object;
            EffWorldNode *path;
            ObjBase *data;
            EffWorldNode *slot;
            Dds3SlotResource *resource;

            sprintf(modelName, D_0039FE08, index);
            sprintf(collisionName, D_0039FE18, index);
            sprintf(pathName, D_0039FE28, index);
            modelKey = fldFindSearchId(modelName);
            collisionKey = fldFindSearchId(collisionName);
            if (modelKey == -1) {
                object = fldResolveWorldObjectByResourceId(collisionKey);
            } else {
                object = (EffWorldNode *)dds3FindWorldObjectNodeByKey(world, modelKey, 6);
                if (object == NULL) {
                    object = fldResolveWorldObjectByResourceId(collisionKey);
                }
            }
            path = (EffWorldNode *)dds3FindIndexedObjectChainNodeByName(world, 0x10, (const u8 *)pathName);
            data = effObjGetDataHandle(object);
            slot = (EffWorldNode *)data->slots[1];
            if (slot == NULL) {
                dds3EnsureSlotData(object);
                slot = (EffWorldNode *)data->slots[1];
            }
            resource = (Dds3SlotResource *)slot->data;
            if (resource->sourceObject == 0) {
                dds3SetSlotKey(slot, path);
                dds3ReplaceObjectResource(slot);
            }
            sdfSetFloatCounterDirection((u32 *)dds3GetObjectResourceHandle(slot), 0);
            evtScaleSlotByClampedMultiplier(object, 1.0f);
            dds3InvokeMoverUpdate(slot);
        }
        if (fldTestRoomModeFlag(fldAreaState.area, fldAreaState.floor + 1, index)) {
            s32 key;
            EffWorldNode *object;

            sprintf(modelName, D_0039FE08, index);
            key = fldFindSearchId(modelName);
            object = (EffWorldNode *)dds3FindWorldObjectNodeByKey(world, key, 6);
            dds3SetObjectPayloadWord8(object, 2);
        }
    }
}



void fldLoadSceneRequestFiles(FldLoadRequest *request) {
    char directory[32];
    char path[64];
    s32 i;

    fldSetDisplayState(request->record->displayState);
    D_003C91D0[0] = (u32)request->record->resourceA;
    D_003C91D0[1] = (u32)request->record->resourceB;
    D_003C91D0[2] = (u32)request->record->resourceC;
    D_003C91D0[3] = (u32)request->record->resourceD;
    for (i = 0; i < 4; i++) {
        D_003C91E0[i] = 0;
        D_003C91F0[i] = 0;
    }
    if (fldAreaState.area < 500) {
        for (i = 0; i < 4; i++) {
            if (D_003C91D0[i] != 0) {
                fldFormatAreaDirectory(directory, fldAreaState.area, fldAreaState.floor + 1);
                sprintf(path, D_003BAC40, directory, D_003C91D0[i]);
                D_003C91E0[i] = (u32)sdfReadNamedResource(path, &D_003C91F0[i], 0);
            }
        }
    }
    fldSpawnSceneResourceBatches(request->spawnBatches, request->spawnBatchCount);
}

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(FldSceneRequest *request) {
    s32 state;
    fldSpawnActionObjects(request->spawnList, request->spawnCount);
    state = fldAreaState.area;
    if (state != 1 && state < 200) fldCreatePlayerObject();
    fldSpawnPlacementObjectsFromBatches(request->unk04, request->unk00, 0);
    fldAreaState.fallbackResourceName = request->record->displayState;
}

void fldProcessFieldRequestAlternate(FldSceneRequest *request) {
    fldSpawnPlacementObjectsFromBatches(request->unk04, request->unk00, 1);
}

s32 fldGetTransferChunkPayload(s32 record) {
    return record + 0xc;
}

void fldRelocatePackedWords(u32 *table, u32 base, u8 *data, s32 size) {
    u8 *p = data;
    s32 code;
    s32 i;

    while (p - data < size) {
        code = *p++;
        if ((code & 1) == 0) {
            code >>= 1;
        } else if ((code & 2) == 0) {
            code = (code | (*p++ << 8)) >> 2;
        } else if ((code & 4) == 0) {
            code = (code | (p[0] << 8) | (p[1] << 16)) >> 3;
            p += 2;
        } else {
            code = (code >> 3) + 2;
            for (i = 0; i < code; i++) {
                table++;
                *table -= base;
            }
            continue;
        }
        table += code;
        *table -= base;
    }
}

/* Relocate the words described by this packed-resource transfer chunk. */
void fldRelocatePackedTransferChunk(u32 buffer, FldTransferChunk *chunk) {
    sdfRelocatePackedResourceWords(buffer, buffer, (s32)buffer + chunk->offset, chunk->size);
}

void fldRelocateTransferChunkWords(u32 buffer, FldTransferChunk *chunk) {
    fldRelocatePackedWords((u32 *)buffer, buffer, (u8 *)((s32)buffer + chunk->offset), chunk->size);
}

void fldSetPendingAreaAndFloor(u32 area, u32 floor) {
    fldPendingArea = area;
    fldPendingFloor = floor;
}

extern char D_0039FE38[]; /* "%sf%03d_%03d.LB" */
extern s32 sprintf(char *, const char *, ...);

s32 fldLoadAreaResource(void) {
    char directory[64];
    char path[80];
    u32 area = fldPendingArea;
    u32 floor = fldPendingFloor;

    if (area != 0 || floor != 0) {
        fldFreeDisplayObjects();
        fldAreaState.resourceArea = area;
        fldAreaState.resourceFloor = floor;
        fldFormatAreaDirectory(directory, area, 1);
        sprintf(path, D_0039FE38, directory, area, floor);
        fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
        fldAreaState.resourceFlag = 1;
        return 1;
    }
    return 0;
}

extern s32 fldGetLocationCoordinateValue(s32, s32);
extern void sdfRaiseDeviceThreadPriority(void);
extern s32 D_003BAC80;
extern void fldFreeDisplayObjects(void);



s32 fldRequestAreaResource(s32 area, s32 room) {
    char directory[64];
    char path[80];

    if (fldAreaState.area != area) {
        return 1;
    }
    if (fldAreaState.resourceFlag == 1) {
        return 1;
    }
    if (fldAreaState.resourceArea == area &&
        fldAreaState.resourceFloor == room) {
        fldAreaState.resourceFlag = 2;
        return 1;
    }
    fldFreeDisplayObjects();
    fldAreaState.resourceArea = area;
    fldAreaState.resourceFloor = room;
    if (fldGetLocationCoordinateValue(area, room) & 0x10) {
        D_003BAC80 = 1;
    } else {
        D_003BAC80 = 0;
    }
    sdfRaiseDeviceThreadPriority();
    D_003BA734 = 1;
    fldFormatAreaDirectory(directory, area, 1);
    sprintf(path, D_0039FE38, directory, area, room);
    fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
    fldAreaState.resourceFlag = 1;
    return 1;
}

void fldFreeDisplayObjects(void) {
    if (fldAreaLoadRequest != 0) {
        PacWork *work = ((FilePacRequest *)fldAreaLoadRequest)->packet.queueHead;

        if (work != NULL) {
            do {
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)work->resourceHandle);
                work = work->next;
            } while (work != NULL);
        }
        btlStageTaskCleanupCallback(fldAreaLoadRequest);
        fldAreaLoadRequest = NULL;
    }
    fldPendingArea = 0;
    fldPendingFloor = 0;
    fldAreaState.resourceArea = 0;
    fldAreaState.resourceFloor = 0;
    fldAreaState.resourceFlag = 0;
}

u32 fldPollAreaResourceLoad(void) {
    u32 sceneState = fldAreaState.resourceFlag;

    if (sceneState != 0) {
        if (sceneState == 1) {
            if (fileRequestIsReady(fldAreaLoadRequest) != 0) {
                fldAreaState.resourceFlag = 0;
                D_003BA734 = 0;
            }
        }
    }
    return 0;
}

u32 fldGetResourceReadyFlag(void) {
    return D_0032E428[0];
}

u8 fldIsAreaResourceReady(void) {
    if (fldAreaLoadRequest != 0) {
        if (fileRequestIsReady(fldAreaLoadRequest) != 0) {
            return 1;
        }
    }
    return D_0032E428[0] != 0;
}

s32 fldIsAreaFloorResourceReady(s32 area, s32 room) {
    if (fldAreaState.resourceArea != area || fldAreaState.resourceFloor != room) {
        return 0;
    }
    if (fldAreaLoadRequest != 0 && fileRequestIsReady(fldAreaLoadRequest) != 0) {
        return 1;
    }
    return fldAreaState.resourceFlag != 0;
}

extern u32 fldCachedRoomResourceData, fldCachedRoomF1ResourceData, fldCachedRoomF2ResourceData, fldCachedRoomKF2ResourceData;
extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;

struct SdfMemBlock *fldLoadCachedRoomResourceIfLocationMatches(void **destination, s32 area, s32 room) {
    if (fldAreaState.resourceArea == area) {
        if (fldAreaState.resourceFloor == room) {
            struct SdfMemBlock *allocation = sdfAllocGeneralBlock(fldCachedRoomResourceSize);
            void *data = (void *)sdfResourceRetainAddress(allocation);
            *destination = data;
            memcpy(data, (void *)fldCachedRoomResourceData, fldCachedRoomResourceSize);
            return allocation;
        }
    }
    return NULL;
}

struct SdfMemBlock *fldLoadCachedRoomF1ResourceIfLocationMatches(void **destination, s32 area, s32 room) {
    if (fldAreaState.resourceArea == area) {
        if (fldAreaState.resourceFloor == room) {
            struct SdfMemBlock *allocation = sdfAllocGeneralBlock(fldCachedRoomF1ResourceSize);
            void *data = (void *)sdfResourceRetainAddress(allocation);
            *destination = data;
            memcpy(data, (void *)fldCachedRoomF1ResourceData, fldCachedRoomF1ResourceSize);
            return allocation;
        }
    }
    return NULL;
}

struct SdfMemBlock *fldLoadCachedRoomF2ResourceIfLocationMatches(void **destination, s32 area, s32 room) {
    if (fldAreaState.resourceArea == area) {
        if (fldAreaState.resourceFloor == room) {
            struct SdfMemBlock *allocation = sdfAllocGeneralBlock(fldCachedRoomF2ResourceSize);
            void *data = (void *)sdfResourceRetainAddress(allocation);
            *destination = data;
            memcpy(data, (void *)fldCachedRoomF2ResourceData, fldCachedRoomF2ResourceSize);
            return allocation;
        }
    }
    return NULL;
}

struct SdfMemBlock *fldLoadCachedRoomKF2ResourceIfLocationMatches(void **destination, s32 area, s32 room) {
    if (fldAreaState.resourceArea == area) {
        if (fldAreaState.resourceFloor == room) {
            struct SdfMemBlock *allocation = sdfAllocGeneralBlock(fldCachedRoomKF2ResourceSize);
            void *data = (void *)sdfResourceRetainAddress(allocation);
            *destination = data;
            memcpy(data, (void *)fldCachedRoomKF2ResourceData, fldCachedRoomKF2ResourceSize);
            return allocation;
        }
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_0039FE08);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_0039FE18);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_0039FE28);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_0039FE38);

void fldFormatAreaResourceName(char *out) {
    char directory[32];
    s32 area = fldAreaState.area;
    s32 room = fldAreaState.floor;

    fldFormatAreaDirectory(directory, area, 1);
    if (area == 0x1C) {
        if (mdlFlagTest(0x5E1)) {
            sprintf(out, "%sf%03d_000.LB", directory, 0x1C);
        } else if (mdlFlagTest(0x5E2)) {
            sprintf(out, "%sf%03d_00a.LB", directory, 0x1C);
        } else if (mdlFlagTest(0x5E3)) {
            sprintf(out, "%sf%03d_00b.LB", directory, 0x1C);
        } else if (mdlFlagTest(0x5E4)) {
            sprintf(out, "%sf%03d_00c.LB", directory, 0x1C);
        } else {
            sprintf(out, "%sf%03d_00d.LB", directory, 0x1C);
        }
        return;
    }
    if (area == 0x1B) {
        switch (room) {
        case 8 ... 21:
            sprintf(out, "%sf%03d_00a.LB", directory, area);
            break;
        case 7:
            sprintf(out, "%sf%03d_00b.LB", directory, area);
            break;
        case 22:
        case 23:
        case 27:
            if (mdlFlagTest(0x58D)) {
                sprintf(out, "%sf%03d_00d.LB", directory, area);
            } else {
                sprintf(out, "%sf%03d_00c.LB", directory, area);
            }
            break;
        case 24 ... 26:
        default:
            sprintf(out, "%sf%03d_000.LB", directory, area);
            break;
        }
        return;
    }
    sprintf(out, "%sf%03d_000.LB", directory, area);
}

u8 fldHasAreaResourceNameChanged(void) {
    char buf[32];

    fldFormatAreaResourceName(buf);
    return strcmp(D_003C9200, buf) != 0;
}


extern void fldSetNpcPalette(void *source);
extern void fldUploadSkyBuffer(void *source);
extern void fldCopyActorWaypointTable(const void *source);
extern void fldCopyInfoTable(const void *);
extern void fldSetSceneRecordChunk(u8 *chunk, s32 resourceHandle);
extern void fldCacheMapLabelLengths();

void fldLoadAreaPackedResources(void) {
    char name[32];
    FilePacRequest *request;
    PacWork *work;

    if (fldAreaState.area < 200) {
        fldFormatAreaResourceName(name);
        strcpy(D_003C9200, name);
        fldAreaPackedArchive = fileQueuePlainDispatchRequest(name);
        fileWaitForRequestReady(fldAreaPackedArchive);
        request = (FilePacRequest *)fldAreaPackedArchive;
        for (work = request->packet.queueHead; work != NULL; work = work->next) {
            switch (*(u16 *)(work->packet + 2)) {
            case FLD_PACKED_RESOURCE_INFO_TABLE:
                fldCopyInfoTable(work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_NPC_PALETTE:
                fldSetNpcPalette(work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_SKY_BUFFER:
                fldUploadSkyBuffer(work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE:
                fldCopyActorWaypointTable(work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_RETAINED_COPY:
                fldAreaCachedResource = (u32)sdfAllocGeneralBlock(
                    sdfMemoryGetBlockSize((struct SdfMemBlock *)work->resourceHandle));
                memcpy((void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)fldAreaCachedResource),
                       (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)work->resourceHandle),
                       sdfMemoryGetBlockSize((struct SdfMemBlock *)work->resourceHandle));
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK:
                fldSetSceneRecordChunk(work->dataCursor, (s32)work->resourceHandle);
                break;
            }
        }
        fldCacheMapLabelLengths(fldAreaState.area % 100);
    }
}

void fldReleaseAreaResourceCache(void) {
    u32 resourceHandle = fldAreaCachedResource;

    if (resourceHandle != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)resourceHandle);
        fldAreaCachedResource = 0;
    }
    resourceHandle = (u32)fldAreaPackedArchive;
    if (resourceHandle != 0) {
        btlStageTaskCleanupCallback((void *)resourceHandle);
        fldAreaPackedArchive = 0;
    }
    D_003C9200[0] = D_003BAC90[0];
}

extern s32 D_003BAC10;
extern u32 D_003BAC30;
extern s32 D_003BAE40;
extern FldTransferChunk *D_003BAC18;
extern u32 D_003BAC1C;
extern FldTransferChunk *D_003BAC20;
extern u32 D_003BAC24;
extern FldTransferChunk *D_003BAC28;
extern u32 D_003BAC2C;
extern struct FileRequest *D_003BAC38;
extern s32 D_003BAC4C;
extern u32 fldCachedRoomResourceAllocation;
extern u32 fldCachedRoomF1ResourceAllocation;
extern u32 fldCachedRoomF2ResourceAllocation;
extern u32 fldCachedRoomKF2ResourceAllocation;
extern u32 fldAreaCachedResource;
extern void fldSetSceneLocation(s32 area, s32 floor, s32 stage);
extern void fldLoadPlayerModel(void);
extern void fldInitSparkTable(void);
extern void fldInitializeMenuResources(void);
extern void fldUpdateSparkSlots(void);
extern void fldActivateRoomFlagObjects(void);
extern void fldInitializeActorMotionSlots(void);
extern DevRequest *sndLoadNamedOffsetResourceList(const char *name);
extern s32 evtRetainSceneResource(EffWorldNode *worldNode, void *resourceHandle);
extern char D_003BAC98[];
extern char D_003BACA0[];
extern char D_003BACA8[];
extern FieldPlayerSceneWork D_0032F1A0;
extern void fldAllocateBackgroundBuffer(void);
extern void fldResetObjectSlots();
extern void fldReleaseTitleSlots();
extern void fldReleaseSceneRecordChunk();
extern void fldCreateResourceScriptObjects(void);
extern u8 fldHasAreaResourceNameChanged(void);
extern void fldLoadAreaPackedResources(void);
extern void fldReleaseAreaResourceCache(void);
extern void fldRelocatePackedTransferChunk(u32 buffer, FldTransferChunk *chunk);

const char D_0039FF18[] = "%sk%03d_%03d.f2";

u32 fldLoadAreaRoomResources(const char *name) {
    char path[0x80];
    char directory[0x40];
    void *roomOffsets;
    void *roomData;
    FldTransferChunk *chunk;
    struct FileRequest *request;
    FieldPlayerSceneWork *sceneWork = &D_0032F1A0;
    s32 area;
    s32 room;
    s32 isField;
    PacWork *work;
    s32 index;

    D_003BAC30 = 0;
    D_003BAC10 = 0;
    fldCachedRoomF1ResourceAllocation = 0;
    fldCachedRoomF2ResourceAllocation = 0;
    fldCachedRoomKF2ResourceAllocation = 0;
    D_003BAE40 = 0;
    area = dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) >> 16;
    room = dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) & 0xFFFF;
    fldSetSceneLocation(area, room - 1, 0);
    if (fldAreaState.area != 1) {
        if (fldAreaState.area < 200) {
            fldLoadPlayerModel();
            fldInitSparkTable();
        }
        if ((u32)(fldAreaState.area - 200) >= 300) {
            fldAllocateBackgroundBuffer();
        }
    }
    fldResetRecordState();
    fldResetZoneRecordsAndActorSlots();
    fldResetPendingSounds();
    fldClearMenuEntries();
    fldResetObjectSlots();
    if (fldAreaState.unk20 == 0) {
        fldInitializeMenuResources();
        fldLoadAreaPackedResources();
    } else if (fldHasAreaResourceNameChanged() != 0 || (u32)(fldAreaState.area - 0x1B) < 2) {
        fldReleaseTitleSlots();
        fldReleaseSceneRecordChunk();
        fldReleaseAreaResourceCache();
        fldLoadAreaPackedResources();
    }
    isField = (u32)(fldAreaState.area - 200) < 300;
    if (fldAreaState.area < 100) {
        isField = 1;
    }
    if (isField != 0) {
        if (fldIsAreaFloorResourceReady(area, room) != 0) {
            request = fldAreaLoadRequest;
            fldAreaLoadRequest = NULL;
            D_003BAC38 = request;
            fldFreeDisplayObjects();
        } else {
            fldFormatAreaDirectory(directory, area, 1);
            sprintf(path, D_0039FE38, directory, area, room);
            request = fileQueuePlainDispatchRequest(path);
            D_003BAC38 = request;
            fileWaitForRequestReady(request);
        }
        index = 0;
        D_003BAC14 = NULL;
        work = ((FilePacRequest *)D_003BAC38)->packet.queueHead;
        fldCachedRoomResourceAllocation = 0;
        for (; work != NULL; work = work->next) {
            switch (index) {
            case 0:
                D_003BAC14 = sndBuildResourceHandleListFromOffsets((const SdfTextureOffsetListHeader *)work->dataCursor);
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)work->resourceHandle);
                fldCachedRoomResourceAllocation = 0;
                break;
            case 1:
                fldCachedRoomF2ResourceAllocation = 0;
                roomData = work->dataCursor;
                chunk = (FldTransferChunk *)((u8 *)roomData + 8);
                D_003BAC24 = (u32)roomData;
                D_003BAC20 = chunk;
                fldRelocatePackedTransferChunk((u32)roomData, chunk);
                fldProcessFieldRequest((FldSceneRequest *)fldGetTransferChunkPayload((s32)chunk));
                break;
            case 2:
                fldCachedRoomF1ResourceAllocation = 0;
                roomData = work->dataCursor;
                chunk = (FldTransferChunk *)((u8 *)roomData + 8);
                D_003BAC1C = (u32)roomData;
                D_003BAC18 = chunk;
                fldRelocatePackedTransferChunk((u32)roomData, chunk);
                fldLoadSceneRequestFiles((FldLoadRequest *)fldGetTransferChunkPayload((s32)chunk));
                break;
            }
            index++;
        }
    }
    if (isField == 0) {
        fldCachedRoomResourceAllocation = (u32)fldLoadCachedRoomResourceIfLocationMatches(&roomOffsets, area, room);
        if (fldCachedRoomResourceAllocation == 0) {
            sprintf(path, D_003BAC98, name);
            D_003BAC14 = sndLoadNamedOffsetResourceList(path);
        } else {
            D_003BAC14 = sndBuildResourceHandleListFromOffsets(roomOffsets);
            sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldCachedRoomResourceAllocation);
            fldCachedRoomResourceAllocation = 0;
        }
    }
    if (isField == 0) {
        sprintf(path, D_003BACA0, name);
        fldCachedRoomF2ResourceAllocation = (u32)fldLoadCachedRoomF2ResourceIfLocationMatches(&roomData, area, room);
        if (fldCachedRoomF2ResourceAllocation == 0) {
            fldCachedRoomF2ResourceAllocation = (u32)sdfReadNamedResource(path, &roomData, 0);
        }
        chunk = (FldTransferChunk *)((u8 *)roomData + 8);
        D_003BAC24 = (u32)roomData;
        D_003BAC20 = chunk;
        fldRelocatePackedTransferChunk((u32)roomData, chunk);
        fldProcessFieldRequest((FldSceneRequest *)fldGetTransferChunkPayload((s32)chunk));
    }
    if (fldGetLocationCoordinateValue(area, room) & 0x10) {
        fldFormatAreaDirectory(directory, area, 1);
        sprintf(path, D_0039FF18, directory, area, room);
        fldCachedRoomKF2ResourceAllocation = (u32)fldLoadCachedRoomKF2ResourceIfLocationMatches(&roomData, area, room);
        if (fldCachedRoomKF2ResourceAllocation == 0) {
            fldCachedRoomKF2ResourceAllocation = (u32)sdfReadNamedResource(path, &roomData, 0);
        }
        chunk = (FldTransferChunk *)((u8 *)roomData + 8);
        D_003BAC2C = (u32)roomData;
        D_003BAC28 = chunk;
        fldRelocatePackedTransferChunk((u32)roomData, chunk);
        fldProcessFieldRequestAlternate((FldSceneRequest *)fldGetTransferChunkPayload((s32)chunk));
    }
    if (isField == 0) {
        sprintf(path, D_003BACA8, name);
        fldCachedRoomF1ResourceAllocation = (u32)fldLoadCachedRoomF1ResourceIfLocationMatches(&roomData, area, room);
        if (fldCachedRoomF1ResourceAllocation == 0) {
            fldCachedRoomF1ResourceAllocation = (u32)sdfReadNamedResource(path, &roomData, 0);
        }
        chunk = (FldTransferChunk *)((u8 *)roomData + 8);
        D_003BAC1C = (u32)roomData;
        D_003BAC18 = chunk;
        fldRelocatePackedTransferChunk((u32)roomData, chunk);
        fldLoadSceneRequestFiles((FldLoadRequest *)fldGetTransferChunkPayload((s32)chunk));
    }
    fldCreateResourceScriptObjects();
    fldActivateRoomFlagObjects();
    if (sceneWork->unk58 != 0 && area < 200 && fldAreaCachedResource != 0) {
        D_003BAC4C = (s32)sdfAllocGeneralBlock(sdfMemoryGetBlockSize((struct SdfMemBlock *)fldAreaCachedResource));
        memcpy((void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)D_003BAC4C),
               (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)fldAreaCachedResource),
               sdfMemoryGetBlockSize((struct SdfMemBlock *)fldAreaCachedResource));
        evtRetainSceneResource(dds3GetWorldSecondaryObject(), (void *)D_003BAC4C);
    }
    fldInitializeActorMotionSlots();
    fldAreaState.unk20 = 0;
    if (fldAreaState.transitionMode != 0) {
        fldUpdateSparkSlots();
    }
    return fldCachedRoomF1ResourceAllocation;
}

extern u32 fldLoadAreaRoomResourcesFromRequest(u32, u32, u32, u32,
                        const SdfTextureOffsetListHeader *, u32);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLoadAreaRoomResourcesFromRequest);

extern s32 D_003BAC10;
extern FldTransferChunk *D_003BAC18;
extern u32 D_003BAC1C;
extern FldTransferChunk *D_003BAC20;
extern u32 D_003BAC24;
extern FldTransferChunk *D_003BAC28;
extern u32 D_003BAC2C;
extern struct FileRequest *D_003BAC38;
extern s32 D_003BAC4C;
extern u32 fldCachedRoomResourceAllocation;
extern u32 fldCachedRoomF1ResourceAllocation;
extern u32 fldCachedRoomF2ResourceAllocation;
extern u32 fldCachedRoomKF2ResourceAllocation;

void fldReleaseFieldResources(void) {
    s32 i;
    PacWork *work;

    fldReleaseBackgroundBuffer();
    fldPlayPendingSounds();
    fldDestroyTitleTask();
    if (fldTitleMiniIsActive() != 0) {
        kwlnTaskDestroyWithHierarchyByName("fldTitleMini", 1);
    }
    fldReleaseObjectSlots();
    fldResetObjectSlots();
    if (fldAreaState.area < 0xC8) {
        if (fldAreaState.unk20 != 0) {
            if ((u32)(fldAreaState.area - 0x1B) < 2U) {
                fldReleaseTitleSlots(&fldAreaState);
                fldReleaseMenuSlotsAfterWait();
                fldReleaseSceneRecordChunk();
                fldReleaseAreaResourceCache();
            }
        } else {
            fldReleaseTitleSlots(&fldAreaState);
            fldReleaseMenuSlotsAfterWait();
            fldReleaseSceneRecordChunk();
            fldReleaseResourceHandles();
            fldReleaseAreaResourceCache();
            fldReleaseSkyResources();
        }
        fldResetRecordState();
        fldResetZoneRecordsAndActorSlots();
        fldResetPendingSounds();
        fldClearMenuEntries();
        mnuReleaseResourceEntries();
        fldReleaseTextureSlots();
    }
    sdfResourceListRelease(D_003BAC14, 1);
    D_003BAC14 = 0;
    for (i = 0; i < 4; i++) {
        if (D_003C91E0[i] != 0) {
            sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_003C91E0[i]);
            D_003C91E0[i] = 0;
            D_003C91F0[i] = 0;
        }
    }
    if (D_003BAC30 != 0) {
        if (D_003BAC18 != 0) {
            fldRelocateTransferChunkWords(D_003BAC1C, D_003BAC18);
            D_003BAC18 = 0;
            D_003BAC1C = 0;
        }
        if (D_003BAC20 != 0) {
            fldRelocateTransferChunkWords(D_003BAC24, D_003BAC20);
            D_003BAC20 = 0;
            D_003BAC24 = 0;
        }
        if (D_003BAC28 != 0) {
            fldRelocateTransferChunkWords(D_003BAC2C, D_003BAC28);
            D_003BAC28 = 0;
            D_003BAC2C = 0;
        }
    }
    D_003BAC10 = 0;
    D_003BAC4C = 0;
    if (fldCachedRoomResourceAllocation != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldCachedRoomResourceAllocation);
        fldCachedRoomResourceAllocation = 0;
    }
    if (fldCachedRoomF1ResourceAllocation != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldCachedRoomF1ResourceAllocation);
        fldCachedRoomF1ResourceAllocation = 0;
    }
    if (fldCachedRoomF2ResourceAllocation != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldCachedRoomF2ResourceAllocation);
        fldCachedRoomF2ResourceAllocation = 0;
    }
    if (fldCachedRoomKF2ResourceAllocation != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldCachedRoomKF2ResourceAllocation);
        fldCachedRoomKF2ResourceAllocation = 0;
    }
    if (fldAreaState.area < 0xC8 && fldAreaState.cachedArea != fldAreaState.area) {
        fldAreaState.cachedArea = fldAreaState.area;
    }
    if (D_003BAC38 != 0) {
        work = ((FilePacRequest *)D_003BAC38)->packet.queueHead;
        i = 0;
        if (work != 0) {
            do {
                if (i > 0) {
                    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)work->resourceHandle);
                }
                work = work->next;
                i++;
            } while (work != 0);
        }
        btlStageTaskCleanupCallback(D_003BAC38);
        D_003BAC38 = NULL;
    }
}

extern s32 sprintf(char *, const char *, ...);

extern s32 sprintf(char *, const char *, ...);
/* All 25 code words match. Check_unit rodata range extends beyond selector strings into the separately-owned next string. */
void fldFormatAreaDirectory(char *path, s32 field, s32 unused) {
    if (field < 200) sprintf(path, "/fld/f/f%03d/", field);
    else if (field < 500) sprintf(path, "/fld/b/f%03d/", field);
    else sprintf(path, "/fld/e/f%03d/", field);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00128BB8);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_0039FF88);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00128DA0);

/* Enable or skip relocation of remaining transfer chunks during field teardown. */
void fldSetRelocateOnRelease(u32 relocateOnRelease) {
    D_003BAC30 = relocateOnRelease;
}

void fldInitDisplayObjects(void) {
    if (D_003BACD4 == 0) {
        SdfAsset *object;
        D_003BACD4 = 1;
        object = sdfCreateAssetWithDrawEntries();
        D_003BACEC = (u32)object;
        object->primaryStateFloat = 1.0f;
        D_003BD7C0 = (u32)sdfCreateAssetWithDrawEntries();
        fldMarkerTexture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(D_0032F260));
    }
}

SdfPoolNode *fldGetDisplayTableRow(void) {
    return &kwlnDrawSurfaces[fldDisplayRow];
}

typedef struct FldSpriteCorner {
    s32 u, v;
    u8 pad08[8];
    s32 x, y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} FldSpriteCorner;

/* Sprite vertex record (0x50 bytes): colour, then two corners of the quad. */
typedef struct FldSpriteVertex {
    s32 r, g, b, a;
    FldSpriteCorner corner[2];
} FldSpriteVertex;

void fldSubmitSpriteRect(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, SdfTex *texture) {
    void *handle = sdfConsAllocateColumnPacket(1);
    FldSpriteVertex *vtx = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    SdfListHead *command;
    SdfPoolNode *descriptor;

    vtx->r = color & 0xFF;
    vtx->g = (color >> 8) & 0xFF;
    vtx->b = (color >> 16) & 0xFF;
    vtx->a = (color >> 24) & 0xFF;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + w * 16;
    vtx->corner[1].y = yt + h * 8;
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    sdfConsCreateDrawPacket(command, texture, 0);
    sdfAppendPacket(command, (u32)handle);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void evtSubmitTexturedRectPacket(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, SdfTex *texture) {
    SdfListHead *list;
    SdfPoolNode *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = sdfCreateResetPacketList();
    sdfConsCreateDrawPacket(list, texture, 0);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (width << 4) + 0x7000;
    bottom = top + (height << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (textureWidth << 4);
    vBottom = vTop + (textureHeight << 4);
    sdfQueueGouraudTexturedQuad(list, 0x40,
        left, top, uLeft, vTop, color0,
        right, top, uRight, vTop, color1,
        left, bottom, uLeft, vBottom, color3,
        right, bottom, uRight, vBottom, color2,
        -1, NULL);
    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append(surface, list);
}

void fldSubmitSpriteRectFloat(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, SdfTex *texture) {
    void *handle = sdfConsAllocateColumnPacket(1);
    FldSpriteVertex *vtx = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)handle);
    s32 r = color & 0xFF;
    s32 g = (color >> 8) & 0xFF;
    s32 b = (color >> 16) & 0xFF;
    s32 a = (color >> 24) & 0xFF;
    s32 ubase = u * 16;
    s32 vbase = v * 16;
    s32 xl = (s32)(x * 16.0f) + 0x7000;
    s32 yt = (s32)(y * 8.0f) + 0x7900;
    SdfListHead *command;
    SdfPoolNode *descriptor;

    vtx->r = r;
    vtx->g = g;
    vtx->b = b;
    vtx->a = a;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + (s32)(w * 16.0f);
    vtx->corner[1].y = yt + (s32)(h * 8.0f);
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    sdfConsCreateDrawPacket(command, texture, 0);
    sdfAppendPacket(command, (u32)handle);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

extern u8 D_00324650[];

void fldProjectPointSetup(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

        VU0_LOAD_MATRIX(sdfViewMatrix);
;
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    VU0_MOVE_MATRIX_TO_B();
    VU0_LOAD_VF_MEMORY(vf10, vec);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF_MEMORY(vf11, D_00324650);
;
    VU0_MUL(vf10, vf10, vf11);
;
    VU0_LOAD_VF_MEMORY(vf11, D_00324660);
;
    VU0_ADD(vf10, vf10, vf11);
;
    VU0_STORE_VF(vf10, result);
;
    *dstX = result[0];
    *dstY = result[1];
}

extern u8 D_003249B0[];
extern u8 D_003249F0[];
extern u8 D_00324A00[];
extern u8 D_00329790[];

void fldProjectPointSetupAlt(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

        VU0_LOAD_MATRIX(D_00329790);
;
    sdfPostmultiplyVuMatrixFromMemory(D_003249B0);
    VU0_MOVE_MATRIX_TO_B();
    VU0_LOAD_VF_MEMORY(vf10, vec);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF_MEMORY(vf11, D_003249F0);
;
    VU0_MUL(vf10, vf10, vf11);
;
    VU0_LOAD_VF_MEMORY(vf11, D_00324A00);
;
    VU0_ADD(vf10, vf10, vf11);
;
    VU0_STORE_VF(vf10, result);
;
    *dstX = result[0];
    *dstY = result[1];
}

/* vu0 routine: prepare the shared projection and secondary matrix bank. */
void fldPrepareProjectionMatrix(void) {
    u8 *matrix;
        VU0_LOAD_MATRIX(sdfViewMatrix);
;
    matrix = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(matrix);
    VU0_MOVE_MATRIX_TO_B();
    matrix += 0x40;
    VU0_LOAD_VF_MEMORY(vf11, matrix);
;
    VU0_LOAD_VF_MEMORY(vf12, D_00324660);
;
}

void fldProjectPointWithPreparedMatrix(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];
    VU0_LOAD_VF_MEMORY(vf10, vec);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, result);
;
    *dstX = result[0];
    *dstY = result[1];
}

void fldSelectDisplayBuffer(u32 displayRow) {
    fldDisplayRow = displayRow;
}


void fldSubmitGsCommandWord(s32 lower, s32 bits, u64 upper) {
    SdfListHead *command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *entry;
    SdfPoolNode *descriptor;

    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(0x30);
    entry = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfAppendPacket(command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitFrameQuad(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17) {
    SdfListHead *command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    SdfPoolNode *descriptor;

    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(0x30);
    data = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    data[4] = (bit17 << 17) | 0x10000 | (bit15 << 15) | (bit14 << 14) | (bit12 << 12) | (bit4 << 4) | (bit1 << 1) | bit0;
    data[5] = 0x47;
    sdfAppendPacket(command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

/* Write the selected blend equation to GS ALPHA_1. */
void fldSetGsAlphaBlendEquation(s32 mode) {
    SdfListHead *command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    SdfPoolNode *descriptor;

    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(0x30);
    data = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    switch (mode) {
    case 1:
        data[4] = 0x48;
        break;
    case 2:
        data[4] = 0x42;
        break;
    case 3:
        data[4] = 0x84;
        break;
    case 4:
        data[4] = 0x06;
        break;
    case 5:
        data[4] = 0x89;
        break;
    case 6:
        data[4] = 0x42;
        break;
    case 10:
        data[4] = 0x2A;
        break;
    case 20:
        data[4] = 0x4A;
        break;
    default:
        data[4] = 0x44;
        break;
    }
    data[5] = 0x42;
    sdfAppendPacket(command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitGsLinesScaled(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    s32 coords[4];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    SdfPoolNode *descriptor;
    s32 i;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    sdfConsInitPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitGsLines(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    u32 coords[4];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    SdfPoolNode *descriptor;
    u32 *pos;
    u64 lo;
    u64 hi;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y1;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    sdfConsInitPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitGsQuadTagged(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    SdfPoolNode *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitPackedRectangleGsPacket(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
    s32 *pos;
    SdfPoolNode *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    tag = vertexTag;
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitGsRect(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    u32 coords[8];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u32 *pos;
    SdfPoolNode *descriptor;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y0;
    coords[4] = x1;
    coords[5] = y1;
    coords[6] = x0;
    coords[7] = y1;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFFFFULL;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitGsGradientTriangle(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2) {
    s32 coords[6];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;
    SdfPoolNode *descriptor;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    coords[4] = x2 * 16;
    coords[5] = y2 * 16;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 3));
    sdfConsInitPacketHeader(packet, 0x4D, 2, 0x41, 3);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(coords[i * 2] + 0x7000) |
            ((u64)(coords[i * 2 + 1] + 0x7900) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitGsGradientQuad(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3) {
    s32 coords[8];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    SdfPoolNode *descriptor;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = x * 16;
    coords[5] = (y + h) * 16;
    coords[6] = (x + w) * 16;
    coords[7] = (y + h) * 16;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader(packet, 0x4C, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitGsGradientQuadFloatRect(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
    s32 coords[8];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    SdfPoolNode *descriptor;

    coords[0] = (s32)(x * 16.0f);
    coords[1] = (s32)(y * 8.0f);
    coords[2] = (s32)((x + w) * 16.0f);
    coords[3] = (s32)(y * 8.0f);
    coords[4] = (s32)((x + w) * 16.0f);
    coords[5] = (s32)((y + h) * 8.0f);
    coords[6] = (s32)(x * 16.0f);
    coords[7] = (s32)((y + h) * 8.0f);
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitTaggedGsRectangle(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag) {
    s32 coords[8];
    SdfListHead *command;
    SdfDrawPacket *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
    s32 *pos;
    SdfPoolNode *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    tag = vertexTag;
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

extern void sdfConsInitDmaPacketHeader(DmaPacketHeader *, u32, s32);
extern void sdfVuLoadRotationMatrixY(f32);
extern void sdfInitGeometryDmaPacket(u8 *, const f32 *);
extern void sdfBuildTexturedQuadVifPacket(u8 *, const f32 *, u32, const BillTextureQuad *, const f32 *);

void fldSubmitModelPacket(SdfTex *texture, FldMarkerPacket *modelData) {
    SdfListHead *command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    DmaPacketHeader *header;
    u8 *packet;
    f32 mat[16];
    SdfPoolNode *descriptor;

    sdfInitPacketList(command);
    header = (DmaPacketHeader *)sdfAllocPacketAligned(0x20);
    sdfConsInitDmaPacketHeader(header, (u32)sdfTexGetPrimaryBuffer(texture), sdfTexGetPrimaryBufferSize(texture));
    sdfAppendReferencePacket(command, (u32)header);
    sdfVuLoadRotationMatrixY(modelData->scale);
        VU0_STORE_MATRIX(mat);
;
    packet = (u8 *)sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket(packet, mat);
    sdfAppendPacket(command, (u32)packet);
    packet = (u8 *)sdfAllocPacketAligned(0x80);
    sdfBuildTexturedQuadVifPacket(packet, modelData->pos, modelData->color, &modelData->uv, modelData->quad);
    sdfAppendPacket(command, (u32)packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

extern s32 kwlnGetDrawBufferIndex(void);
extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];
extern void sdfBuildMaskedFrameRegisterReference(s32, SdfDmaReferenceChainPacket *, s32);
extern void sdfBuildFrameRegisterReference(s32, SdfDmaReferenceChainPacket *, s32);

void fldSubmitPrimaryFramePacket(void) {
    SdfListHead *command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfDmaReferenceChainPacket *texture;
    SdfPoolNode *descriptor;
    sdfInitPacketList(command);
    texture = (SdfDmaReferenceChainPacket *)sdfAllocPacketAligned(0x40);
    sdfBuildMaskedFrameRegisterReference((s32)(kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].baseBytes), texture, 0);
    sdfAppendDmaTagToList(command, (u32)texture);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldSubmitAlternateFramePacket(void) {
    SdfListHead *command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfDmaReferenceChainPacket *texture;
    SdfPoolNode *descriptor;
    sdfInitPacketList(command);
    texture = (SdfDmaReferenceChainPacket *)sdfAllocPacketAligned(0x40);
    sdfBuildFrameRegisterReference((s32)(kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].baseBytes), texture, 0);
    sdfAppendDmaTagToList(command, (u32)texture);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

extern f32 D_0032F4E0[][4];
extern u32 D_0032F500[];
extern void *sdfDrawCachedMatrixLinePairsVU(const f32 (*)[4], const u32 *, s32, u32);

void fldSubmitVectorColorPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    SdfListHead *list;
    s32 record;
    SdfPoolNode *descriptor;
    D_0032F4E0[0][0] = x;
    D_0032F4E0[0][1] = y;
    D_0032F4E0[0][2] = z;
    D_0032F4E0[1][0] = u;
    D_0032F4E0[1][1] = v;
    D_0032F4E0[1][2] = w;
    D_0032F500[1] = second;
    D_0032F500[0] = first;
    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    record = (s32)sdfDrawCachedMatrixLinePairsVU(D_0032F4E0, D_0032F500, 2, 0x80);
    sdfAppendPacket(list, record);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, list);
}

extern void *sdfBuildPrimitiveAttributeVifPacket(SdfPrimitiveRequest *);
void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    SdfPrimitiveRequest desc;
    f32 verts[12];
    s32 indices[3];
    SdfListHead *command;
    SdfPoolNode *descriptor;

    command = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    sdfConsAppendClearPacket(command, 0);
    sdfConsAppendAssetPacket(command, (SdfAsset *)D_003BACEC, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.stripWordCount = 1;
    desc.vertexCount = 3;
    desc.positions = verts;
    desc.vertexColors = indices;
    verts[0] = f0;
    verts[1] = f1;
    verts[2] = f2;
    verts[4] = f3;
    verts[5] = f4;
    verts[6] = f5;
    verts[8] = f6;
    verts[9] = f7;
    verts[10] = f8;
    indices[0] = a0;
    indices[1] = a1;
    indices[2] = a2;
    sdfAppendPacket(command, (u32)sdfBuildPrimitiveAttributeVifPacket(&desc));
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}


/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitTexturedOverlaySprite(void) {
    SdfListHead *list = (SdfListHead *)sdfAllocatePacketList(NULL);
    SdfDmaNode *reference = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    sdfAppendDmaPrimary(list,
        (u32)(kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].baseBytes), reference);
    texturePacket = (u64 *)sdfAllocPacketAligned(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfAppendPacket(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x44;
    blendPacket[7] = 0x42;
    sdfAppendPacket(list, (u32)blendPacket);
    sprite = sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = 0x30;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF7;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0x3FFF;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = 0x9009;
    vertex->corner[1].y = 0x8705;
    vertex->corner[1].mask = 0x3FFF;
    vertex->corner[1].flag = 0;
    sdfAppendPacket(list, (u32)sprite);
    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append(surface, list);
}


/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitOverlaySpriteWithRenderState(s32 mode) {
    SdfListHead *list = (SdfListHead *)sdfAllocatePacketList(NULL);
    SdfDmaNode *reference = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    sdfAppendDmaPrimary(list,
        (u32)(kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].baseBytes), reference);
    texturePacket = (u64 *)sdfAllocPacketAligned(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfAppendPacket(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x8000000064ULL;
    blendPacket[7] = 0x42;
    sdfAppendPacket(list, (u32)blendPacket);
    sprite = sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)sprite);
    if (mode == 0) {
        vertex->r = 0x81;
        vertex->g = 0x81;
        vertex->b = 0x81;
        vertex->a = 0x80;
    } else {
        vertex->r = 0x81;
        vertex->g = 0x81;
        vertex->b = 0x84;
        vertex->a = 0x80;
    }
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfAppendPacket(list, (u32)sprite);
    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append(surface, list);
}


/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitOverlayStateAndSprite(s32 alpha) {
    SdfListHead *list = (SdfListHead *)sdfAllocatePacketList(NULL);
    SdfDmaNode *reference = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    sdfAppendDmaPrimary(list,
        (u32)(kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].baseBytes), reference);
    texturePacket = (u64 *)sdfAllocPacketAligned(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfAppendPacket(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = ((u64)(u32)alpha << 32) | 0x64;
    blendPacket[7] = 0x42;
    sdfAppendPacket(list, (u32)blendPacket);
    sprite = sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfAppendPacket(list, (u32)sprite);
    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append(surface, list);
}


void fldDrawMarkerQuad(f32 *pos) {
    FldMarkerPacket packet;
    f32 half = 36.0f;

    packet.uv.components[0] = 0;
    packet.uv.components[1] = 0;
    packet.uv.components[2] = 0x200;
    packet.uv.components[3] = 0;
    packet.uv.components[4] = 0x200;
    packet.uv.components[5] = 0x200;
    packet.uv.components[6] = 0;
    packet.uv.components[7] = 0x200;
    packet.quad[0] = -half;
    packet.quad[1] = -half;
    packet.quad[2] = half;
    packet.quad[3] = -half;
    packet.quad[4] = half;
    packet.quad[5] = half;
    packet.quad[6] = -half;
    packet.quad[7] = half;
    packet.color = 0x8080FF80;
    packet.pos[0] = pos[0];
    packet.pos[1] = pos[1];
    packet.pos[2] = pos[2];
    packet.scale = 0.0f;
    fldSelectDisplayBuffer(0x39);
    fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
    fldSubmitModelPacket(fldMarkerTexture, &packet);
}

void fldDrawMarkerQuadColored(f32 *pos, s32 color) {
    FldMarkerPacket packet;
    f32 half = 36.0f;

    packet.uv.components[0] = 0;
    packet.uv.components[1] = 0;
    packet.uv.components[2] = 0x200;
    packet.uv.components[3] = 0;
    packet.uv.components[4] = 0x200;
    packet.uv.components[5] = 0x200;
    packet.uv.components[6] = 0;
    packet.uv.components[7] = 0x200;
    packet.quad[0] = -half;
    packet.quad[1] = -half;
    packet.quad[2] = half;
    packet.quad[3] = -half;
    packet.quad[4] = half;
    packet.quad[5] = half;
    packet.quad[6] = -half;
    packet.quad[7] = half;
    packet.color = color;
    packet.pos[0] = pos[0];
    packet.pos[1] = pos[1];
    packet.pos[2] = pos[2];
    packet.scale = 0.0f;
    fldSelectDisplayBuffer(0x39);
    fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
    fldSubmitModelPacket(fldMarkerTexture, &packet);
}

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

void fldDrawStretchableFrame(s32 x, s32 y, s32 width, s32 height) {
    fldSubmitSpriteRect(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitSpriteRect(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitSpriteRect(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
}

void fldAllocateBackgroundBuffer(void) {
    if (fldBackgroundBuffer == 0) {
        fldBackgroundBuffer = (s32)(u32)sdfAllocateBlockBySizeThreshold(0x70000);
    }
}

void fldReleaseBackgroundBuffer(void) {
    if (fldBackgroundBuffer != 0) {
        sdfReleaseChipOrRetainedResource((void *)(u32)fldBackgroundBuffer);
        fldBackgroundBuffer = 0;
    }
}

extern u32 D_003980F0[];

void fldSubmitBackgroundResourcePacket(void) {
    if (fldBackgroundBuffer != 0) {
        SdfListHead *packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateResourcePacket(packet, (SdfTexResource *)D_003980F0[0],
                                0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0, 0, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append(descriptor, packet);
    }
}

void fldSubmitBackgroundDescriptorPacket(void) {
    if (fldBackgroundBuffer != 0) {
        SdfListHead *packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateDescriptorPacket(packet, (SdfTexResource *)D_003980F0[0],
                                  0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append(descriptor, packet);
    }
}

void fldDrawConvertedTextGlyph(x, y, first, second)
s32 x;
s32 y;
u32 first;
const u8 *second;
{
    struct FrFontGlyph *object;

    object = itfCreateConvertedTextGlyph(x << 4, y << 4, 0, first, second, 0);
    frFontDrawGlyphInDefaultMode(object);
    frFontQueueGlyphForCurrentDrawBuffer(object);
}

void fldAdvanceQuadRow(FldQuadState *quad) {
    quad->rowY = quad->rowY + 0x60;
}

void fldStartQuadPacketList(FldQuadState *quad) {
    s32 packet;
    SdfListHead *packetList;

    packetList = sdfCreateResetPacketList();
    quad->packetList = packetList;
    packet = sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket((SdfGsBlendPacket *)packet);
    sdfAppendPacket(quad->packetList, packet);
}

extern SdfPoolNode D_00325708;
extern SdfPoolNode kwlnPositionedTextSurface;
extern void sdfInvertScaledVuTransform(void);

void fldDrawProjectedValue(f32 x, f32 y, f32 z, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;
    f32 screenX;
    f32 screenY;

    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfInvertScaledVuTransform();
    fldProjectPointSetup(&screenX, &screenY, x, y, z);
    if (screenX < -4000.0f || screenX > 4000.0f ||
        screenY < -4000.0f || screenY > 4000.0f) {
        return;
    }

    quad.rowX = 0x73C0;
    quad.rowY = 0x8440;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x73C0;
    quad.unk4 = 0x8440;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, (s32)screenX * 16, (s32)screenY * 16, quad.drawDepth, 1);
    sdfAppendPacket(quad.packetList,
                    (u32)sdfFormatSifPacket(&packet, D_003BACE0, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00325708.append(&D_00325708, quad.packetList);
}

void fldDrawFloorQuad(s32 x, s32 y, const char *format) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x73C0;
    quad.rowY = 0x7CC0;
    quad.drawDepth = 0x0FFFFF7E;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x73C0;
    quad.unk4 = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7D;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, 0);
    sdfAppendPacket(quad.packetList, (u32)sdfFormatSifPacket(&packet, format));
    fldAdvanceQuadRow(&quad);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, quad.packetList);
}

void fldDrawFloorQuadA(s32 x, s32 y, s32 packetField, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x73C0;
    quad.rowY = 0x7CC0;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x73C0;
    quad.unk4 = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, packetField);
    sdfAppendPacket(quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00325708.append(&D_00325708, quad.packetList);
}

void fldDrawMapQuadTiled(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket(quad.packetList, (u32)sdfFormatSifPacket(&packet, D_003BACE0, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00325708.append(&D_00325708, quad.packetList);
}

void fldDrawMapQuadTiledAlt(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket(quad.packetList, (u32)sdfFormatSifPacket(&packet, D_003BACE8, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00325708.append(&D_00325708, quad.packetList);
}

void fldDrawMapQuad(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket(quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00325708.append(&D_00325708, quad.packetList);
}

void fldDrawMapQuadPacket(s32 x, s32 y, s32 packetField, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, packetField);
    sdfAppendPacket(quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00325708.append(&D_00325708, quad.packetList);
}

void fldDrawMapQuadScaled(s32 packetField, s32 drawValue, f32 x, f32 y) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + (s32)(x * 16.0f), quad.rowY + (s32)(y * 8.0f), quad.drawDepth, packetField);
    sdfAppendPacket(quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00325708.append(&D_00325708, quad.packetList);
}

void fldDrawFilledDisc(u32 fade, f32 x, f32 y, f32 z, f32 radius) {
    s32 color;
    s32 angle;
    s32 prev;
    s32 i;
    f32 a;
    f32 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;

    if (fade != 0) {
        color = fade & 0xFF;
        fldSelectDisplayBuffer(0x39);
        fldSetGsAlphaBlendEquation(5);
        if (color > 0x80) {
            color = 0x80;
        }
        color = 0x80 - color;
        if (color >= 0x80) {
            color = 0x7F;
        }
        if (color < 0x10) {
            color = 0x10;
        }
        color = (color << 24) | 0x10101;
        for (i = 0; i < 360; i += 10) {
            angle = (i + 360) % 360;
            prev = (i + 350) % 360;
            a = (f32)angle * 3.14f / 180.0f;
            sinA = sdfSinPoly(a) * radius;
            cosA = sdfEvaluateCosineViaSinePhaseShift(a) * radius;
            b = (f32)prev * 3.14f / 180.0f;
            sinB = sdfSinPoly(b) * radius;
            cosB = sdfEvaluateCosineViaSinePhaseShift(b) * radius;
            fldSubmitGsTriangle(color, 0x80101010, 0x80101010,
                                x + 0.0f, y - 2.0f, z + 0.0f,
                                x + sinA, y - 2.0f, z + cosA,
                                x + sinB, y - 2.0f, z + cosB);
        }
        fldSetGsAlphaBlendEquation(0);
    }
}

void fldSubmitAreaOverlayQuads(s32 a1, s32 a2) {
    SdfListHead *list = (SdfListHead *)sdfAllocatePacketList(NULL);
    SdfDmaNode *reference = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *handle;
    FldSpriteVertex *vertex;
    s32 quarter;
    SdfPoolNode *descriptor;

    sdfAppendDmaPrimary(list,
        (u32)(kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].baseBytes), reference);
    texturePacket = (u64 *)sdfAllocPacketAligned(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfAppendPacket(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x48;
    blendPacket[7] = 0x42;
    sdfAppendPacket(list, (u32)blendPacket);
    handle = sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)handle);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = a1;
    quarter = a2;
    if (a2 < 0) {
        quarter = a2 + 3;
    }
    quarter >>= 2;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF7 - a2;
    vertex->corner[0].y = 0x78FB - quarter;
    vertex->corner[0].mask = 0x3FFF;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = a2 + quarter + 0x9009;
    vertex->corner[1].y = quarter + 0x8705;
    vertex->corner[1].mask = 0x3FFF;
    vertex->corner[1].flag = 0;
    sdfAppendPacket(list, (u32)handle);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, list);
}

s32 fldGetEncounterRuntimeResult(void) {
    s32 state = fldEncounterRuntimeState;
    s32 result;

    if (state < 3) {
        if (state < 0) {
            result = D_003BACFC;
        } else {
            result = btlGetRuntimeResult();
        }
    } else {
        result = D_003BACFC;
    }
    return result;
}

void fldSetEncounterPendingValue(u32 value) {
    D_003BACF8 = value;
}

s32 fldEncProc(KwlnTask *task) {
    s32 state = fldEncounterRuntimeState;

    if (state < 3) {
        if (state >= 0) {
            btlUpdateRuntimeFadeState();
        }
    }
    return 0;
}

void fldRequestEncounterWithFade(u32 mode, s32 recordIndex) {
    if ((recordIndex < 0x400) && ((datBattleSceneRecords[recordIndex].flags & 0x8000) != 0))
    {
        kwlnFadeBackgroundStartOut(0);
        fldSetEncounterMode(3);
        return;
    }
    sndSetSequenceVolumePan(0xf, 0x7f, 0x3f);
    fldSetEncounterMode(mode);
}

s32 fldSetEncounterMode(s32 mode) {
    fldEncounterRuntimeState = mode;
    D_003BACFC = 0;
    if (fldGetEncounterRuntimeResult() == 0) {
        if (fldEncounterRuntimeState < 3) {
            if (fldEncounterRuntimeState >= 0) {
                btlActivateRuntime(fldEncounterRuntimeState);
                if (dds3GetWorldObject() != 0) {
                    dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 1);
                }
            }
        }
    }
}


void fldResetEncounterAsyncState(KwlnTask *task) {
    btlResetAsyncState();
}

void fldCreateEncounterTask(void) {
    kwlnTaskCreate(fldEncounterTaskName, 0x2B0F, 0, 1, fldEncProc, fldResetEncounterAsyncState, 0);
    btlClearRuntimeState();
}

extern f32 fldLookAtNearPoint[];
extern f32 fldLookAtFarPoint[];
extern f32 D_0032E41C[];
void fldUpdateLookAtSegmentDistance(void) {
    f32 *distance = D_0032E41C;
    f32 dx = fldLookAtNearPoint[0] - fldLookAtFarPoint[0];
    f32 dy = fldLookAtNearPoint[1] - fldLookAtFarPoint[1];
    f32 dz = fldLookAtNearPoint[2] - fldLookAtFarPoint[2];
    *distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

extern f32 fldGetTriangleHeightAtXZ(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

/* Probe the support surface under the camera's focus and its mirrored heading. */
void fldProbeCameraSupportSurface(void) {
    FldAreaWork *cam;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    f32 offset[3];
    f32 x;
    s32 index;
    EffWorldNode *node;

    fldAreaState.unk5C = 0.0f;
    fldAreaState.unk170 = 0.0f;
    if (fldAreaState.overrideSupportRecordIndex != -1) {
        index = fldAreaState.overrideSupportRecordIndex;
    } else if (fldAreaState.unkA4 >= 0) {
        index = fldAreaState.unkA4;
    } else {
        return;
    }
    if (fldValueRecords == 0) {
        return;
    }
    node = (EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value;
    if (node != NULL) {
        offset[0] = node->inner->position[0];
        offset[1] = node->inner->position[1];
        offset[2] = node->inner->position[2];
    } else {
        offset[0] = 0.0f;
        offset[1] = 0.0f;
        offset[2] = 0.0f;
    }
    cam = &fldAreaState;
    xs[0] = ((FldValueRecord *)fldValueRecords)[index].vertices[0][0] + offset[0];
    ys[0] = ((FldValueRecord *)fldValueRecords)[index].vertices[0][1] + offset[1];
    zs[0] = ((FldValueRecord *)fldValueRecords)[index].vertices[0][2] + offset[2];
    xs[1] = ((FldValueRecord *)fldValueRecords)[index].vertices[1][0] + offset[0];
    ys[1] = ((FldValueRecord *)fldValueRecords)[index].vertices[1][1] + offset[1];
    zs[1] = ((FldValueRecord *)fldValueRecords)[index].vertices[1][2] + offset[2];
    xs[2] = ((FldValueRecord *)fldValueRecords)[index].vertices[2][0] + offset[0];
    ys[2] = ((FldValueRecord *)fldValueRecords)[index].vertices[2][1] + offset[1];
    zs[2] = ((FldValueRecord *)fldValueRecords)[index].vertices[2][2] + offset[2];
    x = cam->x - sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist;
    cam->unk5C = cam->y - fldGetTriangleHeightAtXZ(x,
        cam->z - sdfEvaluateCosineViaSinePhaseShift(cam->negatedAngle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist, xs, ys, zs);
    x = cam->x - sdfSinPoly(-cam->angle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist;
    cam->unk170 = cam->y - fldGetTriangleHeightAtXZ(x,
        cam->z - sdfEvaluateCosineViaSinePhaseShift(-cam->angle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist, xs, ys, zs);
    if (cam->overrideSupportRecordIndex == -1) {
        cam->unk5C = 0.0f;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldUpdateFieldCameraViewState);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012CED0);

void fldUpdateCameraProjectionEndpoints(void) {
    FldAreaWork *cam = &fldAreaState;
    f32 angle;

    D_00330650[0] = cam->x - sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * 80.0f;
    D_00330650[1] = cam->y + fldCameraFollowRows[cam->rowIdx].y;
    D_00330650[2] = cam->z - sdfEvaluateCosineViaSinePhaseShift(cam->negatedAngle * 3.14f / 180.0f) * 80.0f;
    D_00330650[3] = 1.0f;
    angle = cam->negatedAngle * 3.14f / 180.0f;
    D_00330660[0] = cam->x + sdfSinPoly(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_00330660[1] = cam->y + fldCameraFollowRows[cam->rowIdx].targetY;
    D_00330660[2] = cam->z + sdfEvaluateCosineViaSinePhaseShift(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_00330660[3] = 1.0f;
}

void fldInterpolateCameraEndpoints(void) {
    union {
        u128 q;
        f32 f[4];
    } nearPoint, farPoint;
    s32 *world = fldGetPlayerSceneStateAddress();
    f32 ratio = (f32)D_003BAD24 / (f32)D_003BAD28;
    f32 remaining = 1.0f - ratio;
    EffWorldNode *node;

    nearPoint.f[0] = D_00330650[0] * ratio + D_00330630[0] * remaining;
    nearPoint.f[1] = D_00330650[1] * ratio + D_00330630[1] * remaining;
    nearPoint.f[2] = D_00330650[2] * ratio + D_00330630[2] * remaining;
    farPoint.f[0] = D_00330660[0] * ratio + D_00330660[0] * remaining;
    farPoint.f[1] = D_00330660[1] * ratio + D_00330660[1] * remaining;
    farPoint.f[2] = D_00330660[2] * ratio + D_00330660[2] * remaining;
    PCP_COPY_VECTOR(fldLookAtNearPoint, &nearPoint);
    PCP_COPY_VECTOR(fldLookAtFarPoint, &farPoint);
    effObjSetInnerPosition((EffWorldNode *)(u32)*world, &farPoint.q);
    node = (EffWorldNode *)(u32)*world;
    node->ops->update(node);
    D_003BAD24++;
    if (D_003BAD28 < D_003BAD24) {
        fldAreaState.mode = 0;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012D528);

extern void dds3SetCameraVector(EffWorldNode *camera, u128 *worldEye);
extern f32 fldGetNormalizedComplementaryAngle(f32 ax, f32 ay, f32 bx, f32 by);
extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);
extern void dds3LoadOrBuildObjectMatrix(EffWorldNode *object);

void fldUpdateCameraViewFromPlayer(void) {
    f32 nearPoint[4];
    f32 farPoint[4];
    s32 *world = fldGetPlayerSceneStateAddress();
    EffWorldNode *node;
    f32 angle;

    effObjFetchInnerRotationNormalized((EffWorldNode *)fldPlayerObject);
    angle = effMiscComputeQuaternionRotatedReferenceAngle() * 180.0f / 3.14f;
    nearPoint[0] = fldLookAtNearPoint[0] + (fldAreaState.x - fldLookAtNearPoint[0]);
    nearPoint[1] = fldLookAtNearPoint[1] + ((fldAreaState.y + fldCameraFollowRows[fldAreaState.rowIdx].y) - fldLookAtNearPoint[1]);
    nearPoint[2] = fldLookAtNearPoint[2] + (fldAreaState.z - fldLookAtNearPoint[2]);
    fldAreaState.angle = angle;
    fldAreaState.targetAngle = angle;
    dds3SetCameraVector((EffWorldNode *)(u32)*world, (u128 *)nearPoint);
    dds3LoadOrBuildObjectMatrix((EffWorldNode *)fldPlayerObject);
    farPoint[3] = 1.0f;
    farPoint[0] = fldAreaState.focusPos[0];
    farPoint[1] = fldAreaState.focusPos[1];
    farPoint[2] = fldAreaState.focusPos[2];
    effObjSetInnerPosition((EffWorldNode *)(u32)*world, (u128 *)farPoint);
    farPoint[0] = fldLookAtFarPoint[0] + (farPoint[0] - fldLookAtFarPoint[0]);
    farPoint[1] = fldLookAtFarPoint[1] + (farPoint[1] - fldLookAtFarPoint[1]);
    farPoint[2] = fldLookAtFarPoint[2] + (farPoint[2] - fldLookAtFarPoint[2]);
    effObjSetInnerPosition((EffWorldNode *)(u32)*world, (u128 *)farPoint);
    PCP_COPY_VECTOR(fldLookAtNearPoint, nearPoint);
    PCP_COPY_VECTOR(fldLookAtFarPoint, farPoint);
    node = (EffWorldNode *)(u32)*world;
    node->ops->update(node);
    fldAreaState.dist = fsqrtf((nearPoint[0] - farPoint[0]) * (nearPoint[0] - farPoint[0]) +
                               (nearPoint[1] - farPoint[1]) * (nearPoint[1] - farPoint[1]) +
                               (nearPoint[2] - farPoint[2]) * (nearPoint[2] - farPoint[2]));
    fldAreaState.negatedAngle = fldGetNormalizedComplementaryAngle(farPoint[0], farPoint[2], nearPoint[0], nearPoint[2]) + 180.0f;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012DD70);

/* Suppress the world object's current entry when it is already selected. */
s64 fldGetUnselectedWorldEntry(void) {
    s32 worldObject;
    s64 currentEntry;
    s64 selectedEntry;

    worldObject = (s32)dds3GetWorldObject();
    currentEntry = (s32)dds3GetWorldCameraObject((EffWorldNode *)worldObject);
    selectedEntry = fldGetPlayerSceneState();
    if (selectedEntry == currentEntry) {
        currentEntry = 0;
    }
    return currentEntry;
}

void fldSetCameraMoveMode(u32 value) {
    D_003BAD1C = value;
    dds3TransformCameraVectorsByInnerRotation(dds3GetWorldCameraObject(dds3GetWorldObject()), D_003C9230,
                                              D_003C9220);
    D_003BAD20 = 0;
}


extern void effObjSetNodeFlags(ObjectTransform *, u32);

void fldUpdateCameraMoveOscillation(void) {
    f32 direction = 0.0f;
    f32 phase = D_003BAD20;
    EffWorldNode *camera;

    if (D_003BAD1C != 0) {
        if (D_003BAD1C == 1) {
            direction = 1.0f;
        }
        if (D_003BAD1C == 2) {
            direction = 1.0f;
        }
        if (D_003BAD1C == -1) {
            direction = -1.0f;
        }
        if (D_003BAD1C == -2) {
            direction = -1.0f;
        }
        camera = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (D_003BAD1C == 1 || D_003BAD1C == -1) {
            if (phase < 3.14f) {
                phase += 0.2f;
                camera->inner->position[1] = D_003C9220[1] + sdfSinPoly(phase * direction) * 2.5f;
            } else {
                phase += 0.02f;
                camera->inner->position[1] = D_003C9220[1] + sdfSinPoly(phase * direction) * 2.0f;
            }
        }
        if (D_003BAD1C == 2 || D_003BAD1C == -2) {
            phase += 11.0f;
            if (phase > 0.0f) {
                phase -= 1.0f;
            } else if (phase > -3.14f) {
                phase -= 0.2f;
                camera->inner->position[1] = D_003C9220[1] + sdfSinPoly(phase * direction) * 2.0f;
            } else {
                phase -= 0.01f;
            }
            phase -= 11.0f;
        }
        D_003BAD20 = phase;
        effObjSetNodeFlags(camera->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
    }
}

void fldClearCameraMoveMode(void) {
    D_003BAD1C = 0;
}

typedef struct FldModelMatrices {
    u8 pad00[0xC];
    u8 **rows; /* 0x0C: slot-indexed matrix blocks */
} FldModelMatrices;

void fldUpdateCameraProximity(void) {
    f32 vec[4];
    s32 slot;
    u8 *model;
    u8 **modelRef;
    u8 **matrices;

    memset(vec, 0, sizeof(vec));
    vec[3] = 1.0f;
    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x40) {
        if (fldTestSceneControlFlags(0x40) == 0) {
            return;
        }
    }
    slot = dds3GetObjectOwnedHandle((EffWorldNode *)fldPlayerObject)->resourceSlots[4];
    modelRef = (u8 **)fldPlayerModelContext->inner;
    if (slot >= 0) {
        model = *modelRef;
        matrices = ((FldModelMatrices *)model)->rows;
            VU0_LOAD_MATRIX(matrices[slot] + 0xC0);
;
        VU0_LOAD_VF(vf10, vec);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, vec);
        if (fldPointDistance(vec[0], vec[1], vec[2], fldLookAtFarPoint[0], fldLookAtFarPoint[1], fldLookAtFarPoint[2]) < 45.0f) {
            fldTestSceneControlFlags(0x40);
            fldClearCameraModelColor();
            fldClearCameraObjectTransitionFlags();
            fldClearCameraObjectHighlightFlag();
        } else {
            fldSetCameraObjectTransitionFlag();
            if (fldAreaState.mode == 1 || fldAreaState.mode == 3) {
                fldClearCameraObjectHighlightFlag();
            } else if (fldAreaState.dist < 100.0f) {
                fldSetCameraObjectHighlightFlag();
            } else {
                fldClearCameraObjectHighlightFlag();
            }
        }
    }
}

s32 fldUpdateCameraFollow(void) {
    FldAreaWork *cam;
    s32 *world = fldGetPlayerSceneStateAddress();
    if (*world != 0 && fldPlayerObject != 0) {
        fldRestoreCameraModelColor();
        if (fldGetUnselectedWorldEntry() != 0) {
            fldToggleWorldNodeState(1);
            fldUpdateCameraMoveOscillation();
            fldClearCameraObjectHighlightFlag();
            return 0;
        }
        fldProbeCameraSupportSurface();
        cam = &fldAreaState;
        dds3SetCameraFieldOfView(*world, fldCameraFollowRows[cam->rowIdx].fov * 3.14f / 180.0f);
        switch (cam->mode) {
        case 0:
            func_0012D528();
            if (cam->dist < 50.0f) {
                func_0012D528();
            }
            break;
        case 1:
            func_0012DD70();
            break;
        case 4:
            fldInterpolateCameraEndpoints();
            break;
        }
        fldUpdateCameraProximity();
    }
    return 0;
}

void fldResetCameraModelHandles(void) {
    D_003BAD38 = 0;
    D_003BAD34 = 0xffffffff;
    D_003BAD3C = 0;
    D_003BAD40 = 0;
}

void fldReleaseCameraModel(u32 enabled) {
    if (enabled == 0) {
        D_003BAD40 = 0;
        if (fldPlayerModelContext != 0) {
            mdlResumeAllContextMotions(fldPlayerModelContext);
        }
    } else {
        D_003BAD40 = enabled;
        mdlSuspendAllContextMotions(fldPlayerModelContext);
    }
}

void fldSetCameraModelMotionRequest(u32 index, u32 value) {
    D_003BAD34 = index;
    D_003BAD38 = value;
}

void fldUpdateCameraModelMotion(s32 modelMotion, s32 motion, f32 blendFrames) {
    s32 currentMotion;

    if (D_0032E4C8[0] == 1) {
        if ((s32)D_003BAD40 > 0) {
            if (--D_003BAD40 != 0) {
                return;
            }
            mdlResumeAllContextMotions(fldPlayerModelContext);
        }
        currentMotion = D_003BAD3C;
        switch (motion) {
        case 1:
            break;
        case 2:
            if ((s32)D_003BAD38 > 0) {
                motion = 3;
            }
            break;
        case 3:
            if (currentMotion == 0x66) {
                motion = 2;
                currentMotion = 2;
            }
            break;
        default:
            if (currentMotion == 0x66) {
                motion = 2;
                currentMotion = 2;
            } else {
                motion = 3;
            }
            break;
        }
        if (D_003BAD34 == 1) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 1, motion, blendFrames, blendFrames);
            D_003BAD34 = -1;
            if (motion == 2 && D_003BAD3C == 3) {
                D_003BAD3C = motion;
                fldPlayerModelContext->first->frameStep = 1.0f;
                mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
            }
        } else if (D_003BAD34 == 0x66) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 2, 0.0f, blendFrames);
            D_003BAD34 = -1;
            D_003BAD3C = 0x66;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
        } else if (D_003BAD34 == 2) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 2, 0.0f, blendFrames);
            D_003BAD34 = -1;
            D_003BAD3C = 2;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
        } else if (D_003BAD34 == 3) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 3, 0.0f, blendFrames);
            D_003BAD34 = -1;
            D_003BAD3C = 3;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 3, blendFrames, blendFrames);
        } else if (D_003BAD34 == 5) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 5, 0.0f, blendFrames);
            D_003BAD34 = -1;
            D_003BAD3C = 5;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 5, blendFrames, blendFrames);
        } else if (D_003BAD34 == 6) {
            if (motion == 1) {
                mdlSetNodeFrameStep(fldPlayerModelContext, 1, 2.0f);
                mdlAddEntryPlain(fldPlayerModelContext, 1, 6);
                D_003BAD34 = -1;
            } else {
                mdlSetNodeFrameStep(fldPlayerModelContext, 1, 2.0f);
                mdlAddEntryPlain(fldPlayerModelContext, 1, 4);
                D_003BAD34 = -1;
                D_003BAD3C = 4;
                fldPlayerModelContext->first->frameStep = 1.0f;
                mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 4, blendFrames, blendFrames);
            }
        } else if (currentMotion != motion && D_003BAD3C != 5) {
            D_003BAD3C = motion;
            fldPlayerModelContext->first->frameStep = 1.5f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, motion, blendFrames, blendFrames);
            if (D_003BAD38 == 0) {
                mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
                mdlAddEntryFlaggedEx(fldPlayerModelContext, 1, motion, blendFrames, blendFrames);
            }
        }
        if ((s32)D_003BAD38 > 0) {
            D_003BAD38--;
        }
    } else if (modelMotion != motion) {
        fldPlayerModelContext->first->frameStep = 1.0f;
        mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, motion, blendFrames, blendFrames);
    }
}

struct EffRandState;
extern u32 effMiscRand(struct EffRandState *state);
extern void fldSetSequenceVolume(s32 category, s32 volume);
extern void fldQueuePrimaryEffectPosition(f32 x, f32 y, f32 z);
extern void fldQueueSecondaryEffectPosition(f32 x, f32 y, f32 z);
extern s32 sdfLoadMapRecordPositionVector(SdfModel *model, s32 id);
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern s32 D_003BAD44;

/* Dispatch camera-model frame crossings to sequence sounds and queued effects. */
void fldDispatchCameraModelFrameEvents(s32 motion, s32 selector) {
    union {
        u128 q;
        f32 f[4];
    } position;
    MdlCtx *cameraModel;
    f32 previousFrame;
    f32 currentFrame;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distance;
    s32 sequenceCategory;
    s32 volume;

    memset(&position, 0, sizeof(position));
    position.f[3] = 1.0f;

    cameraModel = fldPlayerModelContext;
    previousFrame = fldAreaState.unk16C;
    currentFrame = cameraModel->first->currentFrame;
    fldAreaState.unk16C = currentFrame;

    if (fldAreaState.unk8C == 0) {
        switch (fldAreaState.unk88) {
        case 1: sequenceCategory = 0x14; break;
        case 2: sequenceCategory = 0x18; break;
        case 3: sequenceCategory = 0x1C; break;
        default: sequenceCategory = 0x10; break;
        }
    } else {
        switch (fldAreaState.unk8C - 1) {
        case 1: sequenceCategory = 0x14; break;
        case 2: sequenceCategory = 0x18; break;
        case 3: sequenceCategory = 0x1C; break;
        default: sequenceCategory = 0x10; break;
        }
    }

    if (fldAreaState.mode == 1) {
        dx = fldLookAtFarPoint[0] - fldLookAtNearPoint[0];
        dy = fldLookAtFarPoint[1] - fldLookAtNearPoint[1];
        dz = fldLookAtFarPoint[2] - fldLookAtNearPoint[2];
        distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 500.0f;
        if (distance > 7000.0f) distance = 7000.0f;
        if (distance < 0.0f) distance = 0.0f;
        volume = ((7000 - (s32)distance) * 127) / 7000;
        if (volume >= 128) volume = 127;
        if (volume < 0) volume = 0;
    } else {
        volume = 127;
        volume -= (effMiscRand(NULL) >> 2) & 0x3F;
        if (volume >= 128) volume = 127;
        if (volume < 0) volume = 0;
    }

    if (fldAreaState.unk118 == 1) {
        sequenceCategory = (D_003BAD44 % 2) + 0x19;
        if (selector != 1) return;
        if ((previousFrame < 7.0f && currentFrame >= 7.0f) ||
            (previousFrame < 17.0f && currentFrame >= 17.0f)) {
            sndSetSequenceVolumePan(0x670000 + sequenceCategory, 0x7F, 0x3F);
            D_003BAD44 += (effMiscRand(NULL) >> 4 & 1) + 1;
        }
        return;
    }

    if (motion != 0 && motion != 2 && motion != 3 && selector == 2) {
        fldSetSequenceVolume(sequenceCategory + 3, volume);
        fldQueueSecondaryEffectPosition(fldAreaState.x,
                                        fldAreaState.y - 5.0f,
                                        fldAreaState.z);
        return;
    }
    sequenceCategory += D_003BAD44 % 3;

    if (selector == 1) {
        if (previousFrame < 8.0f && currentFrame >= 8.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_003BAD44 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xCA) == 0) return;
            VU0_STORE_VF(vf10, &position);
            fldQueueSecondaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 18.0f && currentFrame >= 18.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_003BAD44 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xC9) == 0) return;
            VU0_STORE_VF(vf10, &position);
            fldQueuePrimaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
    } else if (selector == 0x17) {
        if (previousFrame < 88.0f && currentFrame >= 88.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_003BAD44 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xCA) == 0) return;
            VU0_STORE_VF(vf10, &position);
            fldQueueSecondaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 18.0f && currentFrame >= 18.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_003BAD44 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xC9) == 0) return;
            VU0_STORE_VF(vf10, &position);
            fldQueuePrimaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 42.0f && currentFrame >= 42.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_003BAD44 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xCA) == 0) return;
            VU0_STORE_VF(vf10, &position);
            fldQueueSecondaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 63.0f && currentFrame >= 63.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_003BAD44 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xC9) == 0) return;
            VU0_STORE_VF(vf10, &position);
            fldQueuePrimaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
        }
    }
}

extern s32 D_003BAB50;
extern void fldApplyRelativeZoneMotion(f32 *);

/* Probe the current player position; consume a pending target into work/object state. */

void fldUpdateCameraTarget(void) {
    union {
        u128 q;
        f32 f[4];
    } vec;
    f32 cur[3];
    FldAreaWork *st;
    u128 *dst;

    if (fldPlayerObject != 0 && (st = &fldAreaState, st->positionMode != 1) && D_003BAB50 != 0) {
        cur[0] = st->x;
        cur[1] = st->y;
        cur[2] = st->z;
        fldApplyRelativeZoneMotion(cur);
        if (st->positionPending != 0) {
            st->x = st->targetX;
            st->y = st->targetY;
            st->z = st->targetZ;
            vec.f[0] = st->targetX;
            vec.f[1] = st->targetY;
            vec.f[2] = st->targetZ;
            effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, &vec.q);
            st->positionPending = 0;
            effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
            VU0_STORE_VF(vf10, &vec);
;
            dst = (u128 *)(*(u32 *)(fldPlayerObject + 0x1C) + 0x70);
            PCP_COPY_VECTOR(dst, &vec);
        }
    }
}

extern const PcpVectorCopyF32 D_003A0048;
extern f32 fldAngleDifference(f32 current, f32 target);
extern void effMiscAxisAngleToQuaternionVU(f32 angle);
extern void fldRefreshAreaStateFromPosition(f32 *);

/* Follow the selected mode-1 heading while preserving field position history.
 * The four legacy SDK stores are qualified only for this pinned emitted body;
 * each vector consumer and conditional join requires a fresh audit on changes. */
void fldUpdateModeOneCameraPose(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 smoothed[4] __attribute__((aligned(16)));
    f32 quaternion[4] __attribute__((aligned(16)));
    PcpVectorCopyF32 axis;
    f32 current[3];
    f32 difference;
    f32 distance;
    f32 step;
    s32 motion;

    axis = D_003A0048;
    if (fldAreaState.positionMode != 1) {
        return;
    }
    effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF_UNCLOBBERED(vf10, position);
    fldAreaState.previousX = fldAreaState.x;
    fldAreaState.previousY = fldAreaState.y;
    fldAreaState.previousZ = fldAreaState.z;
    fldAreaState.x = position[0];
    fldAreaState.y = position[1];
    fldAreaState.z = position[2];
    if (D_003BAB50 != 0) {
        effObjSetNodeFlags(((EffWorldNode *)fldPlayerObject)->inner,
                          OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
        if (D_003BAB50 != 0) {
            if (fldAreaState.unk170 > 0.0f) {
                fldAreaState.y = position[1] + 5.0f;
            } else {
                fldAreaState.y = position[1] + 10.0f;
            }
        }
    }
    while (fldAreaState.negatedAngle >= 360.0f) {
        fldAreaState.negatedAngle -= 360.0f;
    }
    while (fldAreaState.negatedAngle < 0.0f) {
        fldAreaState.negatedAngle += 360.0f;
    }
    while (fldAreaState.angle >= 360.0f) {
        fldAreaState.angle -= 360.0f;
    }
    while (fldAreaState.angle < 0.0f) {
        fldAreaState.angle += 360.0f;
    }
    while (fldAreaState.unk174 >= 360.0f) {
        fldAreaState.unk174 -= 360.0f;
    }
    while (fldAreaState.unk174 < 0.0f) {
        fldAreaState.unk174 += 360.0f;
    }
    effObjFetchInnerRotationNormalized((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF_UNCLOBBERED(vf10, quaternion);
    effMiscComputeQuaternionRotatedReferenceAngle();
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfInvertScaledVuTransform();
    motion = 23;
    difference = fldAngleDifference(fldAreaState.angle, fldAreaState.unk174);
    if (difference < 0.0f) {
        difference = -difference;
    }
    /* Native literal is 0x3D9F49F4; do not replace by rounded 14/180. */
    distance = (180.0f - difference) * 0x1.3e93e8p-4f;
    position[0] = fldAreaState.x - sdfSinPoly(-fldAreaState.unk174 * 3.14f / 180.0f) * distance;
    position[1] = fldAreaState.y;
    position[2] = fldAreaState.z - sdfEvaluateCosineViaSinePhaseShift(-fldAreaState.unk174 * 3.14f / 180.0f) * distance;
    effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
    if (D_003BAB50 != 0) {
        fldAreaState.x = position[0];
        fldAreaState.y = position[1];
        fldAreaState.z = position[2];
        current[0] = fldAreaState.x;
        current[1] = fldAreaState.y;
        current[2] = fldAreaState.z;
        fldRefreshAreaStateFromPosition(current);
        if (fldAreaState.positionPending != 0) {
            fldAreaState.x = fldAreaState.targetX;
            fldAreaState.y = fldAreaState.targetY + 44.0f;
            fldAreaState.z = fldAreaState.targetZ;
            position[0] = fldAreaState.x;
            position[1] = fldAreaState.y;
            position[2] = fldAreaState.z;
            effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
            fldAreaState.positionPending = 0;
        }
        effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        PCP_COPY_VECTOR_F32(smoothed, ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition);
        if (ffabsf(smoothed[0] - position[0]) < 1.0f) {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[0] = position[0];
        } else {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[0] =
                smoothed[0] + (position[0] - smoothed[0]) / 3.0f;
        }
        if (ffabsf(smoothed[1] - position[1]) < 1.0f) {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[1] = position[1];
        } else {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[1] =
                smoothed[1] + (position[1] - smoothed[1]) / 3.0f;
        }
        if (ffabsf(smoothed[2] - position[2]) < 1.0f) {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[2] = position[2];
        } else {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[2] =
                smoothed[2] + (position[2] - smoothed[2]) / 3.0f;
        }
        ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[3] = 1.0f;
    }
    difference = fldAngleDifference(fldAreaState.angle, fldAreaState.unk174);
    step = difference / 6.0f;
    if (step < 0.0f) {
        step = -step;
    }
    if (step < 2.0f) {
        step = 2.0f;
    }
    if ((0.0f <= difference && difference <= step) ||
        (difference <= 0.0f && -step <= difference)) {
        difference = fldAreaState.unk174;
    } else if (difference < 0.0f) {
        difference = fldAreaState.angle - step;
    } else {
        difference = fldAreaState.angle + step;
    }
    fldAreaState.angle = difference;
    VU0_LOAD_VF(vf10, axis.components);
    effMiscAxisAngleToQuaternionVU(fldAreaState.angle * 3.14f / 180.0f);
    VU0_STORE_VF_UNCLOBBERED(vf10, quaternion);
    effObjSetInnerRotation((EffWorldNode *)fldPlayerObject, (u128 *)quaternion);
    if (fldPlayerModelContext->current.h.motionIndex != motion) {
        fldPlayerModelContext->first->frameStep = 1.0f;
        mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, motion, 5.0f, 5.0f);
    }
    fldAreaState.x = position[0];
    fldAreaState.y = position[1];
    fldAreaState.z = position[2];
}


/* Signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */
/* Publish the vertical step to field state, then rebuild the object vector.
 * Each branch-local SDK snapshot dominates all of its scalar reads; there is
 * no earlier scratch read or loop-carried value. The finite qualification
 * and memory-contract limitation are documented in docs/idioms.md. */
s32 func_0012FC20(void) {
    f32 position[4] __attribute__((aligned(16)));
    if (fldAreaState.verticalStepDirection > 0) {
        effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        fldAreaState.previousX = fldAreaState.x;
        fldAreaState.previousY = fldAreaState.y;
        fldAreaState.previousZ = fldAreaState.z;
        fldAreaState.x = position[0];
        fldAreaState.y = position[1] - 2.0f;
        fldAreaState.z = position[2];
        position[0] = fldAreaState.x;
        position[1] = fldAreaState.y;
        position[2] = fldAreaState.z;
        effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
        fldClearCameraObjectTransitionFlags();
        fldClearCameraObjectHighlightFlag();
    } else if (fldAreaState.verticalStepDirection < 0) {
        effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        fldAreaState.previousX = fldAreaState.x;
        fldAreaState.previousY = fldAreaState.y;
        fldAreaState.previousZ = fldAreaState.z;
        fldAreaState.x = position[0];
        fldAreaState.y = position[1] + 2.0f;
        fldAreaState.z = position[2];
        position[0] = fldAreaState.x;
        position[1] = fldAreaState.y;
        position[2] = fldAreaState.z;
        effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
        fldClearCameraObjectTransitionFlags();
        fldClearCameraObjectHighlightFlag();
    } else {
        return 0;
    }
    return 0;
}


extern void mnuTitleStreamUpdateAndLogBgm(void);

/* Advance the field transition while the camera model's motion drives completion. */
s32 fldUpdateFieldTransition(void) {
    FldAreaWork *work = &fldAreaState;
    MdlCtx *model;
    s16 node;

    if (work->unk17C > 0) {
        model = fldPlayerModelContext;
        node = model->current.h.motionIndex;
        if (node != 7) {
            kwlnFadeStartIn(8);
            mnuQueueTitleSoundStreamEntry(0x26);
            model = fldPlayerModelContext;
            model->first->frameStep = 1.0f;
            mdlAddEntryPlain(model, 0, 7);
        }

        {
            Motion *const thresholdMotion = fldPlayerModelContext->first;
            f32 currentFrame = thresholdMotion->currentFrame;
            if (work->unk16C < 13.0f && 13.0f <= currentFrame) {
                kwlnPadStartMotor(0, 1, 10);
                kwlnPadStartMotor(1, 0xE6, 10);
                mnuTitleStreamUpdateAndLogBgm();
            }
        }
        {
            Motion *const completionMotion = fldPlayerModelContext->first;
            if (completionMotion->state == SDF_MOTION_STATE_TERMINAL) {
                mnuMarkTitleStreamResetPending();
                mnuResetTitleStreamLocked();
                fldAreaState.unk17C = 0;
                fldSetSceneControlFlags(0x40);
                return 0;
            }
        }

        fldDispatchCameraModelFrameEvents(node, 7);
        return -1;
    }

    return 0;
}

extern s32 ptyAnyUnitFlagMatch(u32 statusMask, s32 flagMode);
extern void kwlnPadStartMotor(u32 motor, u8 level, s32 duration);
extern void fldSetSceneControlFlags(u32 mask);

s32 fldUpdateAreaDamageReaction(void) {
    FldAreaWork *work = &fldAreaState;
    MdlCtx *model;

    if (work->unk184 > 0) {
        if (work->unk11C == 0 && ptyAnyUnitFlagMatch(0x80, 1) != 0) {
            kwlnFadeOutStart(0x80, 0x20, 0x20, 6);
            kwlnPadStartMotor(0, 1, 6);
            kwlnPadStartMotor(1, 0x96, 6);
            work->unk184 = 0;
            fldSetSceneControlFlags(0x40);
            return -1;
        }

        model = fldPlayerModelContext;
        if (model->current.h.motionIndex != 4) {
            model->first->frameStep = 1.0f;
            mdlAddEntryPlain(model, 0, 4);
            kwlnPadStartMotor(0, 1, 30);
            kwlnPadStartMotor(1, 0x96, 30);
            model = fldPlayerModelContext;
        }

        if (model->first->state == SDF_MOTION_STATE_TERMINAL) {
            fldAreaState.unk184 = 0;
            fldSetSceneControlFlags(0x40);
        } else {
            return -1;
        }
    }
    return 0;
}

extern s8 D_00324510[2][2][16];
extern u8 D_003BA870[2][2][2];
extern s32 D_003BAD2C;
extern s32 D_003BAD30;
extern s32 D_003BAD48;
extern s32 D_003BAD4C;
extern f32 D_003BAD50;
extern s32 D_003BAD54;
extern f32 D_0032F170[12];
extern s32 fileTestSavedSlotFlags(u32);
extern f32 fldSnapAngleToCompassPoint(f32);
extern f32 fldSnapAngleToCompassOctant(f32);
extern f32 fldSnapAngleToCardinalDirection(f32);
extern s32 fldIsNearSpark(f32, f32, f32);
extern void func_00136FE8(f32 *);
extern void fldRestoreCameraModelColor(void);
extern void fldClearCameraModelColor(void);
extern void fldDisableCameraObjectFlag(void);
extern void fldSetCameraObjectTransitionFlag(void);
extern void fldSetCameraObjectHighlightFlag(void);

/* Advance field movement, collision, heading, and the interactive camera pose.
 * The legacy SDK stores are qualified only for this pinned emitted body;
 * actual stack alignment and every subsequent consumer are audited. */
INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A0048);

s32 func_0012FF48(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 smoothed[4] __attribute__((aligned(16)));
    f32 quaternion[4] __attribute__((aligned(16)));
    PcpVectorCopyF32 axis = {{0.0f, 1.0f, 0.0f, 1.0f}};
    f32 directionAngles[16] = {
        0.0f, -22.5f, -45.0f, -67.5f, -90.0f, -112.5f, -135.0f, -157.5f,
        180.0f, 157.5f, 135.0f, 112.5f, 90.0f, 67.5f, 45.0f, 22.5f
    };
    f32 current[3];
    f32 heading;
    f32 difference;
    f32 step;
    f32 distance;
    f32 radians;
    s32 direction = -1;
    s32 snapInput = 0;
    s32 motion;
    s16 oldMotion;

    if (D_003BAD30 > 0) {
        D_003BAD30--;
    }
    if (fldUpdateFieldTransition() == -1) {
        return 0;
    }
    if (fldUpdateAreaDamageReaction() == -1) {
        return 0;
    }
    if (fldTestSceneControlFlags(0x40) == 0) {
        return 0;
    }
    if (D_003BAD40 != 0) {
        fldUpdateCameraModelMotion(0, 0, 6.0f);
        return 0;
    }

    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x100) {
        if (D_003BA870[0][0][1] < 0x40) {
            if (fileTestSavedSlotFlags(3) != 0) {
                fldAreaState.cameraTilt -= 10.0f;
            } else {
                fldAreaState.cameraTilt += 10.0f;
            }
            fldAreaState.cameraTiltActive = 1;
        }
        if (D_003BA870[0][0][1] > 0xC0) {
            if (fileTestSavedSlotFlags(3) != 0) {
                fldAreaState.cameraTilt += 10.0f;
            } else {
                fldAreaState.cameraTilt -= 10.0f;
            }
            fldAreaState.cameraTiltActive = 1;
        }
        if (fldAreaState.cameraTilt < -150.0f) {
            fldAreaState.cameraTilt = -150.0f;
        }
        if (fldAreaState.cameraTilt > 150.0f) {
            fldAreaState.cameraTilt = 150.0f;
        }
        if (D_00324510[1][0][3] < 0) {
            fldAreaState.cameraTiltActive = 0;
            fldAreaState.cameraTilt = 0.0f;
        }
    }

    if (D_003BA870[0][1][0] < 0x40 || D_003BA870[0][1][0] > 0xC0 ||
        D_003BA870[0][1][1] < 0x40 || D_003BA870[0][1][1] > 0xC0) {
        f32 x = D_003BA870[0][1][0] - 128;
        f32 y = D_003BA870[0][1][1] - 128;
        f32 angle;
        y = -y;
        if (-64.0f < x && x < 64.0f) {
            x = 0.0f;
        }
        if (-64.0f < y && y < 64.0f) {
            y = 0.0f;
        }
        angle = sdfAtan2(x, y);
        angle *= 180.0f / 3.14f;
        angle += 11.25f;
        if (angle < 0.0f) {
            angle += 360.0f;
        }
        if (angle > 360.0f) {
            angle -= 360.0f;
        }
        direction = (s32)(angle / 22.5f);
    }
    if (direction == -1) {
        if (D_00324510[1][0][4] != 0 && D_00324510[1][0][6] != 0) {
            direction = 14;
        } else if (D_00324510[1][0][4] != 0 && D_00324510[1][0][7] != 0) {
            direction = 10;
        } else if (D_00324510[1][0][5] != 0 && D_00324510[1][0][6] != 0) {
            direction = 2;
        } else if (D_00324510[1][0][5] != 0 && D_00324510[1][0][7] != 0) {
            direction = 6;
        } else if (D_00324510[1][0][4] != 0) {
            direction = 12;
        } else if (D_00324510[1][0][5] != 0) {
            direction = 4;
        } else if (D_00324510[1][0][6] != 0) {
            direction = 0;
        } else if (D_00324510[1][0][7] != 0) {
            direction = 8;
        }
    }
    if (D_00324510[1][0][3] < 0) {
        D_003BAD48 = 0;
        D_003BAD54 = 0;
    }
    if (D_003BAD48 != 0) {
        D_003BAD54++;
        if (D_003BAD4C != direction) {
            D_003BAD48 = 0;
        } else if (D_003BAD54 > 12) {
            D_003BAD48 = 0;
        }
    }
    if (D_00324510[1][0][3] != 0 || (D_00324510[1][0][8] != 0 && D_00324510[1][0][10] != 0)) {
        if (fldAreaState.mode != 1 && fldAreaState.mode != 3) {
            snapInput = 1;
        }
    }
    if (fldAreaState.sceneState > 0) {
        direction = -1;
        snapInput = 0;
    }

    effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF_UNCLOBBERED(vf10, position);
    fldAreaState.previousX = fldAreaState.x;
    fldAreaState.previousY = fldAreaState.y;
    fldAreaState.previousZ = fldAreaState.z;
    fldAreaState.x = position[0];
    fldAreaState.y = position[1];
    fldAreaState.z = position[2];
    while (fldAreaState.negatedAngle >= 360.0f) {
        fldAreaState.negatedAngle -= 360.0f;
    }
    while (fldAreaState.negatedAngle < 0.0f) {
        fldAreaState.negatedAngle += 360.0f;
    }
    while (fldAreaState.angle >= 360.0f) {
        fldAreaState.angle -= 360.0f;
    }
    while (fldAreaState.angle < 0.0f) {
        fldAreaState.angle += 360.0f;
    }
    effObjFetchInnerRotationNormalized((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF_UNCLOBBERED(vf10, quaternion);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfInvertScaledVuTransform();
    effObjSetNodeFlags(((EffWorldNode *)fldPlayerObject)->inner,
                      OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
    if (direction != -1 && D_003BAB50 != 0) {
        if (fldAreaState.unk170 > 0.0f) {
            fldAreaState.y = position[1] + 5.0f;
        } else {
            fldAreaState.y = position[1] + 10.0f;
        }
    }
    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x40) {
        motion = 18;
    } else {
        motion = 2;
    }
    oldMotion = fldPlayerModelContext->current.h.motionIndex;
    effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
    heading = fldAreaState.targetAngle;
    if (direction != -1) {
        heading = directionAngles[direction] - fldAreaState.negatedAngle;
        if (D_003BAD48 != 0) {
            heading = D_003BAD50;
        } else {
            D_003BAD50 = heading;
        }
        if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x40) {
            motion = 17;
        } else {
            motion = 1;
        }
        if (fldAreaState.unk118 != 0) {
            distance = 32.0f;
        } else {
            distance = 25.0f;
        }
        if (fldAreaState.unk170 > 0.0f) {
            distance += ffabsf(fldAreaState.unk170) / 40.0f + 2.0f;
        }
        difference = fldAngleDifference(fldAreaState.angle, heading);
        if (difference < 0.0f) {
            difference = -difference;
        }
        distance = (distance / 180.0f) * (180.0f - difference);
        radians = -heading * 3.14f / 180.0f;
        position[0] = fldAreaState.x - sdfSinPoly(radians) * distance;
        position[1] = fldAreaState.y;
        position[2] = fldAreaState.z - sdfEvaluateCosineViaSinePhaseShift(radians) * distance;
        effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
    } else {
        position[0] = fldAreaState.x;
        position[1] = fldAreaState.y;
        position[2] = fldAreaState.z;
        effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
    }
    while (heading >= 360.0f) {
        heading -= 360.0f;
    }
    while (heading < 0.0f) {
        heading += 360.0f;
    }
    fldAreaState.targetAngle = fldSnapAngleToCompassPoint(heading);
    if (fldAreaState.transitionMode != 0 && direction != -1 &&
        fldIsNearSpark(position[0], position[1], position[2]) != 0) {
        position[0] = fldAreaState.x;
        position[1] = fldAreaState.y;
        position[2] = fldAreaState.z;
        effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
    }
    if (D_003BAB50 != 0) {
        if (direction != -1) {
            fldAreaState.x = position[0];
            fldAreaState.y = position[1];
            fldAreaState.z = position[2];
            current[0] = fldAreaState.x;
            current[1] = fldAreaState.y;
            current[2] = fldAreaState.z;
            fldRefreshAreaStateFromPosition(current);
            if (fldAreaState.positionPending != 0) {
                fldAreaState.x = fldAreaState.targetX;
                fldAreaState.y = fldAreaState.targetY + 44.0f;
                fldAreaState.z = fldAreaState.targetZ;
                position[0] = fldAreaState.x;
                position[1] = fldAreaState.y;
                position[2] = fldAreaState.z;
                effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
                fldAreaState.positionPending = 0;
            }
        }
        effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        PCP_COPY_VECTOR_F32(smoothed, ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition);
        if (ffabsf(smoothed[0] - position[0]) < 1.0f) {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[0] = position[0];
        } else {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[0] =
                smoothed[0] + (position[0] - smoothed[0]) / 3.0f;
        }
        if (ffabsf(smoothed[1] - position[1]) < 1.0f) {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[1] = position[1];
        } else {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[1] =
                smoothed[1] + (position[1] - smoothed[1]) / 3.0f;
        }
        if (ffabsf(smoothed[2] - position[2]) < 1.0f) {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[2] = position[2];
        } else {
            ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[2] =
                smoothed[2] + (position[2] - smoothed[2]) / 3.0f;
        }
        ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[3] = 1.0f;
    }
    func_00136FE8(position);
    difference = fldAngleDifference(fldAreaState.angle, heading);
    step = difference / 5.0f;
    if (step < 0.0f) {
        step = -step;
    }
    if (step < 2.0f) {
        step = 2.0f;
    }
    if ((0.0f <= difference && difference <= step) ||
        (difference <= 0.0f && -step <= difference)) {
        difference = heading;
    } else if (difference < 0.0f) {
        difference = fldAreaState.angle - step;
    } else {
        difference = fldAreaState.angle + step;
    }
    fldAreaState.angle = difference;
    VU0_LOAD_VF(vf10, axis.components);
    effMiscAxisAngleToQuaternionVU(fldAreaState.angle * 3.14f / 180.0f);
    VU0_STORE_VF_UNCLOBBERED(vf10, quaternion);
    effObjSetInnerRotation((EffWorldNode *)fldPlayerObject, (u128 *)quaternion);
    if (fldAreaState.mode != 3 && fldAreaState.mode != 1) {
        if (D_003BAD2C != 0) {
            difference = fldAngleDifference(fldAreaState.negatedAngle, fldAreaState.cameraTargetAngle);
            step = 30.0f;
            if ((0.0f <= difference && difference <= step) ||
                (difference <= 0.0f && -step <= difference)) {
                D_003BAD2C = 0;
                difference = fldAreaState.cameraTargetAngle;
            } else if (difference < 0.0f) {
                difference = fldAreaState.negatedAngle - step;
            } else {
                difference = fldAreaState.negatedAngle + step;
            }
            fldAreaState.negatedAngle = difference;
        }
        if (D_003BAD2C == 0) {
            if (D_00324510[1][0][3] < 0) {
                fldAreaState.cameraTargetAngle = -fldAreaState.targetAngle;
                fldAreaState.cameraTargetAngle = fldSnapAngleToCardinalDirection(fldAreaState.cameraTargetAngle);
                difference = fldAngleDifference(fldAreaState.cameraTargetAngle, fldAreaState.negatedAngle);
                if (difference < -100.0f || difference > 100.0f) {
                    D_003BAD30 = 2;
                    fldAreaState.negatedAngle = fldAreaState.cameraTargetAngle;
                } else {
                    D_003BAD2C = 1;
                }
                if (direction != -1) {
                    D_003BAD4C = direction;
                    D_003BAD48 = 1;
                    D_003BAD54 = 0;
                }
            }
        }
        if (fldAreaState.unkA0 != -1 && direction != -1 &&
            (s32)fldAreaState.negatedAngle != (s32)fldAreaState.cameraTargetAngle) {
            difference = fldAngleDifference(fldAreaState.negatedAngle, fldAreaState.cameraTargetAngle);
            step = 0.4f;
            if ((0.0f <= difference && difference <= step) ||
                (difference <= 0.0f && -step <= difference)) {
                difference = fldAreaState.cameraTargetAngle;
            } else if (difference < 0.0f) {
                difference = fldAreaState.negatedAngle - step;
            } else {
                difference = fldAreaState.negatedAngle + step;
            }
            fldAreaState.negatedAngle = difference;
        }
        if (snapInput != 0) {
            if (D_00324510[1][0][8] < 0 && D_00324510[1][0][10] == 0) {
                fldAreaState.cameraTargetAngle -= 88.0f;
                fldAreaState.cameraTargetAngle = fldSnapAngleToCardinalDirection(fldAreaState.cameraTargetAngle);
                D_003BAD2C = 1;
                fldAreaState.targetAngle = -fldAreaState.cameraTargetAngle;
            } else if (D_00324510[1][0][10] < 0 && D_00324510[1][0][8] == 0) {
                fldAreaState.cameraTargetAngle += 88.0f;
                fldAreaState.cameraTargetAngle = fldSnapAngleToCardinalDirection(fldAreaState.cameraTargetAngle);
                D_003BAD2C = 1;
                fldAreaState.targetAngle = -fldAreaState.cameraTargetAngle;
            }
        } else {
            s32 reversed = fileTestSavedSlotFlags(3) == 0;
            if (D_00324510[1][0][8] != 0) {
                if (D_00324510[1][0][10] == 0) {
                    fldAreaState.negatedAngle -= 4.5f;
                    goto update_camera_angle;
                }
            } else if (D_00324510[1][0][10] != 0) {
                fldAreaState.negatedAngle += 4.5f;
                goto update_camera_angle;
            }
            if (D_003BA870[0][0][0] < 0x20) {
                if (reversed) {
                    fldAreaState.negatedAngle += 3.0f;
                } else {
                    fldAreaState.negatedAngle -= 3.0f;
                }
            } else if (D_003BA870[0][0][0] < 0x40) {
                if (reversed) {
                    fldAreaState.negatedAngle += 1.5f;
                } else {
                    fldAreaState.negatedAngle -= 1.5f;
                }
            } else if (D_003BA870[0][0][0] > 0xE0) {
                if (reversed) {
                    fldAreaState.negatedAngle -= 3.0f;
                } else {
                    fldAreaState.negatedAngle += 3.0f;
                }
            } else if (D_003BA870[0][0][0] > 0xC0) {
                if (reversed) {
                    fldAreaState.negatedAngle -= 1.5f;
                } else {
                    fldAreaState.negatedAngle += 1.5f;
                }
            } else {
                goto camera_input_done;
            }
update_camera_angle:
            fldAreaState.cameraTargetAngle = fldSnapAngleToCompassOctant(fldAreaState.negatedAngle);
        }
    }
camera_input_done:
    dds3SetOwnedWorldInnerValue((EffWorldNode *)fldPlayerObject, 0);
    fldRestoreCameraModelColor();
    if (fldAreaState.dist < 20.0f) {
        fldClearCameraModelColor();
        fldDisableCameraObjectFlag();
        fldClearCameraObjectTransitionFlags();
clear_highlight:
        fldClearCameraObjectHighlightFlag();
    } else {
        fldDisableCameraObjectFlag();
        fldSetCameraObjectTransitionFlag();
        if (fldAreaState.mode == 1 || fldAreaState.mode == 3) {
            goto clear_highlight;
        }
        if (fldAreaState.dist < 100.0f) {
            fldSetCameraObjectHighlightFlag();
        } else {
            fldClearCameraObjectHighlightFlag();
        }
    }
    fldUpdateCameraModelMotion(oldMotion, motion, 6.0f);
    fldDispatchCameraModelFrameEvents(oldMotion, motion);
    fldAreaState.x = position[0];
    fldAreaState.y = position[1];
    fldAreaState.z = position[2];
    D_0032F170[0] = ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[0];
    D_0032F170[1] = ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[1];
    D_0032F170[2] = ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[2];
    D_0032F170[3] = ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[3];
    D_0032F170[4] = ((EffWorldNode *)fldPlayerObject)->inner->rotation[0];
    D_0032F170[5] = ((EffWorldNode *)fldPlayerObject)->inner->rotation[1];
    D_0032F170[6] = ((EffWorldNode *)fldPlayerObject)->inner->rotation[2];
    D_0032F170[7] = ((EffWorldNode *)fldPlayerObject)->inner->rotation[3];
    return 0;
}

/* Field camera model: node id at +0x12, render-data pointer at +0x18. */
s32 fldUpdateCameraFrame(void) {
    s16 node;

    if (D_0032E570[0] != 0) {
        return 0;
    }
    if (fldGetSceneReadyFlag() != 0) {
        return 0;
    }
    if (fldTestSceneControlFlags(0x40) == 0) {
        if (fldAreaState.mode == 1 || fldAreaState.mode == 3) {
            fldClearCameraObjectHighlightFlag();
        }
        func_0012FC20();
        fldUpdateCameraTarget();
        fldUpdateModeOneCameraPose();
        node = fldPlayerModelContext->current.h.motionIndex;
        fldDispatchCameraModelFrameEvents(node, node);
        if (fldAreaState.unk118 == 1) {
            fldUpdateCameraModelMotion(0, 0, 6.0f);
        }
        return 0;
    }
    if (D_0032E400[0] == 1) {
        fldClearCameraObjectHighlightFlag();
        func_0012FF48();
    } else {
        func_0012FF48();
    }
    return 0;
}

u8 fldHasPendingSceneFlags(void) {
    if (fldAreaState.unk17C == 0) {
        if (fldAreaState.unk184 == 0) {
            if (fldAreaState.verticalStepDirection == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void fldSetCameraNodeModeWithZero(void);

void fldEnableCameraObjectFlag(void) {
    dds3SetObjectFlags(fldPlayerObject, 1);
    fldSetCameraNodeModeWithZero();
}

void fldDisableCameraObjectFlag(void) {
    dds3ClearObjectFlags(fldPlayerObject, 1);
}

void fldClearCameraObjectHighlightFlag(void) {
    dds3SetOwnedWorldInnerValue(fldPlayerObject, 0);
    dds3ClearObjectFlags(fldPlayerObject, 0x800);
}

void fldSetCameraObjectHighlightFlag(void) {
    dds3SetOwnedWorldInnerValue(fldPlayerObject, 0x80);
    dds3SetObjectFlags(fldPlayerObject, 0x800);
}

void fldClearCameraModelColor(void) {
    fldPlayerModelContext->inner->color = 0;
}

void fldRestoreCameraModelColor(void) {
    fldPlayerModelContext->inner->color = 0x80808080;
}

void fldSetCameraObjectTransitionFlag(void) {
    dds3SetObjectFlags(fldPlayerObject, 0x200);
}

void fldClearCameraObjectTransitionFlags(void) {
    dds3ClearObjectFlags(fldPlayerObject, 0x400);
    dds3ClearObjectFlags(fldPlayerObject, 0x200);
}

extern void effObjClearNodeFlags(ObjectTransform *, s32);
/* State-first XYZ publication; SDK snapshot audit: docs/idioms.md. */
s32 fldUpdateCameraObjectPose(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 current[3];
    ObjectTransform *inner;

    fldAreaState.positionMode = 0;
    if (D_003BAB50 == 0) return 0;
    effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF_UNCLOBBERED(vf10, position);
    fldAreaState.previousX = fldAreaState.x;
    fldAreaState.previousY = fldAreaState.y;
    fldAreaState.previousZ = fldAreaState.z;
    fldAreaState.x = position[0];
    fldAreaState.y = position[1] + 10.0f;
    fldAreaState.z = position[2];
    current[0] = fldAreaState.x;
    current[1] = fldAreaState.y;
    current[2] = fldAreaState.z;
    fldRefreshAreaStateFromPosition(current);
    if (fldAreaState.positionPending != 0) {
        fldAreaState.x = fldAreaState.targetX;
        fldAreaState.y = fldAreaState.targetY + 44.0f;
        fldAreaState.z = fldAreaState.targetZ;
        position[0] = fldAreaState.x;
        position[1] = fldAreaState.y;
        position[2] = fldAreaState.z;
        effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)position);
        fldAreaState.positionPending = 0;
    }
    effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF_UNCLOBBERED(vf10, position);
    inner = ((EffWorldNode *)fldPlayerObject)->inner;
    PCP_COPY_VECTOR(inner->smoothedPosition, position);
    effObjClearNodeFlags(inner, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
    return 0;
}

extern s32 fldGetLocationCoordinateValue(s32, s32);
extern void fldUpdateCameraModelMotion(s32, s32, f32);

void fldSetCameraNodeModeWithTen(void) {
    s16 node = fldPlayerModelContext->current.h.motionIndex;
    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x40) {
        fldUpdateCameraModelMotion(node, 0x12, 10.0f);
        return;
    }
    fldUpdateCameraModelMotion(node, 3, 10.0f);
}

void fldSetCameraNodeModeWithZero(void) {
    s16 node = fldPlayerModelContext->current.h.motionIndex;
    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x40) {
        fldUpdateCameraModelMotion(node, 0x12, 0.0f);
        return;
    }
    fldUpdateCameraModelMotion(node, 3, 0.0f);
}



void fldAddCameraModelEntry(s32 value) {
    MdlCtx *model = fldPlayerModelContext;
    model->first->frameStep = 1.0f;
    mdlAddEntryPlainEx(model, 0, value, 2.0f, 5.0f);
}

void fldAddCameraModelPair(s32 first, s32 second) {
    mdlSetNodeFrameStep(fldPlayerModelContext, 0, 1.0f);
    mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
    mdlAddEntryFlagged(fldPlayerModelContext, 0, first);
    mdlAddEntryFlagged(fldPlayerModelContext, 1, second);
}

void fldClearCameraFacingPointRequest(void) {
    D_0032E538[0] = 0;
}

void fldClearCameraHeadingRequest(void) {
    D_0032E544[0] = 0;
}


/* Camera facing requests share the field-work block. Both request states
 * advance from 1 to 2 when their new angle is installed. */
void fldQueueCameraXYOverride(f32 targetX, f32 targetZ) {
    FldAreaWork *work = &fldAreaState;

    work->facingPointX = targetX;
    work->facingPointZ = targetZ;
    work->pointState = 1;
}

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z) {
    FldAreaWork *work;
    f32 angle;

    angle = sdfAtan2(x, z);
    work = &fldAreaState;
    angle *= 180.0f / 3.14f;
    work->angleState = 1;
    work->overrideAngle = -angle;
}

void fldApplyCameraFacingPoint(void) {
    FldAreaWork *cameraWork = &fldAreaState;

    if (cameraWork->pointState != 0) {
        f32 deltaX = cameraWork->x - cameraWork->facingPointX;
        f32 deltaZ = cameraWork->z - cameraWork->facingPointZ;
        f32 angle;

        cameraWork->pointState = 2;
        angle = sdfAtan2(deltaX, deltaZ);
        angle *= 180.0f / 3.14f;
        cameraWork->targetAngle = -angle;
    }
}

void fldApplyPendingCameraHeading(void) {
    FldAreaWork *cameraWork = &fldAreaState;

    if (cameraWork->angleState != 0) {
        cameraWork->angleState = 2;
        cameraWork->targetAngle = cameraWork->overrideAngle;
    }
}

extern const PcpVectorCopyF32 D_003A00A8;

/* Advance a pending heading, retain position history, and export the player's
 * smoothed position and rotation. Vertical movement bypasses position damping. */
void fldUpdateSmoothedCameraPose(void) {
    f32 position[4];
    f32 smoothed[4];
    union {
        u128 q;
        f32 f[4];
    } quaternion;
    f32 difference;
    f32 step;
    u32 player;

    memset(&quaternion, 0, sizeof(quaternion));
    quaternion.f[3] = 1.0f;
    {
        PcpVectorCopyF32 axis;

        axis = D_003A00A8;

        if (fldPlayerObject != 0) {
            if (fldAreaState.pointState == 2 || fldAreaState.angleState == 2) {
                difference = fldAngleDifference(fldAreaState.angle, fldAreaState.targetAngle);
                step = difference / 5.0f;
                if (step < 0.0f) {
                    step = -step;
                }
                if (step < 2.0f) {
                    step = 2.0f;
                }
                if ((0.0f <= difference && difference <= step) ||
                    (difference <= 0.0f && -step <= difference)) {
                    fldAreaState.angleState = 0;
                    fldAreaState.pointState = 0;
                    difference = fldAreaState.targetAngle;
                } else if (difference < 0.0f) {
                    difference = fldAreaState.angle - step;
                } else {
                    difference = fldAreaState.angle + step;
                }
                fldAreaState.angle = difference;
                VU0_LOAD_VF(vf10, axis.components);
                effMiscAxisAngleToQuaternionVU(fldAreaState.angle * 3.14f / 180.0f);
                /* The following out-of-line setter consumes all four lanes;
                 * no C scalar read is moved across this legacy SDK store. */
                VU0_STORE_VF_UNCLOBBERED(vf10, quaternion.f);
                effObjSetInnerRotation((EffWorldNode *)fldPlayerObject, &quaternion.q);
            } else {
                f32 angle;
                effObjFetchInnerRotationNormalized((EffWorldNode *)fldPlayerObject);
                angle = effMiscComputeQuaternionRotatedReferenceAngle() * 180.0f / 3.14f;
                fldAreaState.angle = angle;
                fldAreaState.targetAngle = angle;
            }

            effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
            VU0_STORE_VF(vf10, position);
            fldAreaState.previousX = fldAreaState.x;
            fldAreaState.previousY = fldAreaState.y;
            fldAreaState.previousZ = fldAreaState.z;
            fldAreaState.x = position[0];
            fldAreaState.y = position[1];
            fldAreaState.z = position[2];
            effObjSetNodeFlags(((EffWorldNode *)fldPlayerObject)->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
            effObjSetNodeFlags(((EffWorldNode *)fldPlayerObject)->inner, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
            player = fldPlayerObject;
            if (fldAreaState.verticalStepDirection != 0) {
                PCP_COPY_VECTOR(((EffWorldNode *)player)->inner->smoothedPosition,
                                ((EffWorldNode *)player)->inner->position);
            } else {
                fldSetCameraObjectTransitionFlag();
                PCP_COPY_VECTOR_F32(smoothed, ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition);
                if (ffabsf(smoothed[0] - position[0]) < 1.0f) {
                    ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[0] = position[0];
                } else {
                    ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[0] =
                        smoothed[0] + (position[0] - smoothed[0]) / 3.0f;
                }
                if (ffabsf(smoothed[1] - position[1]) < 1.0f) {
                    ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[1] = position[1];
                } else {
                    ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[1] =
                        smoothed[1] + (position[1] - smoothed[1]) / 3.0f;
                }
                if (ffabsf(smoothed[2] - position[2]) < 1.0f) {
                    ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[2] = position[2];
                } else {
                    ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[2] =
                        smoothed[2] + (position[2] - smoothed[2]) / 3.0f;
                }
                ((EffWorldNode *)fldPlayerObject)->inner->smoothedPosition[3] = 1.0f;
                player = fldPlayerObject;
            }
            D_0032F170[0] = ((EffWorldNode *)player)->inner->smoothedPosition[0];
            D_0032F170[1] = ((EffWorldNode *)player)->inner->smoothedPosition[1];
            D_0032F170[2] = ((EffWorldNode *)player)->inner->smoothedPosition[2];
            D_0032F170[3] = ((EffWorldNode *)player)->inner->smoothedPosition[3];
            D_0032F170[4] = ((EffWorldNode *)player)->inner->rotation[0];
            D_0032F170[5] = ((EffWorldNode *)player)->inner->rotation[1];
            D_0032F170[6] = ((EffWorldNode *)player)->inner->rotation[2];
            D_0032F170[7] = ((EffWorldNode *)player)->inner->rotation[3];
        }
    }
}


extern SdfFlagListParams fldCameraColorParameters[];
extern FldCameraSetting *fldCameraSettings;

extern FldCameraSetting D_003306D0;
extern void *fldSkyLightSetBuffer;
extern void *D_003BAD60;
extern FldSkyGradientRecord *D_003BAD74;
extern s32 D_003BAD88, D_003BAD8C;
extern f32 D_003BAD90, D_003BAD94;

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A00A8);

void fldLoadBattleSkyAndFilter(void) {
    s32 i;
    DevState *command;

    if (fldSkyLightSetBuffer == 0) {
        fldSkyLightSetBuffer = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_003BAD60 == 0) {
        D_003BAD60 = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_003BAD74 == 0) {
        D_003BAD74 = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(0x12400));
    }
    if (fldCameraSettings == 0) {
        fldCameraSettings = (FldCameraSetting *)sdfResourceRetainAddress(sdfAllocGeneralBlock(
            sizeof(FldCameraSetting) * FIELD_CAMERA_SETTING_COUNT));
        for (i = 0; i < FIELD_CAMERA_SETTING_COUNT; i++) {
            fldCameraSettings[i] = D_003306D0;
        }
        fldCameraColorParameters->color.segmentMode = SDF_COLOR_TRACK_MODE_ENDPOINTS;
        fldCameraColorParameters->color.finalColor = fldCameraColorParameters->color.initialColor = 0x80808080;
        fldCameraColorParameters->alpha.alpha = 0x40;
        fldCameraColorParameters->alpha.surfaceIndex = 2;
        fldCameraColorParameters->alpha.fadeInFraction = 0.0f;
        fldCameraColorParameters->alpha.fadeOutFraction = 1.0f;
        fldCameraColorParameters->maxFrames = 0;
        fldCameraColorParameters->count = 0xFF;
        fldCameraColorParameters->speed = 20.0f;
    }
    command = sdfDevCreateCommandState("/fld/f/bin/FILTER.FLD");
    sdfDevQueueReadAndWait(command, D_003BAD74, 0x12400);
    sdfDevWaitThenReleaseCommandState(command);
    command = sdfDevCreateCommandState("/fld/f/bin/BATTLEBG.SKY");
    sdfDevQueueReadAndWait(command, D_003BAD60, 0xE000);
    sdfDevWaitThenReleaseCommandState(command);
}

extern char D_003A0100[];
extern u32 fldRainTextureData;

void fldLoadSkyResource(s32 area) {
    char path[64];
    char directory[32];
    DevState *command;

    fldSkyDrawState = 0x80;
    if (area < 200) {
        fldFormatAreaDirectory(directory, area, 1);
        sprintf(path, "%sF%03d.SKY", directory, area);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, fldSkyLightSetBuffer, 0xE000);
        sdfDevWaitThenReleaseCommandState(command);
        if (area >= 2 && area < 100 && fldRainTextureResource == 0) {
            fldRainTextureResource = (s32)(u32)sdfReadNamedResource(D_003A0100, &fldRainTextureData, 0);
            fldRainTextureReference = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(fldRainTextureData));
        }
    }
}

void fldReleaseSkyResources(void) {
    if (fldRainTextureReference != 0) {
        sdfTexReleaseReferenceViaHandler(fldRainTextureReference);
        fldRainTextureReference = 0;
    }
    if (fldRainTextureResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldRainTextureResource);
        fldRainTextureResource = 0;
    }
    if (fldCameraColorEffect != 0) {
        effReleaseSelectionFlagList(fldCameraColorEffect);
        fldCameraColorEffect = 0;
    }
    fldCameraColorEnabled = 0;
}

void fldUploadSkyBuffer(void *src) {
    fldSkyDrawState = 0x80;
    memcpy(fldSkyLightSetBuffer, src, 0xE000);
    fldReleaseSkyResources();
    if (D_0032E3C0[0] >= 2 && D_0032E3C0[0] < 100 && fldRainTextureResource == 0) {
        fldRainTextureResource = (s32)(u32)sdfReadNamedResource(D_003A0100, &fldRainTextureData, 0);
        fldRainTextureReference = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(fldRainTextureData));
    }
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A0100);

void fldUpdateSwayOffset(void) {
    fldSwayOffset = 0;
    switch (fldSwayMode) {
    case 1:
        fldSwayPhase += 0.1f;
        fldSwayOffset = sdfSinPoly(fldSwayPhase) * 32.0f;
        break;
    case 2:
        fldSwayPhase += 0.2f;
        fldSwayOffset = sdfSinPoly(fldSwayPhase) * 32.0f;
        break;
    case 3:
        fldSwayPhase += 0.05f;
        fldSwayOffset = sdfSinPoly(fldSwayPhase) * 32.0f;
        break;
    case 4:
        fldSwayPhase += 0.1f;
        fldSwayOffset = sdfSinPoly(fldSwayPhase) * 48.0f;
        break;
    case 5:
        fldSwayPhase += 0.2f;
        fldSwayOffset = sdfSinPoly(fldSwayPhase) * 48.0f;
        break;
    case 6:
        fldSwayPhase += 0.05f;
        fldSwayOffset = sdfSinPoly(fldSwayPhase) * 48.0f;
        break;
    }
}

/* Draw the current filter grid, blending the previous grid during a fade. */
void fldDrawSkyGradient(void) {
    s32 column, row;
    s32 current = D_003BAD88;
    s32 previous;
    s32 color0, color1, color2, color3;
    s32 r0, g0, b0, a0;
    s32 r1, g1, b1, a1;
    s32 r2, g2, b2, a2;
    s32 r3, g3, b3, a3;
    f32 currentWeight, previousWeight;
    s32 oldR0, oldG0, oldB0, oldA0;
    s32 oldR1, oldG1, oldB1, oldA1;
    s32 oldR2, oldG2, oldB2, oldA2;
    s32 oldR3, oldG3, oldB3, oldA3;

    if (current == 0 || fldGetSceneReadyFlag() != 0) {
        return;
    }
    currentWeight = D_003BAD90;
    if (currentWeight < 1.0f) {
        currentWeight += 1.0f / D_003BAD94;
        if (currentWeight > 1.0f) {
            currentWeight = 1.0f;
            D_003BAD90 = currentWeight;
            D_003BAD8C = current;
        } else {
            D_003BAD90 = currentWeight;
        }
    }
    previousWeight = 1.0f - currentWeight;
    previous = D_003BAD8C;

    fldSelectDisplayBuffer(0x44);
    fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
    switch (D_003BAD74[current].mode) {
    case 0:
        fldSetGsAlphaBlendEquation(0);
        break;
    case 1:
        fldSetGsAlphaBlendEquation(1);
        break;
    case 2:
        fldSetGsAlphaBlendEquation(2);
        break;
    }

    for (row = 0; row < 7; row++) {
        for (column = 0; column < 8; column++) {
            color0 = D_003BAD74[current].colors[row][column];
            color1 = D_003BAD74[current].colors[row][column + 1];
            color2 = D_003BAD74[current].colors[row + 1][column + 1];
            color3 = D_003BAD74[current].colors[row + 1][column];
            r0 = color0 & 0xFF;
            g0 = (color0 >> 8) & 0xFF;
            b0 = (color0 >> 16) & 0xFF;
            a0 = (u32)color0 >> 24;
            r1 = color1 & 0xFF;
            g1 = (color1 >> 8) & 0xFF;
            b1 = (color1 >> 16) & 0xFF;
            a1 = (u32)color1 >> 24;
            r2 = color2 & 0xFF;
            g2 = (color2 >> 8) & 0xFF;
            b2 = (color2 >> 16) & 0xFF;
            a2 = (u32)color2 >> 24;
            r3 = color3 & 0xFF;
            g3 = (color3 >> 8) & 0xFF;
            b3 = (color3 >> 16) & 0xFF;
            a3 = (u32)color3 >> 24;

            a0 += fldSwayOffset;
            a1 += fldSwayOffset;
            a2 += fldSwayOffset;
            a3 += fldSwayOffset;
            a0 += (s32)D_0032E570[14];
            a1 += (s32)D_0032E570[14];
            a2 += (s32)D_0032E570[14];
            a3 += (s32)D_0032E570[14];
            if (a0 < 0) a0 = 0;
            if (a0 > 255) a0 = 255;
            if (a1 < 0) a1 = 0;
            if (a1 > 255) a1 = 255;
            if (a2 < 0) a2 = 0;
            if (a2 > 255) a2 = 255;
            /* Retail has no corresponding clamp for a3. */

            if (D_003BAD90 < 1.0f) {
                s32 oldColor0 = D_003BAD74[previous].colors[row][column];
                s32 oldColor1 = D_003BAD74[previous].colors[row][column + 1];
                s32 oldColor2 = D_003BAD74[previous].colors[row + 1][column + 1];
                s32 oldColor3 = D_003BAD74[previous].colors[row + 1][column];
                oldR0 = oldColor0 & 0xFF;
                oldG0 = (oldColor0 >> 8) & 0xFF;
                oldB0 = (oldColor0 >> 16) & 0xFF;
                oldA0 = (u32)oldColor0 >> 24;
                oldR1 = oldColor1 & 0xFF;
                oldG1 = (oldColor1 >> 8) & 0xFF;
                oldB1 = (oldColor1 >> 16) & 0xFF;
                oldA1 = (u32)oldColor1 >> 24;
                oldR2 = oldColor2 & 0xFF;
                oldG2 = (oldColor2 >> 8) & 0xFF;
                oldB2 = (oldColor2 >> 16) & 0xFF;
                oldA2 = (u32)oldColor2 >> 24;
                oldR3 = oldColor3 & 0xFF;
                oldG3 = (oldColor3 >> 8) & 0xFF;
                oldB3 = (oldColor3 >> 16) & 0xFF;
                oldA3 = (u32)oldColor3 >> 24;

                oldA0 += fldSwayOffset;
                oldA1 += fldSwayOffset;
                oldA2 += fldSwayOffset;
                oldA3 += fldSwayOffset;
                oldA0 += (s32)D_0032E570[14];
                oldA1 += (s32)D_0032E570[14];
                oldA2 += (s32)D_0032E570[14];
                oldA3 += (s32)D_0032E570[14];
                if (oldA0 < 0) oldA0 = 0;
                if (oldA0 > 255) oldA0 = 255;
                if (oldA1 < 0) oldA1 = 0;
                if (oldA1 > 255) oldA1 = 255;
                if (oldA2 < 0) oldA2 = 0;
                if (oldA2 > 255) oldA2 = 255;

                r0 = (s32)(r0 * currentWeight + oldR0 * previousWeight);
                g0 = (s32)(g0 * currentWeight + oldG0 * previousWeight);
                b0 = (s32)(b0 * currentWeight + oldB0 * previousWeight);
                a0 = (s32)(a0 * currentWeight + oldA0 * previousWeight);
                r1 = (s32)(r1 * currentWeight + oldR1 * previousWeight);
                g1 = (s32)(g1 * currentWeight + oldG1 * previousWeight);
                b1 = (s32)(b1 * currentWeight + oldB1 * previousWeight);
                a1 = (s32)(a1 * currentWeight + oldA1 * previousWeight);
                r2 = (s32)(r2 * currentWeight + oldR2 * previousWeight);
                g2 = (s32)(g2 * currentWeight + oldG2 * previousWeight);
                b2 = (s32)(b2 * currentWeight + oldB2 * previousWeight);
                a2 = (s32)(a2 * currentWeight + oldA2 * previousWeight);
                r3 = (s32)(r3 * currentWeight + oldR3 * previousWeight);
                g3 = (s32)(g3 * currentWeight + oldG3 * previousWeight);
                b3 = (s32)(b3 * currentWeight + oldB3 * previousWeight);
                a3 = (s32)(a3 * currentWeight + oldA3 * previousWeight);
            }

            if (fldSkyDrawState != 0x80) {
                r0 = r0 * (s32)fldSkyDrawState / 128;
                g0 = g0 * (s32)fldSkyDrawState / 128;
                b0 = b0 * (s32)fldSkyDrawState / 128;
                a0 = a0 * (s32)fldSkyDrawState / 128;
                r1 = r1 * (s32)fldSkyDrawState / 128;
                g1 = g1 * (s32)fldSkyDrawState / 128;
                b1 = b1 * (s32)fldSkyDrawState / 128;
                a1 = a1 * (s32)fldSkyDrawState / 128;
                r2 = r2 * (s32)fldSkyDrawState / 128;
                g2 = g2 * (s32)fldSkyDrawState / 128;
                b2 = b2 * (s32)fldSkyDrawState / 128;
                a2 = a2 * (s32)fldSkyDrawState / 128;
                r3 = r3 * (s32)fldSkyDrawState / 128;
                g3 = g3 * (s32)fldSkyDrawState / 128;
                b3 = b3 * (s32)fldSkyDrawState / 128;
                a3 = a3 * (s32)fldSkyDrawState / 128;
            }
            fldSubmitGsGradientQuad(column * 64, row * 32, 64, 32,
                r0, g0, b0, a0, r1, g1, b1, a1,
                r2, g2, b2, a2, r3, g3, b3, a3);
        }
    }
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    fldSetGsAlphaBlendEquation(0);
}

extern s32 D_003BAD58;
void fldSetFadeTarget(s32 area, s32 value, s32 duration) {
    if (D_003BAD58 == 0 && D_0032E3C0[0] < 40) {
        duration = 0;
    }
    if (duration == 0) {
        D_003BAD8C = area;
        D_003BAD90 = 1.0f;
        D_003BAD94 = 1.0f;
    } else {
        D_003BAD90 = 0.0f;
        D_003BAD8C = D_003BAD88;
        D_003BAD94 = (f32)duration;
    }
    D_003BAD88 = area;
    D_003BAD74[area].mode = value;
}

void fldSetSwayMode(u32 mode) {
    fldSwayMode = mode;
    fldSwayPhase = 0;
    fldSwayOffset = 0;
}

void fldSetSkyDrawState(u32 value) {
    fldSkyDrawState = value;
}

u32 fldGetSkyDrawState(void) {
    return fldSkyDrawState;
}

void fldSetSkyAlpha(u32 value) {
    D_0032E5A8[0] = value;
}

u32 fldGetSkyAlpha(void) {
    return D_0032E5A8[0];
}

void fldBeginSelectedValueTransition(u32 value) {
    u32 previousValue = D_0032E59C[0];

    D_003BAD9C = value;
    D_003BADA0 = 0;
    D_003BADA4 = previousValue;
    fldApplySkyLightSet(previousValue, 1);
}

extern char D_003A0138[];

/* Alternate the selected value at the requested rate, and apply live lighting edits. */
void fldUpdateSelectedValueTransition(void) {
    EvtUnit *player;
    f32 direction[4];
    s32 red, green, blue;
    u32 colorA, colorB;

    switch ((s32)D_003BAD9C) {
    case 0: /* Disabled. */
        break;
    case 1:
        D_003BADA0 += 1.0f;
        if (D_003BADA0 == 30.0f) {
            fldApplySkyLightSet((D_003BADA4 + 1) & 0xFF, 30);
        } else if (D_003BADA0 == 60.0f) {
            fldApplySkyLightSet(D_003BADA4, 30);
            D_003BADA0 = 0;
        }
        break;
    case 2:
        D_003BADA0 += 1.0f;
        if (D_003BADA0 == 15.0f) {
            fldApplySkyLightSet((D_003BADA4 + 1) & 0xFF, 1);
        } else if (D_003BADA0 == 30.0f) {
            fldApplySkyLightSet(D_003BADA4, 1);
            D_003BADA0 = 0;
        }
        break;
    case 3:
        D_003BADA0 += 1.0f;
        if (D_003BADA0 == 60.0f) {
            fldApplySkyLightSet((D_003BADA4 + 1) & 0xFF, 60);
        } else if (D_003BADA0 == 120.0f) {
            fldApplySkyLightSet(D_003BADA4, 60);
            D_003BADA0 = 0;
        }
        break;
    }
    if (kwlnTaskGetTaskByName(D_003A0138) != NULL) {
        player = evtUnitGetNestedValue((EffWorldNode *)fldPlayerObject);
        evtSetUnitStatusFlags(player);
        red = D_00330670[0] * 128.0f;
        green = D_00330670[1] * 128.0f;
        blue = D_00330670[2] * 128.0f;
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = D_003306C0[0] * 128.0f;
        green = D_003306C0[1] * 128.0f;
        blue = D_003306C0[2] * 128.0f;
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        evtInitializeUnitColorTransition(player, 0, colorA, colorB);
        direction[0] = D_00330670[4];
        direction[1] = D_00330670[5];
        direction[2] = D_00330670[6];
        direction[3] = 0.0f;
        VU0_LOAD_VF(vf10, direction);
        evtSetUnitNormalizedDirection(player, 0);
    }
}

/* VU0 direction setters consume vf10, as in the DDS2 lighting routine. */
void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                   f32 redB, f32 greenB, f32 blueB,
                   f32 x, f32 y, f32 z) {
    EvtUnit *player;
    u32 colorA;
    u32 colorB;
    s32 red, green, blue;
    f32 direction[4];

    player = evtUnitGetNestedValue((EffWorldNode *)fldPlayerObject);
    evtSetUnitStatusFlags(player);
    red = redA * 128.0f;
    green = greenA * 128.0f;
    blue = blueA * 128.0f;
    colorA = red | (blue << 16) | (green << 8) | 0x80000000;
    red = redB * 128.0f;
    green = greenB * 128.0f;
    blue = blueB * 128.0f;
    colorB = red | (blue << 16) | (green << 8) | 0x80000000;
    evtInitializeUnitColorTransition(player, duration, colorA, colorB);
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
    direction[3] = 0.0f;
    VU0_LOAD_VF(vf10, direction);
    evtSetUnitNormalizedDirection(player, duration);
    VU0_LOAD_VF(vf10, direction);
    evtSetUnitNormalizedDirection(player, 0);
    D_00330670[0] = redA;
    D_00330670[1] = greenA;
    D_00330670[2] = blueA;
    D_00330670[4] = x;
    D_00330670[5] = y;
    D_00330670[6] = z;
    D_003306C0[0] = redB;
    D_003306C0[1] = greenB;
    D_003306C0[2] = blueB;
}

extern void kwlnSetDrawColorTarget(s32, f32 *);
extern s32 kwlnSetLightColorTarget(s32, s32, f32 *);
extern void kwlnSetBackgroundColorTarget(s32, f32 *);
extern s32 kwlnSetLightDirectionTarget(s32, s32, f32 *);
extern void evtSetDrawVectorTarget(s32, f32, f32, f32, f32);
extern void fldSetSwayMode(u32);
extern u32 D_003BADA8;
extern void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                          f32 redB, f32 greenB, f32 blueB, f32 x, f32 y, f32 z);

/* Start fading to the sky light set at index. A zero scene command also applies the set to the
 * display state and player lighting using the (clamped) transition duration. */
void fldApplySkyLightSet(u32 index, s32 mode) {
    FldAreaWork *state = &fldAreaState;
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (state->area < 200) {
        if (D_003BAD58 == 0 && state->area < 40) {
            mode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (mode != 0) {
            duration = defaultDuration;
        }
        if (mode >= 2) {
            duration = mode;
        }
        D_003BAD98 = index;
        D_003BADC8 = index;
        D_003BADA8 = 0;
        if (state->sceneCommand == 0) {
            light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
            area = light->fadeArea;
            D_0032E570[13] = area;
            D_0032E570[14] = light->displayStateSlot14;
            value = light->fadeMode;
            D_0032E570[15] = value;
            D_0032E570[16] = light->swayMode;
            fldSetFadeTarget(area, value, duration);
            fldSetSwayMode(D_0032E570[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            kwlnSetDrawColorTarget(duration, vec);
            evtSetDrawVectorTarget(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            kwlnSetBackgroundColorTarget(duration, vec);
            fldSetPlayerLighting(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                          light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                          light->unitLightDirection[0], light->unitLightDirection[1],
                          light->unitLightDirection[2]);
        }
    }
}

extern s32 D_0032E3C0[];
extern s16 D_0032E4D8[];
extern s32 D_003BAD58;

void fldApplyPendingSceneValueWithSpeed(s32 speed) {
    if (D_003BAD58 == 0 && D_0032E3C0[0] < 40) {
        speed = 0;
    }
    if (D_003BADC8 != 0 && D_003BAD98 != D_003BADC8) {
        if (D_0032E4D8[0] == 0) {
            D_0032E59C[0] = D_003BADC8;
        }
        fldApplySkyLightSet(D_003BADC8, speed);
    }
}

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
extern void kwlnSetDrawColorTarget(s32, f32 *);
extern s32 kwlnSetLightColorTarget(s32, s32, f32 *);
extern void kwlnSetBackgroundColorTarget(s32, f32 *);
extern s32 kwlnSetLightDirectionTarget(s32, s32, f32 *);
extern void evtSetDrawVectorTarget(s32, f32, f32, f32, f32);

void fldTransitionSkyLightSet(s32 index, s32 mode) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;
    u32 previousIndex;

    if (fldAreaState.area < 200) {
        if (D_003BAD58 == 0 && fldAreaState.area < 40) {
            mode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (mode != 0) {
            duration = defaultDuration;
        }
        if (mode >= 2) {
            duration = mode;
        }
        if (D_003BAD98 != index) {
            previousIndex = D_003BADC8;
            if (D_003BADA8 == 0) {
                previousIndex = D_003BAD98;
            }
            D_003BADAC = fldSwayMode;
            D_003BADB0 = fldSwayPhase;
            D_003BADB4 = fldSwayOffset;
            D_003BADB8 = D_003BAD88;
            D_003BADBC = D_003BAD8C;
            D_003BADC0 = D_003BAD90;
            D_003BADC4 = D_003BAD94;
            D_003BADC8 = previousIndex;
            D_003BADCC = D_003BAD9C;
            D_003BADD0 = D_003BADA0;
            D_003BADD4 = D_003BADA4;
            D_003BADA8 = 1;
            D_003BAD98 = index;
            if (fldAreaState.sceneCommand == 0) {
                light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
                D_0032E570[11] = index;
                area = light->fadeArea;
                D_0032E570[13] = area;
                D_0032E570[14] = light->displayStateSlot14;
                value = light->fadeMode;
                D_0032E570[15] = value;
                D_0032E570[16] = light->swayMode;
                fldSetFadeTarget(area, value, duration);
                fldSetSwayMode(D_0032E570[16]);
                vec[0] = light->drawColorFixedR * 0.00390625f;
                vec[1] = light->drawColorFixedG * 0.00390625f;
                vec[2] = light->drawColorFixedB * 0.00390625f;
                vec[3] = 0;
                kwlnSetDrawColorTarget(duration, vec);
                evtSetDrawVectorTarget(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
                dir[0] = light->lightDirectionAX;
                dir[1] = light->lightDirectionAY;
                dir[2] = light->lightDirectionAZ;
                dir[3] = 0;
                kwlnSetLightDirectionTarget(duration, 0, dir);
                vec[0] = light->lightColorAX;
                vec[1] = light->lightColorAY;
                vec[2] = light->lightColorAZ;
                vec[3] = 0;
                kwlnSetLightColorTarget(duration, 0, vec);
                dir[0] = light->lightDirectionBX;
                dir[1] = light->lightDirectionBY;
                dir[2] = light->lightDirectionBZ;
                dir[3] = 0;
                kwlnSetLightDirectionTarget(duration, 1, dir);
                vec[0] = light->lightColorBX;
                vec[1] = light->lightColorBY;
                vec[2] = light->lightColorBZ;
                vec[3] = 0;
                kwlnSetLightColorTarget(duration, 1, vec);
                dir[0] = light->lightDirectionCX;
                dir[1] = light->lightDirectionCY;
                dir[2] = light->lightDirectionCZ;
                dir[3] = 0;
                kwlnSetLightDirectionTarget(duration, 2, dir);
                vec[0] = light->lightColorCX;
                vec[1] = light->lightColorCY;
                vec[2] = light->lightColorCZ;
                vec[3] = 0;
                kwlnSetLightColorTarget(duration, 2, vec);
                vec[0] = light->backgroundColorX;
                vec[1] = light->backgroundColorY;
                vec[2] = light->backgroundColorZ;
                vec[3] = 1.0f;
                kwlnSetBackgroundColorTarget(duration, vec);
                fldSetPlayerLighting(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                              light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                              light->unitLightDirection[0], light->unitLightDirection[1],
                              light->unitLightDirection[2]);
            }
        }
    }
}

void fldSetCameraObjectActiveFlag(s32 enabled) {
    if (enabled == 0) {
        dds3ClearObjectFlags(fldPlayerObject, 0x100);
        return;
    }
    dds3SetObjectFlags(fldPlayerObject, 0x100);
    evtEndObjectValueTransition((EffWorldNode *)fldPlayerObject);
}

/*BEGIN fldApplySceneLightSet*/
extern void kwlnSetDrawColorTarget(s32, f32 *);
extern s32 kwlnSetLightColorTarget(s32, s32, f32 *);
extern void kwlnSetBackgroundColorTarget(s32, f32 *);
extern s32 kwlnSetLightDirectionTarget(s32, s32, f32 *);
extern void evtSetDrawVectorTarget(s32, f32, f32, f32, f32);
extern void fldSetSwayMode(u32);
extern u32 D_003BADDC;
extern void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                          f32 redB, f32 greenB, f32 blueB, f32 x, f32 y, f32 z);

void fldApplySceneLightSet(s32 index, s32 mode) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (D_0032E3C0[0] < 200) {
        if (D_003BAD58 == 0 && D_0032E3C0[0] < 40) {
            mode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (mode != 0) {
            duration = defaultDuration;
        }
        if (mode >= 2) {
            duration = mode;
        }
        if (D_003BADD8 != index) {
            if (index == 0) {
                index = D_003BAD98;
            }
            light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
            D_003BADDC = D_0032E570[11];
            D_0032E570[11] = index;
            D_003BADD8 = index;
            area = light->fadeArea;
            D_0032E570[13] = area;
            D_0032E570[14] = light->displayStateSlot14;
            value = light->fadeMode;
            D_0032E570[15] = value;
            D_0032E570[16] = light->swayMode;
            fldSetFadeTarget(area, value, duration);
            fldSetSwayMode(D_0032E570[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            kwlnSetDrawColorTarget(duration, vec);
            evtSetDrawVectorTarget(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            kwlnSetBackgroundColorTarget(duration, vec);
            fldSetPlayerLighting(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                          light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                          light->unitLightDirection[0], light->unitLightDirection[1],
                          light->unitLightDirection[2]);
        }
    }
}
/*END fldApplySceneLightSet*/

void fldSetDisplayState(u32 state) {
    D_003BAD98 = state;
    D_003BADC8 = 0;
    D_0032E570[12] = 0;
    D_0032E570[11] = state;
    D_003BADD8 = 0;
}

void fldInitializeDisplayPointerTable(void) {
    u32 *displayPointers = D_00330738;

    memset(displayPointers, 0, 0x14);
    displayPointers[0] = (u32)D_003306B0;
    displayPointers[1] = (u32)D_003306C0;
}

extern void kwlnSetDrawColorTarget(s32, f32 *);
extern s32 kwlnSetLightColorTarget(s32, s32, f32 *);
extern void kwlnSetBackgroundColorTarget(s32, f32 *);
extern s32 kwlnSetLightDirectionTarget(s32, s32, f32 *);
extern void evtSetDrawVectorTarget(s32, f32, f32, f32, f32);
extern void fldSetSwayMode(u32);

void fldApplySkyLightSetToPlayerVU(void) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    EvtUnit *unit;
    u32 colorA;
    u32 colorB;
    s32 red, green, blue;

    if (D_0032E3C0[0] != 1 && D_0032E3C0[0] < 200) {
        if (D_003BADD8 != 0) {
            light = &((FldLightSet *)fldSkyLightSetBuffer)[D_003BADD8];
        } else {
            light = &((FldLightSet *)fldSkyLightSetBuffer)[D_003BAD98];
        }
        fldSetFadeTarget(D_0032E570[13], D_0032E570[15], 0);
        fldSetSwayMode(D_0032E570[16]);

        vec[0] = light->drawColorFixedR * 0.00390625f;
        vec[1] = light->drawColorFixedG * 0.00390625f;
        vec[2] = light->drawColorFixedB * 0.00390625f;
        vec[3] = 0.0f;
        kwlnSetDrawColorTarget(0, vec);
        evtSetDrawVectorTarget(0, light->fogNearValue, light->fogFarValue,
                               light->depthNearValue, light->depthFarValue);

        dir[0] = light->lightDirectionAX;
        dir[1] = light->lightDirectionAY;
        dir[2] = light->lightDirectionAZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 0, dir);
        vec[0] = light->lightColorAX;
        vec[1] = light->lightColorAY;
        vec[2] = light->lightColorAZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 0, vec);
        dir[0] = light->lightDirectionBX;
        dir[1] = light->lightDirectionBY;
        dir[2] = light->lightDirectionBZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 1, dir);
        vec[0] = light->lightColorBX;
        vec[1] = light->lightColorBY;
        vec[2] = light->lightColorBZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 1, vec);
        dir[0] = light->lightDirectionCX;
        dir[1] = light->lightDirectionCY;
        dir[2] = light->lightDirectionCZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 2, dir);
        vec[0] = light->lightColorCX;
        vec[1] = light->lightColorCY;
        vec[2] = light->lightColorCZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 2, vec);
        vec[0] = light->backgroundColorX;
        vec[1] = light->backgroundColorY;
        vec[2] = light->backgroundColorZ;
        vec[3] = 1.0f;
        kwlnSetBackgroundColorTarget(0, vec);

        unit = evtUnitGetNestedValue((EffWorldNode *)fldPlayerObject);
        evtSetUnitStatusFlags(unit);
        red = light->unitColorA[0] * 128.0f;
        green = light->unitColorA[1] * 128.0f;
        blue = light->unitColorA[2] * 128.0f;
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = light->unitColorB[0] * 128.0f;
        green = light->unitColorB[1] * 128.0f;
        blue = light->unitColorB[2] * 128.0f;
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        evtInitializeUnitColorTransition(unit, 0, colorA, colorB);
        dir[0] = light->unitLightDirection[0];
        dir[1] = light->unitLightDirection[1];
        dir[2] = light->unitLightDirection[2];
        dir[3] = 0.0f;
        VU0_LOAD_VF(vf10, dir);
        evtSetUnitNormalizedDirection(unit, 0);
        VU0_LOAD_VF(vf10, dir);
        evtSetUnitNormalizedDirection(unit, 0);
    }
}

void fldApplyLightSetCurrent(void) {
    FldLightSet *light = &((FldLightSet *)D_003BAD60)[D_003BAD98];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    area = light->fadeArea;
    D_0032E570[13] = area;
    D_0032E570[14] = light->displayStateSlot14;
    value = light->fadeMode;
    D_0032E570[15] = value;
    D_0032E570[16] = light->swayMode;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_0032E570[16]);
    vec[0] = light->drawColorFixedR * 0.00390625f;
    vec[1] = light->drawColorFixedG * 0.00390625f;
    vec[2] = light->drawColorFixedB * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 0, dir);
    vec[0] = light->lightColorAX;
    vec[1] = light->lightColorAY;
    vec[2] = light->lightColorAZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 1, dir);
    vec[0] = light->lightColorBX;
    vec[1] = light->lightColorBY;
    vec[2] = light->lightColorBZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 2, dir);
    vec[0] = light->lightColorCX;
    vec[1] = light->lightColorCY;
    vec[2] = light->lightColorCZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 2, vec);
    vec[0] = light->backgroundColorX;
    vec[1] = light->backgroundColorY;
    vec[2] = light->backgroundColorZ;
    vec[3] = 1.0f;
    kwlnSetBackgroundColorTarget(0, vec);
}

void fldApplyLightSetIndex(s32 index) {
    FldLightSet *light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    D_003BAD98 = index;
    area = light->fadeArea;
    D_0032E570[13] = area;
    D_0032E570[14] = light->displayStateSlot14;
    value = light->fadeMode;
    D_0032E570[15] = value;
    D_0032E570[16] = light->swayMode;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_0032E570[16]);
    vec[0] = light->drawColorFixedR * 0.00390625f;
    vec[1] = light->drawColorFixedG * 0.00390625f;
    vec[2] = light->drawColorFixedB * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 0, dir);
    vec[0] = light->lightColorAX;
    vec[1] = light->lightColorAY;
    vec[2] = light->lightColorAZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 1, dir);
    vec[0] = light->lightColorBX;
    vec[1] = light->lightColorBY;
    vec[2] = light->lightColorBZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 2, dir);
    vec[0] = light->lightColorCX;
    vec[1] = light->lightColorCY;
    vec[2] = light->lightColorCZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 2, vec);
    vec[0] = light->backgroundColorX;
    vec[1] = light->backgroundColorY;
    vec[2] = light->backgroundColorZ;
    vec[3] = 1.0f;
    kwlnSetBackgroundColorTarget(0, vec);
}

extern FldCameraSetting fldAppliedCameraSettings[];
void fldActivateCameraColorSetting(s32 enable) {
    FldCameraSetting *setting;
    FldColorParams *color;

    if (fldCameraColorEnabled == 0 && enable != 0) {
        if (fldCameraColorEffect != 0) {
            effReleaseSelectionFlagList(fldCameraColorEffect);
        }
        setting = fldCameraSettings;
        fldCameraColorEffect = 0;
        color = &setting->color;
        if (color->enabled != 0) {
            fldCameraColorParameters->color.finalColor = fldCameraColorParameters->color.initialColor = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
            fldCameraColorParameters->alpha.alpha = color->vectorY;
            switch (color->mode) {
            case 0:
                fldCameraColorParameters->alpha.surfaceIndex = 1;
                break;
            case 1:
                fldCameraColorParameters->alpha.surfaceIndex = 2;
                break;
            default:
                fldCameraColorParameters->alpha.surfaceIndex = 3;
                break;
            }
            fldCameraColorParameters->count = color->slotIndex;
            fldCameraColorParameters->speed = color->vectorZ;
            fldCameraColorEffect = effCreateSelectionFlagListFromWork(fldCameraColorParameters);
            setting = fldCameraSettings;
        }
    } else {
        setting = fldCameraSettings;
    }
    *fldAppliedCameraSettings = *setting;
    fldCameraColorEnabled = enable;
}

extern s32 D_003BADE0;
s32 fldComposeFadeColor(s32 fade, s32 color, s32 alpha) {
    s32 scaled;

    if (fade < 0) {
        fade = 0;
    }
    scaled = alpha * D_003BADE0 / 100;
    if (fade < 0xE0) {
        return color | (scaled << 24);
    }
    scaled = (1.0f - (f32)(fade - 0xE0) * 0.00390625f) * scaled;
    if (scaled < 0) {
        scaled = 0;
    }
    if (scaled > 0x80) {
        scaled = 0x80;
    }
    return color | (scaled << 24);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldDrawCameraColorScrollingGradient);

extern char D_003A0100[];
extern u32 fldRainTextureData;

void fldInitializeCameraColorResource(void) {
    fldRainTextureResource = (s32)(u32)sdfReadNamedResource(D_003A0100, &fldRainTextureData, 0);
    fldRainTextureReference = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(fldRainTextureData));
    fldCameraColorEffect = effCreateSelectionFlagListFromWork(fldCameraColorParameters);
    if (fldRainTextureResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldRainTextureResource);
        fldRainTextureResource = 0;
    }
    fldUpdateCameraColorEffect(fldCameraSettings);
    fldCameraColorEnabled = 1;
}

void fldDrawCameraColorLayerPass(void) {
    fldDrawCameraColorScrollingGradient();
}

void fldReleaseCameraColorEffect(void) {
    fldCameraColorEnabled = 0;
    if (fldRainTextureReference != 0) {
        sdfTexReleaseReferenceViaHandler(fldRainTextureReference);
        fldRainTextureReference = 0;
    }
    if (fldCameraColorEffect != 0) {
        effReleaseSelectionFlagList(fldCameraColorEffect);
        fldCameraColorEffect = 0;
    }
}

void fldCopyCameraSetting(FldCameraSetting *destination) {
    *destination = *fldAppliedCameraSettings;
}

void fldUpdateCameraColorEffect(FldCameraSetting *setting) {
    FldColorParams *color = &setting->color;

    if (color->enabled != 0) {
        fldCameraColorParameters->color.finalColor = fldCameraColorParameters->color.initialColor = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
        fldCameraColorParameters->alpha.alpha = color->vectorY;
        switch (color->mode) {
        case 0:
            fldCameraColorParameters->alpha.surfaceIndex = 1;
            break;
        case 1:
            fldCameraColorParameters->alpha.surfaceIndex = 2;
            break;
        default:
            fldCameraColorParameters->alpha.surfaceIndex = 3;
            break;
        }
        fldCameraColorParameters->count = color->slotIndex;
        fldCameraColorParameters->speed = color->vectorZ;
        itfCopyColorFields(fldCameraColorEffect, fldCameraColorParameters);
    }
    *fldAppliedCameraSettings = *setting;
}


/* Keep both the resource handles and retained addresses: callers use the
 * retained storage, whereas the handles are needed at release time. */
void fldAllocateRecordStorage(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(FIELD_VALUE_RECORD_STORAGE_SIZE);
    void *storage;

    fldValueRecordResource = (u32)allocation;
    storage = (void *)sdfResourceRetainAddress(allocation);
    fldValueRecords = (u32)storage;
    memset(storage, 0, FIELD_VALUE_RECORD_STORAGE_SIZE);
    allocation = sdfAllocGeneralBlock(FIELD_AUX_RECORD_STORAGE_SIZE);
    fldAuxRecordResource = (u32)allocation;
    storage = (void *)sdfResourceRetainAddress(allocation);
    fldAuxRecordBuffer = (u32)storage;
    memset(storage, 0, FIELD_AUX_RECORD_STORAGE_SIZE);
}

/* Release both retained resources and clear the usable buffer addresses. */
void fldReleaseRecordStorage(void) {
    sdfDecrementAllocationReferenceCount((struct SdfMemBlock *)fldValueRecordResource);
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldValueRecordResource);
    fldValueRecords = 0;
    sdfDecrementAllocationReferenceCount((struct SdfMemBlock *)fldAuxRecordResource);
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldAuxRecordResource);
    fldAuxRecordBuffer = 0;
}

/* Dot product of XYZ only; any fourth vector component is ignored. */
float fldDotVector(float *left, float *right) {
    return *left * *right + left[1] * right[1] + left[2] * right[2];
}

/* Return the Euclidean XYZ length without normalizing the input vector. */
f32 fldCalculateVectorLength(const f32 *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

/* Build a normalized plane from three vertices spaced four floats apart.
 * Snap near-zero normal components before deriving D from the third vertex.
 * Degenerate triangles retain the native divide-by-zero behavior. */
void fldCalcTrianglePlane(f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant) {
    f32 firstPoint[4];
    f32 secondPoint[4];
    f32 thirdPoint[4];
    f32 inverseLength;

    firstPoint[0] = vertices[0];
    firstPoint[1] = vertices[1];
    firstPoint[2] = vertices[2];
    secondPoint[0] = vertices[4];
    secondPoint[1] = vertices[5];
    secondPoint[2] = vertices[6];
    thirdPoint[0] = vertices[8];
    thirdPoint[1] = vertices[9];
    thirdPoint[2] = vertices[10];
    *normalX = (secondPoint[1] - firstPoint[1]) * (thirdPoint[2] - secondPoint[2]) - (secondPoint[2] - firstPoint[2]) * (thirdPoint[1] - secondPoint[1]);
    *normalY = (secondPoint[2] - firstPoint[2]) * (thirdPoint[0] - secondPoint[0]) - (secondPoint[0] - firstPoint[0]) * (thirdPoint[2] - secondPoint[2]);
    *normalZ = (secondPoint[0] - firstPoint[0]) * (thirdPoint[1] - secondPoint[1]) - (secondPoint[1] - firstPoint[1]) * (thirdPoint[0] - secondPoint[0]);
    inverseLength = 1.0f / fsqrtf(*normalX * *normalX + *normalY * *normalY + *normalZ * *normalZ);
    *normalX = *normalX * inverseLength;
    *normalY = *normalY * inverseLength;
    *normalZ = *normalZ * inverseLength;
    if (*normalX > -0.0001f && *normalX < 0.0001f) {
        *normalX = 0.0f;
    }
    if (*normalY > -0.0001f && *normalY < 0.0001f) {
        *normalY = 0.0f;
    }
    if (*normalZ > -0.0001f && *normalZ < 0.0001f) {
        *normalZ = 0.0f;
    }
    *planeConstant = -(*normalX * thirdPoint[0] + *normalY * thirdPoint[1] + *normalZ * thirdPoint[2]);
}

/* Return the first matching record's value; zero also denotes a missing ID. */
s32 fldGetRecordValueById(s32 id) {
    s32 index = 0;

    if (fldValueRecordCount > 0) {
        FldValueRecord *record = (FldValueRecord *)fldValueRecords;
        do {
            if (record->id == id) {
                return record->value;
            }
            index++;
            record++;
        } while (index < fldValueRecordCount);
    }
    return 0;
}

/* Update every matching ID, including duplicates; leave other records intact. */
void fldSetRecordValueById(s32 id, s32 value) {
    s32 index;

    for (index = 0; index < fldValueRecordCount; index++) {
        if (((FldValueRecord *)fldValueRecords)[index].id == id) {
            ((FldValueRecord *)fldValueRecords)[index].value = value;
        }
    }
}

void fldResetRecordState(void) {
    s32 index;
    for (index = 0; index < fldValueRecordCount; index++) {
        ((FldValueRecord *)fldValueRecords)[index].value = 0;
    }
    fldValueRecordCount = 0;
    fldAreaState.unkA0 = -1;
    fldAreaState.unkA4 = -1;
    fldAreaState.unkAC = -1;
    D_003BADF8 = 0;
    D_003BADEC = 0;
    if (fldValueRecords != 0) {
        fldReleaseRecordStorage();
    }
}


void fldBuildCollisionRecordsFromGeometry(void *geometry, f32 *translation,
                   u32 id, s32 sceneFlag, s32 ownerIndex) {
    f32 vertices[4][4];
    f32 copiedVertices[4][4];
    /* Inferred four-point capacity from related native workspace maps;
     * this operation populates only its three defining points. */
    f32 edgeTriangle[4][4];
    f32 center[3];
    f32 capturedPlane[4];
    f32 minimum[2];
    f32 maximum[2];
    FldValueRecord *record;
    s32 pass;
    s32 remaining;
    f32 (*sourceVertices)[4];
    void *stopData;
    s32 trianglesRemaining;
    FldCollisionFace *face;
    f32 *planeX;
    f32 *planeY;
    f32 *planeZ;
    f32 *limit0;
    f32 *limit1;
    f32 *limit2;
    f32 *limit3;
    f32 absX;
    f32 absY;
    f32 absZ;
    f32 runningMinimum;
    s32 i;
    s32 component;
    FldCollisionDescriptor *source = geometry;

    if (fldValueRecords == 0) {
        fldAllocateRecordStorage();
    }
    for (pass = 0; pass < 5; pass++) {
        s32 faceCount;
        FldCollisionFace *firstFace;
        sourceVertices = source->vertices;
        faceCount = source->faceCount;
        firstFace = source->faces;
        stopData = source->stopData;
        trianglesRemaining = source->triangleCount;
        if (faceCount > 0) {
            face = firstFace;
            remaining = faceCount;
            do {
                u32 vertexIndex;
                record = &((FldValueRecord *)fldValueRecords)[fldValueRecordCount];
                record->count = trianglesRemaining > 0 ? 3 : 4;
                vertexIndex = face->vertexIndices[0];
                vertices[0][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[0][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[0][2] = sourceVertices[vertexIndex][2] + translation[2];
                vertexIndex = face->vertexIndices[1];
                vertices[1][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[1][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[1][2] = sourceVertices[vertexIndex][2] + translation[2];
                vertexIndex = face->vertexIndices[2];
                vertices[2][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[2][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[2][2] = sourceVertices[vertexIndex][2] + translation[2];
                if (record->count == 3) {
                    for (i = 0; i < 3; i++) {
                        copiedVertices[i][0] = vertices[i][0];
                        copiedVertices[i][1] = vertices[i][1];
                        copiedVertices[i][2] = vertices[i][2];
                    }
                } else {
                    vertexIndex = face->vertexIndices[3];
                    vertices[3][0] = sourceVertices[vertexIndex][0] + translation[0];
                    vertices[3][1] = sourceVertices[vertexIndex][1] + translation[1];
                    vertices[3][2] = sourceVertices[vertexIndex][2] + translation[2];
                    for (i = 0; i < 4; i++) {
                        copiedVertices[i][0] = vertices[i][0];
                        copiedVertices[i][1] = vertices[i][1];
                        copiedVertices[i][2] = vertices[i][2];
                    }
                }
                fldCalcTrianglePlane(copiedVertices[0], &capturedPlane[0],
                                     &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                for (i = 0; i < 3; i++) {
                    record->normal[i] = capturedPlane[i];
                }
                record->normal[3] = 0.0f;
                for (i = 0; i < record->count; i++) {
                    for (component = 0; component < 3; component++) {
                        record->vertices[i][component] = copiedVertices[i][component];
                    }
                    record->vertices[i][3] = 1.0f;
                }
                record->planeConstant = fldDotVector(record->normal, record->vertices[0]);
                absX = record->normal[0];
                absX = ffabsf(absX);
                absY = ffabsf(record->normal[1]);
                absZ = ffabsf(record->normal[2]);
                if (pass == 0) {
                    if (!(absX < absY) || !(absZ < absY)) goto next_face;
                    if (record->normal[1] > 0.0f) goto next_face;
                    if (record->normal[1] > -0.98f) goto next_face;
                } else if (pass == 1) {
                    if (!(absX < absY) || !(absZ < absY)) goto next_face;
                    if (record->normal[1] > 0.0f) goto next_face;
                    if (record->normal[1] <= -0.98f) goto next_face;
                } else if (pass == 2) {
                    if (!(absY <= absX) || !(absZ <= absX)) goto next_face;
                } else if (pass == 3) {
                    if (!(absX < absZ) || !(absY <= absZ)) goto next_face;
                } else if (pass == 4) {
                    if (!(absX <= absY) || !(absZ <= absY)) goto next_face;
                    if (record->normal[1] <= 0.0f) goto next_face;
                }
                if (absY <= absX && absZ <= absX) {
                    record->mode = 0;
                    runningMinimum = vertices[0][1];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->count; i++) {
                        if (vertices[i][1] < runningMinimum) {
                            runningMinimum = vertices[i][1];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][1] > maximum[0]) maximum[0] = vertices[i][1];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->bound[0] = runningMinimum;
                    record->bound[1] = minimum[1];
                    record->bound[2] = maximum[0];
                    record->bound[3] = maximum[1];
                    if (record->count == 3) {
                        center[0] = 0.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3.0f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2]) / 3.0f;
                    } else {
                        center[0] = 0.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1] + vertices[3][1]) * 0.25f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2] + vertices[3][2]) * 0.25f;
                    }
                    edgeTriangle[0][0] = 0.0f;
                    edgeTriangle[0][1] = vertices[0][1];
                    edgeTriangle[0][2] = vertices[0][2];
                    edgeTriangle[1][0] = 0.0f;
                    edgeTriangle[1][1] = vertices[1][1];
                    edgeTriangle[1][2] = vertices[1][2];
                    edgeTriangle[2][0] = 10.0f;
                    edgeTriangle[2][1] = vertices[1][1];
                    edgeTriangle[2][2] = vertices[1][2];
                    planeX = &record->plane[0][0];
                    planeY = &record->plane[0][1];
                    planeZ = &record->plane[0][2];
                    limit0 = &record->limit[0];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    if (fldDotVector(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = 0.0f;
                    edgeTriangle[0][1] = vertices[1][1];
                    edgeTriangle[0][2] = vertices[1][2];
                    edgeTriangle[1][0] = 0.0f;
                    edgeTriangle[1][1] = vertices[2][1];
                    edgeTriangle[1][2] = vertices[2][2];
                    edgeTriangle[2][0] = 10.0f;
                    edgeTriangle[2][1] = vertices[2][1];
                    edgeTriangle[2][2] = vertices[2][2];
                    planeX = &record->plane[1][0];
                    planeY = &record->plane[1][1];
                    planeZ = &record->plane[1][2];
                    limit1 = &record->limit[1];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    if (fldDotVector(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    }
                    if (record->count == 3) {
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[3][1];
                        edgeTriangle[1][2] = vertices[3][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[3][1];
                        edgeTriangle[2][2] = vertices[3][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[3][1];
                        edgeTriangle[0][2] = vertices[3][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[3][0];
                        planeY = &record->plane[3][1];
                        planeZ = &record->plane[3][2];
                        limit3 = &record->limit[3];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else if (absX <= absY && absZ <= absY) {
                    record->mode = 1;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->count; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->bound[0] = runningMinimum;
                    record->bound[1] = minimum[1];
                    record->bound[2] = maximum[0];
                    record->bound[3] = maximum[1];
                    if (record->count == 3) {
                        center[1] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3.0f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2]) / 3.0f;
                    } else {
                        center[1] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0] + vertices[3][0]) * 0.25f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2] + vertices[3][2]) * 0.25f;
                    }
                    edgeTriangle[0][0] = vertices[0][0];
                    edgeTriangle[0][1] = 0.0f;
                    edgeTriangle[0][2] = vertices[0][2];
                    edgeTriangle[1][0] = vertices[1][0];
                    edgeTriangle[1][1] = 0.0f;
                    edgeTriangle[1][2] = vertices[1][2];
                    edgeTriangle[2][0] = vertices[1][0];
                    edgeTriangle[2][1] = 10.0f;
                    edgeTriangle[2][2] = vertices[1][2];
                    planeX = &record->plane[0][0];
                    planeY = &record->plane[0][1];
                    planeZ = &record->plane[0][2];
                    limit0 = &record->limit[0];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    if (fldDotVector(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = vertices[1][0];
                    edgeTriangle[0][1] = 0.0f;
                    edgeTriangle[0][2] = vertices[1][2];
                    edgeTriangle[1][0] = vertices[2][0];
                    edgeTriangle[1][1] = 0.0f;
                    edgeTriangle[1][2] = vertices[2][2];
                    edgeTriangle[2][0] = vertices[2][0];
                    edgeTriangle[2][1] = 10.0f;
                    edgeTriangle[2][2] = vertices[2][2];
                    planeX = &record->plane[1][0];
                    planeY = &record->plane[1][1];
                    planeZ = &record->plane[1][2];
                    limit1 = &record->limit[1];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    if (fldDotVector(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    }
                    if (record->count == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = vertices[3][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[3][2];
                        edgeTriangle[2][0] = vertices[3][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[3][2];
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = vertices[3][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[3][2];
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->plane[3][0];
                        planeY = &record->plane[3][1];
                        planeZ = &record->plane[3][2];
                        limit3 = &record->limit[3];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else {
                    record->mode = 2;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][1];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->count; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][1] < minimum[1]) minimum[1] = vertices[i][1];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][1] > maximum[1]) maximum[1] = vertices[i][1];
                    }
                    record->bound[0] = runningMinimum;
                    record->bound[1] = minimum[1];
                    record->bound[2] = maximum[0];
                    record->bound[3] = maximum[1];
                    if (record->count == 3) {
                        center[2] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3.0f;
                    } else {
                        center[2] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0] + vertices[3][0]) * 0.25f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1] + vertices[3][1]) * 0.25f;
                    }
                    edgeTriangle[0][0] = vertices[0][0];
                    edgeTriangle[0][1] = vertices[0][1];
                    edgeTriangle[0][2] = 0.0f;
                    edgeTriangle[1][0] = vertices[1][0];
                    edgeTriangle[1][1] = vertices[1][1];
                    edgeTriangle[1][2] = 0.0f;
                    edgeTriangle[2][0] = vertices[1][0];
                    edgeTriangle[2][1] = vertices[1][1];
                    edgeTriangle[2][2] = 10.0f;
                    planeX = &record->plane[0][0];
                    planeY = &record->plane[0][1];
                    planeZ = &record->plane[0][2];
                    limit0 = &record->limit[0];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    if (fldDotVector(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldDotVector(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = vertices[1][0];
                    edgeTriangle[0][1] = vertices[1][1];
                    edgeTriangle[0][2] = 0.0f;
                    edgeTriangle[1][0] = vertices[2][0];
                    edgeTriangle[1][1] = vertices[2][1];
                    edgeTriangle[1][2] = 0.0f;
                    edgeTriangle[2][0] = vertices[2][0];
                    edgeTriangle[2][1] = vertices[2][1];
                    edgeTriangle[2][2] = 10.0f;
                    planeX = &record->plane[1][0];
                    planeY = &record->plane[1][1];
                    planeZ = &record->plane[1][2];
                    limit1 = &record->limit[1];
                    fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    if (fldDotVector(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldDotVector(planeX, edgeTriangle[1]);
                    }
                    if (record->count == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[3][0];
                        edgeTriangle[1][1] = vertices[3][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[3][0];
                        edgeTriangle[2][1] = vertices[3][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->plane[2][0];
                        planeY = &record->plane[2][1];
                        planeZ = &record->plane[2][2];
                        limit2 = &record->limit[2];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = vertices[3][0];
                        edgeTriangle[0][1] = vertices[3][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->plane[3][0];
                        planeY = &record->plane[3][1];
                        planeZ = &record->plane[3][2];
                        limit3 = &record->limit[3];
                        fldCalcTrianglePlane(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        if (fldDotVector(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldDotVector(planeX, edgeTriangle[2]);
                        }
                    }
                }
                record->unkC8 = D_003BADF8;
                record->id = id;
                record->value = 0;
                record->face = face;
                record->stopData = stopData;
                record->sceneFlag = sceneFlag;
                record->unkE0 = ownerIndex;
                record->unkCA = 0;
                fldValueRecordCount++;
next_face:
                remaining--;
                face++;
                trianglesRemaining--;
            } while (remaining != 0);
        }
    }
    D_003BADF8++;
}


s32 fldClassifyPositionInZoneWithMargin(f32 *out, s32 mode, s32 count, f32 margin, f32 *pos, FldValueRecord *zone) {
    f32 probe[3];
    f32 planar[2];
    f32 best = margin;
    f32 dist;
    f32 slack;
    s32 inside = 0;
    s32 i;

    if (mode == 0) {
        probe[0] = 0.0f;
        probe[1] = pos[1];
        probe[2] = pos[2];
        planar[0] = pos[1];
        planar[1] = pos[2];
    } else if (mode == 1) {
        probe[0] = pos[0];
        probe[1] = 0.0f;
        probe[2] = pos[2];
        planar[0] = pos[0];
        planar[1] = pos[2];
    } else {
        probe[0] = pos[0];
        probe[1] = pos[1];
        probe[2] = 0.0f;
        planar[0] = pos[0];
        planar[1] = pos[1];
    }
    for (i = 0; i < count; i++) {
        dist = fldDotVector(probe, zone->plane[i]) - zone->limit[i];
        slack = dist + margin;
        if (slack < 0.0f) {
            *out = -1.0f;
            return -1;
        }
        if (planar[0] < zone->bound[0] - margin || planar[1] < zone->bound[1] - margin ||
            zone->bound[2] + margin < planar[0] || zone->bound[3] + margin < planar[1]) {
            *out = -1.0f;
            return -1;
        }
        if (dist < 0.0f) {
            if (mode == 1) {
                best = dist + 45.0f;
            } else {
                best = slack;
            }
            inside = 1;
        }
    }
    if (best < 0.001f) {
        best = -1.0f;
    }
    *out = best;
    return inside;
}

extern f32 D_00330750[3];
extern s32 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);
extern s32 fldTestMapSlotAuxiliaryFlag(s32 mapId, u32 slotIndex, s32 bit);

void fldUpdatePositionZoneHits(f32 *input) {
    f32 position[4];
    f32 previous[4];
    f32 savedPosition[4];
    f32 savedPrevious[4];
    f32 absoluteDelta[4];
    f32 clearance;
    FldValueRecord *record;
    ObjectTransform *transform;
    ObjectTransform *anchor;
    f32 distance;
    s32 relative;
    s32 inside;
    s32 index;
    f32 margin;

    previous[1] = D_00330750[1];
    position[1] = input[1] - 35.0f;
    position[0] = input[0];
    position[2] = input[2];
    previous[0] = D_00330750[0];
    previous[2] = D_00330750[2];
    absoluteDelta[0] = ffabsf(input[0] - D_00330750[0]);
    absoluteDelta[2] = ffabsf(input[2] - D_00330750[2]);
    margin = 45.0f;
    for (index = 0; index < fldValueRecordCount; index++) {
        if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x80)) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                                 ((FldValueRecord *)fldValueRecords)[index].sceneFlag)) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].unkE0 > 0) {
            if (fldTestMapSlotAuxiliaryFlag(fldAreaState.area, fldAreaState.floor + 1,
                                            ((FldValueRecord *)fldValueRecords)[index].unkE0)) {
                continue;
            }
        }
        savedPosition[0] = position[0];
        savedPosition[1] = position[1];
        savedPosition[2] = position[2];
        savedPrevious[0] = previous[0];
        savedPrevious[1] = previous[1];
        savedPrevious[2] = previous[2];
        if (((FldValueRecord *)fldValueRecords)[index].value != 0) {
            transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
            relative = 1;
            position[0] -= transform->position[0];
            position[1] -= transform->position[1];
            position[2] -= transform->position[2];
            previous[0] -= transform->position[0];
            previous[1] -= transform->position[1];
            previous[2] -= transform->position[2];
        } else {
            relative = 0;
        }
        distance = fldDotVector(position, ((FldValueRecord *)fldValueRecords)[index].normal);
        distance -= ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        if (ffabsf(distance) < margin) {
            record = &((FldValueRecord *)fldValueRecords)[index];
            fldClassifyPositionInZoneWithMargin(&clearance, record->mode, record->count, 45.0f, position, record);
            inside = clearance < 0.0f ? 0 : 1;
            if (inside == 1) {
                if (((FldValueRecord *)fldValueRecords)[index].value != 0) {
                    if (fldAreaState.unk90 == -1 ||
                        ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[fldAreaState.unk90].value)->key ==
                            ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->key) {
                        fldAreaState.unk90 = index;
                    } else {
                        fldAreaState.unk94 = index;
                    }
                    anchor = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[0] = anchor->position[0];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[1] = anchor->position[1];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[2] = anchor->position[2];
                }
            }
        }
        if (relative) {
            transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
            position[0] += transform->position[0];
            position[1] += transform->position[1];
            position[2] += transform->position[2];
            previous[0] += transform->position[0];
            previous[1] += transform->position[1];
            previous[2] += transform->position[2];
        }
    }
}

/* Indexed fields retain index-first addresses; see docs/idioms.md,
 * "Trace-justified indexed field addresses: relative zone motion". */
void fldApplyRelativeZoneMotion(f32 *position) {
    f32 newPosition[3];
    f32 previous[3];
    f32 movement[4];
    s32 index;

    memset(movement, 0, sizeof(movement));
    movement[3] = 1.0f;
    fldAreaState.positionPending = 0;
    if (fldAreaState.unk90 == -1) {
        fldUpdatePositionZoneHits(position);
    }
    index = fldAreaState.unk90;
    if (index != -1) {
        if (((FldValueRecord *)fldValueRecords)[index].value != 0) {
            ObjectTransform *transform;

            transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
            movement[0] = transform->position[0] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[0];
            movement[1] = transform->position[1] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[1];
            movement[2] = transform->position[2] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[2];
            ((FldValueRecord *)fldValueRecords)[index].previousPosition[0] = transform->position[0];
            ((FldValueRecord *)fldValueRecords)[index].previousPosition[1] = transform->position[1];
            ((FldValueRecord *)fldValueRecords)[index].previousPosition[2] = transform->position[2];
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                               ((FldValueRecord *)fldValueRecords)[index].sceneFlag) == 0) {
            index = fldAreaState.unk94;
            if (index != -1 && fldTestRoomSceneFlag(fldAreaState.area,
                        fldAreaState.floor + 1,
                        ((FldValueRecord *)fldValueRecords)[index].sceneFlag) == 0) {
                if (((FldValueRecord *)fldValueRecords)[index].value != 0) {
                    ObjectTransform *transform;

                    transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
                    movement[0] += transform->position[0] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[0];
                    movement[1] += transform->position[1] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[1];
                    movement[2] += transform->position[2] - ((FldValueRecord *)fldValueRecords)[index].previousPosition[2];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[0] = transform->position[0];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[1] = transform->position[1];
                    ((FldValueRecord *)fldValueRecords)[index].previousPosition[2] = transform->position[2];
                }
            }
            newPosition[0] = position[0] + movement[0];
            newPosition[1] = position[1] + movement[1];
            newPosition[2] = position[2] + movement[2];
            previous[0] = D_00330750[0];
            previous[1] = D_00330750[1];
            previous[2] = D_00330750[2];
            fldAreaState.positionPending = 1;
            fldAreaState.targetX = newPosition[0];
            fldAreaState.targetY = newPosition[1];
            fldAreaState.targetZ = newPosition[2];
            position[0] = newPosition[0];
            position[1] = newPosition[1];
            position[2] = newPosition[2];
            D_00330750[0] = newPosition[0];
            D_00330750[1] = newPosition[1];
            D_00330750[2] = newPosition[2];
        }
    }
}

/* Non-owning native alias of fldAreaState.unk11C (FldAreaWork + 0x11C). */
extern s32 D_0032E4CC[];

/* Classify flagged zone faces after checking the current movement. */
void func_00136FE8(f32 *position) {
    f32 point[3];
    f32 reference[3];
    f32 distanceVector[3];
    f32 savedPoint[3];
    f32 savedReference[3];
    f32 axisDistance[3];
    f32 margin;
    f32 planeDistance;
    f32 classifiedMargin;
    FldAreaWork *areaState;
    s32 selectedAxis;
    s32 pass;
    s32 recordIndex;

    D_0032E4CC[0] = 0;

    /* Capture both snapshots before lowering the current probe for testing. */
    point[0] = position[0];
    point[1] = position[1];
    point[2] = position[2];
    reference[0] = D_00330750[0];
    reference[1] = D_00330750[1];
    reference[2] = D_00330750[2];
    point[1] -= 35.0f;
    distanceVector[0] = point[0] - reference[0];
    distanceVector[1] = point[1] - reference[1];
    distanceVector[2] = point[2] - reference[2];

    if (fldCalculateVectorLength(distanceVector) < 0.1f) {
        position[0] = reference[0];
        position[1] = reference[1];
        position[2] = reference[2];
        return;
    }

    axisDistance[0] = ffabsf(point[0] - reference[0]);
    axisDistance[2] = ffabsf(point[2] - reference[2]);
    selectedAxis = axisDistance[2] < axisDistance[0] ? 0 : 2;

    fldUpdatePositionZoneHits(position);

    /* Linked-node translations accumulate across records and both passes. */
    for (pass = 0; pass < 2; pass++) {
        recordIndex = 0;
        if (fldValueRecordCount > 0) {
            areaState = &fldAreaState;
            do {
                EffWorldNode *node;

                margin = ((FldValueRecord *)fldValueRecords)[recordIndex].mode == 1 ? 45.0f : 60.0f;
                if (pass == 0) {
                    if (((FldValueRecord *)fldValueRecords)[recordIndex].mode == selectedAxis) {
                        continue;
                    }
                } else if (((FldValueRecord *)fldValueRecords)[recordIndex].mode != selectedAxis) {
                    continue;
                }

                if (fldTestRoomSceneFlag(areaState->area, areaState->floor + 1, ((FldValueRecord *)fldValueRecords)[recordIndex].sceneFlag) != 0) {
                    continue;
                }

                savedPoint[0] = point[0];
                savedPoint[1] = point[1];
                savedPoint[2] = point[2];
                savedReference[0] = reference[0];
                savedReference[1] = reference[1];
                savedReference[2] = reference[2];

                node = (EffWorldNode *)((FldValueRecord *)fldValueRecords)[recordIndex].value;
                if (node != 0) {
                    point[0] = savedPoint[0] - node->inner->position[0];
                    point[1] = savedPoint[1] - node->inner->position[1];
                    point[2] = savedPoint[2] - node->inner->position[2];
                    reference[0] = savedReference[0] - node->inner->position[0];
                    reference[1] = savedReference[1] - node->inner->position[1];
                    reference[2] = savedReference[2] - node->inner->position[2];
                }

                planeDistance = fldDotVector(point, ((FldValueRecord *)fldValueRecords)[recordIndex].normal);
                {
                    u32 faceAttributes = ((FldValueRecord *)fldValueRecords)[recordIndex].face->attributes;
                    planeDistance = ffabsf(planeDistance - ((FldValueRecord *)fldValueRecords)[recordIndex].planeConstant);
                    if ((faceAttributes & 0x4000) == 0) {
                        continue;
                    }
                    {
                        s32 accepted = 0;
                        if (!(planeDistance < margin)) {
                            continue;
                        }
                        fldClassifyPositionInZoneWithMargin(
                            &classifiedMargin,
                            ((FldValueRecord *)fldValueRecords)[recordIndex].mode,
                            ((FldValueRecord *)fldValueRecords)[recordIndex].count,
                            margin, point,
                            &((FldValueRecord *)fldValueRecords)[recordIndex]);
                        if (!(classifiedMargin < 0.0f)) {
                            accepted = 1;
                        }
                        if (accepted == 1) {
                            areaState->unk11C = accepted;
                        }
                    }
                }
            } while (++recordIndex < fldValueRecordCount);
        }
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldRefreshAreaStateFromPosition);

void func_00137E90(void) {
}

void func_00137E98(void) {
}

void func_00137EA0(void) {
}

/* Project away the selected axis and reject points outside expanded bounds.
 * Return the last negative plane's margin-adjusted distance, not a minimum;
 * no negative plane leaves margin unchanged. Reject results below 0.001. */
f32 fldGetPositionZoneClearance(s32 axisMode, s32 planeCount, f32 margin, f32 *position, FldValueRecord *zone) {
    f32 projectedPosition[3];
    f32 planarPosition[2];
    f32 clearance = margin;
    f32 planeDistance;
    f32 marginDistance;
    s32 planeIndex;

    if (axisMode == 0) {
        projectedPosition[0] = 0.0f;
        projectedPosition[1] = position[1];
        projectedPosition[2] = position[2];
        planarPosition[0] = position[1];
        planarPosition[1] = position[2];
    } else if (axisMode == 1) {
        projectedPosition[0] = position[0];
        projectedPosition[1] = 0.0f;
        projectedPosition[2] = position[2];
        planarPosition[0] = position[0];
        planarPosition[1] = position[2];
    } else {
        projectedPosition[0] = position[0];
        projectedPosition[1] = position[1];
        projectedPosition[2] = 0.0f;
        planarPosition[0] = position[0];
        planarPosition[1] = position[1];
    }
    for (planeIndex = 0; planeIndex < planeCount; planeIndex++) {
        planeDistance = fldDotVector(projectedPosition, zone->plane[planeIndex]) - zone->limit[planeIndex];
        marginDistance = planeDistance + margin;
        if (marginDistance < 0.0f || planarPosition[0] < zone->bound[0] - margin || planarPosition[1] < zone->bound[1] - margin ||
            zone->bound[2] + margin < planarPosition[0] || zone->bound[3] + margin < planarPosition[1]) {
            return -1.0f;
        }
        if (planeDistance < 0.0f) {
            clearance = marginDistance;
        }
    }
    if (clearance < 0.001f) {
        clearance = -1.0f;
    }
    return clearance;
}

typedef struct FldStopEntry {
    u32 unk0;
    const u8 *name;
} FldStopEntry;

s32 fldFindFieldSegmentIntersection(f32 *start, f32 *end, f32 *hitPoint) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    ObjectTransform *transform;
    EffWorldNode *secondary;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    s32 found;
    FldStopEntry *entry;
    s32 relative;
    s32 index;

    found = -1;
    secondary = dds3GetWorldSecondaryObject();
    if (!((u32)(fldAreaState.area - 200) < 300)) {
        fldToggleWorldNodeState(0);
    }
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < fldValueRecordCount; index++) {
        if (((FldValueRecord *)fldValueRecords)[index].mode == 1) {
            if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x100)) {
                continue;
            }
        }
        if (((FldValueRecord *)fldValueRecords)[index].face->attributes & 0xA01) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                                 ((FldValueRecord *)fldValueRecords)[index].sceneFlag)) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].value != 0) {
            relative = 1;
            transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
            origin[0] -= transform->position[0];
            origin[1] -= transform->position[1];
            origin[2] -= transform->position[2];
            target[0] -= transform->position[0];
            target[1] -= transform->position[1];
            target[2] -= transform->position[2];
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldDotVector(probeStart, ((FldValueRecord *)fldValueRecords)[index].normal) -
                        ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        endDistance = fldDotVector(probeEnd, ((FldValueRecord *)fldValueRecords)[index].normal) -
                      ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        if ((startDistance < 0.0f && 0.0f <= endDistance) || (endDistance < 0.0f && 0.0f <= startDistance)) {
            startDistance = ffabsf(startDistance);
            endDistance = ffabsf(endDistance);
            total = startDistance + endDistance;
            ratio = startDistance / total;
            anchor[0] = probeStart[0];
            anchor[1] = probeStart[1];
            anchor[2] = probeStart[2];
            delta[0] = probeEnd[0] - probeStart[0];
            delta[1] = probeEnd[1] - probeStart[1];
            delta[2] = probeEnd[2] - probeStart[2];
            probeStart[0] += delta[0] * ratio;
            probeStart[1] += delta[1] * ratio;
            probeStart[2] += delta[2] * ratio;
            clearance = fldGetPositionZoneClearance(((FldValueRecord *)fldValueRecords)[index].mode, ((FldValueRecord *)fldValueRecords)[index].count, 2.0f, probeStart, &((FldValueRecord *)fldValueRecords)[index]);
            if (0.0f <= clearance) {
                if (((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x400) {
                    entry = &((FldStopEntry *)((FldValueRecord *)fldValueRecords)[index].stopData)
                                [((FldValueRecord *)fldValueRecords)[index].face->stop];
                    dds3SetObjectPayloadWord8(dds3FindIndexedObjectChainNodeByName(secondary, 6, entry->name), 4);
                } else {
                found = index;
                ratio = (startDistance + clearance) / total;
                delta[0] = probeEnd[0] - anchor[0];
                delta[1] = probeEnd[1] - anchor[1];
                delta[2] = probeEnd[2] - anchor[2];
                anchor[0] += delta[0] * ratio;
                anchor[1] += delta[1] * ratio;
                anchor[2] += delta[2] * ratio;
                probeStart[0] = anchor[0];
                probeStart[1] = anchor[1];
                probeStart[2] = anchor[2];
                origin[0] = anchor[0];
                origin[1] = anchor[1];
                origin[2] = anchor[2];
                }
            }
        }
        if (relative) {
            transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
            origin[0] += transform->position[0];
            origin[1] += transform->position[1];
            origin[2] += transform->position[2];
            target[0] += transform->position[0];
            target[1] += transform->position[1];
            target[2] += transform->position[2];
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

s32 fldClipSegmentToPositionZones(f32 *start, f32 *end, f32 *hitPoint) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    ObjectTransform *transform;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    s32 found;
    s32 relative;
    s32 index;

    found = -1;
    dds3GetWorldSecondaryObject();
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < fldValueRecordCount; index++) {
        if (((FldValueRecord *)fldValueRecords)[index].mode == 1) {
            if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x100)) {
                continue;
            }
        }
        if (((FldValueRecord *)fldValueRecords)[index].face->attributes & 0xA01) {
            continue;
        }
        if (fldTestRoomSceneFlag(fldAreaState.area, fldAreaState.floor + 1,
                                 ((FldValueRecord *)fldValueRecords)[index].sceneFlag)) {
            continue;
        }
        if (!(((FldValueRecord *)fldValueRecords)[index].face->attributes & 0x8000)) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].face->special[0] != 4) {
            continue;
        }
        if (((FldValueRecord *)fldValueRecords)[index].value != 0) {
            relative = 1;
            transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
            origin[0] -= transform->position[0];
            origin[1] -= transform->position[1];
            origin[2] -= transform->position[2];
            target[0] -= transform->position[0];
            target[1] -= transform->position[1];
            target[2] -= transform->position[2];
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldDotVector(probeStart, ((FldValueRecord *)fldValueRecords)[index].normal) -
                        ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        endDistance = fldDotVector(probeEnd, ((FldValueRecord *)fldValueRecords)[index].normal) -
                      ((FldValueRecord *)fldValueRecords)[index].planeConstant;
        if ((startDistance < 0.0f && 0.0f <= endDistance) || (endDistance < 0.0f && 0.0f <= startDistance)) {
            startDistance = ffabsf(startDistance);
            endDistance = ffabsf(endDistance);
            total = startDistance + endDistance;
            ratio = startDistance / total;
            anchor[0] = probeStart[0];
            anchor[1] = probeStart[1];
            anchor[2] = probeStart[2];
            delta[0] = probeEnd[0] - probeStart[0];
            delta[1] = probeEnd[1] - probeStart[1];
            delta[2] = probeEnd[2] - probeStart[2];
            probeStart[0] += delta[0] * ratio;
            probeStart[1] += delta[1] * ratio;
            probeStart[2] += delta[2] * ratio;
            clearance = fldGetPositionZoneClearance(((FldValueRecord *)fldValueRecords)[index].mode, ((FldValueRecord *)fldValueRecords)[index].count, 2.0f, probeStart, &((FldValueRecord *)fldValueRecords)[index]);
            if (0.0f <= clearance) {
                found = index;
                ratio = (startDistance + clearance) / total;
                delta[0] = probeEnd[0] - anchor[0];
                delta[1] = probeEnd[1] - anchor[1];
                delta[2] = probeEnd[2] - anchor[2];
                anchor[0] += delta[0] * ratio;
                anchor[1] += delta[1] * ratio;
                anchor[2] += delta[2] * ratio;
                probeStart[0] = anchor[0];
                probeStart[1] = anchor[1];
                probeStart[2] = anchor[2];
                origin[0] = anchor[0];
                origin[1] = anchor[1];
                origin[2] = anchor[2];
            }
        }
        if (relative) {
            transform = ((EffWorldNode *)((FldValueRecord *)fldValueRecords)[index].value)->inner;
            origin[0] += transform->position[0];
            origin[1] += transform->position[1];
            origin[2] += transform->position[2];
            target[0] += transform->position[0];
            target[1] += transform->position[1];
            target[2] += transform->position[2];
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

/* Add the four vertical faces of an axis-aligned field collision box. */
void func_00138910(s32 id, f32 x, f32 y, f32 z, f32 extent, f32 height) {
    f32 origin[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 index;
    f32 minX, minY, minZ, maxX, maxZ;

    index = D_003BADEC;
    minX = x - extent;
    maxX = x + extent;
    minY = y - height;
    /* Build the front boundary before the back boundary, matching the
     * native corner ordering. The traced sched1 death notes then place
     * the final Z/extent use on minZ; local allocation keeps Z with maxZ. */
    maxZ = z + extent;
    minZ = z - extent;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.vertexCount = 8;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.faceCount = 4;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.triangleCount = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.vertices = ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.faces = ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry.stopData = NULL;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[0][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[1][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[2][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][1] = minY;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[3][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[4][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][2] = maxZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[5][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][0] = minX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[6][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][0] = maxX;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][1] = y;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][2] = minZ;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].vertices[7][3] = 1.0f;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[1] = 1;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[2] = 5;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].vertexIndices[3] = 4;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[0].special[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[0] = 1;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[1] = 3;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[2] = 7;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].vertexIndices[3] = 5;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[1].special[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[0] = 3;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[1] = 2;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[2] = 6;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].vertexIndices[3] = 7;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[2].special[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].attributes = 128;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].moveFloor = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].sound = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].stop = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].place = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].automap[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].automap[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[0] = 2;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[1] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[2] = 4;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].vertexIndices[3] = 6;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].encounterType = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].encounter = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].special[0] = 0;
    ((FldWallMeshRecord *)fldAuxRecordBuffer)[index].faces[3].special[1] = 0;
    fldBuildCollisionRecordsFromGeometry(&((FldWallMeshRecord *)fldAuxRecordBuffer)[index].geometry,
                  origin, dds3AdvanceWorldCounter(), 0, id);
    D_003BADEC++;
}

void func_00138C28(void) {
}

#include "fld_room_state.h"

typedef struct FldRoomPlanes {
    f32 plane[6][4];
    f32 limit[6];
    u8 pad78[0x140 - 0x78];
} FldRoomPlanes;
extern FldRoomPlanes D_003C9470[];
extern f32 fldDotVector(f32 *, f32 *);
extern FldRoomState fldRoomRecords[];
extern void fldResetActorSlots(void);
extern s32 D_003BAE04;
extern s32 D_003BAE08;
extern s32 D_003BAE0C;
extern s32 D_003BAE10;
extern s32 D_003BAE18;
extern s32 D_003BAE1C;
extern s32 D_003BAE20;
extern s32 D_003BAE2C;
extern s32 D_003BAE34;

/* Reset room records and selection sentinels, then apply actor-slot defaults. */
void fldResetZoneRecordsAndActorSlots(void) {
    s32 roomIndex;
    s32 vectorIndex;

    fldTaskSlotCount = 0;
    D_003BAE28 = 0;
    D_003BAE2C = 0;
    for (roomIndex = 0; roomIndex < FIELD_ROOM_RECORD_COUNT; roomIndex++) {
        for (vectorIndex = 0; vectorIndex < FIELD_ROOM_CORNER_COUNT; vectorIndex++) {
            fldRoomRecords[roomIndex].corner[vectorIndex][0] = 0.0f;
            fldRoomRecords[roomIndex].corner[vectorIndex][1] = 0.0f;
            fldRoomRecords[roomIndex].corner[vectorIndex][2] = 0.0f;
            fldRoomRecords[roomIndex].corner[vectorIndex][3] = 1.0f;
        }
        fldRoomRecords[roomIndex].center[0] = 0.0f;
        fldRoomRecords[roomIndex].center[1] = 0.0f;
        fldRoomRecords[roomIndex].center[2] = 0.0f;
        fldRoomRecords[roomIndex].center[3] = 1.0f;
        for (vectorIndex = 0; vectorIndex < FIELD_ROOM_PLANE_COUNT; vectorIndex++) {
            fldRoomRecords[roomIndex].plane[vectorIndex][0] = 0.0f;
            fldRoomRecords[roomIndex].plane[vectorIndex][1] = 0.0f;
            fldRoomRecords[roomIndex].plane[vectorIndex][2] = 0.0f;
            fldRoomRecords[roomIndex].plane[vectorIndex][3] = 1.0f;
            fldRoomRecords[roomIndex].limit[vectorIndex] = 0.0f;
        }
        D_003308B0[roomIndex] = 0;
        fldRoomRecords[roomIndex].worldObjectKey = 0;
        fldRoomRecords[roomIndex].unk10C = 0.0f;
        fldRoomRecords[roomIndex].unk110 = 0.0f;
        fldRoomRecords[roomIndex].unk114 = 0.0f;
        fldRoomRecords[roomIndex].unk118 = 0.0f;
        fldRoomRecords[roomIndex].unk11C = 0.0f;
        fldRoomRecords[roomIndex].unk130 = 0;
        fldRoomRecords[roomIndex].roomId = -1;
        fldRoomRecords[roomIndex].unk134 = -1;
        fldRoomRecords[roomIndex].mode = 0;
        fldRoomRecords[roomIndex].unk138 = -1;
        fldRoomRecords[roomIndex].unk120 = -1;
        fldRoomRecords[roomIndex].axisMode = 0;
        fldRoomRecords[roomIndex].shapeKind = 0;
    }
    D_003BAE04 = -1;
    D_003BAE08 = -1;
    D_003BAE0C = -1;
    D_003BAE10 = -1;
    D_003BAE18 = -1;
    D_003BAE1C = -1;
    D_003BAE20 = -1;
    D_003BAE34 = 0;
    fldResetActorSlots();
}

extern s32 D_003BAE18;
extern s32 D_003BAE20;
extern s32 D_003BAE2C;
typedef struct FldTriggerSphere {
    u32 kind;
    f32 radius;
} FldTriggerSphere;

typedef struct FldTriggerPlane {
    u32 kind;
    f32 width;
    f32 height;
} FldTriggerPlane;

typedef struct FldTriggerBox {
    u32 kind;
    f32 size[3];
} FldTriggerBox;

/* Serialized kind0/1/2 records occupy8/12/16 bytes. The action constructor
 * embeds only the real8-byte sphere prefix, followed by its task descriptor;
 * shape pointers inspect the member selected by kind, not a padded view. */
typedef union FldTriggerShape {
    u32 kind;
    FldTriggerSphere sphere;
    FldTriggerPlane plane;
    FldTriggerBox box;
} FldTriggerShape;

typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
    u32 unk08;
    FldTriggerShape *shape;
} FldTaskInfo;
extern void *dds3GetWorldObjectPayload(EffWorldNode *object);
/* Clear slot handles and destroy named tasks reached through linked display values. */
void fldResetTaskSlots(void) {
    s32 slotIndex;
    void *world;
    void *task;
    FldTaskInfo *taskInfo;

    D_003BAE2C = 1;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        D_003308B0[slotIndex] = 0;
    }
    D_003BAE18 = -1;
    D_003BAE1C = -1;
    D_003BAE20 = -1;
    world = dds3GetWorldSecondaryObject();
    if (world != 0) {
        for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
            taskInfo = (FldTaskInfo *)D_003307B0[slotIndex]->data;
            if (taskInfo->slot >= 0) {
                task = dds3GetWorldObjectPayload((EffWorldNode *)dds3FindWorldObjectNodeByKey(world, *(u32 *)D_003C92E0[taskInfo->slot], 0xD));
                if (scrFindNamedProcessNode(task) != 0) {
                    evtDestroyNamedTask(dds3GetWorldObject(), task);
                }
            }
        }
    }
}

/* Append a display value and return its index; no capacity check is performed. */
u32 fldPushDisplayValue(u32 value, EffWorldNode *object) {
    u32 index = D_003BAE28;

    D_003C92E0[index] = value;
    D_003BAE28 = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00138ED0);


/* vu0 routine: actor-facing probe for the world kind-0x11 position payload. */
s32 fldTestRoomProbeFacingAndRange(EffWorldNode *actor, EffWorldNode *entry) {
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
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (fldRoomRecords[i].worldObjectKey == entry->key) {
            kind = ((FldTaskInfo *)D_003307B0[i]->data)->shape->kind;
            switch (kind) {
            case 0:
                source = entry->data;
                position[0] = source->position.x;
                position[1] = source->position.y;
                position[2] = source->position.z;
                VU0_LOAD_VF(vf10, actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                VU0_STORE_VF(vf30, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                VU0_NORMALIZE_VF10();
                VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
                VU0_MOVE_VF(vf12, vf10);
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, actor->inner->position);
                VU0_SUB(vf10, vf10, vf11);
                VU0_STORE_VF(vf10, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                length = fldNormalizeProbeVector();
                VU0_MOVE_VF(vf11, vf12);
                VU0_DOT_XYZ(dot, vf10, vf11);
                angle = dot * 180.0f / 3.14f;
                if (angle < 0.0f) {
                    return 0;
                }
                if (200.0f < length) {
                    return 0;
                }
                return 1;
            case 1:
                return fldRoomRecords[i].shapeKind == kind;
            case 2:
                VU0_LOAD_VF(vf10, actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                VU0_STORE_VF(vf30, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF(vf10, dir);
                dot = fldDotVector(dir, fldRoomRecords[i].center);
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
s32 fldTestRoomProbeFacing(EffWorldNode *actor, EffWorldNode *entry) {
    f32 dir[4];
    f32 position[4];
    EvtWorldSourceTransformPrefix *source;
    f32 dot;
    f32 angle;
    s32 i;
    u32 kind;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (fldRoomRecords[i].worldObjectKey == entry->key) {
            kind = ((FldTaskInfo *)D_003307B0[i]->data)->shape->kind;
            switch (kind) {
            case 0:
                source = entry->data;
                PCP_COPY_VECTOR(position, &source->position);
                VU0_LOAD_VF(vf10, actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                VU0_STORE_VF(vf30, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                VU0_NORMALIZE_VF10();
                VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
                VU0_MOVE_VF(vf12, vf10);
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, actor->inner->position);
                VU0_SUB(vf10, vf10, vf11);
                VU0_STORE_VF(vf10, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                fldNormalizeProbeVector();
                VU0_MOVE_VF(vf11, vf12);
                VU0_DOT_XYZ(dot, vf10, vf11);
                angle = dot * 180.0f / 3.14f;
                if (angle < 0.0f) {
                    return 0;
                }
                return 1;
            case 1:
                return fldRoomRecords[i].shapeKind == kind;
            case 2:
                VU0_LOAD_VF(vf10, actor->inner->rotation);
                effMiscQuaternionToMatrixVU();
                VU0_STORE_VF(vf30, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF(vf10, dir);
                dot = fldDotVector(dir, fldRoomRecords[i].center);
                if (dot < 0.5f) {
                    return 0;
                }
                return 1;
            }
        }
    }
    return 0;
}


s32 fldTestActorRoomProbeCondition(s32 index, EffWorldNode *actor, f32 *position) {
    f32 dir[4];
    f32 length;
    f32 dot;
    f32 angle;
    u32 kind;

    kind = ((FldTaskInfo *)D_003307B0[index]->data)->shape->kind;
    switch (kind) {
    case 0:
        VU0_LOAD_VF(vf10, actor->inner->rotation);
        effMiscQuaternionToMatrixVU();
        VU0_STORE_VF(vf30, dir);
        dir[1] = 0.0f;
        VU0_LOAD_VF(vf10, dir);
        VU0_NORMALIZE_VF10();
        VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_LOAD_VF(vf11, actor->inner->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, dir);
        dir[1] = 0.0f;
        VU0_LOAD_VF(vf10, dir);
        length = fldNormalizeProbeVector();
        VU0_MOVE_VF(vf11, vf12);
        VU0_DOT_XYZ(dot, vf10, vf11);
        angle = dot * 180.0f / 3.14f;
        if (angle < 0.0f) {
            return 0;
        }
        if (200.0f < length) {
            return 0;
        }
        return 1;
    case 1:
        return fldRoomRecords[index].shapeKind == kind;
    case 2:
        VU0_LOAD_VF(vf10, actor->inner->rotation);
        effMiscQuaternionToMatrixVU();
        VU0_STORE_VF(vf30, dir);
        dir[1] = 0.0f;
        VU0_LOAD_VF(vf10, dir);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, dir);
        dot = fldDotVector(dir, fldRoomRecords[index].center);
        if (dot < 0.5f) {
            return 0;
        }
        return 1;
    }
    return 0;
}

s32 fldProbeRoomPlanes(f32 *direction, s32 index) {
    f32 probe[3];
    f32 planar[2];
    s32 i;

    if (fldRoomRecords[index].axisMode == 0) {
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
        if (fldDotVector(probe, D_003C9470[index].plane[i + 1]) - D_003C9470[index].limit[i + 1] < 0.0f) {
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
        if (fldDotVector(point, plane) - *limit > 0.0f) {
            return 0;
        }
        plane += 4;
        limit++;
    }
    return 1;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013AFC8);

s32 fldDestroyFlaggedNamedTask(char *flag, u32 slot) {
    D_003308B0[slot] = 0;
    if (scrFindNamedProcessNode(flag) != 0) {
        evtDestroyNamedTask(dds3GetWorldObject(), flag);
        return 0;
    }
    return -1;
}

u32 fldDestroyTaskSlot(u32 slot) {
    u32 *task = &D_003308B0[slot];

    if (kwlnTaskIsRegistered((KwlnTask *)*task) != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)*task, 0);
    }
    *task = 0;
    return 0;
}

s32 fldRestartSceneResourceTask(void) {
    u8 *object;
    u32 state = D_003BAB3C;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && D_0032C9A0[0] != 0) {
        evtDestroyNamedTask(dds3GetWorldObject(), (const char *)D_0032C9A0);
    }
    D_0032C9A0[0] = 0;
    D_003BAB3C = 0;
    object = fldSelectCurrentActorOnNextFloor();
    if (scrFindNamedProcessNode((char *)object) == 0) {
        evtStartSceneResourceTask(dds3GetWorldObject(), (const char *)object);
    }
    return 1;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013B1D8);

void fldDrawTaskMarkers(void) {
    s32 count = fldTaskSlotCount;
    s32 i = 0;
    if (count > 0) {
        FldFileResource **entry = D_003307B0;
        do {
            fldDrawMarkerQuad((*entry)->transform);
            i++;
            entry++;
        } while (i < fldTaskSlotCount);
    }
}

/* Clear unregistered task handles; retain the native global-count loop bound. */
void fldClearInactiveTaskSlots(void) {
    s32 count = fldTaskSlotCount;
    s32 slotIndex = 0;
    if (count > 0) {
        u32 *taskSlot = D_003308B0;
        do {
            if (kwlnTaskIsRegistered((KwlnTask *)*taskSlot) == 0) {
                *taskSlot = 0;
            }
            slotIndex++;
            taskSlot++;
        } while (slotIndex < fldTaskSlotCount);
    }
}

/* Return the first matching task slot's record ID; -1 denotes a miss. */
s32 fldFindTaskRecordId(u32 task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_003308B0[slotIndex] == task) {
            return D_003307B0[slotIndex]->id;
        }
    }
    return -1;
}


extern s32 fldTaskSlotCount;
/* Return the first matching task slot's room ID; -1 denotes a miss. */
s32 fldFindRoomByTask(u32 task) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_003308B0[slotIndex] == task) {
            return fldRoomRecords[slotIndex].roomId;
        }
    }
    return -1;
}


/* Return the first matching task slot's record name; NULL on miss. */
const char *fldGetTaskRecordValue(u32 task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_003308B0[slotIndex] == task) {
            return D_003307B0[slotIndex]->name;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013BF48);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013C138);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013C2B8);

s32 fldCheckEntryActive(s32 entry) {
    s32 entryIdx = D_003BAE1C;
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
s32 fldHasActiveTasks(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_003308B0[slotIndex] != 0) {
            return 1;
        }
    }
    return 0;
}

s32 fldGetCurrentSceneSelectionId(void) {
    s32 slot = D_003BAE3C;

    if (slot < 0) {
        return -1;
    }
    return D_003C9518[slot * 160];
}


extern s32 D_003BAE38;
extern const char D_00332DF0[];
extern s32 strcmp(const char *, const char *);

const char *fldResolveEventSetByNameKindAreaOne(const char *eventName) {
    s32 i;
    s32 j;
    if (eventName == 0) return 0;
    for (i = 0; i < 8; i++) {
        if (D_00332E30.packs[i].setIndex != 0) {
            for (j = 0; j < 5; j++) {
                if (D_00332E30.packs[i].hits[j].area == fldAreaState.floor + 1 &&
                    strcmp(eventName, D_00332E30.packs[i].hits[j].eventName) == 0) {
                    s32 candidateSet = D_00332E30.packs[i].setIndex;
                    if (D_00332E30.sets[D_003BAE38].kindArea.packed == 1) {
                        D_003BAE38 = candidateSet;
                        return D_00332DF0;
                    }
                }
            }
        }
    }
    for (i = 0; i < 40; i++) {
        if (D_00332E30.sets[i].kindArea.packed == 1 &&
            D_00332E30.sets[i].action == fldAreaState.floor + 1 &&
            strcmp(eventName, D_00332E30.sets[i].eventName) == 0) {
            D_003BAE38 = i;
            return D_00332DF0;
        }
    }
    return 0;
}


const char *fldResolveEventSetByNameKindAreaZero(const char *eventName) {
    s32 i;
    s32 j;
    if (eventName == 0) return 0;
    for (i = 0; i < 8; i++) {
        if (D_00332E30.packs[i].setIndex != 0) {
            for (j = 0; j < 5; j++) {
                if (D_00332E30.packs[i].hits[j].area == fldAreaState.floor + 1 &&
                    strcmp(eventName, D_00332E30.packs[i].hits[j].eventName) == 0) {
                    s32 candidateSet = D_00332E30.packs[i].setIndex;
                    if (D_00332E30.sets[D_003BAE38].kindArea.packed == 0) {
                        D_003BAE38 = candidateSet;
                        return D_00332DF0;
                    }
                }
            }
        }
    }
    for (i = 0; i < 40; i++) {
        if (D_00332E30.sets[i].kindArea.packed == 0 &&
            D_00332E30.sets[i].action == fldAreaState.floor + 1 &&
            strcmp(eventName, D_00332E30.sets[i].eventName) == 0) {
            D_003BAE38 = i;
            return D_00332DF0;
        }
    }
    return 0;
}


const char *fldResolveEventSetByNameEitherKindArea(const char *eventName) {
    s32 i;
    s32 j;
    if (eventName == 0) return 0;
    for (i = 0; i < 8; i++) {
        if (D_00332E30.packs[i].setIndex != 0) {
            for (j = 0; j < 5; j++) {
                if (D_00332E30.packs[i].hits[j].area == fldAreaState.floor + 1 &&
                    strcmp(eventName, D_00332E30.packs[i].hits[j].eventName) == 0) {
                    D_003BAE38 = D_00332E30.packs[i].setIndex;
                    return D_00332DF0;
                }
            }
        }
    }
    for (i = 0; i < 40; i++) {
        if ((u16)D_00332E30.sets[i].kindArea.packed < 2 &&
            D_00332E30.sets[i].action == fldAreaState.floor + 1 &&
            strcmp(eventName, D_00332E30.sets[i].eventName) == 0) {
            D_003BAE38 = i;
            return D_00332DF0;
        }
    }
    return 0;
}

extern s32 fldGetActorStat0(s32 attribute);
/* Follow the INF flag/message graph to its terminal event or actor action.
 * Linked selectors recurse in tail position; the native routine loops without
 * growing the stack. Targets and eventHit are signed serialized bytes. */
INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A0138);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A0150);

s32 fldResolveEventSelectorTarget(s32 selector) {
    s32 target;
    s32 index;

    if (selector < 10) {
        index = selector - 1;
        if (D_00332E30.sets[D_003BAE38].flags[index].flag == 0) {
            target = (s8)D_00332E30.sets[D_003BAE38].flags[index].offTarget;
        } else if (mdlFlagTest(D_00332E30.sets[D_003BAE38].flags[index].flag)) {
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
        fldResolveEventSetByNameEitherKindArea(D_003307B0[D_003BAE3C]->name);
        switch ((s8)D_00332E30.sets[D_003BAE38].eventHit) {
        case 0: return 199;
        case 4: return 104;
        case 5: return 105;
        case 6: return 106;
        case 7: return 107;
        }
        fldFindActorEntryByName(D_003307B0[D_003BAE3C]->name);
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

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013CEB0);

s32 fldGetActorSlotAttribute(s32 actorId, s32 attribute) {
    s32 index = actorId - 10;
    s32 value = 0;

    switch (attribute) {
    case 0:
        value = D_00332E30.sets[D_003BAE38].messages[index].kind;
        break;
    case 1:
        value = D_00332E30.sets[D_003BAE38].messages[index].message;
        break;
    case 2:
        if (D_00332E30.sets[D_003BAE38].messages[index].view & 0x80) {
            value = 1;
        }
        break;
    }
    return value;
}



extern s32 sprintf(char *, const char *, ...);

extern char D_003A01F8[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */
void fldLoadInfoTable(s32 field) {
    char path[64];
    char directory[32];
    DevState *command;
    if (field < 200) {
        fldFormatAreaDirectory(directory, field, 1);
        sprintf(path, D_003A01F8, directory, field);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, &D_00332E30, sizeof(D_00332E30));
        sdfDevWaitThenReleaseCommandState(command);
    }
}

void fldCopyInfoTable(const void *source) {
    memcpy(&D_00332E30, source, sizeof(D_00332E30));
}

extern s32 D_003BAE40;
extern u8 *fldFindActorEntryByName(const char *);

void fldStartSelectedActorTask(void) {
    s32 slot;
    char *actor;

    D_003BAE40 = 1;
    slot = D_003BAE3C;
    actor = (char *)fldFindActorEntryByName(D_003307B0[slot]->name);
    if (scrFindNamedProcessNode(actor) != NULL) {
        return;
    }
    D_003308B0[slot] = evtStartSceneResourceTask(dds3GetWorldObject(), actor);
}

extern u8 D_003369B0[];
extern u8 D_003369C0[];
extern u8 D_003369D0[];
extern u8 D_003369E0[];
extern u8 D_00336A00[];
extern u8 D_00336A10[];
extern u8 D_00336A30[];
extern u8 D_00336A40[];
extern u8 D_00336A50[];
extern s32 D_003BAE6C;

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A01F8);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A0200);

u8 *fldPickActorTemplateByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = &fldActorWaypointRows.actors[i];
        flag = entry->requiredFlag;
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && (fldAreaState.transitionMode == 0 || !(entry->flags31 & 4))
            && entry->floor == fldAreaState.floor + 1
            && strcmp(name, entry->name) == 0) {
            switch (entry->kind) {
            case 1:
                if (entry->variantMode == 0) {
                    sub = entry->variant;
                    if (sub != 0) {
                        if (sub == 1) {
                            D_003BAE6C = 6;
                            return D_00336A10;
                        }
                    }
                }
                D_003BAE6C = 0;
                return D_003369B0;
            case 2:
                D_003BAE6C = 1;
                return D_003369C0;
            case 3:
                D_003BAE6C = 2;
                return D_003369D0;
            case 4:
                D_003BAE6C = 3;
                return D_003369E0;
            case 5:
                D_003BAE6C = 5;
                return D_00336A00;
            case 10:
                D_003BAE6C = 8;
                return D_00336A30;
            case 11:
                D_003BAE6C = 9;
                return D_00336A40;
            case 12:
                D_003BAE6C = 10;
                return D_00336A50;
            }
        }
        i++;
    } while (i < 0x100);
    D_003BAE6C = -1;
    return 0;
}

u8 *fldFindActorEntryByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = &fldActorWaypointRows.actors[i];
        flag = entry->requiredFlag;
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && (fldAreaState.transitionMode == 0 || !(entry->flags31 & 4))
            && entry->floor == fldAreaState.floor + 1
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
extern u8 D_00336A30[];
u8 *fldSelectCurrentActorOnNextFloor(void) {
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
        record = fldGetTaskRecordValue((u32)scrGetCurrentContext()->task);
        if (record == 0) {
            return;
        }
        if (fldFindActorEntryByName(record) == 0) {
            return;
        }
    }
    index = fldSelectedActorEntryIndex;
    entry = &fldActorWaypointRows.actors[index];
    kind = entry->kind;
    if (kind == 1) {
        if (entry->floor == D_0032E3C4[0] + 1) {
            fldPlayFieldSeVolumePan(entry->sound);
            fldReleaseActorTasksById(fldActorSlots[index].actorId);
            return;
        }
    } else if (kind == 2) {
        if (entry->floor == fldAreaState.floor + 1) {
            s32 motion = entry->motion;

            fldAreaState.unk174 = fldActorSlots[index].firstValues[0];
            fldAreaState.positionMode = 1;
            if (motion == 1) {
                kwlnFadeInStart(0xC0, 0xC0, 0xC0, 0xF);
                return;
            }
            kwlnFadeInStart(0, 0, 0, 0xF);
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

void fldSelectCameraByActorName(const char *name) {
    FldActorEntry *entry;
    char *entryName;
    EffWorldNode *camera;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldActorWaypointRows.actors[i];
        if (entry->requiredFlag != 0 && mdlFlagTest(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 0) {
            continue;
        }
        entryName = entry->name;
        if (entry->floor == fldAreaState.floor + 1) {
            if (strcmp(name, entryName) == 0) {
                if (entry->variantMode == 2) {
                    return;
                }
                if (entry->variantMode == 0 && entry->linkKind == 3) {
                    camera = (EffWorldNode *)dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(), 4,
                                                                                  entry->linkName);
                    dds3SetWorldCameraObject(dds3GetWorldObject(), camera);
                    fldEnableCameraObjectFlag();
                    return;
                }
            }
        }
    }
}

u8 fldIsSceneStateEight(void) {
    return D_003BAE68 == 8;
}

void fldApplySceneRoomSelection(FldActorEntry *entry) {
    if (entry->selectedRoom != 0) {
        fldAreaState.unk88 = entry->selectedRoom - 1;
    }
    fldAreaState.rowIdx = entry->rowIndex;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013DF60);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldDispatchSceneAction);

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

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013EC68);

extern void fldLoadActorWaypointTable(s32);
extern void fldInitializeLinkedSequence(FieldSequenceRecord *, s32, s32, const char *, s32, s32, const char *);

void fldInitializeActorLinkedSequence(s32 mode, s32 index, FieldSequenceRecord *sequence) {
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
    fldLoadActorWaypointTable(field);
    for (i = 0; i < 0x100; i++) {
        entry = &fldActorWaypointRows.actors[i];
        if (entry->requiredFlag != 0 && !mdlFlagTest(entry->requiredFlag)) {
            continue;
        }
        if (entry->kind != 7 || motion != entry->motion ||
            index != entry->secondaryMotion || entry->variantMode != 0) {
            continue;
        }
        fldAreaState.rowIdx = entry->rowIndex;
        if (entry->flags54 != 0) {
            D_003BAB3C = entry->flags54;
            strcpy((char *)D_0032C9A0, entry->taskName);
            D_003BAB40 = i + 1;
        }
        if (entry->flags31 & 1) {
            fldAreaState.commandEnabled = 1;
        } else {
            fldAreaState.commandEnabled = 0;
        }
        if (entry->variant != 0) {
            field = entry->variant;
        }
        kind = 1;
        if (entry->sequenceKind != 0) {
            kind = entry->sequenceKind;
        }
        if (field != 0 && kind != 0) {
            fldInitializeLinkedSequence(sequence, field, kind, entry->sequenceName,
                                        entry->sequenceCode, entry->linkKind, entry->linkName);
            sequence->mode = 5;
            if (fldAreaState.sequenceCode != entry->sequenceCode && entry->sequenceCode != 0) {
                fldAreaState.sequenceCode = entry->sequenceCode;
            }
            if (entry->selectedRoom != 0) {
                fldAreaState.unk88 = entry->selectedRoom - 1;
            }
        }
        return;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F100);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F340);

/* Read a selected actor state or a named world-object value; cases 1 and 2
 * deliberately fall through when no named object is found. */
s32 fldGetActorStat0(s32 attribute) {
    FldActorEntry *actor = &fldActorWaypointRows.actors[fldSelectedActorEntryIndex];
    EffWorldNode *objectNode;
    s32 secondaryMotion;

    switch (attribute) {
    case 0:
        return actor->motion;
    case 1:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->motionName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 2:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->otherName);
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
INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A0270);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_003A0280);

s32 fldGetActorStat1(u32 attribute) {
    FldActorEntry *actor = &fldActorWaypointRows.actors[fldSelectedActorEntryIndex];
    EffWorldNode *objectNode;

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
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->motionName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 2:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->otherName);
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
    EffWorldNode *objectNode;
    s32 flags;

    switch (attribute) {
    case 0:
        return D_00336A60[motionIndex].defaultMotionId;
    case 1:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00336A60[motionIndex].primaryName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 2:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00336A60[motionIndex].secondaryName);
        if (objectNode != NULL) {
            return objectNode->key;
        }
    case 3:
        D_003BAE44 = D_00336A60[motionIndex].unk24;
        return 0;
    case 4:
        D_003BAE48 = D_00336A60[motionIndex].unk34;
        return 0;
    case 5:
        return D_00336A60[motionIndex].unk44;
    case 6:
        flags = actor->flags64;
        if (flags & 1) {
            return actor->value67;
        }
        return -1;
    }
    return 0;
}

s32 fldGetRowValue(u32 kind) {
    s32 slot = D_003BAE4C;

    switch (kind) {
    case 0:
        return fldActorWaypointRows.headers[slot].count;
    case 1:
        return fldActorWaypointRows.headers[slot].unk4;
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
        return fldActorWaypointRows.headers[slot].count - D_003BAE50 - 1;
    }
    return 0;
}

s32 fldFindTableEntry(s32 index) {
    s32 slot = D_003BAE4C;
    s32 count = fldActorWaypointRows.headers[slot].count;

    if (count - 1 < index) {
        return count - D_003BAE50 - 1;
    }
    index = count - index - 1;
    return fldActorWaypointRows.headers[slot].body.data[index];
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013FA40);

/* Apply native actor-slot defaults; fields not written here remain untouched. */
void fldResetActorSlots(void) {
    s32 actorIndex;

    for (actorIndex = 0; actorIndex < FIELD_ACTOR_SLOT_COUNT; actorIndex++) {
        fldActorSlots[actorIndex].kind = 0;
        fldActorSlots[actorIndex].actorId = 0;
        fldActorSlots[actorIndex].unk08 = 0;
        fldActorSlots[actorIndex].firstKey = -1;
        fldActorSlots[actorIndex].unk10 = 0;
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

void fldLoadActorWaypointTable(s32 field) {
    char path[64];
    char directory[32];
    DevState *command;
    if (field >= 100) {
        memset(&fldActorWaypointRows, 0, 0x6CA0);
    } else {
        fldFormatAreaDirectory(directory, field, 1);
        sprintf(path, "%sF%03d.WAP", directory, field);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, &fldActorWaypointRows, 0x6CA0);
        sdfDevWaitThenReleaseCommandState(command);
    }
}

void fldCopyActorWaypointTable(const void *source) {
    memcpy(&fldActorWaypointRows, source, 0x6CA0);
}

extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);

void fldInitializeActorMotionSlots(void) {
    f32 rotation[4];
    f32 angle;
    EffWorldNode *world;
    s32 firstDirection = 0;
    s32 secondDirection = 0;
    s32 actorIndex;
    s32 slotIndex;
    s32 primaryMotion;
    FldActorEntry *actor;
    FldActorRow *row;
    EffWorldNode *node;

    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;
    world = (EffWorldNode *)dds3GetWorldSecondaryObject();
    for (actorIndex = 0; actorIndex < FIELD_ACTOR_SLOT_COUNT; actorIndex++) {
        actor = &fldActorWaypointRows.actors[actorIndex];
        row = &fldActorSlots[actorIndex];
        if (actor->kind == 2) {
            if (actor->floor == D_0032E3C4[0] + 1) {
                for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
                    if (D_003307B0[slotIndex]->name != NULL &&
                        strcmp(D_003307B0[slotIndex]->name, actor->name) == 0) {
                        FldFileResource *resource = D_003307B0[slotIndex];
                        f32 *transform = resource->transform;
                        row->actorId = resource->id;
                        rotation[0] = transform[4];
                        rotation[1] = transform[5];
                        rotation[2] = transform[6];
                        rotation[3] = transform[7];
                        VU0_LOAD_VF(vf10, rotation);
                        angle = effMiscComputeQuaternionRotatedReferenceAngle() * 180.0f / 3.14f;
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
                node = dds3FindIndexedObjectChainNodeByName(world, 6, (const u8 *)actor->motionName);
                if (node != NULL) {
                    row->firstKey = node->key;
                    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
                        if (D_003307B0[slotIndex]->name != NULL &&
                            strcmp(D_003307B0[slotIndex]->name, actor->name) == 0) {
                            FldFileResource *resource = D_003307B0[slotIndex];
                            f32 *transform = resource->transform;
                            row->actorId = resource->id;
                            rotation[0] = transform[4];
                            rotation[1] = transform[5];
                            rotation[2] = transform[6];
                            rotation[3] = transform[7];
                            VU0_LOAD_VF(vf10, rotation);
                            angle = effMiscComputeQuaternionRotatedReferenceAngle() * 180.0f / 3.14f;
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
                node = dds3FindIndexedObjectChainNodeByName(world, 6, (const u8 *)actor->otherName);
                if (node != NULL) {
                    row->secondKey = node->key;
                    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
                        if (D_003307B0[slotIndex]->name != NULL &&
                            strcmp(D_003307B0[slotIndex]->name, actor->name) == 0) {
                            FldFileResource *resource = D_003307B0[slotIndex];
                            f32 *transform = resource->transform;
                            row->actorId = resource->id;
                            rotation[0] = transform[4];
                            rotation[1] = transform[5];
                            rotation[2] = transform[6];
                            rotation[3] = transform[7];
                            VU0_LOAD_VF(vf10, rotation);
                            angle = effMiscComputeQuaternionRotatedReferenceAngle() * 180.0f / 3.14f;
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
                node = dds3FindIndexedObjectChainNodeByName(world, 6, (const u8 *)actor->taskName);
                if (node != NULL) {
                    row->transitionKey = node->key;
                }
            }
            primaryMotion = actor->motion;
            row->kind = 1;
            row->unk10 = actor->secondaryMotion;
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
                switch (row->unk10) {
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

extern s32 sdfDebugPrintf(const char *, ...);
extern void dds3SetObjectPayloadWord8(EffWorldNode *object, u32 value);

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
                node = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), key, 6);
                if (node != NULL) {
                    sdfDebugPrintf("DOOR SISETU FADE 6\n");
                    dds3SetObjectPayloadWord8(node, 9);
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

extern s32 fldGetCampSceneControlMode(void), fldGetSceneReadyOrPendingState(void), fileMenuTaskExists(void);
extern s32 fldHasKiretaLabelProcess(void), fldHasHirakenaiLabelProcess(void), fldHasBadkaifukuLabelProcess(void);
extern s32 fldIsEventPhaseAtLeastTwo(void);
extern s32 frFontMeasureTextAdvance(void *, s32, s32);
extern void fldDrawGaugeBar(s32);
extern void fldDrawTitleBanner(s32, s32);
extern u8 D_0033E900[];

s32 fldUpdateTitleBannerTask(KwlnTask *task) {
    u16 *ticket;
    u8 *label;
    s32 width;
    s32 x;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (fldGetSceneReadyOrPendingState() != 0) {
        return 0;
    }
    if (fileMenuTaskExists() != 0) {
        return 0;
    }
    if (fldHasKiretaLabelProcess() != 0) {
        return 0;
    }
    if (fldHasHirakenaiLabelProcess() != 0) {
        return 0;
    }
    if (fldHasBadkaifukuLabelProcess() != 0) {
        return 0;
    }
    if (fldIsEventPhaseAtLeastTwo() != 0) {
        return 0;
    }
    ticket = (u16 *)kwlnTaskGetUserValue(task);
    if (ticket[2] != 0) {
        label = D_0033E900 + ticket[1] * 32;
        width = frFontMeasureTextAdvance(label, 1, 0x13);
        x = 0xF6 - (width >> 1);
        fldDrawGaugeBar(width);
        fldDrawTitleBanner(x, 0x131);
        fldDrawConvertedTextGlyph(x + 0x14, 0x98, 0xA09DC380, D_0033E900 + ticket[1] * 32);
        ticket[0] = ticket[0] + 1;
        ticket[2] = 0;
    }
    return 0;
}

s32 fldInitializeTitleBannerTask(KwlnTask *task) {
    u16 *ticket = sdfAllocSizeClassBlock(8);

    ticket[1] = 1;
    ticket[0] = 0;
    ticket[2] = 0;
    ticket[3] = 0;
    kwlnTaskSetUserValue(task, (u32)ticket);
    return (s32)fldUpdateTitleBannerTask;
}

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC10);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC14);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC18);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC1C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC20);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC24);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC28);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC2C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC30);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAreaPackedArchive);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC38);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAreaLoadRequest);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC40);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAreaCachedResource);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC4C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomResourceAllocation);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF1ResourceAllocation);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF2ResourceAllocation);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomKF2ResourceAllocation);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomResourceData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF1ResourceData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF2ResourceData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomKF2ResourceData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF1ResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF2ResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomKF2ResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC80);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldPendingArea);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldPendingFloor);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC90);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAC98);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACA0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACA8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACB0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACB8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACC0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldDisplayRow);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACD4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldBackgroundBuffer);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACE0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACE8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACEC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldMarkerTexture);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACF8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BACFC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldEncounterRuntimeState);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldEncounterTaskName);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD10);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD14);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD18);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD1C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD20);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD24);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD28);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD2C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD30);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD34);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD38);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD3C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD40);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD44);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD48);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD4C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD50);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD54);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD58);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSkyLightSetBuffer);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD60);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraSettings);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraColorEffect);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraColorEnabled);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldRainTextureReference);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD74);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSkyDrawState);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSwayMode);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSwayPhase);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSwayOffset);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD88);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD8C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD90);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD94);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD98);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAD9C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADA0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADA4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADA8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADAC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADB0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADB4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADB8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADBC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADC0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADC4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADC8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADCC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADD0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADD4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADD8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADDC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADE0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADE4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAuxRecordBuffer);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADEC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldValueRecords);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldValueRecordCount);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BADF8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAuxRecordResource);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldValueRecordResource);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE04);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE08);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE0C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE10);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldTaskSlotCount);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE18);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE1C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE20);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE24);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE28);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE2C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE30);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE34);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE38);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE3C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE40);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE44);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE48);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE4C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE50);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE54);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE58);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE5C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE60);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSelectedActorEntryIndex);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE68);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_003BAE6C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldFieldTaskHandle);

