#include "common.h"

/* The selected script entry and the terminal value of the native load state. */
enum {
    EVT_PACK_ENTRY_POINT_KIND = 0,
    EVT_PACK_LOAD_COMPLETE = 2
};

extern s32 kwlnTaskGetUserValue(void);
extern void *evtGetTaskData(s32 eventId);
extern s32 evtCreateWorldObjectFromResource(s32, s32, s32, s32, s32, s32);
extern void fldSetRelocateOnRelease(u32);
extern u32 kwlnDrawControlFlags;
extern void sdfReleaseChipBlock(s32);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D230);

typedef struct EvtFieldBeEntry {
    s32 type;
    u8 pad04[8];
    s32 offset;
    s32 secondaryResourceId;
    s32 resourceId;
    u8 pad18[8];
} EvtFieldBeEntry;

typedef struct EvtFieldBeHeader {
    u8 pad00[0x10];
    s32 count;
} EvtFieldBeHeader;

typedef struct EvtFieldBeTaskData {
    u8 pad00[0x10];
    u8 *payload;
    EvtFieldBeHeader *header;
    EvtFieldBeEntry *entries;
} EvtFieldBeTaskData;


/* Start a field BE from the task's resource table when all four payloads exist. */
s32 func_0025D390(s32 eventId, s32 resourceId) {
    EvtFieldBeTaskData *data;
    EvtFieldBeEntry *entry;
    s32 count;
    s32 remaining;
    s32 *baseData;
    s32 *type2Data;
    s32 *type3Data;
    s32 *type4Data;

    data = evtGetTaskData(eventId);
    baseData = NULL;
    type3Data = NULL;
    type2Data = NULL;
    type4Data = NULL;
    count = data->header->count;
    if (count > 0) {
        entry = data->entries;
        remaining = count;
        do {
            if (entry->resourceId == resourceId) {
                switch (entry->type) {
                case 2:
                    type2Data = (s32 *)(data->payload + entry->offset);
                    break;
                case 3:
                    type3Data = (s32 *)(data->payload + entry->offset);
                    break;
                case 4:
                    type4Data = (s32 *)(data->payload + entry->offset);
                    break;
                }
            }
            if (entry->secondaryResourceId == resourceId && entry->type == 1) {
                baseData = (s32 *)(data->payload + entry->offset);
            }
            entry++;
        } while (--remaining != 0);
    }

    if (baseData != NULL && type2Data != NULL && type3Data != NULL && type4Data != NULL) {
        evtCreateWorldObjectFromResource(baseData[0], baseData[1], (s32)type2Data, (s32)type3Data, (s32)type4Data, 0);
        fldSetRelocateOnRelease(1);
        kwlnDrawControlFlags |= 0x02000000;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D4D0);

/* Free the current task's user-value block. */
void evtFreeEventPackState(void) {
    s32 stateHandle;

    stateHandle = kwlnTaskGetUserValue();
    sdfReleaseChipBlock(stateHandle);
}

void func_0025D4D0(void);
extern void func_0035C860(char *, char *, ...);
extern void *sdfAllocSizeClassBlock(s32 size);
extern void kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern char D_00424890[];

/* Allocate a three-word task parameter block and format its "mse_..." name. */
void evtCreateMotionSeTask(s32 taskArg, s32 namePart1, s32 namePart2) {
    char taskName[0x20];
    s32 *params;

    func_0035C860(taskName, D_00424890, namePart1, namePart2);
    params = sdfAllocSizeClassBlock(0xC);
    memset(params, 0, 0xC);
    params[0] = taskArg;
    params[1] = namePart1;
    params[2] = namePart2;
    kwlnTaskCreate(taskName, 0x3EC, 0, 0, func_0025D4D0, evtFreeEventPackState, params);
}

typedef struct EvtPackScriptEntry {
    s32 kind;
    u8 pad04[8];
    u32 dataOffset;
    u8 pad10[0x10];
} EvtPackScriptEntry;

typedef struct EvtPackScriptHeader {
    u8 pad00[0x10];
    s32 entryCount;
    u8 pad14[0xC];
    EvtPackScriptEntry entries[0];
} EvtPackScriptHeader;

typedef struct EvtPackLoadState {
    s32 eventId;
    s32 loaded;
    s32 fileHandle;
    s32 resourceHandle;
    u8 *data;
    EvtPackScriptHeader *header;
    EvtPackScriptEntry *entries;
    u8 *entryPoint;
    u8 pad20[4];
    s32 sceneAllocation1;
    u8 pad28[8];
    s32 sceneAllocation2;
    u8 pad34[4];
    s32 effect72;
    s32 effect71;
    s32 effect76;
    s32 effect75;
} EvtPackLoadState;

extern char D_004248A0[];
extern char D_004377E0[];
extern s32 func_0035B6E0();
extern char D_00453C50[];
extern s32 fileQueueDefaultCallbackRequest(char *path);

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtBeginEventPackScriptLoad(EvtPackLoadState *state) {
    s32 eventId;
    s32 directoryId;
    s32 fileHandle;

    func_0035B6E0(D_004377E0);
    eventId = state->eventId;
    directoryId = eventId - eventId % 10;
    func_0035C860(D_00453C50, D_004248A0, directoryId, eventId, eventId);
    fileHandle = fileQueueDefaultCallbackRequest(D_00453C50);
    state->fileHandle = fileHandle;
    state->loaded = 1;
}

struct SdfAllocation;
struct FileRequest;
struct FileWork;
struct FileCleanup;
extern s32 fileIsRequestReadyInCurrentMode(struct FileRequest *);
extern u32 fileGetResourceHandle(struct FileWork *);
extern s32 filePollEntryCleanup(struct FileCleanup *);
extern u32 sdfResourceRetainAddress(struct SdfAllocation *);
extern char D_004377E8[];

/* Retain a ready pack and select its first entry-point record.
 * Clearing the request handle and marking completion occur on separate calls;
 * no entry-point record leaves the existing entryPoint value untouched. */
void evtCompleteEventPackScriptLoad(EvtPackLoadState *state) {
    EvtPackScriptHeader *header;
    s32 entryIndex;

    if (state->fileHandle != 0) {
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)state->fileHandle) != 0) {
            state->resourceHandle = fileGetResourceHandle((struct FileWork *)state->fileHandle);
            filePollEntryCleanup((struct FileCleanup *)state->fileHandle);
            state->fileHandle = 0;
            header = (EvtPackScriptHeader *)sdfResourceRetainAddress(
                (struct SdfAllocation *)state->resourceHandle);
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

s32 evtTickPackLoad(void) {
    EvtPackLoadState *state = (EvtPackLoadState *)kwlnTaskGetUserValue();

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
extern void sdfTexReleaseReferenceViaHandler(s32);
extern void sdfQueueNonzeroResourceId(s32);
extern void sdfReleaseResourceAllocation(s32);
extern void sdfReleaseChipBlock(s32);


/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtReleaseEventPackResources(void) {
    s32 stateHandle = kwlnTaskGetUserValue();
    EvtPackLoadState *state = (EvtPackLoadState *)stateHandle;

    fileWaitIdle();
    if (stateHandle != 0) {
        if (state->effect72 != 0) {
            effInitCh72Id();
            sdfTexReleaseReferenceViaHandler(state->effect72);
        }
        if (state->effect71 != 0) {
            effInitCh71Id();
            sdfTexReleaseReferenceViaHandler(state->effect71);
        }
        if (state->effect76 != 0) {
            effInitCh76Id();
            sdfTexReleaseReferenceViaHandler(state->effect76);
        }
        if (state->effect75 != 0) {
            effInitCh75Id();
            sdfTexReleaseReferenceViaHandler(state->effect75);
        }
        if (state->fileHandle != 0) {
            filePollEntryCleanup((struct FileCleanup *)state->fileHandle);
        }
        if (state->resourceHandle != 0) {
            sdfQueueNonzeroResourceId(state->resourceHandle);
        }
        if (state->sceneAllocation1 != 0) {
            sdfReleaseResourceAllocation(state->sceneAllocation1);
        }
        if (state->sceneAllocation2 != 0) {
            sdfReleaseResourceAllocation(state->sceneAllocation2);
        }
    }
    sdfReleaseChipBlock(stateHandle);
}

INCLUDE_RODATA(const s32, "event/evtEventPack", D_00424890);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_004248A0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377D8);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

