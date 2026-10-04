#include "common.h"

#include "fpu.h"
#include "pcp_vu0.h"
#include "sdf.h"

typedef struct SdfDrawPacket SdfDrawPacket;
typedef struct DmaPacketHeader DmaPacketHeader;
/* Packet addresses are 32-bit handles; GS and GIF payload words remain 64-bit. */

extern s32 fldCameraModelObject;

extern void fldFreeDisplayObjects(void);

extern s32 fldSecondarySceneModelHandle;

extern s32 fldSecondarySceneObject;

extern s32 datBattleSceneRecords;

extern s32 sdfCreateResetPacketList(void);

extern s32 sdfAllocPacketAligned(s32);

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern u32 fldPendingArea;

extern u32 fldPendingFloor;

extern u32 D_00435FC0;

extern u32 fldDisplayRow;

extern s32 fldBackgroundBuffer;

extern s32 sdfAllocateBlockBySizeThreshold(u32);

extern u32 D_00436088;

extern u64 dds3GetWorldObject(void);

extern s64 dds3GetWorldCameraObject(u64);

extern s64 fldGetPlayerSceneState(void);

extern u32 D_004360AC;

extern u32 D_004360C4;

extern u32 D_004360C8;

extern u32 D_004360CC;

extern u32 D_004360D0;

extern u32 fldPlayerObject;

extern s32 fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern s32 fldRainTextureReference;

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

extern u32 fldMarkerTexture;

extern u32 D_00438EC8;

extern u8 D_0038A700[];

extern void *sdfCreateAssetWithDrawEntries(void);

extern u32 sdfTexAcquireResourceTexture(void *);

extern u32 D_004360B0;

extern u8 D_00444980[];

extern u8 D_00444970[];

extern void dds3TransformCameraVectorsByInnerRotation(s64, void *, void *);

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

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u64);
    u32 unk14[3];
} FieldBufferDescriptor;

extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern u32 sdfConsFinalizePacketHeader(u32, s32);

extern u32 kwlnDrawSurfaces[];


extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void *sdfConsAllocateColumnPacket(s32);

extern s32 sdfConsCreateDrawPacket(s32, s32, s32);

extern s32 sdfConsCalculateDrawPacketSize(s32, s32);

extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);

extern s32 sdfConsMeasurePacketWithHeader(s32);

extern u32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_0032DB30(s32, u32, s32);

extern void sdfAppendDmaTagToList(SdfListHead *, u32);

extern void func_0032DB78(s32, u32, s32);

extern f32 D_0038A980[];

extern u32 D_0038A9A0[];

extern void *func_00348158(const void *, const void *, s32, s32);

extern void *memset(void *s, s32 c, u32 n);

typedef struct FldPrimDesc {
    s16 kind;
    s16 count;
    u8 pad4[4];
    s32 color;
    u8 padC[4];
    f32 *verts;
    u8 pad14[0xC];
    s32 *indices;
    u8 pad24[8];
} FldPrimDesc; /* 0x2C bytes */

extern void sdfConsAppendClearPacket(s32, s32 (*)(s32));

extern void sdfConsAppendAssetPacket(s32, void *, s32 (*)(s32));

extern void *func_0033B050(FldPrimDesc *);

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u32);
    u32 unk14[3];
} FieldResourceDescriptor;

extern u32 D_0040B2A0[];

extern u32 sdfAllocatePacketList(s32);

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

typedef struct {
    u8 pad0[0x10];
    void (*invoke)(void *, s32);
} FldGfxCallback;

extern FldGfxCallback kwlnPositionedTextSurface;

extern void sdfPktInit(void *, s32, s32, s32, s32);

extern s32 sdfFormatSifPacket();

extern void sdfInvertScaledVuTransform(void);

extern FldGfxCallback D_00380708;

extern u8 D_00436070[];

extern u8 D_00436078[];

extern s32 fldEncounterRuntimeState;

extern u32 D_0043608C;

extern s32 func_0022E450(void);

extern s32 fldEncProc(void);

extern void func_0022E0E0(void);

extern void mdlSetNodeFloat20(s32, s32, f32);

extern void mdlAddEntryFlagged(s32, s32, s32);

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

extern void mdlSuspendAllContextMotions(s32 object);

extern void mdlResumeAllContextMotions(s32 object);

extern s32 mdlAddEntryPlainEx(s32, s32, s32, f32, f32);

extern u32 D_0043612C;

extern u32 D_00436130;

extern u32 D_00436134;

extern u32 D_003899B4[];

extern void func_00135A68(u32 value, s32 enabled);

extern u32 D_00436128;

extern u32 D_00436158;

extern s16 D_00389898[];

extern u8 D_0037F650[];

extern u8 D_0037F9B0[];

extern u8 D_0037F9F0[];

extern u8 D_0037FA00[];

extern u8 D_00384790[];

extern s32 sdfTexGetPrimaryBuffer(s32);

extern s32 sdfTexGetPrimaryBufferSize(s32);

extern void sdfConsInitDmaPacketHeader(DmaPacketHeader *, u32, s32);

extern void sdfAppendReferencePacket(SdfListHead *, u32);

extern void func_003365B8(f32);

extern void sdfInitGeometryDmaPacket(u8 *, const f32 *);

extern void func_0033B530(u64, u8 *, s32, u8 *, u8 *);

extern s32 D_00435F30;

extern void func_00139950(f32 *);

/* The target position is copied to the current camera position when pending. */
typedef struct {
    u8 pad0[0x84];
    s32 positionPending;
    u8 pad88[0xC4];
    f32 currentX;
    f32 currentY;
    f32 currentZ;
    f32 unk158;
    f32 unk15C;
    f32 unk160;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    u8 pad170[0x14];
    s32 unk184;
} FldCamState;

typedef struct FldCameraOverrides {
    u8 pad00[0x14C];
    f32 currentX;            /* 0x14C */
    u8 pad150[4];
    f32 currentZ;            /* 0x154 */
    u8 pad158[0x1C];
    f32 currentHeading;      /* 0x174 */
    u8 pad178[0x1C];
    u32 xyPending;           /* 0x194 */
    f32 xyValue0;            /* 0x198 */
    f32 xyValue1;            /* 0x19C */
    u32 headingPending;      /* 0x1A0 */
    f32 targetHeading;       /* 0x1A4 */
} FldCameraOverrides;

typedef struct FldAreaResourceState {
    s32 pad00[30];
    s32 resourceFlag;     /* 0x78 */
    s32 area;             /* 0x7C */
    s32 room;             /* 0x80 */
} FldAreaResourceState;
/* The 0x28-byte scene table checks its status halfword at +0x20. */
typedef struct FldSceneFlagRecord {
    u8 pad00[0x20];
    u16 status;
    u8 pad22[6];
} FldSceneFlagRecord;

/* Model's color state and node index, observed in the paired color setters. */
typedef struct FldModelColorState {
    u8 pad00[0x1C];
    u32 color;
} FldModelColorState;

typedef struct FldModelHandle {
    u8 pad00[0x12];
    s16 node;
    u8 pad14[4];
    FldModelColorState *colorState;
} FldModelHandle;

extern void btlActivateRuntime(s32 mode);

extern void dds3SetWorldObjectDataValue(u64, s8);

extern char fldEncounterTaskName[];

extern void btlClearRuntimeState(void);

extern s32 kwlnTaskCreate(s32 name, s32 priority, s32, s32, s32, s32, s32);

extern void func_00131000(s16, s32, f32);

extern s32 fldGetLocationCoordinateValue(s32, s32);

typedef struct {
    f32 dist;
    f32 y;
    f32 targetY;
    f32 fov;
    f32 unk10;
    f32 unk14;
} FldCamRow; /* 0x18 bytes */

typedef struct {
    u8 pad0[0x50];
    s32 mode;
    u8 pad54[4];
    s32 rowIdx;
    u8 pad5C[8];
    f32 angle;
    u8 pad68[4];
    f32 dist;
    u8 pad70[0xDC];
    f32 x;
    f32 y;
    f32 z;
} FldCamWork;

extern FldCamRow fldCameraFollowRows[];

extern f32 D_0038BAF0[];

extern f32 D_0038BB00[];

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern s32 *fldGetPlayerSceneStateAddress();

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129660);

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

extern void fldCopyInfoTable(u32);

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
                fldCopyInfoTable(entry->payload);
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

extern s32 D_00435FA4;

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
        void *object;
        D_00436064 = 1;
        object = sdfCreateAssetWithDrawEntries();
        D_0043607C = (u32)object;
        *(f32 *)((u8 *)object + 0x1C) = 1.0f;
        D_00438EC8 = (u32)sdfCreateAssetWithDrawEntries();
        fldMarkerTexture = sdfTexAcquireResourceTexture(D_0038A700);
    }
}

u32 *fldGetDisplayTableRow(void) {
    return &kwlnDrawSurfaces[fldDisplayRow * 8];
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

void fldSubmitSpriteRect(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, u32 drawMode) {
    s32 handle = (s32)sdfConsAllocateColumnPacket(1);
    FldSpriteVertex *vtx = (FldSpriteVertex *)sdfConsMeasurePacketWithHeader(handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    s32 command;
    FieldBufferDescriptor *descriptor;

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
    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    sdfConsCreateDrawPacket(command, drawMode, 0);
    sdfAppendPacket((SdfListHead *)command, handle);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B690);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B7F8);

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
    FieldBufferDescriptor *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(0x30);
    entry = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitFrameQuad(s32 flag0, s32 flag1, s32 field4, s32 field12, s32 flag14, s32 flag15, s32 unused, s32 field17) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    FieldBufferDescriptor *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(0x30);
    data = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    data[4] = (field17 << 17) | 0x10000 | (flag15 << 15) | (flag14 << 14) | (field12 << 12) | (field4 << 4) | (flag1 << 1) | flag0;
    data[5] = 0x47;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

/* Write the selected blend equation to GS ALPHA_1, including DDS2's FIX mode. */
void func_0012BE18(s32 mode) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    FieldBufferDescriptor *descriptor;

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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitGsLinesScaled(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    s32 coords[4];
    s32 command;
    s32 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    FieldBufferDescriptor *descriptor;
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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitGsLines(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    u32 coords[4];
    s32 command;
    s32 packet;
    u64 *dst;
    FieldBufferDescriptor *descriptor;
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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitGsQuadTagged(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    s32 command;
    s32 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    FieldBufferDescriptor *descriptor;
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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
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
    FieldBufferDescriptor *descriptor;
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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitGsRect(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    u32 coords[8];
    s32 command;
    s32 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u32 *pos;
    FieldBufferDescriptor *descriptor;
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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", fldSubmitGsGradientTriangle);

void fldSubmitGsGradientQuad(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3) {
    s32 coords[8];
    s32 command;
    s32 packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    FieldBufferDescriptor *descriptor;

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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void func_0012CB08(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
    s32 coords[8];
    s32 command;
    s32 packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    FieldBufferDescriptor *descriptor;

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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
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
    FieldBufferDescriptor *descriptor;
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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

/* Model draw input: angle for the VU0 matrix and a geometry packet value. */
typedef struct FldModelPacketInput {
    u8 pad00[0x40];
    s32 geometryValue; /* 0x40 */
    f32 angle;         /* 0x44 */
} FldModelPacketInput;

void fldSubmitModelPacket(s32 textureId, u8 *modelData) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 header;
    s32 packet;
    f32 mat[16];
    FieldBufferDescriptor *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    header = sdfAllocPacketAligned(0x20);
    sdfConsInitDmaPacketHeader((DmaPacketHeader *)header, sdfTexGetPrimaryBuffer(textureId), sdfTexGetPrimaryBufferSize(textureId));
    sdfAppendReferencePacket((SdfListHead *)command, header);
    func_003365B8(((FldModelPacketInput *)modelData)->angle);
    VU0_STORE_MATRIX(mat);
    packet = sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket((u8 *)packet, mat);
    sdfAppendPacket((SdfListHead *)command, packet);
    packet = sdfAllocPacketAligned(0x80);
    func_0033B530(packet, modelData, ((FldModelPacketInput *)modelData)->geometryValue, modelData + 0x10, modelData + 0x20);
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitPrimaryFramePacket(void) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 texture;
    FieldBufferDescriptor *descriptor;
    sdfInitPacketList((SdfListHead *)command);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList((SdfListHead *)command, texture);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitAlternateFramePacket(void) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 texture;
    FieldBufferDescriptor *descriptor;
    sdfInitPacketList((SdfListHead *)command);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB78((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList((SdfListHead *)command, texture);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitVectorColorPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    s32 resource;
    s32 record;
    FieldBufferDescriptor *descriptor;
    D_0038A980[0] = x;
    D_0038A980[1] = y;
    D_0038A980[2] = z;
    D_0038A980[4] = u;
    D_0038A980[5] = v;
    D_0038A980[6] = w;
    D_0038A9A0[1] = second;
    D_0038A9A0[0] = first;
    resource = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)resource);
    record = (s32)func_00348158(D_0038A980, D_0038A9A0, 2, 0x80);
    sdfAppendPacket((SdfListHead *)resource, record);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, resource);
}

void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    FldPrimDesc desc;
    f32 verts[12];
    s32 indices[3];
    s32 command;
    FieldBufferDescriptor *descriptor;

    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    sdfConsAppendClearPacket(command, 0);
    sdfConsAppendAssetPacket(command, (void *)D_0043607C, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.kind = 1;
    desc.count = 3;
    desc.verts = verts;
    desc.indices = indices;
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
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D3E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D5C0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D7E0);

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
        FieldResourceDescriptor *descriptor;
        sdfCreateResourcePacket(packet, D_0040B2A0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0, 0, 0);
        descriptor = (FieldResourceDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
        descriptor->open(descriptor, packet);
    }
}

void fldSubmitBackgroundDescriptorPacket(void) {
    if (fldBackgroundBuffer != 0) {
        u32 packet = sdfAllocatePacketList(0);
        FieldResourceDescriptor *descriptor;
        sdfCreateDescriptorPacket(packet, D_0040B2A0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0);
        descriptor = (FieldResourceDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
        descriptor->open(descriptor, packet);
    }
}

void func_0012DDC0(s32 x, s32 y, u64 firstPayload, u64 secondPayload) {
    u64 object;

    object = func_0019F460(x << 4, y << 4, 0, firstPayload, secondPayload, 0);
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
    u8 packet[16];
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
    sdfPktInit(packet, (s32)screenX * 16, (s32)screenY * 16, quad.drawDepth, 1);
    sdfAppendPacket((SdfListHead *)quad.packetList,
                    sdfFormatSifPacket(packet, D_00436070, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawFloorQuad(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    u8 packet[16];

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
    sdfPktInit(packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow(&quad);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, quad.packetList);
}

void fldDrawFloorQuadA(s32 x, s32 y, s32 packetField, s32 drawValue) {
    FldQuadState quad;
    u8 packet[16];

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
    sdfPktInit(packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadTiled(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    u8 packet[16];

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
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, D_00436070, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadTiledAlt(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    u8 packet[16];

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
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, D_00436078, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuad(s32 x, s32 y, s32 drawValue) {
    FldQuadState quad;
    u8 packet[16];

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
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadPacket(s32 x, s32 y, s32 packetField, s32 drawValue) {
    FldQuadState quad;
    u8 packet[16];

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
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadScaled(s32 packetField, s32 drawValue, f32 x, f32 y) {
    FldQuadState quad;
    u8 packet[16];

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
    sdfPktInit(packet, quad.rowX + (s32)(x * 16.0f), quad.rowY + (s32)(y * 8.0f), quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E720);

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
            func_0022E0E0();
        }
    }
    return 0;
}

void fldRequestEncounterWithFade(u32 mode, s32 recordIndex) {
    if ((recordIndex < 0x400) && ((((FldSceneFlagRecord *)datBattleSceneRecords)[recordIndex].status & 0x8000) != 0))
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
                    dds3SetWorldObjectDataValue(dds3GetWorldObject(), 1);
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
    FldCamWork *cam = (FldCamWork *)fldAreaState;
    f32 angle;

    D_0038BAF0[0] = cam->x - sdfSinPoly(cam->angle * 3.14f / 180.0f) * 80.0f;
    D_0038BAF0[1] = cam->y + fldCameraFollowRows[cam->rowIdx].y;
    D_0038BAF0[2] = cam->z - sdfEvaluateCosineViaSinePhaseShift(cam->angle * 3.14f / 180.0f) * 80.0f;
    D_0038BAF0[3] = 1.0f;
    angle = cam->angle * 3.14f / 180.0f;
    D_0038BB00[0] = cam->x + sdfSinPoly(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_0038BB00[1] = cam->y + fldCameraFollowRows[cam->rowIdx].targetY;
    D_0038BB00[2] = cam->z + sdfEvaluateCosineViaSinePhaseShift(angle) * fldCameraFollowRows[cam->rowIdx].dist;
    D_0038BB00[3] = 1.0f;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F908);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012FA58);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001300A0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001302A0);

s64 fldGetUnselectedWorldEntry(void) {
    u64 worldObject;
    s64 object;
    s64 currentObject;

    worldObject = dds3GetWorldObject();
    object = dds3GetWorldCameraObject(worldObject);
    currentObject = fldGetPlayerSceneState();
    if (currentObject == object) {
        object = 0;
    }
    return object;
}

void fldSetCameraMoveMode(u32 value) {
    D_004360AC = value;
    dds3TransformCameraVectorsByInnerRotation(dds3GetWorldCameraObject(dds3GetWorldObject()), D_00444980, D_00444970);
    D_004360B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130A40);

void fldClearCameraMoveMode(void) {
    D_004360AC = 0;
}

extern u8 *dds3GetObjectOwnedHandle(u32);
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
    slot = *(s32 *)(dds3GetObjectOwnedHandle(fldPlayerObject) + 0x5C);
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
            if (((FldCamWork *)fldAreaState)->mode == 1 || ((FldCamWork *)fldAreaState)->mode == 3) {
                fldClearCameraObjectHighlightFlag();
            } else if (((FldCamWork *)fldAreaState)->dist < 100.0f) {
                fldSetCameraObjectHighlightFlag();
            } else {
                fldClearCameraObjectHighlightFlag();
            }
        }
    }
}

extern void fldRestoreSceneModelColors(void);
extern void func_00130A40(void);
extern void func_0012FA58(void);
extern void func_001302A0(void);
extern void func_0012F908(void);

s32 fldUpdateCameraFollow(void) {
    FldCamWork *cam;
    s32 *world = fldGetPlayerSceneStateAddress();
    if (*world != 0 && fldPlayerObject != 0) {
        fldRestoreSceneModelColors();
        if (fldGetUnselectedWorldEntry() != 0) {
            fldToggleWorldNodeState(1);
            func_00130A40();
            fldClearCameraObjectHighlightFlag();
            return 0;
        }
        func_0012EDB0();
        cam = (FldCamWork *)fldAreaState;
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
            mdlResumeAllContextMotions(fldCameraModelObject);
        }
    } else {
        D_004360D0 = enabled;
        mdlSuspendAllContextMotions(fldCameraModelObject);
    }
}

void func_00130FF0(u32 first, u32 second) {
    D_004360C4 = first;
    D_004360C8 = second;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131000);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131478);

void fldUpdateCameraTarget(void) {
    union {
        u128 q;
        f32 f[4];
    } vec;
    f32 cur[3];
    FldCamState *st;
    u128 *dst;

    if (fldPlayerObject != 0 && (st = (FldCamState *)fldAreaState, st->unk184 != 1) && D_00435F30 != 0) {
        cur[0] = st->currentX;
        cur[1] = st->currentY;
        cur[2] = st->currentZ;
        func_00139950(cur);
        if (st->positionPending != 0) {
            st->currentX = st->targetX;
            st->currentY = st->targetY;
            st->currentZ = st->targetZ;
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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131B50);

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

/* Field camera model: node id at +0x12. */
typedef struct FldCameraModel {
    u8 pad00[0x12];
    s16 nodeId;
} FldCameraModel;

s32 fldUpdateCameraFrame(void) {
    s16 node;

    if (D_00389988[0] != 0) {
        return 0;
    }
    if (fldGetSceneReadyFlag() != 0) {
        return 0;
    }
    if (fldTestSceneControlFlags(0x40) == 0) {
        if (fldAreaState[20] == 1 || fldAreaState[20] == 3) {
            fldClearCameraObjectHighlightFlag();
        }
        func_001321F8();
        fldUpdateCameraTarget();
        func_00131B50();
        node = ((FldCameraModel *)fldCameraModelObject)->nodeId;
        func_00131478(node, node);
        if (fldAreaState[70] == 1) {
            func_00131000(0, 0, 6.0f);
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
    if (fldAreaState[0x62] == 0) {
        if (fldAreaState[0x64] == 0) {
            if (fldAreaState[0x63] == 0) {
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
    ((FldModelHandle *)fldCameraModelObject)->colorState->color = 0;
    if (hasSecondObject) {
        ((FldModelHandle *)fldSecondarySceneModelHandle)->colorState->color = 0;
    }
}

void fldRestoreSceneModelColors(void) {
    u8 hasSecondObject;

    hasSecondObject = fldSecondarySceneObject != 0;
    ((FldModelHandle *)fldCameraModelObject)->colorState->color = 0x80808080;
    if (hasSecondObject) {
        ((FldModelHandle *)fldSecondarySceneModelHandle)->colorState->color = 0x80808080;
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
    s16 node = ((FldModelHandle *)fldCameraModelObject)->node;
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        func_00131000(node, 0x12, 10.0f);
        return;
    }
    func_00131000(node, 3, 10.0f);
}

void fldSetCameraNodeModeWithZero(void) {
    s16 node = ((FldModelHandle *)fldCameraModelObject)->node;
    if (fldGetLocationCoordinateValue(fldAreaState[4], fldAreaState[5] + 1) & 0x40) {
        func_00131000(node, 0x12, 0.0f);
        return;
    }
    func_00131000(node, 3, 0.0f);
}

s32 fldAddCameraModelEntry(s32 value) {
    s32 object = fldCameraModelObject;
    *(f32 *)(*(s32 *)(object + 0x1C) + 0x20) = 1.0f;
    return mdlAddEntryPlainEx(object, 0, value, 2.0f, 5.0f);
}

void fldAddCameraModelPair(s32 first, s32 second) {
    mdlSetNodeFloat20(fldCameraModelObject, 0, 1.0f);
    mdlSetNodeFloat20(fldCameraModelObject, 1, 1.0f);
    mdlAddEntryFlagged(fldCameraModelObject, 0, first);
    mdlAddEntryFlagged(fldCameraModelObject, 1, second);
}

void func_00133DB8(void) {
    D_00389904[0] = 0;
}

void func_00133DC8(void) {
    D_00389910[0] = 0;
}

void fldQueueCameraXYOverride(f32 first, f32 second) {
    FldCameraOverrides *camera = (FldCameraOverrides *)fldAreaState;

    camera->xyValue0 = first;
    camera->xyValue1 = second;
    camera->xyPending = 1;
}

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z) {
    FldCameraOverrides *camera;
    f32 angle;

    angle = sdfAtan2(x, z);
    camera = (FldCameraOverrides *)fldAreaState;
    angle *= 180.0f / 3.14f;
    camera->headingPending = 1;
    camera->targetHeading = -angle;
}

void fldUpdateCameraHeadingFromXY(void) {
    f32 dx;
    f32 dz;

    if (((FldCameraOverrides *)fldAreaState)->xyPending != 0) {
        dx = ((FldCameraOverrides *)fldAreaState)->currentX - ((FldCameraOverrides *)fldAreaState)->xyValue0;
        dz = ((FldCameraOverrides *)fldAreaState)->currentZ - ((FldCameraOverrides *)fldAreaState)->xyValue1;
        if (dx < 0.0001f && dx > -0.0001f && dz < 0.0001f && dz > -0.0001f) {
            return;
        }
        ((FldCameraOverrides *)fldAreaState)->currentHeading = -(sdfAtan2(dx, dz) * (180.0f / 3.14f));
        ((FldCameraOverrides *)fldAreaState)->xyPending = 2;
    }
}

void fldApplyPendingCameraHeading(void) {
    FldCameraOverrides *camera = (FldCameraOverrides *)fldAreaState;

    if (camera->headingPending != 0) {
        camera->headingPending = 2;
        camera->currentHeading = camera->targetHeading;
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133F08);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413280);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413290);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132A0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001343E8);

typedef struct FldSkyBuffer {
    u32 word[0x3800];
} FldSkyBuffer;

extern FldSkyBuffer *fldSkyLightSetBuffer;

extern u32 fldRainTextureData;

extern char D_00413350[];

extern u32 sdfReadNamedResource(const char *, u32 *, s32);

extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001355D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135840);

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

