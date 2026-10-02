#include "common.h"

extern u32 mnuTitleStreamSemaphore;

extern u64 scrReadIntParameter(u64);

extern u64 sdfSoundIsCommandBusy(void);

extern s32 mnuPollTitleStreamStateLocked(void);

extern u32 D_00437A2C;

extern s32 mnuMovieMenuState;

extern s32 kwlnTaskGetUserValue();

extern s32 mnuTitleSoundTask;

extern u32 mnuTitleStreamStatus[];

extern u32 D_00438FEC;

extern u32 mnuTitleSoundBufferState[];

extern char D_00428680[]; /* "titleProc" */

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_00437A00[];

extern char *strcat(char *, char *);

extern u8 D_00437A08[];

typedef struct AtracInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} AtracInfo;

extern s32 WaitSema(u32);

extern s32 SignalSema(u32);

typedef struct MemBlock MemBlock;
extern MemBlock *func_003292A8(s32 size);
extern u32 sdfMemoryGetBlockAddress(MemBlock *block);
extern s32 sceSifInitIopHeap(void);
extern s32 sceSifAllocIopHeap(s32 size);
extern void Exit(s32 status);
extern s32 D_00438FD8;

extern void func_0035C860();
extern u32 D_00437A20[2];

extern u32 D_00437A28;

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A05C0);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A08D8);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A0EE8);

void mnuSetTitleSequenceVolumePan(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

void func_002A1020(void) {
}

void func_002A1028(void) {
}

void func_002A1030(void) {
}

void func_002A1038(void) {
}

char *mnuBuildSoundResourcePath(char *dst, char *filename) {
    *(u64p *)dst = *(u64p *)D_00437A00;
    return strcat(dst, filename);
}

char *mnuBuildVoiceResourcePath(char *dst, char *filename) {
    *(u64p *)dst = *(u64p *)D_00437A08;
    return strcat(dst, filename);
}

extern s8 D_004379FC;

extern char D_003E0B30[];

extern char D_003E0B50[];

extern s32 D_003D9D60[];

extern s32 D_00437A18;

extern s16 D_00438FCC;

extern s32 D_00437A1C;

extern void func_00341AD8(char *, s32, char *, s32);

extern void sndEnsureMidiBankResident(s32);

extern void func_002A1E58(void);

extern void mnuCreateTitleEffectTask(void);

void mnuInitializeTitleAudioAndEffects(void) {
    if (D_004379FC != 0) {
        func_00341CF8();
        return;
    }
    D_004379FC = 1;
    func_00341AD8(D_003E0B30, 4, D_003E0B50, 4);
    sndEnsureMidiBankResident(D_003D9D60[0]);
    D_00437A18 = 4;
    D_00438FCC = -1;
    D_00437A1C = 0;
    func_002A1E58();
    mnuCreateTitleEffectTask();
}

u32 func_002A1118(void) {
    return 0x608;
}

typedef struct TitleEffectState {
    s32 soundNameIndex; /* Index into the five-byte sound-name entries. */
    s32 frameCounter;
} TitleEffectState;

u32 mnuIncrementTitleEffectFrameCounter(void) {
    TitleEffectState *state;

    state = (TitleEffectState *)kwlnTaskGetUserValue();
    state->frameCounter = state->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    mnuTitleSoundTask = 0;
}

extern u8 D_00437A10[];

/* The task owns a two-word title-effect state; its second word counts frames. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *state = (TitleEffectState *)func_00328D68(8);
    u32 task = kwlnTaskCreate(D_00437A10, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    mnuTitleSoundTask = task;
    kwlnTaskSetUserValue(task, state);
    state->soundNameIndex = 0;
    state->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 effect) {
    TitleEffectState *state = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        func_00342690();
    }
    sdfSoundSendNamedCommand(effect, 0x7f);
    state->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 value) {
    ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->soundNameIndex = value;
}

extern char D_00428610[];

extern char D_00428628[];

extern char D_003E09F0[];

void mnuPlayTitleVoiceFile(char *filename) {
    char path[16];
    TitleEffectState *state = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        func_0035B6E0(D_00428610);
        func_00342690();
    }
    func_0035C860(path, D_00428628, D_003E09F0 + 5 * state->soundNameIndex, filename);
    sdfSoundSendNamedCommand(path, 0x64);
    state->frameCounter = 0;
}

void func_002A1308(void) {
    func_00342690();
}

void mnuQueryTitleSoundBusy(void) {
    sdfSoundIsCommandBusy();
}

void func_002A1338(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->frameCounter;
}

u32 sndOpStartTrackFromScript(void) {
    u64 sequence;

    sequence = scrReadIntParameter(0);
    sndStartTrackDefault(sequence);
    return 1;
}

u32 sndOpSetTrackDefaultVolumePan(void) {
    u64 sequence;

    sequence = scrReadIntParameter(0);
    sndSetSequenceVolumePan(sequence, 0x7f, 0x3f);
    return 1;
}

u32 func_002A13C0(void) {
    func_00341CF8();
    return 1;
}

s32 mnuInitializeTitleEffects(void) {
    s32 value;
    value = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        func_00342690();
    }
    sdfSoundSendNamedCommand(value, 0x7f);
    return 1;
}

u32 func_002A1430(void) {
    func_00342690();
    return 1;
}

u32 sndOpPushCommandBusyState(void) {
    u64 soundBusy;

    soundBusy = sdfSoundIsCommandBusy();
    scrSetIntegerReturnValue(soundBusy);
    return 1;
}

u32 func_002A1478(void) {
    return 1;
}

s32 movCheckStartupSoundState(void) {
    if (mnuPollTitleStreamStateLocked() == 0) {
        func_002A2200(scrReadIntParameter(0));
        return 0;
    }
    return mnuPollTitleStreamStateLocked() != 1;
}

u32 func_002A14D0(void) {
    mnuResetTitleStreamAfterFileIdle();
    return 1;
}

u32 mnuResetTitlePlaybackCommands(void) {
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    return 1;
}

u32 mnuAdvanceTitleStreamFromScript(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    return 1;
}

u8 mnuIsTitleStreamIdle(void) {
    s64 soundState;

    soundState = mnuPollTitleStreamStateLocked();
    return soundState == 0;
}

s32 sndCopyWordsToIopSynchronously(u32 source, u32 destination, u32 words) {
    struct {
        u32 source;
        u32 destination;
        u32 size;
        u32 attributes;
    } transfer;
    s32 request;
    transfer.source = source;
    transfer.destination = destination;
    transfer.size = words * 4;
    transfer.attributes = 0;
    FlushCache(0);
    request = sceSifSetDma(&transfer, 1);
    while (sceSifDmaStat(request) >= 0) {
    }
    return request;
}

void func_002A15B0(s32 words) {
    s32 bytes = words * 4;
    s32 i;

    D_00437A28 = sdfMemoryGetBlockAddress(func_003292A8(bytes));
    for (i = 0; i < words * 2; i++) {
        ((u16 *)D_00437A28)[i] = 0;
    }
    sceSifInitIopHeap();
    D_00438FD8 = sceSifAllocIopHeap(words * 8);
    if (D_00438FD8 <= 0) {
        Exit(0);
    }
    D_00437A20[0] = D_00438FD8;
    D_00437A20[1] = D_00438FD8 + bytes;
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[0], words);
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[1], words);
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1678);

void mnuInitTitleSoundRemoteRequest(u32 arg0) {
    sceSdRemoteInit();
    func_002A15B0(arg0);
    D_00437A2C = 0;
}

extern u64 func_0034E820();

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1790);

void sndUploadStreamToBothIopBuffers(u32 source) {
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[0], source);
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[1], source);
    func_0034E820(1, 0x8010, 0xf80, 0);
    func_0034E820(1, 0x8010, 0x1080, 0);
    func_0034E820(1, 0x80e0, 0, 2, 0, 0);
}

/* Title-stream sample buffer. */
typedef struct MixSource {
    u8 pad00[0x18];
    s16 *samples; /* 0x18 */
} MixSource;

void sndMixSampleBuffers(s16 *dst, MixSource *first, MixSource *second) {
    s16 *in = second->samples;
    s16 *out = dst;
    s32 i;
    for (i = 0; i < 0x800; i++) {
        *out++ = *in++;
    }
    in = first->samples;
    out -= 0x800;
    for (i = 0; i < 0x800; i++) {
        s32 sample = *out + *in++;
        if (sample > 0x7FFF) {
            sample = 0x7FFF;
        }
        if (sample < -0x7FFF) {
            sample = -0x7FFF;
        }
        *out++ = sample;
    }
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1928);

void mnuRunTitleStreamThread(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        WaitSema(mnuTitleStreamSemaphore);
        func_002A1928();
        SignalSema(mnuTitleStreamSemaphore);
    }
}

extern s32 mnuTitleStreamThread;

extern u8 mnuTitleStreamThreadStack[];

void mnuCreateTitleStreamThread(void) {
    mnuTitleStreamSemaphore = sdfCreateSemaphore(1, 0xff, 0);
    sdfStartTrackedThread(&mnuTitleStreamThread, mnuRunTitleStreamThread, mnuTitleStreamThreadStack,
                  0x1000, 0x45, 0);
    sdfThreadSleepSelf();
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1E58);

void mnuReadTitleStreamStatusLocked(AtracInfo *out) {
    WaitSema(mnuTitleStreamSemaphore);
    out->unk0 = mnuTitleStreamStatus[0];
    out->unk4 = mnuTitleStreamStatus[1];
    out->unk8 = mnuTitleStreamStatus[3];
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuWriteTitleStreamStatusLocked(u32 *values) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus[0] = values[0];
    mnuTitleStreamStatus[1] = values[1];
    mnuTitleStreamStatus[3] = values[2];
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuLoadTitleStreamFrameData(char *filePath, u32 *work) {
    void *fileData;
    s32 frames;
    u32 request = sdfReadNamedResource(filePath, &fileData, 0);
    s32 bytes = sdfMemoryGetBlockSize(request);
    memcpy((void *)work[5], fileData, bytes);
    frames = bytes / (s32)work[2];
    work[1] = 0;
    work[0] = frames;
    sdfQueueNonzeroResourceId(request);
}

void mnuStoreTaskResult(char *audioPath) {
    D_00438FEC = func_002C80C8(audioPath);
    mnuTitleStreamStatus[9] = 1;
}

extern u32 D_00454D58[];
extern s32 fileIsRequestReadyInCurrentMode(u32);
extern s32 fileGetResourceHandle(u32);
extern u32 fileGetLoadedDataAddress(u32);
extern s32 fileGetResourceSize(u32);
extern void filePollEntryCleanup(u32);
extern MemBlock *func_003293C8(s32);
extern void func_003504A8(u32 *);

/* When the pending title-stream file is ready, copy it into a fresh block,
 * record the entry count (size / entry size) and mark the queue as loaded. */
s32 mnuCompleteTitleStreamFileLoad(u32 *queue) {
    s32 ready = fileIsRequestReadyInCurrentMode(D_00438FEC);

    if (ready != 0) {
        s32 handle = fileGetResourceHandle(D_00438FEC);
        u32 data = fileGetLoadedDataAddress(D_00438FEC);
        s32 size = fileGetResourceSize(D_00438FEC);
        MemBlock *block;

        filePollEntryCleanup(D_00438FEC);
        block = func_003293C8(size);
        mnuTitleStreamStatus[5] = sdfMemoryGetBlockAddress(block);
        mnuTitleStreamStatus[8] = (u32)block;
        memcpy((void *)queue[5], (void *)data, size);
        queue[0] = size / (s32)queue[2];
        queue[1] = 0;
        sdfQueueNonzeroResourceId(handle);
        func_003504A8(D_00454D58);
        mnuTitleStreamStatus[9] = 2;
        ready = 1;
    }
    return ready;
}

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428600);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428610);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428628);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2198);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2200);

u32 mnuUpdateTitleTransition(void) {
    if (mnuTitleStreamStatus[9] == 1) {
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    return mnuTitleStreamStatus[9];
}

s32 mnuPollTitleStreamStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[9] == 1) {
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    SignalSema(mnuTitleStreamSemaphore);
    return mnuTitleStreamStatus[9];
}

extern u32 D_00454D68[];

void mnuResetTitleStreamAfterFileIdle(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuUpdateTitleTransition() == 1) {
        fileWaitIdle();
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    if (mnuTitleStreamStatus[4] != 1) {
        mnuTitleStreamStatus[4] = 0;
        mnuTitleStreamStatus[9] = 3;
        D_00454D68[0] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuMarkTitleStreamResetPending(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus[1] = 0;
    mnuTitleStreamStatus[4] = 2;
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_00437A38;

void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[4] == 1 && mnuTitleStreamStatus[9] == 3) {
        mnuTitleStreamStatus[9] = 4;
        D_00454D68[0] = 0;
        D_00437A38 = 6;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuCommitTitleStreamReadyState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[4] == 1 && mnuTitleStreamStatus[9] == 3) {
        D_00437A38 = mnuTitleStreamStatus[9];
        mnuTitleStreamStatus[9] = 4;
        D_00454D68[0] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

extern u8 D_00455DB0[];

extern void sdfQueueNonzeroResourceId(u32 arg0);

void mnuResetTitleStream(void) {
    if (mnuTitleStreamStatus[8] != 0) {
        sdfQueueNonzeroResourceId(mnuTitleStreamStatus[8]);
        mnuTitleStreamStatus[9] = 0;
        mnuTitleStreamStatus[8] = 0;
        mnuTitleStreamStatus[5] = 0;
        mnuTitleStreamStatus[6] = (u32)D_00455DB0;
    }
}

void mnuResetTitleStreamLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_00455D98[];

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428650);

void mnuInitializeTitleSoundBuffer(void) {
    u32 *work = mnuTitleSoundBufferState;
    u32 *decoder = D_00455D98;
    MemBlock *allocation;
    s32 buffer;

    WaitSema(mnuTitleStreamSemaphore);
    work[7] = (u32)decoder;
    allocation = func_003292A8(0x1C200);
    buffer = sdfMemoryGetBlockAddress(allocation);
    work[8] = (u32)allocation;
    work[4] = 2;
    ((u32 *)work[7])[2] = 2;
    work[5] = buffer;
    work[6] = (u32)D_00455DB0;
    work[2] = 0xC0;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", work);
    func_003504A8(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2628);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A27A8);

s32 mnuGetSoundBufferStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState[8] == 0) {
        SignalSema(mnuTitleStreamSemaphore);
        return 0;
    }
    if (mnuTitleSoundBufferState[4] == 2) {
        SignalSema(mnuTitleStreamSemaphore);
        return 2;
    }
    SignalSema(mnuTitleStreamSemaphore);
    return 3;
}

void mnuClearInactiveSoundBufferState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState[4] != 1) {
        mnuTitleSoundBufferState[4] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuReleaseSoundBuffer(void);

void mnuResetSoundBuffer(void) {
    mnuTitleSoundBufferState[1] = 0;
    mnuTitleSoundBufferState[4] = 2;
    mnuReleaseSoundBuffer();
}

void mnuReleaseSoundBuffer(void) {
    u32 *state = mnuTitleSoundBufferState;
    u32 buffer = state[8];

    if (buffer == 0) {
        return;
    }
    sdfQueueNonzeroResourceId(buffer);
    state[8] = 0;
}

void mnuResetSoundBufferLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetSoundBuffer();
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuReleaseSoundBufferLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuReleaseSoundBuffer();
    SignalSema(mnuTitleStreamSemaphore);
}

void func_002A2AA0(void) {
    func_002A3D70();
    func_002A3E38(1);
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2AC0);

extern void mnuStopTitleMovieDraw(void);

extern void func_003458E8(u32);

extern void mnuReleaseMenuResourceSlots(void);

extern void mnuDestroyMovieMenuSelectionList(void);

extern void func_003297C8(u32);

extern u32 D_00435BB0;

void mnuReleaseTitleMenuAssetsAndMarkClosed(void) {
    mnuStopTitleMovieDraw();
    func_003458E8(0);
    mnuReleaseMenuResourceSlots();
    mnuReleaseSpriteHandle();
    mnuDestroyMovieMenuSelectionList();
    func_003297C8(*(u32 *)mnuMovieMenuState);
    mnuMovieMenuState = 0;
    D_00435BB0 = 1;
}

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428680);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2C28);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A30C0);

u32 func_002A3A50(void) {
    func_002A2AC0();
    return 0xffffffff;
}

s32 mnuDestroyTitleMenuTask(void) {
    mnuReleaseTitleMenuAssetsAndMarkClosed();
    kwlnTaskDestroyWithHierarchyByName(D_00428680, 1);
    return 0;
}

u32 func_002A3AA0(void) {
    func_002A2AC0(0);
    return 0;
}

void mnuRestartRuntimeAfterViewer(void) {
    evtDestroySecondaryWorldNode();
    sdfDestroyRuntimeTask();
    sdfCreateRuntimeTask();
}
