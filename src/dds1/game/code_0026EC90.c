#include "common.h"

extern void mnuDestroyWindowContainer(u32);

extern void mnuReleaseResourceList(u32);

extern s32 kwlnFadeIsActive(void);

extern u8 D_0037B8BC[];

extern u8 D_0037B888[];

extern u8 D_003253C8[];

extern u32 func_002BC630(u32 *);

extern u32 effAppendListEntry(u32 *, char *, u32, u32, u32 *);

extern u32 D_0037C248[][2];

extern u32 D_0037C2C8[][2];

extern u32 D_0037C2F0[][2];

extern char D_003B1140[]; /* "mnuStaffImageProc" */

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

extern u8 D_0037B168[];

extern u32 D_003BC62C;

extern u32 D_003BC630;

extern u16 D_003BA72C;

extern u32 *D_003BC610;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern char D_003B1AC8[];

extern s32 func_00101A70();

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

extern s32 func_002E4960(s32, s32, s32, s32, char *, ...);

extern void sdfAppendPacket(s32, s32);

extern void func_002D6080(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern u32 D_003DC5C8[];

extern s32 mnuMovieViewer();

extern char D_003BC6B8[]; /* "camp" */

extern char D_003B20C0[]; /* "camp_draw" */

extern char D_003B20D0[]; /* "camp_update" */

extern s8 D_003BC6B4;

extern char D_003B1A78[]; /* "mnuMovieDraw" */

extern void effResolveAndReleaseResource(u32);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026EC90);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F118);

typedef struct StaffImage {
    u8 pad00[8];
    s32 x;
    s32 y;
    u8 pad10[0xC];
} StaffImage;

extern StaffImage D_0037AFC0[];
extern u32 D_0037AF70[];
extern s32 D_003BC614;
extern u8 D_0037B950[];
extern u8 D_0037B970[];
extern u8 D_0037B980[];
extern u8 D_0037C388[];
INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F230);

void func_0026F500(void) {
    mnuLoadStaffFonts();
}

void func_0026F518(void) {
    mnuUnloadStaffFonts();
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F530);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F5E8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1140);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F918);


s32 mnuStaffImageProc(void) {
    func_0026E720(-10, -10, 0, 0x80, D_003BC610[1], 0x10, 0, 0x27);
    func_0026E720(D_0037AFC0[D_003BC614].x - 5, D_0037AFC0[D_003BC614].y - 5, 0, 0x80, D_003BC610[1], D_0037AF70[D_003BC610[5]], 0, 0x53);
    func_0026F230(0x53);
    func_0026F918();
    return 0;
}

void func_0026FD88(void) {
    s64 pendingWork;

    D_003BA72C = 2;
    func_0026A808();
    func_0026A950();
    func_0026F518();
    do {
        pendingWork = sdfCheckPendingWorkWithInterrupts();
    } while (pendingWork != 0);
    func_002D0A10(*D_003BC610);
    D_003BC610 = (u32 *)0x0;
}

void func_0026FDD8(void) {
    func_002BDD60(D_003BC610[1]);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_002ECA40(0);
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026FE10);

extern u32 D_003BA8EC;
extern u16 D_003BA72C;
extern char D_003B1168[];
extern s32 func_002D03F8(s32);
extern s32 sdfResourceRetainAddress(s32);
extern void func_0026A5F0(s32);
extern void func_0026F5E8(void);
extern void func_0026FD88(void);

void mnuMovieCreateTask(void) {
    s32 handle;
    u32 *movie;

    D_003BA8EC = 0x80000000;
    handle = func_002D03F8(0x20);
    movie = (u32 *)sdfResourceRetainAddress(handle);
    D_003BC610 = movie;
    movie[0] = handle;
    movie[2] = 0;
    movie[3] = 0;
    func_0026A5F0(0x13);
    D_003BA72C = 1;
    kwlnTaskCreate(D_003B1168, 0x408, 0, 0, func_0026F5E8, func_0026FD88, 0);
}

u32 mnuStartStaffMovieRequest(void) {
    mnuMovieCreateTask();
    return 0xffffffff;
}

s32 mnuStopStaffTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B1140, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003B1168, 1);
    return 0;
}

s32 mnuMovieDraw(void) {
    func_002ECCF8(D_0037B888, D_003253C8);
    return 0;
}

void func_0026FFA0(u32 resource, void *data) {
    if (D_003BC62C == 0) {
        func_002ED8D0(D_0037B888, data, resource);
        D_003BC62C = kwlnTaskCreate(D_003B1A78, 0x2afb, 1, 1, mnuMovieDraw, 0, 0);
    }
}

void func_0026FFF8(s32 index) {
    u8 *entry = D_0037B168 + index * 24;

    func_0026FFA0(*(s32 *)entry, (s32)(entry + 4));
}

void func_00270030(void) {
    if (D_003BC62C == 0) {
        return;
    }
    func_002EDAE0(D_0037B888);
    kwlnTaskDestroyWithHierarchy(D_003BC62C, 0);
    D_003BC62C = 0;
}

void func_00270068(void) {
    func_002EDBB8(D_0037B888);
}

s32 func_00270088(void) {
    return D_0037B8BC[0];
}

s32 mnuSetFrameDivisor(void) {
    func_002EC5E0(0x3c / D_003BA72C);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1168);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1178);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1198);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B11B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B11D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B11F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1218);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1238);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1258);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1278);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1298);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B12B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B12D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B12F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1318);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1338);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1358);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1378);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1398);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B13B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B13D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B13F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1418);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1438);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1458);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1478);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1498);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B14B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B14D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B14F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1518);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1538);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1558);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1578);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1598);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B15B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B15D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B15F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1618);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1638);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1658);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1678);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1698);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B16B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B16D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B16F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1718);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1738);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1758);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1778);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1798);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B17B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B17D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B17F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1818);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1838);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1850);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1868);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1888);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18A0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18D0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18F0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1908);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1920);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1938);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1958);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1970);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1990);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B19B0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B19C8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B19E8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A08);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A20);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A38);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A58);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A78);

void mnuCreateMovieManagerTask(void) {
    sdfSoundInitIpuStream();
    kwlnTaskCreate("movieMan", 0x385, 1, 0, mnuSetFrameDivisor, 0, 0);
}

u32 func_00270110(void) {
    s32 movieIndex;

    movieIndex = scrReadIntParameter(0);
    func_0026FFF8(movieIndex);
    D_003BC630 = 0;
    return 1;
}

u32 func_00270140(void) {
    func_00270030();
    D_003BC630 = 0;
    kwlnDrawEnableDc8(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_00270170);

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

            func_002CFF98(node);
            node = next;
        } while (node != NULL);
        D_003DC560.head = NULL;
        D_003DC560.top = 0;
        D_003DC560.cursor = 0;
        D_003DC560.total = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_002702A0);

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

void func_00270558(void) {
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
        sdfAppendPacket(packets, func_002E4960(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, D_003BC648, (i == selected) ? '>' : ' ', node->path));
    }
    if (D_003DC560.top != 0) {
        func_002D6080(packets, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8, 0xFF0080, 0);
    }
    if (node != NULL) {
        func_002D6080(packets, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08, 0xFF0080, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_00270738);

INCLUDE_ASM(const s32, "game/code_0026EC90", mnuMovieViewer);

void mnuCreateMovieViewerTask(void) {
    func_002702A0();
    D_003DC560.task = kwlnTaskCreate(D_003B1AC8, 0x2b02, 1, 0, mnuMovieViewer, 0, 0);
}

void mnuDestroyMovieViewerTask(void) {
    s32 task = kwlnTaskGetTaskByName(D_003B1AC8);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
        D_003DC560.task = 0;
        func_00270030();
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
void func_00270B10(void) {
    if (D_003DC578.wordPending != 0) {
        D_003DC578.word = *(s32 *)D_003DC578.wordSource;
    }
    if (D_003DC578.blockPending != 0) {
        memcpy(D_003DC578.block, (void *)D_003DC578.blockSource, 0x40);
    }
}

s32 func_00270BC8(void) {
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
    func_00270B10();
    packets = sdfCreateResetPacketList();
    sdfAppendPacket(packets, func_0011D3E8(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000, 0x40806020));
    sdfCreatePacketA(packets, 0x80A03000, 0, (D_003DC578.cursor & 7) * 0xC0 + 0x7180, (D_003DC578.cursor >> 3) * 0xC0 + 0x79C0, (D_003DC578.cursor & 7) * 0xC0 + 0x7240, (D_003DC578.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfAppendPacket(packets, func_002E4960(0x7180, 0x79C0, 0xFF0080, 0, D_003BC650, D_003DC578.wordSource));
    if (D_003DC578.wordPending != 0) {
        sdfAppendPacket(packets, func_002E4960(0x7840, 0x79C0, 0xFF0080, 0, D_003BC650, D_003DC578.word));
    } else {
        sdfAppendPacket(packets, func_002E4960(0x7840, 0x79C0, 0xFF0080, 0, D_003B1AD8));
    }
    n = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfAppendPacket(packets, func_002E4960(0x7180, y, 0xFF0080, 0, D_003BC650, D_003DC578.blockSource + n));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, n++) {
            if (D_003DC578.blockPending != 0) {
                sdfAppendPacket(packets, func_002E4960(x, y, 0xFF0080, 0, D_003BC658, D_003DC578.block[n]));
            } else {
                sdfAppendPacket(packets, func_002E4960(x, y, 0xFF0080, 0, D_003BC660));
            }
        }
    }
    D_00325708.submitPacket(&D_00325708, packets);
    return 0;
}

typedef struct ResourceRef8 {
    s32 index;
    s32 pad;
} ResourceRef8;

extern ResourceRef8 D_0037C210[];
extern char D_003B2020[];
extern u32 effLoadIndexedResource(char *, s32, s32);

void mnuLoadStaffImageHandles(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        D_003DC5C8[i] = effLoadIndexedResource(D_003B2020, D_0037C210[i].index, 1);
    }
}

void mnuResolveStaffImageHandles(u32 *dst) {
    s32 i;

    for (i = 0; i < 7; i++) {
        u32 *src = &D_003DC5C8[i];
        u32 *out = &dst[i];

        effResolveAndReleaseResource(*src);
        *out = *src;
    }
}

void mnuReleaseStaffImageHandles(u32 *resources) {
    s32 index;
    for (index = 0; index < 7; index++) {
        u32 *slot = &resources[index];
        func_002BD870(D_003DC5C8[index]);
        *slot = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1AC8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1AD8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1AF0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B00);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B10);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B20);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B30);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B48);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B60);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B78);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1B88);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1BA0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1BB8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1BC8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1BE0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1BF8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1C10);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1C28);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1C38);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1C48);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1C60);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1C78);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1C90);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1CA8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1CB8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1CC8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1CD8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1CE8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1CF8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D08);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D28);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D38);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D48);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D58);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D68);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D78);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D88);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1D98);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1DA8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1DB8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1DC8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1DD8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1DE8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1DF8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E08);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E18);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E28);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E48);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E58);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E68);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E78);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E88);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1E98);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1EA8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1EB8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1EC8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1EE0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F00);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F18);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F30);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F48);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F60);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F70);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F80);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1F90);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1FA0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1FB0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1FC0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1FD0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1FE0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1FF0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B2000);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B2010);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B2020);

void *mnuGetStaffCategoryEntries(s32 kind, s32 *count, u8 *data) {
    switch (kind) {
    case 1:
        *count = 4;
        return data + 0xe0;
    case 2:
        *count = 2;
        return data + 0xd8;
    case 3:
        *count = 2;
        return data + 0x7c;
    case 4:
        *count = 9;
        return data + 0xf0;
    case 5:
        *count = 1;
        return data + 0x114;
    default:
        *count = 0;
        return 0;
    }
}

extern u8 *D_003BAA00;
extern s8 D_003BC6B5;

void func_00271180(s32 list, s32 count, u8 *work) {
    s32 i;

    effResolveAndReleaseResource(*(u32 *)list);
    for (i = 0; i < 5; i++) {
        u8 *slot = D_003BAA00 + 0xA60 + i * 0x1A4;

        if ((*(u16 *)slot & 1) != 0) {
            s32 index = *(u16 *)(slot + 4) + D_003BC6B5;

            effResolveAndReleaseResource(*(u32 *)(list + index * 4 - 4));
        }
    }
}

void movReleaseCategoryModels(s32 kind, u8 *work) {
    s32 count;
    s32 *entries = (s32 *)mnuGetStaffCategoryEntries(kind, &count, work);
    if (kind != 4) {
        s32 i;
        for (i = 0; i < count; i++) {
            effResolveAndReleaseResource(entries[i]);
        }
    } else {
        func_00271180(entries, count, work);
    }
}

extern void func_002BD870(u32);

void func_002712A0(kind, work)
s32 kind;
u8 *work;
{
    s32 count;
    s32 i = 0;
    u8 *buffer = mnuGetStaffCategoryEntries(kind, &count, work);

    if (count > 0) {
        u32 *handles = (u32 *)buffer;
        do {
            func_002BD870(*handles++);
        } while (++i < count);
    }
}

void mnuSetStaffDisplayMode(s32 next, u8 *context) {
    s32 previous = *(s32 *)(context + 0x910);
    if (next != previous) {
        if (previous != 0) {
            func_002712A0(previous);
        }
        if (next != 0) {
            movReleaseCategoryModels(next, context);
        }
        *(s32 *)(context + 0x910) = next;
    }
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_00271368);

typedef struct StaffSpriteHandles {
    u8 pad00[0x118];
    u32 primaryImage;
    u32 resourceList;
    u32 secondaryImage;
    u32 images[3];
    u32 extraImages[2];
} StaffSpriteHandles;

void mnuReleaseStaffSpriteHandles(StaffSpriteHandles *handles) {
    u32 *image = handles->extraImages;
    u32 index = 0;
    effDestroyPackedBatch(handles->primaryImage);
    effDestroyPackedBatch(handles->secondaryImage);
    do {
        effDestroyPackedBatch(*image++);
        index++;
    } while (index < 2);
}

void func_00271480(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    mnuInitPageWindow(arg0, arg3, arg1[3], 7, arg1[4], 0, *arg1, 0x11);
    func_0027FAA8(arg0, *arg1);
    func_0027FBE0(arg0, arg1 + 9);
    func_0027FC10(arg0, arg1 + 0x11);
    mnuRegisterResourceHandles(arg0, arg1 + 0x19);
    mnuUpdateHandleStates(arg0);
}

void func_00271500(u32 *list, u32 *state) {
    s32 i;
    s32 flag;

    mnuResolveStaffImageHandles(state);
    flag = func_002BC630(list) == 1;
    for (i = 0; i < 16; i++) {
        effAppendListEntry(list, D_003B2020, D_0037C248[i][flag], 1, state + 0x24 / 4 + i);
    }
    for (i = 0; i < 5; i++) {
        effAppendListEntry(list, D_003B2020, D_0037C2C8[i][flag], 1, state + 0x64 / 4 + i);
    }
    for (i = 0; i < 2; i++) {
        effAppendListEntry(list, "/camp/spr/n_sta/", D_0037C2F0[i][flag], 1, state + 0x1C / 4 + i);
    }
}

void mnuReleaseStaffResourceGroups(u32 *resources) {
    u32 *inner = resources + 1;
    s32 i;

    mnuReleaseStaffImageHandles(resources);
    for (i = 0; i < 16; i++) {
        func_002BDD60(resources[9 + i]);
    }
    for (i = 0; i < 5; i++) {
        func_002BDD60(inner[24 + i]);
    }
    for (i = 0; i < 2; i++) {
        func_002BDD60(resources[7 + i]);
    }
}

typedef struct StaffSlots {
    u32 baseResources[7];    /* 0x00 */
    u32 pairResources[2];    /* 0x1C */
    u32 mainResources[16];   /* 0x24 */
    u32 extraResources[5];   /* 0x64 */
} StaffSlots;

s32 mnuStaffSlotsAllFilled(s32 unused, StaffSlots *slots) {
    s32 i;

    func_002BC748();
    for (i = 0; i < 7; i++) {
        if (slots->baseResources[i] == 0) {
            return 0;
        }
    }
    for (i = 0; i < 16; i++) {
        if (slots->mainResources[i] == 0) {
            return 0;
        }
    }
    for (i = 0; i < 5; i++) {
        if (slots->extraResources[i] == 0) {
            return 0;
        }
    }
    for (i = 0; i < 2; i++) {
        if (slots->pairResources[i] == 0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_002717D8);

s64 func_00271948(u32 *resources) {
    s32 i;

    mnuReleaseStaffResourceGroups(resources + 0x18);
    for (i = 0; i < 2; i++) {
        func_002BDD60(resources[54 + i]);
    }
    for (i = 0; i < 4; i++) {
        func_002BDD60(resources[56 + i]);
    }
    for (i = 0; i < 9; i++) {
        func_002BDD60(resources[60 + i]);
    }
    return func_002BDD60(resources[69]);
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_002719F0);

void func_00271B40(void) {
}

void func_00271B48(void) {
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_00271B50);

void mnuCreateStaffPanelSet(StaffSpriteHandles *menu) {
    menu->resourceList = func_0027D4A0(0, *(u32 *)((u8 *)menu + 0x6C), menu->secondaryImage);
    menu->images[0] = func_00271B50(D_0037B950, 8, 0x300, menu, D_0037C388);
    mnuForwardDupArg(menu->images[0], *(u32 *)((u8 *)menu + 0x74), 0, 0, 0);
    menu->images[1] = func_00271B50(D_0037B970, 3, 0x2C0, menu, 0);
    mnuSetWindowContainerState(menu->images[1], 0x100);
    menu->images[2] = func_00271B50(D_0037B980, 2, 0x200, menu, 0);
    mnuSetWindowContainerState(menu->images[2], 0x100);
}

void func_00271DF8(StaffSpriteHandles *handles) {
    u32 *image = handles->images;
    u32 index = 0;
    do {
        mnuDestroyWindowContainer(*image++);
    } while (++index < 3);
    mnuReleaseResourceList(handles->resourceList);
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_00271E58);

extern s32 func_00101A70();

extern s8 D_003BC6B4;

extern void func_002D0918(u32);

void mnuDestroyStaffMenuTask(u32 task) {
    u8 *work = (u8 *)func_00101A70(task);
    if (work == NULL) {
        return;
    }
    func_00285600(work + 8, task);
    func_00271DF8((StaffSpriteHandles *)work);
    func_0027E690(*(u32 *)(work + 0x138));
    mnuShutdownContext(work + 0x15C);
    func_0024DBC8();
    mnuReleaseAssets(work + 0x13C);
    func_00271948(work);
    mnuReleaseStaffSpriteHandles((StaffSpriteHandles *)work);
    func_002BC618(*(u32 *)(work + 0x5c));
    func_002D0918(*(u32 *)work);
    D_003BC6B4 = 2;
    func_002E9730();
}

u32 func_00271FC8(void) {
    s32 context;

    context = func_00101A70();
    func_00283BF8(context + 0x914, 0x53);
    return 0;
}

s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 buttons = func_00285B20(8);
    s32 result;

    if (func_002719F0(menu) == 0) {
        return 0;
    }
    result = 0;
    if (func_0024DC08() == 0) {
        if (buttons & 8) {
            if (func_002877A8() != 1) {
                if (fileConsumeConfigTaskReady() == 0) {
                    mnuDestroyCampTasks();
                    mnuPlayInputSound(0, 2, 0);
                    return -1;
                }
            }
            mnuPlayInputSound(0, 0x8000, 0);
        }
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B20C0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B20D0);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_002720B0);

void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC6B8, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003B20C0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003B20D0, 0);
}

s32 mnuAcknowledgeCampState(void) {
    s8 state = D_003BC6B4;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC6B4 = 0;
    }
    return 0;
}

u8 mnuIsFadeIdle(void) {
    s64 fadeActive;

    fadeActive = kwlnFadeIsActive();
    return fadeActive == 0;
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_00272280);

extern u32 D_0037B988[];

void mnuCreateStaffImageSprite(s32 index) {
    u32 *object = (u32 *)func_00197760(0x2F0, 0x1E0, 0, 0xa09dc35a,
                                      D_0037B988[index], 0);
    func_001958A0(object, 1, 0x54);
    func_00194920(object);
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_002723B0);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_00272518);

void func_00272668(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_00272518(arg0, arg1, arg2, arg3, arg4, 0, arg5);
}

void mnuDrawStaffCampScreen(s32 arg0, s32 arg1) {
    u8 *menu = (u8 *)func_00101A70(arg1);

    mnuDrawBackdrop(menu + 0x13C, 0x20);
    if (func_002719F0(arg1) == 0) {
        return;
    }
    func_0027E8D8(-0x10, -8, 0, *(s32 *)(menu + 0x138), 0x53);
    func_00282BE8(0, 0, 0, menu + 0x15C, 0x53);
    if (arg0 == 0) {
        func_002BF790(0x1AB0, 0x70, 0, 1, *(s32 *)(menu + 0x64), 6, 0x53);
        func_002BF790(0x17A0, 0x78, 0, 1, *(s32 *)(menu + 0x60), 0xF, 0x53);
        func_002BF790(0x1E40, 0x78, 0, 1, *(s32 *)(menu + 0x60), 0x10, 0x53);
    }
}

void func_00272778(u32 arg0) {
    mnuDrawStaffCampScreen(0, arg0);
}

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B2100);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC614);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC618);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC620);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC628);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC62C);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC630);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC638);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC640);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC648);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC650);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC658);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC660);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC668);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC670);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC678);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC680);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC688);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC690);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC698);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC6A0);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC6A8);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC6B0);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC6B4);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC6B5);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC6B8);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC6C0);

