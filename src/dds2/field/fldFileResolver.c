#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "sdf_gs_header.h"
#include "sdf_pac_reloc.h"
#include "fld_script_resource.h"
#include "dds3_path.h"
#include "sdf_packet_header.h"
#include "eff.h"
#include "fld_player_scene_state.h"
#include "fld_marker_packet.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "fld_camera_model_motion_request.h"
#include "fld_placement_batch_mode.h"
#include "sdf_vu0.h"
#include "sdf_resource_list_release_api.h"
#include "kwln_background_fade_api.h"
#include "fld_movement_hold_state_api.h"
#include "fld_scene_ready_api.h"
#include "fld_area_resource_request_reuse_api.h"
#include "fld_scene_record_api.h"
#include "fld_scene_object_key_result.h"
#include "fld_light_set.h"
#include "fld_sky_buffer.h"
#include "fld_sky_gradient_record.h"
#include "fld_quad_state.h"
#include "kwln_frame_draw_buffer.h"
#include "kwln_draw_buffer.h"
#include "mnu_title_stream.h"
#include "fld_bill_config.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "fld_resource_resolver.h"
#include "fld_clear_menu_entries_api.h"
#include "fld_play_pending_sounds_api.h"
#include "fld_release_texture_slots_api.h"
#include "fld_transfer_chunk.h"
#include "fld_camera_follow_row.h"
#include "fld_named_camera_table.h"
#include "fld.h"
#include "fld_encounter_runtime_api.h"
#include "fld_scene_request.h"
#include "fld_room_resource_api.h"
#include "fld_spawn_event.h"
#include "dds3obj.h"
#include "dds3_world_value.h"
#include "dds3_object_flags.h"
#include "sdf_texture_file.h"
#include "sdf_texture_primary_buffer_api.h"
#include "evt_action_object.h"
#include "sdf_texture_offset_list.h"
#include "evt_script_model.h"
#include "sdf_model.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "kwln_fade.h"
#include "sdf_vu_matrix.h"
#include "btl.h"
#include "itf.h"
/* Declared before first use (were implicit int). */
extern void btlResetAsyncState(void);
extern void fldSetFileResolverIdx(u32 state);
extern s32 fldSetEncounterMode(s32 encounterMode);



extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
#include "dds3_world_secondary.h"
struct dds3Basic_t;
extern struct dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation);
extern char D_00412FF0[]; 
struct dds3Basic_t *fldMakeDmyMatter(void) {
    f32 args[8];
    struct dds3Basic_t *matter;
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = 0;
    args[5] = 0;
    args[6] = 0;
    args[7] = 0;
    matter = dds3AddMatterObjectBasic(dds3AllocObjectId(), args, args + 4);
    dds3SetUnitObjectId(matter, D_00412FF0);
    return matter;
}

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 resourceId) {
    u32 resourceIndex = 0;
    dds3Basic_t *secondaryWorld = dds3GetControlScene();
    FldFileResource *resource = D_00438EC0;

    for (; resourceIndex < D_00438EC4; resourceIndex++, resource++) {
        if (resourceId == resource->id) {
            if (resource->names != NULL) {
                FldFileNameEntry *nameEntry = resource->names->entries;
                u32 nameIndex = 0;

                for (; nameIndex < resource->names->count; nameIndex++, nameEntry++) {
                    dds3Basic_t *resolvedObject = dds3SceneBasicSearchNameAndType(secondaryWorld, 6, (const u8 *)nameEntry->name);
                    if (resolvedObject != NULL) {
                        return resolvedObject;
                    }
                }
            } else {
                dds3Basic_t *resolvedObject = fldFindWorldNodeForRecordId(resource->id);
                if (resolvedObject == NULL) {
                    resolvedObject = fldMakeDmyMatter();
                    fldBindWorldNodeToRecordId(resource->id, resolvedObject);
                }
                return resolvedObject;
            }
        }
    }
    return NULL;
}

dds3Basic_t *fldGetModelRange_From_MatterName(const char *resourceEntryName) {
    u32 resourceIndex = 0;
    dds3Basic_t *secondaryWorld = dds3GetControlScene();
    FldFileResource *resource = D_00438EC0;

    for (; resourceIndex < D_00438EC4; resourceIndex++, resource++) {
        if (resource->names != NULL) {
            FldFileNameEntry *nameEntry = resource->names->entries;
            u32 nameIndex = 0;

            for (; nameIndex < resource->names->count; nameIndex++, nameEntry++) {
                if (strcmp(resourceEntryName, nameEntry->name) == 0) {
                    dds3Basic_t *resolvedObject = dds3SceneBasicSearchNameAndType(secondaryWorld, 11, (const u8 *)resource->name);
                    if (resolvedObject != NULL) {
                        return resolvedObject;
                    }
                }
            }
        }
    }
    return NULL;
}

#include "eff_light.h"
#include "fld_area_work.h"
#include "fld_npc_palette.h"
#include "fld_collision_geometry.h"
#include "fld_spawn_collision.h"

struct EffNodeDescriptor;


/* Complete serialized descriptors shared with their existing consumers. */

typedef char FldSpawnBatchSize[(sizeof(FldSpawnBatch) == 12) ? 1 : -1];

extern FldFileResource *D_00438EB8;
extern u32 D_00438EBC;
extern u32 D_00444920[];
extern struct EffNodeDescriptor *D_00444940[];
extern char D_00435FD0[];
extern dds3Basic_t *effObjCreateResourceKindZero(const char *, void *, s32);
extern dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *, void *, s32);
extern dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *, void *, s32);
extern dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *, void *, s32);
extern dds3Basic_t *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *, void *, void *);
extern dds3Basic_t *dds3AddESPEffectObjectBasic(const char *, void *, void *, f32);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *, u32);
extern void dds3EffectObjectBasicStatOff(dds3Basic_t *, u32);
extern void dds3EffectObjectBasicSetFldEx(dds3Basic_t *, BillConfig *);
extern dds3Basic_t *dds3AddLightObjectBasic(s32);
extern dds3Basic_t *dds3AddPathBasic(s32 key, Dds3PathCurveTable *work, const char *name);
extern dds3Basic_t *dds3AddModelRangeBasicNew(s32, void *, const f32 *, const char *);
extern void dds3GetRadiusPoseSet(dds3Basic_t *, f32);
extern void dds3SetLightObjectResoruce(dds3Basic_t *, WorldTransformSetup *);
extern void fldMakePlane(const FldCollisionDescriptor *geometry, const f32 *translation,
                                               u32 id, s32 sceneFlag, s32 ownerIndex);
extern s32 fldCommand_GetHojiID(const char *);
extern void dds3SetPathBasicSpline(dds3Basic_t *path, u32 value);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00412FF0);

void fldMakeResouceBasicFL1(FldSpawnBatch *batch, u32 batchCount) {
    f32 position[4];
    f32 rotation[4];
    char path[64];
    char directory[32];
    s8 loaded[4];
    void *cached[4];
    u32 cacheCount = sizeof(loaded);
    u32 batchIndex;
    u32 resourceCount;
    u32 resourceIndex;
    FldFileResource *resource;
    dds3Basic_t *spawnedObject;
    u32 pathIndex;

    memset(loaded, 0, cacheCount);
    memset(cached, 0, cacheCount * sizeof(*cached));
    D_00438EB8 = NULL;
    D_00438EBC = 0;
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, batch++) {
        resource = batch->entries;
        resourceCount = batch->count;
        switch (batch->kind) {
        case 0:
        case 1:
            break;
        case FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE:
            D_00438EB8 = resource;
            D_00438EBC = resourceCount;
            break;
        case FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                BillConfig *config;
                /* The resolver stores these borrowed string addresses as 32-bit words. */
                const char * const *resourcePathSuffixes;
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
                    resourcePathSuffixes = (const char * const *)D_00444920;
                    cacheByteOffset = config->resourceIndex * sizeof(*cached);
                    if (loaded[config->resourceIndex] == 0) {
                        fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor);
                        sprintf(path, D_00435FD0, directory,
                                     resourcePathSuffixes[((BillConfig *)resource->data)->resourceIndex]);
                        spawnedObject = effObjCreateResourceKindZero(path, position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                        loaded[((BillConfig *)resource->data)->resourceIndex] = 1;
                        cached[((BillConfig *)resource->data)->resourceIndex] = spawnedObject;
                    } else {
                        spawnedObject = dds3AddTMXEffectObjectBasic3(*(void **)((u8 *)&cached + cacheByteOffset), position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    }
                    break;
                case 1:
                    resourcePathSuffixes = (const char * const *)D_00444920;
                    cacheByteOffset = config->resourceIndex * sizeof(*cached);
                    if (loaded[config->resourceIndex] == 0) {
                        fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor);
                        sprintf(path, D_00435FD0, directory,
                                     resourcePathSuffixes[((BillConfig *)resource->data)->resourceIndex]);
                        spawnedObject = dds3AddP2AEffectObjectBasic(path, position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        loaded[((BillConfig *)resource->data)->resourceIndex] = 1;
                        cached[((BillConfig *)resource->data)->resourceIndex] = spawnedObject;
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    } else {
                        spawnedObject = dds3AddP2AEffectObjectBasic3(*(void **)((u8 *)&cached + cacheByteOffset), position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    }
                    break;
                case 2:
                    if (D_00444940[config->resourceIndex] != NULL) {
                        spawnedObject = effObjSpawnDescriptorBoundEffect(D_00444940[config->resourceIndex], position, rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    }
                    break;
                case 100:
                    spawnedObject = dds3AddESPEffectObjectBasic(resource->name, position, rotation, config->width * 0.5f);
                    dds3EffectObjectBasicStatOff(spawnedObject, 1);
                    break;
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_LIGHT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
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
                spawnedObject = dds3AddLightObjectBasic(resource->id);
                dds3SetUnitObjectId(spawnedObject, resource->name);
                dds3SetPosPoseSet(spawnedObject, (u128 *)position);
                /* Type-12 serialization uses this shared slot for outer radius. */
                dds3GetRadiusPoseSet(spawnedObject, ((WorldTransformSetup *)resource->data)->transform.rotation[1]);
                dds3SetLightObjectResoruce(spawnedObject, resource->data);
                if ((u32)fldGlobalBase.area - 200U < 300U) {
                    ((EffLightData *)spawnedObject->data)->flags |= 8;
                    dds3SetUnitObjectId(spawnedObject, "BTL_DEF_LIGHT");
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_PATH:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                dds3AddPathBasic(resource->id, resource->data, resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_COLLISION:
            for (resourceIndex = 0, pathIndex = 1; resourceIndex < resourceCount; resourceIndex++, resource++, pathIndex++) {
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
                    s32 room = fldCommand_GetHojiID(resource->name);
                    fldMakePlane(((FldSpawnCollision *)resource->data)->header, position,
                                  resource->id, room, -1);
                    spawnedObject = dds3AddModelRangeBasicNew(resource->id, resource->data, position, resource->name);
                    dds3SetPathBasicSpline(spawnedObject, pathIndex);
                }
            }
            break;
        }
    }
}


#include "fld_area_work.h"
#include "evt_world.h"
#include "evt_world_object_api.h"
#include "evt_world_source_transform.h"
#include "common.h"
#include "sdf_column_packet_api.h"
#include "sdf_packet_append.h"
#include "sdf_packet_builders.h"
#include "fr_font.h"
#include "sdf_packet_list.h"
#include "sdf_dev_state.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "fld_inf.h"
#include "sdf_primitive_packet_api.h"
#include "dds3obj.h"
#include "fld.h"
#include "fld_waypoint.h"
#include "file_request_api.h"
#include "file_pac.h"
#include "fld_packed_resource_kind.h"
#include "field_stage.h"
#include "sdf_pac_work.h"

#include "fpu.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "dat_state.h"
#include "mdl.h"
#include "kwln_task_lifecycle.h"

enum {
    FIELD_CAMERA_SETTING_COUNT = 8
};

typedef struct SdfDrawPacket SdfDrawPacket;
typedef struct DmaPacketHeader DmaPacketHeader;
/* Packet addresses are 32-bit handles; GS and GIF payload words remain 64-bit. */

typedef struct FldUnitLightParams {
    f32 color[4];
    f32 direction[4];
    f32 unknown[8];
} FldUnitLightParams;

typedef struct FldUnitLightColor {
    f32 color[4];
} FldUnitLightColor;

typedef char FldUnitLightParams_size_must_be_0x40[(sizeof(FldUnitLightParams) == 0x40) ? 1 : -1];
typedef char FldUnitLightColor_size_must_be_0x10[(sizeof(FldUnitLightColor) == 0x10) ? 1 : -1];

extern FldUnitLightParams D_0038BB10;
extern FldUnitLightColor D_0038BB60;

extern dds3ModelHandle_t *fldPlayerModelContext;
extern s32 D_00389888[];

extern void fldFreeNextFile(void);

extern dds3ModelHandle_t *fldSecondarySceneModelContext;

extern dds3Basic_t *fldSecondarySceneObject;



#include "sdf_packet_alloc.h"


extern u32 fldPendingArea;

extern u32 fldPendingFloor;

extern u32 gFldFileResetMode;

extern u32 rsetFrameMaskOtOffset;

extern void *fldDrwZBuf;

extern u32 D_00436088;

#include "dds3_world_primary.h"




extern s32 D_004360AC;

extern FldCameraModelMotionRequest D_004360C4;

extern s32 D_004360C8;

extern s32 D_004360CC;
extern s32 D_004360D4;

extern s32 D_004360D0;

extern struct dds3Basic_t *fldPlayerObject;

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern sdfTexHandle_t *fldRainTextureReference;

extern struct sdfMemHandle_t *fldRainTextureResource;

extern u32 gFilterAnime;

extern u32 gFilterAlpha;

extern FilePacRequest *pFldNextFileDataF2;


extern u8 D_00435BB4;

extern u32 D_003897E8[];

extern char D_00444950[];


extern struct sdfMemHandle_t *fldFieldPackData;

extern struct FileRequest *gFldFileF1;

extern u8 D_00436020[];



extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];


extern u32 D_00389904[];

extern u32 D_00389910[];

extern u32 D_003899C0[];

extern void fldCreatePlayerObject(void);

extern void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatches, u32 batchCount,
                                                FldPlacementBatchMode mode);

extern u32 D_00436064;

extern u32 D_0043607C;

extern sdfTexHandle_t *fldMarkerTexture;

extern u32 D_00438EC8;

extern u8 D_0038A700[];

extern sdfMat_t *sdfMatNew(void);
extern f32 D_004360B0;

extern f32 D_00444980[];

extern f32 D_00444970[];

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                      f32 *targetPositionOut);

extern f32 sdfAtan2(f32 y, f32 x);

extern f32 fldLookAtNearPoint[];

extern f32 fldCamTarget[];

extern u32 D_00444920[];
extern struct EffNodeDescriptor *D_00444940[];
extern struct sdfMemHandle_t *D_00444930[];
extern char D_00435FD0[];
extern void fldMakeResouceBasicFL1(FldSpawnBatch *, u32);

extern f32 D_003897DC[];



extern char D_004130D8[]; 








extern const u8 *fldCachedRoomResourceData, *fldCachedRoomF1ResourceData, *fldCachedRoomF2ResourceData,
    *fldCachedRoomKF2ResourceData;

extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;

extern void *sdf2DMakeStdPacket(void *, s32);

extern SdfPoolNode sdfGrOtTable[];


extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);



extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);


extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);


extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];

extern void sdfBuildMaskedFrameRegisterReference(const u8 *, SdfDmaReferenceChainPacket *, s32);


extern void sdfBuildFrameRegisterReference(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern f32 D_0038A980[][4];

extern u32 D_0038A9A0[];







extern sdfGrFrameSet_t D_0040B290;



extern SdfPoolNode kwlnPositionedTextSurface;



extern void InversMatrix(void);

extern SdfPoolNode D_00380708;

extern u8 D_00436070[];

extern u8 D_00436078[];

extern s32 fldEncounterRuntimeState;

extern u32 D_0043608C;

extern s8 btlGetRuntimeResult(void);

extern s32 fldEncProc(dds3ProcessID_t *task);

extern s32 btlUpdateRuntimeFadeState(void);



extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;


extern s32 D_00389780[];

extern s32 D_004360E8;

extern s32 D_00436118, D_0043611C;

extern FldSkyGradientRecord *D_00436104;

extern f32 D_00436120, D_00436124;

extern s32 D_00389780[];

extern s32 D_004360E8;




extern u32 D_0043612C;

extern f32 D_00436130;

extern u32 D_00436134;

extern u32 D_003899B4[];

extern void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);
extern char D_00413388[];

extern u32 D_00436128;

extern u32 D_00436158;

extern s16 D_00389898[];

extern u8 D_0037F650[];

extern u8 D_0037F9B0[];

extern u8 D_0037F9F0[];

extern u8 D_0037FA00[];

extern u8 D_00384790[];


extern void sdf3DMakeDirectRefPacket(DmaPacketHeader *, u32, s32);



extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);

extern void sdfBillBoardMake1(u8 *, const f32 *, u32, const BillTextureQuad *, const f32 *);

extern s32 D_00435F30;

extern void fldHitChkUnitElv(f32 *);







extern void btlActivateRuntime(s32 mode);


extern char fldEncounterTaskName[];

extern void btlClearRuntimeState(void);


extern void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames);

extern s16 fldGetLocationCoordinateFlags(s32 area, s32 locationIndex);

extern FldCamRow fldCameraFollowRows[];

extern f32 D_0038BAF0[];

extern f32 D_0038BB00[];




extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);

extern void fldToggleWorldNodeState(s64);

extern void fldProbeCameraSupportSurface(void);

/* Relocated collision headers contain their eight-word geometry header. */

#include "fld_placement_data.h"

/* A relocated batch consists of a kind, count, and complete resource rows. */

extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
extern s32 fldRoomEventPlacementCount;
extern s16 D_00389874[];
extern s32 fldCommand_GetHojiID(const char *);
extern void fldMakePlane(const FldCollisionDescriptor *geometry, const f32 *translation,
                                               u32 id, s32 sceneFlag, s32 ownerIndex);
extern dds3Basic_t *dds3AddModelRangeBasicNew(s32, void *, const f32 *, const char *);
extern void dds3SetPathBasicSpline(dds3Basic_t *path, u32 value);
extern dds3Basic_t *dds3AddEventBasic(s32 key, const char *resourcePath, const char *name);
extern dds3Basic_t *dds3AddPathBasic(s32 key, Dds3PathCurveTable *work, const char *name);
#include "fld_event_display_resource_api.h"

extern void func_0014D380(s32 mode, u32 pointId, f32 *rotation,
                            f32 x, f32 y, f32 z);
extern void func_0014C2B8(u32, f32 *, f32, f32, f32, s32, const char *, s32);
extern void func_0014C9B0(u32, f32 *, f32, f32, f32);
extern s32 fldSetSparkVectors(s32 index, const u128 *position, const u128 *rotation);
extern void fldAppendPendingSoundForScene(s32 soundId, f32 radius, f32 sourceX, f32 sourceY, f32 sourceZ);
extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32, u128 *, u128 *);

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

void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatch, u32 batchCount, FldPlacementBatchMode mode) {
    f32 position[4];
    f32 rotation[4];
    const f32 *transform;
    FldFileResource *spawnResource;
    u32 entryCount;
    u32 resourceIndex;
    u32 batchIndex;
    u32 pathState;
    FldSpawnEvent *event;
    u32 *parameters;
    FldSpecialPoint *point;
    FldPendingSoundParams *pendingSound;
    dds3Basic_t *spawnedObject;
    s32 room;
    u32 id;

    if (mode == FLD_PLACEMENT_BATCH_MODE_INITIAL) {
        D_00438EC0 = NULL;
        D_00438EC4 = 0;
    }
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, spawnBatch++) {
        spawnResource = spawnBatch->entries;
        entryCount = spawnBatch->count;
        switch (spawnBatch->kind) {
        case 0:
        case 1:
        case FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE:
        case 5:
        case 8:
        case FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT:
            break;
        case FLD_SPAWN_BATCH_KIND_COLLISION:
            if (mode != FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                break;
            }
            D_00438EC0 = spawnResource;
            D_00438EC4 = entryCount;
            pathState = 1;
            for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++, spawnResource++, pathState++) {
                spawnResource->id |= 0x10000;
                fldCopySpawnTransform(position, rotation, spawnResource->transform);
                if (((FldSpawnCollision *)spawnResource->data)->kind == 0) {
                    room = fldCommand_GetHojiID(spawnResource->name);
                    fldMakePlane(((FldSpawnCollision *)spawnResource->data)->header,
                                 position, spawnResource->id, room, -1);
                    spawnedObject = dds3AddModelRangeBasicNew(spawnResource->id, spawnResource->data,
                                              position, spawnResource->name);
                    dds3SetPathBasicSpline(spawnedObject, pathState);
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_EVENT:
            for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++, spawnResource++) {
                if (mode != FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                    spawnResource->id |= 0x800000;
                } else {
                    fldRoomEventPlacementCount++;
                }
                event = spawnResource->data;
                spawnedObject = dds3AddEventBasic(spawnResource->id, event->label, spawnResource->name);
                fldEveHit_AddEveData(spawnResource, spawnedObject);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_PATH:
            for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++, spawnResource++) {
                if (mode != FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                    spawnResource->id |= 0x800000;
                }
                dds3AddPathBasic(spawnResource->id, spawnResource->data, spawnResource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_PLACEMENT_DATA:
            for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++, spawnResource++) {
                if (mode != FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                    spawnResource->id |= 0x800000;
                }
                transform = spawnResource->transform;
                fldCopySpawnTransform(position, rotation, transform);
                switch (((FldPlacementData *)spawnResource->data)->kind) {
                case FLD_PLACEMENT_KIND_SPECIAL_POINT:
                    point = ((FldPlacementData *)spawnResource->data)->payload.specialPoint;
                    switch (point->kind) {
                    case 1:
                        func_0014D380(0, point->id, rotation, position[0], position[1], position[2]);
                        if (mode == FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                            fldRoomEventPlacementCount++;
                        }
                        break;
                    case 2:
                        func_0014D380(1, point->id, rotation, position[0], position[1], position[2]);
                        if (mode == FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                            fldRoomEventPlacementCount++;
                        }
                        break;
                    case 3:
                        fldSetSparkVectors(point->id, (const u128 *)position, (const u128 *)rotation);
                        break;
                    }
                    break;
                case FLD_PLACEMENT_KIND_POINT:
                    dds3AddPointBasic(spawnResource->id, (EvtWorldSourceTransformPrefix *)transform, spawnResource->name);
                    break;
                case FLD_PLACEMENT_KIND_REGISTERED_POINT:
                    spawnedObject = dds3AddPointBasic(spawnResource->id, (EvtWorldSourceTransformPrefix *)transform, spawnResource->name);
                    fldRegisterRoomObject(spawnResource, spawnedObject);
                    break;
                case FLD_PLACEMENT_KIND_EVENT_PLACEMENT:
                    if (D_00389874[0] != 0) {
                        break;
                    }
                    parameters = ((FldPlacementData *)spawnResource->data)->payload.words;
                    id = parameters[0];
                    room = fldCommand_GetHojiID(spawnResource->name);
                    if (mode != FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                        FldPlacementData *placement = spawnResource->data;
                        func_0014C2B8(id, rotation, position[0], position[1], position[2],
                                      room, spawnResource->name, placement->event + fldRoomEventPlacementCount);
                    } else {
                        FldPlacementData *placement = spawnResource->data;
                        func_0014C2B8(id, rotation, position[0], position[1], position[2],
                                      room, spawnResource->name, placement->event);
                    }
                    break;
                case FLD_PLACEMENT_KIND_WORD_ARRAY_PLACEMENT:
                    parameters = ((FldPlacementData *)spawnResource->data)->payload.words;
                    func_0014C9B0(parameters[0], rotation, position[0], position[1], position[2]);
                    if (mode == FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                        fldRoomEventPlacementCount++;
                    }
                    break;
                case FLD_PLACEMENT_KIND_PENDING_SOUND:
                    pendingSound = ((FldPlacementData *)spawnResource->data)->payload.pendingSound;
                    fldAppendPendingSoundForScene(pendingSound->value, pendingSound->scalar,
                                                  position[0], position[1], position[2]);
                    break;
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_CAMERA:
            for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++, spawnResource++) {
                if (mode != FLD_PLACEMENT_BATCH_MODE_INITIAL) {
                    spawnResource->id |= 0x800000;
                }
                fldCopySpawnTransform(position, rotation, spawnResource->transform);
                spawnedObject = dds3AddCameraObjectBasicTypeFree(spawnResource->id, (u128 *)position, (u128 *)rotation);
                ((CameraData *)spawnedObject->data)->fieldOfView = *(f32 *)spawnResource->data;
                dds3SetUnitObjectId(spawnedObject, spawnResource->name);
            }
            break;
        case 7:
            for (;;) {
            }
            break;
        }
    }
}
void fldSpawnActionObjects(FldActionSpawn *spawnList, u32 spawnCount) {
    u32 spawnIndex;
    struct dds3Basic_t *spawnedObject;

    dds3GetControlScene();
    for (spawnIndex = 0; spawnIndex < spawnCount; spawnIndex++, spawnList++) {
        spawnedObject = dds3AddAreaBasic(spawnList->key, spawnList->objectName);
        if (spawnIndex == 0) {
            fldGlobalBase.taskHandle = spawnedObject;
        }
    }
}

#include "fld_resource_name.h"
#include "fld_file_resource_link_view.h"

extern FldFileResource *D_00438EB8;
extern u32 D_00438EBC;
extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
extern struct DevRequest *fldRoomSoundResources;
struct dds3Basic_t;
extern struct dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation);
extern void dds3MatterObjectBasicSetOt(struct dds3Basic_t *, s32);
extern s32 fldCommand_GetHojiID(const char *);
extern void dds3MatterObjectBasicSetHojiID(struct dds3Basic_t *, u32);
extern void dds3ObjectBaseMakeMatter(struct dds3Basic_t *);
extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *);

void fldCreateResourceScriptObjects(void) {
    f32 position[4];
    f32 rotation[4];
    FldFileResource *resource = D_00438EB8;
    u32 resourceCount = D_00438EBC;
    dds3Basic_t *secondaryWorld;
    struct dds3Basic_t *scriptObject;
    FldFileResource *bindingResource;
    FldFileNameEntry *nameEntry;
    FldScriptResource *scriptResource;
    const FldResourceName *linkedResourceName;
    s32 resourceIndex;
    s32 bindingIndex;
    u32 nameIndex;

    if (resource == NULL) {
        return;
    }
    secondaryWorld = dds3GetControlScene();
    for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
        scriptResource = resource->data;
        dds3AddModelBasic2(resource->id, &scriptResource->itemList,
                                         scriptResource->motionTable, fldRoomSoundResources, resource->name);
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
        scriptObject = dds3AddMatterObjectBasic(resource->id, position, rotation);
        dds3SetUnitObjectId(scriptObject, resource->name);
        if (fldGlobalBase.area >= 200 && fldGlobalBase.area < 500) {
            if (fldGlobalBase.area == 230 && fldGlobalBase.floor == 6 && resourceIndex == 2) {
                dds3MatterObjectBasicSetOt(scriptObject, 6);
            } else {
                switch (resourceIndex) {
                    case 0:
                        dds3MatterObjectBasicSetOt(scriptObject, 2);
                        break;
                    case 1:
                        dds3MatterObjectBasicSetOt(scriptObject, 3);
                        break;
                    case 2:
                        dds3MatterObjectBasicSetOt(scriptObject, 4);
                        break;
                    default:
                        dds3MatterObjectBasicSetOt(scriptObject, 5);
                        break;
                }
            }
        } else {
            dds3MatterObjectBasicSetOt(scriptObject, 7);
        }
        dds3MatterObjectBasicSetHojiID(scriptObject, fldCommand_GetHojiID(resource->name));
        dds3ObjectBaseSetControlBasic(scriptObject, dds3SearchSceneBasic(secondaryWorld, resource->id, 10));
        dds3ObjectBaseMakeMatter(scriptObject);
        bindingResource = D_00438EC0;
        for (bindingIndex = 0; bindingIndex < D_00438EC4; bindingIndex++, bindingResource++) {
            if (bindingResource->names != NULL) {
                nameEntry = bindingResource->names->entries;
                nameIndex = 0;
                for (; nameIndex < bindingResource->names->count; nameIndex++, nameEntry++) {
                    if (strcmp(resource->name, nameEntry->name) == 0) {
                        fldBindWorldNodeToRecordId(bindingResource->id, scriptObject);
                        break;
                    }
                }
            }
        }
        /* Retail fetches the link descriptor even when the object is NULL. */
        linkedResourceName = ((const FldFileResourceLinkView *)resource)->linkedResourceName;
        if (scriptObject != NULL) {
            dds3ObjectBaseSetControlBasic(scriptObject, dds3SceneBasicSearchNameAndType(secondaryWorld, 2, (const u8 *)linkedResourceName->name));
            dds3ObjectBaseRegistArea(scriptObject);
        }
    }
}

#include "eff_object.h"
#include "dds3_path.h"
#include "fld_resource_resolver.h"

extern u8 fldTestMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit);
extern u8 fldTestRoomModeFlag(s32 mapId, u32 slotIndex, s32 bit);
extern void dds3ObjectBaseCreateMover(dds3Basic_t *);
extern void dds3MoverBasicMakePath(dds3Basic_t *object);
extern void evtPathRate(dds3Basic_t *object, f32 multiplier);
extern void dds3MoverBasicUpdate(dds3Basic_t *);
extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *, u32);
extern char D_004130A8[], D_004130B8[], D_004130C8[];

/* Restore the room's named objects and enable their retained path controllers. */
void fldMakeObjectBasic3(void) {
    char modelName[16];
    char collisionName[16];
    char pathName[16];
    dds3Basic_t *world = dds3GetControlScene();
    s32 roomFlagIndex;

    for (roomFlagIndex = 1; roomFlagIndex < 16; roomFlagIndex++) {
        if (fldTestMapSlotValueFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomFlagIndex)) {
            FldSceneObjectKeyResult modelKey, collisionKey;
            dds3Basic_t *object;
            dds3Basic_t *path;
            dds3ObjectBaseControlWorkData_t *data;
            dds3Basic_t *slot;
            Dds3SlotResource *resource;

            sprintf(modelName, D_004130A8, roomFlagIndex);
            sprintf(collisionName, D_004130B8, roomFlagIndex);
            sprintf(pathName, D_004130C8, roomFlagIndex);
            modelKey = fldCommand_SEARCH_ID(modelName);
            collisionKey = fldCommand_SEARCH_ID(collisionName);
            if (modelKey == FLD_SCENE_OBJECT_KEY_NOT_FOUND) {
                object = fldGetMatter_From_ModelRangeID(collisionKey);
            } else {
                object = dds3SearchSceneBasic(world, modelKey, 6);
                if (object == NULL) {
                    object = fldGetMatter_From_ModelRangeID(collisionKey);
                }
            }
            path = dds3SceneBasicSearchNameAndType(world, 0x10, (const u8 *)pathName);
            data = dds3MatterObjectBasicGetControlWorkData(object);
            slot = (dds3Basic_t *)data->slots[1];
            if (slot == NULL) {
                dds3ObjectBaseCreateMover(object);
                slot = (dds3Basic_t *)data->slots[1];
            }
            resource = (Dds3SlotResource *)slot->data;
            if (resource->sourceObject == 0) {
                dds3MoverBasicSetPath(slot, path);
                dds3MoverBasicMakePath(slot);
            }
            dds3PathBasicControlAlterMode(&dds3MoverBasicGetPathControl(slot)->counter, 0);
            evtPathRate(object, 1.0f);
            dds3MoverBasicUpdate(slot);
        }
        if (fldTestRoomModeFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomFlagIndex)) {
            FldSceneObjectKeyResult key;
            dds3Basic_t *object;

            sprintf(modelName, D_004130A8, roomFlagIndex);
            key = fldCommand_SEARCH_ID(modelName);
            object = dds3SearchSceneBasic(world, key, 6);
            dds3MatterObjectBasicSetAlpMode(object, 2);
        }
    }
}



void fldLoadSceneRequestFiles(FldLoadRequest *request) {
    char directory[32];
    char path[64];
    s32 resourceSlotIndex;

    fldSetFileResolverIdx(request->record->fileResolverIndex);
    D_00444920[0] = (u32)request->record->resourceA;
    D_00444920[1] = (u32)request->record->resourceB;
    D_00444920[2] = (u32)request->record->resourceC;
    D_00444920[3] = (u32)request->record->resourceD;
    for (resourceSlotIndex = 0; resourceSlotIndex < 4; resourceSlotIndex++) {
        D_00444930[resourceSlotIndex] = NULL;
        D_00444940[resourceSlotIndex] = NULL;
    }
    if (fldGlobalBase.area < 500) {
        for (resourceSlotIndex = 0; resourceSlotIndex < 4; resourceSlotIndex++) {
            if (D_00444920[resourceSlotIndex] != 0) {
                fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
                sprintf(path, D_00435FD0, directory, D_00444920[resourceSlotIndex]);
                D_00444930[resourceSlotIndex] = sdfDevLoadFileEx(path, (u32 *)&D_00444940[resourceSlotIndex], 0);
            }
        }
    }
    fldMakeResouceBasicFL1(request->spawnBatches, request->spawnBatchCount);
}

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(FldSceneRequest *request) {
    s32 areaId;
    fldSpawnActionObjects(request->spawnList, request->spawnCount);
    areaId = fldGlobalBase.area;
    if (areaId != 1 && areaId < 200) fldCreatePlayerObject();
    fldSpawnPlacementObjectsFromBatches(request->spawnBatches, request->spawnBatchCount,
                                        FLD_PLACEMENT_BATCH_MODE_INITIAL);
    fldGlobalBase.fallbackResourceTaskName = request->record->fallbackResourceName;
}

void fldProcessFieldRequestAlternate(FldSceneRequest *request) {
    fldSpawnPlacementObjectsFromBatches(request->spawnBatches, request->spawnBatchCount,
                                        FLD_PLACEMENT_BATCH_MODE_APPEND);
}

u8 *fldGetInfoHeader(FldTransferChunk *chunk) {
    return (u8 *)chunk + sizeof(*chunk);
}

void sdfAddressResolverReset(u32 *table, u32 base, const u8 *data, s32 size) {
    const u8 *p = data;
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
void fldAddressResolver(u8 *buffer, FldTransferChunk *chunk) {
    sdfAddressResolver((s32 *)buffer, (u32)buffer, buffer + chunk->offset, chunk->size);
}

void fldRelocateTransferChunkWords(u8 *buffer, FldTransferChunk *chunk) {
    sdfAddressResolverReset((u32 *)buffer, (u32)buffer, buffer + chunk->offset, chunk->size);
}

void fldLoadNextFile_SetId_BtlRet(u32 area, u32 floor) {
    fldPendingArea = area;
    fldPendingFloor = floor;
}

FldBattleReturnAreaLoadResult fldTryStartPendingBattleReturnAreaLoad(void) {
    char directory[64];
    char path[80];
    u32 area = fldPendingArea;
    u32 floor = fldPendingFloor;

    if (area != 0 || floor != 0) {
        fldFreeNextFile();
        fldGlobalBase.resourceArea = area;
        fldGlobalBase.resourceFloor = floor;
        fldMakeFilePath(directory, area, 1);
        sprintf(path, D_004130D8, directory, area, floor);
        pFldNextFileDataF2 = (FilePacRequest *)pacLoad(path);
        fldGlobalBase.resourceFlag = FLD_AREA_RESOURCE_REQUEST_LOAD_PENDING;
        return FLD_BATTLE_RETURN_AREA_LOAD_STARTED;
    }
    return FLD_BATTLE_RETURN_AREA_LOAD_NOT_REQUESTED;
}

extern void sdfFileCmdThreadSetLoPrio(void);

extern s32 D_00436010;

s32 fldRequestAreaResource(s32 area, s32 room) {
    char directory[64];
    char path[80];

    if (fldGlobalBase.area != area) {
        return 1;
    }
    if (fldGlobalBase.resourceFlag == FLD_AREA_RESOURCE_REQUEST_LOAD_PENDING) {
        return 1;
    }
    if (fldGlobalBase.resourceArea == area &&
        fldGlobalBase.resourceFloor == room) {
        fldGlobalBase.resourceFlag = FLD_AREA_RESOURCE_REQUEST_SATISFIED_FROM_CACHE;
        return 1;
    }
    fldFreeNextFile();
    fldGlobalBase.resourceArea = area;
    fldGlobalBase.resourceFloor = room;
    if (fldGetLocationCoordinateFlags(area, room) & 0x10) {
        D_00436010 = 1;
    } else {
        D_00436010 = 0;
    }
    sdfFileCmdThreadSetLoPrio();
    D_00435BB4 = 1;
    fldMakeFilePath(directory, area, 1);
    sprintf(path, D_004130D8, directory, area, room);
    pFldNextFileDataF2 = (FilePacRequest *)pacLoad(path);
    fldGlobalBase.resourceFlag = FLD_AREA_RESOURCE_REQUEST_LOAD_PENDING;
    return 1;
}

void fldFreeNextFile(void) {
    if (pFldNextFileDataF2 != 0) {
        PacWork *work = pFldNextFileDataF2->packet.queueHead;

        if (work != 0) {
            do {
                sdfMemFreeDelayedDirect(PAC_WORK_ALLOCATION(work));
                work = work->next;
            } while (work != 0);
        }
        pacFileHandleFree_(pFldNextFileDataF2);
        pFldNextFileDataF2 = NULL;
    }
    fldPendingArea = 0;
    fldPendingFloor = 0;
    fldGlobalBase.resourceArea = 0;
    fldGlobalBase.resourceFloor = 0;
    fldGlobalBase.resourceFlag = FLD_AREA_RESOURCE_REQUEST_IDLE;
}

u32 fldPollAreaResourceLoad(void) {
    FldAreaResourceRequestState resourceState = fldGlobalBase.resourceFlag;

    if (resourceState != FLD_AREA_RESOURCE_REQUEST_IDLE) {
        if (resourceState == FLD_AREA_RESOURCE_REQUEST_LOAD_PENDING) {
            if (pacCheckEnd((struct FileRequest *)pFldNextFileDataF2) != 0) {
                fldGlobalBase.resourceFlag = FLD_AREA_RESOURCE_REQUEST_IDLE;
                D_00435BB4 = 0;
            }
        }
    }
    return 0;
}

FldNextFileLoadState fldGetNextFileLoadState(void) {
    return D_003897E8[0];
}

u8 fldLoadNextGetStatus2(void) {
    if (pFldNextFileDataF2 != 0) {
        if (pacCheckEnd((struct FileRequest *)pFldNextFileDataF2) != 0) {
            return 1;
        }
    }
    return D_003897E8[0] != 0;
}

FldAreaResourceRequestReuseResult fldCanReuseAreaResourceRequest(s32 area, s32 room) {
    if (fldGlobalBase.resourceArea != area || fldGlobalBase.resourceFloor != room) {
        return FLD_AREA_RESOURCE_REQUEST_NOT_REUSABLE;
    }
    if (pFldNextFileDataF2 != 0 && pacCheckEnd((struct FileRequest *)pFldNextFileDataF2) != 0) {
        return FLD_AREA_RESOURCE_REQUEST_REUSABLE;
    }
    return fldGlobalBase.resourceFlag != FLD_AREA_RESOURCE_REQUEST_IDLE
               ? FLD_AREA_RESOURCE_REQUEST_REUSABLE
               : FLD_AREA_RESOURCE_REQUEST_NOT_REUSABLE;
}

struct sdfMemHandle_t *fldCopyCachedRoomResourceIfLocationMatches(const SdfTextureOffsetListHeader **destination, s32 area, s32 room) {
    FldAreaWork *state = &fldGlobalBase;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
            struct sdfMemHandle_t *allocation = sdfMemAllocDirect(fldCachedRoomResourceSize);
            void *data = (void *)sdfMemLock(allocation);
            *destination = data;
            memcpy(data, fldCachedRoomResourceData, fldCachedRoomResourceSize);
            return allocation;
        }
    }
    return NULL;
}

struct sdfMemHandle_t *fldCopyCachedRoomF1ResourceIfLocationMatches(u8 **destination, s32 area, s32 room) {
    FldAreaWork *state = &fldGlobalBase;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
            struct sdfMemHandle_t *allocation = sdfMemAllocDirect(fldCachedRoomF1ResourceSize);
            u8 *data = (u8 *)sdfMemLock(allocation);
            *destination = data;
            memcpy(data, fldCachedRoomF1ResourceData, fldCachedRoomF1ResourceSize);
            return allocation;
        }
    }
    return NULL;
}

struct sdfMemHandle_t *fldCopyCachedRoomF2ResourceIfLocationMatches(u8 **destination, s32 area, s32 room) {
    FldAreaWork *state = &fldGlobalBase;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
            struct sdfMemHandle_t *allocation = sdfMemAllocDirect(fldCachedRoomF2ResourceSize);
            u8 *data = (u8 *)sdfMemLock(allocation);
            *destination = data;
            memcpy(data, fldCachedRoomF2ResourceData, fldCachedRoomF2ResourceSize);
            return allocation;
        }
    }
    return NULL;
}

struct sdfMemHandle_t *fldCopyCachedRoomKF2ResourceIfLocationMatches(u8 **destination, s32 area, s32 room) {
    FldAreaWork *state = &fldGlobalBase;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
            struct sdfMemHandle_t *allocation = sdfMemAllocDirect(fldCachedRoomKF2ResourceSize);
            u8 *data = (u8 *)sdfMemLock(allocation);
            *destination = data;
            memcpy(data, fldCachedRoomKF2ResourceData, fldCachedRoomKF2ResourceSize);
            return allocation;
        }
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004130A8);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004130B8);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004130C8);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004130D8);

void fldFormatAreaResourceName(char *out) {
    char directory[32];
    s32 area = D_00389780[0];

    fldMakeFilePath(directory, area, 1);
    if (area == 0x17 && evtBitCheck(0x19)) {
        sprintf(out, "%sf%03d_00a.LB", directory, 0x17);
        return;
    }
    if (area == 0x18 && evtBitCheck(0x19)) {
        sprintf(out, "%sf%03d_00a.LB", directory, 0x18);
        return;
    }
    if (area == 0x1B && evtBitCheck(0x19)) {
        sprintf(out, "%sf%03d_00a.LB", directory, 0x1B);
        return;
    }
    sprintf(out, "%sf%03d_000.LB", directory, area);
}

u8 fldHasAreaResourceNameChanged(void) {
    char buf[32];

    fldFormatAreaResourceName(buf);
    return strcmp(D_00444950, buf) != 0;
}

extern void fldInfLoadFromMem(const FldInfTable *source);

extern void fldGetDoorEntry(const FldWaypointBlock *source);



extern void fldAdoptSceneRecordChunk(u8 *chunk, struct sdfMemHandle_t *resourceHandle);
#include "fld_label_cache_api.h"

void fldLoadFieldPack(void) {
    char name[32];
    FilePacRequest *request;
    PacWork *work;

    if (fldGlobalBase.area < 200) {
        fldFormatAreaResourceName(name);
        strcpy(D_00444950, name);
        gFldFileF1 = pacLoad(name);
        pacSync(gFldFileF1);
        request = (FilePacRequest *)gFldFileF1;
        for (work = request->packet.queueHead; work != NULL; work = work->next) {
            switch (*(u16 *)(work->packet + 2)) {
            case FLD_PACKED_RESOURCE_INFO_TABLE:
                fldInfLoadFromMem((const FldInfTable *)work->dataCursor);
                sdfMemFreeDelayedDirect(
                    PAC_WORK_ALLOCATION(work));
                break;
            case FLD_PACKED_RESOURCE_NPC_PALETTE:
                fldSetNpcPalette((const FldNpcPalette *)work->dataCursor);
                sdfMemFreeDelayedDirect(
                    PAC_WORK_ALLOCATION(work));
                break;
            case FLD_PACKED_RESOURCE_SKY_BUFFER:
                fldUploadSkyBuffer((const FldSkyBuffer *)work->dataCursor);
                sdfMemFreeDelayedDirect(
                    PAC_WORK_ALLOCATION(work));
                break;
            case FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE:
                fldGetDoorEntry((const FldWaypointBlock *)work->dataCursor);
                sdfMemFreeDelayedDirect(
                    PAC_WORK_ALLOCATION(work));
                break;
            case FLD_PACKED_RESOURCE_RETAINED_COPY:
                fldFieldPackData = sdfMemAllocDirect(
                    sdfMemGetSize(PAC_WORK_ALLOCATION(work)));
                memcpy((void *)sdfMemGetPtr(fldFieldPackData),
                       (void *)sdfMemGetPtr(PAC_WORK_ALLOCATION(work)),
                       sdfMemGetSize(PAC_WORK_ALLOCATION(work)));
                sdfMemFreeDelayedDirect(
                    PAC_WORK_ALLOCATION(work));
                break;
            case FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK:
                fldAdoptSceneRecordChunk(work->dataCursor, PAC_WORK_ALLOCATION(work));
                break;
            }
        }
        fldCacheMapLabelLengths(fldGlobalBase.area % 100);
    }
}

void fldFreePac0(void) {
    struct sdfMemHandle_t *cachedResource = fldFieldPackData;

    if (cachedResource != NULL) {
        sdfMemFreeDelayedDirect(cachedResource);
        fldFieldPackData = NULL;
    }
    if (gFldFileF1 != NULL) {
        pacFileHandleFree_(gFldFileF1);
        gFldFileF1 = NULL;
    }
    D_00444950[0] = D_00436020[0];
}

extern u32 gFldFileResetMode;
extern s32 gFldInf_WpExit;
extern FldTransferChunk *fldRoomF1Chunk;
extern u8 *fldRoomF1Data;
extern FldTransferChunk *fldRoomF2Chunk;
extern u8 *fldRoomF2Data;
extern FldTransferChunk *fldRoomKF2Chunk;
extern u8 *fldRoomKF2Data;
extern FilePacRequest *D_00435FC8;
extern struct sdfMemHandle_t *D_00435FDC;
extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;
extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;
extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;
extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;
extern struct sdfMemHandle_t *fldFieldPackData;
extern void fldSetSceneLocation(s32 area, s32 floor, s32 stage);
extern void fldLoadPlayerModel(void);
extern void fldInitSparkTable(void);
extern void fldInitializeMenuResources(void);
extern void fldUpdateSparkSlots(void);
extern void fldMakeObjectBasic3(void);
extern void fldInitializeActorMotionSlots(void);
extern struct DevRequest *sndLoadNamedOffsetResourceList(const char *name);
extern char D_00436028[];
extern char D_00436030[];
extern char D_00436038[];
extern FieldPlayerSceneWork fldSceneParam;
extern void fldDrwZAlloc(void);
#include "fld_object_slot_api.h"
extern void fldReleaseResourceSlots(void);
extern void fldCreateResourceScriptObjects(void);
extern u8 fldHasAreaResourceNameChanged(void);
extern void fldLoadFieldPack(void);
extern void fldFreePac0(void);
extern void fldAddressResolver(u8 *buffer, FldTransferChunk *chunk);

const char D_00413128[] = "%sk%03d_%03d.f2";

extern void fldClrCollision(void);
extern void fldResetZoneRecordsAndActorSlots(void);
extern void fldSe_Init(void);

struct sdfMemHandle_t *fldLoadAreaRoomResources(const char *resourceName) {
    char path[0x80];
    char directory[0x40];
    const SdfTextureOffsetListHeader *roomOffsets;
    union {
        u8 *data;
        u32 resolvedAddress;
    } roomData;
    FldTransferChunk *chunk;
    FilePacRequest *request;
    FieldPlayerSceneWork *sceneWork = &fldSceneParam;
    s32 area;
    s32 room;
    s32 isField;
    PacWork *work;
    s32 index;

    gFldFileResetMode = 0;
    fldRoomEventPlacementCount = 0;
    fldCachedRoomF1ResourceAllocation = NULL;
    fldCachedRoomF2ResourceAllocation = NULL;
    fldCachedRoomKF2ResourceAllocation = NULL;
    gFldInf_WpExit = 0;
    area = dds3GetMapNo(dds3GetControlScene()) >> 16;
    room = dds3GetMapNo(dds3GetControlScene()) & 0xFFFF;
    fldSetSceneLocation(area, room - 1, 0);
    if (fldGlobalBase.area != 1) {
        if (fldGlobalBase.area < 200) {
            fldLoadPlayerModel();
            fldInitSparkTable();
        }
        if ((u32)(fldGlobalBase.area - 200) >= 300) {
            fldDrwZAlloc();
        }
    }
    fldClrCollision();
    fldResetZoneRecordsAndActorSlots();
    fldSe_Init();
    fldClearMenuEntries();
    fldResetObjectSlots();
    if (fldGlobalBase.reuseAreaResources == 0) {
        fldInitializeMenuResources();
        fldLoadFieldPack();
    } else if (fldHasAreaResourceNameChanged() != 0 || (u32)(fldGlobalBase.area - 0x1B) < 2) {
        fldReleaseResourceSlots();
        fldReleaseSceneRecordChunk();
        fldFreePac0();
        fldLoadFieldPack();
    }
    isField = (u32)(fldGlobalBase.area - 200) < 300;
    if (fldGlobalBase.area < 100) {
        isField = 1;
    }
    if (isField != 0) {
        if (fldCanReuseAreaResourceRequest(area, room) != FLD_AREA_RESOURCE_REQUEST_NOT_REUSABLE) {
            request = pFldNextFileDataF2;
            pFldNextFileDataF2 = NULL;
            D_00435FC8 = request;
            fldFreeNextFile();
        } else {
            fldMakeFilePath(directory, area, 1);
            sprintf(path, D_004130D8, directory, area, room);
            request = (FilePacRequest *)pacLoad(path);
            D_00435FC8 = request;
            pacSync((struct FileRequest *)request);
        }
        index = 0;
        fldRoomSoundResources = NULL;
        work = D_00435FC8->packet.queueHead;
        fldCachedRoomResourceAllocation = NULL;
        for (; work != NULL; work = work->next) {
            switch (index) {
            case 0:
                fldRoomSoundResources = sdfTexSetTB((const SdfTextureOffsetListHeader *)work->dataCursor);
                sdfMemFreeDelayedDirect(PAC_WORK_ALLOCATION(work));
                fldCachedRoomResourceAllocation = NULL;
                break;
            case 1:
                fldCachedRoomF2ResourceAllocation = NULL;
                roomData.data = work->dataCursor;
                chunk = (FldTransferChunk *)((u8 *)roomData.data + 8);
                fldRoomF2Data = roomData.data;
                fldRoomF2Chunk = chunk;
                fldAddressResolver(roomData.data, chunk);
                fldProcessFieldRequest((FldSceneRequest *)fldGetInfoHeader(chunk));
                break;
            case 2:
                fldCachedRoomF1ResourceAllocation = NULL;
                roomData.data = work->dataCursor;
                chunk = (FldTransferChunk *)((u8 *)roomData.data + 8);
                fldRoomF1Data = roomData.data;
                fldRoomF1Chunk = chunk;
                fldAddressResolver(roomData.data, chunk);
                fldLoadSceneRequestFiles((FldLoadRequest *)fldGetInfoHeader(chunk));
                break;
            }
            index++;
        }
    }
    if (isField == 0) {
        fldCachedRoomResourceAllocation = fldCopyCachedRoomResourceIfLocationMatches(&roomOffsets, area, room);
        if (fldCachedRoomResourceAllocation == NULL) {
            sprintf(path, D_00436028, resourceName);
            fldRoomSoundResources = sndLoadNamedOffsetResourceList(path);
        } else {
            fldRoomSoundResources = sdfTexSetTB(roomOffsets);
            sdfMemFreeDelayedDirect(fldCachedRoomResourceAllocation);
            fldCachedRoomResourceAllocation = NULL;
        }
    }
    if (isField == 0) {
        sprintf(path, D_00436030, resourceName);
        fldCachedRoomF2ResourceAllocation = fldCopyCachedRoomF2ResourceIfLocationMatches(&roomData.data, area, room);
        if (fldCachedRoomF2ResourceAllocation == NULL) {
            fldCachedRoomF2ResourceAllocation = sdfDevLoadFileEx(path, &roomData.resolvedAddress, 0);
        }
        chunk = (FldTransferChunk *)(roomData.data + 8);
        fldRoomF2Data = roomData.data;
        fldRoomF2Chunk = chunk;
        fldAddressResolver(roomData.data, chunk);
        fldProcessFieldRequest((FldSceneRequest *)fldGetInfoHeader(chunk));
    }
    if (fldGetLocationCoordinateFlags(area, room) & 0x10) {
        fldMakeFilePath(directory, area, 1);
        sprintf(path, D_00413128, directory, area, room);
        fldCachedRoomKF2ResourceAllocation = fldCopyCachedRoomKF2ResourceIfLocationMatches(&roomData.data, area, room);
        if (fldCachedRoomKF2ResourceAllocation == NULL) {
            fldCachedRoomKF2ResourceAllocation = sdfDevLoadFileEx(path, &roomData.resolvedAddress, 0);
        }
        chunk = (FldTransferChunk *)(roomData.data + 8);
        fldRoomKF2Data = roomData.data;
        fldRoomKF2Chunk = chunk;
        fldAddressResolver(roomData.data, chunk);
        fldProcessFieldRequestAlternate((FldSceneRequest *)fldGetInfoHeader(chunk));
    }
    if (isField == 0) {
        sprintf(path, D_00436038, resourceName);
        fldCachedRoomF1ResourceAllocation = fldCopyCachedRoomF1ResourceIfLocationMatches(&roomData.data, area, room);
        if (fldCachedRoomF1ResourceAllocation == NULL) {
            fldCachedRoomF1ResourceAllocation = sdfDevLoadFileEx(path, &roomData.resolvedAddress, 0);
        }
        chunk = (FldTransferChunk *)(roomData.data + 8);
        fldRoomF1Data = roomData.data;
        fldRoomF1Chunk = chunk;
        fldAddressResolver(roomData.data, chunk);
        fldLoadSceneRequestFiles((FldLoadRequest *)fldGetInfoHeader(chunk));
    }
    fldCreateResourceScriptObjects();
    fldMakeObjectBasic3();
    if (sceneWork->retainCachedAreaResource != 0 && area < 200 && fldFieldPackData != NULL) {
        D_00435FDC = sdfMemAllocDirect(sdfMemGetSize(fldFieldPackData));
        memcpy((void *)sdfMemGetPtr(D_00435FDC),
               (void *)sdfMemGetPtr(fldFieldPackData),
               sdfMemGetSize(fldFieldPackData));
        dds3SceneBasicLoadScriptFile_FromMem(dds3GetControlScene(), D_00435FDC);
    }
    fldInitializeActorMotionSlots();
    fldGlobalBase.reuseAreaResources = FLD_AREA_RESOURCE_REUSE_DISABLED;
    if (fldGlobalBase.specialEventActive != 0) {
        fldUpdateSparkSlots();
    }
    return fldCachedRoomF1ResourceAllocation;
}

extern struct sdfMemHandle_t *fldLoadAreaRoomResourcesFromRequest(u32 area, u32 room,
                        u8 *resource0Data, u8 *resource1Data,
                        const SdfTextureOffsetListHeader *textureOffsets, u32 flags);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLoadAreaRoomResourcesFromRequest);

extern void fldDestroyTitleTask(void);

extern void fldFreeSceneResources(void);

extern void fldReleaseObjectSlots(void);

extern void fldReleaseResourceSlots(void);



extern u8 fldTitleMiniIsActive(void);


extern void fldReleaseMenuSlotsAfterWait(void);

extern void fldReleaseIndexedResourceEffect(void);


extern void mnuReleaseResourceEntries(void);


extern struct sdfMemHandle_t *D_00444930[];

extern struct EffNodeDescriptor *D_00444940[];

extern s32 fldRoomEventPlacementCount;

extern struct DevRequest *fldRoomSoundResources;

extern FldTransferChunk *fldRoomF1Chunk;

extern u8 *fldRoomF1Data;

extern FldTransferChunk *fldRoomF2Chunk;

extern u8 *fldRoomF2Data;

extern FldTransferChunk *fldRoomKF2Chunk;

extern u8 *fldRoomKF2Data;

extern FilePacRequest *D_00435FC8;

extern struct sdfMemHandle_t *D_00435FDC;

extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;

void fldDrwZFree(void);
void fldReleaseSkyResources(void);

void fldReleaseFieldResources(void) {
    s32 i;
    PacWork *work;

    fldDrwZFree();
    fldPlayPendingSounds();
    fldDestroyTitleTask();
    if (fldTitleMiniIsActive() != 0) {
        dds3KillProcessName("fldTitleMini", 1);
    }
    fldReleaseObjectSlots();
    fldResetObjectSlots();
    if (fldGlobalBase.area < 0xC8) {
        if (fldGlobalBase.reuseAreaResources != 0) {
            if ((u32)(fldGlobalBase.area - 0x1B) < 2U) {
                fldReleaseResourceSlots();
                fldReleaseMenuSlotsAfterWait();
                fldReleaseSceneRecordChunk();
                fldFreePac0();
            }
        } else {
            fldReleaseResourceSlots();
            fldReleaseMenuSlotsAfterWait();
            fldReleaseSceneRecordChunk();
            fldFreeSceneResources();
            fldFreePac0();
            fldReleaseSkyResources();
        }
        fldClrCollision();
        fldResetZoneRecordsAndActorSlots();
        fldSe_Init();
        fldClearMenuEntries();
        mnuReleaseResourceEntries();
        fldReleaseTextureSlots();
        fldReleaseIndexedResourceEffect();
    }
    sdfTexListFree(fldRoomSoundResources, SDF_TEX_LIST_RELEASE_REFERENCES);
    fldRoomSoundResources = 0;
    for (i = 0; i < 4; i++) {
        if (D_00444930[i] != NULL) {
            sdfMemFreeDelayedDirect(D_00444930[i]);
            D_00444930[i] = NULL;
            D_00444940[i] = NULL;
        }
    }
    if (gFldFileResetMode != 0) {
        if (fldRoomF1Chunk != 0) {
            fldRelocateTransferChunkWords(fldRoomF1Data, fldRoomF1Chunk);
            fldRoomF1Chunk = 0;
            fldRoomF1Data = NULL;
        }
        if (fldRoomF2Chunk != 0) {
            fldRelocateTransferChunkWords(fldRoomF2Data, fldRoomF2Chunk);
            fldRoomF2Chunk = 0;
            fldRoomF2Data = NULL;
        }
        if (fldRoomKF2Chunk != 0) {
            fldRelocateTransferChunkWords(fldRoomKF2Data, fldRoomKF2Chunk);
            fldRoomKF2Chunk = 0;
            fldRoomKF2Data = NULL;
        }
    }
    fldRoomEventPlacementCount = 0;
    D_00435FDC = NULL;
    if (fldCachedRoomResourceAllocation != NULL) {
        sdfMemFreeDelayedDirect(fldCachedRoomResourceAllocation);
        fldCachedRoomResourceAllocation = NULL;
    }
    if (fldCachedRoomF1ResourceAllocation != NULL) {
        sdfMemFreeDelayedDirect(fldCachedRoomF1ResourceAllocation);
        fldCachedRoomF1ResourceAllocation = NULL;
    }
    if (fldCachedRoomF2ResourceAllocation != NULL) {
        sdfMemFreeDelayedDirect(fldCachedRoomF2ResourceAllocation);
        fldCachedRoomF2ResourceAllocation = NULL;
    }
    if (fldCachedRoomKF2ResourceAllocation != NULL) {
        sdfMemFreeDelayedDirect(fldCachedRoomKF2ResourceAllocation);
        fldCachedRoomKF2ResourceAllocation = NULL;
    }
    if (fldGlobalBase.area < 0xC8 && fldGlobalBase.cachedArea != fldGlobalBase.area) {
        fldGlobalBase.cachedArea = fldGlobalBase.area;
    }
    if (D_00435FC8 != 0) {
        work = D_00435FC8->packet.queueHead;
        i = 0;
        if (work != 0) {
            do {
                if (i > 0) {
                    sdfMemFreeDelayedDirect(PAC_WORK_ALLOCATION(work));
                }
                work = work->next;
                i++;
            } while (work != 0);
        }
        pacFileHandleFree_(D_00435FC8);
        D_00435FC8 = NULL;
    }
}

void fldMakeFilePath(char *buffer, s32 area, s32 unused) {
    if (area < 200) {
        sprintf(buffer, "/fld/f/f%03d/", area);
    } else if (area < 500) {
        sprintf(buffer, "/fld/b/f%03d/", area);
    } else {
        sprintf(buffer, "/fld/e/f%03d/", area);
    }
}

void fldDbgPrint2D(s32 x, s32 y, const char *format);
extern u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);
extern char D_00436040[];
extern char D_00436048[];
extern char D_00436050[];
extern char D_00413198[]; /* "0123456789ABCDEF", shared with fldHojiDebugView2 */

/* Debug view of the area's map-slot values (GMC) and room scene flags (HIT):
 * one row per floor 1..31 and one column per slot 0..15, "." for clear. Each
 * draw recomputes the row offset: GCSE PRE merges the `floor * 12 - 0x4C`
 * terms into one reaching register but leaves every `<< 3` per call, as retail does. */
void fldHojiDebugView(void) {
    char text[32];
    s32 floor;
    s32 slot;

    sprintf(text, "GMC-HOJI");
    fldDbgPrint2D(0, -0x320, text);
    sprintf(text, "HIT-HOJI");
    fldDbgPrint2D(0xDC0, -0x320, text);
    sprintf(text, D_00413198);
    fldDbgPrint2D(0, -0x2C0, text);
    fldDbgPrint2D(0xDC0, -0x2C0, text);
    for (floor = 0; floor < 0x1F; floor++) {
        s32 gmcX;
        s32 hitX;

        sprintf(text, D_00436040, floor + 1);
        fldDbgPrint2D(-0x200, (floor * 12 - 0x4C) << 3, text);
        hitX = 0xDC0;
        gmcX = 0;
        for (slot = 0; slot < 0x10; slot++) {
            s32 value = fldTestMapSlotValueFlag(fldGlobalBase.area, floor + 1, slot);

            if (value == 0) {
                sprintf(text, D_00436048);
            } else {
                sprintf(text, D_00436050, value);
            }
            fldDbgPrint2D(gmcX, (floor * 12 - 0x4C) << 3, text);
            gmcX += 0xC0;
            value = fldTestRoomSceneFlag(fldGlobalBase.area, floor + 1, slot);
            if (value == 0) {
                sprintf(text, D_00436048);
            } else {
                sprintf(text, D_00436050, value);
            }
            fldDbgPrint2D(hitX, (floor * 12 - 0x4C) << 3, text);
            hitX += 0xC0;
        }
    }
}

extern u8 fldTestRoomObjectModeFlag(s32 mapId, u32 slotIndex, s32 bit);

/* Same debug grid as fldHojiDebugView for the room mode (VIS) and room object
 * mode (ANM) flags. */
INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413198);

void fldHojiDebugView2(void) {
    char text[32];
    s32 floor;
    s32 slot;

    sprintf(text, "VIS-HOJI");
    fldDbgPrint2D(0, -0x320, text);
    sprintf(text, "ANM-HOJI");
    fldDbgPrint2D(0xDC0, -0x320, text);
    sprintf(text, D_00413198);
    fldDbgPrint2D(0, -0x2C0, text);
    fldDbgPrint2D(0xDC0, -0x2C0, text);
    for (floor = 0; floor < 0x1F; floor++) {
        s32 gmcX;
        s32 hitX;

        sprintf(text, D_00436040, floor + 1);
        fldDbgPrint2D(-0x200, (floor * 12 - 0x4C) << 3, text);
        hitX = 0xDC0;
        gmcX = 0;
        for (slot = 0; slot < 0x10; slot++) {
            s32 value = fldTestRoomModeFlag(fldGlobalBase.area, floor + 1, slot);

            if (value == 0) {
                sprintf(text, D_00436048);
            } else {
                sprintf(text, D_00436050, value);
            }
            fldDbgPrint2D(gmcX, (floor * 12 - 0x4C) << 3, text);
            gmcX += 0xC0;
            value = fldTestRoomObjectModeFlag(fldGlobalBase.area, floor + 1, slot);
            if (value == 0) {
                sprintf(text, D_00436048);
            } else {
                sprintf(text, D_00436050, value);
            }
            fldDbgPrint2D(hitX, (floor * 12 - 0x4C) << 3, text);
            hitX += 0xC0;
        }
    }
}

/* Enable or skip relocation of remaining transfer chunks during field teardown. */
void fldFileAddrResetMode(u32 relocateOnRelease) {
    gFldFileResetMode = relocateOnRelease;
}

void fldDrawInit(void) {
    if (D_00436064 == 0) {
        sdfMat_t *object;
        D_00436064 = 1;
        object = sdfMatNew();
        D_0043607C = (u32)object;
        object->primaryStateFloat = 1.0f;
        D_00438EC8 = (u32)sdfMatNew();
        fldMarkerTexture = sdfTexSet((SdfTextureFileHeader *)(D_0038A700));
    }
}

SdfPoolNode *fldDrawGetOt(void) {
    return &sdfGrOtTable[rsetFrameMaskOtOffset];
}

#include "fld_sprite_vertex.h"

typedef struct SdfGsTextureAlphaFlushPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite textureAlpha;
    SdfGsRegisterWrite textureFlush;
} SdfGsTextureAlphaFlushPacket;

typedef char SdfGsTextureAlphaFlushPacket_size_must_be_0x40[
    (sizeof(SdfGsTextureAlphaFlushPacket) == 0x40) ? 1 : -1];
typedef char SdfGsTextureAlphaFlushPacket_payload_offsets_must_match[
    (((u32)&((SdfGsTextureAlphaFlushPacket *)0)->textureAlpha == 0x20) &&
     ((u32)&((SdfGsTextureAlphaFlushPacket *)0)->textureFlush == 0x30)) ? 1 : -1];

void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture) {
    void *handle = sdf2DAllocSprite(1);
    FldSpriteVertex *vtx = (FldSpriteVertex *)sdf2DGetPrimPtr(handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, texture, SDF_GS_CONTEXT_1);
    sdfDrawTagLink(command, (u32)handle);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void DrwSprite2DC(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;
    SdfPoolNode *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, texture, SDF_GS_CONTEXT_1);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (width << 4) + 0x7000;
    bottom = top + (height << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (textureWidth << 4);
    vBottom = vTop + (textureHeight << 4);
    sdfPrimLinkGt4(list, 0x40,
        left, top, uLeft, vTop, color0,
        right, top, uRight, vTop, color1,
        left, bottom, uLeft, vBottom, color3,
        right, bottom, uRight, vBottom, color2,
        -1, NULL);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->append(surface, list);
}

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture) {
    void *handle = sdf2DAllocSprite(1);
    FldSpriteVertex *vtx = (FldSpriteVertex *)sdf2DGetPrimPtr(handle);
    s32 r = color & 0xFF;
    s32 g = (color >> 8) & 0xFF;
    s32 b = (color >> 16) & 0xFF;
    s32 a = (color >> 24) & 0xFF;
    s32 ubase = u * 16;
    s32 vbase = v * 16;
    s32 xl = (s32)(x * 16.0f) + 0x7000;
    s32 yt = (s32)(y * 8.0f) + 0x7900;
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, texture, SDF_GS_CONTEXT_1);
    sdfDrawTagLink(command, (u32)handle);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void fldPoint3To2(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    VU0_LOAD_MATRIX_MEMORY(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)sdfProjectionMatrix);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_BARRIER(vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B_BARRIER(D_0037F650);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B_BARRIER(D_0037F660);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPoint3To2H(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    VU0_LOAD_MATRIX_MEMORY(D_00384790);
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)D_0037F9B0);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_BARRIER(vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B_BARRIER(D_0037F9F0);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B_BARRIER(D_0037FA00);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPrepareProjectionMatrix(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX_MEMORY(sdfViewMatrix);
    matrix = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)matrix);
    SDF_VU0_COPYMATRIX_A_TO_B();
    matrix += 0x40;
    SDF_VU0_LOADVECTOR_B_BARRIER(matrix);
    VU0_LOAD_VF_MEMORY(vf12, D_0037F660);
}

void fldProjectPointWithPreparedMatrix(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];
    SDF_VU0_LOADVECTOR_BARRIER(vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}


void rsetOtOfs(u32 displayIndex) {
    rsetFrameMaskOtOffset = displayIndex;
}

void rsetTexa(s32 lower, s32 bits, u64 upper) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    void *packet;
    u64 *entry;
    SdfPoolNode *descriptor;

    sdfDrawTagInit(command);
    packet = (void *)sdfGrAllocPacketBuff(0x30);
    entry = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void rsetTest(s32 flag0, s32 flag1, s32 field4, s32 field12, s32 flag14, s32 flag15, s32 unused, s32 field17) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    void *packet;
    u64 *data;
    SdfPoolNode *descriptor;

    sdfDrawTagInit(command);
    packet = (void *)sdfGrAllocPacketBuff(0x30);
    data = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    data[4] = (field17 << 17) | 0x10000 | (flag15 << 15) | (flag14 << 14) | (field12 << 12) | (field4 << 4) | (flag1 << 1) | flag0;
    data[5] = 0x47;
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

/* Write the selected blend equation to GS ALPHA_1, including DDS2's FIX mode. */
void rsetAlpha(s32 mode) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    void *packet;
    u64 *data;
    SdfPoolNode *descriptor;

    sdfDrawTagInit(command);
    packet = (void *)sdfGrAllocPacketBuff(0x30);
    data = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
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
    case 30:
        data[4] = 0x80000000AAULL;
        break;
    default:
        data[4] = 0x44;
        break;
    }
    data[5] = 0x42;
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void aDrawLineStripPrim(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    s32 coords[4];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void aDrawLineDirect(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    u32 coords[4];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void aDrawBoxF_Direct(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    u32 coords[8];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void aDrawTriG(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2) {
    s32 coords[6];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void fldSubmitGsGradientQuad(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3) {
    s32 coords[8];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4C, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void aDrawBoxGF(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
    s32 coords[8];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void fldSubmitTaggedGsRectangle(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag) {
    s32 coords[8];
    sdfDrawTag_t *command;
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
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
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
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void drawSprite3D(sdfTexHandle_t *texture, FldMarkerPacket *modelData) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    DmaPacketHeader *header;
    u8 *packet;
    f32 mat[16];
    SdfPoolNode *descriptor;

    sdfDrawTagInit(command);
    header = (DmaPacketHeader *)sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket(header, (u32)sdfTexGetPacket(texture), sdfTexGetPacketLength(texture));
    sdfAppendReferencePacket(command, (u32)header);
    SetRotateMatrixZ(modelData->scale);
    VU0_STORE_MATRIX(mat);
    packet = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(packet, mat);
    sdfDrawTagLink(command, (u32)packet);
    packet = (u8 *)sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1(packet, modelData->pos, modelData->color, &modelData->uv, modelData->quad);
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void fldSubmitPrimaryFramePacket(void) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    SdfDmaReferenceChainPacket *texture;
    SdfPoolNode *descriptor;
    sdfDrawTagInit(command);
    texture = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);
    sdfBuildMaskedFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, texture, 0);
    sdfDrawTagLinkRefGif(command, (u32)texture);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void fldSubmitAlternateFramePacket(void) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    SdfDmaReferenceChainPacket *texture;
    SdfPoolNode *descriptor;
    sdfDrawTagInit(command);
    texture = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);
    sdfBuildFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, texture, 0);
    sdfDrawTagLinkRefGif(command, (u32)texture);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void ooyTestLine3D(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    sdfDrawTag_t *list;
    s32 record;
    SdfPoolNode *descriptor;
    D_0038A980[0][0] = x;
    D_0038A980[0][1] = y;
    D_0038A980[0][2] = z;
    D_0038A980[1][0] = u;
    D_0038A980[1][1] = v;
    D_0038A980[1][2] = w;
    D_0038A9A0[1] = second;
    D_0038A9A0[0] = first;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    record = (s32)effDebugDrawWireModel((const f32 (*)[4])D_0038A980, D_0038A9A0, 2, 0x80);
    sdfDrawTagLink(list, record);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, list);
}

void fldDrawPoly3(s32 color0, s32 color1, s32 color2,
                         f32 x0, f32 y0, f32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2) {
    SdfPrimitiveRequest desc;
    f32 verts[12];
    s32 indices[3];
    sdfDrawTag_t *command;
    SdfPoolNode *descriptor;

    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf3DPrimLinkResetTm(command, 0);
    sdf3DPrimLinkMat(command, (sdfMat_t *)D_0043607C, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.stripWordCount = 1;
    desc.vertexCount = 3;
    desc.positions = verts;
    desc.vertexColors = indices;
    verts[0] = x0;
    verts[1] = y0;
    verts[2] = z0;
    verts[4] = x1;
    verts[5] = y1;
    verts[6] = z1;
    verts[8] = x2;
    verts[9] = y2;
    verts[10] = z2;
    indices[0] = color0;
    indices[1] = color1;
    indices[2] = color2;
    sdfDrawTagLink(command, (u32)sdf3DPrimMakePoly(&desc));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, command);
}

void fldSubmitTexturedOverlaySprite(void) {
    sdfDrawTag_t *packetList;
    SdfDmaNode *dmaPacket;
    s32 drawBufferIndex;
    SdfGsTextureAlphaFlushPacket *firstPacket;
    sdfPrimTransPacket_t *secondPacket;
    void *columnPacket;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    packetList = (sdfDrawTag_t *)sdfPrimAllocDrawTag(0);
    dmaPacket = (SdfDmaNode *)sdfGrAllocPacketBuff(0x20);
    drawBufferIndex = (s32)dds3GetFrameID();
    sdfGrLinkDispFrameTex(packetList,
                        (u32)(kwlnFrameDrawPacketRecords[drawBufferIndex].baseBytes),
                        dmaPacket);

    /* Set texture alpha and flush the texture cache. */
    firstPacket = (SdfGsTextureAlphaFlushPacket *)sdfGrAllocPacketBuff(0x40);
    firstPacket->header.dmaTag = 3;
    firstPacket->header.vifCommands = 0x5000000310000000ULL;
    firstPacket->header.gifTag = 0x1000000000008002ULL;
    firstPacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    firstPacket->textureAlpha.value = 0x0000008000000080ULL;
    firstPacket->textureAlpha.registerId = SDF_GS_TEXA;
    firstPacket->textureFlush.value = 0;
    firstPacket->textureFlush.registerId = SDF_GS_TEXFLUSH;
    sdfDrawTagLink(packetList, (u32)firstPacket);

    /* Set the primary-context alpha test and blending values. */
    secondPacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    secondPacket->header.dmaTag = 3;
    secondPacket->header.vifCommands = 0x5000000310000000ULL;
    secondPacket->header.gifTag = 0x1000000000008002ULL;
    secondPacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    secondPacket->registers.test.value = 0x0000000000031001ULL;
    secondPacket->registers.test.registerId = SDF_GS_TEST_1;
    secondPacket->registers.alpha.value = 0x44;
    secondPacket->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(packetList, (u32)secondPacket);

    columnPacket = sdf2DAllocSprite(1);
    vertex = (FldSpriteVertex *)sdf2DGetPrimPtr(columnPacket);
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
    sdfDrawTagLink(packetList, (u32)columnPacket);

    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->append(surface, packetList);
}

void fldSubmitOverlaySpriteWithRenderState(s32 mode) {
    sdfDrawTag_t *packetList;
    SdfDmaNode *dmaPacket;
    s32 drawBufferIndex;
    SdfGsTextureAlphaFlushPacket *firstPacket;
    sdfPrimTransPacket_t *secondPacket;
    void *columnPacket;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    packetList = (sdfDrawTag_t *)sdfPrimAllocDrawTag(0);
    dmaPacket = (SdfDmaNode *)sdfGrAllocPacketBuff(0x20);
    drawBufferIndex = (s32)dds3GetFrameID();
    sdfGrLinkDispFrameTex(packetList,
                        (u32)(kwlnFrameDrawPacketRecords[drawBufferIndex].baseBytes),
                        dmaPacket);

    firstPacket = (SdfGsTextureAlphaFlushPacket *)sdfGrAllocPacketBuff(0x40);
    firstPacket->header.dmaTag = 3;
    firstPacket->header.vifCommands = 0x5000000310000000ULL;
    firstPacket->header.gifTag = 0x1000000000008002ULL;
    firstPacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    firstPacket->textureAlpha.value = 0x0000008000000080ULL;
    firstPacket->textureAlpha.registerId = SDF_GS_TEXA;
    firstPacket->textureFlush.value = 0;
    firstPacket->textureFlush.registerId = SDF_GS_TEXFLUSH;
    sdfDrawTagLink(packetList, (u32)firstPacket);

    secondPacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    secondPacket->header.dmaTag = 3;
    secondPacket->header.vifCommands = 0x5000000310000000ULL;
    secondPacket->header.gifTag = 0x1000000000008002ULL;
    secondPacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    secondPacket->registers.test.value = 0x0000000000033001ULL;
    secondPacket->registers.test.registerId = SDF_GS_TEST_1;
    secondPacket->registers.alpha.value = 0x0000008000000064ULL;
    secondPacket->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(packetList, (u32)secondPacket);

    columnPacket = sdf2DAllocSprite(1);
    vertex = (FldSpriteVertex *)sdf2DGetPrimPtr(columnPacket);
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
    sdfDrawTagLink(packetList, (u32)columnPacket);

    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->append(surface, packetList);
}

void fldSubmitOverlayStateAndSprite(s32 alpha) {
    sdfDrawTag_t *packetList;
    SdfDmaNode *dmaPacket;
    s32 drawBufferIndex;
    SdfGsTextureAlphaFlushPacket *texturePacket;
    sdfPrimTransPacket_t *blendPacket;
    void *columnPacket;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    packetList = (sdfDrawTag_t *)sdfPrimAllocDrawTag(0);
    dmaPacket = (SdfDmaNode *)sdfGrAllocPacketBuff(0x20);
    drawBufferIndex = (s32)dds3GetFrameID();
    sdfGrLinkDispFrameTex(packetList,
                        (u32)(kwlnFrameDrawPacketRecords[drawBufferIndex].baseBytes),
                        dmaPacket);

    texturePacket = (SdfGsTextureAlphaFlushPacket *)sdfGrAllocPacketBuff(0x40);
    texturePacket->header.dmaTag = 3;
    texturePacket->header.vifCommands = 0x5000000310000000ULL;
    texturePacket->header.gifTag = 0x1000000000008002ULL;
    texturePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    texturePacket->textureAlpha.value = 0x0000008000000080ULL;
    texturePacket->textureAlpha.registerId = SDF_GS_TEXA;
    texturePacket->textureFlush.value = 0;
    texturePacket->textureFlush.registerId = SDF_GS_TEXFLUSH;
    sdfDrawTagLink(packetList, (u32)texturePacket);

    blendPacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    blendPacket->header.dmaTag = 3;
    blendPacket->header.vifCommands = 0x5000000310000000ULL;
    blendPacket->header.gifTag = 0x1000000000008002ULL;
    blendPacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    blendPacket->registers.test.value = 0x0000000000033001ULL;
    blendPacket->registers.test.registerId = SDF_GS_TEST_1;
    blendPacket->registers.alpha.value = ((u64)(u32)alpha << 32) | 0x64;
    blendPacket->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(packetList, (u32)blendPacket);

    columnPacket = sdf2DAllocSprite(1);
    vertex = (FldSpriteVertex *)sdf2DGetPrimPtr(columnPacket);
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
    sdfDrawTagLink(packetList, (u32)columnPacket);

    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->append(surface, packetList);
}

void fldDrawPos(const f32 position[3]) {
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
    packet.pos[0] = position[0];
    packet.pos[1] = position[1];
    packet.pos[2] = position[2];
    packet.scale = 0.0f;
    rsetOtOfs(0x39);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    drawSprite3D(fldMarkerTexture, &packet);
}

void fldDrawMarkerQuadColored(const f32 position[3], s32 color) {
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
    packet.pos[0] = position[0];
    packet.pos[1] = position[1];
    packet.pos[2] = position[2];
    packet.scale = 0.0f;
    rsetOtOfs(0x39);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    drawSprite3D(fldMarkerTexture, &packet);
}

void fldDraw2DBar(s32 x, s32 y, s32 width, s32 height) {
    DrwSprite2D(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    DrwSprite2D(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, fldMarkerTexture);
    DrwSprite2D(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}

void fldDrwZAlloc(void) {
    if (fldDrwZBuf == 0) {
        fldDrwZBuf = sdfAllocAuto(0x70000);
    }
}

void fldDrwZFree(void) {
    if (fldDrwZBuf != 0) {
        sdfFreeAutoDelayed(fldDrwZBuf);
        fldDrwZBuf = 0;
    }
}

void fldDrwZSave(void) {
    if (fldDrwZBuf != 0) {
        sdfDrawTag_t *packet = sdfPrimAllocDrawTag(0);
        SdfPoolNode *descriptor;
        sdfLinkStoreImage(packet, D_0040B290.buffers[2],
                                0, 0, 0x200, 0xE0, (s32)(u32)fldDrwZBuf, 0, 0, 0);
        descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
        descriptor->append(descriptor, packet);
    }
}

void fldDrwZLoad(void) {
    if (fldDrwZBuf != 0) {
        sdfDrawTag_t *packet = sdfPrimAllocDrawTag(0);
        SdfPoolNode *descriptor;
        sdfLinkLoadImage(packet, D_0040B290.buffers[2],
                                  0, 0, 0x200, 0xE0, (s32)(u32)fldDrwZBuf, 0);
        descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
        descriptor->append(descriptor, packet);
    }
}

void fldKanjiDraw(s32 x, s32 y, u32 colors, const u8 *text) {
    struct _FRQ *object;

    object = itfCreateConvertedTextGlyph(x << 4, y << 4, 0, colors, text, 0);
    frFontDrawGlyphInDefaultMode(object);
    frDestroyFontRequestAfter(object);
}

void LF(FldQuadState *quad) {
    quad->rowY = quad->rowY + 0x60;
}

void initDrawTag(FldQuadState *quad) {
    s32 packet;
    sdfDrawTag_t *packetList;

    packetList = dds3FontReqAllocDrawTag();
    quad->packetList = packetList;
    packet = sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket((sdfPrimTransPacket_t *)packet);
    sdfDrawTagLink(quad->packetList, packet);
}

void fldDbgPrint3D(f32 x, f32 y, f32 z, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;
    f32 screenX;
    f32 screenY;

    VU0_LOAD_MATRIX(sdfViewMatrix);
    InversMatrix();
    fldPoint3To2(&screenX, &screenY, x, y, z);
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
    initDrawTag(&quad);
    sdfPktInit(&packet, (s32)screenX * 16, (s32)screenY * 16, quad.drawDepth, 1);
    sdfDrawTagLink(quad.packetList,
                    (u32)sdfDevPrintfEx(&packet, D_00436070, drawValue));
    LF(&quad);
    D_00380708.append(&D_00380708, quad.packetList);
}

void fldDbgPrint2D(s32 x, s32 y, const char *format) {
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
    initDrawTag(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, 0);
    sdfDrawTagLink(quad.packetList, (u32)sdfDevPrintfEx(&packet, format));
    LF(&quad);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, quad.packetList);
}

void fldDbgPrint2DC(s32 x, s32 y, s32 packetField, s32 drawValue) {
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
    initDrawTag(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, packetField);
    sdfDrawTagLink(quad.packetList, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    D_00380708.append(&D_00380708, quad.packetList);
}

void fldDbgPrintNum2D(s32 x, s32 y, s32 drawValue) {
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
    initDrawTag(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfDrawTagLink(quad.packetList, (u32)sdfDevPrintfEx(&packet, D_00436070, drawValue));
    LF(&quad);
    D_00380708.append(&D_00380708, quad.packetList);
}

void fldDbgPrintNum2DH(s32 x, s32 y, s32 drawValue) {
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
    initDrawTag(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfDrawTagLink(quad.packetList, (u32)sdfDevPrintfEx(&packet, D_00436078, drawValue));
    LF(&quad);
    D_00380708.append(&D_00380708, quad.packetList);
}

void fldDbgPrintStr2D(s32 x, s32 y, s32 drawValue) {
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
    initDrawTag(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfDrawTagLink(quad.packetList, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    D_00380708.append(&D_00380708, quad.packetList);
}

void fldDbgPrintStr2DC(s32 x, s32 y, s32 packetField, s32 drawValue) {
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
    initDrawTag(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, packetField);
    sdfDrawTagLink(quad.packetList, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    D_00380708.append(&D_00380708, quad.packetList);
}

void fldDbgPrintStr2DCF(s32 packetField, s32 drawValue, f32 x, f32 y) {
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
    initDrawTag(&quad);
    sdfPktInit(&packet, quad.rowX + (s32)(x * 16.0f), quad.rowY + (s32)(y * 8.0f), quad.drawDepth, packetField);
    sdfDrawTagLink(quad.packetList, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    D_00380708.append(&D_00380708, quad.packetList);
}

void fldDrawMaruKage(u32 fade, f32 x, f32 y, f32 z, f32 radius) {
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
        rsetOtOfs(0x39);
        rsetAlpha(5);
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
            sinA = sdfSin(a) * radius;
            cosA = sdfCos(a) * radius;
            b = (f32)prev * 3.14f / 180.0f;
            sinB = sdfSin(b) * radius;
            cosB = sdfCos(b) * radius;
            fldDrawPoly3(color, 0x80101010, 0x80101010,
                                x + 0.0f, y - 2.0f, z + 0.0f,
                                x + sinA, y - 2.0f, z + cosA,
                                x + sinB, y - 2.0f, z + cosB);
        }
        rsetAlpha(0);
    }
}

void fldDrawExpandedSpriteStrip(s32 alpha, s32 offset) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfPrimAllocDrawTag(NULL);
    SdfDmaNode *reference = (SdfDmaNode *)sdfGrAllocPacketBuff(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *handle;
    FldSpriteVertex *vertex;
    s32 quarter;
    SdfPoolNode *descriptor;

    sdfGrLinkDispFrameTex(list,
        (u32)(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes), reference);
    texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x48;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);
    handle = sdf2DAllocSprite(1);
    vertex = (FldSpriteVertex *)sdf2DGetPrimPtr(handle);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    quarter = offset;
    if (offset < 0) {
        quarter = offset + 3;
    }
    quarter >>= 2;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF7 - offset;
    vertex->corner[0].y = 0x78FB - quarter;
    vertex->corner[0].mask = 0x3FFF;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = offset + quarter + 0x9009;
    vertex->corner[1].y = quarter + 0x8705;
    vertex->corner[1].mask = 0x3FFF;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, (u32)handle);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    descriptor->append(descriptor, list);
}

s32 fldGetEncounterRuntimeResult(void) {
    s32 state = fldEncounterRuntimeState;
    s32 result;

    if (state < 3) {
        if (state < 0) {
            result = D_0043608C;
        } else {
            result = btlGetRuntimeResult();
        }
    } else {
        result = D_0043608C;
    }
    return result;
}

void fldSetEncounterPendingValue(u32 pendingValue) {
    D_00436088 = pendingValue;
}

s32 fldEncProc(dds3ProcessID_t *task) {
    s32 state = fldEncounterRuntimeState;

    if (state < 3) {
        if (state >= 0) {
            btlUpdateRuntimeFadeState();
        }
    }
    return 0;
}

#define FLD_ENCOUNTER_MODE_BYPASS_BATTLE_RUNTIME 3

extern void p2sdPlaySE(s32 sequence, s32 volume, s32 pan);

void fldRequestEncounterWithFade(s32 encounterMode, s32 recordIndex) {
    if ((recordIndex < 0x400) && ((datBattleSceneRecords[recordIndex].flags & 0x8000) != 0))
    {
        kwlnFadeBackgroundStartOut(0);
        fldSetEncounterMode(FLD_ENCOUNTER_MODE_BYPASS_BATTLE_RUNTIME);
        return;
    }
    p2sdPlaySE(0xf, 0x7f, 0x3f);
    fldSetEncounterMode(encounterMode);
}

s32 fldSetEncounterMode(s32 encounterMode) {
    fldEncounterRuntimeState = encounterMode;
    D_0043608C = 0;
    if (fldGetEncounterRuntimeResult() == 0) {
        if (fldEncounterRuntimeState < 3) {
            if (fldEncounterRuntimeState >= 0) {
                btlActivateRuntime(fldEncounterRuntimeState);
                if (dds3GetActiveScene() != 0) {
                    dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
                }
            }
        }
    }
}

void fldResetEncounterAsyncState(dds3ProcessID_t *task) {
    btlResetAsyncState();
}

void fldCreateEncounterTask(void) {
    dds3StartProcess(fldEncounterTaskName, 0x2B0F, 0, 1, fldEncProc, fldResetEncounterAsyncState, 0);
    btlClearRuntimeState();
}

void fldUpdateLookAtSegmentDistance(void) {
    f32 *distance = D_003897DC;
    f32 dx = fldLookAtNearPoint[0] - fldCamTarget[0];
    f32 dy = fldLookAtNearPoint[1] - fldCamTarget[1];
    f32 dz = fldLookAtNearPoint[2] - fldCamTarget[2];
    *distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

extern FldValueRecord *fldMatterTable;
extern f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

/* Probe the support surface under the camera's focus and its mirrored heading. */
void fldProbeCameraSupportSurface(void) {
    FldAreaWork *cam;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    f32 offset[3];
    f32 x;
    s32 index;
    dds3Basic_t *node;

    fldGlobalBase.cameraSupportHeightOffset = 0.0f;
    fldGlobalBase.cameraSupportHeightDelta = 0.0f;
    if (fldGlobalBase.overrideSupportRecordIndex != -1) {
        index = fldGlobalBase.overrideSupportRecordIndex;
    } else if (fldGlobalBase.unkA4 >= 0) {
        index = fldGlobalBase.unkA4;
    } else {
        return;
    }
    if (fldMatterTable == NULL) {
        return;
    }
    node = fldMatterTable[index].worldNode;
    if (node != NULL) {
        offset[0] = node->inner->position[0];
        offset[1] = node->inner->position[1];
        offset[2] = node->inner->position[2];
    } else {
        offset[0] = 0.0f;
        offset[1] = 0.0f;
        offset[2] = 0.0f;
    }
    cam = &fldGlobalBase;
    xs[0] = fldMatterTable[index].vertices[0][0] + offset[0];
    ys[0] = fldMatterTable[index].vertices[0][1] + offset[1];
    zs[0] = fldMatterTable[index].vertices[0][2] + offset[2];
    xs[1] = fldMatterTable[index].vertices[1][0] + offset[0];
    ys[1] = fldMatterTable[index].vertices[1][1] + offset[1];
    zs[1] = fldMatterTable[index].vertices[1][2] + offset[2];
    xs[2] = fldMatterTable[index].vertices[2][0] + offset[0];
    ys[2] = fldMatterTable[index].vertices[2][1] + offset[1];
    zs[2] = fldMatterTable[index].vertices[2][2] + offset[2];
    x = cam->x - sdfSin(cam->negatedAngle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist;
    cam->cameraSupportHeightOffset = cam->y - fldCamCalcSlopeOffset(x,
        cam->z - sdfCos(cam->negatedAngle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist, xs, ys, zs);
    x = cam->x - sdfSin(-cam->angle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist;
    cam->cameraSupportHeightDelta = cam->y - fldCamCalcSlopeOffset(x,
        cam->z - sdfCos(-cam->angle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist, xs, ys, zs);
    if (cam->overrideSupportRecordIndex == -1) {
        cam->cameraSupportHeightOffset = 0.0f;
    }
}

extern f32 EulerAngleAxisY(void);
extern s32 fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]);
extern f32 D_004360A0, D_004360A4, D_004360A8;
typedef union FldLookAtVector {
    u128 q;
    f32 f[4];
} FldLookAtVector;
void fldInitializeCameraViewFromPlayer(void) {
    FldAreaWork *area;
    FldLookAtVector nearPoint;
    FldLookAtVector referencePoint;
    FldLookAtVector farPoint;
    FldLookAtVector referenceCopy;
    FldLookAtVector farCopy;
    FldLookAtVector collisionPoint;
    FldLookAtVector playerPosition;
    dds3Basic_t *node;
    dds3Basic_t **sceneSlot = fldGetPlayerSceneStateAddress();
    f32 angle;
    f32 phase;
    f32 radiansPerHalfTurn;
    f32 trigResult;
    f32 rowDistance;
    f32 heightOffset;
    f32 positionZ;
    f32 distance;
    f32 ratio;
    f32 scale;
    f32 dx;
    f32 dy;
    f32 dz;

    dds3GetRotPoseSet(fldPlayerObject);
    angle = EulerAngleAxisY() * 180.0f / 3.14f;
    fldGlobalBase.angle = angle;
    fldGlobalBase.targetAngle = angle;
    dds3GetPosPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR_NOMEM(&playerPosition);

    radiansPerHalfTurn = 3.14f;
    area = &fldGlobalBase;
    area->x = playerPosition.f[0];
    area->y = playerPosition.f[1];
    area->z = playerPosition.f[2];
    area->previousX = playerPosition.f[0];
    area->previousY = playerPosition.f[1];
    area->previousZ = playerPosition.f[2];
    area->negatedAngle = -area->angle;
    area->cameraTargetAngle = area->negatedAngle;
    phase = area->negatedAngle * radiansPerHalfTurn / 180.0f;
    trigResult = sdfSin(phase);
    nearPoint.f[0] = area->x - trigResult * 80.0f;
    nearPoint.f[1] = area->y + fldCameraFollowRows[area->rowIdx].y;
    phase = area->negatedAngle * radiansPerHalfTurn / 180.0f;
    trigResult = sdfCos(phase);
    nearPoint.f[2] = area->z - trigResult * 80.0f;
    phase = area->negatedAngle * radiansPerHalfTurn / 180.0f;
    trigResult = sdfSin(phase);
    farPoint.f[0] = area->x + trigResult * fldCameraFollowRows[area->rowIdx].dist;
    farPoint.f[1] = area->y + fldCameraFollowRows[area->rowIdx].targetY;
    phase = area->negatedAngle * radiansPerHalfTurn / 180.0f;
    trigResult = sdfCos(phase);
    rowDistance = fldCameraFollowRows[area->rowIdx].dist;
    positionZ = area->z;
    farPoint.f[2] = positionZ + trigResult * rowDistance;
    PCP_COPY_VECTOR_UNCLOBBERED(fldLookAtNearPoint, &nearPoint);
    PCP_COPY_VECTOR_UNCLOBBERED(fldCamTarget, &farPoint);

    area->cameraSupportHeightOffset = 0.0f;
    area->overrideSupportRecordIndex = -1;
    area->unkA4 = -1;
    area->unk60 = area->cameraSupportHeightOffset;
    heightOffset = area->cameraSupportHeightOffset;
    farPoint.f[1] += heightOffset;
    referencePoint.f[0] = area->x;
    referencePoint.f[1] = area->y + fldCameraFollowRows[area->rowIdx].y;
    referencePoint.f[2] = positionZ;
    SDF_VU0_COPYVECTOR(&referenceCopy, &referencePoint);
    SDF_VU0_COPYVECTOR(&farCopy, &farPoint);

    fldHitChkCamera(farCopy.f, referenceCopy.f, collisionPoint.f);

    ratio = D_004360A0 / D_004360A8;
    dx = collisionPoint.f[0] - referencePoint.f[0];
    dy = collisionPoint.f[1] - referencePoint.f[1];
    dz = collisionPoint.f[2] - referencePoint.f[2];
    distance = fsqrtf(dx * dx + dy * dy + dz * dz);
    node = *sceneSlot;
    scale = distance * ratio;
    D_004360A4 = scale;

    farPoint.f[0] = referencePoint.f[0] -
        ((referencePoint.f[0] - collisionPoint.f[0]) / distance * scale);
    farPoint.f[2] = referencePoint.f[2] -
        ((referencePoint.f[2] - collisionPoint.f[2]) / distance * scale);
    farPoint.f[1] = referencePoint.f[1] -
        ((referencePoint.f[1] - collisionPoint.f[1]) / distance * scale) - heightOffset;

    dds3SetPosPoseSet(node, &farPoint.q);
    SDF_VU0_COPYVECTOR(fldLookAtNearPoint, &nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, &farPoint);
    node = *sceneSlot;
    node->ops->update(node);
    fldUpdateLookAtSegmentDistance();
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012F400);

void fldUpdateCameraProjectionEndpoints(void) {
    FldAreaWork *cam = &fldGlobalBase;
    f32 angle;

    D_0038BAF0[0] = cam->x - sdfSin(cam->negatedAngle * 3.14f / 180.0f) * 80.0f;
    D_0038BAF0[1] = cam->y + fldCameraFollowRows[cam->rowIdx].y;
    D_0038BAF0[2] = cam->z - sdfCos(cam->negatedAngle * 3.14f / 180.0f) * 80.0f;
    D_0038BAF0[3] = 1.0f;
    angle = cam->negatedAngle * 3.14f / 180.0f;
    D_0038BB00[0] = cam->x + sdfSin(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_0038BB00[1] = cam->y + fldCameraFollowRows[cam->rowIdx].targetY;
    D_0038BB00[2] = cam->z + sdfCos(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_0038BB00[3] = 1.0f;
}

extern f32 D_0038BAD0[];
extern s32 D_004360B4;
extern s32 D_004360B8;
extern s32 D_003897C0[];


void fldCamUpdateFollow(void) {
    union {
        u128 q;
        f32 f[4];
    } nearPoint, farPoint;
    dds3Basic_t **world = fldGetPlayerSceneStateAddress();
    f32 ratio = (f32)D_004360B4 / (f32)D_004360B8;
    f32 remaining = 1.0f - ratio;
    dds3Basic_t *node;

    nearPoint.f[0] = D_0038BAF0[0] * ratio + D_0038BAD0[0] * remaining;
    nearPoint.f[1] = D_0038BAF0[1] * ratio + D_0038BAD0[1] * remaining;
    nearPoint.f[2] = D_0038BAF0[2] * ratio + D_0038BAD0[2] * remaining;
    farPoint.f[0] = D_0038BB00[0] * ratio + D_0038BB00[0] * remaining;
    farPoint.f[1] = D_0038BB00[1] * ratio + D_0038BB00[1] * remaining;
    farPoint.f[2] = D_0038BB00[2] * ratio + D_0038BB00[2] * remaining;
    SDF_VU0_COPYVECTOR(fldLookAtNearPoint, &nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, &farPoint);
    dds3SetPosPoseSet(*world, &farPoint.q);
    node = *world;
    node->ops->update(node);
    D_004360B4++;
    if (D_004360B8 < D_004360B4) {
        D_003897C0[0] = 0;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012FA58);

extern void dds3CameraObjectBasicSetWorldEye(dds3Basic_t *camera, const u128 *worldEye);
extern f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ);
extern f32 EulerAngleAxisY(void);
extern void dds3GetTMPoseSet(dds3Basic_t *object);

void fldUpdateCameraViewFromPlayer(void) {
    f32 nearPoint[4];
    f32 farPoint[4];
    dds3Basic_t **world = fldGetPlayerSceneStateAddress();
    dds3Basic_t *node;
    f32 angle;

    dds3GetRotPoseSet(fldPlayerObject);
    angle = EulerAngleAxisY() * 180.0f / 3.14f;
    nearPoint[0] = fldLookAtNearPoint[0] + (fldGlobalBase.x - fldLookAtNearPoint[0]);
    nearPoint[1] = fldLookAtNearPoint[1] + ((fldGlobalBase.y + fldCameraFollowRows[fldGlobalBase.rowIdx].y) - fldLookAtNearPoint[1]);
    nearPoint[2] = fldLookAtNearPoint[2] + (fldGlobalBase.z - fldLookAtNearPoint[2]);
    fldGlobalBase.angle = angle;
    fldGlobalBase.targetAngle = angle;
    dds3CameraObjectBasicSetWorldEye(*world, (u128 *)nearPoint);
    dds3GetTMPoseSet(fldPlayerObject);
    farPoint[3] = 1.0f;
    farPoint[0] = fldGlobalBase.focusPos[0];
    farPoint[1] = fldGlobalBase.focusPos[1];
    farPoint[2] = fldGlobalBase.focusPos[2];
    dds3SetPosPoseSet(*world, (u128 *)farPoint);
    farPoint[0] = fldCamTarget[0] + (farPoint[0] - fldCamTarget[0]);
    farPoint[1] = fldCamTarget[1] + (farPoint[1] - fldCamTarget[1]);
    farPoint[2] = fldCamTarget[2] + (farPoint[2] - fldCamTarget[2]);
    dds3SetPosPoseSet(*world, (u128 *)farPoint);
    SDF_VU0_COPYVECTOR(fldLookAtNearPoint, nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, farPoint);
    node = *world;
    node->ops->update(node);
    fldGlobalBase.dist = fsqrtf((nearPoint[0] - farPoint[0]) * (nearPoint[0] - farPoint[0]) +
                               (nearPoint[1] - farPoint[1]) * (nearPoint[1] - farPoint[1]) +
                               (nearPoint[2] - farPoint[2]) * (nearPoint[2] - farPoint[2]));
    fldGlobalBase.negatedAngle = GetSphericalPosMove(farPoint[0], farPoint[2], nearPoint[0], nearPoint[2]) + 180.0f;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001302A0);

dds3Basic_t *fldCamNowEveCamera(void) {
    dds3Basic_t *worldObject = dds3GetActiveScene();
    dds3Basic_t *object = dds3GetActiveCameraSceneBasic(worldObject);

    if (fldGetPlayerSceneState() == object) {
        object = NULL;
    }
    return object;
}

void fldSetElvCameraScene(s32 mode) {
    D_004360AC = mode;
    dds3CameraObjectBasicGetTgtCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), D_00444980,
                                              D_00444970);
    D_004360B0 = 0;
}


extern void dds3OnPoseSetFlag(dds3PoseSet_t *node, u32 flags);

void fldCamElvTask(void) {
    f32 direction = 0.0f;
    f32 phase = D_004360B0;
    dds3Basic_t *camera;

    if (D_004360AC != 0) {
        if (D_004360AC == 1) {
            direction = 1.0f;
        }
        if (D_004360AC == 2) {
            direction = 1.0f;
        }
        if (D_004360AC == -1) {
            direction = -1.0f;
        }
        if (D_004360AC == -2) {
            direction = -1.0f;
        }
        camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
        if (D_004360AC == 1 || D_004360AC == -1) {
            if (phase < 3.14f) {
                phase += 0.2f;
                camera->inner->position[1] = D_00444970[1] + sdfSin(phase * direction) * 2.5f;
            } else {
                phase += 0.02f;
                camera->inner->position[1] = D_00444970[1] + sdfSin(phase * direction) * 2.0f;
            }
        }
        if (D_004360AC == 2 || D_004360AC == -2) {
            phase += 11.0f;
            if (phase > 0.0f) {
                phase -= 1.0f;
            } else if (phase > -3.14f) {
                phase -= 0.2f;
                camera->inner->position[1] = D_00444970[1] + sdfSin(phase * direction) * 2.0f;
            } else {
                phase -= 0.01f;
            }
            phase -= 11.0f;
        }
        D_004360B0 = phase;
        dds3OnPoseSetFlag(camera->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
    }
}

void fldClearCameraMoveMode(void) {
    D_004360AC = 0;
}

extern f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz);
extern void fldSetCameraObjectHighlightFlag(void);
extern void fldClearSceneModelColors(void);
extern void fldMarkPrimaryObjectByPeerPresence(void);
extern void fldClearCameraObjectTransitionFlags(void);
extern void fldClearCameraObjectHighlightFlag(void);

extern u8 fldEncountTestFlag(u32 mask);

typedef struct FldModelMatrices {
    u8 pad00[0xC];
    u8 **rows; /* 0x0C: slot-indexed matrix blocks */
} FldModelMatrices;

void fldUpdateCameraProximity(void) {
    f32 vec[4];
    f32 range = 45.0f;
    s32 slot;
    u8 *model;
    u8 **modelRef;
    u8 **matrices;

    memset(vec, 0, sizeof(vec));
    vec[3] = 1.0f;
    if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x40) {
        if (fldEncountTestFlag(0x40) == 0) {
            return;
        }
    }
    slot = dds3GetBaseControlWorkData(fldPlayerObject)->resourceSlots[4];
    modelRef = (u8 **)fldPlayerModelContext->inner;
    if (evtBitCheck(0x31)) {
        range = 56.0f;
        slot = 7;
    }
    if (slot >= 0) {
        model = *modelRef;
        matrices = ((FldModelMatrices *)model)->rows;
        VU0_LOAD_MATRIX(matrices[slot] + 0xC0);
        SDF_VU0_LOADVECTOR(vec);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(vec);
        if (fldPointDistance(vec[0], vec[1], vec[2], fldCamTarget[0], fldCamTarget[1], fldCamTarget[2]) < range) {
            fldEncountTestFlag(0x40);
            fldClearSceneModelColors();
            fldClearCameraObjectTransitionFlags();
            fldClearCameraObjectHighlightFlag();
        } else {
            fldMarkPrimaryObjectByPeerPresence();
            if (fldGlobalBase.mode == FLD_CAMERA_MODE_OBJECT_FOCUS || fldGlobalBase.mode == FLD_CAMERA_MODE_ALTERNATE_FOCUS) {
                fldClearCameraObjectHighlightFlag();
            } else if (fldGlobalBase.dist < 100.0f) {
                fldSetCameraObjectHighlightFlag();
            } else {
                fldClearCameraObjectHighlightFlag();
            }
        }
    }
}

extern void fldRestoreSceneModelColors(void);
extern void fldCamElvTask(void);
extern void func_0012FA58(void);
extern void func_001302A0(void);
extern void fldCamUpdateFollow(void);

s32 fldUpdateCameraFollow(void) {
    FldAreaWork *cam;
    dds3Basic_t **world = fldGetPlayerSceneStateAddress();
    if (*world != 0 && fldPlayerObject != 0) {
        fldRestoreSceneModelColors();
        if (fldCamNowEveCamera() != 0) {
            fldToggleWorldNodeState(1);
            fldCamElvTask();
            fldClearCameraObjectHighlightFlag();
            return 0;
        }
        fldProbeCameraSupportSurface();
        cam = &fldGlobalBase;
        dds3CameraObjectBasicSetFovy(*world, fldCameraFollowRows[cam->rowIdx].fov * 3.14f / 180.0f);
        switch (cam->mode) {
        case FLD_CAMERA_MODE_PLAYER_FOLLOW:
            func_0012FA58();
            if (cam->dist < 50.0f) {
                func_0012FA58();
            }
            break;
        case FLD_CAMERA_MODE_OBJECT_FOCUS:
            func_001302A0();
            break;
        case FLD_CAMERA_MODE_FOLLOW_UPDATE:
            fldCamUpdateFollow();
            break;
        }
        fldUpdateCameraProximity();
    }
    return 0;
}

s32 fldSyncObjectFlagsB(void) {
    s32 result;

    if (fldPlayerObject == 0) {
        return 0;
    }
    if (fldSecondarySceneObject == 0) {
        return 0;
    }
    result = dds3ObjectBaseCheckFlag(fldPlayerObject, 1);
    if (result != 0) {
        dds3ObjectBaseOnFlag(fldSecondarySceneObject, 1);
        result = 0;
    }
    return result;
}

void fldResetCameraModelHandles(void) {
    D_004360C8 = 0;
    D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
    D_004360CC = 0;
    D_004360D0 = 0;
}

void fldReleaseCameraModel(u32 enabled) {
    if (enabled == 0) {
        D_004360D0 = 0;
        if (fldPlayerModelContext != 0) {
            mdlMotionPauseOff(fldPlayerModelContext);
        }
    } else {
        D_004360D0 = enabled;
        mdlMotionPause(fldPlayerModelContext);
    }
}

void fldSetCameraModelMotionRequest(FldCameraModelMotionRequest request,
                                    u32 frameCountdown) {
    D_004360C4 = request;
    D_004360C8 = frameCountdown;
}

void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames) {
    s32 currentMotion;

    if (D_00389888[0] == 1) {
        if (D_004360D0 > 0) {
            if (--D_004360D0 != 0) {
                return;
            }
            mdlMotionPauseOff(fldPlayerModelContext);
        }
        currentMotion = D_004360CC;
        switch (requestedMotionIndex) {
        case 1:
            break;
        case 2:
            if (D_004360C8 > 0) {
                requestedMotionIndex = 3;
            }
            break;
        case 3:
            if (currentMotion == 0x66) {
                requestedMotionIndex = 2;
                currentMotion = 2;
            }
            break;
        default:
            if (currentMotion == 0x66) {
                requestedMotionIndex = 2;
                currentMotion = 2;
            } else {
                requestedMotionIndex = 3;
            }
            break;
        }
        if (D_004360C4 == 1) {
            mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 1.0f);
            mdlSmoothLoopMotion(fldPlayerModelContext, 1, requestedMotionIndex, blendFrames, blendFrames);
            D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
            if (requestedMotionIndex == 2 && D_004360CC == 3) {
                D_004360CC = requestedMotionIndex;
                fldPlayerModelContext->first->frameStep = 1.0f;
                mdlSmoothLoopMotion(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
            }
        } else if (D_004360C4 == 0x66) {
            mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 1.0f);
            mdlSmoothSingleMotion(fldPlayerModelContext, 1, 2, 0.0f, blendFrames);
            D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
            D_004360CC = 0x66;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlSmoothLoopMotion(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
        } else if (D_004360C4 == 2) {
            mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 1.0f);
            mdlSmoothSingleMotion(fldPlayerModelContext, 1, 2, 0.0f, blendFrames);
            D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
            D_004360CC = 2;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlSmoothLoopMotion(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
        } else if (D_004360C4 == 3) {
            mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 1.0f);
            mdlSmoothSingleMotion(fldPlayerModelContext, 1, 3, 0.0f, blendFrames);
            D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
            D_004360CC = 3;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlSmoothLoopMotion(fldPlayerModelContext, 0, 3, blendFrames, blendFrames);
        } else if (D_004360C4 == 5) {
            mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 1.0f);
            mdlSmoothSingleMotion(fldPlayerModelContext, 1, 5, 0.0f, blendFrames);
            D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
            D_004360CC = 5;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlSmoothLoopMotion(fldPlayerModelContext, 0, 5, blendFrames, blendFrames);
        } else if (D_004360C4 == 6) {
            if (requestedMotionIndex == 1) {
                mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 2.0f);
                mdlSingleMotion(fldPlayerModelContext, 1, 6);
                D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
            } else {
                mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 2.0f);
                mdlSingleMotion(fldPlayerModelContext, 1, 4);
                D_004360C4 = FLD_CAMERA_MODEL_MOTION_REQUEST_NONE;
                D_004360CC = 4;
                fldPlayerModelContext->first->frameStep = 1.0f;
                mdlSmoothLoopMotion(fldPlayerModelContext, 0, 4, blendFrames, blendFrames);
            }
        } else if (currentMotion != requestedMotionIndex && D_004360CC != 5) {
            D_004360CC = requestedMotionIndex;
            fldPlayerModelContext->first->frameStep = 1.5f;
            mdlSmoothLoopMotion(fldPlayerModelContext, 0, requestedMotionIndex, blendFrames, blendFrames);
            if (D_004360C8 == 0) {
                mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 1.0f);
                mdlSmoothLoopMotion(fldPlayerModelContext, 1, requestedMotionIndex, blendFrames, blendFrames);
            }
        }
        if (D_004360C8 > 0) {
            D_004360C8--;
        }
    } else if (currentMotionIndex != requestedMotionIndex) {
        fldPlayerModelContext->first->frameStep = 1.0f;
        mdlSmoothLoopMotion(fldPlayerModelContext, 0, requestedMotionIndex, blendFrames, blendFrames);
        if (fldSecondarySceneObject != 0) {
            fldSecondarySceneModelContext->first->frameStep = 1.0f;
            mdlSmoothLoopMotion(fldSecondarySceneModelContext, 0, requestedMotionIndex, blendFrames, blendFrames);
        }
    }
}

struct EffRandState;
extern u32 dds3GetRandA(struct EffRandState *state);
extern void fldSetSequenceVolume(s32 category, s32 volume);
extern void fldQueuePrimaryEffectPosition(f32 x, f32 y, f32 z);
extern void fldQueueSecondaryEffectPosition(f32 x, f32 y, f32 z);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *model, s32 id);

/* Dispatch camera-model frame crossings to sequence sounds and queued effects. */
void fldDispatchCameraModelFrameEvents(s32 motion, s32 selector) {
    union {
        u128 q;
        f32 f[4];
    } position;
    dds3ModelHandle_t *cameraModel;
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
    previousFrame = fldGlobalBase.previousCameraModelFrame;
    currentFrame = cameraModel->first->currentFrame;
    fldGlobalBase.previousCameraModelFrame = currentFrame;

    if (fldGlobalBase.unk8C == 0) {
        switch (fldGlobalBase.selectedRoomIndex) {
        case 1: sequenceCategory = 0x14; break;
        case 2: sequenceCategory = 0x18; break;
        case 3: sequenceCategory = 0x1C; break;
        default: sequenceCategory = 0x10; break;
        }
    } else {
        switch (fldGlobalBase.unk8C - 1) {
        case 1: sequenceCategory = 0x14; break;
        case 2: sequenceCategory = 0x18; break;
        case 3: sequenceCategory = 0x1C; break;
        default: sequenceCategory = 0x10; break;
        }
    }

    if (fldGlobalBase.mode == FLD_CAMERA_MODE_OBJECT_FOCUS) {
        dx = fldCamTarget[0] - fldLookAtNearPoint[0];
        dy = fldCamTarget[1] - fldLookAtNearPoint[1];
        dz = fldCamTarget[2] - fldLookAtNearPoint[2];
        distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 500.0f;
        if (distance > 7000.0f) {
            distance = 7000.0f;
        }
        if (distance < 0.0f) {
            distance = 0.0f;
        }
        volume = ((7000 - (s32)distance) * 127) / 7000;
        if (volume >= 128) {
            volume = 127;
        }
        if (volume < 0) {
            volume = 0;
        }
    } else {
        volume = 127;
        volume -= (dds3GetRandA(NULL) >> 2) & 0x3F;
        if (volume >= 128) {
            volume = 127;
        }
        if (volume < 0) {
            volume = 0;
        }
    }

    if (fldGlobalBase.playerModelAlternate == 1) {
        sequenceCategory = (D_004360D4 % 2) + 0x19;
        if (selector != 1) {
            return;
        }
        if ((previousFrame < 7.0f && currentFrame >= 7.0f) ||
            (previousFrame < 17.0f && currentFrame >= 17.0f)) {
            p2sdPlaySE(0x680000 + sequenceCategory, 0x7F, 0x3F);
            D_004360D4 += (dds3GetRandA(NULL) >> 4 & 1) + 1;
        }
        return;
    }

    if (motion != 0 && motion != 2 && motion != 3 && selector == 2) {
        fldSetSequenceVolume(sequenceCategory + 3, volume);
        fldQueueSecondaryEffectPosition(fldGlobalBase.x, fldGlobalBase.y - 5.0f, fldGlobalBase.z);
        return;
    }
    sequenceCategory += D_004360D4 % 3;

    if (selector == 1) {
        if (previousFrame < 8.0f && currentFrame >= 8.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (dds3GetRandA(NULL) >> 4 & 1) + 1;
            if (sdfModelGetMarkPos(fldPlayerModelContext->inner, 0xCA) == 0) {
                return;
            }
            SDF_VU0_STOREVECTOR(&position);
            fldQueueSecondaryEffectPosition(position.f[0], fldGlobalBase.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 18.0f && currentFrame >= 18.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (dds3GetRandA(NULL) >> 4 & 1) + 1;
            if (sdfModelGetMarkPos(fldPlayerModelContext->inner, 0xC9) == 0) {
                return;
            }
            SDF_VU0_STOREVECTOR(&position);
            fldQueuePrimaryEffectPosition(position.f[0], fldGlobalBase.y - 5.0f, position.f[2]);
            return;
        }
    } else if (selector == 0x17) {
        if (previousFrame < 88.0f && currentFrame >= 88.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (dds3GetRandA(NULL) >> 4 & 1) + 1;
            if (sdfModelGetMarkPos(fldPlayerModelContext->inner, 0xCA) == 0) {
                return;
            }
            SDF_VU0_STOREVECTOR(&position);
            fldQueueSecondaryEffectPosition(position.f[0], fldGlobalBase.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 18.0f && currentFrame >= 18.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (dds3GetRandA(NULL) >> 4 & 1) + 1;
            if (sdfModelGetMarkPos(fldPlayerModelContext->inner, 0xC9) == 0) {
                return;
            }
            SDF_VU0_STOREVECTOR(&position);
            fldQueuePrimaryEffectPosition(position.f[0], fldGlobalBase.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 42.0f && currentFrame >= 42.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (dds3GetRandA(NULL) >> 4 & 1) + 1;
            if (sdfModelGetMarkPos(fldPlayerModelContext->inner, 0xCA) == 0) {
                return;
            }
            SDF_VU0_STOREVECTOR(&position);
            fldQueueSecondaryEffectPosition(position.f[0], fldGlobalBase.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 63.0f && currentFrame >= 63.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (dds3GetRandA(NULL) >> 4 & 1) + 1;
            if (sdfModelGetMarkPos(fldPlayerModelContext->inner, 0xC9) == 0) {
                return;
            }
            SDF_VU0_STOREVECTOR(&position);
            fldQueuePrimaryEffectPosition(position.f[0], fldGlobalBase.y - 5.0f, position.f[2]);
        }
    }
}

/* Probe the current player position; consume a pending target into work/object state. */
void fldUpdateCameraTarget(void) {
    union {
        u128 q;
        f32 f[4];
    } vec;
    f32 cur[3];
    FldAreaWork *st;
    u128 *dst;

    if (fldPlayerObject != 0 &&
        (st = &fldGlobalBase, st->modeOneCameraState != FLD_MODE_ONE_CAMERA_ACTIVE) && D_00435F30 != 0) {
        cur[0] = st->x;
        cur[1] = st->y;
        cur[2] = st->z;
        fldHitChkUnitElv(cur);
        if (st->positionPending != FLD_POSITION_UPDATE_NONE) {
            st->x = st->targetX;
            st->y = st->targetY;
            st->z = st->targetZ;
            vec.f[0] = st->targetX;
            vec.f[1] = st->targetY;
            vec.f[2] = st->targetZ;
            dds3SetPosPoseSet(fldPlayerObject, (u128 *)vec.f);
            st->positionPending = FLD_POSITION_UPDATE_NONE;
            dds3GetPosPoseSet(fldPlayerObject);
            SDF_VU0_STOREVECTOR(&vec);
            dst = (u128 *)fldPlayerObject->inner->smoothedPosition;
            SDF_VU0_COPYVECTOR(dst, &vec);
        }
    }
}

extern const PcpVectorCopyF32 D_00413280;
extern f32 fldAngleDifference(f32 fromAngle, f32 toAngle);
extern void FromRotation(f32 angle);
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

    axis = D_00413280;
    if (fldGlobalBase.modeOneCameraState != FLD_MODE_ONE_CAMERA_ACTIVE) {
        return;
    }
    dds3GetPosPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR_NOMEM(position);
    fldGlobalBase.previousX = fldGlobalBase.x;
    fldGlobalBase.previousY = fldGlobalBase.y;
    fldGlobalBase.previousZ = fldGlobalBase.z;
    fldGlobalBase.x = position[0];
    fldGlobalBase.y = position[1];
    fldGlobalBase.z = position[2];
    if (D_00435F30 != 0) {
        dds3OnPoseSetFlag(fldPlayerObject->inner,
                          OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
        if (D_00435F30 != 0) {
            if (fldGlobalBase.cameraSupportHeightDelta > 0.0f) {
                fldGlobalBase.y = position[1] + 5.0f;
            } else {
                fldGlobalBase.y = position[1] + 10.0f;
            }
        }
    }
    while (fldGlobalBase.negatedAngle >= 360.0f) {
        fldGlobalBase.negatedAngle -= 360.0f;
    }
    while (fldGlobalBase.negatedAngle < 0.0f) {
        fldGlobalBase.negatedAngle += 360.0f;
    }
    while (fldGlobalBase.angle >= 360.0f) {
        fldGlobalBase.angle -= 360.0f;
    }
    while (fldGlobalBase.angle < 0.0f) {
        fldGlobalBase.angle += 360.0f;
    }
    while (fldGlobalBase.modeOneCameraHeadingTarget >= 360.0f) {
        fldGlobalBase.modeOneCameraHeadingTarget -= 360.0f;
    }
    while (fldGlobalBase.modeOneCameraHeadingTarget < 0.0f) {
        fldGlobalBase.modeOneCameraHeadingTarget += 360.0f;
    }
    dds3GetRotPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR_NOMEM(quaternion);
    EulerAngleAxisY();
    VU0_LOAD_MATRIX(sdfViewMatrix);
    InversMatrix();
    motion = 23;
    difference = fldAngleDifference(fldGlobalBase.angle, fldGlobalBase.modeOneCameraHeadingTarget);
    if (difference < 0.0f) {
        difference = -difference;
    }
    /* Native literal is 0x3D9F49F4; do not replace by rounded 14/180. */
    distance = (180.0f - difference) * 0x1.3e93e8p-4f;
    position[0] = fldGlobalBase.x - sdfSin(-fldGlobalBase.modeOneCameraHeadingTarget * 3.14f / 180.0f) * distance;
    position[1] = fldGlobalBase.y;
    position[2] = fldGlobalBase.z - sdfCos(-fldGlobalBase.modeOneCameraHeadingTarget * 3.14f / 180.0f) * distance;
    dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
    if (D_00435F30 != 0) {
        fldGlobalBase.x = position[0];
        fldGlobalBase.y = position[1];
        fldGlobalBase.z = position[2];
        current[0] = fldGlobalBase.x;
        current[1] = fldGlobalBase.y;
        current[2] = fldGlobalBase.z;
        fldRefreshAreaStateFromPosition(current);
        if (fldGlobalBase.positionPending != FLD_POSITION_UPDATE_NONE) {
            fldGlobalBase.x = fldGlobalBase.targetX;
            fldGlobalBase.y = fldGlobalBase.targetY + 44.0f;
            fldGlobalBase.z = fldGlobalBase.targetZ;
            position[0] = fldGlobalBase.x;
            position[1] = fldGlobalBase.y;
            position[2] = fldGlobalBase.z;
            dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
            fldGlobalBase.positionPending = FLD_POSITION_UPDATE_NONE;
        }
        dds3GetPosPoseSet(fldPlayerObject);
        SDF_VU0_STOREVECTOR_NOMEM(position);
        PCP_COPY_VECTOR_F32(smoothed, fldPlayerObject->inner->smoothedPosition);
        if (ffabsf(smoothed[0] - position[0]) < 1.0f) {
            fldPlayerObject->inner->smoothedPosition[0] = position[0];
        } else {
            fldPlayerObject->inner->smoothedPosition[0] =
                smoothed[0] + (position[0] - smoothed[0]) / 3.0f;
        }
        if (ffabsf(smoothed[1] - position[1]) < 1.0f) {
            fldPlayerObject->inner->smoothedPosition[1] = position[1];
        } else {
            fldPlayerObject->inner->smoothedPosition[1] =
                smoothed[1] + (position[1] - smoothed[1]) / 3.0f;
        }
        if (ffabsf(smoothed[2] - position[2]) < 1.0f) {
            fldPlayerObject->inner->smoothedPosition[2] = position[2];
        } else {
            fldPlayerObject->inner->smoothedPosition[2] =
                smoothed[2] + (position[2] - smoothed[2]) / 3.0f;
        }
        fldPlayerObject->inner->smoothedPosition[3] = 1.0f;
    }
    difference = fldAngleDifference(fldGlobalBase.angle, fldGlobalBase.modeOneCameraHeadingTarget);
    step = difference / 6.0f;
    if (step < 0.0f) {
        step = -step;
    }
    if (step < 2.0f) {
        step = 2.0f;
    }
    if ((0.0f <= difference && difference <= step) ||
        (difference <= 0.0f && -step <= difference)) {
        difference = fldGlobalBase.modeOneCameraHeadingTarget;
    } else if (difference < 0.0f) {
        difference = fldGlobalBase.angle - step;
    } else {
        difference = fldGlobalBase.angle + step;
    }
    fldGlobalBase.angle = difference;
    SDF_VU0_LOADVECTOR(axis.components);
    FromRotation(fldGlobalBase.angle * 3.14f / 180.0f);
    SDF_VU0_STOREVECTOR_NOMEM(quaternion);
    dds3SetRotPoseSet(fldPlayerObject, (u128 *)quaternion);
    if (fldPlayerModelContext->current.h.motionIndex != motion) {
        fldPlayerModelContext->first->frameStep = 1.0f;
        mdlSmoothLoopMotion(fldPlayerModelContext, 0, motion, 5.0f, 5.0f);
    }
    fldGlobalBase.x = position[0];
    fldGlobalBase.y = position[1];
    fldGlobalBase.z = position[2];
}


/* Signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */
/* Publish the vertical step to field state, then rebuild the object vector.
 * Each branch-local SDK snapshot dominates all of its scalar reads; there is
 * no earlier scratch read or loop-carried value. The finite qualification
 * and memory-contract limitation are documented in docs/idioms.md. */
s32 fldApplyPlayerVerticalStep(void) {
    f32 position[4] __attribute__((aligned(16)));
    if (fldGlobalBase.verticalStepDirection > 0) {
        dds3GetPosPoseSet(fldPlayerObject);
        SDF_VU0_STOREVECTOR_NOMEM(position);
        fldGlobalBase.previousX = fldGlobalBase.x;
        fldGlobalBase.previousY = fldGlobalBase.y;
        fldGlobalBase.previousZ = fldGlobalBase.z;
        fldGlobalBase.x = position[0];
        fldGlobalBase.y = position[1] - 2.0f;
        fldGlobalBase.z = position[2];
        position[0] = fldGlobalBase.x;
        position[1] = fldGlobalBase.y;
        position[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
        fldClearCameraObjectTransitionFlags();
        fldClearCameraObjectHighlightFlag();
    } else if (fldGlobalBase.verticalStepDirection < 0) {
        dds3GetPosPoseSet(fldPlayerObject);
        SDF_VU0_STOREVECTOR_NOMEM(position);
        fldGlobalBase.previousX = fldGlobalBase.x;
        fldGlobalBase.previousY = fldGlobalBase.y;
        fldGlobalBase.previousZ = fldGlobalBase.z;
        fldGlobalBase.x = position[0];
        fldGlobalBase.y = position[1] + 2.0f;
        fldGlobalBase.z = position[2];
        position[0] = fldGlobalBase.x;
        position[1] = fldGlobalBase.y;
        position[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
        fldClearCameraObjectTransitionFlags();
        fldClearCameraObjectHighlightFlag();
    } else {
        return 0;
    }
    return 0;
}


extern void mnuResetTitleStreamAfterFileIdle(void);

extern void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);
extern void fldEncountSetFlag(u32 mask);

typedef s32 FldMovementGateResult;
enum {
    FLD_MOVEMENT_GATE_PROCEED = 0,
    FLD_MOVEMENT_GATE_HELD = -1
};

/* Advance the field transition while the camera model's motion drives completion. */
FldMovementGateResult fldUpdateFieldTransition(void) {
    FldAreaWork *work = &fldGlobalBase;
    dds3ModelHandle_t *model;
    s16 node;

    if (work->fieldTransitionMovementGate > 0) {
        model = fldPlayerModelContext;
        node = model->current.h.motionIndex;
        if (node != 7) {
            dds3FadeIn2(8);
            mnuQueueTitleSoundStreamEntry(0x26);
            model = fldPlayerModelContext;
            model->first->frameStep = 1.0f;
            mdlSingleMotion(model, 0, 7);
        }

        {
            sdfMotionCtrlr_t *const thresholdMotion = fldPlayerModelContext->first;
            f32 currentFrame = thresholdMotion->currentFrame;
            if (work->previousCameraModelFrame < 13.0f && 13.0f <= currentFrame) {
                dds3PadVibrationSet(0, 1, 10);
                dds3PadVibrationSet(1, 0xE6, 10);
                mnuResetTitleStreamAfterFileIdle();
            }
        }
        {
            sdfMotionCtrlr_t *const completionMotion = fldPlayerModelContext->first;
            if (completionMotion->state == SDF_MOTION_STATE_TERMINAL) {
                mnuMarkTitleStreamResetPending();
                mnuResetTitleStreamLocked();
                fldGlobalBase.fieldTransitionMovementGate = 0;
                fldEncountSetFlag(0x40);
                return FLD_MOVEMENT_GATE_PROCEED;
            }
        }

        fldDispatchCameraModelFrameEvents(node, 7);
        return FLD_MOVEMENT_GATE_HELD;
    }

    return FLD_MOVEMENT_GATE_PROCEED;
}

extern u8 ptyAnyLivingUnitHasStatus(u32 statusMask, s32 frontlineOnly);
extern void fldPlayMenuSound(s32 soundId);
extern s32 fldPlaceAreaDamageEffect(f32 x, f32 y, f32 z);

FldMovementGateResult fldUpdateAreaDamageReaction(void) {
    FldAreaWork *work = &fldGlobalBase;
    dds3ModelHandle_t *model;

    if (work->areaDamageReactionMovementGate > 0) {
        if (work->terrainAttritionKind == FLD_TERRAIN_ATTRITION_NONE && ptyAnyLivingUnitHasStatus(0x80, 1) != 0) {
            fldPlayMenuSound(0x22);
            dds3FadeIn(0x80, 0x20, 0x20, 6);
            dds3PadVibrationSet(0, 1, 6);
            dds3PadVibrationSet(1, 0x96, 6);
            work->areaDamageReactionMovementGate = 0;
            fldEncountSetFlag(0x40);
            return FLD_MOVEMENT_GATE_HELD;
        }

        model = fldPlayerModelContext;
        if (model->current.h.motionIndex != 4) {
            model->first->frameStep = 1.0f;
            mdlSingleMotion(model, 0, 4);
            fldPlaceAreaDamageEffect(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
            fldPlayMenuSound(0x22);
            dds3PadVibrationSet(0, 1, 15);
            dds3PadVibrationSet(1, 0x64, 15);
            model = fldPlayerModelContext;
        }

        if (model->first->state == SDF_MOTION_STATE_TERMINAL) {
            fldGlobalBase.areaDamageReactionMovementGate = 0;
            fldEncountSetFlag(0x40);
        } else {
            return FLD_MOVEMENT_GATE_HELD;
        }
    }
    return FLD_MOVEMENT_GATE_PROCEED;
}

extern s8 dds3PadButtonState[2][2][16];
extern u8 D_00435C40[2][2][2];
extern s32 D_004360BC;
extern s32 D_004360C0;
extern s32 D_004360D8;
extern s32 D_004360DC;
extern f32 D_004360E0;
extern s32 D_004360E4;
extern f32 D_0038A610[12];
extern s32 fileTestSavedSlotFlags(u32);
extern f32 fldSnapAngleToCompassPoint(f32);
extern f32 fldPlayerCalc360_8(f32);
extern f32 fldPlayerCalc360_4(f32);
extern u8 fldIsNearSpark(f32 x, f32 y, f32 z);
extern void fldHitChkUnitDamage(f32 *);
extern void fldRestoreSceneModelColors(void);
extern void fldClearSceneModelColors(void);
extern void fldShowSceneModels(void);
extern void fldMarkPrimaryObjectByPeerPresence(void);
extern void fldSetCameraObjectHighlightFlag(void);
extern s32 fldIsCurrentGridGuideMarkerActive(void);
extern void fldGetFieldFloatPair(f32 *, f32 *);

/* Field movement, player pose and camera input controller. */
INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413280);

s32 fldUpdateFieldPlayerMovement(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 smoothed[4] __attribute__((aligned(16)));
    f32 quaternion[4] __attribute__((aligned(16)));
    PcpVectorCopyF32 axis = {{0.0f, 1.0f, 0.0f, 1.0f}};
    f32 directionAngles[16] = {
        0.0f, -22.5f, -45.0f, -67.5f, -90.0f, -112.5f, -135.0f, -157.5f,
        180.0f, 157.5f, 135.0f, 112.5f, 90.0f, 67.5f, 45.0f, 22.5f
    };
    f32 current[3];
    /* The guide query writes one float through each output pointer. */
    f32 guideX[1];
    f32 guideZ[1];
    f32 heading;
    f32 difference;
    f32 step;
    f32 distance;
    f32 radians;
    s32 direction = -1;
    s32 snapInput = 0;
    s32 motion;
    s16 oldMotion;

    if (D_004360C0 > 0) {
        D_004360C0--;
    }
    if (fldUpdateFieldTransition() == FLD_MOVEMENT_GATE_HELD) {
        return 0;
    }
    if (fldUpdateAreaDamageReaction() == FLD_MOVEMENT_GATE_HELD) {
        return 0;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        return 0;
    }
    if (D_004360D0 != 0) {
        fldUpdateCameraModelMotion(0, 0, 6.0f);
        return 0;
    }

    if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x100) {
        if (D_00435C40[0][0][1] < 0x40) {
            if (fileTestSavedSlotFlags(3) != 0) {
                fldGlobalBase.cameraTilt -= 10.0f;
            } else {
                fldGlobalBase.cameraTilt += 10.0f;
            }
            fldGlobalBase.cameraTiltActive = 1;
        }
        if (D_00435C40[0][0][1] > 0xC0) {
            if (fileTestSavedSlotFlags(3) != 0) {
                fldGlobalBase.cameraTilt += 10.0f;
            } else {
                fldGlobalBase.cameraTilt -= 10.0f;
            }
            fldGlobalBase.cameraTiltActive = 1;
        }
        if (fldGlobalBase.cameraTilt < -150.0f) {
            fldGlobalBase.cameraTilt = -150.0f;
        }
        if (fldGlobalBase.cameraTilt > 150.0f) {
            fldGlobalBase.cameraTilt = 150.0f;
        }
        if (dds3PadButtonState[1][0][3] < 0) {
            fldGlobalBase.cameraTiltActive = 0;
            fldGlobalBase.cameraTilt = 0.0f;
        }
    }

    if (D_00435C40[0][1][0] < 0x40 || D_00435C40[0][1][0] > 0xC0 ||
        D_00435C40[0][1][1] < 0x40 || D_00435C40[0][1][1] > 0xC0) {
        f32 x = D_00435C40[0][1][0] - 128;
        f32 y = D_00435C40[0][1][1] - 128;
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
        if (dds3PadButtonState[1][0][4] != 0 && dds3PadButtonState[1][0][6] != 0) {
            direction = 14;
        } else if (dds3PadButtonState[1][0][4] != 0 && dds3PadButtonState[1][0][7] != 0) {
            direction = 10;
        } else if (dds3PadButtonState[1][0][5] != 0 && dds3PadButtonState[1][0][6] != 0) {
            direction = 2;
        } else if (dds3PadButtonState[1][0][5] != 0 && dds3PadButtonState[1][0][7] != 0) {
            direction = 6;
        } else if (dds3PadButtonState[1][0][4] != 0) {
            direction = 12;
        } else if (dds3PadButtonState[1][0][5] != 0) {
            direction = 4;
        } else if (dds3PadButtonState[1][0][6] != 0) {
            direction = 0;
        } else if (dds3PadButtonState[1][0][7] != 0) {
            direction = 8;
        }
    }
    if (dds3PadButtonState[1][0][3] < 0) {
        D_004360D8 = 0;
        D_004360E4 = 0;
    }
    if (D_004360D8 != 0) {
        D_004360E4++;
        if (D_004360DC != direction) {
            D_004360D8 = 0;
        } else if (D_004360E4 > 12) {
            D_004360D8 = 0;
        }
    }
    if (dds3PadButtonState[1][0][3] != 0 || (dds3PadButtonState[1][0][8] != 0 && dds3PadButtonState[1][0][10] != 0)) {
        if (fldGlobalBase.mode != FLD_CAMERA_MODE_OBJECT_FOCUS && fldGlobalBase.mode != FLD_CAMERA_MODE_ALTERNATE_FOCUS) {
            snapInput = 1;
        }
    }
    if (fldGlobalBase.sceneState > 0) {
        direction = -1;
        snapInput = 0;
    }

    dds3GetPosPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR_NOMEM(position);
    fldGlobalBase.previousX = fldGlobalBase.x;
    fldGlobalBase.previousY = fldGlobalBase.y;
    fldGlobalBase.previousZ = fldGlobalBase.z;
    fldGlobalBase.x = position[0];
    fldGlobalBase.y = position[1];
    fldGlobalBase.z = position[2];
    while (fldGlobalBase.negatedAngle >= 360.0f) {
        fldGlobalBase.negatedAngle -= 360.0f;
    }
    while (fldGlobalBase.negatedAngle < 0.0f) {
        fldGlobalBase.negatedAngle += 360.0f;
    }
    while (fldGlobalBase.angle >= 360.0f) {
        fldGlobalBase.angle -= 360.0f;
    }
    while (fldGlobalBase.angle < 0.0f) {
        fldGlobalBase.angle += 360.0f;
    }
    dds3GetRotPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR_NOMEM(quaternion);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    InversMatrix();
    dds3OnPoseSetFlag(fldPlayerObject->inner,
                      OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
    if (fldSecondarySceneObject != 0) {
        dds3OnPoseSetFlag(fldSecondarySceneObject->inner,
                          OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
    }
    if (direction != -1 && D_00435F30 != 0) {
        if (fldGlobalBase.cameraSupportHeightDelta > 0.0f) {
            fldGlobalBase.y = position[1] + 5.0f;
        } else {
            fldGlobalBase.y = position[1] + 10.0f;
        }
    }
    if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x40) {
        motion = 18;
    } else {
        motion = 2;
    }
    oldMotion = fldPlayerModelContext->current.h.motionIndex;
    dds3GetPosPoseSet(fldPlayerObject);
    heading = fldGlobalBase.targetAngle;
    if (direction != -1) {
        heading = directionAngles[direction] - fldGlobalBase.negatedAngle;
        if (D_004360D8 != 0) {
            heading = D_004360E0;
        } else {
            D_004360E0 = heading;
        }
        if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x40) {
            motion = 17;
        } else {
            motion = 1;
        }
        if (fldIsCurrentGridGuideMarkerActive() != 0) {
            fldGetFieldFloatPair(guideX, guideZ);
            motion = 16;
            fldGlobalBase.x -= (fldGlobalBase.x - guideX[0]) * 0.25f;
            fldGlobalBase.z -= (fldGlobalBase.z - guideZ[0]) * 0.25f;
            position[0] = fldGlobalBase.x;
            position[1] = fldGlobalBase.y;
            position[2] = fldGlobalBase.z;
            dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
            if (fldGlobalBase.movementSoundActive == 0) {
                p2sdPlaySE(0x690062, 0x7F, 0x3F);
            }
            fldGlobalBase.movementSoundActive = 1;
        } else {
            fldGlobalBase.movementSoundActive = 0;
            if (fldGlobalBase.playerModelAlternate != 0) {
                distance = 32.0f;
            } else {
                distance = 25.0f;
            }
            if (fldGlobalBase.cameraSupportHeightDelta > 0.0f) {
                distance += ffabsf(fldGlobalBase.cameraSupportHeightDelta) / 40.0f + 2.0f;
            }
            difference = fldAngleDifference(fldGlobalBase.angle, heading);
            if (difference < 0.0f) {
                difference = -difference;
            }
            distance = (distance / 180.0f) * (180.0f - difference);
            radians = -heading * 3.14f / 180.0f;
            position[0] = fldGlobalBase.x - sdfSin(radians) * distance;
            position[1] = fldGlobalBase.y;
            position[2] = fldGlobalBase.z - sdfCos(radians) * distance;
            dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
        }
    } else {
        if (fldIsCurrentGridGuideMarkerActive() != 0) {
            fldGetFieldFloatPair(guideX, guideZ);
            motion = 16;
            fldGlobalBase.x -= (fldGlobalBase.x - guideX[0]) * 0.25f;
            fldGlobalBase.z -= (fldGlobalBase.z - guideZ[0]) * 0.25f;
            if (fldGlobalBase.movementSoundActive == 0) {
                p2sdPlaySE(0x690062, 0x7F, 0x3F);
            }
            fldGlobalBase.movementSoundActive = 1;
        } else {
            fldGlobalBase.movementSoundActive = 0;
        }
        position[0] = fldGlobalBase.x;
        position[1] = fldGlobalBase.y;
        position[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
    }
    while (heading >= 360.0f) {
        heading -= 360.0f;
    }
    while (heading < 0.0f) {
        heading += 360.0f;
    }
    fldGlobalBase.targetAngle = fldSnapAngleToCompassPoint(heading);
    if (fldGlobalBase.specialEventActive != 0 && direction != -1 &&
        fldIsNearSpark(position[0], position[1], position[2]) != 0) {
        position[0] = fldGlobalBase.x;
        position[1] = fldGlobalBase.y;
        position[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
    }
    if (D_00435F30 != 0) {
        if (direction != -1) {
            fldGlobalBase.x = position[0];
            fldGlobalBase.y = position[1];
            fldGlobalBase.z = position[2];
            current[0] = fldGlobalBase.x;
            current[1] = fldGlobalBase.y;
            current[2] = fldGlobalBase.z;
            fldRefreshAreaStateFromPosition(current);
            if (fldGlobalBase.positionPending != FLD_POSITION_UPDATE_NONE) {
                fldGlobalBase.x = fldGlobalBase.targetX;
                fldGlobalBase.y = fldGlobalBase.targetY + 44.0f;
                fldGlobalBase.z = fldGlobalBase.targetZ;
                position[0] = fldGlobalBase.x;
                position[1] = fldGlobalBase.y;
                position[2] = fldGlobalBase.z;
                dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
                fldGlobalBase.positionPending = FLD_POSITION_UPDATE_NONE;
            }
        }
        dds3GetPosPoseSet(fldPlayerObject);
        SDF_VU0_STOREVECTOR_NOMEM(position);
        PCP_COPY_VECTOR_F32(smoothed, fldPlayerObject->inner->smoothedPosition);
        if (ffabsf(smoothed[0] - position[0]) < 1.0f) {
            fldPlayerObject->inner->smoothedPosition[0] = position[0];
        } else {
            fldPlayerObject->inner->smoothedPosition[0] =
                smoothed[0] + (position[0] - smoothed[0]) / 3.0f;
        }
        if (ffabsf(smoothed[1] - position[1]) < 1.0f) {
            fldPlayerObject->inner->smoothedPosition[1] = position[1];
        } else {
            fldPlayerObject->inner->smoothedPosition[1] =
                smoothed[1] + (position[1] - smoothed[1]) / 3.0f;
        }
        if (ffabsf(smoothed[2] - position[2]) < 1.0f) {
            fldPlayerObject->inner->smoothedPosition[2] = position[2];
        } else {
            fldPlayerObject->inner->smoothedPosition[2] =
                smoothed[2] + (position[2] - smoothed[2]) / 3.0f;
        }
        fldPlayerObject->inner->smoothedPosition[3] = 1.0f;
        if (fldSecondarySceneObject != 0) {
            dds3PoseSet_t *source = fldPlayerObject->inner;
            dds3PoseSet_t *secondary = fldSecondarySceneObject->inner;
            secondary->smoothedPosition[0] = source->smoothedPosition[0];
            secondary->smoothedPosition[1] = source->smoothedPosition[1] + 0.0f;
            secondary->smoothedPosition[2] = source->smoothedPosition[2];
            secondary->smoothedPosition[3] = source->smoothedPosition[3];
        }
    }
    fldHitChkUnitDamage(position);
    difference = fldAngleDifference(fldGlobalBase.angle, heading);
    step = difference / 5.0f;
    if (step < 0.0f) {
        step = -step;
    }
    if (step < 2.0f) {
        step = 2.0f;
    }
    if (fldIsCurrentGridGuideMarkerActive() != 0) {
        step *= 0.25f;
    }
    if ((0.0f <= difference && difference <= step) ||
        (difference <= 0.0f && -step <= difference)) {
        difference = heading;
    } else if (difference < 0.0f) {
        difference = fldGlobalBase.angle - step;
    } else {
        difference = fldGlobalBase.angle + step;
    }
    fldGlobalBase.angle = difference;
    SDF_VU0_LOADVECTOR(axis.components);
    FromRotation(fldGlobalBase.angle * 3.14f / 180.0f);
    SDF_VU0_STOREVECTOR_NOMEM(quaternion);
    dds3SetRotPoseSet(fldPlayerObject, (u128 *)quaternion);
    if (fldSecondarySceneObject != 0) {
        dds3SetRotPoseSet(fldSecondarySceneObject, (u128 *)quaternion);
    }
    if (fldGlobalBase.mode != FLD_CAMERA_MODE_ALTERNATE_FOCUS && fldGlobalBase.mode != FLD_CAMERA_MODE_OBJECT_FOCUS) {
        if (D_004360BC != 0) {
            difference = fldAngleDifference(fldGlobalBase.negatedAngle, fldGlobalBase.cameraTargetAngle);
            step = 30.0f;
            if ((0.0f <= difference && difference <= step) ||
                (difference <= 0.0f && -step <= difference)) {
                D_004360BC = 0;
                difference = fldGlobalBase.cameraTargetAngle;
            } else if (difference < 0.0f) {
                difference = fldGlobalBase.negatedAngle - step;
            } else {
                difference = fldGlobalBase.negatedAngle + step;
            }
            fldGlobalBase.negatedAngle = difference;
        }
        if (D_004360BC == 0) {
            if (dds3PadButtonState[1][0][3] < 0) {
                fldGlobalBase.cameraTargetAngle = -fldGlobalBase.targetAngle;
                fldGlobalBase.cameraTargetAngle = fldPlayerCalc360_4(fldGlobalBase.cameraTargetAngle);
                difference = fldAngleDifference(fldGlobalBase.cameraTargetAngle, fldGlobalBase.negatedAngle);
                if (difference < -100.0f || difference > 100.0f) {
                    D_004360C0 = 2;
                    fldGlobalBase.negatedAngle = fldGlobalBase.cameraTargetAngle;
                } else {
                    D_004360BC = 1;
                }
                if (direction != -1) {
                    D_004360DC = direction;
                    D_004360D8 = 1;
                    D_004360E4 = 0;
                }
            }
        }
        if (fldGlobalBase.unkA0 != -1 && direction != -1 &&
            (s32)fldGlobalBase.negatedAngle != (s32)fldGlobalBase.cameraTargetAngle) {
            difference = fldAngleDifference(fldGlobalBase.negatedAngle, fldGlobalBase.cameraTargetAngle);
            step = 0.4f;
            if ((0.0f <= difference && difference <= step) ||
                (difference <= 0.0f && -step <= difference)) {
                difference = fldGlobalBase.cameraTargetAngle;
            } else if (difference < 0.0f) {
                difference = fldGlobalBase.negatedAngle - step;
            } else {
                difference = fldGlobalBase.negatedAngle + step;
            }
            fldGlobalBase.negatedAngle = difference;
        }
        if (snapInput != 0) {
            if (dds3PadButtonState[1][0][8] < 0 && dds3PadButtonState[1][0][10] == 0) {
                fldGlobalBase.cameraTargetAngle -= 88.0f;
                fldGlobalBase.cameraTargetAngle = fldPlayerCalc360_4(fldGlobalBase.cameraTargetAngle);
                D_004360BC = 1;
                fldGlobalBase.targetAngle = -fldGlobalBase.cameraTargetAngle;
            } else if (dds3PadButtonState[1][0][10] < 0 && dds3PadButtonState[1][0][8] == 0) {
                fldGlobalBase.cameraTargetAngle += 88.0f;
                fldGlobalBase.cameraTargetAngle = fldPlayerCalc360_4(fldGlobalBase.cameraTargetAngle);
                D_004360BC = 1;
                fldGlobalBase.targetAngle = -fldGlobalBase.cameraTargetAngle;
            }
        } else {
            s32 reversed = fileTestSavedSlotFlags(3) == 0;
            if (dds3PadButtonState[1][0][8] != 0 && dds3PadButtonState[1][0][10] == 0) {
                fldGlobalBase.negatedAngle -= 4.5f;
            } else if (dds3PadButtonState[1][0][8] == 0 && dds3PadButtonState[1][0][10] != 0) {
                fldGlobalBase.negatedAngle += 4.5f;
            } else if (D_00435C40[0][0][0] < 0x20) {
                if (reversed) {
                    fldGlobalBase.negatedAngle += 3.0f;
                } else {
                    fldGlobalBase.negatedAngle -= 3.0f;
                }
            } else if (D_00435C40[0][0][0] < 0x40) {
                if (reversed) {
                    fldGlobalBase.negatedAngle += 1.5f;
                } else {
                    fldGlobalBase.negatedAngle -= 1.5f;
                }
            } else if (D_00435C40[0][0][0] > 0xE0) {
                if (reversed) {
                    fldGlobalBase.negatedAngle -= 3.0f;
                } else {
                    fldGlobalBase.negatedAngle += 3.0f;
                }
            } else if (D_00435C40[0][0][0] > 0xC0) {
                if (reversed) {
                    fldGlobalBase.negatedAngle -= 1.5f;
                } else {
                    fldGlobalBase.negatedAngle += 1.5f;
                }
            } else {
                goto camera_input_done;
            }
            fldGlobalBase.cameraTargetAngle = fldPlayerCalc360_8(fldGlobalBase.negatedAngle);
        }
    }
camera_input_done:
    dds3ObjectBaseSetControlBasicSvs(fldPlayerObject, 0);
    fldRestoreSceneModelColors();
    if (fldGlobalBase.dist < 20.0f) {
        fldClearSceneModelColors();
        fldShowSceneModels();
        fldClearCameraObjectTransitionFlags();
        fldClearCameraObjectHighlightFlag();
    } else {
        fldShowSceneModels();
        fldMarkPrimaryObjectByPeerPresence();
        if (fldGlobalBase.mode == FLD_CAMERA_MODE_OBJECT_FOCUS || fldGlobalBase.mode == FLD_CAMERA_MODE_ALTERNATE_FOCUS) {
            fldClearCameraObjectHighlightFlag();
        } else if (fldGlobalBase.dist < 100.0f) {
            fldSetCameraObjectHighlightFlag();
        } else {
            fldClearCameraObjectHighlightFlag();
        }
    }
    fldUpdateCameraModelMotion(oldMotion, motion, 6.0f);
    fldDispatchCameraModelFrameEvents(oldMotion, motion);
    fldGlobalBase.x = position[0];
    fldGlobalBase.y = position[1];
    fldGlobalBase.z = position[2];
    D_0038A610[0] = fldPlayerObject->inner->smoothedPosition[0];
    D_0038A610[1] = fldPlayerObject->inner->smoothedPosition[1];
    D_0038A610[2] = fldPlayerObject->inner->smoothedPosition[2];
    D_0038A610[3] = fldPlayerObject->inner->smoothedPosition[3];
    D_0038A610[4] = fldPlayerObject->inner->rotation[0];
    D_0038A610[5] = fldPlayerObject->inner->rotation[1];
    D_0038A610[6] = fldPlayerObject->inner->rotation[2];
    D_0038A610[7] = fldPlayerObject->inner->rotation[3];
    return 0;
}


extern s32 fldApplyPlayerVerticalStep(void);
extern void fldUpdateModeOneCameraPose(void);
extern void fldDispatchCameraModelFrameEvents(s32 motion, s32 selector);
extern s32 fldUpdateFieldPlayerMovement(void);
extern u32 fldRunDanteGunFlag[];
extern s32 D_003897C0[];

s32 fldUpdateCameraFrame(void) {
    s16 node;

    if (fldRunDanteGunFlag[0] != 0) {
        return 0;
    }
    if (fldGetSceneReadyFlag() != FLD_SCENE_NOT_READY) {
        return 0;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        if (fldGlobalBase.mode == FLD_CAMERA_MODE_OBJECT_FOCUS || fldGlobalBase.mode == FLD_CAMERA_MODE_ALTERNATE_FOCUS) {
            fldClearCameraObjectHighlightFlag();
        }
        fldApplyPlayerVerticalStep();
        fldUpdateCameraTarget();
        fldUpdateModeOneCameraPose();
        node = fldPlayerModelContext->current.h.motionIndex;
        fldDispatchCameraModelFrameEvents(node, node);
        if (fldGlobalBase.playerModelAlternate == 1) {
            fldUpdateCameraModelMotion(0, 0, 6.0f);
        }
        fldSyncObjectFlagsB();
        return 0;
    }
    if (D_003897C0[0] == 1) {
        fldClearCameraObjectHighlightFlag();
        fldUpdateFieldPlayerMovement();
        fldSyncObjectFlagsB();
    } else {
        fldUpdateFieldPlayerMovement();
        fldSyncObjectFlagsB();
    }
    return 0;
}

FldMovementHoldState fldGetMovementHoldState(void) {
    if (fldGlobalBase.fieldTransitionMovementGate == 0) {
        if (fldGlobalBase.areaDamageReactionMovementGate == 0) {
            if (fldGlobalBase.verticalStepDirection == 0) {
                return FLD_MOVEMENT_HOLD_CLEAR;
            }
        }
    }
    return FLD_MOVEMENT_HOLD_ACTIVE;
}

void fldSetCameraNodeModeWithZero(void);

void fldHideSceneModelsAndResetCamera(void) {
    dds3ObjectBaseOnFlag(fldPlayerObject, 1);
    if (fldSecondarySceneObject != 0) {
        dds3ObjectBaseOnFlag(fldSecondarySceneObject, 1);
    }
    fldSetCameraNodeModeWithZero();
}

void fldShowSceneModels(void) {
    dds3ObjectBaseOffFlag(fldPlayerObject, 1);
    if (fldSecondarySceneObject != 0) {
        dds3ObjectBaseOffFlag(fldSecondarySceneObject, 1);
        return;
    }
}

void fldClearCameraObjectHighlightFlag(void) {
    dds3ObjectBaseSetControlBasicSvs(fldPlayerObject, 0);
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x800);
}

void fldSetCameraObjectHighlightFlag(void) {
    dds3ObjectBaseSetControlBasicSvs(fldPlayerObject, 0x80);
    dds3ObjectBaseOnFlag(fldPlayerObject, 0x800);
}

void fldClearSceneModelColors(void) {
    s8 hasSecondObject;

    hasSecondObject = fldSecondarySceneObject != 0;
    fldPlayerModelContext->inner->color = 0;
    if (hasSecondObject) {
        fldSecondarySceneModelContext->inner->color = 0;
    }
}

void fldRestoreSceneModelColors(void) {
    s8 hasSecondObject;

    hasSecondObject = fldSecondarySceneObject != 0;
    fldPlayerModelContext->inner->color = 0x80808080;
    if (hasSecondObject) {
        fldSecondarySceneModelContext->inner->color = 0x80808080;
    }
}

void fldMarkPrimaryObjectByPeerPresence(void) {
    if (fldSecondarySceneObject != 0) {
        dds3ObjectBaseOnFlag(fldPlayerObject, 0x400);
        return;
    }
    dds3ObjectBaseOnFlag(fldPlayerObject, 0x200);
}

void fldClearCameraObjectTransitionFlags(void) {
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x400);
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x200);
}

extern void dds3OffPoseSetFlag(dds3PoseSet_t *, u32);
/* State-first XYZ publication; SDK snapshot audit: docs/idioms.md. */
s32 fldSynchronizePlayerPositionAndAreaState(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 current[3];
    dds3PoseSet_t *inner;

    fldGlobalBase.modeOneCameraState = FLD_MODE_ONE_CAMERA_INACTIVE;
    if (D_00435F30 == 0) return 0;
    dds3GetPosPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR_NOMEM(position);
    fldGlobalBase.previousX = fldGlobalBase.x;
    fldGlobalBase.previousY = fldGlobalBase.y;
    fldGlobalBase.previousZ = fldGlobalBase.z;
    fldGlobalBase.x = position[0];
    fldGlobalBase.y = position[1] + 10.0f;
    fldGlobalBase.z = position[2];
    current[0] = fldGlobalBase.x;
    current[1] = fldGlobalBase.y;
    current[2] = fldGlobalBase.z;
    fldRefreshAreaStateFromPosition(current);
    if (fldGlobalBase.positionPending != FLD_POSITION_UPDATE_NONE) {
        fldGlobalBase.x = fldGlobalBase.targetX;
        fldGlobalBase.y = fldGlobalBase.targetY + 44.0f;
        fldGlobalBase.z = fldGlobalBase.targetZ;
        position[0] = fldGlobalBase.x;
        position[1] = fldGlobalBase.y;
        position[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(fldPlayerObject, (u128 *)position);
        fldGlobalBase.positionPending = FLD_POSITION_UPDATE_NONE;
    }
    dds3GetPosPoseSet(fldPlayerObject);
    SDF_VU0_STOREVECTOR_NOMEM(position);
    inner = fldPlayerObject->inner;
    SDF_VU0_COPYVECTOR(inner->smoothedPosition, position);
    dds3OffPoseSetFlag(inner, OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION);
    return 0;
}

void fldSetCameraNodeModeWithTen(void) {
    s16 node = fldPlayerModelContext->current.h.motionIndex;
    if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x40) {
        fldUpdateCameraModelMotion(node, 0x12, 10.0f);
        return;
    }
    fldUpdateCameraModelMotion(node, 3, 10.0f);
}

void fldSetCameraNodeModeWithZero(void) {
    s16 node = fldPlayerModelContext->current.h.motionIndex;
    if (fldGetLocationCoordinateFlags(fldGlobalBase.area, fldGlobalBase.floor + 1) & 0x40) {
        fldUpdateCameraModelMotion(node, 0x12, 0.0f);
        return;
    }
    fldUpdateCameraModelMotion(node, 3, 0.0f);
}

void fldAddCameraModelEntry(s32 motionIndex) {
    dds3ModelHandle_t *model = fldPlayerModelContext;
    model->first->frameStep = 1.0f;
    mdlSmoothSingleMotion(model, 0, motionIndex, 2.0f, 5.0f);
}

void fldAddCameraModelPair(s32 firstMotionIndex, s32 secondMotionIndex) {
    mdlSetMotionSpeedEx(fldPlayerModelContext, 0, 1.0f);
    mdlSetMotionSpeedEx(fldPlayerModelContext, 1, 1.0f);
    mdlLoopMotion(fldPlayerModelContext, 0, firstMotionIndex);
    mdlLoopMotion(fldPlayerModelContext, 1, secondMotionIndex);
}

void fldClearCameraFacingPointRequest(void) {
    D_00389904[0] = 0;
}

void fldClearCameraHeadingRequest(void) {
    D_00389910[0] = 0;
}

void fldQueueCameraXYOverride(f32 targetX, f32 targetZ) {
    FldAreaWork *camera = &fldGlobalBase;

    camera->facingPointX = targetX;
    camera->facingPointZ = targetZ;
    camera->facingPointState = FLD_CAMERA_TURN_REQUEST_PENDING;
}

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z) {
    FldAreaWork *camera;
    f32 angle;

    angle = sdfAtan2(x, z);
    camera = &fldGlobalBase;
    angle *= 180.0f / 3.14f;
    camera->headingState = FLD_CAMERA_TURN_REQUEST_PENDING;
    camera->overrideAngle = -angle;
}

void fldUpdateCameraHeadingFromXY(void) {
    f32 dx;
    f32 dz;

    if (fldGlobalBase.facingPointState != FLD_CAMERA_TURN_REQUEST_IDLE) {
        dx = fldGlobalBase.x - fldGlobalBase.facingPointX;
        dz = fldGlobalBase.z - fldGlobalBase.facingPointZ;
        if (dx < 0.0001f && dx > -0.0001f && dz < 0.0001f && dz > -0.0001f) {
            return;
        }
        fldGlobalBase.targetAngle = -(sdfAtan2(dx, dz) * (180.0f / 3.14f));
        fldGlobalBase.facingPointState = FLD_CAMERA_TURN_REQUEST_TURNING;
    }
}

void fldApplyPendingCameraHeading(void) {
    FldAreaWork *camera = &fldGlobalBase;

    if (camera->headingState != FLD_CAMERA_TURN_REQUEST_IDLE) {
        camera->headingState = FLD_CAMERA_TURN_REQUEST_TURNING;
        camera->targetAngle = camera->overrideAngle;
    }
}

/* ASM turning: smooth angle toward targetAngle, clearing both request states on arrival. */
INCLUDE_ASM(const s32, "field/fldFileResolver", fldUpdateCameraHeadingTransition);

extern SdfFlagListParams fldCameraColorParameters[];
extern FldCameraSetting *fldCameraSettings;
extern FldCameraSetting D_0038BB70;
extern FldLightSet *D_004360F0;

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004132E0);

void fldLoadBattleSkyAndFilter(void) {
    s32 cameraSettingIndex;
    sdfFileCmdHandle_t *command;

    if (fldSkyLightSetBuffer == 0) {
        fldSkyLightSetBuffer = (void *)sdfMemLock(sdfMemAllocDirect(0xE000));
    }
    if (D_004360F0 == 0) {
        D_004360F0 = (void *)sdfMemLock(sdfMemAllocDirect(0xE000));
    }
    if (D_00436104 == 0) {
        D_00436104 = (void *)sdfMemLock(sdfMemAllocDirect(0x12400));
    }
    if (fldCameraSettings == 0) {
        fldCameraSettings = (FldCameraSetting *)sdfMemLock(sdfMemAllocDirect(
            sizeof(FldCameraSetting) * FIELD_CAMERA_SETTING_COUNT));
        for (cameraSettingIndex = 0; cameraSettingIndex < FIELD_CAMERA_SETTING_COUNT; cameraSettingIndex++) {
            fldCameraSettings[cameraSettingIndex] = D_0038BB70;
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
    command = sdfFileOpen("/fld/f/bin/FILTER.FLD");
    sdfFileRead(command, D_00436104, 0x12400);
    sdfFileClose(command);
    command = sdfFileOpen("/fld/f/bin/BATTLEBG.SKY");
    sdfFileRead(command, D_004360F0, 0xE000);
    sdfFileClose(command);
}

extern u32 fldRainTextureData;

extern char D_00413350[];



void fldLoadSkyResource(s32 area) {
    char path[64];
    char directory[32];
    sdfFileCmdHandle_t *command;

    gFilterAlpha = 0x80;
    if (area < 200) {
        fldMakeFilePath(directory, area, 1);
        if (area == 0x17 && evtBitCheck(0x19)) {
            sprintf(path, "%sF%03dt.SKY", directory, 0x17);
        } else if (area == 0x18 && evtBitCheck(0x19)) {
            sprintf(path, "%sF%03dt.SKY", directory, 0x18);
        } else if (area == 0x1B && evtBitCheck(0x19)) {
            sprintf(path, "%sF%03dT.SKY", directory, 0x1B);
        } else {
            sprintf(path, "%sF%03d.SKY", directory, area);
        }
        command = sdfFileOpen(path);
        sdfFileRead(command, fldSkyLightSetBuffer, 0xE000);
        sdfFileClose(command);
        if (area >= 2 && area < 100 && fldRainTextureResource == NULL) {
            fldRainTextureResource = sdfDevLoadFileEx(D_00413350, &fldRainTextureData, 0);
            fldRainTextureReference = sdfTexSet((const SdfTextureFileHeader *)(fldRainTextureData));
        }
    }
}

void fldReleaseSkyResources(void) {
    if (fldRainTextureReference != 0) {
        sdfTexReleaseReferenceViaHandler(fldRainTextureReference);
        fldRainTextureReference = 0;
    }
    if (fldRainTextureResource != NULL) {
        sdfMemFreeDelayedDirect(fldRainTextureResource);
        fldRainTextureResource = NULL;
    }
    if (fldCameraColorEffect != 0) {
        effReleaseSelectionFlagList(fldCameraColorEffect);
        fldCameraColorEffect = 0;
    }
    fldCameraColorEnabled = 0;
}

void fldUploadSkyBuffer(const FldSkyBuffer *src) {
    gFilterAlpha = 0x80;
    *fldSkyLightSetBuffer = *src;
    fldReleaseSkyResources();
    if (D_00389780[0] >= 2 && D_00389780[0] < 100 && fldRainTextureResource == NULL) {
        fldRainTextureResource = sdfDevLoadFileEx(D_00413350, &fldRainTextureData, 0);
        fldRainTextureReference = sdfTexSet((const SdfTextureFileHeader *)(fldRainTextureData));
    }
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413350);

enum {
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_32 = 1,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_32 = 2,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_32 = 3,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_48 = 4,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_48 = 5,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_48 = 6,
};

void fldSkyCalcFilter(void) {
    gFilterAnimeOfsA = 0;
    switch (gFilterAnime) {
    case FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_32:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_32:
        gFilterAnimeCntF += 0.2f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_32:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_48:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_48:
        gFilterAnimeCntF += 0.2f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_48:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    }
}

/* Draw the current filter grid, blending the previous grid during a fade. */
void fldSkyDrawFilter(void) {
    s32 column, row;
    s32 current = D_00436118;
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

    if (current == 0 || fldGetSceneReadyFlag() != FLD_SCENE_NOT_READY) {
        return;
    }
    currentWeight = D_00436120;
    if (currentWeight < 1.0f) {
        currentWeight += 1.0f / D_00436124;
        if (currentWeight > 1.0f) {
            currentWeight = 1.0f;
            D_00436120 = currentWeight;
            D_0043611C = current;
        } else {
            D_00436120 = currentWeight;
        }
    }
    previousWeight = 1.0f - currentWeight;
    previous = D_0043611C;

    rsetOtOfs(0x44);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    switch (D_00436104[current].mode) {
    case 0:
        rsetAlpha(0);
        break;
    case 1:
        rsetAlpha(1);
        break;
    case 2:
        rsetAlpha(2);
        break;
    }

    for (row = 0; row < 7; row++) {
        for (column = 0; column < 8; column++) {
            color0 = D_00436104[current].colors[row][column];
            color1 = D_00436104[current].colors[row][column + 1];
            color2 = D_00436104[current].colors[row + 1][column + 1];
            color3 = D_00436104[current].colors[row + 1][column];
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

            a0 += gFilterAnimeOfsA;
            a1 += gFilterAnimeOfsA;
            a2 += gFilterAnimeOfsA;
            a3 += gFilterAnimeOfsA;
            a0 += (s32)fldRunDanteGunFlag[14];
            a1 += (s32)fldRunDanteGunFlag[14];
            a2 += (s32)fldRunDanteGunFlag[14];
            a3 += (s32)fldRunDanteGunFlag[14];
            if (a0 < 0) a0 = 0;
            if (a0 > 255) a0 = 255;
            if (a1 < 0) a1 = 0;
            if (a1 > 255) a1 = 255;
            if (a2 < 0) a2 = 0;
            if (a2 > 255) a2 = 255;
            /* Retail has no corresponding clamp for a3. */

            if (D_00436120 < 1.0f) {
                s32 oldColor0 = D_00436104[previous].colors[row][column];
                s32 oldColor1 = D_00436104[previous].colors[row][column + 1];
                s32 oldColor2 = D_00436104[previous].colors[row + 1][column + 1];
                s32 oldColor3 = D_00436104[previous].colors[row + 1][column];
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

                oldA0 += gFilterAnimeOfsA;
                oldA1 += gFilterAnimeOfsA;
                oldA2 += gFilterAnimeOfsA;
                oldA3 += gFilterAnimeOfsA;
                oldA0 += (s32)fldRunDanteGunFlag[14];
                oldA1 += (s32)fldRunDanteGunFlag[14];
                oldA2 += (s32)fldRunDanteGunFlag[14];
                oldA3 += (s32)fldRunDanteGunFlag[14];
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

            if (gFilterAlpha != 0x80) {
                r0 = r0 * (s32)gFilterAlpha / 128;
                g0 = g0 * (s32)gFilterAlpha / 128;
                b0 = b0 * (s32)gFilterAlpha / 128;
                a0 = a0 * (s32)gFilterAlpha / 128;
                r1 = r1 * (s32)gFilterAlpha / 128;
                g1 = g1 * (s32)gFilterAlpha / 128;
                b1 = b1 * (s32)gFilterAlpha / 128;
                a1 = a1 * (s32)gFilterAlpha / 128;
                r2 = r2 * (s32)gFilterAlpha / 128;
                g2 = g2 * (s32)gFilterAlpha / 128;
                b2 = b2 * (s32)gFilterAlpha / 128;
                a2 = a2 * (s32)gFilterAlpha / 128;
                r3 = r3 * (s32)gFilterAlpha / 128;
                g3 = g3 * (s32)gFilterAlpha / 128;
                b3 = b3 * (s32)gFilterAlpha / 128;
                a3 = a3 * (s32)gFilterAlpha / 128;
            }
            fldSubmitGsGradientQuad(column * 64, row * 32, 64, 32,
                r0, g0, b0, a0, r1, g1, b1, a1,
                r2, g2, b2, a2, r3, g3, b3, a3);
        }
    }
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}

void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration) {
    if (D_004360E8 == 0 && D_00389780[0] < 40) {
        duration = 0;
    }
    if (duration == 0) {
        D_0043611C = area;
        D_00436120 = 1.0f;
        D_00436124 = 1.0f;
    } else {
        D_00436120 = 0.0f;
        D_0043611C = D_00436118;
        D_00436124 = (f32)duration;
    }
    D_00436118 = area;
    D_00436104[area].mode = targetMode;
}

void fldSetFilterAnime(u32 mode) {
    gFilterAnime = mode;
    gFilterAnimeCntF = 0;
    gFilterAnimeOfsA = 0;
}

void fldSkySetAlpha(u32 drawState) {
    gFilterAlpha = drawState;
}

u32 fldSkyGetAlpha(void) {
    return gFilterAlpha;
}

void fldSkySetAlphaOfs(u32 alpha) {
    D_003899C0[0] = alpha;
}

u32 fldSkyGetAlphaOfs(void) {
    return D_003899C0[0];
}

void fldSkySetTableAnime(u32 transitionMode) {
    u32 selectedValue = D_003899B4[0];

    D_0043612C = transitionMode;
    D_00436130 = 0;
    D_00436134 = selectedValue;
    fldApplySkyLightSet(selectedValue, 1);
}

void fldUpdateSelectedValueTransition(void) {
    evtUnitHandle_t *player;
    f32 direction[4];
    s32 red, green, blue;
    u32 colorA, colorB;

    switch ((s32)D_0043612C) {
    case 0: /* Disabled. */
        break;
    case 1:
        D_00436130 += 1.0f;
        if (D_00436130 == 30.0f) {
            fldApplySkyLightSet((D_00436134 + 1) & 0xFF, 30);
        } else if (D_00436130 == 60.0f) {
            fldApplySkyLightSet(D_00436134, 30);
            D_00436130 = 0;
        }
        break;
    case 2:
        D_00436130 += 1.0f;
        if (D_00436130 == 15.0f) {
            fldApplySkyLightSet((D_00436134 + 1) & 0xFF, 1);
        } else if (D_00436130 == 30.0f) {
            fldApplySkyLightSet(D_00436134, 1);
            D_00436130 = 0;
        }
        break;
    case 3:
        D_00436130 += 1.0f;
        if (D_00436130 == 60.0f) {
            fldApplySkyLightSet((D_00436134 + 1) & 0xFF, 60);
        } else if (D_00436130 == 120.0f) {
            fldApplySkyLightSet(D_00436134, 60);
            D_00436130 = 0;
        }
        break;
    }
    if (dds3SearchProcessName(D_00413388) != NULL) {
        player = evtGetUnitHandle2(fldPlayerObject);
        evtUnitLightOn(player);
        red = D_0038BB10.color[0] * 128.0f;
        green = D_0038BB10.color[1] * 128.0f;
        blue = D_0038BB10.color[2] * 128.0f;
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = D_0038BB60.color[0] * 128.0f;
        green = D_0038BB60.color[1] * 128.0f;
        blue = D_0038BB60.color[2] * 128.0f;
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        evtSetUnitLightAndAmbient(player, 0, colorA, colorB);
        direction[0] = D_0038BB10.direction[0];
        direction[1] = D_0038BB10.direction[1];
        direction[2] = D_0038BB10.direction[2];
        direction[3] = 0.0f;
        SDF_VU0_LOADVECTOR(direction);
        evtSetUnitLightDir(player, 0);
    }
}

/* vu0 routine: the event direction setter takes its vector in vf10. */
void fldSetPlayerAndPeerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                  f32 redB, f32 greenB, f32 blueB,
                  f32 x, f32 y, f32 z) {
    evtUnitHandle_t *player;
    evtUnitHandle_t *secondary = 0;
    u32 colorA;
    u32 colorB;
    s32 red, green, blue;
    f32 direction[4];

    if (fldSecondarySceneObject != 0) {
        secondary = evtGetUnitHandle2(fldSecondarySceneObject);
        evtUnitLightOn(secondary);
    }
    player = evtGetUnitHandle2(fldPlayerObject);
    evtUnitLightOn(player);
    red = redA * 128.0f;
    green = greenA * 128.0f;
    blue = blueA * 128.0f;
    colorA = red | (blue << 16) | (green << 8) | 0x80000000;
    red = redB * 128.0f;
    green = greenB * 128.0f;
    blue = blueB * 128.0f;
    colorB = red | (blue << 16) | (green << 8) | 0x80000000;
    evtSetUnitLightAndAmbient(player, duration, colorA, colorB);
    if (fldSecondarySceneObject != 0) {
        evtSetUnitLightAndAmbient(secondary, duration, colorA, colorB);
    }
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
    direction[3] = 0.0f;
    SDF_VU0_LOADVECTOR(direction);
    evtSetUnitLightDir(player, duration);
    if (fldSecondarySceneObject != 0) {
        evtSetUnitLightDir(secondary, duration);
    }
    SDF_VU0_LOADVECTOR(direction);
    evtSetUnitLightDir(player, 0);
    if (fldSecondarySceneObject != 0) {
        evtSetUnitLightDir(secondary, 0);
    }
    D_0038BB10.color[0] = redA;
    D_0038BB10.color[1] = greenA;
    D_0038BB10.color[2] = blueA;
    D_0038BB10.direction[0] = x;
    D_0038BB10.direction[1] = y;
    D_0038BB10.direction[2] = z;
    D_0038BB60.color[0] = redB;
    D_0038BB60.color[1] = greenB;
    D_0038BB60.color[2] = blueB;
}


extern void dds3SetFogColor(s32, f32 *);
extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, f32 *color);
extern void dds3SetAmbientColor(s32, f32 *);
extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, f32 *direction);
extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);
extern void fldSetFilterAnime(u32);
extern u32 D_00436138, D_0043613C;
extern f32 D_00436140;
extern s32 D_00436144, D_00436148, D_0043614C;
extern f32 D_00436150, D_00436154;
extern u32 D_0043615C;
extern f32 D_00436160;
extern u32 D_00436164;

void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (fldGlobalBase.area < 200) {
        if (D_004360E8 == 0 && fldGlobalBase.area < 40) {
            durationMode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (durationMode != 0) {
            duration = defaultDuration;
        }
        if (durationMode >= 2) {
            duration = durationMode;
        }
        D_00436128 = skyLightSetIndex;
        D_00436158 = skyLightSetIndex;
        D_00436138 = 0;
        if (fldGlobalBase.sceneCommand == 0) {
            light = &fldSkyLightSetBuffer->lightSets[skyLightSetIndex];
            area = light->fadeArea;
            fldRunDanteGunFlag[13] = area;
            fldRunDanteGunFlag[14] = light->filterAlphaOffset;
            value = light->fadeMode;
            fldRunDanteGunFlag[15] = value;
            fldRunDanteGunFlag[16] = light->swayMode;
            fldSetFadeTarget(area, value, duration);
            fldSetFilterAnime(fldRunDanteGunFlag[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            dds3SetFogColor(duration, vec);
            dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            dds3SetAmbientColor(duration, vec);
            fldSetPlayerAndPeerLighting(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                          light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                          light->unitLightDirection[0], light->unitLightDirection[1],
                          light->unitLightDirection[2]);
        }
    }
}

void fldApplyPendingSceneValueWithSpeed(s32 speed) {
    if (D_004360E8 == 0 && D_00389780[0] < 40) {
        speed = 0;
    }
    if (D_00436158 != 0 && D_00436128 != D_00436158) {
        if (D_00389898[0] == 0) {
            D_003899B4[0] = D_00436158;
        }
        fldApplySkyLightSet(D_00436158, speed);
    }
}

/*BEGIN fldTransitionSkyLightSet*/
void fldTransitionSkyLightSet(s32 lightSetIndex, s32 mode) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;
    u32 previousIndex;

    if (fldGlobalBase.area < 200) {
        if (D_004360E8 == 0 && fldGlobalBase.area < 40) {
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
        if (D_00436128 != lightSetIndex) {
            previousIndex = D_00436158;
            if (D_00436138 == 0) {
                previousIndex = D_00436128;
            }
            D_0043613C = gFilterAnime;
            D_00436140 = gFilterAnimeCntF;
            D_00436144 = gFilterAnimeOfsA;
            D_00436148 = D_00436118;
            D_0043614C = D_0043611C;
            D_00436150 = D_00436120;
            D_00436154 = D_00436124;
            D_00436158 = previousIndex;
            D_0043615C = D_0043612C;
            D_00436160 = D_00436130;
            D_00436164 = D_00436134;
            D_00436138 = 1;
            D_00436128 = lightSetIndex;
            if (fldGlobalBase.sceneCommand == 0) {
                light = &fldSkyLightSetBuffer->lightSets[lightSetIndex];
                fldRunDanteGunFlag[11] = lightSetIndex;
                area = light->fadeArea;
                fldRunDanteGunFlag[13] = area;
                fldRunDanteGunFlag[14] = light->filterAlphaOffset;
                value = light->fadeMode;
                fldRunDanteGunFlag[15] = value;
                fldRunDanteGunFlag[16] = light->swayMode;
                fldSetFadeTarget(area, value, duration);
                fldSetFilterAnime(fldRunDanteGunFlag[16]);
                vec[0] = light->drawColorFixedR * 0.00390625f;
                vec[1] = light->drawColorFixedG * 0.00390625f;
                vec[2] = light->drawColorFixedB * 0.00390625f;
                vec[3] = 0;
                dds3SetFogColor(duration, vec);
                dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
                dir[0] = light->lightDirectionAX;
                dir[1] = light->lightDirectionAY;
                dir[2] = light->lightDirectionAZ;
                dir[3] = 0;
                dds3SetLightDir(duration, 0, dir);
                vec[0] = light->lightColorAX;
                vec[1] = light->lightColorAY;
                vec[2] = light->lightColorAZ;
                vec[3] = 0;
                dds3SetLightColor(duration, 0, vec);
                dir[0] = light->lightDirectionBX;
                dir[1] = light->lightDirectionBY;
                dir[2] = light->lightDirectionBZ;
                dir[3] = 0;
                dds3SetLightDir(duration, 1, dir);
                vec[0] = light->lightColorBX;
                vec[1] = light->lightColorBY;
                vec[2] = light->lightColorBZ;
                vec[3] = 0;
                dds3SetLightColor(duration, 1, vec);
                dir[0] = light->lightDirectionCX;
                dir[1] = light->lightDirectionCY;
                dir[2] = light->lightDirectionCZ;
                dir[3] = 0;
                dds3SetLightDir(duration, 2, dir);
                vec[0] = light->lightColorCX;
                vec[1] = light->lightColorCY;
                vec[2] = light->lightColorCZ;
                vec[3] = 0;
                dds3SetLightColor(duration, 2, vec);
                vec[0] = light->backgroundColorX;
                vec[1] = light->backgroundColorY;
                vec[2] = light->backgroundColorZ;
                vec[3] = 1.0f;
                dds3SetAmbientColor(duration, vec);
                fldSetPlayerAndPeerLighting(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                              light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                              light->unitLightDirection[0], light->unitLightDirection[1],
                              light->unitLightDirection[2]);
            }
        }
    }
}
/*END fldTransitionSkyLightSet*/

void fldClearCameraObjectActiveFlag(void) {
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x100);
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413388);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldRoomEventPlacementCount);

INCLUDE_SDATA(struct DevRequest *, "field/fldFileResolver", fldRoomSoundResources);

INCLUDE_SDATA(FldTransferChunk *, "field/fldFileResolver", fldRoomF1Chunk);

INCLUDE_SDATA(u8 *, "field/fldFileResolver", fldRoomF1Data);

INCLUDE_SDATA(FldTransferChunk *, "field/fldFileResolver", fldRoomF2Chunk);

INCLUDE_SDATA(u8 *, "field/fldFileResolver", fldRoomF2Data);

INCLUDE_SDATA(FldTransferChunk *, "field/fldFileResolver", fldRoomKF2Chunk);

INCLUDE_SDATA(u8 *, "field/fldFileResolver", fldRoomKF2Data);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFldFileResetMode);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFldFileF1);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FC8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", pFldNextFileDataF2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FD0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldFieldPackData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FDC);

INCLUDE_SDATA(struct sdfMemHandle_t *, "field/fldFileResolver", fldCachedRoomResourceAllocation);

INCLUDE_SDATA(struct sdfMemHandle_t *, "field/fldFileResolver", fldCachedRoomF1ResourceAllocation);

INCLUDE_SDATA(struct sdfMemHandle_t *, "field/fldFileResolver", fldCachedRoomF2ResourceAllocation);

INCLUDE_SDATA(struct sdfMemHandle_t *, "field/fldFileResolver", fldCachedRoomKF2ResourceAllocation);

INCLUDE_SDATA(const u8 *, "field/fldFileResolver", fldCachedRoomResourceData);

INCLUDE_SDATA(const u8 *, "field/fldFileResolver", fldCachedRoomF1ResourceData);

INCLUDE_SDATA(const u8 *, "field/fldFileResolver", fldCachedRoomF2ResourceData);

INCLUDE_SDATA(const u8 *, "field/fldFileResolver", fldCachedRoomKF2ResourceData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF1ResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomF2ResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCachedRoomKF2ResourceSize);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436010);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldPendingArea);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldPendingFloor);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436020);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436028);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436030);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436038);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436040);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436048);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436050);

INCLUDE_SDATA(const s32, "field/fldFileResolver", rsetFrameMaskOtOffset);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436064);

INCLUDE_SDATA(void * const, "field/fldFileResolver", fldDrwZBuf);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436070);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436078);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0043607C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldMarkerTexture);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436088);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0043608C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldEncounterRuntimeState);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldEncounterTaskName);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360A0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360A4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360A8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360AC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360B0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360B4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360B8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360BC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360C0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360C4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360C8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360CC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360D0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360D4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360D8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360DC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360E0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360E4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360E8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSkyLightSetBuffer);

INCLUDE_SDATA(FldLightSet * const, "field/fldFileResolver", D_004360F0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraSettings);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraColorEffect);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraColorEnabled);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldRainTextureReference);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436104);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFilterAlpha);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFilterAnime);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFilterAnimeCntF);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFilterAnimeOfsA);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436118);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0043611C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436120);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436124);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436128);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0043612C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436130);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436134);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436138);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0043613C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436140);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436144);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436148);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0043614C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436150);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436154);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436158);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0043615C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436160);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436164);
