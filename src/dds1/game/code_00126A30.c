#include "common.h"
#include "evt_unit.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "sdf.h"

typedef struct SdfDrawPacket SdfDrawPacket;
typedef struct DmaPacketHeader DmaPacketHeader;
/* Packet addresses are 32-bit handles; GS and GIF payload words remain 64-bit. */

extern s32 scrGetCurrentContext(void);
extern void fldPlayFieldSeVolumePan(s32);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern void kwlnFadeSetRGB(s32, s32, s32);
extern void fldReleaseActorTasksById(s32);
/* Per-actor slot in the field's actor table; reset by fldResetActorSlots and
   scanned by fldReleaseActorTasksById. 0x5C is the stride retail uses. */
typedef struct {
    u32 unk00[11]; /* 0x00 */
    f32 unk2C;     /* 0x2C: written as a float by fldApplyActorEntryTrigger */
    u32 unk30[8];  /* 0x30 */
    u32 unk50[3];  /* 0x50 */
} FldActorRow;

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

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u32);
    u32 unk14[3];
} FieldResourceDescriptor;

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u64);
    u32 unk14[3];
} FieldBufferDescriptor;

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
extern u64 dds3GetWorldSecondaryObject(void);
extern s32 evtSpawnActionObj2(s32, s32);
extern s32 fldGetSceneReadyFlag(void);
extern s32 D_0032E400[];
extern void fldClearCameraObjectHighlightFlag(void);
extern void func_0012FC20(void);
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
extern void func_0012E510(void);
extern s32 fldTestSceneControlFlags(s32);
extern u8 *dds3GetObjectOwnedHandle(s32);
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
typedef struct {
    u8 pad0[0x50];
    s32 mode;
    u8 pad54[4];
    s32 rowIdx;
    u8 pad5C[8];
    f32 angle;
    u8 pad68[4];
    f32 dist;
    u8 pad70[0xD0];
    f32 x;
    f32 y;
    f32 z;
} FldCamWork;
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
extern s32 sdfConsCreateDrawPacket(s32, s32, s32);

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

extern s32 D_003BADF8;
extern s32 D_003BADEC;
extern u32 fldSkyDrawState;

extern u32 fldSwayMode;
extern f32 fldSwayPhase;
extern s32 fldSwayOffset;

extern s32 fldCameraColorEffect;
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

extern u64 dds3GetWorldObject(void);
extern s64 dds3GetWorldCameraObject(u64);
extern s64 fldGetPlayerSceneState(void);

extern s32 datBattleSceneRecords;

extern u32 D_003BACF8;

extern u32 sdfCreateResetPacketList(void);
extern s32 sdfAllocPacketAligned(s32);

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

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
extern u32 kwlnDrawSurfaces[];
extern u8 sdfViewMatrix[];
extern u32 D_003BACD4;
extern u32 D_003BACEC;
extern u32 fldMarkerTexture;
extern u32 D_003BD7C0;
extern u8 D_0032F260[];
extern void *sdfCreateAssetWithDrawEntries(void);
extern u32 sdfTexAcquireResourceTexture(void *);
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
extern u32 D_003BAD20;
extern char fldEncounterTaskName[];
extern u8 D_003C9230[];
extern u8 D_003C9220[];
extern s32 fldEncProc(void);
extern void fldResetEncounterAsyncState(void);
extern void func_00213808(void);
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_001270A8);

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
extern void fldCopyInfoTable();
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
extern s32 D_003BAC14;
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
        void *object;
        D_003BACD4 = 1;
        object = sdfCreateAssetWithDrawEntries();
        D_003BACEC = (u32)object;
        *(f32 *)((u8 *)object + 0x1C) = 1.0f;
        D_003BD7C0 = (u32)sdfCreateAssetWithDrawEntries();
        fldMarkerTexture = sdfTexAcquireResourceTexture(D_0032F260);
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

void fldSubmitSpriteRect(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, u32 packetFlags) {
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
    sdfConsCreateDrawPacket(command, packetFlags, 0);
    sdfAppendPacket((SdfListHead *)command, handle);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129178);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001292E0);

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

void fldPrepareProjectionMatrix(void) {
    u8 *matrix;
        VU0_LOAD_MATRIX(sdfViewMatrix);
;
    matrix = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(matrix);
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf28\n"
        "vmove.xyzw vf25, vf29\n"
        "vmove.xyzw vf26, vf30\n"
        "vmove.xyzw vf27, vf31\n"
        ".set reorder"
        : : : "memory");
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

void fldSubmitFrameQuad(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 packet;
    u64 *data;
    FieldBufferDescriptor *descriptor;

    sdfInitPacketList((SdfListHead *)command);
    packet = sdfAllocPacketAligned(0x30);
    data = (u64 *)sdfConsFinalizePacketHeader(packet, 0x30);
    data[4] = (bit17 << 17) | 0x10000 | (bit15 << 15) | (bit14 << 14) | (bit12 << 12) | (bit4 << 4) | (bit1 << 1) | bit0;
    data[5] = 0x47;
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

/* Write the selected blend equation to GS ALPHA_1. */
void func_00129900(s32 mode) {
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

INCLUDE_ASM(const s32, "game/code_00126A30", fldSubmitGsGradientTriangle);

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

void func_0012A5D8(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
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

void func_0012A890(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag) {
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

extern s32 sdfTexGetPrimaryBuffer(s32);
extern s32 sdfTexGetPrimaryBufferSize(s32);
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
    func_002DD708(((FldModelPacketInput *)modelData)->angle);
        VU0_STORE_MATRIX(mat);
;
    packet = sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket((u8 *)packet, mat);
    sdfAppendPacket((SdfListHead *)command, packet);
    packet = sdfAllocPacketAligned(0x80);
    func_002E2680(packet, modelData, ((FldModelPacketInput *)modelData)->geometryValue, modelData + 0x10, modelData + 0x20);
    sdfAppendPacket((SdfListHead *)command, packet);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

extern s32 kwlnGetDrawBufferIndex(void);
extern u8 kwlnFrameDrawPacketRecords[];
extern void func_002D4C80(s32, u32, s32);
extern void func_002D4CC8(s32, u32, s32);
extern void sdfAppendDmaTagToList(SdfListHead *, u32);

void fldSubmitPrimaryFramePacket(void) {
    s32 command = sdfAllocPacketAligned(0x20);
    s32 texture;
    FieldBufferDescriptor *descriptor;
    sdfInitPacketList((SdfListHead *)command);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4C80((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
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
    func_002D4CC8((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList((SdfListHead *)command, texture);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
}

extern f32 D_0032F4E0[];
extern u32 D_0032F500[];
extern void *func_002EF2B0(const void *, const void *, s32, s32);

void fldSubmitVectorColorPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    s32 resource;
    s32 record;
    FieldBufferDescriptor *descriptor;
    D_0032F4E0[0] = x;
    D_0032F4E0[1] = y;
    D_0032F4E0[2] = z;
    D_0032F4E0[4] = u;
    D_0032F4E0[5] = v;
    D_0032F4E0[6] = w;
    D_0032F500[1] = second;
    D_0032F500[0] = first;
    resource = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)resource);
    record = (s32)func_002EF2B0(D_0032F4E0, D_0032F500, 2, 0x80);
    sdfAppendPacket((SdfListHead *)resource, record);
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, resource);
}

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
extern void *func_002E21A0(FldPrimDesc *);
void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    FldPrimDesc desc;
    f32 verts[12];
    s32 indices[3];
    s32 command;
    FieldBufferDescriptor *descriptor;

    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)command);
    sdfConsAppendClearPacket(command, 0);
    sdfConsAppendAssetPacket(command, (void *)D_003BACEC, 0);
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
    sdfAppendPacket((SdfListHead *)command, (u32)func_002E21A0(&desc));
    descriptor = (FieldBufferDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
    descriptor->open(descriptor, command);
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
        FieldResourceDescriptor *descriptor;
        sdfCreateResourcePacket(packet, D_003980F0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0, 0, 0);
        descriptor = (FieldResourceDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
        descriptor->open(descriptor, packet);
    }
}

void fldSubmitBackgroundDescriptorPacket(void) {
    if (fldBackgroundBuffer != 0) {
        u32 packet = sdfAllocatePacketList(0);
        FieldResourceDescriptor *descriptor;
        sdfCreateDescriptorPacket(packet, D_003980F0[0], 0, 0, 0x200, 0xE0, fldBackgroundBuffer, 0);
        descriptor = (FieldResourceDescriptor *)&kwlnDrawSurfaces[fldDisplayRow * 8];
        descriptor->open(descriptor, packet);
    }
}

void func_0012B890(x, y, first, second)
s32 x;
s32 y;
u64 first;
u64 second;
{
    u64 object;

    object = func_00197760(x << 4, y << 4, 0, first, second, 0);
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B940);


typedef struct {
    u8 pad0[0x10];
    void (*invoke)(void *, s32);
} FldGfxCallback;
extern FldGfxCallback D_00325708;
extern FldGfxCallback kwlnPositionedTextSurface;
extern void sdfPktInit(void *, s32, s32, s32, s32);
extern s32 sdfFormatSifPacket();

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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow((s32)&quad);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.rowX + x, quad.rowY + y, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, D_003BACE0, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, D_003BACE8, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, 0);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.rowX + x * 16, quad.rowY + y * 8, quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.packetList);
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
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.rowX + (s32)(x * 16.0f), quad.rowY + (s32)(y * 8.0f), quad.drawDepth, packetField);
    sdfAppendPacket((SdfListHead *)quad.packetList, sdfFormatSifPacket(packet, drawValue));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.packetList);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C1F0);

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
    if ((recordIndex < 0x400) && ((*(u16 *)((s32)recordIndex * 0x28 + datBattleSceneRecords + 0x20) & 0x8000) != 0))
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
    FldCamWork *cam = (FldCamWork *)fldAreaState;
    f32 angle;

    D_00330650[0] = cam->x - sdfSinPoly(cam->angle * 3.14f / 180.0f) * 80.0f;
    D_00330650[1] = cam->y + fldCameraFollowRows[cam->rowIdx].y;
    D_00330650[2] = cam->z - sdfEvaluateCosineViaSinePhaseShift(cam->angle * 3.14f / 180.0f) * 80.0f;
    D_00330650[3] = 1.0f;
    angle = cam->angle * 3.14f / 180.0f;
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
    u64 worldObject;
    s64 currentEntry;
    s64 selectedEntry;

    worldObject = dds3GetWorldObject();
    currentEntry = dds3GetWorldCameraObject(worldObject);
    selectedEntry = fldGetPlayerSceneState();
    if (selectedEntry == currentEntry) {
        currentEntry = 0;
    }
    return currentEntry;
}

void fldSetCameraMoveMode(u32 value) {
    D_003BAD1C = value;
    dds3TransformCameraVectorsByInnerRotation(dds3GetWorldCameraObject(dds3GetWorldObject()), D_003C9230, D_003C9220);
    D_003BAD20 = 0;
}

INCLUDE_ASM(void, "game/code_00126A30", func_0012E510);

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
    slot = *(s32 *)(dds3GetObjectOwnedHandle(fldPlayerObject) + 0x5C);
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

s32 fldUpdateCameraFollow(void) {
    FldCamWork *cam;
    s32 *world = fldGetPlayerSceneStateAddress();
    if (*world != 0 && fldPlayerObject != 0) {
        fldRestoreCameraModelColor();
        if (fldGetUnselectedWorldEntry() != 0) {
            fldToggleWorldNodeState(1);
            func_0012E510();
            fldClearCameraObjectHighlightFlag();
            return 0;
        }
        func_0012C880();
        cam = (FldCamWork *)fldAreaState;
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

/* The target position is copied to the current camera position when pending. */
typedef struct {
    u8 pad0[0x84];
    s32 positionPending;
    u8 pad88[0xB8];
    f32 currentX;
    f32 currentY;
    f32 currentZ;
    f32 unk14C;
    f32 unk150;
    f32 unk154;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    u8 pad164[0x14];
    s32 unk178;
} FldCamState;

void fldUpdateCameraTarget(void) {
    union {
        u128 q;
        f32 f[4];
    } vec;
    f32 cur[3];
    FldCamState *st;
    u128 *dst;

    if (fldPlayerObject != 0 && (st = (FldCamState *)fldAreaState, st->unk178 != 1) && D_003BAB50 != 0) {
        cur[0] = st->currentX;
        cur[1] = st->currentY;
        cur[2] = st->currentZ;
        func_00136DA0(cur);
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
;
            dst = (u128 *)(*(u32 *)(fldPlayerObject + 0x1C) + 0x70);
            PCP_COPY_VECTOR(dst, &vec);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012F578);

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
        if (fldAreaState[20] == 1 || fldAreaState[20] == 3) {
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
    if (fldAreaState[0x5F] == 0) {
        if (fldAreaState[0x61] == 0) {
            if (fldAreaState[0x60] == 0) {
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

typedef struct FldCameraFacingWork {
    u8 pad000[0x140];
    f32 cameraX;        /* 0x140 */
    u8 pad144[4];
    f32 cameraZ;        /* 0x148 */
    u8 pad14C[0x1C];
    f32 facingAngle;    /* 0x168 */
    u8 pad16C[0x1C];
    u32 pointState;     /* 0x188: 1 pending, 2 applied */
    f32 targetX;        /* 0x18C */
    f32 targetZ;        /* 0x190 */
    u32 angleState;     /* 0x194: 1 pending, 2 applied */
    f32 targetAngle;    /* 0x198 */
} FldCameraFacingWork;

/* Camera facing requests share the field-work block. Both request states
 * advance from 1 to 2 when their new angle is installed. */
void fldQueueCameraXYOverride(f32 targetX, f32 targetZ) {
    FldCameraFacingWork *work = (FldCameraFacingWork *)fldAreaState;

    work->targetX = targetX;
    work->targetZ = targetZ;
    work->pointState = 1;
}

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z) {
    FldCameraFacingWork *work;
    f32 angle;

    angle = sdfAtan2(x, z);
    work = (FldCameraFacingWork *)fldAreaState;
    angle *= 180.0f / 3.14f;
    work->angleState = 1;
    work->targetAngle = -angle;
}

void fldApplyCameraFacingPoint(void) {
    FldCameraFacingWork *cameraWork = (FldCameraFacingWork *)fldAreaState;

    if (cameraWork->pointState != 0) {
        f32 deltaX = cameraWork->cameraX - cameraWork->targetX;
        f32 deltaZ = cameraWork->cameraZ - cameraWork->targetZ;
        f32 angle;

        cameraWork->pointState = 2;
        angle = sdfAtan2(deltaX, deltaZ);
        angle *= 180.0f / 3.14f;
        cameraWork->facingAngle = -angle;
    }
}

void fldApplyPendingCameraHeading(void) {
    FldCameraFacingWork *cameraWork = (FldCameraFacingWork *)fldAreaState;

    if (cameraWork->angleState != 0) {
        cameraWork->angleState = 2;
        cameraWork->facingAngle = cameraWork->targetAngle;
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131688);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0048);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0058);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0068);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A00A8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131A88);

extern void *fldSkyLightSetBuffer;
extern char D_003A0100[];
extern u32 fldRainTextureData;
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
extern s32 *D_003BAD74;
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
extern void *D_003BAD60;
extern s32 kwlnSetDrawColorTarget(s32, void *);
extern s32 func_00107FD8(s32, s32, void *);
extern s32 kwlnSetBackgroundColorTarget(s32, void *);
extern s32 func_001080D8(s32, s32, void *);
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
        func_001080D8(0, 0, dir);
        vec[0] = light->lightVectorAX;
        vec[1] = light->lightVectorAY;
        vec[2] = light->lightVectorAZ;
        vec[3] = 0.0f;
        func_00107FD8(0, 0, vec);
        dir[0] = light->lightDirectionBX;
        dir[1] = light->lightDirectionBY;
        dir[2] = light->lightDirectionBZ;
        dir[3] = 0.0f;
        func_001080D8(0, 1, dir);
        vec[0] = light->lightVectorBX;
        vec[1] = light->lightVectorBY;
        vec[2] = light->lightVectorBZ;
        vec[3] = 0.0f;
        func_00107FD8(0, 1, vec);
        dir[0] = light->lightDirectionCX;
        dir[1] = light->lightDirectionCY;
        dir[2] = light->lightDirectionCZ;
        dir[3] = 0.0f;
        func_001080D8(0, 2, dir);
        vec[0] = light->lightVectorCX;
        vec[1] = light->lightVectorCY;
        vec[2] = light->lightVectorCZ;
        vec[3] = 0.0f;
        func_00107FD8(0, 2, vec);
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
    func_001080D8(0, 0, dir);
    vec[0] = light->lightVectorAX;
    vec[1] = light->lightVectorAY;
    vec[2] = light->lightVectorAZ;
    vec[3] = 0;
    func_00107FD8(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    func_001080D8(0, 1, dir);
    vec[0] = light->lightVectorBX;
    vec[1] = light->lightVectorBY;
    vec[2] = light->lightVectorBZ;
    vec[3] = 0;
    func_00107FD8(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    func_001080D8(0, 2, dir);
    vec[0] = light->lightVectorCX;
    vec[1] = light->lightVectorCY;
    vec[2] = light->lightVectorCZ;
    vec[3] = 0;
    func_00107FD8(0, 2, vec);
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
    func_001080D8(0, 0, dir);
    vec[0] = light->lightVectorAX;
    vec[1] = light->lightVectorAY;
    vec[2] = light->lightVectorAZ;
    vec[3] = 0;
    func_00107FD8(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    func_001080D8(0, 1, dir);
    vec[0] = light->lightVectorBX;
    vec[1] = light->lightVectorBY;
    vec[2] = light->lightVectorBZ;
    vec[3] = 0;
    func_00107FD8(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    func_001080D8(0, 2, dir);
    vec[0] = light->lightVectorCX;
    vec[1] = light->lightVectorCY;
    vec[2] = light->lightVectorCZ;
    vec[3] = 0;
    func_00107FD8(0, 2, vec);
    vec[0] = light->finalVectorX;
    vec[1] = light->finalVectorY;
    vec[2] = light->finalVectorZ;
    vec[3] = 1.0f;
    kwlnSetBackgroundColorTarget(0, vec);
}

typedef struct FldColorParams {
    s32 enabled;
    s32 slotIndex;  /* 0x04: stored to the effect work at +0x38 */
    s32 mode;
    s32 red;
    s32 green;
    s32 blue;
    s32 vectorY;    /* 0x18: copied to the effect work at +0x24 */
    s32 vectorZ;    /* 0x1C: copied to the effect work at +0x3C */
} FldColorParams;
typedef struct FldCameraSetting {
    s32 unk0;
    FldColorParams color;
    u8 pad24[0x30];
} FldCameraSetting; /* 0x54 bytes */
typedef struct FldFadeColor {
    u8 pad0[4];
    s32 colorA;
    s32 colorB;
    u8 padC[0x18];
    s32 unk24;
    s32 unk28;
    u8 pad2C[0xC];
    s32 unk38;
    f32 unk3C;
} FldFadeColor;
extern FldFadeColor fldCameraColorParameters[];
extern FldCameraSetting *fldCameraSettings;
extern FldCameraSetting fldAppliedCameraSettings[];
extern u32 effCreateSelectionFlagListFromWork(const void *);
extern void effReleaseSelectionFlagList(s32);
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
            fldCameraColorParameters->colorB = fldCameraColorParameters->colorA = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
            fldCameraColorParameters->unk24 = color->vectorY;
            switch (color->mode) {
            case 0:
                fldCameraColorParameters->unk28 = 1;
                break;
            case 1:
                fldCameraColorParameters->unk28 = 2;
                break;
            default:
                fldCameraColorParameters->unk28 = 3;
                break;
            }
            fldCameraColorParameters->unk38 = color->slotIndex;
            fldCameraColorParameters->unk3C = color->vectorZ;
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
extern void fldUpdateCameraColorEffect(FldCameraSetting *);

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

extern void itfCopyColorFields(s32, void *);
void fldUpdateCameraColorEffect(FldCameraSetting *setting) {
    FldColorParams *color = &setting->color;

    if (color->enabled != 0) {
        fldCameraColorParameters->colorB = fldCameraColorParameters->colorA = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
        fldCameraColorParameters->unk24 = color->vectorY;
        switch (color->mode) {
        case 0:
            fldCameraColorParameters->unk28 = 1;
            break;
        case 1:
            fldCameraColorParameters->unk28 = 2;
            break;
        default:
            fldCameraColorParameters->unk28 = 3;
            break;
        }
        fldCameraColorParameters->unk38 = color->slotIndex;
        fldCameraColorParameters->unk3C = color->vectorZ;
        itfCopyColorFields(fldCameraColorEffect, fldCameraColorParameters);
    }
    *fldAppliedCameraSettings = *setting;
}


/* Keep both the resource handles and retained addresses: callers use the
 * retained storage, whereas the handles are needed at release time. */
void fldAllocateRecordStorage(void) {
    u8 *storage = sdfAllocGeneralBlock(0x72000);

    fldValueRecordResource = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    fldValueRecords = (u32)storage;
    memset(storage, 0, 0x72000);
    storage = sdfAllocGeneralBlock(0x4A00);
    fldAuxRecordResource = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    fldAuxRecordBuffer = (u32)storage;
    memset(storage, 0, 0x4A00);
}

void fldReleaseRecordStorage(void) {
    sdfDecrementAllocationReferenceCount(fldValueRecordResource);
    sdfQueueNonzeroResourceId(fldValueRecordResource);
    fldValueRecords = 0;
    sdfDecrementAllocationReferenceCount(fldAuxRecordResource);
    sdfQueueNonzeroResourceId(fldAuxRecordResource);
    fldAuxRecordBuffer = 0;
}

float fldDotVector(float *left, float *right) {
    return *left * *right + left[1] * right[1] + left[2] * right[2];
}

f32 fldCalculateVectorLength(const f32 *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void fldCalcTrianglePlane(f32 *points, f32 *nx, f32 *ny, f32 *nz, f32 *planeD) {
    f32 p0[4];
    f32 p1[4];
    f32 p2[4];
    f32 inv;

    p0[0] = points[0];
    p0[1] = points[1];
    p0[2] = points[2];
    p1[0] = points[4];
    p1[1] = points[5];
    p1[2] = points[6];
    p2[0] = points[8];
    p2[1] = points[9];
    p2[2] = points[10];
    *nx = (p1[1] - p0[1]) * (p2[2] - p1[2]) - (p1[2] - p0[2]) * (p2[1] - p1[1]);
    *ny = (p1[2] - p0[2]) * (p2[0] - p1[0]) - (p1[0] - p0[0]) * (p2[2] - p1[2]);
    *nz = (p1[0] - p0[0]) * (p2[1] - p1[1]) - (p1[1] - p0[1]) * (p2[0] - p1[0]);
    inv = 1.0f / fsqrtf(*nx * *nx + *ny * *ny + *nz * *nz);
    *nx = *nx * inv;
    *ny = *ny * inv;
    *nz = *nz * inv;
    if (*nx > -0.0001f && *nx < 0.0001f) {
        *nx = 0.0f;
    }
    if (*ny > -0.0001f && *ny < 0.0001f) {
        *ny = 0.0f;
    }
    if (*nz > -0.0001f && *nz < 0.0001f) {
        *nz = 0.0f;
    }
    *planeD = -(*nx * p2[0] + *ny * p2[1] + *nz * p2[2]);
}

typedef struct FldRecE4 {
    u8 pad0[0xCC];
    s32 id;
    s32 value;
    u8 padD4[0x10];
} FldRecE4; /* 0xE4 bytes */
s32 fldGetRecordValueById(s32 key) {
    s32 i = 0;

    if (fldValueRecordCount > 0) {
        FldRecE4 *record = (FldRecE4 *)fldValueRecords;
        do {
            if (record->id == key) {
                return record->value;
            }
            i++;
            record++;
        } while (i < fldValueRecordCount);
    }
    return 0;
}

void fldSetRecordValueById(s32 id, s32 value) {
    s32 i;

    for (i = 0; i < fldValueRecordCount; i++) {
        if (((FldRecE4 *)fldValueRecords)[i].id == id) {
            ((FldRecE4 *)fldValueRecords)[i].value = value;
        }
    }
}

void fldResetRecordState(void) {
    s32 count = fldValueRecordCount;
    if (count > 0) {
        /* Required to match: advance a pointer to the value field, not the record base. */
        u8 *record = (u8 *)fldValueRecords + 0xd0;
        do {
            count--;
            *(s32 *)record = 0;
            record += 0xe4;
        } while (count != 0);
    }
    fldValueRecordCount = 0;
    fldAreaState[0x28] = -1;
    fldAreaState[0x29] = -1;
    fldAreaState[0x2b] = -1;
    D_003BADF8 = 0;
    D_003BADEC = 0;
    if (fldValueRecords != 0) {
        fldReleaseRecordStorage();
    }
}


INCLUDE_ASM(const s32, "game/code_00126A30", func_00135360);

/* Axis-aligned trigger zone: up to four bounding planes plus a 2D extent. */
typedef struct FldZone {
    s16 mode;
    s16 count;
    u8 pad4[0x14];
    f32 plane[4][4]; /* 0x18 */
    f32 limit[4];    /* 0x58 */
    f32 bound[4];    /* 0x68: min0, min1, max0, max1 */
} FldZone;

s32 fldClassifyPositionInZoneWithMargin(f32 margin, f32 *out, s32 mode, s32 count, f32 *pos, FldZone *zone) {
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

f32 fldGetPositionZoneClearance(f32 margin, s32 mode, s32 count, f32 *pos, FldZone *zone) {
    f32 probe[3];
    f32 planar[2];
    f32 best = margin;
    f32 dist;
    f32 slack;
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
        if (slack < 0.0f || planar[0] < zone->bound[0] - margin || planar[1] < zone->bound[1] - margin ||
            zone->bound[2] + margin < planar[0] || zone->bound[3] + margin < planar[1]) {
            return -1.0f;
        }
        if (dist < 0.0f) {
            best = slack;
        }
    }
    if (best < 0.001f) {
        best = -1.0f;
    }
    return best;
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

void fldResetZoneRecordsAndActorSlots(void) {
    s32 i;
    s32 j;

    fldTaskSlotCount = 0;
    D_003BAE28 = 0;
    D_003BAE2C = 0;
    for (i = 0; i < 0x40; i++) {
        for (j = 0; j < 8; j++) {
            fldRoomRecords[i].corner[j][0] = 0.0f;
            fldRoomRecords[i].corner[j][1] = 0.0f;
            fldRoomRecords[i].corner[j][2] = 0.0f;
            fldRoomRecords[i].corner[j][3] = 1.0f;
        }
        fldRoomRecords[i].center[0] = 0.0f;
        fldRoomRecords[i].center[1] = 0.0f;
        fldRoomRecords[i].center[2] = 0.0f;
        fldRoomRecords[i].center[3] = 1.0f;
        for (j = 0; j < 6; j++) {
            fldRoomRecords[i].plane[j][0] = 0.0f;
            fldRoomRecords[i].plane[j][1] = 0.0f;
            fldRoomRecords[i].plane[j][2] = 0.0f;
            fldRoomRecords[i].plane[j][3] = 1.0f;
            fldRoomRecords[i].limit[j] = 0.0f;
        }
        D_003308B0[i] = 0;
        fldRoomRecords[i].unk108 = 0;
        fldRoomRecords[i].unk10C = 0.0f;
        fldRoomRecords[i].unk110 = 0.0f;
        fldRoomRecords[i].unk114 = 0.0f;
        fldRoomRecords[i].unk118 = 0.0f;
        fldRoomRecords[i].unk11C = 0.0f;
        fldRoomRecords[i].unk130 = 0;
        fldRoomRecords[i].roomId = -1;
        fldRoomRecords[i].unk134 = -1;
        fldRoomRecords[i].mode = 0;
        fldRoomRecords[i].unk138 = -1;
        fldRoomRecords[i].unk120 = -1;
        fldRoomRecords[i].axisMode = 0;
        fldRoomRecords[i].unk13C = 0;
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

extern u64 dds3GetWorldSecondaryObject(void);
extern s32 D_003BAE18;
extern s32 D_003BAE20;
extern s32 D_003BAE2C;
typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
} FldTaskInfo;
extern s32 dds3FindWorldObjectNodeByKey(u64, u32, s32);
extern u32 dds3GetPathState(s32);
void fldResetTaskSlots(void) {
    s32 i;
    u64 world;
    u32 id;
    FldTaskInfo *info;

    D_003BAE2C = 1;
    for (i = 0; i < fldTaskSlotCount; i++) {
        D_003308B0[i] = 0;
    }
    D_003BAE18 = -1;
    D_003BAE1C = -1;
    D_003BAE20 = -1;
    world = dds3GetWorldSecondaryObject();
    if (world != 0) {
        for (i = 0; i < fldTaskSlotCount; i++) {
            info = *(FldTaskInfo **)(D_003307B0[i] + 8);
            if (info->slot >= 0) {
                id = dds3GetPathState(dds3FindWorldObjectNodeByKey(world, *(u32 *)D_003C92E0[info->slot], 0xD));
                if (scrFindNamedProcessNode(id) != 0) {
                    evtDestroyNamedTask(dds3GetWorldObject(), id);
                }
            }
        }
    }
}

u32 fldPushDisplayValue(u32 value) {
    u32 i = D_003BAE28;

    D_003C92E0[i] = value;
    D_003BAE28 = i + 1;
    return i;
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
s32 func_0013A720(FldProbeActor *actor, NodeA *entry) {
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
        if (fldRoomRecords[i].unk108 == *(u32 *)(entry->pad + 4)) {
            kind = *((FldProbeKind *)D_003307B0[i][8])->kind;
            switch (kind) {
            case 0:
                source = *(f32 **)(entry->pad + 0x18);
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
s32 func_0013A9B0(FldProbeActor *actor, NodeA *entry) {
    f32 dir[4];
    f32 position[4];
    f32 dot;
    f32 angle;
    s32 i;
    u32 kind;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (fldRoomRecords[i].unk108 == *(u32 *)(entry->pad + 4)) {
            kind = *((FldProbeKind *)D_003307B0[i][8])->kind;
            switch (kind) {
            case 0:
                PCP_COPY_VECTOR(position, *(f32 **)(entry->pad + 0x18));
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
        evtDestroyNamedTask(dds3GetWorldObject(), flag);
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
        evtDestroyNamedTask(dds3GetWorldObject(), (u32)D_0032C9A0);
    }
    D_0032C9A0[0] = 0;
    D_003BAB3C = 0;
    object = fldSelectCurrentActorOnNextFloor();
    if (scrFindNamedProcessNode((u32)object) == 0) {
        evtStartSceneResourceTask(dds3GetWorldObject(), object);
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

void fldClearInactiveTaskSlots(void) {
    s32 count = fldTaskSlotCount;
    s32 i = 0;
    if (count > 0) {
        u32 *entry = D_003308B0;
        do {
            if (kwlnTaskIsRegistered(*entry) == 0) {
                *entry = 0;
            }
            i++;
            entry++;
        } while (i < fldTaskSlotCount);
    }
}

s32 fldFindTaskRecordId(u32 task) {
    s32 i;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (D_003308B0[i] == task) {
            return D_003307B0[i][0];
        }
    }
    return -1;
}


extern s32 fldTaskSlotCount;
s32 fldFindRoomByTask(u32 task) {
    s32 i;

    for (i = 0; i < fldTaskSlotCount; i++) {
        if (D_003308B0[i] == task) {
            return fldRoomRecords[i].roomId;
        }
    }
    return -1;
}


s32 fldGetTaskRecordValue(u32 task) {
    s32 i;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (D_003308B0[i] == task) {
            return D_003307B0[i][2];
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

s32 fldHasActiveTasks(void) {
    s32 i;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (D_003308B0[i] != 0) {
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C600);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C7F8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C9E0);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0150);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CBA8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CEB0);

INCLUDE_ASM(const s32, "game/code_00126A30", fldGetActorSlotAttribute);

extern void fldFormatAreaDirectory(char *, s32, s32);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);
extern u8 D_00332E30[];

extern void fldFormatAreaDirectory(char *, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 sdfDevQueueReadAndWait(u32, void *, u32);
extern void sdfDevWaitThenReleaseCommandState(u32);
extern u8 D_00332E30[];
extern char D_003A01F8[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */
void fldLoadInfoTable(s32 field) {
    char path[64];
    char directory[32];
    u32 command;
    if (field < 200) {
        fldFormatAreaDirectory(directory, field, 1);
        func_003014F0(path, D_003A01F8, directory, field);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, D_00332E30, 0x3B80);
        sdfDevWaitThenReleaseCommandState(command);
    }
}

void fldCopyInfoTable(const void *source) {
    memcpy(D_00332E30, source, 0x3B80);
}

extern s32 D_003BAE40;
extern u8 *fldFindActorEntryByName(const char *);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D650);

extern s32 mdlFlagTest(s32);
typedef struct FldAreaState {
    u8 pad0[0x14];
    s32 floor;
    u8 pad18[0xEC];
    s16 unk104;
} FldAreaState;
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
/* Packed 0x6C-byte field actor row. Offsets come from the parallel actor
 * searches, motion lookups, and sound dispatch below. */
typedef struct FldActorEntry {
    s8 kind;               /* 0x00 */
    u8 pad01;
    s16 requiredFlag;      /* 0x02: zero or a model-flag ID */
    s16 floor;             /* 0x04: current floor plus one */
    char name[0x0C];       /* 0x06 */
    s16 motion;            /* 0x12 */
    s16 secondaryMotion;   /* 0x14 */
    s16 sound;             /* 0x16 */
    char motionName[0x0C]; /* 0x18 */
    char otherName[0x0C];  /* 0x24 */
    s8 variantMode;        /* 0x30 */
    u8 flags31;            /* 0x31 */
    s16 variant;           /* 0x32 */
    u8 pad34[0x10];
    s8 linkKind;           /* 0x44 */
    u8 pad45;
    char linkName[0x0E];    /* 0x46 */
    u8 flags54;            /* 0x54 */
    u8 pad55[0x0F];
    u8 flags64;            /* 0x64 */
    u8 unk65;
    u8 unk66;
    s8 value67;            /* 0x67 */
    u8 unk68;
    u8 unk69;
    u8 unk6A;
    u8 unk6B;
} FldActorEntry;

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
        return fldTestBits(entry->flags54, 8);
    }
    return 0;
}

/* Same +0xE4 task-record key layout used by DDS2's EffCmdWork view. */
typedef struct FldTaskRecordWork {
    u8 pad00[0xE4];
    u32 key;
} FldTaskRecordWork;

/* Kinds 10, 11 and 12 have no case body, and that is deliberate: retail tests
   them in exactly this order (10, 11, 12, then 4), so moving any of them
   changes the branch layout and stops matching. */
void fldApplyActorEntryTrigger(s32 checkTaskRecord) {
    s32 index;
    s32 kind;
    s32 record;
    FldActorEntry *entry;

    if (checkTaskRecord != 0) {
        record = fldGetTaskRecordValue(((FldTaskRecordWork *)scrGetCurrentContext())->key);
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
            fldReleaseActorTasksById(fldActorSlots[index].unk00[1]);
            return;
        }
    } else if (kind == 2) {
        if (entry->floor == fldAreaState[5] + 1) {
            s32 motion = entry->motion;

            *(f32 *)&fldAreaState[93] = fldActorSlots[index].unk2C;
            fldAreaState[94] = 1;
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

extern s32 dds3FindIndexedObjectChainNodeByName(u64, s32, void *);
extern void dds3SetWorldCameraObject(u64, s32);
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
                    dds3SetWorldCameraObject(dds3GetWorldObject(), (s32)camera);
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

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013EF10);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F100);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F340);

s32 fldGetActorStat0(s32 mode) {
    FldActorEntry *actor = (FldActorEntry *)(D_00337D00 + fldSelectedActorEntryIndex * 108);
    s32 *entry;
    s32 flags;

    switch (mode) {
    case 0:
        return actor->motion;
    case 1:
        entry = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->motionName);
        if (entry != NULL) {
            return entry[1];
        }
    case 2:
        entry = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->otherName);
        if (entry != NULL) {
            return entry[1];
        }
    case 3:
        flags = (u16)actor->secondaryMotion;
        if (flags & 1) {
            return 1;
        }
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0270);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0280);

s32 fldGetActorStat1(u32 kind) {
    FldActorEntry *entry = (FldActorEntry *)(D_00337D00 + fldSelectedActorEntryIndex * 108);
    s32 *found;

    switch (kind) {
    case 0:
        switch (entry->motion) {
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
        found = dds3FindObjectChainNodeByName(dds3GetWorldObject(), entry->motionName);
        if (found != NULL) {
            return found[1];
        }
    case 2:
        found = dds3FindObjectChainNodeByName(dds3GetWorldObject(), entry->otherName);
        if (found != NULL) {
            return found[1];
        }
        return entry->secondaryMotion;
    case 3:
        return entry->secondaryMotion;
    case 4:
        return entry->sound;
    }
    return 0;
}

s32 fldGetActorMotionEntry(u32 kind) {
    FldActorEntry *entry = (FldActorEntry *)(D_00337D00 + fldSelectedActorEntryIndex * 108);
    s16 index = entry->motion;
    s32 *found;
    s32 flags;

    switch (kind) {
    case 0:
        return D_00336A60[index].defaultMotionId;
    case 1:
        found = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00336A60[index].primaryName);
        if (found != NULL) {
            return found[1];
        }
    case 2:
        found = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00336A60[index].secondaryName);
        if (found != NULL) {
            return found[1];
        }
    case 3:
        D_003BAE44 = D_00336A60[index].unk24;
        return 0;
    case 4:
        D_003BAE48 = D_00336A60[index].unk34;
        return 0;
    case 5:
        return D_00336A60[index].unk44;
    case 6:
        flags = entry->flags64;
        if (flags & 1) {
            return entry->value67;
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

void fldResetActorSlots(void) {
    s32 i;

    for (i = 0; i < 256; i++) {
        fldActorSlots[i].unk00[0] = 0;
        fldActorSlots[i].unk00[1] = 0;
        fldActorSlots[i].unk00[2] = 0;
        fldActorSlots[i].unk00[3] = -1;
        fldActorSlots[i].unk00[4] = 0;
        fldActorSlots[i].unk00[5] = -1;
        fldActorSlots[i].unk00[6] = -1;
        fldActorSlots[i].unk00[7] = 0;
        fldActorSlots[i].unk00[8] = 0;
        fldActorSlots[i].unk00[9] = 0;
        fldActorSlots[i].unk2C = 0.0f;
        fldActorSlots[i].unk30[0] = 0;
        fldActorSlots[i].unk30[1] = 0;
        fldActorSlots[i].unk30[2] = 0;
        fldActorSlots[i].unk30[3] = 0;
        fldActorSlots[i].unk30[4] = 0;
        fldActorSlots[i].unk30[5] = 0;
        fldActorSlots[i].unk30[6] = 0;
        fldActorSlots[i].unk30[7] = 0;
        fldActorSlots[i].unk50[0] = 0;
        fldActorSlots[i].unk50[1] = 0;
        fldActorSlots[i].unk50[2] = 0;
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
                if (task->unk00[0] == 1 && task->unk00[1] == id) {
                    task->unk00[0] = 2;
                    task->unk00[7] = 0;
                    task->unk00[8] = 0;
                    task->unk00[9] = 0;
                    if (actor->motion == 5 || actor->secondaryMotion == 5 || actor->motion == 6
                        || actor->secondaryMotion == 6 || actor->motion == 7 || actor->secondaryMotion == 7
                        || actor->motion == 8 || actor->secondaryMotion == 8) {
                        task->unk00[10] = 0x28;
                    } else {
                        task->unk00[10] = 0x14;
                    }
                    task->unk2C = 0.0f;
                    task->unk30[0] = 0;
                    task->unk30[1] = 0;
                    task->unk30[5] = 0;
                    task->unk30[6] = 0;
                    task->unk30[7] = 0;
                    fldApplyPendingCameraHeading();
                }
            }
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0380);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00140BE8);

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
        func_0012B890(x + 0x14, 0x98, 0xA09DC380, (u64)(D_0033E900 + ticket[1] * 32));
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

