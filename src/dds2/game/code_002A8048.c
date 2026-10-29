#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_parameter_index.h"
#include "scr_command_result.h"
#include "scr_integer_return_value_api.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "kwln.h"
#include "kwln_task_runner.h"
#include "kwln_task_create.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "kwln_task_lifecycle.h"
#include "mnu_movie_transfer.h"
#include "mnu_movie_list.h"
#include "mnu_movie_playback_api.h"
#include "sce_io.h"
#include "btl.h"
/* Declared before first use (were implicit int). */
extern void dds3Filter2Off(s32 duration);
extern void sdfIpuInit(void);

extern u16 dds3VblanksPerFrame;

extern s32 mnuMovieShutdownCounter;

extern movieStream_t mnuMovieDrawContext;

extern MnuMovieTransfer movieViewerDebugFlag;

extern char mnuMovieViewerTaskName[];

extern MnuMovieList mnuMovieList;

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);


extern s8 dds3PadBitTable[32];
extern s8 dds3PadButtonState[64];
extern SdfMovieDescriptor mnuMovieViewerDrawDescriptor;
extern u16 D_00438A90[4];
extern u16 D_00438FF0;
extern SdfPoolNode D_003805A8;
extern SdfPoolNode D_00380708;
extern char D_0042A428[];

extern KwlnTaskUpdateResult mnuMovieViewer(dds3ProcessID_t *task);
extern void mnuStartMovieDrawTaskForResource(const char *, const SdfMovieDescriptor *);
void mnuRequestIndexedMovieResource(s32 index);

void movieStop(void);

KwlnTaskUpdateResult movieMan_Process(dds3ProcessID_t *task) {
    sdfAdvanceStreamPlayback(0x3c / dds3VblanksPerFrame);
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

void dds3StartMovieManager(void) {
    sdfIpuInit();
    dds3StartProcess("movieMan", 0x385, 1, 0, movieMan_Process, 0, 0);
}

u32 mnuScriptRequestMovieByIndex(void) {
    s32 movieIndex;

    movieIndex = scrGetIntPara(0);
    mnuRequestIndexedMovieResource(movieIndex);
    mnuMovieShutdownCounter = 0;
    return 1;
}

u32 mnuScriptStopMovieAndResetDraw(void) {
    movieStop();
    mnuMovieShutdownCounter = 0;
    dds3Filter2Off(0);
    return 1;
}

extern s8 D_0037F530[];
extern ScrCommandResult scrCommandIsProcessControlFlagClear(void);
extern void dds3SetFilter2Para(u32);
extern void dds3SetFilter2RGBA(u32);
extern void dds3Filter2On(s32);
extern s32 itfPanelReleaseHold(void);

s32 mnuUpdateMovieDrawShutdownCountdown(void) {
    if (mnuMovieShutdownCounter == 0 && scrCommandIsProcessControlFlagClear() == 1 && D_0037F530[0] < 0) {
        mnuMovieShutdownCounter = 1;
        dds3SetFilter2Para(0x44);
        dds3SetFilter2RGBA(0x80000000);
        dds3Filter2On(0x1E);
        itfPanelReleaseHold();
    }
    if (mnuMovieShutdownCounter > 0) {
        if (mnuMovieShutdownCounter == 0x1E) {
            movieStop();
            scrSetIntReturnValue(0);
            return 1;
        }
        mnuMovieShutdownCounter = mnuMovieShutdownCounter + 1;
    }
    scrSetIntReturnValue(mnuIsMoviePlaybackFinished() == SDF_MOVIE_COMPLETION_INCOMPLETE);
    return 1;
}

extern u8 mnuGetMoviePlaybackMode(void);
extern u8 mnuIsMovieDrawContextActive(void);

u8 mnuCanDrawMovie(void) {
    if (mnuGetMoviePlaybackMode() == 2) {
        if (mnuIsMovieDrawContextActive() != 0) {
            return 1;
        }
    }
    return 0;
}

void movieViewerFree(void) {
    MnuMovieListNode *node = mnuMovieList.head;
    if (node != NULL) {
        do {
            MnuMovieListNode *next = node->next;
            sdfChipFreeDirect(node);
            node = next;
        } while (node != NULL);
        mnuMovieList.head = NULL;
        mnuMovieList.top = 0;
        mnuMovieList.cursor = 0;
        mnuMovieList.total = 0;
    }
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_chip.h"
#include "mnu_movie_list.h"
#include "sce_io.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"

extern void movieViewerFree(void);
extern MnuMovieList mnuMovieList;
extern u8 sdfHddValid;

/* Builds the movie viewer's file list in debug mode: scans pfs0:/movie,
 * pfs0:/movie2 and pfs0:/movie3 for .ipu and .ips files (skipping
 * directories), appending one allocated node holding the full path per
 * file, and stores the total count. As retail, a directory that does not
 * open ends the scan there, leaving the list unterminated and the count
 * unstored. DDS1's routine scans the first two. Upstream: INCLUDE_ASM in
 * game/code_002A8048. */
void mnuBuildMovieViewerList(void) {
    SceDirent entry;
    MnuMovieListNode *node;
    MnuMovieListNode **tail;
    char *extension;
    s32 directory;
    s32 nameLength;
    s32 prefixLength;
    s32 count;

    movieViewerFree();
    if (sdfHddValid == 0) {
        return;
    }
    directory = sceDopen("pfs0:/movie");
    if (directory < 0) {
        return;
    }
    prefixLength = sizeof("pfs0:/movie") - 1;
    count = 0;
    tail = &mnuMovieList.head;
    while ((nameLength = sceDread(directory, &entry)) > 0) {
        if ((entry.stat.mode & 0xF000) == 0x1000) {
            continue;
        }
        extension = strrchr(entry.name, '.');
        if (extension == NULL) {
            continue;
        }
        if (strcasecmp(extension, ".ipu") != 0 && strcasecmp(extension, ".ips") != 0) {
            continue;
        }
        node = sdfChipAllocDirect(prefixLength + 1 + nameLength + 8);
        count++;
        memcpy(node->path, "pfs0:/movie", prefixLength);
        node->path[prefixLength] = '/';
        memcpy(node->path + prefixLength + 1, entry.name, nameLength);
        node->path[prefixLength + 1 + nameLength] = '\0';
        *tail = node;
        tail = &node->next;
    }
    sceDclose(directory);

    directory = sceDopen("pfs0:/movie2");
    if (directory < 0) {
        return;
    }
    prefixLength = sizeof("pfs0:/movie2") - 1;
    while ((nameLength = sceDread(directory, &entry)) > 0) {
        if ((entry.stat.mode & 0xF000) == 0x1000) {
            continue;
        }
        extension = strrchr(entry.name, '.');
        if (extension == NULL) {
            continue;
        }
        if (strcasecmp(extension, ".ipu") != 0 && strcasecmp(extension, ".ips") != 0) {
            continue;
        }
        node = sdfChipAllocDirect(prefixLength + 1 + nameLength + 8);
        count++;
        memcpy(node->path, "pfs0:/movie2", prefixLength);
        node->path[prefixLength] = '/';
        memcpy(node->path + prefixLength + 1, entry.name, nameLength);
        node->path[prefixLength + 1 + nameLength] = '\0';
        *tail = node;
        tail = &node->next;
    }
    sceDclose(directory);

    directory = sceDopen("pfs0:/movie3");
    if (directory < 0) {
        return;
    }
    prefixLength = sizeof("pfs0:/movie3") - 1;
    while ((nameLength = sceDread(directory, &entry)) > 0) {
        if ((entry.stat.mode & 0xF000) == 0x1000) {
            continue;
        }
        extension = strrchr(entry.name, '.');
        if (extension == NULL) {
            continue;
        }
        if (strcasecmp(extension, ".ipu") != 0 && strcasecmp(extension, ".ips") != 0) {
            continue;
        }
        node = sdfChipAllocDirect(prefixLength + 1 + nameLength + 8);
        count++;
        memcpy(node->path, "pfs0:/movie3", prefixLength);
        node->path[prefixLength] = '/';
        memcpy(node->path + prefixLength + 1, entry.name, nameLength);
        node->path[prefixLength + 1 + nameLength] = '\0';
        *tail = node;
        tail = &node->next;
    }
    *tail = NULL;
    mnuMovieList.total = count;
    sceDclose(directory);
}
#else
INCLUDE_ASM(const s32, "game/code_002A8048", mnuBuildMovieViewerList);
#endif

MnuMovieListNode *movieViewerStep(void) {
    MnuMovieListNode *entry = mnuMovieList.head;
    s32 remaining = mnuMovieList.cursor;
    if (entry != 0 && remaining > 0) {
        do {
            entry = entry->next;
            remaining--;
        } while (entry != 0 && remaining > 0);
    }
    return entry;
}

INCLUDE_SDATA(const s32, "game/code_002A8048", mnuMovieShutdownCounter);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF0);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF8);

void movieDebugDrawInfo(void) {
    MnuMovieListNode *node;
    sdfDrawTag_t *packets;
    s32 selected;
    s32 i;

    if (mnuMovieList.head == NULL || mnuMovieList.playing != 0) {
        return;
    }
    packets = mnuMovieList.packets;
    sdfDrawTagLink(packets, (u32)dds3PrimFrameRect(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000, 0x60404040));
    selected = mnuMovieList.cursor;
    i = mnuMovieList.top;
    node = mnuMovieList.head;
    selected -= i;
    for (; i > 0; i--) {
        node = node->next;
    }
    for (i = 0; i < 8 && node != NULL; i++, node = node->next) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, "%c%s", (i == selected) ? '>' : ' ', node->path));
    }
    if (mnuMovieList.top != 0) {
        sdfPrimLinkF3(packets, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8, 0xFF0080, 0);
    }
    if (node != NULL) {
        sdfPrimLinkF3(packets, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08, 0xFF0080, 0);
    }
}



void MovieViewerDrawFrameInfo(void) {
    sdfDrawTag_t *list;
    if (mnuIsMoviePlaybackFinished() == SDF_MOVIE_COMPLETION_INCOMPLETE) {
        list = mnuMovieList.packets;
        sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90, 0x30000000, 0x60404040));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d", mnuMovieDrawContext.soundNode.playbackFrameIndex, (s32)mnuMovieDrawContext.soundNode.cycleLength));
    }
}

KwlnTaskUpdateResult mnuMovieViewer(dds3ProcessID_t *task) {
    MnuMovieListNode *node;
    s32 nextTop;

    if (dds3PadButtonState[0x2D] != 0) {
        mnuMovieDrawContext.soundNode.playbackCadenceStep = 1;
    } else {
        mnuMovieDrawContext.soundNode.playbackCadenceStep = 0x1E;
    }

    if (dds3PadButtonState[0x28] < 0) {
        mnuMovieViewerDrawDescriptor.drawX = 0x7000;
        mnuMovieViewerDrawDescriptor.drawY = 0x7900;
        mnuMovieViewerDrawDescriptor.drawWidth = 0x2000;
        mnuMovieViewerDrawDescriptor.drawHeight = 0x0E00;
    }
    if (dds3PadButtonState[0x2A] < 0) {
        mnuMovieViewerDrawDescriptor.drawX = 0x7000;
        mnuMovieViewerDrawDescriptor.drawY = 0x7B00;
        mnuMovieViewerDrawDescriptor.drawWidth = 0x2000;
        mnuMovieViewerDrawDescriptor.drawHeight = 0x0A00;
    }

    if (mnuMovieList.playing != 0) {
        if (mnuIsMoviePlaybackFinished() != SDF_MOVIE_COMPLETION_INCOMPLETE) {
            mnuMovieList.playing = 0;
        } else {
            s32 previousButtons = D_00438FF0;
            if (((D_00438A90[0] ^ previousButtons) & D_00438A90[0]) != 0) {
                mnuMovieList.playing = 0;
            }
        }
    } else if (dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x28] < 0 || dds3PadButtonState[0x2A] < 0) {
        node = movieViewerStep();
        if (node != NULL) {
            movieStop();
            mnuStartMovieDrawTaskForResource(node->path, &mnuMovieViewerDrawDescriptor);
            mnuMovieList.playing = 1;
        } else {
            movieStop();
            mnuRequestIndexedMovieResource(0x30);
        }
    } else if (((u8)dds3PadButtonState[0x27] & 2) != 0 && mnuMovieList.cursor < mnuMovieList.total - 1) {
        mnuMovieList.cursor++;
        nextTop = mnuMovieList.cursor - 7;
        if (mnuMovieList.top < nextTop) {
            mnuMovieList.top = nextTop;
        }
    } else if (((u8)dds3PadButtonState[0x26] & 2) != 0 && mnuMovieList.cursor > 0) {
        mnuMovieList.cursor--;
        if (mnuMovieList.cursor < mnuMovieList.top) {
            mnuMovieList.top = mnuMovieList.cursor;
        }
    }

    mnuMovieList.packets = (sdfDrawTag_t *)sdfPrimAllocDrawTag(0);
    movieDebugDrawInfo();
    MovieViewerDrawFrameInfo();
    D_003805A8.append(&D_003805A8, mnuMovieList.packets);
    D_00438FF0 = D_00438A90[0];
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

void InitMovieViewer(void) {
    mnuBuildMovieViewerList();
    mnuMovieList.task = dds3StartProcess(mnuMovieViewerTaskName, 0x2b02, 1, 0,
                                       mnuMovieViewer, 0, 0);
}

void FinishMovieViewer(void) {
    dds3ProcessID_t *movieTask = dds3SearchProcessName(mnuMovieViewerTaskName);
    if (movieTask != NULL) {
        dds3KillProcess(movieTask, 0);
        mnuMovieList.task = 0;
        movieStop();
    }
    movieViewerFree();
}

void movieViewerSetFlags(void) {
    movieViewerDebugFlag.wordPending = 1;
    movieViewerDebugFlag.blockPending = 1;
}

void movieViewerInitRegs(void) {
    movieViewerDebugFlag.wordSource = 0x10002010;
    movieViewerDebugFlag.blockSource = (u32)&mnuMovieDrawContext;
    movieViewerSetFlags();
}

/* Copy a source word and a 0x40-byte block when their pending flags are set. */
void movieViewerDrawInfo(void) {
    if (movieViewerDebugFlag.wordPending != 0) {
        movieViewerDebugFlag.word = *(s32 *)movieViewerDebugFlag.wordSource;
    }
    if (movieViewerDebugFlag.blockPending != 0) {
        memcpy(movieViewerDebugFlag.block, (void *)movieViewerDebugFlag.blockSource, sizeof(movieViewerDebugFlag.block));
    }
}

s32 movieViewerDebugDraw(void) {
    sdfDrawTag_t *packets;
    s32 n;
    s32 i;
    s32 x;
    s32 y;
    s32 col;

    if (movieViewerDebugFlag.started == 0) {
        movieViewerDebugFlag.started = 1;
        movieViewerInitRegs();
    }
    if (dds3PadBitTable[0x11] != 0) {
        movieViewerSetFlags();
    } else if (dds3PadBitTable[0x12] != 0) {
        movieViewerInitRegs();
    } else if (dds3PadBitTable[0x15] & 2) {
        movieViewerDebugFlag.cursor++;
        if (movieViewerDebugFlag.cursor == 0x10) {
            movieViewerDebugFlag.cursor = 0;
        }
    } else if (dds3PadBitTable[0x14] & 2) {
        if (movieViewerDebugFlag.cursor != 0) {
            movieViewerDebugFlag.cursor--;
        } else {
            movieViewerDebugFlag.cursor = 0xF;
        }
    } else {
        n = 1 << ((~movieViewerDebugFlag.cursor & 7) * 4);
        i = movieViewerDebugFlag.cursor >> 3;
        if (dds3PadBitTable[0x17] & 2) {
            if (i == 0) {
                movieViewerDebugFlag.wordPending = 0;
                movieViewerDebugFlag.wordSource += n;
            } else {
                movieViewerDebugFlag.blockPending = 0;
                movieViewerDebugFlag.blockSource += n;
            }
        }
        if (dds3PadBitTable[0x16] & 2) {
            if (i == 0) {
                movieViewerDebugFlag.wordPending = 0;
                movieViewerDebugFlag.wordSource -= n;
            } else {
                movieViewerDebugFlag.blockPending = 0;
                movieViewerDebugFlag.blockSource -= n;
            }
        }
    }
    movieViewerDrawInfo();
    packets = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(packets, (u32)dds3PrimFrameRect(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000, 0x40806020));
    sdfPrimLinkRect(packets, 0x80A03000, 0, (movieViewerDebugFlag.cursor & 7) * 0xC0 + 0x7180, (movieViewerDebugFlag.cursor >> 3) * 0xC0 + 0x79C0, (movieViewerDebugFlag.cursor & 7) * 0xC0 + 0x7240, (movieViewerDebugFlag.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7180, 0x79C0, 0xFF0080, 0, "%08X", movieViewerDebugFlag.wordSource));
    if (movieViewerDebugFlag.wordPending != 0) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, "%08X", movieViewerDebugFlag.word));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, D_0042A428));
    }
    n = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7180, y, 0xFF0080, 0, "%08X", movieViewerDebugFlag.blockSource + n));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, n++) {
            if (movieViewerDebugFlag.blockPending != 0) {
                sdfDrawTagLink(packets, (u32)sdfDevPrintf(x, y, 0xFF0080, 0, "%02X", movieViewerDebugFlag.block[n]));
            } else {
                sdfDrawTagLink(packets, (u32)sdfDevPrintf(x, y, 0xFF0080, 0, "**"));
            }
        }
    }
    D_00380708.append(&D_00380708, packets);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002A8048", mnuMovieViewerTaskName);

INCLUDE_RODATA(const s32, "game/code_002A8048", D_0042A428);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B20);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B28);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B30);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B38);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B40);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B48);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B50);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B58);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B60);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B68);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B6C);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B6E);

INCLUDE_SDATA(const s32, "game/code_002A8048", campExitState);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B73);

