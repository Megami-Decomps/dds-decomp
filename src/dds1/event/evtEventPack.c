#include "sdk_ps2.h"
#include "common.h"
#include "dds3_task_create.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "evt_unit.h"
#include "evt_sound_variation.h"
#include "evt_task.h"
#include "mdl.h"
#include "mdl_motion_controller_lookup_api.h"
#include "file_request_api.h"
#include "sdf_texture_offset_list.h"
#include "fld_file_addr_reset_mode.h"
#include "evt_event_pack_task.h"
#include "dds3_world_primary.h"
#include "snd_packed_track_load_status.h"
#include "evt_viewer.h"
#include "evt_sky_alpha_animation_status.h"
#include "eff_event.h"
#include "evt_effect_scale_object.h"
#include "sdf_texture_file.h"
#include "btl.h"
#include "sdf_debug_api.h"

/* Declared before first use (were implicit int). */

extern void dds3EffectObjectBasicStatOn(dds3Basic_t *object, u32 flags);
struct SoundMixer;
extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(struct SoundMixer *resource, const void *position, const void *scale);

/* Native viewer child node; frame rows traverse this same linked record. */

/* World-slot node borrowed by a viewer group. Its data is slot-specific;
 * model groups use EvtModelSlot, while all named nodes share the list links. */

/* The native 0x84-byte viewer entry owns its child list and borrows info.
 * EvtRuntime.frameGroup selects one of these entries, not a separate list. */

/* The command API stores words; timed-prompt drawing consumes their text pointers. */

/* The file header emits this whole word; type-8 spans use its low halfword. */

/* Native 0x2490-byte viewer runtime shared by dialogs, task polls and file writers. */

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

dds3ProcessID_t *evtSearchEventPack(EvtPackEventId eventId) {
    char taskName[32];

    evtMakeEventPackName(eventId, taskName);
    return dds3SearchProcessName((const char *)taskName);
}

/* Return its load phase, or EVT_PACK_LOAD_NOT_FOUND when the task is absent. */
EvtPackLoadStatus evtGetEventPackStat(EvtPackEventId eventId) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventId);
    if (task != 0) {
        EvtPackLoadState *state = (EvtPackLoadState *)dds3GetProcessData(task);
        return state->loadPhase;
    }
    return EVT_PACK_LOAD_NOT_FOUND;
}

EvtPackLoadState *evtGetPackProcessData(EvtPackEventId eventId) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventId);
    if (task != 0) {
        return (EvtPackLoadState *)dds3GetProcessData(task);
    }
    return NULL;
}

void *evtGetEventPackResourceAddress(EvtPackEventId eventId, EvtPackSecondaryResourceId secondaryResourceId) {
    s32 i;
    EvtPackLoadState *data;
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventId);
    if (task != 0) {
        data = (EvtPackLoadState *)dds3GetProcessData(task);
        if (data->loadPhase != EVT_PACK_LOAD_COMPLETE) {
            return 0;
        }
        for (i = 0; i < data->header->entryCount; i++) {
            if (data->entries[i].secondaryResourceId == secondaryResourceId) {
                return data->data + data->entries[i].dataOffset;
            }
        }
        return 0;
    }
    return NULL;
}
extern void Eve_DistortionBlurSetTex(sdfTexHandle_t *);

void evtRefreshTaskEffectTexture(EvtPackEventId eventId, EvtPackSecondaryResourceId key) {
    EvtPackLoadState *data = evtGetPackProcessData(eventId);
    void *address = evtGetEventPackResourceAddress(eventId, key);
    sdfTexHandle_t *texture;
    if (address != 0) {
        sdfTexHandle_t *old = data->effect72;
        if (old != 0) {
            sdfTexFreeDelayed(old);
            data->effect72 = 0;
        }
        texture = sdfTexSet((SdfTextureFileHeader *)(address));
        Eve_DistortionBlurSetTex(texture);
        data->effect72 = texture;
    }
}

extern EvtModelScaleEntry *D_003BAA10;
extern EvtModelScaleEntry *D_003BAA20;

/* Spawn from a keyed task resource with two zeroed constructor vectors.
 * A nonnegative index applies the selected model-table scale relative to the base. */
dds3Basic_t *evtSpawnResourceObject(EvtPackEventId eventId, EvtPackSecondaryResourceId key, s32 index) {
    u8 vecA[16];
    u8 vecB[16];
    void *found;
    dds3Basic_t *obj;
    EvtEffectInner *inner;

    memset(vecA, 0, 0x10);
    memset(vecB, 0, 0x10);
    found = evtGetEventPackResourceAddress(eventId, key);
    if (found != 0) {
        obj = dds3AddBEDEffectObjectBasic2(found, vecA, vecB);
        if (obj != 0) {
            dds3EffectObjectBasicStatOn(obj, 1);
            if (index >= 0) {
                inner = ((EvtEffectObject *)obj)->inner;
                effEveBEDCutSetScale(inner->scaledObject, D_003BAA20[index].modelScale / D_003BAA10->modelScale);
            }
            return obj;
        }
        return obj;
    }
    return found;
}

extern EvtPackedSoundTrackId evtPlaySe(s32 eventId, EvtSoundVariation variation);

extern s32 evtLoadArea_FromMemory(s32 area, s32 room, u8 *resource0Data, u8 *resource1Data,
                                            const SdfTextureOffsetListHeader *textureOffsets, s32 flags);
extern u32 dds3DrawFlag;

extern s32 evtMotionSeWork;
extern void mdlPackDecode(s32, s32, s32, const u8 *, s32);
extern s32 evtSetRegistUnit(s32 slotKind, s32 resource);

/* Decode one retained unit payload and return its new viewer-object key. */
EvtMotionSeModelKey evtInitMotionSeProcess(EvtPackEventId eventId, EvtMotionSeResourceId resourceId) {
    const EvtPackLoadState *packState;
    const EvtPackEntry *entry;
    s32 packEntryIndex;
    s32 entrySize;
    const u8 *modelPayload;
    EvtMotionSeModelKey viewerObjectKey;

    packState = evtGetPackProcessDataView(eventId);
    packEntryIndex = 0;
    if (packState->header->entryCount > 0) {
        entry = packState->entries;
        do {
            if (entry->secondaryResourceId == resourceId) {
                switch (entry->kind) {
                case EVT_PACK_KIND_MODEL:
                    entrySize = entry->dataSize;
                    modelPayload = packState->data + entry->dataOffset;
                    printf("unit decode rid:%d, size:%d\n", resourceId, entrySize);
                    switch (eventId) {
                    case 0x2A4:
                    case 0x2AC:
                    case 0x2AE:
                    case 0x2B0:
                        mdlPackDecode(3, evtMotionSeWork, 0x101, modelPayload, entrySize);
                        break;
                    default:
                        mdlPackDecode(3, evtMotionSeWork, 0x103, modelPayload, entrySize);
                        break;
                    }
                    viewerObjectKey = evtSetRegistUnit(3, evtMotionSeWork);
                    evtMotionSeWork++;
                    if (evtMotionSeWork >= 0x7D0) {
                        evtMotionSeWork = 0x3E8;
                    }
                    printf("BE regist unit %d < %d >\n", evtMotionSeWork, entrySize);
                    return viewerObjectKey;
                }
            }
            entry++;
        } while (++packEntryIndex < packState->header->entryCount);
    }
    return EVT_MOTION_SE_MODEL_KEY_NOT_FOUND;
}

/* Start a field BE from the task's resource table when all four payloads exist. */
s32 evtRegistBEField(EvtPackEventId eventId, EvtPackFieldResourceId resourceId) {
    const EvtPackLoadState *packState;
    const EvtPackEntry *entry;
    s32 entryCount;
    s32 remainingEntryCount;
    const EvtAreaRoomIds *areaRoomIds;
    u8 *resource0Data;
    u8 *resource1Data;
    const SdfTextureOffsetListHeader *textureOffsets;

    packState = evtGetPackProcessDataView(eventId);
    areaRoomIds = NULL;
    resource1Data = NULL;
    resource0Data = NULL;
    textureOffsets = NULL;
    entryCount = packState->header->entryCount;
    if (entryCount > 0) {
        entry = packState->entries;
        remainingEntryCount = entryCount;
        do {
            if (entry->resourceId.field == resourceId) {
                switch (entry->kind) {
                case EVT_PACK_KIND_AREA_RESOURCE_PRIMARY:
                    resource0Data = packState->data + entry->dataOffset;
                    break;
                case EVT_PACK_KIND_AREA_RESOURCE_SECONDARY:
                    resource1Data = packState->data + entry->dataOffset;
                    break;
                case EVT_PACK_KIND_TEXTURE_OFFSETS:
                    textureOffsets = (const SdfTextureOffsetListHeader *)(packState->data + entry->dataOffset);
                    break;
                }
            }
            if (entry->secondaryResourceId == resourceId && entry->kind == EVT_PACK_KIND_AREA_ROOM_IDS) {
                areaRoomIds = (const EvtAreaRoomIds *)(packState->data + entry->dataOffset);
            }
            entry++;
        } while (--remainingEntryCount != 0);
    }

    if (areaRoomIds != NULL && resource0Data != NULL && resource1Data != NULL && textureOffsets != NULL) {
        evtLoadArea_FromMemory(areaRoomIds->area, areaRoomIds->room, resource0Data, resource1Data, textureOffsets, 0);
        fldFileAddrResetMode(FLD_FILE_ADDR_RESET_MODE_RELOCATE_ON_RELEASE);
        dds3DrawFlag |= 0x02000000;
        return 1;
    }
    return 0;
}

dds3TaskUpdateResult_t evtMotionSeProcess1(dds3ProcessID_t *task) {
    EvtMotionSeTaskParams *motionSeParams;
    dds3Basic_t *motionNode;
    const EvtPackLoadState *packState;
    evtUnitHandle_t *unit;
    u16 motionId;
    s32 motionFrame;
    s32 entryIndex;
    s32 cueIndex;
    EvtSoundVariation variation;
    const EvtMotionSeCue *cues;

    motionSeParams = (EvtMotionSeTaskParams *)dds3GetProcessData(task);
    if (dds3GetActiveScene() == NULL) {
        return DDS3_TASK_UPDATE_FINISH_CALLBACK;
    }
    motionNode = dds3SearchSceneBasic(dds3GetActiveScene(), motionSeParams->modelKey, EFF_WORLD_KIND_FOLLOW_MODEL);
    if (motionNode == NULL) {
        return DDS3_TASK_UPDATE_FINISH_CALLBACK;
    }
    packState = evtGetPackProcessDataView(motionSeParams->eventId);
    if (packState == NULL) {
        return DDS3_TASK_UPDATE_FINISH_CALLBACK;
    }
    if (motionSeParams->eventId < 600) {
        return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
    }
    unit = ((EvtMotionSeUnitLink *)motionNode->workdata)->unit;
    if (unit == NULL) {
        return DDS3_TASK_UPDATE_FINISH_CALLBACK;
    }
    motionId = mdlGetMotionCtrlr(unit->owner, 0)->motionIndex;
    motionFrame = unit->owner->first->currentFrame;
    for (entryIndex = 0; entryIndex < packState->header->entryCount; entryIndex++) {
        if (packState->entries[entryIndex].kind == EVT_PACK_KIND_MOTION_CUES &&
            packState->entries[entryIndex].resourceId.motion == motionSeParams->resourceId &&
            packState->entries[entryIndex].motionId == motionId) {
            cues = (const EvtMotionSeCue *)(packState->data + packState->entries[entryIndex].dataOffset);
            for (cueIndex = 0; cueIndex < packState->entries[entryIndex].cueCount; cueIndex++) {
                if (cues[cueIndex].frame == motionFrame) {
                    variation = cues[cueIndex].fade;
                    evtPlaySe(motionSeParams->eventId, variation);
                    printf("EVE_SE motno = %d, frame = %d\n", motionId, motionFrame);
                }
            }
        }
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

/* Free the three-word parameter block owned by the motion-SE task. */
void evtFreeScriptProcessData(dds3ProcessID_t *task)
{
    EvtMotionSeTaskParams *params = (EvtMotionSeTaskParams *)(u32)dds3GetProcessData(task);

    sdfChipFreeDirect(params);
}

/* Preserve the complete native format record, including its trailing zeros. */
const char evtMotionSeTaskNameFormat[0x10] __attribute__((aligned(8))) = "mse_%d_%d";

/* Allocate a three-word task parameter block and format its "mse_..." name. */
dds3ProcessID_t *evtLoadModelSe(EvtMotionSeModelKey modelKey, EvtPackEventId eventId,
                                EvtMotionSeResourceId resourceId) {
    char taskName[0x20];
    EvtMotionSeTaskParams *params;

    sprintf(taskName, evtMotionSeTaskNameFormat, eventId, resourceId);
    params = sdfChipAllocDirect(sizeof(*params));
    memset(params, 0, sizeof(*params));
    params->modelKey = modelKey;
    params->eventId = eventId;
    params->resourceId = resourceId;
    return dds3StartProcess(taskName, 0x3EC, 0, 0, evtMotionSeProcess1, evtFreeScriptProcessData, (u32)params);
}

const char evtPackScriptPathFormat[0x20] __attribute__((aligned(8))) = "/event/e%03d/e%03d/scr/e%03d.be";
extern char D_003BC370[];
extern char evtPackScriptPathBuffer[];

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtLoadEventScript(EvtPackLoadState *state) {
    EvtPackEventId eventId;
    s32 eventDirectoryId;
    struct FileRequest *request;

    printf(D_003BC370);
    eventId = state->eventId;
    eventDirectoryId = eventId - eventId % 10;
    sprintf(evtPackScriptPathBuffer, evtPackScriptPathFormat, eventDirectoryId, eventId, eventId);
    request = fileLoad(evtPackScriptPathBuffer);
    state->pendingRequest = request;
    state->loadPhase = EVT_PACK_LOAD_REQUEST_PENDING;
}

struct FileRequest;

extern char D_003BC378[];

/* Retain a ready pack and select its first entry-point record.
 * Clearing the request handle and marking completion occur on separate calls;
 * no entry-point record leaves the existing entryPoint value untouched. */
void evtCheckEventScriptLoaded(EvtPackLoadState *state) {
    const EvtPackHeader *header;
    s32 entryPointIndex;

    if (state->pendingRequest != NULL) {
        if (pacFileCheckEnd(state->pendingRequest) != 0) {
            state->resourceAllocation = fileGetMemHandle(state->pendingRequest);
            fileHandleFree(state->pendingRequest);
            state->pendingRequest = NULL;
            header = (const EvtPackHeader *)sdfMemLock(state->resourceAllocation);
            state->data = (u8 *)header;
            state->header = header;
            state->entries = header->entries;
            for (entryPointIndex = 0; entryPointIndex < state->header->entryCount; entryPointIndex++) {
                if (state->entries[entryPointIndex].kind == EVT_PACK_KIND_ENTRY_POINT) {
                    state->entryPoint = state->data + state->entries[entryPointIndex].dataOffset;
                    break;
                }
            }
        }
    } else {
        printf(D_003BC378);
        state->loadPhase = EVT_PACK_LOAD_COMPLETE;
    }
}

dds3TaskUpdateResult_t evtTickPackLoad(dds3ProcessID_t *task) {
    EvtPackLoadState *packState = (EvtPackLoadState *)dds3GetProcessData(task);

    switch (packState->loadPhase) {
    default:
        if (packState->loadPhase < EVT_PACK_LOAD_COMPLETE) {
            if (packState->loadPhase == EVT_PACK_LOAD_NOT_STARTED) {
                evtLoadEventScript(packState);
            }
        }
        break;
    case EVT_PACK_LOAD_REQUEST_PENDING:
        evtCheckEventScriptLoaded(packState);
        break;
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

extern void fileSyncAll(void);
extern void Eve_DistortionBlurSetDefTex(void);
extern void Eve_PartialMultiBlurSetDefTex(void);
extern void Eve_RippleBlurSetDefTex(void);
extern void Eve_PartialMultiFilterSetDefTex(void);
/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtDestroyEventPackTask(dds3ProcessID_t *task) {
    EvtPackLoadState *packState = (EvtPackLoadState *)dds3GetProcessData(task);

    fileSyncAll();
    if (packState != NULL) {
        if (packState->effect72 != 0) {
            Eve_DistortionBlurSetDefTex();
            sdfTexFreeDelayed(packState->effect72);
        }
        if (packState->effect71 != 0) {
            Eve_PartialMultiBlurSetDefTex();
            sdfTexFreeDelayed(packState->effect71);
        }
        if (packState->effect76 != 0) {
            Eve_RippleBlurSetDefTex();
            sdfTexFreeDelayed(packState->effect76);
        }
        if (packState->effect75 != 0) {
            Eve_PartialMultiFilterSetDefTex();
            sdfTexFreeDelayed(packState->effect75);
        }
        if (packState->pendingRequest != NULL) {
            fileHandleFree(packState->pendingRequest);
        }
        if (packState->resourceAllocation != NULL) {
            sdfMemFreeDelayedDirect(packState->resourceAllocation);
        }
        if (packState->sceneAllocation1 != 0) {
            sdfMemFreeDirect(packState->sceneAllocation1);
        }
        if (packState->sceneAllocation2 != 0) {
            sdfMemFreeDirect(packState->sceneAllocation2);
        }
    }
    sdfChipFreeDirect(packState);
}

INCLUDE_SDATA(const s32, "event/evtEventPack", evtMotionSeWork);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC370);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_003BC378);

