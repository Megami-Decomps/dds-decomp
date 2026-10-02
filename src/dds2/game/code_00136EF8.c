#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

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

extern u64 dds3GetWorldObject(void);

extern s32 mdlFlagTest(s32);
extern int strcmp(const char *, const char *);

extern void evtSetDrawSurfaceIndex();
extern void evtSubmitGsRegister47();
extern void func_00108BD8();
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
extern s32 scrGetCurrentContext(void);
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

typedef struct FldRecE4 {
    u8 pad0[0xCC];
    s32 id;
    s32 value;
    u8 padD4[0x10];
} FldRecE4; /* 0xE4 bytes */

extern s32 D_00436188;

extern s32 D_0043617C;

extern s32 fldAreaState[];

extern u32 D_00444A30[];

extern s32 D_004361AC;

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 D_004361A8;

extern s32 D_004361B0;

extern s32 D_004361B4;

extern s32 evtStartSceneResourceTask();

extern s32 D_004361BC;

typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
} FldTaskInfo;

extern s32 dds3FindWorldObjectNodeByKey(u64, u32, s32);

extern u32 dds3GetPathState(s32);

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

/* Per-actor slot in the field's actor table; reset by fldResetActorSlots and
   scanned by fldReleaseActorTasksById. 0x5C is the stride retail uses. */
typedef struct {
    u32 unk00[11]; /* 0x00 */
    f32 unk2C;     /* 0x2C: read as a float when copying to fldAreaState[96] */
    u32 unk30[8];  /* 0x30 */
    u32 unk50[3];  /* 0x50 */
} FldActorRow; /* 0x5C bytes */

extern FldActorRow fldActorSlots[];

typedef struct FldAreaState {
    u8 pad0[0x14];
    s32 areaIndex;
    u8 pad18[0xEC];
    s16 unk104;
} FldAreaState;

/* Field actor table entries are 0x6C bytes; the area index stored by the
 * field state is zero-based, whereas entry->floor is one-based. */
typedef struct FldActorEntry {
    /* 0x00 */ s8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ s16 requiredFlag;
    /* 0x04 */ s16 floor;
    /* 0x06 */ char name[0xC];
    /* 0x12 */ s16 motion;
    /* 0x14 */ s16 secondaryMotion;
    /* 0x16 */ s16 sound;
    /* 0x18 */ u8 motionName[0xC];
    /* 0x24 */ u8 otherName[0xC];
    /* 0x30 */ s8 variantMode;
    /* 0x31 */ u8 flags31;
    /* 0x32 */ s16 variant;
    /* 0x34 */ s16 warpEntry;
    /* 0x36 */ s16 warpEntry2;
    /* 0x38 */ char warpName[0xC];
    /* 0x44 */ s8 linkKind;
    /* 0x45 */ s8 unk45;
    /* 0x46 */ char linkName[0xC];
    /* 0x52 */ s8 unk52;
    /* 0x53 */ s8 unk53;
    /* 0x54 */ s8 flags54;
    /* 0x55 */ char pad55[0xF];
    /* 0x64 */ u8 flags64;
    /* 0x65 */ s8 unk65;
    /* 0x66 */ s8 unk66;
    /* 0x67 */ s8 value67;
    /* 0x68 */ s8 unk68;
    /* 0x69 */ s8 unk69;
    /* 0x6A */ s8 unk6A;
    /* 0x6B */ s8 unk6B;
} FldActorEntry; /* 0x6C bytes */

/* Axis-aligned trigger zone: up to four bounding planes plus a 2D extent. */
typedef struct FldZone {
    s16 mode;
    s16 count;
    u8 pad4[0x14];
    f32 plane[4][4]; /* 0x18 */
    f32 limit[4];    /* 0x58 */
    f32 bound[4];    /* 0x68: min0, min1, max0, max1 */
} FldZone;

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
    D_00436188 = 0;
    D_0043617C = 0;
    if (fldValueRecords != 0) {
        fldReleaseRecordStorage();
    }
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137F10);

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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013AC40);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B0D0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B4F8);

void func_0013B810(void) {
}

void fldResetZoneRecordsAndActorSlots(void) {
    s32 i;
    s32 j;

    fldTaskSlotCount = 0;
    D_004361B8 = 0;
    D_004361BC = 0;
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
        D_0038BD50[i] = 0;
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

void fldResetTaskSlots(void) {
    s32 i;
    u64 world;
    u32 id;
    FldTaskInfo *info;

    D_004361BC = 1;
    for (i = 0; i < fldTaskSlotCount; i++) {
        D_0038BD50[i] = 0;
    }
    D_004361A8 = -1;
    D_004361AC = -1;
    D_004361B0 = -1;
    world = dds3GetWorldSecondaryObject();
    if (world != 0) {
        for (i = 0; i < fldTaskSlotCount; i++) {
            info = *(FldTaskInfo **)(D_0038BC50[i] + 8);
            if (info->slot >= 0) {
                id = dds3GetPathState(dds3FindWorldObjectNodeByKey(world, *(u32 *)D_00444A30[info->slot], 0xD));
                if (scrFindNamedProcessNode(id) != 0) {
                    evtDestroyNamedTask(dds3GetWorldObject(), id);
                }
            }
        }
    }
}

s32 fldPushDisplayValue(u32 value) {
    s32 index = D_004361B8;
    D_00444A30[index] = value;
    D_004361B8 = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013BAB8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D308);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D598);

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
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
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

void fldClearInactiveTaskSlots(void) {
    s32 count = fldTaskSlotCount;
    s32 i = 0;
    if (count > 0) {
        u32 *entry = D_0038BD50;
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
        if (D_0038BD50[i] == task) {
            return D_0038BC50[i][0];
        }
    }
    return -1;
}

s32 fldFindRoomByTask(u32 task) {
    s32 i;

    for (i = 0; i < fldTaskSlotCount; i++) {
        if (D_0038BD50[i] == task) {
            return fldRoomRecords[i].roomId;
        }
    }
    return -1;
}

s32 fldGetTaskRecordValue(u32 task) {
    s32 i;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (D_0038BD50[i] == task) {
            return D_0038BC50[i][2];
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

s32 fldHasActiveTasks(void) {
    s32 i;
    for (i = 0; i < fldTaskSlotCount; i++) {
        if (D_0038BD50[i] != 0) {
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
            && entry->floor == ((FldAreaState *)fldAreaState)->areaIndex + 1
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
            && entry->floor == ((FldAreaState *)fldAreaState)->areaIndex + 1
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

typedef struct FldTaskRecordWork {
    u8 pad00[0xE4];
    u32 key;
} FldTaskRecordWork;

void fldApplyActorEntryTrigger(s32 useTaskRecord) {
    s32 index;
    s32 kind;
    s32 record;
    FldActorEntry *entry;

    if (useTaskRecord != 0) {
        record = fldGetTaskRecordValue(((FldTaskRecordWork *)scrGetCurrentContext())->key);
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
            fldBeginNpcInteractionById(fldActorSlots[index].unk00[1]);
            return;
        }
    } else if (kind == 2) {
        if (entry->floor == fldAreaState[5] + 1) {
            fldAreaState[97] = 1;
            *(f32 *)&fldAreaState[96] = fldActorSlots[index].unk2C;
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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140A58);

u8 fldIsSceneStateEight(void) {
    return D_004361F8 == 8;
}

void fldApplySceneRoomSelection(s8 *actorEntry) {
    if (actorEntry[0x53] != 0) {
        fldAreaState[0x22] = actorEntry[0x53] - 1;
    }
    fldAreaState[0x16] = actorEntry[0x45];
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
s32 fldGetActorStat0(s32 mode) {
    FldActorEntry *actor = (FldActorEntry *)(D_003932A0 + fldSelectedActorEntryIndex * 108);
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

extern s32 D_003897C0[];

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134C0);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134D0);

s32 fldGetMappedActorStateAttribute(u32 mode) {
    FldActorEntry *actor = (FldActorEntry *)D_003932A0 + fldSelectedActorEntryIndex;
    s32 *entry;

    switch (mode) {
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
        entry = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->motionName);
        if (entry != NULL) {
            return entry[1];
        }
    case 2:
        entry = dds3FindObjectChainNodeByName(dds3GetWorldObject(), actor->otherName);
        if (entry != NULL) {
            return entry[1];
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
s32 fldGetActorMotionEntry(u32 kind) {
    FldActorEntry *entry = (FldActorEntry *)(D_003932A0 + fldSelectedActorEntryIndex * 108);
    s16 index = entry->motion;
    s32 *found;
    s32 flags;

    switch (kind) {
    case 0:
        return D_00391FA0[index].defaultMotionId;
    case 1:
        found = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00391FA0[index].primaryName);
        if (found != NULL) {
            return found[1];
        }
    case 2:
        found = dds3FindObjectChainNodeByName(dds3GetWorldObject(), D_00391FA0[index].secondaryName);
        if (found != NULL) {
            return found[1];
        }
    case 3:
        D_004361D4 = D_00391FA0[index].unk24;
        return 0;
    case 4:
        D_004361D8 = D_00391FA0[index].unk34;
        return 0;
    case 5:
        return D_00391FA0[index].unk44;
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

INCLUDE_ASM(const s32, "game/code_00136EF8", fldFindTableEntry);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142670);

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
    u32 *npc;
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
            npc = (u32 *)&fldActorSlots[i];
            if (npc[0] == 1 && npc[1] == id) {
                fldApplyRoomObjectModeZero(0, 0, actor->motionName, 0);
                return;
            }
        }
    } else {
        for (i = 0; i < 256; i++) {
            actor = (FldActorEntry *)(D_003932A0 + i * 108);
            npc = (u32 *)&fldActorSlots[i];
            if (npc[0] == 1 && npc[1] == id) {
                npc[0] = 2;
                npc[7] = 0;
                npc[8] = 0;
                npc[9] = 0;
                if (actor->motion == 5 || actor->secondaryMotion == 5 || actor->motion == 6 || actor->secondaryMotion == 6
                    || actor->motion == 7 || actor->secondaryMotion == 7 || actor->motion == 8 || actor->secondaryMotion == 8) {
                    npc[10] = 0x28;
                } else {
                    npc[10] = 0x14;
                }
                npc[11] = 0;
                npc[12] = 0;
                npc[13] = 0;
                npc[17] = 0;
                npc[18] = 0;
                npc[19] = 0;
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
        if (entry->kind == 1 && entry->floor == ((FldAreaState *)fldAreaState)->areaIndex + 1 && entry->motion != 0) {
            fldApplyRoomObjectModeOne(0, 0, entry->motionName, 0);
        }
        if (entry->kind == 11 && entry->floor == ((FldAreaState *)fldAreaState)->areaIndex + 1 && entry->motion == 3) {
            fldApplyRoomObjectModeOne(0, 0, entry->otherName, 0);
        }
    }
}

extern s32 D_00399F90[];

void fldDrawTitleBannerFrame(s32 mode) {
    evtSetDrawSurfaceIndex(0x53);
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108EC0(0x97, 0x128, 0x3A, 0x24, 1, 2, 0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldAreaState[0x7F]);
    func_00108EC0(0xD1, 0x128, 0x5E, 0x24, 0x3A, 2, 1, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldAreaState[0x7F]);
    func_00108EC0(0x12F, 0x128, 0x3A, 0x24, 0x3B, 2, -0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  fldAreaState[0x7F]);
    if (mode < 0x18) {
        func_00108BD8(1);
        func_00108EC0(0xA0, 0x12B, 0x22, 0x24, 1, 0x21, 0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], fldAreaState[0x7F]);
        func_00108EC0(0x13E, 0x12B, 0x22, 0x24, 0x23, 0x21, -0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], fldAreaState[0x7F]);
        func_00108BD8(0);
    }
}

extern s32 D_00389978[];

void fldDrawTitleBannerCursor(s32 x, s32 y) {
    evtSetDrawSurfaceIndex(0x53);
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108EC0(x, y, 0x12, 0x13, 1, 0x25, 0x12, 0x13, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00389978[0]);
    func_00108BD8(0);
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

