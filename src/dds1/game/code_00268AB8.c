#include "common.h"
#include "kwln.h"

#define BRS_RESULT_COUNTER_PAIR_COUNT 5
#define BRS_RESULT_SETTLED_POLL_LIMIT 6
#define BRS_RESULT_SETTLED_POLL_CLAMP 7

#define SND_TRACK_DEFAULT_VOLUME 0x7f
#define SND_TRACK_DEFAULT_PAN 0x3f
#define SND_TRANSFER_WORD_BYTES 4
#define SND_TRANSFER_WORD_HALFWORDS 2
#define SND_TRANSFER_PAIR_WORD_BYTES 8
#define SND_MIX_SAMPLE_COUNT 0x800
#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_EFFECT_BYTES 8
#define MNU_TITLE_VOICE_PREFIX_BYTES 5
#define MNU_TITLE_VOICE_PATH_BYTES 16
#define MNU_TITLE_SOUND_PATH_BYTES 32

/* Each compressed-frame size has a corresponding 600-frame allocation. */
#define MNU_SOUND_FRAME_BYTES_LARGE 0x180
#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0
#define MNU_SOUND_FRAME_BYTES_SMALL 0x60
#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400
#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200
#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100

/* Shared word positions in the title stream and sound-buffer state arrays. */
#define MNU_STREAM_FRAME_COUNT_INDEX 0
#define MNU_STREAM_FRAME_BYTES_INDEX 2
#define MNU_STREAM_CONTROL_INDEX 4
#define MNU_STREAM_DATA_ADDRESS_INDEX 5
#define MNU_STREAM_ALLOCATION_INDEX 8
#define MNU_STREAM_LOAD_STATE_INDEX 9

#define MNU_STREAM_LOAD_IDLE 0
#define MNU_STREAM_LOAD_PENDING 1
#define MNU_STREAM_LOAD_COPIED 2
#define MNU_STREAM_COMMIT_READY 3
#define MNU_STREAM_COMMIT_COMPLETE 4

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

extern u32 kwlnTaskGetUserValue();

extern s32 mnuTitleSoundTask;

extern char D_003771D8[];

/* libc sprintf returns the signed vfprintf character count. */
extern s32 func_003014F0(char *dst, const char *fmt, ...);

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

extern KwlnTask *kwlnTaskGetTaskByName(const char *);
extern char D_003AFBA0[]; /* "result2_draw" */

typedef struct BrsResultCounter {
    u8 pad00[0x14];
    s8 phase;
    u8 pad15[0x1F];
    s32 remaining;
    u8 pad38[0x14];
    s8 unk4C;
    u8 pad4D[0x13];
    s8 unk60;
    u8 pad61[7];
} BrsResultCounter;

typedef struct BrsResultWork {
    u8 pad00[0xD40];
    s32 settledFrames;
    u8 padD44[0x19C];
    BrsResultCounter levelCounters[8];
    BrsResultCounter profileCounters[8];
} BrsResultWork;

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268AB8);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268D40);

/* Complete after six successful polls of five counter pairs.
 * Early returns deliberately leave the accumulated settled count unchanged. */
s32 brsPollResultCounterCompletion(void) {
    KwlnTask *resultTask = kwlnTaskGetTaskByName(D_003AFBA0);
    BrsResultWork *resultWork;
    s32 counterIndex;

    if (resultTask == NULL) {
        return 0;
    }
    resultWork = (BrsResultWork *)kwlnTaskGetUserValue(resultTask);
    for (counterIndex = 0; counterIndex < BRS_RESULT_COUNTER_PAIR_COUNT; counterIndex++) {
        if (resultWork->levelCounters[counterIndex].phase >= 2) {
            if (resultWork->levelCounters[counterIndex].remaining != 0 ||
                resultWork->levelCounters[counterIndex].unk4C != 0 ||
                resultWork->levelCounters[counterIndex].unk60 != 0) {
                return 0;
            }
        } else if (resultWork->levelCounters[counterIndex].remaining != 0) {
            return 0;
        }
        if (resultWork->profileCounters[counterIndex].phase >= 2) {
            if (resultWork->profileCounters[counterIndex].phase == 2) {
                if (resultWork->profileCounters[counterIndex].remaining != 0) {
                    return 0;
                }
            }
        }
    }
    resultWork->settledFrames++;
    resultWork->settledFrames = resultWork->settledFrames <= 0 ? 0 :
        resultWork->settledFrames >= BRS_RESULT_SETTLED_POLL_CLAMP ? BRS_RESULT_SETTLED_POLL_LIMIT : resultWork->settledFrames;
    if (resultWork->settledFrames < BRS_RESULT_SETTLED_POLL_LIMIT) {
        return 0;
    }
    return 1;
}

void mnuSetTitleSequenceVolumePan(u32 sequenceId) {
    sndSetSequenceVolumePan(sequenceId, SND_TRACK_DEFAULT_VOLUME, SND_TRACK_DEFAULT_PAN);
}

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

/* Copy the packed eight-byte prefix, then append the caller's filename. */
char *mnuBuildSoundResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC590;
    return strcat(pathBuffer, filename);
}

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC598;
    return strcat(pathBuffer, filename);
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
    TitleEffectState *effectState;

    effectState = (TitleEffectState *)kwlnTaskGetUserValue();
    effectState->frameCounter = effectState->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    mnuTitleSoundTask = 0;
}

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *effectState = (TitleEffectState *)sdfAllocSizeClassBlock(MNU_TITLE_EFFECT_BYTES);
    u32 effectTask = kwlnTaskCreate(D_003BC5A0, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    mnuTitleSoundTask = effectTask;
    kwlnTaskSetUserValue(effectTask, effectState);
    effectState->soundNameIndex = 0;
    effectState->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(command, 0x7f);
    effectState->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex) {
    ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->soundNameIndex = prefixIndex;
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFC80);

/* The native char*-typed voice argument is passed to numeric %04d formatting;
 * retain that signature rather than treating it as a filename string. */
void mnuPlayTitleVoiceFile(char *voiceArgument) {
    char voicePath[MNU_TITLE_VOICE_PATH_BYTES];
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);

    if (sdfSoundIsCommandBusy() != 0) {
        func_003003F0("now playeng start...\n");
        sdfSoundStopNamedPlayback();
    }
    func_003014F0(voicePath, "%s%04d.ADB", D_003771D8 + MNU_TITLE_VOICE_PREFIX_BYTES * effectState->soundNameIndex, voiceArgument);
    func_003003F0("--------------- VOICE -> %s\n", voicePath);
    sdfSoundSendNamedCommand(voicePath, 0x64);
    effectState->frameCounter = 0;
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
    sndSetSequenceVolumePan(sequence, SND_TRACK_DEFAULT_VOLUME, SND_TRACK_DEFAULT_PAN);
    return 1;
}

u32 func_002697B0(void) {
    func_002E8E50();
    return 1;
}

s32 mnuInitializeTitleEffects(void) {
    s32 commandName;
    commandName = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(commandName, 0x7f);
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
    mnuTitleStreamUpdateAndLogBgm();
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
    s64 loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

/* Flush EE cache, submit one word-sized transfer to IOP, and busy-wait for DMA.
 * Return the native request ID; there is no failure retry or timeout here. */
s32 sndCopyWordsToIopSynchronously(u32 sourceAddress, u32 iopAddress, u32 wordCount) {
    struct {
        u32 source;
        u32 destination;
        u32 size;
        u32 attributes;
    } transfer;
    s32 dmaRequest;
    transfer.source = sourceAddress;
    transfer.destination = iopAddress;
    transfer.size = wordCount * SND_TRANSFER_WORD_BYTES;
    transfer.attributes = 0;
    FlushCache(0);
    dmaRequest = sceSifSetDma(&transfer, 1);
    while (sceSifDmaStat(dmaRequest) >= 0) {
    }
    return dmaRequest;
}

/* Zero one EE staging buffer, then seed both halves of the IOP allocation. */
void sndInitializeStreamTransferBuffers(s32 wordCount) {
    s32 bufferBytes = wordCount * SND_TRANSFER_WORD_BYTES;
    s32 halfwordIndex;

    D_003BC5B8 = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_003BC5B8)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    D_003BD8C0 = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (D_003BD8C0 <= 0) {
        Exit(0);
    }
    D_003BC5B0[0] = D_003BD8C0;
    D_003BC5B0[1] = D_003BD8C0 + bufferBytes;
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[1], wordCount);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269A68);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269B80);

/* The argument is a word count, not a source address; both use the EE buffer. */
void sndUploadStreamToBothIopBuffers(u32 wordCount) {
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[1], wordCount);
    func_002F5990(1, 0x8010, 0xf80, 0);
    func_002F5990(1, 0x8010, 0x1080, 0);
    func_002F5990(1, 0x80e0, 0, 2, 0, 0);
}

/* Title-stream sample buffer. */
typedef struct MixSource {
    u8 pad00[0x18];
    s16 *samples; /* 0x18 */
} MixSource;

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, MixSource *firstSource, MixSource *secondSource) {
    s16 *destinationCursor = destinationSamples;
    s16 *sourceCursor = secondSource->samples;
    s32 sampleIndex;

    for (sampleIndex = 0; sampleIndex < SND_MIX_SAMPLE_COUNT; sampleIndex++) {
        *destinationCursor++ = *sourceCursor++;
    }
    sourceCursor = firstSource->samples;
    destinationCursor -= SND_MIX_SAMPLE_COUNT;
    for (sampleIndex = 0; sampleIndex < SND_MIX_SAMPLE_COUNT; sampleIndex++) {
        s32 mixedSample = *destinationCursor + *sourceCursor++;

        if (mixedSample > SND_MIX_SAMPLE_LIMIT) {
            mixedSample = SND_MIX_SAMPLE_LIMIT;
        }
        if (mixedSample < -SND_MIX_SAMPLE_LIMIT) {
            mixedSample = -SND_MIX_SAMPLE_LIMIT;
        }
        *destinationCursor++ = mixedSample;
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

/* Read state words 0, 1 and 3; the frame-byte word 2 is not in this snapshot. */
void mnuReadTitleStreamStatusLocked(AtracInfo *statusSnapshot) {
    WaitSema(mnuTitleStreamSemaphore);
    statusSnapshot->unk0 = mnuTitleStreamStatus[MNU_STREAM_FRAME_COUNT_INDEX];
    statusSnapshot->unk4 = mnuTitleStreamStatus[1];
    statusSnapshot->unk8 = mnuTitleStreamStatus[3];
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 mnuTitleStreamSemaphore;

extern u32 mnuTitleStreamStatus[];

/* Restore the same condensed three-word status mapping under the lock. */
void mnuWriteTitleStreamStatusLocked(u32 *statusValues) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus[MNU_STREAM_FRAME_COUNT_INDEX] = statusValues[0];
    mnuTitleStreamStatus[1] = statusValues[1];
    mnuTitleStreamStatus[3] = statusValues[2];
    SignalSema(mnuTitleStreamSemaphore);
}

/* Copy into the state array's installed buffer, count complete compressed
 * frames using its frame-byte divisor, then release the loaded resource. */
void mnuLoadTitleStreamFrameData(char *filePath, u32 *streamState) {
    void *fileData;
    s32 frameCount;
    u32 resourceHandle = sdfReadNamedResource(filePath, &fileData, 0);
    s32 fileBytes = sdfMemoryGetBlockSize(resourceHandle);
    memcpy((void *)streamState[MNU_STREAM_DATA_ADDRESS_INDEX], fileData, fileBytes);
    frameCount = fileBytes / (s32)streamState[MNU_STREAM_FRAME_BYTES_INDEX];
    streamState[1] = 0;
    streamState[MNU_STREAM_FRAME_COUNT_INDEX] = frameCount;
    sdfQueueNonzeroResourceId(resourceHandle);
}

void mnuStoreTaskResult(void) {
    D_003BD8D4 = fileQueueDefaultCallbackRequest();
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

/* Allocate in global status, but use the supplied state's copy destination
 * and frame counts. Keep those distinct accesses and the cleanup-before-copy
 * ordering; return 1 after a ready file is copied, otherwise its ready result. */
s32 mnuCompleteTitleStreamFileLoad(u32 *destinationState) {
    s32 ready = fileIsRequestReadyInCurrentMode(D_003BD8D4);

    if (ready != 0) {
        s32 resourceHandle = fileGetResourceHandle(D_003BD8D4);
        u32 fileDataAddress = fileGetLoadedDataAddress(D_003BD8D4);
        s32 fileBytes = fileGetResourceSize(D_003BD8D4);
        MemBlock *allocation;

        filePollEntryCleanup(D_003BD8D4);
        allocation = sdfAllocGeneralBlockHigh(fileBytes);
        mnuTitleStreamStatus[MNU_STREAM_DATA_ADDRESS_INDEX] = sdfMemoryGetBlockAddress(allocation);
        mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX] = (u32)allocation;
        memcpy((void *)destinationState[MNU_STREAM_DATA_ADDRESS_INDEX], (void *)fileDataAddress, fileBytes);
        destinationState[MNU_STREAM_FRAME_COUNT_INDEX] = fileBytes / (s32)destinationState[MNU_STREAM_FRAME_BYTES_INDEX];
        destinationState[1] = 0;
        sdfQueueNonzeroResourceId(resourceHandle);
        func_002F7628(D_003D9168);
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_LOAD_COPIED;
        ready = 1;
    }
    return ready;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A588);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A5F0);

/* Poll a pending file without acquiring the semaphore in this entry point. */
u32 mnuUpdateTitleTransition(void) {
    if (mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] == MNU_STREAM_LOAD_PENDING) {
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    return mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX];
}

/* Poll under the semaphore, but retain the native load-state read after unlock. */
s32 mnuPollTitleStreamStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] == MNU_STREAM_LOAD_PENDING) {
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    SignalSema(mnuTitleStreamSemaphore);
    return mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX];
}

extern void fileWaitIdle(void);

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFCF0);

/* Wait/copy a pending file while locked, then prepare a commit for controls
 * other than 1. DDS1 also prints its retail BGM banner before unlocking. */
void mnuTitleStreamUpdateAndLogBgm(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuUpdateTitleTransition() == MNU_STREAM_LOAD_PENDING) {
        fileWaitIdle();
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    if (mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] != 1) {
        mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] = 0;
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_COMMIT_READY;
        *(u32 *)D_003D9178 = 0;
    }
    func_003003F0("----------- AT3 BGM Play ------------\n");
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuMarkTitleStreamResetPending(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus[1] = 0;
    mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] = 2;
    SignalSema(mnuTitleStreamSemaphore);
}

/* Commit the ready load only for control 1, and select the native value 6. */
void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] == 1 && mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] == MNU_STREAM_COMMIT_READY) {
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_COMMIT_COMPLETE;
        *(u32 *)D_003D9178 = 0;
        D_003BC5C8 = 6;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_003BC5C8;

/* Save the pre-transition load-state read before marking the commit complete. */
void mnuCommitTitleStreamReadyState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] == 1 && mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] == MNU_STREAM_COMMIT_READY) {
        D_003BC5C8 = mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX];
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_COMMIT_COMPLETE;
        *(u32 *)D_003D9178 = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

/* Reset load/data slots only when an allocation is present; otherwise do nothing. */
void mnuResetTitleStream(void) {
    if (mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX] != 0) {
        sdfQueueNonzeroResourceId(mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX]);
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_LOAD_IDLE;
        mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX] = 0;
        mnuTitleStreamStatus[MNU_STREAM_DATA_ADDRESS_INDEX] = 0;
        mnuTitleStreamStatus[6] = (u32)D_003DA1C0;
    }
}

void mnuResetTitleStreamLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_003DA1A8[];

/* Install the default medium-frame buffer and stream under the shared lock. */
void mnuInitializeTitleSoundBuffer(void) {
    u32 *streamState = mnuTitleSoundBufferState;
    u32 *decoder = D_003DA1A8;
    MemBlock *allocation;
    s32 bufferAddress;

    WaitSema(mnuTitleStreamSemaphore);
    streamState[7] = (u32)decoder;
    allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    streamState[MNU_STREAM_ALLOCATION_INDEX] = (u32)allocation;
    streamState[MNU_STREAM_CONTROL_INDEX] = 2;
    ((u32 *)streamState[7])[2] = 2;
    streamState[MNU_STREAM_DATA_ADDRESS_INDEX] = bufferAddress;
    streamState[6] = (u32)D_003DA1C0;
    streamState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", streamState);
    func_002F7628(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

typedef struct MnuTitleStreamEntry {
    u8 format;
    u8 pad;
    s16 parameter;
    char filename[12];
} MnuTitleStreamEntry;

extern MnuTitleStreamEntry D_00377650[];
extern char D_003AFCF0[];

/* Each format reserves 600 compressed frames before loading its named stream. */
void func_0026AA28(s32 soundEntryIndex) {
    MemBlock *allocation = NULL;
    s32 bufferAddress;
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState[6] = (u32)D_003DA1C0;
    mnuTitleSoundBufferState[3] = D_00377650[soundEntryIndex].parameter;
    mnuTitleSoundBufferState[7] = (u32)D_003DA1A8;
    mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] = 2;
    switch (D_00377650[soundEntryIndex].format) {
    case 1:
        D_003DA1A8[2] = 1;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_003DA1A8[2] = 2;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_003DA1A8[2] = 3;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_003DA1A8[2] = 4;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState[MNU_STREAM_ALLOCATION_INDEX] = (u32)allocation;
    mnuTitleSoundBufferState[MNU_STREAM_DATA_ADDRESS_INDEX] = bufferAddress;
    func_003014F0(soundPath, D_003AFCF0, D_00377650[soundEntryIndex].filename);
    mnuLoadTitleStreamFrameData(soundPath, mnuTitleSoundBufferState);
    func_002F7628(D_003DA1A8);
    SignalSema(mnuTitleStreamSemaphore);
}

/* Install an in-memory ATRAC stream and configure the decoder for its frame
 * format while holding the shared sound-buffer semaphore. The native code
 * has no default-format guard or buffer-capacity check. */
void func_0026ABA8(void *compressedData, s32 dataBytes, s32 format) {
    MemBlock *allocation = NULL;
    s32 bufferAddress;
    s32 frameCount;

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState[6] = (u32)D_003DA1C0;
    mnuTitleSoundBufferState[7] = (u32)D_003DA1A8;
    mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] = 2;
    mnuTitleSoundBufferState[3] = -1;
    switch (format) {
    case 1:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState[MNU_STREAM_ALLOCATION_INDEX] = (u32)allocation;
    mnuTitleSoundBufferState[MNU_STREAM_DATA_ADDRESS_INDEX] = bufferAddress;
    memcpy((void *)bufferAddress, compressedData, dataBytes);
    frameCount = dataBytes / (s32)mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX];
    mnuTitleSoundBufferState[1] = 0;
    mnuTitleSoundBufferState[MNU_STREAM_FRAME_COUNT_INDEX] = frameCount;
    func_002F7628(D_003DA1A8);
    SignalSema(mnuTitleStreamSemaphore);
}

/* These return codes are not load-state codes: 0 means no allocation,
 * 2 means control 2, and 3 covers the remaining allocated states. */
s32 mnuGetSoundBufferStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState[MNU_STREAM_ALLOCATION_INDEX] == 0) {
        SignalSema(mnuTitleStreamSemaphore);
        return 0;
    }
    if (mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] == 2) {
        SignalSema(mnuTitleStreamSemaphore);
        return 2;
    }
    SignalSema(mnuTitleStreamSemaphore);
    return 3;
}

void mnuPrintTitleDebugBanner(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] != 1) {
        mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] = 0;
    }
    func_003003F0(D_003AFD48);
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuResetSoundBuffer(void) {
    mnuTitleSoundBufferState[1] = 0;
    mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] = 2;
    mnuReleaseSoundBuffer();
}

/* Release the allocation handle, not the data address, then clear its slot. */
void mnuReleaseSoundBuffer(void) {
    u32 *streamState = mnuTitleSoundBufferState;
    u32 allocationHandle = streamState[MNU_STREAM_ALLOCATION_INDEX];

    if (allocationHandle == 0) {
        return;
    }
    sdfReleaseResourceAllocation(allocationHandle);
    streamState[MNU_STREAM_ALLOCATION_INDEX] = 0;
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

