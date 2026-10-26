#include "common.h"
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
#include "sce_io.h"
#include "btl.h"
/* Declared before first use (were implicit int). */
extern void dds3Filter2Off(s32 duration);
extern s32 scrGetIntPara(s32 parameterIndex);
extern void sdfIpuInit(void);

extern movieStream_t mnuMovieDrawContext;

extern char D_003B1168[]; /* "staffProc" */

extern MnuMovieTransfer movieViewerDebugFlag;


extern s8 dds3PadBitTable[32];

extern SdfPoolNode D_00325708;

extern char D_003B1AD8[];

extern char D_003BC648[];

extern char D_003BC650[];

extern char D_003BC658[];

extern char D_003BC660[];



extern s32 mnuMovieShutdownCounter;

extern u16 dds3VblanksPerFrame;

extern char mnuMovieViewerTaskName[];

extern MnuMovieList mnuMovieList;

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);


extern KwlnTaskUpdateResult mnuMovieViewer(dds3ProcessID_t *task);

extern char D_003B1A78[]; /* "mnuMovieDraw" */

extern u16 dds3VblanksPerFrame;

extern char D_003B1168[];

void dds3PlayMovieByIndex(s32 index);

s32 mnuIsMoviePlaybackFinished(void);

u8 mnuGetMoviePlaybackMode(void);

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
    dds3PlayMovieByIndex(movieIndex);
    mnuMovieShutdownCounter = 0;
    return 1;
}

u32 mnuScriptStopMovieAndResetDraw(void) {
    movieStop();
    mnuMovieShutdownCounter = 0;
    dds3Filter2Off(0);
    return 1;
}

extern void dds3SetFilter2RGBA(u32);

extern void dds3SetFilter2Para(u32);

extern void dds3Filter2On(s32);

extern ScrCommandResult scrCommandIsProcessControlFlagClear(void);

extern s32 itfPanelReleaseHold(void);


extern s8 D_00324530[];

s32 mnuUpdateMovieDrawShutdownCountdown(void) {
    if (mnuMovieShutdownCounter == 0 && scrCommandIsProcessControlFlagClear() == 1 && D_00324530[0] < 0) {
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
    scrSetIntReturnValue(mnuIsMoviePlaybackFinished() == 0);
    return 1;
}

u8 mnuCanDrawMovie(void) {
    u8 movieState;

    movieState = mnuGetMoviePlaybackMode();
    return movieState == 2;
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

INCLUDE_ASM(const s32, "game/code_00270098", mnuBuildMovieViewerList);

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
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, D_003BC648, (i == selected) ? '>' : ' ', node->path));
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
    if (mnuIsMoviePlaybackFinished() == 0) {
        list = mnuMovieList.packets;
        sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90, 0x30000000, 0x60404040));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d", mnuMovieDrawContext.soundNode.playbackFrameIndex, (s32)mnuMovieDrawContext.soundNode.cycleLength));
    }
}

extern u8 dds3PadButtonState[2][2][16];
extern SdfMovieDescriptor mnuMovieViewerDrawDescriptor;
extern u16 D_003BD3A0[4];
extern u16 D_003BD8E4;
extern SdfPoolNode D_003255A8;
extern void mnuStartMovieDrawTaskForResource(const char *fileName, SdfMovieDescriptor *descriptor);

KwlnTaskUpdateResult mnuMovieViewer(dds3ProcessID_t *task) {
    MnuMovieListNode *node;
    s32 nextTop;

    if ((s8)dds3PadButtonState[1][0][0x0D] != 0) {
        mnuMovieDrawContext.soundNode.playbackCadenceStep = 1;
    } else {
        mnuMovieDrawContext.soundNode.playbackCadenceStep = 0x1E;
    }

    if ((s8)dds3PadButtonState[1][0][0x08] < 0) {
        mnuMovieViewerDrawDescriptor.drawX = 0x7000;
        mnuMovieViewerDrawDescriptor.drawY = 0x7900;
        mnuMovieViewerDrawDescriptor.drawWidth = 0x2000;
        mnuMovieViewerDrawDescriptor.drawHeight = 0x0E00;
    }
    if ((s8)dds3PadButtonState[1][0][0x0A] < 0) {
        mnuMovieViewerDrawDescriptor.drawX = 0x7000;
        mnuMovieViewerDrawDescriptor.drawY = 0x7B00;
        mnuMovieViewerDrawDescriptor.drawWidth = 0x2000;
        mnuMovieViewerDrawDescriptor.drawHeight = 0x0A00;
    }

    if (mnuMovieList.playing != 0) {
        if (mnuIsMoviePlaybackFinished() != 0) {
            mnuMovieList.playing = 0;
        } else {
            s32 previousButtons = D_003BD8E4;
            if (((D_003BD3A0[0] ^ previousButtons) & D_003BD3A0[0]) != 0) {
                mnuMovieList.playing = 0;
            }
        }
    } else if ((s8)dds3PadButtonState[1][0][0x01] < 0 || (s8)dds3PadButtonState[1][0][0x08] < 0 || (s8)dds3PadButtonState[1][0][0x0A] < 0) {
        node = movieViewerStep();
        if (node != NULL) {
            movieStop();
            mnuStartMovieDrawTaskForResource(node->path, &mnuMovieViewerDrawDescriptor);
            mnuMovieList.playing = 1;
        } else {
            movieStop();
            dds3PlayMovieByIndex(0);
        }
    } else if (((u8)dds3PadButtonState[1][0][0x07] & 2) != 0 && mnuMovieList.cursor < mnuMovieList.total - 1) {
        mnuMovieList.cursor++;
        nextTop = mnuMovieList.cursor - 7;
        if (mnuMovieList.top < nextTop) {
            mnuMovieList.top = nextTop;
        }
    } else if (((u8)dds3PadButtonState[1][0][0x06] & 2) != 0 && mnuMovieList.cursor > 0) {
        mnuMovieList.cursor--;
        if (mnuMovieList.cursor < mnuMovieList.top) {
            mnuMovieList.top = mnuMovieList.cursor;
        }
    }

    mnuMovieList.packets = (sdfDrawTag_t *)sdfPrimAllocDrawTag(0);
    movieDebugDrawInfo();
    MovieViewerDrawFrameInfo();
    D_003255A8.append(&D_003255A8, mnuMovieList.packets);
    D_003BD8E4 = D_003BD3A0[0];
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

void InitMovieViewer(void) {
    mnuBuildMovieViewerList();
    mnuMovieList.task = dds3StartProcess(mnuMovieViewerTaskName, 0x2b02, 1, 0,
                                       mnuMovieViewer, 0, 0);
}

void FinishMovieViewer(void) {
    dds3ProcessID_t *task = dds3SearchProcessName(mnuMovieViewerTaskName);
    if (task != NULL) {
        dds3KillProcess(task, 0);
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
        memcpy(movieViewerDebugFlag.block, (void *)movieViewerDebugFlag.blockSource, 0x40);
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
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7180, 0x79C0, 0xFF0080, 0, D_003BC650, movieViewerDebugFlag.wordSource));
    if (movieViewerDebugFlag.wordPending != 0) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, D_003BC650, movieViewerDebugFlag.word));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, D_003B1AD8));
    }
    n = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x7180, y, 0xFF0080, 0, D_003BC650, movieViewerDebugFlag.blockSource + n));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, n++) {
            if (movieViewerDebugFlag.blockPending != 0) {
                sdfDrawTagLink(packets, (u32)sdfDevPrintf(x, y, 0xFF0080, 0, D_003BC658, movieViewerDebugFlag.block[n]));
            } else {
                sdfDrawTagLink(packets, (u32)sdfDevPrintf(x, y, 0xFF0080, 0, D_003BC660));
            }
        }
    }
    D_00325708.append(&D_00325708, packets);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00270098", mnuMovieViewerTaskName);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1AD8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1AF0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B00);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B10);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B20);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B30);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B48);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B60);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B78);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1B88);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1BA0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1BB8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1BC8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1BE0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1BF8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1C10);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1C28);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1C38);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1C48);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1C60);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1C78);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1C90);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1CA8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1CB8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1CC8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1CD8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1CE8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1CF8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D08);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D28);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D38);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D48);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D58);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D68);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D78);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D88);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1D98);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1DA8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1DB8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1DC8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1DD8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1DE8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1DF8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E08);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E18);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E28);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E48);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E58);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E68);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E78);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E88);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1E98);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1EA8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1EB8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1EC8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1EE0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F00);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F18);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F30);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F48);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F60);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F70);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F80);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1F90);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1FA0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1FB0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1FC0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1FD0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1FE0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1FF0);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B2000);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B2010);

INCLUDE_SDATA(const s32, "game/code_00270098", mnuMovieShutdownCounter);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC638);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC640);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC648);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC650);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC658);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC660);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC668);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC670);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC678);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC680);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC688);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC690);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC698);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC6A0);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC6A8);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC6B0);

