#include "common.h"
#include "kwln.h"
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

extern EffWorldNode *dds3GetWorldObject(void);
extern Motion *mdlFindNodeById(MdlCtx *, s32);
extern s32 evtSetBgmVolumePan(s32, s32);

/* The selected script entry and the terminal value of the native load state. */
enum {
    EVT_PACK_ENTRY_POINT_KIND = 0,
    EVT_PACK_LOAD_COMPLETE = 2
};

extern s32 evtCreateWorldObjectFromResource(s32, s32, s32, s32,
                                            const SdfTextureOffsetListHeader *, s32);
extern void fldSetRelocateOnRelease(u32);
extern u32 kwlnDrawControlFlags;

extern s32 D_004377D8;
extern s32 func_0035B6E0(const char *, ...);
extern void mdlLoadViewerPackage(s32, s32, s32, s32, s32);
extern s32 mdlSpawnCameraSlotViewerObject(s32, s32);

/* Keep the complete diagnostic records, including their native trailing zeros. */
const char D_00424830[0x20] __attribute__((aligned(8))) = "unit decode rid:%d, size:%d\n";
const char D_00424850[0x20] __attribute__((aligned(8))) = "BE regist unit %d < %d >\n";

/* Decode one retained unit payload and return its new viewer-object key. */
s32 evtCreateModelFromPackResource(s32 eventId, s32 resourceId) {
    EvtPackLoadState *data;
    EvtPackEntry *entry;
    s32 i;
    s32 entrySize;
    u8 *payload;
    s32 unitKey;

    data = evtGetEventPackLoadState(eventId);
    i = 0;
    if (data->header->entryCount > 0) {
        entry = data->entries;
        do {
            if (entry->secondaryResourceId == resourceId) {
                switch (entry->kind) {
                case 5:
                    entrySize = entry->dataSize;
                    payload = data->data + entry->dataOffset;
                    func_0035B6E0(D_00424830, resourceId, entrySize);
                    switch (eventId) {
                    case 0x2A4:
                    case 0x2AC:
                    case 0x2AE:
                    case 0x2B0:
                        mdlLoadViewerPackage(3, D_004377D8, 0x101, (s32)payload, entrySize);
                        break;
                    default:
                        mdlLoadViewerPackage(3, D_004377D8, 0x103, (s32)payload, entrySize);
                        break;
                    }
                    unitKey = mdlSpawnCameraSlotViewerObject(3, D_004377D8);
                    D_004377D8++;
                    if (D_004377D8 >= 0x7D0) {
                        D_004377D8 = 0x3E8;
                    }
                    func_0035B6E0(D_00424850, D_004377D8, entrySize);
                    return unitKey;
                }
            }
            entry++;
        } while (++i < data->header->entryCount);
    }
    return -1;
}

/* Start a field BE from the task's resource table when all four payloads exist. */
s32 evtTryCreateWorldObjectFromPackResourceSet(s32 eventId, s32 resourceId) {
    EvtPackLoadState *data;
    EvtPackEntry *entry;
    s32 count;
    s32 remaining;
    s32 *baseData;
    s32 *type2Data;
    s32 *type3Data;
    const SdfTextureOffsetListHeader *type4Data;

    data = evtGetEventPackLoadState(eventId);
    baseData = NULL;
    type3Data = NULL;
    type2Data = NULL;
    type4Data = NULL;
    count = data->header->entryCount;
    if (count > 0) {
        entry = data->entries;
        remaining = count;
        do {
            if (entry->resourceId == resourceId) {
                switch (entry->kind) {
                case 2:
                    type2Data = (s32 *)(data->data + entry->dataOffset);
                    break;
                case 3:
                    type3Data = (s32 *)(data->data + entry->dataOffset);
                    break;
                case 4:
                    type4Data = (const SdfTextureOffsetListHeader *)(data->data + entry->dataOffset);
                    break;
                }
            }
            if (entry->secondaryResourceId == resourceId && entry->kind == 1) {
                baseData = (s32 *)(data->data + entry->dataOffset);
            }
            entry++;
        } while (--remaining != 0);
    }

    if (baseData != NULL && type2Data != NULL && type3Data != NULL && type4Data != NULL) {
        evtCreateWorldObjectFromResource(baseData[0], baseData[1], (s32)type2Data, (s32)type3Data, type4Data, 0);
        fldSetRelocateOnRelease(1);
        kwlnDrawControlFlags |= 0x02000000;
        return 1;
    }
    return 0;
}

s32 evtUpdateMotionSeTask(KwlnTask *task) {
    EvtMotionSeTaskParams *params;
    EffWorldNode *node;
    EvtPackLoadState *data;
    EvtUnit *unit;
    u16 motionId;
    s32 frame;
    s32 entryIndex;
    s32 cueIndex;
    EvtMotionSeCue *cues;

    params = (EvtMotionSeTaskParams *)kwlnTaskGetUserValue(task);
    if (dds3GetWorldObject() == NULL) {
        return -1;
    }
    node = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), params->modelKey, 5);
    if (node == NULL) {
        return -1;
    }
    data = evtGetEventPackLoadState(params->eventTaskId);
    if (data == NULL) {
        return -1;
    }
    if (params->eventTaskId < 600) {
        return 0;
    }
    unit = ((EvtMotionSeUnitLink *)node->data)->unit;
    if (unit == NULL) {
        return -1;
    }
    motionId = mdlFindNodeById(unit->owner, 0)->motionIndex;
    frame = unit->owner->first->currentFrame;
    for (entryIndex = 0; entryIndex < data->header->entryCount; entryIndex++) {
        if (data->entries[entryIndex].kind == 6 &&
            data->entries[entryIndex].resourceId == params->resourceId &&
            data->entries[entryIndex].motionId == motionId) {
            cues = (EvtMotionSeCue *)(data->data + data->entries[entryIndex].dataOffset);
            for (cueIndex = 0; cueIndex < data->entries[entryIndex].cueCount; cueIndex++) {
                if (cues[cueIndex].frame == frame) {
                    evtSetBgmVolumePan(params->eventTaskId, cues[cueIndex].fade);
                    func_0035B6E0("EVE_SE motno = %d, frame = %d\n", motionId, frame);
                }
            }
        }
    }
    return 0;
}

/* Free the three-word parameter block owned by the motion-SE task. */
void evtFreeMotionSeTaskParams(KwlnTask *task) {
    void *params = (void *)(u32)kwlnTaskGetUserValue(task);

    sdfReleaseChipBlock(params);
}

extern void func_0035C860(char *, const char *, ...);
extern void *sdfAllocSizeClassBlock(s32 size);
extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);
/* Preserve the complete native format record, including its trailing zeros. */
const char D_00424890[0x10] __attribute__((aligned(8))) = "mse_%d_%d";

/* Allocate a three-word task parameter block and format its "mse_..." name. */
KwlnTask *evtCreateMotionSeTask(s32 modelKey, s32 eventTaskId, s32 resourceId) {
    char taskName[0x20];
    EvtMotionSeTaskParams *params;

    func_0035C860(taskName, D_00424890, eventTaskId, resourceId);
    params = sdfAllocSizeClassBlock(0xC);
    memset(params, 0, 0xC);
    params->modelKey = modelKey;
    params->eventTaskId = eventTaskId;
    params->resourceId = resourceId;
    return kwlnTaskCreate(taskName, 0x3EC, 0, 0, evtUpdateMotionSeTask, evtFreeMotionSeTaskParams, (u32)params);
}

const char D_004248A0[0x20] __attribute__((aligned(8))) = "/event/e%03d/e%03d/scr/e%03d.be";
extern char D_004377E0[];
extern char D_00453C50[];

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtBeginEventPackScriptLoad(EvtPackLoadState *state) {
    s32 eventId;
    s32 directoryId;
    struct FileRequest *request;

    func_0035B6E0(D_004377E0);
    eventId = state->eventId;
    directoryId = eventId - eventId % 10;
    func_0035C860(D_00453C50, D_004248A0, directoryId, eventId, eventId);
    request = fileQueueDefaultCallbackRequest(D_00453C50);
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
    s32 entryIndex;

    if (state->pendingRequest != NULL) {
        if (fileIsRequestReadyInCurrentMode(state->pendingRequest) != 0) {
            state->resourceAllocation = (struct SdfMemBlock *)(u32)fileGetResourceHandle((struct FileRequest *)state->pendingRequest);
            filePollEntryCleanup(state->pendingRequest);
            state->pendingRequest = NULL;
            header = (EvtPackHeader *)sdfResourceRetainAddress(state->resourceAllocation);
            state->data = (u8 *)header;
            state->header = header;
            state->entries = header->entries;
            for (entryIndex = 0; entryIndex < state->header->entryCount; entryIndex++) {
                if (state->entries[entryIndex].kind == EVT_PACK_ENTRY_POINT_KIND) {
                    state->entryPoint = state->data + state->entries[entryIndex].dataOffset;
                    break;
                }
            }
        }
    } else {
        func_0035B6E0(D_004377E8);
        state->loaded = EVT_PACK_LOAD_COMPLETE;
    }
}

s32 evtTickPackLoad(KwlnTask *task) {
    EvtPackLoadState *state = (EvtPackLoadState *)kwlnTaskGetUserValue(task);

    switch (state->loaded) {
    default:
        if (state->loaded < 2) {
            if (state->loaded == 0) {
                evtBeginEventPackScriptLoad(state);
            }
        }
        break;
    case 1:
        evtCompleteEventPackScriptLoad(state);
        break;
    }
    return 0;
}

extern void fileWaitIdle(void);
extern void effInitCh72Id(void);
extern void effInitCh71Id(void);
extern void effInitCh76Id(void);
extern void effInitCh75Id(void);
extern void sdfTexReleaseReferenceViaHandler(SdfTex *);


/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtReleaseEventPackResources(KwlnTask *task) {
    s32 stateHandle = kwlnTaskGetUserValue(task);
    EvtPackLoadState *state = (EvtPackLoadState *)stateHandle;

    fileWaitIdle();
    if (stateHandle != 0) {
        if (state->effect72 != 0) {
            effInitCh72Id();
            sdfTexReleaseReferenceViaHandler(state->effect72);
        }
        if (state->effect71 != 0) {
            effInitCh71Id();
            sdfTexReleaseReferenceViaHandler((SdfTex *)state->effect71);
        }
        if (state->effect76 != 0) {
            effInitCh76Id();
            sdfTexReleaseReferenceViaHandler((SdfTex *)state->effect76);
        }
        if (state->effect75 != 0) {
            effInitCh75Id();
            sdfTexReleaseReferenceViaHandler((SdfTex *)state->effect75);
        }
        if (state->pendingRequest != NULL) {
            filePollEntryCleanup(state->pendingRequest);
        }
        if (state->resourceAllocation != NULL) {
            sdfQueueGeneralAllocationRelease(state->resourceAllocation);
        }
        if (state->sceneAllocation1 != 0) {
            sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(state->sceneAllocation1));
        }
        if (state->sceneAllocation2 != 0) {
            sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(state->sceneAllocation2));
        }
    }
    sdfReleaseChipBlock(stateHandle);
}

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377D8);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

