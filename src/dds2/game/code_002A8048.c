#include "common.h"
#include "sdf.h"

extern s32 scrReadIntParameter(s32);

extern u16 mnuMovieTaskState;

extern s32 mnuMovieShutdownCounter;

extern u8 mnuMovieDrawContext[];

/* State of the debug viewer: an IPU register word and a 0x40-byte block, edited nibble by nibble. */
typedef struct MnuMovieTransfer {
    u8 started;          /* 0x00 */
    u8 cursor;           /* 0x01: nibble being edited, 0..15 */
    u8 wordPending;      /* 0x02 */
    u8 blockPending;     /* 0x03 */
    u32 wordSource;      /* 0x04: register address, dereferenced as s32 when copying word */
    u32 blockSource;     /* 0x08 */
    s32 word;            /* 0x0C */
    u8 block[0x40];      /* 0x10 */
} MnuMovieTransfer;

extern MnuMovieTransfer mnuMovieDrawSources;

extern char mnuMovieViewerTaskName[];

typedef struct MovieListNode {
    struct MovieListNode *next;
    char path[4];
} MovieListNode;

typedef struct MovieList {
    u32 task;
    MovieListNode *head;
    s16 top;       /* first visible entry */
    s16 cursor;
    s16 total;
    s8 playing;
    u8 padF;
    s32 packets;
} MovieList;

extern MovieList mnuMovieList;

extern s32 func_0011F250(s32, s32, s32, s32, s32, s32, s32);

extern s32 sdfCreateFormattedSifCommand(s32, s32, s32, s32, char *, ...);

extern void sdfAppendPacket(SdfListHead *, u32);

extern void sdfQueueFlatTriangle(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 (*)(s32));


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
extern SdfPoolNode D_00380708;
extern char D_0042A428[];
extern s32 sdfCreateResetPacketList(void);
extern void sdfCreatePacketA(SdfListHead *, s32, s32, s32, s32, s32, s32, s32, s32 (*)(s32));

extern s32 mnuMovieViewer();

void mnuRequestIndexedMovieResource(s32 index);

void mnuStopMovieDrawTask(void);

s32 mnuCheckMovieDecoderStatus(void);

s32 mnuSetFrameDivisor(void) {
    func_00345488(0x3c / mnuMovieTaskState);
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

extern s8 D_0037F530[];
extern s32 scrCommandIsProcessControlFlagClear(void);
extern void kwlnDrawSetDc8Second(u32);
extern void kwlnDrawSetDc8First(u32);
extern void kwlnDrawSetupDc8(s32);
extern s32 itfPanelReleaseHold(void);
extern void scrSetIntegerReturnValue(s32);

s32 mnuUpdateMovieDrawShutdownCountdown(void) {
    if (mnuMovieShutdownCounter == 0 && scrCommandIsProcessControlFlagClear() == 1 && D_0037F530[0] < 0) {
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

extern u8 func_002A8028(void);
extern u8 func_002A8038(void);

s32 func_002A81C8(void) {
    if (func_002A8028() == 2) {
        if (func_002A8038() != 0) {
            return 1;
        }
    }
    return 0;
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

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A8268);

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

INCLUDE_SDATA(const s32, "game/code_002A8048", mnuMovieShutdownCounter);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF0);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF8);

void mnuDrawMovieList(void) {
    MovieListNode *node;
    SdfListHead *packets;
    s32 selected;
    s32 i;

    if (mnuMovieList.head == NULL || mnuMovieList.playing != 0) {
        return;
    }
    packets = (SdfListHead *)mnuMovieList.packets;
    sdfAppendPacket(packets, func_0011F250(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000, 0x60404040));
    selected = mnuMovieList.cursor;
    i = mnuMovieList.top;
    node = mnuMovieList.head;
    selected -= i;
    for (; i > 0; i--) {
        node = node->next;
    }
    for (i = 0; i < 8 && node != NULL; i++, node = node->next) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, "%c%s", (i == selected) ? '>' : ' ', node->path));
    }
    if (mnuMovieList.top != 0) {
        sdfQueueFlatTriangle((s32)packets, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8, 0xFF0080, 0);
    }
    if (node != NULL) {
        sdfQueueFlatTriangle((s32)packets, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08, 0xFF0080, 0);
    }
}

typedef struct MovieStatus {
    u8 pad00[0x64];
    s32 total;
    s32 pad68;
    s32 current;
} MovieStatus;

extern s32 D_00457E58[];

void mnuDrawMovieProgressCounter(void) {
    SdfListHead *list;
    if (mnuCheckMovieDecoderStatus() == 0) {
        list = (SdfListHead *)D_00457E58[0];
        sdfAppendPacket(list, func_0011F250(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90, 0x30000000, 0x60404040));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d", ((MovieStatus *)mnuMovieDrawContext)->current, ((MovieStatus *)mnuMovieDrawContext)->total));
    }
}

INCLUDE_ASM(const s32, "game/code_002A8048", mnuMovieViewer);

void mnuCreateMovieViewerTask(void) {
    func_002A8268();
    mnuMovieList.task = kwlnTaskCreate(mnuMovieViewerTaskName, 0x2b02, 1, 0, mnuMovieViewer, 0, 0);
}

void mnuDestroyMovieViewerTask(void) {
    s32 movieTask = kwlnTaskGetTaskByName(mnuMovieViewerTaskName);
    if (movieTask != 0) {
        kwlnTaskDestroyWithHierarchy(movieTask, 0);
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
        memcpy(mnuMovieDrawSources.block, (void *)mnuMovieDrawSources.blockSource, sizeof(mnuMovieDrawSources.block));
    }
}

s32 mnuUpdateIpuRegisterViewer(void) {
    SdfListHead *packets;
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
    packets = (SdfListHead *)sdfCreateResetPacketList();
    sdfAppendPacket(packets, func_0011F250(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000, 0x40806020));
    sdfCreatePacketA(packets, 0x80A03000, 0, (mnuMovieDrawSources.cursor & 7) * 0xC0 + 0x7180, (mnuMovieDrawSources.cursor >> 3) * 0xC0 + 0x79C0, (mnuMovieDrawSources.cursor & 7) * 0xC0 + 0x7240, (mnuMovieDrawSources.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7180, 0x79C0, 0xFF0080, 0, "%08X", mnuMovieDrawSources.wordSource));
    if (mnuMovieDrawSources.wordPending != 0) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x79C0, 0xFF0080, 0, "%08X", mnuMovieDrawSources.word));
    } else {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x79C0, 0xFF0080, 0, D_0042A428));
    }
    n = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7180, y, 0xFF0080, 0, "%08X", mnuMovieDrawSources.blockSource + n));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, n++) {
            if (mnuMovieDrawSources.blockPending != 0) {
                sdfAppendPacket(packets, sdfCreateFormattedSifCommand(x, y, 0xFF0080, 0, "%02X", mnuMovieDrawSources.block[n]));
            } else {
                sdfAppendPacket(packets, sdfCreateFormattedSifCommand(x, y, 0xFF0080, 0, "**"));
            }
        }
    }
    D_00380708.append((SdfListHead *)&D_00380708, packets);
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

INCLUDE_SDATA(const s32, "game/code_002A8048", mnuCampTaskState);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B73);

