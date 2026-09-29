#include "common.h"

void sdfRelocatePackedResourceWords(int *param_1, int param_2, u8 *param_3, int param_4);

extern u32 D_004365E8;

/* One 0x14-byte slot per message window; the first field points at its state. */
typedef struct ItfMesSlot {
    struct ItfMesState *mes;
    u8 unk4[0x10];
} ItfMesSlot;

extern ItfMesSlot D_0045296C[];

/* Globals behind D_003D6EA0: word at +0x4, bitfield at +0xC. */
typedef struct ItfMesGlobals {
    u32 unk0; /* 0x0 */
    u32 unk4; /* 0x4: read by func_0019B870 */
    u32 unk8; /* 0x8 */
    u16 flags; /* 0xC: set/cleared by itfMesSetGlobalFlags */
    u16 unkE; /* 0xE */
} ItfMesGlobals;

extern ItfMesGlobals D_00452940;

#define ITF_MES_MAGIC_MSG0 0x3047534d
#define ITF_MES_MAGIC_MSG1 0x3147534d

typedef struct ItfMesRelocResource {
    u8 pad00[8];
    u32 magic;
    u8 pad0C[4];
    s32 fixupOffset;
    s32 fixupCount;
    u8 pad18[4];
    u8 relocated;
    u8 pad1D[3];
    u8 payload[1];
} ItfMesRelocResource;

/* 3 words zeroed by func_0019D0A0. */
typedef struct ItfMesZero {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} ItfMesZero;

extern ItfMesZero D_003B4770;

/* Sized table indexed by func_0019D268: s16 count + u32 items. */
typedef struct ItfMesTable {
    u8 unk0[0x18]; /* 0x0 */
    s16 count;      /* 0x18 */
    s16 unk1A;      /* 0x1A */
    u32 items[1];   /* 0x1C */
} ItfMesTable;

/* 8-byte entry selected by func_0019D1D8/func_0019D1F0. */
typedef struct ItfMesEntry {
    u32 unk0;            /* 0x0: item list read by func_0019D5D0 */
    ItfMesTable *table;  /* 0x4: read by func_0019C920 */
} ItfMesEntry;

/* Record behind ItfMesState.sub; func_0019D240 reads word +0x18. */
typedef struct ItfMesSub {
    u8 unk0[0x18];      /* 0x0 */
    u32 entryCount;     /* 0x18: next entry index */
    u8 unk1C[4];        /* 0x1C */
    ItfMesEntry entries[1]; /* 0x20: indexed message entries */
} ItfMesSub;

typedef struct FrFontGlyph FrFontGlyph;

/* Block at ItfMesState +0x14. */
typedef struct ItfMesBlk14 {
    u32 unk0;            /* +0x0 */
    u32 unk4;            /* +0x4 */
    FrFontGlyph *unk8;   /* +0x8 */
    u16 selectedIndex;   /* +0xC: chooses an item in the next entry */
    u16 padE;
} ItfMesBlk14;

/* Block at ItfMesState +0x24. */
typedef struct ItfMesBlk24 {
    u32 unk0;            /* +0x0 */
    u32 unk4;            /* +0x4 */
    ItfMesTable *unk8;   /* +0x8 */
    FrFontGlyph *unkC;   /* +0xC */
    u32 unk10;           /* +0x10 */
} ItfMesBlk24;

/* Block at ItfMesState +0x40. */
typedef struct ItfMesBlk40 {
    u32 unk0;            /* +0x0 */
    u32 unk4;            /* +0x4 */
    FrFontGlyph *unk8;   /* +0x8 */
    u32 panelValue;      /* +0xC: set by itfMesScriptSetPanelValue */
    u16 unk10;           /* +0x10 */
    s16 unk12;           /* +0x12 */
    u16 unk14;           /* +0x14 */
    u16 unk16;           /* +0x16 */
} ItfMesBlk40;

/* Message-window state behind each ItfMesSlot. */
typedef struct ItfMesState {
    u32 flags;          /* 0x0: low half status, high half mask */
    ItfMesSub *sub;     /* 0x4 */
    u8 unk8[0xC];       /* 0x8 */
    ItfMesBlk14 blk14;    /* 0x14: passed to func_0019DDA8 */
    ItfMesBlk24 blk24;    /* 0x24: passed to func_0019DDD0 */
    u8 unk38;             /* 0x38 */
    u8 unk39;           /* 0x39: set by func_0019CB78 */
    u8 unk3A[2];        /* 0x3A */
    s16 unk3C;          /* 0x3C: read by func_0019C548 */
    s16 unk3E;          /* 0x3E: read by func_0019C528 */
    ItfMesBlk40 blk40;  /* 0x40 */
    u8 unk58[0x78];     /* 0x58 */
    u32 tableD0[1];  /* 0xD0: indexed by func_0019C568 (true length unknown) */
    u8 unkD4[0x108]; /* 0xD4 */
    u32 unk1DC;      /* 0x1DC: set by func_0019CB98 */
} ItfMesState;

ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 index);

ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub);

u32 itfMesGetTableItem(ItfMesTable *table, s32 index);

/* Item chained off a window node (+0x28); recolored by itfMesRecolorNodeChildren. */
typedef struct ItfMesItem {
    u8 unk0[0x10];         /* 0x0 */
    u32 word10;            /* 0x10: low byte is the color */
    u8 unk14[2];           /* 0x14 */
    u8 flag16;             /* 0x16: tested by func_001A5B70 */
    u8 unk17[0x11];        /* 0x17 */
    struct ItfMesItem *next; /* 0x28 */
} ItfMesItem;

/* Window chain node shared by positioning and child-formatting helpers. */
typedef struct ItfMesNode {
    u8 unk0[4];        /* 0x0 */
    s32 x;             /* 0x4: adjusted with horizontal node offsets */
    s32 y;             /* 0x8: groups nodes on the same row */
    s32 advance;       /* 0xC: accumulated within a row */
    u8 unk10[4];       /* 0x10 */
    s32 unk14;         /* 0x14: set by func_001A5950 */
    u8 unk18[4];       /* 0x18 */
    ItfMesItem *child; /* 0x1C */
    u8 unk20[4];       /* 0x20 */
    struct ItfMesNode *next; /* 0x24 */
} ItfMesNode;

/* Operands of itfMesCountSpanSteps: word at +0x8, divisor at +0x12. */
typedef struct ItfMesSpan {
    u8 unk0[8]; /* 0x0 */
    s32 unk8;   /* 0x8 */
    u8 unkC[6]; /* 0xC */
    s16 unk12;  /* 0x12 */
} ItfMesSpan;

s32 scrGetWindow(void);

s32 func_0010D650(s32 arg0);

void func_001A4090(s32 window, u32 value);

void func_001A40B0(s32 window, s32 count);

void func_00154F18(s32 arg0);

void itfMesSetWindowHighFlags(s32 window, u32 value);

void func_001A45C0(s32 window, s32 arg1, s32 arg2, s32 arg3);

void itfMesClearWindowHighFlags(s32 window, u32 value);

void func_001A4218(s32 window, s32 arg1, s32 arg2);

void func_001A4988(s32 window, s32 arg1, s32 arg2);

void func_001A4A10(s32 window, s32 arg1, s32 arg2);

void func_001A34D0(s32 window, s32 arg1, s32 arg2);

void itfMesCleanupWindow(s32 window, s32 arg1);

void itfMesResetWindow(s32 window);

void func_001A50D8(s32 window);

void func_0019C5B0(FrFontGlyph *arg0);

void func_001A5E00(void *arg0, s32 arg1);

void func_001A5DD8(void *arg0, s32 arg1);

/* Glyph/record chain walked by func_001958A0/frFontLinkGlyph. */
typedef struct FrFontGlyph {
    union {
        s16 h;                        /* 0x0: halfword view */
        struct { s8 b0; s8 b1; } b;   /* 0x0: byte views */
    } u0;
    s16 unk2;         /* 0x2 */
    s32 x;            /* 0x4: horizontal position */
    s32 y;            /* 0x8: vertical position */
    s32 advance;      /* 0xC: advance shifted by four when linking glyphs */
    u32 unk10;        /* 0x10 */
    union {
        u32 w;        /* 0x14: word view */
        u8 b[4];      /* 0x14: byte views */
    } u14;
    union {
        u32 w;            /* 0x18: word view */
        u8 b[4];          /* 0x18: byte views */
    } unk18;
    struct FrFontGlyph *firstChild; /* 0x1C: chain traversed by frFontMeasureGlyphChain */
    struct FrFontGlyph *unk20; /* 0x20 */
    struct FrFontGlyph *previous; /* 0x24: backward link through the glyph chain */
    struct FrFontGlyph *next; /* 0x28: next glyph in chain */
    struct FrFontGlyph *chainHead; /* 0x2C: first glyph in the linked chain */
    u32 unk30;        /* 0x30 */
    u32 unk34;        /* 0x34 */
    u32 unk38;        /* 0x38 */
    s32 unk3C;        /* 0x3C */
    s32 unk40;        /* 0x40 */
} FrFontGlyph;

s32 itfMesScriptSetPanelValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    func_001A4090(window, func_0010D650(0));
    return 1;
}

s32 itfMesScriptSetWindowValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    func_001A40B0(window, func_0010D650(0));
    return 1;
}

s32 itfMesScriptActivatePanel(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowHighFlags(window, 0x200000);
    func_00154F18(2);
    return 1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3458);

void func_001A34D0(s32 window, s32 arg1, s32 arg2) {
    ItfMesState *mes = D_0045296C[window].mes;
    u32 flags = mes->flags;

    if (flags & 0x300) {
        mes->flags = flags | 0x300;
    }
    if (flags & 0x3000) {
        mes->flags = mes->flags | 0x3000;
    }
    itfMesCleanupWindow(window, 1);
    itfMesResetWindow(window);
    mes->flags &= 0xFFDFFFFF;
}

u32 func_001A3560(void) {
    return 1;
}

u32 func_001A3568(void) {
    return 1;
}

s32 itfMesScriptSetWindowGeometry(void) {
    s32 window = scrGetWindow();
    s32 x;
    s32 y;
    s32 width;

    if (window < 0) {
        return 1;
    }
    x = func_0010D650(0);
    y = func_0010D650(1);
    width = func_0010D650(2);
    func_001A45C0(window, x, y, width);
    return 1;
}

s32 itfMesScriptToggleMessageFlag(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (func_0010D650(0)) {
        itfMesSetWindowHighFlags(window, 0x400000);
    } else {
        itfMesClearWindowHighFlags(window, 0x400000);
    }
    return 1;
}

s32 itfMesScriptSetScaledPosition(void) {
    s32 window = scrGetWindow();
    s32 x;
    s32 y;

    if (window < 0) {
        return 1;
    }
    x = func_0010D650(0);
    y = func_0010D650(1);
    func_001A4218(window, x << 4, y << 3);
    return 1;
}

s32 itfMesScriptSetDefaultBounds(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    func_001A4218(window, 0x4B0, 0xAF8);
    return 1;
}

s32 itfMesScriptToggleHighFlags(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (func_0010D650(0)) {
        itfMesClearWindowHighFlags(window, 0x800000);
        itfMesClearWindowHighFlags(window, 0x100000);
    } else {
        itfMesSetWindowHighFlags(window, 0x800000);
        itfMesSetWindowHighFlags(window, 0x100000);
    }
    return 1;
}

s32 itfMesScriptSetMessageOption(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    func_001A4988(window, func_0010D650(0), 0);
    return 1;
}

s32 itfMesScriptSetMessagePair(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = func_0010D650(0);
    second = func_0010D650(1);
    func_001A4988(window, first, second);
    return 1;
}

s32 itfMesScriptSetMessageRange(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = func_0010D650(0);
    second = func_0010D650(1);
    func_001A4A10(window, first, second);
    return 1;
}

u32 itfMesGetGlobalWindowValue(void) {
    return D_00452940.unk4;
}

void itfMesSetFlags(u32 arg0) {
    D_004365E8 = D_004365E8 | arg0;
}

void itfMesClearFlags(u32 arg0) {
    D_004365E8 = D_004365E8 & ~arg0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A38D8);

void func_001A39D0(s32 window) {
    if (window >= 0 && D_0045296C[window].mes != NULL) {
        func_001A50D8(window);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3A18);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3C28);

void itfMesCleanupWindow(s32 window, s32 arg1) {
    ItfMesState *mes;
    ItfMesBlk24 *blk24;
    ItfMesBlk14 *blk14;

    if (window < 0) {
        return;
    }
    mes = D_0045296C[window].mes;
    blk24 = &mes->blk24;
    blk14 = &mes->blk14;
    if (blk24->unkC != NULL) {
        func_0019C5B0(blk24->unkC);
        blk24->unkC = NULL;
    }
    func_001A5E00(blk24, 0);
    mes->flags &= ~7;
    mes->flags &= 0xFFFDFFFF;
    if (arg1 == 0) {
        return;
    }
    if (blk14->unk8 != NULL) {
        func_0019C5B0(blk14->unk8);
        blk14->unk8 = NULL;
    }
    func_001A5DD8(blk14, 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3DA8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3EE0);

void itfMesResetWindow(s32 window) {
    ItfMesState *mes = D_0045296C[window].mes;
    ItfMesBlk40 *blk = &mes->blk40;
    if (blk->unk8 != NULL) {
        func_0019C5B0(blk->unk8);
        blk->unk8 = NULL;
    }
    blk->panelValue = 0;
    blk->unk10 = 0;
    blk->unk16 = 0;
    blk->unk12 = -1;
    mes->flags &= ~0x38;
    mes->flags &= 0xFFFBFFFF;
}

void func_001A4090(s32 window, u32 value) {
    D_0045296C[window].mes->blk40.panelValue = value;
}

void func_001A40B0(s32 window, s32 count) {
    ItfMesBlk40 *blk = &D_0045296C[window].mes->blk40;
    u32 bits = blk->panelValue;
    s32 bit = bits & 1;
    s32 zeros = bit == 0;
    s32 i = 0;
    if (count > 0) {
        do {
            i++;
            bits >>= 1;
            bit = bits & 1;
            if (bit == 0) {
                zeros++;
            }
        } while (i < count);
    }
    zeros = bit == 0 ? zeros - 1 : 0;
    blk->unk12 = zeros;
    blk->unk14 = zeros;
}

void itfMesOffsetNodeChain(ItfMesNode *node, s32 dx, s32 dy);

void itfMesBlk14MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk14 *blk = &D_0045296C[window].mes->blk14;
    s32 delta[2];
    delta[0] = x - blk->unk0;
    delta[1] = y - blk->unk4;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain((ItfMesNode *)blk->unk8, delta[0], delta[1]);
        blk->unk0 = x;
        blk->unk4 = y;
    }
}

void itfMesBlk14MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk14 *blk = &D_0045296C[window].mes->blk14;
    itfMesOffsetNodeChain((ItfMesNode *)blk->unk8, dx, dy);
    blk->unk0 += dx;
    blk->unk4 += dy;
}

void func_001A4218(s32 window, s32 x, s32 y) {
    ItfMesBlk24 *blk = &D_0045296C[window].mes->blk24;
    s32 delta[2];
    delta[0] = x - blk->unk0;
    delta[1] = y - blk->unk4;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain((ItfMesNode *)blk->unkC, delta[0], delta[1]);
        blk->unk0 = x;
        blk->unk4 = y;
    }
}

void func_001A42A8(s32 window, s32 dx, s32 dy) {
    ItfMesBlk24 *blk = &D_0045296C[window].mes->blk24;
    itfMesOffsetNodeChain((ItfMesNode *)blk->unkC, dx, dy);
    blk->unk0 += dx;
    blk->unk4 += dy;
}

void func_001A4318(s32 window, s32 x, s32 y) {
    ItfMesBlk40 *blk = &D_0045296C[window].mes->blk40;
    s32 delta[2];
    delta[0] = x - blk->unk0;
    delta[1] = y - blk->unk4;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain((ItfMesNode *)blk->unk8, delta[0], delta[1]);
        blk->unk0 = x;
        blk->unk4 = y;
    }
}

void func_001A43A8(s32 window, s32 dx, s32 dy) {
    ItfMesBlk40 *blk = &D_0045296C[window].mes->blk40;
    itfMesOffsetNodeChain((ItfMesNode *)blk->unk8, dx, dy);
    blk->unk0 += dx;
    blk->unk4 += dy;
}

void func_001A4418(s32 window, s32 value) {
    u8 *mes = (u8 *)D_0045296C[window].mes;
    if (*(s32 *)(mes + 0xC) != value) {
        func_001A5950(*(ItfMesNode **)(mes + 0x1C), value);
        func_001A5950(*(ItfMesNode **)(mes + 0x30), value);
        func_001A5950(*(ItfMesNode **)(mes + 0x48), value);
        *(s32 *)(mes + 0xC) = value;
    }
}

u32 itfMesGetWindowFlags(s32 window) {
    return D_0045296C[window].mes->flags;
}

void itfMesReplaceWindowHighFlags(s32 window, u32 value) {
    ItfMesState *mes;

    mes = D_0045296C[window].mes;
    mes->flags = (u32)(u16)mes->flags | (value & 0xffff0000);
}

void itfMesSetWindowHighFlags(s32 window, u32 value) {
    ItfMesState *mes;

    mes = D_0045296C[window].mes;
    mes->flags = mes->flags | (value & 0xffff0000);
}

void itfMesClearWindowHighFlags(s32 window, u32 value) {
    ItfMesState *mes;

    mes = D_0045296C[window].mes;
    mes->flags = mes->flags & (~value | 0xffff);
}

s16 func_001A4538(s32 window) {
    return D_0045296C[window].mes->blk40.unk12;
}

s16 func_001A4558(s32 window) {
    return D_0045296C[window].mes->unk3E;
}

s16 func_001A4578(s32 window) {
    return D_0045296C[window].mes->unk3C;
}

u32 itfMesGetWindowTableValue(s32 window, s32 index) {
    return D_0045296C[window].mes->tableD0[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A45C0);

void func_001A4858(s32 window, u32 arg1, u32 arg2) {
    func_001A5480((u32)D_0045296C[window].mes, arg1, arg2, 0);
}

void func_001A4888(s32 window) {
    func_001A5480((u32)D_0045296C[window].mes);
}

ItfMesSub *func_001A48B8(s32 window, ItfMesSub *sub) {
    ItfMesState *mes = D_0045296C[window].mes;
    ItfMesSub *previous = mes->sub;
    ItfMesEntry *entry;
    s32 value;
    mes->sub = sub;
    itfMesRelocate(sub);
    entry = itfMesGetNextEntry(sub);
    value = 0;
    if (*(s32 *)((u8 *)sub + 8) == 0x3147534D) {
        value = *(s32 *)((u8 *)entry + 8);
    }
    *(s32 *)&mes->unk8[0] = value;
    return previous;
}

u32 itfMesGetEntryTableItem(s32 window, s32 entryIndex, s32 itemIndex) {
    return itfMesGetTableItem(itfMesGetEntry(D_0045296C[window].mes, entryIndex)->table, itemIndex);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4988);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4A10);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4AD8);

void func_001A4B98(s32 window, u8 value) {
    D_0045296C[window].mes->unk39 = value;
}

void func_001A4BB8(s32 window, u32 value) {
    D_0045296C[window].mes->unk1DC = value;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4BD8);

void itfMesSetGlobalFlags(u32 bits) {
    D_00452940.flags |= bits;
}

void itfMesClearGlobalFlags(u32 bits) {
    D_00452940.flags &= ~bits;
}

u16 itfMesGetGlobalFlags(void) {
    return D_00452940.flags;
}

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CE0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CF0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D00);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D10);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4CF8);

void itfMesClearGlobalWords(void) {
    D_003B4770.unk0 = 0;
    D_003B4770.unk4 = 0;
    D_003B4770.unk8 = 0;
}

typedef struct ItfMesWindowRec {
    u8 pad00[0xC];
    ItfMesState *mes;
    s32 handle;
} ItfMesWindowRec;

extern ItfMesWindowRec D_00452960[];
extern void btlReleaseEffectResourceHandles();
extern void func_001A5FA0();
extern void itfReleasePoolNode();

void func_001A50D8(s32 window) {
    ItfMesWindowRec *rec = &D_00452960[window];
    ItfMesState *mes = rec->mes;
    if (mes != NULL) {
        itfMesCleanupWindow(window, 1);
        itfMesResetWindow(window);
        btlReleaseEffectResourceHandles(mes);
        func_001A5FA0(mes->tableD0);
        mes->flags = 0;
        func_003297C8(rec->handle);
        rec->mes = NULL;
        itfReleasePoolNode(rec, (u8 *)D_00452960 - 0x10);
        *(s32 *)((u8 *)D_00452960 - 0x20) -= 1;
    }
}

/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void itfMesRelocate(ItfMesRelocResource *resource)
{
    u8 *base;
    u8 *fixups;
    s32 size;
    if (resource->relocated == 0) {
        // Keep the source's pointer-add expression: direct payload access changes ee-gcc codegen.
        base = (u8 *)resource + 0x20;
        fixups = (u8 *)resource + resource->fixupOffset;
        size = resource->fixupCount;
        sdfRelocatePackedResourceWords((int *)base, (int)base, fixups, size);
        resource->relocated = 1;
    }
}

u32 itfMesIsMsgData(ItfMesRelocResource *resource) {
    u32 matched;

    matched = 0;
    if ((resource->magic == ITF_MES_MAGIC_MSG0) || (resource->magic == ITF_MES_MAGIC_MSG1)) {
        matched = 1;
    }
    return matched;
}

ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 index) {
    ItfMesEntry *entries = mes->sub->entries;

    return &entries[index];
}

ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub) {
    ItfMesEntry *entries = sub->entries;

    return &entries[sub->entryCount];
}

u32 func_001A5228(s32 window, s32 index) {
    return itfMesGetEntry(D_0045296C[window].mes, index)->unk0;
}

u32 itfMesGetEntryCount(s32 window) {
    return D_0045296C[window].mes->sub->entryCount;
}

u32 itfMesGetTableItem(ItfMesTable *table, s32 index) {
    s32 count = table->count;

    if (index < 0 || index >= count) {
        return 0;
    }
    return table->items[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A52B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5480);

extern void func_0019E8F0();

void func_001A55B0(s32 *table) {
    s32 i;
    for (i = 0; i < 0x20; i++) {
        func_0019E8F0(i, table[i]);
    }
}

u32 itfMesGetNextEntrySelectedItem(ItfMesState *mes) {
    s32 *items;

    items = (s32 *)itfMesGetNextEntry(mes->sub);
    return *(u32 *)((u32)mes->blk14.selectedIndex * 4 + *items);
}

s32 itfMesCountZeroBits(s32 count, u32 bits) {
    s32 zeros;
    u32 bit;

    zeros = 0;
    while (0 < count) {
        bit = bits & 1;
        bits = bits >> 1;
        count = count - 1;
        if (bit == 0) {
            zeros = zeros + 1;
        }
    }
    return zeros;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5670);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5760);

ItfMesNode *itfMesGetLastNode(ItfMesNode *node) {
    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

void func_001A58B8(u8 *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x20);
    *(u8 *)(arg1 + 0x15) = *arg0 >> 1;
    *(u8 *)(arg1 + 0x12) = *(u8 *)(temp_v0 + 0x15);
    *(u8 *)(arg1 + 0x13) = *(u8 *)(temp_v0 + 0x14);
    *(u8 *)(arg1 + 0x14) = *(u8 *)(temp_v0 + 0x16);
}

s32 itfMesCountSpanSteps(ItfMesSpan *arg0, ItfMesSpan *arg1) {
    return ((arg1->unk8 - arg0->unk8) >> 3) / arg1->unk12 + 1;
}

void itfMesOffsetNodeChain(ItfMesNode *node, s32 dx, s32 dy) {
    if (node == NULL) {
        return;
    }
    do {
        node->x += dx;
        node->y += dy;
        node = node->next;
    } while (node != NULL);
}

/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void func_001A5950(ItfMesNode *node, s32 value) {
    while (node != NULL) {
        node->unk14 = value;
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5988);

void func_001A5A28(FrFontGlyph *glyph, u8 value) {
    FrFontGlyph *child;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->u14.b[0] = value;
        }
    }
}

/* Persona 4 func_0027a4d0 @ 0027A4D0 (src/itfMesManager.c), recompiled unchanged */
void itfMesRecolorNodeChildren(ItfMesNode *node, u32 color) {
    ItfMesItem *item;

    for (; node != NULL; node = node->next) {
        for (item = node->child; item != NULL; item = item->next) {
            item->word10 = item->word10 & 0xffffff00 | color;
        }
    }
}

s32 itfMesMaxGroupedExtent(ItfMesNode *node) {
    s32 best = 0;

    while (node != NULL) {
        s32 key = node->y;
        s32 total = 0;

        do {
            total += node->advance;
            node = node->next;
        } while (node != NULL && key == node->y);
        if (total > best) {
            best = total;
        }
    }
    return best << 4;
}

s32 func_001A5B20(s32 skip, u32 mask) {
    s32 bit = 0;
    while (bit < 0x20) {
        if ((mask & 1) == 0) {
            if (--skip < 0) {
                break;
            }
        }
        bit++;
        mask >>= 1;
    }
    return bit;
}

/* Persona 4 func_0027a580 @ 0027A580 (src/itfMesManager.c), recompiled unchanged */
void func_001A5B70(ItfMesNode *node) {
    for (; node != NULL; node = node->next) {
        if (node->child->flag16 == 0) {
            frFontEnableContextMode(node);
        }
    }
}

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365E8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436608);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436610);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436618);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436620);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436628);

