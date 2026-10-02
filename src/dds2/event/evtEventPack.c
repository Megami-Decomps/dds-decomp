#include "common.h"

extern s32 kwlnTaskGetUserValue(void);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D230);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D390);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D4D0);

/* Free the event state shared with the motion-sound task. */
void evtFreeEventPackState(void) {
    s32 state;

    state = kwlnTaskGetUserValue();
    sdfReleaseChipBlock(state);
}

void func_0025D4D0(void);
extern void func_0035C860(char *, char *, ...);
extern void *func_00328D68(s32 size);
extern void kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern char D_00424890[];

/* Allocate a three-word task parameter block and format its "mse_..." name. */
void evtCreateMotionSeTask(s32 taskArg, s32 namePart1, s32 namePart2) {
    char taskName[0x20];
    s32 *params;

    func_0035C860(taskName, D_00424890, namePart1, namePart2);
    params = func_00328D68(0xC);
    memset(params, 0, 0xC);
    params[0] = taskArg;
    params[1] = namePart1;
    params[2] = namePart2;
    kwlnTaskCreate(taskName, 0x3EC, 0, 0, func_0025D4D0, evtFreeEventPackState, params);
}

typedef struct EvtPackLoadState {
    s32 eventId;
    s32 loaded;
    s32 fileHandle;
} EvtPackLoadState;

extern char D_004248A0[];
extern char D_004377E0[];
extern s32 func_0035B6E0();
extern char D_00453C50[];
extern s32 func_002C80C8(char *path);

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtBeginEventPackScriptLoad(EvtPackLoadState *state) {
    s32 eventId;
    s32 directoryId;
    s32 fileHandle;

    func_0035B6E0(D_004377E0);
    eventId = state->eventId;
    directoryId = eventId - eventId % 10;
    func_0035C860(D_00453C50, D_004248A0, directoryId, eventId, eventId);
    fileHandle = func_002C80C8(D_00453C50);
    state->fileHandle = fileHandle;
    state->loaded = 1;
}

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D7E0);

extern void func_0025D7E0(EvtPackLoadState *state);

s32 func_0025D8C8(void) {
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
        func_0025D7E0(state);
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
extern void filePollEntryCleanup(s32);
extern void sdfQueueNonzeroResourceId(s32);
extern void func_003297C8(s32);
extern void sdfReleaseChipBlock(s32);

/* Keep this task resource layout identical to DDS1's EvtPackResources. */
typedef struct EvtPackResources {
    u8 pad00[8];
    s32 objectHandle;        /* 0x08 */
    s32 resourceHandle;      /* 0x0C */
    u8 pad10[0x14];
    s32 sceneAllocation1;    /* 0x24 */
    u8 pad28[8];
    s32 sceneAllocation2;    /* 0x30 */
    u8 pad34[4];
    s32 effect72;            /* 0x38 */
    s32 effect71;            /* 0x3C */
    s32 effect76;            /* 0x40 */
    s32 effect75;            /* 0x44 */
} EvtPackResources;

/* Release the event task's owned handles, then free its state. */
void evtReleaseEventPackResources(void) {
    s32 state = kwlnTaskGetUserValue();
    EvtPackResources *resources = (EvtPackResources *)state;

    fileWaitIdle();
    if (state != 0) {
        if (resources->effect72 != 0) {
            effInitCh72Id();
            sdfTexReleaseReferenceViaHandler(resources->effect72);
        }
        if (resources->effect71 != 0) {
            effInitCh71Id();
            sdfTexReleaseReferenceViaHandler(resources->effect71);
        }
        if (resources->effect76 != 0) {
            effInitCh76Id();
            sdfTexReleaseReferenceViaHandler(resources->effect76);
        }
        if (resources->effect75 != 0) {
            effInitCh75Id();
            sdfTexReleaseReferenceViaHandler(resources->effect75);
        }
        if (resources->objectHandle != 0) {
            filePollEntryCleanup(resources->objectHandle);
        }
        if (resources->resourceHandle != 0) {
            sdfQueueNonzeroResourceId(resources->resourceHandle);
        }
        if (resources->sceneAllocation1 != 0) {
            func_003297C8(resources->sceneAllocation1);
        }
        if (resources->sceneAllocation2 != 0) {
            func_003297C8(resources->sceneAllocation2);
        }
    }
    sdfReleaseChipBlock(state);
}

INCLUDE_RODATA(const s32, "event/evtEventPack", D_00424890);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_004248A0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377D8);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

