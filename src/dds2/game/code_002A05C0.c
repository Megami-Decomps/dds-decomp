#include "common.h"
#include "kwln.h"
#include "mnu.h"

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

extern u32 mnuTitleStreamSemaphore;

extern u64 scrReadIntParameter(u64);

extern u64 sdfSoundIsCommandBusy(void);

extern s32 mnuPollTitleStreamStateLocked(void);

extern u32 D_00437A2C;

typedef struct MovieMenuState {
    struct MemBlock *allocation;
    u8 pad04[0x0C];
    s32 state;
    s32 cursor;
    s32 mode;
    u8 pad1C[0x14];
    u32 bar[2];           /* 0x30 */
    u32 barB[2];          /* 0x38 */
    u32 barSmall[3];      /* 0x40 */
    PickList paired;      /* 0x4C */
    SlideBarTimed timedA; /* 0xE0: pos below 0x200 while the menu bar is on screen */
    SlideBarTimed timedB; /* 0xF0 */
    u8 movie[0xC];        /* 0x100 */
    s32 movieDrawn;       /* 0x10C */
    s32 movieAlpha;       /* 0x110 */
    s32 movieFrame;       /* 0x114 */
} MovieMenuState;

extern MovieMenuState *mnuMovieMenuState;

extern u32 kwlnTaskGetUserValue();

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
extern MemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfMemoryGetBlockAddress(MemBlock *block);
extern s32 sceSifInitIopHeap(void);
extern s32 sceSifAllocIopHeap(s32 size);
extern void Exit(s32 status);
extern s32 D_00438FD8;

/* libc sprintf returns the signed vfprintf character count. */
extern s32 func_0035C860(char *dst, const char *fmt, ...);
extern u32 D_00437A20[2];

extern u32 D_00437A28;

extern KwlnTask *kwlnTaskGetTaskByName(const char *);
extern char D_00428550[]; /* "result2_draw" */

typedef struct BrsResultCounter {
    u8 pad00[0x14];
    s8 phase;
    u8 pad15[7];
    s32 alpha;
    u8 pad20[0x10];
    s32 shown;
    s32 remaining;
    s32 increment;
    u8 pad3C[5];
    s8 unk41;
    u8 pad42[2];
    s32 unk44;
    s32 unk48;
    s8 unk4C;
    u8 pad4D[0x13];
    s8 unk60;
    u8 pad61[7];
} BrsResultCounter;

typedef struct BrsResultWork {
    u8 pad00[0xAEAC];
    s32 settledFrames;
    u8 padAEB0[8];
    s8 soundStarted;
    u8 padAEB9[0x193];
    BrsResultCounter levelCounters[8];
    BrsResultCounter profileCounters[8];
    u8 padB6CC[0x14];
    s32 fadeProgress;
    u8 padB6E4[0x20];
} BrsResultWork;

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A05C0);
INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A08D8);

/* Complete after six successful polls of five counter pairs.
 * Early returns deliberately leave the accumulated settled count unchanged. */
s32 brsPollResultCounterCompletion(void) {
    KwlnTask *resultTask = kwlnTaskGetTaskByName(D_00428550);
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

void func_002A1020(void) {
}

void func_002A1028(void) {
}

void func_002A1030(void) {
}

void func_002A1038(void) {
}

/* Copy the packed eight-byte prefix, then append the caller's filename. */
char *mnuBuildSoundResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_00437A00;
    return strcat(pathBuffer, filename);
}

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_00437A08;
    return strcat(pathBuffer, filename);
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
    TitleEffectState *effectState;

    effectState = (TitleEffectState *)kwlnTaskGetUserValue();
    effectState->frameCounter = effectState->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    mnuTitleSoundTask = 0;
}

extern u8 D_00437A10[];

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *effectState = (TitleEffectState *)sdfAllocSizeClassBlock(MNU_TITLE_EFFECT_BYTES);
    u32 effectTask = kwlnTaskCreate(D_00437A10, 0x5214, 1, 1,
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

extern char D_00428610[];

extern char D_00428628[];

extern char D_003E09F0[];

/* The native char*-typed voice argument is passed to numeric %04d formatting;
 * retain that signature rather than treating it as a filename string. */
void mnuPlayTitleVoiceFile(char *voiceArgument) {
    char voicePath[MNU_TITLE_VOICE_PATH_BYTES];
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        func_0035B6E0(D_00428610);
        sdfSoundStopNamedPlayback();
    }
    func_0035C860(voicePath, D_00428628, D_003E09F0 + MNU_TITLE_VOICE_PREFIX_BYTES * effectState->soundNameIndex, voiceArgument);
    sdfSoundSendNamedCommand(voicePath, 0x64);
    effectState->frameCounter = 0;
}

void mnuStopTitleVoicePlayback(void) {
    sdfSoundStopNamedPlayback();
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
    sndSetSequenceVolumePan(sequence, SND_TRACK_DEFAULT_VOLUME, SND_TRACK_DEFAULT_PAN);
    return 1;
}

u32 func_002A13C0(void) {
    func_00341CF8();
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

    D_00437A28 = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_00437A28)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    D_00438FD8 = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (D_00438FD8 <= 0) {
        Exit(0);
    }
    D_00437A20[0] = D_00438FD8;
    D_00437A20[1] = D_00438FD8 + bufferBytes;
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[0], wordCount);
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[1], wordCount);
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1678);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_00437A2C = 0;
}

extern u64 func_0034E820();

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1790);

/* The argument is a word count, not a source address; both use the EE buffer. */
void sndUploadStreamToBothIopBuffers(u32 wordCount) {
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[0], wordCount);
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[1], wordCount);
    func_0034E820(1, 0x8010, 0xf80, 0);
    func_0034E820(1, 0x8010, 0x1080, 0);
    func_0034E820(1, 0x80e0, 0, 2, 0, 0);
}

/* Title-stream sample buffer. */
typedef struct MixSource {
    u8 pad00[0x18];
    s16 *samples; /* 0x18 */
} MixSource;

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, MixSource *firstSource, MixSource *secondSource) {
    s16 *sourceCursor = secondSource->samples;
    s16 *destinationCursor = destinationSamples;
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

/* Read state words 0, 1 and 3; the frame-byte word 2 is not in this snapshot. */
void mnuReadTitleStreamStatusLocked(AtracInfo *statusSnapshot) {
    WaitSema(mnuTitleStreamSemaphore);
    statusSnapshot->unk0 = mnuTitleStreamStatus[MNU_STREAM_FRAME_COUNT_INDEX];
    statusSnapshot->unk4 = mnuTitleStreamStatus[1];
    statusSnapshot->unk8 = mnuTitleStreamStatus[3];
    SignalSema(mnuTitleStreamSemaphore);
}

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

void mnuStoreTaskResult(char *audioPath) {
    D_00438FEC = fileQueueDefaultCallbackRequest(audioPath);
    mnuTitleStreamStatus[9] = 1;
}

extern u32 D_00454D58[];
extern s32 fileIsRequestReadyInCurrentMode(u32);
extern s32 fileGetResourceHandle(u32);
extern u32 fileGetLoadedDataAddress(u32);
extern s32 fileGetResourceSize(u32);
extern void filePollEntryCleanup(u32);
extern MemBlock *sdfAllocGeneralBlockHigh(s32);
extern void func_003504A8(u32 *);

/* Allocate in global status, but use the supplied state's copy destination
 * and frame counts. Keep those distinct accesses and the cleanup-before-copy
 * ordering; return 1 after a ready file is copied, otherwise its ready result. */
s32 mnuCompleteTitleStreamFileLoad(u32 *destinationState) {
    s32 ready = fileIsRequestReadyInCurrentMode(D_00438FEC);

    if (ready != 0) {
        s32 resourceHandle = fileGetResourceHandle(D_00438FEC);
        u32 fileDataAddress = fileGetLoadedDataAddress(D_00438FEC);
        s32 fileBytes = fileGetResourceSize(D_00438FEC);
        MemBlock *allocation;

        filePollEntryCleanup(D_00438FEC);
        allocation = sdfAllocGeneralBlockHigh(fileBytes);
        mnuTitleStreamStatus[MNU_STREAM_DATA_ADDRESS_INDEX] = sdfMemoryGetBlockAddress(allocation);
        mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX] = (u32)allocation;
        memcpy((void *)destinationState[MNU_STREAM_DATA_ADDRESS_INDEX], (void *)fileDataAddress, fileBytes);
        destinationState[MNU_STREAM_FRAME_COUNT_INDEX] = fileBytes / (s32)destinationState[MNU_STREAM_FRAME_BYTES_INDEX];
        destinationState[1] = 0;
        sdfQueueNonzeroResourceId(resourceHandle);
        func_003504A8(D_00454D58);
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_LOAD_COPIED;
        ready = 1;
    }
    return ready;
}

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428600);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428610);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428628);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2198);


typedef struct MnuTitleStreamEntry {
    u8 format;
    u8 pad;
    s16 parameter;
    char filename[12];
} MnuTitleStreamEntry;

extern MnuTitleStreamEntry D_003E0B60[];

extern u8 D_00455DB0[];

extern char D_00428650[];

/* Load the named sound stream for the requested entry format under the lock. */
void func_002A2200(s32 soundEntryIndex) {
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus[7] = (u32)D_00454D58;
    mnuTitleStreamStatus[6] = (u32)D_00455DB0;
    mnuTitleStreamStatus[3] = D_003E0B60[soundEntryIndex].parameter;
    mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] = 2;
    switch (D_003E0B60[soundEntryIndex].format) {
    case 1:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_LARGE;
        break;
    case 2:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 3:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 4:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_SMALL;
        break;
    }
    func_0035C860(soundPath, D_00428650, D_003E0B60[soundEntryIndex].filename);
    mnuStoreTaskResult(soundPath);
    SignalSema(mnuTitleStreamSemaphore);
}

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

extern u32 D_00454D68[];

/* Wait/copy a pending file while locked, then prepare a commit for controls
 * other than 1. Unlike DDS1, this entry point has no BGM banner output. */
void mnuResetTitleStreamAfterFileIdle(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuUpdateTitleTransition() == MNU_STREAM_LOAD_PENDING) {
        fileWaitIdle();
        mnuCompleteTitleStreamFileLoad(mnuTitleStreamStatus);
    }
    if (mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] != 1) {
        mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] = 0;
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_COMMIT_READY;
        D_00454D68[0] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuMarkTitleStreamResetPending(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus[1] = 0;
    mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] = 2;
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_00437A38;

/* Commit the ready load only for control 1, and select the native value 6. */
void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] == 1 && mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] == MNU_STREAM_COMMIT_READY) {
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_COMMIT_COMPLETE;
        D_00454D68[0] = 0;
        D_00437A38 = 6;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

/* Save the pre-transition load-state read before marking the commit complete. */
void mnuCommitTitleStreamReadyState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus[MNU_STREAM_CONTROL_INDEX] == 1 && mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] == MNU_STREAM_COMMIT_READY) {
        D_00437A38 = mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX];
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_COMMIT_COMPLETE;
        D_00454D68[0] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

extern u8 D_00455DB0[];

extern void sdfQueueNonzeroResourceId(u32 arg0);

/* Reset load/data slots only when an allocation is present; otherwise do nothing. */
void mnuResetTitleStream(void) {
    if (mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX] != 0) {
        sdfQueueNonzeroResourceId(mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX]);
        mnuTitleStreamStatus[MNU_STREAM_LOAD_STATE_INDEX] = MNU_STREAM_LOAD_IDLE;
        mnuTitleStreamStatus[MNU_STREAM_ALLOCATION_INDEX] = 0;
        mnuTitleStreamStatus[MNU_STREAM_DATA_ADDRESS_INDEX] = 0;
        mnuTitleStreamStatus[6] = (u32)D_00455DB0;
    }
}

void mnuResetTitleStreamLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_00455D98[];

/* Install the default medium-frame buffer and stream under the shared lock. */
INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428650);

void mnuInitializeTitleSoundBuffer(void) {
    u32 *streamState = mnuTitleSoundBufferState;
    u32 *decoder = D_00455D98;
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
    streamState[6] = (u32)D_00455DB0;
    streamState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", streamState);
    func_003504A8(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

extern MnuTitleStreamEntry D_003E0F60[];
extern char D_00428650[];

/* Each format reserves 600 compressed frames before loading its named stream. */
void func_002A2628(s32 soundEntryIndex) {
    MemBlock *allocation = NULL;
    s32 bufferAddress;
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState[6] = (u32)D_00455DB0;
    mnuTitleSoundBufferState[3] = D_003E0F60[soundEntryIndex].parameter;
    mnuTitleSoundBufferState[7] = (u32)D_00455D98;
    mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] = 2;
    switch (D_003E0F60[soundEntryIndex].format) {
    case 1:
        D_00455D98[2] = 1;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_00455D98[2] = 2;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_00455D98[2] = 3;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_00455D98[2] = 4;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState[MNU_STREAM_ALLOCATION_INDEX] = (u32)allocation;
    mnuTitleSoundBufferState[MNU_STREAM_DATA_ADDRESS_INDEX] = bufferAddress;
    func_0035C860(soundPath, D_00428650, D_003E0F60[soundEntryIndex].filename);
    mnuLoadTitleStreamFrameData(soundPath, mnuTitleSoundBufferState);
    func_003504A8(D_00455D98);
    SignalSema(mnuTitleStreamSemaphore);
}

/* Install an in-memory ATRAC stream and configure the decoder for its frame
 * format while holding the shared sound-buffer semaphore. The native code
 * has no default-format guard or buffer-capacity check. */
void func_002A27A8(void *compressedData, s32 dataBytes, s32 format) {
    MemBlock *allocation = NULL;
    s32 bufferAddress;
    s32 frameCount;

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState[6] = (u32)D_00455DB0;
    mnuTitleSoundBufferState[7] = (u32)D_00455D98;
    mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] = 2;
    mnuTitleSoundBufferState[3] = -1;
    switch (format) {
    case 1:
        D_00455D98[2] = format;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_00455D98[2] = format;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_00455D98[2] = format;
        mnuTitleSoundBufferState[MNU_STREAM_FRAME_BYTES_INDEX] = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_00455D98[2] = format;
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
    func_003504A8(D_00455D98);
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

void mnuClearInactiveSoundBufferState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] != 1) {
        mnuTitleSoundBufferState[MNU_STREAM_CONTROL_INDEX] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuReleaseSoundBuffer(void);

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
    sdfQueueNonzeroResourceId(allocationHandle);
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

void func_002A2AA0(void) {
    func_002A3D70();
    func_002A3E38(1);
}

extern void kwlnFadeBackgroundStartOut(s32);
extern void mnuStopTitleMovieDraw(void);
extern void *sdfResourceRetainAddress(MemBlock *);
extern void *memset(void *, s32, u32);
extern void mnuRecreateMenuSelectionList(void);
extern void mnuSelectMenuListCursorByAdvance(s32);
extern s32 func_002A30C0(KwlnTask *);
extern u32 D_00435CBC;

KwlnTask *mnuCreateTitleMenuTask(s32 mode) {
    MemBlock *allocation;

    kwlnFadeBackgroundStartOut(0);
    mnuStopTitleMovieDraw();
    allocation = sdfAllocGeneralBlock(sizeof(MovieMenuState));
    mnuMovieMenuState = sdfResourceRetainAddress(allocation);
    memset(mnuMovieMenuState, 0, sizeof(MovieMenuState));
    mnuMovieMenuState->allocation = allocation;
    mnuRecreateMenuSelectionList();
    mnuMovieMenuState->state = 0;
    mnuMovieMenuState->cursor = 0;
    if (mode == 0) {
        mnuSelectMenuListCursorByAdvance(0);
    } else if (mode == 1) {
        mnuSelectMenuListCursorByAdvance(2);
    } else if (mode == 2) {
        mnuSelectMenuListCursorByAdvance(0);
    } else if (mode == 3) {
        mnuSelectMenuListCursorByAdvance(1);
    }
    mnuMovieMenuState->mode = mode;
    func_002A2AA0();
    D_00435CBC = 0x80000000;
    return kwlnTaskCreate(D_00428680, 0x2B19, 1, 0, func_002A30C0, 0, 0);
}

extern void func_003458E8(u32);


extern void mnuReleaseMenuResourceSlots(void);

extern void mnuDestroyMovieMenuSelectionList(void);

extern void sdfReleaseResourceAllocation(u32);

extern u32 D_00435BB0;

void mnuReleaseTitleMenuAssetsAndMarkClosed(void) {
    mnuStopTitleMovieDraw();
    func_003458E8(0);
    mnuReleaseMenuResourceSlots();
    mnuReleaseSpriteHandle();
    mnuDestroyMovieMenuSelectionList();
    sdfReleaseResourceAllocation((u32)mnuMovieMenuState->allocation);
    mnuMovieMenuState = 0;
    D_00435BB0 = 1;
}

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428680);

extern void mnuSlideBarSetState(u32 *work, s32 state);
extern void mnuSlideBarSetStateB(u32 *state, u32 mode);
extern void mnuSlideBarSetStateSmall(u32 *state, u32 mode);
extern void mnuPairedSlideBarSetState(PickList *work, s32 state);
extern void mnuTimedSlideBarSetState(SlideBarTimed *bar, s32 mode, s32 timer);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern void sdfSetGridScaledDrawBounds(s32 firstStart, s32 secondStart, s32 firstLength, s32 secondLength, u32 value);
extern void mnuArmTitleMovieDrawAndResetFrame(u32 arg0, s32 arg1);
extern u32 mnuIsTitleMovieDrawActive(void);
extern void func_002A50F8(void *work);

/* Title-menu event handler (proposed mnuHandleTitleMenuEvent): drives the menu's slide bars, the background
 * movie and its fade for menu events 2..32; returns 1 once the movie has stopped after event 32. */
s32 func_002A2C28(s32 event) {
    s32 done = 0;

    switch (event) {
    case 2:
        mnuSlideBarSetStateSmall(mnuMovieMenuState->barSmall, 2);
        mnuPairedSlideBarSetState(&mnuMovieMenuState->paired, 2);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedA, 2, 0);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedB, 2, 0);
        mnuSlideBarSetState(mnuMovieMenuState->bar, 2);
        mnuSlideBarSetStateB(mnuMovieMenuState->barB, 2);
        break;
    case 3:
        kwlnFadeInStart(0, 0, 0, 0);
        mnuSlideBarSetStateSmall(mnuMovieMenuState->barSmall, 3);
        mnuPairedSlideBarSetState(&mnuMovieMenuState->paired, 3);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedA, 2, 0);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedB, 3, 0);
        mnuSlideBarSetState(mnuMovieMenuState->bar, 3);
        mnuSlideBarSetStateB(mnuMovieMenuState->barB, 2);
        func_003458E8(1);
        sdfSetGridScaledDrawBounds(-0xA7, 0x5A, 0x200, 0x1C0, 0x80808080);
        mnuArmTitleMovieDrawAndResetFrame(0x5B, 0);
        break;
    case 5:
        mnuSlideBarSetStateSmall(mnuMovieMenuState->barSmall, 2);
        mnuPairedSlideBarSetState(&mnuMovieMenuState->paired, 2);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedA, 2, 0);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedB, 2, 0);
        mnuSlideBarSetState(mnuMovieMenuState->bar, 2);
        mnuSlideBarSetStateB(mnuMovieMenuState->barB, 2);
        func_002A50F8(mnuMovieMenuState->movie);
        func_003458E8(0);
        mnuStopTitleMovieDraw();
        break;
    case 19:
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedA, 3, 0);
        break;
    case 20:
        mnuPairedSlideBarSetState(&mnuMovieMenuState->paired, 1);
        break;
    case 21:
        if (++mnuMovieMenuState->cursor < 0x14B) {
            break;
        }
        mnuSlideBarSetState(mnuMovieMenuState->bar, 1);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedB, 1, 0);
        break;
    case 23:
        mnuSlideBarSetStateSmall(mnuMovieMenuState->barSmall, 1);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedA, 3, 0);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedA, 0, 0x14);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedB, 3, 0);
        mnuSlideBarSetState(mnuMovieMenuState->bar, 3);
        mnuSlideBarSetStateB(mnuMovieMenuState->barB, 1);
        func_003458E8(1);
        sdfSetGridScaledDrawBounds(-0xA7, 0x5A, 0x200, 0x1C0, 0x808080);
        mnuArmTitleMovieDrawAndResetFrame(0x5B, 1);
        break;
    case 25:
        mnuPairedSlideBarSetState(&mnuMovieMenuState->paired, 3);
        /* fallthrough */
    case 26:
    case 31:
        if (mnuMovieMenuState->timedA.pos < 0x200) {
            if (mnuMovieMenuState->movieDrawn == 0) {
                mnuArmTitleMovieDrawAndResetFrame(0x5B, 1);
            }
            if (mnuMovieMenuState->movieFrame < 0x32A) {
                mnuMovieMenuState->movieAlpha += 4;
                if (mnuMovieMenuState->movieAlpha > 0x80) {
                    mnuMovieMenuState->movieAlpha = 0x80;
                }
            } else if (mnuMovieMenuState->movieDrawn != 0) {
                mnuMovieMenuState->movieAlpha -= 4;
                if (mnuMovieMenuState->movieAlpha < 0) {
                    mnuMovieMenuState->movieAlpha = 0;
                }
                if (mnuMovieMenuState->movieAlpha == 0) {
                    mnuStopTitleMovieDraw();
                }
            }
        }
        sdfSetGridScaledDrawBounds(-0xA7, 0x5A, 0x200, 0x1C0, (mnuMovieMenuState->movieAlpha << 24) | 0x808080);
        mnuMovieMenuState->movieFrame++;
        break;
    case 32:
        if (!mnuIsTitleMovieDrawActive()) {
            break;
        }
        done = 1;
        mnuSlideBarSetStateSmall(mnuMovieMenuState->barSmall, 2);
        mnuPairedSlideBarSetState(&mnuMovieMenuState->paired, 2);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedA, 2, 0);
        mnuTimedSlideBarSetState(&mnuMovieMenuState->timedB, 2, 0);
        mnuSlideBarSetState(mnuMovieMenuState->bar, 2);
        mnuSlideBarSetStateB(mnuMovieMenuState->barB, 2);
        func_002A50F8(mnuMovieMenuState->movie);
        mnuStopTitleMovieDraw();
        func_003458E8(0);
        break;
    }
    return done;
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A30C0);

u32 func_002A3A50(s32 mode) {
    mnuCreateTitleMenuTask(mode);
    return 0xffffffff;
}

s32 mnuDestroyTitleMenuTask(void) {
    mnuReleaseTitleMenuAssetsAndMarkClosed();
    kwlnTaskDestroyWithHierarchyByName(D_00428680, 1);
    return 0;
}

u32 func_002A3AA0(void) {
    mnuCreateTitleMenuTask(0);
    return 0;
}

void mnuRestartRuntimeAfterViewer(void) {
    evtDestroySecondaryWorldNode();
    sdfDestroyRuntimeTask();
    sdfCreateRuntimeTask();
}
