#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

/* Retained field-area work, not a camera-only object. Unknown regions remain
 * opaque; this prefix covers the camera, event state and fldmix map resources. */
typedef struct FldResourceBlock {
    s32 unk0;  /* Retained fldmix.LB node value; only written here. */
    s32 block; /* sdfMemoryGetBlockAddress(unk0). */
} FldResourceBlock;

typedef struct FldAreaWork {
    u8 pad00[0x10];
    s32 area; /* 0x10 */
    s32 room; /* 0x14: the floor/room argument of fldSetSceneLocation. */
    s32 unk18;
    u8 pad1C[0x14];
    f32 focusPos[3]; /* 0x30 */
    u8 pad3C[0x14];
    s32 unk50;
    u8 pad54[0x10];
    f32 negatedAngle;
    u8 pad68[8];
    s32 unk70;
    u8 pad74[0x4C];
    s32 unkC0;
    u8 padC4[0x40];
    s16 eventActive; /* 0x104 */
    u8 pad106[0xE];
    s32 unk114;
    s32 unk118;
    u8 pad11C[0xE];
    s16 unk12A;
    u8 pad12C[0xC];
    s32 unk138;
    u8 pad13C[4];
    f32 x;
    f32 y;
    f32 z;
    u8 pad14C[0x18];
    f32 angle;
    u8 pad168[0x34];
    FldResourceBlock mapResources[4]; /* 0x19C: fldmix.LB nodes 10..13. */
} FldAreaWork;

#define FLD_WORK ((FldAreaWork *)fldAreaState)

typedef struct FldVec4 {
    f32 v[4];
} FldVec4;

extern char fldTitleTaskName[]; /* "fldTitle" */

extern char D_003A0828[]; /* "fldTitleMini" */

extern u32 fldDamEffectResource;

extern u32 fldDamEffectData;

extern u32 fldDamEffectNode;

extern u32 fldDamEffectPositioned;

extern u32 fldYukEffectResource;

extern u32 fldYukEffectData;

extern u32 fldYukEffectNode;

extern u32 fldYukEffectPositioned;

extern s32 fldWeatherLimitTexture;

extern s32 D_003BAFBC;

extern s32 D_003BAFC4;

extern s32 D_003BAFB8;

extern s32 D_003BAFC0;

extern s32 D_003BAFB4;

extern u32 D_003BAFA0;

extern u32 D_003BAFA8;

extern u32 D_003BAFB0;

extern s32 datGameState;

extern u32 fldSceneReady;

extern u32 fldSceneRecords;

extern s32 fldSceneRecordCount;

extern s32 fldSceneRecordResource;

extern u32 fldFixedArchiveLoadPhase;

extern u32 fldArchiveLoadPending;

extern u32 fldCurrentBgmHandle;

extern u32 fldFieldTaskHandle;

extern s32 fldPendingSoundCount;

extern s32 fldAreaFlagIndex;

extern s32 D_003BAEB4;

extern s32 fldAreaState[];

extern s32 fldPrimaryEffectPositionPending;

extern s32 fldSecondaryEffectPositionPending;

extern s32 D_0032E474[];

extern s32 D_0033EB78[];

extern void func_002E96D8(s32 arg0);

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern s32 dds3GetWorldObject(void);

extern void effUpdateNode(u32 arg0);

extern void fldRelocatePackedTransferChunk(u32 arg0, s32 arg1);

extern s32 func_001277A8(s32 arg0);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void fldReleaseCameraModel(s32 arg0);

extern void evtSetSolarOverlayFullyVisible(void);

extern s32 D_0032E5C4[];

extern s32 D_0032E4C4[];

extern u8 D_0034D8F0[];

extern void evtCreateMessageWindowIfMissing(void *arg0);

extern void dspStartEntry(s32 arg0);

extern void fldResetPlayerSceneObjectState(void);

extern s32 D_0032E478[];

extern f32 fldPrimaryQueuedEffectPosition[];

extern f32 fldSecondaryQueuedEffectPosition[];

extern s32 D_003D62C8[];

extern void *D_003D62A4[];

extern s32 D_003D62A8[];

extern s32 fldCurrentBgmId[];

extern void func_002E8DD0();

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern void sndStartTrackDefault(s32 arg0);

extern void *sdfAllocSizeClassBlock(s32 size);

extern void kwlnTaskSetUserValue(s32 arg0, void *arg1);

extern s32 fldFieldTaskUpdate(void);

extern s32 D_0032E3C0[];

extern s32 fldSceneSoundBase;

extern void ddsReleaseUnitObject(s32 arg0);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0xC];
} FldEnt14; /* 0x14 bytes */

extern FldEnt14 fldSparkObjectEntries[];

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x10C];
} FldEnt110; /* 0x110 bytes */

extern FldEnt110 *D_003BAA48;

extern s32 fldGetCurrentSceneSelectionId(void);

typedef struct {
    s32 *unk0;
    u8 pad4[0x4C];
} FldTbl50; /* 0x50 bytes */

extern FldTbl50 D_003D46C0[];

extern s32 kwlnTaskGetTaskByName(void *name);

extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern u8 D_003BAE78[];

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

typedef struct {
    s32 flags;
    s32 soundId;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FldClear18; /* 0x18 bytes */

extern FldClear18 fldPendingSounds[];

extern u64 kwlnTaskGetUserValue(void);

extern void fldSelectDisplayBuffer(u32);

extern void func_00129900(u32);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void fldSubmitSpriteRect(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001411F0);

extern s32 fldGetCampSceneControlMode(void);

extern s32 fldGetSceneReadyOrPendingState(void);

extern s32 fldTitleIsActive(void);

extern void fldDrawAnimatedFieldBanner(s32, s32, s32);

extern void func_00147188(s32);

extern s32 D_003BAB08;

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
    if (D_003BAB08 != -999) {
        fldDrawAnimatedFieldBanner(0x80, 0, 0);
    } else {
        func_00147188(0);
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
        fldFieldTaskHandle = kwlnTaskCreate(D_003BAE78, 0x2B0B, 1, 1, fldFieldTaskCreate, fldFieldTaskDestroy, 0);
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

void fldAppendClearEntry(s32 id, f32 x, f32 y, f32 z, f32 w) {
    s32 i = fldPendingSoundCount;

    fldPendingSounds[i].flags = 0;
    fldPendingSounds[i].soundId = id;
    fldPendingSounds[i].x = x;
    fldPendingSounds[i].y = y;
    fldPendingSounds[i].z = z;
    fldPendingSounds[i].w = w;
    fldPendingSoundCount = i + 1;
}

extern s32 fldStageSoundBaseTable[];

extern s32 mdlFlagTest();

void fldPlayPendingSounds(void) {
    s32 stage;
    s32 i;

    stage = D_0032E3C0[0];
    if (stage == 11) {
        stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
    }
    for (i = 0; i < fldPendingSoundCount; i++) {
        if (fldPendingSounds[i].flags & 1) {
            fldPendingSounds[i].flags &= ~1;
            func_002E8DD0(fldStageSoundBaseTable[stage] + fldPendingSounds[i].soundId);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001415F8);

extern s32 D_0033EB7C[];

extern s32 D_0033EB80[];

extern s32 D_0033EB84[];

extern s32 D_0033EB88[];

extern s32 D_0033EB90[];

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0470);

s32 fldResolveSpecialBgmTrack(s32 id) {
    s32 result;

    switch (id) {
    case 0x80:
        result = D_0033EB78[0] + 1;
        break;
    case 0x81:
        result = D_0033EB7C[0] + 1;
        break;
    case 0x82:
        result = D_0033EB80[0] + 1;
        break;
    case 0x83:
        result = D_0033EB7C[0] + 2;
        break;
    case 0x84:
        result = D_0033EB84[0] + 1;
        break;
    case 0x85:
        result = D_0033EB84[0] + 2;
        break;
    case 0x86:
        result = D_0033EB88[0] + 1;
        break;
    case 0x87:
        result = D_0033EB90[0] + 1;
        break;
    case 0x88:
        result = D_0033EB90[0] + 2;
        break;
    case 0x89:
        result = D_0033EB90[0] + 3;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

typedef struct {
    s16 id;
    s16 flag;
    s8 data[0x40];
} FldEnt44; /* 0x44 bytes */

extern FldEnt44 D_0032E5C8[32];

s8 fldFindSceneEntryData(s32 id, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_0032E5C8[i].id == id && (D_0032E5C8[i].flag == 0 || mdlFlagTest(D_0032E5C8[i].flag) != 0)) {
            return D_0032E5C8[i].data[idx];
        }
    }
    return 0;
}

void fldStartSceneBgm(void) {
    s32 handle = fldResolveSpecialBgmTrack(fldAreaState[10]);
    s32 stage;
    s32 idx;

    if (handle != -1 || fldAreaState[4] < 0x32) {
        if (handle == -1) {
            stage = fldAreaState[4];
            if (stage == 11) {
                stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, fldAreaState[5] + 1);
            fldSceneSoundBase = fldStageSoundBaseTable[stage];
            fldAreaState[10] = idx;
            handle = fldSceneSoundBase + idx;
        }
        if (fldAreaState[76] != 1) {
            if (fldCurrentBgmHandle != handle) {
                func_002E96D8(fldCurrentBgmHandle);
            }
            fldCurrentBgmHandle = handle;
            sndStartTrackDefault(handle);
        }
    }
}

void fldStartTitleBgmIfSelected(void) {
    if (fldCurrentBgmId[0] == 0x80) {
        sndStartTrackDefault(D_0033EB78[0] + 1);
    }
}

extern s32 fldResolveSpecialBgmTrack(s32);

extern u64 sndStartTrackAlternate(s32);

void fldStartSceneBgmAlternate(void) {
    s32 handle = fldResolveSpecialBgmTrack(fldAreaState[10]);
    s32 stage;
    s32 idx;

    if (handle != -1 || fldAreaState[4] < 0x32) {
        if (handle == -1) {
            stage = fldAreaState[4];
            if (stage == 11) {
                stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, fldAreaState[5] + 1);
            fldSceneSoundBase = fldStageSoundBaseTable[stage];
            fldAreaState[10] = idx;
            handle = fldSceneSoundBase + idx;
        }
        if (fldAreaState[76] != 1) {
            fldCurrentBgmHandle = handle;
            sndStartTrackAlternate(handle);
        }
    }
}

void fldStopCurrentBgm(void) {
    sndSendSingleWordControlPacket(fldCurrentBgmHandle);
    fldCurrentBgmHandle = 0;
}

void fldReleaseCurrentBgm(void) {
    func_002E96D8(fldCurrentBgmHandle);
    if (fldAreaState[10] >= 0x80) {
        fldAreaState[10] = 1;
    }
    fldCurrentBgmHandle = 0;
}

void func_00141D80(void) {
    func_002E8E00();
}

void fldPlayCurrentBgmSound(void) {
    func_002E8DD0(fldCurrentBgmHandle);
    fldCurrentBgmId[0] = 0;
}

void fldPlayFieldSeVolumePan(s32 soundId) {
    if (D_0032E3C0[0] < 50 && soundId >= 16) {
        if (soundId == 0x80) {
            sndSetSequenceVolumePan(0x6C0060, 0x7F, 0x3F);
        } else if (soundId == 0x81) {
            sndSetSequenceVolumePan(0x6C0061, 0x7F, 0x3F);
        } else if (soundId == 0x82) {
            sndSetSequenceVolumePan(0x6C0064, 0x7F, 0x3F);
        } else {
            sndSetSequenceVolumePan(fldSceneSoundBase + soundId, 0x7F, 0x3F);
        }
    }
}

void fldPlayFieldSe(s32 soundId) {
    if (D_0032E3C0[0] < 50) {
        if (soundId == 0x100) {
            func_002E8DD0(0x6C0060);
        } else if (soundId == 0x101) {
            func_002E8DD0(0x6C0061);
        } else if (soundId == 0x102) {
            func_002E8DD0(0x6C0064);
        } else {
            func_002E8DD0(fldSceneSoundBase + soundId);
        }
    }
}

void fldSetSequenceVolume(s32 category, s32 volume) {
    if (D_0032E3C0[0] < 0x32) {
        sndSetSequenceVolumePan(fldSceneSoundBase + category, volume, 0x3F);
    }
}

extern s32 fldCurrentBgmMode;

extern s32 D_0033EAE8[];

extern void func_002E96D8();

void fldSetBgmMode(s32 mode) {
    switch (mode) {
    case -1:
        if (fldCurrentBgmMode != mode) {
            func_002E8DD0(fldCurrentBgmMode);
            fldCurrentBgmMode = mode;
        }
        break;
    case 0:
        mode = -1;
        if (fldCurrentBgmMode != mode) {
            func_002E96D8(fldCurrentBgmMode);
            fldCurrentBgmMode = mode;
        }
        break;
    case 1:
        mode = D_0033EB78[0] + 1;
        sndStartTrackDefault(mode);
        fldCurrentBgmMode = mode;
        break;
    case 2:
        mode = D_0033EAE8[0] + 1;
        sndStartTrackDefault(mode);
        fldCurrentBgmMode = mode;
        break;
    }
}

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 fldSceneBgmArchiveTrack;

extern s32 fldSceneBgmArchivePhase;

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
            stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
        }
        idx = fldFindSceneEntryData(stage, fldAreaState[5] + 1);
        fldSceneSoundBase = fldStageSoundBaseTable[stage];
        fldAreaState[10] = idx;
        handle = fldSceneSoundBase + idx;
    }
    fldSceneBgmArchiveTrack = handle;
    fldSceneBgmArchivePhase = 0;
}

extern s32 sndFindPackedTrackLoadStatus(s32);

extern void sndEnsureMidiBankResident(s32);

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
    func_002E8DD0(archiveId * 0x10000 + soundId + 0x30000000);
}

void fldResetArchiveLoadPhase(void) {
    fldFixedArchiveLoadPhase = 0;
}

s32 fldStepArchiveLoad(void) {
    switch (fldFixedArchiveLoadPhase) {
    case 0:
        if (sndFindPackedTrackLoadStatus(0x670000) == 0) {
            sndEnsureMidiBankResident(0x670000);
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        } else {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 2;
        }
        break;
    case 1:
        if (sndFindPackedTrackLoadStatus(0x670000) == 1) {
            fldFixedArchiveLoadPhase = fldFixedArchiveLoadPhase + 1;
        }
        break;
    default:
        fldFixedArchiveLoadPhase = -1;
        return 1;
    }
    return 0;
}

u32 fldGetCurrentBgmHandle(void) {
    return fldCurrentBgmHandle;
}

extern s32 func_00121818(s32, s32);

extern s32 func_00121870(s32, s32);

extern s32 fldFindMapCoordinateIndex(s32, s32);

extern s32 strlen(const char *);

extern char D_0033F06C[][0x1C], D_0034286C[][0x1C], D_0034606C[][0x1C];

extern char D_0033EC90[][0x18];

extern s16 D_003D43B0[], D_003D4408[], D_003D4460[];

extern s16 D_003BD7D0;

void fldCacheMapLabelLengths(s32 map) {
    s32 i;

    for (i = 0; i < 41; i++) {
        D_003D43B0[i] = strlen(D_0033F06C[func_00121818(map, i)]);
    }
    for (i = 0; i < 24; i++) {
        D_003D4460[i] = strlen(D_0034286C[func_00121870(map, i)]);
    }
    for (i = 0; i < 41; i++) {
        D_003D4408[i] = strlen(D_0034606C[fldFindMapCoordinateIndex(map, i)]);
    }
    i = 0;
    if (D_0032E3C0[0] < 100) {
        i = D_0032E3C0[0];
    }
    D_003BD7D0 = strlen(D_0033EC90[i]);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142408);

void fldSetSceneRecordChunk(s32 chunk, s32 resourceHandle) {
    /* Descriptor from func_001277A8 precedes the 0x14-byte scene rows. */
    typedef struct {
        u8 pad00[4];
        s32 rows; /* 0x04: first scene row */
        s32 count; /* 0x08: number of scene rows */
    } SceneHeader;
    if (D_0032E3C0[0] < 0xC8) {
        s32 source = chunk;
        s32 resource = resourceHandle;
        s32 transferStart = source + 8;

        fldSceneRecordResource = resource;
        fldRelocatePackedTransferChunk(chunk, transferStart);
        {
            s32 header = func_001277A8(transferStart);
            s32 rows = ((SceneHeader *)header)->rows;
            s32 count = ((SceneHeader *)header)->count;

            fldSceneRecordCount = count;
            fldSceneRecords = rows;
        }
    }
}

void fldInitSceneMapLabels(void) {
    s32 sceneId = D_0032E3C0[0];

    if (sceneId < 0xC8) {
        fldCacheMapLabelLengths(sceneId % 100);
        func_00142408();
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

INCLUDE_ASM(const s32, "game/code_001411F0", func_001426E0);

typedef struct {
    u8 pad0[0xC];
    s32 *unkC;
} FldEmitterRes;

typedef struct {
    FldEmitterRes *res;
    u8 pad4[0x4C];
    f32 pos[4];
} FldEmitter;

extern u8 D_00325838[];

extern void sdfDrawNodeBuildMatrix();

extern void sdfModelUpdateCurrentFrameTransforms();

extern void func_002D9238();

void fldSetEmitterPosition(FldEmitter *emitter, f32 x, f32 y, f32 z) {
    f32 pos[4];
    s32 handle;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    handle = *emitter->res->unkC;
        VU0_LOAD_VF_MEMORY(vf10, pos);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, emitter->pos);
    sdfDrawNodeBuildMatrix(handle);
    sdfModelUpdateCurrentFrameTransforms(emitter);
    func_002D9238(D_00325838, emitter);
}

typedef struct {
    s16 u;
    s16 v;
    s16 width;
    s16 height;
    s16 anchorX;
    s16 anchorY;
    s32 textureSlot;
} FldProjectedSprite;

extern FldProjectedSprite D_0033EBA0[];
extern u32 D_003D40A0[];

void func_00142800(s32 index, u32 color, f32 x, f32 y) {
    FldProjectedSprite *sprite = &D_0033EBA0[index];
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
                        D_003D40A0[D_0033EBA0[index].textureSlot]);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001428C0);

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
    s32 model; /* 0xC */
    FldPoint *pos; /* 0x10 */
} FldSceneRecord; /* 0x14 bytes */

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

void func_00142C78(FldModelView *model, s32 index, s32 clearFlag) {
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

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142D78);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001447D0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00144D30);

extern void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z);

typedef struct FldFogParams {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s32 unk10;
} FldFogParams;

extern FldFogParams D_00324B30;

extern u128 D_00324A20;

extern u128 D_00324A30;

extern u128 D_00324A40;

extern s32 D_003BAEB8;

extern s32 D_003BAEBC;

extern s32 D_003BAEC0;

extern s32 D_003BAEC4;

extern s32 D_003BAEC8;

extern s32 D_003BAED0;

extern s32 D_003BAECC;

extern s32 D_003BAED4;

INCLUDE_ASM(const s32, "game/code_001411F0", func_00145B18);

extern void sdfTexReleaseReferenceViaHandler();

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern void frFontSetSharedRenderFlags(s32);

void fldReleaseTitleSlots(void) {
    if (D_003D40A0[2] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[2]);
    }
    if (D_003D40A0[3] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[3]);
    }
    D_003D40A0[2] = 0;
    D_003D40A0[3] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    fldSceneReady = 0;
    frFontSetSharedRenderFlags(0x54);
}

extern u32 D_00348F30[];

typedef struct FldSlot {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
} FldSlot;

extern FldSlot D_003D40B0[];

extern void sdfReleaseDevSlot(u32, s32, s32);

void fldReleaseMenuSlots(void) {
    s32 i;

    for (i = 0; i < fldSceneRecordCount; i++) {
        if (D_00348F30[i] != 0) {
            sdfReleaseDevSlot(D_00348F30[i], 1, 1);
        }
    }
    for (i = 0; i < 64; i++) {
        D_00348F30[i] = 0;
        D_003D40B0[i].unk_0 = 0;
        D_003D40B0[i].unk_4 = 0;
        D_003D40B0[i].unk_8 = 0;
    }
    if (D_003D40A0[0] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[0]);
    }
    D_003D40A0[0] = 0;
    if (D_003D40A0[1] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[1]);
    }
    D_003D40A0[1] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    fldSceneReady = 0;
}

extern void fldClearSceneLifecycleFlags(s32);

extern f32 D_00324980[];

extern FldVec4 D_003A05D8[]; /* default camera up vectors (3 copies), the first still read by asm func_00145B18 */

extern void fldApplySkyLightSetToPlayerVU(void);

extern void dds3SetWorldObjectDataValue(s32, s32);

/* Enters the field camera state for a fresh scene: releases the title slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldAreaWork *cam = (FldAreaWork *)fldAreaState;
    f32 focus[4];
    f32 eye[4];
    FldVec4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 cx;
    s32 cz;

    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseTitleSlots();
    fldSceneReady = 0;
    fldClearSceneLifecycleFlags(1);
    fldPreparePlayerSceneCameraTarget();
    evtSetSolarOverlayFullyVisible();
    fldApplySkyLightSetToPlayerVU();
    cam->unk70 = 4;
    frFontSetSharedRenderFlags(0x54);
    dds3SetWorldObjectDataValue(dds3GetWorldObject(), 1);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->room;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->room, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    iz = cz + entry[1];
    ix = cx + entry[0];
    memcpy(&up, &D_003A05D8[1], sizeof(up));
    D_003BAECC = 0;
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_00324A30, eye);
    PCP_COPY_VECTOR(&D_00324A20, focus);
    PCP_COPY_VECTOR(&D_00324A40, &up);
    D_00324B30.unk0 = 255.0f;
    D_00324B30.unk8 = 1000.0f;
    D_00324B30.unk4 = 255.0f;
    D_00324B30.unkC = 20000.0f;
    D_00324B30.unk10 = 0x108010;
    D_00324980[4] = 2244.0f;
    D_00324980[5] = 2118.0f;
}

extern s32 sdfModelCreateWithAlternateItems(s32, s32);

extern s32 sdfTexAcquireResourceTexture();

/* Loads the scene's models into the menu slots, then centers the camera on the
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
    s32 cx;
    s32 cz;
    s32 i;

    rec = (FldSceneRecord *)fldSceneRecords;
    for (i = 0; i < fldSceneRecordCount; i++, rec++) {
        D_00348F30[i] = sdfModelCreateWithAlternateItems(0, rec->model);
        ((FldPoint *)D_003D40B0)[i].x = rec->pos->x;
        ((FldPoint *)D_003D40B0)[i].y = rec->pos->y;
        ((FldPoint *)D_003D40B0)[i].z = rec->pos->z;
    }
    cam = (FldAreaWork *)fldAreaState;
    D_003D40A0[0] = sdfTexAcquireResourceTexture(cam->mapResources[0].block);
    D_003D40A0[1] = sdfTexAcquireResourceTexture(cam->mapResources[1].block);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->room;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->room, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    iz = cz + entry[1];
    ix = cx + entry[0];
    memcpy(&up, &D_003A05D8[2], sizeof(up));
    D_003BAECC = 0;
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_00324A30, eye);
    PCP_COPY_VECTOR(&D_00324A20, focus);
    PCP_COPY_VECTOR(&D_00324A40, &up);
    D_00324B30.unk0 = 255.0f;
    D_00324B30.unk8 = 1000.0f;
    D_00324B30.unk4 = 255.0f;
    D_00324B30.unkC = 20000.0f;
    D_00324B30.unk10 = 0x108010;
    D_00324980[4] = 2244.0f;
    D_00324980[5] = 2118.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseMenuSlots();
}

u32 fldGetSceneReadyFlag(void) {
    return fldSceneReady;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001462D8);

/* Re-centers the scene camera on the current scene's entry point. */
INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A05D8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0608);

void fldCenterCameraOnEntry(void) {
    FldAreaWork *cam = (FldAreaWork *)fldAreaState;
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 entry[2];
    s32 ix;
    s32 iz;

    D_003BAEB4 = cam->room;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->room, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_00324A30, eye);
    PCP_COPY_VECTOR(&D_00324A20, focus);
    PCP_COPY_VECTOR(&D_00324A40, up);
    D_00324B30.unk0 = 255.0f;
    D_00324B30.unk8 = 1000.0f;
    D_00324B30.unk4 = 255.0f;
    D_00324B30.unkC = 20000.0f;
    D_00324B30.unk10 = 0x808080;
}

/* Stores the area, floor/room and third location selection, and selects the
 * area's flag table. The third selection's interpretation is left unknown. */
void fldSetSceneLocation(s32 area, s32 floor, s32 stage) {
    FLD_WORK->area = area;
    FLD_WORK->unk18 = stage;
    D_003BAEB4 = FLD_WORK->room = floor;
    fldAreaFlagIndex = area % 100;
}

/* DDS1 floor flags start at +0x13F70; DDS2 stores them elsewhere. */
typedef struct FldAreaFlagsView {
    u8 pad00[0x13F70];
    u64 areaFlags[13][64];
} FldAreaFlagsView;

extern s32 D_0032C900[];

/* The area's last two digits map to a save-table row; floor and bit are zero-based. */
s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_0032C900[area % 100];
    if (areaIndex == -1) return 0;
    return (((FldAreaFlagsView *)datGameState)->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetFlagAndFindRecord(s32 flagNumber) {
    s32 areaIndex;
    s32 bit;

    if (flagNumber > 0) {
        areaIndex = D_0032C900[fldAreaFlagIndex % 100];
        if (areaIndex != -1) {
            bit = flagNumber - 1;
            fldAreaState[6] = bit;
            fldAreaState[0x2F] = flagNumber;
            ((FldAreaFlagsView *)datGameState)->areaFlags[areaIndex][fldAreaState[5]] |= 1ULL << bit;
            fldAreaState[0x30] = fldFindRecordItem(fldAreaState[5], bit);
        }
    }
}

/* The public setters take one-based floor and flag numbers. */
void fldSetFlagBit(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_0032C900[area % 100];
    if (areaIndex != -1) {
        ((FldAreaFlagsView *)datGameState)->areaFlags[areaIndex][floor] |= 1ULL << bit;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_0032C900[area % 100];
    if (areaIndex != -1) {
        ((FldAreaFlagsView *)datGameState)->areaFlags[areaIndex][floor] &= ~(1ULL << bit);
    }
}

void func_00146CA8(s32 index) {
    if (index >= 0x40) {
        D_0032E474[0] = -1;
    } else {
        D_0032E474[0] = index;
    }
}

void func_00146CD0(s32 index) {
    if (index >= 0x40) {
        D_0032E478[0] = -1;
    } else {
        D_0032E478[0] = index;
    }
}

typedef struct {
    u8 pad0[8];
    u32 count;
    u8 padC[8];
} FldSceneEntry; /* 0x14 bytes */

extern s32 fldFindRecordItem(s32 scene, u32 index);

s32 fldFindPreviousMarkedValue(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = -1;
    FldSceneEntry *entry = (FldSceneEntry *)fldSceneRecords;

    for (i = 0; i < (s32)fldSceneRecordCount; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldGetFloorFlag(fldAreaFlagIndex, i, j) != 0) {
                item = fldFindRecordItem(i, j);
                if (item < limit && best < item) {
                    best = item;
                }
            }
        }
    }
    if (best == -1) {
        return limit;
    }
    return best;
}

s32 fldFindNextMarkedValue(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = 999;
    FldSceneEntry *entry = (FldSceneEntry *)fldSceneRecords;

    for (i = 0; i < (s32)fldSceneRecordCount; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldGetFloorFlag(fldAreaFlagIndex, i, j) != 0) {
                item = fldFindRecordItem(i, j);
                if (limit < item && item < best) {
                    best = item;
                }
            }
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
            f32 *position = *(f32 **)(entry + 0x10);
            *x = position[0];
            *z = position[2];
            return;
        }
    }
    *x = 0.0f;
    *z = 0.0f;
}

extern s32 D_003BAEAC;

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
            if (D_003BAEAC != 0) {
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

INCLUDE_ASM(const s32, "game/code_001411F0", fldCheckSceneReady);

void func_001470E0(void) {
    if (fldSceneReady == 1) {
        func_001447D0();
        func_00142D78();
    }
}

void fldResetCameraAndSceneView(void) {
    fldCenterCameraOnEntry();
    func_00144D30();
}

void fldClearAllAreaFloorFlags(void) {
    u64 *words;
    s32 remaining;
    s32 block;
    s32 index;

    index = 0;
    block = datGameState;
    do {
        words = (u64 *)(block + 0x13f70);
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        index = index + 1;
        block = block + 0x200;
    } while (index < 0xd);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147188);

extern s32 fldEffectTextureLoadHandles[];

extern s32 fldEffectTextureData[];

extern s32 fldEffectTextureNodes[];

extern s32 D_0034C8A0[];

extern s32 sdfReadNamedResource();

extern s32 func_0014FE28();

extern char *D_003A06B0[4]; /* {"/fld/f/bin/KUT_3Z.EPL", "ASI_2MZ", "ASI_2HZ", "MAN_2Z"} */

void fldLoadFieldEffectTextureSlots(void) {
    char *names[4];
    s32 i;

    memcpy(names, D_003A06B0, sizeof(names));
    for (i = 0; i < 4; i++) {
        fldEffectTextureLoadHandles[i] = sdfReadNamedResource(names[i], &fldEffectTextureData[i], 0);
        fldEffectTextureNodes[i] = func_0014FE28(fldEffectTextureData[i]);
        D_0034C8A0[i] = 0;
    }
}

extern s32 fldEffectTextureLoadHandles[4];

extern s32 fldEffectTextureData[4];

extern s32 fldEffectTextureNodes[4];

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

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147638);

extern s32 mnuPositionedResourceNodes[];

extern s32 mnuPositionedResourceActive[];

extern s32 D_003BAF88;

void mnuInitializeResourceEntries(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        mnuPositionedResourceNodes[i] = func_0014FE28(D_003BAF88);
        mnuPositionedResourceActive[i] = 0;
    }
}

extern void effRestartNodeInstance(s32 handle);

extern void effCopyVectorToNodeInstance(s32 handle, f32 *pos);

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
    s32 index;
    for (index = 0; index < 4; index++) {
        if (mnuPositionedResourceNodes[index] != 0 && mnuPositionedResourceActive[index] != 0) {
            effUpdateNode(mnuPositionedResourceNodes[index]);
        }
    }
}

extern void *fileQueuePlainDispatchRequest(const char *);

extern void func_00288C50(void *);

extern void func_00288788(void *);

extern s32 sdfMemoryGetBlockAddress(s32);

extern s32 D_003BD7EC, D_003BD7F0, D_003BAF68, D_003BAF6C, D_003BAF50, D_003BAF54;

extern s32 D_003BAF74, D_003BAF78, D_003BAF5C, D_003BAF60, D_003BAF84, D_003BAF88;

extern s32 D_003BAF44, D_003BAF48, D_003BD7D4, D_003BD7D8, D_003BD7DC, D_003BD7E0;

extern s32 D_003BD7E4, D_003BD7E8;

typedef struct FldLbNode {
    struct FldLbNode *next; /* 0x00 */
    u8 unk04[4];
    s32 value;              /* 0x08 */
} FldLbNode;

typedef struct FldLbFile {
    u8 unk00[0x60];
    FldLbNode *nodes; /* 0x60 */
} FldLbFile;


INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0650);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0668);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0680);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0698);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A06B0);

/* Retains fldmix.LB node values and their memory blocks in field work.
 * Nodes 10..13 supply the map resources later acquired by the scene camera. */
void fldParseMixLb(void) {
    FldLbFile *lb;
    FldLbNode *node;
    u32 index;
    s32 value;

    index = 0;
    lb = fileQueuePlainDispatchRequest("/fld/f/bin/fldmix.LB");
    func_00288C50(lb);
    for (node = lb->nodes; node != NULL; node = node->next, index++) {
        switch (index) {
        case 0:
            value = node->value;
            D_003BD7EC = value;
            D_003BD7F0 = sdfMemoryGetBlockAddress(value);
            break;
        case 1:
            value = node->value;
            D_003BAF68 = value;
            D_003BAF6C = sdfMemoryGetBlockAddress(value);
            break;
        case 2:
            value = node->value;
            D_003BAF50 = value;
            D_003BAF54 = sdfMemoryGetBlockAddress(value);
            break;
        case 3:
            value = node->value;
            D_003BAF74 = value;
            D_003BAF78 = sdfMemoryGetBlockAddress(value);
            break;
        case 4:
            value = node->value;
            D_003BAF5C = value;
            D_003BAF60 = sdfMemoryGetBlockAddress(value);
            break;
        case 5:
            value = node->value;
            D_003BAF84 = value;
            D_003BAF88 = sdfMemoryGetBlockAddress(value);
            break;
        case 6:
            value = node->value;
            D_003BAF44 = value;
            D_003BAF48 = sdfMemoryGetBlockAddress(value);
            break;
        case 7:
            value = node->value;
            D_003BD7D4 = value;
            D_003BD7D8 = sdfMemoryGetBlockAddress(value);
            break;
        case 8:
            value = node->value;
            D_003BD7DC = value;
            D_003BD7E0 = sdfMemoryGetBlockAddress(value);
            break;
        case 9:
            value = node->value;
            D_003BD7E4 = value;
            D_003BD7E8 = sdfMemoryGetBlockAddress(value);
            break;
        case 10:
            value = node->value;
            FLD_WORK->mapResources[0].unk0 = value;
            FLD_WORK->mapResources[0].block = sdfMemoryGetBlockAddress(value);
            break;
        case 11:
            value = node->value;
            FLD_WORK->mapResources[1].unk0 = value;
            FLD_WORK->mapResources[1].block = sdfMemoryGetBlockAddress(value);
            break;
        case 12:
            value = node->value;
            FLD_WORK->mapResources[2].unk0 = value;
            FLD_WORK->mapResources[2].block = sdfMemoryGetBlockAddress(value);
            break;
        case 13:
            value = node->value;
            FLD_WORK->mapResources[3].unk0 = value;
            FLD_WORK->mapResources[3].block = sdfMemoryGetBlockAddress(value);
            break;
        }
    }
    func_00288788(lb);
}

extern s32 D_003BAF48, D_003BAF4C, D_003BAF40, D_003BAF3C;

extern s32 D_003BAF30, D_003BAF34, D_003BAF38;

extern s32 D_003BD7F0, D_003BD7D8, D_003BD7E0, D_003BD7E8;

extern s32 effCreateNodeFromDescriptor(s32);

extern s32 sdfTexAcquireResourceTexture(s32);

void fldInitializeMenuResources(void) {
    if (D_0032E3C0[0] < 200) {
        D_003BAF4C = effCreateNodeFromDescriptor(D_003BAF48);
        D_003BAF3C = effCreateNodeFromDescriptor(D_003BD7F0);
        D_003BAF40 = 0;
        D_003BAF30 = sdfTexAcquireResourceTexture(D_003BD7D8);
        D_003BAF34 = sdfTexAcquireResourceTexture(D_003BD7E0);
        D_003BAF38 = sdfTexAcquireResourceTexture(D_003BD7E8);
    }
}

extern void sdfTexReleaseReferenceViaHandler();

extern s32 D_003BAF30;

extern s32 D_003BAF34;

extern s32 D_003BAF38;

extern s32 D_003BAF3C;

extern s32 D_003BAF4C;

extern s32 D_003BAF58;

extern s32 D_003BAF64;

void fldReleaseResourceHandles(void) {
    if (D_003BAF30 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF30);
        D_003BAF30 = 0;
    }
    if (D_003BAF34 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF34);
        D_003BAF34 = 0;
    }
    if (D_003BAF38 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF38);
        D_003BAF38 = 0;
    }
    if (D_003BAF4C != 0) {
        effDestroyNode(D_003BAF4C);
        D_003BAF4C = 0;
    }
    if (D_003BAF58 != 0) {
        effDestroyNode(D_003BAF58);
        D_003BAF58 = 0;
    }
    if (D_003BAF64 != 0) {
        effDestroyNode(D_003BAF64);
        D_003BAF64 = 0;
    }
    if (D_003BAF3C != 0) {
        effDestroyNode(D_003BAF3C);
        D_003BAF3C = 0;
    }
}

typedef struct {
    s32 unk0, unk4, unk8, unkC;
    u8 pad10[0x10];
    s32 *unk20;
    u8 pad24[8];
    s32 unk2C, unk30, unk34, unk38;
    s16 unk3C, unk3E;
    char name[0x10];
} FldTblEnt50; /* 0x50 bytes */

extern FldTblEnt50 fldRoomEffectEntries[];

extern s32 fldRoomEffectEntryCount;

void fldClearMenuEntries(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        fldRoomEffectEntries[i].unk0 = 0;
        fldRoomEffectEntries[i].unk4 = 0;
        fldRoomEffectEntries[i].unk8 = 0;
        fldRoomEffectEntries[i].unkC = 0;
        fldRoomEffectEntries[i].unk20 = 0;
        fldRoomEffectEntries[i].unk2C = 0;
        fldRoomEffectEntries[i].unk30 = 0;
        fldRoomEffectEntries[i].unk34 = 0;
        fldRoomEffectEntries[i].unk38 = 0;
        fldRoomEffectEntries[i].unk3C = 0;
        fldRoomEffectEntries[i].unk3E = 0;
    }
    fldRoomEffectEntryCount = 0;
}

/* One of the 32 field object slots. Field names match the DDS2 copy of this
 * struct (src/dds2/game/code_001442D0.c), which is already reviewed. */
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

extern s32 D_003BAF70;

extern s32 D_003BAF7C;

extern s32 fldObjectSlotCount;

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
    D_003BAF70 = 0;
    D_003BAF7C = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147DB0);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0718);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0728);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001480D0);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0758);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0768);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001486D0);

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

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148D78);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0788);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0798);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148FF0);

extern s32 D_0032E4C8[];

extern f32 fldBannerColorPhase;

extern s32 ptyAnyUnitFlagMatch(s32, s32);

extern f32 sdfSinPoly(f32);

extern void func_00129178(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, u32);

void fldDrawAnimatedFieldBanner(s32 alpha, s32 x, s32 y) {
    u32 color;

    if (D_0032E4C8[0] != 1) {
        fldSelectDisplayBuffer(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitSpriteRect(x + 0x123, y, 0x7D, 0x34, 1, 0x31, 0x7D, 0x34, 0x80808080, D_003BAF30);
        fldSubmitSpriteRect(x + 0x1A0, y, 0x62, 0x34, 0x7D, 0x31, 1, 0x34, 0x80808080, D_003BAF30);
        if (ptyAnyUnitFlagMatch(0x5D0, 0) != 0) {
            color = 0x808080;
            fldSubmitSpriteRect(x + 0x17F, y + 0x28, 0x73, 0x1A, 1, 0x66, 0x73, 0x1A, 0x80808080, D_003BAF30);
            if (fldBannerColorPhase < 45.0f) {
                color = (s32)(sdfSinPoly(fldBannerColorPhase * 4.0f * 3.14f / 180.0f) * 128.0f) + 0x80;
                color |= (color << 8) | (color << 16);
            }
            func_00129900(1);
            func_00129178(x + 0x17F, y + 0x28, 0x73, 0x1A, 1, 0x66, 0x73, 0x1A, color, color | 0x5A000000, color | 0x5A000000, color, D_003BAF30);
            fldBannerColorPhase += 1.0f;
            if (fldBannerColorPhase > 90.0f) {
                fldBannerColorPhase = 0.0f;
            }
        }
        func_00129900(0);
    }
}

void fldDrawGaugeBar(s32 width) {
    s32 x;

    if (D_0032E3C0[0] < 200) {
        fldSelectDisplayBuffer(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        x = 0x9D - (width >> 1);
        fldSubmitSpriteRect(x, 0x123, 0x63, 0x2E, 2, 1, 0x63, 0x2E, 0x80808080, D_003BAF30);
        fldSubmitSpriteRect(x + 0x63, 0x123, width, 0x2E, 0x64, 1, 1, 0x2E, 0x80808080, D_003BAF30);
        x += width;
        fldSubmitSpriteRect(x + 0x63, 0x123, 0x63, 0x2E, 0x65, 1, -0x63, 0x2E, 0x80808080, D_003BAF30);
        func_00129900(0);
    }
}

extern void fldSelectDisplayBuffer(u32);

extern void func_00129900(u32);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void fldSubmitSpriteRect(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32);

void fldDrawTitleBanner(s32 x, s32 y) {
    if (D_0032E3C0[0] < 200) {
        fldSelectDisplayBuffer(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitSpriteRect(x, y, 0x10, 0x11, 0x68, 1, 0x10, 0x11, 0x80808080, D_003BAF30);
        func_00129900(0);
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149810);

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

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149B68);

extern s32 fldRoomEffectEntryCount;

extern s32 fldTestMapSlotAuxiliaryFlag();

extern void dds3SetObjectFlags();

void fldFireRoomEffects(void) {
    s32 i;

    for (i = 0; i < fldRoomEffectEntryCount; i++) {
        s32 room = fldRoomEffectEntries[i].unk38;
        if (room != 0 && fldTestMapSlotAuxiliaryFlag(fldAreaState[4], fldAreaState[5] + 1, room) != 0) {
            if (fldRoomEffectEntries[i].unk20 != 0) {
                dds3SetObjectFlags(fldRoomEffectEntries[i].unk20, 1);
            }
        }
    }
}

extern u8 D_003D44A0[0x200];

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 sdfDevCreateCommandState(const char *);

extern u32 sdfDevQueueReadAndWait(u32, void *, u32);

extern void sdfDevWaitThenReleaseCommandState(u32);

void fldLoadNpcPalette(s32 field) {
    char path[64];
    char directory[32];
    u32 command;

    if (field < 100) {
        fldFormatAreaDirectory(directory, field, 1);
        func_003014F0(path, "%sF%03d.NPL", directory, field);
        command = sdfDevCreateCommandState(path);
        sdfDevQueueReadAndWait(command, D_003D44A0, 0x200);
        sdfDevWaitThenReleaseCommandState(command);
    }
}

void fldSetNpcPalette(void *src) {
    memcpy(D_003D44A0, src, 0x200);
}

extern int strcmp(const char *, const char *);

s32 fldFindEffectByName(const char *name) {
    s32 i;

    for (i = 0; i < fldRoomEffectEntryCount; i++) {
        if (fldRoomEffectEntries[i].unk0 != 999 && strcmp(fldRoomEffectEntries[i].name, name) == 0) {
            return fldRoomEffectEntries[i].unk20[1];
        }
    }
    return 0;
}

s32 fldGetCurrentSceneSelectionResource(void) {
    s32 index = fldGetCurrentSceneSelectionId();

    if (index < 0) {
        return 0;
    }
    return D_003D46C0[index].unk0[1];
}

void func_0014A298(u32 value) {
    D_003BAFB0 = value;
}

s32 fldTitleIsActive(void) {
    return kwlnTaskGetTaskByName(fldTitleTaskName) != 0;
}

void func_0014A2C8(void) {
    D_003BAFA8 = 0;
    D_003BAFA0 = 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", fldTitle);

void fldReleaseTitleTextureReference(void) {
    if (D_003BAFB4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFB4);
        D_003BAFB4 = 0;
    }
}

extern void fldTitle(void);

extern void sdfReleaseResourceAllocation(s32);

extern u32 D_003BAFA4;

extern u32 D_003BAFAC;

extern u32 D_003BAF9C;

extern s32 sdfReadNamedResource();

extern char D_003A0810[]; /* "/fld/f/pnl/df%03d.tmx" */

void fldStartTitle(s32 field, s32 mode, s32 option) {
    char path[32];
    s32 size;
    s32 handle;

    D_003BAFA8 = mode;
    D_003BAFAC = option;
    D_003BAFA4 = field;
    D_003BAF9C = 0;
    D_003BAFA0 = 0;
    D_003BAFB0 = 0;
    func_003014F0(path, D_003A0810, field);
    handle = sdfReadNamedResource(path, &size, 0);
    D_003BAFB4 = sdfTexAcquireResourceTexture(size);
    sdfReleaseResourceAllocation(handle);
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(fldTitleTaskName, 0x2B0A, 0, 1, fldTitle, fldReleaseTitleTextureReference, 0);
    }
}

void fldDestroyTitleTask(void) {
    if (fldTitleIsActive() != 0) {
        kwlnTaskDestroyWithHierarchyByName(fldTitleTaskName, 1);
    }
}

s32 fldTitleMiniIsActive(void) {
    return kwlnTaskGetTaskByName(D_003A0828) != 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", fldTitleMini);

void fldReleaseTitleMiniTexture(void) {
    if (D_003BAFC4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFC4);
        D_003BAFC4 = 0;
    }
}

extern void fldTitleMini(void);

INCLUDE_RODATA(const s32, "game/code_001411F0", fldTitleTaskName);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0810);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0824);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0828);

void fldStartMiniTitleForUnlock(s32 id) {
    char path[0x20];
    s32 data;
    s32 handle;

    switch (id) {
    case 22:
        if (mdlFlagTest(0x440) == 0) {
            return;
        }
        break;
    case 23:
        if (mdlFlagTest(0x480) == 0) {
            return;
        }
        break;
    case 24:
        if (mdlFlagTest(0x4C0) == 0) {
            return;
        }
        break;
    case 25:
        if (mdlFlagTest(0x500) == 0) {
            return;
        }
        break;
    case 26:
        if (mdlFlagTest(0x560) == 0) {
            return;
        }
        break;
    case 27:
        if (mdlFlagTest(0x580) == 0) {
            return;
        }
        break;
    case 28:
        if (mdlFlagTest(0x5A0) == 0) {
            return;
        }
        break;
    case 29:
        if (mdlFlagTest(0x600) == 0) {
            return;
        }
        break;
    case 30:
        if (mdlFlagTest(0x620) == 0) {
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
    D_003BAFC0 = id;
    D_003BAFB8 = 0;
    D_003BAFBC = 0;
    func_003014F0(path, "/fld/f/pnl/ds%03d.tmx", id);
    handle = sdfReadNamedResource(path, &data, 0);
    D_003BAFC4 = sdfTexAcquireResourceTexture(data);
    sdfReleaseResourceAllocation(handle);
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(D_003A0828, 0x2B0A, 0, 1, fldTitleMini, fldReleaseTitleMiniTexture, 0);
    }
}

static const char s_fieldWeatherLimitPath[] __attribute__((aligned(16))) = "/fld/f/bin/limit_00.tmx";

void fldRequestMiniTitleDismiss(void) {
    if (D_003BAFBC == 0) {
        D_003BAFBC = 1;
    }
}

extern s32 sdfReadNamedResource();

extern s32 sdfTexAcquireResourceTexture();

extern s32 func_0014FE28();

void fldLoadWeatherEffects(void) {
    s32 handle;
    s32 size;

    handle = sdfReadNamedResource(s_fieldWeatherLimitPath, &size, 0);
    fldWeatherLimitTexture = sdfTexAcquireResourceTexture(size);
    sdfQueueNonzeroResourceId(handle);
    fldDamEffectResource = sdfReadNamedResource("/fld/f/bin/FH_DAM_2.EPL", &fldDamEffectData, 0);
    fldDamEffectNode = func_0014FE28(fldDamEffectData);
    fldDamEffectPositioned = 0;
    fldYukEffectResource = sdfReadNamedResource("/fld/f/bin/YUK_2.EPL", &fldYukEffectData, 0);
    fldYukEffectNode = func_0014FE28(fldYukEffectData);
    fldYukEffectPositioned = 0;
}

void fldReleaseWeatherEffects(void) {
    if (fldWeatherLimitTexture != 0) {
        sdfTexReleaseReferenceViaHandler(fldWeatherLimitTexture);
        fldWeatherLimitTexture = 0;
    }
    effDestroyNode(fldDamEffectNode);
    fldDamEffectNode = 0;
    fldDamEffectPositioned = 0;
    sdfQueueNonzeroResourceId(fldDamEffectResource);
    fldDamEffectResource = 0;
    fldDamEffectData = 0;
    effDestroyNode(fldYukEffectNode);
    fldYukEffectNode = 0;
    fldYukEffectPositioned = 0;
    sdfQueueNonzeroResourceId(fldYukEffectResource);
    fldYukEffectResource = 0;
    fldYukEffectData = 0;
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

void *fldFindFieldEntryByKeyPair(s32 key0, s32 key1) {
    FldEnt110 *entry = D_003BAA48;
    s32 index = 0;

    while (index < 8) {
        if (entry->unk0 == key0) {
            if (entry->unk2 == key1) {
                return entry;
            }
        }
        index++;
        entry++;
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08E8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08F8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B688);

void fldClearObjectEntryHandles(void) {
    FldEnt14 *entry = fldSparkObjectEntries;
    s32 i = 0x10;

    do {
        s32 temp = entry->unk0;

        i--;
        if (temp != 0) {
            ddsReleaseUnitObject(temp);
            entry->unk0 = 0;
        }
        entry++;
    } while (i >= 0);
}

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

extern s16 D_0032E4B4[];

void fldInitSparkTable(void) {
    s32 i;

    if (D_0032E4B4[0] == 0) {
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

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BA50);

extern void effObjSetInnerFirstVec();

extern s32 *dds3GetObjectBaseResourceHandle();

extern void dds3ClearObjectFlags();

extern FldVec4 D_003A0918;

void fldFreeSparkSlot(s32 index) {
    FldVec4 vec;
    s32 obj;
    s32 *flags;
    s16 slot;

    vec = D_003A0918;
    if (fldSparkSlots[index].hasVectors == 1 && fldSparkSlots[index].active != 0 && fldSparkSlots[index].objectSlot != -1) {
        vec.v[0] = fldSparkSlots[index].pos[0];
        vec.v[2] = fldSparkSlots[index].pos[2];
        effObjSetInnerFirstVec(fldSparkObjectEntries[fldSparkSlots[index].objectSlot].unk0, &vec);
        obj = fldSparkObjectEntries[fldSparkSlots[index].objectSlot].unk0;
        flags = dds3GetObjectBaseResourceHandle(obj);
        *flags |= 1;
        dds3ClearObjectFlags(obj, 0x400);
        slot = fldSparkSlots[index].objectSlot;
        fldSparkSlots[index].objectSlot = -1;
        fldSparkObjectEntries[slot].unk4 = -1;
    }
}

extern s32 fldSparkControlState[];

extern void func_0014B688();

extern void func_0014BA50();

void fldUpdateSparkSlots(void) {
    s32 i;

    func_0014B688();
    for (i = 0; i < 64 && i < fldSparkControlState[12]; i++) {
        if (fldSparkSlots[i].hasVectors != 0 && fldSparkSlots[i].active != 0) {
            func_0014BA50(i, fldSparkSlots[i].unk28);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BDF8);

extern s32 fldSparkControlState[];

s32 fldIsNearSpark(f32 x, f32 y, f32 z) {
    s32 i;

    for (i = 0; i < 64 && i < fldSparkControlState[12]; i++) {
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

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C210);

extern s32 fldSparkControlState[];

extern void evtStoreValueAndCaptureWindowPanelValue(s32);

extern void evtSetMessageWindowOptionWhenOpen(s32);

extern void mdlFlagSet(s32);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C468);

extern s32 D_0032E5C4[];

extern s32 fldSparkControlState[];

extern void func_001239C8(void);

extern void fldClearObjectEntryHandles(void);

extern void fldReleaseWeatherEffects(void);

extern void fldStartSceneBgmAlternate(void);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void evtSetSolarOverlayFullyVisible(void);

/* Restores field presentation once an active event finishes, then clears the
 * event latch. An already-cleared latch leaves field resources untouched. */
void fldFinishEventFieldState(void) {
    FldAreaWork *state = (FldAreaWork *)fldAreaState;

    if (state->eventActive != 0) {
        D_0032E5C4[0] = 0;
        func_001239C8();
        fldClearObjectEntryHandles();
        fldReleaseWeatherEffects();
        fldStartSceneBgmAlternate();
        fldPreparePlayerSceneCameraTarget();
        state->eventActive = 0;
        fldSparkControlState[2] = 0;
        fldSparkControlState[3] = 0;
        evtSetSolarOverlayFullyVisible();
    }
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0918);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0928);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0978);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C648);

s32 func_0014CAF8(void) {
    return D_003D62C8[0];
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014CB08);

/* Releases scene presentation and camera resources before clearing the event
 * latch and installing the existing field-state values. */
void fldResetEventSceneState(void) {
    fldReleaseCameraModel(0);
    D_0032E5C4[0] = 0;
    FLD_WORK->unk118 = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldPreparePlayerSceneCameraTarget();
    FLD_WORK->unk114 = 1;
    FLD_WORK->eventActive = 0;
    D_003D62A8[0] = 0;
    FLD_WORK->unk138 = 1;
    evtSetSolarOverlayFullyVisible();
}

void fldStartDeferredFieldExit(void) {
    evtCreateMessageWindowIfMissing(D_0034D8F0);
    dspStartEntry(5);
    fldResetPlayerSceneObjectState();
    D_0032E4C4[0] = 2;
}

/* Completes deferred exit state 2 only after the message window stops being
 * controlled, then closes its display channel and restores the camera target. */
void fldFinishDeferredExit(void) {
    if (FLD_WORK->unk114 == 2) {
        func_0024DD78();
        if (!evtGetMessageWindowControlState()) {
            evtFinishMessageWindowAndNotify();
            dspCloseChannel();
            fldPreparePlayerSceneCameraTarget();
            FLD_WORK->unk114 = 0;
        }
    }
}

s32 fldIsEventPhaseAtLeastTwo(void) {
    if (D_003D62A8[0] < 2) {
        return 0;
    }
    return 1;
}

void fldTickWeatherEffectNodes(void) {
    fldUpdateWeatherEffectNodes();
}

/* Script execution state retains the task id used by the room lookup. */
typedef struct FldScriptTask {
    u8 pad00[0xE4];
    s32 taskId; /* 0xE4 */
} FldScriptTask;

s32 func_0014D100(void) {
    return *(s16 *)((u8 *)D_003D62A4[0] + 0xC);
}
INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE78);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldCurrentBgmHandle);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneSoundBase);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldPendingSoundCount);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE90);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE94);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldCurrentBgmMode);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneBgmArchiveTrack);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneBgmArchivePhase);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldArchiveLoadPending);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldFixedArchiveLoadPhase);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEAC);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldAreaFlagIndex);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEB4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEB8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEBC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAECC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAED0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAED4);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneReady);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneRecords);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneRecordCount);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSceneRecordResource);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEE8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEEC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEFC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF00);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF04);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF08);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF0C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF10);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF14);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF18);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF1C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF20);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF24);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF28);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldRoomEffectEntryCount);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF30);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF34);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF38);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF3C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF40);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF44);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF48);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF4C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF50);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF54);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF58);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF5C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF60);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF64);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF68);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF6C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF70);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF74);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF78);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF7C);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldObjectSlotCount);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF84);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF88);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldPrimaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldSecondaryEffectPositionPending);

INCLUDE_SDATA(const s32, "game/code_001411F0", mnuPositionedResourceCursor);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldBannerColorPhase);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF9C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFAC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFBC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFC0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFC4);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldDamEffectResource);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldDamEffectData);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldDamEffectNode);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldDamEffectPositioned);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldYukEffectResource);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldYukEffectData);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldYukEffectNode);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldYukEffectPositioned);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFE8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFEC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFF0);

INCLUDE_SDATA(const s32, "game/code_001411F0", fldWeatherLimitTexture);

