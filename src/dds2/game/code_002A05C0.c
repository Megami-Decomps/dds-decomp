#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "mnu_title_effect.h"
#include "mnu.h"
#include "file.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_thread.h"

/* The two title-audio states own the complete ten-word frame/decoder record. */
typedef struct TitleAudioStreamState {
    s32 frameCount;
    s32 frameIndex;
    s32 frameBytes;
    s32 repeatFrame;
    s32 control;
    u8 *compressedData;
    s16 *samples;
    u32 *decoder;
    struct SdfMemBlock *allocation;
    s32 loadState;
} TitleAudioStreamState;
typedef char TitleAudioStreamState_size[(sizeof(TitleAudioStreamState) == 0x28) ? 1 : -1];

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


#define MNU_STREAM_LOAD_IDLE 0
#define MNU_STREAM_LOAD_PENDING 1
#define MNU_STREAM_LOAD_COPIED 2
#define MNU_STREAM_COMMIT_READY 3
#define MNU_STREAM_COMMIT_COMPLETE 4

extern u32 mnuTitleStreamSemaphore;

extern s32 scrReadIntParameter(s32);

extern u32 sdfSoundIsCommandBusy(void);

extern s32 mnuPollTitleStreamStateLocked(void);

extern u32 D_00437A2C;

typedef struct MovieMenuState {
    struct SdfMemBlock *allocation;
    u8 pad04[0x0C];
    s32 state;
    s32 cursor;
    s32 mode;
    u8 pad1C[0x10];
    s8 movieOpened;      /* 0x2C */
    u8 pad2D[3];
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


extern KwlnTask *mnuTitleSoundTask;
extern KwlnTask *kwlnTaskCreate();

extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 D_00438FEC;

extern TitleAudioStreamState mnuTitleSoundBufferState;

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

extern s32 sceSifInitIopHeap(void);
extern s32 sceSifAllocIopHeap(s32 size);
extern void Exit(s32 status);
extern s32 D_00438FD8;

/* libc sprintf returns the signed vfprintf character count. */
extern s32 func_0035C860(char *dst, const char *fmt, ...);
extern u32 D_00437A20[2];

extern u32 D_00437A28;

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

u32 mnuIncrementTitleEffectFrameCounter(KwlnTask *task) {
    TitleEffectState *effectState;

    effectState = (TitleEffectState *)kwlnTaskGetUserValue(task);
    effectState->frameCounter = effectState->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    mnuTitleSoundTask = 0;
}

extern u8 D_00437A10[];

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *effectState = (TitleEffectState *)sdfAllocSizeClassBlock(MNU_TITLE_EFFECT_BYTES);
    KwlnTask *effectTask = kwlnTaskCreate(D_00437A10, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    mnuTitleSoundTask = effectTask;
    kwlnTaskSetUserValue(effectTask, (u32)effectState);
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

s32 mnuQueryTitleSoundBusy(void) {
    return sdfSoundIsCommandBusy();
}

void func_002A1338(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->frameCounter;
}

u32 sndOpStartTrackFromScript(void) {
    s32 sequence;

    sequence = scrReadIntParameter(0);
    sndStartTrackDefault(sequence);
    return 1;
}

u32 sndOpSetTrackDefaultVolumePan(void) {
    s32 sequence;

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
    u32 soundBusy;

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

extern s32 func_0034E820(s32, s32, ...);
extern u64 D_00438FD0;

s32 func_002A1790(u32 source, u32 words) {
    u64 status = func_0034E820(1, 0x8100, 0);

    if ((D_00438FD0 & 0x1000000) == (status & 0x1000000)) {
        return 0;
    }
    sndCopyWordsToIopSynchronously(source, D_00437A20[D_00437A2C & 1], words);
    D_00438FD0 = status;
    D_00437A2C ^= 1;
    return 1;
}

/* The argument is a word count, not a source address; both use the EE buffer. */
void sndUploadStreamToBothIopBuffers(u32 wordCount) {
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[0], wordCount);
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[1], wordCount);
    func_0034E820(1, 0x8010, 0xf80, 0);
    func_0034E820(1, 0x8010, 0x1080, 0);
    func_0034E820(1, 0x80e0, 0, 2, 0, 0);
}

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, TitleAudioStreamState *firstSource, TitleAudioStreamState *secondSource) {
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

extern SdfThreadNode mnuTitleStreamThread __attribute__((section(".sbss")));

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
    statusSnapshot->unk0 = mnuTitleStreamStatus.frameCount;
    statusSnapshot->unk4 = mnuTitleStreamStatus.frameIndex;
    statusSnapshot->unk8 = mnuTitleStreamStatus.repeatFrame;
    SignalSema(mnuTitleStreamSemaphore);
}

/* Restore the same condensed three-word status mapping under the lock. */
void mnuWriteTitleStreamStatusLocked(u32 *statusValues) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.frameCount = statusValues[0];
    mnuTitleStreamStatus.frameIndex = statusValues[1];
    mnuTitleStreamStatus.repeatFrame = statusValues[2];
    SignalSema(mnuTitleStreamSemaphore);
}

/* Copy into the state's installed buffer, count complete compressed
 * frames using its frame-byte divisor, then release the loaded resource. */

void mnuLoadTitleStreamFrameData(char *filePath, TitleAudioStreamState *streamState) {
    void *fileData;
    s32 frameCount;
    struct SdfMemBlock *resourceHandle = sdfReadNamedResource(filePath, (u32 *)&fileData, 0);
    s32 fileBytes = sdfMemoryGetBlockSize(resourceHandle);
    memcpy(streamState->compressedData, fileData, fileBytes);
    frameCount = fileBytes / streamState->frameBytes;
    streamState->frameIndex = 0;
    streamState->frameCount = frameCount;
    sdfQueueGeneralAllocationRelease(resourceHandle);
}

void mnuStoreTaskResult(char *audioPath) {
    D_00438FEC = (u32)fileQueueDefaultCallbackRequest(audioPath);
    mnuTitleStreamStatus.loadState = 1;
}

extern u32 D_00454D58[];

extern void func_003504A8(u32 *);

/* Allocate in global status, but use the supplied state's copy destination
 * and frame counts. Keep those distinct accesses and the cleanup-before-copy
 * ordering; return 1 after a ready file is copied, otherwise its ready result. */
s32 mnuCompleteTitleStreamFileLoad(TitleAudioStreamState *destinationState) {
    s32 ready = fileIsRequestReadyInCurrentMode((struct FileRequest *)D_00438FEC);

    if (ready != 0) {
        s32 resourceHandle = (s32)fileGetResourceHandle((struct FileRequest *)(u32)D_00438FEC);
        u32 fileDataAddress = fileGetLoadedDataAddress((struct FileRequest *)(u32)D_00438FEC);
        s32 fileBytes = (s32)fileGetResourceSize((struct FileRequest *)(u32)D_00438FEC);
        struct SdfMemBlock *allocation;

        filePollEntryCleanup((struct FileRequest *)(u32)D_00438FEC);
        allocation = sdfAllocGeneralBlockHigh(fileBytes);
        mnuTitleStreamStatus.compressedData = (u8 *)sdfMemoryGetBlockAddress(allocation);
        mnuTitleStreamStatus.allocation = allocation;
        memcpy(destinationState->compressedData, (void *)fileDataAddress, fileBytes);
        destinationState->frameCount = fileBytes / destinationState->frameBytes;
        destinationState->frameIndex = 0;
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)resourceHandle);
        func_003504A8(D_00454D58);
        mnuTitleStreamStatus.loadState = MNU_STREAM_LOAD_COPIED;
        ready = 1;
    }
    return ready;
}

extern u8 D_00455DB0[];

/* Install the fixed title track and queue its compressed frames under the lock. */
INCLUDE_RODATA(const s32, "game/code_002A05C0", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428600);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428610);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428628);

void func_002A2198(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = D_00454D58;
    mnuTitleStreamStatus.samples = (s16 *)D_00455DB0;
    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    D_00454D58[2] = 2;
    mnuStoreTaskResult("/soundat3/b_bgm2-2.at3");
    SignalSema(mnuTitleStreamSemaphore);
}


typedef struct MnuTitleStreamEntry {
    u8 format;
    u8 pad;
    s16 parameter;
    char filename[12];
} MnuTitleStreamEntry;

extern MnuTitleStreamEntry D_003E0B60[];



extern char D_00428650[];

/* Load the named sound stream for the requested entry format under the lock. */
void func_002A2200(s32 soundEntryIndex) {
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = D_00454D58;
    mnuTitleStreamStatus.samples = (s16 *)D_00455DB0;
    mnuTitleStreamStatus.repeatFrame = D_003E0B60[soundEntryIndex].parameter;
    mnuTitleStreamStatus.control = 2;
    switch (D_003E0B60[soundEntryIndex].format) {
    case 1:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        break;
    case 2:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 3:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 4:
        D_00454D58[2] = D_003E0B60[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        break;
    }
    func_0035C860(soundPath, D_00428650, D_003E0B60[soundEntryIndex].filename);
    mnuStoreTaskResult(soundPath);
    SignalSema(mnuTitleStreamSemaphore);
}

/* Poll a pending file without acquiring the semaphore in this entry point. */
u32 mnuUpdateTitleTransition(void) {
    if (mnuTitleStreamStatus.loadState == MNU_STREAM_LOAD_PENDING) {
        mnuCompleteTitleStreamFileLoad(&mnuTitleStreamStatus);
    }
    return mnuTitleStreamStatus.loadState;
}

/* Poll under the semaphore, but retain the native load-state read after unlock. */
s32 mnuPollTitleStreamStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.loadState == MNU_STREAM_LOAD_PENDING) {
        mnuCompleteTitleStreamFileLoad(&mnuTitleStreamStatus);
    }
    SignalSema(mnuTitleStreamSemaphore);
    return mnuTitleStreamStatus.loadState;
}

extern u32 D_00454D68[];

/* Wait/copy a pending file while locked, then prepare a commit for controls
 * other than 1. Unlike DDS1, this entry point has no BGM banner output. */
void mnuResetTitleStreamAfterFileIdle(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuUpdateTitleTransition() == MNU_STREAM_LOAD_PENDING) {
        fileWaitIdle();
        mnuCompleteTitleStreamFileLoad(&mnuTitleStreamStatus);
    }
    if (mnuTitleStreamStatus.control != 1) {
        mnuTitleStreamStatus.control = 0;
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_READY;
        D_00454D68[0] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuMarkTitleStreamResetPending(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.frameIndex = 0;
    mnuTitleStreamStatus.control = 2;
    SignalSema(mnuTitleStreamSemaphore);
}

extern s32 D_00437A38;

/* Commit the ready load only for control 1, and select the native value 6. */
void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == MNU_STREAM_COMMIT_READY) {
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_COMPLETE;
        D_00454D68[0] = 0;
        D_00437A38 = 6;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

/* Save the pre-transition load-state read before marking the commit complete. */
void mnuCommitTitleStreamReadyState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == MNU_STREAM_COMMIT_READY) {
        D_00437A38 = mnuTitleStreamStatus.loadState;
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_COMPLETE;
        D_00454D68[0] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

extern u8 D_00455DB0[];


/* Reset load/data slots only when an allocation is present; otherwise do nothing. */
void mnuResetTitleStream(void) {
    if (mnuTitleStreamStatus.allocation != 0) {
        sdfQueueGeneralAllocationRelease(mnuTitleStreamStatus.allocation);
        mnuTitleStreamStatus.loadState = MNU_STREAM_LOAD_IDLE;
        mnuTitleStreamStatus.allocation = 0;
        mnuTitleStreamStatus.compressedData = 0;
        mnuTitleStreamStatus.samples = (s16 *)D_00455DB0;
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
    TitleAudioStreamState *streamState = &mnuTitleSoundBufferState;
    u32 *decoder = D_00455D98;
    struct SdfMemBlock *allocation;
    s32 bufferAddress;

    WaitSema(mnuTitleStreamSemaphore);
    streamState->decoder = decoder;
    allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    streamState->allocation = allocation;
    streamState->control = 2;
    streamState->decoder[2] = 2;
    streamState->compressedData = (u8 *)bufferAddress;
    streamState->samples = (s16 *)D_00455DB0;
    streamState->frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", streamState);
    func_003504A8(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

extern MnuTitleStreamEntry D_003E0F60[];
extern char D_00428650[];

/* Each format reserves 600 compressed frames before loading its named stream. */
void func_002A2628(s32 soundEntryIndex) {
    struct SdfMemBlock *allocation = NULL;
    s32 bufferAddress;
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState.samples = (s16 *)D_00455DB0;
    mnuTitleSoundBufferState.repeatFrame = D_003E0F60[soundEntryIndex].parameter;
    mnuTitleSoundBufferState.decoder = D_00455D98;
    mnuTitleSoundBufferState.control = 2;
    switch (D_003E0F60[soundEntryIndex].format) {
    case 1:
        D_00455D98[2] = 1;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_00455D98[2] = 2;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_00455D98[2] = 3;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_00455D98[2] = 4;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState.allocation = allocation;
    mnuTitleSoundBufferState.compressedData = (u8 *)bufferAddress;
    func_0035C860(soundPath, D_00428650, D_003E0F60[soundEntryIndex].filename);
    mnuLoadTitleStreamFrameData(soundPath, &mnuTitleSoundBufferState);
    func_003504A8(D_00455D98);
    SignalSema(mnuTitleStreamSemaphore);
}

/* Install an in-memory ATRAC stream and configure the decoder for its frame
 * format while holding the shared sound-buffer semaphore. The native code
 * has no default-format guard or buffer-capacity check. */
void func_002A27A8(void *compressedData, s32 dataBytes, s32 format) {
    struct SdfMemBlock *allocation = NULL;
    s32 bufferAddress;
    s32 frameCount;

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState.samples = (s16 *)D_00455DB0;
    mnuTitleSoundBufferState.decoder = D_00455D98;
    mnuTitleSoundBufferState.control = 2;
    mnuTitleSoundBufferState.repeatFrame = -1;
    switch (format) {
    case 1:
        D_00455D98[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_00455D98[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_00455D98[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_00455D98[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState.allocation = allocation;
    mnuTitleSoundBufferState.compressedData = (u8 *)bufferAddress;
    memcpy((void *)bufferAddress, compressedData, dataBytes);
    frameCount = dataBytes / mnuTitleSoundBufferState.frameBytes;
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.frameCount = frameCount;
    func_003504A8(D_00455D98);
    SignalSema(mnuTitleStreamSemaphore);
}

/* These return codes are not load-state codes: 0 means no allocation,
 * 2 means control 2, and 3 covers the remaining allocated states. */
s32 mnuGetSoundBufferStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState.allocation == 0) {
        SignalSema(mnuTitleStreamSemaphore);
        return 0;
    }
    if (mnuTitleSoundBufferState.control == 2) {
        SignalSema(mnuTitleStreamSemaphore);
        return 2;
    }
    SignalSema(mnuTitleStreamSemaphore);
    return 3;
}

void mnuClearInactiveSoundBufferState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState.control != 1) {
        mnuTitleSoundBufferState.control = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuReleaseSoundBuffer(void);

void mnuResetSoundBuffer(void) {
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.control = 2;
    mnuReleaseSoundBuffer();
}

/* Release the allocation handle, not the data address, then clear its slot. */
void mnuReleaseSoundBuffer(void) {
    TitleAudioStreamState *streamState = &mnuTitleSoundBufferState;
    struct SdfMemBlock *allocationHandle = streamState->allocation;

    if (allocationHandle == 0) {
        return;
    }
    sdfQueueGeneralAllocationRelease(allocationHandle);
    streamState->allocation = 0;
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
extern void *memset(void *, s32, u32);
extern void mnuRecreateMenuSelectionList(void);
extern void mnuSelectMenuListCursorByAdvance(s32);
extern s32 func_002A30C0(KwlnTask *);
extern u32 D_00435CBC;

KwlnTask *mnuCreateTitleMenuTask(s32 mode) {
    struct SdfMemBlock *allocation;

    kwlnFadeBackgroundStartOut(0);
    mnuStopTitleMovieDraw();
    allocation = sdfAllocGeneralBlock(sizeof(MovieMenuState));
    mnuMovieMenuState = (void *)sdfResourceRetainAddress((struct SdfMemBlock *)(allocation));
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


extern u32 D_00435BB0;

void mnuReleaseTitleMenuAssetsAndMarkClosed(void) {
    mnuStopTitleMovieDraw();
    func_003458E8(0);
    mnuReleaseMenuResourceSlots();
    mnuReleaseSpriteHandle();
    mnuDestroyMovieMenuSelectionList();
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)((u32)mnuMovieMenuState->allocation));
    mnuMovieMenuState = 0;
    D_00435BB0 = 1;
}

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
INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428680);

s32 mnuHandleTitleMenuEvent(s32 event) {
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

/* Actual cross-unit contracts used by the title-menu callback. */
typedef struct MovieMenuPulse {
    s32 active;
    s32 phase;
    s32 alpha;
} MovieMenuPulse;

typedef struct SlideBarPair {
    u8 active[2];
    u8 pad[2];
    s32 pos[2];
} SlideBarPair;

extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern s32 kwlnFadeIsActive(void);
extern void kwlnFadeClear(void);
extern s32 mnuIsAnyMenuInputPressed(void);
extern u8 mnuHasSpriteHandle(void);
extern void mnuStartMovieMenuSfx16(s32);
extern void mnuStartMovieMenuSfx17(u32);
extern void mnuStartMovieMenuSfx18(u32);
extern u8 func_002A8028(void);
extern s32 mnuCheckMovieDecoderStatus(void);
extern void mnuClearGlobalMenuStateFields(void);
extern s32 mnuPollMovieMenuInputAndTimeout(void);
extern void mnuUpdateTitlePageByMode(void);
extern void mnuTitleResetSequenceTimers(void);
extern void mnuDrawTitleSceneForPhase(void);
extern u32 func_002A3C78(void);
extern s32 func_002A5C58(void);
extern void func_002A5890(void);
extern void func_002A5F80(void);
extern void func_00342580(u32);
extern void sdfSoundSetChannelCount(u32);
extern void sndStartTrackExtended(s32);
extern void dds3AdminSubmitModeRequest(s32, void *, u32, s32);
extern void mnuRestartRuntimeAfterViewer(s32);
extern void mnuAdvanceTimedSlideBar(SlideBarTimed *, s32);
extern void mnuAdvanceMultiSpriteSlideBar(SlideBar *, s32);
extern void func_002A5040(SlideBarTimed *, SlideBar *, s32);
extern void func_002A4208(MovieMenuPulse *, s32);
extern void mnuAdvancePairedSlideBars(SlideBarPair *, s32);
extern void mnuAdvanceSlideBarValue(SlideBar *, s32);
extern void mnuAdvanceSlideBar(PickList *, s32);
extern void mnuDrawAndAdvanceMovieMenuBar(SlideBar *, s32);

s32 func_002A30C0(KwlnTask *task) {
    s32 volume;
    s32 menuAction;
    s32 selectedItem;

    switch (mnuMovieMenuState->state) {
    case 0:
        mnuMovieMenuState->cursor = 0;
        mnuMovieMenuState->state = 2;
        sndEnsureMidiBankResident(0x310000);
        break;
    case 1:
    case 22:
    case 27:
    case 29:
    case 35:
        break;
    case 2:
        mnuMovieMenuState->cursor = 0;
        D_00435BB0 = 0;
        if (mnuMovieMenuState->mode != 0) {
            mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
            mnuMovieMenuState->state = 3;
            if (mnuMovieMenuState->mode == 3) {
                if (fileLoadStateChanged() == 0) {
                    fileCacheSlotFlagsFromState();
                } else {
                    fileRestoreSlotFlagsToState();
                }
            }
        } else {
            kwlnFadeOutStart(0, 0, 0, 0x14);
            mnuMovieMenuState->state = 5;
        }
        mnuMovieMenuState->mode = 0;
        break;
    case 3:
        if (mnuIsTitleMovieDrawActive() == 0) {
            mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
        } else if (func_002A8028() != 0) {
            mnuMovieMenuState->state = 4;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 4:
        if (mnuHasSpriteHandle() != 0) {
            mnuMovieMenuState->state = 31;
            kwlnFadeOutStart(0, 0, 0, 0x14);
            if (mnuMovieMenuState->mode >= 0) {
                if (mnuMovieMenuState->mode < 2) {
                    sndStartTrackExtended(0x310000);
                }
            }
        }
        break;
    case 5:
        kwlnFadeOutStart(0, 0, 0, 0x14);
        mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
        mnuMovieMenuState->state++;
        break;
    case 6:
        mnuStartMovieMenuSfx16(0x80);
        if (kwlnFadeIsActive() == 0) {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 7:
        mnuStartMovieMenuSfx16(0x80);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->cursor = 0x4B;
        }
        if (mnuMovieMenuState->cursor < 0x4B) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 8:
        volume = 0x80 - mnuMovieMenuState->cursor * 8;
        if (volume > 0) {
            mnuStartMovieMenuSfx16(volume);
        }
        if (mnuMovieMenuState->cursor < 0x16) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 9:
        volume = mnuMovieMenuState->cursor * 6;
        if (volume >= 0x81) {
            volume = 0x80;
        }
        if (volume > 0) {
            mnuStartMovieMenuSfx17(volume);
        }
        if (mnuMovieMenuState->cursor < 0x14) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 10:
        mnuStartMovieMenuSfx17(0x80);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->cursor = 0x4B;
        }
        if (mnuMovieMenuState->cursor < 0x4B) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 11:
        volume = 0x80 - mnuMovieMenuState->cursor * 8;
        if (volume > 0) {
            mnuStartMovieMenuSfx17(volume);
        }
        if (mnuMovieMenuState->cursor < 0x16) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 12:
        volume = mnuMovieMenuState->cursor * 6;
        if (volume >= 0x81) {
            volume = 0x80;
        }
        if (volume > 0) {
            mnuStartMovieMenuSfx18(volume);
        }
        if (mnuMovieMenuState->cursor < 0x14) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 13:
        mnuStartMovieMenuSfx18(0x80);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->cursor = 0x4B;
        }
        if (mnuMovieMenuState->cursor < 0x4B) {
            mnuMovieMenuState->cursor++;
        } else {
            kwlnFadeInStart(0, 0, 0, 0x16);
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 14:
        if (mnuMovieMenuState->cursor < 0x16) {
            mnuStartMovieMenuSfx18(0x80);
            mnuMovieMenuState->cursor++;
        } else {
            kwlnFadeOutStart(0, 0, 0, 0x1E);
            mnuMovieMenuState->state = 15;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 15: {
        MovieMenuState *menu;
        mnuArmTitleMovieDrawAndResetFrame(0x5C, 0);
        menu = mnuMovieMenuState;
        menu->movieOpened = 0;
        menu->state = 16;
        break;
    }
    case 16:
        if (func_002A8028() != 0) {
            mnuMovieMenuState->state = 17;
        }
        break;
    case 17:
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->state = 18;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 0x16);
            sdfSoundSetChannelCount(0x2D);
            mnuMovieMenuState->movieOpened = 1;
        }
        if (mnuCheckMovieDecoderStatus() != 0) {
            mnuMovieMenuState->state = 18;
            mnuMovieMenuState->cursor = 0x2D;
        }
        break;
    case 18:
        if (mnuMovieMenuState->cursor >= 0x2D) {
            mnuStopTitleMovieDraw();
            mnuMovieMenuState->state = 19;
            mnuMovieMenuState->cursor = 0;
        } else {
            mnuMovieMenuState->cursor++;
        }
        break;
    case 19:
        kwlnFadeOutStart(0, 0, 0, 0x1E);
        if (mnuMovieMenuState->movieOpened != 0) {
            mnuMovieMenuState->state = 23;
        } else {
            mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
            func_002A5F80();
            mnuMovieMenuState->state = 20;
        }
        break;
    case 20:
        if (func_002A8028() != 0) {
            mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
            mnuMovieMenuState->state = 21;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 21:
        mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->state = 23;
            mnuMovieMenuState->cursor = 0;
            mnuStopTitleMovieDraw();
        }
        if (mnuCheckMovieDecoderStatus() != 0) {
            mnuMovieMenuState->state = 23;
            mnuMovieMenuState->cursor = 0;
            mnuStopTitleMovieDraw();
        }
        break;
    case 23:
        mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
        mnuMovieMenuState->state = 24;
        break;
    case 24:
        if (mnuHasSpriteHandle() != 0) {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 25:
        mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
        mnuMovieMenuState->cursor = 0;
        mnuMovieMenuState->state++;
        func_002A5890();
        break;
    case 26:
        mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
        menuAction = mnuPollMovieMenuInputAndTimeout();
        if (menuAction != -1) {
            if (menuAction != 1) {
                mnuUpdateTitlePageByMode();
            } else {
                mnuSelectMenuListCursorByAdvance(0);
                mnuUpdateTitlePageByMode();
                mnuMovieMenuState->state = 31;
                mnuMovieMenuState->cursor = 0;
                mnuTitleResetSequenceTimers();
            }
        } else {
            mnuUpdateTitlePageByMode();
            mnuMovieMenuState->state = 28;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 0xF);
            func_00342580(0x310000);
        }
        break;
    case 28:
        if (mnuMovieMenuState->cursor < 0x78) {
            mnuUpdateTitlePageByMode();
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = 5;
            mnuStopTitleMovieDraw();
            func_003458E8(0);
        }
        break;
    case 30:
        if (kwlnFadeIsActive() == 0) {
            mnuMovieMenuState->state = 31;
            mnuMovieMenuState->cursor = 0;
            mnuTitleResetSequenceTimers();
        }
        break;
    case 31:
        mnuHandleTitleMenuEvent(mnuMovieMenuState->state);
        menuAction = func_002A5C58();
        switch (menuAction) {
        case 1:
            mnuDrawTitleSceneForPhase();
            mnuMovieMenuState->state = 32;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 0xF);
            break;
        case 2:
            mnuDrawTitleSceneForPhase();
            mnuMovieMenuState->state = 33;
            mnuMovieMenuState->cursor = 0;
            mnuClearGlobalMenuStateFields();
            mnuMovieMenuState->state = 26;
            mnuMovieMenuState->cursor = 0;
            break;
        default:
            mnuDrawTitleSceneForPhase();
            break;
        }
        break;
    case 32:
        if (mnuMovieMenuState->cursor < 0xF) {
            mnuMovieMenuState->cursor++;
        } else if (mnuHandleTitleMenuEvent(mnuMovieMenuState->state) == 0) {
            selectedItem = func_002A3C78();
            switch (selectedItem) {
            case 1:
                kwlnFadeOutStart(0, 0, 0, 0x14);
                mnuMovieMenuState->state = 34;
                mnuMovieMenuState->cursor = 0;
                break;
            case 0:
                mnuMovieMenuState->mode = 1;
                dds3AdminSubmitModeRequest(3, 0, 0, 0);
                mnuMovieMenuState->state = 35;
                break;
            case 2:
                func_00342580(0x310000);
                mnuMovieMenuState->mode = selectedItem;
                mnuMovieMenuState->cursor = 0;
                kwlnFadeOutStart(0, 0, 0, 0xA);
                dds3AdminSubmitModeRequest(0xD, 0, 0, 0);
                mnuMovieMenuState->state = 35;
                break;
            }
        }
        break;
    case 33:
        mnuClearGlobalMenuStateFields();
        mnuMovieMenuState->state = 26;
        mnuMovieMenuState->cursor = 0;
        break;
    case 34:
        kwlnFadeClear();
        mnuRestartRuntimeAfterViewer(0);
        dds3AdminSubmitModeRequest(0x1E, 0, 0, 0);
        mnuMovieMenuState->state = 35;
        mnuMovieMenuState->mode = 3;
        mnuMovieMenuState->cursor = 0;
        break;
    }

    mnuAdvanceTimedSlideBar(&mnuMovieMenuState->timedA, 0x52);
    mnuAdvanceMultiSpriteSlideBar((SlideBar *)&mnuMovieMenuState->timedB, 0x52);
    func_002A5040(&mnuMovieMenuState->timedA, (SlideBar *)&mnuMovieMenuState->timedB, 0x52);
    func_002A4208((MovieMenuPulse *)mnuMovieMenuState->barSmall, 0x52);
    mnuAdvancePairedSlideBars((SlideBarPair *)mnuMovieMenuState->movie, 0x52);
    mnuAdvanceSlideBarValue((SlideBar *)mnuMovieMenuState->barB, 0x52);
    mnuAdvanceSlideBar(&mnuMovieMenuState->paired, 0x53);
    mnuDrawAndAdvanceMovieMenuBar((SlideBar *)mnuMovieMenuState->bar, 0x53);
    return 0;
}

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

/* The title-menu callback passes zero; the restart does not use it. */
void mnuRestartRuntimeAfterViewer(s32 unused) {
    evtDestroySecondaryWorldNode();
    sdfDestroyRuntimeTask();
    sdfCreateRuntimeTask();
}
