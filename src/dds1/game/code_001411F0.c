#include "fld_area_work.h"
#include "evt_world.h"
#include "common.h"
#include "btl_stage_task_cleanup.h"
#include "sdf_textured_rect.h"
#include "sdf_motion.h"
#include "mdl.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "sdf_dev_state.h"
#include "sdf_resource.h"
#include "eff_transform.h"
#include "sdf_model.h"
#include "fld_scene_record.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_node_descriptor.h"
#include "kwln.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_texture_file.h"

extern u8 D_00324510[2][2][16];
extern u8 D_003BA878[2][2];
extern f32 sdfViewEyeVector[4];
extern f32 sdfViewTargetVector[4];
extern f32 sdfViewUpVector[4];
extern u32 fldGetSceneReadyFlag(void);
extern void kwlnFadeStartOut(s32);
extern void kwlnFadeStartIn(s32);
extern void fldGetVisibleSceneBounds(f32 *, f32 *, f32 *, f32 *);






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

extern SdfTex *fldWeatherLimitTexture;

extern s32 D_003BAFBC;

extern SdfTex *D_003BAFC4;

extern s32 D_003BAFB8;

extern s32 D_003BAFC0;

extern SdfTex *D_003BAFB4;

extern u32 D_003BAFA0;

extern u32 D_003BAFA8;

extern u32 D_003BAFB0;


extern u32 fldSceneReady;

extern u32 fldFixedArchiveLoadPhase;

extern u32 fldArchiveLoadPending;

extern u32 fldCurrentBgmHandle;

extern u32 fldFieldTaskHandle;

extern s32 fldPendingSoundCount;

extern s32 fldAreaFlagIndex;

extern s32 D_003BAEB4;



extern s32 fldPrimaryEffectPositionPending;

extern s32 fldSecondaryEffectPositionPending;

extern s32 D_0032E474[];

extern s32 D_0033EB78[];

extern void func_002E96D8(s32 arg0);


extern s32 dds3GetWorldObject(void);


typedef struct FldTransferChunk {
    u32 unk0;
    s32 offset;
    u32 size;
} FldTransferChunk;

extern void fldRelocatePackedTransferChunk(u32 buffer, FldTransferChunk *chunk);

extern s32 func_001277A8(s32 arg0);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void fldReleaseCameraModel(u32 enabled);

extern void evtSetSolarOverlayFullyVisible(void);

struct FileRequest;

struct SdfMemBlock;
extern s8 D_00324530[];
extern s32 D_0032E4C0[];
extern s32 D_0032E4E8[];
extern struct FileRequest *D_003BAFE8;
extern u32 D_003BAFEC;
extern u32 D_003BAFF0;
extern void func_0012EA40(u32, u32);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern s32 evtGetMessageWindowControlState(void);
extern void evtFinishMessageWindowAndNotify(void);
extern s32 dspCloseChannel(void);
extern void mnuAdvanceTitleStateUnderSemaphore(void);
extern void *func_00115298(void *, f32 *, f32 *);
extern void effObjClearFlags(void *, s32);
extern void effObjSetFlags(void *, s32);
extern void effObjReplaceActiveEventNode(void *, u32);
extern void mnuMarkTitleStreamResetPending(void);
extern void fldSetCameraNodeModeWithTen(void);
extern u8 D_003A0978[];
extern void sdfQueueGeneralAllocationRelease(struct SdfMemBlock *);

extern s32 D_0032E5C4[];

extern s32 D_0032E4C4[];

extern u8 D_0034D8F0[];

extern s32 evtCreateMessageWindowIfMissing(s32 unused);

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



extern s32 fldFieldTaskUpdate(void);

extern s32 D_0032E3C0[];

extern s32 fldSceneSoundBase;

extern void ddsReleaseUnitObject(EffWorldNode *node);

typedef struct {
    EffWorldNode *objectHandle;
    s32 unk4;
    u8 pad8[0xC];
} FldEnt14; /* 0x14 bytes */

extern FldEnt14 fldSparkObjectEntries[];

/* Each DDS1 resource row owns sixteen 16-byte spark sequences. */
typedef struct FldSparkSequence {
    s32 count;
    s8 slots[12];
} FldSparkSequence;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 entryCount;            /* 0x04 */
    s32 duration;              /* 0x08 */
    s16 unk0C;
    s16 reservedModel;         /* 0x0E: model for reserved object slot 16. */
    FldSparkSequence sequences[16]; /* 0x10 */
} FldEnt110; /* 0x110 bytes */

typedef char FldSparkSequenceSizeCheck[(sizeof(FldSparkSequence) == 0x10) ? 1 : -1];
typedef char FldEnt110SizeCheck[(sizeof(FldEnt110) == 0x110) ? 1 : -1];

extern FldEnt110 *D_003BAA48;

extern s32 fldGetCurrentSceneSelectionId(void);

typedef struct {
    s32 *unk0;
    u8 pad4[0x4C];
} FldTbl50; /* 0x50 bytes */

extern FldTbl50 D_003D46C0[];


extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern u8 D_003BAE78[];


typedef struct {
    s32 flags;
    s32 soundId;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FldClear18; /* 0x18 bytes */

extern FldClear18 fldPendingSounds[];


extern void fldSelectDisplayBuffer(u32);

extern void func_00129900(s32);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void fldSubmitSpriteRect(s32, s32, s32, s32, s32, s32, s32, s32, s32, SdfTex *);

typedef struct FldTitleBannerMenu {
    u16 position; /* 0x00 */
    u16 choice;   /* 0x02 */
    u16 pending;  /* 0x04 */
    u16 reserved; /* 0x06 */
} FldTitleBannerMenu;

extern u32 fldInputPanelTaskHandle;
extern u32 D_0032E570[];
extern s32 D_003BAE40;
extern u8 fldGetCampSceneControlMode(void);
extern u8 fldGetSceneReadyOrPendingState(void);
extern s32 fileMenuTaskExists(void);
extern u8 fldHasKiretaLabelProcess(void);
extern u8 fldHasHirakenaiLabelProcess(void);
extern u8 fldHasBadkaifukuLabelProcess(void);
extern s32 fldIsEventPhaseAtLeastTwo(void);
extern void fldApplyCameraFacingPoint(void);

s32 func_001411F0(void) {
    FldTitleBannerMenu *menu;
    FldAreaWork *area;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (fldGetSceneReadyOrPendingState() != 0) {
        return 0;
    }
    if (D_0032E570[0] != 0) {
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

    menu = (FldTitleBannerMenu *)kwlnTaskGetUserValue((KwlnTask *)fldInputPanelTaskHandle);
    if (menu->pending == 0) {
        return -1;
    }

    if (fldAreaState.titleFade != 0 || D_003BAE40 == 1 || (s8)D_00324510[1][0][1] < 0) {
        if ((s8)D_00324510[1][0][1] < 0) {
            fldApplyCameraFacingPoint();
        }
        menu->pending = 0;
        area = &fldAreaState;
        if (area->titleFade != 0) {
            kwlnFadeStartIn(8);
            area->titleFade = 0;
        }
        return 1;
    }
    return 0;
}

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

void *fldFieldTaskCreate(KwlnTask *task) {
    s32 *work;

    work = sdfAllocSizeClassBlock(0x10);
    work[0] = 0;
    work[1] = 0;
    work[2] = 0;
    work[3] = 0;
    kwlnTaskSetUserValue(task, (u32)work);
    return fldFieldTaskUpdate;
}

void fldFieldTaskDestroy(KwlnTask *task) {
    void *work;

    work = (void *)kwlnTaskGetUserValue(task);
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
        kwlnTaskDestroyWithHierarchy((KwlnTask *)fldFieldTaskHandle, 1);
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
    s32 handle = fldResolveSpecialBgmTrack(fldAreaState.sequenceCode);
    s32 stage;
    s32 idx;

    if (handle != -1 || fldAreaState.area < 0x32) {
        if (handle == -1) {
            stage = fldAreaState.area;
            if (stage == 11) {
                stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, fldAreaState.floor + 1);
            fldSceneSoundBase = fldStageSoundBaseTable[stage];
            fldAreaState.sequenceCode = idx;
            handle = fldSceneSoundBase + idx;
        }
        if (fldAreaState.unk130 != 1) {
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
    s32 handle = fldResolveSpecialBgmTrack(fldAreaState.sequenceCode);
    s32 stage;
    s32 idx;

    if (handle != -1 || fldAreaState.area < 0x32) {
        if (handle == -1) {
            stage = fldAreaState.area;
            if (stage == 11) {
                stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, fldAreaState.floor + 1);
            fldSceneSoundBase = fldStageSoundBaseTable[stage];
            fldAreaState.sequenceCode = idx;
            handle = fldSceneSoundBase + idx;
        }
        if (fldAreaState.unk130 != 1) {
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
    if (fldAreaState.sequenceCode >= 0x80) {
        fldAreaState.sequenceCode = 1;
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
    s32 handle = fldResolveSpecialBgmTrack(fldAreaState.sequenceCode);
    s32 stage;
    s32 idx;

    if (handle == -1) {
        if (fldAreaState.area >= 0x32) {
            return;
        }
        stage = fldAreaState.area;
        if (stage == 11) {
            stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
        }
        idx = fldFindSceneEntryData(stage, fldAreaState.floor + 1);
        fldSceneSoundBase = fldStageSoundBaseTable[stage];
        fldAreaState.sequenceCode = idx;
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

extern s32 mdlFlagTest(s32);
extern s32 func_003014F0(char *, const char *, ...);
extern void fldFormatAreaDirectory(char *, s32, s32);

/* The base format occupies the first 16-byte-aligned AMB format record. */
const char D_003A04B0[16] __attribute__((aligned(16))) = "%sf%03d.amb";

void fldLoadAreaModelTransfers(void) {
    char path[128];
    char directory[64];
    u32 resourceAddress;
    s32 transferStart;
    s32 area;
    s32 room;
    u32 *header;
    s32 rows;
    s32 count;

    fldFormatAreaDirectory(directory, fldAreaState.area, 1);
    area = fldAreaState.area;
    if (area == 0x1C) {
        if (mdlFlagTest(0x5E1)) {
            func_003014F0(path, D_003A04B0, directory, fldAreaState.area);
        } else if (mdlFlagTest(0x5E2)) {
            func_003014F0(path, "%sf%03da.amb", directory, fldAreaState.area);
        } else if (mdlFlagTest(0x5E3)) {
            func_003014F0(path, "%sf%03db.amb", directory, fldAreaState.area);
        } else if (mdlFlagTest(0x5E4)) {
            func_003014F0(path, "%sf%03dc.amb", directory, fldAreaState.area);
        } else {
            func_003014F0(path, "%sf%03dd.amb", directory, fldAreaState.area);
        }
    } else if (area == 0x1B) {
        room = fldAreaState.floor;
        switch (room) {
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            func_003014F0(path, "%sf%03da.amb", directory, fldAreaState.area);
            break;
        case 7:
            func_003014F0(path, "%sf%03db.amb", directory, fldAreaState.area);
            break;
        case 22:
        case 23:
        case 27:
            if (mdlFlagTest(0x58D)) {
                func_003014F0(path, "%sf%03dd.amb", directory, fldAreaState.area);
            } else {
                func_003014F0(path, "%sf%03dc.amb", directory, fldAreaState.area);
            }
            break;
        case 24:
        case 25:
        case 26:
        default:
            func_003014F0(path, D_003A04B0, directory, fldAreaState.area);
            break;
        }
    } else {
        func_003014F0(path, D_003A04B0, directory, fldAreaState.area);
    }

    fldSceneRecordResource = sdfReadNamedResource(path, &resourceAddress, 0);
    transferStart = resourceAddress + 8;
    fldRelocatePackedTransferChunk(resourceAddress, (FldTransferChunk *)transferStart);
    header = (u32 *)func_001277A8(transferStart);
    rows = header[1];
    count = header[2];
    fldSceneRecordCount = count;
    fldSceneRecords = (FldSceneRecord *)(u32)rows;
}

void fldSetSceneRecordChunk(u8 *chunk, s32 resourceHandle) {
    /* Descriptor from func_001277A8 precedes the 0x14-byte scene rows. */
    typedef struct {
        u8 pad00[4];
        s32 rows; /* 0x04: first scene row */
        s32 count; /* 0x08: number of scene rows */
    } SceneHeader;
    if (D_0032E3C0[0] < 0xC8) {
        u8 *transferStart = chunk + 8;

        fldSceneRecordResource = (struct SdfMemBlock *)(u32)resourceHandle;
        fldRelocatePackedTransferChunk((u32)chunk, (FldTransferChunk *)transferStart);
        {
            s32 header = func_001277A8((s32)(u32)transferStart);
            s32 rows = ((SceneHeader *)header)->rows;
            s32 count = ((SceneHeader *)header)->count;

            fldSceneRecordCount = count;
            fldSceneRecords = (FldSceneRecord *)(u32)rows;
        }
    }
}

void fldInitSceneMapLabels(void) {
    s32 sceneId = D_0032E3C0[0];

    if (sceneId < 0xC8) {
        fldCacheMapLabelLengths(sceneId % 100);
        fldLoadAreaModelTransfers();
    }
}

void fldReleaseSceneRecordChunk(void) {
    if (fldSceneRecordResource != 0) {
        sdfQueueGeneralAllocationRelease(fldSceneRecordResource);
    }
    fldSceneRecordResource = 0;
    fldSceneRecords = 0;
    fldSceneRecordCount = 0;
}

typedef struct FldSlot {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
} FldSlot;

extern SdfTex *D_003D40A0[4];
extern SdfModel *D_00348F30[];
extern FldSlot D_003D40B0[];

void func_001426E0(void) {
    s32 i;

    fldAreaState.unkC4 = -1;
    fldAreaState.unkC8 = -1;
    fldSceneRecordResource = 0;
    fldSceneRecords = 0;
    fldSceneRecordCount = 0;
    D_003D40A0[0] = 0;
    D_003D40A0[1] = 0;
    D_003D40A0[2] = 0;
    D_003D40A0[3] = 0;
    for (i = 0; i < 64; i++) {
        D_00348F30[i] = 0;
        D_003D40B0[i].unk_0 = 0;
        D_003D40B0[i].unk_4 = 0;
        D_003D40B0[i].unk_8 = 0;
    }
}


extern SdfPoolNode *D_00325838[];

extern void sdfDrawNodeBuildMatrix(SdfDrawNode *);

extern void sdfModelUpdateCurrentFrameTransforms(SdfModel *);

extern void func_002D9238(SdfPoolNode **, SdfModel *);

void fldSetEmitterPosition(SdfModel *emitter, f32 x, f32 y, f32 z) {
    f32 pos[4];
    SdfDrawNode *handle;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    handle = *(SdfDrawNode **)emitter->list->buffer;
        VU0_LOAD_VF_MEMORY(vf10, pos);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, emitter->matrix[3]);
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
extern SdfTex *D_003D40A0[4];

void func_00142800(f32 x, f32 y, s32 index, u32 color) {
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

s32 fldFindRecordItem(s32 scene, u32 index) {
    s32 result = 1;
    FldSceneRecord *rec = fldSceneRecords;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)fldSceneRecordCount; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (i == scene && j == index) {
                result = item->floor + 1;
            }
        }
    }
    return result;
}


/* Model request buffers contain the root followed by indexed draw nodes. */
void func_00142C78(SdfModel *model, s32 index, s32 clearFlag) {
    SdfDrawNode *parent = ((SdfDrawNode **)model->list->buffer)[index + 1];
    SdfDrawNode *first;
    SdfDrawNode *node;
    SdfDrawNode *next;
    SdfDrawNode *headNext;
    u16 flags;

    if (parent->children == NULL) {
        return;
    }
    parent->children->flags &= ~SDF_DRAW_NODE_FLAG_SKIP_RENDER;
    if (clearFlag == 0) {
        parent->children->flags |= SDF_DRAW_NODE_FLAG_SKIP_RENDER;
    }

    first = parent->children;
    headNext = first->next;
    if (headNext == NULL) {
        return;
    }
    node = headNext;
    if (parent != node->parent || first == node) {
        return;
    }
    for (;;) {
        flags = node->flags & ~SDF_DRAW_NODE_FLAG_SKIP_RENDER;
        node->flags = flags;
        if (clearFlag == 0) {
            node->flags = flags | SDF_DRAW_NODE_FLAG_SKIP_RENDER;
        }
        next = node->next;
        if (next == NULL) {
            return;
        }
        node = next;
        if (parent != node->parent) {
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
    FldSceneRecord *rec = fldSceneRecords;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)fldSceneRecordCount; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (max < item->floor) {
                max = item->floor;
            }
        }
    }
    return max + 1;
}

#include "field_stage.h"
typedef struct FrFontGlyph FrFontGlyph;
extern FrFontGlyph *D_003BD7CC;
extern s32 D_003BAEAC, D_003BAEB8, D_003BAEBC, D_003BAEC0;
extern s32 D_003BAEF0, D_003BAEF4, D_003BAF00, D_003BAF04, D_003BAF08;
extern f32 D_003BAEF8, D_003BAEFC, D_003BAF0C, D_003BAF10, D_003BAF14;
extern u8 D_00324510[2][2][16];
extern f32 D_00329790[4][4], sdfViewMatrix[4][4];
extern f32 D_00324A20[4] __attribute__((aligned(16)));
extern SdfModel *D_00348F30[];
extern void frFontSetSharedRenderFlags(u32);
extern void sdfInvertScaledVuTransform(void);
extern s32 fldFindNextMarkedValue(s32), fldFindPreviousMarkedValue(s32);
extern void fldSubmitGsQuadTagged(s32, s32, s32, s32, u32, u32, u32, u32);
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitQuadFromVertices(f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,f32,u32,u32,u32,u32);
extern s32 fldTestStageExplorationCell(s32, f32, f32);
extern s32 fldGetFloorFlag(s32, s32, s32);
extern void fldSubmitPrimaryFramePacket(void), fldSubmitAlternateFramePacket(void);
extern void fldGetSceneEntryPosition(s32, f32 *, f32 *);
extern void fldProjectPointSetupAlt(f32 *, f32 *, f32, f32, f32);
extern void func_001428C0(s32, f32, f32, f32);
extern void fldGetVisibleSceneBounds(f32 *, f32 *, f32 *, f32 *);
extern s32 fldFindMapCoordinateIndex(s32, s32);
extern f32 sdfSinPoly(f32);
extern FrFontGlyph *itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, FrFontGlyph *);
extern void frFontStoreShiftedRenderValue(FrFontGlyph *, u32);
extern s32 frFontDrawGlyphInDefaultMode(FrFontGlyph *);
extern s32 frFontDrawGlyphWithSharedFlags(FrFontGlyph *, s8);
extern s32 frFontQueueGlyphForCurrentDrawBuffer(FrFontGlyph *);


void func_00142D78(void) {
    s32 proposed;
    s32 selectedFloor;
    s32 itemVisible;
    s32 i, j, k;
    FldSceneRecord *record;
    FldItem *item;
    FieldStageCoordinate *stage;
    f32 left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
    f32 sceneX, sceneZ;
    f32 playerX, playerY;
    f32 minX, maxZ, maxX, minZ;
    f32 focus[4] __attribute__((aligned(16)));
    s32 labelAlpha;
    u32 labelColor;

    proposed = fldGetMaxItemValue();
    frFontSetSharedRenderFlags(0x5E);
    VU0_LOAD_MATRIX(D_00329790);
    sdfInvertScaledVuTransform();
    D_003BAEF0 = D_003BAEB8;
    if ((s8)D_00324510[1][0][8] < 0 && D_003BAEF0 < proposed) {
        proposed = D_003BAEF0 + 1;
        if (!D_003BAEAC) proposed = fldFindNextMarkedValue(D_003BAEF0);
        if (D_003BAEF0 != proposed) {
            sndSetSequenceVolumePan(2, 127, 63);
            D_003BAEF4 = D_003BAEF0;
            D_003BAEF0 = proposed;
            D_003BAEF8 = -2500.0f;
            D_003BAF04 = -24;
            D_003BAEFC = 0.0f;
            D_003BAF00 = 0;
            D_003BAF08 = 0;
        }
    }
    if ((s8)D_00324510[1][0][10] < 0 && D_003BAEF0 >= 2) {
        proposed = D_003BAEF0 - 1;
        if (!D_003BAEAC) proposed = fldFindPreviousMarkedValue(D_003BAEF0);
        if (D_003BAEF0 != proposed) {
            sndSetSequenceVolumePan(2, 127, 63);
            D_003BAEF4 = D_003BAEF0;
            D_003BAEF0 = proposed;
            D_003BAEF8 = 2500.0f;
            D_003BAF04 = 24;
            D_003BAEFC = 0.0f;
            D_003BAF00 = 0;
            D_003BAF08 = 0;
        }
    }
    if (D_003BAF00 < 128) D_003BAF00 += 16;
    if (D_003BAF04 < 0) { D_003BAF04 += 4; D_003BAF08 += 4; }
    if (D_003BAF04 > 0) { D_003BAF04 -= 4; D_003BAF08 -= 4; }
    if (D_003BAEF8 < 1000.0f && D_003BAEF8 > -1000.0f) {
        D_003BAEF8 = 0.0f;
    } else if (D_003BAEF8 > 0.0f) {
        D_003BAEF8 -= 500.0f; D_003BAEFC -= 500.0f;
    } else if (D_003BAEF8 < 0.0f) {
        D_003BAEF8 += 500.0f; D_003BAEFC += 500.0f;
    } else { D_003BAEF8 = 0.0f; }
    D_003BAEB8 = D_003BAEF0;
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, D_00329790);
    fldSelectDisplayBuffer(0x56);
    func_00129900(0);
    fldSubmitFrameQuad(1, 0, 128, 1, 0, 0, 1, 1);
    fldSubmitGsQuadTagged(0, 0, 512, 224, 0, 0, 0, 0);
    fldSelectDisplayBuffer(0x57);
    fldSubmitFrameQuad(1, 0, 128, 1, 0, 0, 1, 1);
    fldSelectDisplayBuffer(0x56);
    fldSubmitFrameQuad(1, 1, 128, 1, 0, 0, 1, 1);
    stage = fldFindStageCoordinateRecord(fldAreaState.area, D_003BAEF0);
    if (stage != NULL) {
        s32 run;
        f32 step = (f32)stage->cellSize;
        u32 color;
        evtSetDrawSurfaceIndex(0x56);
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        color = D_003BAEAC ? 0x80000080 : 0x80000000;
        for (j = 0, sceneZ = stage->originZ; j < stage->rows;
             j++, sceneZ -= step) {
            if (sceneZ > (f32)D_003BAEC0 + 8400.0f) continue;
            if (sceneZ < (f32)D_003BAEC0 - 8400.0f) break;
            run = 0;
            for (i = 0, sceneX = stage->originX; i < stage->cols;
                 i++, sceneX += step) {
                if (sceneX < (f32)D_003BAEBC - 11400.0f) continue;
                if (sceneX > (f32)D_003BAEBC + 11400.0f) break;
                if (fldTestStageExplorationCell(D_003BAEF0, sceneX, sceneZ)) {
                    if (run == 0) { left = sceneX - 50.0f; top = sceneZ + 50.0f; }
                    bottom = sceneZ - step - 50.0f;
                    right = sceneX + step + 50.0f;
                    run++;
                } else {
                    if (run > 0) evtSubmitQuadFromVertices(left, D_003BAEF8 - 10.0f, top,
                    right, D_003BAEF8 - 10.0f, top,
                    left, D_003BAEF8 - 10.0f, bottom,
                    right, D_003BAEF8 - 10.0f, bottom, color, color, 0x80000000, 0x80000000);
                    run = 0;
                }
            }
            if (run > 0) evtSubmitQuadFromVertices(left, D_003BAEF8 - 10.0f, top,
                    right, D_003BAEF8 - 10.0f, top,
                    left, D_003BAEF8 - 10.0f, bottom,
                    right, D_003BAEF8 - 10.0f, bottom, color, color, 0x80000000, 0x80000000);
            run = 0;
        }
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 1, 1, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(0);
    }


    if (D_003BAEAC != 0) {
        fldSubmitFrameQuad(1, 0, 128, 1, 0, 0, 1, 1);
    }

    selectedFloor = D_003BAEF0 - 1;
    record = fldSceneRecords;
    for (i = 0; i < fldSceneRecordCount; i++, record++) {
        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            itemVisible = 0;
            if (selectedFloor == item->floor) {
                if (fldGetFloorFlag(fldAreaFlagIndex, i, j) != 0) {
                    itemVisible = 1;
                }
                if (D_003BAEAC != 0) {
                    itemVisible = 1;
                }
            }
            func_00142C78(D_00348F30[i], item->nodeIndex, itemVisible);
        }
    }
    for (i = 0; i < fldSceneRecordCount; i++) {
        D_00348F30[i]->color = 0x80808080;
        fldSetEmitterPosition(D_00348F30[i], 0.0f, D_003BAEF8, 0.0f);
    }

    fldSelectDisplayBuffer(0x56);
    func_00129900(0);
    fldSubmitPrimaryFramePacket();
    fldSubmitFrameQuad(1, 0, 128, 1, 0, 0, 1, 1);
    fldSubmitGsQuadTagged(0, 0, 512, 224, 0, 0, 0, 0);

    record = fldSceneRecords;
    selectedFloor = D_003BAEF0 - 1;
    for (i = 0; i < fldSceneRecordCount; i++, record++) {
        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            itemVisible = 0;
            if (selectedFloor == item->floor) {
                if (fldGetFloorFlag(fldAreaFlagIndex, i, j) != 0) {
                    itemVisible = 1;
                }
                if (D_003BAEAC != 0) {
                    itemVisible = 1;
                }
            }
            func_00142C78(D_00348F30[i], item->nodeIndex, itemVisible);
        }
        D_00348F30[i]->color = 0x80808080;
        fldSetEmitterPosition(D_00348F30[i], -50.0f, D_003BAEF8, 50.0f);
        fldSetEmitterPosition(D_00348F30[i], 50.0f, D_003BAEF8, 50.0f);
        fldSetEmitterPosition(D_00348F30[i], -50.0f, D_003BAEF8, -50.0f);
        fldSetEmitterPosition(D_00348F30[i], 50.0f, D_003BAEF8, -50.0f);

        item = record->items;
        for (j = 0; j < record->count; j++, item++) {
            func_00142C78(D_00348F30[i], item->nodeIndex,
                          selectedFloor == item->floor);
        }
        D_00348F30[i]->color = 0x10101010;
        fldSetEmitterPosition(D_00348F30[i], 0.0f, D_003BAEF8, 0.0f);
        D_00348F30[i]->color = 0x80808080;
    }

    fldSelectDisplayBuffer(0x5A);
    fldSubmitAlternateFramePacket();
    if (stage != NULL) {
        f32 step;
        s32 run;

        evtSetDrawSurfaceIndex(0x5A);
        evtSubmitPrimaryGsTest(1, 0, 128, 1, 1, 1, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(0);
        step = stage->cellSize;
        sceneZ = stage->originZ;
        for (j = 0; j < stage->rows; j++, sceneZ -= step) {
            if ((f32)D_003BAEC0 + 8400.0f < sceneZ) {
                continue;
            }
            if (sceneZ < (f32)D_003BAEC0 - 8400.0f) {
                break;
            }
            run = 0;
            sceneX = stage->originX;
            for (i = 0; i < stage->cols; i++, sceneX += step) {
                if (sceneX < (f32)D_003BAEBC - 11400.0f) {
                    continue;
                }
                if ((f32)D_003BAEBC + 11400.0f < sceneX) {
                    break;
                }
                if (fldTestStageExplorationCell(D_003BAEF0, sceneX, sceneZ) != 0) {
                    if (run == 0) {
                        left = sceneX - 50.0f;
                        top = sceneZ + 50.0f;
                    }
                    bottom = sceneZ - step - 50.0f;
                    right = sceneX + step + 50.0f;
                    run++;
                } else {
                    if (run > 0) {
                        evtSubmitQuadFromVertices(
                            left, D_003BAEF8 - 10.0f, top,
                            right, D_003BAEF8 - 10.0f, top,
                            left, D_003BAEF8 - 10.0f, bottom,
                            right, D_003BAEF8 - 10.0f, bottom,
                            0x80867173, 0x80867173, 0x80867173, 0x80867173);
                    }
                    run = 0;
                }
            }
            if (run > 0) {
                evtSubmitQuadFromVertices(
                    left, D_003BAEF8 - 10.0f, top,
                    right, D_003BAEF8 - 10.0f, top,
                    left, D_003BAEF8 - 10.0f, bottom,
                    right, D_003BAEF8 - 10.0f, bottom,
                    0x80867173, 0x80867173, 0x80867173, 0x80867173);
            }
            run = 0;
        }
        evtSubmitPrimaryAlphaBlendMode(0);
    }

    func_00129900(0);
    fldSubmitFrameQuad(1, 0, 128, 1, 0, 0, 1, 1);
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, sdfViewMatrix);
    D_003BAF10 += 0.4f;
    D_003BAF14 += 0.2f;
    PCP_COPY_VECTOR_F32(focus, D_00324A20);

    if (D_003BAF00 >= 128) {
        selectedFloor = D_003BAEF0 - 1;
        record = fldSceneRecords;
        for (i = 0; i < fldSceneRecordCount; i++, record++) {
            sceneX = 0.0f;
            sceneZ = 0.0f;
            item = record->items;
            for (j = 0; j < record->count; j++, item++) {
                itemVisible = 0;
                if (selectedFloor == item->floor) {
                    if (fldGetFloorFlag(fldAreaFlagIndex, i, j) != 0) {
                    itemVisible = 1;
                }
                    if (D_003BAEAC != 0) {
                        itemVisible = 1;
                    }
                }
                if (itemVisible != 0) {
                    FldIcon *icon = item->icons;

                    if (icon != NULL) {
                        for (k = 0; k < item->iconCount; k++, icon++) {
                            s32 spriteIndex = 0;
                            s32 iconVisible;
                            f32 offsetX = 0.0f;
                            f32 offsetZ = 0.0f;
                            f32 projectedX;
                            f32 projectedY;

                            iconVisible = 0;
                            switch (icon->kind) {
                            case 1:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 8;
                                    iconVisible = 1;
                                }
                                break;
                            case 2:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 9;
                                    iconVisible = 1;
                                }
                                break;
                            case 3:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 10;
                                    iconVisible = 1;
                                }
                                break;
                            case 4:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 5;
                                    iconVisible = 1;
                                }
                                break;
                            case 5:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 2;
                                    iconVisible = 1;
                                }
                                break;
                            case 6:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 4;
                                    iconVisible = 1;
                                }
                                break;
                            case 7:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 6;
                                    iconVisible = 1;
                                }
                                break;
                            case 8:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 7;
                                    iconVisible = 1;
                                }
                                break;
                            case 9:
                                spriteIndex = 3;
                                iconVisible = 1;
                                break;
                            case 10:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 12;
                                    iconVisible = 1;
                                }
                                break;
                            case 11:
                                spriteIndex = 11;
                                iconVisible = 1;
                                offsetZ = sdfSinPoly(D_003BAF14 + (i + k) * 3.3f) * 30.0f;
                                break;
                            case 12:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 13;
                                    iconVisible = 1;
                                }
                                break;
                            case 13:
                                if (D_003BAEAC != 0 ||
                                    fldTestStageExplorationCell(D_003BAEF0,
                                        icon->position->x + sceneX + offsetX,
                                        icon->position->z + sceneZ + offsetZ) != 0) {
                                    spriteIndex = 14;
                                    iconVisible = 1;
                                }
                                break;
                            default:
                                spriteIndex = 11;
                                break;
                            }
                            if (iconVisible != 0) {
                                f32 x;
                                f32 z;

                                x = icon->position->x + sceneX + offsetX;
                                offsetX = focus[0] - x;
                                if (offsetX < 0.0f) {
                                    offsetX = -offsetX;
                                }
                                z = icon->position->z + sceneZ + offsetZ;
                                offsetZ = focus[2] - z;
                                if (offsetZ < 0.0f) {
                                    offsetZ = -offsetZ;
                                }
                                if (offsetX < 80000.0f && offsetZ < 80000.0f) {
                                    fldProjectPointSetupAlt(&projectedX, &projectedY,
                                                            x, D_003BAEF8, z);
                                    func_00142800(projectedX, projectedY,
                                                  spriteIndex, 0x80808080);
                                }
                            }
                        }
                    }
                }
            }
        }
        /* The fade gate deliberately stays open for the player-marker tail. */

/* Source-only reconstruction of func_00142D78, starting at 0x00143EC0.
 * Continues inside the D_003BAF00 >= 128 block after the icon loops.
 * Function-scope locals and declarations are supplied by the prefix. */

        if (D_003BAEF0 == fldAreaState.unkC0) {
            f32 worldX;
            f32 worldZ;
            f32 distanceX;
            f32 distanceZ;

            fldGetSceneEntryPosition(D_003BAEB4, &sceneX, &sceneZ);
            fldSubmitFrameQuad(1, 0, 128, 3, 0, 0, 1, 1);
            func_00129900(1);
            worldX = fldAreaState.x + sceneX;
            distanceX = focus[0] - worldX;
            if (distanceX < 0.0f) {
                distanceX = -distanceX;
            }
            worldZ = fldAreaState.z + sceneZ;
            distanceZ = focus[2] - worldZ;
            if (distanceZ < 0.0f) {
                distanceZ = -distanceZ;
            }
            fldProjectPointSetupAlt(&playerX, &playerY,
                                    worldX, D_003BAEF8, worldZ);
            if (distanceX < 80000.0f && distanceZ < 80000.0f) {
                func_00129900(1);
                func_001428C0(1, playerX, playerY, -fldAreaState.negatedAngle);
                func_00129900(0);
                func_001428C0(0, playerX, playerY, fldAreaState.angle);
            }
        }
    }

    fldSelectDisplayBuffer(0x5E);
    func_00129900(0);
    func_00129900(1);
    fldSubmitFrameQuad(1, 0, 128, 1, 0, 0, 1, 1);
    fldSubmitSpriteRect(0, 0, 512, 448, 32, 59, 22, 22,
                        0x40808080, D_003D40A0[1]);
    func_00129900(0);
    func_00129900(0);
    fldSubmitSpriteRect(332, 410, 156, 27, 2, 83, 156, 27,
                        0x80808080, D_003D40A0[0]);

    top = sdfSinPoly(D_003BAF0C);
    i = (s32)(top * 32.0f) + 96;
    D_003BAF0C += 0.2f;
    i = i + (i << 8) + (i << 16)
                 + 0x80000000U;
    fldGetVisibleSceneBounds(&minX, &maxZ, &maxX, &minZ);
    minX = (s32)((minX - 600.0f) / 600.0f) * 600;
    maxX = (s32)((maxX + 600.0f) / 600.0f) * 600;
    maxZ = (s32)((maxZ - 600.0f) / 600.0f) * 600;
    minZ = (s32)((minZ + 600.0f) / 600.0f) * 600;
    fldSubmitFrameQuad(1, 0, 128, 1, 0, 0, 1, 1);
    func_00129900(0);

    fldSubmitSpriteRect(242, 63, 28, 27, 172, 61, 28, 27,
                        0x80808080, D_003D40A0[0]);
    fldSubmitSpriteRect(20, 222, 28, 27, 201, 89, 28, 27,
                        0x80808080, D_003D40A0[0]);
    fldSubmitSpriteRect(242, 386, 28, 27, 172, 89, 28, 27,
                        0x80808080, D_003D40A0[0]);
    fldSubmitSpriteRect(464, 222, 28, 27, 201, 61, 28, 27,
                        0x80808080, D_003D40A0[0]);

    if ((f32)D_003BAEC0 < maxZ) {
        fldSubmitSpriteRect(242, 53, 28, 40, 2, 2, 28, 40,
                            i, D_003D40A0[1]);
    }
    if (minZ < (f32)D_003BAEC0) {
        fldSubmitSpriteRect(242, 383, 28, 40, 2, 43, 28, 40,
                            i, D_003D40A0[1]);
    }
    if (minX < (f32)D_003BAEBC) {
        fldSubmitSpriteRect(11, 222, 39, 27, 31, 30, 39, 27,
                            i, D_003D40A0[1]);
    }
    if ((f32)D_003BAEBC < maxX) {
        fldSubmitSpriteRect(462, 222, 39, 27, 31, 2, 39, 27,
                            i, D_003D40A0[1]);
    }
    fldSubmitSpriteRect(173, 0, 166, 80, 2, 2, 166, 80,
                        0x80808080, D_003D40A0[0]);
    fldSubmitSpriteRect(339, 0, 190, 80, 167, 2, 1, 80,
                        0x80808080, D_003D40A0[0]);

    {
        const u8 *title = (const u8 *)D_0033EC90[fldAreaState.area % 100];

        D_003BD7CC = (FrFontGlyph *)itfCreateConvertedTextGlyph(
            (450 - D_003BD7D0 * 6) << 4, 112, 0, 0xA09DC380,
            title, 0);
        frFontStoreShiftedRenderValue(D_003BD7CC, 0xFFFFFF);
        frFontDrawGlyphInDefaultMode(D_003BD7CC);
        frFontQueueGlyphForCurrentDrawBuffer(D_003BD7CC);
    }

    fldSelectDisplayBuffer(0x5E);
    func_00129900(0);
    fldSubmitFrameQuad(1, 0, 128, 2, 0, 0, 1, 2);
    fldSubmitFrameQuad(1, 5, 128, 3, 0, 0, 1, 2);

    if (D_003BAF04 != 0 && (u32)D_003BAEF4 - 1U < 40U
        && D_003D4408[D_003BAEF4] >= 2) {
        s32 labelIndex;

        labelAlpha = D_003BAF08;
        if (labelAlpha < 0) {
            labelAlpha = -labelAlpha;
        }
        labelAlpha = 128 - (labelAlpha << 3);
        if (labelAlpha < 0) {
            labelAlpha = 0;
        }
        labelColor = labelAlpha | 0x78759B00;
        labelIndex = fldFindMapCoordinateIndex(fldAreaState.area % 100, D_003BAEF4);
        D_003BD7CC = (FrFontGlyph *)itfCreateConvertedTextGlyph(
            (450 - (D_003D4408[D_003BAEF4] / 2) * 12) << 4,
            (D_003BAF08 + 42) << 3, 0, labelColor,
            (const u8 *)D_0034606C[labelIndex], 0);
        frFontStoreShiftedRenderValue(D_003BD7CC, 0xFFFFFF);
        frFontDrawGlyphWithSharedFlags(D_003BD7CC, 1);
        frFontQueueGlyphForCurrentDrawBuffer(D_003BD7CC);
    }

    if ((u32)D_003BAEF0 - 1U < 40U && D_003D4408[D_003BAEF0] >= 2) {
        s32 labelIndex;

        labelAlpha = D_003BAF04;
        if (labelAlpha < 0) {
            labelAlpha = -labelAlpha;
        }
        labelAlpha = 128 - (labelAlpha << 3);
        if (labelAlpha < 0) {
            labelAlpha = 0;
        }
        labelColor = labelAlpha | 0x78759B00;
        labelIndex = fldFindMapCoordinateIndex(fldAreaState.area % 100, D_003BAEF0);
        D_003BD7CC = (FrFontGlyph *)itfCreateConvertedTextGlyph(
            (450 - (D_003D4408[D_003BAEF0] / 2) * 12) << 4,
            (D_003BAF04 << 3) + 332, 0, labelColor,
            (const u8 *)D_0034606C[labelIndex], 0);
        frFontStoreShiftedRenderValue(D_003BD7CC, 0xFFFFFF);
        frFontDrawGlyphWithSharedFlags(D_003BD7CC, 1);
        frFontQueueGlyphForCurrentDrawBuffer(D_003BD7CC);
    }

    VU0_LOAD_MATRIX(sdfViewMatrix);
}

extern f32 D_003BAF18;
extern f32 D_00329790[4][4];
extern f32 sdfViewMatrix[4][4];
extern f32 sdfSinPoly(f32);
extern void sdfInvertScaledVuTransform(void);
extern void sdfConsCacheTransformedNode(SdfProjectionRecord *, void *);
extern void fldSubmitGsQuadTagged(s32, s32, s32, s32, u32, u32, u32, u32);

void fldDrawFieldBackdropFrame(void) {
    s32 i;
    s32 x;
    s32 y;
    s32 alpha;
    u32 color;

    /* The stored single-precision phase step is just below 0.1. */
    D_003BAF18 += 0x1.999998p-4f;
    VU0_LOAD_MATRIX(D_00329790);
    sdfInvertScaledVuTransform();
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, D_00329790);

    fldSelectDisplayBuffer(0x56);
    fldSubmitFrameQuad(1, 0, 0x80, 1, 0, 0, 1, 1);
    func_00129900(0);
    fldSubmitGsQuadTagged(0, 0, 0x200, 0xE0, 0, 0, 0, 0x80);
    fldSubmitSpriteRect(0, 0, 0x200, 0x1C0, 0, 0, 0x40, 0x40,
                        0x80404040, D_003D40A0[2]);
    func_00129900(1);

    alpha = 0x5C - (s32)(sdfSinPoly(D_003BAF18 + 3.0f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    fldSubmitSpriteRect(0x0E, 6, 0x100, 0x100, 0, 0, 0x100, 0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSinPoly(D_003BAF18 + 1.5f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    fldSubmitSpriteRect(0x25, 0x1D, 0x100, 0x100, 0, 0, 0x100, 0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSinPoly(D_003BAF18) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    fldSubmitSpriteRect(0x3C, 0x34, 0x100, 0x100, 0, 0, 0x100, 0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSinPoly(D_003BAF18) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    fldSubmitSpriteRect(0xC4, 0x8C, 0x100, 0x100, 0x100, 0x100, -0x100, -0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSinPoly(D_003BAF18 + 2.0f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    fldSubmitSpriteRect(0xDB, 0xA3, 0x100, 0x100, 0x100, 0x100, -0x100, -0x100,
                        color, D_003D40A0[3]);
    alpha = 0x5C - (s32)(sdfSinPoly(D_003BAF18 + 3.0f) * 32.0f + 32.0f);
    color = ((u32)alpha << 24) | 0x00808080;
    fldSubmitSpriteRect(0xF2, 0xBA, 0x100, 0x100, 0x100, 0x100, -0x100, -0x100,
                        color, D_003D40A0[3]);
    func_00129900(1);

    for (i = 0; i < 9; i++) {
        x = 0x0C + i * 0x17;
        y = -(i * 0x32);
        fldSubmitSpriteRect(x, y, 5, 0x1A3, 3, 0x56, 5, 0x1C,
                            0x20808080, D_003D40A0[1]);
    }
    for (i = 0; i < 19; i++) {
        x = -(i * 0x0A);
        y = 4 + i * 0x17;
        fldSubmitSpriteRect(x, y, 0xC3, 5, 0x0B, 0x56, 0x19, 5,
                            0x20808080, D_003D40A0[1]);
    }
    for (i = 0; i < 9; i++) {
        x = 0x1EF - i * 0x17;
        y = 0x1D + i * 0x32;
        fldSubmitSpriteRect(x, y, 5, 0x1A3, 3, 0x72, 5, -0x1C,
                            0x20808080, D_003D40A0[1]);
    }
    for (i = 0; i < 19; i++) {
        x = 0x13D + i * 0x0A;
        y = 0x1B7 - i * 0x17;
        fldSubmitSpriteRect(x, y, 0xC3, 5, 0x24, 0x56, -0x19, 5,
                            0x20808080, D_003D40A0[1]);
    }

    func_00129900(0);
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, sdfViewMatrix);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    fldSelectDisplayBuffer(0x5E);
    fldSubmitFrameQuad(1, 5, 0x81, 1, 0, 0, 1, 2);
    func_00129900(0);
}


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

extern void sdfTexReleaseReferenceViaHandler(SdfTex *);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern void frFontSetSharedRenderFlags(u32);

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

extern SdfModel *D_00348F30[];

extern FldSlot D_003D40B0[];

extern void sdfReleaseDevSlot(SdfModel *, s32, s32);

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


extern FldVec4 D_003A05D8[]; /* default camera up vectors (3 copies), the first still read by asm func_00145B18 */

extern void fldApplySkyLightSetToPlayerVU(void);


/* Enters the field camera state for a fresh scene: releases the title slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldAreaWork *cam = &fldAreaState;
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
    cam->sceneMode = 4;
    frFontSetSharedRenderFlags(0x54);
    dds3SetWorldObjectDrawEnabled((EffWorldNode *)(u32)dds3GetWorldObject(), 1);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->floor;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->floor, &entry[0], &entry[1]);
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
    D_00324980.camera.offsetX = 2244.0f;
    D_00324980.camera.offsetY = 2118.0f;
}

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

    rec = fldSceneRecords;
    for (i = 0; i < fldSceneRecordCount; i++, rec++) {
        D_00348F30[i] = sdfModelCreateWithAlternateItems(0, rec->model);
        ((FldPoint *)D_003D40B0)[i].x = rec->pos->x;
        ((FldPoint *)D_003D40B0)[i].y = rec->pos->y;
        ((FldPoint *)D_003D40B0)[i].z = rec->pos->z;
    }
    cam = &fldAreaState;
    D_003D40A0[0] = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(cam->mapResources[0].resourceAddress));
    D_003D40A0[1] = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(cam->mapResources[1].resourceAddress));
    D_003BAED4 = 0;
    D_003BAEB4 = cam->floor;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->floor, &entry[0], &entry[1]);
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
    D_00324980.camera.offsetX = 2244.0f;
    D_00324980.camera.offsetY = 2118.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseMenuSlots();
}

u32 fldGetSceneReadyFlag(void) {
    return fldSceneReady;
}

/* Handle automap input, clamp the grid position, and refresh its camera. */
INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A05D8);

void func_001462D8(void) {
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 minX, maxZ, maxX, minZ;
    s32 closeRequested = 0;

    if (fldGetSceneReadyFlag() == 0) {
        return;
    }
    if (D_0032E5C4[0] != 0) {
        if ((s8)D_00324510[1][0][2] != 0) {
            closeRequested = 1;
        }
    } else if ((s8)D_00324510[1][0][2] < 0) {
        closeRequested = 1;
    }

    /* A close request does not advance the input-delay counter. */
    if (closeRequested == 0) {
        D_003BAECC++;
    }
    if (closeRequested != 0 ||
        ((s8)D_00324510[1][0][3] < 0 && D_003BAED4 == 0)) {
        fldEnterSceneCamera();
        kwlnFadeStartOut(0);
        kwlnFadeStartIn(8);
        return;
    }
    if ((s8)D_00324510[1][0][3] < 0) {
        sndSetSequenceVolumePan(10, 127, 63);
        D_003BAEBC = D_003BAEC4;
        D_003BAEC0 = D_003BAEC8;
        D_003BAEB8 = D_003BAED0;
        D_003BAED4 = 0;
    }

    if (D_003BAECC >= 11) {
        if ((s8)D_003BA878[0][1] != 0) {
            if (D_003BA878[0][1] & 2) {
                if ((s8)D_00324510[1][0][5] != 0 &&
                    (s8)D_00324510[1][0][7] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 -= 600;
                } else if ((s8)D_00324510[1][0][5] != 0 &&
                           (s8)D_00324510[1][0][6] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 += 600;
                } else if ((s8)D_00324510[1][0][4] != 0 &&
                           (s8)D_00324510[1][0][7] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 -= 600;
                } else if ((s8)D_00324510[1][0][4] != 0 &&
                           (s8)D_00324510[1][0][6] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 += 600;
                } else if ((s8)D_00324510[1][0][5] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                } else if ((s8)D_00324510[1][0][4] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                } else if ((s8)D_00324510[1][0][7] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEC0 -= 600;
                } else if ((s8)D_00324510[1][0][6] != 0) {
                    D_003BAED4 = 1;
                    D_003BAEC0 += 600;
                }
            }
        } else {
            /* A held diagonal consumes the direction choice even when its
             * Z-repeat bit is absent; do not fall back to a cardinal step. */
            if ((s8)D_00324510[1][0][5] != 0 &&
                (s8)D_00324510[1][0][7] != 0) {
                if (D_00324510[1][0][7] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 -= 600;
                }
            } else if ((s8)D_00324510[1][0][5] != 0 &&
                       (s8)D_00324510[1][0][6] != 0) {
                if (D_00324510[1][0][6] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC += 600;
                    D_003BAEC0 += 600;
                }
            } else if ((s8)D_00324510[1][0][4] != 0 &&
                       (s8)D_00324510[1][0][7] != 0) {
                if (D_00324510[1][0][7] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 -= 600;
                }
            } else if ((s8)D_00324510[1][0][4] != 0 &&
                       (s8)D_00324510[1][0][6] != 0) {
                if (D_00324510[1][0][6] & 2) {
                    D_003BAED4 = 1;
                    D_003BAEBC -= 600;
                    D_003BAEC0 += 600;
                }
            } else if (D_00324510[1][0][5] & 2) {
                D_003BAED4 = 1;
                D_003BAEBC += 600;
            } else if (D_00324510[1][0][4] & 2) {
                D_003BAED4 = 1;
                D_003BAEBC -= 600;
            } else if (D_00324510[1][0][7] & 2) {
                D_003BAED4 = 1;
                D_003BAEC0 -= 600;
            } else if (D_00324510[1][0][6] & 2) {
                D_003BAED4 = 1;
                D_003BAEC0 += 600;
            }
        }
    }

    fldGetVisibleSceneBounds(&minX, &maxZ, &maxX, &minZ);
    /* Quantize the two X endpoints, then the two Z endpoints. */
    minX = (s32)((minX - 600.0f) / 600.0f) * 600;
    maxX = (s32)((maxX + 600.0f) / 600.0f) * 600;
    maxZ = (s32)((maxZ - 600.0f) / 600.0f) * 600;
    minZ = (s32)((minZ + 600.0f) / 600.0f) * 600;

    if ((f32)D_003BAEBC < minX) {
        D_003BAEBC = (s32)minX;
    }
    if (maxX < (f32)D_003BAEBC) {
        D_003BAEBC = (s32)maxX;
    }
    if ((f32)D_003BAEC0 < minZ) {
        D_003BAEC0 = (s32)minZ;
    }
    if (maxZ < (f32)D_003BAEC0) {
        D_003BAEC0 = (s32)maxZ;
    }

    focus[0] = D_003BAEBC;
    focus[1] = 0.0f;
    focus[2] = D_003BAEC0;
    eye[0] = D_003BAEBC;
    eye[1] = -24000.0f;
    eye[2] = D_003BAEC0;
    PCP_COPY_VECTOR(&D_00324A30, eye);
    PCP_COPY_VECTOR(&D_00324A20, focus);
    PCP_COPY_VECTOR(&D_00324A40, up);
    D_00324B30.unk0 = 255.0f;
    D_00324B30.unk8 = 1000.0f;
    D_00324B30.unk4 = 255.0f;
    D_00324B30.unkC = 20000.0f;
    D_00324B30.unk10 = 0x808080;
    PCP_COPY_VECTOR(sdfViewTargetVector, eye);
    PCP_COPY_VECTOR(sdfViewEyeVector, focus);
    PCP_COPY_VECTOR(sdfViewUpVector, up);
}

/* Re-centers the scene camera on the current scene's entry point. */
void fldCenterCameraOnEntry(void) {
    FldAreaWork *cam = &fldAreaState;
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 entry[2];
    s32 ix;
    s32 iz;

    D_003BAEB4 = cam->floor;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->floor, &entry[0], &entry[1]);
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
    fldAreaState.area = area;
    fldAreaState.unk18 = stage;
    D_003BAEB4 = fldAreaState.floor = floor;
    fldAreaFlagIndex = area % 100;
}


extern s32 D_0032C900[];

/* The area's last two digits map to a save-table row; floor and bit are zero-based. */
s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_0032C900[area % 100];
    if (areaIndex == -1) return 0;
    return (datGameState->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetFlagAndFindRecord(s32 flagNumber) {
    s32 areaIndex;
    s32 bit;

    if (flagNumber > 0) {
        areaIndex = D_0032C900[fldAreaFlagIndex % 100];
        if (areaIndex != -1) {
            bit = flagNumber - 1;
            fldAreaState.unk18 = bit;
            fldAreaState.flagNumber = flagNumber;
            datGameState->areaFlags[areaIndex][fldAreaState.floor] |= 1ULL << bit;
            fldAreaState.unkC0 = fldFindRecordItem(fldAreaState.floor, bit);
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
        datGameState->areaFlags[areaIndex][floor] |= 1ULL << bit;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_0032C900[area % 100];
    if (areaIndex != -1) {
        datGameState->areaFlags[areaIndex][floor] &= ~(1ULL << bit);
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


extern s32 fldFindRecordItem(s32 scene, u32 index);

s32 fldFindPreviousMarkedValue(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = -1;
    FldSceneRecord *entry = fldSceneRecords;

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
    FldSceneRecord *entry = fldSceneRecords;

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
    FldSceneRecord *entry = fldSceneRecords;
    for (i = 0; i < (s32)fldSceneRecordCount; i++, entry++) {
        if (i == index) {
            FldPoint *position = entry->pos;
            *x = position->x;
            *z = position->z;
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
    rec = fldSceneRecords;
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

void fldCheckSceneReady(void) {
    if (D_0032E3C0[0] >= 200) {
        return;
    }
    if (fldSceneReady == 1) {
        func_001462D8();
    }
}

void func_001470E0(void) {
    if (fldSceneReady == 1) {
        fldDrawFieldBackdropFrame();
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
    DatGameState *block;
    s32 index;

    index = 0;
    block = datGameState;
    do {
        words = block->areaFlags[index];
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        index = index + 1;

    } while (index < 0xd);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147188);

extern s32 fldEffectTextureLoadHandles[];

extern s32 fldEffectTextureData[];

extern s32 fldEffectTextureNodes[];

extern s32 D_0034C8A0[];


extern s32 func_0014FE28();

extern char *D_003A06B0[4]; /* {"/fld/f/bin/KUT_3Z.EPL", "ASI_2MZ", "ASI_2HZ", "MAN_2Z"} */

void fldLoadFieldEffectTextureSlots(void) {
    char *names[4];
    s32 i;

    memcpy(names, D_003A06B0, sizeof(names));
    for (i = 0; i < 4; i++) {
        fldEffectTextureLoadHandles[i] = (s32)sdfReadNamedResource(names[i], (u32 *)&fldEffectTextureData[i], 0);
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
            effDestroyNode((EffNode *)(u32)fldEffectTextureNodes[i]);
            fldEffectTextureNodes[i] = 0;
            sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldEffectTextureLoadHandles[i]);
            fldEffectTextureLoadHandles[i] = 0;
            fldEffectTextureData[i] = 0;
        }
    }
}

extern u32 fldPlayerObject;

extern f32 effMiscComputeQuaternionRotatedReferenceAngle(void);

extern void func_002DD688(f32 angle);

void fldUpdateEffectTextureTransforms(void) {
    f32 vec[4];
    u8 mat[64];
    f32 angle;
    s32 i;

    memset(vec, 0, sizeof(vec));
    vec[3] = 1.0f;
    effObjFetchInnerRotationNormalized((EffWorldNode *)fldPlayerObject);
    angle = effMiscComputeQuaternionRotatedReferenceAngle();
    for (i = 0; i < 4; i++) {
        if (fldEffectTextureNodes[i] != 0) {
            vec[0] = fldAreaState.x;
            vec[2] = fldAreaState.z;
            vec[1] = fldAreaState.y;
            if (i == 3) {
                vec[1] = fldAreaState.y - 100.0f;
            }
            effCopyVectorToNodeInstance((EffNode *)(u32)fldEffectTextureNodes[i], vec);
            func_002DD688(-angle);
            VU0_STORE_MATRIX_UNCLOBBERED(mat);
            effApplyNodeTransformMatrix((EffNode *)(u32)fldEffectTextureNodes[i], mat);
            effUpdateNode((EffNode *)(u32)fldEffectTextureNodes[i]);
        }
    }
}

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



extern s32 mnuPositionedResourceCursor;

void mnuSpawnResourceAtPosition(f32 x, f32 y, f32 z) {
    f32 pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 handle;

    handle = mnuPositionedResourceNodes[mnuPositionedResourceCursor];
    if (handle != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        effRestartNodeInstance((EffNode *)(u32)handle);
        effCopyVectorToNodeInstance((EffNode *)(u32)mnuPositionedResourceNodes[mnuPositionedResourceCursor], pos);
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
            effDestroyNode((EffNode *)(u32)mnuPositionedResourceNodes[i]);
            mnuPositionedResourceNodes[i] = 0;
        }
    }
}

void fldUpdateMenuResourceEffects(void) {
    s32 index;
    for (index = 0; index < 4; index++) {
        if (mnuPositionedResourceNodes[index] != 0 && mnuPositionedResourceActive[index] != 0) {
            effUpdateNode((EffNode *)(u32)mnuPositionedResourceNodes[index]);
        }
    }
}




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


/* Retains fldmix.LB node values and their memory blocks in field work.
 * Nodes 10..13 supply the map resources later acquired by the scene camera. */
INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0650);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0668);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0680);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0698);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A06B0);

void fldParseMixLb(void) {
    FldLbFile *lb;
    FldLbNode *node;
    u32 index;
    s32 value;

    index = 0;
    lb = (FldLbFile *)fileQueuePlainDispatchRequest("/fld/f/bin/fldmix.LB");
    func_00288C50((struct FileRequest *)lb);
    for (node = lb->nodes; node != NULL; node = node->next, index++) {
        switch (index) {
        case 0:
            value = node->value;
            D_003BD7EC = value;
            D_003BD7F0 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 1:
            value = node->value;
            D_003BAF68 = value;
            D_003BAF6C = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 2:
            value = node->value;
            D_003BAF50 = value;
            D_003BAF54 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 3:
            value = node->value;
            D_003BAF74 = value;
            D_003BAF78 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 4:
            value = node->value;
            D_003BAF5C = value;
            D_003BAF60 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 5:
            value = node->value;
            D_003BAF84 = value;
            D_003BAF88 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 6:
            value = node->value;
            D_003BAF44 = value;
            D_003BAF48 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 7:
            value = node->value;
            D_003BD7D4 = value;
            D_003BD7D8 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 8:
            value = node->value;
            D_003BD7DC = value;
            D_003BD7E0 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 9:
            value = node->value;
            D_003BD7E4 = value;
            D_003BD7E8 = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 10:
            value = node->value;
            fldAreaState.mapResources[0].allocation = (struct SdfMemBlock *)(u32)value;
            fldAreaState.mapResources[0].resourceAddress = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 11:
            value = node->value;
            fldAreaState.mapResources[1].allocation = (struct SdfMemBlock *)(u32)value;
            fldAreaState.mapResources[1].resourceAddress = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 12:
            value = node->value;
            fldAreaState.mapResources[2].allocation = (struct SdfMemBlock *)(u32)value;
            fldAreaState.mapResources[2].resourceAddress = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        case 13:
            value = node->value;
            fldAreaState.mapResources[3].allocation = (struct SdfMemBlock *)(u32)value;
            fldAreaState.mapResources[3].resourceAddress = sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)value);
            break;
        }
    }
    func_00288788(lb);
}

extern s32 D_003BAF48, D_003BAF4C, D_003BAF40, D_003BAF3C;

extern SdfTex *D_003BAF30, *D_003BAF34, *D_003BAF38;

extern s32 D_003BD7F0, D_003BD7D8, D_003BD7E0, D_003BD7E8;

void fldInitializeMenuResources(void) {
    if (D_0032E3C0[0] < 200) {
        D_003BAF4C = (s32)effCreateNodeFromDescriptor((EffNodeDescriptor *)(u32)D_003BAF48);
        D_003BAF3C = (s32)effCreateNodeFromDescriptor((EffNodeDescriptor *)(u32)D_003BD7F0);
        D_003BAF40 = 0;
        D_003BAF30 = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(D_003BD7D8));
        D_003BAF34 = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(D_003BD7E0));
        D_003BAF38 = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(D_003BD7E8));
    }
}

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
        effDestroyNode((EffNode *)(u32)D_003BAF4C);
        D_003BAF4C = 0;
    }
    if (D_003BAF58 != 0) {
        effDestroyNode((EffNode *)(u32)D_003BAF58);
        D_003BAF58 = 0;
    }
    if (D_003BAF64 != 0) {
        effDestroyNode((EffNode *)(u32)D_003BAF64);
        D_003BAF64 = 0;
    }
    if (D_003BAF3C != 0) {
        effDestroyNode((EffNode *)(u32)D_003BAF3C);
        D_003BAF3C = 0;
    }
}

typedef struct {
    s32 unk0;
    f32 unk4, unk8, unkC;
    f32 unk10[4];
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
        fldRoomEffectEntries[i].unk4 = 0.0f;
        fldRoomEffectEntries[i].unk8 = 0.0f;
        fldRoomEffectEntries[i].unkC = 0.0f;
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
    u8 pad10[0x10];
    s32 effectVariant;
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
            effDestroyNode((EffNode *)(u32)fldObjectSlots[i].effectNode);
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
            effDestroyNode((EffNode *)(u32)fldObjectSlots[i].effectNode);
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

extern s32 fldTestSceneControlFlags(s32);

void fldUpdateObjectActivation(void) {
    s32 i;
    MdlCtx *resource;

    for (i = 0; i < fldObjectSlotCount; i++) {
        switch (fldObjectSlots[i].unk8) {
        case 0:
            if (fldObjectSlots[i].activationRequested == 1) {
                resource = (MdlCtx *)(u32)dds3GetObjectBaseResourceHandle((EffWorldNode *)(u32)fldObjectSlots[i].unk0);
                mdlAddEntryPlain(resource, 0, 1);
                fldObjectSlots[i].unk8 = fldObjectSlots[i].activationRequested;
                switch (fldObjectSlots[i].effectVariant) {
                case 0:
                case 3:
                    if (D_003BAF58 != 0) {
                        effDestroyNode((EffNode *)(u32)D_003BAF58);
                        D_003BAF58 = 0;
                    }
                    D_003BAF58 = (s32)effCreateNodeFromDescriptor((EffNodeDescriptor *)(u32)D_003BAF54);
                    effRestartNodeInstance((EffNode *)(u32)D_003BAF4C);
                    break;
                case 1:
                    if (D_003BAF64 != 0) {
                        effDestroyNode((EffNode *)(u32)D_003BAF64);
                        D_003BAF64 = 0;
                    }
                    D_003BAF64 = (s32)effCreateNodeFromDescriptor((EffNodeDescriptor *)(u32)D_003BAF60);
                    effRestartNodeInstance((EffNode *)(u32)D_003BAF4C);
                    break;
                }
            } else {
                resource = (MdlCtx *)(u32)dds3GetObjectBaseResourceHandle((EffWorldNode *)(u32)fldObjectSlots[i].unk0);
                if (resource->first->state == SDF_MOTION_STATE_TERMINAL) {
                    mdlAddEntryPlain(resource, 0, 0);
                }
            }
            break;
        case 1:
            resource = (MdlCtx *)(u32)dds3GetObjectBaseResourceHandle((EffWorldNode *)(u32)fldObjectSlots[i].unk0);
            if (resource->first->state == SDF_MOTION_STATE_TERMINAL) {
                mdlAddEntryFlagged(resource, 0, 2);
                fldObjectSlots[i].activationRequested = 2;
                fldObjectSlots[i].unk8 = 2;
            }
            break;
        case 2:
            if (fldTestSceneControlFlags(0x40)) {
                resource = (MdlCtx *)(u32)dds3GetObjectBaseResourceHandle((EffWorldNode *)(u32)fldObjectSlots[i].unk0);
                mdlAddEntryFlagged(resource, 0, 3);
                fldObjectSlots[i].activationRequested = 3;
                fldObjectSlots[i].unk8 = 3;
                if (D_003BAF58 != 0) {
                    effDestroyNode((EffNode *)(u32)D_003BAF58);
                    D_003BAF58 = 0;
                }
                if (D_003BAF64 != 0) {
                    effDestroyNode((EffNode *)(u32)D_003BAF64);
                    D_003BAF64 = 0;
                }
            }
            break;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0788);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0798);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148FF0);

extern s32 D_0032E4C8[];

extern f32 fldBannerColorPhase;

extern s32 ptyAnyUnitFlagMatch(s32, s32);

extern f32 sdfSinPoly(f32);

extern void func_00129178(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, SdfTex *);

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

extern void func_00129900(s32);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

void fldDrawTitleBanner(s32 x, s32 y) {
    if (D_0032E3C0[0] < 200) {
        fldSelectDisplayBuffer(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitSpriteRect(x, y, 0x10, 0x11, 0x68, 1, 0x10, 0x11, 0x80808080, D_003BAF30);
        func_00129900(0);
    }
}

extern s32 func_0014CAF8(void);
extern s16 *fldFindLocationCoordinateRecord(s32, s32);
extern void fldSubmitGsGradientQuad(s32, s32, s32, s32, u32, u32, u32, u32,
    u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

void fldDrawLocationPanel(void) {
    FldAreaWork *work;
    u32 color;

    if (func_0014CAF8() != 0) {
        return;
    }
    work = &fldAreaState;
    if (work->unk118 == 1 || work->deferredExit != 0) {
        return;
    }
    if (fldFindLocationCoordinateRecord(work->area, work->floor + 1)[2] <= 0) {
        return;
    }
    switch (work->overlayMode) {
    case 0:
    case 2:
    case 4:
    case 6:
        break;
    case 1:
    case 3:
    case 5:
    case 7:
        color = 0x80808080;
        fldSelectDisplayBuffer(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        fldSubmitGsGradientQuad(0x194, 0x91, 0x5F, 0x47, 0, 0, 0, 0x26,
            0, 0, 0, 0x26, 0, 0, 0, 0x26, 0, 0, 0, 0x26);
        fldSubmitSpriteRect(0x188, 0x117, 0x78, 0x80, 1, 0, 0x78, 0x80, color, D_003BAF34);
        fldSubmitSpriteRect(0x188, 0x197, 0x78, 0x1C, 1, 0x7F, 0x78, 1, color, D_003BAF34);
        func_00129900(0);
        fldSubmitGsGradientQuad(0x190, 0x8F, 0x70, 0x49, 0, 0xD, 0xA2, 0x18,
            0, 0xD, 0xA2, 0, 0, 0xD, 0xA2, 0, 0, 0xD, 0xA2, 0);
        fldSubmitGsGradientQuad(0x193, 0x90, 0x6D, 0x4E, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x18);
        fldSubmitSpriteRect(0x1B4, 0x112, 0x1F, 0x10, 1, 1, 0x1F, 0x10, color, D_003BAF38);
        func_00129900(0);
        fldResetCameraAndSceneView();
        break;
    }
}

extern s32 fldTestSceneLifecycleFlags(u32);

void fldFlushQueuedEffectPositions(void) {
    if (fldAreaState.area < 0xC8) {
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
            if (fldAreaState.colorEffectSuppressed == 0) {
                mnuSpawnResourceAtPosition(fldSecondaryQueuedEffectPosition[0], fldSecondaryQueuedEffectPosition[1], fldSecondaryQueuedEffectPosition[2]);
            }
            fldSecondaryEffectPositionPending = 0;
        }
        if (fldPrimaryEffectPositionPending != 0) {
            if (fldAreaState.colorEffectSuppressed == 0) {
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
        if (room != 0 && fldTestMapSlotAuxiliaryFlag(fldAreaState.area, fldAreaState.floor + 1, room) != 0) {
            if (fldRoomEffectEntries[i].unk20 != 0) {
                dds3SetObjectFlags(fldRoomEffectEntries[i].unk20, 1);
            }
        }
    }
}

extern u8 D_003D44A0[0x200];

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_003014F0(char *, const char *, ...);




void fldLoadNpcPalette(s32 field) {
    char path[64];
    char directory[32];
    DevState *command;

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


extern u32 D_003BAFA4;

extern u32 D_003BAFAC;

extern u32 D_003BAF9C;


extern char D_003A0810[]; /* "/fld/f/pnl/df%03d.tmx" */

void fldStartTitle(s32 field, s32 mode, s32 option) {
    char path[32];
    s32 resourceAddress;
    s32 handle;

    D_003BAFA8 = mode;
    D_003BAFAC = option;
    D_003BAFA4 = field;
    D_003BAF9C = 0;
    D_003BAFA0 = 0;
    D_003BAFB0 = 0;
    func_003014F0(path, D_003A0810, field);
    handle = (s32)sdfReadNamedResource(path, (u32 *)&resourceAddress, 0);
    D_003BAFB4 = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(resourceAddress));
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(handle));
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

/* Animate the unlock title's split caption, highlight and expanding fade. */
s32 fldTitleMini(void) {
    s32 alpha;
    u32 color;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 nextFrame;
    u32 highlightRgb;

    if (fldGetCampSceneControlMode() == 1) {
        return 0;
    }
    fldSelectDisplayBuffer(0x53);
    fldSubmitFrameQuad(1, 5, 0x80, 3, 0, 0, 1, 2);
    func_00129900(0);
    switch (D_003BAFBC) {
    case 0:
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 3) {
            D_003BAFBC = 1;
            D_003BAFB8 = 0;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    case 1:
        if (D_003BAFB8 >= 8) {
            alpha = (D_003BAFB8 - 8) * 16;
            fldSubmitGsGradientQuad(0x4C, 0xB1, 0xB4, 2, 0, 0, 0, 0,
                0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128);
            fldSubmitGsGradientQuad(0x100, 0xB1, 0xB4, 2, 0, 0, 0, alpha * 0x33 / 128,
                0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0);
        }
        alpha = D_003BAFB8 * 128 / 10;
        if (alpha > 128) {
            alpha = 128;
        }
        color = (alpha << 24) | 0x808080;
        x = 218 - D_003BAFB8 * 6;
        fldSubmitSpriteRect(x < 128 ? 128 : x, 0x152, 0x80, 0x1C,
            0, 0x24, 0x80, 0x1C, color, D_003BAFC4);
        x = 166 + D_003BAFB8 * 6;
        fldSubmitSpriteRect(x > 256 ? 256 : x, 0x152, 0x80, 0x1C,
            0x80, 0x24, 0x80, 0x1C, color, D_003BAFC4);
        if (D_003BAFB8 >= 10) {
            alpha = (D_003BAFB8 - 10) * 25;
            if (alpha > 128) {
                alpha = 128;
            }
            func_00129900(1);
            fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        if (D_003BAFB8 >= 8) {
            alpha = (D_003BAFB8 - 8) * 16;
            func_00129900(0);
            fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 15) {
            D_003BAFB8 = 0;
            D_003BAFBC++;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    case 2:
        alpha = 128;
        fldSubmitGsGradientQuad(0x4C, 0xB1, 0xB4, 2, 0, 0, 0, 0,
            0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128);
        fldSubmitGsGradientQuad(0x100, 0xB1, 0xB4, 2, 0, 0, 0, alpha * 0x33 / 128,
            0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0);
        alpha -= D_003BAFB8 * 8;
        if (alpha > 0) {
            func_00129900(1);
            fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        func_00129900(0);
        fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
            0x80808080, D_003BAFC4);
        func_00129900(0);
        fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
            0x80808080, D_003BAFC4);
        highlightRgb = 0x808080;
        if (D_003BAFB8 < 4) {
            alpha = D_003BAFB8 * 40 / 3;
            func_00129900(1);
            fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
                (alpha << 24) | highlightRgb, D_003BAFC4);
        } else {
            alpha = 56 - D_003BAFB8 * 4;
            if (alpha > 0) {
                func_00129900(1);
                fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x24, 0, 0, 0x100, 0x24,
                    (alpha << 24) | highlightRgb, D_003BAFC4);
            }
        }
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 30) {
            D_003BAFB8 = 0;
            D_003BAFBC++;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    case 3:
        alpha = 128;
        if (D_003BAFB8 >= 5) {
            alpha = 128 - (D_003BAFB8 - 5) * 12;
        }
        if (alpha > 0) {
            fldSubmitGsGradientQuad(0x4C, 0xB1, 0xB4, 2, 0, 0, 0, 0,
                0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128);
            fldSubmitGsGradientQuad(0x100, 0xB1, 0xB4, 2, 0, 0, 0, alpha * 0x33 / 128,
                0, 0, 0, 0, 0, 0, 0, alpha * 0x33 / 128, 0, 0, 0, 0);
        }
        if (D_003BAFB8 < 10) {
            alpha = 128 - D_003BAFB8 * 16;
            if (alpha > 0) {
                func_00129900(0);
                fldSubmitSpriteRect(0x80, 0x152, 0x100, 0x1C, 0, 0x24, 0x100, 0x1C,
                    (alpha << 24) | 0x808080, D_003BAFC4);
            }
        }
        alpha = 128;
        if (D_003BAFB8 >= 5) {
            alpha = 128 - (D_003BAFB8 - 5) * 16;
        }
        if (D_003BAFB8 < 10) {
            x = 128;
            width = 256;
            y = 338;
            height = 36;
        } else {
            width = (D_003BAFB8 - 10) * 128 / 15;
            x = 128 - width / 2;
            width = 256 + width;
            height = (D_003BAFB8 - 10) * 32 / 15;
            y = 338 + height / 2;
            height = 36 - height;
        }
        if (alpha > 0) {
            func_00129900(0);
            fldSubmitSpriteRect(x, y, width, height, 0, 0, 0x100, 0x24,
                (alpha << 24) | 0x808080, D_003BAFC4);
        }
        nextFrame = D_003BAFB8 + 1;
        if (nextFrame > 30) {
            D_003BAFB8 = 0;
            D_003BAFBC++;
        } else {
            D_003BAFB8 = nextFrame;
        }
        break;
    default:
        return -1;
    }
    return 0;
}


void fldReleaseTitleMiniTexture(void) {
    if (D_003BAFC4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFC4);
        D_003BAFC4 = 0;
    }
}

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
    handle = (s32)sdfReadNamedResource(path, (u32 *)&data, 0);
    D_003BAFC4 = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(data));
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(handle));
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


extern s32 func_0014FE28();

void fldLoadWeatherEffects(void) {
    s32 handle;
    s32 resourceAddress;

    handle = (s32)sdfReadNamedResource(s_fieldWeatherLimitPath, (u32 *)&resourceAddress, 0);
    fldWeatherLimitTexture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(resourceAddress));
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)handle);
    fldDamEffectResource = (u32)sdfReadNamedResource("/fld/f/bin/FH_DAM_2.EPL", &fldDamEffectData, 0);
    fldDamEffectNode = func_0014FE28(fldDamEffectData);
    fldDamEffectPositioned = 0;
    fldYukEffectResource = (u32)sdfReadNamedResource("/fld/f/bin/YUK_2.EPL", &fldYukEffectData, 0);
    fldYukEffectNode = func_0014FE28(fldYukEffectData);
    fldYukEffectPositioned = 0;
}

void fldReleaseWeatherEffects(void) {
    if (fldWeatherLimitTexture != 0) {
        sdfTexReleaseReferenceViaHandler(fldWeatherLimitTexture);
        fldWeatherLimitTexture = 0;
    }
    effDestroyNode((EffNode *)(u32)fldDamEffectNode);
    fldDamEffectNode = 0;
    fldDamEffectPositioned = 0;
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldDamEffectResource);
    fldDamEffectResource = 0;
    fldDamEffectData = 0;
    effDestroyNode((EffNode *)(u32)fldYukEffectNode);
    fldYukEffectNode = 0;
    fldYukEffectPositioned = 0;
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldYukEffectResource);
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
        effRestartNodeInstance((EffNode *)(u32)fldDamEffectNode);
        effCopyVectorToNodeInstance((EffNode *)(u32)fldDamEffectNode, pos);
        fldDamEffectPositioned = 1;
    }
    if (fldYukEffectNode != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        effRestartNodeInstance((EffNode *)(u32)fldYukEffectNode);
        effCopyVectorToNodeInstance((EffNode *)(u32)fldYukEffectNode, pos);
        fldYukEffectPositioned = 1;
    }
}

void fldUpdateWeatherEffectNodes(void) {
    if (fldDamEffectNode != 0 && fldDamEffectPositioned != 0) {
        effUpdateNode((EffNode *)(u32)fldDamEffectNode);
    }
    if (fldYukEffectNode != 0 && fldYukEffectPositioned != 0) {
        effUpdateNode((EffNode *)(u32)fldYukEffectNode);
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

typedef struct FldSparkController {
    void *object;              /* 0x00 */
    FldEnt110 *entry;           /* 0x04 */
    s32 phase;                 /* 0x08 */
    s32 countdown;             /* 0x0C */
    s32 terminated;            /* 0x10 */
    s32 mode;                  /* 0x14 */
    s32 modeCountdown;         /* 0x18 */
    s32 pendingMode;           /* 0x1C */
    s32 frame;                 /* 0x20 */
    s32 pulse;                 /* 0x24 */
    s32 dialogPhase;           /* 0x28 */
    s32 unk2C;                 /* 0x2C */
    s32 entryCount;            /* 0x30 */
    s32 sequenceIndex;         /* 0x34 */
    s32 sequenceRemaining;     /* 0x38 */
    s32 cursor;                /* 0x3C */
} FldSparkController;

extern FldSparkController fldSparkControlState;

typedef struct FldSparkEntryInfo {
    u8 pad00[0xE];
    s16 modelId;
} FldSparkEntryInfo;

extern u32 dds3AdvanceWorldCounter(void);
extern EffWorldNode *dds3SpawnCameraSlotObj5(s32 value, void *position, void *rotation);
extern void dds3SetWorldNodeValue(EffWorldNode *node, u32 value);
extern void func_00111E30(EffWorldNode *node, s32 kind, s32 resource);
extern void effObjSetInnerScale(EffWorldNode *node, u128 *vector);
extern void func_00147DB0(void *unit);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08E8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08F8);

const char D_003A0908[] = "HUNT_MDL_UNIT";

extern FldVec4 D_003A08E8;
extern FldVec4 D_003A08F8;

void func_0014B688(void) {
    FldVec4 position = D_003A08E8;
    FldVec4 rotation;
    MdlCtx *resource;
    s32 i;

    memset(&rotation, 0, sizeof(rotation));
    {
        FldVec4 scale = D_003A08F8;

        for (i = 0; i < 17; i++) {
            fldSparkObjectEntries[i].objectHandle = NULL;
            fldSparkObjectEntries[i].unk4 = -1;
            *(s32 *)fldSparkObjectEntries[i].pad8 = 0;
        }
        for (i = 0; i < 17; i++) {
            fldSparkObjectEntries[i].objectHandle = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), &position, &rotation);
            dds3SetWorldNodeValue(fldSparkObjectEntries[i].objectHandle, (u32)D_003A0908);
            if (i == 16) {
                func_00111E30(fldSparkObjectEntries[i].objectHandle, 1, ((FldSparkEntryInfo *)fldSparkControlState.entry)->modelId);
            } else {
                func_00111E30(fldSparkObjectEntries[i].objectHandle, 1, 0x146);
            }
            effObjSetInnerScale(fldSparkObjectEntries[i].objectHandle, (u128 *)&scale);
            resource = (MdlCtx *)dds3GetObjectBaseResourceHandle(fldSparkObjectEntries[i].objectHandle);
            resource->first->frameStep = 1.0f;
            mdlAddEntryFlagged(resource, 0, 0);
            resource->flags |= 1;
            dds3SetObjectFlags(fldSparkObjectEntries[i].objectHandle, 0x400);
            func_00147DB0(evtUnitGetNestedValue(fldSparkObjectEntries[i].objectHandle));
        }
    }
}

void fldClearObjectEntryHandles(void) {
    FldEnt14 *entry = fldSparkObjectEntries;
    s32 i = 0x10;

    do {
        EffWorldNode *objectHandle = entry->objectHandle;

        i--;
        if (objectHandle != NULL) {
            ddsReleaseUnitObject(objectHandle);
            entry->objectHandle = NULL;
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

struct EffRandState;
extern u32 effMiscRand(struct EffRandState *state);

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

/* Complete 0x40-byte field spark controller, including the weather timer. */
extern s32 D_003D62DC[];
struct EffWorldNode;


s32 fldBindSparkSlotObject(s32 index, s32 reserved) {
    s32 slot;
    s32 i;
    s32 candidate;
    s32 *flags;
    u128 position;

    if (reserved == 0) {
        slot = -1;
        for (i = 0; i < 16; i++) {
            candidate = (fldSparkControlState.cursor + i) % 16;
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
    D_003D62DC[0] = (slot + 1) % 16;
    if (fldSparkSlots[index].hasVectors == 1) {
        PCP_COPY_VECTOR(&position, fldSparkSlots[index].pos);
        effObjSetInnerPosition(fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].pos);
        effObjSetInnerRotation(fldSparkObjectEntries[slot].objectHandle, (u128 *)fldSparkSlots[index].vel);
        flags = (s32 *)dds3GetObjectBaseResourceHandle(fldSparkObjectEntries[slot].objectHandle);
        *flags &= ~1;
        fldSparkSlots[index].objectSlot = slot;
        fldSparkObjectEntries[slot].unk4 = index;
        return 1;
    }
    return 0;
}

extern void dds3ClearObjectFlags();

extern FldVec4 D_003A0918;

void fldFreeSparkSlot(s32 index) {
    FldVec4 vec;
    EffWorldNode *obj;
    s32 *flags;
    s16 slot;

    vec = D_003A0918;
    if (fldSparkSlots[index].hasVectors == 1 && fldSparkSlots[index].active != 0 && fldSparkSlots[index].objectSlot != -1) {
        vec.v[0] = fldSparkSlots[index].pos[0];
        vec.v[2] = fldSparkSlots[index].pos[2];
        effObjSetInnerPosition(fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle, (u128 *)&vec);
        obj = fldSparkObjectEntries[fldSparkSlots[index].objectSlot].objectHandle;
        flags = (s32 *)dds3GetObjectBaseResourceHandle(obj);
        *flags |= 1;
        dds3ClearObjectFlags(obj, 0x400);
        slot = fldSparkSlots[index].objectSlot;
        fldSparkSlots[index].objectSlot = -1;
        fldSparkObjectEntries[slot].unk4 = -1;
    }
}

extern FldSparkController fldSparkControlState;

extern void func_0014B688();

extern s32 fldBindSparkSlotObject(s32, s32);

void fldUpdateSparkSlots(void) {
    s32 i;

    func_0014B688();
    for (i = 0; i < 64 && i < fldSparkControlState.entryCount; i++) {
        if (fldSparkSlots[i].hasVectors != 0 && fldSparkSlots[i].active != 0) {
            fldBindSparkSlotObject(i, fldSparkSlots[i].unk28);
        }
    }
}

void func_0014BDF8(void) {
    FldSparkSequence *sequence;
    s32 i;
    s32 slot;
    FldVec4 position __attribute__((aligned(16)));
    f32 dx;
    f32 dy;
    f32 dz;

    if (fldSparkControlState.sequenceRemaining == 0) {
        i = fldSparkControlState.sequenceIndex;
        sequence = &fldSparkControlState.entry->sequences[i];
        fldSparkControlState.sequenceRemaining = sequence->count;
        if (sequence->count == -1) {
            fldSparkControlState.terminated = 1;
            return;
        }
        if (sequence->count == 0) {
            slot = sequence->slots[0];
            fldBindSparkSlotObject(slot, 1);
            fldSparkSlots[slot].unk28 = 1;
            fldSparkSlots[slot].active = 1;
            fldSparkSlots[slot].unk2E = 0;
            fldSparkControlState.sequenceRemaining = 1;
            sndSetSequenceVolumePan(0x670015, 0x7F, 0x3F);
        } else {
            for (i = 0; i < sequence->count; i++) {
                slot = sequence->slots[i];
                fldBindSparkSlotObject(slot, 0);
                fldSparkSlots[slot].unk28 = 0;
                fldSparkSlots[slot].active = 1;
                fldSparkSlots[slot].unk2E = 0;
            }
            sndSetSequenceVolumePan(0x670014, 0x7F, 0x3F);
        }
        fldSparkControlState.sequenceIndex++;
    }
    for (i = 0; i < 64 && i < fldSparkControlState.entryCount; i++) {
        switch (fldSparkSlots[i].active) {
        case 0:
            break;
        case 1:
            fldSparkSlots[i].unk2E++;
            if (fldSparkSlots[i].objectSlot != -1 && fldSparkControlState.mode == 1 &&
                fldSparkControlState.pulse != 0 && (u32)(fldSparkControlState.modeCountdown - 10) < 6) {
                PCP_COPY_VECTOR(&position, fldSparkSlots[i].pos);
                dx = fldAreaState.x - position.v[0];
                dy = fldAreaState.y - position.v[1];
                dz = fldAreaState.z - position.v[2];
                if (fsqrtf(dx * dx + dy * dy + dz * dz) < 200.0f) {
                    fldSetWeatherEffectPos(position.v[0], position.v[1], position.v[2]);
                    fldAreaState.collectedCount++;
                    fldAreaState.score += 0x32;
                    fldSparkControlState.sequenceRemaining--;
                    sndSetSequenceVolumePan(0x670013, 0x7F, 0x3F);
                    fldReleaseCameraModel(3);
                    fldSparkControlState.modeCountdown += 3;
                    fldFreeSparkSlot(i);
                    fldSparkSlots[i].unk28 = 0;
                    fldSparkSlots[i].active = 0;
                    fldSparkSlots[i].unk2E = effMiscRand(0) % 270 + 30;
                    fldSparkControlState.pulse = 0;
                }
            }

            break;
        }
    }
}

extern FldSparkController fldSparkControlState;

s32 fldIsNearSpark(f32 x, f32 y, f32 z) {
    s32 i;

    for (i = 0; i < 64 && i < fldSparkControlState.entryCount; i++) {
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

/* Ten atlas origins for digits 0-9, each stored as a U/V atlas coordinate pair. */
extern const s32 D_003A0928[10][2];
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);

void fldDrawWeatherLimitTimer(void) {
    s32 digitOrigins[10][2];
    s32 remainingFrames;
    s32 remainingSeconds;
    s32 digit;
    u32 vertexColor;

    memcpy(digitOrigins, D_003A0928, sizeof(digitOrigins));
    remainingFrames = fldSparkControlState.countdown;
    vertexColor = 0x80808080;
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(0);
    kwlnDrawTexturedColorQuad(20, 20, 113, 31, 1, 0, 113, 31,
                  vertexColor, vertexColor, vertexColor, vertexColor, fldWeatherLimitTexture);
    remainingSeconds = remainingFrames / 30;
    if (remainingSeconds >= 100) {
        remainingSeconds = 99;
    }
    digit = remainingSeconds / 10;
    if (digit > 0) {
        kwlnDrawTexturedColorQuad(92, 37, 15, 16, digitOrigins[digit][0], digitOrigins[digit][1], 15, 16,
                      vertexColor, vertexColor, vertexColor, vertexColor, fldWeatherLimitTexture);
    }
    digit = remainingSeconds % 10;
    kwlnDrawTexturedColorQuad(105, 37, 15, 16, digitOrigins[digit][0], digitOrigins[digit][1], 15, 16,
                  vertexColor, vertexColor, vertexColor, vertexColor, fldWeatherLimitTexture);
}

extern FldSparkController fldSparkControlState;

extern void evtStoreValueAndCaptureWindowPanelValue(s32);

extern void evtSetMessageWindowOptionWhenOpen(s32);

extern void mdlFlagSet(s32);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C468);

extern s32 D_0032E5C4[];

extern FldSparkController fldSparkControlState;

extern void func_001239C8(void);

extern void fldClearObjectEntryHandles(void);

extern void fldReleaseWeatherEffects(void);

extern void fldStartSceneBgmAlternate(void);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void evtSetSolarOverlayFullyVisible(void);

/* Restores field presentation once an active event finishes, then clears the
 * event latch. An already-cleared latch leaves field resources untouched. */
void fldFinishEventFieldState(void) {
    FldAreaWork *state = &fldAreaState;

    if (state->transitionMode != 0) {
        D_0032E5C4[0] = 0;
        func_001239C8();
        fldClearObjectEntryHandles();
        fldReleaseWeatherEffects();
        fldStartSceneBgmAlternate();
        fldPreparePlayerSceneCameraTarget();
        state->transitionMode = 0;
        fldSparkControlState.phase = 0;
        fldSparkControlState.countdown = 0;
        evtSetSolarOverlayFullyVisible();
    }
}

extern void func_0024DD78(void);
extern s8 evtGetCapturedWindowPanelValue(void);
extern void fldAddCameraModelEntry(s32);
extern void func_0026A5F0(s32);
extern s32 mnuPollTitleStreamStateLocked(void);
extern void func_0014B688(void);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern void evtSetSolarOverlayFullyTransparent(void);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0918);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0928);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0978);

void fldUpdateSparkMessageSequence(void) {
    f32 position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    f32 rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    s32 choice;
    void *node;

    switch (fldSparkControlState.dialogPhase) {
    case 12:
        func_0024DD78();
        if (evtGetMessageWindowControlState() == 0) {
            evtFinishMessageWindowAndNotify();
            dspCloseChannel();
            fldSparkControlState.dialogPhase = 11;
            fldAreaState.transitionMode = 0;
            evtSetSolarOverlayFullyVisible();
        }
        break;
    case 1:
        func_0024DD78();
        if (evtGetMessageWindowControlState() == 0) {
            choice = evtGetCapturedWindowPanelValue();
            if (choice == 0) {
                evtFinishMessageWindowAndNotify();
                dspCloseChannel();
                fldSparkControlState.dialogPhase = 5;
                fldSparkControlState.unk2C = 0;
            } else if (choice == 1) {
                evtFinishMessageWindowAndNotify();
                dspCloseChannel();
                fldSparkControlState.dialogPhase = 11;
                fldAreaState.transitionMode = 0;
                evtSetSolarOverlayFullyVisible();
            } else {
                evtFinishMessageWindowAndNotify();
                dspCloseChannel();
                fldSparkControlState.dialogPhase = 4;
                fldSparkControlState.unk2C = 0;
                evtCreateMessageWindowIfMissing((s32)D_0034D8F0);
                dspStartEntry(3);
                evtStoreValueAndCaptureWindowPanelValue(2);
                evtSetMessageWindowOptionWhenOpen(0);
            }
        }
        break;
    case 4:
        func_0024DD78();
        if (evtGetMessageWindowControlState() == 0) {
            evtFinishMessageWindowAndNotify();
            dspCloseChannel();
            fldSparkControlState.dialogPhase = 1;
            fldSparkControlState.unk2C = 0;
        }
        break;
    case 5:
        dds3WorkClear();
        fldReleaseCurrentBgm();
        D_003BAFE8 = fileQueueDefaultCallbackRequest((const char *)D_003A0978);
        fldSparkControlState.dialogPhase = 6;
        fldSparkControlState.unk2C = 0;
        break;
    case 6:
        if (fileIsRequestReadyInCurrentMode(D_003BAFE8) != 0) {
            D_003BAFF0 = fileGetLoadedDataAddress(D_003BAFE8);
            D_003BAFEC = fileGetResourceHandle(D_003BAFE8);
            position[0] = fldAreaState.x;
            position[1] = fldAreaState.y;
            position[2] = fldAreaState.z;
            node = func_00115298((void *)D_003BAFF0, position, rotation);
            fldSparkControlState.object = node;
            effObjClearFlags(node, 1);
            filePollEntryCleanup(D_003BAFE8);
            fldAddCameraModelEntry(0xD);
            D_0032E5C4[0] = 1;
            fldResetArchiveLoadPhase();
            fldSparkControlState.unk2C = 0;
            fldSparkControlState.dialogPhase = 2;
        }
        break;
    case 2:
        if (fldSparkControlState.unk2C == 20) {
            effObjSetFlags(fldSparkControlState.object, 1);
            effObjReplaceActiveEventNode(fldSparkControlState.object, 0);
        }
        fldSparkControlState.unk2C++;
        if (fldStepArchiveLoad() != 0) {
            func_0026A5F0(0x12);
            fldSparkControlState.dialogPhase = 3;
        }
        break;
    case 3:
        if (fldSparkControlState.unk2C == 20) {
            effObjSetFlags(fldSparkControlState.object, 1);
            effObjReplaceActiveEventNode(fldSparkControlState.object, 0);
        }
        fldSparkControlState.unk2C++;
        if (mnuPollTitleStreamStateLocked() == 2) {
            sndSetSequenceVolumePan(0x670010, 0x7F, 0x3F);
            fldSparkControlState.dialogPhase = 8;
        }
        break;
    case 8:
        if (fldSparkControlState.unk2C == 46) {
            kwlnFadeInStart(0xFF, 0xFF, 0xFF, 4);
        }
        if (fldSparkControlState.unk2C == 20) {
            effObjSetFlags(fldSparkControlState.object, 1);
            effObjReplaceActiveEventNode(fldSparkControlState.object, 0);
        }
        if (++fldSparkControlState.unk2C > 50) {
            fldSparkControlState.unk2C = 0;
            fldSparkControlState.dialogPhase = 9;
        }
        break;
    case 9:
        func_0014B688();
        fldLoadWeatherEffects();
        func_001239C8();
        func_0012EA40(0x66, 0);
        fldSparkControlState.unk2C = 0;
        fldSparkControlState.dialogPhase = 10;
        evtSetSolarOverlayFullyTransparent();
        kwlnFadeOutStart(0xFF, 0xFF, 0xFF, 4);
        break;
    case 10:
        fldDrawWeatherLimitTimer();
        if (++fldSparkControlState.unk2C > 10) {
            effObjClearFlags(fldSparkControlState.object, 1);
            dds3RemoveWorldObjectNode(fldSparkControlState.object);
            sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_003BAFEC);
            fldSparkControlState.unk2C = 0;
            fldSparkControlState.dialogPhase = 7;
            mnuTitleStreamUpdateAndLogBgm();
        }
        break;
    case 7:
        fldDrawWeatherLimitTimer();
        fldSparkControlState.dialogPhase = 0;
        fldSparkControlState.phase = 1;
        fldPreparePlayerSceneCameraTarget();
        break;
    case 11:
        fldSparkControlState.phase = 1;
        fldSparkControlState.dialogPhase = 0;
        fldPreparePlayerSceneCameraTarget();
        break;
    }
}

s32 func_0014CAF8(void) {
    return D_003D62C8[0];
}

void fldUpdateSparkEncounterScene(void) {
    f32 firstVector[4];
    f32 secondVector[4];
    s32 phase;
    s32 value;
    void *node;
    FldAreaWork *area;

    memset(firstVector, 0, sizeof(firstVector));
    firstVector[3] = 1.0f;
    memset(secondVector, 0, sizeof(secondVector));
    secondVector[3] = 1.0f;

    phase = fldSparkControlState.phase;
    if (phase == 1) {
        if (fldSparkControlState.mode == 0) {
            if (D_00324530[0] < 0) {
                func_0012EA40(6, 20);
                sndSetSequenceVolumePan(0x670012, 0x7F, 0x3F);
                fldSparkControlState.mode = 1;
                fldSparkControlState.pulse = 1;
                fldSparkControlState.modeCountdown = 20;
                fldSparkControlState.pendingMode = 0;
            }
        } else if (fldSparkControlState.modeCountdown > 0) {
            value = fldSparkControlState.modeCountdown - 1;
            fldSparkControlState.modeCountdown = value;
            if (value == 0) {
                if (fldSparkControlState.pendingMode == 0) {
                    s32 pendingMode;
                    func_0012EA40(1, 0);
                    pendingMode = fldSparkControlState.pendingMode;
                    fldSparkControlState.mode = pendingMode;
                    fldSparkControlState.modeCountdown = 0;
                    fldSparkControlState.pendingMode = 0;
                }
            } else if (fldSparkControlState.mode == 1 && fldSparkControlState.pendingMode == 0 && value < 14 && D_00324530[0] < 0) {
                func_0012EA40(6, 20);
                sndSetSequenceVolumePan(0x670012, 0x7F, 0x3F);
                fldSparkControlState.mode = 1;
                fldSparkControlState.pulse = 1;
                fldSparkControlState.modeCountdown = 20;
            }
        }

        if (func_0014CAF8() == 0 && fldTestSceneControlFlags(0x40) != 0) {
            if (fldSparkControlState.countdown == 300) {
                sndSetSequenceVolumePan(0x670017, 0x7F, 0x3F);
            }
            if (fldSparkControlState.countdown > 0) {
                fldSparkControlState.countdown--;
            }
        }

        fldDrawWeatherLimitTimer();
        if (fldSparkControlState.countdown < 31) {
            fldSparkControlState.countdown = 0;
            func_002E8DD0(0x670017);
            sndSetSequenceVolumePan(0x670016, 0x7F, 0x3F);
            fldReleaseCameraModel(0);
            func_0012EA40(2, 0);
            fldSetCameraNodeModeWithTen();
            fldSparkControlState.phase = 3;
            evtCreateMessageWindowIfMissing((s32)D_0034D8F0);
            dspStartEntry(4);
            fldResetPlayerSceneObjectState();
            fldClearObjectEntryHandles();
            fldReleaseWeatherEffects();
        } else {
            func_0014BDF8();
        }

        if (fldSparkControlState.terminated != 0) {
            func_002E8DD0(0x670017);
            mnuMarkTitleStreamResetPending();
            D_0032E4C0[0] = 1;
            fldResetPlayerSceneObjectState();
            fldSparkControlState.phase = 7;
            mdlFlagSet(0x819);
        }
        return;
    } else if (phase == 3) {
        func_0024DD78();
        if (evtGetMessageWindowControlState() != 0) {
            return;
        }
        evtFinishMessageWindowAndNotify();
        dspCloseChannel();
        fldSparkControlState.frame = 0;
        fldSparkControlState.phase = 6;
        mnuAdvanceTitleStateUnderSemaphore();
        D_003BAFE8 = fileQueueDefaultCallbackRequest(D_003A0978);
    } else if (phase == 6) {
        if (fileIsRequestReadyInCurrentMode(D_003BAFE8) == 0) {
            return;
        }
        D_003BAFF0 = fileGetLoadedDataAddress(D_003BAFE8);
        D_003BAFEC = fileGetResourceHandle(D_003BAFE8);
        area = &fldAreaState;
        firstVector[0] = area->x;
        firstVector[1] = area->y;
        firstVector[2] = area->z;
        node = func_00115298((void *)D_003BAFF0, firstVector, secondVector);
        fldSparkControlState.object = node;
        effObjClearFlags(node, 1);
        filePollEntryCleanup(D_003BAFE8);
        func_0012EA40(5, 0);
        effObjSetFlags(fldSparkControlState.object, 1);
        effObjReplaceActiveEventNode(fldSparkControlState.object, 0);
        fldSparkControlState.phase = 7;
    } else if (phase == 7) {
        fldSparkControlState.frame++;
        if (fldSparkControlState.frame == 40) {
            kwlnFadeInStart(255, 255, 255, 4);
        }
        if (fldSparkControlState.frame < 45) {
            return;
        }
        fldSparkControlState.phase = 8;
    } else if (phase == 8) {
        sndSetSequenceVolumePan(0x670011, 0x7F, 0x3F);
        func_001239C8();
        fldStartSceneBgmAlternate();
        evtSetSolarOverlayFullyVisible();
        D_0032E4E8[0] = 1;
        fldSparkControlState.phase = 9;
        fldSparkControlState.frame = 0;
    } else if (phase == 9) {
        fldSparkControlState.frame++;
        if (fldSparkControlState.frame == 1) {
            kwlnFadeOutStart(255, 255, 255, 4);
        }
        if (fldSparkControlState.frame == 6) {
            node = fldSparkControlState.object;
            effObjClearFlags(node, 1);
            dds3RemoveWorldObjectNode(fldSparkControlState.object);
            sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_003BAFEC);
        }
        if (fldSparkControlState.frame >= 11) {
            D_0032E5C4[0] = 0;
            fldPreparePlayerSceneCameraTarget();
            fldSparkControlState.phase = 0;
            D_0032E4B4[0] = 0;
        }
    } else {
        return;
    }
}

/* Releases scene presentation and camera resources before clearing the event
 * latch and installing the existing field-state values. */
void fldResetEventSceneState(void) {
    fldReleaseCameraModel(0);
    D_0032E5C4[0] = 0;
    fldAreaState.unk118 = 0;
    fldClearObjectEntryHandles();
    fldReleaseWeatherEffects();
    fldPreparePlayerSceneCameraTarget();
    fldAreaState.deferredExit = 1;
    fldAreaState.transitionMode = 0;
    D_003D62A8[0] = 0;
    fldAreaState.unk138 = 1;
    evtSetSolarOverlayFullyVisible();
}

void fldStartDeferredFieldExit(void) {
    evtCreateMessageWindowIfMissing((s32)D_0034D8F0);
    dspStartEntry(5);
    fldResetPlayerSceneObjectState();
    D_0032E4C4[0] = 2;
}

/* Completes deferred exit state 2 only after the message window stops being
 * controlled, then closes its display channel and restores the camera target. */
void fldFinishDeferredExit(void) {
    if (fldAreaState.deferredExit == 2) {
        func_0024DD78();
        if (!evtGetMessageWindowControlState()) {
            evtFinishMessageWindowAndNotify();
            dspCloseChannel();
            fldPreparePlayerSceneCameraTarget();
            fldAreaState.deferredExit = 0;
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

