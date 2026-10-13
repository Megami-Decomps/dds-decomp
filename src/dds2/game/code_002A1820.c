/* Transport definitions stay in the predecessor; see docs/idioms.md,
 * "Title-audio controller transport boundary". */

#include "common.h"
#include "mnu_result.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "kwln.h"
#include "mnu_title_effect.h"
#include "mnu.h"
#include "file.h"
#include "kwln_task_lifecycle.h"
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

typedef struct AtracInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} AtracInfo;

extern void *memcpy(void *, const void *, u32);

extern u32 D_004285E0[4];

extern u32 D_004285D0[4];

extern u32 mnuTitleStreamSemaphore;

extern s32 mnuPollTitleStreamStateLocked(void);

extern MovieMenuState *mnuMovieMenuState;

extern KwlnTask *kwlnTaskCreate();

extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 D_00438FEC;

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern char D_00428680[];

extern s32 WaitSema(u32);

extern s32 SignalSema(u32);

extern s32 func_0035C860(char *dst, const char *fmt, ...);

extern u32 D_00437A20[2];

extern u32 D_00437A28;

extern void sndEnsureMidiBankResident(s32);

extern void mnuInitializeTitleAudioStreams(void);

extern char D_00428610[];

extern char D_00428628[];

extern s32 func_0034E820(s32, s32, ...);

s32 sndCopyWordsToIopSynchronously(u32 sourceAddress, u32 iopAddress, u32 wordCount);

void mnuInitTitleSoundRemoteRequest(u32 wordCount);

s32 mnuCopySoundStreamOnStatusChange(u32 source, u32 words);

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

extern s32 D_00437A30, D_00437A34, D_00437A38, D_00437A3C;
extern u32 D_00454D58[], D_00455D98[];
extern u8 D_00455DB0[], D_003E2760[];

void mnuUpdateTitleAudioStreams(void) {
    s32 tick = D_00437A3C + 1;
    u8 *source;

    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == 4) {
        if (D_00454D58[4] < 31) {
            if (tick < D_00437A38) {
                D_00437A3C = tick;
            } else {
                D_00454D58[4]++;
                D_00437A3C = 0;
            }
        } else {
            D_00437A3C = tick;
            D_00454D58[4] = 0;
            mnuTitleStreamStatus.control = 2;
            mnuTitleStreamStatus.frameIndex = 0;
            mnuResetTitleStream();
        }
    } else {
        D_00437A3C = tick;
    }

    if (mnuTitleStreamStatus.control == 0 ||
        (D_00437A30 != 0 && mnuTitleStreamStatus.control == 1)) {
        D_00454D58[0] = (u32)(mnuTitleStreamStatus.compressedData
                              + mnuTitleStreamStatus.frameBytes * mnuTitleStreamStatus.frameIndex);
        func_00350570(D_00454D58);
    }
    if (mnuTitleStreamStatus.control == 1) {
        mnuTitleStreamStatus.samples = (s16 *)D_00454D58[1];
    } else {
        mnuTitleStreamStatus.samples = (s16 *)D_00455DB0;
    }

    if (mnuTitleSoundBufferState.control == 0 ||
        (D_00437A30 != 0 && mnuTitleSoundBufferState.control == 1)) {
        D_00455D98[0] = (u32)(mnuTitleSoundBufferState.compressedData
                              + mnuTitleSoundBufferState.frameBytes * mnuTitleSoundBufferState.frameIndex);
        func_00350570(D_00455D98);
    }
    if (mnuTitleSoundBufferState.control == 1) {
        mnuTitleSoundBufferState.samples = (s16 *)D_00455D98[1];
    } else {
        mnuTitleSoundBufferState.samples = (s16 *)D_00455DB0;
    }

    if (mnuTitleStreamStatus.control == 2) {
        switch (mnuTitleSoundBufferState.control) {
        case 2:
    
            D_00437A30 = mnuCopySoundStreamOnStatusChange((u32)D_00455DB0, 0x400);
            source = (u8 *)mnuTitleStreamStatus.samples;
            memcpy(D_00455DB0, source, 0x1000);
            if (D_00437A34 != 0) {
                sndUploadStreamToBothIopBuffers(0x400);
            }
            D_00437A34 = 0;
            return;
        }
    }

    if (D_00437A34 == 0) {
        func_002A1678(0x400);
    }
    D_00437A34 = 1;

    if (mnuTitleStreamStatus.control != 2 && mnuTitleSoundBufferState.control == 2) {
        D_00437A30 = mnuCopySoundStreamOnStatusChange((u32)mnuTitleStreamStatus.samples, 0x400);
        memcpy(D_003E2760, mnuTitleStreamStatus.samples, 0x1000);
    } else {
        sndMixSampleBuffers((s16 *)D_003E2760, &mnuTitleSoundBufferState, &mnuTitleStreamStatus);
        D_00437A30 = mnuCopySoundStreamOnStatusChange((u32)D_003E2760, 0x400);
    }

    if (mnuTitleStreamStatus.control == 0) {
        D_00437A30 = 1;
        mnuTitleStreamStatus.control = 1;
    }
    if (D_00437A30 != 0 && mnuTitleStreamStatus.control == 1) {
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
        D_00437A30 = 1;
        mnuTitleSoundBufferState.control = 1;
    }
    if (D_00437A30 != 0 && mnuTitleSoundBufferState.control == 1) {
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

extern SdfThreadNode mnuTitleStreamThread __attribute__((section(".sbss")));

extern u8 mnuTitleStreamThreadStack[];

void mnuCreateTitleStreamThread(void) {
    mnuTitleStreamSemaphore = sdfCreateSemaphore(1, 0xff, 0);
    sdfStartTrackedThread(&mnuTitleStreamThread, mnuRunTitleStreamThread, mnuTitleStreamThreadStack,
                  0x1000, 0x45, 0);
    sdfThreadSleepSelf();
}

extern u32 D_00454D58[];
extern u8 D_00455DB0[];
extern u32 D_00455D98[];
extern u8 D_00453D30[];
extern u8 D_00454D70[];

void mnuInitializeTitleAudioStreams(void) {
    memset(D_00455DB0, 0, 0x1000);

    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.compressedData = 0;
    mnuTitleStreamStatus.allocation = 0;
    mnuTitleStreamStatus.samples = (s16 *)D_00455DB0;
    mnuTitleStreamStatus.decoder = D_00454D58;
    mnuTitleStreamStatus.loadState = 0;

    mnuTitleSoundBufferState.control = 2;
    mnuTitleSoundBufferState.compressedData = 0;
    mnuTitleSoundBufferState.repeatFrame = 0;
    mnuTitleSoundBufferState.allocation = 0;
    mnuTitleSoundBufferState.samples = (s16 *)D_00455DB0;
    mnuTitleSoundBufferState.decoder = D_00455D98;
    mnuTitleSoundBufferState.loadState = 0;

    mnuInitTitleSoundRemoteRequest(0x400);

    D_00454D58[1] = (u32)D_00453D30;
    D_00454D58[3] = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(0x68CA));
    D_00454D58[4] = 0;
    D_00455D98[1] = (u32)D_00454D70;
    D_00455D98[3] = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(0x68CA));
    D_00455D98[4] = 2;

    mnuCreateTitleStreamThread();
}

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


/* Install the fixed title track and queue its compressed frames under the lock. */
INCLUDE_RODATA(const s32, "game/code_002A1820", D_00428590);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_004285A0);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_004285B0);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_004285C0);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_004285D0);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_004285E0);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_00428600);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_00428610);

INCLUDE_RODATA(const s32, "game/code_002A1820", D_00428628);

void mnuStartFixedSoundStream(void) {
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


/* Install the default medium-frame buffer and stream under the shared lock. */
INCLUDE_RODATA(const s32, "game/code_002A1820", D_00428650);

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
void mnuLoadTitleSoundBufferEntry(s32 soundEntryIndex) {
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

void mnuLoadTitleMenuAssets(void) {
    mnuLoadMovieMenuResourceSlots();
    mnuEnsureMovieMenuSpriteHandle(1);
}

extern void kwlnFadeBackgroundStartOut(s32);
extern void mnuStopTitleMovieDraw(void);
extern void *memset(void *, s32, u32);
extern void mnuRecreateMenuSelectionList(void);
extern void mnuSelectMenuListCursorByAdvance(s32);
extern s32 mnuUpdateTitleMenuTask(KwlnTask *);
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
    mnuLoadTitleMenuAssets();
    D_00435CBC = 0x80000000;
    return kwlnTaskCreate(D_00428680, 0x2B19, 1, 0, mnuUpdateTitleMenuTask, 0, 0);
}



extern void mnuReleaseMenuResourceSlots(void);

extern void mnuDestroyMovieMenuSelectionList(void);


extern u32 D_00435BB0;

void mnuReleaseTitleMenuAssetsAndMarkClosed(void) {
    mnuStopTitleMovieDraw();
    sdfSetStreamFrameDrawMode(0);
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
extern void mnuResetMovieWorkState(void *work);

/* Title-menu event handler (proposed mnuHandleTitleMenuEvent): drives the menu's slide bars, the background
 * movie and its fade for menu events 2..32; returns 1 once the movie has stopped after event 32. */
INCLUDE_RODATA(const s32, "game/code_002A1820", D_00428680);

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
        sdfSetStreamFrameDrawMode(1);
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
        mnuResetMovieWorkState(mnuMovieMenuState->movie);
        sdfSetStreamFrameDrawMode(0);
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
        sdfSetStreamFrameDrawMode(1);
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
        mnuResetMovieWorkState(mnuMovieMenuState->movie);
        mnuStopTitleMovieDraw();
        sdfSetStreamFrameDrawMode(0);
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
extern void mnuDrawMovieMenuSpriteSlot1F(s32);
extern void mnuDrawMovieMenuSpriteSlot20(u32);
extern void mnuDrawMovieMenuSpriteSlot21(u32);
extern u8 func_002A8028(void);
extern s32 mnuCheckMovieDecoderStatus(void);
extern void mnuClearGlobalMenuStateFields(void);
extern s32 mnuPollMovieMenuInputAndTimeout(void);
extern void mnuUpdateTitlePageByMode(void);
extern void mnuTitleResetSequenceTimers(void);
extern void mnuDrawTitleSceneForPhase(void);
extern u32 mnuGetMovieMenuSelectionIndex(void);
extern s32 func_002A5C58(void);
extern void func_002A5890(void);
extern void func_002A5F80(void);
extern void sndReleaseTrackHandle(u32);
extern void sdfSoundSetChannelCount(u32);
extern void sndStartTrackExtended(s32);
extern void dds3AdminSubmitModeRequest(s32, void *, u32, s32);
extern void mnuRestartRuntimeAfterViewer(s32);
extern void mnuAdvanceTimedSlideBar(SlideBarTimed *, s32);
extern void mnuAdvanceMultiSpriteSlideBar(SlideBar *, s32);
extern void mnuDrawMovieMenuSlideBar(SlideBarTimed *, SlideBar *, s32);
extern void mnuDrawExpandingMovieMenuPulseBar(MovieMenuPulse *, s32);
extern void mnuAdvancePairedSlideBars(SlideBarPair *, s32);
extern void mnuAdvanceSlideBarValue(SlideBar *, s32);
extern void mnuAdvanceSlideBar(PickList *, s32);
extern void mnuDrawAndAdvanceMovieMenuBar(SlideBar *, s32);

s32 mnuUpdateTitleMenuTask(KwlnTask *task) {
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
        mnuDrawMovieMenuSpriteSlot1F(0x80);
        if (kwlnFadeIsActive() == 0) {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 7:
        mnuDrawMovieMenuSpriteSlot1F(0x80);
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
            mnuDrawMovieMenuSpriteSlot1F(volume);
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
            mnuDrawMovieMenuSpriteSlot20(volume);
        }
        if (mnuMovieMenuState->cursor < 0x14) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 10:
        mnuDrawMovieMenuSpriteSlot20(0x80);
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
            mnuDrawMovieMenuSpriteSlot20(volume);
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
            mnuDrawMovieMenuSpriteSlot21(volume);
        }
        if (mnuMovieMenuState->cursor < 0x14) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state++;
        }
        break;
    case 13:
        mnuDrawMovieMenuSpriteSlot21(0x80);
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
            mnuDrawMovieMenuSpriteSlot21(0x80);
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
            sndReleaseTrackHandle(0x310000);
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
            sdfSetStreamFrameDrawMode(0);
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
            selectedItem = mnuGetMovieMenuSelectionIndex();
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
                sndReleaseTrackHandle(0x310000);
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
    mnuDrawMovieMenuSlideBar(&mnuMovieMenuState->timedA, (SlideBar *)&mnuMovieMenuState->timedB, 0x52);
    mnuDrawExpandingMovieMenuPulseBar((MovieMenuPulse *)mnuMovieMenuState->barSmall, 0x52);
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
