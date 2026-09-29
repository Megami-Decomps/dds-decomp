#include "common.h"

/* Sized table indexed by itfMesGetTableItem: s16 count + u32 items. */
typedef struct ItfMesTable {
    u8 unk0[0x18]; /* 0x0 */
    s16 count;     /* 0x18 */
    s16 unk1A;     /* 0x1A */
    u32 items[1];  /* 0x1C */
} ItfMesTable;

/* 8-byte entry selected by itfMesGetEntry/itfMesGetNextEntry. */
typedef struct ItfMesEntry {
    u32 unk0;            /* 0x0: item list read by func_0019D5D0 */
    ItfMesTable *table;  /* 0x4: read by func_0019C920 */
} ItfMesEntry;

/* Record behind ItfMesState.sub; itfMesGetEntryCount reads word +0x18. */
typedef struct ItfMesSub {
    u8 unk0[0x18];      /* 0x0 */
    u32 entryCount;        /* 0x18: index of next entry */
    u8 unk1C[4];          /* 0x1C */
    ItfMesEntry entries[1]; /* 0x20 */
} ItfMesSub;

typedef struct FrFontGlyph FrFontGlyph;

/* Block at ItfMesState +0x14. */
typedef struct ItfMesBlk14 {
    u32 unk0;            /* +0x0 */
    u32 unk4;            /* +0x4 */
    FrFontGlyph *unk8;   /* +0x8 */
    u32 unkC;            /* +0xC */
} ItfMesBlk14;

/* Block at ItfMesState +0x24. */
typedef struct ItfMesBlk24 {
    u32 unk0;            /* +0x0 */
    u8 unk4[4];          /* +0x4 */
    ItfMesTable *unk8;   /* +0x8 */
    FrFontGlyph *unkC;   /* +0xC */
    u32 unk10;           /* +0x10 */
} ItfMesBlk24;

/* Block at ItfMesState +0x40. */
typedef struct ItfMesBlk40 {
    u32 unk0;            /* +0x0 */
    u32 unk4;            /* +0x4 */
    FrFontGlyph *unk8;   /* +0x8 */
    u32 unkC;            /* +0xC */
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

/* One 0x14-byte slot per message window. */
typedef struct ItfMesSlot {
    ItfMesState *mes;
    u8 unk4[0x10]; /* 0x4 */
} ItfMesSlot;

/* Item chained off a window node (+0x28); recolored by func_0019DA50. */
typedef struct ItfMesItem {
    u8 unk0[0x10];         /* 0x0 */
    u32 word10;            /* 0x10: low byte is the color */
    u8 flag14;             /* 0x14: byte set by func_0019D9F8 */
    u8 unk15;              /* 0x15 */
    u8 flag16;             /* 0x16: tested by func_0019DB40 */
    u8 unk17[0x11];        /* 0x17 */
    struct ItfMesItem *next; /* 0x28 */
} ItfMesItem;

/* Window chain node walked by func_0019D920/DA50/DB40. */
typedef struct ItfMesNode {
    u8 unk0[4];        /* 0x0 */
    s32 x;            /* 0x4: adjusted with horizontal node offsets */
    s32 y;            /* 0x8: groups nodes on the same row */
    s32 advance;      /* 0xC: accumulated within a row */
    u8 unk10[4];       /* 0x10 */
    s32 unk14;         /* 0x14: set by func_0019D920 */
    u8 unk18[4];       /* 0x18 */
    ItfMesItem *child; /* 0x1C */
    u8 unk20[4];       /* 0x20 */
    struct ItfMesNode *next; /* 0x24 */
} ItfMesNode;

/* Relocatable message blob: magic + fixup table + payload. */
typedef struct ItfMesBin {
    u8 unk0[8];     /* 0x0 */
    u32 magic;      /* 0x8: "MSG0"/"MSG1" */
    u8 unkC[4];     /* 0xC */
    s32 fixupOff;   /* 0x10 */
    s32 fixupSize;  /* 0x14 */
    u8 unk18[4];    /* 0x18 */
    u8 relocated;   /* 0x1C */
    u8 unk1D[3];    /* 0x1D */
    u8 data[1];     /* 0x20: relocated base */
} ItfMesBin;

/* Single-use request block for func_0019D5D0: handle at +4, index at +0x20. */
typedef struct ItfMesIndex {
    u8 unk0[4];        /* 0x0 */
    ItfMesSub *handle; /* 0x4 */
    u8 unk8[0x18]; /* 0x8 */
    u16 index;    /* 0x20 */
} ItfMesIndex;

/* Row of the shade table read by func_0019D888. */
typedef struct ItfMesShade {
    u8 unk0[0x14]; /* 0x0 */
    u8 unk14;      /* 0x14 */
    u8 unk15;      /* 0x15 */
    u8 unk16;      /* 0x16 */
} ItfMesShade;

typedef struct ItfMesColorSrc {
    u8 val0;             /* 0x0 */
    u8 unk1[0x1F];       /* 0x1 */
    ItfMesShade *shade;  /* 0x20 */
} ItfMesColorSrc;

typedef struct ItfMesColorDst {
    u8 unk0[0x12]; /* 0x0 */
    u8 unk12;      /* 0x12 */
    u8 unk13;      /* 0x13 */
    u8 unk14;      /* 0x14 */
    u8 unk15;      /* 0x15 */
} ItfMesColorDst;

/* Globals behind D_003D6EA0: word at +0x4, bitfield at +0xC. */
typedef struct ItfMesGlobals {
    u32 unk0; /* 0x0 */
    u32 unk4; /* 0x4: read by func_0019B870 */
    u32 unk8; /* 0x8 */
    u16 flags; /* 0xC: set/cleared by itfMesSetGlobalFlags/itfMesClearGlobalFlags */
    u16 unkE; /* 0xE */
} ItfMesGlobals;

/* 3 words zeroed by func_0019D0A0. */
typedef struct ItfMesZero {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} ItfMesZero;

/* Operands of func_0019D8B8: word at +0x8, divisor at +0x12. */
typedef struct ItfMesSpan {
    u8 unk0[8]; /* 0x0 */
    s32 unk8;   /* 0x8 */
    u8 unkC[6]; /* 0xC */
    s16 unk12;  /* 0x12 */
} ItfMesSpan;

extern ItfMesSlot D_003D6ECC[];
extern ItfMesGlobals D_003D6EA0;
extern ItfMesZero D_00357D80;

extern u32 D_003BB1E8;

s32 scrGetWindow(void);
s32 func_0010D428(s32 arg0);
void func_0014DAF0(s32 arg0);
void func_0019C060(s32 window, u32 value);
void func_0019C080(s32 window, u32 value);
void func_0019C1E8(s32 window, s32 arg1, s32 arg2);
void itfMesSetWindowHighFlags(s32 window, u32 value);
void itfMesClearWindowHighFlags(s32 window, u32 value);
void func_0019C968(s32 window, s32 arg1, s32 arg2);
void func_0019B4A0(s32 window, s32 arg1, s32 arg2);
void itfMesCleanupWindow(s32 window, s32 arg1);
void itfMesResetWindow(s32 window);
ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 index);
ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub);
u32 itfMesGetTableItem(ItfMesTable *table, s32 index);
void func_0019D0B8(s32 window);
void func_0019C590(s32 window, s32 arg1, s32 arg2, s32 arg3);
void func_0019C9F0(s32 window, s32 arg1, s32 arg2);
void func_00194920(FrFontGlyph *arg0);
void func_0019DDD0(void *arg0, s32 arg1);
void func_0019DDA8(void *arg0, s32 arg1);

void func_002EB278(int *param_1, int param_2, u8 *param_3, int param_4);

s32 itfMesScriptSetPanelValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    func_0019C060(window, func_0010D428(0));
    return 1;
}

s32 itfMesScriptSetWindowValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    func_0019C080(window, func_0010D428(0));
    return 1;
}

s32 itfMesScriptActivatePanel(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowHighFlags(window, 0x200000);
    func_0014DAF0(2);
    return 1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B428);

void func_0019B4A0(s32 window, s32 arg1, s32 arg2) {
    ItfMesState *mes = D_003D6ECC[window].mes;
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

u32 func_0019B530(void) {
    return 1;
}

u32 func_0019B538(void) {
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
    x = func_0010D428(0);
    y = func_0010D428(1);
    width = func_0010D428(2);
    func_0019C590(window, x, y, width);
    return 1;
}

s32 itfMesScriptToggleMessageFlag(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (func_0010D428(0)) {
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
    x = func_0010D428(0);
    y = func_0010D428(1);
    func_0019C1E8(window, x << 4, y << 3);
    return 1;
}

s32 itfMesScriptSetDefaultBounds(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    func_0019C1E8(window, 0x4B0, 0xAF8);
    return 1;
}

s32 itfMesScriptToggleHighFlags(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (func_0010D428(0)) {
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
    func_0019C968(window, func_0010D428(0), 0);
    return 1;
}

s32 itfMesScriptSetMessagePair(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = func_0010D428(0);
    second = func_0010D428(1);
    func_0019C968(window, first, second);
    return 1;
}

s32 itfMesScriptSetMessageRange(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = func_0010D428(0);
    second = func_0010D428(1);
    func_0019C9F0(window, first, second);
    return 1;
}

u32 func_0019B870(void) {
    return D_003D6EA0.unk4;
}

void itfMesSetFlags(u32 flags) {
    D_003BB1E8 |= flags;
}

void itfMesClearFlags(u32 flags) {
    D_003BB1E8 &= ~flags;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B8A8);

void func_0019B9A0(s32 window) {
    if (window >= 0 && D_003D6ECC[window].mes != NULL) {
        func_0019D0B8(window);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B9E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BBF8);

void itfMesCleanupWindow(s32 window, s32 arg1) {
    ItfMesState *mes;
    ItfMesBlk24 *blk24;
    ItfMesBlk14 *blk14;

    if (window < 0) {
        return;
    }
    mes = D_003D6ECC[window].mes;
    blk24 = &mes->blk24;
    blk14 = &mes->blk14;
    if (blk24->unkC != NULL) {
        func_00194920(blk24->unkC);
        blk24->unkC = NULL;
    }
    func_0019DDD0(blk24, 0);
    mes->flags &= ~7;
    mes->flags &= 0xFFFDFFFF;
    if (arg1 == 0) {
        return;
    }
    if (blk14->unk8 != NULL) {
        func_00194920(blk14->unk8);
        blk14->unk8 = NULL;
    }
    func_0019DDA8(blk14, 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BD78);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BEB0);

void itfMesResetWindow(s32 window) {
    ItfMesState *mes = D_003D6ECC[window].mes;
    ItfMesBlk40 *blk = &mes->blk40;

    if (blk->unk8 != NULL) {
        func_00194920(blk->unk8);
        blk->unk8 = NULL;
    }
    blk->unkC = 0;
    blk->unk10 = 0;
    blk->unk16 = 0;
    blk->unk12 = -1;
    mes->flags &= ~0x38;
    mes->flags &= 0xFFFBFFFF;
}

void func_0019C060(s32 window, u32 value) {
    D_003D6ECC[window].mes->blk40.unkC = value;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C080);

void func_0019C0E8(s32 window, s32 x, s32 y) {
    ItfMesBlk14 *blk = &D_003D6ECC[window].mes->blk14;
    s32 delta[2];

    delta[0] = x - blk->unk0;
    delta[1] = y - blk->unk4;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    func_0019D8E8((ItfMesNode *)blk->unk8, delta[0], delta[1]);
    blk->unk0 = x;
    blk->unk4 = y;
}

extern void func_0019D8E8(ItfMesNode *, s32, s32);

void func_0019C178(s32 window, s32 dx, s32 dy) {
    ItfMesBlk14 *blk = &D_003D6ECC[window].mes->blk14;

    func_0019D8E8((ItfMesNode *)blk->unk8, dx, dy);
    blk->unk0 += dx;
    blk->unk4 += dy;
}

void func_0019C1E8(s32 window, s32 x, s32 y) {
    ItfMesBlk24 *blk = &D_003D6ECC[window].mes->blk24;
    s32 delta[2];

    delta[0] = x - blk->unk0;
    delta[1] = y - *(u32 *)blk->unk4;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    func_0019D8E8((ItfMesNode *)blk->unkC, delta[0], delta[1]);
    blk->unk0 = x;
    *(u32 *)blk->unk4 = y;
}

void func_0019C278(s32 window, s32 dx, s32 dy) {
    ItfMesBlk24 *blk = &D_003D6ECC[window].mes->blk24;

    func_0019D8E8((ItfMesNode *)blk->unkC, dx, dy);
    blk->unk0 += dx;
    *(u32 *)blk->unk4 += dy;
}

void func_0019C2E8(s32 window, s32 x, s32 y) {
    ItfMesBlk40 *blk = &D_003D6ECC[window].mes->blk40;
    s32 delta[2];

    delta[0] = x - blk->unk0;
    delta[1] = y - blk->unk4;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    func_0019D8E8((ItfMesNode *)blk->unk8, delta[0], delta[1]);
    blk->unk0 = x;
    blk->unk4 = y;
}

void func_0019C378(s32 window, s32 dx, s32 dy) {
    ItfMesBlk40 *blk = &D_003D6ECC[window].mes->blk40;

    func_0019D8E8((ItfMesNode *)blk->unk8, dx, dy);
    blk->unk0 += dx;
    blk->unk4 += dy;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C3E8);

u32 func_0019C458(s32 window) {
    return D_003D6ECC[window].mes->flags;
}

void func_0019C478(s32 window, u32 value) {
    ItfMesState *mes = D_003D6ECC[window].mes;

    mes->flags = (u32)(u16)mes->flags | (value & 0xffff0000);
}

void itfMesSetWindowHighFlags(s32 window, u32 flags) {
    ItfMesState *mes = D_003D6ECC[window].mes;

    mes->flags |= flags & 0xffff0000;
}

void itfMesClearWindowHighFlags(s32 window, u32 flags) {
    ItfMesState *mes = D_003D6ECC[window].mes;

    mes->flags &= ~flags | 0xffff;
}

s16 func_0019C508(s32 window) {
    return D_003D6ECC[window].mes->blk40.unk12;
}

s16 func_0019C528(s32 window) {
    return D_003D6ECC[window].mes->unk3E;
}

s16 func_0019C548(s32 window) {
    return D_003D6ECC[window].mes->unk3C;
}

u32 func_0019C568(s32 window, s32 index) {
    return D_003D6ECC[window].mes->tableD0[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C590);

void func_0019C838(s32 window, u32 arg1, u32 arg2) {
    func_0019D460((u32)D_003D6ECC[window].mes, arg1, arg2, 0);
}

void func_0019C868(s32 window) {
    func_0019D460((u32)D_003D6ECC[window].mes);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C898);

u32 func_0019C920(s32 window, s32 arg1, s32 arg2) {
    return itfMesGetTableItem(itfMesGetEntry(D_003D6ECC[window].mes, arg1)->table, arg2);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C968);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C9F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CAB8);

void func_0019CB78(s32 window, u8 value) {
    D_003D6ECC[window].mes->unk39 = value;
}

void func_0019CB98(s32 window, u32 value) {
    D_003D6ECC[window].mes->unk1DC = value;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CBB8);

void itfMesSetGlobalFlags(u32 bits) {
    D_003D6EA0.flags |= bits;
}

void itfMesClearGlobalFlags(u32 bits) {
    D_003D6EA0.flags &= ~bits;
}

u16 itfMesGetGlobalFlags(void) {
    return D_003D6EA0.flags;
}

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1480);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1490);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14A0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCD8);

void func_0019D0A0(void) {
    D_00357D80.unk0 = 0;
    D_00357D80.unk4 = 0;
    D_00357D80.unk8 = 0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D0B8);

/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void itfMesRelocate(ItfMesBin *bin) {
    if (bin->relocated == 0) {
        func_002EB278((int *)bin->data, (int)bin->data,
                      (u8 *)bin + bin->fixupOff, bin->fixupSize);
        bin->relocated = 1;
    }
}

u32 itfMesIsMsgData(ItfMesBin *bin) {
    u32 valid;

    valid = 0;
    if (bin->magic == 0x3047534d || bin->magic == 0x3147534d) { /* "MSG0"/"MSG1" */
        valid = 1;
    }
    return valid;
}

ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 index) {
    ItfMesEntry *entries = mes->sub->entries;

    return &entries[index];
}

ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub) {
    ItfMesEntry *entries = sub->entries;

    return &entries[sub->entryCount];
}

u32 func_0019D208(s32 window, s32 index) {
    return itfMesGetEntry(D_003D6ECC[window].mes, index)->unk0;
}

u32 itfMesGetEntryCount(s32 window) {
    return D_003D6ECC[window].mes->sub->entryCount;
}

u32 itfMesGetTableItem(ItfMesTable *table, s32 index) {
    s32 count = table->count;

    if (index < 0 || index >= count) {
        return 0;
    }
    return table->items[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D298);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D460);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D580);

u32 func_0019D5D0(ItfMesIndex *req) {
    u32 *table;

    table = *(u32 **)itfMesGetNextEntry(req->handle);
    return table[req->index];
}

s32 itfMesCountZeroBits(s32 bits, u32 value) {
    s32 count;
    u32 bit;

    count = 0;
    while (bits > 0) {
        bit = value & 1;
        value >>= 1;
        bits--;
        if (bit == 0) {
            count++;
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D640);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D730);

ItfMesNode *itfMesGetLastNode(ItfMesNode *node) {
    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

void func_0019D888(ItfMesColorSrc *src, ItfMesColorDst *dst) {
    ItfMesShade *shade = src->shade;

    dst->unk15 = src->val0 >> 1;
    dst->unk12 = shade->unk15;
    dst->unk13 = shade->unk14;
    dst->unk14 = shade->unk16;
}

s32 func_0019D8B8(ItfMesSpan *arg0, ItfMesSpan *arg1) {
    return ((arg1->unk8 - arg0->unk8) >> 3) / arg1->unk12 + 1;
}

void func_0019D8E8(ItfMesNode *node, s32 arg1, s32 arg2) {
    if (node == NULL) {
        return;
    }
    do {
        node->x += arg1;
        node->y += arg2;
        node = node->next;
    } while (node != NULL);
}

/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void func_0019D920(ItfMesNode *node, s32 value) {
    while (node != NULL) {
        node->unk14 = value;
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D958);

void func_0019D9F8(ItfMesNode *node, u8 value) {
    ItfMesItem *item;

    for (; node != NULL; node = node->next) {
        for (item = node->child; item != NULL; item = item->next) {
            item->flag14 = value;
        }
    }
}

/* Persona 4 func_0027a4d0 @ 0027A4D0 (src/itfMesManager.c), recompiled unchanged */
void func_0019DA50(ItfMesNode *node, u32 color) {
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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019DAF0);

/* Persona 4 func_0027a580 @ 0027A580 (src/itfMesManager.c), recompiled unchanged */
void func_0019DB40(ItfMesNode *node) {
    for (; node != NULL; node = node->next) {
        if (node->child->flag16 == 0) {
            func_00195388(node);
        }
    }
}

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB1E8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB1F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB1F8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB208);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB210);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB218);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB220);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB228);

