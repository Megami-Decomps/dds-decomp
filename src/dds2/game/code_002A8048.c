#include "common.h"

extern u64 scrReadIntParameter(u64);

extern u16 D_00435BAC;

extern u32 D_00437AE8;

extern u8 D_003E5608[];

extern u8 D_00457E60[];

extern char D_0042A418[];

extern u32 D_00457E48[];

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

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A8120);

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A81C8);

typedef struct MovieList {
    u32 task;
    u32 *head;
    s16 count;
    s16 offset;
    s16 unkC;
} MovieList;

void mnuClearMovieList(void) {
    u32 *node = ((MovieList *)D_00457E48)->head;
    if (node != NULL) {
        do {
            u32 *next = (u32 *)*node;
            func_00328E48(node);
            node = next;
        } while (node != NULL);
        ((MovieList *)D_00457E48)->head = NULL;
        ((MovieList *)D_00457E48)->count = 0;
        ((MovieList *)D_00457E48)->offset = 0;
        ((MovieList *)D_00457E48)->unkC = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A8268);

u32 mnuGetMovieListNodeAtOffset(void) {
    u8 *state = (u8 *)D_00457E48;
    u32 entry = D_00457E48[1];
    s32 remaining = *(s16 *)(state + 0xA);
    if (entry != 0 && remaining > 0) {
        do {
            entry = *(u32 *)entry;
            remaining--;
        } while (entry != 0 && remaining > 0);
    }
    return entry;
}

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A8610);

typedef struct MovieStatus {
    u8 pad00[0x64];
    s32 total;
    s32 pad68;
    s32 current;
} MovieStatus;

extern s32 D_00457E58[];

extern s32 func_0011F250(s32, s32, s32, s32, s32, s32, s32);

extern s32 func_0033D810(s32, s32, s32, s32, char *, s32, s32);

extern void sdfAppendPacket(s32, s32);

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
    D_00457E48[0] = kwlnTaskCreate(D_0042A418, 0x2b02, 1, 0, func_002A88A0, 0, 0);
}

void mnuDestroyMovieViewerTask(void) {
    s32 movieTask = func_00101740(D_0042A418);
    if (movieTask != 0) {
        kwlnTaskDestroyWithHierarchy(movieTask, 0);
        D_00457E48[0] = 0;
        func_002A7FD0();
    }
    mnuClearMovieList();
}

void func_002A8B78(void) {
    D_00457E60[2] = 1;
    D_00457E60[3] = 1;
}

void func_002A8B90(void) {
    u32 *task = (u32 *)D_00457E60;
    task[1] = 0x10002010;
    task[2] = (u32)D_003E5608;
    func_002A8B78();
}

/* Copy a source word and a 0x40-byte block when their pending flags are set. */
typedef struct MnuMovieTransfer {
    u8 pad00[2];
    u8 wordPending;      /* 0x02 */
    u8 blockPending;     /* 0x03 */
    s32 *wordSource;     /* 0x04 */
    void *blockSource;   /* 0x08 */
    s32 word;            /* 0x0C */
    u8 block[0x40];      /* 0x10 */
} MnuMovieTransfer;

void func_002A8BC8(void) {
    MnuMovieTransfer *state = (MnuMovieTransfer *)D_00457E60;

    if (state->wordPending != 0) {
        state->word = *state->wordSource;
    }
    if (state->blockPending != 0) {
        memcpy(state->block, state->blockSource, 0x40);
    }
}

INCLUDE_RODATA(const s32, "game/code_002A8048", D_0042A418);

INCLUDE_ASM(const s32, "game/code_002A8048", func_002A8C80);
INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AE8);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF0);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437AF8);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B00);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B08);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B10);

INCLUDE_SDATA(const s32, "game/code_002A8048", D_00437B18);

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

