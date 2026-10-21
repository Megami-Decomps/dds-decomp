#include "sdk_ps2.h"
#include "common.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "evt_unit.h"
#include "evt_event_pack.h"
#include "file.h"
#include "evt_motion_se.h"
#include "evt_task.h"
#include "eff_transform.h"
#include "dds3obj.h"
#include "mdl.h"
#include "file_request_api.h"
#include "sdf_texture_offset_list.h"

#include "dds3_world_primary.h"
extern Motion *mdlFindNodeById(MdlCtx *ctx, s32 searchId);
extern s32 evtSetBgmVolumePan(s32 id, s32 fade);

/* The selected script entry and the terminal value of the native load state. */
enum {
    EVT_PACK_ENTRY_POINT_KIND = 0,
    EVT_PACK_LOAD_COMPLETE = 2
};

extern s32 evtCreateWorldObjectFromResource(s32 area, s32 room, s32 resource0Data, s32 resource1Data,
                                            const SdfTextureOffsetListHeader *textureOffsets, s32 flags);
extern void fldSetRelocateOnRelease(u32 relocateOnRelease);
extern u32 kwlnDrawControlFlags;

extern s32 evtNextCameraSlotViewerKey;
#include "sdf_debug_api.h"
extern void mdlLoadViewerPackage(s32, s32, s32, s32, s32);
extern s32 mdlSpawnCameraSlotViewerObject(s32 slotKind, s32 resource);

/* Keep the complete diagnostic records, including their native trailing zeros. */
const char D_00424830[0x20] __attribute__((aligned(8))) = "unit decode rid:%d, size:%d\n";
const char D_00424850[0x20] __attribute__((aligned(8))) = "BE regist unit %d < %d >\n";

/* Decode one retained unit payload and return its new viewer-object key. */
s32 evtCreateModelFromPackResource(s32 eventId, s32 resourceId) {
    EvtPackLoadState *packState;
    EvtPackEntry *entry;
    s32 packEntryIndex;
    s32 entrySize;
    u8 *modelPayload;
    s32 viewerObjectKey;

    packState = evtGetEventPackLoadState(eventId);
    packEntryIndex = 0;
    if (packState->header->entryCount > 0) {
        entry = packState->entries;
        do {
            if (entry->secondaryResourceId == resourceId) {
                switch (entry->kind) {
                case 5:
                    entrySize = entry->dataSize;
                    modelPayload = packState->data + entry->dataOffset;
                    sdfDebugPrintf(D_00424830, resourceId, entrySize);
                    switch (eventId) {
                    case 0x2A4:
                    case 0x2AC:
                    case 0x2AE:
                    case 0x2B0:
                        mdlLoadViewerPackage(3, evtNextCameraSlotViewerKey, 0x101, (s32)modelPayload, entrySize);
                        break;
                    default:
                        mdlLoadViewerPackage(3, evtNextCameraSlotViewerKey, 0x103, (s32)modelPayload, entrySize);
                        break;
                    }
                    viewerObjectKey = mdlSpawnCameraSlotViewerObject(3, evtNextCameraSlotViewerKey);
                    evtNextCameraSlotViewerKey++;
                    if (evtNextCameraSlotViewerKey >= 0x7D0) {
                        evtNextCameraSlotViewerKey = 0x3E8;
                    }
                    sdfDebugPrintf(D_00424850, evtNextCameraSlotViewerKey, entrySize);
                    return viewerObjectKey;
                }
            }
            entry++;
        } while (++packEntryIndex < packState->header->entryCount);
    }
    return -1;
}

/* Start a field BE from the task's resource table when all four payloads exist. */
s32 evtTryCreateWorldObjectFromPackResourceSet(s32 eventId, s32 resourceId) {
    EvtPackLoadState *packState;
    EvtPackEntry *entry;
    s32 entryCount;
    s32 remainingEntryCount;
    s32 *areaRoomIds;
    s32 *resource0Data;
    s32 *resource1Data;
    const SdfTextureOffsetListHeader *textureOffsets;

    packState = evtGetEventPackLoadState(eventId);
    areaRoomIds = NULL;
    resource1Data = NULL;
    resource0Data = NULL;
    textureOffsets = NULL;
    entryCount = packState->header->entryCount;
    if (entryCount > 0) {
        entry = packState->entries;
        remainingEntryCount = entryCount;
        do {
            if (entry->resourceId == resourceId) {
                switch (entry->kind) {
                case 2:
                    resource0Data = (s32 *)(packState->data + entry->dataOffset);
                    break;
                case 3:
                    resource1Data = (s32 *)(packState->data + entry->dataOffset);
                    break;
                case 4:
                    textureOffsets = (const SdfTextureOffsetListHeader *)(packState->data + entry->dataOffset);
                    break;
                }
            }
            if (entry->secondaryResourceId == resourceId && entry->kind == 1) {
                areaRoomIds = (s32 *)(packState->data + entry->dataOffset);
            }
            entry++;
        } while (--remainingEntryCount != 0);
    }

    if (areaRoomIds != NULL && resource0Data != NULL && resource1Data != NULL && textureOffsets != NULL) {
        evtCreateWorldObjectFromResource(areaRoomIds[0], areaRoomIds[1], (s32)resource0Data, (s32)resource1Data, textureOffsets, 0);
        fldSetRelocateOnRelease(1);
        kwlnDrawControlFlags |= 0x02000000;
        return 1;
    }
    return 0;
}

s32 evtUpdateMotionSeTask(KwlnTask *task) {
    EvtMotionSeTaskParams *motionSeParams;
    EffWorldNode *motionNode;
    EvtPackLoadState *packState;
    EvtUnit *unit;
    u16 motionId;
    s32 motionFrame;
    s32 entryIndex;
    s32 cueIndex;
    EvtMotionSeCue *cues;

    motionSeParams = (EvtMotionSeTaskParams *)dds3GetProcessData(task);
    if (dds3GetWorldObject() == NULL) {
        return -1;
    }
    motionNode = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), motionSeParams->modelKey, EFF_WORLD_KIND_FOLLOW_MODEL);
    if (motionNode == NULL) {
        return -1;
    }
    packState = evtGetEventPackLoadState(motionSeParams->eventTaskId);
    if (packState == NULL) {
        return -1;
    }
    if (motionSeParams->eventTaskId < 600) {
        return 0;
    }
    unit = ((EvtMotionSeUnitLink *)motionNode->data)->unit;
    if (unit == NULL) {
        return -1;
    }
    motionId = mdlFindNodeById(unit->owner, 0)->motionIndex;
    motionFrame = unit->owner->first->currentFrame;
    for (entryIndex = 0; entryIndex < packState->header->entryCount; entryIndex++) {
        if (packState->entries[entryIndex].kind == 6 &&
            packState->entries[entryIndex].resourceId == motionSeParams->resourceId &&
            packState->entries[entryIndex].motionId == motionId) {
            cues = (EvtMotionSeCue *)(packState->data + packState->entries[entryIndex].dataOffset);
            for (cueIndex = 0; cueIndex < packState->entries[entryIndex].cueCount; cueIndex++) {
                if (cues[cueIndex].frame == motionFrame) {
                    evtSetBgmVolumePan(motionSeParams->eventTaskId, cues[cueIndex].fade);
                    sdfDebugPrintf("EVE_SE motno = %d, frame = %d\n", motionId, motionFrame);
                }
            }
        }
    }
    return 0;
}

/* Free the three-word parameter block owned by the motion-SE task. */
void evtFreeMotionSeTaskParams(KwlnTask *task) {
    void *params = (void *)(u32)dds3GetProcessData(task);

    sdfChipFreeDirect(params);
}

/* Preserve the complete native format record, including its trailing zeros. */
const char evtMotionSeTaskNameFormat[0x10] __attribute__((aligned(8))) = "mse_%d_%d";


/* Allocate a three-word task parameter block and format its "mse_..." name. */
KwlnTask *evtCreateMotionSeTask(s32 modelKey, s32 eventTaskId, s32 resourceId) {
    char taskName[0x20];
    EvtMotionSeTaskParams *params;

    sprintf(taskName, evtMotionSeTaskNameFormat, eventTaskId, resourceId);
    params = sdfChipAllocDirect(0xC);
    memset(params, 0, 0xC);
    params->modelKey = modelKey;
    params->eventTaskId = eventTaskId;
    params->resourceId = resourceId;
    return dds3StartProcess(taskName, 0x3EC, 0, 0, evtUpdateMotionSeTask, evtFreeMotionSeTaskParams, (u32)params);
}

const char evtPackScriptPathFormat[0x20] __attribute__((aligned(8))) = "/event/e%03d/e%03d/scr/e%03d.be";
extern char D_004377E0[];
extern char evtPackScriptPathBuffer[];

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtBeginEventPackScriptLoad(EvtPackLoadState *state) {
    s32 eventId;
    s32 eventDirectoryId;
    struct FileRequest *request;

    sdfDebugPrintf(D_004377E0);
    eventId = state->eventId;
    eventDirectoryId = eventId - eventId % 10;
    sprintf(evtPackScriptPathBuffer, evtPackScriptPathFormat, eventDirectoryId, eventId, eventId);
    request = fileQueueDefaultCallbackRequest(evtPackScriptPathBuffer);
    state->pendingRequest = request;
    state->loaded = 1;
}

struct FileRequest;
struct FileWork;


extern char D_004377E8[];

/* Retain a ready pack and select its first entry-point record.
 * Clearing the request handle and marking completion occur on separate calls;
 * no entry-point record leaves the existing entryPoint value untouched. */
void evtCompleteEventPackScriptLoad(EvtPackLoadState *state) {
    EvtPackHeader *header;
    s32 entryPointIndex;

    if (state->pendingRequest != NULL) {
        if (fileIsRequestReadyInCurrentMode(state->pendingRequest) != 0) {
            state->resourceAllocation = fileGetResourceHandle((struct FileRequest *)state->pendingRequest);
            filePollEntryCleanup(state->pendingRequest);
            state->pendingRequest = NULL;
            header = (EvtPackHeader *)sdfMemLock(state->resourceAllocation);
            state->data = (u8 *)header;
            state->header = header;
            state->entries = header->entries;
            for (entryPointIndex = 0; entryPointIndex < state->header->entryCount; entryPointIndex++) {
                if (state->entries[entryPointIndex].kind == EVT_PACK_ENTRY_POINT_KIND) {
                    state->entryPoint = state->data + state->entries[entryPointIndex].dataOffset;
                    break;
                }
            }
        }
    } else {
        sdfDebugPrintf(D_004377E8);
        state->loaded = EVT_PACK_LOAD_COMPLETE;
    }
}

s32 evtTickPackLoad(KwlnTask *task) {
    EvtPackLoadState *packState = (EvtPackLoadState *)dds3GetProcessData(task);

    switch (packState->loaded) {
    default:
        if (packState->loaded < 2) {
            if (packState->loaded == 0) {
                evtBeginEventPackScriptLoad(packState);
            }
        }
        break;
    case 1:
        evtCompleteEventPackScriptLoad(packState);
        break;
    }
    return 0;
}

extern void fileWaitIdle(void);
extern void effInitCh72Id(void);
extern void effInitCh71Id(void);
extern void effInitCh76Id(void);
extern void effInitCh75Id(void);
/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtReleaseEventPackResources(KwlnTask *task) {
    s32 stateHandle = dds3GetProcessData(task);
    EvtPackLoadState *packState = (EvtPackLoadState *)stateHandle;

    fileWaitIdle();
    if (stateHandle != 0) {
        if (packState->effect72 != 0) {
            effInitCh72Id();
            sdfTexReleaseReferenceViaHandler(packState->effect72);
        }
        if (packState->effect71 != 0) {
            effInitCh71Id();
            sdfTexReleaseReferenceViaHandler(packState->effect71);
        }
        if (packState->effect76 != 0) {
            effInitCh76Id();
            sdfTexReleaseReferenceViaHandler(packState->effect76);
        }
        if (packState->effect75 != 0) {
            effInitCh75Id();
            sdfTexReleaseReferenceViaHandler(packState->effect75);
        }
        if (packState->pendingRequest != NULL) {
            filePollEntryCleanup(packState->pendingRequest);
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
    sdfChipFreeDirect(stateHandle);
}

INCLUDE_SDATA(const s32, "event/evtEventPack", evtNextCameraSlotViewerKey);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

