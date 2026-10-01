#include "common.h"

extern u8 D_0037B888[];

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

extern MnuMovieTransfer D_003DC578;

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

extern MnuViewerPad D_00398628;

extern MnuPacketDev D_00325708;

extern char D_003B1AD8[];

extern char D_003BC648[];

extern char D_003BC650[];

extern char D_003BC658[];

extern char D_003BC660[];

extern s32 sdfCreateResetPacketList(void);

extern void sdfCreatePacketA(s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 D_003BC630;

extern u16 D_003BA72C;

extern char D_003B1AC8[];

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

extern MovieListState D_003DC560;

extern s32 func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);

extern s32 sdfCreateFormattedSifCommand(s32, s32, s32, s32, char *, ...);

extern void sdfAppendPacket(s32, s32);

extern void func_002D6080(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 mnuMovieViewer();

extern char D_003B1A78[]; /* "mnuMovieDraw" */

extern u16 D_003BA72C;

extern char D_003B1168[];

void func_0026FFF8(s32 index);

s32 func_00270068(void);

s32 func_00270088(void);

void mnuStopMovieDrawTask(void);

s32 mnuSetFrameDivisor(void) {
    func_002EC5E0(0x3c / D_003BA72C);
    return 0;
}

void mnuCreateMovieManagerTask(void) {
    sdfSoundInitIpuStream();
    kwlnTaskCreate("movieMan", 0x385, 1, 0, mnuSetFrameDivisor, 0, 0);
}

u32 mnuScriptRequestMovieByIndex(void) {
    s32 movieIndex;

    movieIndex = scrReadIntParameter(0);
    func_0026FFF8(movieIndex);
    D_003BC630 = 0;
    return 1;
}

u32 mnuScriptStopMovieAndResetDraw(void) {
    mnuStopMovieDrawTask();
    D_003BC630 = 0;
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

s32 func_00270170(void) {
    if (D_003BC630 == 0 && scrCommandIsProcessControlFlagClear() == 1 && D_00324530[0] < 0) {
        D_003BC630 = 1;
        kwlnDrawSetDc8Second(0x44);
        kwlnDrawSetDc8First(0x80000000);
        kwlnDrawSetupDc8(0x1E);
        itfPanelReleaseHold();
    }
    if (D_003BC630 > 0) {
        if (D_003BC630 == 0x1E) {
            mnuStopMovieDrawTask();
            scrSetIntegerReturnValue(0);
            return 1;
        }
        D_003BC630 = D_003BC630 + 1;
    }
    scrSetIntegerReturnValue(func_00270068() == 0);
    return 1;
}

u8 func_00270218(void) {
    s64 movieState;

    movieState = func_00270088();
    return movieState == 2;
}

void mnuClearMovieList(void) {
    MovieListNode *node = D_003DC560.head;

    if (node != NULL) {
        do {
            MovieListNode *next = node->next;

            sdfReleaseChipBlock(node);
            node = next;
        } while (node != NULL);
        D_003DC560.head = NULL;
        D_003DC560.top = 0;
        D_003DC560.cursor = 0;
        D_003DC560.total = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00270098", func_002702A0);

u32 mnuGetMovieListNodeAtOffset(void) {
    MovieListNode *entry = D_003DC560.head;
    s32 remaining = D_003DC560.cursor;
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

    if (D_003DC560.head == NULL || D_003DC560.playing != 0) {
        return;
    }
    packets = D_003DC560.packets;
    sdfAppendPacket(packets, func_0011D3E8(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000, 0x60404040));
    selected = D_003DC560.cursor;
    i = D_003DC560.top;
    node = D_003DC560.head;
    selected -= i;
    for (; i > 0; i--) {
        node = node->next;
    }
    for (i = 0; i < 8 && node != NULL; i++, node = node->next) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, D_003BC648, (i == selected) ? '>' : ' ', node->path));
    }
    if (D_003DC560.top != 0) {
        func_002D6080(packets, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8, 0xFF0080, 0);
    }
    if (node != NULL) {
        func_002D6080(packets, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08, 0xFF0080, 0);
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
    if (func_00270068() == 0) {
        list = D_003DC570[0];
        sdfAppendPacket(list, func_0011D3E8(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90, 0x30000000, 0x60404040));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d", ((MovieStatus *)D_0037B888)->current, ((MovieStatus *)D_0037B888)->total));
    }
}

INCLUDE_ASM(const s32, "game/code_00270098", mnuMovieViewer);

void mnuCreateMovieViewerTask(void) {
    func_002702A0();
    D_003DC560.task = kwlnTaskCreate(D_003B1AC8, 0x2b02, 1, 0, mnuMovieViewer, 0, 0);
}

void mnuDestroyMovieViewerTask(void) {
    s32 task = kwlnTaskGetTaskByName(D_003B1AC8);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
        D_003DC560.task = 0;
        mnuStopMovieDrawTask();
    }
    mnuClearMovieList();
}

void func_00270AC0(void) {
    D_003DC578.wordPending = 1;
    D_003DC578.blockPending = 1;
}

void func_00270AD8(void) {
    D_003DC578.wordSource = 0x10002010;
    D_003DC578.blockSource = (u32)D_0037B888;
    func_00270AC0();
}

/* Copy a source word and a 0x40-byte block when their pending flags are set. */
void mnuCommitPendingMovieDrawValues(void) {
    if (D_003DC578.wordPending != 0) {
        D_003DC578.word = *(s32 *)D_003DC578.wordSource;
    }
    if (D_003DC578.blockPending != 0) {
        memcpy(D_003DC578.block, (void *)D_003DC578.blockSource, 0x40);
    }
}

s32 mnuUpdateIpuRegisterViewer(void) {
    s32 packets;
    s32 n;
    s32 i;
    s32 x;
    s32 y;
    s32 col;

    if (D_003DC578.started == 0) {
        D_003DC578.started = 1;
        func_00270AD8();
    }
    if (D_00398628.reset != 0) {
        func_00270AC0();
    } else if (D_00398628.init != 0) {
        func_00270AD8();
    } else if (D_00398628.next & 2) {
        D_003DC578.cursor++;
        if (D_003DC578.cursor == 0x10) {
            D_003DC578.cursor = 0;
        }
    } else if (D_00398628.prev & 2) {
        if (D_003DC578.cursor != 0) {
            D_003DC578.cursor--;
        } else {
            D_003DC578.cursor = 0xF;
        }
    } else {
        n = 1 << ((~D_003DC578.cursor & 7) * 4);
        i = D_003DC578.cursor >> 3;
        if (D_00398628.right & 2) {
            if (i == 0) {
                D_003DC578.wordPending = 0;
                D_003DC578.wordSource += n;
            } else {
                D_003DC578.blockPending = 0;
                D_003DC578.blockSource += n;
            }
        }
        if (D_00398628.left & 2) {
            if (i == 0) {
                D_003DC578.wordPending = 0;
                D_003DC578.wordSource -= n;
            } else {
                D_003DC578.blockPending = 0;
                D_003DC578.blockSource -= n;
            }
        }
    }
    mnuCommitPendingMovieDrawValues();
    packets = sdfCreateResetPacketList();
    sdfAppendPacket(packets, func_0011D3E8(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000, 0x40806020));
    sdfCreatePacketA(packets, 0x80A03000, 0, (D_003DC578.cursor & 7) * 0xC0 + 0x7180, (D_003DC578.cursor >> 3) * 0xC0 + 0x79C0, (D_003DC578.cursor & 7) * 0xC0 + 0x7240, (D_003DC578.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7180, 0x79C0, 0xFF0080, 0, D_003BC650, D_003DC578.wordSource));
    if (D_003DC578.wordPending != 0) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x79C0, 0xFF0080, 0, D_003BC650, D_003DC578.word));
    } else {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x79C0, 0xFF0080, 0, D_003B1AD8));
    }
    n = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7180, y, 0xFF0080, 0, D_003BC650, D_003DC578.blockSource + n));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, n++) {
            if (D_003DC578.blockPending != 0) {
                sdfAppendPacket(packets, sdfCreateFormattedSifCommand(x, y, 0xFF0080, 0, D_003BC658, D_003DC578.block[n]));
            } else {
                sdfAppendPacket(packets, sdfCreateFormattedSifCommand(x, y, 0xFF0080, 0, D_003BC660));
            }
        }
    }
    D_00325708.submitPacket(&D_00325708, packets);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1AC8);

INCLUDE_RODATA(const s32, "game/code_00270098", D_003B1AD8);

INCLUDE_SDATA(const s32, "game/code_00270098", D_003BC630);

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

