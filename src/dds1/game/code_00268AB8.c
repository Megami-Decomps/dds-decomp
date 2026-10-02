#include "common.h"

extern s32 dds3AdvanceWorldCounter(void);

extern s32 dds3CreateCameraObject(s32 counter, f32 *position, f32 *rotation);

extern void dds3SetWorldEntryCallbackTarget(void *, const char *);

extern void effObjSetInnerFloat(s32 object, f32 value);

extern s32 dds3GetWorldSecondaryObject(void);

extern void dds3SetWorldCameraObject(s32, void *);

extern u8 D_003DC1C0[];

extern u8 D_003DC1D0[];

extern void *mnuTitleCameraObject;

extern char D_003AFD48[]; /* "---------- AT3 --------\n", followed by 8 zero bytes no C function emits */

extern u32 mnuTitleSoundBufferState[];

extern u32 D_003DA1C0[];

extern void sdfQueueNonzeroResourceId(s32);

extern u32 mnuTitleStreamStatus[];

extern u32 mnuTitleStreamSemaphore;

extern u32 D_003BD8D4;

extern u32 D_003BC5BC;

extern u32 D_003BC5C8;

extern u8 D_003D9178[];

extern s32 mnuPollTitleStreamStateLocked(void);

extern u64 sdfSoundIsCommandBusy(void);

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC590[];

extern u8 D_003BC598[];

extern char *strcat(char *, char *);

extern s32 kwlnTaskGetUserValue();

extern s32 mnuTitleSoundTask;

extern char D_003771D8[];

extern void func_003014F0(char *dst, char *fmt, char *name, char *arg);

extern s32 func_003003F0(const char *fmt, ...);

extern void mnuCreateTitleEffectTask(void);

extern u8 D_003BC5A0[];

extern u64 scrReadIntParameter(u64);

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;

extern u64 func_002F5990();

typedef struct MemBlock MemBlock;

extern MemBlock *sdfAllocGeneralBlock(s32 size);

extern u32 sdfMemoryGetBlockAddress(MemBlock *block);

extern s32 sceSifInitIopHeap(void);

extern s32 sceSifAllocIopHeap(s32 size);

extern void Exit(s32 status);

extern s32 D_003BD8C0;

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268AB8);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268D40);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_002692E0);

void mnuSetTitleSequenceVolumePan(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

char *mnuBuildSoundResourcePath(char *dst, char *filename) {
    *(u64p *)dst = *(u64p *)D_003BC590;
    return strcat(dst, filename);
}

char *mnuBuildVoiceResourcePath(char *dst, char *filename) {
    *(u64p *)dst = *(u64p *)D_003BC598;
    return strcat(dst, filename);
}

extern s8 D_003BC58C;

extern char D_00377318[];

extern char D_00377338[];

extern s32 D_00370D10[];

extern s32 D_003BC5A8;

extern s16 D_003BD8B0;

extern s32 D_003BC5AC;

extern void func_002E8C30(char *, s32, char *, s32);

extern void sndEnsureMidiBankResident(s32);

extern void func_0026A248(void);

extern void mnuCreateTitleEffectTask(void);

void mnuInitializeTitleAudioAndEffects(void) {
    if (D_003BC58C != 0) {
        func_002E8E50();
        return;
    }
    D_003BC58C = 1;
    func_002E8C30(D_00377318, 4, D_00377338, 4);
    sndEnsureMidiBankResident(D_00370D10[0]);
    D_003BC5A8 = 4;
    D_003BD8B0 = -1;
    D_003BC5AC = 0;
    func_0026A248();
    mnuCreateTitleEffectTask();
}

u32 func_002694F8(void) {
    return 0x599;
}

typedef struct TitleEffectState {
    s32 soundNameIndex; /* Selects a five-byte sound-name entry in the DDS2 twin. */
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

void mnuCreateTitleEffectTask(void) {
    TitleEffectState *state = (TitleEffectState *)sdfAllocSizeClassBlock(8);
    u32 task = kwlnTaskCreate(D_003BC5A0, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    mnuTitleSoundTask = task;
    kwlnTaskSetUserValue(task, state);
    state->soundNameIndex = 0;
    state->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *state = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(command, 0x7f);
    state->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 value) {
    ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->soundNameIndex = value;
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFC80);

void mnuPlayTitleVoiceFile(char *filename) {
    char path[16];
    TitleEffectState *state = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);

    if (sdfSoundIsCommandBusy() != 0) {
        func_003003F0("now playeng start...\n");
        sdfSoundStopNamedPlayback();
    }
    func_003014F0(path, "%s%04d.ADB", D_003771D8 + 5 * state->soundNameIndex, filename);
    func_003003F0("--------------- VOICE -> %s\n", path);
    sdfSoundSendNamedCommand(path, 0x64);
    state->frameCounter = 0;
}

void mnuStopTitleVoicePlayback(void) {
    sdfSoundStopNamedPlayback();
}

void mnuQueryTitleSoundBusy(void) {
    sdfSoundIsCommandBusy();
}

void func_00269728(void) {
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

u32 func_002697B0(void) {
    func_002E8E50();
    return 1;
}

s32 mnuInitializeTitleEffects(void) {
    s32 value;
    value = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(value, 0x7f);
    return 1;
}

u32 sndOpStopNamedPlayback(void) {
    sdfSoundStopNamedPlayback();
    return 1;
}

u32 sndOpPushCommandBusyState(void) {
    u64 soundBusy;

    soundBusy = sdfSoundIsCommandBusy();
    scrSetIntegerReturnValue(soundBusy);
    return 1;
}

u32 func_00269868(void) {
    return 1;
}

s32 movCheckStartupSoundState(void) {
    if (mnuPollTitleStreamStateLocked() == 0) {
        func_0026A5F0(scrReadIntParameter(0));
        return 0;
    }
    return mnuPollTitleStreamStateLocked() != 1;
}

u32 func_002698C0(void) {
    mnuRunTitleStreamTransitionAndLogBgm();
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

void sndInitializeStreamTransferBuffers(s32 words) {
    s32 bytes = words * 4;
    s32 i;

    D_003BC5B8 = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(bytes));
    for (i = 0; i < words * 2; i++) {
        ((u16 *)D_003BC5B8)[i] = 0;
    }
    sceSifInitIopHeap();
    D_003BD8C0 = sceSifAllocIopHeap(words * 8);
    if (D_003BD8C0 <= 0) {
        Exit(0);
    }
    D_003BC5B0[0] = D_003BD8C0;
    D_003BC5B0[1] = D_003BD8C0 + bytes;
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[0], words);
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[1], words);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269A68);

void mnuInitTitleSoundRemoteRequest(u32 arg0) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(arg0);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269B80);

void sndUploadStreamToBothIopBuffers(u32 source) {
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[0], source);
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[1], source);
    func_002F5990(1, 0x8010, 0xf80, 0);
    func_002F5990(1, 0x8010, 0x1080, 0);
    func_002F5990(1, 0x80e0, 0, 2, 0, 0);
}

/* Title-stream sample buffer. */
typedef struct MixSource {
    u8 pad00[0x18];
    s16 *samples; /* 0x18 */
} MixSource;

void sndMixSampleBuffers(s16 *dst, MixSource *first, MixSource *second) {
    s16 *out = dst;
    s16 *in = second->samples;
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

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269D18);

void mnuRunTitleStreamThread(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        WaitSema(mnuTitleStreamSemaphore);
        func_00269D18();
        SignalSema(mnuTitleStreamSemaphore);
    }
}

extern u32 mnuTitleStreamSemaphore;

extern s32 mnuTitleStreamThread;

extern u8 mnuTitleStreamThreadStack[];

void mnuCreateTitleStreamThread(void) {
    mnuTitleStreamSemaphore = sdfCreateSemaphore(1, 0xff, 0);
    sdfStartTrackedThread(&mnuTitleStreamThread, mnuRunTitleStreamThread, mnuTitleStreamThreadStack,
                  0x1000, 0x45, 0);
    sdfThreadSleepSelf();
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A248);

typedef struct AtracInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} AtracInfo;

extern s32 WaitSema(u32);

extern s32 SignalSema(u32);

void mnuReadTitleStreamStatusLocked(AtracInfo *out) {
    WaitSema(mnuTitleStreamSemaphore);
    out->unk0 = mnuTitleStreamStatus[0];
    out->unk4 = mnuTitleStreamStatus[1];
    out->unk8 = mnuTitleStreamStatus[3];
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 mnuTitleStreamSemaphore;

extern u32 mnuTitleStreamStatus[];

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

void mnuStoreTaskResult(void) {
    D_003BD8D4 = func_00288B48();
    mnuTitleStreamStatus[9] = 1;
}

extern u32 D_003D9168[];

extern s32 fileIsRequestReadyInCurrentMode(u32);

extern s32 fileGetResourceHandle(u32);

extern u32 fileGetLoadedDataAddress(u32);

extern s32 fileGetResourceSize(u32);

extern void filePollEntryCleanup(u32);

extern MemBlock *sdfAllocGeneralBlockHigh(s32);

extern void func_002F7628(u32 *);

/* When the pending title-stream file is ready, copy it into a fresh block,
 * record the entry count (size / entry size) and mark the queue as loaded. */
s32 mnuCompleteTitleStreamFileLoad(u32 *queue) {
    s32 ready = fileIsRequestReadyInCurrentMode(D_003BD8D4);

    if (ready != 0) {
        s32 handle = fileGetResourceHandle(D_003BD8D4);
        u32 data = fileGetLoadedDataAddress(D_003BD8D4);
        s32 size = fileGetResourceSize(D_003BD8D4);
        MemBlock *block;

        filePollEntryCleanup(D_003BD8D4);
        block = sdfAllocGeneralBlockHigh(size);
        mnuTitleStreamStatus[5] = sdfMemoryGetBlockAddress(block);
        mnuTitleStreamStatus[8] = (u32)block;
        memcpy((void *)queue[5], (void *)data, size);
        queue[0] = size / (s32)queue[2];
        queue[1] = 0;
        sdfQueueNonzeroResourceId(handle);
        func_002F7628(D_003D9168);
        mnuTitleStreamStatus[9] = 2;
        ready = 1;
    }
    return ready;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A588);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A5F0);

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

extern void fileWaitIdle(void);

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFCF0);

void mnuRunTitleStreamTransitionAndLogBgm(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuUpdateTitleTransition() == 1) {
        fileWaitIdle();
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    if (mnuTitleStreamStatus[4] != 1) {
        mnuTitleStreamStatus[4] = 0;
        mnuTitleStreamStatus[9] = 3;
        *(u32 *)D_003D9178 = 0;
    }
    func_003003F0("----------- AT3 BGM Play ------------\n");
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuMarkTitleStreamResetPending(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus[1] = 0;
    mnuTitleStreamStatus[4] = 2;
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[4] == 1 && mnuTitleStreamStatus[9] == 3) {
        mnuTitleStreamStatus[9] = 4;
        *(u32 *)D_003D9178 = 0;
        D_003BC5C8 = 6;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_003BC5C8;

void mnuCommitTitleStreamReadyState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[4] == 1 && mnuTitleStreamStatus[9] == 3) {
        D_003BC5C8 = mnuTitleStreamStatus[9];
        mnuTitleStreamStatus[9] = 4;
        *(u32 *)D_003D9178 = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

/* Release the title stream request and reset the playback state. */
void mnuResetTitleStream(void) {
    if (mnuTitleStreamStatus[8] != 0) {
        sdfQueueNonzeroResourceId(mnuTitleStreamStatus[8]);
        mnuTitleStreamStatus[9] = 0;
        mnuTitleStreamStatus[8] = 0;
        mnuTitleStreamStatus[5] = 0;
        mnuTitleStreamStatus[6] = (u32)D_003DA1C0;
    }
}

void mnuResetTitleStreamLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_003DA1A8[];

void mnuInitializeTitleSoundBuffer(void) {
    u32 *work = mnuTitleSoundBufferState;
    u32 *decoder = D_003DA1A8;
    MemBlock *allocation;
    s32 buffer;

    WaitSema(mnuTitleStreamSemaphore);
    work[7] = (u32)decoder;
    allocation = sdfAllocGeneralBlock(0x1C200);
    buffer = sdfMemoryGetBlockAddress(allocation);
    work[8] = (u32)allocation;
    work[4] = 2;
    ((u32 *)work[7])[2] = 2;
    work[5] = buffer;
    work[6] = (u32)D_003DA1C0;
    work[2] = 0xC0;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", work);
    func_002F7628(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AA28);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026ABA8);

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

void mnuPrintTitleDebugBanner(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState[4] != 1) {
        mnuTitleSoundBufferState[4] = 0;
    }
    func_003003F0(D_003AFD48);
    SignalSema(mnuTitleStreamSemaphore);
}

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
    sdfReleaseResourceAllocation(buffer);
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

void func_0026AEB0(void) {
    func_0026BFC8();
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFD48);

void mnuCreateTitleCameraWorldEntry(void) {
    mnuTitleCameraObject = dds3CreateCameraObject(dds3AdvanceWorldCounter(), (f32 *)D_003DC1C0, (f32 *)D_003DC1D0);
    dds3SetWorldEntryCallbackTarget(mnuTitleCameraObject, "title_camera");
    effObjSetInnerFloat(mnuTitleCameraObject, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), mnuTitleCameraObject);
}
INCLUDE_SDATA(const s32, "game/code_00268AB8", mnuTitleSoundTask);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC58C);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC590);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC598);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5A0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5A8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5AC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5B0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5B8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5BC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C4);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5CC);

