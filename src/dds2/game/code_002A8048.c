#include "common.h"

extern u64 scrReadIntParameter(u64);

extern u16 D_00435BAC;

extern s32 D_00437AE8;

extern u8 D_003E5608[];

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

extern MnuMovieTransfer D_00457E60;

extern char D_0042A418[];

typedef struct MovieList {
    u32 task;
    u32 *head;
    s16 top;       /* first visible entry */
    s16 cursor;
    s16 total;
    s8 playing;
    u8 padF;
    s32 packets;
} MovieList;

extern MovieList D_00457E48;

extern s32 func_0011F250(s32, s32, s32, s32, s32, s32, s32);

extern s32 func_0033D810(s32, s32, s32, s32, char *, ...);

extern void sdfAppendPacket(s32, s32);

extern void func_0032EF30(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

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

extern MnuViewerPad D_0040B7D8;
extern MnuPacketDev D_00380708;
extern char D_0042A428[];
extern s32 sdfCreateResetPacketList(void);
extern void sdfCreatePacketA(s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 func_002A88A0();

void func_002A7AF0();

void func_002A7FD0(void);

s32 func_002A8008(void);

s32 mnuSetFrameDivisor(void) {
    func_00345488(0x3c / D_00435BAC);
    return 0;
}

void mnuCreateMovieManagerTask(void) {
    sdfSoundInitIpuStream();
    kwlnTaskCreate("movieMan", 0x385, 1, 0, mnuSetFrameDivisor, 0, 0);
}

u32 func_002A80C0(void) {
    u64 temp_v0;

    temp_v0 = scrReadIntParameter(0);
    func_002A7AF0(temp_v0);
    D_00437AE8 = 0;
    return 1;
}

u32 func_002A80F0(void) {
    func_002A7FD0();
    D_00437AE8 = 0;
    kwlnDrawEnableDc8(0);
    return 1;
}

extern s8 D_0037F530[];
extern s32 func_0010F058(void);
extern void kwlnDrawSetDc8Second(u32);
extern void kwlnDrawSetDc8First(u32);
extern void kwlnDrawSetupDc8(s32);
extern s32 itfPanelReleaseHold(void);
extern void func_0010D818(s32);

s32 func_002A8120(void) {
    if (D_00437AE8 == 0 && func_0010F058() == 1 && D_0037F530[0] < 0) {
        D_00437AE8 = 1;
        kwlnDrawSetDc8Second(0x44);
        kwlnDrawSetDc8First(0x80000000);
        kwlnDrawSetupDc8(0x1E);
        itfPanelReleaseHold();
    }
    if (D_00437AE8 > 0) {
        if (D_00437AE8 == 0x1E) {
            func_002A7FD0();
            func_0010D818(0);
            return 1;
        }
        D_00437AE8 = D_00437AE8 + 1;
    }
    func_0010D818(func_002A8008() == 0);
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
    u32 *node = D_00457E48.head;
    if (node != NULL) {
        do {
            u32 *next = (u32 *)*node;
            func_00328E48(node);
            node = next;
        } while (node != NULL);
        D_00457E48.head = NULL;
        D_00457E48.top = 0;
        D_00457E48.cursor = 0;
        D_00457E48.total = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A8268);

u32 mnuGetMovieListNodeAtOffset(void) {
    u32 entry = (u32)D_00457E48.head;
    s32 remaining = D_00457E48.cursor;
    if (entry != 0 && remaining > 0) {
        do {
            entry = *(u32 *)entry;
            remaining--;
        } while (entry != 0 && remaining > 0);
    }
    return entry;
}

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AE8);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF0);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF8);

void func_002A8610(void) {
    u32 *node;
    s32 packets;
    s32 selected;
    s32 i;

    if (D_00457E48.head == NULL || D_00457E48.playing != 0) {
        return;
    }
    packets = D_00457E48.packets;
    sdfAppendPacket(packets, func_0011F250(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000, 0x60404040));
    selected = D_00457E48.cursor;
    i = D_00457E48.top;
    node = D_00457E48.head;
    selected -= i;
    for (; i > 0; i--) {
        node = (u32 *)*node;
    }
    for (i = 0; i < 8 && node != NULL; i++, node = (u32 *)*node) {
        sdfAppendPacket(packets, func_0033D810(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, "%c%s", (i == selected) ? '>' : ' ', (s32)(node + 1)));
    }
    if (D_00457E48.top != 0) {
        func_0032EF30(packets, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8, 0xFF0080, 0);
    }
    if (node != NULL) {
        func_0032EF30(packets, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08, 0xFF0080, 0);
    }
}

typedef struct MovieStatus {
    u8 pad00[0x64];
    s32 total;
    s32 pad68;
    s32 current;
} MovieStatus;

extern s32 D_00457E58[];

void func_002A87F0(void) {
    s32 list;
    if (func_002A8008() == 0) {
        list = D_00457E58[0];
        sdfAppendPacket(list, func_0011F250(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90, 0x30000000, 0x60404040));
        sdfAppendPacket(list, func_0033D810(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d", ((MovieStatus *)D_003E5608)->current, ((MovieStatus *)D_003E5608)->total));
    }
}

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A88A0);

void mnuCreateMovieViewerTask(void) {
    func_002A8268();
    D_00457E48.task = kwlnTaskCreate(D_0042A418, 0x2b02, 1, 0, func_002A88A0, 0, 0);
}

void mnuDestroyMovieViewerTask(void) {
    s32 movieTask = func_00101740(D_0042A418);
    if (movieTask != 0) {
        kwlnTaskDestroyWithHierarchy(movieTask, 0);
        D_00457E48.task = 0;
        func_002A7FD0();
    }
    mnuClearMovieList();
}

void func_002A8B78(void) {
    D_00457E60.wordPending = 1;
    D_00457E60.blockPending = 1;
}

void func_002A8B90(void) {
    D_00457E60.wordSource = 0x10002010;
    D_00457E60.blockSource = (u32)D_003E5608;
    func_002A8B78();
}

/* Copy a source word and a 0x40-byte block when their pending flags are set. */
void func_002A8BC8(void) {
    if (D_00457E60.wordPending != 0) {
        D_00457E60.word = *(s32 *)D_00457E60.wordSource;
    }
    if (D_00457E60.blockPending != 0) {
        memcpy(D_00457E60.block, (void *)D_00457E60.blockSource, 0x40);
    }
}

s32 func_002A8C80(void) {
    s32 packets;
    s32 n;
    s32 i;
    s32 x;
    s32 y;
    s32 col;

    if (D_00457E60.started == 0) {
        D_00457E60.started = 1;
        func_002A8B90();
    }
    if (D_0040B7D8.reset != 0) {
        func_002A8B78();
    } else if (D_0040B7D8.init != 0) {
        func_002A8B90();
    } else if (D_0040B7D8.next & 2) {
        D_00457E60.cursor++;
        if (D_00457E60.cursor == 0x10) {
            D_00457E60.cursor = 0;
        }
    } else if (D_0040B7D8.prev & 2) {
        if (D_00457E60.cursor != 0) {
            D_00457E60.cursor--;
        } else {
            D_00457E60.cursor = 0xF;
        }
    } else {
        n = 1 << ((~D_00457E60.cursor & 7) * 4);
        i = D_00457E60.cursor >> 3;
        if (D_0040B7D8.right & 2) {
            if (i == 0) {
                D_00457E60.wordPending = 0;
                D_00457E60.wordSource += n;
            } else {
                D_00457E60.blockPending = 0;
                D_00457E60.blockSource += n;
            }
        }
        if (D_0040B7D8.left & 2) {
            if (i == 0) {
                D_00457E60.wordPending = 0;
                D_00457E60.wordSource -= n;
            } else {
                D_00457E60.blockPending = 0;
                D_00457E60.blockSource -= n;
            }
        }
    }
    func_002A8BC8();
    packets = sdfCreateResetPacketList();
    sdfAppendPacket(packets, func_0011F250(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000, 0x40806020));
    sdfCreatePacketA(packets, 0x80A03000, 0, (D_00457E60.cursor & 7) * 0xC0 + 0x7180, (D_00457E60.cursor >> 3) * 0xC0 + 0x79C0, (D_00457E60.cursor & 7) * 0xC0 + 0x7240, (D_00457E60.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfAppendPacket(packets, func_0033D810(0x7180, 0x79C0, 0xFF0080, 0, "%08X", D_00457E60.wordSource));
    if (D_00457E60.wordPending != 0) {
        sdfAppendPacket(packets, func_0033D810(0x7840, 0x79C0, 0xFF0080, 0, "%08X", D_00457E60.word));
    } else {
        sdfAppendPacket(packets, func_0033D810(0x7840, 0x79C0, 0xFF0080, 0, D_0042A428));
    }
    n = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfAppendPacket(packets, func_0033D810(0x7180, y, 0xFF0080, 0, "%08X", D_00457E60.blockSource + n));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, n++) {
            if (D_00457E60.blockPending != 0) {
                sdfAppendPacket(packets, func_0033D810(x, y, 0xFF0080, 0, "%02X", D_00457E60.block[n]));
            } else {
                sdfAppendPacket(packets, func_0033D810(x, y, 0xFF0080, 0, "**"));
            }
        }
    }
    D_00380708.submitPacket(&D_00380708, packets);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002A8048", D_0042A418);

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

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B72);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B73);

