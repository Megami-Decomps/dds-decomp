#include "fld_area_work.h"
#include "sdf_packet_list.h"
#include "ee_mmi.h"
#include "kwln.h"
#include "pcp_vu0.h"
#include "common.h"
#include "sdf_model_scalars.h"
#include "dds3_path.h"
#include "sdf_dev_state.h"
#include "sdf_resource.h"
#include "field_stage.h"
#include "eff_blur.h"
#include "fpu.h"
#include "fld.h"
#include "evt_world.h"
#include "dds3obj.h"
#include "mdl.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"

extern u32 D_003BABE0;

extern s32 fldDeferredCommand;

extern u32 fldSceneControlFlags;

/* The native controller keeps the flags base and accesses its stage at +4.
 * Both original four-byte SDATA labels retain their storage. */
typedef struct FldSequenceController {
    u32 flags;
    s32 stage;
} FldSequenceController;

typedef char FldSequenceController_size_check[
    sizeof(FldSequenceController) == 8 ? 1 : -1];

extern FldSequenceController fldSceneLifecycleFlags;

extern s32 D_003BABF0;

extern u32 D_003BABD0;

extern void *dds3GetWorldSecondaryObject(void);





extern u32 dds3GetObjectPayloadWord8(EffWorldNode *object);

extern void dds3SetObjectPayloadWord8(EffWorldNode *object, u32 value);

extern u32 fldDeferredCommandParameter;

extern s32 D_003BAB08;

extern s32 D_003BAB0C;

extern s32 D_003BAB10;

extern s32 D_003BAB14;

extern s32 D_003BAB18;

extern s32 D_003BAB1C;

extern s32 D_003BAB20;

extern s32 D_0032E4DC[];

extern u8 D_00324F88[];

extern u8 D_003257F8[];

extern u8 D_00324530[];

extern char D_0039FCA0[];

extern char D_0039FCB0[];

extern char D_0039FCC8[];

extern void func_0013E5A8(u32 arg0);

extern u32 fldGetSceneReadyFlag(void);

extern struct ScrData *scrFindNamedProcessNode(char *arg0);

extern void sdfStoreMessageWordsAndNotifyConsumer(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern u32 fldPlayerObject;

extern u32 fldPlayerModelResource;

extern u32 D_0032E498[];

extern u32 D_003BAB58;

extern u32 D_0032E3D0[];

extern u32 D_003BABE8;

extern u32 fldCameraModelObject;

extern void fldSetPendingAreaAndFloor(u32, u32);

extern s32 fldIsAreaResourceReady(void);

extern u32 fldPollAreaResourceLoad(void);

extern u32 fldGetResourceReadyFlag(void);

extern void fldFreeDisplayObjects(void);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern s8 dds3AdminGetRequestedMode(void);

extern s32 dds3AdminReadPreviousUnsignedSample(void);

extern void func_0013F100(s32, u32);

extern u32 D_0032E4EC[];

extern s32 D_003BABEC;

extern void mdlFlagSet(s32);

void fldDispatchDeferredFieldCommand(void);

extern FieldPlayerSceneWork D_0032F1A0;

extern u32 dds3AdvanceWorldCounter(void);

extern void dds3WorkClear(void);

extern char D_0039FBC0[]; /* "fldProcSequence" */

struct EffWorldNode;

extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);

extern u8 D_003BAB3C;

extern u8 D_0032C9A0[];

extern u32 mnuAcknowledgeCampState(void);

void fldClearSceneCommandFlag(void);

void fldClearFieldTransitionFlag(void);

void fldClearSecondarySceneFlag(void);

void fldClearPrimarySceneFlag(void);

void fldClearSceneControlFlags(u32 arg0);

extern u32 D_003BAB64;

extern u32 D_003BAB60;

extern u32 D_003BAB5C;

extern void mdlLoadViewerPackage(s32, s32, s32, void *, u32);

void fldLoadPlayerModel(void);

u8 fldTestSceneControlFlags(u32 arg0);

u32 *fldGetPlayerSceneStateAddress(void);

extern u32 D_003BAC08[2];

extern void fldResetEventSceneState(void);

extern u32 func_0014D100(void);

extern void fldResetTaskSlots(void);

extern void fldSetSceneLifecycleFlags(u32);

extern void evtKillFieldScriptTasks(void);

extern char D_003BAAE0[];

extern char D_003BAAE8[];

extern char D_003BAAF0[];

extern char D_003BAAF8[];

extern char D_003BAB00[];

extern u8 fldGetCampSceneControlMode(void);

extern u32 D_0032E570[];



extern u32 D_0032E570[];

/* Search location records 1..639; a miss returns the reserved first record, * not NULL, so callers can read the fallback record's fields. */ s16 *fldFindLocationCoordinateRecord(s32 x, s32 y);

/* Read the matching location record's fourth signed halfword; zero on miss. */ s32 fldGetLocationCoordinateValue(s32 x, s32 y);

void fldLoadFieldTables(void);

void fldTestDrawCreate(void);

void fldTestDrawDestroy(void);

/* Return to-minus-from after integer-degree reduction and one wrap adjustment. */
f32 fldAngleDifference(f32 fromAngle, f32 toAngle) {
    f32 difference;

    if (fromAngle < 0.0f || toAngle < 0.0f) {
        fromAngle += 360.0f;
        toAngle += 360.0f;
    }
    fromAngle = (s32)fromAngle % 360;
    toAngle = (s32)toAngle % 360;
    difference = fromAngle - toAngle;
    if (difference > 180.0f || difference < -180.0f) {
        if (fromAngle < toAngle) {
            fromAngle += 360.0f;
        } else {
            toAngle += 360.0f;
        }
    }
    return toAngle - fromAngle;
}

f32 fldSnapAngleToCompassPoint(f32 angle) {
    f32 compass[16] = {0.0f, 22.5f, 45.0f, 67.5f, 90.0f, 112.5f, 135.0f, 157.5f, 180.0f, 202.5f, 225.0f, 247.5f, 270.0f, 292.5f, 315.0f, 337.5f};
    f32 best;
    s32 index;
    f32 diff;

    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(22.5f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(fldAngleDifference(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(67.5f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(112.5f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(fldAngleDifference(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(fldAngleDifference(157.5f, angle));
    if (diff < best) {
        best = diff;
        index = 7;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 8;
    }
    diff = fabsf(fldAngleDifference(202.5f, angle));
    if (diff < best) {
        best = diff;
        index = 9;
    }
    diff = fabsf(fldAngleDifference(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 10;
    }
    diff = fabsf(fldAngleDifference(247.5f, angle));
    if (diff < best) {
        best = diff;
        index = 11;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 12;
    }
    diff = fabsf(fldAngleDifference(292.5f, angle));
    if (diff < best) {
        best = diff;
        index = 13;
    }
    diff = fabsf(fldAngleDifference(315.0f, angle));
    if (diff < best) {
        best = diff;
        index = 14;
    }
    diff = fabsf(fldAngleDifference(337.5f, angle));
    if (diff < best) {
        index = 15;
    }
    return compass[index];
}

/* Snap an angle in degrees to the nearest of the eight compass directions. */
f32 fldSnapAngleToCompassOctant(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(fldAngleDifference(315.0f, angle));
    if (diff < best) {
        index = 7;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 1:
        angle = 45.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 3:
        angle = 135.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 5:
        angle = 225.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    case 7:
        angle = 315.0f;
        break;
    }
    return angle;
}

f32 fldSnapAngleToCardinalDirection(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    }
    return angle;
}

INCLUDE_ASM(const s32, "game/code_00122030", func_001228D8);

extern f32 func_001228D8(f32, f32, f32, f32);

f32 fldGetNormalizedComplementaryAngle(f32 ax, f32 ay, f32 bx, f32 by) {
    s32 angle = (s32)(360.0f - func_001228D8(ax, ay, bx, by) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "game/code_00122030", func_00122A00);

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

/* For kind-6 objects in mode 4, set mode 3 or clear it to 0.
 * status covers the count, payload mode word and cursor result. */
void fldToggleWorldNodeState(s64 clearMode) {
    WorldIndexNode *valueChain;
    u32 status;
    EffWorldNode *worldObject;

    valueChain = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 6);
    status = dds3GetWorldValueCount((WorldValueIndices *)valueChain);
    if (status == 0) {
        return;
    }
    dds3ResetObjectValueCursor((WorldValueIndices *)valueChain);
    do {
        worldObject = (EffWorldNode *)dds3ReadIndexedWorldObjectWord((WorldValueIndices *)valueChain);
        status = dds3GetObjectPayloadWord8(worldObject);
        if (status == 4) {
            if (clearMode == 0) {
                dds3SetObjectPayloadWord8(worldObject, 3);
            }
            else {
                dds3SetObjectPayloadWord8(worldObject, 0);
            }
        }
        status = dds3AdvanceObjectValueCursor((WorldValueIndices *)valueChain);
    } while (status != 0);
    dds3DestroyWorldIndexNode(valueChain);
}

extern u32 D_0032E570[];

extern void sdfResetGameRuntime(s32);

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

void fldSetDeferredFieldCommand(u32, u32);

extern void dds3AdminSubmitModeRequest(s32, void *, u32, s32);

void fldPrepareDeferredSceneTransition(void) {
    if (datGameState->header.transition != 0) {
        sdfResetGameRuntime(1);
    } else {
        sdfResetGameRuntime(0);
    }
    if (fileLoadStateChanged() == 0) {
        fileCacheSlotFlagsFromState();
    } else {
        fileRestoreSlotFlagsToState();
    }
    mdlFlagSet(0xc0f);
    {
        s32 code;
        D_0032E570[0x44 / 4] = 1;
        code = 0x259;
        D_0032E570[0x4c / 4] = 0;
        fldSetDeferredFieldCommand(0x15, 0x259);
        dds3AdminSubmitModeRequest(6, &code, 4, 0);
    }
}

extern s32 fileGetSelectionPendingFlag(void);

extern void func_00120C08(s32);

extern char D_003BAB68[];

void fldStartSequenceRecord(void) {
    FieldSequenceRecord record;

    if (fileGetSelectionPendingFlag() == 1) {
        return;
    }
    if (datGameState->header.transition != 0) {
        fldPrepareDeferredSceneTransition();
        return;
    }
    func_00120C08(1);
    D_0032E570[0x4C / 4] = 0;
    D_0032E570[0x44 / 4] = 1;
    mdlFlagSet(0xC0F);
    fldSetDeferredFieldCommand(0, 0);
    fldInitializeSequenceAndResetFlags(&record, 1, 1, D_003BAB68);
    record.options = 1;
    dds3AdminSubmitModeRequest(5, &record, 0xA0, 0);
}

extern u64 D_003BAB70[], D_003BAB78[], D_003BAB80[], D_003BAB88[], D_003BAB90[];

extern u64 D_003BAB98[], D_003BABA0[], D_003BABA8[], D_003BABB0[];

extern u8 D_0032F240[];

extern void func_003003F0(void *);

extern void fldInitDisplayObjects(void);

extern void fldResetPlayerSceneTransformState(void);

extern void fldResetPendingSounds(void);

extern void effMiscSeedRandom(void *, s32);

extern void fldParseMixLb(void);

extern void fldLoadBattleSkyAndFilter(void);

extern void func_001426E0(void);

void fldInitializeDisplayAndTables(void) {
    func_003003F0(D_003BAB70);
    fldInitDisplayObjects();
    func_003003F0(D_003BAB78);
    fldResetPlayerSceneTransformState();
    func_003003F0(D_003BAB80);
    fldResetPendingSounds();
    func_003003F0(D_003BAB88);
    effMiscSeedRandom(D_0032F240, 0x1e240);
    func_003003F0(D_003BAB90);
    fldParseMixLb();
    func_003003F0(D_003BAB98);
    fldLoadBattleSkyAndFilter();
    func_003003F0(D_003BABA0);
    func_001426E0();
    func_003003F0(D_003BABA8);
    fldLoadFieldTables();
    func_003003F0(D_003BABB0);
}

void fldResetPlayerSceneTransformState(void) {
    FieldPlayerSceneWork *sceneWork = &D_0032F1A0;

    VU0_STORE_VF(vf0, &sceneWork->position);
    VU0_STORE_VF(vf0, &sceneWork->rotation);
    fldPlayerObject = 0;
    *fldGetPlayerSceneStateAddress() = 0;
}

/* Fill a sequence packet for stage/kind/name, then clear the temporary override.
 * Unspecified packet bytes deliberately retain their previous contents. */
void fldInitializeSequenceAndResetFlags(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
    D_003BAB3C = 0;
    D_0032C9A0[0] = 0;
}

/* Fill the alternate-mode sequence packet for stage/kind/name.
 * Reuse a running sequence in the same area; otherwise request a work clear. */
void fldInitializeAlternateSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 3;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a field sequence packet, including its narrowed code, link and detail name.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldInitializeFieldSequenceRecord(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->mode = 2;
    record->unk_62 = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Fill a sequence packet with a narrowed code and note instead of a detail/link.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldInitializeSequenceWithNote(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (fldAreaState.area == stage) {
            fldAreaState.unk20 = 1;
        } else {
            dds3WorkClear();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = code;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    strcpy(record->note, subname);
    record->options = 0;
}

void fldInitializeLinkedSequence(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->enabled = 1;
    record->stage = stage;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->mode = 2;
    record->unk_62 = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

/* Converts the field work's primary and secondary state words into a
 * compact scene status, giving the primary state precedence. */
u32 fldGetSceneStatusCode(void) {
    FieldPlayerSceneWork *sceneWork = &D_0032F1A0;

    if (sceneWork->primaryState == 1) {
        return 1;
    }
    if (sceneWork->secondaryState == 3) {
        return 3;
    }
    if (sceneWork->secondaryState == 4) {
        return 4;
    }
    if (sceneWork->secondaryState == 5) {
        return 5;
    }
    if (sceneWork->secondaryState == 6) {
        return 2;
    }
    return sceneWork->primaryState != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "game/code_00122030", func_001233D0);

void fldUnloadPlayerModel(void);


/* Load the coordinate/field-selected player variant only when it changes or its
 * resource is absent; the cached variant is part of native area work. */
INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FBC0);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FBD0);

void fldLoadPlayerModel(void) {
    s32 model;

    if (fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1) & 0x20) {
        model = 2;
    } else {
        model = fldAreaState.unk118 != 0;
    }
    if (fldAreaState.playerModelVariant != model || fldPlayerModelResource == 0) {
        if (fldPlayerModelResource != 0) {
            fldUnloadPlayerModel();
        }
        switch (model) {
        case 0:
            fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_a.PB", &D_003BAB5C, &D_003BAB64);
            break;
        case 1:
            fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_b.PB", &D_003BAB5C, &D_003BAB64);
            break;
        default:
            fldPlayerModelResource = (u32)sdfReadNamedResource("/model/field/player_l.PB", &D_003BAB5C, &D_003BAB64);
            break;
        }
        fldAreaState.playerModelVariant = model;
    }
}

void fldUnloadPlayerModel(void) {
    if (fldPlayerModelResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)fldPlayerModelResource);
        fldPlayerModelResource = 0;
        D_0032E4EC[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00122030", func_001239C8);

void fldPrepareResourceBuffer(void) {
    void *source;
    void *buffer;
    fldLoadPlayerModel();
    D_003BAB60 = D_003BAB64;
    D_003BAB58 = (u32)sdfAllocGeneralBlock(D_003BAB64);
    source = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)fldPlayerModelResource);
    buffer = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)D_003BAB58);
    memcpy(buffer, source, D_003BAB60);
    D_003BAB5C = (u32)buffer;
    mdlLoadViewerPackage(2, 0, 0x101, buffer, D_003BAB60);
    sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_003BAB58);
    D_003BAB58 = 0;
}

void fldReleaseResources(void) {
    if (D_003BAB58 != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)D_003BAB58);
        D_003BAB58 = 0;
    }
    if (D_0032E3D0[0] == 0 && D_003BABE8 == 0) {
        fldUnloadPlayerModel();
        fldSetPendingAreaAndFloor(0, 0);
    }
}

void fldReleasePlayerSceneResources(void) {
    FieldPlayerSceneWork *sceneWork;

    if (fldPlayerObject != 0) {
        if (dds3GetWorldSecondaryObject() != 0) {
            sceneWork = &D_0032F1A0;
            effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->position);
            effObjFetchInnerRotationNormalized((EffWorldNode *)fldPlayerObject);
            VU0_STORE_VF(vf10, &sceneWork->rotation);
        }
        fldPlayerObject = 0;
        fldCameraModelObject = 0;
        *fldGetPlayerSceneStateAddress() = 0;
        fldReleaseResources();
    }
}

void fldCleanupFieldScene(void) {
    fldResetTaskSlots();
    fldTestDrawDestroy();
    fldReleasePlayerSceneResources();
    evtDestroySecondaryWorldNode();
    fldReleaseCampSceneTasks();
}

u32 * fldGetPlayerSceneStateAddress(void) {
    return &D_003BABD0;
}

u32 fldGetPlayerSceneState(void) {
    return *fldGetPlayerSceneStateAddress();
}

extern s32 dds3InvokeSlot1Handler(void *object, Dds3MoverUpdate update);

s32 func_001243C0(ObjectTransform *relativeTransform, EffWorldNode *target);

extern void func_001372D0(f32 *position);

void fldSetSceneControlFlags(u32 mask);

/* Raise the scene-state minima, clear pending slots and aim ten units above
 * the fetched player position. Existing larger mode/state values are retained. */
void fldPreparePlayerSceneCameraTarget(void) {
    f32 position[4];

    if (fldPlayerObject != 0) {
        fldSetSceneControlFlags(0x40);
        dds3InvokeSlot1Handler((void *)fldPlayerObject, func_001243C0);
    }
    fldAreaState.unkE8 = 4;
    if (fldAreaState.sceneMode < 4) {
        fldAreaState.sceneMode = 4;
    }
    if (fldAreaState.sceneState < 5) {
        fldAreaState.sceneState = 5;
    }
    fldAreaState.unk90 = -1;
    fldAreaState.unk94 = -1;
    fldUpdateCameraTarget();
    effObjFetchInnerPosition((EffWorldNode *)fldPlayerObject);
    VU0_STORE_VF(vf10, position);
    position[1] += 10.0f;
    func_001372D0(position);
}

void fldResetPlayerSceneObjectState(void) {
    if (fldPlayerObject != 0) {
        fldClearSceneControlFlags(0x40);
        dds3InvokeSlot1Handler((void *)fldPlayerObject, 0);
    }
    D_0032E498[0] = 4;
}

extern void dds3ClearObjectFlags(void *, s32);


extern void func_00111E30(u32, s32, s32);

extern EffWorldNode *dds3SpawnCameraSlotObj5(s32, void *, void *);

extern s32 D_0032F1DC[];

extern char D_0039FC50[]; /* "PLAYER_UNIT" */

extern s32 D_003BAB50;

/* Initialize the homogeneous vectors and create/register the player if absent. */
void fldCreatePlayerObject(void) {
    f32 position[4];
    f32 rotation[4];

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    memset(rotation, 0, sizeof(rotation));
    rotation[3] = 1.0f;
    if (fldPlayerObject == 0) {
        fldPlayerObject = (u32)dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), position, rotation);
        dds3SetWorldNodeValue((struct EffWorldNode *)fldPlayerObject, (u32)D_0039FC50);
        dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)fldPlayerObject);
        if (D_003BAB50 != 0) {
            dds3ClearObjectFlags((void *)fldPlayerObject, 0x20);
        }
        fldPrepareResourceBuffer();
        func_00111E30(fldPlayerObject, 2, D_0032F1DC[0]);
    }
}

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

extern FieldVec4 D_0039FC60;

extern FieldVec4 D_0039FC70;

extern u128 D_0032F170[3];

extern char D_003BABB8[];

extern void effMiscAxisAngleToQuaternionVU(f32);

extern void effMiscQuatMultiplyVU(void);

extern void effObjSetInnerFloat(EffWorldNode *, f32);


extern void fldResetCameraModelHandles(void);


extern void dds3SetObjectFlags(void *, s32);

extern void func_00133640(s16, s32);

extern void func_00132FD0(u32, s32);

extern void fldBeginSelectedValueTransition(u32);

s32 fldGetSceneCommandState(void);

void fldEnterSceneCommand(void);

extern void fldSetCameraObjectActiveFlag(s32);

extern void func_001312D8(void);

extern EffWorldNode *evtSpawnActionObj11(s32, void *, s32);

/* Install the two supplied homogeneous transform vectors and prepare field
 * model, scene commands and the borrowed action transform. */
void fldPreparePlayerAndCameraScene(u128 *transform) {
    FieldVec4 quaternion;
    FieldVec4 axis;
    FieldVec4 scale;
    u128 *rotation;
    EffWorldNode *object;

    memset(&quaternion, 0, sizeof(quaternion));
    quaternion.w = 1.0f;
    axis = D_0039FC60;
    scale = D_0039FC70;
    VU0_LOAD_VF(vf10, &axis);
    rotation = transform + 1;
    effMiscAxisAngleToQuaternionVU(3.1415926f);
    VU0_LOAD_VF(vf11, rotation);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF(vf10, &quaternion);
    object = (EffWorldNode *)fldPlayerObject;
    if (object == 0) {
        object = dds3SpawnCameraSlotObj5(dds3AdvanceWorldCounter(), transform, &quaternion);
        fldPlayerObject = (u32)object;
        dds3SetWorldNodeValue(object, (u32)D_0039FC50);
        dds3SetWorldPlayerObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)fldPlayerObject);
        if (D_003BAB50 != 0) {
            dds3ClearObjectFlags((void *)fldPlayerObject, 0x20);
        }
        fldPrepareResourceBuffer();
        func_00111E30(fldPlayerObject, 2, D_0032F1DC[0]);
        object = (EffWorldNode *)fldPlayerObject;
    } else {
        ObjectTransform *inner = object->inner;
        PCP_COPY_VECTOR(inner->position, transform);
        PCP_COPY_VECTOR(inner->smoothedPosition, transform);
        PCP_COPY_VECTOR(inner->rotation, rotation);
    }
    effObjSetInnerPosition(object, transform);
    effObjSetInnerRotation((EffWorldNode *)fldPlayerObject, (u128 *)&quaternion);
    effObjSetInnerScale((EffWorldNode *)fldPlayerObject, (u128 *)&scale);
    fldCameraModelObject = dds3GetObjectBaseResourceHandle((EffWorldNode *)(u32)fldPlayerObject);
    effObjSetInnerFloat((EffWorldNode *)fldPlayerObject, 90.0f);
    if (fldAreaState.unk118 == 0) {
        mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 0, 2);
    } else {
        mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 0, 2);
        mdlAddEntryFlagged((MdlCtx *)fldCameraModelObject, 1, 2);
        fldResetCameraModelHandles();
    }
    sdfSetModelScalarOverrides(((MdlCtx *)fldCameraModelObject)->inner, 15.0f, 0.0f);
    dds3SetObjectFlags((void *)fldPlayerObject, 0x200);
    if (fldAreaState.area < 200) {
        func_00133640(0, 0);
        func_00132FD0(D_0032E570[11], 0);
        fldBeginSelectedValueTransition(D_0032E570[12]);
        if (fldGetSceneCommandState() == 1) {
            if (fldAreaState.commandEnabled != 0) {
                fldEnterSceneCommand();
            } else {
                fldAreaState.sceneCommand = 0;
                func_00133640(0, 0);
                func_00132FD0(D_0032E570[11], 0);
                fldBeginSelectedValueTransition(D_0032E570[12]);
            }
        } else if (fldAreaState.commandEnabled == 0) {
            fldAreaState.sceneCommand = 0;
        }
        fldSetCameraObjectActiveFlag(fldAreaState.commandEnabled);
    }
    func_001312D8();
    evtSpawnActionObj11(dds3AdvanceWorldCounter(), D_0032F170, (s32)D_003BABB8);
}

extern FieldVec4 D_0039FC80;

extern FieldVec4 D_0039FC90;

extern char D_003BABC0[];

extern u32 dds3CreateConfiguredCameraObject(s32, FieldVec4 *, FieldVec4 *, FieldVec4 *);

extern void dds3SetCameraVector(struct EffWorldNode *camera, u128 *worldEye);

extern void effObjSetInnerFloat(EffWorldNode *, f32);


/* Create the secondary camera at target origin with the stored eye/up vectors. */
void fldCreateSecondaryWorldCamera(void) {
    FieldVec4 localUp = D_0039FC80;
    FieldVec4 targetPosition;
    FieldVec4 worldEye;
    u32 *cameraObjectSlot = &D_003BABD0;
    u32 cameraObject;
    memset(&targetPosition, 0, sizeof(targetPosition));
    targetPosition.w = 1.0f;
    worldEye = D_0039FC90;
    cameraObject = dds3CreateConfiguredCameraObject(dds3AdvanceWorldCounter(), &targetPosition, &worldEye, &localUp);
    *cameraObjectSlot = cameraObject;
    dds3SetWorldNodeValue((struct EffWorldNode *)cameraObject, (u32)D_003BABC0);
    dds3SetCameraVector((struct EffWorldNode *)*cameraObjectSlot, (u128 *)&worldEye);
    effObjSetInnerFloat((EffWorldNode *)*cameraObjectSlot, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), (EffWorldNode *)*cameraObjectSlot);
}

s32 func_001243C0(ObjectTransform *relativeTransform, EffWorldNode *target) {
    return 0;
}

s32 fldSelectSceneCommand(void) {
    s32 mode = fldAreaState.area;
    s32 result = 0x64;
    if (mode == 0x15) {
        s32 variant = fldAreaState.floor;
        result = 0x66;
        if (variant == 2) result = 0x64;
        if (variant == 0x17) result = 0x65;
    }
    if (mode == 0x1a) {
        s32 variant = fldAreaState.floor;
        result = 0x66;
        if (variant == 0x12) result = 0x65;
        if (variant == 0x13) result = 0x65;
        if (variant == 0x15) result = 0x65;
    }
    if (mode == 0x1d) {
        s32 variant = fldAreaState.floor;
        result = 0x66;
        if (variant == 1) result = 0x64;
        if (variant == 9) result = 0x65;
        if (variant == 0xa) result = 0x65;
    }
    if (mode == 0x25 && fldAreaState.floor == 0xf) {
        result = 0x64;
    }
    return result;
}

void fldConsumeSceneCommandFlag(void) {
    if ((datGameState->world.fieldFlags & 8) != 0) {
        fldClearSceneCommandFlag();
        fldAreaState.consumedFlags |= 1;
    }
}

void fldClearSceneCommandFlag(void) {
    datGameState->world.fieldFlags &= ~8;
    fldAreaState.consumedFlags &= ~1;
}

extern void fldPlayFieldSeVolumePan(s32);

extern void func_00133640(s16, s32);

void fldEnterSceneCommand(void) {
    s32 code;

    if (fldAreaState.commandEnabled == 0) {
        return;
    }
    if ((datGameState->world.fieldFlags & 8) == 0) {
        fldPlayFieldSeVolumePan(0x29);
    }
    code = fldSelectSceneCommand();
    fldAreaState.sceneCommand = code;
    datGameState->world.fieldFlags |= 8;
    fldAreaState.consumedFlags &= ~1;
    func_00133640(code, 0);
}

s32 fldGetSceneCommandState(void) {
    if ((datGameState->world.fieldFlags & 8) != 0) {
        return 1;
    }
    if (D_0032E4DC[0] != 0) {
        return 0;
    }
    return -1;
}

/* Latch the area/floor command while enabled and flagged; stop it when either
 * gate clears. Disable passes transition parameter 0; flag removal passes 20. */
void fldUpdateSceneCommand(void) {
    FldAreaWork *state = &fldAreaState;
    s32 code;

    if (state->commandEnabled == 0) {
        if (state->sceneCommand != 0) {
            state->sceneCommand = 0;
            func_00133640(0, 0);
        }
    } else if ((datGameState->world.fieldFlags & 8) != 0) {
        if (state->sceneCommand == 0) {
            code = fldSelectSceneCommand();
            state->sceneCommand = code;
            func_00133640(code, 0x14);
        }
    } else if (state->sceneCommand != 0) {
        state->sceneCommand = 0;
        func_00133640(0, 0x14);
        fldPlayFieldSeVolumePan(0x2A);
    }
}

/* Game-state work flags and area-state consumption markers use different bits. */
enum {
    FIELD_TRANSITION_WORK_FLAG = 4,
    FIELD_SECONDARY_SCENE_WORK_FLAG = 2,
    FIELD_PRIMARY_SCENE_WORK_FLAG = 1,
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

/* Clear a set transition flag and mark its consumption in area state. */
void fldConsumeFieldTransitionFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_TRANSITION_WORK_FLAG) != 0) {
        fldClearFieldTransitionFlag();
        fldAreaState.consumedFlags |= FIELD_TRANSITION_CONSUMED_FLAG;
    }
}

/* Clear the transition work flag and its area-state consumption marker. */
void fldClearFieldTransitionFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
}

/* Set the transition work flag and reset its area-state consumption marker. */
void fldSetFieldTransitionFlag(void) {
    datGameState->world.fieldFlags |= FIELD_TRANSITION_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_TRANSITION_CONSUMED_FLAG;
}

/* Return whether the transition work flag is set. */
u8 fldTestFieldTransitionFlag(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_TRANSITION_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set secondary-scene flag and mark its consumption in area state. */
void fldConsumeSecondarySceneFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_SECONDARY_SCENE_WORK_FLAG) != 0) {
        fldClearSecondarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the secondary-scene work flag and its consumption marker. */
void fldClearSecondarySceneFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Select secondary rather than primary, resetting secondary's consume marker. */
void fldSetSecondarySceneFlag(void) {
    datGameState->world.fieldFlags = (datGameState->world.fieldFlags | FIELD_SECONDARY_SCENE_WORK_FLAG) & ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_SECONDARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the secondary-scene work flag is set. */
u8 fldTestSecondarySceneFlag(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_SECONDARY_SCENE_WORK_FLAG;
    return fieldFlags != 0;
}

/* Clear a set primary-scene flag and mark its consumption in area state. */
void fldConsumePrimarySceneFlag(void) {
    if ((datGameState->world.fieldFlags & FIELD_PRIMARY_SCENE_WORK_FLAG) != 0) {
        fldClearPrimarySceneFlag();
        fldAreaState.consumedFlags |= FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
    }
}

/* Clear the primary-scene work flag and its area-state consumption marker. */
void fldClearPrimarySceneFlag(void) {
    datGameState->world.fieldFlags &= ~FIELD_PRIMARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Select primary rather than secondary, resetting primary's consume marker. */
void fldSetPrimarySceneFlag(void) {
    datGameState->world.fieldFlags = (datGameState->world.fieldFlags | FIELD_PRIMARY_SCENE_WORK_FLAG) & ~FIELD_SECONDARY_SCENE_WORK_FLAG;
    fldAreaState.consumedFlags &= ~FIELD_PRIMARY_SCENE_CONSUMED_FLAG;
}

/* Return whether the primary-scene work flag is set. */
u8 fldIsFlagActive(void) {
    s32 fieldFlags = datGameState->world.fieldFlags;
    fieldFlags &= FIELD_PRIMARY_SCENE_WORK_FLAG;
    if (fieldFlags == 0) return 0;
    return 1;
}

void func_001248D0(void) {
    sdfStoreMessageWordsAndNotifyConsumer(D_003257F8, (s32)D_00324F88, (s32)(D_00324F88 + 0xC0), (s32)(D_00324F88 + 0x100), (s32)(D_00324F88 + 0xE0));
}

typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;

extern FldEncEntry *fldEncounterRollTable;

extern s32 mdlFlagTest();

extern s32 effMiscRand();

enum {
    FIELD_ENCOUNTER_ROLL_ENTRY_COUNT = 16
};

#define FIELD_ENCOUNTER_PERCENT_RANGE 100U

/* Return the first successful stage/flag-gated roll, or -1 when none succeed.
 * Each eligible row rolls independently; chance retains its unsigned cast. */
s16 fldRollEncounter(void) {
    s32 encounterIndex;
    s32 eligible;

    for (encounterIndex = 0; encounterIndex < FIELD_ENCOUNTER_ROLL_ENTRY_COUNT; encounterIndex++) {
        if (fldEncounterRollTable[encounterIndex].stage == fldAreaState.area) {
            eligible = 1;
            if (fldEncounterRollTable[encounterIndex].flag != -1) {
                eligible = mdlFlagTest(fldEncounterRollTable[encounterIndex].flag) != 0;
            }
            if (eligible != 0) {
                if ((u32)effMiscRand(0) % FIELD_ENCOUNTER_PERCENT_RANGE < (u32)fldEncounterRollTable[encounterIndex].chance) {
                    return fldEncounterRollTable[encounterIndex].result;
                }
            }
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00122030", func_001249E0);

/* Queue the current area and next floor, reset the scene lifecycle, and return -1.
 * The pending pair is consumed later with a 200-entry offset on the area value. */
s32 fldAdvanceToNextScene(void) {
    u32 scene;
    u32 area;

    fldResetEventSceneState();
    D_003BAC08[0] = 0;
    D_003BAC08[1] = func_0014D100();
    scene = fldAreaState.floor;
    area = fldAreaState.area;
    fldAreaState.nextArea = area;
    fldAreaState.nextFloor = scene + 1;
    fldResetTaskSlots();
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    fldResetPlayerSceneObjectState();
    evtKillFieldScriptTasks();
    return -1;
}

u8 fldHasKiretaLabelProcess(void) {
    return scrFindNamedProcessNode(D_0039FCA0) != 0;
}

u8 fldHasHirakenaiLabelProcess(void) {
    return scrFindNamedProcessNode(D_0039FCC8) != 0;
}

u8 fldHasBadkaifukuLabelProcess(void) {
    return scrFindNamedProcessNode(D_0039FCB0) != 0;
}

u8 fldGetCampSceneControlMode(void) {
    if (D_003BABEC > 0) {
        return 2;
    }
    if (mnuAcknowledgeCampState() != 0) {
        return 1;
    }
    return fldTestSceneControlFlags(0x20) != 0 ? 0 : 3;
}

extern void *dds3GetWorldObject(void);


extern void kwlnFadeStartIn(s32);

extern void kwlnFadeInStart(s32, s32, s32, s32);

extern void func_00131688(void);

extern u8 fldHasPendingSceneFlags(void);

extern void fldSetCameraNodeModeWithTen(void);

extern void kwlnFadeStartOut(s32);

extern void mnuCreateCampTasks(void);

extern void evtSetSolarOverlayFullyVisible(void);

extern void evtSetSolarOverlayFullyTransparent(void);

extern void fldApplySkyLightSetToPlayerVU(void);

extern void sndSetSequenceVolumePan(s32, s32, s32);

extern void fldClearSceneLifecycleFlags(u32);

extern u8 fldTestSceneLifecycleFlags(u32);

extern s16 D_0032E4D8[];

s32 func_00124F58(void) {
    FldAreaWork *scene;
    s32 control;

    if (D_003BABEC > 0) {
        D_003BABEC--;
        if (D_003BABEC == 0) {
            dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 0);
            kwlnFadeStartIn(4);
            mnuCreateCampTasks();
        }
        return 0;
    }
    if (fldTestSceneControlFlags(0x20) == 0 && fldGetCampSceneControlMode() == 3) {
        fldClearSceneLifecycleFlags(1);
        fldPreparePlayerSceneCameraTarget();
        evtSetSolarOverlayFullyVisible();
        /* Retail retains both branches of this shared scene-state gate. */
        if (D_0032E4D8[0] != 0) {
            fldApplySkyLightSetToPlayerVU();
        } else {
            fldApplySkyLightSetToPlayerVU();
        }
        dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 1);
        fldSetSceneControlFlags(0x20);
        kwlnFadeStartOut(0);
        kwlnFadeStartIn(8);
        return 0;
    }
    control = fldTestSceneControlFlags(0x20);
    if (control == 0) {
        return control;
    }
    control = fldTestSceneControlFlags(0x40);
    if (control == 0) {
        return control;
    }
    if (fldTestSceneLifecycleFlags(1) == 1 || fldHasPendingSceneFlags() != 0 || fldGetSceneReadyFlag() != 0) {
        return 0;
    }
    scene = &fldAreaState;
    if (scene->sceneMode == 0 && (s8)D_00324530[0] < 0) {
        sndSetSequenceVolumePan(0xE, 0x7F, 0x3F);
        fldSetSceneLifecycleFlags(1);
        fldResetPlayerSceneObjectState();
        func_00131688();
        evtSetSolarOverlayFullyTransparent();
        scene->sceneMode = 4;
        scene->sceneState = 5;
        fldClearSceneControlFlags(0x20);
        kwlnFadeInStart(0, 0, 0, 4);
        D_003BABEC = 5;
        fldSetCameraNodeModeWithTen();
    }
    return 0;
}

u8 fldGetSceneReadyOrPendingState(void) {
    if (D_003BABF0 > 0) {
        return 2;
    }
    return fldGetSceneReadyFlag() != 0;
}

struct ObjBase;

extern void func_00145B18(void);

extern void func_00121B88(s32, s32, f32, f32, f32);

extern void evtStartSceneResourceTask(u64, void *);

/* Gate next-floor input on camp/readiness flags, then start the native scene
 * transition or its delayed fade path. All return paths retain zero. */
s32 fldUpdateNextFloorTransition(void) {
    s32 transitionInput = 0;
    FldAreaWork *scene;

    if (fldGetCampSceneControlMode() != 0) {
        return 0;
    }
    if (D_0032E570[0x54 / 4] & fldTestSceneControlFlags(0x40)) {
        if ((s8)D_00324530[2] != 0) {
            transitionInput = 1;
        }
    } else if ((s8)D_00324530[2] < 0) {
        transitionInput = 1;
    }
    scene = &fldAreaState;
    if (fldFindLocationCoordinateRecord(scene->area, scene->floor + 1)[2] <= 0) {
        if (scene->sceneMode == 0) {
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                evtStartSceneResourceTask((u64)dds3GetWorldObject(), D_0039FCC8);
            }
        }
    } else {
        if (D_003BABF0 > 0) {
            D_003BABF0--;
            if (D_003BABF0 == 0) {
                dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 0);
                kwlnFadeStartIn(4);
                func_00145B18();
            }
            return 0;
        }
        if (fldGetSceneReadyOrPendingState() != 0) {
            return 0;
        }
        func_00121B88(scene->floor, scene->unkC0, scene->x, scene->z, 50.0f);
        if (scene->sceneMode == 0) {
            if (fldHasPendingSceneFlags() != 0) {
                return 0;
            }
            if (fldTestSceneControlFlags(0x40) != 0 && transitionInput != 0 && D_0032E570[0x48 / 4] == 0) {
                scene->sceneMode = 4;
                scene->sceneState = 5;
                fldResetPlayerSceneObjectState();
                func_00131688();
                kwlnFadeInStart(0, 0, 0, 4);
                D_003BABF0 = 5;
                fldSetCameraNodeModeWithTen();
            }
        }
    }
    return 0;
}

extern void func_0014C468(void);

/* Dispatch one pending field command, preferring the temporary override
 * over the scene-work buffer and its saved fallback. */
s32 fldDispatchPendingSceneResource(void) {
    FieldPlayerSceneWork *sceneWork = &D_0032F1A0;
    u32 overrideFlags;

    if (fldAreaState.unk100 == 1) {
        func_0014C468();
    }
    fldAreaState.unk100 = 0;
    if ((D_003BAB3C & 2) && *(s8 *)D_0032C9A0 != 0) {
        evtStartSceneResourceTask((u64)dds3GetWorldSecondaryObject(), D_0032C9A0);
        overrideFlags = D_003BAB3C;
        if (!(overrideFlags & 1)) {
            D_0032C9A0[0] = 0;
            D_003BAB3C = overrideFlags & 0xFB;
        }
        return 1;
    }
    if (sceneWork->primaryState == 0 && sceneWork->resourceName[0] != 0) {
        evtStartSceneResourceTask((u64)dds3GetWorldSecondaryObject(), sceneWork->resourceName);
        return 1;
    }
    if (fldAreaState.fallbackResourceName != 0) {
        evtStartSceneResourceTask((u64)dds3GetWorldSecondaryObject(), fldAreaState.fallbackResourceName);
        return 1;
    }
    return 0;
}

extern void dds3SetWorldObject(void *);

extern void evtEnableSolarPhaseAdvance(void);

extern s32 fileMenuTaskExists(void);

extern void fldBeginSelectedValueTransition(u32);

extern void fldCheckSceneReady(void);

extern void fldCreateInputPanelTask(void);

extern void fldEnsureTask(void);

extern void fldFinishDeferredExit(void);

extern void fldFlushQueuedEffectPositions(void);

extern u32 fldGetArchiveLoadPending(void);

extern void fldLoadSceneModelsAndCamera(void);

extern void fldPrepareSceneBgmArchive(void);

extern void fldRequestMiniTitleDismiss(void);

extern s32 fldRestartSceneResourceTask(void);

extern s32 fldSetEncounterMode(s32);

extern void fldStartDeferredFieldExit(void);

extern void fldStartSceneBgm(void);

extern void fldStartSceneBgmAlternate(void);

extern void fldStartTitleBgmIfSelected(void);

extern s32 fldStepSceneBgmArchive(void);

extern s32 fldUpdateCameraFollow(void);

extern s32 fldUpdateCameraFrame(void);

extern void fldUpdateObjectActivation(void);

extern void fldUpdateSwayOffset(void);

extern void func_00120EC8(void);

extern void func_001233D0(void);

extern void func_001249E0(void);

extern void func_00132BD0(void);

extern void func_00132FD0(u32, s32);

extern void func_0013B1D8(EffWorldNode *);

extern void func_001415F8(void);

extern void fldUpdateSparkMessageSequence(void);

extern s32 func_0014CAF8(void);

extern void fldUpdateSparkEncounterScene(void);

extern void func_0014DAF0(s32);

extern s32 kwlnFadeIsActive(void);

struct KwlnTask;

extern struct KwlnTask *kwlnTaskFindByPriority(u32);

extern void mnuMarkTitleStreamResetPending(void);

extern s32 mnuPollTitleStreamStateLocked(void);

extern void mnuResetTitleStreamLocked(void);

extern u32 D_003BABE8;

extern u32 D_003BA8EC;

extern u32 D_003BAD58;

extern u32 D_003BAC08[2];

extern EffBlurScatterWork *D_003BABE4;

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FC40);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FC50);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FC60);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FC70);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FC80);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FC90);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FCA0);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FCB0);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FCC8);

s32 fldProcSequence(void) {
    EffBlurScatterParams blur;
    FieldPlayerSceneWork *scene = &D_0032F1A0;
    FldSequenceController *controller = &fldSceneLifecycleFlags;
    s32 mode;
    s32 coordinateFlags;

    if (dds3AdminGetRequestedMode() != -1) return 0;
    if (fileMenuTaskExists() != 0) return 0;
    fldPollAreaResourceLoad();
    if (controller->stage == 0) {
        if (fldGetResourceReadyFlag() == 1) return 0;
        if (fldAreaState.transitionMode == 0 && mnuPollTitleStreamStateLocked() != 0) {
            mnuMarkTitleStreamResetPending();
            mnuResetTitleStreamLocked();
            return 0;
        }
    }
    mode = fldGetCampSceneControlMode();
    if (mode < 3) {
        if (mode > 0) {
            func_00124F58();
            return 0;
        }
    }
    if (fldTestSceneControlFlags(0x40) == 0) func_00131688();
    switch (controller->stage) {
    case 0:
        D_003BABE8 = 0;
        fldAreaState.unk130 = 1;
        func_00120EC8();
        D_003BA8EC = 0x80000000;
        evtEnableSolarPhaseAdvance();
        D_003BAD58 = 0;
        func_001233D0();
        dds3SetWorldObject(dds3GetWorldSecondaryObject());
        fldCreateInputPanelTask();
        if (fldAreaState.transitionMode == 0) evtSetSolarOverlayFullyVisible();
        fldEnsureTask();
        if (fldAreaState.area >= 200) {
            D_0032E570[11] = 0;
            D_0032E570[12] = 0;
            func_00132FD0(0, 0);
            fldBeginSelectedValueTransition(D_0032E570[12]);
        }
        coordinateFlags = fldGetLocationCoordinateValue(fldAreaState.area, fldAreaState.floor + 1);
        if (coordinateFlags & 0x40) {
            blur.count = 15;
            blur.delaySpread = 15;
            blur.angleStep = 0.1999999881f;
            blur.color = 0x3C989884;
            blur.blendControl = 0x44;
            blur.uvDisplacementAngleDegrees = 0.005f;
            blur.uvDisplacementAmplitude = 0.13f;
            blur.x = 0;
            blur.y = 0;
            blur.positionSpread = 0x100;
            blur.size = 200;
            D_003BABE4 = effBlurCreateScatterWork(&blur);
        } else if (coordinateFlags & 0x80) {
            blur.count = 60;
            blur.delaySpread = 15;
            blur.angleStep = 0.221999988f;
            blur.color = 0x28848484;
            blur.blendControl = 0x44;
            blur.uvDisplacementAngleDegrees = 0.005f;
            blur.uvDisplacementAmplitude = 0.13f;
            blur.x = 0;
            blur.y = 0;
            blur.positionSpread = 0x100;
            blur.size = 136;
            D_003BABE4 = effBlurCreateScatterWork(&blur);
        }
        kwlnFadeStartOut(0);
        fldPrepareSceneBgmArchive();
        fldLoadSceneModelsAndCamera();
        fldClearSceneControlFlags(0x40);
        fldDispatchPendingSceneResource();
        fldUpdateCameraFollow();
        controller->stage++;
        break;
    case 1:
        if (fldStepSceneBgmArchive() != 0) {
            fldAreaState.unk130 = 0;
            if (fldAreaState.transitionMode == 0) {
                if (fldGetSceneStatusCode() == 1 || fldGetSceneStatusCode() == 4 || fldGetSceneStatusCode() == 6) {
                    fldStartSceneBgmAlternate();
                } else if (scene->sequenceMode == 0) {
                    fldStartSceneBgm();
                } else {
                    fldStartTitleBgmIfSelected();
                }
            }
            controller->stage++;
        }
        break;
    case 2:
        controller->stage++;
        break;
    case 3:
        if (fldGetArchiveLoadPending() != 0) return 0;
        controller->stage++;
        if (fldAreaState.unk17C == 0 && fldAreaState.skipFade == 0 &&
            fldAreaState.titleFade == 0 && kwlnFadeIsActive() == 0) {
            kwlnFadeStartIn(8);
        }
        if (kwlnTaskFindByPriority(0x3EA) == NULL && func_0014CAF8() == 0) {
            fldPreparePlayerSceneCameraTarget();
        }
        dds3ClearObjectFlags((void *)fldPlayerObject, 0x40);
        fldSetSceneControlFlags(0x10);
        fldSetSceneControlFlags(0x20);
        fldRequestMiniTitleDismiss();
        if (fldAreaState.deferredExit == 1) fldStartDeferredFieldExit();
        break;
    case 4:
        D_003BAD58 = 1;
        if (fldTestSceneControlFlags(0x40) != 0 && fldRestartSceneResourceTask() != 0) return 0;
        if (fldTestSceneControlFlags(0x40) != 0 && fldAreaState.titleFade != 0) {
            func_0013B1D8((EffWorldNode *)fldPlayerObject);
            break;
        }
        fldFinishDeferredExit();
        if (fldAreaState.transitionMode != 0) {
            fldUpdateSparkEncounterScene();
            func_00121B88(fldAreaState.floor, fldAreaState.unkC0,
                         fldAreaState.x, fldAreaState.z, 50.0f);
        } else if (D_0032E570[0] == 0) {
            func_00124F58();
            if (fldTestSceneControlFlags(0x20) == 0) break;
            if (D_0032E570[0] == 0) fldUpdateNextFloorTransition();
        }
        if (func_0014CAF8() != 0) fldUpdateSparkMessageSequence();
        if (fldAreaState.pendingSceneRequest == 1) fldAdvanceToNextScene();
        if (fldTestSceneControlFlags(0x40) != 0) {
            func_0014DAF0(1);
            func_001249E0();
        }
        if (fldTestSceneLifecycleFlags(2) == 1) {
            controller->stage++;
            break;
        }
        fldUpdateCameraFrame();
        if (fldTestSceneControlFlags(0x40) != 0) func_0013B1D8((EffWorldNode *)fldPlayerObject);
        break;
    case 5:
        dds3SetWorldObjectDrawEnabled(dds3GetWorldObject(), 0);
        fldSetEncounterMode(fldAreaState.encounterMode);
        func_0014DAF0(0);
        fldStopCurrentBgm();
        sndSetSequenceVolumePan(15, 127, 63);
        /* Both exit phases release the same scene resources. */
    case 7:
        fldFreeDisplayObjects();
        fldSetPendingAreaAndFloor(fldAreaState.area, fldAreaState.floor + 1);
        controller->stage = 0;
        fldAreaState.transitionCount++;
        if (fldAreaState.pendingSceneRequest == 0) D_003BABE8 = 1;
        else fldAreaState.pendingSceneRequest = 0;
        dds3AdminSubmitModeRequest(14, D_003BAC08, 8, 1);
        return -1;
    case 6:
    default:
        break;
    }
    switch (fldAreaState.overlayMode) {
    case 2:
        if (fldAreaState.overlayCounter > 0) fldAreaState.overlayCounter--;
        break;
    case 6:
        if (fldAreaState.overlayCounter > 0) fldAreaState.overlayCounter--;
        break;
    case 3:
    case 7:
        if (fldAreaState.overlayCounter < 15) fldAreaState.overlayCounter++;
        break;
    case 0:
    case 1:
    case 4:
    case 5:
    default:
        break;
    }
    if (fldGetCampSceneControlMode() != 1) {
        fldUpdateSceneCommand();
        func_00132BD0();
        fldUpdateSwayOffset();
    }
    if (controller->stage >= 4) {
        func_001415F8();
        if (D_0032E570[0] == 0) {
            fldCheckSceneReady();
            fldGetSceneReadyFlag();
        }
        if (fldGetSceneReadyFlag() == 0 && fldGetCampSceneControlMode() == 0 &&
            fldHasKiretaLabelProcess() == 0 && fldHasHirakenaiLabelProcess() == 0 &&
            fldHasBadkaifukuLabelProcess() == 0) {
            fldFlushQueuedEffectPositions();
            fldUpdateObjectActivation();
        }
    }
    if (fldGetCampSceneControlMode() != 1) fldUpdateCameraFollow();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00122030", fldProcDraw);

/* Lifecycle flags and the separate scene-control flags have independent
 * set, clear and test operations. */
void fldSetSceneLifecycleFlags(u32 mask) {
    u32 *flags = &fldSceneLifecycleFlags.flags;
    *flags |= mask;
}

void fldClearSceneLifecycleFlags(u32 mask) {
    u32 *flags = &fldSceneLifecycleFlags.flags;
    *flags &= ~mask;
}

u8 fldTestSceneLifecycleFlags(u32 mask) {
    return (fldSceneLifecycleFlags.flags & mask) != 0;
}

void fldSetSceneControlFlags(u32 mask) {
    fldSceneControlFlags = fldSceneControlFlags | mask;
}

void fldClearSceneControlFlags(u32 mask) {
    fldSceneControlFlags = fldSceneControlFlags & ~mask;
}

u8 fldTestSceneControlFlags(u32 mask) {
    return (fldSceneControlFlags & mask) != 0;
}

extern s32 D_003BABF4;
extern char D_0039FD30[], D_0039FD40[];
extern s32 fldProcDraw(void);

/* Start the field scene tasks; a zero mode copies the caller's sequence record into the scene work. */
void func_00125E08(FieldSequenceRecord *record, u32 mode) {
    FieldSequenceRecord *scene = (FieldSequenceRecord *)&D_0032F1A0;
    FldSequenceController *controller = &fldSceneLifecycleFlags;

    controller->stage = 0;
    controller->flags = 0;
    scene->unk_34 = mode;
    if (mode == 0) {
        scene->unk_3c = record->unk_3c;
        scene->unk_38 = record->unk_38;
        strcpy(scene->name, record->name);
        scene->stage = record->stage;
        scene->code = record->code;
        scene->kind = record->kind;
        scene->unk_62 = record->unk_62;
        scene->enabled = record->enabled;
        scene->mode = record->mode;
        scene->link = record->link;
        strcpy(scene->detail, record->detail);
        strcpy(scene->note, record->note);
        scene->options = record->options;
    }
    kwlnTaskCreate((s32)D_0039FD30, 0x3F7, 0, 0, (s32)fldProcSequence, 0, 0);
    kwlnTaskCreate((s32)D_0039FD40, 0x2B0A, 0, 0, (s32)fldProcDraw, 0, 0);
    D_003BABF4 = 1;
    fldTestDrawCreate();
}

extern s32 D_003BABF4;

extern EffBlurScatterWork *D_003BABE4;

extern char D_0039FD30[], D_0039FD40[];

extern void kwlnFadeResetBackground(void);

extern void fldDestroyPanelTaskIfPresent(void), evtSetSolarOverlayFullyTransparent(void), fldDestroyTask(void);

extern void mnuDestroyCampTasks(void), scrDestroyAllNamedProcesses(void);

extern void fldReleaseMenuSlotsAfterWait(void);

void fldReleaseCampSceneTasks(void) {
    if (D_003BABF4 == 0) return;
    D_003BABF4 = 0;
    kwlnFadeResetBackground();
    fldClearSceneControlFlags(0x10);
    fldClearSceneControlFlags(0x20);
    fldSetSceneLifecycleFlags(1);
    fldSetSceneLifecycleFlags(2);
    kwlnTaskDestroyWithHierarchyByName(D_0039FD30, 1);
    kwlnTaskDestroyWithHierarchyByName(D_0039FD40, 1);
    fldDestroyPanelTaskIfPresent();
    evtSetSolarOverlayFullyTransparent();
    fldDestroyTask();
    if (D_003BABE4 != 0) {
        effBlurReleaseFirstResource(D_003BABE4);
        D_003BABE4 = 0;
    }
    mnuDestroyCampTasks();
    scrDestroyAllNamedProcesses();
    fldReleaseMenuSlotsAfterWait();
}

u8 fldIsFieldResourceWaitFinished(void) {
    if (D_0032E3D0[0] == 0 && fldIsAreaResourceReady() != 0) {
        fldPollAreaResourceLoad();
        if (fldGetResourceReadyFlag() == 1) return 0;
        fldFreeDisplayObjects();
        return 0;
    }
    return sdfCheckPendingWorkWithInterrupts() == 0;
}

void func_00126038(void) {
    fldSelectActorFromSceneIndexTables();
}

void fldStopSceneBgm(void) {
    fldStopCurrentBgm();
}

void fldProcessDeferredSceneCommand(void) {
    if (fldDeferredCommand != 0) {
        fldDispatchDeferredFieldCommand();
        return;
    }
    func_0013EC68();
}

void fldSetDeferredFieldCommand(u32 command, u32 parameter) {
    fldDeferredCommand = command;
    fldDeferredCommandParameter = parameter;
}

void fldDispatchDeferredFieldCommand(void) {
    if (fldDeferredCommand == 0) return;
    if (dds3AdminGetRequestedMode() > 0) return;
    if (dds3AdminGetRequestedMode() < 0 && (dds3AdminReadPreviousUnsignedSample() & 1) != 0) return;
    func_0013F100(fldDeferredCommand, fldDeferredCommandParameter);
    fldDeferredCommand = 0;
}

void fldSetPendingSceneAction(u32 argument) {
    D_003BABE0 = argument;
}

void fldRunPendingSceneAction(void) {
    u32 argument;

    argument = D_003BABE0;
    if (argument != 0) {
        func_0013E5A8(argument);
        D_003BABE0 = 0;
    }
}

/* Consume the pending pair of field-script values. -1 in both slots means
 * no request; the first value is returned with its 200-entry base offset. */
s32 fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter) {
    if (fldAreaState.nextArea == -1 && fldAreaState.nextFloor == -1) {
        return 0;
    }
    *outCode = fldAreaState.nextArea + 0xC8;
    *outParameter = fldAreaState.nextFloor;
    fldAreaState.nextArea = -1;
    fldAreaState.nextFloor = -1;
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FD30);

INCLUDE_RODATA(const s32, "game/code_00122030", D_0039FD40);

