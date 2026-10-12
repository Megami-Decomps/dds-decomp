/* Transport definitions stay in the predecessor; see docs/idioms.md,
 * "Title-audio controller transport boundary". */

#include "common.h"
#include "mnu_result.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "mnu_title_effect.h"
#include "file.h"
#include "file_request_api.h"
#include "sdf_thread.h"
#include "mnu_title_stream.h"

#define SND_MIX_SAMPLE_COUNT 0x800
#define SND_MIX_SAMPLE_LIMIT 0x7FFF
#define MNU_TITLE_SOUND_PATH_BYTES 32
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

extern u32 dds3AdvanceWorldCounter(void);

extern s32 dds3CreateCameraObject(s32 counter, f32 *position, f32 *rotation);

extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);

extern void effObjSetInnerFloat(s32 object, f32 value);

extern s32 dds3GetWorldSecondaryObject(void);

extern u8 D_003DC1C0[];

extern u8 D_003DC1D0[];

extern void *mnuTitleCameraObject;

extern char D_003AFD48[];

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern u32 D_003DA1C0[];

extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 mnuTitleStreamSemaphore;

extern u32 D_003BD8D4;

extern s32 D_003BC5C8;

extern u8 D_003D9178[];

extern s32 mnuPollTitleStreamStateLocked(void);

extern s32 func_003014F0(char *dst, const char *fmt, ...);

extern s32 func_003003F0(const char *fmt, ...);

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;

extern void mnuInitializeTitleAudioStreams(void);

extern s32 func_002F5990(s32, s32, ...);

s32 sndCopyWordsToIopSynchronously(u32 sourceAddress, u32 iopAddress, u32 wordCount);

void mnuInitTitleSoundRemoteRequest(u32 wordCount);

s32 func_00269B80(u32 source, u32 words);

/* The argument is a word count, not a source address; both use the EE buffer. */
void sndUploadStreamToBothIopBuffers(u32 wordCount) {
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[1], wordCount);
    func_002F5990(1, 0x8010, 0xf80, 0);
    func_002F5990(1, 0x8010, 0x1080, 0);
    func_002F5990(1, 0x80e0, 0, 2, 0, 0);
}

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, TitleAudioStreamState *firstSource, TitleAudioStreamState *secondSource) {
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

extern s32 D_003BC5C0;
extern s32 D_003BC5C4;
extern s32 D_003BC5C8;
extern s32 D_003BC5CC;
extern u32 D_003D9168[];
extern u32 D_003DA1A8[];
extern u32 D_003DA1C0[];
extern u8 D_00378E50[];
extern TitleAudioStreamState mnuTitleStreamStatus;
extern TitleAudioStreamState mnuTitleSoundBufferState;


void mnuUpdateTitleAudioStreams(void) {
    s32 tick = D_003BC5CC + 1;
    u8 *source;

    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == 4) {
        if (D_003D9168[4] < 31) {
            if (tick < D_003BC5C8) {
                D_003BC5CC = tick;
            } else {
                D_003D9168[4]++;
                D_003BC5CC = 0;
            }
        } else {
            D_003BC5CC = tick;
            D_003D9168[4] = 0;
            mnuTitleStreamStatus.control = 2;
            mnuTitleStreamStatus.frameIndex = 0;
            mnuResetTitleStream();
        }
    } else {
        D_003BC5CC = tick;
    }

    if (mnuTitleStreamStatus.control == 0 ||
        (D_003BC5C0 != 0 && mnuTitleStreamStatus.control == 1)) {
        D_003D9168[0] = (u32)(mnuTitleStreamStatus.compressedData
                              + mnuTitleStreamStatus.frameBytes * mnuTitleStreamStatus.frameIndex);
        func_002F76F0(D_003D9168);
    }
    if (mnuTitleStreamStatus.control == 1) {
        mnuTitleStreamStatus.samples = (s16 *)D_003D9168[1];
    } else {
        mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
    }

    if (mnuTitleSoundBufferState.control == 0 ||
        (D_003BC5C0 != 0 && mnuTitleSoundBufferState.control == 1)) {
        D_003DA1A8[0] = (u32)(mnuTitleSoundBufferState.compressedData
                              + mnuTitleSoundBufferState.frameBytes * mnuTitleSoundBufferState.frameIndex);
        func_002F76F0(D_003DA1A8);
    }
    if (mnuTitleSoundBufferState.control == 1) {
        mnuTitleSoundBufferState.samples = (s16 *)D_003DA1A8[1];
    } else {
        mnuTitleSoundBufferState.samples = (s16 *)D_003DA1C0;
    }

    if (mnuTitleStreamStatus.control == 2 && mnuTitleSoundBufferState.control == 2) {
        D_003BC5C0 = func_00269B80((u32)D_003DA1C0, 0x400);
        source = (u8 *)mnuTitleStreamStatus.samples;
        memcpy(D_003DA1C0, source, 0x1000);
        if (D_003BC5C4 != 0) {
            sndUploadStreamToBothIopBuffers(0x400);
        }
        D_003BC5C4 = 0;
        return;
    }

    if (D_003BC5C4 == 0) {
        func_00269A68(0x400);
    }
    D_003BC5C4 = 1;

    if (mnuTitleStreamStatus.control != 2 && mnuTitleSoundBufferState.control == 2) {
        D_003BC5C0 = func_00269B80((u32)mnuTitleStreamStatus.samples, 0x400);
        memcpy(D_00378E50, mnuTitleStreamStatus.samples, 0x1000);
    } else {
        sndMixSampleBuffers((s16 *)D_00378E50, &mnuTitleSoundBufferState, &mnuTitleStreamStatus);
        D_003BC5C0 = func_00269B80((u32)D_00378E50, 0x400);
    }

    if (mnuTitleStreamStatus.control == 0) {
        D_003BC5C0 = 1;
        mnuTitleStreamStatus.control = 1;
    }
    if (D_003BC5C0 != 0 && mnuTitleStreamStatus.control == 1) {
        mnuTitleStreamStatus.frameIndex++;
        if (mnuTitleStreamStatus.frameIndex >= mnuTitleStreamStatus.frameCount) {
            if (mnuTitleStreamStatus.repeatFrame == -1) {
                mnuTitleStreamStatus.frameIndex = 0;
                mnuTitleStreamStatus.control = 2;
                mnuResetTitleStream();
            } else {
                mnuTitleStreamStatus.frameIndex = mnuTitleStreamStatus.repeatFrame;
            }
        }
    }

    if (mnuTitleSoundBufferState.control == 0) {
        D_003BC5C0 = 1;
        mnuTitleSoundBufferState.control = 1;
    }
    if (D_003BC5C0 != 0 && mnuTitleSoundBufferState.control == 1) {
        mnuTitleSoundBufferState.frameIndex++;
        if (mnuTitleSoundBufferState.frameIndex >= mnuTitleSoundBufferState.frameCount) {
            mnuTitleSoundBufferState.frameIndex = 0;
            mnuTitleSoundBufferState.control = 2;
            mnuReleaseSoundBuffer();
        }
    }
}


void mnuRunTitleStreamThread(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        WaitSema(mnuTitleStreamSemaphore);
        mnuUpdateTitleAudioStreams();
        SignalSema(mnuTitleStreamSemaphore);
    }
}

extern u32 mnuTitleStreamSemaphore;

extern SdfThreadNode mnuTitleStreamThread __attribute__((section(".sbss")));

extern u8 mnuTitleStreamThreadStack[];

void mnuCreateTitleStreamThread(void) {
    mnuTitleStreamSemaphore = sdfCreateSemaphore(1, 0xff, 0);
    sdfStartTrackedThread(&mnuTitleStreamThread, mnuRunTitleStreamThread, mnuTitleStreamThreadStack,
                  0x1000, 0x45, 0);
    sdfThreadSleepSelf();
}

extern u32 D_003D9168[];
extern u32 D_003DA1A8[];
extern u8 D_003D8140[];
extern u8 D_003D9180[];

void mnuInitializeTitleAudioStreams(void) {
    memset(D_003DA1C0, 0, 0x1000);

    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.compressedData = 0;
    mnuTitleStreamStatus.allocation = 0;
    mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
    mnuTitleStreamStatus.decoder = D_003D9168;
    mnuTitleStreamStatus.loadState = 0;

    mnuTitleSoundBufferState.control = 2;
    mnuTitleSoundBufferState.compressedData = 0;
    mnuTitleSoundBufferState.repeatFrame = 0;
    mnuTitleSoundBufferState.allocation = 0;
    mnuTitleSoundBufferState.samples = (s16 *)D_003DA1C0;
    mnuTitleSoundBufferState.decoder = D_003DA1A8;
    mnuTitleSoundBufferState.loadState = 0;

    mnuInitTitleSoundRemoteRequest(0x400);

    D_003D9168[1] = (u32)D_003D8140;
    D_003D9168[3] = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(0x68CA));
    D_003D9168[4] = 0;
    D_003DA1A8[1] = (u32)D_003D9180;
    D_003DA1A8[3] = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(0x68CA));
    D_003DA1A8[4] = 2;

    mnuCreateTitleStreamThread();
}

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
    statusSnapshot->unk0 = mnuTitleStreamStatus.frameCount;
    statusSnapshot->unk4 = mnuTitleStreamStatus.frameIndex;
    statusSnapshot->unk8 = mnuTitleStreamStatus.repeatFrame;
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 mnuTitleStreamSemaphore;


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
    D_003BD8D4 = (u32)fileQueueDefaultCallbackRequest(audioPath);
    mnuTitleStreamStatus.loadState = 1;
}

extern u32 D_003D9168[];







extern void func_002F7628(u32 *);

/* Allocate in global status, but use the supplied state's copy destination
 * and frame counts. Keep those distinct accesses and the cleanup-before-copy
 * ordering; return 1 after a ready file is copied, otherwise its ready result. */
s32 mnuCompleteTitleStreamFileLoad(TitleAudioStreamState *destinationState) {
    s32 ready = fileIsRequestReadyInCurrentMode((struct FileRequest *)D_003BD8D4);

    if (ready != 0) {
        s32 resourceHandle = (s32)fileGetResourceHandle((struct FileRequest *)(u32)D_003BD8D4);
        u32 fileDataAddress = fileGetLoadedDataAddress((struct FileRequest *)(u32)D_003BD8D4);
        s32 fileBytes = (s32)fileGetResourceSize((struct FileRequest *)(u32)D_003BD8D4);
        struct SdfMemBlock *allocation;

        filePollEntryCleanup((struct FileRequest *)(u32)D_003BD8D4);
        allocation = sdfAllocGeneralBlockHigh(fileBytes);
        mnuTitleStreamStatus.compressedData = (u8 *)sdfMemoryGetBlockAddress(allocation);
        mnuTitleStreamStatus.allocation = allocation;
        memcpy(destinationState->compressedData, (void *)fileDataAddress, fileBytes);
        destinationState->frameCount = fileBytes / destinationState->frameBytes;
        destinationState->frameIndex = 0;
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)resourceHandle);
        func_002F7628(D_003D9168);
        mnuTitleStreamStatus.loadState = MNU_STREAM_LOAD_COPIED;
        ready = 1;
    }
    return ready;
}

/* Install the fixed title track and queue its compressed frames under the lock. */
void mnuStartFixedSoundStream(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = D_003D9168;
    mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    D_003D9168[2] = 2;
    mnuStoreTaskResult("/soundat3/b_bgm2-2.at3");
    SignalSema(mnuTitleStreamSemaphore);
}

typedef struct MnuTitleStreamEntry {
    u8 format;
    u8 pad;
    s16 parameter;
    char filename[12];
} MnuTitleStreamEntry;

extern MnuTitleStreamEntry D_00377650[];

extern MnuTitleStreamEntry D_00377350[];

extern char D_003AFCF0[];


/* Load the named sound stream for the requested entry format under the lock. */
void func_0026A5F0(s32 soundEntryIndex) {
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = D_003D9168;
    mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
    mnuTitleStreamStatus.repeatFrame = D_00377350[soundEntryIndex].parameter;
    mnuTitleStreamStatus.control = 2;
    switch (D_00377350[soundEntryIndex].format) {
    case 1:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        break;
    case 2:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 3:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 4:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        break;
    }
    func_003014F0(soundPath, D_003AFCF0, D_00377350[soundEntryIndex].filename);
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

extern void fileWaitIdle(void);

/* Wait/copy a pending file while locked, then prepare a commit for controls
 * other than 1. DDS1 also prints its retail BGM banner before unlocking. */
INCLUDE_RODATA(const s32, "game/code_00269C10", D_003AFCF0);

void mnuTitleStreamUpdateAndLogBgm(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuUpdateTitleTransition() == MNU_STREAM_LOAD_PENDING) {
        fileWaitIdle();
        mnuCompleteTitleStreamFileLoad(&mnuTitleStreamStatus);
    }
    if (mnuTitleStreamStatus.control != 1) {
        mnuTitleStreamStatus.control = 0;
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_READY;
        *(u32 *)D_003D9178 = 0;
    }
    func_003003F0("----------- AT3 BGM Play ------------\n");
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuMarkTitleStreamResetPending(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.frameIndex = 0;
    mnuTitleStreamStatus.control = 2;
    SignalSema(mnuTitleStreamSemaphore);
}

/* Commit the ready load only for control 1, and select the native value 6. */
void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == MNU_STREAM_COMMIT_READY) {
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_COMPLETE;
        *(u32 *)D_003D9178 = 0;
        D_003BC5C8 = 6;
    }
    SignalSema(mnuTitleStreamSemaphore);
}


/* Save the pre-transition load-state read before marking the commit complete. */
void mnuCommitTitleStreamReadyState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == MNU_STREAM_COMMIT_READY) {
        D_003BC5C8 = mnuTitleStreamStatus.loadState;
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_COMPLETE;
        *(u32 *)D_003D9178 = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

/* Reset load/data slots only when an allocation is present; otherwise do nothing. */
void mnuResetTitleStream(void) {
    if (mnuTitleStreamStatus.allocation != 0) {
        sdfQueueGeneralAllocationRelease(mnuTitleStreamStatus.allocation);
        mnuTitleStreamStatus.loadState = MNU_STREAM_LOAD_IDLE;
        mnuTitleStreamStatus.allocation = 0;
        mnuTitleStreamStatus.compressedData = 0;
        mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
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
    TitleAudioStreamState *streamState = &mnuTitleSoundBufferState;
    u32 *decoder = D_003DA1A8;
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
    streamState->samples = (s16 *)D_003DA1C0;
    streamState->frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", streamState);
    func_002F7628(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

extern char D_003AFCF0[];

/* Each format reserves 600 compressed frames before loading its named stream. */
void mnuLoadTitleSoundBufferEntry(s32 soundEntryIndex) {
    struct SdfMemBlock *allocation = NULL;
    s32 bufferAddress;
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState.samples = (s16 *)D_003DA1C0;
    mnuTitleSoundBufferState.repeatFrame = D_00377650[soundEntryIndex].parameter;
    mnuTitleSoundBufferState.decoder = D_003DA1A8;
    mnuTitleSoundBufferState.control = 2;
    switch (D_00377650[soundEntryIndex].format) {
    case 1:
        D_003DA1A8[2] = 1;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_003DA1A8[2] = 2;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_003DA1A8[2] = 3;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_003DA1A8[2] = 4;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState.allocation = allocation;
    mnuTitleSoundBufferState.compressedData = (u8 *)bufferAddress;
    func_003014F0(soundPath, D_003AFCF0, D_00377650[soundEntryIndex].filename);
    mnuLoadTitleStreamFrameData(soundPath, &mnuTitleSoundBufferState);
    func_002F7628(D_003DA1A8);
    SignalSema(mnuTitleStreamSemaphore);
}

/* Install an in-memory ATRAC stream and configure the decoder for its frame
 * format while holding the shared sound-buffer semaphore. The native code
 * has no default-format guard or buffer-capacity check. */
void func_0026ABA8(void *compressedData, s32 dataBytes, s32 format) {
    struct SdfMemBlock *allocation = NULL;
    s32 bufferAddress;
    s32 frameCount;

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState.samples = (s16 *)D_003DA1C0;
    mnuTitleSoundBufferState.decoder = D_003DA1A8;
    mnuTitleSoundBufferState.control = 2;
    mnuTitleSoundBufferState.repeatFrame = -1;
    switch (format) {
    case 1:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_003DA1A8[2] = format;
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
    func_002F7628(D_003DA1A8);
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

void mnuPrintTitleDebugBanner(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState.control != 1) {
        mnuTitleSoundBufferState.control = 0;
    }
    func_003003F0(D_003AFD48);
    SignalSema(mnuTitleStreamSemaphore);
}

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
    sdfReleaseResourceAllocation(allocationHandle);
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

void func_0026AEB0(void) {
    mnuLoadMovieMenuResourceSlots();
}

INCLUDE_RODATA(const s32, "game/code_00269C10", D_003AFD48);

void mnuCreateTitleCameraWorldEntry(void) {
    mnuTitleCameraObject = dds3CreateCameraObject(dds3AdvanceWorldCounter(), (f32 *)D_003DC1C0, (f32 *)D_003DC1D0);
    dds3SetWorldNodeValue(mnuTitleCameraObject, (u32)"title_camera");
    effObjSetInnerFloat(mnuTitleCameraObject, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), mnuTitleCameraObject);
}
