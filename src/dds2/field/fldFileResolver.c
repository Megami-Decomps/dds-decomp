#include "sdf_gs_blend.h"
#include "common.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "fld_resource_resolver.h"
#include "fld.h"
#include "dds3obj.h"
#include "sdf_texture_file.h"
#include "evt_action_object.h"
#include "sdf_texture_offset_list.h"
#include "evt_script_model.h"
#include "sdf_model.h"


extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
extern void *dds3GetWorldSecondaryObject(void);
struct EffWorldNode;
extern u32 dds3AdvanceWorldCounter(void);
extern struct EffWorldNode *dds3SpawnInnerVecObj6(s32, f32 *, void *);
extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);
extern char D_00412FF0[]; /* "FLD_DMY_MATTER" */
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
    dds3SetWorldNodeValue(matter, (u32)D_00412FF0);
    return matter;
}

void *fldResolveWorldObjectByResourceId(u32 id) {
    u32 i = 0;
    EffWorldNode *world = dds3GetWorldSecondaryObject();
    FldFileResource *resource = D_00438EC0;

    for (; i < D_00438EC4; i++, resource++) {
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
                void *object = (void *)fldGetRecordValueById(resource->id);
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

void *fldResolveWorldObjectByResourceEntryName(const char *name) {
    u32 i = 0;
    EffWorldNode *world = dds3GetWorldSecondaryObject();
    FldFileResource *resource = D_00438EC0;

    for (; i < D_00438EC4; i++, resource++) {
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

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00412FF0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001289A8);

#include "fld_area_work.h"
#include "evt_world.h"
#include "evt_world_source_transform.h"
#include "common.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_append.h"
#include "sdf_packet_builders.h"
#include "fr_font.h"
#include "sdf_packet_list.h"
#include "sdf_dev_state.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "fld_inf.h"
#include "sdf_primitive.h"
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


extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);

extern MdlCtx *fldPlayerModelContext;
extern s32 D_00389888[];

extern void fldFreeDisplayObjects(void);

extern MdlCtx *fldSecondarySceneModelContext;

extern s32 fldSecondarySceneObject;



extern s32 sdfAllocPacketAligned(s32);

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern u32 fldPendingArea;

extern u32 fldPendingFloor;

extern u32 D_00435FC0;

extern u32 fldDisplayRow;

extern s32 fldBackgroundBuffer;

extern u32 D_00436088;

extern void *dds3GetWorldObject(void);


extern s64 fldGetPlayerSceneState(void);

extern u32 D_004360AC;

extern s32 D_004360C4;

extern s32 D_004360C8;

extern s32 D_004360CC;
extern s32 D_004360D4;

extern s32 D_004360D0;

extern u32 fldPlayerObject;

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern SdfTex *fldRainTextureReference;

extern s32 fldRainTextureResource;

extern u32 fldSwayMode;

extern u32 fldSkyDrawState;

extern struct FileRequest *fldAreaLoadRequest;


extern u8 D_00435BB4;

extern u32 D_003897E8[];

extern char D_00444950[];

extern s32 strcmp(const char *a, const char *b);

extern u32 fldAreaCachedResource;

extern struct FileRequest *fldAreaPackedArchive;

extern u8 D_00436020[];



extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];

extern void sdfPostmultiplyVuMatrixFromMemory(void *src);

extern u32 D_00389904[];

extern u32 D_00389910[];

extern u32 D_003899C0[];

extern void fldCreatePlayerObject(void);

extern void func_00128FE8(u32, u32, s32);

extern u32 D_00436064;

extern u32 D_0043607C;

extern SdfTex *fldMarkerTexture;

extern u32 D_00438EC8;

extern u8 D_0038A700[];

extern SdfAsset *sdfCreateAssetWithDrawEntries(void);
extern void sdfTexReleaseReferenceViaHandler(SdfTex *);

extern f32 D_004360B0;

extern f32 D_00444980[];

extern f32 D_00444970[];

extern void dds3TransformCameraVectorsByInnerRotation(EffWorldNode *camera, f32 *worldEyeOut,
                                                      f32 *targetPositionOut);

extern f32 sdfAtan2(f32 y, f32 x);

extern f32 fldLookAtNearPoint[];

extern f32 fldLookAtFarPoint[];

/* Data transfer descriptor: source-relative byte offset and transfer size. */
typedef struct FldTransferChunk {
    u32 unk0;
    s32 offset;
    u32 size;
} FldTransferChunk;

/* Four resource references are copied into the scene's load queue in order. */
typedef struct FldLoadRecord {
    u32 unk_0;
    u32 displayState;
    void *resourceA;
    void *resourceB;
    void *resourceC;
    void *resourceD;
} FldLoadRecord;

typedef struct FldLoadRequest {
    u32 unk_0;
    u32 unk_4;
    FldLoadRecord *record;
} FldLoadRequest;

extern u32 D_00444920[], D_00444930[], D_00444940[];
extern char D_00435FD0[];
extern void func_001289A8(u32, u32);

extern f32 D_003897DC[];



extern char D_004130D8[]; /* "%sf%03d_%03d.LB" */

extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_0035C860(char *, const char *, ...);

extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_0035C860(char *, const char *, ...);



extern u32 fldCachedRoomResourceData, fldCachedRoomF1ResourceData, fldCachedRoomF2ResourceData, fldCachedRoomKF2ResourceData;

extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;

extern u32 sdfConsFinalizePacketHeader(u32, s32);

extern SdfPoolNode kwlnDrawSurfaces[];


extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void *sdfConsAllocateColumnPacket(s32);


extern s32 sdfConsCalculateDrawPacketSize(s32, s32);

extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);

extern s32 sdfConsMeasurePacketWithHeader(s32);

extern u32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_0032DB30(s32, u32, s32);


extern void func_0032DB78(s32, u32, s32);

extern f32 D_0038A980[][4];

extern u32 D_0038A9A0[];

extern void *func_00348158(const f32 (*)[4], const u32 *, s32, u32);

extern void *memset(void *s, s32 c, u32 n);




extern void *func_0033B050(SdfPrimitiveRequest *);

extern u32 D_0040B2A0[];

/* Packed quad input: geometry fields precede the live packet origin and depth.
 * Preserve the unclassified words for the opaque renderer. */
typedef struct {
    s32 baseX;
    s32 baseY;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 rgba;
    s32 rowX;      /* 0x1C: quad origin X */
    s32 rowY;      /* 0x20: quad origin Y */
    s32 drawDepth;
    s32 packetList; /* 0x28 */
} FldQuadState; /* 0x2C bytes */

extern SdfPoolNode kwlnPositionedTextSurface;



extern void sdfInvertScaledVuTransform(void);

extern SdfPoolNode D_00380708;

extern u8 D_00436070[];

extern u8 D_00436078[];

extern s32 fldEncounterRuntimeState;

extern u32 D_0043608C;

extern s32 func_0022E450(void);

extern s32 fldEncProc(void);

extern void btlUpdateRuntimeFadeState(void);



extern f32 fldSwayPhase;

extern s32 fldSwayOffset;

extern f32 sdfSinPoly(f32);

extern s32 D_00389780[];

extern s32 D_004360E8;

extern s32 D_00436118, D_0043611C;

extern s32 *D_00436104;

extern f32 D_00436120, D_00436124;

extern s32 D_00389780[];

extern s32 D_004360E8;




extern u32 D_0043612C;

extern f32 D_00436130;

extern u32 D_00436134;

extern u32 D_003899B4[];

extern void func_00135A68(u32 value, s32 enabled);
extern char D_00413388[];

extern u32 D_00436128;

extern u32 D_00436158;

extern s16 D_00389898[];

extern u8 D_0037F650[];

extern u8 D_0037F9B0[];

extern u8 D_0037F9F0[];

extern u8 D_0037FA00[];

extern u8 D_00384790[];

extern SdfTexBuf *sdfTexGetPrimaryBuffer(SdfTex *);

extern s32 sdfTexGetPrimaryBufferSize(SdfTex *);

extern void sdfConsInitDmaPacketHeader(DmaPacketHeader *, u32, s32);


extern void func_003365B8(f32);

extern void sdfInitGeometryDmaPacket(u8 *, const f32 *);

extern void func_0033B530(u64, u8 *, s32, u8 *, u8 *);

extern s32 D_00435F30;

extern void func_00139950(f32 *);







extern void btlActivateRuntime(s32 mode);


extern char fldEncounterTaskName[];

extern void btlClearRuntimeState(void);

extern s32 kwlnTaskCreate(s32 name, s32 priority, s32, s32, s32, s32, s32);

extern void fldUpdateCameraModelMotion(s32, s32, f32);

extern s32 fldGetLocationCoordinateValue(s32, s32);

typedef struct {
    f32 dist;
    f32 y;
    f32 targetY;
    f32 fov;
    f32 unk10;
    f32 unk14;
} FldCamRow; /* 0x18 bytes */


extern FldCamRow fldCameraFollowRows[];

extern f32 D_0038BAF0[];

extern f32 D_0038BB00[];

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern u32 *fldGetPlayerSceneStateAddress(void);

extern void dds3SetCameraFieldOfView(s32, f32);

extern void fldToggleWorldNodeState(s32);

extern void func_0012EDB0(void);

/* Relocated collision headers contain their eight-word geometry header. */
typedef struct FldSpawnCollision {
    u32 kind;
    u32 word04;
    u32 *header;
    u32 word0C;
    u32 geometry[8];
} FldSpawnCollision;

typedef struct FldSpawnEvent {
    u32 flags;
    const char *label;
    u32 reserved[2];
} FldSpawnEvent;

/* Type-10 placement records have a fixed four-word serialized header. */
typedef struct FldPlacementData {
    u32 kind;
    s32 event;
    u32 visible;
    void *parameters;
} FldPlacementData;

typedef struct FldSpecialPoint {
    u32 kind;
    u32 id;
} FldSpecialPoint;

/* A relocated batch consists of a kind, count, and complete resource rows. */
typedef struct FldSpawnBatch {
    u32 kind;
    u32 count;
    FldFileResource *entries;
} FldSpawnBatch;

extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
extern s32 D_00435FA0;
extern s16 D_00389874[];
extern s32 fldParseRoomNumberFromName(char *);
extern void func_00137F10(void *, f32 *, u32, s32, s32);
extern EffWorldNode *evtSpawnActionObjB(s32, s32, s32, s32);
extern void dds3SetPathStateValue(EffWorldNode *, u32);
extern EffWorldNode *evtSpawnActionObjD(s32, void *, s32);
extern EffWorldNode *evtSpawnActionObj10(s32, void *, s32);
extern s32 fldPushDisplayValue(u32, EffWorldNode *);
extern s32 func_0013BAB8(FldFileResource *, EffWorldNode *);
extern void func_0014D380(s32, u32, f32 *, f32, f32, f32);
extern void func_0014C2B8(u32, f32 *, f32, f32, f32, s32, const char *, s32);
extern void func_0014C9B0(u32, f32 *, f32, f32, f32);
extern s32 fldSetSparkVectors(s32, const u128 *, const u128 *);
extern void fldAppendPendingSoundForScene(s32, f32, f32, f32, f32);
extern EffWorldNode *dds3CreateCameraObject(s32, void *, void *);
extern void dds3SetWorldNodeValue(EffWorldNode *, u32);

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

void func_00128FE8(u32 batchAddress, u32 batchCount, s32 appended) {
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
        D_00438EC0 = NULL;
        D_00438EC4 = 0;
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
            D_00438EC0 = resource;
            D_00438EC4 = count;
            pathState = 1;
            for (i = 0; i < count; i++, resource++, pathState++) {
                resource->id |= 0x10000;
                fldCopySpawnTransform(position, rotation, resource->transform);
                if (((FldSpawnCollision *)resource->data)->kind == 0) {
                    room = fldParseRoomNumberFromName((char *)resource->name);
                    func_00137F10(((FldSpawnCollision *)resource->data)->header,
                                 position, resource->id, room, -1);
                    object = evtSpawnActionObjB(resource->id, (s32)resource->data,
                                              (s32)position, (s32)resource->name);
                    dds3SetPathStateValue(object, pathState);
                }
            }
            break;
        case 6:
            for (i = 0; i < count; i++, resource++) {
                if (appended != 0) {
                    resource->id |= 0x800000;
                } else {
                    D_00435FA0++;
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
                evtSpawnActionObj10(resource->id, resource->data, (s32)resource->name);
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
                        func_0014D380(0, point->id, rotation, position[0], position[1], position[2]);
                        if (appended == 0) {
                            D_00435FA0++;
                        }
                        break;
                    case 2:
                        func_0014D380(1, point->id, rotation, position[0], position[1], position[2]);
                        if (appended == 0) {
                            D_00435FA0++;
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
                    func_0013BAB8(resource, object);
                    break;
                case 2:
                    if (D_00389874[0] != 0) {
                        break;
                    }
                    parameters = ((FldPlacementData *)resource->data)->parameters;
                    id = parameters[0];
                    room = fldParseRoomNumberFromName((char *)resource->name);
                    if (appended != 0) {
                        FldPlacementData *placement = resource->data;
                        func_0014C2B8(id, rotation, position[0], position[1], position[2],
                                      room, resource->name, placement->event + D_00435FA0);
                    } else {
                        FldPlacementData *placement = resource->data;
                        func_0014C2B8(id, rotation, position[0], position[1], position[2],
                                      room, resource->name, placement->event);
                    }
                    break;
                case 3:
                    parameters = ((FldPlacementData *)resource->data)->parameters;
                    func_0014C9B0(parameters[0], rotation, position[0], position[1], position[2]);
                    if (appended == 0) {
                        D_00435FA0++;
                    }
                    break;
                case 7:
                    parameters = ((FldPlacementData *)resource->data)->parameters;
                    fldAppendPendingSoundForScene(parameters[1], *(f32 *)parameters,
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

extern void *dds3GetWorldSecondaryObject(void);

typedef struct FldActionSpawn {
    u32 unk0;
    u32 secondValue;
    u32 firstValue;
} FldActionSpawn;

/* Layout shared by the two field-request dispatch paths. */
typedef struct FldSceneRequest {
    u32 primaryValue;
    u32 secondaryValue;
    FldLoadRecord *record;
    u32 spawnCount;
    FldActionSpawn *spawnList;
} FldSceneRequest;

void fldSpawnActionObjects(FldActionSpawn *list, u32 count) {
    u32 i;
    struct EffWorldNode *node;

    dds3GetWorldSecondaryObject();
    for (i = 0; i < count; i++, list++) {
        node = evtSpawnActionObj2(list->firstValue, list->secondValue);
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

extern FldFileResource *D_00438EB8;
extern u32 D_00438EBC;
extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
extern struct DevRequest *D_00435FA4;
struct EffWorldNode;
extern struct EffWorldNode *dds3SpawnInnerVecObj6(s32, f32 *, void *);
extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);
extern void effObjSetActiveId(struct EffWorldNode *, s32);
extern s32 fldParseRoomNumberFromName(char *);
extern void effObjSetRoomNumber(struct EffWorldNode *, u32);
extern void func_00112168(void *);
extern void fldSetRecordValueById(s32, s32);
extern s32 dds3RegisterObjectInHandlerIndex(void *);

void fldCreateResourceScriptObjects(void) {
    f32 position[4];
    f32 rotation[4];
    FldFileResource *resource = D_00438EB8;
    u32 count = D_00438EBC;
    void *world;
    struct EffWorldNode *object;
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
                                         script->motionTable, D_00435FA4, resource->name);
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
        effObjSetRoomNumber(object, fldParseRoomNumberFromName((char *)resource->name));
        dds3SetSlotByKind(object, dds3FindWorldObjectNodeByKey(world, resource->id, 10));
        func_00112168(object);
        binding = D_00438EC0;
        for (j = 0; j < D_00438EC4; j++, binding++) {
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
            dds3SetSlotByKind(object, dds3FindIndexedObjectChainNodeByName(world, 2, (const u8 *)linkedName->name));
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
extern void dds3SetSlotKey(EffWorldNode *, EffWorldNode *);
extern void dds3ReplaceObjectResource(EffWorldNode *);
extern void sdfSetFloatCounterDirection(u32 *, u32);
extern void evtScaleSlotByClampedMultiplier(void *, f32);
extern void dds3InvokeMoverUpdate(EffWorldNode *);
extern void dds3SetObjectPayloadWord8(EffWorldNode *, u32);
extern s32 func_0035C860(char *, const char *, ...);
extern char D_004130A8[], D_004130B8[], D_004130C8[];

/* Restore the room's named objects and enable their retained path controllers. */
void func_00129940(void) {
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

            func_0035C860(modelName, D_004130A8, index);
            func_0035C860(collisionName, D_004130B8, index);
            func_0035C860(pathName, D_004130C8, index);
            modelKey = fldFindSearchId(modelName);
            collisionKey = fldFindSearchId(collisionName);
            if (modelKey == -1) {
                object = (EffWorldNode *)fldResolveWorldObjectByResourceId(collisionKey);
            } else {
                object = (EffWorldNode *)dds3FindWorldObjectNodeByKey(world, modelKey, 6);
                if (object == NULL) {
                    object = (EffWorldNode *)fldResolveWorldObjectByResourceId(collisionKey);
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

            func_0035C860(modelName, D_004130A8, index);
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
    D_00444920[0] = (u32)request->record->resourceA;
    D_00444920[1] = (u32)request->record->resourceB;
    D_00444920[2] = (u32)request->record->resourceC;
    D_00444920[3] = (u32)request->record->resourceD;
    for (i = 0; i < 4; i++) {
        D_00444930[i] = 0;
        D_00444940[i] = 0;
    }
    if (fldAreaState.area < 500) {
        for (i = 0; i < 4; i++) {
            if (D_00444920[i] != 0) {
                fldFormatAreaDirectory(directory, fldAreaState.area, fldAreaState.floor + 1);
                func_0035C860(path, D_00435FD0, directory, D_00444920[i]);
                D_00444930[i] = (u32)sdfReadNamedResource(path, &D_00444940[i], 0);
            }
        }
    }
    func_001289A8(request->unk_4, request->unk_0);
}

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(FldSceneRequest *request) {
    s32 state;
    fldSpawnActionObjects(request->spawnList, request->spawnCount);
    state = fldAreaState.area;
    if (state != 1 && state < 200) fldCreatePlayerObject();
    func_00128FE8(request->secondaryValue, request->primaryValue, 0);
    fldAreaState.fallbackResourceName = request->record->displayState;
}

void fldProcessFieldRequestAlternate(FldSceneRequest *request) {
    func_00128FE8(request->secondaryValue, request->primaryValue, 1);
}

s32 func_00129D60(s32 record) {
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
        func_0035C860(path, D_004130D8, directory, area, floor);
        fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
        fldAreaState.resourceFlag = 1;
        return 1;
    }
    return 0;
}

extern void sdfRaiseDeviceThreadPriority(void);

extern s32 D_00436010;

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
        D_00436010 = 1;
    } else {
        D_00436010 = 0;
    }
    sdfRaiseDeviceThreadPriority();
    D_00435BB4 = 1;
    fldFormatAreaDirectory(directory, area, 1);
    func_0035C860(path, D_004130D8, directory, area, room);
    fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
    fldAreaState.resourceFlag = 1;
    return 1;
}

void fldFreeDisplayObjects(void) {
    if (fldAreaLoadRequest != 0) {
        PacWork *work = ((FilePacRequest *)fldAreaLoadRequest)->packet.queueHead;

        if (work != 0) {
            do {
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)(u32)work->resourceHandle);
                work = work->next;
            } while (work != 0);
        }
        func_002C7CE8(fldAreaLoadRequest);
        fldAreaLoadRequest = NULL;
    }
    fldPendingArea = 0;
    fldPendingFloor = 0;
    fldAreaState.resourceArea = 0;
    fldAreaState.resourceFloor = 0;
    fldAreaState.resourceFlag = 0;
}

u32 fldPollAreaResourceLoad(void) {
    u32 resourceFlag = fldAreaState.resourceFlag;

    if (resourceFlag != 0) {
        if (resourceFlag == 1) {
            if (fileRequestIsReady(fldAreaLoadRequest) != 0) {
                fldAreaState.resourceFlag = 0;
                D_00435BB4 = 0;
            }
        }
    }
    return 0;
}

u32 fldGetResourceReadyFlag(void) {
    return D_003897E8[0];
}

u8 fldIsAreaResourceReady(void) {
    if (fldAreaLoadRequest != 0) {
        if (fileRequestIsReady(fldAreaLoadRequest) != 0) {
            return 1;
        }
    }
    return D_003897E8[0] != 0;
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

struct SdfMemBlock *fldLoadCachedRoomResourceIfLocationMatches(void **destination, s32 area, s32 room) {
    FldAreaWork *state = &fldAreaState;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
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
    FldAreaWork *state = &fldAreaState;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
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
    FldAreaWork *state = &fldAreaState;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
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
    FldAreaWork *state = &fldAreaState;

    if (state->resourceArea == area) {
        if (state->resourceFloor == room) {
            struct SdfMemBlock *allocation = sdfAllocGeneralBlock(fldCachedRoomKF2ResourceSize);
            void *data = (void *)sdfResourceRetainAddress(allocation);
            *destination = data;
            memcpy(data, (void *)fldCachedRoomKF2ResourceData, fldCachedRoomKF2ResourceSize);
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

    fldFormatAreaDirectory(directory, area, 1);
    if (area == 0x17 && mdlFlagTest(0x19)) {
        func_0035C860(out, "%sf%03d_00a.LB", directory, 0x17);
        return;
    }
    if (area == 0x18 && mdlFlagTest(0x19)) {
        func_0035C860(out, "%sf%03d_00a.LB", directory, 0x18);
        return;
    }
    if (area == 0x1B && mdlFlagTest(0x19)) {
        func_0035C860(out, "%sf%03d_00a.LB", directory, 0x1B);
        return;
    }
    func_0035C860(out, "%sf%03d_000.LB", directory, area);
}

u8 fldHasAreaResourceNameChanged(void) {
    char buf[32];

    fldFormatAreaResourceName(buf);
    return strcmp(D_00444950, buf) != 0;
}

extern void fldCopyInfoTable(FldInfTable *);

struct FldNpcPalette;
extern void fldSetNpcPalette(struct FldNpcPalette *source);

struct FldSkyBuffer;
extern void fldUploadSkyBuffer(struct FldSkyBuffer *source);

extern void fldCopyActorWaypointTable(FldWaypointBlock *source);



extern void fldSetSceneRecordChunk(u8 *chunk, s32 resourceId);

extern void fldCacheMapLabelLengths();

void fldLoadAreaPackedResources(void) {
    char name[32];
    FilePacRequest *request;
    PacWork *work;

    if (fldAreaState.area < 200) {
        fldFormatAreaResourceName(name);
        strcpy(D_00444950, name);
        fldAreaPackedArchive = fileQueuePlainDispatchRequest(name);
        func_002C81D0(fldAreaPackedArchive);
        request = (FilePacRequest *)fldAreaPackedArchive;
        for (work = request->packet.queueHead; work != NULL; work = work->next) {
            switch (*(u16 *)(work->packet + 2)) {
            case FLD_PACKED_RESOURCE_INFO_TABLE:
                fldCopyInfoTable((FldInfTable *)work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)(u32)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_NPC_PALETTE:
                fldSetNpcPalette((struct FldNpcPalette *)work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)(u32)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_SKY_BUFFER:
                fldUploadSkyBuffer((struct FldSkyBuffer *)work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)(u32)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE:
                fldCopyActorWaypointTable((FldWaypointBlock *)work->dataCursor);
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)(u32)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_RETAINED_COPY:
                fldAreaCachedResource = (u32)sdfAllocGeneralBlock(
                    sdfMemoryGetBlockSize((struct SdfMemBlock *)(u32)work->resourceHandle));
                memcpy((void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)fldAreaCachedResource),
                       (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)work->resourceHandle),
                       sdfMemoryGetBlockSize((struct SdfMemBlock *)(u32)work->resourceHandle));
                sdfQueueGeneralAllocationRelease(
                    (struct SdfMemBlock *)(u32)work->resourceHandle);
                break;
            case FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK:
                fldSetSceneRecordChunk(work->dataCursor, work->resourceHandle);
                break;
            }
        }
        fldCacheMapLabelLengths(fldAreaState.area % 100);
    }
}

void fldReleaseAreaResourceCache(void) {
    u32 cachedResource = fldAreaCachedResource;

    if (cachedResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)cachedResource);
        fldAreaCachedResource = 0;
    }
    cachedResource = (u32)fldAreaPackedArchive;
    if (cachedResource != 0) {
        func_002C7CE8((void *)cachedResource);
        fldAreaPackedArchive = 0;
    }
    D_00444950[0] = D_00436020[0];
}

extern u32 D_00435FC0;
extern s32 D_004361D0;
extern FldTransferChunk *D_00435FA8;
extern u32 D_00435FAC;
extern FldTransferChunk *D_00435FB0;
extern u32 D_00435FB4;
extern FldTransferChunk *D_00435FB8;
extern u32 D_00435FBC;
extern struct FileRequest *D_00435FC8;
extern s32 D_00435FDC;
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
extern void func_00129940(void);
extern void func_00142B70(void);
extern struct DevRequest *sndLoadNamedOffsetResourceList(const char *name);
extern s32 evtRetainSceneResource(EffWorldNode *worldNode, void *resourceHandle);
extern char D_00436028[];
extern char D_00436030[];
extern char D_00436038[];
extern FieldPlayerSceneWork D_0038A640;
extern void fldAllocateBackgroundBuffer(void);
extern void fldResetObjectSlots();
extern void fldReleaseResourceSlots();
extern void fldReleaseSceneRecordChunk();
extern void fldCreateResourceScriptObjects(void);
extern u8 fldHasAreaResourceNameChanged(void);
extern void fldLoadAreaPackedResources(void);
extern void fldReleaseAreaResourceCache(void);
extern void fldRelocatePackedTransferChunk(u32 buffer, FldTransferChunk *chunk);

const char D_00413128[] = "%sk%03d_%03d.f2";

u32 func_0012A6F0(const char *name) {
    char path[0x80];
    char directory[0x40];
    void *roomOffsets;
    void *roomData;
    FldTransferChunk *chunk;
    struct FileRequest *request;
    FieldPlayerSceneWork *sceneWork = &D_0038A640;
    s32 area;
    s32 room;
    s32 isField;
    PacWork *work;
    s32 index;

    D_00435FC0 = 0;
    D_00435FA0 = 0;
    fldCachedRoomF1ResourceAllocation = 0;
    fldCachedRoomF2ResourceAllocation = 0;
    fldCachedRoomKF2ResourceAllocation = 0;
    D_004361D0 = 0;
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
        fldReleaseResourceSlots();
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
            D_00435FC8 = request;
            fldFreeDisplayObjects();
        } else {
            fldFormatAreaDirectory(directory, area, 1);
            func_0035C860(path, D_004130D8, directory, area, room);
            request = fileQueuePlainDispatchRequest(path);
            D_00435FC8 = request;
            func_002C81D0(request);
        }
        index = 0;
        D_00435FA4 = NULL;
        work = ((FilePacRequest *)D_00435FC8)->packet.queueHead;
        fldCachedRoomResourceAllocation = 0;
        for (; work != NULL; work = work->next) {
            switch (index) {
            case 0:
                D_00435FA4 = sndBuildResourceHandleListFromOffsets((const SdfTextureOffsetListHeader *)work->dataCursor);
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)work->resourceHandle);
                fldCachedRoomResourceAllocation = 0;
                break;
            case 1:
                fldCachedRoomF2ResourceAllocation = 0;
                roomData = work->dataCursor;
                chunk = (FldTransferChunk *)((u8 *)roomData + 8);
                D_00435FB4 = (u32)roomData;
                D_00435FB0 = chunk;
                fldRelocatePackedTransferChunk((u32)roomData, chunk);
                fldProcessFieldRequest((FldSceneRequest *)func_00129D60((s32)chunk));
                break;
            case 2:
                fldCachedRoomF1ResourceAllocation = 0;
                roomData = work->dataCursor;
                chunk = (FldTransferChunk *)((u8 *)roomData + 8);
                D_00435FAC = (u32)roomData;
                D_00435FA8 = chunk;
                fldRelocatePackedTransferChunk((u32)roomData, chunk);
                fldLoadSceneRequestFiles((FldLoadRequest *)func_00129D60((s32)chunk));
                break;
            }
            index++;
        }
    }
    if (isField == 0) {
        fldCachedRoomResourceAllocation = (u32)fldLoadCachedRoomResourceIfLocationMatches(&roomOffsets, area, room);
        if (fldCachedRoomResourceAllocation == 0) {
            func_0035C860(path, D_00436028, name);
            D_00435FA4 = sndLoadNamedOffsetResourceList(path);
        } else {
            D_00435FA4 = sndBuildResourceHandleListFromOffsets(roomOffsets);
            sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldCachedRoomResourceAllocation);
            fldCachedRoomResourceAllocation = 0;
        }
    }
    if (isField == 0) {
        func_0035C860(path, D_00436030, name);
        fldCachedRoomF2ResourceAllocation = (u32)fldLoadCachedRoomF2ResourceIfLocationMatches(&roomData, area, room);
        if (fldCachedRoomF2ResourceAllocation == 0) {
            fldCachedRoomF2ResourceAllocation = (u32)sdfReadNamedResource(path, &roomData, 0);
        }
        chunk = (FldTransferChunk *)((u8 *)roomData + 8);
        D_00435FB4 = (u32)roomData;
        D_00435FB0 = chunk;
        fldRelocatePackedTransferChunk((u32)roomData, chunk);
        fldProcessFieldRequest((FldSceneRequest *)func_00129D60((s32)chunk));
    }
    if (fldGetLocationCoordinateValue(area, room) & 0x10) {
        fldFormatAreaDirectory(directory, area, 1);
        func_0035C860(path, D_00413128, directory, area, room);
        fldCachedRoomKF2ResourceAllocation = (u32)fldLoadCachedRoomKF2ResourceIfLocationMatches(&roomData, area, room);
        if (fldCachedRoomKF2ResourceAllocation == 0) {
            fldCachedRoomKF2ResourceAllocation = (u32)sdfReadNamedResource(path, &roomData, 0);
        }
        chunk = (FldTransferChunk *)((u8 *)roomData + 8);
        D_00435FBC = (u32)roomData;
        D_00435FB8 = chunk;
        fldRelocatePackedTransferChunk((u32)roomData, chunk);
        fldProcessFieldRequestAlternate((FldSceneRequest *)func_00129D60((s32)chunk));
    }
    if (isField == 0) {
        func_0035C860(path, D_00436038, name);
        fldCachedRoomF1ResourceAllocation = (u32)fldLoadCachedRoomF1ResourceIfLocationMatches(&roomData, area, room);
        if (fldCachedRoomF1ResourceAllocation == 0) {
            fldCachedRoomF1ResourceAllocation = (u32)sdfReadNamedResource(path, &roomData, 0);
        }
        chunk = (FldTransferChunk *)((u8 *)roomData + 8);
        D_00435FAC = (u32)roomData;
        D_00435FA8 = chunk;
        fldRelocatePackedTransferChunk((u32)roomData, chunk);
        fldLoadSceneRequestFiles((FldLoadRequest *)func_00129D60((s32)chunk));
    }
    fldCreateResourceScriptObjects();
    func_00129940();
    if (sceneWork->unk58 != 0 && area < 200 && fldAreaCachedResource != 0) {
        D_00435FDC = (s32)sdfAllocGeneralBlock(sdfMemoryGetBlockSize((struct SdfMemBlock *)fldAreaCachedResource));
        memcpy((void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)D_00435FDC),
               (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)fldAreaCachedResource),
               sdfMemoryGetBlockSize((struct SdfMemBlock *)fldAreaCachedResource));
        evtRetainSceneResource(dds3GetWorldSecondaryObject(), (void *)D_00435FDC);
    }
    func_00142B70();
    fldAreaState.unk20 = 0;
    if (fldAreaState.transitionMode != 0) {
        fldUpdateSparkSlots();
    }
    return fldCachedRoomF1ResourceAllocation;
}

extern u32 func_0012AC90(u32, u32, u32, u32,
                        const SdfTextureOffsetListHeader *, u32);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012AC90);

extern void fldClearMenuEntries();

extern void fldDestroyTitleTask();

extern void fldFreeSceneResources();

extern void fldPlayPendingSounds();

extern void fldReleaseObjectSlots();

extern void fldReleaseResourceSlots();

extern void fldReleaseTextureSlots();

extern void fldResetObjectSlots();

extern void fldResetRecordState();

extern s32 fldTitleMiniIsActive();

extern void fldResetZoneRecordsAndActorSlots();

extern void fldResetPendingSounds();

extern void fldReleaseSceneRecordChunk();

extern void fldReleaseMenuSlotsAfterWait();

extern void fldReleaseIndexedResourceEffect();


extern void mnuReleaseResourceEntries();

extern void sdfResourceListRelease();

extern u32 D_00444930[];

extern u32 D_00444940[];

extern s32 D_00435FA0;

extern struct DevRequest *D_00435FA4;

extern FldTransferChunk *D_00435FA8;

extern u32 D_00435FAC;

extern FldTransferChunk *D_00435FB0;

extern u32 D_00435FB4;

extern FldTransferChunk *D_00435FB8;

extern u32 D_00435FBC;

extern struct FileRequest *D_00435FC8;

extern s32 D_00435FDC;

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
                fldReleaseResourceSlots(&fldAreaState);
                fldReleaseMenuSlotsAfterWait();
                fldReleaseSceneRecordChunk();
                fldReleaseAreaResourceCache();
            }
        } else {
            fldReleaseResourceSlots(&fldAreaState);
            fldReleaseMenuSlotsAfterWait();
            fldReleaseSceneRecordChunk();
            fldFreeSceneResources();
            fldReleaseAreaResourceCache();
            fldReleaseSkyResources();
        }
        fldResetRecordState();
        fldResetZoneRecordsAndActorSlots();
        fldResetPendingSounds();
        fldClearMenuEntries();
        mnuReleaseResourceEntries();
        fldReleaseTextureSlots();
        fldReleaseIndexedResourceEffect();
    }
    sdfResourceListRelease(D_00435FA4, 1);
    D_00435FA4 = 0;
    for (i = 0; i < 4; i++) {
        if (D_00444930[i] != 0) {
            sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_00444930[i]);
            D_00444930[i] = 0;
            D_00444940[i] = 0;
        }
    }
    if (D_00435FC0 != 0) {
        if (D_00435FA8 != 0) {
            fldRelocateTransferChunkWords(D_00435FAC, D_00435FA8);
            D_00435FA8 = 0;
            D_00435FAC = 0;
        }
        if (D_00435FB0 != 0) {
            fldRelocateTransferChunkWords(D_00435FB4, D_00435FB0);
            D_00435FB0 = 0;
            D_00435FB4 = 0;
        }
        if (D_00435FB8 != 0) {
            fldRelocateTransferChunkWords(D_00435FBC, D_00435FB8);
            D_00435FB8 = 0;
            D_00435FBC = 0;
        }
    }
    D_00435FA0 = 0;
    D_00435FDC = 0;
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
    if (D_00435FC8 != 0) {
        work = ((FilePacRequest *)D_00435FC8)->packet.queueHead;
        i = 0;
        if (work != 0) {
            do {
                if (i > 0) {
                    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)(u32)work->resourceHandle);
                }
                work = work->next;
                i++;
            } while (work != 0);
        }
        func_002C7CE8(D_00435FC8);
        D_00435FC8 = NULL;
    }
}

void fldFormatAreaDirectory(char *buffer, s32 area, s32 unused) {
    if (area < 200) {
        func_0035C860(buffer, "/fld/f/f%03d/", area);
    } else if (area < 500) {
        func_0035C860(buffer, "/fld/b/f%03d/", area);
    } else {
        func_0035C860(buffer, "/fld/e/f%03d/", area);
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012B0D0);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413198);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012B2B8);

/* Enable or skip relocation of remaining transfer chunks during field teardown. */
void fldSetRelocateOnRelease(u32 relocateOnRelease) {
    D_00435FC0 = relocateOnRelease;
}

void fldInitDisplayObjects(void) {
    if (D_00436064 == 0) {
        SdfAsset *object;
        D_00436064 = 1;
        object = sdfCreateAssetWithDrawEntries();
        D_0043607C = (u32)object;
        object->unk1C = 1.0f;
        D_00438EC8 = (u32)sdfCreateAssetWithDrawEntries();
        fldMarkerTexture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(D_0038A700));
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
    s32 handle = (s32)sdfConsAllocateColumnPacket(1);
    FldSpriteVertex *vtx = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader(handle);
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
    sdfAppendPacket(command, handle);
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
    s32 handle = (s32)sdfConsAllocateColumnPacket(1);
    FldSpriteVertex *vtx = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader(handle);
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
    sdfAppendPacket(command, handle);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, command);
}

void fldProjectPointSetup(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    VU0_LOAD_MATRIX_MEMORY(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    VU0_MOVE_MATRIX_TO_B();
    VU0_LOAD_VF_MEMORY(vf10, vec);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF_MEMORY(vf11, D_0037F650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF_MEMORY(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldProjectPointSetupAlt(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    VU0_LOAD_MATRIX_MEMORY(D_00384790);
    sdfPostmultiplyVuMatrixFromMemory(D_0037F9B0);
    VU0_MOVE_MATRIX_TO_B();
    VU0_LOAD_VF_MEMORY(vf10, vec);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF_MEMORY(vf11, D_0037F9F0);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF_MEMORY(vf11, D_0037FA00);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPrepareProjectionMatrix(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX_MEMORY(sdfViewMatrix);
    matrix = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(matrix);
    VU0_MOVE_MATRIX_TO_B();
    matrix += 0x40;
    VU0_LOAD_VF_MEMORY(vf11, matrix);
    VU0_LOAD_VF_MEMORY(vf12, D_0037F660);
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
    *dstX = result[0];
    *dstY = result[1];
}


void fldSelectDisplayBuffer(u32 displayIndex) {
    fldDisplayRow = displayIndex;
}

void fldSubmitGsCommandWord(s32 lower, s32 bits, u64 upper) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *entry;
    SdfPoolNode *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(0x30);
    entry = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitFrameQuad(s32 flag0, s32 flag1, s32 field4, s32 field12, s32 flag14, s32 flag15, s32 unused, s32 field17) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    SdfPoolNode *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(0x30);
    data = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    data[4] = (field17 << 17) | 0x10000 | (flag15 << 15) | (flag14 << 14) | (field12 << 12) | (field4 << 4) | (flag1 << 1) | flag0;
    data[5] = 0x47;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

/* Write the selected blend equation to GS ALPHA_1, including DDS2's FIX mode. */
void func_0012BE18(s32 mode) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    SdfPoolNode *descriptor;

    sdfInitPacketList((SdfListHead *)command);
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
    case 30:
        data[4] = 0x80000000AAULL;
        break;
    default:
        data[4] = 0x44;
        break;
    }
    data[5] = 0x42;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitGsLinesScaled(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    s32 coords[4];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitGsLines(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    u32 coords[4];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitGsQuadTagged(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitPackedRectangleGsPacket(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitGsRect(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    u32 coords[8];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitGsGradientTriangle(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2) {
    s32 coords[6];
    s32 command;
    s32 packet;
    u64 *dst;
    s32 i;
    SdfPoolNode *descriptor;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    coords[4] = x2 * 16;
    coords[5] = y2 * 16;
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 3));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x4D, 2, 0x41, 3);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitGsGradientQuad(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3) {
    s32 coords[8];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x4C, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void func_0012CB08(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
    s32 coords[8];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitTaggedGsRectangle(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag) {
    s32 coords[8];
    s32 command;
    s32 packet;
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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

/* Model draw input: angle for the VU0 matrix and a geometry packet value. */
typedef struct FldModelPacketInput {
    u8 pad00[0x40];
    s32 geometryValue; /* 0x40 */
    f32 angle;         /* 0x44 */
} FldModelPacketInput;

void fldSubmitModelPacket(SdfTex *texture, u8 *modelData) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 header;
    s32 packet;
    f32 mat[16];
    SdfPoolNode *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    header = sdfAllocPacketAligned(0x20);
    sdfConsInitDmaPacketHeader((DmaPacketHeader *)header, (u32)sdfTexGetPrimaryBuffer(texture), sdfTexGetPrimaryBufferSize(texture));
    sdfAppendReferencePacket((SdfListHead *)command, header);
    func_003365B8(((FldModelPacketInput *)modelData)->angle);
    VU0_STORE_MATRIX(mat);
    packet = sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket((u8 *)packet, mat);
    sdfAppendPacket((SdfListHead *)command, packet);
    packet = sdfAllocPacketAligned(0x80);
    func_0033B530(packet, modelData, ((FldModelPacketInput *)modelData)->geometryValue, modelData + 0x10, modelData + 0x20);
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitPrimaryFramePacket(void) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 texture;
    SdfPoolNode *descriptor;
    sdfInitPacketList((SdfListHead *)command);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList((SdfListHead *)command, texture);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitAlternateFramePacket(void) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 texture;
    SdfPoolNode *descriptor;
    sdfInitPacketList((SdfListHead *)command);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB78((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList((SdfListHead *)command, texture);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void fldSubmitVectorColorPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    s32 resource;
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
    resource = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)resource);
    record = (s32)func_00348158(D_0038A980, D_0038A9A0, 2, 0x80);
    sdfAppendPacket((SdfListHead *)resource, record);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)resource);
}

void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    SdfPrimitiveRequest desc;
    f32 verts[12];
    s32 indices[3];
    s32 command;
    SdfPoolNode *descriptor;

    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    sdfConsAppendClearPacket((SdfListHead *)command, 0);
    sdfConsAppendAssetPacket((SdfListHead *)command, (SdfAsset *)D_0043607C, 0);
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
    sdfAppendPacket((SdfListHead *)command, (u32)func_0033B050(&desc));
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, (SdfListHead *)command);
}

void func_0012D3E0(void) {
    SdfListHead *packetList;
    SdfDmaNode *dmaPacket;
    s32 drawBufferIndex;
    u64 *firstPacket;
    u64 *secondPacket;
    s32 columnPacket;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    packetList = (SdfListHead *)sdfAllocatePacketList(0);
    dmaPacket = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    drawBufferIndex = (s32)kwlnGetDrawBufferIndex();
    sdfAppendDmaPrimary(packetList,
                        (u32)(kwlnFrameDrawPacketRecords + drawBufferIndex * 0x1F40),
                        dmaPacket);

    /* Set texture alpha and flush the texture cache. */
    firstPacket = (u64 *)sdfAllocPacketAligned(0x40);
    firstPacket[0] = 3;
    firstPacket[1] = 0x5000000310000000ULL;
    firstPacket[2] = 0x1000000000008002ULL;
    firstPacket[3] = 14;
    firstPacket[4] = 0x0000008000000080ULL;
    firstPacket[5] = 0x3B;
    firstPacket[6] = 0;
    firstPacket[7] = 0x3F;
    sdfAppendPacket(packetList, (u32)firstPacket);

    /* Set the primary-context alpha test and blending values. */
    secondPacket = (u64 *)sdfAllocPacketAligned(0x40);
    secondPacket[0] = 3;
    secondPacket[1] = 0x5000000310000000ULL;
    secondPacket[2] = 0x1000000000008002ULL;
    secondPacket[3] = 14;
    secondPacket[4] = 0x0000000000031001ULL;
    secondPacket[5] = 0x47;
    secondPacket[6] = 0x44;
    secondPacket[7] = 0x42;
    sdfAppendPacket(packetList, (u32)secondPacket);

    columnPacket = (s32)sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader(columnPacket);
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
    sdfAppendPacket(packetList, (u32)columnPacket);

    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append(surface, packetList);
}

void fldSubmitOverlaySpriteWithRenderState(s32 mode) {
    SdfListHead *packetList;
    SdfDmaNode *dmaPacket;
    s32 drawBufferIndex;
    u64 *firstPacket;
    u64 *secondPacket;
    s32 columnPacket;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    packetList = (SdfListHead *)sdfAllocatePacketList(0);
    dmaPacket = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    drawBufferIndex = (s32)kwlnGetDrawBufferIndex();
    sdfAppendDmaPrimary(packetList,
                        (u32)(kwlnFrameDrawPacketRecords + drawBufferIndex * 0x1F40),
                        dmaPacket);

    firstPacket = (u64 *)sdfAllocPacketAligned(0x40);
    firstPacket[0] = 3;
    firstPacket[1] = 0x5000000310000000ULL;
    firstPacket[2] = 0x1000000000008002ULL;
    firstPacket[3] = 14;
    firstPacket[4] = 0x0000008000000080ULL;
    firstPacket[5] = 0x3B;
    firstPacket[6] = 0;
    firstPacket[7] = 0x3F;
    sdfAppendPacket(packetList, (u32)firstPacket);

    secondPacket = (u64 *)sdfAllocPacketAligned(0x40);
    secondPacket[0] = 3;
    secondPacket[1] = 0x5000000310000000ULL;
    secondPacket[2] = 0x1000000000008002ULL;
    secondPacket[3] = 14;
    secondPacket[4] = 0x0000000000033001ULL;
    secondPacket[5] = 0x47;
    secondPacket[6] = 0x0000008000000064ULL;
    secondPacket[7] = 0x42;
    sdfAppendPacket(packetList, (u32)secondPacket);

    columnPacket = (s32)sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader(columnPacket);
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
    sdfAppendPacket(packetList, (u32)columnPacket);

    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append(surface, packetList);
}

void fldSubmitOverlayStateAndSprite(s32 alpha) {
    SdfListHead *packetList;
    SdfDmaNode *dmaPacket;
    s32 drawBufferIndex;
    u64 *texturePacket;
    u64 *blendPacket;
    s32 columnPacket;
    FldSpriteVertex *vertex;
    SdfPoolNode *surface;

    packetList = (SdfListHead *)sdfAllocatePacketList(0);
    dmaPacket = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    drawBufferIndex = (s32)kwlnGetDrawBufferIndex();
    sdfAppendDmaPrimary(packetList,
                        (u32)(kwlnFrameDrawPacketRecords + drawBufferIndex * 0x1F40),
                        dmaPacket);

    texturePacket = (u64 *)sdfAllocPacketAligned(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 14;
    texturePacket[4] = 0x0000008000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfAppendPacket(packetList, (u32)texturePacket);

    blendPacket = (u64 *)sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 14;
    blendPacket[4] = 0x0000000000033001ULL;
    blendPacket[5] = 0x47;
    blendPacket[6] = ((u64)(u32)alpha << 32) | 0x64;
    blendPacket[7] = 0x42;
    sdfAppendPacket(packetList, (u32)blendPacket);

    columnPacket = (s32)sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader(columnPacket);
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
    sdfAppendPacket(packetList, (u32)columnPacket);

    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append(surface, packetList);
}

typedef struct FldMarkerPacket {
    f32 pos[3];
    s32 pad0C;
    s16 rot[8];
    f32 quad[8];
    s32 color;
    f32 scale;
} FldMarkerPacket;

void fldDrawMarkerQuad(f32 *pos) {
    FldMarkerPacket packet;
    f32 half = 36.0f;

    packet.rot[0] = 0;
    packet.rot[1] = 0;
    packet.rot[2] = 0x200;
    packet.rot[3] = 0;
    packet.rot[4] = 0x200;
    packet.rot[5] = 0x200;
    packet.rot[6] = 0;
    packet.rot[7] = 0x200;
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
    func_0012BE18(0);
    fldSubmitModelPacket(fldMarkerTexture, (u8 *)&packet);
}

void fldDrawMarkerQuadColored(f32 *pos, s32 color) {
    FldMarkerPacket packet;
    f32 half = 36.0f;

    packet.rot[0] = 0;
    packet.rot[1] = 0;
    packet.rot[2] = 0x200;
    packet.rot[3] = 0;
    packet.rot[4] = 0x200;
    packet.rot[5] = 0x200;
    packet.rot[6] = 0;
    packet.rot[7] = 0x200;
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
    func_0012BE18(0);
    fldSubmitModelPacket(fldMarkerTexture, (u8 *)&packet);
}

void fldDrawStretchableFrame(s32 x, s32 y, s32 width, s32 height) {
    fldSubmitSpriteRect(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitSpriteRect(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitSpriteRect(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_0012BE18(0);
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

void fldSubmitBackgroundResourcePacket(void) {
    if (fldBackgroundBuffer != 0) {
        SdfListHead *packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateResourcePacket(packet, (SdfTexResource *)D_0040B2A0[0],
                                0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0, 0, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append(descriptor, packet);
    }
}

void fldSubmitBackgroundDescriptorPacket(void) {
    if (fldBackgroundBuffer != 0) {
        SdfListHead *packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateDescriptorPacket(packet, (SdfTexResource *)D_0040B2A0[0],
                                  0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append(descriptor, packet);
    }
}

void func_0012DDC0(s32 x, s32 y, u32 firstPayload, const u8 *secondPayload) {
    u32 object;

    object = itfCreateConvertedTextGlyph(x << 4, y << 4, 0, firstPayload, secondPayload, 0);
    frFontDrawGlyphInDefaultMode(object);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)object);
}

void fldAdvanceQuadRow(FldQuadState *quad) {
    quad->rowY = quad->rowY + 0x60;
}

void fldStartQuadPacketList(FldQuadState *quad) {
    s32 packet;
    u32 packetList;

    packetList = (u32)sdfCreateResetPacketList();
    quad->packetList = packetList;
    packet = sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket((SdfGsBlendPacket *)packet);
    sdfAppendPacket((SdfListHead *)quad->packetList, packet);
}

void func_0012DE70(f32 x, f32 y, f32 z, s32 drawValue) {
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
    quad.rgba = 0x80806020;
    quad.baseX = 0x73C0;
    quad.baseY = 0x8440;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, (s32)screenX * 16, (s32)screenY * 16, quad.drawDepth, 1);
    sdfAppendPacket((SdfListHead *)quad.packetList,
                    (u32)sdfFormatSifPacket(&packet, D_00436070, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append(&D_00380708, (SdfListHead *)quad.packetList);
}

void fldDrawFloorQuad(s32 x, s32 y, const char *format) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x73C0;
    quad.rowY = 0x7CC0;
    quad.drawDepth = 0x0FFFFF7E;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.rgba = 0x80806020;
    quad.baseX = 0x73C0;
    quad.baseY = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7D;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, format));
    fldAdvanceQuadRow(&quad);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)quad.packetList);
}

void fldDrawFloorQuadA(s32 x, s32 y, s32 packetField, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x73C0;
    quad.rowY = 0x7CC0;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.rgba = 0x80806020;
    quad.baseX = 0x73C0;
    quad.baseY = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append(&D_00380708, (SdfListHead *)quad.packetList);
}

void fldDrawMapQuadTiled(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.rgba = 0x80806020;
    quad.baseX = 0x7000;
    quad.baseY = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, D_00436070, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append(&D_00380708, (SdfListHead *)quad.packetList);
}

void fldDrawMapQuadTiledAlt(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.rgba = 0x80806020;
    quad.baseX = 0x7000;
    quad.baseY = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, D_00436078, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append(&D_00380708, (SdfListHead *)quad.packetList);
}

void fldDrawMapQuad(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.rgba = 0x80806020;
    quad.baseX = 0x7000;
    quad.baseY = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append(&D_00380708, (SdfListHead *)quad.packetList);
}

void fldDrawMapQuadPacket(s32 x, s32 y, s32 packetField, s32 drawValue) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.rgba = 0x80806020;
    quad.baseX = 0x7000;
    quad.baseY = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append(&D_00380708, (SdfListHead *)quad.packetList);
}

void fldDrawMapQuadScaled(s32 packetField, s32 drawValue, f32 x, f32 y) {
    FldQuadState quad;
    SifCommand packet;

    quad.rowX = 0x7000;
    quad.rowY = 0x7900;
    quad.drawDepth = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.rgba = 0x80806020;
    quad.baseX = 0x7000;
    quad.baseY = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + (s32)(x * 16.0f), quad.rowY + (s32)(y * 8.0f), quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append(&D_00380708, (SdfListHead *)quad.packetList);
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
        func_0012BE18(5);
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
        func_0012BE18(0);
    }
}

void fldDrawExpandedSpriteStrip(s32 alpha, s32 offset) {
    SdfListHead *list = (SdfListHead *)sdfAllocatePacketList(NULL);
    SdfDmaNode *reference = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    s32 handle;
    FldSpriteVertex *vertex;
    s32 quarter;
    SdfPoolNode *descriptor;

    sdfAppendDmaPrimary(list,
        (u32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), reference);
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
    handle = (s32)sdfConsAllocateColumnPacket(1);
    vertex = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader(handle);
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
    sdfAppendPacket(list, handle);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append(descriptor, list);
}

s32 fldGetEncounterRuntimeResult(void) {
    s32 state = fldEncounterRuntimeState;
    s32 result;

    if (state < 3) {
        if (state < 0) {
            result = D_0043608C;
        } else {
            result = func_0022E450();
        }
    } else {
        result = D_0043608C;
    }
    return result;
}

void fldSetEncounterPendingValue(u32 value) {
    D_00436088 = value;
}

s32 fldEncProc(void) {
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
    D_0043608C = 0;
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

void fldResetEncounterAsyncState(void) {
    btlResetAsyncState();
}

void fldCreateEncounterTask(void) {
    kwlnTaskCreate((s32)fldEncounterTaskName, 0x2B0F, 0, 1, (s32)fldEncProc, (s32)fldResetEncounterAsyncState, 0);
    btlClearRuntimeState();
}

void fldUpdateLookAtSegmentDistance(void) {
    f32 *distance = D_003897DC;
    f32 dx = fldLookAtNearPoint[0] - fldLookAtFarPoint[0];
    f32 dy = fldLookAtNearPoint[1] - fldLookAtFarPoint[1];
    f32 dz = fldLookAtNearPoint[2] - fldLookAtFarPoint[2];
    *distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

extern u32 fldValueRecords;
extern f32 func_00124A10(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

/* Probe the support surface under the camera's focus and its mirrored heading. */
void func_0012EDB0(void) {
    FldAreaWork *cam;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    f32 offset[3];
    f32 x;
    s32 index;
    EffWorldNode *node;

    fldAreaState.unk5C = 0.0f;
    fldAreaState.unk17C = 0.0f;
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
    cam->unk5C = cam->y - func_00124A10(x,
        cam->z - sdfEvaluateCosineViaSinePhaseShift(cam->negatedAngle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist, xs, ys, zs);
    x = cam->x - sdfSinPoly(-cam->angle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist;
    cam->unk17C = cam->y - func_00124A10(x,
        cam->z - sdfEvaluateCosineViaSinePhaseShift(-cam->angle * 3.14f / 180.0f) * fldCameraFollowRows[cam->rowIdx].dist, xs, ys, zs);
    if (cam->overrideSupportRecordIndex == -1) {
        cam->unk5C = 0.0f;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012F078);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012F400);

void fldUpdateCameraProjectionEndpoints(void) {
    FldAreaWork *cam = &fldAreaState;
    f32 angle;

    D_0038BAF0[0] = cam->x - sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * 80.0f;
    D_0038BAF0[1] = cam->y + fldCameraFollowRows[cam->rowIdx].y;
    D_0038BAF0[2] = cam->z - sdfEvaluateCosineViaSinePhaseShift(cam->negatedAngle * 3.14f / 180.0f) * 80.0f;
    D_0038BAF0[3] = 1.0f;
    angle = cam->negatedAngle * 3.14f / 180.0f;
    D_0038BB00[0] = cam->x + sdfSinPoly(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_0038BB00[1] = cam->y + fldCameraFollowRows[cam->rowIdx].targetY;
    D_0038BB00[2] = cam->z + sdfEvaluateCosineViaSinePhaseShift(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_0038BB00[3] = 1.0f;
}

extern f32 D_0038BAD0[];
extern s32 D_004360B4;
extern s32 D_004360B8;
extern s32 D_003897C0[];


void fldInterpolateCameraEndpoints(void) {
    union {
        u128 q;
        f32 f[4];
    } nearPoint, farPoint;
    u32 *world = fldGetPlayerSceneStateAddress();
    f32 ratio = (f32)D_004360B4 / (f32)D_004360B8;
    f32 remaining = 1.0f - ratio;
    EffWorldNode *node;

    nearPoint.f[0] = D_0038BAF0[0] * ratio + D_0038BAD0[0] * remaining;
    nearPoint.f[1] = D_0038BAF0[1] * ratio + D_0038BAD0[1] * remaining;
    nearPoint.f[2] = D_0038BAF0[2] * ratio + D_0038BAD0[2] * remaining;
    farPoint.f[0] = D_0038BB00[0] * ratio + D_0038BB00[0] * remaining;
    farPoint.f[1] = D_0038BB00[1] * ratio + D_0038BB00[1] * remaining;
    farPoint.f[2] = D_0038BB00[2] * ratio + D_0038BB00[2] * remaining;
    PCP_COPY_VECTOR(fldLookAtNearPoint, &nearPoint);
    PCP_COPY_VECTOR(fldLookAtFarPoint, &farPoint);
    effObjSetInnerPosition((EffWorldNode *)(u32)*world, &farPoint.q);
    node = (EffWorldNode *)(u32)*world;
    node->ops->update(node);
    D_004360B4++;
    if (D_004360B8 < D_004360B4) {
        D_003897C0[0] = 0;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0012FA58);

extern void dds3SetCameraVector(EffWorldNode *camera, u128 *worldEye);
extern f32 fldGetNormalizedComplementaryAngle(f32 ax, f32 ay, f32 bx, f32 by);
extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);
extern void dds3LoadOrBuildObjectMatrix(EffWorldNode *object);

void func_001300A0(void) {
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

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001302A0);

s64 fldGetUnselectedWorldEntry(void) {
    s32 worldObject;
    s64 object;
    s64 currentObject;

    worldObject = (s32)dds3GetWorldObject();
    object = (s32)dds3GetWorldCameraObject((EffWorldNode *)worldObject);
    currentObject = fldGetPlayerSceneState();
    if (currentObject == object) {
        object = 0;
    }
    return object;
}

void fldSetCameraMoveMode(u32 value) {
    D_004360AC = value;
    dds3TransformCameraVectorsByInnerRotation(dds3GetWorldCameraObject(dds3GetWorldObject()), D_00444980,
                                              D_00444970);
    D_004360B0 = 0;
}


extern void effObjSetNodeFlags(void *, s32);

void fldUpdateCameraMoveOscillation(void) {
    f32 direction = 0.0f;
    f32 phase = D_004360B0;
    EffWorldNode *camera;

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
        camera = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (D_004360AC == 1 || D_004360AC == -1) {
            if (phase < 3.14f) {
                phase += 0.2f;
                camera->inner->position[1] = D_00444970[1] + sdfSinPoly(phase * direction) * 2.5f;
            } else {
                phase += 0.02f;
                camera->inner->position[1] = D_00444970[1] + sdfSinPoly(phase * direction) * 2.0f;
            }
        }
        if (D_004360AC == 2 || D_004360AC == -2) {
            phase += 11.0f;
            if (phase > 0.0f) {
                phase -= 1.0f;
            } else if (phase > -3.14f) {
                phase -= 0.2f;
                camera->inner->position[1] = D_00444970[1] + sdfSinPoly(phase * direction) * 2.0f;
            } else {
                phase -= 0.01f;
            }
            phase -= 11.0f;
        }
        D_004360B0 = phase;
        effObjSetNodeFlags(camera->inner, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
    }
}

void fldClearCameraMoveMode(void) {
    D_004360AC = 0;
}

extern f32 fldPointDistance(f32, f32, f32, f32, f32, f32);
extern void fldSetCameraObjectHighlightFlag(void);
extern void fldClearSceneModelColors(void);
extern void fldMarkPrimaryObjectByPeerPresence(void);
extern void fldClearCameraObjectTransitionFlags(void);

extern u8 fldTestSceneControlFlags(u32 mask);

void fldUpdateCameraProximity(void) {
    f32 vec[4];
    f32 range = 45.0f;
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
    if (mdlFlagTest(0x31)) {
        range = 56.0f;
        slot = 7;
    }
    if (slot >= 0) {
        model = *modelRef;
        matrices = *(u8 ***)(model + 0xC);
        VU0_LOAD_MATRIX(matrices[slot] + 0xC0);
        VU0_LOAD_VF(vf10, vec);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vec);
        if (fldPointDistance(vec[0], vec[1], vec[2], fldLookAtFarPoint[0], fldLookAtFarPoint[1], fldLookAtFarPoint[2]) < range) {
            fldTestSceneControlFlags(0x40);
            fldClearSceneModelColors();
            fldClearCameraObjectTransitionFlags();
            fldClearCameraObjectHighlightFlag();
        } else {
            fldMarkPrimaryObjectByPeerPresence();
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

extern void fldRestoreSceneModelColors(void);
extern void fldUpdateCameraMoveOscillation(void);
extern void func_0012FA58(void);
extern void func_001302A0(void);
extern void fldInterpolateCameraEndpoints(void);

s32 fldUpdateCameraFollow(void) {
    FldAreaWork *cam;
    u32 *world = fldGetPlayerSceneStateAddress();
    if (*world != 0 && fldPlayerObject != 0) {
        fldRestoreSceneModelColors();
        if (fldGetUnselectedWorldEntry() != 0) {
            fldToggleWorldNodeState(1);
            fldUpdateCameraMoveOscillation();
            fldClearCameraObjectHighlightFlag();
            return 0;
        }
        func_0012EDB0();
        cam = &fldAreaState;
        dds3SetCameraFieldOfView(*world, fldCameraFollowRows[cam->rowIdx].fov * 3.14f / 180.0f);
        switch (cam->mode) {
        case 0:
            func_0012FA58();
            if (cam->dist < 50.0f) {
                func_0012FA58();
            }
            break;
        case 1:
            func_001302A0();
            break;
        case 4:
            fldInterpolateCameraEndpoints();
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
    result = dds3TestObjectFlags(fldPlayerObject, 1);
    if (result != 0) {
        dds3SetObjectFlags(fldSecondarySceneObject, 1);
        result = 0;
    }
    return result;
}

void fldResetCameraModelHandles(void) {
    D_004360C8 = 0;
    D_004360C4 = 0xffffffff;
    D_004360CC = 0;
    D_004360D0 = 0;
}

void fldReleaseCameraModel(u32 enabled) {
    if (enabled == 0) {
        D_004360D0 = 0;
        if (fldPlayerModelContext != 0) {
            mdlResumeAllContextMotions(fldPlayerModelContext);
        }
    } else {
        D_004360D0 = enabled;
        mdlSuspendAllContextMotions(fldPlayerModelContext);
    }
}

void func_00130FF0(u32 first, u32 second) {
    D_004360C4 = first;
    D_004360C8 = second;
}

void fldUpdateCameraModelMotion(s32 modelMotion, s32 motion, f32 blendFrames) {
    s32 currentMotion;

    if (D_00389888[0] == 1) {
        if (D_004360D0 > 0) {
            if (--D_004360D0 != 0) {
                return;
            }
            mdlResumeAllContextMotions(fldPlayerModelContext);
        }
        currentMotion = D_004360CC;
        switch (motion) {
        case 1:
            break;
        case 2:
            if (D_004360C8 > 0) {
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
        if (D_004360C4 == 1) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 1, motion, blendFrames, blendFrames);
            D_004360C4 = -1;
            if (motion == 2 && D_004360CC == 3) {
                D_004360CC = motion;
                fldPlayerModelContext->first->frameStep = 1.0f;
                mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
            }
        } else if (D_004360C4 == 0x66) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 2, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 0x66;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
        } else if (D_004360C4 == 2) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 2, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 2;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 2, blendFrames, blendFrames);
        } else if (D_004360C4 == 3) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 3, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 3;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 3, blendFrames, blendFrames);
        } else if (D_004360C4 == 5) {
            mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
            mdlAddEntryPlainEx(fldPlayerModelContext, 1, 5, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 5;
            fldPlayerModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 5, blendFrames, blendFrames);
        } else if (D_004360C4 == 6) {
            if (motion == 1) {
                mdlSetNodeFrameStep(fldPlayerModelContext, 1, 2.0f);
                mdlAddEntryPlain(fldPlayerModelContext, 1, 6);
                D_004360C4 = -1;
            } else {
                mdlSetNodeFrameStep(fldPlayerModelContext, 1, 2.0f);
                mdlAddEntryPlain(fldPlayerModelContext, 1, 4);
                D_004360C4 = -1;
                D_004360CC = 4;
                fldPlayerModelContext->first->frameStep = 1.0f;
                mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, 4, blendFrames, blendFrames);
            }
        } else if (currentMotion != motion && D_004360CC != 5) {
            D_004360CC = motion;
            fldPlayerModelContext->first->frameStep = 1.5f;
            mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, motion, blendFrames, blendFrames);
            if (D_004360C8 == 0) {
                mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
                mdlAddEntryFlaggedEx(fldPlayerModelContext, 1, motion, blendFrames, blendFrames);
            }
        }
        if (D_004360C8 > 0) {
            D_004360C8--;
        }
    } else if (modelMotion != motion) {
        fldPlayerModelContext->first->frameStep = 1.0f;
        mdlAddEntryFlaggedEx(fldPlayerModelContext, 0, motion, blendFrames, blendFrames);
        if (fldSecondarySceneObject != 0) {
            fldSecondarySceneModelContext->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(fldSecondarySceneModelContext, 0, motion, blendFrames, blendFrames);
        }
    }
}

struct EffRandState;
extern u32 effMiscRand(struct EffRandState *state);
extern void fldSetSequenceVolume(s32 category, s32 volume);
extern void fldQueuePrimaryEffectPosition(f32 x, f32 y, f32 z);
extern void fldQueueSecondaryEffectPosition(f32 x, f32 y, f32 z);
extern s32 sdfLoadMapRecordPositionVector(SdfModel *model, s32 id);
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

/* Dispatch camera-model frame crossings to sequence sounds and queued effects. */
void func_00131478(s32 motion, s32 selector) {
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
    previousFrame = fldAreaState.unk178;
    currentFrame = cameraModel->first->currentFrame;
    fldAreaState.unk178 = currentFrame;

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
        volume -= (effMiscRand(NULL) >> 2) & 0x3F;
        if (volume >= 128) {
            volume = 127;
        }
        if (volume < 0) {
            volume = 0;
        }
    }

    if (fldAreaState.unk118 == 1) {
        sequenceCategory = (D_004360D4 % 2) + 0x19;
        if (selector != 1) {
            return;
        }
        if ((previousFrame < 7.0f && currentFrame >= 7.0f) ||
            (previousFrame < 17.0f && currentFrame >= 17.0f)) {
            sndSetSequenceVolumePan(0x680000 + sequenceCategory, 0x7F, 0x3F);
            D_004360D4 += (effMiscRand(NULL) >> 4 & 1) + 1;
        }
        return;
    }

    if (motion != 0 && motion != 2 && motion != 3 && selector == 2) {
        fldSetSequenceVolume(sequenceCategory + 3, volume);
        fldQueueSecondaryEffectPosition(fldAreaState.x, fldAreaState.y - 5.0f, fldAreaState.z);
        return;
    }
    sequenceCategory += D_004360D4 % 3;

    if (selector == 1) {
        if (previousFrame < 8.0f && currentFrame >= 8.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xCA) == 0) {
                return;
            }
            VU0_STORE_VF(vf10, &position);
            fldQueueSecondaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 18.0f && currentFrame >= 18.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xC9) == 0) {
                return;
            }
            VU0_STORE_VF(vf10, &position);
            fldQueuePrimaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
    } else if (selector == 0x17) {
        if (previousFrame < 88.0f && currentFrame >= 88.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xCA) == 0) {
                return;
            }
            VU0_STORE_VF(vf10, &position);
            fldQueueSecondaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 18.0f && currentFrame >= 18.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xC9) == 0) {
                return;
            }
            VU0_STORE_VF(vf10, &position);
            fldQueuePrimaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 42.0f && currentFrame >= 42.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xCA) == 0) {
                return;
            }
            VU0_STORE_VF(vf10, &position);
            fldQueueSecondaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
            return;
        }
        if (previousFrame < 63.0f && currentFrame >= 63.0f) {
            fldSetSequenceVolume(sequenceCategory, volume);
            D_004360D4 += (effMiscRand(NULL) >> 4 & 1) + 1;
            if (sdfLoadMapRecordPositionVector(fldPlayerModelContext->inner, 0xC9) == 0) {
                return;
            }
            VU0_STORE_VF(vf10, &position);
            fldQueuePrimaryEffectPosition(position.f[0], fldAreaState.y - 5.0f, position.f[2]);
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

    if (fldPlayerObject != 0 && (st = &fldAreaState, st->positionMode != 1) && D_00435F30 != 0) {
        cur[0] = st->x;
        cur[1] = st->y;
        cur[2] = st->z;
        func_00139950(cur);
        if (st->positionPending != 0) {
            st->x = st->targetX;
            st->y = st->targetY;
            st->z = st->targetZ;
            vec.f[0] = st->targetX;
            vec.f[1] = st->targetY;
            vec.f[2] = st->targetZ;
            effObjSetInnerPosition((EffWorldNode *)fldPlayerObject, (u128 *)vec.f);
            st->positionPending = 0;
            effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
            VU0_STORE_VF(vf10, &vec);
            dst = (u128 *)(*(u32 *)(fldPlayerObject + 0x1C) + 0x70);
            PCP_COPY_VECTOR(dst, &vec);
        }
    }
}

/* ASM mode-1 path: save previous XYZ, consume positionPending, and wrap degree angles. */
INCLUDE_ASM(const s32, "field/fldFileResolver", func_00131B50);

/* ASM signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */
INCLUDE_ASM(const s32, "field/fldFileResolver", func_001321F8);

extern void func_002A2200(s32 soundEntryIndex);
extern void mnuResetTitleStreamAfterFileIdle(void);
extern void mnuMarkTitleStreamResetPending(void);
extern void mnuResetTitleStreamLocked(void);

/* Advance the field transition while the camera model's motion drives completion. */
s32 func_001322D8(void) {
    FldAreaWork *work = &fldAreaState;
    MdlCtx *model;
    s16 node;

    if (work->unk188 > 0) {
        model = fldPlayerModelContext;
        node = model->current.h.motionIndex;
        if (node != 7) {
            kwlnFadeStartIn(8);
            func_002A2200(0x26);
            model = fldPlayerModelContext;
            model->first->frameStep = 1.0f;
            mdlAddEntryPlain(model, 0, 7);
        }

        {
            Motion *const thresholdMotion = fldPlayerModelContext->first;
            f32 currentFrame = thresholdMotion->currentFrame;
            if (work->unk178 < 13.0f && 13.0f <= currentFrame) {
                kwlnPadStartMotor(0, 1, 10);
                kwlnPadStartMotor(1, 0xE6, 10);
                mnuResetTitleStreamAfterFileIdle();
            }
        }
        {
            Motion *const completionMotion = fldPlayerModelContext->first;
            if (completionMotion->state == SDF_MOTION_STATE_TERMINAL) {
                mnuMarkTitleStreamResetPending();
                mnuResetTitleStreamLocked();
                fldAreaState.unk188 = 0;
                fldSetSceneControlFlags(0x40);
                return 0;
            }
        }

        func_00131478(node, 7);
        return -1;
    }

    return 0;
}

extern s32 ptyAnyUnitFlagMatch(u32 statusMask, s32 flagMode);
extern void fldPlayMenuSound(s32 soundId);
extern void kwlnFadeOutStart(s32 red, s32 green, s32 blue, s32 duration);
extern void kwlnPadStartMotor(u32 motor, u8 level, s32 duration);
extern void fldSetSceneControlFlags(u32 mask);
extern s32 fldPlaceAreaDamageEffect(f32 x, f32 y, f32 z);

s32 func_00132408(void) {
    FldAreaWork *work = &fldAreaState;
    MdlCtx *model;

    if (work->unk190 > 0) {
        if (work->unk11C == 0 && ptyAnyUnitFlagMatch(0x80, 1) != 0) {
            fldPlayMenuSound(0x22);
            kwlnFadeOutStart(0x80, 0x20, 0x20, 6);
            kwlnPadStartMotor(0, 1, 6);
            kwlnPadStartMotor(1, 0x96, 6);
            work->unk190 = 0;
            fldSetSceneControlFlags(0x40);
            return -1;
        }

        model = fldPlayerModelContext;
        if (model->current.h.motionIndex != 4) {
            model->first->frameStep = 1.0f;
            mdlAddEntryPlain(model, 0, 4);
            fldPlaceAreaDamageEffect(fldAreaState.x, fldAreaState.y, fldAreaState.z);
            fldPlayMenuSound(0x22);
            kwlnPadStartMotor(0, 1, 15);
            kwlnPadStartMotor(1, 0x64, 15);
            model = fldPlayerModelContext;
        }

        if (model->first->state == SDF_MOTION_STATE_TERMINAL) {
            fldAreaState.unk190 = 0;
            fldSetSceneControlFlags(0x40);
        } else {
            return -1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00132540);

extern u32 fldGetSceneReadyFlag(void);
extern void fldClearCameraObjectHighlightFlag(void);
extern s32 func_001321F8(void);
extern void func_00131B50(void);
extern void func_00131478(s32, s32);
extern void func_00132540(void);
extern u32 D_00389988[];
extern s32 D_003897C0[];

s32 fldUpdateCameraFrame(void) {
    s16 node;

    if (D_00389988[0] != 0) {
        return 0;
    }
    if (fldGetSceneReadyFlag() != 0) {
        return 0;
    }
    if (fldTestSceneControlFlags(0x40) == 0) {
        if (fldAreaState.mode == 1 || fldAreaState.mode == 3) {
            fldClearCameraObjectHighlightFlag();
        }
        func_001321F8();
        fldUpdateCameraTarget();
        func_00131B50();
        node = fldPlayerModelContext->current.h.motionIndex;
        func_00131478(node, node);
        if (fldAreaState.unk118 == 1) {
            fldUpdateCameraModelMotion(0, 0, 6.0f);
        }
        fldSyncObjectFlagsB();
        return 0;
    }
    if (D_003897C0[0] == 1) {
        fldClearCameraObjectHighlightFlag();
        func_00132540();
        fldSyncObjectFlagsB();
    } else {
        func_00132540();
        fldSyncObjectFlagsB();
    }
    return 0;
}

u8 fldHasPendingSceneFlags(void) {
    if (fldAreaState.unk188 == 0) {
        if (fldAreaState.unk190 == 0) {
            if (fldAreaState.verticalStepDirection == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void fldHideSceneModelsAndResetCamera(void) {
    dds3SetObjectFlags(fldPlayerObject, 1);
    if (fldSecondarySceneObject != 0) {
        dds3SetObjectFlags(fldSecondarySceneObject, 1);
    }
    fldSetCameraNodeModeWithZero();
}

void fldShowSceneModels(void) {
    dds3ClearObjectFlags(fldPlayerObject, 1);
    if (fldSecondarySceneObject != 0) {
        dds3ClearObjectFlags(fldSecondarySceneObject, 1);
        return;
    }
}

void fldClearCameraObjectHighlightFlag(void) {
    dds3SetOwnedWorldInnerValue(fldPlayerObject, 0);
    dds3ClearObjectFlags(fldPlayerObject, 0x800);
}

void fldSetCameraObjectHighlightFlag(void) {
    dds3SetOwnedWorldInnerValue(fldPlayerObject, 0x80);
    dds3SetObjectFlags(fldPlayerObject, 0x800);
}

void fldClearSceneModelColors(void) {
    u8 hasSecondObject;

    hasSecondObject = fldSecondarySceneObject != 0;
    fldPlayerModelContext->inner->color = 0;
    if (hasSecondObject) {
        fldSecondarySceneModelContext->inner->color = 0;
    }
}

void fldRestoreSceneModelColors(void) {
    u8 hasSecondObject;

    hasSecondObject = fldSecondarySceneObject != 0;
    fldPlayerModelContext->inner->color = 0x80808080;
    if (hasSecondObject) {
        fldSecondarySceneModelContext->inner->color = 0x80808080;
    }
}

void fldMarkPrimaryObjectByPeerPresence(void) {
    if (fldSecondarySceneObject != 0) {
        dds3SetObjectFlags(fldPlayerObject, 0x400);
        return;
    }
    dds3SetObjectFlags(fldPlayerObject, 0x200);
}

void fldClearCameraObjectTransitionFlags(void) {
    dds3ClearObjectFlags(fldPlayerObject, 0x400);
    dds3ClearObjectFlags(fldPlayerObject, 0x200);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00133B10);

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
    MdlCtx *object = fldPlayerModelContext;
    object->first->frameStep = 1.0f;
    mdlAddEntryPlainEx(object, 0, value, 2.0f, 5.0f);
}

void fldAddCameraModelPair(s32 first, s32 second) {
    mdlSetNodeFrameStep(fldPlayerModelContext, 0, 1.0f);
    mdlSetNodeFrameStep(fldPlayerModelContext, 1, 1.0f);
    mdlAddEntryFlagged(fldPlayerModelContext, 0, first);
    mdlAddEntryFlagged(fldPlayerModelContext, 1, second);
}

void func_00133DB8(void) {
    D_00389904[0] = 0;
}

void func_00133DC8(void) {
    D_00389910[0] = 0;
}

void fldQueueCameraXYOverride(f32 first, f32 second) {
    FldAreaWork *camera = &fldAreaState;

    camera->facingPointX = first;
    camera->facingPointZ = second;
    camera->pointState = 1;
}

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z) {
    FldAreaWork *camera;
    f32 angle;

    angle = sdfAtan2(x, z);
    camera = &fldAreaState;
    angle *= 180.0f / 3.14f;
    camera->angleState = 1;
    camera->overrideAngle = -angle;
}

void fldUpdateCameraHeadingFromXY(void) {
    f32 dx;
    f32 dz;

    if (fldAreaState.pointState != 0) {
        dx = fldAreaState.x - fldAreaState.facingPointX;
        dz = fldAreaState.z - fldAreaState.facingPointZ;
        if (dx < 0.0001f && dx > -0.0001f && dz < 0.0001f && dz > -0.0001f) {
            return;
        }
        fldAreaState.targetAngle = -(sdfAtan2(dx, dz) * (180.0f / 3.14f));
        fldAreaState.pointState = 2;
    }
}

void fldApplyPendingCameraHeading(void) {
    FldAreaWork *camera = &fldAreaState;

    if (camera->angleState != 0) {
        camera->angleState = 2;
        camera->targetAngle = camera->overrideAngle;
    }
}

/* ASM turning: smooth angle toward targetAngle, clearing pointState/angleState on arrival. */
INCLUDE_ASM(const s32, "field/fldFileResolver", func_00133F08);

typedef struct FldSkyBuffer {
    u32 word[0x3800];
} FldSkyBuffer;

extern FldSkyBuffer *fldSkyLightSetBuffer;




extern SdfFlagListParams fldCameraColorParameters[];
extern FldCameraSetting *fldCameraSettings;
extern FldCameraSetting D_0038BB70;
extern void *D_004360F0;

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413280);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413290);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004132A0);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004132E0);

void fldLoadBattleSkyAndFilter(void) {
    s32 i;
    DevState *command;

    if (fldSkyLightSetBuffer == 0) {
        fldSkyLightSetBuffer = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_004360F0 == 0) {
        D_004360F0 = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_00436104 == 0) {
        D_00436104 = (void *)sdfResourceRetainAddress(sdfAllocGeneralBlock(0x12400));
    }
    if (fldCameraSettings == 0) {
        fldCameraSettings = (FldCameraSetting *)sdfResourceRetainAddress(sdfAllocGeneralBlock(
            sizeof(FldCameraSetting) * FIELD_CAMERA_SETTING_COUNT));
        for (i = 0; i < FIELD_CAMERA_SETTING_COUNT; i++) {
            fldCameraSettings[i] = D_0038BB70;
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
    sdfDevQueueReadAndWait(command, D_00436104, 0x12400);
    sdfDevWaitThenReleaseCommandState(command);
    command = sdfDevCreateCommandState("/fld/f/bin/BATTLEBG.SKY");
    sdfDevQueueReadAndWait(command, D_004360F0, 0xE000);
    sdfDevWaitThenReleaseCommandState(command);
}

extern u32 fldRainTextureData;

extern char D_00413350[];



void fldLoadSkyResource(s32 area) {
    char path[64];
    char directory[32];
    DevState *command;

    fldSkyDrawState = 0x80;
    if (area < 200) {
        fldFormatAreaDirectory(directory, area, 1);
        if (area == 0x17 && mdlFlagTest(0x19)) {
            func_0035C860(path, "%sF%03dt.SKY", directory, 0x17);
        } else if (area == 0x18 && mdlFlagTest(0x19)) {
            func_0035C860(path, "%sF%03dt.SKY", directory, 0x18);
        } else if (area == 0x1B && mdlFlagTest(0x19)) {
            func_0035C860(path, "%sF%03dT.SKY", directory, 0x1B);
        } else {
            func_0035C860(path, "%sF%03d.SKY", directory, area);
        }
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, fldSkyLightSetBuffer, 0xE000);
        sdfDevWaitThenReleaseCommandState(command);
        if (area >= 2 && area < 100 && fldRainTextureResource == 0) {
            fldRainTextureResource = (s32)(u32)sdfReadNamedResource(D_00413350, &fldRainTextureData, 0);
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

void fldUploadSkyBuffer(FldSkyBuffer *src) {
    fldSkyDrawState = 0x80;
    *fldSkyLightSetBuffer = *src;
    fldReleaseSkyResources();
    if (D_00389780[0] >= 2 && D_00389780[0] < 100 && fldRainTextureResource == 0) {
        fldRainTextureResource = (s32)(u32)sdfReadNamedResource(D_00413350, &fldRainTextureData, 0);
        fldRainTextureReference = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(fldRainTextureData));
    }
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413350);

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

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00134A18);

void fldSetFadeTarget(s32 area, s32 value, s32 duration) {
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
    D_00436104[area * 73] = value;
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

void func_00135588(u32 value) {
    D_003899C0[0] = value;
}

u32 func_00135598(void) {
    return D_003899C0[0];
}

void fldBeginSelectedValueTransition(u32 value) {
    u32 selectedValue = D_003899B4[0];

    D_0043612C = value;
    D_00436130 = 0;
    D_00436134 = selectedValue;
    func_00135A68(selectedValue, 1);
}

void func_001355D8(void) {
    EvtUnit *player;
    f32 direction[4];
    s32 red, green, blue;
    u32 colorA, colorB;

    switch ((s32)D_0043612C) {
    case 0: /* Disabled. */
        break;
    case 1:
        D_00436130 += 1.0f;
        if (D_00436130 == 30.0f) {
            func_00135A68((D_00436134 + 1) & 0xFF, 30);
        } else if (D_00436130 == 60.0f) {
            func_00135A68(D_00436134, 30);
            D_00436130 = 0;
        }
        break;
    case 2:
        D_00436130 += 1.0f;
        if (D_00436130 == 15.0f) {
            func_00135A68((D_00436134 + 1) & 0xFF, 1);
        } else if (D_00436130 == 30.0f) {
            func_00135A68(D_00436134, 1);
            D_00436130 = 0;
        }
        break;
    case 3:
        D_00436130 += 1.0f;
        if (D_00436130 == 60.0f) {
            func_00135A68((D_00436134 + 1) & 0xFF, 60);
        } else if (D_00436130 == 120.0f) {
            func_00135A68(D_00436134, 60);
            D_00436130 = 0;
        }
        break;
    }
    if (kwlnTaskGetTaskByName(D_00413388) != NULL) {
        player = evtUnitGetNestedValue((EffWorldNode *)fldPlayerObject);
        evtSetUnitStatusFlags(player);
        red = D_0038BB10.color[0] * 128.0f;
        green = D_0038BB10.color[1] * 128.0f;
        blue = D_0038BB10.color[2] * 128.0f;
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = D_0038BB60.color[0] * 128.0f;
        green = D_0038BB60.color[1] * 128.0f;
        blue = D_0038BB60.color[2] * 128.0f;
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        evtInitializeUnitColorTransition(player, 0, colorA, colorB);
        direction[0] = D_0038BB10.direction[0];
        direction[1] = D_0038BB10.direction[1];
        direction[2] = D_0038BB10.direction[2];
        direction[3] = 0.0f;
        VU0_LOAD_VF(vf10, direction);
        evtSetUnitNormalizedDirection(player, 0);
    }
}

/* vu0 routine: the event direction setter takes its vector in vf10. */
void fldSetPlayerAndPeerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                  f32 redB, f32 greenB, f32 blueB,
                  f32 x, f32 y, f32 z) {
    EvtUnit *player;
    EvtUnit *secondary = 0;
    u32 colorA;
    u32 colorB;
    s32 red, green, blue;
    f32 direction[4];

    if (fldSecondarySceneObject != 0) {
        secondary = evtUnitGetNestedValue((EffWorldNode *)fldSecondarySceneObject);
        evtSetUnitStatusFlags(secondary);
    }
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
    if (fldSecondarySceneObject != 0) {
        evtInitializeUnitColorTransition(secondary, duration, colorA, colorB);
    }
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
    direction[3] = 0.0f;
    VU0_LOAD_VF(vf10, direction);
    evtSetUnitNormalizedDirection(player, duration);
    if (fldSecondarySceneObject != 0) {
        evtSetUnitNormalizedDirection(secondary, duration);
    }
    VU0_LOAD_VF(vf10, direction);
    evtSetUnitNormalizedDirection(player, 0);
    if (fldSecondarySceneObject != 0) {
        evtSetUnitNormalizedDirection(secondary, 0);
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

typedef struct {
    u8 type;
    u8 pad1[3];
    s32 unk4; /* 0x04: exported to the display state as slot 14; never read back */
    s32 fadeValue;
    s32 swayMode;
    u8 pad10[0xC];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 fixedVectorX;
    s32 fixedVectorY;
    s32 fixedVectorZ;
    f32 lightVectorAX;
    f32 lightVectorAY;
    f32 lightVectorAZ;
    f32 lightDirectionAX;
    f32 lightDirectionAY;
    f32 lightDirectionAZ;
    f32 lightVectorBX;
    f32 lightVectorBY;
    f32 lightVectorBZ;
    f32 lightDirectionBX;
    f32 lightDirectionBY;
    f32 lightDirectionBZ;
    f32 lightVectorCX;
    f32 lightVectorCY;
    f32 lightVectorCZ;
    f32 lightDirectionCX;
    f32 lightDirectionCY;
    f32 lightDirectionCZ;
    f32 finalVectorX;
    f32 finalVectorY;
    f32 finalVectorZ;
    f32 unitColorA[3];
    f32 unitLightDirection[3];
    u8 padA4[0x30];
    f32 unitColorB[3];
} FldLightSet; /* 0xE0 bytes */

extern s32 kwlnSetDrawColorTarget(s32, void *);
extern s32 kwlnSetLightColorTarget(s32, s32, void *);
extern s32 kwlnSetBackgroundColorTarget(s32, void *);
extern s32 kwlnSetLightDirectionTarget(s32, s32, void *);
extern s32 evtSetDrawVectorTarget(s32, f32, f32, f32, f32);
extern void fldSetSwayMode(u32);
extern u32 D_00436138, D_0043613C;
extern f32 D_00436140;
extern s32 D_00436144, D_00436148, D_0043614C;
extern f32 D_00436150, D_00436154;
extern u32 D_0043615C;
extern f32 D_00436160;
extern u32 D_00436164;

void func_00135A68(u32 index, s32 mode) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (fldAreaState.area < 200) {
        if (D_004360E8 == 0 && fldAreaState.area < 40) {
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
        D_00436128 = index;
        D_00436158 = index;
        D_00436138 = 0;
        if (fldAreaState.sceneCommand == 0) {
            light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
            area = light->type;
            D_00389988[13] = area;
            D_00389988[14] = light->unk4;
            value = light->fadeValue;
            D_00389988[15] = value;
            D_00389988[16] = light->swayMode;
            fldSetFadeTarget(area, value, duration);
            fldSetSwayMode(D_00389988[16]);
            vec[0] = light->fixedVectorX * 0.00390625f;
            vec[1] = light->fixedVectorY * 0.00390625f;
            vec[2] = light->fixedVectorZ * 0.00390625f;
            vec[3] = 0;
            kwlnSetDrawColorTarget(duration, vec);
            evtSetDrawVectorTarget(duration, light->unk1C, light->unk24, light->unk20, light->unk28);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 0, dir);
            vec[0] = light->lightVectorAX;
            vec[1] = light->lightVectorAY;
            vec[2] = light->lightVectorAZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 1, dir);
            vec[0] = light->lightVectorBX;
            vec[1] = light->lightVectorBY;
            vec[2] = light->lightVectorBZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 2, dir);
            vec[0] = light->lightVectorCX;
            vec[1] = light->lightVectorCY;
            vec[2] = light->lightVectorCZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 2, vec);
            vec[0] = light->finalVectorX;
            vec[1] = light->finalVectorY;
            vec[2] = light->finalVectorZ;
            vec[3] = 1.0f;
            kwlnSetBackgroundColorTarget(duration, vec);
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
        func_00135A68(D_00436158, speed);
    }
}

/*BEGIN func_00135D80*/
void func_00135D80(s32 index, s32 mode) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;
    u32 previousIndex;

    if (fldAreaState.area < 200) {
        if (D_004360E8 == 0 && fldAreaState.area < 40) {
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
        if (D_00436128 != index) {
            previousIndex = D_00436158;
            if (D_00436138 == 0) {
                previousIndex = D_00436128;
            }
            D_0043613C = fldSwayMode;
            D_00436140 = fldSwayPhase;
            D_00436144 = fldSwayOffset;
            D_00436148 = D_00436118;
            D_0043614C = D_0043611C;
            D_00436150 = D_00436120;
            D_00436154 = D_00436124;
            D_00436158 = previousIndex;
            D_0043615C = D_0043612C;
            D_00436160 = D_00436130;
            D_00436164 = D_00436134;
            D_00436138 = 1;
            D_00436128 = index;
            if (fldAreaState.sceneCommand == 0) {
                light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
                D_00389988[11] = index;
                area = light->type;
                D_00389988[13] = area;
                D_00389988[14] = light->unk4;
                value = light->fadeValue;
                D_00389988[15] = value;
                D_00389988[16] = light->swayMode;
                fldSetFadeTarget(area, value, duration);
                fldSetSwayMode(D_00389988[16]);
                vec[0] = light->fixedVectorX * 0.00390625f;
                vec[1] = light->fixedVectorY * 0.00390625f;
                vec[2] = light->fixedVectorZ * 0.00390625f;
                vec[3] = 0;
                kwlnSetDrawColorTarget(duration, vec);
                evtSetDrawVectorTarget(duration, light->unk1C, light->unk24, light->unk20, light->unk28);
                dir[0] = light->lightDirectionAX;
                dir[1] = light->lightDirectionAY;
                dir[2] = light->lightDirectionAZ;
                dir[3] = 0;
                kwlnSetLightDirectionTarget(duration, 0, dir);
                vec[0] = light->lightVectorAX;
                vec[1] = light->lightVectorAY;
                vec[2] = light->lightVectorAZ;
                vec[3] = 0;
                kwlnSetLightColorTarget(duration, 0, vec);
                dir[0] = light->lightDirectionBX;
                dir[1] = light->lightDirectionBY;
                dir[2] = light->lightDirectionBZ;
                dir[3] = 0;
                kwlnSetLightDirectionTarget(duration, 1, dir);
                vec[0] = light->lightVectorBX;
                vec[1] = light->lightVectorBY;
                vec[2] = light->lightVectorBZ;
                vec[3] = 0;
                kwlnSetLightColorTarget(duration, 1, vec);
                dir[0] = light->lightDirectionCX;
                dir[1] = light->lightDirectionCY;
                dir[2] = light->lightDirectionCZ;
                dir[3] = 0;
                kwlnSetLightDirectionTarget(duration, 2, dir);
                vec[0] = light->lightVectorCX;
                vec[1] = light->lightVectorCY;
                vec[2] = light->lightVectorCZ;
                vec[3] = 0;
                kwlnSetLightColorTarget(duration, 2, vec);
                vec[0] = light->finalVectorX;
                vec[1] = light->finalVectorY;
                vec[2] = light->finalVectorZ;
                vec[3] = 1.0f;
                kwlnSetBackgroundColorTarget(duration, vec);
                fldSetPlayerAndPeerLighting(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                              light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                              light->unitLightDirection[0], light->unitLightDirection[1],
                              light->unitLightDirection[2]);
            }
        }
    }
}
/*END func_00135D80*/

void func_00136098(void) {
    dds3ClearObjectFlags(fldPlayerObject, 0x100);
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00413388);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FA0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FA4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FA8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FAC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FB0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FB4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FB8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FBC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FC0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAreaPackedArchive);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FC8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAreaLoadRequest);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FD0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldAreaCachedResource);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00435FDC);

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

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldDisplayRow);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436064);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldBackgroundBuffer);

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

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004360F0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraSettings);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraColorEffect);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldCameraColorEnabled);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldRainTextureReference);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_00436104);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSkyDrawState);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSwayMode);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSwayPhase);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldSwayOffset);

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

