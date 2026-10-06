#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "fld.h"
#include "evt_world.h"
#include "scr.h"

/* Fixed allocation sizes and native room/actor table dimensions. */
enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256
};

/* vu0 routine: normalize vf10 and return its original XYZ length. */
static inline f32 fldNormalizeProbeVector(void) {
    f32 length;

    VU0_LENGTH_VF10(length);
    VU0_NORMALIZE_VF10();
    return length;
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

extern void itfCopyColorFields(s32, void *);

extern void *dds3GetWorldObject(void);
extern u32 *dds3FindIndexedObjectChainNodeByName();
extern void dds3SetWorldCameraObject();

extern s32 mdlFlagTest(s32);
extern int strcmp(const char *, const char *);

extern void evtSetDrawSurfaceIndex();
extern void evtSubmitPrimaryGsTest();
extern void evtSubmitPrimaryAlphaBlendMode();
extern void func_00108EC0();

extern s32 fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern s32 fldRainTextureReference;

extern u32 fldAuxRecordBuffer;

extern u32 fldValueRecords;

extern u32 fldAuxRecordResource;

extern u32 fldValueRecordResource;

extern s32 D_004361F8;

extern void sdfQueueNonzeroResourceId(u32 resource);

extern void *memset(void *s, s32 c, u32 n);

extern void *sdfAllocGeneralBlock(s32 size);

extern void *sdfResourceRetainAddress(void *p);

extern u32 D_0038BD50[];

extern s32 scrFindNamedProcessNode(u32 task);

extern s32 evtDestroyNamedTask(u64 world, u32 task);

extern s32 kwlnTaskIsRegistered(u32 task);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern s32 fldTaskSlotCount;

extern u32 *D_0038BC50[];

extern void fldDrawMarkerQuad(u32 value);

extern s32 D_004361CC;

extern s32 D_004361D0;

extern s32 D_004361B8;
extern s32 D_004361C4;
extern s32 D_00436194;
extern s32 D_00436198;
extern s32 D_0043619C;
extern s32 D_004361A0;
extern void fldResetActorSlots(void);

extern u8 *fldFindActorEntryByName(const char *);
extern void fldPlayMenuSound(s32);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern void kwlnFadeSetRGB(s32, s32, s32);
extern void fldApplyPendingCameraHeading(void);
extern void fldApplyRoomObjectModeZero(s32, s32, void *, s32);
extern void fldBeginNpcInteractionById(s32);
extern s32 D_003898FC[];
extern s32 D_00389780[];
extern s32 D_00399F60[];
extern u8 D_00399EA0[][16];
extern s32 fldGetCampSceneControlMode(void);
extern s32 fldGetSceneReadyOrPendingState(void);
extern s32 fileMenuTaskExists(void);
extern s32 fldHasKiretaLabelProcess(void);
extern s32 fldHasHirakenaiLabelProcess(void);
extern s32 fldHasBadkaifukuLabelProcess(void);
extern s32 fldIsEventPhaseAtLeastTwo(void);
extern u32 kwlnTaskGetUserValue(void *);
extern void func_0012DDC0(s32, s32, u32, const u8 *);

extern s16 D_00444C68[];

extern u32 fldSelectedActorEntryIndex;

extern s16 D_003932B2[];

typedef struct {
    s16 unk0;
    s16 unk2;
} FldIndexPair;

extern FldIndexPair D_0038A3B8[];

extern FldIndexPair D_0038A480[];

extern s32 D_004361E4;

extern s32 D_004361EC;

extern s32 D_004361F0;

extern s32 D_00435F28;

extern s32 D_00389784[];

extern u8 D_003932A0[];

extern u8 D_00391E50[];
extern u8 D_00391E6C[];
extern u8 D_00391E88[];
extern u8 D_00391EA4[];
extern u8 D_00391EDC[];
extern u8 D_00391EF8[];
extern u8 D_00391F30[];
extern u8 D_00391F4C[];
extern u8 D_00391F68[];
extern u8 D_00391F84[];
extern s32 D_004361FC;

extern s32 fldRainTextureResource;

extern u32 sdfTexAcquireResourceTexture(void *);

extern char D_00413350[];

extern u32 fldRainTextureData;

extern u32 fldCameraSettings;

extern FldFadeColor fldCameraColorParameters[];

extern u32 sdfReadNamedResource(const char *, u32 *, s32);

extern u32 effCreateSelectionFlagListFromWork(const void *);


extern void fldUpdateCameraColorEffect(FldCameraSetting *setting);


extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern u32 sdfDevCreateCommandState(const char *);

extern u32 sdfDevQueueReadAndWait(u32, void *, u32);

extern void sdfDevWaitThenReleaseCommandState(u32);

extern s32 fldValueRecordCount;
extern s32 D_00436188;
extern s32 D_0043617C;


extern s32 fldAreaState[];

extern u32 D_00444A30[];

extern s32 D_004361AC;

extern void *dds3GetWorldSecondaryObject(void);

extern s32 D_004361A8;

extern s32 D_004361B0;

extern s32 D_004361B4;

extern s32 evtStartSceneResourceTask();

extern s32 D_004361BC;

typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
} FldTaskInfo;

typedef struct ActionObj ActionObj;

typedef struct WorldListNode WorldListNode;

extern WorldListNode *dds3FindWorldObjectNodeByKey(EvtWorldObject *object, u32 key, s32 kind);

extern u32 dds3GetPathState(s32 path);

typedef struct FldRoomPlanes {
    f32 plane[6][4];
    f32 limit[6];
    u8 pad78[0x140 - 0x78];
} FldRoomPlanes;

extern FldRoomPlanes D_00444BC0[];

extern f32 fldDotVector(f32 *, f32 *);

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

extern FldRoomState fldRoomRecords[];

extern u8 D_0038E2D0[];

extern u8 D_0038E2D0[];

extern char D_00413448[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */

extern s32 *dds3FindObjectChainNodeByName();

typedef struct FldNpcMotion {
    s32 defaultMotionId;
    u8 primaryName[0x10];
    u8 secondaryName[0x10];
    u8 unk24[0x10];
    u8 unk34[0x10];
    s32 unk44;
} FldNpcMotion; /* 0x48 bytes */

extern FldNpcMotion D_00391FA0[];

extern void *D_004361D4;

extern void *D_004361D8;

extern u32 D_004361DC;

extern s32 D_004361E0;

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

/* Actor slots use three transition keys, signed frame counters and two float
   triples with per-frame increments; func_00142B70 initializes the motion data. */
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

typedef struct FldAreaState {
    u8 pad0[0x10];
    s32 area;
    s32 floor;
    u8 pad18[0x40];
    s32 unk58;
    u8 pad5C[0x2C];
    s32 unk88;
    u8 pad8C[0x74];
    s32 unk100;
    s16 unk104;
} FldAreaState;



INCLUDE_ASM(const s32, "game/code_00136EF8", func_00136EF8);

void fldInitializeCameraColorResource(void) {
    fldRainTextureResource = sdfReadNamedResource(D_00413350, &fldRainTextureData, 0);
    fldRainTextureReference = sdfTexAcquireResourceTexture((void *)fldRainTextureData);
    fldCameraColorEffect = effCreateSelectionFlagListFromWork(fldCameraColorParameters);
    if (fldRainTextureResource != 0) {
        sdfQueueNonzeroResourceId(fldRainTextureResource);
        fldRainTextureResource = 0;
    }
    fldUpdateCameraColorEffect(fldCameraSettings);
    fldCameraColorEnabled = 1;
}

void func_00137888(void) {
    func_00136EF8();
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

typedef struct FldSaveHeader {
    u32 word[0x15]; /* 0x54 bytes */
} FldSaveHeader;

extern FldCameraSetting fldAppliedCameraSettings[];

void fldCopyCameraSetting(FldSaveHeader *dst) {
    *dst = *(FldSaveHeader *)fldAppliedCameraSettings;
}


void fldUpdateCameraColorEffect(FldCameraSetting *setting) {
    FldColorParams *color = &setting->color;

    if (color->enabled != 0) {
        fldCameraColorParameters->colorB = fldCameraColorParameters->colorA =
            (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
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
    D_00436188 = 0;
    D_0043617C = 0;
    if (fldValueRecords != 0) {
        fldReleaseRecordStorage();
    }
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137F10);

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
            /* The mode-1 plane uses a fixed 45-unit inset, unlike the
               other projections, which use the caller's margin. */
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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139628);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139950);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139B98);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139EC0);

void func_0013AA78(void) {
}

void func_0013AA80(void) {
}

void func_0013AA88(void) {
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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013AC40);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B0D0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B4F8);

void func_0013B810(void) {
}

/* Reset room records and selection sentinels, then apply actor-slot defaults. */
void fldResetZoneRecordsAndActorSlots(void) {
    s32 roomIndex;
    s32 vectorIndex;

    fldTaskSlotCount = 0;
    D_004361B8 = 0;
    D_004361BC = 0;
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
        D_0038BD50[roomIndex] = 0;
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
    D_00436194 = -1;
    D_00436198 = -1;
    D_0043619C = -1;
    D_004361A0 = -1;
    D_004361A8 = -1;
    D_004361AC = -1;
    D_004361B0 = -1;
    D_004361C4 = 0;
    fldResetActorSlots();
}

/* Clear slot handles and destroy named tasks reached through linked display values. */
void fldResetTaskSlots(void) {
    s32 slotIndex;
    EvtWorldObject *world;
    u32 task;
    FldTaskInfo *taskInfo;

    D_004361BC = 1;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        D_0038BD50[slotIndex] = 0;
    }
    D_004361A8 = -1;
    D_004361AC = -1;
    D_004361B0 = -1;
    world = dds3GetWorldSecondaryObject();
    if (world != 0) {
        for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
            taskInfo = *(FldTaskInfo **)(D_0038BC50[slotIndex] + 8);
            if (taskInfo->slot >= 0) {
                task = dds3GetPathState((s32)dds3FindWorldObjectNodeByKey(world, *(u32 *)D_00444A30[taskInfo->slot], 0xD));
                if (scrFindNamedProcessNode(task) != 0) {
                    evtDestroyNamedTask(dds3GetWorldObject(), task);
                }
            }
        }
    }
}

/* Append a display value and return its index; no capacity check is performed. */
s32 fldPushDisplayValue(u32 value, ActionObj *unusedObject) {
    s32 index = D_004361B8;
    D_00444A30[index] = value;
    D_004361B8 = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013BAB8);

typedef struct FldProbeKind {
    u8 pad00[0xC];
    u32 *kind; /* 0x0C */
} FldProbeKind;

typedef struct FldProbeTarget {
    u8 pad00[0x40];
    f32 position[4]; /* 0x40 */
    f32 quaternion[4]; /* 0x50 */
} FldProbeTarget;

typedef struct FldProbeActor {
    u8 pad00[0x1C];
    FldProbeTarget *target; /* 0x1C */
} FldProbeActor;

extern void effMiscQuaternionToMatrixVU(void);
/* vu0 routine: actor-facing probe for the world kind-0x11 position payload. */
s32 fldTestRoomProbeFacingAndRange(FldProbeActor *actor, NodeA *entry) {
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
            kind = *((FldProbeKind *)D_0038BC50[i][8])->kind;
            switch (kind) {
            case 0:
                source = entry->payload;
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
s32 fldTestRoomProbeFacing(FldProbeActor *actor, NodeA *entry) {
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
            kind = *((FldProbeKind *)D_0038BC50[i][8])->kind;
            switch (kind) {
            case 0:
                PCP_COPY_VECTOR(position, entry->payload);
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

    kind = *((FldProbeKind *)D_0038BC50[index][8])->kind;
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
        if (fldDotVector(probe, D_00444BC0[index].plane[i + 1]) - D_00444BC0[index].limit[i + 1] < 0.0f) {
            return -1;
        }
    }
    return 0;
}

s32 fldRoomContainsPoint(f32 *direction, s32 index) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (fldDotVector(direction, D_00444BC0[index].plane[i]) - D_00444BC0[index].limit[i] > 0.0f) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DBB0);

s32 fldDestroyFlaggedNamedTask(u32 flag, u32 slot) {
    D_0038BD50[slot] = 0;
    if (scrFindNamedProcessNode(flag) != 0) {
        evtDestroyNamedTask(dds3GetWorldObject(), flag);
        return 0;
    }
    return -1;
}

u32 fldDestroyTaskSlot(u32 index) {
    u32 *taskSlot = &D_0038BD50[index];

    if (kwlnTaskIsRegistered(*taskSlot) != 0) {
        kwlnTaskDestroyWithHierarchy(*taskSlot, 0);
    }
    *taskSlot = 0;
    return 0;
}

extern s8 D_00387D60[];
extern u8 D_00435F24;
extern u8 *fldSelectCurrentActorOnNextFloor(void);

s32 fldRestartSceneResourceTask(void) {
    u8 *object;
    u32 state = D_00435F24;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && D_00387D60[0] != 0) {
        evtDestroyNamedTask(dds3GetWorldObject(), D_00387D60);
    }
    D_00387D60[0] = 0;
    D_00435F24 = 0;
    object = fldSelectCurrentActorOnNextFloor();
    if (scrFindNamedProcessNode((u32)object) == 0) {
        evtStartSceneResourceTask(dds3GetWorldObject(), object);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DDC0);

void fldDrawTaskMarkers(void) {
    s32 count = fldTaskSlotCount;
    s32 i = 0;
    if (count > 0) {
        u32 **entry = D_0038BC50;
        do {
            fldDrawMarkerQuad((*entry)[4]);
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
        u32 *taskSlot = D_0038BD50;
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
        if (D_0038BD50[slotIndex] == task) {
            return D_0038BC50[slotIndex][0];
        }
    }
    return -1;
}

/* Return the first matching task slot's room ID; -1 denotes a miss. */
s32 fldFindRoomByTask(u32 task) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_0038BD50[slotIndex] == task) {
            return fldRoomRecords[slotIndex].roomId;
        }
    }
    return -1;
}

/* Return the first matching task slot's third record word; zero on miss. */
s32 fldGetTaskRecordValue(u32 task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_0038BD50[slotIndex] == task) {
            return D_0038BC50[slotIndex][2];
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EB30);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013ED20);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EEA0);

extern s32 D_004361C0;

s32 fldCheckEntryActive(s32 value) {
    s32 blocked = D_004361C0;
    s32 index = D_004361AC;

    if (blocked != 0) {
        return 1;
    }
    if (index == -1) {
        return 0;
    }
    if (fldRoomRecords[index].mode == 1 && fldRoomRecords[index].unk138 == value) {
        return 1;
    }
    return 0;
}

/* Test for any nonzero slot handle, without consulting task registration. */
s32 fldHasActiveTasks(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldTaskSlotCount; slotIndex++) {
        if (D_0038BD50[slotIndex] != 0) {
            return 1;
        }
    }
    return 0;
}

s32 fldGetCurrentSceneSelectionId(void) {
    s32 index = D_004361CC;

    if (index < 0) {
        return -1;
    }
    return D_00444C68[index * 160];
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F1E8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F3E0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F5C8);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004133A0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F790);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013FA98);

extern s32 D_004361C8;

INCLUDE_ASM(const s32, "game/code_00136EF8", fldGetActorSlotAttribute);

void fldLoadInfoTable(s32 field) {
    char path[64];
    char directory[32];
    u32 command;
    if (field < 200) {
        fldFormatAreaDirectory(directory, field, 1);
        func_0035C860(path, D_00413448, directory, field);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, D_0038E2D0, 0x3B80);
        sdfDevWaitThenReleaseCommandState(command);
    }
}

typedef struct FldSaveBlock {
    u32 word[0xEE0]; /* 0x3B80 bytes */
} FldSaveBlock;

void fldCopyInfoTable(FldSaveBlock *src) {
    *(FldSaveBlock *)D_0038E2D0 = *src;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140238);

/* Resolve a named actor in the current area to its template; the selected
 * template kind is also recorded for the caller in D_004361FC. */
INCLUDE_RODATA(const s32, "game/code_00136EF8", D_00413448);

u8 *fldPickActorTemplateByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = (FldActorEntry *)(D_003932A0 + i * 108);
        flag = entry->requiredFlag;
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && entry->floor == ((FldAreaState *)fldAreaState)->floor + 1
            && strcmp(name, entry->name) == 0) {
            switch (entry->kind) {
            case 1:
                if (((FldAreaState *)fldAreaState)->unk104 != 0 && (entry->flags31 & 4)) {
                    D_004361FC = 0xB;
                    return D_00391F84;
                }
                if (entry->variantMode == 0) {
                    sub = entry->variant;
                    if (sub != 0) {
                        if (sub == 1) {
                            D_004361FC = 6;
                            return D_00391EF8;
                        }
                    }
                }
                D_004361FC = 0;
                return D_00391E50;
            case 2:
                D_004361FC = 1;
                return D_00391E6C;
            case 3:
                D_004361FC = 2;
                return D_00391E88;
            case 4:
                D_004361FC = 3;
                return D_00391EA4;
            case 5:
                D_004361FC = 5;
                return D_00391EDC;
            case 10:
                D_004361FC = 8;
                return D_00391F30;
            case 11:
                D_004361FC = 9;
                return D_00391F4C;
            case 12:
                D_004361FC = 0xA;
                return D_00391F68;
            }
        }
        i++;
    } while (i < 0x100);
    D_004361FC = -1;
    return 0;
}

/* Like the template lookup, but also records the matching actor slot and
 * the kind-specific scene state before returning its template. */
u8 *fldFindActorEntryByName(const char *name) {
    s32 i = 0;
    FldActorEntry *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = (FldActorEntry *)(D_003932A0 + i * 108);
        flag = entry->requiredFlag;
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && entry->floor == ((FldAreaState *)fldAreaState)->floor + 1
            && strcmp(name, entry->name) == 0) {
            fldSelectedActorEntryIndex = i;
            switch (entry->kind) {
            case 1:
                if (((FldAreaState *)fldAreaState)->unk104 != 0 && (entry->flags31 & 4)) {
                    D_004361FC = 0xB;
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
u8 *fldSelectCurrentActorOnNextFloor(void) {
    s32 index = D_00435F28;
    FldActorEntry *entry = (FldActorEntry *)(D_003932A0 + index * 108);
    if (entry->floor == D_00389784[0] + 1 && entry->kind == 10) {
        fldSelectedActorEntryIndex = index;
        D_004361F8 = 8;
        return D_00391F30;
    }
    return 0;
}

s32 fldGetSelectedActorMotionId(void) {
    return D_003932B2[fldSelectedActorEntryIndex * 54];
}

/* Inspect two independent properties of the selected actor, depending on mode:
 * mode 0 derives a size from the first two states; mode 1 tests a flag. */
s32 fldQuerySelectedActorMotionState(s32 mode) {
    FldActorEntry *entry = (FldActorEntry *)(D_003932A0 + fldSelectedActorEntryIndex * 0x6C);
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
    s32 record;
    FldActorEntry *entry;

    if (useTaskRecord != 0) {
        record = fldGetTaskRecordValue((u32)scrGetCurrentContext()->task);
        if (record == 0) {
            return;
        }
        if (fldFindActorEntryByName((const char *)record) == 0) {
            return;
        }
    }
    index = fldSelectedActorEntryIndex;
    entry = (FldActorEntry *)(D_003932A0 + index * 108);
    kind = entry->kind;
    if (kind == 1) {
        if (entry->floor == D_00389784[0] + 1) {
            fldPlayMenuSound(entry->sound);
            fldBeginNpcInteractionById(fldActorSlots[index].actorId);
            return;
        }
    } else if (kind == 2) {
        if (entry->floor == fldAreaState[5] + 1) {
            fldAreaState[97] = 1;
            *(f32 *)&fldAreaState[96] = fldActorSlots[index].firstValues[0];
            if (entry->motion == 1) {
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
            D_003898FC[0] = 0x64;
        } else {
            D_003898FC[0] = -0x64;
        }
    } else if (kind == 10) {
    } else if (kind == 11) {
        if (entry->floor == D_00389784[0] + 1) {
            if (entry->motion == 3) {
                fldApplyRoomObjectModeZero(0, 0, entry->otherName, 0);
                return;
            }
        }
    } else if (kind == 12) {
    } else if (kind == 4) {
        fldApplyPendingCameraHeading();
    }
}

void func_00140A58(const char *name) {
    FldActorEntry *entry;
    char *entryName;
    u32 *camera;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = (FldActorEntry *)D_003932A0 + i;
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
                    camera = dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(), 4, entry->linkName);
                    dds3SetWorldCameraObject(dds3GetWorldObject(), camera);
                    fldHideSceneModelsAndResetCamera();
                    return;
                }
            }
        }
    }
}

u8 fldIsSceneStateEight(void) {
    return D_004361F8 == 8;
}

void fldApplySceneRoomSelection(FldActorEntry *actorEntry) {
    if (actorEntry->unk53 != 0) {
        ((FldAreaState *)fldAreaState)->unk88 = actorEntry->unk53 - 1;
    }
    ((FldAreaState *)fldAreaState)->unk58 = actorEntry->unk45;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140BC8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001411F8);

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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141898);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141B20);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141CF0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141F58);

/* Read a selected actor state or a named world-object value; cases 1 and 2
 * deliberately fall through when no named object is found. */
s32 fldGetActorStat0(s32 attribute) {
    FldActorEntry *actor = (FldActorEntry *)(D_003932A0 + fldSelectedActorEntryIndex * 108);
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

extern s32 D_003897C0[];

/* Read a selected actor attribute; selector zero maps its motion code.
 * Missing named objects fall through to subsequent attribute cases. */
INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134C0);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134D0);

s32 fldGetMappedActorStateAttribute(u32 attribute) {
    FldActorEntry *actor = (FldActorEntry *)D_003932A0 + fldSelectedActorEntryIndex;
    s32 *objectNode;

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
    case 5:
        return D_003897C0[0] != 1;
    }
    return 0;
}

/* Look up a motion-table attribute indexed by this actor's state; named
 * object lookups fall through to the next attribute if absent. */
s32 fldGetActorMotionEntry(u32 attribute) {
    FldActorEntry *actor = (FldActorEntry *)(D_003932A0 + fldSelectedActorEntryIndex * 108);
    s16 motionIndex = actor->motion;
    s32 *objectNode;
    s32 flags;

    switch (attribute) {
    case 0:
        return D_00391FA0[motionIndex].defaultMotionId;
    case 1:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00391FA0[motionIndex].primaryName);
        if (objectNode != NULL) {
            return objectNode[1];
        }
    case 2:
        objectNode = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00391FA0[motionIndex].secondaryName);
        if (objectNode != NULL) {
            return objectNode[1];
        }
    case 3:
        D_004361D4 = D_00391FA0[motionIndex].unk24;
        return 0;
    case 4:
        D_004361D8 = D_00391FA0[motionIndex].unk34;
        return 0;
    case 5:
        return D_00391FA0[motionIndex].unk44;
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
    s32 slot = D_004361DC;

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
        return fldActorWaypointRows[slot].count - D_004361E0 - 1;
    }
    return 0;
}

s32 fldFindTableEntry(s32 index) {
    s32 slot = D_004361DC;
    s32 count = (slot + fldActorWaypointRows)->count;

    if (count - 1 < index) {
        return count - D_004361E0 - 1;
    }
    index = count - index - 1;
    return fldActorWaypointRows[slot].body.data[index];
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142670);

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
        memset(fldActorWaypointRows, 0, 0x6D00);
    } else {
        fldFormatAreaDirectory(directory, field, 1);
        func_0035C860(path, "%sF%03d.WAP", directory, field);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, fldActorWaypointRows, 0x6D00);
        sdfDevWaitThenReleaseCommandState(command);
    }
}

typedef struct FldWaypointBlock {
    u32 word[0x1B40]; /* 0x6D00 bytes */
} FldWaypointBlock;

void fldCopyActorWaypointTable(FldWaypointBlock *src) {
    *(FldWaypointBlock *)fldActorWaypointRows = *src;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142B70);

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
            actor = (FldActorEntry *)(D_003932A0 + i * 108);
            npc = &fldActorSlots[i];
            if (npc->kind == 1 && npc->actorId == id) {
                fldApplyRoomObjectModeZero(0, 0, actor->motionName, 0);
                return;
            }
        }
    } else {
        for (i = 0; i < 256; i++) {
            actor = (FldActorEntry *)(D_003932A0 + i * 108);
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

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004135D0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143910);

void func_00143C90(void) {
}

extern void fldApplyRoomObjectModeOne(s32, s32, void *, s32);

void fldApplyCurrentAreaActorEntries(void) {
    FldActorEntry *entry;
    s32 i;

    for (i = 0; i < 256; i++) {
        entry = (FldActorEntry *)(D_003932A0 + i * 108);
        if (entry->kind == 1 && entry->floor == ((FldAreaState *)fldAreaState)->floor + 1 && entry->motion != 0) {
            fldApplyRoomObjectModeOne(0, 0, entry->motionName, 0);
        }
        if (entry->kind == 11 && entry->floor == ((FldAreaState *)fldAreaState)->floor + 1 && entry->motion == 3) {
            fldApplyRoomObjectModeOne(0, 0, entry->otherName, 0);
        }
    }
}

extern s32 D_00399F90[];

void fldDrawTitleBannerFrame(s32 mode) {
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108EC0(0x97, 0x128, 0x3A, 0x24, 1, 2, 0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldAreaState[0x7F]);
    func_00108EC0(0xD1, 0x128, 0x5E, 0x24, 0x3A, 2, 1, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldAreaState[0x7F]);
    func_00108EC0(0x12F, 0x128, 0x3A, 0x24, 0x3B, 2, -0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldAreaState[0x7F]);
    if (mode < 0x18) {
        evtSubmitPrimaryAlphaBlendMode(1);
        func_00108EC0(0xA0, 0x12B, 0x22, 0x24, 1, 0x21, 0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], fldAreaState[0x7F]);
        func_00108EC0(0x13E, 0x12B, 0x22, 0x24, 0x23, 0x21, -0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], fldAreaState[0x7F]);
        evtSubmitPrimaryAlphaBlendMode(0);
    }
}

extern s32 D_00389978[];

void fldDrawTitleBannerCursor(s32 x, s32 y) {
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108EC0(x, y, 0x12, 0x13, 1, 0x25, 0x12, 0x13, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00389978[0]);
    evtSubmitPrimaryAlphaBlendMode(0);
}

typedef struct FldMenuState {
    u16 position;
    u16 choice;
    u16 pending;
} FldMenuState;

s32 func_00144028(void *task) {
    FldMenuState *menu;

    if (D_00389780[0] >= 200) {
        return 0;
    }
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
    menu = (FldMenuState *)kwlnTaskGetUserValue(task);
    if (menu->pending != 0) {
        fldDrawTitleBannerFrame(menu->position);
        fldDrawTitleBannerCursor(D_00399F60[menu->choice] - 9, 0x130);
        itfSetTextDrawLimit(0x13);
        func_0012DDC0(D_00399F60[menu->choice] + 10, 0x98, 0xA09DC380, D_00399EA0[menu->choice]);
        itfSetTextDrawLimit(-1);
        menu->position++;
        menu->pending = 0;
    } else {
        menu->position = 0;
    }
    return 0;
}

extern void *sdfAllocSizeClassBlock(s32 size);
extern void kwlnTaskSetUserValue(s32, void *);

void *fldInitializeTitleBannerTask(s32 task) {
    s16 *node = sdfAllocSizeClassBlock(8);
    node[1] = 1;
    node[0] = 0;
    node[2] = 0;
    node[3] = 0;
    kwlnTaskSetUserValue(task, node);
    return func_00144028;
}

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436174);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldAuxRecordBuffer);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_0043617C);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecords);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecordCount);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436188);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldAuxRecordResource);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldValueRecordResource);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436194);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436198);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_0043619C);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361A0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldTaskSlotCount);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361A8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361AC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361B0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361B4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361B8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361BC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361C0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361C4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361C8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361CC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361D0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361D4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361D8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361DC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361E0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361E4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361E8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361EC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361F0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldSelectedActorEntryIndex);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361F8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361FC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", fldFieldTaskHandle);

