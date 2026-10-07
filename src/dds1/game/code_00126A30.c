#include "common.h"
#include "fld_inf.h"
extern FldInfTable D_00332E30;
#include "sdf_primitive.h"
#include "evt_unit.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "fld.h"
#include "scr.h"
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
extern void kwlnFadeInStart(s32, s32, s32, s32);
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

extern u8 D_00337D00[];
typedef struct FldNpcMotion {
    s32 defaultMotionId;
    u8 primaryName[0x10];
    u8 secondaryName[0x10];
    u8 unk24[0x10];
    u8 unk34[0x10];
    s32 unk44;
} FldNpcMotion; /* 0x48 bytes */
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
extern void *dds3GetWorldSecondaryObject(void);
extern s32 evtSpawnActionObj2(s32, s32);
extern s32 fldGetSceneReadyFlag(void);
extern s32 D_0032E400[];
extern void fldClearCameraObjectHighlightFlag(void);
extern s32 func_0012FC20(void);
extern void func_0012F578(void);
extern void func_0012EEA0(s16, s16);
extern void func_0012FF48(void);
extern void func_0012EA50(s16, s32, f32);
extern s32 *fldGetPlayerSceneStateAddress();
extern void dds3SetCameraFieldOfView(s32, f32);
extern void fldToggleWorldNodeState(s32);
extern void func_0012C880(void);
extern void fldRestoreCameraModelColor(void);
extern void func_0012D528(void);
extern void func_0012D3D8(void);
extern void func_0012DD70(void);
extern void fldUpdateCameraMoveOscillation(void);
extern s32 fldTestSceneControlFlags(s32);
extern f32 fldPointDistance(f32, f32, f32, f32, f32, f32);
extern void fldClearCameraModelColor(void);
extern void func_00131290(void);
extern void fldClearCameraObjectTransitionFlags(void);
typedef struct {
    f32 dist;
    f32 y;
    f32 targetY;
    f32 fov;
    f32 unk10;
    f32 unk14;
} FldCamRow; /* 0x18 bytes */
/* Retained field-area work: projection, player-position history and facing requests.
 * The ASM camera/motion routines share these words; this is not a separate camera object. */
typedef struct FldAreaWork {
    u8 pad0[0x50];
    s32 mode;
    u8 pad54[4];
    s32 rowIdx;
    u8 pad5C[8];
    f32 negatedAngle;
    u8 pad68[4];
    f32 dist;
    u8 pad70[0x14];
    s32 positionPending;
    u8 pad88[0xB8];
    f32 x;
    f32 y;
    f32 z;
    f32 previousX; /* Saved XYZ history, also read by the ASM collision probes. */
    f32 previousY;
    f32 previousZ;
    f32 targetX; /* XYZ installed when positionPending is consumed. */
    f32 targetY;
    f32 targetZ;
    f32 angle;       /* Current player heading in degrees. */
    f32 targetAngle; /* Desired heading; the ASM updater smooths angle toward it. */
    f32 unk16C;
    f32 unk170;
    f32 unk174; /* Degree-valued reference in the mode-1 ASM position update. */
    s32 positionMode; /* 1 selects the ASM position update instead of the normal path. */
    s32 unk17C;
    s32 verticalStepDirection; /* Positive lowers Y by 2; negative raises it by 2. */
    s32 unk184;
    u32 pointState; /* 1 queued, 2 heading installed; ASM clears it after turning. */
    f32 facingPointX;
    f32 facingPointZ;
    u32 angleState; /* Same handshake for the explicit angle request. */
    f32 overrideAngle; /* Queued angle copied to targetAngle by the C consumer. */
} FldAreaWork;
extern FldCamRow fldCameraFollowRows[];
extern f32 D_00330650[];
extern f32 D_00330660[];
extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void btlActivateRuntime(s32 mode);
extern void dds3SetWorldObjectDataValue(u64, s8);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern u32 sdfConsFinalizePacketHeader(u32, s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);
extern s32 sdfConsMeasurePacketWithHeader(s32);
extern void *sdfConsAllocateColumnPacket(s32);
extern s32 sdfConsCreateDrawPacket(SdfListHead *, SdfTex *, s32);

extern s32 D_0032E3C0[];
extern s8 D_0032C9A0[];
extern void fldReleaseSkyResources(void);
extern void fldCreatePlayerObject(void);
extern u8 *fldSelectCurrentActorOnNextFloor(void);
extern s32 evtStartSceneResourceTask(u64, u8 *);
extern void func_00126A30(u32, u32, s32);

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
extern s32 fldRainTextureReference;
extern s32 fldRainTextureResource;

extern s32 fldCameraModelObject;

extern u32 fldPlayerObject;
extern u8 D_003BAB3C;

extern u32 D_003BAD34;
extern u32 D_003BAD38;
extern u32 D_003BAD3C;
extern u32 D_003BAD40;

extern u32 D_003BAD1C;

extern void *dds3GetWorldObject(void);
extern s32 dds3GetWorldCameraObject(s32);
extern s64 fldGetPlayerSceneState(void);


extern u32 D_003BACF8;

extern s32 sdfCreateResetPacketList(void);
extern s32 sdfAllocPacketAligned(s32);

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern s32 fldBackgroundBuffer;
extern s32 sdfAllocateBlockBySizeThreshold(u32);

extern u32 fldDisplayRow;

extern u32 D_003BAC30;

extern u32 fldPendingArea;
extern u32 fldPendingFloor;
extern u32 D_003BAD98;
extern u32 D_003BAD9C;
extern u32 D_003BADA0;
extern u32 D_003BADA4;
extern u32 D_003BADC8;
extern u32 D_003BADD8;
extern u32 D_003BAE28;
extern s32 D_003BAE3C;
extern u32 fldSelectedActorEntryIndex;
extern u32 D_0032E428[];
extern s32 fldAreaState[];
extern u32 D_0032E538[];
extern u32 D_0032E544[];
extern u32 D_0032E570[];
extern s16 D_00333910[];
extern s16 D_00333912[];
extern u8 D_0033391C[];
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
extern SdfTex *sdfTexAcquireResourceTexture(void *);
extern u8 sdfProjectionMatrix[];
extern u8 D_00324660[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *src);
extern char D_003C9200[];
extern u32 D_003C92E0[];
extern s16 D_00337D12[];
extern s16 D_003C9518[];
extern void fldFormatAreaResourceName(char *arg0);
extern void func_00132FD0(u32 arg0, s32 arg1);
extern s32 strcmp(const char *a, const char *b);
extern f32 D_003BAD20;
extern char fldEncounterTaskName[];
extern u8 D_003C9230[];
extern f32 D_003C9220[];
extern s32 fldEncProc(void);
extern void fldResetEncounterAsyncState(void);
extern s32 func_00213808(void);
extern void btlClearRuntimeState(void);
extern void dds3TransformCameraVectorsByInnerRotation(s64 arg0, void *arg1, void *arg2);
extern f32 sdfAtan2(f32 arg0, f32 arg1);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern u32 fldAreaLoadRequest;
extern u8 D_003BACE0[];
extern u8 D_003BACE8[];
extern s32 fldEncounterRuntimeState;
extern u32 D_003BACFC;
extern u32 D_00330738[];
extern u32 D_003306B0[];
extern u32 D_003306C0[];
extern u32 D_003308B0[];
extern u32 fileRequestIsReady(u32 arg0);
extern void *memset(void *s, s32 c, u32 n);
extern void *sdfAllocSizeClassBlock(s32 size);
extern void kwlnTaskSetUserValue(u32 arg0, void *arg1);
extern s32 fldDrawPendingTitleBannerWhenIdle(u32 task);
extern s32 kwlnTaskIsRegistered(u32 arg0);
extern s32 func_00213B50(void);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern s32 fldValueRecordCount;
extern s32 D_003BADF8;
extern s32 D_003BADEC;
extern u8 D_003BA734;
extern s32 scrFindNamedProcessNode(u32 arg0);
extern s32 evtDestroyNamedTask(u64 arg0, u32 arg1);
extern u32 fldAreaCachedResource;
extern u32 fldAreaPackedArchive;
extern u8 D_003BAC90[];
extern void sdfQueueNonzeroResourceId(u32 arg0);
extern void func_00288788(u32 arg0);
extern void mdlSuspendAllContextMotions(s32 arg0);
extern void mdlResumeAllContextMotions(s32 arg0);
extern s32 D_003BAE30;
extern s32 D_003BAE1C;
extern s32 fldTaskSlotCount;
extern s16 D_003C9510[];
extern void *sdfAllocGeneralBlock(s32 size);
extern void *sdfResourceRetainAddress(void *p);
extern u32 D_003BAE4C;
extern s32 D_003BAE50;
extern s32 *dds3FindObjectChainNodeByName();
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
extern void kwlnTaskDestroyWithHierarchyByName();
extern void mnuReleaseResourceEntries();
extern void sdfResourceListRelease();
extern void fldReleaseBackgroundBuffer(void);

typedef struct {
    s16 data[12];
} FldRowData; /* 0x18 bytes */

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 count;
    FldRowData body;
} FldS16Row; /* 0x20 bytes */
extern FldS16Row fldActorWaypointRows[];
extern u32 *D_003307B0[];
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

/* A packed-resource chunk uses an offset from the base and a byte length. */
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

/* Layout shared by the two field-request dispatch paths. */
typedef struct FldSceneRequest {
    u32 primaryValue;
    u32 secondaryValue;
    FldLoadRecord *record;
    u32 spawnCount;
    FldActionSpawn *spawnList;
} FldSceneRequest;

extern u32 D_003C91D0[], D_003C91E0[], D_003C91F0[];
extern char D_003BAC40[];
extern u32 sdfReadNamedResource(const char *, u32 *, s32);
extern void func_001263F0(u32, u32);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00126A30);

void fldSpawnActionObjects(FldActionSpawn *list, u32 count) {
    u32 i;

    dds3GetWorldSecondaryObject();
    for (i = 0; i < count; i++) {
        s32 handle = evtSpawnActionObj2(list->firstValue, list->secondValue);
        list++;
        if (i == 0) {
            fldAreaState[0] = handle;
        }
    }
}

typedef struct FldScriptResource {
    u8 pad00[0x10];
    u8 parameters[0x10];
    struct MotionTable *unk20;
} FldScriptResource;

typedef struct FldResourceName {
    u32 unk00;
    const char *name;
} FldResourceName;

extern FldFileResource *D_003BD7B0;
extern u32 D_003BD7B4;
extern FldFileResource *D_003BD7B8;
extern u32 D_003BD7BC;
extern struct DevRequest *D_003BAC14;
extern EffWorldNode *evtCreateScriptObjectWithResource(s32, void *, struct MotionTable *, void *, const char *);
extern void *dds3SpawnInnerVecObj6(s32, f32 *, void *);
extern void dds3SetWorldEntryCallbackTarget(void *, const char *);
extern void effObjSetModelHolder(void *, u32);
extern s32 fldParseRoomNumberFromName(const char *);
extern void func_00113DD0(void *, u32);
extern void *dds3FindWorldObjectNodeByKey(void *, u32, s32);
extern void dds3SetSlotByKind(void *, void *);
extern void func_00111F40(void *);
extern void fldSetRecordValueById(s32, s32);
extern void *dds3FindIndexedObjectChainNodeByName(void *, s32, const u8 *);
extern void dds3RegisterObjectInHandlerIndex(void *);

void fldCreateResourceScriptObjects(void) {
    f32 position[4];
    f32 rotation[4];
    FldFileResource *resource = D_003BD7B0;
    u32 count = D_003BD7B4;
    void *world;
    void *object;
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
        evtCreateScriptObjectWithResource(resource->id, script->parameters,
                                         script->unk20, D_003BAC14, resource->name);
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
        dds3SetWorldEntryCallbackTarget(object, resource->name);
        if (fldAreaState[4] >= 200 && fldAreaState[4] < 500) {
            if (fldAreaState[4] == 230 && fldAreaState[5] == 6 && i == 2) {
                effObjSetModelHolder(object, 6);
            } else {
                switch (i) {
                    case 0:
                        effObjSetModelHolder(object, 2);
                        break;
                    case 1:
                        effObjSetModelHolder(object, 3);
                        break;
                    case 2:
                        effObjSetModelHolder(object, 4);
                        break;
                    default:
                        effObjSetModelHolder(object, 5);
                        break;
                }
            }
        } else {
            effObjSetModelHolder(object, 7);
        }
        func_00113DD0(object, fldParseRoomNumberFromName(resource->name));
        dds3SetSlotByKind(object, dds3FindWorldObjectNodeByKey(world, resource->id, 10));
        func_00111F40(object);
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127388);

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
    if (fldAreaState[4] < 500) {
        for (i = 0; i < 4; i++) {
            if (D_003C91D0[i] != 0) {
                fldFormatAreaDirectory(directory, fldAreaState[4], fldAreaState[5] + 1);
                func_003014F0(path, D_003BAC40, directory, D_003C91D0[i]);
                D_003C91E0[i] = sdfReadNamedResource(path, &D_003C91F0[i], 0);
            }
        }
    }
    func_001263F0(request->unk_4, request->unk_0);
}

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(FldSceneRequest *request) {
    s32 state;
    fldSpawnActionObjects(request->spawnList, request->spawnCount);
    state = fldAreaState[4];
    if (state != 1 && state < 200) fldCreatePlayerObject();
    func_00126A30(request->secondaryValue, request->primaryValue, 0);
    fldAreaState[1] = request->record->displayState;
}

void fldProcessFieldRequestAlternate(FldSceneRequest *request) {
    func_00126A30(request->secondaryValue, request->primaryValue, 1);
}

s32 func_001277A8(s32 record) {
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
extern s32 func_003014F0(char *, const char *, ...);
extern u32 fileQueuePlainDispatchRequest(char *);
extern void fldFormatAreaDirectory(char *, s32, s32);

s32 fldLoadAreaResource(void) {
    char directory[64];
    char path[80];
    u32 area = fldPendingArea;
    u32 floor = fldPendingFloor;

    if (area != 0 || floor != 0) {
        fldFreeDisplayObjects();
        fldAreaState[31] = area;
        fldAreaState[32] = floor;
        fldFormatAreaDirectory(directory, area, 1);
        func_003014F0(path, D_0039FE38, directory, area, floor);
        fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
        fldAreaState[30] = 1;
        return 1;
    }
    return 0;
}

extern s32 fldGetLocationCoordinateValue(s32, s32);
extern void sdfRaiseDeviceThreadPriority(void);
extern s32 D_003BAC80;
extern void fldFreeDisplayObjects(void);

typedef struct FldAreaResourceState {
    s32 pad00[30];
    s32 resourceFlag;     /* 0x78 */
    s32 area;             /* 0x7C */
    s32 room;             /* 0x80 */
} FldAreaResourceState;

s32 fldRequestAreaResource(s32 area, s32 room) {
    char directory[64];
    char path[80];

    if (fldAreaState[4] != area) {
        return 1;
    }
    if (((FldAreaResourceState *)fldAreaState)->resourceFlag == 1) {
        return 1;
    }
    if (((FldAreaResourceState *)fldAreaState)->area == area &&
        ((FldAreaResourceState *)fldAreaState)->room == room) {
        ((FldAreaResourceState *)fldAreaState)->resourceFlag = 2;
        return 1;
    }
    fldFreeDisplayObjects();
    ((FldAreaResourceState *)fldAreaState)->area = area;
    ((FldAreaResourceState *)fldAreaState)->room = room;
    if (fldGetLocationCoordinateValue(area, room) & 0x10) {
        D_003BAC80 = 1;
    } else {
        D_003BAC80 = 0;
    }
    sdfRaiseDeviceThreadPriority();
    D_003BA734 = 1;
    fldFormatAreaDirectory(directory, area, 1);
    func_003014F0(path, D_0039FE38, directory, area, room);
    fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
    ((FldAreaResourceState *)fldAreaState)->resourceFlag = 1;
    return 1;
}

typedef struct FldAreaResourceNode {
    struct FldAreaResourceNode *next; /* 0x00 */
    u32 pad04;
    u32 resourceHandle; /* 0x08 */
} FldAreaResourceNode;

typedef struct FldAreaResource {
    u8 pad00[0x60];
    FldAreaResourceNode *nodes; /* 0x60 */
} FldAreaResource;

void fldFreeDisplayObjects(void) {
    if (fldAreaLoadRequest != 0) {
        FldAreaResourceNode *node = ((FldAreaResource *)fldAreaLoadRequest)->nodes;

        if (node != NULL) {
            do {
                sdfQueueNonzeroResourceId(node->resourceHandle);
                node = node->next;
            } while (node != NULL);
        }
        func_00288788(fldAreaLoadRequest);
        fldAreaLoadRequest = 0;
    }
    fldPendingArea = 0;
    fldPendingFloor = 0;
    fldAreaState[31] = 0;
    fldAreaState[32] = 0;
    fldAreaState[30] = 0;
}

u32 fldPollAreaResourceLoad(void) {
    u32 sceneState = fldAreaState[0x1E];

    if (sceneState != 0) {
        if (sceneState == 1) {
            if (fileRequestIsReady(fldAreaLoadRequest) != 0) {
                fldAreaState[0x1E] = 0;
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
    if (fldAreaState[31] != area || fldAreaState[32] != room) {
        return 0;
    }
    if (fldAreaLoadRequest != 0 && fileRequestIsReady(fldAreaLoadRequest) != 0) {
        return 1;
    }
    return fldAreaState[30] != 0;
}

extern u32 fldCachedRoomResourceData, D_003BAC64, D_003BAC68, D_003BAC6C;
extern u32 fldCachedRoomResourceSize, D_003BAC74, D_003BAC78, D_003BAC7C;

void *fldLoadCachedRoomResourceIfLocationMatches(void **destination, s32 area, s32 room) {
    if (fldAreaState[31] == area) {
        if (fldAreaState[32] == room) {
            void *buffer = sdfAllocGeneralBlock(fldCachedRoomResourceSize);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)fldCachedRoomResourceData, fldCachedRoomResourceSize);
            return buffer;
        }
    }
    return NULL;
}

void *func_00127CB8(void **destination, s32 area, s32 room) {
    if (fldAreaState[31] == area) {
        if (fldAreaState[32] == room) {
            void *buffer = sdfAllocGeneralBlock(D_003BAC74);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_003BAC64, D_003BAC74);
            return buffer;
        }
    }
    return NULL;
}

void *func_00127D30(void **destination, s32 area, s32 room) {
    if (fldAreaState[31] == area) {
        if (fldAreaState[32] == room) {
            void *buffer = sdfAllocGeneralBlock(D_003BAC78);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_003BAC68, D_003BAC78);
            return buffer;
        }
    }
    return NULL;
}

void *func_00127DA8(void **destination, s32 area, s32 room) {
    if (fldAreaState[31] == area) {
        if (fldAreaState[32] == room) {
            void *buffer = sdfAllocGeneralBlock(D_003BAC7C);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_003BAC6C, D_003BAC7C);
            return buffer;
        }
    }
    return NULL;
}

extern s32 mdlFlagTest(s32);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_0039FE38);

void fldFormatAreaResourceName(char *out) {
    char directory[32];
    s32 area = fldAreaState[4];
    s32 room = fldAreaState[5];

    fldFormatAreaDirectory(directory, area, 1);
    if (area == 0x1C) {
        if (mdlFlagTest(0x5E1)) {
            func_003014F0(out, "%sf%03d_000.LB", directory, 0x1C);
        } else if (mdlFlagTest(0x5E2)) {
            func_003014F0(out, "%sf%03d_00a.LB", directory, 0x1C);
        } else if (mdlFlagTest(0x5E3)) {
            func_003014F0(out, "%sf%03d_00b.LB", directory, 0x1C);
        } else if (mdlFlagTest(0x5E4)) {
            func_003014F0(out, "%sf%03d_00c.LB", directory, 0x1C);
        } else {
            func_003014F0(out, "%sf%03d_00d.LB", directory, 0x1C);
        }
        return;
    }
    if (area == 0x1B) {
        switch (room) {
        case 8 ... 21:
            func_003014F0(out, "%sf%03d_00a.LB", directory, area);
            break;
        case 7:
            func_003014F0(out, "%sf%03d_00b.LB", directory, area);
            break;
        case 22:
        case 23:
        case 27:
            if (mdlFlagTest(0x58D)) {
                func_003014F0(out, "%sf%03d_00d.LB", directory, area);
            } else {
                func_003014F0(out, "%sf%03d_00c.LB", directory, area);
            }
            break;
        case 24 ... 26:
        default:
            func_003014F0(out, "%sf%03d_000.LB", directory, area);
            break;
        }
        return;
    }
    func_003014F0(out, "%sf%03d_000.LB", directory, area);
}

u8 fldHasAreaResourceNameChanged(void) {
    char buf[32];

    fldFormatAreaResourceName(buf);
    return strcmp(D_003C9200, buf) != 0;
}

extern void func_00288C50(u32);
extern void fldSetNpcPalette();
extern void fldUploadSkyBuffer();
extern void fldCopyActorWaypointTable();
extern void fldCopyInfoTable(const void *);
extern u32 sdfMemoryGetBlockSize(u32);
extern u32 sdfMemoryGetBlockAddress(u32);
extern void fldSetSceneRecordChunk(u32, u32);
extern void fldCacheMapLabelLengths();

typedef struct FldPackedEntry {
    struct FldPackedEntry *next;
    u8 pad04[4];
    u32 blockHandle;
    u32 payload;
    u8 pad10[2];
    u16 kind;
} FldPackedEntry;

typedef struct FldPackedArchive {
    u8 pad00[0x60];
    FldPackedEntry *entries;
} FldPackedArchive;

void fldLoadAreaPackedResources(void) {
    char name[32];
    FldPackedEntry *entry;

    if (fldAreaState[4] < 200) {
        fldFormatAreaResourceName(name);
        strcpy(D_003C9200, name);
        fldAreaPackedArchive = fileQueuePlainDispatchRequest(name);
        func_00288C50(fldAreaPackedArchive);
        for (entry = ((FldPackedArchive *)fldAreaPackedArchive)->entries; entry != NULL;
             entry = entry->next) {
            switch (entry->kind) {
            case 1:
                fldCopyInfoTable((const void *)entry->payload);
                sdfQueueNonzeroResourceId(entry->blockHandle);
                break;
            case 2:
                fldSetNpcPalette(entry->payload);
                sdfQueueNonzeroResourceId(entry->blockHandle);
                break;
            case 3:
                fldUploadSkyBuffer(entry->payload);
                sdfQueueNonzeroResourceId(entry->blockHandle);
                break;
            case 4:
                fldCopyActorWaypointTable(entry->payload);
                sdfQueueNonzeroResourceId(entry->blockHandle);
                break;
            case 5:
                fldAreaCachedResource = (u32)sdfAllocGeneralBlock(sdfMemoryGetBlockSize(entry->blockHandle));
                memcpy((void *)sdfMemoryGetBlockAddress(fldAreaCachedResource),
                       (void *)sdfMemoryGetBlockAddress(entry->blockHandle),
                       sdfMemoryGetBlockSize(entry->blockHandle));
                sdfQueueNonzeroResourceId(entry->blockHandle);
                break;
            case 6:
                fldSetSceneRecordChunk(entry->payload, entry->blockHandle);
                break;
            }
        }
        fldCacheMapLabelLengths(fldAreaState[4] % 100);
    }
}

void fldReleaseAreaResourceCache(void) {
    u32 resourceHandle = fldAreaCachedResource;

    if (resourceHandle != 0) {
        sdfQueueNonzeroResourceId(resourceHandle);
        fldAreaCachedResource = 0;
    }
    resourceHandle = fldAreaPackedArchive;
    if (resourceHandle != 0) {
        func_00288788(resourceHandle);
        fldAreaPackedArchive = 0;
    }
    D_003C9200[0] = D_003BAC90[0];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001281E0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128780);

extern s32 D_003BAC10;
extern FldTransferChunk *D_003BAC18;
extern u32 D_003BAC1C;
extern FldTransferChunk *D_003BAC20;
extern u32 D_003BAC24;
extern FldTransferChunk *D_003BAC28;
extern u32 D_003BAC2C;
extern FldAreaResource *D_003BAC38;
extern s32 D_003BAC4C;
extern u32 D_003BAC50;
extern u32 D_003BAC54;
extern u32 D_003BAC58;
extern u32 D_003BAC5C;

void fldReleaseFieldResources(void) {
    s32 i;
    FldAreaResourceNode *node;

    fldReleaseBackgroundBuffer();
    fldPlayPendingSounds();
    fldDestroyTitleTask();
    if (fldTitleMiniIsActive() != 0) {
        kwlnTaskDestroyWithHierarchyByName("fldTitleMini", 1);
    }
    fldReleaseObjectSlots();
    fldResetObjectSlots();
    if (fldAreaState[4] < 0xC8) {
        if (fldAreaState[8] != 0) {
            if ((u32)(fldAreaState[4] - 0x1B) < 2U) {
                fldReleaseTitleSlots(fldAreaState);
                fldReleaseMenuSlotsAfterWait();
                fldReleaseSceneRecordChunk();
                fldReleaseAreaResourceCache();
            }
        } else {
            fldReleaseTitleSlots(fldAreaState);
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
            sdfQueueNonzeroResourceId(D_003C91E0[i]);
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
    if (D_003BAC50 != 0) {
        sdfQueueNonzeroResourceId(D_003BAC50);
        D_003BAC50 = 0;
    }
    if (D_003BAC54 != 0) {
        sdfQueueNonzeroResourceId(D_003BAC54);
        D_003BAC54 = 0;
    }
    if (D_003BAC58 != 0) {
        sdfQueueNonzeroResourceId(D_003BAC58);
        D_003BAC58 = 0;
    }
    if (D_003BAC5C != 0) {
        sdfQueueNonzeroResourceId(D_003BAC5C);
        D_003BAC5C = 0;
    }
    if (fldAreaState[4] < 0xC8 && fldAreaState[7] != fldAreaState[4]) {
        fldAreaState[7] = fldAreaState[4];
    }
    if (D_003BAC38 != 0) {
        node = D_003BAC38->nodes;
        i = 0;
        if (node != 0) {
            do {
                if (i > 0) {
                    sdfQueueNonzeroResourceId(node->resourceHandle);
                }
                node = node->next;
                i++;
            } while (node != 0);
        }
        func_00288788((u32)D_003BAC38);
        D_003BAC38 = 0;
    }
}

extern s32 func_003014F0(char *, const char *, ...);

extern s32 func_003014F0(char *, const char *, ...);
/* All 25 code words match. Check_unit rodata range extends beyond selector strings into the separately-owned next string. */
void fldFormatAreaDirectory(char *path, s32 field, s32 unused) {
    if (field < 200) func_003014F0(path, "/fld/f/f%03d/", field);
    else if (field < 500) func_003014F0(path, "/fld/b/f%03d/", field);
    else func_003014F0(path, "/fld/e/f%03d/", field);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128BB8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_0039FF88);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128DA0);

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
        object->unk1C = 1.0f;
        D_003BD7C0 = (u32)sdfCreateAssetWithDrawEntries();
        fldMarkerTexture = sdfTexAcquireResourceTexture(D_0032F260);
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
    descriptor->append((SdfListHead *)descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129178);

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
    descriptor->append((SdfListHead *)descriptor, command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

void fldSubmitFrameQuad(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    SdfPoolNode *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(0x30);
    data = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    data[4] = (bit17 << 17) | 0x10000 | (bit15 << 15) | (bit14 << 14) | (bit12 << 12) | (bit4 << 4) | (bit1 << 1) | bit0;
    data[5] = 0x47;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

/* Write the selected blend equation to GS ALPHA_1. */
void func_00129900(s32 mode) {
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
    default:
        data[4] = 0x44;
        break;
    }
    data[5] = 0x42;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

INCLUDE_ASM(const s32, "game/code_00126A30", fldSubmitGsGradientTriangle);

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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

void func_0012A5D8(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

void func_0012A890(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag) {
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

extern u32 sdfTexGetPrimaryBuffer(SdfTex *);
extern s32 sdfTexGetPrimaryBufferSize(SdfTex *);
extern void sdfConsInitDmaPacketHeader(DmaPacketHeader *, u32, s32);
extern void sdfAppendReferencePacket(SdfListHead *, u32);
extern void func_002DD708(f32);
extern void sdfInitGeometryDmaPacket(u8 *, const f32 *);
extern void func_002E2680(u64, u8 *, s32, u8 *, u8 *);

/* The model draw input supplies a rotation and a second packet parameter. */
typedef struct FldModelPacketInput {
    u8 pad00[0x40];
    s32 geometryValue; /* 0x40: forwarded to func_002E2680 */
    f32 angle;         /* 0x44: applied to the VU0 matrix */
} FldModelPacketInput;
void fldSubmitModelPacket(SdfTex *texture, u8 *modelData) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 header;
    s32 packet;
    f32 mat[16];
    SdfPoolNode *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    header = sdfAllocPacketAligned(0x20);
    sdfConsInitDmaPacketHeader((DmaPacketHeader *)header, sdfTexGetPrimaryBuffer(texture), sdfTexGetPrimaryBufferSize(texture));
    sdfAppendReferencePacket((SdfListHead *)command, header);
    func_002DD708(((FldModelPacketInput *)modelData)->angle);
        VU0_STORE_MATRIX(mat);
;
    packet = sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket((u8 *)packet, mat);
    sdfAppendPacket((SdfListHead *)command, packet);
    packet = sdfAllocPacketAligned(0x80);
    func_002E2680(packet, modelData, ((FldModelPacketInput *)modelData)->geometryValue, modelData + 0x10, modelData + 0x20);
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

extern s32 kwlnGetDrawBufferIndex(void);
extern u8 kwlnFrameDrawPacketRecords[];
extern void func_002D4C80(s32, u32, s32);
extern void func_002D4CC8(s32, u32, s32);
extern void sdfAppendDmaTagToList(SdfListHead *, u32);

void fldSubmitPrimaryFramePacket(void) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 texture;
    SdfPoolNode *descriptor;
    sdfInitPacketList((SdfListHead *)command);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4C80((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList((SdfListHead *)command, texture);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

void fldSubmitAlternateFramePacket(void) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 texture;
    SdfPoolNode *descriptor;
    sdfInitPacketList((SdfListHead *)command);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4CC8((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList((SdfListHead *)command, texture);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

extern f32 D_0032F4E0[][4];
extern u32 D_0032F500[];
extern void *func_002EF2B0(const f32 (*)[4], const u32 *, s32, u32);

void fldSubmitVectorColorPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    s32 resource;
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
    resource = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)resource);
    record = (s32)func_002EF2B0(D_0032F4E0, D_0032F500, 2, 0x80);
    sdfAppendPacket((SdfListHead *)resource, record);
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)resource);
}

extern void sdfConsAppendClearPacket(s32, s32 (*)(s32));
extern void sdfConsAppendAssetPacket(s32, void *, s32 (*)(s32));
extern void *func_002E21A0(SdfPrimitiveRequest *);
void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    SdfPrimitiveRequest desc;
    f32 verts[12];
    s32 indices[3];
    s32 command;
    SdfPoolNode *descriptor;

    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    sdfConsAppendClearPacket(command, 0);
    sdfConsAppendAssetPacket(command, (void *)D_003BACEC, 0);
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
    sdfAppendPacket((SdfListHead *)command, (u32)func_002E21A0(&desc));
    descriptor = &kwlnDrawSurfaces[fldDisplayRow];
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012AEB0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B090);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B2B0);

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
    func_00129900(0);
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
    func_00129900(0);
    fldSubmitModelPacket(fldMarkerTexture, (u8 *)&packet);
}

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

void fldDrawStretchableFrame(s32 x, s32 y, s32 width, s32 height) {
    fldSubmitSpriteRect(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitSpriteRect(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitSpriteRect(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, fldMarkerTexture);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_00129900(0);
}

void fldAllocateBackgroundBuffer(void) {
    if (fldBackgroundBuffer == 0) {
        fldBackgroundBuffer = sdfAllocateBlockBySizeThreshold(0x70000);
    }
}

void fldReleaseBackgroundBuffer(void) {
    if (fldBackgroundBuffer != 0) {
        sdfReleaseChipOrRetainedResource(fldBackgroundBuffer);
        fldBackgroundBuffer = 0;
    }
}

extern u32 D_003980F0[];
extern u32 sdfAllocatePacketList(s32);
extern void sdfCreateDescriptorPacket(u32, u32, s32, s32, s32, s32, u32, s32);
extern void sdfCreateResourcePacket(u32, u32, s32, s32, s32, s32, u32, s32, s32, s32);

void fldSubmitBackgroundResourcePacket(void) {
    if (fldBackgroundBuffer != 0) {
        u32 packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateResourcePacket(packet, D_003980F0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0, 0, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append((SdfListHead *)descriptor, (SdfListHead *)packet);
    }
}

void fldSubmitBackgroundDescriptorPacket(void) {
    if (fldBackgroundBuffer != 0) {
        u32 packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateDescriptorPacket(packet, D_003980F0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append((SdfListHead *)descriptor, (SdfListHead *)packet);
    }
}

void func_0012B890(x, y, first, second)
s32 x;
s32 y;
u32 first;
const u8 *second;
{
    u32 object;

    object = itfCreateConvertedTextGlyph(x << 4, y << 4, 0, first, second, 0);
    frFontDrawGlyphInDefaultMode(object);
    frFontQueueGlyphInSelectedSlot(object);
}
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
    s32 rowX;
    s32 rowY;
    s32 drawDepth;
    s32 packetList;
} FldQuadState; /* 0x2C bytes */

void fldAdvanceQuadRow(s32 quadState) {
    ((FldQuadState *)quadState)->rowY = ((FldQuadState *)quadState)->rowY + 0x60;
}

void fldStartQuadPacketList(s32 quadState) {
    s32 packet;
    u32 packetList;

    packetList = sdfCreateResetPacketList();
    ((FldQuadState *)quadState)->packetList = packetList;
    packet = sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket(packet);
    sdfAppendPacket((SdfListHead *)((FldQuadState *)quadState)->packetList, packet);
}

extern SdfPoolNode D_00325708;
extern SdfPoolNode kwlnPositionedTextSurface;
extern void sdfPktInit(SifCommand *, s32, s32, s32, s32);
extern void *sdfFormatSifPacket(void *, const char *, ...);
extern void sdfInvertScaledVuTransform(void);

void func_0012B940(f32 x, f32 y, f32 z, s32 drawValue) {
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, (s32)screenX * 16, (s32)screenY * 16, quad.drawDepth, 1);
    sdfAppendPacket((SdfListHead *)quad.packetList,
                    (u32)sdfFormatSifPacket(&packet, D_003BACE0, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)quad.packetList);
}

void fldDrawFloorQuad(s32 x, s32 y, s32 drawValue) {
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow((s32)&quad);
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, D_003BACE0, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, D_003BACE8, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(&packet, quad.rowX + (s32)(x * 16.0f), quad.rowY + (s32)(y * 8.0f), quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)quad.packetList);
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
        func_00129900(5);
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
        func_00129900(0);
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C428);

s32 fldGetEncounterRuntimeResult(void) {
    s32 state = fldEncounterRuntimeState;
    s32 result;

    if (state < 3) {
        if (state < 0) {
            result = D_003BACFC;
        } else {
            result = func_00213B50();
        }
    } else {
        result = D_003BACFC;
    }
    return result;
}

void fldSetEncounterPendingValue(u32 value) {
    D_003BACF8 = value;
}

s32 fldEncProc(void) {
    s32 state = fldEncounterRuntimeState;

    if (state < 3) {
        if (state >= 0) {
            func_00213808();
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
                    dds3SetWorldObjectDataValue((s32)dds3GetWorldObject(), 1);
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C880);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012CB48);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012CED0);

void fldUpdateCameraProjectionEndpoints(void) {
    FldAreaWork *cam = (FldAreaWork *)fldAreaState;
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012D3D8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012D528);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012DB70);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012DD70);

/* Suppress the world object's current entry when it is already selected. */
s64 fldGetUnselectedWorldEntry(void) {
    s32 worldObject;
    s64 currentEntry;
    s64 selectedEntry;

    worldObject = (s32)dds3GetWorldObject();
    currentEntry = dds3GetWorldCameraObject(worldObject);
    selectedEntry = fldGetPlayerSceneState();
    if (selectedEntry == currentEntry) {
        currentEntry = 0;
    }
    return currentEntry;
}

void fldSetCameraMoveMode(u32 value) {
    D_003BAD1C = value;
    dds3TransformCameraVectorsByInnerRotation(dds3GetWorldCameraObject((s32)dds3GetWorldObject()), D_003C9230, D_003C9220);
    D_003BAD20 = 0;
}


extern void effObjSetNodeFlags(void *, s32);

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
        camera = (EffWorldNode *)dds3GetWorldCameraObject((s32)dds3GetWorldObject());
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
        effObjSetNodeFlags(camera->inner, 1);
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
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        if (fldTestSceneControlFlags(0x40) == 0) {
            return;
        }
    }
    slot = dds3GetObjectOwnedHandle(fldPlayerObject)->resourceSlots[4];
    modelRef = *(u8 ***)(fldCameraModelObject + 0x18);
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
            func_00131290();
            if (((FldAreaWork *)fldAreaState)->mode == 1 || ((FldAreaWork *)fldAreaState)->mode == 3) {
                fldClearCameraObjectHighlightFlag();
            } else if (((FldAreaWork *)fldAreaState)->dist < 100.0f) {
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
        func_0012C880();
        cam = (FldAreaWork *)fldAreaState;
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
            func_0012D3D8();
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
        if (fldCameraModelObject != 0) {
            mdlResumeAllContextMotions(fldCameraModelObject);
        }
    } else {
        D_003BAD40 = enabled;
        mdlSuspendAllContextMotions(fldCameraModelObject);
    }
}

void func_0012EA40(u32 index, u32 value) {
    D_003BAD34 = index;
    D_003BAD38 = value;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012EA50);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012EEA0);

extern s32 D_003BAB50;
extern void func_00136DA0(f32 *);

/* Probe the current player position; consume a pending target into work/object state. */

void fldUpdateCameraTarget(void) {
    union {
        u128 q;
        f32 f[4];
    } vec;
    f32 cur[3];
    FldAreaWork *st;
    u128 *dst;

    if (fldPlayerObject != 0 && (st = (FldAreaWork *)fldAreaState, st->positionMode != 1) && D_003BAB50 != 0) {
        cur[0] = st->x;
        cur[1] = st->y;
        cur[2] = st->z;
        func_00136DA0(cur);
        if (st->positionPending != 0) {
            st->x = st->targetX;
            st->y = st->targetY;
            st->z = st->targetZ;
            vec.f[0] = st->targetX;
            vec.f[1] = st->targetY;
            vec.f[2] = st->targetZ;
            effObjSetInnerFirstVec(fldPlayerObject, vec.f);
            st->positionPending = 0;
            effObjFetchInnerFirstVec(fldPlayerObject);
            VU0_STORE_VF(vf10, &vec);
;
            dst = (u128 *)(*(u32 *)(fldPlayerObject + 0x1C) + 0x70);
            PCP_COPY_VECTOR(dst, &vec);
        }
    }
}

/* ASM mode-1 path: save previous XYZ, consume positionPending, and wrap degree angles. */
INCLUDE_ASM(const s32, "game/code_00126A30", func_0012F578);

/* ASM signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */
INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FC20);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FD00);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FE30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FF48);

/* Field camera model: node id at +0x12, render-data pointer at +0x18. */
typedef struct FldCameraModel {
    u8 pad00[0x12];
    s16 nodeId;
    u8 pad14[4];
    u8 *renderData;
    u8 *child; /* 0x1C: node used by the model scale setter */
} FldCameraModel;

typedef struct FldCameraRenderData {
    u8 pad00[0x1C];
    u32 color;
} FldCameraRenderData;
typedef struct FldCameraChild {
    u8 pad00[0x20];
    f32 scale; /* 0x20 */
} FldCameraChild;


s32 fldUpdateCameraFrame(void) {
    s16 node;

    if (D_0032E570[0] != 0) {
        return 0;
    }
    if (fldGetSceneReadyFlag() != 0) {
        return 0;
    }
    if (fldTestSceneControlFlags(0x40) == 0) {
        if (((FldAreaWork *)fldAreaState)->mode == 1 || ((FldAreaWork *)fldAreaState)->mode == 3) {
            fldClearCameraObjectHighlightFlag();
        }
        func_0012FC20();
        fldUpdateCameraTarget();
        func_0012F578();
        node = ((FldCameraModel *)fldCameraModelObject)->nodeId;
        func_0012EEA0(node, node);
        if (fldAreaState[70] == 1) {
            func_0012EA50(0, 0, 6.0f);
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
    if (((FldAreaWork *)fldAreaState)->unk17C == 0) {
        if (((FldAreaWork *)fldAreaState)->unk184 == 0) {
            if (((FldAreaWork *)fldAreaState)->verticalStepDirection == 0) {
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
    ((FldCameraRenderData *)((FldCameraModel *)fldCameraModelObject)->renderData)->color = 0;
}

void fldRestoreCameraModelColor(void) {
    ((FldCameraRenderData *)((FldCameraModel *)fldCameraModelObject)->renderData)->color = 0x80808080;
}

void func_00131290(void) {
    dds3SetObjectFlags(fldPlayerObject, 0x200);
}

void fldClearCameraObjectTransitionFlags(void) {
    dds3ClearObjectFlags(fldPlayerObject, 0x400);
    dds3ClearObjectFlags(fldPlayerObject, 0x200);
}

extern void func_001372D0(f32 *);
extern void effObjClearNodeFlags(void *, s32);
INCLUDE_ASM(const s32, "game/code_00126A30", func_001312D8);

extern s32 fldGetLocationCoordinateValue(s32, s32);
extern void func_0012EA50(s16, s32, f32);

void fldSetCameraNodeModeWithTen(void) {
    s16 node = ((FldCameraModel *)fldCameraModelObject)->nodeId;
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        func_0012EA50(node, 0x12, 10.0f);
        return;
    }
    func_0012EA50(node, 3, 10.0f);
}

void fldSetCameraNodeModeWithZero(void) {
    s16 node = ((FldCameraModel *)fldCameraModelObject)->nodeId;
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        func_0012EA50(node, 0x12, 0.0f);
        return;
    }
    func_0012EA50(node, 3, 0.0f);
}

extern s32 fldCameraModelObject;
extern s32 mdlAddEntryPlainEx(s32, s32, s32, f32, f32);
s32 fldAddCameraModelEntry(s32 value) {
    s32 object = fldCameraModelObject;
    ((FldCameraChild *)((FldCameraModel *)object)->child)->scale = 1.0f;
    return mdlAddEntryPlainEx(object, 0, value, 2.0f, 5.0f);
}

extern void mdlSetNodeFloat20(s32, s32, f32);
extern void mdlAddEntryFlagged(s32, s32, s32);

void fldAddCameraModelPair(s32 first, s32 second) {
    mdlSetNodeFloat20(fldCameraModelObject, 0, 1.0f);
    mdlSetNodeFloat20(fldCameraModelObject, 1, 1.0f);
    mdlAddEntryFlagged(fldCameraModelObject, 0, first);
    mdlAddEntryFlagged(fldCameraModelObject, 1, second);
}

void func_00131580(void) {
    D_0032E538[0] = 0;
}

void func_00131590(void) {
    D_0032E544[0] = 0;
}


/* Camera facing requests share the field-work block. Both request states
 * advance from 1 to 2 when their new angle is installed. */
void fldQueueCameraXYOverride(f32 targetX, f32 targetZ) {
    FldAreaWork *work = (FldAreaWork *)fldAreaState;

    work->facingPointX = targetX;
    work->facingPointZ = targetZ;
    work->pointState = 1;
}

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z) {
    FldAreaWork *work;
    f32 angle;

    angle = sdfAtan2(x, z);
    work = (FldAreaWork *)fldAreaState;
    angle *= 180.0f / 3.14f;
    work->angleState = 1;
    work->overrideAngle = -angle;
}

void fldApplyCameraFacingPoint(void) {
    FldAreaWork *cameraWork = (FldAreaWork *)fldAreaState;

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
    FldAreaWork *cameraWork = (FldAreaWork *)fldAreaState;

    if (cameraWork->angleState != 0) {
        cameraWork->angleState = 2;
        cameraWork->targetAngle = cameraWork->overrideAngle;
    }
}

/* ASM turning: smooth angle toward targetAngle, clearing pointState/angleState on arrival. */
INCLUDE_ASM(const s32, "game/code_00126A30", func_00131688);

extern SdfFlagListParams fldCameraColorParameters[];
extern FldCameraSetting *fldCameraSettings;

extern FldCameraSetting D_003306D0;
extern void *fldSkyLightSetBuffer;
extern void *D_003BAD60;
extern s32 *D_003BAD74;
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0048);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0058);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0068);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A00A8);

void fldLoadBattleSkyAndFilter(void) {
    s32 i;
    u32 command;

    if (fldSkyLightSetBuffer == 0) {
        fldSkyLightSetBuffer = sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_003BAD60 == 0) {
        D_003BAD60 = sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_003BAD74 == 0) {
        D_003BAD74 = sdfResourceRetainAddress(sdfAllocGeneralBlock(0x12400));
    }
    if (fldCameraSettings == 0) {
        fldCameraSettings = sdfResourceRetainAddress(sdfAllocGeneralBlock(
            sizeof(FldCameraSetting) * FIELD_CAMERA_SETTING_COUNT));
        for (i = 0; i < FIELD_CAMERA_SETTING_COUNT; i++) {
            fldCameraSettings[i] = D_003306D0;
        }
        fldCameraColorParameters->color.mode = 0;
        fldCameraColorParameters->color.colorB = fldCameraColorParameters->color.colorA = 0x80808080;
        fldCameraColorParameters->alpha.alpha = 0x40;
        fldCameraColorParameters->alpha.surfaceIndex = 2;
        fldCameraColorParameters->alpha.fadeIn = 0.0f;
        fldCameraColorParameters->alpha.fadeOut = 1.0f;
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
extern u32 sdfReadNamedResource(const char *, u32 *, s32);

void fldLoadSkyResource(s32 area) {
    char path[64];
    char directory[32];
    u32 command;

    fldSkyDrawState = 0x80;
    if (area < 200) {
        fldFormatAreaDirectory(directory, area, 1);
        func_003014F0(path, "%sF%03d.SKY", directory, area);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, fldSkyLightSetBuffer, 0xE000);
        sdfDevWaitThenReleaseCommandState(command);
        if (area >= 2 && area < 100 && fldRainTextureResource == 0) {
            fldRainTextureResource = sdfReadNamedResource(D_003A0100, &fldRainTextureData, 0);
            fldRainTextureReference = sdfTexAcquireResourceTexture((void *)fldRainTextureData);
        }
    }
}

void fldReleaseSkyResources(void) {
    if (fldRainTextureReference != 0) {
        sdfTexReleaseReferenceViaHandler(fldRainTextureReference);
        fldRainTextureReference = 0;
    }
    if (fldRainTextureResource != 0) {
        sdfQueueNonzeroResourceId(fldRainTextureResource);
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
        fldRainTextureResource = sdfReadNamedResource(D_003A0100, &fldRainTextureData, 0);
        fldRainTextureReference = sdfTexAcquireResourceTexture((void *)fldRainTextureData);
    }
}

extern f32 sdfSinPoly(f32);
INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0100);

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

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132010);

extern s32 D_003BAD58;
extern s32 D_003BAD88, D_003BAD8C;
extern f32 D_003BAD90, D_003BAD94;
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
    D_003BAD74[area * 73] = value;
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

void func_00132B80(u32 value) {
    D_0032E5A8[0] = value;
}

u32 func_00132B90(void) {
    return D_0032E5A8[0];
}

void fldBeginSelectedValueTransition(u32 value) {
    u32 previousValue = D_0032E59C[0];

    D_003BAD9C = value;
    D_003BADA0 = 0;
    D_003BADA4 = previousValue;
    func_00132FD0(previousValue, 1);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132BD0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132E38);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132FD0);

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
        func_00132FD0(D_003BADC8, speed);
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001332E8);

void fldSetCameraObjectActiveFlag(s32 enabled) {
    if (enabled == 0) {
        dds3ClearObjectFlags(fldPlayerObject, 0x100);
        return;
    }
    dds3SetObjectFlags(fldPlayerObject, 0x100);
    evtEndObjectValueTransition(fldPlayerObject);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133640);

INCLUDE_ASM(const s32, "game/code_00126A30", fldSetDisplayState);

void fldInitializeDisplayPointerTable(void) {
    u32 *displayPointers = D_00330738;

    memset(displayPointers, 0, 0x14);
    displayPointers[0] = (u32)D_003306B0;
    displayPointers[1] = (u32)D_003306C0;
}

/* Three directional light vectors and paired values, plus fixed-point and
 * final homogeneous vectors. The opaque light setters consume each triplet. */
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

extern s32 evtUnitGetNestedValue(u8 *object);
extern void evtSetUnitStatusFlags(EvtUnit *unit);
extern void func_00221D00(EvtUnit *unit, s32 index, s32 colorA, s32 colorB);
extern void evtSetUnitNormalizedDirection(EvtUnit *unit, s32 index);

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

        vec[0] = light->fixedVectorX * 0.00390625f;
        vec[1] = light->fixedVectorY * 0.00390625f;
        vec[2] = light->fixedVectorZ * 0.00390625f;
        vec[3] = 0.0f;
        kwlnSetDrawColorTarget(0, vec);
        evtSetDrawVectorTarget(0, light->unk1C, light->unk24,
                               light->unk20, light->unk28);

        dir[0] = light->lightDirectionAX;
        dir[1] = light->lightDirectionAY;
        dir[2] = light->lightDirectionAZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 0, dir);
        vec[0] = light->lightVectorAX;
        vec[1] = light->lightVectorAY;
        vec[2] = light->lightVectorAZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 0, vec);
        dir[0] = light->lightDirectionBX;
        dir[1] = light->lightDirectionBY;
        dir[2] = light->lightDirectionBZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 1, dir);
        vec[0] = light->lightVectorBX;
        vec[1] = light->lightVectorBY;
        vec[2] = light->lightVectorBZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 1, vec);
        dir[0] = light->lightDirectionCX;
        dir[1] = light->lightDirectionCY;
        dir[2] = light->lightDirectionCZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 2, dir);
        vec[0] = light->lightVectorCX;
        vec[1] = light->lightVectorCY;
        vec[2] = light->lightVectorCZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 2, vec);
        vec[0] = light->finalVectorX;
        vec[1] = light->finalVectorY;
        vec[2] = light->finalVectorZ;
        vec[3] = 1.0f;
        kwlnSetBackgroundColorTarget(0, vec);

        unit = (EvtUnit *)evtUnitGetNestedValue((u8 *)fldPlayerObject);
        evtSetUnitStatusFlags(unit);
        red = light->unitColorA[0] * 128.0f;
        green = light->unitColorA[1] * 128.0f;
        blue = light->unitColorA[2] * 128.0f;
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = light->unitColorB[0] * 128.0f;
        green = light->unitColorB[1] * 128.0f;
        blue = light->unitColorB[2] * 128.0f;
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        func_00221D00(unit, 0, colorA, colorB);
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

    area = light->type;
    D_0032E570[13] = area;
    D_0032E570[14] = light->unk4;
    value = light->fadeValue;
    D_0032E570[15] = value;
    D_0032E570[16] = light->swayMode;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_0032E570[16]);
    vec[0] = light->fixedVectorX * 0.00390625f;
    vec[1] = light->fixedVectorY * 0.00390625f;
    vec[2] = light->fixedVectorZ * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->unk1C, light->unk24, light->unk20, light->unk28);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 0, dir);
    vec[0] = light->lightVectorAX;
    vec[1] = light->lightVectorAY;
    vec[2] = light->lightVectorAZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 1, dir);
    vec[0] = light->lightVectorBX;
    vec[1] = light->lightVectorBY;
    vec[2] = light->lightVectorBZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 2, dir);
    vec[0] = light->lightVectorCX;
    vec[1] = light->lightVectorCY;
    vec[2] = light->lightVectorCZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 2, vec);
    vec[0] = light->finalVectorX;
    vec[1] = light->finalVectorY;
    vec[2] = light->finalVectorZ;
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
    area = light->type;
    D_0032E570[13] = area;
    D_0032E570[14] = light->unk4;
    value = light->fadeValue;
    D_0032E570[15] = value;
    D_0032E570[16] = light->swayMode;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_0032E570[16]);
    vec[0] = light->fixedVectorX * 0.00390625f;
    vec[1] = light->fixedVectorY * 0.00390625f;
    vec[2] = light->fixedVectorZ * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->unk1C, light->unk24, light->unk20, light->unk28);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 0, dir);
    vec[0] = light->lightVectorAX;
    vec[1] = light->lightVectorAY;
    vec[2] = light->lightVectorAZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 1, dir);
    vec[0] = light->lightVectorBX;
    vec[1] = light->lightVectorBY;
    vec[2] = light->lightVectorBZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 2, dir);
    vec[0] = light->lightVectorCX;
    vec[1] = light->lightVectorCY;
    vec[2] = light->lightVectorCZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 2, vec);
    vec[0] = light->finalVectorX;
    vec[1] = light->finalVectorY;
    vec[2] = light->finalVectorZ;
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
            fldCameraColorParameters->color.colorB = fldCameraColorParameters->color.colorA = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_00134348);

extern char D_003A0100[];
extern u32 fldRainTextureData;
extern u32 sdfReadNamedResource(const char *, u32 *, s32);

void fldInitializeCameraColorResource(void) {
    fldRainTextureResource = sdfReadNamedResource(D_003A0100, &fldRainTextureData, 0);
    fldRainTextureReference = sdfTexAcquireResourceTexture((void *)fldRainTextureData);
    fldCameraColorEffect = effCreateSelectionFlagListFromWork(fldCameraColorParameters);
    if (fldRainTextureResource != 0) {
        sdfQueueNonzeroResourceId(fldRainTextureResource);
        fldRainTextureResource = 0;
    }
    fldUpdateCameraColorEffect(fldCameraSettings);
    fldCameraColorEnabled = 1;
}

void func_00134CD8(void) {
    func_00134348();
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
        fldCameraColorParameters->color.colorB = fldCameraColorParameters->color.colorA = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
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
    u8 *storage = sdfAllocGeneralBlock(FIELD_VALUE_RECORD_STORAGE_SIZE);

    fldValueRecordResource = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    fldValueRecords = (u32)storage;
    memset(storage, 0, FIELD_VALUE_RECORD_STORAGE_SIZE);
    storage = sdfAllocGeneralBlock(FIELD_AUX_RECORD_STORAGE_SIZE);
    fldAuxRecordResource = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    fldAuxRecordBuffer = (u32)storage;
    memset(storage, 0, FIELD_AUX_RECORD_STORAGE_SIZE);
}

/* Release both retained resources and clear the usable buffer addresses. */
void fldReleaseRecordStorage(void) {
    sdfDecrementAllocationReferenceCount(fldValueRecordResource);
    sdfQueueNonzeroResourceId(fldValueRecordResource);
    fldValueRecords = 0;
    sdfDecrementAllocationReferenceCount(fldAuxRecordResource);
    sdfQueueNonzeroResourceId(fldAuxRecordResource);
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
    fldAreaState[40] = -1;
    fldAreaState[41] = -1;
    fldAreaState[43] = -1;
    D_003BADF8 = 0;
    D_003BADEC = 0;
    if (fldValueRecords != 0) {
        fldReleaseRecordStorage();
    }
}


INCLUDE_ASM(const s32, "game/code_00126A30", func_00135360);


s32 fldClassifyPositionInZoneWithMargin(f32 margin, f32 *out, s32 mode, s32 count, f32 *pos, FldValueRecord *zone) {
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136A78);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136DA0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136FE8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001372D0);

void func_00137E90(void) {
}

void func_00137E98(void) {
}

void func_00137EA0(void) {
}

/* Project away the selected axis and reject points outside expanded bounds.
 * Return the last negative plane's margin-adjusted distance, not a minimum;
 * no negative plane leaves margin unchanged. Reject results below 0.001. */
f32 fldGetPositionZoneClearance(f32 margin, s32 axisMode, s32 planeCount, f32 *position, FldValueRecord *zone) {
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138058);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001384E8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138910);

void func_00138C28(void) {
}

typedef struct FldRoomState {
    f32 corner[8][4]; /* 0x00 */
    f32 center[4];    /* 0x80 */
    f32 plane[6][4];  /* 0x90 */
    f32 limit[6];     /* 0xF0 */
    s32 unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    s32 unk120;
    u8 pad124[0xC];
    s16 unk130;
    s16 roomId; /* 0x132: returned by fldFindRoomByTask */
    s16 unk134;
    s16 mode;
    s16 unk138;
    s16 axisMode;
    s32 unk13C;
} FldRoomState; /* 0x140 bytes */

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
        fldRoomRecords[roomIndex].unk108 = 0;
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
        fldRoomRecords[roomIndex].unk13C = 0;
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
typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
} FldTaskInfo;
extern u32 dds3GetPathState(void *);
/* Clear slot handles and destroy named tasks reached through linked display values. */
void fldResetTaskSlots(void) {
    s32 slotIndex;
    void *world;
    u32 task;
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
            taskInfo = *(FldTaskInfo **)(D_003307B0[slotIndex] + 8);
            if (taskInfo->slot >= 0) {
                task = dds3GetPathState(dds3FindWorldObjectNodeByKey(world, *(u32 *)D_003C92E0[taskInfo->slot], 0xD));
                if (scrFindNamedProcessNode(task) != 0) {
                    evtDestroyNamedTask((s32)dds3GetWorldObject(), task);
                }
            }
        }
    }
}

/* Append a display value and return its index; no capacity check is performed. */
u32 fldPushDisplayValue(u32 value) {
    u32 index = D_003BAE28;

    D_003C92E0[index] = value;
    D_003BAE28 = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138ED0);

typedef struct FldProbeKind {
    u8 pad00[0xC];
    u32 *kind; /* 0x0C */
} FldProbeKind;

typedef struct FldProbeTarget {
    u8 pad00[0x40];
    u8 position[0x10]; /* 0x40 */
    u8 quaternion[0x10]; /* 0x50 */
} FldProbeTarget;

typedef struct FldProbeActor {
    u8 pad00[0x1C];
    FldProbeTarget *target; /* 0x1C */
} FldProbeActor;

extern void effMiscQuaternionToMatrixVU(void);
/* vu0 routine: actor-facing probe for the world kind-0x11 position payload. */
s32 fldTestRoomProbeFacingAndRange(FldProbeActor *actor, EffWorldNode *entry) {
    f32 dir[4];
    f32 position[4];
    f32 length;
    f32 dot;
    f32 angle;
    f32 *source;
    s32 i;
    u32 kind;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (fldRoomRecords[i].unk108 == entry->key) {
            kind = *((FldProbeKind *)D_003307B0[i][8])->kind;
            switch (kind) {
            case 0:
                source = entry->data;
                position[0] = source[0];
                position[1] = source[1];
                position[2] = source[2];
                VU0_LOAD_VF(vf10, actor->target->quaternion);
                effMiscQuaternionToMatrixVU();
                VU0_STORE_VF(vf30, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                VU0_NORMALIZE_VF10();
                VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
                VU0_MOVE_VF(vf12, vf10);
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, actor->target->position);
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
                return fldRoomRecords[i].unk13C == kind;
            case 2:
                VU0_LOAD_VF(vf10, actor->target->quaternion);
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
s32 fldTestRoomProbeFacing(FldProbeActor *actor, EffWorldNode *entry) {
    f32 dir[4];
    f32 position[4];
    f32 dot;
    f32 angle;
    s32 i;
    u32 kind;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (fldRoomRecords[i].unk108 == entry->key) {
            kind = *((FldProbeKind *)D_003307B0[i][8])->kind;
            switch (kind) {
            case 0:
                PCP_COPY_VECTOR(position, entry->data);
                VU0_LOAD_VF(vf10, actor->target->quaternion);
                effMiscQuaternionToMatrixVU();
                VU0_STORE_VF(vf30, dir);
                dir[1] = 0.0f;
                VU0_LOAD_VF(vf10, dir);
                VU0_NORMALIZE_VF10();
                VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
                VU0_MOVE_VF(vf12, vf10);
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, actor->target->position);
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
                return fldRoomRecords[i].unk13C == kind;
            case 2:
                VU0_LOAD_VF(vf10, actor->target->quaternion);
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


s32 fldTestActorRoomProbeCondition(s32 index, FldProbeActor *actor, f32 *position) {
    f32 dir[4];
    f32 length;
    f32 dot;
    f32 angle;
    u32 kind;

    kind = *((FldProbeKind *)D_003307B0[index][8])->kind;
    switch (kind) {
    case 0:
        VU0_LOAD_VF(vf10, actor->target->quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_STORE_VF(vf30, dir);
        dir[1] = 0.0f;
        VU0_LOAD_VF(vf10, dir);
        VU0_NORMALIZE_VF10();
        VU0_SCALAR_OP(-1.0f, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_MOVE_VF(vf12, vf10);
        VU0_LOAD_VF(vf10, position);
        VU0_LOAD_VF(vf11, actor->target->position);
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
        return fldRoomRecords[index].unk13C == kind;
    case 2:
        VU0_LOAD_VF(vf10, actor->target->quaternion);
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AFC8);

s32 fldDestroyFlaggedNamedTask(u32 flag, u32 slot) {
    D_003308B0[slot] = 0;
    if (scrFindNamedProcessNode(flag) != 0) {
        evtDestroyNamedTask((s32)dds3GetWorldObject(), flag);
        return 0;
    }
    return -1;
}

u32 fldDestroyTaskSlot(u32 slot) {
    u32 *task = &D_003308B0[slot];

    if (kwlnTaskIsRegistered(*task) != 0) {
        kwlnTaskDestroyWithHierarchy(*task, 0);
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
        evtDestroyNamedTask((s32)dds3GetWorldObject(), (u32)D_0032C9A0);
    }
    D_0032C9A0[0] = 0;
    D_003BAB3C = 0;
    object = fldSelectCurrentActorOnNextFloor();
    if (scrFindNamedProcessNode((u32)object) == 0) {
        evtStartSceneResourceTask((s32)dds3GetWorldObject(), object);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013B1D8);

void fldDrawTaskMarkers(void) {
    s32 count = fldTaskSlotCount;
    s32 i = 0;
    if (count > 0) {
        u32 **entry = D_003307B0;
        do {
            fldDrawMarkerQuad((f32 *)(*entry)[4]);
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
            if (kwlnTaskIsRegistered(*taskSlot) == 0) {
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
            return D_003307B0[slotIndex][0];
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


/* Return the first matching task slot's third record word; zero on miss. */
s32 fldGetTaskRecordValue(u32 task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_003308B0[slotIndex] == task) {
            return D_003307B0[slotIndex][2];
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013BF48);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C138);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C2B8);

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

typedef struct FldAreaState {
    u8 pad0[0x14];
    s32 floor;
    u8 pad18[0xEC];
    s16 unk104;
} FldAreaState;

extern s32 D_003BAE38;
extern const char D_00332DF0[];
extern s32 strcmp(const char *, const char *);

const char *func_0013C600(const char *eventName) {
    s32 i;
    s32 j;
    if (eventName == 0) return 0;
    for (i = 0; i < 8; i++) {
        if (D_00332E30.packs[i].setIndex != 0) {
            for (j = 0; j < 5; j++) {
                if (D_00332E30.packs[i].hits[j].area == ((FldAreaState *)fldAreaState)->floor + 1 &&
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
            D_00332E30.sets[i].action == ((FldAreaState *)fldAreaState)->floor + 1 &&
            strcmp(eventName, D_00332E30.sets[i].eventName) == 0) {
            D_003BAE38 = i;
            return D_00332DF0;
        }
    }
    return 0;
}


const char *func_0013C7F8(const char *eventName) {
    s32 i;
    s32 j;
    if (eventName == 0) return 0;
    for (i = 0; i < 8; i++) {
        if (D_00332E30.packs[i].setIndex != 0) {
            for (j = 0; j < 5; j++) {
                if (D_00332E30.packs[i].hits[j].area == ((FldAreaState *)fldAreaState)->floor + 1 &&
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
            D_00332E30.sets[i].action == ((FldAreaState *)fldAreaState)->floor + 1 &&
            strcmp(eventName, D_00332E30.sets[i].eventName) == 0) {
            D_003BAE38 = i;
            return D_00332DF0;
        }
    }
    return 0;
}


const char *func_0013C9E0(const char *eventName) {
    s32 i;
    s32 j;
    if (eventName == 0) return 0;
    for (i = 0; i < 8; i++) {
        if (D_00332E30.packs[i].setIndex != 0) {
            for (j = 0; j < 5; j++) {
                if (D_00332E30.packs[i].hits[j].area == ((FldAreaState *)fldAreaState)->floor + 1 &&
                    strcmp(eventName, D_00332E30.packs[i].hits[j].eventName) == 0) {
                    D_003BAE38 = D_00332E30.packs[i].setIndex;
                    return D_00332DF0;
                }
            }
        }
    }
    for (i = 0; i < 40; i++) {
        if ((u16)D_00332E30.sets[i].kindArea.packed < 2 &&
            D_00332E30.sets[i].action == ((FldAreaState *)fldAreaState)->floor + 1 &&
            strcmp(eventName, D_00332E30.sets[i].eventName) == 0) {
            D_003BAE38 = i;
            return D_00332DF0;
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0150);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CBA8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CEB0);

INCLUDE_ASM(const s32, "game/code_00126A30", fldGetActorSlotAttribute);

extern void fldFormatAreaDirectory(char *, s32, s32);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);


extern void fldFormatAreaDirectory(char *, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);

extern char D_003A01F8[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */
void fldLoadInfoTable(s32 field) {
    char path[64];
    char directory[32];
    u32 command;
    if (field < 200) {
        fldFormatAreaDirectory(directory, field, 1);
        func_003014F0(path, D_003A01F8, directory, field);
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D650);

extern s32 mdlFlagTest(s32);
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

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A01F8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0200);

u8 *fldPickActorTemplateByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = (FldActorEntry *)(D_00337D00 + i * 108);
        flag = entry->requiredFlag;
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && (((FldAreaState *)fldAreaState)->unk104 == 0 || !(entry->flags31 & 4))
            && entry->floor == ((FldAreaState *)fldAreaState)->floor + 1
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
        entry = (FldActorEntry *)(D_00337D00 + i * 108);
        flag = entry->requiredFlag;
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && (((FldAreaState *)fldAreaState)->unk104 == 0 || !(entry->flags31 & 4))
            && entry->floor == ((FldAreaState *)fldAreaState)->floor + 1
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
    FldActorEntry *entry = (FldActorEntry *)(D_00337D00 + index * 108);
    if (entry->floor == D_0032E3C4[0] + 1 && entry->kind == 10) {
        fldSelectedActorEntryIndex = index;
        D_003BAE68 = 8;
        return D_00336A30;
    }
    return 0;
}

s32 fldGetSelectedActorMotionId(void) {
    return D_00337D12[fldSelectedActorEntryIndex * 54];
}

s32 fldQuerySelectedActorMotionState(s32 query) {
    FldActorEntry *entry = (FldActorEntry *)(D_00337D00 + fldSelectedActorEntryIndex * 108);

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
    s32 record;
    FldActorEntry *entry;

    if (checkTaskRecord != 0) {
        record = fldGetTaskRecordValue((u32)scrGetCurrentContext()->task);
        if (record == 0) {
            return;
        }
        if (fldFindActorEntryByName((const char *)record) == 0) {
            return;
        }
    }
    index = fldSelectedActorEntryIndex;
    entry = (FldActorEntry *)(D_00337D00 + index * 108);
    kind = entry->kind;
    if (kind == 1) {
        if (entry->floor == D_0032E3C4[0] + 1) {
            fldPlayFieldSeVolumePan(entry->sound);
            fldReleaseActorTasksById(fldActorSlots[index].actorId);
            return;
        }
    } else if (kind == 2) {
        if (entry->floor == fldAreaState[5] + 1) {
            s32 motion = entry->motion;

            ((FldAreaWork *)fldAreaState)->unk174 = fldActorSlots[index].firstValues[0];
            ((FldAreaWork *)fldAreaState)->positionMode = 1;
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

extern void dds3SetWorldCameraObject(void *, u32);
void func_0013DDF0(const char *name) {
    FldActorEntry *entry;
    char *entryName;
    u32 *camera;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = (FldActorEntry *)D_00337D00 + i;
        if (entry->requiredFlag != 0 && mdlFlagTest(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 0) {
            continue;
        }
        entryName = entry->name;
        if (entry->floor == ((FldAreaState *)fldAreaState)->floor + 1) {
            if (strcmp(name, entryName) == 0) {
                if (entry->variantMode == 2) {
                    return;
                }
                if (entry->variantMode == 0 && entry->linkKind == 3) {
                    camera = dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(), 4,
                                                                 entry->linkName);
                    dds3SetWorldCameraObject(dds3GetWorldObject(), (u32)camera);
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

void fldApplySceneRoomSelection(s8 *scene) {
    if (scene[0x53] != 0) {
        fldAreaState[0x22] = scene[0x53] - 1;
    }
    fldAreaState[0x16] = scene[0x45];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DF60);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013E5A8);

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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013EC68);

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
        entry = (FldActorEntry *)D_00337D00 + i;
        if (entry->requiredFlag != 0 && !mdlFlagTest(entry->requiredFlag)) {
            continue;
        }
        if (entry->kind != 7 || motion != entry->motion ||
            index != entry->secondaryMotion || entry->variantMode != 0) {
            continue;
        }
        fldAreaState[0x16] = entry->rowIndex;
        if (entry->flags54 != 0) {
            D_003BAB3C = entry->flags54;
            strcpy((char *)D_0032C9A0, entry->taskName);
            D_003BAB40 = i + 1;
        }
        if (entry->flags31 & 1) {
            fldAreaState[0x4B] = 1;
        } else {
            fldAreaState[0x4B] = 0;
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
            if (fldAreaState[0xA] != entry->sequenceCode && entry->sequenceCode != 0) {
                fldAreaState[0xA] = entry->sequenceCode;
            }
            if (entry->selectedRoom != 0) {
                fldAreaState[0x22] = entry->selectedRoom - 1;
            }
        }
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F100);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F340);

/* Read a selected actor state or a named world-object value; cases 1 and 2
 * deliberately fall through when no named object is found. */
s32 fldGetActorStat0(s32 attribute) {
    FldActorEntry *actor = (FldActorEntry *)(D_00337D00 + fldSelectedActorEntryIndex * 108);
    s32 *objectNode;
    s32 secondaryMotion;

    switch (attribute) {
    case 0:
        return actor->motion;
    case 1:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->motionName);
        if (objectNode != NULL) {
            return objectNode[1];
        }
    case 2:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->otherName);
        if (objectNode != NULL) {
            return objectNode[1];
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
INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0270);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0280);

s32 fldGetActorStat1(u32 attribute) {
    FldActorEntry *actor = (FldActorEntry *)(D_00337D00 + fldSelectedActorEntryIndex * 108);
    s32 *objectNode;

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
            return objectNode[1];
        }
    case 2:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->otherName);
        if (objectNode != NULL) {
            return objectNode[1];
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
    FldActorEntry *actor = (FldActorEntry *)(D_00337D00 + fldSelectedActorEntryIndex * 108);
    s16 motionIndex = actor->motion;
    s32 *objectNode;
    s32 flags;

    switch (attribute) {
    case 0:
        return D_00336A60[motionIndex].defaultMotionId;
    case 1:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00336A60[motionIndex].primaryName);
        if (objectNode != NULL) {
            return objectNode[1];
        }
    case 2:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00336A60[motionIndex].secondaryName);
        if (objectNode != NULL) {
            return objectNode[1];
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
        return fldActorWaypointRows[slot].count;
    case 1:
        return fldActorWaypointRows[slot].unk4;
    case 2:
        return fldActorWaypointRows[slot].body.data[0];
    case 3:
        return fldActorWaypointRows[slot].body.data[1];
    case 4:
        return fldActorWaypointRows[slot].body.data[2];
    case 5:
        return fldActorWaypointRows[slot].body.data[3];
    case 6:
        return fldActorWaypointRows[slot].body.data[4];
    case 7:
        return fldActorWaypointRows[slot].body.data[5];
    case 8:
        return fldActorWaypointRows[slot].body.data[6];
    case 9:
        return fldActorWaypointRows[slot].body.data[7];
    case 10:
        return fldActorWaypointRows[slot].body.data[8];
    case 11:
        return fldActorWaypointRows[slot].body.data[9];
    case 12:
        return fldActorWaypointRows[slot].body.data[10];
    case 13:
        return fldActorWaypointRows[slot].body.data[11];
    case 14:
        return fldActorWaypointRows[slot].count - D_003BAE50 - 1;
    }
    return 0;
}

s32 fldFindTableEntry(s32 index) {
    s32 slot = D_003BAE4C;
    s32 count = (slot + fldActorWaypointRows)->count;

    if (count - 1 < index) {
        return count - D_003BAE50 - 1;
    }
    index = count - index - 1;
    return fldActorWaypointRows[slot].body.data[index];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FA40);

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
    u32 command;
    if (field >= 100) {
        memset(fldActorWaypointRows, 0, 0x6CA0);
    } else {
        fldFormatAreaDirectory(directory, field, 1);
        func_003014F0(path, "%sF%03d.WAP", directory, field);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, fldActorWaypointRows, 0x6CA0);
        sdfDevWaitThenReleaseCommandState(command);
    }
}

void fldCopyActorWaypointTable(const void *source) {
    memcpy(fldActorWaypointRows, source, 0x6CA0);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FEC0);

void fldReleaseActorTasksById(s32 id) {
    s32 i;
    FldActorRow *task;
    FldActorEntry *actor;

    if (id != 0) {
        if (id != -1) {
            for (i = 0; i < 256; i++) {
                actor = (FldActorEntry *)(D_00337D00 + i * 108);
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

extern s32 func_003003F0(const char *, ...);
extern void evtSetObjectTransitionWork(void *, u32);

/* Advance an actor slot's first/second interpolation (ease-weighted for 40-frame moves) or start its door
 * transition, returning the current values for the key. */
s32 func_00140BE8(u32 key, f32 *x, f32 *y, f32 *z) {
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
                    func_003003F0("DOOR SISETU FADE 6\n");
                    evtSetObjectTransitionWork(node, 9);
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
extern u16 *kwlnTaskGetUserValue(u32);
extern s32 func_00195CD8(void *, s32, s32);
extern void fldDrawGaugeBar(s32);
extern void fldDrawTitleBanner(s32, s32);
extern u8 D_0033E900[];

s32 fldDrawPendingTitleBannerWhenIdle(u32 task) {
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
    ticket = kwlnTaskGetUserValue(task);
    if (ticket[2] != 0) {
        label = D_0033E900 + ticket[1] * 32;
        width = func_00195CD8(label, 1, 0x13);
        x = 0xF6 - (width >> 1);
        fldDrawGaugeBar(width);
        fldDrawTitleBanner(x, 0x131);
        func_0012B890(x + 0x14, 0x98, 0xA09DC380, D_0033E900 + ticket[1] * 32);
        ticket[0] = ticket[0] + 1;
        ticket[2] = 0;
    }
    return 0;
}

void * fldInitializeTitleBannerTask(u32 task) {
    u16 *ticket = sdfAllocSizeClassBlock(8);

    ticket[1] = 1;
    ticket[0] = 0;
    ticket[2] = 0;
    ticket[3] = 0;
    kwlnTaskSetUserValue(task, ticket);
    return (void *)fldDrawPendingTitleBannerWhenIdle;
}

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC30);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldAreaPackedArchive);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC38);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldAreaLoadRequest);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC40);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldAreaCachedResource);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldCachedRoomResourceData);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC64);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC6C);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldCachedRoomResourceSize);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC74);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC78);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC7C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC80);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldPendingArea);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldPendingFloor);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC90);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC98);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACA0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACA8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACB0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACB8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACC0);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldDisplayRow);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACD4);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldBackgroundBuffer);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACE0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACE8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACEC);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldMarkerTexture);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACF8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACFC);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldEncounterRuntimeState);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldEncounterTaskName);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD44);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD58);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldSkyLightSetBuffer);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD60);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldCameraSettings);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldCameraColorEffect);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldCameraColorEnabled);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldRainTextureReference);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD74);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldSkyDrawState);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldSwayMode);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldSwayPhase);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldSwayOffset);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD88);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD8C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD90);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD94);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD98);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD9C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADAC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADBC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADCC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADDC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE4);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldAuxRecordBuffer);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADEC);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldValueRecords);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldValueRecordCount);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADF8);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldAuxRecordResource);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldValueRecordResource);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE04);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE08);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE0C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE10);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldTaskSlotCount);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE44);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE60);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldSelectedActorEntryIndex);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE6C);

INCLUDE_SDATA(const s32, "game/code_00126A30", fldFieldTaskHandle);

