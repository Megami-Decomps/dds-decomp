#include "common.h"
#include "sdf_model.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "mdl.h"
#include "dat_state.h"
#include "eff.h"
#include "dds3obj.h"

extern void effMiscAxisAngleToQuaternionVU(f32 angle);
extern void effMiscQuatMultiplyVU(void);
extern u32 dds3AdvanceWorldCounter(void);
extern EffWorldNode *dds3SpawnCameraSlotObj5(s32 value, void *position, void *rotation);
extern void dds3SetWorldNodeValue(EffWorldNode *node, u32 value);
extern EffWorldNode *dds3GetWorldSecondaryObject(void);
extern void dds3SetWorldPlayerObject(EffWorldNode *world, EffWorldNode *node);
extern void func_00112058(EffWorldNode *node, s32 kind, s32 resource);
extern void effObjSetInnerFloat(EffWorldNode *node, f32 value);
extern void effObjSetInnerSecondVec(EffWorldNode *node, u128 *vector);
extern void effObjSetInnerThirdVec(EffWorldNode *node, u128 *vector);
extern void sdfSetTextFloatPairOverride(void *param, f32 first, f32 second);
extern void func_00136718(void);
extern void func_001526B8(void);
extern void func_00153FA0(void);
extern EffWorldNode *D_00435F1C;
extern MdlCtx *D_00435F20;

extern void func_001542D8(void);
extern void func_001523F0(void);
extern void func_001525F0(void);
extern void func_00152C88(void);
extern void func_00153068(void);
extern s32 func_001515E0(f32 x, f32 z, s16 *gridX, s16 *gridY);


extern s32 D_003899E0[];
extern f32 sdfViewEyeVector[4];
extern f32 sdfViewTargetVector[4];
extern u8 D_00435C48[2][2];
extern f32 sdfViewUpVector[4];
extern u32 fldGetSceneReadyFlag(void);
extern void kwlnFadeStartOut(s32);
extern void kwlnFadeStartIn(s32);
extern void fldGetVisibleSceneBounds(f32 *, f32 *, f32 *, f32 *);
extern void sndSetSequenceVolumePan(s32, s32, s32);

typedef struct EffNode EffNode;
typedef struct EffNodeDescriptor EffNodeDescriptor;

extern void mdlAddEntryPlain(MdlCtx *, s32, s32);
extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);
extern u8 fldTestSceneControlFlags(u32);
extern void effDestroyNode(EffNode *);

extern f32 fldAngleDifference(f32, f32);

/* Retained field-area work, including camera/event state and named resources.
 * DDS2 loads automap TMX files separately rather than taking them from fldmix.LB. */
typedef struct FldResourceBlock {
    s32 unk0;  /* Retained named-resource result; only written here. */
    s32 block;
} FldResourceBlock;

typedef struct FldTextureResource {
    s32 unk0; /* Retained named-resource result; only written here. */
    s32 block;
    SdfTex *texture;
} FldTextureResource;

typedef struct FldAreaWork {
    u8 pad00[0x10];
    s32 area; /* 0x10 */
    s32 room; /* 0x14: the room argument of fldSetSceneLocation. */
    s32 unk18;
    u8 pad1C[8];
    s32 titleFade;       /* 0x24: enables the field-input transition fade. */
    u8 pad28[8];
    f32 focusPos[3];      /* 0x30 */
    u8 unk3C[0x14];
    s32 focusActive;      /* 0x50 */
    u8 unk54[0x10];
    f32 negatedAngle;     /* 0x64 */
    u8 unk68[8];
    s32 unk70;            /* 0x70 */
    u8 unk74[0x4C];
    s32 unkC0;            /* 0xC0 */
    u8 unkC4[8];
    s32 unkCC;            /* 0xCC: location-panel mode, zero through seven. */
    u8 padD0[0x34];
    s16 eventActive;      /* 0x104 */
    u8 pad106[0xE];
    s32 unk114;
    s32 unk118;
    u8 pad11C[0xE];
    s16 unk12A; /* 0x12A */
    u8 pad12C[0xC];
    s32 unk138;
    u8 pad13C[4];
    s32 targetGuideActive; /* 0x140: selects the per-frame guide update path. */
    s32 unk144;           /* 0x144: location-panel fade countdown. */
    u8 pad148[4];
    f32 x;                /* 0x14C */
    f32 y;
    f32 z;
    u8 unk158[0x18];
    f32 angle;            /* 0x170 */
    u8 pad174[0x34];
    FldResourceBlock mapResources[8]; /* 0x1A8: autmap_1,2,3,5,6,7,8,9. */
    FldTextureResource fieldTextures[4]; /* 0x1E8: d2_fild1..4.tmx. */
} FldAreaWork;

#define FLD_WORK ((FldAreaWork *)fldAreaState)

typedef struct FldSlot0C {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} FldSlot0C; /* 0xC bytes */

extern SdfModel *D_003A5470[];

extern FldSlot0C D_0044F818[];

extern s32 D_004362D4;

extern u32 D_004362D8;

extern s32 D_004362E0;

extern EffNodeDescriptor *D_004362E4;

extern s32 D_004362EC;

extern EffNodeDescriptor *D_004362F0;

extern s32 D_004362F8;

extern EffNodeDescriptor *D_004362FC;

extern s32 D_00436304;

extern s32 D_00436308;

extern s32 D_00436310;

extern s32 D_00436314;

extern s32 D_00436318;

extern s32 D_0043631C;

extern s32 D_00436338;

extern s32 D_0043633C;

extern s32 D_00438EDC;

extern s32 D_00438EE0;

extern s32 D_00438EE4;

extern s32 D_00438EE8;

extern s32 D_00438EEC;

extern s32 D_00438EF0;


extern s32 D_004363C4;

extern void mnuAdvanceTitleStateUnderSemaphore(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern s32 fldRoomEffectEntryCount;

extern u32 D_00436330[];

extern EffNode *effCreateNodeFromDescriptor(EffNodeDescriptor *);

extern s32 fldObjectSlotCount;

extern s32 fldSceneSoundBase;

extern f32 sdfAtan2(f32, f32);

extern f32 fldLookAtFarPoint[];

extern s32 D_003899D8[];

extern s32 D_00436228;

extern s32 D_0039A0B8[];

extern s32 D_0039A028[];

extern s32 func_0035C860(char *, const char *, ...);

extern char D_00413F20[];

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 sdfReadNamedResource(char *resource, void *info, s32 options);

extern SdfTex *D_0044F7F0[];
extern void sdfTexReleaseReferenceViaHandler(SdfTex *);

extern u32 D_004363C8;

extern u32 D_004363CC;

extern u32 D_004363D0;

extern u32 D_004363D4;

extern u32 D_00436380;

extern u32 D_00436384;

extern u32 fldDamEffectNode;

extern u32 fldDamEffectPositioned;

extern u32 D_00436390;

extern u32 D_00436394;

extern u32 fldYukEffectNode;

extern u32 fldYukEffectPositioned;

extern SdfTex *D_004363AC;

extern SdfTex *D_004363B0;

extern SdfTex *D_004363B4;

extern u32 D_00436370;

extern s32 D_00436374;

extern SdfTex *D_00436368;

extern SdfTex *D_0043636C;

extern u32 fldIndexedResourceHandle;

extern u32 fldIndexedResourceData;

extern s32 fldIndexedResourceEffect;


extern u32 D_0043623C;

extern u32 fldFieldTaskHandle;

extern void *kwlnTaskGetUserValue();

extern u32 fldCurrentBgmHandle;

extern u32 fldArchiveLoadPending;

extern u32 fldFixedArchiveLoadPhase;

extern u32 fldSceneRecords;

extern s32 fldSceneRecordResource;

extern u32 fldSceneReady;

extern u32 D_00436364;

extern s32 D_00436354;

extern s32 D_0043635C;

extern s32 D_00436350;

extern s32 D_00436358;

extern s32 D_00436360;

extern void sdfReleaseResourceAllocation(s32);

extern s32 fldTitleTaskUpdate();

extern SdfTex *D_0043637C;

extern void fldStopCurrentBgm(void);

extern void fldPlayCurrentBgmSound(void);

extern void fldReleaseCurrentBgm(void);

extern void fldPlayMenuSound(s32 param);

extern void fldPlayFieldSe(s32 param);

extern s32 fldPollArchiveLoad(s32 param);

extern void fldSetArchiveSoundVolumePan(s32 param0, s32 param1);

extern void fldPlayArchiveSound(s32 param0, s32 param1);

extern void fldStartTitle(s32 param0, s32 param1, s32 param2);

extern void func_0014BF98(s32 handle);

extern s32 fldFindEffectByName(char *str);

extern s32 fldGetCurrentSceneSelectionResource(void);

extern void *sdfAllocSizeClassBlock(s32 size);

extern void kwlnTaskSetUserValue(s32 arg0, void *arg1);

extern s32 fldFieldTaskUpdate(void);

extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern u8 D_00436208[];

extern s32 fldPendingSoundCount;

typedef struct {
    s32 flags;
    s32 soundId;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FldClear18; /* 0x18 bytes */

extern FldClear18 fldPendingSounds[];

extern s32 fldAreaState[];

extern void func_00342580(s32 arg0);

extern s32 fldCurrentBgmId[];

extern void func_00341C78(u32);

extern void fldRelocatePackedTransferChunk(u32 arg0, s32 arg1);

extern s32 func_00129D60(s32 arg0);

extern s32 D_00389780[];

extern s32 fldAreaFlagIndex;

extern s32 D_00436248;

extern s32 D_00389834[];

extern s32 D_00389838[];

extern s32 fldPrimaryEffectPositionPending;

extern f32 fldPrimaryQueuedEffectPosition[];

extern s32 fldSecondaryEffectPositionPending;

extern f32 fldSecondaryQueuedEffectPosition[];

extern char fldTitleTaskName[]; /* "fldTitle" */

extern s32 kwlnTaskGetTaskByName(void *name);

extern char D_00413C80[]; /* "fldTitleMini" */

extern void effUpdateNode(u32 arg0);

extern void ddsReleaseUnitObject(EffWorldNode *node);

typedef struct {
    s32 objectHandle;
    s32 unk4;
    u8 pad8[0xC];
} FldEnt14; /* 0x14 bytes */

extern FldEnt14 fldSparkObjectEntries[];

extern s32 D_00389884[];

extern u8 D_003A9EB0[];

extern void evtCreateMessageWindowIfMissing(void *arg0);

extern s32 dspStartEntry(s32 arg0);

extern void fldResetPlayerSceneObjectState(void);

extern void *D_00451B94[];

extern void func_0035B6E0(const char *fmt, ...);

extern void fldPreparePlayerSceneCameraTarget(void);

extern s32 mnuPositionedResourceNodes[];

extern s32 mnuPositionedResourceActive[];

extern s32 D_0043633C;

extern s32 D_00451B9C[];

extern void fldClearFloorFlag(s32, s32, s32);

extern void fldSetFloorFlag(s32, s32, s32);

extern void sdfWaitSlotReady(void);

extern void fldReleaseSceneDevSlotsAndTextures(void);

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern s32 fldGetCurrentSceneSelectionId(void);

typedef struct {
    s32 *resource;
    u8 pad[0x4C];
} FieldResourceSlot;

extern FieldResourceSlot D_0044FFB0[];

extern void fldCacheMapLabelLengths(s32);

extern void func_00145698(void);

extern s32 fldStageSoundBaseTable[];

extern s32 mdlFlagTest();

typedef struct {
    s16 id;
    s16 flag;
    s8 data[0x40];
} FldEnt44; /* 0x44 bytes */

extern FldEnt44 D_00389A70[32];

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 fldSceneBgmArchiveTrack;

extern s32 fldSceneBgmArchivePhase;

extern s32 sndFindPackedTrackLoadStatus(s32);

extern void sndEnsureMidiBankResident(s32);

extern s32 fldSceneRecordCount;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} FldPoint;

typedef struct {
    u8 pad0[0x10];
    s32 value;
    FldPoint *pointA; /* 0x14 */
    FldPoint *pointB; /* 0x18 */
} FldItem; /* 0x1C bytes */

typedef struct {
    u8 pad0[4];
    FldItem *items;
    u32 count;
    SdfItemListRef *model; /* 0xC */
    FldPoint *pos;     /* 0x10 */
} FldSceneRecord; /* 0x14 bytes */

extern s32 fldFindRecordItem(s32 scene, u32 index);

extern s32 fldEffectTextureLoadHandles[];

extern s32 fldEffectTextureData[];

extern s32 fldEffectTextureNodes[];

extern s32 fldEffectTextureLoadHandles[4];

extern s32 fldEffectTextureData[4];

extern s32 fldEffectTextureNodes[4];

typedef struct {
    s32 unk0, unk4, unk8, unkC;
    u8 pad10[0x10];
    s32 *object;
    u8 pad24[8];
    s32 unk2C, unk30, unk34, room;
    s16 unk3C, unk3E;
    char name[0x10];
} FldTblEnt50; /* 0x50 bytes */

extern FldTblEnt50 fldRoomEffectEntries[];

typedef struct {
    s32 unk0;
    s32 id;
    s32 unk8;
    s32 activationRequested;
    u8 pad10[0x10];
    s32 effectVariant;
    s32 effectNode;
    u8 pad28[8];
} FldObj30; /* 0x30 bytes */

extern FldObj30 fldObjectSlots[32];

extern s32 D_00436300;

extern s32 D_0043630C;

extern s32 fldTestMapSlotAuxiliaryFlag();

extern void dds3SetObjectFlags();

extern void effRestartNodeInstance(EffNode *node);


typedef struct FieldPair48 {
    f32 pos[4];
    f32 vel[4];
    s32 hasVectors;
    s32 active;
    s32 unk28;
    s16 objectSlot;
    s16 unk2E;
} FieldPair48; /* 0x30 bytes */

extern FieldPair48 fldSparkSlots[];

extern u32 effMiscRand();

extern s16 D_00389874[];

typedef struct FldVec4 {
    f32 v[4];
} FldVec4;

extern void effObjSetInnerFirstVec(EffWorldNode *node, u128 *vector);

extern u32 dds3GetObjectBaseResourceHandle(void *object);

extern void dds3ClearObjectFlags();

extern FldVec4 D_00413DE8;

extern s32 fldSparkControlState[];

extern void func_0014F5F0();

extern s32 func_0014F980(s32, s32);

extern s32 fldSparkControlState[];

extern s32 fldSparkControlState[];

extern s32 fldSparkControlState[];

extern s32 dds3GetWorldObject(void);

extern int strcmp(const char *, const char *);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern u8 fldGetCampSceneControlMode(void);
extern u8 fldGetSceneReadyOrPendingState(void);
extern s32 fileMenuTaskExists(void);
extern u8 fldHasKiretaLabelProcess(void);
extern u8 fldHasHirakenaiLabelProcess(void);
extern u8 fldHasBadkaifukuLabelProcess(void);
extern s32 fldIsEventPhaseAtLeastTwo(void);
extern u32 fldInputPanelTaskHandle;
extern void fldUpdateCameraHeadingFromXY(void);
extern void kwlnFadeStartIn(s32 duration);
extern u32 D_00389988[];
extern u8 D_0037F510[2][2][16];
extern s32 D_004361D0;

typedef struct FldTitleBannerMenu {
    u16 position;
    u16 choice;
    u16 pending;
    u16 reserved;
} FldTitleBannerMenu; /* 8-byte allocation from fldInitializeTitleBannerTask. */

typedef struct {
    u8 pad0[0xC];
    s32 *drawNodeHandle;
} FldEmitterRes;

typedef struct {
    FldEmitterRes *res;
    u8 pad4[0x4C];
    f32 pos[4];
} FldEmitter;

extern u8 D_00380838[];

extern void sdfDrawNodeBuildMatrix();

extern void sdfModelUpdateCurrentFrameTransforms();

extern void func_003320E8();

s32 func_001442D0(void) {
    FldTitleBannerMenu *menu;
    FldAreaWork *area;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (fldGetSceneReadyOrPendingState() != 0) {
        return 0;
    }
    if (D_00389988[0] != 0) {
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

    menu = (FldTitleBannerMenu *)kwlnTaskGetUserValue(fldInputPanelTaskHandle);
    if (menu->pending == 0) {
        return -1;
    }
    if (FLD_WORK->titleFade != 0 ||
        D_004361D0 == 1 || (s8)D_0037F510[1][0][1] < 0) {
        if ((s8)D_0037F510[1][0][1] < 0) {
            fldUpdateCameraHeadingFromXY();
        }
        area = (FldAreaWork *)fldAreaState;
        menu->pending = 0;
        if (area->titleFade != 0) {
            kwlnFadeStartIn(8);
            area->titleFade = 0;
        }
        return 1;
    }
    return 0;
}

extern s32 D_00435EE0;

extern void fldDrawAnimatedFieldBanner(s32 alpha, s32 x, s32 y);

s32 fldFieldTaskUpdate(void) {
    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (fldGetSceneReadyOrPendingState() != 0) {
        return 0;
    }
    if (fileMenuTaskExists() != 0) {
        return 0;
    }
    if (fldTitleIsActive() != 0) {
        return 0;
    }
    if (D_00389884[0] == 1) {
        return 0;
    }
    if (D_00435EE0 != -0x3e7) {
        fldDrawAnimatedFieldBanner(0x80, 0, 0);
    } else {
        func_0014B0F8(0);
    }
    return 0;
}

void *fldFieldTaskCreate(s32 task) {
    s32 *work;

    work = sdfAllocSizeClassBlock(0x10);
    work[0] = 0;
    work[1] = 0;
    work[2] = 0;
    work[3] = 0;
    kwlnTaskSetUserValue(task, work);
    return fldFieldTaskUpdate;
}

void fldFieldTaskDestroy(void) {
    u64 work;

    work = kwlnTaskGetUserValue();
    sdfReleaseChipBlock(work);
    fldFieldTaskHandle = 0;
}

void fldEnsureTask(void) {
    if (fldFieldTaskHandle == 0) {
        fldFieldTaskHandle = kwlnTaskCreate(D_00436208, 0x2B0B, 1, 1, fldFieldTaskCreate, fldFieldTaskDestroy, 0);
    }
}

void fldDestroyTask(void) {
    if (fldFieldTaskHandle != 0) {
        kwlnTaskDestroyWithHierarchy(fldFieldTaskHandle, 1);
    }
}

void fldResetPendingSounds(void) {
    FldClear18 *entry = fldPendingSounds;
    s32 remaining = 7;

    do {
        remaining -= 1;
        entry->flags = 0;
        entry->x = 0;
        entry->soundId = 0;
        entry += 1;
    } while (remaining >= 0);
    fldPendingSoundCount = 0;
}

void fldAppendPendingSoundForScene(s32 id, f32 x, f32 y, f32 z, f32 w) {
    if (fldAreaState[4] == 0x1A && mdlFlagTest(0x1F) != 0) {
        return;
    }
    if (fldAreaState[4] == 0x17 && fldAreaState[5] == 7 && (mdlFlagTest(0x4C5) == 0 || mdlFlagTest(0x1C) != 0)) {
        return;
    }
    fldPendingSounds[fldPendingSoundCount].flags = 0;
    fldPendingSounds[fldPendingSoundCount].soundId = id;
    fldPendingSounds[fldPendingSoundCount].x = x;
    fldPendingSounds[fldPendingSoundCount].y = y;
    fldPendingSounds[fldPendingSoundCount].z = z;
    fldPendingSounds[fldPendingSoundCount].w = w;
    fldPendingSoundCount++;
}

void fldPlayPendingSounds(void) {
    s32 stage;
    s32 i;

    stage = D_00389780[0];
    if (stage == 11) {
        stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
    }
    for (i = 0; i < fldPendingSoundCount; i++) {
        if (fldPendingSounds[i].flags & 1) {
            fldPendingSounds[i].flags &= ~1;
            func_00341C78(fldStageSoundBaseTable[stage] + fldPendingSounds[i].soundId);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001447A0);

extern s32 D_0039A0BC[];

extern s32 D_0039A0C0[];

extern s32 D_0039A0C4[];

extern s32 D_0039A0C8[];

extern s32 D_0039A0D0[];

extern const char D_00413700[];

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004136C0);

s32 fldResolveSpecialBgmTrack(s32 id) {
    s32 result;

    switch (id) {
    case 0x80:
        result = D_0039A0B8[0] + 1;
        break;
    case 0x81:
        result = D_0039A0BC[0] + 1;
        break;
    case 0x82:
        result = D_0039A0C0[0] + 1;
        break;
    case 0x83:
        result = D_0039A0BC[0] + 2;
        break;
    case 0x84:
        result = D_0039A0C4[0] + 1;
        break;
    case 0x85:
        result = D_0039A0C4[0] + 2;
        break;
    case 0x86:
        result = D_0039A0C8[0] + 1;
        break;
    case 0x87:
        result = D_0039A0D0[0] + 1;
        break;
    case 0x88:
        result = D_0039A0D0[0] + 2;
        break;
    case 0x89:
        result = D_0039A0D0[0] + 3;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

s8 fldFindSceneEntryData(s32 id, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_00389A70[i].id == id && (D_00389A70[i].flag == 0 || mdlFlagTest(D_00389A70[i].flag) != 0)) {
            return D_00389A70[i].data[idx];
        }
    }
    return 0;
}

void fldStartSceneBgm(void) {
    s32 handle;
    s32 stage;
    s32 idx;

    if (D_003899D8[0] != 0) {
        handle = fldResolveSpecialBgmTrack(fldAreaState[10]);
        if (handle != -1 || fldAreaState[4] < 0x32) {
            if (handle == -1) {
                stage = fldAreaState[4];
                if (stage == 11) {
                    stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
                }
                idx = fldFindSceneEntryData(stage, fldAreaState[5] + 1);
                fldSceneSoundBase = fldStageSoundBaseTable[stage];
                fldAreaState[10] = idx;
                handle = fldSceneSoundBase + idx;
            }
            if (fldAreaState[0x130 / 4] != 1) {
                if (fldCurrentBgmHandle != handle) {
                    func_00342580(fldCurrentBgmHandle);
                }
                fldCurrentBgmHandle = handle;
                sndStartTrackDefault(handle);
            }
        }
    }
}

s32 fldStartTitleBgmIfSelected(void) {
    if (D_003899D8[0] != 0) {
        if (fldCurrentBgmId[0] == 0x80) {
            sndStartTrackDefault(D_0039A0B8[0] + 1);
        }
    }
}

extern void sndStartTrackAlternate(s32, s32);

void fldStartSceneBgmAlternate(void) {
    s32 handle;
    s32 stage;
    s32 idx;

    if (D_003899D8[0] != 0) {
        handle = fldResolveSpecialBgmTrack(fldAreaState[10]);
        if (handle != -1 || fldAreaState[4] < 0x32) {
            if (handle == -1) {
                stage = fldAreaState[4];
                if (stage == 11) {
                    stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
                }
                idx = fldFindSceneEntryData(stage, fldAreaState[5] + 1);
                fldSceneSoundBase = fldStageSoundBaseTable[stage];
                fldAreaState[10] = idx;
                handle = fldSceneSoundBase + idx;
            }
            if (fldAreaState[0x130 / 4] != 1) {
                fldCurrentBgmHandle = handle;
                sndStartTrackAlternate(handle, handle);
            }
        }
    }
}

void fldStopCurrentBgm(void) {
    sndSendSingleWordControlPacket(fldCurrentBgmHandle);
    fldCurrentBgmHandle = 0;
}

void fldReleaseCurrentBgm(void) {
    func_00342580(fldCurrentBgmHandle);
    if (fldAreaState[10] >= 0x80) {
        fldAreaState[10] = 1;
    }
    fldCurrentBgmHandle = 0;
}

void func_00144F48(void) {
    func_00341CA8();
}

void fldPlayCurrentBgmSound(void) {
    func_00341C78(fldCurrentBgmHandle);
    fldCurrentBgmId[0] = 0;
}

void fldPlayMenuSound(s32 id) {
    if (fldAreaState[4] < 0x32) {
        if (id >= 0x10) {
            if (id == 0x80) {
                sndSetSequenceVolumePan(0x6d0060, 0x7f, 0x3f);
            } else if (id == 0x81) {
                sndSetSequenceVolumePan(0x6d0061, 0x7f, 0x3f);
            } else if (id == 0x82) {
                sndSetSequenceVolumePan(0x6d0064, 0x7f, 0x3f);
            } else if (id >= 0x200) {
                sndSetSequenceVolumePan(0x68fe00 + id, 0x7f, 0x3f);
            } else {
                sndSetSequenceVolumePan(fldSceneSoundBase + id, 0x7f, 0x3f);
            }
        }
    }
}

void fldPlayFieldSe(s32 id) {
    if (D_00389780[0] < 50) {
        if (id == 0x100) {
            func_00341C78(0x6D0060);
        } else if (id == 0x101) {
            func_00341C78(0x6D0061);
        } else if (id == 0x102) {
            func_00341C78(0x6D0064);
        } else {
            func_00341C78(fldSceneSoundBase + id);
        }
    }
}

void fldSetSequenceVolume(s32 category, s32 volume) {
    if (D_00389780[0] < 50) {
        sndSetSequenceVolumePan(fldSceneSoundBase + category, volume, 0x3F);
    }
}

void fldSelectBgmMode(s32 selection) {
    s32 volume;
    if (D_003899D8[0] == 0) {
        return;
    }
    switch (selection) {
    case -1:
        if (D_00436228 != selection) {
            func_00341C78(D_00436228);
            D_00436228 = selection;
        }
        break;
    case 0:
        if (D_00436228 != -1) {
            func_00342580(D_00436228);
            D_00436228 = -1;
        }
        break;
    case 1:
        volume = D_0039A0B8[0] + 1;
        sndStartTrackDefault(volume);
        D_00436228 = volume;
        break;
    case 2:
        volume = D_0039A028[0] + 1;
        sndStartTrackDefault(volume);
        D_00436228 = volume;
        break;
    }
}

void fldPrepareSceneBgmArchive(void) {
    s32 handle = fldResolveSpecialBgmTrack(fldAreaState[10]);
    s32 stage;
    s32 idx;

    if (handle == -1) {
        if (fldAreaState[4] >= 0x32) {
            return;
        }
        stage = fldAreaState[4];
        if (stage == 11) {
            stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
        }
        idx = fldFindSceneEntryData(stage, fldAreaState[5] + 1);
        fldSceneSoundBase = fldStageSoundBaseTable[stage];
        fldAreaState[10] = idx;
        handle = fldSceneSoundBase + idx;
    }
    fldSceneBgmArchiveTrack = handle;
    fldSceneBgmArchivePhase = 0;
}

s32 fldStepSceneBgmArchive(void) {
    switch (fldSceneBgmArchivePhase) {
    case 0:
        if (sndFindPackedTrackLoadStatus(fldSceneBgmArchiveTrack) == 0) {
            sndEnsureMidiBankResident(fldSceneBgmArchiveTrack);
            fldSceneBgmArchivePhase = fldSceneBgmArchivePhase + 1;
        } else {
            fldSceneBgmArchivePhase = fldSceneBgmArchivePhase + 2;
        }
        break;
    case 1:
        if (sndFindPackedTrackLoadStatus(fldSceneBgmArchiveTrack) == 1) {
            fldSceneBgmArchivePhase = fldSceneBgmArchivePhase + 1;
        }
        break;
    default:
        fldSceneBgmArchivePhase = -1;
        return 1;
    }
    return 0;
}

u32 fldGetArchiveLoadPending(void) {
    return fldArchiveLoadPending;
}

s32 fldPollArchiveLoad(s32 id) {
    s32 name = 0x30000000 + (id << 16);
    s32 result = sndFindPackedTrackLoadStatus(name);
    if (result == 0) {
        sndEnsureMidiBankResident(name);
        fldArchiveLoadPending = 1;
        return 0;
    }
    if (result == 1) {
        fldArchiveLoadPending = 0;
        return 1;
    }
    return 0;
}

void fldSetArchiveSoundVolumePan(s32 archiveId, s32 soundId) {
    sndSetSequenceVolumePan(archiveId * 0x10000 + soundId + 0x30000000, 0x7f, 0x3f);
}

void fldPlayArchiveSound(s32 archiveId, s32 soundId) {
    func_00341C78(archiveId * 0x10000 + soundId + 0x30000000);
}

void fldResetArchiveLoadPhase(void) {
    fldFixedArchiveLoadPhase = 0;
}

s32 fldStepArchiveLoad(void) {
    switch (fldFixedArchiveLoadPhase) {
    case 0:
        if (sndFindPackedTrackLoadStatus(0x680000) == 0) {
            sndEnsureMidiBankResident(0x680000);
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        } else {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 2;
        }
        break;
    case 1:
        if (sndFindPackedTrackLoadStatus(0x680000) == 1) {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        }
        break;
    default:
        fldFixedArchiveLoadPhase = -1;
        return 1;
    }
    return 0;
}

void fldResetSecondaryArchiveLoadPhase(void) {
    D_0043623C = 0;
}

s32 fldStepSecondaryArchiveLoad(void) {
    switch (D_0043623C) {
    case 0:
        if (sndFindPackedTrackLoadStatus(0x690000) == 0) {
            sndEnsureMidiBankResident(0x690000);
            D_0043623C = D_0043623C + 1;
        } else {
            D_0043623C = D_0043623C + 2;
        }
        break;
    case 1:
        if (sndFindPackedTrackLoadStatus(0x690000) == 1) {
            D_0043623C = D_0043623C + 1;
        }
        break;
    default:
        D_0043623C = -1;
        return 1;
    }
    return 0;
}

u32 fldGetCurrentBgmHandle(void) {
    return fldCurrentBgmHandle;
}

typedef struct FldName30 { char s[0x1E]; } FldName30;

typedef struct FldName34 { char s[0x22]; } FldName34;

typedef struct FldName28 { char s[0x1C]; } FldName28;

typedef struct FldName24 { char s[0x18]; } FldName24;

extern s16 D_0044FC98[];

extern s16 D_0044FD48[];

extern s16 D_0044FCF0[];

extern s16 D_00438ED8;

extern FldName30 D_0039A5AC[];

extern FldName34 D_0039E1AC[];

extern FldName28 D_003A25AC[];

extern FldName24 D_0039A1D0[];

extern s32 strlen(const char *);

extern s32 func_001237B0(s32, s32);

extern s32 func_00123808(s32, s32);

extern s32 fldFindMapCoordinateIndex(s32, s32);

void fldCacheMapLabelLengths(s32 world) {
    s32 i;

    for (i = 0; i < 41; i++) {
        D_0044FC98[i] = strlen(D_0039A5AC[func_001237B0(world, i)].s);
    }
    for (i = 0; i < 24; i++) {
        D_0044FD48[i] = strlen(D_0039E1AC[func_00123808(world, i)].s);
    }
    for (i = 0; i < 41; i++) {
        D_0044FCF0[i] = strlen(D_003A25AC[fldFindMapCoordinateIndex(world, i)].s);
    }
    i = 0;
    if (D_00389780[0] < 100) {
        i = D_00389780[0];
    }
    D_00438ED8 = strlen(D_0039A1D0[i].s);
}

void func_00145698(void) {
    typedef struct {
        u8 pad00[4];
        s32 rows;
        s32 count;
    } SceneHeader;
    char path[0x80];
    char directory[0x40];
    s32 resourceHandle;
    s32 transferStart;
    s32 header;

    fldFormatAreaDirectory(directory, fldAreaState[4], 1);
    func_0035C860(path, (const char *)D_00413700, directory, fldAreaState[4]);
    fldSceneRecordResource = sdfReadNamedResource(path, &resourceHandle, 0);
    transferStart = resourceHandle + 8;
    fldRelocatePackedTransferChunk(resourceHandle, transferStart);
    header = func_00129D60(transferStart);
    fldSceneRecords = ((SceneHeader *)header)->rows;
    fldSceneRecordCount = ((SceneHeader *)header)->count;
}

void fldSetSceneRecordChunk(s32 chunk, s32 resourceId) {
    /* Descriptor from func_00129D60 precedes the 0x14-byte scene rows. */
    typedef struct {
        u8 pad00[4];
        s32 rows; /* 0x04: first scene row */
        s32 count; /* 0x08: number of scene rows */
    } SceneHeader;
    if (D_00389780[0] < 0xC8) {
        s32 source = chunk;
        s32 resource = resourceId;
        s32 transferStart = source + 8;

        fldSceneRecordResource = resource;
        fldRelocatePackedTransferChunk(chunk, transferStart);
        {
            s32 header = func_00129D60(transferStart);
            s32 rows = ((SceneHeader *)header)->rows;
            s32 count = ((SceneHeader *)header)->count;

            fldSceneRecordCount = count;
            fldSceneRecords = rows;
        }
    }
}

void fldInitSceneMapLabels(void) {
    if (D_00389780[0] < 200) {
        fldCacheMapLabelLengths(D_00389780[0] % 100);
        func_00145698();
    }
}

void fldReleaseSceneRecordChunk(void) {
    if (fldSceneRecordResource != 0) {
        sdfQueueNonzeroResourceId(fldSceneRecordResource);
    }
    fldSceneRecordResource = 0;
    fldSceneRecords = 0;
    fldSceneRecordCount = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145818);

void fldSetEmitterPosition(FldEmitter *emitter, f32 x, f32 y, f32 z) {
    f32 pos[4];
    s32 handle;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    handle = *emitter->res->drawNodeHandle;
    VU0_LOAD_VF_MEMORY(vf10, pos);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, emitter->pos);
    sdfDrawNodeBuildMatrix(handle);
    sdfModelUpdateCurrentFrameTransforms(emitter);
    func_003320E8(D_00380838, emitter);
}

extern void fldSubmitSpriteRect(s32, s32, s32, s32, s32, s32, s32, s32,
                                s32, SdfTex *);

typedef struct {
    s16 u;
    s16 v;
    s16 width;
    s16 height;
    s16 anchorX;
    s16 anchorY;
    s32 textureSlot;
} FldProjectedSprite;

extern FldProjectedSprite D_0039A0E0[];

/* Convert projected GS coordinates to the field's centered sprite origin. */
void func_00145948(s32 index, u32 color, f32 x, f32 y) {
    FldProjectedSprite *sprite = &D_0039A0E0[index];
    s32 anchorY = sprite->anchorY;
    s16 u = sprite->u;
    s16 v = sprite->v;

    y -= 2048.0f;
    x -= 2048.0f;
    y += y;
    y += 224.0f;
    x += 256.0f;
    fldSubmitSpriteRect((s32)(x - sprite->anchorX), (s32)(y - anchorY),
                        sprite->width, sprite->height, u, v,
                        sprite->width, sprite->height, color,
                        D_0044F7F0[D_0039A0E0[index].textureSlot]);
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145A08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145D48);

s32 fldFindRecordItem(s32 scene, u32 index) {
    s32 result = 1;
    FldSceneRecord *rec = (FldSceneRecord *)fldSceneRecords;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)fldSceneRecordCount; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (i == scene && j == index) {
                result = item->value + 1;
            }
        }
    }
    return result;
}

typedef struct FldModelNodeView {
    u8 pad0[4];
    struct FldModelNodeView *next;
    struct FldModelNodeView *owner;
    struct FldModelNodeView *children;
    u8 pad10[4];
    u16 flags;
} FldModelNodeView;

typedef struct {
    u8 pad0[0xC];
    FldModelNodeView **entries;
} FldModelNodeListView;

typedef struct {
    FldModelNodeListView *list;
} FldModelView;

void func_00146150(FldModelView *model, s32 index, s32 clearFlag) {
    FldModelNodeView *parent = model->list->entries[index + 1];
    FldModelNodeView *first;
    FldModelNodeView *node;
    FldModelNodeView *next;
    FldModelNodeView *headNext;
    u16 flags;

    if (parent->children == NULL) {
        return;
    }
    parent->children->flags &= ~1;
    if (clearFlag == 0) {
        parent->children->flags |= 1;
    }

    first = parent->children;
    headNext = first->next;
    if (headNext == NULL) {
        return;
    }
    node = headNext;
    if (parent != node->owner || first == node) {
        return;
    }
    for (;;) {
        flags = node->flags & ~1;
        node->flags = flags;
        if (clearFlag == 0) {
            node->flags = flags | 1;
        }
        next = node->next;
        if (next == NULL) {
            return;
        }
        node = next;
        if (parent != node->owner) {
            return;
        }
        if (first != node) {
            continue;
        }
        return;
    }
}

s32 fldGetMaxItemValue(void) {
    s32 max = 0;
    FldSceneRecord *rec = (FldSceneRecord *)fldSceneRecords;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)fldSceneRecordCount; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (max < item->value) {
                max = item->value;
            }
        }
    }
    return max + 1;
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413700);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00146250);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148188);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148488);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148A98);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149A00);

void fldReleaseResourceSlots(void) {
    if (D_0044F7F0[1] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[1]);
    }
    if (D_0044F7F0[2] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[2]);
    }
    if (D_0044F7F0[3] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[3]);
    }
    if (D_0044F7F0[6] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[6]);
    }
    D_0044F7F0[1] = 0;
    D_0044F7F0[2] = 0;
    D_0044F7F0[3] = 0;
    D_0044F7F0[6] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    fldSceneReady = 0;
    frFontSetSharedRenderFlags(0x54);
}

extern void sdfReleaseDevSlot(SdfModel *, s32, s32);

void fldReleaseSceneDevSlotsAndTextures(void) {
    s32 i;

    for (i = 0; i < fldSceneRecordCount; i++) {
        if (D_003A5470[i] != 0) {
            sdfReleaseDevSlot(D_003A5470[i], 1, 1);
        }
    }
    for (i = 0; i < 96; i++) {
        D_003A5470[i] = 0;
        D_0044F818[i].unk0 = 0;
        D_0044F818[i].unk4 = 0;
        D_0044F818[i].unk8 = 0;
    }
    if (D_0044F7F0[5] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[5]);
    }
    if (D_0044F7F0[7] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[7]);
    }
    if (D_0044F7F0[8] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[8]);
    }
    if (D_0044F7F0[9] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[9]);
    }
    D_0044F7F0[5] = 0;
    D_0044F7F0[7] = 0;
    D_0044F7F0[8] = 0;
    D_0044F7F0[9] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    fldSceneReady = 0;
}

extern void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z);

typedef struct FldFogParams {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s32 unk10;
} FldFogParams;

extern FldFogParams D_0037FB30;

extern u128 D_0037FA20;

extern u128 D_0037FA30;

extern u128 D_0037FA40;

extern s32 D_0043624C;

extern s32 D_00436250;

extern s32 D_00436254;

extern s32 D_00436258;

extern s32 D_0043625C;

extern s32 D_00436264;

extern void fldClearSceneLifecycleFlags(s32);


extern FldVec4 D_00413788[]; /* default camera up vectors (3 copies), the first still read by asm func_00149A00 */

extern void fldApplySkyLightSetToPlayerVU(void);

extern void dds3SetWorldObjectDataValue(s32, s32);

extern s32 D_00436260;

extern s32 D_00436268;

/* Enters the field camera state for a fresh scene: releases the resource slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldAreaWork *cam = (FldAreaWork *)fldAreaState;
    f32 focus[4];
    f32 eye[4];
    FldVec4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;

    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseResourceSlots();
    fldSceneReady = 0;
    fldClearSceneLifecycleFlags(1);
    fldPreparePlayerSceneCameraTarget();
    evtSetSolarOverlayFullyVisible();
    fldApplySkyLightSetToPlayerVU();
    cam->unk70 = 4;
    frFontSetSharedRenderFlags(0x54);
    dds3SetWorldObjectDataValue(dds3GetWorldObject(), 1);
    D_00436268 = 0;
    D_00436248 = cam->room;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->room, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    memcpy(&up, &D_00413788[1], sizeof(up));
    D_00436260 = 0;
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_0037FA30, eye);
    PCP_COPY_VECTOR(&D_0037FA20, focus);
    PCP_COPY_VECTOR(&D_0037FA40, &up);
    D_0037FB30.unk0 = 255.0f;
    D_0037FB30.unk8 = 1000.0f;
    D_0037FB30.unk4 = 255.0f;
    D_0037FB30.unkC = 20000.0f;
    D_0037FB30.unk10 = 0x108010;
    D_0037F980.camera.offsetX = 2244.0f;
    D_0037F980.camera.offsetY = 2118.0f;
}



extern SdfTex *sdfTexAcquireResourceTexture(void *);

/* Loads the scene's models into the resource slots, then centers the camera on the
 * scene's entry point. */
void fldLoadSceneModelsAndCamera(void) {
    FldAreaWork *cam;
    FldSceneRecord *rec;
    f32 focus[4];
    f32 eye[4];
    FldVec4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 i;

    rec = (FldSceneRecord *)fldSceneRecords;
    for (i = 0; i < fldSceneRecordCount; i++, rec++) {
        D_003A5470[i] = sdfModelCreateWithAlternateItems(0, rec->model);
        ((FldPoint *)D_0044F818)[i].x = rec->pos->x;
        ((FldPoint *)D_0044F818)[i].y = rec->pos->y;
        ((FldPoint *)D_0044F818)[i].z = rec->pos->z;
    }
    cam = (FldAreaWork *)fldAreaState;
    D_0044F7F0[5] = sdfTexAcquireResourceTexture((void *)cam->mapResources[3].block);
    D_0044F7F0[7] = sdfTexAcquireResourceTexture((void *)cam->mapResources[5].block);
    D_0044F7F0[8] = sdfTexAcquireResourceTexture((void *)cam->mapResources[6].block);
    D_0044F7F0[9] = sdfTexAcquireResourceTexture((void *)cam->mapResources[7].block);
    D_00436268 = 0;
    D_00436248 = cam->room;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->room, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    memcpy(&up, &D_00413788[2], sizeof(up));
    D_00436260 = 0;
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_0037FA30, eye);
    PCP_COPY_VECTOR(&D_0037FA20, focus);
    PCP_COPY_VECTOR(&D_0037FA40, &up);
    D_0037FB30.unk0 = 255.0f;
    D_0037FB30.unk8 = 1000.0f;
    D_0037FB30.unk4 = 255.0f;
    D_0037FB30.unkC = 20000.0f;
    D_0037FB30.unk10 = 0x108010;
    D_0037F980.camera.offsetX = 2241.0f;
    D_0037F980.camera.offsetY = 2113.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseSceneDevSlotsAndTextures();
}

u32 fldGetSceneReadyFlag(void) {
    return fldSceneReady;
}

/* Handle automap input, clamp the grid position, and refresh its camera. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413788);

void func_0014A258(void) {
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 minX, maxZ, maxX, minZ;
    s32 closeRequested = 0;

    if (fldGetSceneReadyFlag() == 0) {
        return;
    }
    if (D_003899E0[0] != 0) {
        if ((s8)D_0037F510[1][0][2] != 0) {
            closeRequested = 1;
        }
    } else if ((s8)D_0037F510[1][0][2] < 0) {
        closeRequested = 1;
    }

    /* A close request does not advance the input-delay counter. */
    if (closeRequested == 0) {
        D_00436260++;
    }
    if (closeRequested != 0 ||
        ((s8)D_0037F510[1][0][3] < 0 && D_00436268 == 0)) {
        fldEnterSceneCamera();
        kwlnFadeStartOut(0);
        kwlnFadeStartIn(8);
        return;
    }
    if ((s8)D_0037F510[1][0][3] < 0) {
        sndSetSequenceVolumePan(10, 127, 63);
        D_00436250 = D_00436258;
        D_00436254 = D_0043625C;
        D_0043624C = D_00436264;
        D_00436268 = 0;
    }

    if (D_00436260 >= 11) {
        if ((s8)D_00435C48[0][1] != 0) {
            if (D_00435C48[0][1] & 2) {
                if ((s8)D_0037F510[1][0][5] != 0 &&
                    (s8)D_0037F510[1][0][7] != 0) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 -= 600;
                } else if ((s8)D_0037F510[1][0][5] != 0 &&
                           (s8)D_0037F510[1][0][6] != 0) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 += 600;
                } else if ((s8)D_0037F510[1][0][4] != 0 &&
                           (s8)D_0037F510[1][0][7] != 0) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 -= 600;
                } else if ((s8)D_0037F510[1][0][4] != 0 &&
                           (s8)D_0037F510[1][0][6] != 0) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 += 600;
                } else if ((s8)D_0037F510[1][0][5] != 0) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                } else if ((s8)D_0037F510[1][0][4] != 0) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                } else if ((s8)D_0037F510[1][0][7] != 0) {
                    D_00436268 = 1;
                    D_00436254 -= 600;
                } else if ((s8)D_0037F510[1][0][6] != 0) {
                    D_00436268 = 1;
                    D_00436254 += 600;
                }
            }
        } else {
            /* A held diagonal consumes the direction choice even when its
             * Z-repeat bit is absent; do not fall back to a cardinal step. */
            if ((s8)D_0037F510[1][0][5] != 0 &&
                (s8)D_0037F510[1][0][7] != 0) {
                if (D_0037F510[1][0][7] & 2) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 -= 600;
                }
            } else if ((s8)D_0037F510[1][0][5] != 0 &&
                       (s8)D_0037F510[1][0][6] != 0) {
                if (D_0037F510[1][0][6] & 2) {
                    D_00436268 = 1;
                    D_00436250 += 600;
                    D_00436254 += 600;
                }
            } else if ((s8)D_0037F510[1][0][4] != 0 &&
                       (s8)D_0037F510[1][0][7] != 0) {
                if (D_0037F510[1][0][7] & 2) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 -= 600;
                }
            } else if ((s8)D_0037F510[1][0][4] != 0 &&
                       (s8)D_0037F510[1][0][6] != 0) {
                if (D_0037F510[1][0][6] & 2) {
                    D_00436268 = 1;
                    D_00436250 -= 600;
                    D_00436254 += 600;
                }
            } else if (D_0037F510[1][0][5] & 2) {
                D_00436268 = 1;
                D_00436250 += 600;
            } else if (D_0037F510[1][0][4] & 2) {
                D_00436268 = 1;
                D_00436250 -= 600;
            } else if (D_0037F510[1][0][7] & 2) {
                D_00436268 = 1;
                D_00436254 -= 600;
            } else if (D_0037F510[1][0][6] & 2) {
                D_00436268 = 1;
                D_00436254 += 600;
            }
        }
    }

    fldGetVisibleSceneBounds(&minX, &maxZ, &maxX, &minZ);
    /* Quantize the two X endpoints, then the two Z endpoints. */
    minX = (s32)((minX - 600.0f) / 600.0f) * 600;
    maxX = (s32)((maxX + 600.0f) / 600.0f) * 600;
    maxZ = (s32)((maxZ - 600.0f) / 600.0f) * 600;
    minZ = (s32)((minZ + 600.0f) / 600.0f) * 600;

    if ((f32)D_00436250 < minX) {
        D_00436250 = (s32)minX;
    }
    if (maxX < (f32)D_00436250) {
        D_00436250 = (s32)maxX;
    }
    if ((f32)D_00436254 < minZ) {
        D_00436254 = (s32)minZ;
    }
    if (maxZ < (f32)D_00436254) {
        D_00436254 = (s32)maxZ;
    }

    focus[0] = D_00436250;
    focus[1] = 0.0f;
    focus[2] = D_00436254;
    eye[0] = D_00436250;
    eye[1] = -24000.0f;
    eye[2] = D_00436254;
    PCP_COPY_VECTOR(&D_0037FA30, eye);
    PCP_COPY_VECTOR(&D_0037FA20, focus);
    PCP_COPY_VECTOR(&D_0037FA40, up);
    D_0037FB30.unk0 = 255.0f;
    D_0037FB30.unk8 = 1000.0f;
    D_0037FB30.unk4 = 255.0f;
    D_0037FB30.unkC = 20000.0f;
    D_0037FB30.unk10 = 0x808080;
    PCP_COPY_VECTOR(sdfViewTargetVector, eye);
    PCP_COPY_VECTOR(sdfViewEyeVector, focus);
    PCP_COPY_VECTOR(sdfViewUpVector, up);
}

/* Re-centers the scene camera on the current scene's entry point. */
void fldCenterCameraOnEntry(void) {
    FldAreaWork *cam = (FldAreaWork *)fldAreaState;
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 entry[2];
    s32 ix;
    s32 iz;

    D_00436248 = cam->room;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->room, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_0037FA30, eye);
    PCP_COPY_VECTOR(&D_0037FA20, focus);
    PCP_COPY_VECTOR(&D_0037FA40, up);
    D_0037FB30.unk0 = 255.0f;
    D_0037FB30.unk8 = 1000.0f;
    D_0037FB30.unk4 = 255.0f;
    D_0037FB30.unkC = 20000.0f;
    D_0037FB30.unk10 = 0x808080;
}

/* Stores the area, room and third location selection, and selects the area's
 * flag table. The third selection's interpretation is left unknown. */
void fldSetSceneLocation(s32 stage, s32 room, s32 entrance) {
    FLD_WORK->area = stage;
    FLD_WORK->unk18 = entrance;
    D_00436248 = FLD_WORK->room = room;
    fldAreaFlagIndex = stage % 100;
}

extern s32 D_00387CE0[];

/* The area number's last two digits map to a save-table row; floor and bit are zero-based. */
s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];

    if (areaIndex == -1) {
        return 0;
    }
    return (datGameState->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetCurrentFloorFlag(s32 flagNumber) {
    s32 areaIndex;
    s32 bit;

    if (flagNumber > 0) {
        areaIndex = D_00387CE0[fldAreaFlagIndex % 100];
        if (areaIndex != -1) {
            bit = flagNumber - 1;
            fldAreaState[6] = bit;
            fldAreaState[47] = flagNumber;
            datGameState->areaFlags[areaIndex][fldAreaState[5]] |= 1ULL << bit;
            fldAreaState[48] = fldFindRecordItem(fldAreaState[5], bit);
        }
    }
}

/* The public setters take one-based floor and flag numbers. */
void fldSetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_00387CE0[area % 100];
    if (areaIndex != -1) {
        datGameState->areaFlags[areaIndex][floor] |= 1ULL << bit;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_00387CE0[area % 100];
    if (areaIndex != -1) {
        datGameState->areaFlags[areaIndex][floor] &= ~(1ULL << bit);
    }
}

void func_0014AC18(s32 index) {
    if (index >= 0x40) {
        D_00389834[0] = -1;
    } else {
        D_00389834[0] = index;
    }
}

void func_0014AC40(s32 index) {
    if (index >= 0x40) {
        D_00389838[0] = -1;
    } else {
        D_00389838[0] = index;
    }
}

s32 fldFindPreviousMarkedValue(s32 limit) {
    s32 group = fldSceneRecords;
    s32 best = -1;
    s32 groupIndex = 0;

    for (groupIndex = 0; groupIndex < (s32)fldSceneRecordCount; groupIndex++, group += 0x14) {
        u32 entryCount = ((FldSceneRecord *)group)->count;
        u32 entryIndex;

        for (entryIndex = 0; entryIndex < entryCount;) {
            s32 marked = fldGetFloorFlag(fldAreaFlagIndex, groupIndex, entryIndex);
            if (marked) {
                s32 value = fldFindRecordItem(groupIndex, entryIndex);
                if (value < limit && value > best) {
                    best = value;
                }
            }
            entryIndex++;
            entryCount = ((FldSceneRecord *)group)->count;
        }
    }
    if (best == -1) {
        return limit;
    }
    return best;
}

s32 fldFindNextMarkedValue(s32 limit) {
    s32 group = fldSceneRecords;
    s32 best = 999;
    s32 groupIndex = 0;

    for (groupIndex = 0; groupIndex < (s32)fldSceneRecordCount; groupIndex++, group += 0x14) {
        u32 entryCount = ((FldSceneRecord *)group)->count;
        u32 entryIndex;

        for (entryIndex = 0; entryIndex < entryCount;) {
            s32 marked = fldGetFloorFlag(fldAreaFlagIndex, groupIndex, entryIndex);
            if (marked) {
                s32 value = fldFindRecordItem(groupIndex, entryIndex);
                if (value > limit && value < best) {
                    best = value;
                }
            }
            entryIndex++;
            entryCount = ((FldSceneRecord *)group)->count;
        }
    }
    if (best == 999) {
        return limit;
    }
    return best;
}

void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z) {
    s32 i;
    s32 entry = fldSceneRecords;
    for (i = 0; i < (s32)fldSceneRecordCount; i++, entry += 0x14) {
        if (i == index) {
            FldPoint *position = ((FldSceneRecord *)entry)->pos;
            *x = position->x;
            *z = position->z;
            return;
        }
    }
    *x = 0.0f;
    *z = 0.0f;
}

extern s32 D_00436240;

void fldGetVisibleSceneBounds(f32 *minX, f32 *maxZ, f32 *maxX, f32 *minZ) {
    s32 room;
    u32 i;
    FldSceneRecord *rec;
    FldItem *item;
    FldPoint *a;
    s32 visible;
    FldPoint *b;

    room = 0;
    *minX = 0.0f;
    *maxZ = 0.0f;
    *maxX = 0.0f;
    *minZ = 0.0f;
    rec = (FldSceneRecord *)fldSceneRecords;
    for (; room < fldSceneRecordCount; room++, rec++) {
        item = rec->items;
        for (i = 0; i < rec->count; i++, item++) {
            visible = 0;
            if (fldGetFloorFlag(fldAreaFlagIndex, room, i) != 0) {
                visible = 1;
            }
            if (D_00436240 != 0) {
                visible = 1;
            }
            if (visible != 0) {
                a = item->pointA;
                if (a != NULL) {
                    if (*minX > a->x) {
                        *minX = a->x;
                    }
                    b = item->pointB;
                    if (*minX > b->x) {
                        *minX = b->x;
                    }
                    if (*maxX < a->x) {
                        *maxX = a->x;
                    }
                    if (*maxX < b->x) {
                        *maxX = b->x;
                    }
                    if (*maxZ < a->z) {
                        *maxZ = a->z;
                    }
                    if (*maxZ < b->z) {
                        *maxZ = b->z;
                    }
                    if (a->z < *minZ) {
                        *minZ = a->z;
                    }
                    if (b->z < *minZ) {
                        *minZ = b->z;
                    }
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", fldCheckSceneReady);

void func_0014B050(void) {
    if (fldSceneReady == 1) {
        func_00148488();
        func_00146250();
    }
}

void fldResetCameraAndSceneView(void) {
    fldCenterCameraOnEntry();
    func_00148A98();
}

void fldClearAllAreaFloorFlags(void) {
    u64 *words;
    s32 remaining;
    DatGameState *block;
    s32 blockIndex;

    blockIndex = 0;
    block = datGameState;
    do {
        words = block->areaFlags[blockIndex];
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        blockIndex = blockIndex + 1;
    } while (blockIndex < 10);
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B0F8);

extern s32 D_003A8E60[];

extern s32 func_001579C8();

extern char *D_00413860[4];

void fldLoadFieldEffectTextureSlots(void) {
    char *names[4];
    s32 i;

    memcpy(names, D_00413860, sizeof(names));
    for (i = 0; i < 4; i++) {
        fldEffectTextureLoadHandles[i] = sdfReadNamedResource(names[i], &fldEffectTextureData[i], 0);
        fldEffectTextureNodes[i] = func_001579C8(fldEffectTextureData[i]);
        D_003A8E60[i] = 0;
    }
}

void fldReleaseTextureSlots(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (fldEffectTextureNodes[i] != 0) {
            effDestroyNode(fldEffectTextureNodes[i]);
            fldEffectTextureNodes[i] = 0;
            sdfQueueNonzeroResourceId(fldEffectTextureLoadHandles[i]);
            fldEffectTextureLoadHandles[i] = 0;
            fldEffectTextureData[i] = 0;
        }
    }
}

extern u32 fldPlayerObject;
extern void effObjFetchInnerSecondVecNorm(EffWorldNode *object);
extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);
extern void func_00336538(f32 angle);

void func_0014B5D8(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 matrix[4][4] __attribute__((aligned(16)));
    f32 angle;
    s32 i;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    effObjFetchInnerSecondVecNorm((EffWorldNode *)fldPlayerObject);
    angle = effMiscComputeQuaternionRotatedReferenceAngle();
    for (i = 0; i < 4; i++) {
        if (fldEffectTextureNodes[i] != 0) {
            position[0] = FLD_WORK->x;
            position[1] = FLD_WORK->y;
            position[2] = FLD_WORK->z;
            if (i == 3) {
                position[1] -= 100.0f;
            }
            effCopyVectorToNodeInstance((EffNode *)fldEffectTextureNodes[i], position);
            func_00336538(-angle);
            VU0_STORE_MATRIX_UNCLOBBERED(matrix);
            effApplyNodeTransformMatrix((EffNode *)fldEffectTextureNodes[i], matrix);
            effUpdateNode(fldEffectTextureNodes[i]);
        }
    }
}

typedef struct FieldResourceIds {
    s32 entries[2];
} FieldResourceIds;

void fldLoadResourceByIndex(s32 index) {
    FieldResourceIds ids = *(FieldResourceIds *)D_00436330;
    fldIndexedResourceHandle = sdfReadNamedResource(ids.entries[index], &fldIndexedResourceData, 0);
    fldIndexedResourceEffect = effCreateNodeFromDescriptor(fldIndexedResourceData);
}

void fldReleaseIndexedResourceEffect(void) {
    if (fldIndexedResourceEffect != 0) {
        effDestroyNode(fldIndexedResourceEffect);
        fldIndexedResourceEffect = 0;
        sdfQueueNonzeroResourceId(fldIndexedResourceHandle);
        fldIndexedResourceHandle = 0;
        fldIndexedResourceData = 0;
    }
}

typedef struct {
    u8 unk00[0x14C];
    f32 position[3];
} FieldPlacementState;

void fldUpdateIndexedResourceEffectPosition(void) {
    f32 position[4];
    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    if (fldIndexedResourceEffect != 0) {
        FieldPlacementState *state = (FieldPlacementState *)fldAreaState;
        position[0] = state->position[0];
        position[1] = state->position[1];
        position[2] = state->position[2];
        effCopyVectorToNodeInstance((EffNode *)fldIndexedResourceEffect, position);
        effUpdateNode(fldIndexedResourceEffect);
    }
}

void mnuInitializeResourceEntries(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        mnuPositionedResourceNodes[i] = func_001579C8(D_0043633C);
        mnuPositionedResourceActive[i] = 0;
    }
}

extern s32 mnuPositionedResourceCursor;

void mnuSpawnResourceAtPosition(f32 x, f32 y, f32 z) {
    f32 pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 handle;

    handle = mnuPositionedResourceNodes[mnuPositionedResourceCursor];
    if (handle != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        effRestartNodeInstance(handle);
        effCopyVectorToNodeInstance((EffNode *)mnuPositionedResourceNodes[mnuPositionedResourceCursor], pos);
        mnuPositionedResourceActive[mnuPositionedResourceCursor] = 1;
        mnuPositionedResourceCursor = (mnuPositionedResourceCursor + 1) % 4;
    }
}

void fldQueuePrimaryEffectPosition(f32 x, f32 y, f32 z) {
    fldPrimaryEffectPositionPending = 1;
    fldPrimaryQueuedEffectPosition[0] = x;
    fldPrimaryQueuedEffectPosition[1] = y;
    fldPrimaryQueuedEffectPosition[2] = z;
}

void fldQueueSecondaryEffectPosition(f32 x, f32 y, f32 z) {
    fldSecondaryEffectPositionPending = 1;
    fldSecondaryQueuedEffectPosition[0] = x;
    fldSecondaryQueuedEffectPosition[1] = y;
    fldSecondaryQueuedEffectPosition[2] = z;
}

void mnuReleaseResourceEntries(void) {
    s32 i;
    fldPrimaryEffectPositionPending = 0;
    fldSecondaryEffectPositionPending = 0;
    for (i = 0; i < 4; i++) {
        if (mnuPositionedResourceNodes[i] != 0) {
            effDestroyNode(mnuPositionedResourceNodes[i]);
            mnuPositionedResourceNodes[i] = 0;
        }
    }
}

void fldUpdateMenuResourceEffects(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (mnuPositionedResourceNodes[i] != 0 && mnuPositionedResourceActive[i] != 0) {
            effUpdateNode(mnuPositionedResourceNodes[i]);
        }
    }
}

extern void *fileQueuePlainDispatchRequest(const char *path);

extern void func_002C81D0(void *);

extern void func_002C7CE8(void *);

extern s32 sdfMemoryGetBlockAddress(s32);

typedef struct FldLbNode {
    struct FldLbNode *next; /* 0x00 */
    u8 unk04[4];
    s32 value;              /* 0x08 */
} FldLbNode;

typedef struct FldLbFile {
    u8 unk00[0x60];
    FldLbNode *nodes; /* 0x60 */
} FldLbFile;

/* Retains field/automap TMX blocks and field-model resources. Unlike DDS1,
 * these map blocks come from separate files; fldmix.LB supplies other nodes. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413800);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413818);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413830);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413848);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413860);

void fldParseMixLb(void) {
    FldLbFile *lb;
    FldLbNode *node;
    u32 index;

    FLD_WORK->fieldTextures[0].unk0 = sdfReadNamedResource("/fld/f/bin/d2_fild1.tmx", &FLD_WORK->fieldTextures[0].block, 0);
    FLD_WORK->fieldTextures[1].unk0 = sdfReadNamedResource("/fld/f/bin/d2_fild2.tmx", &FLD_WORK->fieldTextures[1].block, 0);
    FLD_WORK->fieldTextures[2].unk0 = sdfReadNamedResource("/fld/f/bin/d2_fild3.tmx", &FLD_WORK->fieldTextures[2].block, 0);
    FLD_WORK->fieldTextures[3].unk0 = sdfReadNamedResource("/fld/f/bin/d2_fild4.tmx", &FLD_WORK->fieldTextures[3].block, 0);
    FLD_WORK->mapResources[0].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_1.tmx", &FLD_WORK->mapResources[0].block, 0);
    FLD_WORK->mapResources[1].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_2.tmx", &FLD_WORK->mapResources[1].block, 0);
    FLD_WORK->mapResources[2].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_3.tmx", &FLD_WORK->mapResources[2].block, 0);
    FLD_WORK->mapResources[3].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_5.tmx", &FLD_WORK->mapResources[3].block, 0);
    FLD_WORK->mapResources[4].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_6.tmx", &FLD_WORK->mapResources[4].block, 0);
    FLD_WORK->mapResources[5].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_7.tmx", &FLD_WORK->mapResources[5].block, 0);
    FLD_WORK->mapResources[6].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_8.tmx", &FLD_WORK->mapResources[6].block, 0);
    FLD_WORK->mapResources[7].unk0 = sdfReadNamedResource("/fld/f/bin/autmap_9.tmx", &FLD_WORK->mapResources[7].block, 0);
    index = 0;
    D_004362E0 = sdfReadNamedResource("/fld/f/bin/TOPEN.D3P", &D_004362E4, 0);
    D_00436304 = sdfReadNamedResource("/fld/f/bin/TAKARA2.D3P", &D_00436308, 0);
    D_004362EC = sdfReadNamedResource("/fld/f/bin/TOPEN2.D3P", &D_004362F0, 0);
    D_00436310 = sdfReadNamedResource("/fld/f/bin/TAKARA3.D3P", &D_00436314, 0);
    D_004362F8 = sdfReadNamedResource("/fld/f/bin/TOPEN3.D3P", &D_004362FC, 0);
    D_00436318 = sdfReadNamedResource("/fld/f/bin/TAKARA4.D3P", &D_0043631C, 0);
    D_00438EDC = sdfReadNamedResource("/fld/f/bin/DAMAGE_1.D3P", &D_00438EE0, 0);
    D_00438EE4 = sdfReadNamedResource("/fld/f/bin/DAMAGE_2.D3P", &D_00438EE8, 0);
    D_00438EEC = sdfReadNamedResource("/fld/f/bin/DAMAGE_3.D3P", &D_00438EF0, 0);
    lb = fileQueuePlainDispatchRequest("/fld/f/bin/fldmix.LB");
    func_002C81D0(lb);
    for (node = lb->nodes; node != NULL; node = node->next, index++) {
        switch (index) {
        case 5:
            D_00436338 = node->value;
            D_0043633C = sdfMemoryGetBlockAddress(node->value);
            break;
        case 6:
            D_004362D4 = node->value;
            D_004362D8 = sdfMemoryGetBlockAddress(node->value);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            break;
        }
    }
    func_002C7CE8(lb);
}

extern s32 D_00438EE0;

extern s32 D_00438EE8;

extern s32 D_00438EF0;

extern u32 D_004362D8;

extern EffNode *D_004362DC;

extern u32 fldAreaDamageEffect;

extern s32 fldAreaDamageEffectPlaced;

/* For regular field areas, creates menu effects and acquires texture references
 * from the four retained d2_fild TMX blocks. */
void fldInitializeMenuResources(void) {
    if (fldAreaState[4] < 200) {
        D_004362DC = effCreateNodeFromDescriptor(D_004362D8);
        if (fldAreaState[4] == 26) {
            fldAreaDamageEffect = effCreateNodeFromDescriptor(D_00438EE0);
        } else if (fldAreaState[4] == 29) {
            fldAreaDamageEffect = effCreateNodeFromDescriptor(D_00438EE8);
        } else if (fldAreaState[4] == 30) {
            fldAreaDamageEffect = effCreateNodeFromDescriptor(D_00438EF0);
        } else {
            fldAreaDamageEffect = 0;
        }
        fldAreaDamageEffectPlaced = 0;
        FLD_WORK->fieldTextures[0].texture = sdfTexAcquireResourceTexture((void *)FLD_WORK->fieldTextures[0].block);
        FLD_WORK->fieldTextures[1].texture = sdfTexAcquireResourceTexture((void *)FLD_WORK->fieldTextures[1].block);
        FLD_WORK->fieldTextures[2].texture = sdfTexAcquireResourceTexture((void *)FLD_WORK->fieldTextures[2].block);
        FLD_WORK->fieldTextures[3].texture = sdfTexAcquireResourceTexture((void *)FLD_WORK->fieldTextures[3].block);
    }
}

extern EffNode *D_004362DC;

extern EffNode *D_004362E8;

extern EffNode *D_004362F4;

extern u32 fldAreaDamageEffect;

/* Releases retained field texture references and menu effects. Each reference
 * is cleared immediately so repeated cleanup does not release it twice. */
void fldFreeSceneResources(void) {
    if (FLD_WORK->fieldTextures[0].texture != 0) {
        sdfTexReleaseReferenceViaHandler(FLD_WORK->fieldTextures[0].texture);
        FLD_WORK->fieldTextures[0].texture = 0;
    }
    if (FLD_WORK->fieldTextures[1].texture != 0) {
        sdfTexReleaseReferenceViaHandler(FLD_WORK->fieldTextures[1].texture);
        FLD_WORK->fieldTextures[1].texture = 0;
    }
    if (FLD_WORK->fieldTextures[2].texture != 0) {
        sdfTexReleaseReferenceViaHandler(FLD_WORK->fieldTextures[2].texture);
        FLD_WORK->fieldTextures[2].texture = 0;
    }
    if (FLD_WORK->fieldTextures[3].texture != 0) {
        sdfTexReleaseReferenceViaHandler(FLD_WORK->fieldTextures[3].texture);
        FLD_WORK->fieldTextures[3].texture = 0;
    }
    if (D_004362DC != 0) {
        effDestroyNode(D_004362DC);
        D_004362DC = 0;
    }
    if (D_004362E8 != 0) {
        effDestroyNode(D_004362E8);
        D_004362E8 = 0;
    }
    if (D_004362F4 != 0) {
        effDestroyNode(D_004362F4);
        D_004362F4 = 0;
    }
    if (fldAreaDamageEffect != 0) {
        effDestroyNode(fldAreaDamageEffect);
        fldAreaDamageEffect = 0;
    }
}

void fldClearMenuEntries(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        fldRoomEffectEntries[i].unk0 = 0;
        fldRoomEffectEntries[i].unk4 = 0;
        fldRoomEffectEntries[i].unk8 = 0;
        fldRoomEffectEntries[i].unkC = 0;
        fldRoomEffectEntries[i].object = 0;
        fldRoomEffectEntries[i].unk2C = 0;
        fldRoomEffectEntries[i].unk30 = 0;
        fldRoomEffectEntries[i].unk34 = 0;
        fldRoomEffectEntries[i].room = 0;
        fldRoomEffectEntries[i].unk3C = 0;
        fldRoomEffectEntries[i].unk3E = 0;
    }
    fldRoomEffectEntryCount = 0;
}

void fldResetObjectSlots(void) {
    s32 i;

    fldObjectSlotCount = 0;
    for (i = 0; i < 32; i++) {
        fldObjectSlots[i].id = -1;
        fldObjectSlots[i].unk0 = 0;
        fldObjectSlots[i].unk8 = 0;
        fldObjectSlots[i].activationRequested = 0;
        if (fldObjectSlots[i].effectNode != 0) {
            effDestroyNode(fldObjectSlots[i].effectNode);
        }
        fldObjectSlots[i].effectNode = 0;
    }
    D_00436300 = 0;
    D_0043630C = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BF98);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413AE8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413AF8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014C2B8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B38);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014C9B0);

void fldActivateObjectById(s32 id) {
    s32 i;

    for (i = 0; i < fldObjectSlotCount; i++) {
        if (fldObjectSlots[i].id == id && fldObjectSlots[i].unk8 == 0) {
            fldObjectSlots[i].activationRequested = 1;
        }
    }
}

void fldReleaseObjectSlots(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (fldObjectSlots[i].effectNode != 0) {
            effDestroyNode(fldObjectSlots[i].effectNode);
            fldObjectSlots[i].effectNode = 0;
        }
    }
    fldObjectSlotCount = 0;
    for (i = 0; i < 32; i++) {
        fldObjectSlots[i].id = -1;
        fldObjectSlots[i].unk0 = 0;
        fldObjectSlots[i].unk8 = 0;
        fldObjectSlots[i].activationRequested = 0;
    }
}

void func_0014D0E8(void) {
    s32 i;
    MdlCtx *resource;

    for (i = 0; i < fldObjectSlotCount; i++) {
        switch (fldObjectSlots[i].unk8) {
        case 0:
            if (fldObjectSlots[i].activationRequested == 1) {
                resource = (MdlCtx *)dds3GetObjectBaseResourceHandle(fldObjectSlots[i].unk0);
                mdlAddEntryPlain(resource, 0, 1);
                fldObjectSlots[i].unk8 = fldObjectSlots[i].activationRequested;
                switch (fldObjectSlots[i].effectVariant) {
                case 0:
                case 3:
                    if (D_004362E8 != 0) {
                        effDestroyNode(D_004362E8);
                        D_004362E8 = 0;
                    }
                    if (FLD_WORK->area == 29 || FLD_WORK->area == 30) {
                        D_004362E8 = effCreateNodeFromDescriptor(D_004362FC);
                        effRestartNodeInstance(D_004362DC);
                    } else {
                        D_004362E8 = effCreateNodeFromDescriptor(D_004362E4);
                        effRestartNodeInstance(D_004362DC);
                    }
                    break;
                case 1:
                    if (D_004362F4 != 0) {
                        effDestroyNode(D_004362F4);
                        D_004362F4 = 0;
                    }
                    D_004362F4 = effCreateNodeFromDescriptor(D_004362F0);
                    effRestartNodeInstance(D_004362DC);
                    break;
                }
            } else {
                resource = (MdlCtx *)dds3GetObjectBaseResourceHandle(fldObjectSlots[i].unk0);
                if (resource->first->state == 5) {
                    mdlAddEntryPlain(resource, 0, 0);
                }
            }
            break;
        case 1:
            resource = (MdlCtx *)dds3GetObjectBaseResourceHandle(fldObjectSlots[i].unk0);
            if (resource->first->state == 5) {
                mdlAddEntryFlagged(resource, 0, 2);
                fldObjectSlots[i].activationRequested = 2;
                fldObjectSlots[i].unk8 = 2;
            }
            break;
        case 2:
            if (fldTestSceneControlFlags(0x40)) {
                resource = (MdlCtx *)dds3GetObjectBaseResourceHandle(fldObjectSlots[i].unk0);
                mdlAddEntryFlagged(resource, 0, 3);
                fldObjectSlots[i].activationRequested = 3;
                fldObjectSlots[i].unk8 = 3;
                if (D_004362E8 != 0) {
                    effDestroyNode(D_004362E8);
                    D_004362E8 = 0;
                }
                if (D_004362F4 != 0) {
                    effDestroyNode(D_004362F4);
                    D_004362F4 = 0;
                }
            }
            break;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B78);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D380);

extern void *memset(void *, s32, u32);

extern s32 fldAreaDamageEffectPlaced;

s32 fldPlaceAreaDamageEffect(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = 1.0f;
    effRestartNodeInstance(fldAreaDamageEffect);
    effCopyVectorToNodeInstance((EffNode *)fldAreaDamageEffect, pos);
    fldAreaDamageEffectPlaced = 1;
    return 1;
}

extern void fldSelectDisplayBuffer(u32);
extern void func_0012BE18(s32);
extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 ptyAnyUnitFlagMatch(s32, s32);
extern f32 sdfSinPoly(f32);
extern void func_0012B690(s32, s32, s32, s32, s32, s32, s32, s32, u32,
                          u32, u32, u32, SdfTex *);
extern f32 D_0043634C;

/* Draws the field banner at (x, y), using field textures 0 and 2, unless either
 * field-state gate is 1. alpha is unused; the optional party mark animates. */
void fldDrawAnimatedFieldBanner(s32 alpha, s32 x, s32 y) {
    u32 color;

    if (FLD_WORK->unk118 != 1 && FLD_WORK->unk114 != 1) {
        fldSelectDisplayBuffer(0x53);
        func_0012BE18(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitSpriteRect(x + 0x140, y + 0x10, 0x12, 0x19,
                            0x26, 3, 0x12, 0x19, 0x80808080,
                            FLD_WORK->fieldTextures[0].texture);
        fldSubmitSpriteRect(x + 0x152, y + 0x10, 0x90, 0x19,
                            0x36, 3, 1, 0x19, 0x80808080,
                            FLD_WORK->fieldTextures[0].texture);
        fldSubmitSpriteRect(x + 0x1E2, y - 1, 0x20, 0x39,
                            1, 2, 0x20, 0x39, 0x80808080,
                            FLD_WORK->fieldTextures[0].texture);
        if (ptyAnyUnitFlagMatch(0x5D0, 0) != 0) {
            fldSubmitSpriteRect(x + 0x1B6, y + 0x2E, 0x23, 0xB,
                                0x3B, 0x25, 0x23, 0xB, 0x80808080,
                                FLD_WORK->fieldTextures[2].texture);
            fldSubmitSpriteRect(x + 0x1D8, y + 0x26, 0x1B, 0x21,
                                0x64, 2, 0x1B, 0x21, 0x80808080,
                                FLD_WORK->fieldTextures[2].texture);
            color = 0x808080;
            if (D_0043634C < 45.0f) {
                color = (s32)(sdfSinPoly(D_0043634C * 4.0f * 3.14f / 180.0f) *
                              128.0f) + 0x80;
                color |= (color << 8) | (color << 16);
            }
            func_0012BE18(1);
            func_0012B690(x + 0x1B6, y + 0x2E, 0x23, 0xB,
                          0x3B, 0x25, 0x23, 0xB,
                          color | 0x30000000, color | 0x30000000,
                          color | 0x30000000, color | 0x30000000,
                          FLD_WORK->fieldTextures[2].texture);
            func_0012B690(x + 0x1D8, y + 0x26, 0x1B, 0x21,
                          0x64, 2, 0x1B, 0x21,
                          color | 0x5A000000, color | 0x5A000000,
                          color | 0x5A000000, color | 0x5A000000,
                          FLD_WORK->fieldTextures[2].texture);
            D_0043634C += 1.0f;
            if (D_0043634C > 90.0f) {
                D_0043634C = 0.0f;
            }
        }
        func_0012BE18(0);
    }
}

extern s32 func_00150F10(void);
extern s16 *fldFindLocationCoordinateRecord(s32, s32);

void fldDrawLocationPanel(void) {
    u32 color;

    if (func_00150F10() != 0) {
        return;
    }
    if (FLD_WORK->unk118 == 1 || FLD_WORK->unk114 != 0) {
        return;
    }
    if (fldFindLocationCoordinateRecord(FLD_WORK->area, FLD_WORK->room + 1)[2] <= 0) {
        return;
    }
    switch (FLD_WORK->unkCC) {
    case 0:
    case 2:
    case 4:
    case 6:
        break;
    case 1:
    case 3:
    case 5:
    case 7:
        fldSelectDisplayBuffer(0x53);
        func_0012BE18(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        if (FLD_WORK->unk144 > 0) {
            color = ((u32)((20 - FLD_WORK->unk144) * 128 / 20) << 24) | 0x808080;
        } else {
            color = 0x80808080;
        }
        fldSubmitSpriteRect(0x191, 0x11C, 0x60, 0x10, 1, 2, 0x60, 0x10,
                            color, FLD_WORK->fieldTextures[2].texture);
        fldSubmitSpriteRect(0x191, 0x12C, 0x60, 0x70, 1, 0x11, 0x60, 1,
                            color, FLD_WORK->fieldTextures[2].texture);
        fldSubmitSpriteRect(0x191, 0x19C, 0x60, 0x10, 1, 0x14, 0x60, 0x10,
                            color, FLD_WORK->fieldTextures[2].texture);
        fldSubmitSpriteRect(0x1A4, 0x113, 0x3A, 0x11, 1, 0x2E, 0x3A, 0x11,
                            color, FLD_WORK->fieldTextures[3].texture);
        fldResetCameraAndSceneView();
        fldSelectDisplayBuffer(0x5B);
        func_0012BE18(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitSpriteRect(0x191, 0x11D, 0x1C, 0x16, 0x24, 0x22, 0x1C, 0x16,
                            color, FLD_WORK->fieldTextures[0].texture);
        fldSubmitSpriteRect(0x1C6, 0x191, 0x3A, 0x2C, 1, 1, 0x3A, 0x2C,
                            color, FLD_WORK->fieldTextures[3].texture);
        break;
    }
}

extern s32 fldTestSceneLifecycleFlags(u32);

void fldFlushQueuedEffectPositions(void) {
    if (fldAreaState[4] < 0xC8) {
        if (fldTestSceneLifecycleFlags(1) != 0) {
            return;
        }
        if (fldGetCampSceneControlMode() != 0) {
            return;
        }
        if (fldGetSceneReadyOrPendingState() != 0) {
            return;
        }
        if (fldSecondaryEffectPositionPending != 0) {
            if (FLD_WORK->unk12A == 0) {
                mnuSpawnResourceAtPosition(fldSecondaryQueuedEffectPosition[0], fldSecondaryQueuedEffectPosition[1], fldSecondaryQueuedEffectPosition[2]);
            }
            fldSecondaryEffectPositionPending = 0;
        }
        if (fldPrimaryEffectPositionPending != 0) {
            if (FLD_WORK->unk12A == 0) {
                mnuSpawnResourceAtPosition(fldPrimaryQueuedEffectPosition[0], fldPrimaryQueuedEffectPosition[1], fldPrimaryQueuedEffectPosition[2]);
            }
            fldPrimaryEffectPositionPending = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DEA8);

void fldFireRoomEffects(void) {
    s32 i;

    for (i = 0; i < fldRoomEffectEntryCount; i++) {
        s32 room = fldRoomEffectEntries[i].room;
        if (room != 0 && fldTestMapSlotAuxiliaryFlag(fldAreaState[4], fldAreaState[5] + 1, room) != 0) {
            if (fldRoomEffectEntries[i].object != 0) {
                dds3SetObjectFlags(fldRoomEffectEntries[i].object, 1);
            }
        }
    }
}

extern u32 sdfDevCreateCommandState(const char *);

extern u32 sdfDevQueueReadAndWait(u32, void *, u32);

extern void sdfDevWaitThenReleaseCommandState(u32);

typedef struct FldNpcPalette {
    u32 word[0x80];
} FldNpcPalette; /* 0x200 bytes */

extern FldNpcPalette D_0044FD90;

void fldLoadNpcPalette(s32 field) {
    char path[64];
    char directory[32];
    u32 command;

    if (field < 100) {
        fldFormatAreaDirectory(directory, field, 1);
        if (field == 23 && mdlFlagTest(25) != 0) {
            func_0035C860(path, "%sF033.NPL", directory);
        } else if (field == 24 && mdlFlagTest(25) != 0) {
            func_0035C860(path, "%sF034.NPL", directory);
        } else if (field == 27 && mdlFlagTest(25) != 0) {
            func_0035C860(path, "%sF037.NPL", directory);
        } else {
            func_0035C860(path, "%sF%03d.NPL", directory, field);
        }
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, &D_0044FD90, 0x200);
        sdfDevWaitThenReleaseCommandState(command);
    }
}

void fldSetNpcPalette(FldNpcPalette *src) {
    D_0044FD90 = *src;
}

extern s32 strcmp(const char *, const char *);

s32 fldFindEffectByName(char *name) {
    s32 i;

    for (i = 0; i < fldRoomEffectEntryCount; i++) {
        if (fldRoomEffectEntries[i].unk0 != 999 && strcmp(fldRoomEffectEntries[i].name, name) == 0) {
            return fldRoomEffectEntries[i].object[1];
        }
    }
    return 0;
}

s32 fldGetCurrentSceneSelectionResource(void) {
    s32 index = fldGetCurrentSceneSelectionId();
    if (index >= 0) {
        return D_0044FFB0[index].resource[1];
    }
    return 0;
}

void func_0014E668(u32 value) {
    D_00436364 = value;
}

s32 fldTitleIsActive(void) {
    return kwlnTaskGetTaskByName(fldTitleTaskName) != 0;
}

void func_0014E698(void) {
    D_0043635C = 0;
    D_00436354 = 1;
}

/* Render the field title through delay, fade-in, hold and fade-out phases. */
s32 fldTitleTaskUpdate(void) {
    s32 alpha;
    u32 color;
    s32 nextFrame;

    if (D_00436364 == 0) {
        fldSelectDisplayBuffer(0x53);
    } else {
        fldSelectDisplayBuffer(0x5E);
    }
    fldSubmitFrameQuad(1, 5, 0x80, 3, 0, 0, 1, 1);
    func_0012BE18(0);
    switch (D_00436354) {
    case 0:
        if (D_0043635C > 0) {
            D_0043635C--;
        }
        if (D_0043635C == 0) {
            D_00436354 = 1;
            D_00436350 = 0;
        }
        return 0;
    case 1:
        if (D_00436350 >= 20) {
            alpha = (D_00436350 - 19) * 128 / 15;
            if (alpha > 128) {
                alpha = 128;
            }
            color = (alpha << 24) | 0x808080;
            fldSubmitSpriteRect(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color, D_00436368);
            if (D_00389780[0] == 12) {
                fldSubmitSpriteRect(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color, D_0043636C);
            }
        }
        if (D_00436350 >= 20 && D_00436350 < 30) {
            func_0012BE18(1);
            alpha = (D_00436350 - 20) * 128 / 10;
            if (alpha > 128) {
                alpha = 128;
            }
            if (alpha < 0) {
                alpha = 0;
            }
            color = alpha << 24;
            fldSubmitSpriteRect(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color | 0x807060, D_00436368);
            if (D_00389780[0] == 12) {
                fldSubmitSpriteRect(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color | 0x808080, D_0043636C);
            }
            func_0012BE18(0);
        }
        if (D_00436350 >= 30) {
            func_0012BE18(1);
            alpha = (48 - D_00436350) * 128 / 18;
            if (alpha > 128) {
                alpha = 128;
            }
            if (alpha < 0) {
                alpha = 0;
            }
            color = alpha << 24;
            fldSubmitSpriteRect(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color | 0x807060, D_00436368);
            if (D_00389780[0] == 12) {
                fldSubmitSpriteRect(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color | 0x808080, D_0043636C);
            }
            func_0012BE18(0);
        }
        nextFrame = D_00436350 + 1;
        if (nextFrame > 60) {
            D_00436350 = 0;
            D_00436354++;
        } else {
            D_00436350 = nextFrame;
        }
        break;
    case 2:
        fldSubmitSpriteRect(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, 0x80808080, D_00436368);
        if (D_00389780[0] == 12) {
            fldSubmitSpriteRect(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, 0x80808080, D_0043636C);
        }
        nextFrame = D_00436350 + 1;
        if (nextFrame > 60) {
            D_00436350 = 0;
            D_00436354++;
        } else {
            D_00436350 = nextFrame;
        }
        break;
    case 3:
        alpha = (20 - D_00436350) * 128 / 20;
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha > 128) {
            alpha = 128;
        }
        color = (alpha << 24) | 0x808080;
        fldSubmitSpriteRect(0xF3, 0x134, 0x100, 0x40, 0, 0, 0x100, 0x40, color, D_00436368);
        if (D_00389780[0] == 12) {
            fldSubmitSpriteRect(0xC5, 0x149, 0x65, 0x1A, 1, 1, 0x65, 0x1A, color, D_0043636C);
        }
        nextFrame = D_00436350 + 1;
        if (nextFrame > D_00436360) {
            D_00436350 = 0;
            D_00436354++;
        } else {
            D_00436350 = nextFrame;
        }
        break;
    default:
        return -1;
    }
    return 0;
}

void fldReleaseTitleTextures(void) {
    if (D_00436368 != 0) {
        sdfTexReleaseReferenceViaHandler(D_00436368);
        D_00436368 = 0;
    }
    if (D_0043636C != 0) {
        sdfTexReleaseReferenceViaHandler(D_0043636C);
        D_0043636C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", fldTitleTaskName);

void fldStartTitle(s32 field, s32 arg1, s32 arg2) {
    char path[32];
    s32 resourceAddress;
    s32 handle;

    D_00436350 = 0;
    D_00436354 = 0;
    D_00436358 = field;
    D_0043635C = arg1;
    D_00436360 = arg2;
    D_00436364 = 0;
    if ((u32)(field - 8) < 2) {
        func_0035C860(path, "/fld/f/pnl/df%03d_a.tmx", field);
    } else if (field == 0xC) {
        func_0035C860(path, "/fld/f/pnl/df%03d_b.tmx", 0xC);
    } else {
        func_0035C860(path, "/fld/f/pnl/df%03d.tmx", field);
    }
    handle = sdfReadNamedResource(path, &resourceAddress, 0);
    D_00436368 = sdfTexAcquireResourceTexture((void *)resourceAddress);
    sdfReleaseResourceAllocation(handle);
    if (field == 0xC) {
        func_0035C860(path, "/fld/f/pnl/df_b.tmx");
        handle = sdfReadNamedResource(path, &resourceAddress, 0);
        D_0043636C = sdfTexAcquireResourceTexture((void *)resourceAddress);
        sdfReleaseResourceAllocation(handle);
    }
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(fldTitleTaskName, 0x2B0A, 0, 1, fldTitleTaskUpdate, fldReleaseTitleTextures, 0);
    }
}

void fldDestroyTitleTask(void) {
    if (fldTitleIsActive()) {
        kwlnTaskDestroyWithHierarchyByName(fldTitleTaskName, 1);
    }
}

s32 fldTitleMiniIsActive(void) {
    return kwlnTaskGetTaskByName(D_00413C80) != 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EDB8);

void fldReleaseTitleMiniTexture(void) {
    if (D_0043637C != 0) {
        sdfTexReleaseReferenceViaHandler(D_0043637C);
        D_0043637C = 0;
    }
}

extern s32 D_003898B4[];

extern s32 D_00436378;

extern s32 func_0014EDB8(void);

extern void fldReleaseTitleMiniTexture(void);

extern void sdfReleaseResourceAllocation(s32);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C80);

void fldStartMiniTitleForUnlock(s32 id) {
    char path[0x20];
    s32 data;
    s32 handle;

    switch (id) {
    case 4:
        if (mdlFlagTest(0x420) == 0) {
            return;
        }
        break;
    case 5:
        if (mdlFlagTest(0x424) == 0) {
            return;
        }
        break;
    case 7:
    case 8:
    case 9:
    case 11:
    case 13:
    case 14:
    case 15:
    case 30:
        break;
    case 10:
        if (mdlFlagTest(0x421) == 0) {
            return;
        }
        break;
    case 12:
        if (mdlFlagTest(0x422) == 0) {
            return;
        }
        break;
    case 21:
        if (mdlFlagTest(0x440) == 0) {
            return;
        }
        break;
    case 22:
        if (mdlFlagTest(0x480) == 0) {
            return;
        }
        break;
    case 23:
        if (mdlFlagTest(0x4C0) == 0) {
            return;
        }
        break;
    case 24:
        if (mdlFlagTest(0x500) == 0) {
            return;
        }
        break;
    case 25:
        if (mdlFlagTest(0x540) == 0) {
            return;
        }
        break;
    case 26:
        if (mdlFlagTest(0x580) == 0) {
            return;
        }
        break;
    case 27:
        if (mdlFlagTest(0x5C0) == 0) {
            return;
        }
        break;
    case 28:
        if (mdlFlagTest(0x600) == 0) {
            return;
        }
        break;
    case 29:
        if (mdlFlagTest(0x640) == 0) {
            return;
        }
        break;
    case 31:
        if (mdlFlagTest(0x6E0) == 0) {
            return;
        }
        break;
    case 32:
        if (mdlFlagTest(0x6E1) == 0) {
            return;
        }
        break;
    case 33:
        if (mdlFlagTest(0x6E2) == 0) {
            return;
        }
        break;
    case 34:
        if (mdlFlagTest(0x6E3) == 0) {
            return;
        }
        break;
    case 35:
        if (mdlFlagTest(0x6E4) == 0) {
            return;
        }
        break;
    case 38:
        if (mdlFlagTest(0x6E5) == 0) {
            return;
        }
        break;
    default:
        return;
    }
    D_003898B4[0] = 20;
    D_00436378 = id;
    D_00436370 = 0;
    D_00436374 = 0;
    func_0035C860(path, "/fld/f/pnl/ds_%03d.tmx", id);
    handle = sdfReadNamedResource(path, &data, 0);
    D_0043637C = sdfTexAcquireResourceTexture((void *)data);
    sdfReleaseResourceAllocation(handle);
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(D_00413C80, 0x2B0A, 0, 1, func_0014EDB8, fldReleaseTitleMiniTexture, 0);
    }
}

void fldRequestMiniTitleDismiss(void) {
    if (D_00436374 == 0) {
        D_00436370 = 0;
        D_00436374 = 1;
    }
}

void fldLoadWeatherEffects(void) {
    s32 data;
    s32 handle;

    handle = sdfReadNamedResource("/fld/f/bin/d2_hunt1.tmx", &data, 0);
    D_004363AC = sdfTexAcquireResourceTexture((void *)data);
    sdfQueueNonzeroResourceId(handle);
    handle = sdfReadNamedResource("/fld/f/bin/d2_hunt2.tmx", &data, 0);
    D_004363B0 = sdfTexAcquireResourceTexture((void *)data);
    sdfQueueNonzeroResourceId(handle);
    handle = sdfReadNamedResource("/fld/f/bin/d2_hunt3.tmx", &data, 0);
    D_004363B4 = sdfTexAcquireResourceTexture((void *)data);
    sdfQueueNonzeroResourceId(handle);
    D_00436380 = sdfReadNamedResource("/fld/f/bin/FH_DAM_2.EPL", &D_00436384, 0);
    fldDamEffectNode = func_001579C8(D_00436384);
    fldDamEffectPositioned = 0;
    D_00436390 = sdfReadNamedResource("/fld/f/bin/YUK_2.EPL", &D_00436394, 0);
    fldYukEffectNode = func_001579C8(D_00436394);
    fldYukEffectPositioned = 0;
}

void fldReleaseWeatherEffects(void) {
    if (D_004363AC != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363AC);
        D_004363AC = 0;
    }
    if (D_004363B0 != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363B0);
        D_004363B0 = 0;
    }
    if (D_004363B4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363B4);
        D_004363B4 = 0;
    }
    effDestroyNode(fldDamEffectNode);
    fldDamEffectNode = 0;
    fldDamEffectPositioned = 0;
    sdfQueueNonzeroResourceId(D_00436380);
    D_00436380 = 0;
    D_00436384 = 0;
    effDestroyNode(fldYukEffectNode);
    fldYukEffectNode = 0;
    fldYukEffectPositioned = 0;
    sdfQueueNonzeroResourceId(D_00436390);
    D_00436390 = 0;
    D_00436394 = 0;
}

void fldSetWeatherEffectPos(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (fldDamEffectNode != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        effRestartNodeInstance(fldDamEffectNode);
        effCopyVectorToNodeInstance((EffNode *)fldDamEffectNode, pos);
        fldDamEffectPositioned = 1;
    }
    if (fldYukEffectNode != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        effRestartNodeInstance(fldYukEffectNode);
        effCopyVectorToNodeInstance((EffNode *)fldYukEffectNode, pos);
        fldYukEffectPositioned = 1;
    }
}

void fldUpdateWeatherEffectNodes(void) {
    if (fldDamEffectNode != 0 && fldDamEffectPositioned != 0) {
        effUpdateNode(fldDamEffectNode);
    }
    if (fldYukEffectNode != 0 && fldYukEffectPositioned != 0) {
        effUpdateNode(fldYukEffectNode);
    }
}

typedef struct {
    s16 category;
    s16 id;
    s16 unk04;
    u8 unk06[2];
    s16 unk08;
    u8 unk0A[0xD6];
} FieldResourceRecord;

extern FieldResourceRecord *D_00435E18;

s32 fldFindResourceRecordIndex(s32 category, s32 id) {
    FieldResourceRecord *record = D_00435E18;
    s32 index;
    for (index = 0; index < 8; index++, record++) {
        if (record->category == category && record->id == id) {
            return index;
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DB8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DC8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F5F0);

void fldClearObjectEntryHandles(void) {
    FldEnt14 *entry = fldSparkObjectEntries;
    s32 i = 0x10;

    do {
        s32 objectHandle = entry->objectHandle;

        i--;
        if (objectHandle != 0) {
            ddsReleaseUnitObject((EffWorldNode *)objectHandle);
            entry->objectHandle = 0;
        }
        entry++;
    } while (i >= 0);
}

void fldInitSparkTable(void) {
    s32 i;

    if (D_00389874[0] == 0) {
        for (i = 0; i < 64; i++) {
            fldSparkSlots[i].hasVectors = 0;
            fldSparkSlots[i].active = 0;
            fldSparkSlots[i].unk28 = 0;
            fldSparkSlots[i].objectSlot = -1;
            fldSparkSlots[i].unk2E = effMiscRand(0) % 60 + 15;
        }
    } else {
        for (i = 0; i < 64; i++) {
            fldSparkSlots[i].hasVectors = 0;
            fldSparkSlots[i].objectSlot = -1;
        }
    }
}

void fldResetSparkTable(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        fldSparkSlots[i].active = 0;
        fldSparkSlots[i].unk28 = 0;
        fldSparkSlots[i].objectSlot = -1;
        fldSparkSlots[i].unk2E = effMiscRand(0) % 60 + 15;
    }
}

s32 fldSetSparkVectors(s32 index, const u128 *pos, const u128 *vel) {
    PCP_COPY_VECTOR(fldSparkSlots[index].pos, pos);
    PCP_COPY_VECTOR(fldSparkSlots[index].vel, vel);
    fldSparkSlots[index].hasVectors = 1;
    return 1;
}

s32 func_0014F980(s32 index, s32 reserved) {
    s32 slot;
    s32 i;
    s32 candidate;
    MdlCtx *model;
    u128 position;

    if (reserved == 0) {
        slot = -1;
        for (i = 0; i < 16; i++) {
            candidate = (fldSparkControlState[16] + i) % 16;
            if (fldSparkObjectEntries[candidate].unk4 == -1) {
                slot = candidate;
                break;
            }
        }
    } else {
        slot = 16;
    }
    if (slot < 0) {
        return 0;
    }
    fldSparkControlState[16] = (slot + 1) % 16;
    if (fldSparkSlots[index].hasVectors == 1) {
        PCP_COPY_VECTOR(&position, fldSparkSlots[index].pos);
        effObjSetInnerFirstVec((EffWorldNode *)fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].pos);
        effObjSetInnerSecondVec((EffWorldNode *)fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].vel);
        model = (MdlCtx *)dds3GetObjectBaseResourceHandle((void *)fldSparkObjectEntries[slot].objectHandle);
        model->flags &= ~MDL_SKIP_TRANSFORMS;
        fldSparkSlots[index].objectSlot = slot;
        fldSparkObjectEntries[slot].unk4 = index;
        return 1;
    }
    return 0;
}

void fldFreeSparkSlot(s32 index) {
    FldVec4 vec;
    s32 obj;
    s32 *flags;
    s16 slot;

    vec = D_00413DE8;
    if (fldSparkSlots[index].hasVectors == 1 && fldSparkSlots[index].active != 0 && fldSparkSlots[index].objectSlot != -1) {
        vec.v[0] = fldSparkSlots[index].pos[0];
        vec.v[2] = fldSparkSlots[index].pos[2];
        effObjSetInnerFirstVec((EffWorldNode *)fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle, (u128 *)&vec);
        obj = fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle;
        flags = (s32 *)dds3GetObjectBaseResourceHandle((void *)obj);
        *flags |= 1;
        dds3ClearObjectFlags(obj, 0x400);
        slot = fldSparkSlots[index].objectSlot;
        fldSparkSlots[index].objectSlot = -1;
        fldSparkObjectEntries[slot].unk4 = -1;
    }
}

void fldUpdateSparkSlots(void) {
    s32 i;

    func_0014F5F0();
    for (i = 0; i < 64 && i < fldSparkControlState[13]; i++) {
        if (fldSparkSlots[i].hasVectors != 0 && fldSparkSlots[i].active != 0) {
            func_0014F980(i, fldSparkSlots[i].unk28);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014FD28);

s32 fldIsNearSpark(f32 x, f32 y, f32 z) {
    s32 i;

    for (i = 0; i < 64 && i < fldSparkControlState[13]; i++) {
        if (fldSparkSlots[i].active == 1 && fldSparkSlots[i].objectSlot != -1) {
            f32 dx = x - fldSparkSlots[i].pos[0];
            f32 dy = y - fldSparkSlots[i].pos[1];
            f32 dz = z - fldSparkSlots[i].pos[2];

            if (fsqrtf(dx * dx + dy * dy + dz * dz) < 50.0f) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150138);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150800);


extern void func_00125B10(void);

/* Restores field presentation once an active event finishes, then clears the
 * event latch. An already-cleared latch leaves field resources untouched. */
void fldFinishEventFieldState(void) {
    FldAreaWork *work = (FldAreaWork *)fldAreaState;

    if (work->eventActive != 0) {
        D_003899E0[0] = 0;
        func_00125B10();
        fldClearObjectEntryHandles();
        fldReleaseWeatherEffects();
        fldStartSceneBgmAlternate();
        fldPreparePlayerSceneCameraTarget();
        work->eventActive = 0;
        fldSparkControlState[3] = 0;
        fldSparkControlState[4] = 0;
        evtSetSolarOverlayFullyVisible();
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DE8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DF8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413E48);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413E98);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150A60);

extern s32 D_00451BBC[];

s32 func_00150F10(void) {
    return D_00451BBC[0];
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150F20);

extern s32 D_00451B98[];

void func_001512D8(void) {
    D_00451B98[0] = -1;
}


extern void fldReleaseCameraModel(s32);

/* Releases scene presentation and camera resources before clearing the event
 * latch and installing the existing field-state values. */
void fldResetEventSceneState(void) {
    fldReleaseCameraModel(0);
    D_003899E0[0] = 0;
    FLD_WORK->unk118 = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldPreparePlayerSceneCameraTarget();
    FLD_WORK->unk114 = 1;
    FLD_WORK->eventActive = 0;
    D_00451B9C[0] = 0;
    FLD_WORK->unk138 = 1;
}

/* Restores title/menu and field presentation after an event, then clears the
 * event latch and field gates while retaining the existing reset values. */
void fldResetAfterEvent(void) {
    func_00341C78(0x680017);
    mnuAdvanceTitleStateUnderSemaphore();
    D_003899E0[0] = 0;
    FLD_WORK->unk118 = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldStartSceneBgmAlternate();
    FLD_WORK->unk114 = 0;
    FLD_WORK->eventActive = 0;
    D_00451B9C[0] = 0;
    FLD_WORK->unk138 = 1;
    evtSetSolarOverlayFullyVisible();
}

void fldStartDeferredFieldExit(void) {
    evtCreateMessageWindowIfMissing(D_003A9EB0);
    dspStartEntry(4);
    fldResetPlayerSceneObjectState();
    D_00389884[0] = 2;
}

/* Completes deferred exit state 2 only after the message window stops being
 * controlled, then closes its display channel and restores the camera target. */
void fldFinishDeferredExit(void) {
    if (FLD_WORK->unk114 == 2) {
        func_0026C900();
        if (!evtGetMessageWindowControlState()) {
            evtFinishMessageWindowAndNotify();
            dspCloseChannel();
            fldPreparePlayerSceneCameraTarget();
            FLD_WORK->unk114 = 0;
        }
    }
}

s32 fldIsEventPhaseAtLeastTwo(void) {
    if (D_00451B9C[0] < 2) {
        return 0;
    }
    return 1;
}

void fldTickWeatherEffectNodes(void) {
    fldUpdateWeatherEffectNodes();
}

/* Reads the signed halfword at +6 of the current entry. The record's layout
 * and the meaning of this value are not established elsewhere in this unit. */
s32 func_00151498(void) {
    return *(s16 *)((u8 *)D_00451B94[0] + 6);
}

s32 func_001514A8(void) {
    return D_00451B98[0];
}

extern s16 D_00389876[];

s16 func_001514B8(void) {
    return D_00389876[0];
}

typedef struct FieldGuidePoint {
    u8 x;
    u8 y;
    u16 bearing;
} FieldGuidePoint;

typedef struct FieldGridCoordPair {
    s16 x;
    s16 y;
} FieldGridCoordPair;

typedef struct FieldTargetGuideState {
    s32 mode;
    s32 disabled;
    f32 position[3];
    f32 yaw;
    f32 unk18; /* 0x18: previous model frame, synchronized when update bit 2 is set. */
    f32 unk1C; /* 0x1C: current model frame. */
    s16 gridX;
    s16 gridY;
    s16 targetGridX;
    s16 targetGridY;
    s16 unk28;
    s16 unk2A;
    struct FieldCoordinateRecord *routeRecord;
    FieldGuidePoint *route;
    FieldGridCoordPair *cycleRoute;
    s32 routeIndex;
    s32 routeEnd;
    s32 moveTimer;
    f32 targetYaw;
    f32 stepDistance;
    s32 age;
    s32 updateFlags;
    s32 unk54;
    f32 targetDistance; /* 0x58 */
    f32 targetBearing;  /* 0x5C */
    f32 viewAngleError; /* 0x60 */
    s32 unk64;
    s16 previousGridX;
    s16 previousGridY;
    s32 unk6C;
    s32 cycleIndex;
} FieldTargetGuideState;

extern FieldTargetGuideState fldTargetGuideState;

void fldResetViewState(void) {
    fldTargetGuideState.unk6C = -1;
    fldTargetGuideState.unk54 = 0;
    fldTargetGuideState.updateFlags = 0;
    fldTargetGuideState.unk64 = 0;
    fldTargetGuideState.mode = 0;
    fldTargetGuideState.cycleIndex = 0;
    fldTargetGuideState.cycleRoute = NULL;
    fldTargetGuideState.disabled = 0;
}

void func_001514F8(void) {
    typedef struct {
        u32 firstOffset;
        u32 unk04;
        u32 secondOffset;
    } FieldCoordinateResource;
    char directory[0x20];
    char path[0x40];
    FieldCoordinateResource *resource;
    u32 base;
    u32 first;
    u32 second;
    u32 third;

    fldFormatAreaDirectory(directory, fldAreaState[4], fldAreaState[5] + 1);
    func_0035C860(path, D_00413F20, directory, fldAreaState[4], fldAreaState[5] + 1);
    D_004363C4 = sdfReadNamedResource(path, &resource, 0);
    base = (u32)resource;
    first = base + resource->firstOffset;
    second = base + resource->secondOffset;
    third = base + *(u32 *)(second + 4);
    D_004363C8 = base;
    D_004363CC = first;
    D_004363D0 = second;
    D_004363D4 = third;
}


void fldReleaseTargetGuideResource(void) {
    if (D_004363C4 != 0) {
        sdfQueueNonzeroResourceId(D_004363C4);
        D_004363C4 = 0;
        D_004363C8 = 0;
        D_004363CC = 0;
        D_004363D0 = 0;
        D_004363D4 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001515E0);

extern s16 D_004363BC;

extern s16 D_004363BE;

extern s16 D_004363C0;

extern s16 D_004363C2;

void fldMapGridToScreenPosition(s16 x, s16 y, f32 *outX, f32 *outY) {
    f32 scaleX = (f32)D_004363C0;
    f32 originX = (f32)D_004363BC;
    f32 originY = (f32)D_004363BE;
    f32 sourceX = (f32)x;
    f32 sourceY = (f32)y;
    *outX = originX + scaleX * sourceX;
    *outY = originY + (f32)(-D_004363C2) * sourceY;
}

typedef struct FieldCoordinateRecord {
    u8 x, y, z, w;
    u32 flags; /* 0x04 */
    u8 pad08[8];
} FieldCoordinateRecord;

typedef struct {
    s32 unk00;
    u32 count;
} FieldCoordinateList;

FieldCoordinateRecord *fldFindCoordinateRecord(s16 x, s16 y, s16 z, s16 w) {
    FieldCoordinateList *list = (FieldCoordinateList *)D_004363C8;
    FieldCoordinateRecord *record = (FieldCoordinateRecord *)D_004363CC;
    u32 index;
    for (index = 0; index < list->count; index++, record++) {
        if (record->x == (u8)x && record->y == (u8)y &&
            record->z == (u8)z && record->w == (u8)w) {
            return record;
        }
    }
    return NULL;
}

void fldCalcTargetDistanceYaw(f32 *distance, f32 *angle) {
    f32 *player = (f32 *)fldAreaState;
    FieldTargetGuideState *target = &fldTargetGuideState;
    f32 dx = player[0x14C / 4] - target->position[0];
    f32 dz = player[0x154 / 4] - target->position[2];
    f32 dist = fsqrtf(dx * dx + dz * dz);
    f32 yaw = 0.0f;
    if (!(dist < 1.0f)) {
        yaw = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    *distance = dist;
    *angle = yaw;
}

f32 fldApproachTargetAngleWithMinimumStep(f32 cur, f32 target, f32 speed, f32 minStep) {
    f32 delta = fldAngleDifference(cur, target);
    f32 step = delta / speed;

    if (step < 0.0f) {
        step = -step;
    }
    if (step < minStep) {
        step = minStep;
    }
    if ((delta >= 0.0f && delta <= step) || (delta <= 0.0f && -step <= delta)) {
        delta = target;
    } else if (delta < 0.0f) {
        delta = cur - step;
    } else {
        delta = cur + step;
    }
    return delta;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001519E8);

void func_00151DD8(void) {
}

void fldTickTargetGuideCounter(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}

extern void func_001519E8(s32);
extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern FieldGridCoordPair D_00438EF8;

void fldUpdateTargetGuideFastRoute(void) {
    f32 distance, angle;
    FieldTargetGuideState *state = &fldTargetGuideState;
    FieldGuidePoint *point;
    f32 originX = D_004363BC;
    f32 originY = D_004363BE;

    if (state->age < 300) {
        state->age++;
    }
    if (state->moveTimer <= 0 || --state->moveTimer <= 0) {
        if (state->routeIndex < state->routeEnd) {
            state->routeIndex++;
            state->moveTimer = 20;
        }
        point = state->route;
        if (point != NULL) {
            point += state->routeIndex;
            state->gridX = point->x;
            state->gridY = point->y;
            state->position[0] = originX + (f32)point->x * (f32)D_004363C0;
            state->position[2] = originY + (f32)point->y * (f32)-D_004363C2;
        }
        if (D_00438EF8.x == state->gridX && D_00438EF8.y == state->gridY) {
            state->routeRecord = NULL;
            state->route = NULL;
            state->routeIndex = 0;
            state->routeEnd = 0;
            state->moveTimer = 20;
            state->stepDistance = 0.0f;
        } else {
            func_001519E8(3);
        }
    } else {
        state->position[0] += sdfSinPoly(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->position[2] += sdfEvaluateCosineViaSinePhaseShift(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->yaw = fldApproachTargetAngleWithMinimumStep(state->yaw, 180.0f - state->targetYaw, 10.0f, 1.0f);
    }
    fldCalcTargetDistanceYaw(&distance, &angle);
}

void fldUpdateTargetGuideSlowRoute(void) {
    f32 distance, angle;
    FieldTargetGuideState *state = &fldTargetGuideState;
    FieldGuidePoint *point;
    f32 originX = D_004363BC;
    f32 originY = D_004363BE;

    if (state->age < 300) {
        state->age++;
    }
    if (state->moveTimer <= 0 || --state->moveTimer <= 0) {
        if (state->routeIndex < state->routeEnd) {
            state->routeIndex++;
            state->moveTimer = 80;
        }
        point = state->route;
        if (point != NULL) {
            point += state->routeIndex;
            state->gridX = point->x;
            state->gridY = point->y;
            state->position[0] = originX + (f32)point->x * (f32)D_004363C0;
            state->position[2] = originY + (f32)point->y * (f32)-D_004363C2;
        }
        if (D_00438EF8.x == state->gridX && D_00438EF8.y == state->gridY) {
            state->routeRecord = NULL;
            state->route = NULL;
            state->routeIndex = 0;
            state->routeEnd = 0;
            state->moveTimer = 20;
            state->stepDistance = 0.0f;
        } else {
            func_001519E8(2);
        }
    } else {
        state->position[0] += sdfSinPoly(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->position[2] += sdfEvaluateCosineViaSinePhaseShift(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->yaw = fldApproachTargetAngleWithMinimumStep(state->yaw, 180.0f - state->targetYaw, 15.0f, 1.0f);
    }
    fldCalcTargetDistanceYaw(&distance, &angle);
}


void fldTickTargetGuideAndNotify(void) {
    s32 count = fldTargetGuideState.moveTimer;
    s32 remaining = count - 1;
    if (count > 0) {
        fldTargetGuideState.moveTimer = remaining;
        count = remaining;
    }
    if (count == 0) {
        func_001519E8(1);
    }
}


void fldUpdateTargetGuideCycleMotion(void) {
    f32 distance, angle;
    FieldTargetGuideState *state = &fldTargetGuideState;

    if (state->age < 300) {
        state->age++;
    }
    if (state->moveTimer <= 0 || --state->moveTimer <= 0) {
        if (state->routeIndex < state->routeEnd) {
            state->routeIndex++;
            state->moveTimer = 80;
        }
        if (state->cycleRoute[state->cycleIndex + 1].x == -1) {
            state->cycleIndex = 0;
        } else {
            state->cycleIndex++;
        }
        func_001519E8(5);
    } else {
        state->position[0] += sdfSinPoly(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->position[2] += sdfEvaluateCosineViaSinePhaseShift(state->targetYaw * 3.14f / 180.0f) * state->stepDistance;
        state->yaw = fldApproachTargetAngleWithMinimumStep(state->yaw, 180.0f - state->targetYaw, 15.0f, 1.0f);
    }
    fldCalcTargetDistanceYaw(&distance, &angle);
}

void func_00152390(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}

void func_001523B0(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}

void func_001523D0(void) {
    s32 count = fldTargetGuideState.moveTimer;
    if (count > 0) {
        fldTargetGuideState.moveTimer = count - 1;
    }
}


void func_001523F0(void) {
    FieldTargetGuideState *state = &fldTargetGuideState;
    FldVec4 guidePosition;
    FldVec4 areaPosition;
    f32 dx;
    f32 dz;
    f32 magnitude;
    f32 viewAngle;
    f32 guideX;
    f32 guideZ;
    f32 areaX;
    f32 areaZ;

    state->updateFlags = 0;
    fldCalcTargetDistanceYaw(&state->targetDistance, &state->targetBearing);

    dx = sdfViewEyeVector[0] - sdfViewTargetVector[0];
    dz = sdfViewEyeVector[2] - sdfViewTargetVector[2];
    magnitude = fsqrtf(dx * dx + dz * dz);
    viewAngle = 0.0f;
    if (!(magnitude < 1.0f)) {
        viewAngle = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    magnitude = fldAngleDifference(180.0f - viewAngle, state->yaw);
    if (magnitude < 0.0f) {
        magnitude = -magnitude;
    }
    guideX = state->position[0];
    guideZ = state->position[2];
    areaX = FLD_WORK->x;
    areaZ = FLD_WORK->z;
    state->viewAngleError = magnitude;

    guidePosition.v[0] = guideX;
    guidePosition.v[1] = 0.0f;
    guidePosition.v[2] = guideZ;
    guidePosition.v[3] = 1.0f;
    areaPosition.v[0] = areaX;
    areaPosition.v[1] = 0.0f;
    areaPosition.v[2] = areaZ;
    areaPosition.v[3] = 1.0f;

    if (state->mode == 0) {
        return;
    }
    if (state->mode == 1) {
        if (state->moveTimer > 0) {
            return;
        }
        if (state->gridX != D_00438EF8.x || state->gridY != D_00438EF8.y) {
            func_001519E8(2);
        }
    }
    state = &fldTargetGuideState;
    if (state->mode == 4) {
        if (state->moveTimer > 0) {
            return;
        }
    }
    if (state->mode == 3) {
        if (state->gridX == D_00438EF8.x && state->gridY == D_00438EF8.y) {
            func_001519E8(4);
            state->updateFlags |= 0x80;
            return;
        }
    }
    state = &fldTargetGuideState;
    if (state->mode == 2) {
        if (state->routeRecord != NULL && (state->routeRecord->flags & 1) != 0) {
            func_001519E8(3);
            state->updateFlags |= 0x80;
        }
        return;
    } else {
        return;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F20);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001525F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001526B8);

extern void fldReleaseTargetGuideResource(void);

extern void fldResetViewState(void);

extern void func_00341C78(u32);

void fldResetTargetViewAndSound(void) {
    fldReleaseTargetGuideResource();
    fldResetViewState();
    func_00341C78(0x690061);
}

extern void fldCalcTargetDistanceYaw(f32 *, f32 *);

void fldUpdateViewAngle(void) {
    f32 distance;
    f32 angle;
    FieldTargetGuideState *state;

    fldCalcTargetDistanceYaw(&distance, &angle);
    state = &fldTargetGuideState;
    state->targetYaw = angle;
    state->yaw = 180.0f - angle;
}

extern const f32 D_00413F68[4];
extern f32 D_00451D4C[];
extern void effObjSetNodeFlags(ObjectTransform *inner, u32 flags);
extern void effObjClearNodeFlags(ObjectTransform *inner, u32 flags);
extern void mdlAddEntryFlaggedEx(MdlCtx *ctx, s32 searchId, s32 motionIndex,
                                 f32 blendLeadFrames, f32 blendDurationFrames);

void func_00152C88(void) {
    f32 position[4] __attribute__((aligned(16)));
    f32 smoothedPosition[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    MdlCtx *model = D_00435F20;
    s32 animation;
    s32 changeAnimation;
    f32 angle;
    f32 previousFrame;
    f32 currentFrame;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(smoothedPosition, 0, sizeof(smoothedPosition));
    smoothedPosition[3] = 1.0f;
    memcpy(axis, D_00413F68, sizeof(axis));
    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;

    position[0] = fldTargetGuideState.position[0];
    position[1] = 0.0f;
    position[2] = fldTargetGuideState.position[2];
    effObjSetInnerFirstVec(D_00435F1C, (u128 *)position);

    VU0_LOAD_VF(vf10, axis);
    angle = (D_00451D4C[0] * 3.14f) / 180.0f;
    effMiscAxisAngleToQuaternionVU(angle);
    /* SDK store: the quaternion is consumed by the following vector setter. */
    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
    effObjSetInnerSecondVec(D_00435F1C, (u128 *)rotation);

    if (fldTargetGuideState.updateFlags & 0x40) {
        effObjSetNodeFlags(D_00435F1C->inner, 8);
        PCP_COPY_VECTOR_F32(smoothedPosition, D_00435F1C->inner->smoothedPosition);
        if (fabsf(smoothedPosition[0] - position[0]) < 1.0f) {
            D_00435F1C->inner->smoothedPosition[0] = position[0];
        } else {
            D_00435F1C->inner->smoothedPosition[0] =
                smoothedPosition[0] + (position[0] - smoothedPosition[0]) / 3.0f;
        }
        if (fabsf(smoothedPosition[1] - position[1]) < 1.0f) {
            D_00435F1C->inner->smoothedPosition[1] = position[1];
        } else {
            D_00435F1C->inner->smoothedPosition[1] =
                smoothedPosition[1] + (position[1] - smoothedPosition[1]) / 3.0f;
        }
        if (fabsf(smoothedPosition[2] - position[2]) < 1.0f) {
            D_00435F1C->inner->smoothedPosition[2] = position[2];
        } else {
            D_00435F1C->inner->smoothedPosition[2] =
                smoothedPosition[2] + (position[2] - smoothedPosition[2]) / 3.0f;
        }
        D_00435F1C->inner->smoothedPosition[3] = 1.0f;
    } else {
        effObjClearNodeFlags(D_00435F1C->inner, 8);
    }

    if (fldTargetGuideState.updateFlags & 0x20) {
        mdlAddEntryFlagged(model, 0, fldTargetGuideState.unk54);
    } else if (fldTargetGuideState.updateFlags & 2) {
        animation = fldTargetGuideState.unk54;
        changeAnimation = 1;
        /* Only zero and the 16..18 family suppress a repeated animation. */
        if (animation == 0) {
            goto compare_animation;
        }
        if (animation < 0) {
            goto apply_animation;
        }
        if (animation >= 19) {
            goto apply_animation;
        }
        if (animation < 16) {
            goto apply_animation;
        }
compare_animation:
        changeAnimation = animation != (s32)model->current.h.arg;
apply_animation:
        if (changeAnimation) {
            model->first->frameStep = 1.0f;
            mdlAddEntryFlaggedEx(model, 0, fldTargetGuideState.unk54, 6.0f, 6.0f);
        }
    }

    previousFrame = fldTargetGuideState.unk1C;
    currentFrame = model->first->currentFrame;
    fldTargetGuideState.unk1C = currentFrame;
    fldTargetGuideState.unk18 = previousFrame;
    if (fldTargetGuideState.updateFlags & 2) {
        fldTargetGuideState.unk18 = currentFrame;
    }
}

static inline s32 scaleToVolume(s32 dist, s32 max, s32 range) {
    return (max - dist) * 127 / range;
}

s32 fldCalcDistanceVolume(f32 x, f32 y, f32 z) {
    f32 dx = fldLookAtFarPoint[0] - x;
    f32 dy = fldLookAtFarPoint[1] - y;
    f32 dz = fldLookAtFarPoint[2] - z;
    f32 dist = fsqrtf(dx * dx + dy * dy + dz * dz) - 600.0f;
    s32 volume;
    if (4800.0f < dist) {
        dist = 4800.0f;
    }
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    volume = scaleToVolume((s32)dist, 4800, 4800);
    if (volume > 127) {
        volume = 127;
    }
    if (volume < 0) {
        volume = 0;
    }
    return volume;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153068);

extern char D_00413F78[];

extern s32 D_00451D3C[];

s32 fldReportCampVolumeError(void) {
    if (D_00451D3C[0] != 1) {
        if (fldGetCampSceneControlMode() != 0) {
            func_0035B6E0(D_00413F78);
        }
    }
}

void func_00153410(void) {
    if (fldTargetGuideState.disabled != 1) {
        if (FLD_WORK->targetGuideActive == 0 && fldTestSceneControlFlags(0x40) == 0) {
            if (fldTargetGuideState.unk64 == 14) {
                func_00153068();
                return;
            }
            if (fldGetSceneReadyOrPendingState() != 0) {
                func_0035B6E0(D_00413F78);
                return;
            }
        } else {
            func_001542D8();
            func_001523F0();
            func_001525F0();
            func_00152C88();
            func_00153068();
            func_001515E0(FLD_WORK->x, FLD_WORK->z, &D_00438EF8.x, &D_00438EF8.y);
            fldTargetGuideState.previousGridX = fldTargetGuideState.gridX;
            fldTargetGuideState.previousGridY = fldTargetGuideState.gridY;
        }
    }
}

s32 fldIsTargetWithinInteractionRange(void) {
    f32 distance;
    f32 angle;
    s32 inRange = 1;
    fldCalcTargetDistanceYaw(&distance, &angle);
    if (!(distance < 200.0f)) {
        inRange = 0;
    }
    return inRange;
}

s32 func_00153560(s32 unusedArea, s32 unusedRoom) {
    if (fldAreaState[4] == 23) {
        if (fldAreaState[5] == 10 && mdlFlagTest(1254)) {
            return 0;
        }
        if (fldAreaState[5] == 13 && mdlFlagTest(1255)) {
            return 0;
        }
        if (fldAreaState[5] == 15 && mdlFlagTest(9)) {
            return 0;
        }
        if (fldAreaState[5] == 17 && !mdlFlagTest(1232)) {
            return 0;
        }
        if (fldAreaState[5] == 17 && mdlFlagTest(9)) {
            return 0;
        }
    }
    if (mdlFlagTest(1222) && fldAreaState[4] == 23 &&
        fldAreaState[5] == 10) {
        return 1;
    }
    if (fldAreaState[4] == 23) {
        if (fldAreaState[5] == 13) {
            return 1;
        }
        if (fldAreaState[5] == 15) {
            return 1;
        }
        if (fldAreaState[5] == 17) {
            return 1;
        }
    }
    return 0;
}

extern FieldGridCoordPair D_003AA6A8[10];
extern FieldGridCoordPair D_003AA6D0[10];
extern FieldGridCoordPair D_003AA6F8[10];

/* Select the target guide's route endpoint, motion state and model visibility. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F78);

void func_001536B8(s32 mode) {
    s32 previousMode;

    if (func_00153560(FLD_WORK->area, FLD_WORK->room + 1) == 0) {
        return;
    }
    previousMode = fldTargetGuideState.unk64;
    fldTargetGuideState.updateFlags = 0x40;
    fldTargetGuideState.unk64 = mode;

    switch (mode) {
    case 0:
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(2);
        func_00152C88();
        func_00341C78(0x690061);
        sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        break;
    case 1:
        fldUpdateViewAngle();
        fldTargetGuideState.cycleIndex = 0;
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(5);
        func_00152C88();
        break;
    case 2:
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 0.0f;
        fldTargetGuideState.yaw = 180.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(6);
        func_00152C88();
        break;
    case 3:
        fldTargetGuideState.gridX = D_003AA6A8[1].x;
        fldTargetGuideState.gridY = D_003AA6A8[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(2);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        func_00341C78(0x690061);
        sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        break;
    case 4:
        fldTargetGuideState.gridX = D_003AA6A8[3].x;
        fldTargetGuideState.gridY = D_003AA6A8[3].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 90.0f;
        fldTargetGuideState.yaw = 180.0f - fldTargetGuideState.targetYaw;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(2);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        func_00341C78(0x690061);
        sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        break;
    case 5:
        fldTargetGuideState.gridX = D_003AA6A8[5].x;
        fldTargetGuideState.gridY = D_003AA6A8[5].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(2);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        func_00341C78(0x690061);
        sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        break;
    case 6:
        fldTargetGuideState.gridX = D_003AA6D0[1].x;
        fldTargetGuideState.gridY = D_003AA6D0[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(2);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        D_00435F20->flags &= ~1;
        func_00341C78(0x690061);
        sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        break;
    case 7:
        fldTargetGuideState.gridX = D_003AA6D0[3].x;
        fldTargetGuideState.gridY = D_003AA6D0[3].y;
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(6);
        func_00152C88();
        func_00341C78(0x690061);
        break;
    case 8:
        if (previousMode == 7) {
            fldTargetGuideState.gridX = D_003AA6D0[3].x;
            fldTargetGuideState.gridY = D_003AA6D0[3].y;
            fldTargetGuideState.targetYaw = 180.0f;
            fldTargetGuideState.yaw = 0.0f;
            fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
            func_001519E8(3);
            fldTargetGuideState.updateFlags |= 2;
            func_00152C88();
            func_00341C78(0x690061);
            sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        }
        break;
    case 9:
        fldTargetGuideState.gridX = D_003AA6F8[1].x;
        fldTargetGuideState.gridY = D_003AA6F8[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(2);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        D_00435F20->flags &= ~1;
        func_00341C78(0x690061);
        sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        break;
    case 10:
        fldTargetGuideState.gridX = D_003AA6F8[3].x;
        fldTargetGuideState.gridY = D_003AA6F8[3].y;
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(6);
        func_00152C88();
        func_00341C78(0x690061);
        break;
    case 11:
        if (previousMode == 10) {
            fldTargetGuideState.gridX = D_003AA6F8[3].x;
            fldTargetGuideState.gridY = D_003AA6F8[3].y;
            fldTargetGuideState.yaw = 180.0f;
            fldTargetGuideState.targetYaw = 0.0f;
            fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
            func_001519E8(3);
            fldTargetGuideState.updateFlags |= 2;
            func_00152C88();
            func_00341C78(0x690061);
            sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        }
        break;
    case 12:
        fldTargetGuideState.gridX = D_003AA6D0[1].x;
        fldTargetGuideState.gridY = D_003AA6D0[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(7);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        D_00435F20->flags |= 1;
        func_00341C78(0x690061);
        break;
    case 13:
        fldTargetGuideState.gridX = D_003AA6F8[1].x;
        fldTargetGuideState.gridY = D_003AA6F8[1].y;
        fldUpdateViewAngle();
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.targetYaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(7);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        D_00435F20->flags |= 1;
        func_00341C78(0x690061);
        break;
    case 14:
        fldTargetGuideState.gridX = 0;
        fldTargetGuideState.gridY = 0;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 0.0f;
        fldTargetGuideState.yaw = 180.0f;
        fldTargetGuideState.position[0] = fldTargetGuideState.targetYaw;
        fldTargetGuideState.position[2] = fldTargetGuideState.targetYaw;
        func_001519E8(8);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        D_00435F20->flags |= 1;
        break;
    case 15:
        fldTargetGuideState.gridX = D_003AA6F8[5].x;
        fldTargetGuideState.gridY = D_003AA6F8[5].y;
        fldUpdateViewAngle();
        fldTargetGuideState.targetYaw = 180.0f;
        fldTargetGuideState.yaw = 0.0f;
        fldMapGridToScreenPosition(fldTargetGuideState.gridX, fldTargetGuideState.gridY, &fldTargetGuideState.position[0], &fldTargetGuideState.position[2]);
        func_001519E8(2);
        fldTargetGuideState.updateFlags |= 2;
        func_00152C88();
        func_00341C78(0x690061);
        sndSetSequenceVolumePan(0x690061, 0, 0x3F);
        break;
    }
}


void fldSetTargetGuideEnabled(s32 active) {
    if (active == 0) {
        D_00451D3C[0] = 1;
        return;
    }
    D_00451D3C[0] = 0;
}

/* Five-word rows are shared by the object producer and the guide update. */
typedef struct FieldGuideObjectSlot {
    EffWorldNode *object;
    MdlCtx *model;
    u32 active;
    s16 gridX;
    s16 gridY;
    f32 progress;
} FieldGuideObjectSlot;

typedef char FieldGuideObjectSlot_size_must_be_0x14[
    (sizeof(FieldGuideObjectSlot) == 0x14) ? 1 : -1];

extern FieldGuideObjectSlot D_00451DB0[15];

void func_00153D60(s32 gridX, s32 gridY, s32 slot) {
    f32 position[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    f32 stepX;
    f32 stepY;
    s16 stepScaleX;
    s16 stepScaleY;
    EffWorldNode *object;
    MdlCtx *model;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;
    {
        f32 scale[4] __attribute__((aligned(16))) = {1.2f, 1.2f, 1.2f, 1.0f};
        stepX = (f32)D_004363BC;
        stepY = (f32)D_004363BE;
        object = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), position,
                                        rotation);
        dds3SetWorldNodeValue(object, (u32)"TUKAMI_UNIT");
        dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), object);
        func_00112058(object, 2, 0x10);
        effObjSetInnerFloat(object, 180.0f);

        stepScaleX = D_004363C0;
        stepScaleY = D_004363C2;
        stepX += (f32)stepScaleX * (f32)gridX;
        stepY += (f32)-stepScaleY * (f32)gridY;
        position[0] = stepX;
        position[1] = 0.0f;
        position[2] = stepY;
        scale[0] = 1.0f;
        scale[1] = 0.5f;
        scale[2] = 1.0f;
        PCP_COPY_VECTOR_F32(object->inner->position, position);
        PCP_COPY_VECTOR_F32(object->inner->smoothedPosition, position);
        PCP_COPY_VECTOR_F32(object->inner->rotation, rotation);
        effObjSetInnerFirstVec(object, (u128 *)position);
        effObjSetInnerSecondVec(object, (u128 *)rotation);
        effObjSetInnerThirdVec(object, (u128 *)scale);

        model = (MdlCtx *)dds3GetObjectBaseResourceHandle(object);
        mdlAddEntryFlagged(model, 0, 0);
        sdfSetTextFloatPairOverride(model->inner, 15.0f, 0.0f);
        D_00451DB0[slot].model = model;
        D_00451DB0[slot].object = object;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153FA0);

void func_001540E8(void) {
    f32 position[4] __attribute__((aligned(16))) = {0, 0, 0, 1.0f};
    f32 rotation[4] __attribute__((aligned(16))) = {0, 0, 0, 1.0f};
    f32 axis[4] __attribute__((aligned(16))) = {0, 1.0f, 0, 1.0f};
    f32 scale[4] __attribute__((aligned(16))) = {1.2f, 1.2f, 1.2f, 1.0f};
    ObjectTransform *inner;
    EffWorldNode *object;

    VU0_LOAD_VF(vf10, axis);
    effMiscAxisAngleToQuaternionVU(3.14159265f);
    VU0_LOAD_VF(vf11, rotation);
    effMiscQuatMultiplyVU();
    /* The SDK store is followed by opaque consumers, with no scalar readback. */
    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
    D_00435F1C = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), position, rotation);
    dds3SetWorldNodeValue(D_00435F1C, (u32)"OIKAKE_UNIT");
    dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), D_00435F1C);
    func_00112058(D_00435F1C, 1, 0x103);
    effObjSetInnerFloat(D_00435F1C, 180.0f);
    dds3SetObjectFlags(D_00435F1C, 0x400);
    position[0] = 800.0f;
    position[1] = 0.0f;
    position[2] = 800.0f;
    object = D_00435F1C;
    inner = object->inner;
    PCP_COPY_VECTOR_F32(inner->position, position);
    PCP_COPY_VECTOR_F32(inner->smoothedPosition, position);
    PCP_COPY_VECTOR_F32(inner->rotation, rotation);
    effObjSetInnerFirstVec(object, (u128 *)position);
    effObjSetInnerSecondVec(D_00435F1C, (u128 *)rotation);
    effObjSetInnerThirdVec(D_00435F1C, (u128 *)scale);
    D_00435F20 = (MdlCtx *)dds3GetObjectBaseResourceHandle(D_00435F1C);
    mdlAddEntryFlagged(D_00435F20, 0, 0x11);
    sdfSetTextFloatPairOverride(D_00435F20->inner, 15.0f, 0.0f);
    func_00136718();
    func_001526B8();
    func_00153FA0();
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001542D8);

extern s32 D_003898AC[];

extern s32 D_004363DC;

s32 func_00154528(void) {
    if (D_003898AC[0] != 0) {
        return D_004363DC;
    }
    return 0;
}

extern f32 D_004363E0;

extern f32 D_004363E4;

void fldGetFieldFloatPair(f32 *x, f32 *y) {
    *x = D_004363E0;
    *y = D_004363E4;
}
INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436208);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldCurrentBgmHandle);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneSoundBase);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldPendingSoundCount);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436220);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436224);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436228);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneBgmArchiveTrack);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneBgmArchivePhase);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldArchiveLoadPending);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldFixedArchiveLoadPhase);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043623C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436240);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAreaFlagIndex);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436248);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043624C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436250);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436254);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436258);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043625C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436260);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436264);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436268);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneReady);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneRecords);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneRecordCount);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSceneRecordResource);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043627C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436280);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436284);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436288);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043628C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436290);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436294);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436298);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043629C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldRoomEffectEntryCount);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAreaDamageEffect);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldAreaDamageEffectPlaced);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362EC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362FC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436300);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436304);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436308);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043630C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436310);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436314);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436318);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043631C);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldObjectSlotCount);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldIndexedResourceHandle);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldIndexedResourceData);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldIndexedResourceEffect);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436330);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436338);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043633C);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldPrimaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldSecondaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001442D0", mnuPositionedResourceCursor);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043634C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436350);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436354);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436358);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043635C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436360);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436364);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436368);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043636C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436370);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436374);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436378);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043637C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436380);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436384);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldDamEffectNode);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldDamEffectPositioned);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436390);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436394);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldYukEffectNode);

INCLUDE_SDATA(const s32, "game/code_001442D0", fldYukEffectPositioned);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BE);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C2);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363CC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E4);

