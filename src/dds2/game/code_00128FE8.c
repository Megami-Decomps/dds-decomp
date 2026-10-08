#include "common.h"
#include "kwln.h"
#include "fld_inf.h"
#include "sdf_primitive.h"
#include "dds3obj.h"
#include "fld.h"

#include "fpu.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "evt_unit.h"
#include "dat_state.h"
#include "mdl.h"

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
extern EvtUnit *evtUnitGetNestedValue(EffWorldNode *);
extern void evtSetUnitStatusFlags(EvtUnit *);
extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);
extern void evtSetUnitNormalizedDirection(EvtUnit *, s32);

extern s32 fldCameraModelObject;
extern s32 D_00389888[];

extern void fldFreeDisplayObjects(void);

extern s32 fldSecondarySceneModelHandle;

extern s32 fldSecondarySceneObject;


extern s32 sdfCreateResetPacketList(void);

extern s32 sdfAllocPacketAligned(s32);

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern u32 fldPendingArea;

extern u32 fldPendingFloor;

extern u32 D_00435FC0;

extern u32 fldDisplayRow;

extern s32 fldBackgroundBuffer;

extern s32 sdfAllocateBlockBySizeThreshold(u32);

extern u32 D_00436088;

extern void *dds3GetWorldObject(void);

extern EffWorldNode *dds3GetWorldCameraObject(EffWorldNode *);

extern s64 fldGetPlayerSceneState(void);

extern u32 D_004360AC;

extern s32 D_004360C4;

extern s32 D_004360C8;

extern s32 D_004360CC;

extern s32 D_004360D0;

extern u32 fldPlayerObject;

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern SdfTex *fldRainTextureReference;

extern s32 fldRainTextureResource;

extern u32 fldSwayMode;

extern u32 fldSkyDrawState;

extern u32 fldAreaLoadRequest;

extern u32 fileRequestIsReady(u32 request);

extern u8 D_00435BB4;

extern u32 D_003897E8[];

extern char D_00444950[];

extern s32 strcmp(const char *a, const char *b);

extern u32 fldAreaCachedResource;

extern u32 fldAreaPackedArchive;

extern u8 D_00436020[];

extern void sdfQueueNonzeroResourceId(u32 handle);

extern void func_002C7CE8(u32 handle);

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

extern SdfTex *sdfTexAcquireResourceTexture(void *);
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
extern u32 sdfReadNamedResource(const char *, u32 *, s32);
extern void func_001289A8(u32, u32);

extern f32 D_003897DC[];

extern s32 fldAreaState[];

extern char D_004130D8[]; /* "%sf%03d_%03d.LB" */

extern s32 func_0035C860(char *, const char *, ...);

extern void *fileQueuePlainDispatchRequest(const char *path);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_0035C860(char *, const char *, ...);

extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_0035C860(char *, const char *, ...);

extern void *sdfAllocGeneralBlock(s32 size);

extern void *sdfResourceRetainAddress(void *p);

extern u32 fldCachedRoomResourceData, D_00435FF4, D_00435FF8, D_00435FFC;

extern u32 fldCachedRoomResourceSize, D_00436004, D_00436008, D_0043600C;

extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void sdfAppendDmaPrimary(s32, u32, SdfDmaNode *);
extern u32 sdfConsFinalizePacketHeader(u32, s32);

extern SdfPoolNode kwlnDrawSurfaces[];


extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void *sdfConsAllocateColumnPacket(s32);

extern s32 sdfConsCreateDrawPacket(SdfListHead *, SdfTex *, s32);

extern s32 sdfConsCalculateDrawPacketSize(s32, s32);

extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);

extern s32 sdfConsMeasurePacketWithHeader(s32);

extern u32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_0032DB30(s32, u32, s32);

extern void sdfAppendDmaTagToList(SdfListHead *, u32);

extern void func_0032DB78(s32, u32, s32);

extern f32 D_0038A980[][4];

extern u32 D_0038A9A0[];

extern void *func_00348158(const f32 (*)[4], const u32 *, s32, u32);

extern void *memset(void *s, s32 c, u32 n);


extern void sdfConsAppendClearPacket(s32, s32 (*)(s32));

extern void sdfConsAppendAssetPacket(s32, void *, s32 (*)(s32));

extern void *func_0033B050(SdfPrimitiveRequest *);

extern u32 D_0040B2A0[];

extern s32 sdfAllocatePacketList(s32 (*allocatorArgument)(s32));

extern void sdfCreateResourcePacket(u32, u32, s32, s32, s32, s32, u32, s32, s32, s32);

extern void sdfCreateDescriptorPacket(u32, u32, s32, s32, s32, s32, u32, s32);

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

extern void mdlSetNodeFrameStep(MdlCtx *, s32, f32);

extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);

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

extern void mdlSuspendAllContextMotions(MdlCtx *object);

extern void mdlResumeAllContextMotions(MdlCtx *object);

extern void mdlAddEntryPlainEx(MdlCtx *, s32, s32, f32, f32);
extern void mdlAddEntryPlain(MdlCtx *, s32, s32);
extern void mdlAddEntryFlaggedEx(MdlCtx *, s32, s32, f32, f32);

extern u32 D_0043612C;

extern f32 D_00436130;

extern u32 D_00436134;

extern u32 D_003899B4[];

extern void func_00135A68(u32 value, s32 enabled);
extern KwlnTask *kwlnTaskGetTaskByName(const char *name);
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

extern void sdfAppendReferencePacket(SdfListHead *, u32);

extern void func_003365B8(f32);

extern void sdfInitGeometryDmaPacket(u8 *, const f32 *);

extern void func_0033B530(u64, u8 *, s32, u8 *, u8 *);

extern s32 D_00435F30;

extern void func_00139950(f32 *);

/* Retained field-area work: projection, player-position history and facing requests.
 * DDS2's position/heading tail starts twelve bytes later; its internal offsets agree. */
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
    u8 pad88[0xC4];
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
    f32 unk178;
    f32 unk17C;
    f32 unk180; /* Degree-valued reference in the mode-1 ASM position update. */
    s32 positionMode; /* 1 selects the ASM position update instead of the normal path. */
    s32 unk188;
    s32 verticalStepDirection; /* Positive lowers Y by 2; negative raises it by 2. */
    s32 unk190;
    u32 pointState; /* 1 queued, 2 heading installed; ASM clears it after turning. */
    f32 facingPointX;
    f32 facingPointZ;
    u32 angleState; /* Same handshake for the explicit angle request. */
    f32 overrideAngle; /* Queued angle copied to targetAngle by the C consumer. */
} FldAreaWork;


typedef struct FldAreaResourceState {
    s32 pad00[30];
    s32 resourceFlag;     /* 0x78 */
    s32 area;             /* 0x7C */
    s32 room;             /* 0x80 */
} FldAreaResourceState;
extern void btlActivateRuntime(s32 mode);

extern void dds3SetWorldObjectDataValue(u64, s8);

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00128FE8);

extern s32 evtSpawnActionObj2(u32, u32);

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
    s32 handle;

    dds3GetWorldSecondaryObject();
    for (i = 0; i < count; i++, list++) {
        handle = evtSpawnActionObj2(list->firstValue, list->secondValue);
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

extern FldFileResource *D_00438EB8;
extern u32 D_00438EBC;
extern FldFileResource *D_00438EC0;
extern u32 D_00438EC4;
extern struct DevRequest *D_00435FA4;
extern EffWorldNode *evtCreateScriptObjectWithResource(s32, void *, struct MotionTable *, void *, const char *);
struct EffWorldNode;
extern struct EffWorldNode *dds3SpawnInnerVecObj6(s32, f32 *, void *);
extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);
extern void effObjSetActiveId(struct EffWorldNode *, s32);
extern s32 fldParseRoomNumberFromName(char *);
extern void effObjSetRoomNumber(struct EffWorldNode *, u32);
extern struct EffWorldNode *dds3FindWorldObjectNodeByKey(struct EffWorldNode *, u32, s32);
extern void *dds3SetSlotByKind(ObjBase *, ObjData *);
extern void func_00112168(void *);
extern void fldSetRecordValueById(s32, s32);
extern struct EffWorldNode *dds3FindIndexedObjectChainNodeByName(struct EffWorldNode *, s32, const u8 *);
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
        evtCreateScriptObjectWithResource(resource->id, script->parameters,
                                         script->unk20, D_00435FA4, resource->name);
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
        if (fldAreaState[4] >= 200 && fldAreaState[4] < 500) {
            if (fldAreaState[4] == 230 && fldAreaState[5] == 6 && i == 2) {
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
        dds3SetSlotByKind((ObjBase *)object, (ObjData *)dds3FindWorldObjectNodeByKey(world, resource->id, 10));
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
            dds3SetSlotByKind((ObjBase *)object, (ObjData *)dds3FindIndexedObjectChainNodeByName(world, 2, (const u8 *)linkedName->name));
            dds3RegisterObjectInHandlerIndex(object);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129940);

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
    if (fldAreaState[4] < 500) {
        for (i = 0; i < 4; i++) {
            if (D_00444920[i] != 0) {
                fldFormatAreaDirectory(directory, fldAreaState[4], fldAreaState[5] + 1);
                func_0035C860(path, D_00435FD0, directory, D_00444920[i]);
                D_00444930[i] = sdfReadNamedResource(path, &D_00444940[i], 0);
            }
        }
    }
    func_001289A8(request->unk_4, request->unk_0);
}

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(FldSceneRequest *request) {
    s32 state;
    fldSpawnActionObjects(request->spawnList, request->spawnCount);
    state = fldAreaState[4];
    if (state != 1 && state < 200) fldCreatePlayerObject();
    func_00128FE8(request->secondaryValue, request->primaryValue, 0);
    fldAreaState[1] = request->record->displayState;
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
        ((FldAreaResourceState *)fldAreaState)->area = area;
        ((FldAreaResourceState *)fldAreaState)->room = floor;
        fldFormatAreaDirectory(directory, area, 1);
        func_0035C860(path, D_004130D8, directory, area, floor);
        fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
        ((FldAreaResourceState *)fldAreaState)->resourceFlag = 1;
        return 1;
    }
    return 0;
}

extern void sdfRaiseDeviceThreadPriority(void);

extern s32 D_00436010;

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
        D_00436010 = 1;
    } else {
        D_00436010 = 0;
    }
    sdfRaiseDeviceThreadPriority();
    D_00435BB4 = 1;
    fldFormatAreaDirectory(directory, area, 1);
    func_0035C860(path, D_004130D8, directory, area, room);
    fldAreaLoadRequest = fileQueuePlainDispatchRequest(path);
    ((FldAreaResourceState *)fldAreaState)->resourceFlag = 1;
    return 1;
}

typedef struct FldDisplayNode {
    struct FldDisplayNode *next;
    u8 pad04[4];
    u32 displayObject;
} FldDisplayNode;

typedef struct FldDisplayWork {
    u8 pad00[0x60];
    FldDisplayNode *objects;
} FldDisplayWork;

void fldFreeDisplayObjects(void) {
    if (fldAreaLoadRequest != 0) {
        FldDisplayNode *node = ((FldDisplayWork *)fldAreaLoadRequest)->objects;

        if (node != 0) {
            do {
                sdfQueueNonzeroResourceId(node->displayObject);
                node = node->next;
            } while (node != 0);
        }
        func_002C7CE8(fldAreaLoadRequest);
        fldAreaLoadRequest = 0;
    }
    fldPendingArea = 0;
    fldPendingFloor = 0;
    ((FldAreaResourceState *)fldAreaState)->area = 0;
    ((FldAreaResourceState *)fldAreaState)->room = 0;
    ((FldAreaResourceState *)fldAreaState)->resourceFlag = 0;
}

u32 fldPollAreaResourceLoad(void) {
    u32 resourceFlag = ((FldAreaResourceState *)fldAreaState)->resourceFlag;

    if (resourceFlag != 0) {
        if (resourceFlag == 1) {
            if (fileRequestIsReady(fldAreaLoadRequest) != 0) {
                ((FldAreaResourceState *)fldAreaState)->resourceFlag = 0;
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
    if (((FldAreaResourceState *)fldAreaState)->area != area || ((FldAreaResourceState *)fldAreaState)->room != room) {
        return 0;
    }
    if (fldAreaLoadRequest != 0 && fileRequestIsReady(fldAreaLoadRequest) != 0) {
        return 1;
    }
    return ((FldAreaResourceState *)fldAreaState)->resourceFlag != 0;
}

void *fldLoadCachedRoomResourceIfLocationMatches(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)fldAreaState;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = sdfAllocGeneralBlock(fldCachedRoomResourceSize);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)fldCachedRoomResourceData, fldCachedRoomResourceSize);
            return buffer;
        }
    }
    return NULL;
}

void *func_0012A270(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)fldAreaState;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = sdfAllocGeneralBlock(D_00436004);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_00435FF4, D_00436004);
            return buffer;
        }
    }
    return NULL;
}

void *func_0012A2E8(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)fldAreaState;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = sdfAllocGeneralBlock(D_00436008);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_00435FF8, D_00436008);
            return buffer;
        }
    }
    return NULL;
}

void *func_0012A360(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)fldAreaState;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = sdfAllocGeneralBlock(D_0043600C);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_00435FFC, D_0043600C);
            return buffer;
        }
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004130D8);

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

extern void func_002C81D0(u32);

extern void fldCopyInfoTable(FldInfTable *);

extern void fldSetNpcPalette(u32);

extern void fldUploadSkyBuffer();

extern void fldCopyActorWaypointTable(u32);

extern u32 sdfMemoryGetBlockSize(u32);

extern u32 sdfMemoryGetBlockAddress(u32);

extern void fldSetSceneRecordChunk(u32, u32);

extern void fldCacheMapLabelLengths();

void fldLoadAreaPackedResources(void) {
    char name[32];
    FldPackedEntry *entry;

    if (fldAreaState[4] < 200) {
        fldFormatAreaResourceName(name);
        strcpy(D_00444950, name);
        fldAreaPackedArchive = fileQueuePlainDispatchRequest(name);
        func_002C81D0(fldAreaPackedArchive);
        for (entry = ((FldPackedArchive *)fldAreaPackedArchive)->entries; entry != NULL;
             entry = entry->next) {
            switch (entry->kind) {
            case 1:
                fldCopyInfoTable((FldInfTable *)entry->payload);
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
    u32 cachedResource = fldAreaCachedResource;

    if (cachedResource != 0) {
        sdfQueueNonzeroResourceId(cachedResource);
        fldAreaCachedResource = 0;
    }
    cachedResource = fldAreaPackedArchive;
    if (cachedResource != 0) {
        func_002C7CE8(cachedResource);
        fldAreaPackedArchive = 0;
    }
    D_00444950[0] = D_00436020[0];
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A6F0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012AC90);

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

extern void kwlnTaskDestroyWithHierarchyByName();

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

extern FldDisplayWork *D_00435FC8;

extern s32 D_00435FDC;

extern u32 D_00435FE0;

extern u32 D_00435FE4;

extern u32 D_00435FE8;

extern u32 D_00435FEC;

void fldReleaseFieldResources(void) {
    s32 i;
    FldDisplayNode *node;

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
                fldReleaseResourceSlots(fldAreaState);
                fldReleaseMenuSlotsAfterWait();
                fldReleaseSceneRecordChunk();
                fldReleaseAreaResourceCache();
            }
        } else {
            fldReleaseResourceSlots(fldAreaState);
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
            sdfQueueNonzeroResourceId(D_00444930[i]);
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
    if (D_00435FE0 != 0) {
        sdfQueueNonzeroResourceId(D_00435FE0);
        D_00435FE0 = 0;
    }
    if (D_00435FE4 != 0) {
        sdfQueueNonzeroResourceId(D_00435FE4);
        D_00435FE4 = 0;
    }
    if (D_00435FE8 != 0) {
        sdfQueueNonzeroResourceId(D_00435FE8);
        D_00435FE8 = 0;
    }
    if (D_00435FEC != 0) {
        sdfQueueNonzeroResourceId(D_00435FEC);
        D_00435FEC = 0;
    }
    if (fldAreaState[4] < 0xC8 && fldAreaState[7] != fldAreaState[4]) {
        fldAreaState[7] = fldAreaState[4];
    }
    if (D_00435FC8 != 0) {
        node = D_00435FC8->objects;
        i = 0;
        if (node != 0) {
            do {
                if (i > 0) {
                    sdfQueueNonzeroResourceId(node->displayObject);
                }
                node = node->next;
                i++;
            } while (node != 0);
        }
        func_002C7CE8(D_00435FC8);
        D_00435FC8 = 0;
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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B0D0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413198);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B2B8);

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
        fldMarkerTexture = sdfTexAcquireResourceTexture(D_0038A700);
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

extern void sdfQueueGouraudTexturedQuad(
    s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
    s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
    s32 x2, s32 y2, s32 u2, s32 v2, s32 color2,
    s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocate)(s32));

void func_0012B690(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, SdfTex *texture) {
    SdfListHead *list;
    SdfPoolNode *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = (SdfListHead *)sdfCreateResetPacketList();
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
    sdfQueueGouraudTexturedQuad((s32)list, 0x40,
        left, top, uLeft, vTop, color0,
        right, top, uRight, vTop, color1,
        left, bottom, uLeft, vBottom, color3,
        right, bottom, uRight, vBottom, color2,
        -1, NULL);
    surface = &kwlnDrawSurfaces[fldDisplayRow];
    surface->append((SdfListHead *)surface, list);
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
    descriptor->append((SdfListHead *)descriptor, command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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

INCLUDE_ASM(const s32, "game/code_00128FE8", fldSubmitGsGradientTriangle);

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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)resource);
}

void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    SdfPrimitiveRequest desc;
    f32 verts[12];
    s32 indices[3];
    s32 command;
    SdfPoolNode *descriptor;

    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    sdfConsAppendClearPacket(command, 0);
    sdfConsAppendAssetPacket(command, (void *)D_0043607C, 0);
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
    descriptor->append((SdfListHead *)descriptor, (SdfListHead *)command);
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
    sdfAppendDmaPrimary((s32)packetList,
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
    surface->append((SdfListHead *)surface, packetList);
}

void func_0012D5C0(s32 mode) {
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
    sdfAppendDmaPrimary((s32)packetList,
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
    surface->append((SdfListHead *)surface, packetList);
}

void func_0012D7E0(s32 alpha) {
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
    sdfAppendDmaPrimary((s32)packetList,
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
    surface->append((SdfListHead *)surface, packetList);
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
        fldBackgroundBuffer = sdfAllocateBlockBySizeThreshold(0x70000);
    }
}

void fldReleaseBackgroundBuffer(void) {
    if (fldBackgroundBuffer != 0) {
        sdfReleaseChipOrRetainedResource(fldBackgroundBuffer);
        fldBackgroundBuffer = 0;
    }
}

void fldSubmitBackgroundResourcePacket(void) {
    if (fldBackgroundBuffer != 0) {
        u32 packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateResourcePacket(packet, D_0040B2A0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0, 0, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append((SdfListHead *)descriptor, (SdfListHead *)packet);
    }
}

void fldSubmitBackgroundDescriptorPacket(void) {
    if (fldBackgroundBuffer != 0) {
        u32 packet = sdfAllocatePacketList(0);
        SdfPoolNode *descriptor;
        sdfCreateDescriptorPacket(packet, D_0040B2A0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0);
        descriptor = &kwlnDrawSurfaces[fldDisplayRow];
        descriptor->append((SdfListHead *)descriptor, (SdfListHead *)packet);
    }
}

void func_0012DDC0(s32 x, s32 y, u32 firstPayload, const u8 *secondPayload) {
    u32 object;

    object = itfCreateConvertedTextGlyph(x << 4, y << 4, 0, firstPayload, secondPayload, 0);
    frFontDrawGlyphInDefaultMode(object);
    frFontQueueGlyphInSelectedSlot(object);
}

void fldAdvanceQuadRow(FldQuadState *quad) {
    quad->rowY = quad->rowY + 0x60;
}

void fldStartQuadPacketList(FldQuadState *quad) {
    s32 packet;
    u32 packetList;

    packetList = sdfCreateResetPacketList();
    quad->packetList = packetList;
    packet = sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket(packet);
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
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)quad.packetList);
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
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
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
    fldStartQuadPacketList(&quad);
    sdfPktInit(&packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, (u32)sdfFormatSifPacket(&packet, (const char *)drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)quad.packetList);
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
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)quad.packetList);
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
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)quad.packetList);
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
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)quad.packetList);
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
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)quad.packetList);
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
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)quad.packetList);
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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E958);

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

void fldUpdateLookAtSegmentDistance(void) {
    f32 *distance = D_003897DC;
    f32 dx = fldLookAtNearPoint[0] - fldLookAtFarPoint[0];
    f32 dy = fldLookAtNearPoint[1] - fldLookAtFarPoint[1];
    f32 dz = fldLookAtNearPoint[2] - fldLookAtFarPoint[2];
    *distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012EDB0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F078);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F400);

void fldUpdateCameraProjectionEndpoints(void) {
    FldAreaWork *cam = (FldAreaWork *)fldAreaState;
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
extern void effObjSetInnerFirstVec(EffWorldNode *, u128 *);

void func_0012F908(void) {
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
    effObjSetInnerFirstVec((EffWorldNode *)(u32)*world, &farPoint.q);
    node = (EffWorldNode *)(u32)*world;
    node->ops->update(node);
    D_004360B4++;
    if (D_004360B8 < D_004360B4) {
        D_003897C0[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012FA58);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001300A0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001302A0);

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
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        if (fldTestSceneControlFlags(0x40) == 0) {
            return;
        }
    }
    slot = dds3GetObjectOwnedHandle(fldPlayerObject)->resourceSlots[4];
    modelRef = *(u8 ***)(fldCameraModelObject + 0x18);
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

extern void fldRestoreSceneModelColors(void);
extern void fldUpdateCameraMoveOscillation(void);
extern void func_0012FA58(void);
extern void func_001302A0(void);
extern void func_0012F908(void);

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
        cam = (FldAreaWork *)fldAreaState;
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
            func_0012F908();
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
        if (fldCameraModelObject != 0) {
            mdlResumeAllContextMotions((MdlCtx *)fldCameraModelObject);
        }
    } else {
        D_004360D0 = enabled;
        mdlSuspendAllContextMotions((MdlCtx *)fldCameraModelObject);
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
            mdlResumeAllContextMotions((MdlCtx *)fldCameraModelObject);
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
            mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 1.0f);
            mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 1, motion, blendFrames, blendFrames);
            D_004360C4 = -1;
            if (motion == 2 && D_004360CC == 3) {
                D_004360CC = motion;
                ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.0f;
                mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, 2, blendFrames, blendFrames);
            }
        } else if (D_004360C4 == 0x66) {
            mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 1.0f);
            mdlAddEntryPlainEx((MdlCtx *)fldCameraModelObject, 1, 2, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 0x66;
            ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, 2, blendFrames, blendFrames);
        } else if (D_004360C4 == 2) {
            mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 1.0f);
            mdlAddEntryPlainEx((MdlCtx *)fldCameraModelObject, 1, 2, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 2;
            ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, 2, blendFrames, blendFrames);
        } else if (D_004360C4 == 3) {
            mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 1.0f);
            mdlAddEntryPlainEx((MdlCtx *)fldCameraModelObject, 1, 3, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 3;
            ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, 3, blendFrames, blendFrames);
        } else if (D_004360C4 == 5) {
            mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 1.0f);
            mdlAddEntryPlainEx((MdlCtx *)fldCameraModelObject, 1, 5, 0.0f, blendFrames);
            D_004360C4 = -1;
            D_004360CC = 5;
            ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, 5, blendFrames, blendFrames);
        } else if (D_004360C4 == 6) {
            if (motion == 1) {
                mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 2.0f);
                mdlAddEntryPlain((MdlCtx *)fldCameraModelObject, 1, 6);
                D_004360C4 = -1;
            } else {
                mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 2.0f);
                mdlAddEntryPlain((MdlCtx *)fldCameraModelObject, 1, 4);
                D_004360C4 = -1;
                D_004360CC = 4;
                ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.0f;
                mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, 4, blendFrames, blendFrames);
            }
        } else if (currentMotion != motion && D_004360CC != 5) {
            D_004360CC = motion;
            ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.5f;
            mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, motion, blendFrames, blendFrames);
            if (D_004360C8 == 0) {
                mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 1.0f);
                mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 1, motion, blendFrames, blendFrames);
            }
        }
        if (D_004360C8 > 0) {
            D_004360C8--;
        }
    } else if (modelMotion != motion) {
        ((MdlCtx *)fldCameraModelObject)->first->frameStep = 1.0f;
        mdlAddEntryFlaggedEx((MdlCtx *)fldCameraModelObject, 0, motion, blendFrames, blendFrames);
        if (fldSecondarySceneObject != 0) {
            ((MdlCtx *)fldSecondarySceneModelHandle)->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx((MdlCtx *)fldSecondarySceneModelHandle, 0, motion, blendFrames, blendFrames);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131478);

/* Probe the current player position; consume a pending target into work/object state. */
void fldUpdateCameraTarget(void) {
    union {
        u128 q;
        f32 f[4];
    } vec;
    f32 cur[3];
    FldAreaWork *st;
    u128 *dst;

    if (fldPlayerObject != 0 && (st = (FldAreaWork *)fldAreaState, st->positionMode != 1) && D_00435F30 != 0) {
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
            effObjSetInnerFirstVec(fldPlayerObject, vec.f);
            st->positionPending = 0;
            effObjFetchInnerFirstVec(fldPlayerObject);
            VU0_STORE_VF(vf10, &vec);
            dst = (u128 *)(*(u32 *)(fldPlayerObject + 0x1C) + 0x70);
            PCP_COPY_VECTOR(dst, &vec);
        }
    }
}

/* ASM mode-1 path: save previous XYZ, consume positionPending, and wrap degree angles. */
INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131B50);

/* ASM signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */
INCLUDE_ASM(const s32, "game/code_00128FE8", func_001321F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001322D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00132408);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00132540);

extern u32 fldGetSceneReadyFlag(void);
extern void fldClearCameraObjectHighlightFlag(void);
extern void func_001321F8(void);
extern void func_00131B50(void);
extern void func_00131478(s16, s16);
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
        if (((FldAreaWork *)fldAreaState)->mode == 1 || ((FldAreaWork *)fldAreaState)->mode == 3) {
            fldClearCameraObjectHighlightFlag();
        }
        func_001321F8();
        fldUpdateCameraTarget();
        func_00131B50();
        node = ((MdlCtx *)fldCameraModelObject)->current.h.arg;
        func_00131478(node, node);
        if (fldAreaState[70] == 1) {
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
    if (((FldAreaWork *)fldAreaState)->unk188 == 0) {
        if (((FldAreaWork *)fldAreaState)->unk190 == 0) {
            if (((FldAreaWork *)fldAreaState)->verticalStepDirection == 0) {
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
    ((MdlCtx *)fldCameraModelObject)->inner->color = 0;
    if (hasSecondObject) {
        ((MdlCtx *)fldSecondarySceneModelHandle)->inner->color = 0;
    }
}

void fldRestoreSceneModelColors(void) {
    u8 hasSecondObject;

    hasSecondObject = fldSecondarySceneObject != 0;
    ((MdlCtx *)fldCameraModelObject)->inner->color = 0x80808080;
    if (hasSecondObject) {
        ((MdlCtx *)fldSecondarySceneModelHandle)->inner->color = 0x80808080;
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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133B10);

void fldSetCameraNodeModeWithTen(void) {
    s16 node = ((MdlCtx *)fldCameraModelObject)->current.h.arg;
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        fldUpdateCameraModelMotion(node, 0x12, 10.0f);
        return;
    }
    fldUpdateCameraModelMotion(node, 3, 10.0f);
}

void fldSetCameraNodeModeWithZero(void) {
    s16 node = ((MdlCtx *)fldCameraModelObject)->current.h.arg;
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        fldUpdateCameraModelMotion(node, 0x12, 0.0f);
        return;
    }
    fldUpdateCameraModelMotion(node, 3, 0.0f);
}

void fldAddCameraModelEntry(s32 value) {
    MdlCtx *object = (MdlCtx *)fldCameraModelObject;
    object->first->frameStep = 1.0f;
    mdlAddEntryPlainEx(object, 0, value, 2.0f, 5.0f);
}

void fldAddCameraModelPair(s32 first, s32 second) {
    mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 0, 1.0f);
    mdlSetNodeFrameStep((MdlCtx *)fldCameraModelObject, 1, 1.0f);
    mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 0, first);
    mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 1, second);
}

void func_00133DB8(void) {
    D_00389904[0] = 0;
}

void func_00133DC8(void) {
    D_00389910[0] = 0;
}

void fldQueueCameraXYOverride(f32 first, f32 second) {
    FldAreaWork *camera = (FldAreaWork *)fldAreaState;

    camera->facingPointX = first;
    camera->facingPointZ = second;
    camera->pointState = 1;
}

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z) {
    FldAreaWork *camera;
    f32 angle;

    angle = sdfAtan2(x, z);
    camera = (FldAreaWork *)fldAreaState;
    angle *= 180.0f / 3.14f;
    camera->angleState = 1;
    camera->overrideAngle = -angle;
}

void fldUpdateCameraHeadingFromXY(void) {
    f32 dx;
    f32 dz;

    if (((FldAreaWork *)fldAreaState)->pointState != 0) {
        dx = ((FldAreaWork *)fldAreaState)->x - ((FldAreaWork *)fldAreaState)->facingPointX;
        dz = ((FldAreaWork *)fldAreaState)->z - ((FldAreaWork *)fldAreaState)->facingPointZ;
        if (dx < 0.0001f && dx > -0.0001f && dz < 0.0001f && dz > -0.0001f) {
            return;
        }
        ((FldAreaWork *)fldAreaState)->targetAngle = -(sdfAtan2(dx, dz) * (180.0f / 3.14f));
        ((FldAreaWork *)fldAreaState)->pointState = 2;
    }
}

void fldApplyPendingCameraHeading(void) {
    FldAreaWork *camera = (FldAreaWork *)fldAreaState;

    if (camera->angleState != 0) {
        camera->angleState = 2;
        camera->targetAngle = camera->overrideAngle;
    }
}

/* ASM turning: smooth angle toward targetAngle, clearing pointState/angleState on arrival. */
INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133F08);

typedef struct FldSkyBuffer {
    u32 word[0x3800];
} FldSkyBuffer;

extern FldSkyBuffer *fldSkyLightSetBuffer;




extern SdfFlagListParams fldCameraColorParameters[];
extern FldCameraSetting *fldCameraSettings;
extern FldCameraSetting D_0038BB70;
extern void *D_004360F0;
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413280);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413290);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132A0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132E0);

void fldLoadBattleSkyAndFilter(void) {
    s32 i;
    u32 command;

    if (fldSkyLightSetBuffer == 0) {
        fldSkyLightSetBuffer = sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_004360F0 == 0) {
        D_004360F0 = sdfResourceRetainAddress(sdfAllocGeneralBlock(0xE000));
    }
    if (D_00436104 == 0) {
        D_00436104 = sdfResourceRetainAddress(sdfAllocGeneralBlock(0x12400));
    }
    if (fldCameraSettings == 0) {
        fldCameraSettings = sdfResourceRetainAddress(sdfAllocGeneralBlock(
            sizeof(FldCameraSetting) * FIELD_CAMERA_SETTING_COUNT));
        for (i = 0; i < FIELD_CAMERA_SETTING_COUNT; i++) {
            fldCameraSettings[i] = D_0038BB70;
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
    sdfDevQueueReadAndWait(command, D_00436104, 0x12400);
    sdfDevWaitThenReleaseCommandState(command);
    command = sdfDevCreateCommandState("/fld/f/bin/BATTLEBG.SKY");
    sdfDevQueueReadAndWait(command, D_004360F0, 0xE000);
    sdfDevWaitThenReleaseCommandState(command);
}

extern u32 fldRainTextureData;

extern char D_00413350[];

extern u32 sdfReadNamedResource(const char *, u32 *, s32);


void fldLoadSkyResource(s32 area) {
    char path[64];
    char directory[32];
    u32 command;

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
            fldRainTextureResource = sdfReadNamedResource(D_00413350, &fldRainTextureData, 0);
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

void fldUploadSkyBuffer(FldSkyBuffer *src) {
    fldSkyDrawState = 0x80;
    *fldSkyLightSetBuffer = *src;
    fldReleaseSkyResources();
    if (D_00389780[0] >= 2 && D_00389780[0] < 100 && fldRainTextureResource == 0) {
        fldRainTextureResource = sdfReadNamedResource(D_00413350, &fldRainTextureData, 0);
        fldRainTextureReference = sdfTexAcquireResourceTexture((void *)fldRainTextureData);
    }
}

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413350);

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00134A18);

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135A68);

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135D80);

void func_00136098(void) {
    dds3ClearObjectFlags(fldPlayerObject, 0x100);
}

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413388);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FAC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FBC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldAreaPackedArchive);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldAreaLoadRequest);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FD0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldAreaCachedResource);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FDC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FEC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldCachedRoomResourceData);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FFC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldCachedRoomResourceSize);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436004);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436008);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043600C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436010);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldPendingArea);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldPendingFloor);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436020);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436028);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436030);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436038);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436040);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436048);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436050);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldDisplayRow);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436064);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldBackgroundBuffer);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436070);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436078);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043607C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldMarkerTexture);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436088);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043608C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldEncounterRuntimeState);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldEncounterTaskName);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360AC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360BC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360CC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360DC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldSkyLightSetBuffer);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360F0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldCameraSettings);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldCameraColorEffect);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldCameraColorEnabled);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldRainTextureReference);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436104);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldSkyDrawState);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldSwayMode);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldSwayPhase);

INCLUDE_SDATA(const s32, "game/code_00128FE8", fldSwayOffset);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436118);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043611C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436120);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436124);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436128);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043612C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436130);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436134);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436138);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043613C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436140);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436144);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436148);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043614C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436150);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436154);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436158);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043615C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436160);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436164);

