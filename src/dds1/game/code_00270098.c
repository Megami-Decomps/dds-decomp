#include "common.h"

extern u8 mnuMovieDrawContext[];

extern char D_003B1168[]; /* "staffProc" */

/* State of the debug viewer: an IPU register word and a 0x40-byte block, edited nibble by nibble. */
typedef struct MnuMovieTransfer {
    u8 started;          /* 0x00 */
    u8 cursor;           /* 0x01: nibble being edited, 0..15 */
    u8 wordPending;      /* 0x02 */
    u8 blockPending;     /* 0x03 */
    u32 wordSource;      /* 0x04 */
    u32 blockSource;     /* 0x08 */
    s32 word;            /* 0x0C */
    u8 block[0x40];      /* 0x10 */
} MnuMovieTransfer;

extern MnuMovieTransfer mnuMovieDrawSources;

typedef struct MnuPacketDev {
    u8 pad00[0x10];
    void (*submitPacket)(void *, s32);
} MnuPacketDev;

typedef struct MnuViewerPad {
    u8 pad00[0x11];
    s8 reset;       /* 0x11 */
    s8 init;        /* 0x12 */
    u8 pad13;
    u8 prev;        /* 0x14 */
    u8 next;        /* 0x15 */
    u8 left;        /* 0x16 */
    u8 right;       /* 0x17 */
} MnuViewerPad;

extern MnuViewerPad sdfPadButtonStates;

extern MnuPacketDev D_00325708;

extern char D_003B1AD8[];

extern char D_003BC648[];

extern char D_003BC650[];

extern char D_003BC658[];

extern char D_003BC660[];

extern s32 sdfCreateResetPacketList(void);

extern void sdfCreatePacketA(s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 mnuMovieShutdownCounter;

extern u16 mnuMovieTaskState;

extern char mnuMovieViewerTaskName[];

typedef struct MovieListNode {
    struct MovieListNode *next;
    char path[4];
} MovieListNode;

typedef struct MovieListState {
    u32 task;
    MovieListNode *head;
    s16 top;       /* first visible entry */
    s16 cursor;
    s16 total;
    s8 playing;
    u8 padF;
    s32 packets;
} MovieListState;

extern MovieListState mnuMovieList;

extern s32 func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);

extern s32 sdfCreateFormattedSifCommand(s32, s32, s32, s32, char *, ...);

extern void sdfAppendPacket(s32, s32);

extern void sdfQueueFlatTriangle(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 mnuMovieViewer();

extern char D_003B1A78[]; /* "mnuMovieDraw" */

extern u16 mnuMovieTaskState;

extern char D_003B1168[];

void mnuRequestIndexedMovieResource(s32 index);

s32 mnuCheckMovieDecoderStatus(void);

s32 func_00270088(void);

void mnuStopMovieDrawTask(void);

s32 mnuSetFrameDivisor(void) {
    func_002EC5E0(0x3c / mnuMovieTaskState);
    return 0;
}

void mnuCreateMovieManagerTask(void) {
    sdfSoundInitIpuStream();
    kwlnTaskCreate("movieMan", 0x385, 1, 0, mnuSetFrameDivisor, 0, 0);
}

u32 mnuScriptRequestMovieByIndex(void) {
    s32 movieIndex;

    movieIndex = scrReadIntParameter(0);
    mnuRequestIndexedMovieResource(movieIndex);
    mnuMovieShutdownCounter = 0;
    return 1;
}

u32 mnuScriptStopMovieAndResetDraw(void) {
    mnuStopMovieDrawTask();
    mnuMovieShutdownCounter = 0;
    kwlnDrawEnableDc8(0);
    return 1;
}

extern void kwlnDrawSetDc8First(u32);

extern void kwlnDrawSetDc8Second(u32);

extern void kwlnDrawSetupDc8(s32);

extern s32 scrCommandIsProcessControlFlagClear(void);

extern s32 itfPanelReleaseHold(void);

extern void scrSetIntegerReturnValue(s32);

extern s8 D_00324530[];

s32 mnuUpdateMovieDrawShutdownCountdown(void) {
    if (mnuMovieShutdownCounter == 0 && scrCommandIsProcessControlFlagClear() == 1 && D_00324530[0] < 0) {
        mnuMovieShutdownCounter = 1;
        kwlnDrawSetDc8Second(0x44);
        kwlnDrawSetDc8First(0x80000000);
        kwlnDrawSetupDc8(0x1E);
        itfPanelReleaseHold();
    }
    if (mnuMovieShutdownCounter > 0) {
        if (mnuMovieShutdownCounter == 0x1E) {
            mnuStopMovieDrawTask();
            scrSetIntegerReturnValue(0);
            return 1;
        }
        mnuMovieShutdownCounter = mnuMovieShutdownCounter + 1;
    }
    scrSetIntegerReturnValue(mnuCheckMovieDecoderStatus() == 0);
    return 1;
}

u8 func_00270218(void) {
    s64 movieState;

    movieState = func_00270088();
    return movieState == 2;
}

void mnuClearMovieList(void) {
    MovieListNode *node = mnuMovieList.head;

    if (node != NULL) {
        do {
            MovieListNode *next = node->next;

            sdfReleaseChipBlock(node);
            node = next;
        } while (node != NULL);
        mnuMovieList.head = NULL;
        mnuMovieList.top = 0;
        mnuMovieList.cursor = 0;
        mnuMovieList.total = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00270098", func_002702A0);

u32 mnuGetMovieListNodeAtOffset(void) {
    MovieListNode *entry = mnuMovieList.head;
    s32 remaining = mnuMovieList.cursor;
    if (entry != 0 && remaining > 0) {
        do {
            entry = entry->next;
            remaining--;
        } while (entry != 0 && remaining > 0);
    }
    return (u32)entry;
}

void mnuDrawMovieList(void) {
    MovieListNode *node;
    s32 packets;
    s32 selected;
    s32 i;

    if (mnuMovieList.head == NULL || mnuMovieList.playing != 0) {
        return;
    }
    packets = mnuMovieList.packets;
    sdfAppendPacket(packets, func_0011D3E8(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000, 0x60404040));
    selected = mnuMovieList.cursor;
    i = mnuMovieList.top;
    node = mnuMovieList.head;
    selected -= i;
    for (; i > 0; i--) {
        node = node->next;
    }
    for (i = 0; i < 8 && node != NULL; i++, node = node->next) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, D_003BC648, (i == selected) ? '>' : ' ', node->path));
    }
    if (mnuMovieList.top != 0) {
        sdfQueueFlatTriangle(packets, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8, 0xFF0080, 0);
    }
    if (node != NULL) {
        sdfQueueFlatTriangle(packets, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08, 0xFF0080, 0);
    }
}

typedef struct MovieStatus {
    u8 pad00[0x64];
    s32 total;
    s32 pad68;
    s32 current;
} MovieStatus;

extern s32 D_003DC570[];

void mnuDrawMovieProgressCounter(void) {
    s32 list;
    if (mnuCheckMovieDecoderStatus() == 0) {
        list = D_003DC570[0];
        sdfAppendPacket(list, func_0011D3E8(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90, 0x30000000, 0x60404040));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d", ((MovieStatus *)mnuMovieDrawContext)->current, ((MovieStatus *)mnuMovieDrawContext)->total));
    }
}

INCLUDE_ASM(const s32, "game/code_00270098", mnuMovieViewer);

void mnuCreateMovieViewerTask(void) {
    func_002702A0();
    mnuMovieList.task = kwlnTaskCreate(mnuMovieViewerTaskName, 0x2b02, 1, 0, mnuMovieViewer, 0, 0);
}

void mnuDestroyMovieViewerTask(void) {
    s32 task = kwlnTaskGetTaskByName(mnuMovieViewerTaskName);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
        mnuMovieList.task = 0;
        mnuStopMovieDrawTask();
    }
    mnuClearMovieList();
}

void mnuMarkMovieDrawValuesPending(void) {
    mnuMovieDrawSources.wordPending = 1;
    mnuMovieDrawSources.blockPending = 1;
}

void mnuBindMovieDrawValueSources(void) {
    mnuMovieDrawSources.wordSource = 0x10002010;
    mnuMovieDrawSources.blockSource = (u32)mnuMovieDrawContext;
    mnuMarkMovieDrawValuesPending();
}

/* Copy a source word and a 0x40-byte block when their pending flags are set. */
void mnuCommitPendingMovieDrawValues(void) {
    if (mnuMovieDrawSources.wordPending != 0) {
        mnuMovieDrawSources.word = *(s32 *)mnuMovieDrawSources.wordSource;
    }
    if (mnuMovieDrawSources.blockPending != 0) {
        memcpy(mnuMovieDrawSources.block, (void *)mnuMovieDrawSources.blockSource, 0x40);
    }
}

s32 mnuUpdateIpuRegisterViewer(void) {
    s32 packets;
    s32 n;
    s32 i;
    s32 x;
    s32 y;
    s32 col;

    if (mnuMovieDrawSources.started == 0) {
        mnuMovieDrawSources.started = 1;
        mnuBindMovieDrawValueSources();
    }
    if (sdfPadButtonStates.reset != 0) {
        mnuMarkMovieDrawValuesPending();
    } else if (sdfPadButtonStates.init != 0) {
        mnuBindMovieDrawValueSources();
    } else if (sdfPadButtonStates.next & 2) {
        mnuMovieDrawSources.cursor++;
        if (mnuMovieDrawSources.cursor == 0x10) {
            mnuMovieDrawSources.cursor = 0;
        }
    } else if (sdfPadButtonStates.prev & 2) {
        if (mnuMovieDrawSources.cursor != 0) {
            mnuMovieDrawSources.cursor--;
        } else {
            mnuMovieDrawSources.cursor = 0xF;
        }
    } else {
        n = 1 << ((~mnuMovieDrawSources.cursor & 7) * 4);
        i = mnuMovieDrawSources.cursor >> 3;
        if (sdfPadButtonStates.right & 2) {
            if (i == 0) {
                mnuMovieDrawSources.wordPending = 0;
                mnuMovieDrawSources.wordSource += n;
            } else {
                mnuMovieDrawSources.blockPending = 0;
                mnuMovieDrawSources.blockSource += n;
            }
        }
        if (sdfPadButtonStates.left & 2) {
            if (i == 0) {
                mnuMovieDrawSources.wordPending = 0;
                mnuMovieDrawSources.wordSource -= n;
            } else {
                mnuMovieDrawSources.blockPending = 0;
                mnuMovieDrawSources.blockSource -= n;
            }
        }
    }
    mnuCommitPendingMovieDrawValues();
    packets = sdfCreateResetPacketList();
    sdfAppendPacket(packets, func_0011D3E8(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000, 0x40806020));
    sdfCreatePacketA(packets, 0x80A03000, 0, (mnuMovieDrawSources.cursor & 7) * 0xC0 + 0x7180, (mnuMovieDrawSources.cursor >> 3) * 0xC0 + 0x79C0, (mnuMovieDrawSources.cursor & 7) * 0xC0 + 0x7240, (mnuMovieDrawSources.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7180, 0x79C0, 0xFF0080, 0, D_003BC650, mnuMovieDrawSources.wordSource));
    if (mnuMovieDrawSources.wordPending != 0) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x79C0, 0xFF0080, 0, D_003BC650, mnuMovieDrawSources.word));
    } else {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x79C0, 0xFF0080, 0, D_003B1AD8));
    }
    n = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7180, y, 0xFF0080, 0, D_003BC650, mnuMovieDrawSources.blockSource + n));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, n++) {
            if (mnuMovieDrawSources.blockPending != 0) {
                sdfAppendPacket(packets, sdfCreateFormattedSifCommand(x, y, 0xFF0080, 0, D_003BC658, mnuMovieDrawSources.block[n]));
            } else {
                sdfAppendPacket(packets, sdfCreateFormattedSifCommand(x, y, 0xFF0080, 0, D_003BC660));
            }
        }
    }
    D_00325708.submitPacket(&D_00325708, packets);
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

