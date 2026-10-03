#include "common.h"
#include "pcp_vu0.h"
#include "fpu.h"

extern f32 fldAngleDifference(f32, f32);

/* Field work area (fldAreaState) fields reached through a pointer. */
typedef struct FldWorkView {
    u8 unk00[0x14];
    s32 stage;            /* 0x14 */
    u8 unk18[0x18];
    f32 focusPos[3];      /* 0x30 */
    u8 unk3C[0x14];
    s32 focusActive;      /* 0x50 */
    u8 unk54[0x10];
    f32 negatedAngle;     /* 0x64 */
    u8 unk68[8];
    s32 unk70;            /* 0x70 */
    u8 unk74[0x4C];
    s32 unkC0;            /* 0xC0 */
    u8 unkC4[0x40];
    s16 eventActive;      /* 0x104 */
    u8 unk106[0x24];
    s16 unk12A;           /* 0x12A */
    u8 unk12C[0x20];
    f32 x;                /* 0x14C */
    f32 y;
    f32 z;
    u8 unk158[0x18];
    f32 angle;            /* 0x170 */
    u8 unk174[0x4C];
    struct {
        s32 value;
        s32 block;
    } fldmix[5];          /* 0x1C0 */
} FldWorkView;

typedef struct FldSlot0C {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} FldSlot0C; /* 0xC bytes */

extern s32 D_003A5470[];

extern FldSlot0C D_0044F818[];

extern s32 D_004362D4;

extern u32 D_004362D8;

extern s32 D_004362E0;

extern s32 D_004362E4;

extern s32 D_004362EC;

extern s32 D_004362F0;

extern s32 D_004362F8;

extern s32 D_004362FC;

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

/* DDS2 saves store the area flag table earlier than the DDS1 save layout. */
typedef struct FldAreaFlagsView {
    u8 pad00[0xFCD0];
    u64 areaFlags[13][64];
} FldAreaFlagsView;

extern s32 D_004363C4;

extern void mnuAdvanceTitleStateUnderSemaphore(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern s32 fldRoomEffectEntryCount;

extern u32 D_00436330[];

extern s32 effCreateNodeFromDescriptor(u32);

extern s32 fldObjectSlotCount;

extern s32 fldSceneSoundBase;

extern f32 sdfAtan2(f32, f32);

extern f32 fldLookAtFarPoint[];

extern s32 D_003899D8[];

extern s32 D_00436228;

extern s32 D_0039A0B8[];

extern s32 D_0039A028[];

extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 sdfReadNamedResource(char *, void *, s32);

extern u32 D_0044F7F0[];

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

extern s32 D_004363AC;

extern s32 D_004363B0;

extern s32 D_004363B4;

extern u32 D_00436370;

extern s32 D_00436374;

extern s32 D_00436368;

extern s32 D_0043636C;

extern u32 fldIndexedResourceHandle;

extern u32 fldIndexedResourceData;

extern s32 fldIndexedResourceEffect;

extern s32 datGameState;

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

extern u32 D_00436354;

extern u32 D_0043635C;

extern s32 D_00436350;

extern s32 D_00436358;

extern s32 D_00436360;

extern void sdfReleaseResourceAllocation(s32);

extern void func_0014E6A8();

extern s32 D_0043637C;

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

extern void func_00341C78();

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

extern s32 func_00101740(void *name);

extern char D_00413C80[]; /* "fldTitleMini" */

extern void effUpdateNode(u32 arg0);

extern void ddsReleaseUnitObject(s32 arg0);

typedef struct {
    s32 objectHandle;
    s32 unk4;
    u8 pad8[0xC];
} FldEnt14; /* 0x14 bytes */

extern FldEnt14 fldSparkObjectEntries[];

extern s32 D_00389884[];

extern u8 D_003A9EB0[];

extern void evtCreateMessageWindowIfMissing(void *arg0);

extern void dspStartEntry(s32 arg0);

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
    s32 model;         /* 0xC */
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
    u8 pad10[0x14];
    s32 effectNode;
    u8 pad28[8];
} FldObj30; /* 0x30 bytes */

extern FldObj30 fldObjectSlots[32];

extern s32 D_00436300;

extern s32 D_0043630C;

extern s32 fldTestMapSlotAuxiliaryFlag();

extern void dds3SetObjectFlags();

extern void effRestartNodeInstance(s32 handle);

extern void effCopyVectorToNodeInstance(s32 handle, f32 *pos);

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

extern void effObjSetInnerFirstVec();

extern s32 *dds3GetObjectBaseResourceHandle();

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

INCLUDE_ASM(const s32, "game/code_001442D0", func_001442D0);

extern u8 fldGetCampSceneControlMode(void);

extern u8 fldGetSceneReadyOrPendingState(void);

extern s32 fileMenuTaskExists(void);

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

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145948);

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

extern void sdfReleaseDevSlot(s32, s32, s32);

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

extern f32 D_0037F980[];

extern FldVec4 D_00413788[]; /* default camera up vectors (3 copies), the first still read by asm func_00149A00 */

extern void fldApplySkyLightSetToPlayerVU(void);

extern void dds3SetWorldObjectDataValue(s32, s32);

extern s32 D_00436260;

extern s32 D_00436268;

/* Enters the field camera state for a fresh scene: releases the resource slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldWorkView *cam = (FldWorkView *)fldAreaState;
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
    D_00436248 = cam->stage;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
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
    D_0037F980[4] = 2244.0f;
    D_0037F980[5] = 2118.0f;
}

extern s32 sdfModelCreateWithAlternateItems(s32, s32);

extern s32 sdfTexAcquireResourceTexture();

/* Loads the scene's models into the resource slots, then centers the camera on the
 * scene's entry point. */
void fldLoadSceneModelsAndCamera(void) {
    FldWorkView *cam;
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
    cam = (FldWorkView *)fldAreaState;
    D_0044F7F0[5] = sdfTexAcquireResourceTexture(cam->fldmix[0].block);
    D_0044F7F0[7] = sdfTexAcquireResourceTexture(cam->fldmix[2].block);
    D_0044F7F0[8] = sdfTexAcquireResourceTexture(cam->fldmix[3].block);
    D_0044F7F0[9] = sdfTexAcquireResourceTexture(cam->fldmix[4].block);
    D_00436268 = 0;
    D_00436248 = cam->stage;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
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
    D_0037F980[4] = 2241.0f;
    D_0037F980[5] = 2113.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseSceneDevSlotsAndTextures();
}

u32 fldGetSceneReadyFlag(void) {
    return fldSceneReady;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A258);

/* Re-centers the scene camera on the current scene's entry point. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413788);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137B8);

void fldCenterCameraOnEntry(void) {
    FldWorkView *cam = (FldWorkView *)fldAreaState;
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 entry[2];
    s32 ix;
    s32 iz;

    D_00436248 = cam->stage;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
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

void fldSetSceneLocation(s32 stage, s32 room, s32 entrance) {
    fldAreaState[4] = stage;
    fldAreaState[6] = entrance;
    D_00436248 = fldAreaState[5] = room;
    fldAreaFlagIndex = stage % 100;
}

extern s32 D_00387CE0[];

/* The area number's last two digits map to a save-table row; floor and bit are zero-based. */
s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];

    if (areaIndex == -1) {
        return 0;
    }
    return (((FldAreaFlagsView *)datGameState)->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetCurrentFloorFlag(s32 flagNumber) {
    s32 areaIndex;
    s32 floor;
    s32 bitIndex;

    if (flagNumber <= 0) {
        return;
    }
    areaIndex = D_00387CE0[fldAreaFlagIndex % 100];
    if (areaIndex == -1) {
        return;
    }
    floor = fldAreaState[5];
    bitIndex = flagNumber - 1;
    {
        /* Keep byte-offset arithmetic: direct array indexing changes ee-gcc's codegen. */
        s32 byteOffset = 0xFCD0 + (areaIndex * 64 + floor) * 8;
        u64 mask = (u64)1 << bitIndex;
        u64 *flags = (u64 *)(datGameState + byteOffset);
        fldAreaState[6] = bitIndex;
        fldAreaState[47] = flagNumber;
        *flags |= mask;
    }
    fldAreaState[48] = fldFindRecordItem(floor, bitIndex);
}

/* The public setters take one-based floor and flag numbers. */
void fldSetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];
    floor--;
    bit--;
    if (areaIndex != -1) {
        s32 byteOffset = 0xFCD0 + ((areaIndex * 64 + floor) * 8);
        u64 mask = (u64)1 << bit;
        u64 *flags = (u64 *)(datGameState + byteOffset);
        *flags |= mask;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];
    floor--;
    bit--;
    if (areaIndex != -1) {
        s32 byteOffset = 0xFCD0 + ((areaIndex * 64 + floor) * 8);
        u64 mask = (u64)1 << bit;
        u64 *flags = (u64 *)(datGameState + byteOffset);
        *flags &= ~mask;
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
    s32 block;
    s32 blockIndex;

    blockIndex = 0;
    block = datGameState;
    do {
        words = (u64 *)(block + 0xfcd0);
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        blockIndex = blockIndex + 1;
        block = block + 0x200;
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

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B5D8);

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
        effCopyVectorToNodeInstance(fldIndexedResourceEffect, position);
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
        effCopyVectorToNodeInstance(mnuPositionedResourceNodes[mnuPositionedResourceCursor], pos);
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

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413800);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413818);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413830);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413848);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413860);

void fldParseMixLb(void) {
    FldLbFile *lb;
    FldLbNode *node;
    u32 index;

    fldAreaState[0x1E8 / 4] = sdfReadNamedResource("/fld/f/bin/d2_fild1.tmx", &fldAreaState[0x1EC / 4], 0);
    fldAreaState[0x1F4 / 4] = sdfReadNamedResource("/fld/f/bin/d2_fild2.tmx", &fldAreaState[0x1F8 / 4], 0);
    fldAreaState[0x200 / 4] = sdfReadNamedResource("/fld/f/bin/d2_fild3.tmx", &fldAreaState[0x204 / 4], 0);
    fldAreaState[0x20C / 4] = sdfReadNamedResource("/fld/f/bin/d2_fild4.tmx", &fldAreaState[0x210 / 4], 0);
    fldAreaState[0x1A8 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_1.tmx", &fldAreaState[0x1AC / 4], 0);
    fldAreaState[0x1B0 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_2.tmx", &fldAreaState[0x1B4 / 4], 0);
    fldAreaState[0x1B8 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_3.tmx", &fldAreaState[0x1BC / 4], 0);
    fldAreaState[0x1C0 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_5.tmx", &fldAreaState[0x1C4 / 4], 0);
    fldAreaState[0x1C8 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_6.tmx", &fldAreaState[0x1CC / 4], 0);
    fldAreaState[0x1D0 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_7.tmx", &fldAreaState[0x1D4 / 4], 0);
    fldAreaState[0x1D8 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_8.tmx", &fldAreaState[0x1DC / 4], 0);
    fldAreaState[0x1E0 / 4] = sdfReadNamedResource("/fld/f/bin/autmap_9.tmx", &fldAreaState[0x1E4 / 4], 0);
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

extern u32 D_004362DC;

extern u32 fldAreaDamageEffect;

extern s32 fldAreaDamageEffectPlaced;

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
        fldAreaState[0x1F0 / 4] = sdfTexAcquireResourceTexture(fldAreaState[0x1EC / 4]);
        fldAreaState[0x1FC / 4] = sdfTexAcquireResourceTexture(fldAreaState[0x1F8 / 4]);
        fldAreaState[0x208 / 4] = sdfTexAcquireResourceTexture(fldAreaState[0x204 / 4]);
        fldAreaState[0x214 / 4] = sdfTexAcquireResourceTexture(fldAreaState[0x210 / 4]);
    }
}

extern u32 D_004362DC;

extern u32 D_004362E8;

extern u32 D_004362F4;

extern u32 fldAreaDamageEffect;

void fldFreeSceneResources(void) {
    if (fldAreaState[0x1F0 / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(fldAreaState[0x1F0 / 4]);
        fldAreaState[0x1F0 / 4] = 0;
    }
    if (fldAreaState[0x1FC / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(fldAreaState[0x1FC / 4]);
        fldAreaState[0x1FC / 4] = 0;
    }
    if (fldAreaState[0x208 / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(fldAreaState[0x208 / 4]);
        fldAreaState[0x208 / 4] = 0;
    }
    if (fldAreaState[0x214 / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(fldAreaState[0x214 / 4]);
        fldAreaState[0x214 / 4] = 0;
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

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D0E8);

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
    effCopyVectorToNodeInstance(fldAreaDamageEffect, pos);
    fldAreaDamageEffectPlaced = 1;
    return 1;
}

extern void fldSelectDisplayBuffer(u32);
extern void func_0012BE18(s32);
extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);
extern void fldSubmitSpriteRect(s32, s32, s32, s32, s32, s32, s32, s32,
                                u32, u32);
extern s32 ptyAnyUnitFlagMatch(s32, s32);
extern f32 sdfSinPoly(f32);
extern void func_0012B690(s32, s32, s32, s32, s32, s32, s32, s32, u32,
                          u32, u32, u32, u32);
extern f32 D_0043634C;

void fldDrawAnimatedFieldBanner(s32 alpha, s32 x, s32 y) {
    u32 color;

    if (fldAreaState[0x118 / 4] != 1 && fldAreaState[0x114 / 4] != 1) {
        fldSelectDisplayBuffer(0x53);
        func_0012BE18(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitSpriteRect(x + 0x140, y + 0x10, 0x12, 0x19,
                            0x26, 3, 0x12, 0x19, 0x80808080,
                            fldAreaState[0x1F0 / 4]);
        fldSubmitSpriteRect(x + 0x152, y + 0x10, 0x90, 0x19,
                            0x36, 3, 1, 0x19, 0x80808080,
                            fldAreaState[0x1F0 / 4]);
        fldSubmitSpriteRect(x + 0x1E2, y - 1, 0x20, 0x39,
                            1, 2, 0x20, 0x39, 0x80808080,
                            fldAreaState[0x1F0 / 4]);
        if (ptyAnyUnitFlagMatch(0x5D0, 0) != 0) {
            fldSubmitSpriteRect(x + 0x1B6, y + 0x2E, 0x23, 0xB,
                                0x3B, 0x25, 0x23, 0xB, 0x80808080,
                                fldAreaState[0x208 / 4]);
            fldSubmitSpriteRect(x + 0x1D8, y + 0x26, 0x1B, 0x21,
                                0x64, 2, 0x1B, 0x21, 0x80808080,
                                fldAreaState[0x208 / 4]);
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
                          fldAreaState[0x208 / 4]);
            func_0012B690(x + 0x1D8, y + 0x26, 0x1B, 0x21,
                          0x64, 2, 0x1B, 0x21,
                          color | 0x5A000000, color | 0x5A000000,
                          color | 0x5A000000, color | 0x5A000000,
                          fldAreaState[0x208 / 4]);
            D_0043634C += 1.0f;
            if (D_0043634C > 90.0f) {
                D_0043634C = 0.0f;
            }
        }
        func_0012BE18(0);
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DB50);

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
            if (((FldWorkView *)fldAreaState)->unk12A == 0) {
                mnuSpawnResourceAtPosition(fldSecondaryQueuedEffectPosition[0], fldSecondaryQueuedEffectPosition[1], fldSecondaryQueuedEffectPosition[2]);
            }
            fldSecondaryEffectPositionPending = 0;
        }
        if (fldPrimaryEffectPositionPending != 0) {
            if (((FldWorkView *)fldAreaState)->unk12A == 0) {
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
    return func_00101740(fldTitleTaskName) != 0;
}

void func_0014E698(void) {
    D_0043635C = 0;
    D_00436354 = 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E6A8);

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
    s32 size;
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
    handle = sdfReadNamedResource(path, &size, 0);
    D_00436368 = sdfTexAcquireResourceTexture(size);
    sdfReleaseResourceAllocation(handle);
    if (field == 0xC) {
        func_0035C860(path, "/fld/f/pnl/df_b.tmx");
        handle = sdfReadNamedResource(path, &size, 0);
        D_0043636C = sdfTexAcquireResourceTexture(size);
        sdfReleaseResourceAllocation(handle);
    }
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(fldTitleTaskName, 0x2B0A, 0, 1, func_0014E6A8, fldReleaseTitleTextures, 0);
    }
}

void fldDestroyTitleTask(void) {
    if (fldTitleIsActive()) {
        kwlnTaskDestroyWithHierarchyByName(fldTitleTaskName, 1);
    }
}

s32 fldTitleMiniIsActive(void) {
    return func_00101740(D_00413C80) != 0;
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
    D_0043637C = sdfTexAcquireResourceTexture(data);
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
    D_004363AC = sdfTexAcquireResourceTexture(data);
    sdfQueueNonzeroResourceId(handle);
    handle = sdfReadNamedResource("/fld/f/bin/d2_hunt2.tmx", &data, 0);
    D_004363B0 = sdfTexAcquireResourceTexture(data);
    sdfQueueNonzeroResourceId(handle);
    handle = sdfReadNamedResource("/fld/f/bin/d2_hunt3.tmx", &data, 0);
    D_004363B4 = sdfTexAcquireResourceTexture(data);
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
        effCopyVectorToNodeInstance(fldDamEffectNode, pos);
        fldDamEffectPositioned = 1;
    }
    if (fldYukEffectNode != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        effRestartNodeInstance(fldYukEffectNode);
        effCopyVectorToNodeInstance(fldYukEffectNode, pos);
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
            ddsReleaseUnitObject(objectHandle);
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

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F980);

void fldFreeSparkSlot(s32 index) {
    FldVec4 vec;
    s32 obj;
    s32 *flags;
    s16 slot;

    vec = D_00413DE8;
    if (fldSparkSlots[index].hasVectors == 1 && fldSparkSlots[index].active != 0 && fldSparkSlots[index].objectSlot != -1) {
        vec.v[0] = fldSparkSlots[index].pos[0];
        vec.v[2] = fldSparkSlots[index].pos[2];
        effObjSetInnerFirstVec(fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle, &vec);
        obj = fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle;
        flags = dds3GetObjectBaseResourceHandle(obj);
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

extern s32 D_003899E0[];

extern void func_00125B10(void);

void fldFinishEventFieldState(void) {
    FldWorkView *work = (FldWorkView *)fldAreaState;

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

extern s32 D_003899E0[];

extern void fldReleaseCameraModel(s32);

void fldResetEventSceneState(void) {
    fldReleaseCameraModel(0);
    D_003899E0[0] = 0;
    fldAreaState[0x118 / 4] = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldPreparePlayerSceneCameraTarget();
    fldAreaState[0x114 / 4] = 1;
    ((FldWorkView *)fldAreaState)->eventActive = 0;
    D_00451B9C[0] = 0;
    fldAreaState[0x138 / 4] = 1;
}

void fldResetAfterEvent(void) {
    func_00341C78(0x680017);
    mnuAdvanceTitleStateUnderSemaphore();
    D_003899E0[0] = 0;
    fldAreaState[0x118 / 4] = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldStartSceneBgmAlternate();
    fldAreaState[0x114 / 4] = 0;
    ((FldWorkView *)fldAreaState)->eventActive = 0;
    D_00451B9C[0] = 0;
    fldAreaState[0x138 / 4] = 1;
    evtSetSolarOverlayFullyVisible();
}

void fldStartDeferredFieldExit(void) {
    evtCreateMessageWindowIfMissing(D_003A9EB0);
    dspStartEntry(4);
    fldResetPlayerSceneObjectState();
    D_00389884[0] = 2;
}

void fldFinishDeferredExit(void) {
    if (fldAreaState[0x45] == 2) {
        func_0026C900();
        if (!evtGetMessageWindowControlState()) {
            evtFinishMessageWindowAndNotify();
            dspCloseChannel();
            fldPreparePlayerSceneCameraTarget();
            fldAreaState[0x45] = 0;
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
    u8 pad18[8];
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
    s32 unk50;
    s32 unk54;
    u8 pad58[0xC];
    s32 unk64;
    u8 pad68[4];
    s32 unk6C;
    s32 cycleIndex;
} FieldTargetGuideState;

extern FieldTargetGuideState fldTargetGuideState;

void fldResetViewState(void) {
    fldTargetGuideState.unk6C = -1;
    fldTargetGuideState.unk54 = 0;
    fldTargetGuideState.unk50 = 0;
    fldTargetGuideState.unk64 = 0;
    fldTargetGuideState.mode = 0;
    fldTargetGuideState.cycleIndex = 0;
    fldTargetGuideState.cycleRoute = NULL;
    fldTargetGuideState.disabled = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514F8);

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
    u8 unk04[0xC];
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

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151E00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151FF8);

extern void func_001519E8(s32);

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

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

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

INCLUDE_ASM(const s32, "game/code_001442D0", func_001523F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001525F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001526B8);

extern void fldReleaseTargetGuideResource(void);

extern void fldResetViewState(void);

extern void func_00341C78(s32);

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

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152C88);

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

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153410);

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

s32 func_00153560(void) {
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

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F78);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001536B8);

void fldSetTargetGuideEnabled(s32 active) {
    if (active == 0) {
        D_00451D3C[0] = 1;
        return;
    }
    D_00451D3C[0] = 0;
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414000);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153D60);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153FA0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414020);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414030);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001540E8);

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

