#include "common.h"

/* Sized table indexed by itfMesGetTableItem: s16 count + u32 items. */
typedef struct ItfMesTable {
    u8 unk0[0x18]; /* 0x0 */
    s16 count;     /* 0x18 */
    s16 bitCount; /* 0x1A: number of selection-mask bits / option rows */
    u32 items[1];  /* 0x1C */
} ItfMesTable;

/* 8-byte entry selected by itfMesGetEntry/itfMesGetNextEntry. */
typedef struct ItfMesEntry {
    u32 itemList;        /* 0x0: selected by itfMesGetNextEntrySelectedItem */
    ItfMesTable *table;  /* 0x4: read by itfMesGetEntryTableItem */
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
    u32 x;               /* +0x0 */
    u32 y;               /* +0x4 */
    FrFontGlyph *glyphChain; /* +0x8: positioned and released with this block */
    u32 unkC;            /* +0xC */
} ItfMesBlk14;

/* Block at ItfMesState +0x24. */
typedef struct ItfMesBlk24 {
    u32 x;               /* +0x0 */
    u32 y;               /* +0x4 */
    ItfMesTable *unk8;   /* +0x8 */
    FrFontGlyph *glyphChain; /* +0xC: current entry's glyphs */
    u32 unk10;           /* +0x10 */
    u8 unk14;            /* +0x14 */
    u8 unk15;            /* +0x15: set by func_0019CB78 */
    u8 unk16[2];         /* +0x16 */
    s16 unk18;           /* +0x18: read by func_0019C548 */
    s16 unk1A;           /* +0x1A: read by func_0019C528 */
} ItfMesBlk24;

/* Block at ItfMesState +0x40. */
typedef struct ItfMesBlk40 {
    u32 x;               /* +0x0 */
    u32 y;               /* +0x4 */
    FrFontGlyph *glyphChain; /* +0x8: option-list glyphs */
    u32 panelValue;      /* +0xC: bit mask set by itfMesScriptSetPanelValue */
    u16 unk10;           /* +0x10 */
    s16 clearBitCount;   /* +0x12: cached by itfMesCountClearBits */
    u16 unk14;           /* +0x14 */
    s16 rowCount;        /* +0x16: number of displayed option rows */
    u8 unk18[0xA];       /* +0x18 */
    u16 optionCount;     /* +0x22: entries in use */
    struct {
        s16 id;          /* +0x24 + 4 * n */
        s16 value;       /* +0x26 + 4 * n */
    } options[15];
} ItfMesBlk40;

/* Block at ItfMesState +0xA4. */
typedef struct ItfMesBlkA4 {
    u8 unk0[4];          /* +0x0 */
    void *unk4;          /* +0x4: released by itfMesSetWindowPageAndRefresh */
    u32 panelHandle;       /* +0x8: panel handle */
    u8 unkC[0x1C];       /* +0xC */
    u32 unk28;           /* +0x28 */
} ItfMesBlkA4;

/* Message-window state behind each ItfMesSlot. */
typedef struct ItfMesState {
    u32 flags;          /* 0x0: low half status, high half mask */
    ItfMesSub *sub;     /* 0x4 */
    s32 temporaryFontEntry; /* 0x8: optional entry passed to frFontLoadTemporaryEntry */
    s32 renderValue;    /* 0xC: propagated to glyph nodes and option frame */
    s16 unk10;          /* 0x10 */
    s16 unk12;          /* 0x12 */
    ItfMesBlk14 blk14;    /* 0x14: passed to itfResetCursorPositionAndState */
    ItfMesBlk24 blk24;    /* 0x24: passed to itfMesResetCursorState */
    ItfMesBlk40 blk40;  /* 0x40 */
    u8 unkA0[4];        /* 0xA0 */
    ItfMesBlkA4 blkA4;  /* 0xA4 */
    u32 tableD0[1];  /* 0xD0: indexed by itfMesGetWindowTableValue (true length unknown) */
    u8 unkD4[0x108]; /* 0xD4 */
    u32 callbackAddress; /* 0x1DC: invoked when glyph command 4 is set */
} ItfMesState;

/* One 0x14-byte slot per message window. */
typedef struct ItfMesSlot {
    ItfMesState *mes;
    u8 unk4[0x10]; /* 0x4 */
} ItfMesSlot;

/* Item chained off a window node (+0x28); recolored by itfMesRecolorNodeChildren. */
typedef struct ItfMesItem {
    u8 unk0[0x10];         /* 0x0 */
    u32 word10;            /* 0x10: low byte is the color */
    u8 flag14;             /* 0x14: byte set by itfMesSetChildChainFlags */
    u8 unk15;              /* 0x15 */
    u8 flag16;             /* 0x16: tested by itfMesEnableUnflaggedNodeContexts */
    u8 unk17[0x11];        /* 0x17 */
    struct ItfMesItem *next; /* 0x28 */
} ItfMesItem;

/* Window chain node walked by itfMesSetNodeChainRenderValue/DA50/DB40. */
typedef struct ItfMesNode {
    u8 unk0[4];        /* 0x0 */
    s32 x;            /* 0x4: adjusted with horizontal node offsets */
    s32 y;            /* 0x8: groups nodes on the same row */
    s32 advance;      /* 0xC: accumulated within a row */
    u8 unk10[2];       /* 0x10 */
    s16 rowHeightUnits; /* 0x12: row height in 1/8 units */
    s32 renderValue;    /* 0x14: copied from message-window renderValue */
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

/* Single-use request block for itfMesGetNextEntrySelectedItem: handle at +4, index at +0x20. */
typedef struct ItfMesIndex {
    u8 unk0[4];        /* 0x0 */
    ItfMesSub *handle; /* 0x4 */
    u8 unk8[0x18]; /* 0x8 */
    u16 index;    /* 0x20 */
} ItfMesIndex;

/* Row of the shade table read by itfMesCopyGlyphShade. */
typedef struct ItfMesShade {
    u8 pad00[0x14]; /* 0x0 */
    u8 shade14;     /* 0x14 */
    u8 shade15;     /* 0x15 */
    u8 shade16;     /* 0x16 */
} ItfMesShade;

typedef struct ItfMesColorSrc {
    u8 firstByte;        /* 0x0 */
    u8 pad01[0x1F];      /* 0x1 */
    ItfMesShade *shade;  /* 0x20 */
} ItfMesColorSrc;

typedef struct ItfMesColorDst {
    u8 pad00[0x12]; /* 0x0 */
    u8 shade12;     /* 0x12 */
    u8 shade13;     /* 0x13 */
    u8 shade14;     /* 0x14 */
    u8 shade15;     /* 0x15 */
} ItfMesColorDst;

/* Node of the pool at itfMesWork + 0x10 (0x14 bytes each). */
typedef struct ItfMesPoolNode {
    struct ItfMesPoolNode *previous; /* 0x0 */
    struct ItfMesPoolNode *next;     /* 0x4 */
    s32 index;                       /* 0x8 */
    s32 stateAddress;                /* 0xC: retained message-window state */
    s32 resourceHandle;              /* 0x10: window allocation handle */
} ItfMesPoolNode;

typedef struct ItfMesPool {
    ItfMesPoolNode *activeHead; /* 0x0 */
    ItfMesPoolNode *activeTail; /* 0x4 */
    ItfMesPoolNode *firstFree;  /* 0x8 */
    ItfMesPoolNode *lastFree;   /* 0xC */
} ItfMesPool;

/* Globals behind itfMesWork: word at +0x4, bitfield at +0xC. */
typedef struct ItfMesGlobals {
    u32 activeWindowCount; /* 0x0: incremented on creation, decremented on destruction */
    u32 windowTexture;     /* 0x4: /itf/MESWIN.TMX resource */
    u32 unk8; /* 0x8 */
    u16 flags; /* 0xC: set/cleared by itfMesSetGlobalFlags/itfMesClearGlobalFlags */
    u16 unkE; /* 0xE */
    ItfMesPool pool; /* 0x10 */
    ItfMesPoolNode nodes[0x40]; /* 0x20 */
} ItfMesGlobals;

/* 3 words zeroed by itfMesClearGlobalWords. */
typedef struct ItfMesZero {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} ItfMesZero;

/* Operands of itfMesCountSpanSteps: word at +0x8, divisor at +0x12. */
typedef struct ItfMesSpan {
    u8 unk0[8]; /* 0x0 */
    s32 y;      /* 0x8: node's vertical position */
    u8 unkC[6]; /* 0xC */
    s16 rowHeightUnits; /* 0x12: height divisor after converting y to eighths */
} ItfMesSpan;

extern ItfMesSlot itfWindowSlots[];

extern ItfMesGlobals itfMesWork;

extern ItfMesZero D_00357D80;

extern u32 itfMessageFlags;

s32 scrGetWindow(void);

s32 scrReadIntParameter(s32 arg0);

void func_0014DAF0(s32 arg0);

void itfMesSetWindowPanelValue(s32 window, u32 value);

void itfMesCountClearBits(s32 window, s32 value);

void itfMesBlk24MoveTo(s32 window, s32 arg1, s32 arg2);

void itfMesSetWindowHighFlags(s32 window, u32 value);

void itfMesClearWindowHighFlags(s32 window, u32 value);

void itfMesSetWindowPageAndRefresh(s32 window, s32 arg1, s32 arg2);

void itfMesFinishWindowAndClearStatus(s32 window);

void itfMesCleanupWindow(s32 window, s32 alsoSecondary);

void itfMesResetWindow(s32 window);

ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 index);

ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub);

u32 itfMesGetTableItem(ItfMesTable *table, s32 index);

void itfMesRelocate(ItfMesBin *bin);

void itfMesDestroyWindow(s32 window);

void func_0019C590(s32 window, s32 arg1, s32 arg2, s32 arg3);

void func_0019C9F0(s32 window, s32 arg1, s32 arg2);

void frFontQueueGlyphInSelectedSlot(FrFontGlyph *arg0);

void itfMesResetCursorState(void *arg0, s32 arg1);

void itfResetCursorPositionAndState(void *arg0, s32 arg1);

void sdfRelocatePackedResourceWords(int *param_1, int param_2, u8 *param_3, int param_4);

extern void func_00196BC0();

s32 itfDrawCustomColorText();

s32 func_00195E60();

void func_00196088();

void itfMesEnableUnflaggedNodeContexts(ItfMesNode *node);

void itfMesCopyGlyphShade();

s8 itfTestTextInterfaceMask(s32 arg0);

s32 func_00196BD8();

s32 func_00196BE0();

void evtLipsExecFunction();

void itfMesSetNodeChainRenderValue(ItfMesNode *node, s32 value);

ItfMesNode *itfMesGetLastNode(ItfMesNode *node);

s32 itfMesCountSpanSteps();

void itfMesInitCharTable(s32 *table);

s32 itfMesMaxGroupedExtent(ItfMesNode *node);

extern void frFontLoadTemporaryEntry(u32 arg0);

extern ItfMesNode *itfDrawDefaultColorText(s32 x, s32 y, s32 encodedText, s32 sub);

extern u32 itfLoadTextureFromAsset(const char *path);

extern void itfInitPool(ItfMesPool *pool, ItfMesPoolNode *nodes, s32 count, s32 stride);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern s32 sndVisitQueuedResources(void);

extern void sndFlushMessageQueue(void);

extern void func_0019F6A0(void);

extern struct ItfMesPoolNode *itfAcquirePoolNode();

extern u32 sdfAllocGeneralBlock();

extern u32 sdfResourceRetainAddress();

extern void itfInitializeCursorResetState();

extern void itfResetWindowResourceBlock();

extern void itfClearDrawStateWords();

extern void itfResetBattleFadeState();

extern s32 func_00195ED8();

extern s32 func_00199828();

extern void itfSetPanelLayoutAndNotify();

extern void itfPanelUpdateValuesAndNotify();

s32 itfMesScriptSetPanelValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowPanelValue(window, scrReadIntParameter(0));
    return 1;
}

s32 itfMesScriptSetWindowValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesCountClearBits(window, scrReadIntParameter(0));
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

s32 itfMesFinishScriptWindowIfActive(void) {
    s32 window = scrGetWindow();
    ItfMesState *mes;
    u32 state;

    if (window < 0) {
        return 1;
    }
    mes = itfWindowSlots[window].mes;
    state = mes->flags & 0x300;
    if (state == 0x100 || state == 0x200) {
        itfMesFinishWindowAndClearStatus(window);
        func_0014DAF0(3);
    }
    return state < 1;
}

void itfMesFinishWindowAndClearStatus(s32 window) {
    ItfMesState *mes = itfWindowSlots[window].mes;
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
    x = scrReadIntParameter(0);
    y = scrReadIntParameter(1);
    width = scrReadIntParameter(2);
    func_0019C590(window, x, y, width);
    return 1;
}

s32 itfMesScriptToggleMessageFlag(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (scrReadIntParameter(0)) {
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
    x = scrReadIntParameter(0);
    y = scrReadIntParameter(1);
    itfMesBlk24MoveTo(window, x << 4, y << 3);
    return 1;
}

s32 itfMesScriptSetDefaultBounds(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesBlk24MoveTo(window, 0x4B0, 0xAF8);
    return 1;
}

s32 itfMesScriptToggleHighFlags(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (scrReadIntParameter(0)) {
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
    itfMesSetWindowPageAndRefresh(window, scrReadIntParameter(0), 0);
    return 1;
}

s32 itfMesScriptSetMessagePair(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    itfMesSetWindowPageAndRefresh(window, first, second);
    return 1;
}

s32 itfMesScriptSetMessageRange(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_0019C9F0(window, first, second);
    return 1;
}

u32 itfMesGetGlobalWindowValue(void) {
    return itfMesWork.windowTexture;
}

void itfMesSetFlags(u32 flags) {
    itfMessageFlags |= flags;
}

void itfMesClearFlags(u32 flags) {
    itfMessageFlags &= ~flags;
}

s32 itfMesCreateWindow(ItfMesSub *sub) {
    ItfMesPoolNode *node = itfAcquirePoolNode(&itfMesWork.pool);
    s32 window = node->index;
    ItfMesState *mes;
    u32 handle;

    handle = sdfAllocGeneralBlock(0x1E0);
    node->resourceHandle = handle;
    mes = (ItfMesState *)sdfResourceRetainAddress(handle);
    node->stateAddress = (s32)mes;
    mes->sub = NULL;
    itfMesSetSubResource(window, sub);
    mes->flags = 0;
    mes->unk10 = 0x53;
    mes->renderValue = 0xFFFFF0;
    mes->unk12 = 0;
    mes->callbackAddress = 0;
    itfResetCursorPositionAndState(&mes->blk14, 1);
    itfMesResetCursorState(&mes->blk24, 1);
    itfInitializeCursorResetState(&mes->blk40);
    itfResetWindowResourceBlock(&mes->blkA4);
    itfClearDrawStateWords(mes->tableD0);
    itfResetBattleFadeState((u8 *)mes + 0x1D0, 0);
    itfMesWork.activeWindowCount++;
    return window;
}

void itfMesDestroyWindowIfPresent(s32 window) {
    if (window >= 0 && itfWindowSlots[window].mes != NULL) {
        itfMesDestroyWindow(window);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesStartEntry);

extern s32 itfInitTextDrawArgs(s32 encodedText, s32 sub);

s32 itfMesSelectTableItemText(s32 window, s32 entryIndex, s32 item) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesTable *table = itfMesGetEntry(mes, entryIndex)->table;
    ItfMesBlk24 *blk = &mes->blk24;

    blk->unk18 = item;
    blk->unk8 = table;
    blk->unk1A = table->count;
    if (table->count != 0) {
        return itfInitTextDrawArgs(itfMesGetTableItem(table, (s16)item), 0);
    }
    return 0;
}

void itfMesCleanupWindow(s32 window, s32 alsoSecondary) {
    ItfMesState *mes;
    ItfMesBlk24 *blk24;
    ItfMesBlk14 *blk14;

    if (window < 0) {
        return;
    }
    mes = itfWindowSlots[window].mes;
    blk24 = &mes->blk24;
    blk14 = &mes->blk14;
    if (blk24->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(blk24->glyphChain);
        blk24->glyphChain = NULL;
    }
    itfMesResetCursorState(blk24, 0);
    mes->flags &= ~7;
    mes->flags &= 0xFFFDFFFF;
    if (alsoSecondary == 0) {
        return;
    }
    if (blk14->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(blk14->glyphChain);
        blk14->glyphChain = NULL;
    }
    itfResetCursorPositionAndState(blk14, 0);
}

void itfMesBuildOptionFrame(ItfMesState *mes) {
    ItfMesBlk40 *blk = &mes->blk40;
    ItfMesBlkA4 *blkA4 = &mes->blkA4;
    s32 rect[4];
    s32 width = 0;
    s32 height;
    s32 half;
    s32 i;
    s32 w;

    func_00196088(0x1000, 0x4B0, blk->glyphChain);
    height = blk->rowCount * 21 * 8;
    for (i = 0; i < blk->rowCount; i++) {
        w = func_00195ED8(i, blk->glyphChain);
        if (width < w) {
            width = w;
        }
    }
    half = width / 2;
    rect[0] = 0xE00 - half;
    rect[1] = 0x430;
    rect[2] = 0x1200 + half;
    rect[3] = 0x530 + height;
    blkA4->panelHandle = func_00199828(9, itfMesWork.windowTexture);
    itfSetPanelLayoutAndNotify(blkA4->panelHandle, rect[0], rect[1], rect[2], rect[3], mes->renderValue);
    itfPanelUpdateValuesAndNotify(blkA4->panelHandle, 0, 0, 0, 0);
    mes->flags = (mes->flags & ~0xC00) | 0x400;
}

void itfMesBuildOptionList(s32 window, s32 entryIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlk40 *blk = &mes->blk40;
    ItfMesEntry *entry = itfMesGetEntry(mes, entryIndex);
    ItfMesTable *table;
    s32 count;
    s32 y;

    if (blk->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(blk->glyphChain);
        blk->glyphChain = NULL;
    }
    table = entry->table;
    count = itfMesCountZeroBits(table->bitCount, blk->panelValue);
    y = blk->y - ((count - 1) * 21 << 3);
    itfMesInitCharTable((s32 *)mes->tableD0);
    if (mes->temporaryFontEntry != 0) {
        frFontLoadTemporaryEntry(mes->temporaryFontEntry);
    }
    blk->glyphChain = (FrFontGlyph *)itfMesBuildNodeRows(&table->items[1], table->bitCount, blk->panelValue, blk->x, y, mes->renderValue);
    blk->rowCount = count;
    mes->flags = (mes->flags & ~0x38) | 0x10;
    if (mes->unk12 == 3) {
        itfMesBuildOptionFrame(mes);
    }
    blk->unk10 = 1;
}

void itfMesResetWindow(s32 window) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlk40 *blk = &mes->blk40;

    if (blk->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(blk->glyphChain);
        blk->glyphChain = NULL;
    }
    blk->panelValue = 0;
    blk->unk10 = 0;
    blk->rowCount = 0;
    blk->clearBitCount = -1;
    mes->flags &= ~0x38;
    mes->flags &= 0xFFFBFFFF;
}

void itfMesSetWindowPanelValue(s32 window, u32 value) {
    itfWindowSlots[window].mes->blk40.panelValue = value;
}

/* Count the zero bits below the panel value's low bit within the given range. */
void itfMesCountClearBits(s32 window, s32 value) {
    ItfMesBlk40 *blk = &itfWindowSlots[window].mes->blk40;
    u32 bits = blk->panelValue;
    s32 bit = bits & 1;
    s32 zeros = bit == 0;
    s32 i = 0;

    if (value > 0) {
        do {
            i++;
            bits >>= 1;
            bit = bits & 1;
            if (bit == 0) {
                zeros++;
            }
        } while (i < value);
    }
    zeros = bit == 0 ? zeros - 1 : 0;
    blk->clearBitCount = zeros;
    blk->unk14 = zeros;
}

void itfMesBlk14MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk14 *blk = &itfWindowSlots[window].mes->blk14;
    s32 delta[2];

    delta[0] = x - blk->x;
    delta[1] = y - blk->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, delta[0], delta[1]);
    blk->x = x;
    blk->y = y;
}

extern void itfMesOffsetNodeChain(ItfMesNode *, s32, s32);

void itfMesBlk14MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk14 *blk = &itfWindowSlots[window].mes->blk14;

    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, dx, dy);
    blk->x += dx;
    blk->y += dy;
}

void itfMesBlk24MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk24 *blk = &itfWindowSlots[window].mes->blk24;
    s32 delta[2];

    delta[0] = x - blk->x;
    delta[1] = y - blk->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, delta[0], delta[1]);
    blk->x = x;
    blk->y = y;
}

void itfMesBlk24MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk24 *blk = &itfWindowSlots[window].mes->blk24;

    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, dx, dy);
    blk->x += dx;
    blk->y += dy;
}

void itfMesBlk40MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk40 *blk = &itfWindowSlots[window].mes->blk40;
    s32 delta[2];

    delta[0] = x - blk->x;
    delta[1] = y - blk->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, delta[0], delta[1]);
    blk->x = x;
    blk->y = y;
}

void itfMesBlk40MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk40 *blk = &itfWindowSlots[window].mes->blk40;

    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, dx, dy);
    blk->x += dx;
    blk->y += dy;
}

void func_0019C3E8(s32 window, s32 value) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    if (mes->renderValue == value) {
        return;
    }
    itfMesSetNodeChainRenderValue((ItfMesNode *)mes->blk14.glyphChain, value);
    itfMesSetNodeChainRenderValue((ItfMesNode *)mes->blk24.glyphChain, value);
    itfMesSetNodeChainRenderValue((ItfMesNode *)mes->blk40.glyphChain, value);
    mes->renderValue = value;
}

u32 itfMesGetWindowFlags(s32 window) {
    return itfWindowSlots[window].mes->flags;
}

void itfMesReplaceWindowHighFlags(s32 window, u32 value) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    mes->flags = (u32)(u16)mes->flags | (value & 0xffff0000);
}

void itfMesSetWindowHighFlags(s32 window, u32 flags) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    mes->flags |= flags & 0xffff0000;
}

void itfMesClearWindowHighFlags(s32 window, u32 flags) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    mes->flags &= ~flags | 0xffff;
}

s16 func_0019C508(s32 window) {
    return itfWindowSlots[window].mes->blk40.clearBitCount;
}

s16 func_0019C528(s32 window) {
    return itfWindowSlots[window].mes->blk24.unk1A;
}

s16 func_0019C548(s32 window) {
    return itfWindowSlots[window].mes->blk24.unk18;
}

u32 itfMesGetWindowTableValue(s32 window, s32 index) {
    return itfWindowSlots[window].mes->tableD0[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C590);

void itfMesCopyStringToWindowTableSlot(s32 window, u32 entryIndex, u32 itemIndex) {
    func_0019D460((u32)itfWindowSlots[window].mes, entryIndex, itemIndex, 0);
}

void func_0019C868(s32 window) {
    func_0019D460((u32)itfWindowSlots[window].mes);
}

/* Install a new message record and stash its fixup word in the state. */
ItfMesSub *itfMesSetSubResource(s32 window, ItfMesSub *sub) {
    ItfMesState *mes = itfWindowSlots[window].mes;
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
    mes->temporaryFontEntry = value;
    return previous;
}

u32 itfMesGetEntryTableItem(s32 window, s32 entryIndex, s32 itemIndex) {
    return itfMesGetTableItem(itfMesGetEntry(itfWindowSlots[window].mes, entryIndex)->table, itemIndex);
}

extern void func_0019E048(ItfMesBlkA4 *blk, s32 arg1, s32 arg2);
extern void itfPanelReleasePrimitiveResources(void *primitive);

void itfMesSetWindowPageAndRefresh(s32 window, s32 arg1, s32 arg2) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlkA4 *blk = &mes->blkA4;

    if ((mes->flags & 0x3300) == 0) {
        if (mes->unk12 != arg1 || blk->unk28 != arg2) {
            mes->unk12 = arg1;
            func_0019E048(blk, arg1, arg2);
            if (blk->unk4 != NULL) {
                itfPanelReleasePrimitiveResources(blk->unk4);
                blk->unk4 = NULL;
            }
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C9F0);

s32 itfMesMeasureEntryItem(s32 window, s32 entryIndex, s32 itemIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesTable *table;
    u32 item;
    ItfMesNode *glyph;
    s32 extent;

    table = itfMesGetEntry(mes, entryIndex)->table;
    if (table->count == 0) {
        return 0;
    }
    itfMesInitCharTable((s32 *)mes->tableD0);
    if (mes->temporaryFontEntry != 0) {
        frFontLoadTemporaryEntry(mes->temporaryFontEntry);
    }
    item = itfMesGetTableItem(table, itemIndex);
    if (item == 0) {
        return item;
    }
    glyph = itfDrawDefaultColorText(0, 0, item, 0);
    extent = itfMesMaxGroupedExtent(glyph);
    frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyph);
    return extent;
}

void func_0019CB78(s32 window, u8 value) {
    itfWindowSlots[window].mes->blk24.unk15 = value;
}

void itfMesSetWindowCallbackAddress(s32 window, u32 value) {
    itfWindowSlots[window].mes->callbackAddress = value;
}

void itfMesInit(void) {
    ItfMesPoolNode *node;

    itfMesWork.activeWindowCount = 0;
    itfMesWork.unk8 = 0;
    itfMesWork.flags = 0;
    itfMesWork.windowTexture = itfLoadTextureFromAsset("/itf/MESWIN.TMX");
    itfInitPool(&itfMesWork.pool, itfMesWork.nodes, 0x40, 0x14);
    for (node = itfMesWork.pool.firstFree; node != NULL; node = node->next) {
        node->stateAddress = 0;
    }
    kwlnTaskCreate("CalcMsgMng", 0x409, 0, 0, (void (*)(void))sndVisitQueuedResources, sndFlushMessageQueue, NULL);
    kwlnTaskCreate("DrawMsgMng", 0x2B1A, 0, 0, func_0019F6A0, sndFlushMessageQueue, NULL);
}

void itfMesSetGlobalFlags(u32 bits) {
    itfMesWork.flags |= bits;
}

void itfMesClearGlobalFlags(u32 bits) {
    itfMesWork.flags &= ~bits;
}

u16 itfMesGetGlobalFlags(void) {
    return itfMesWork.flags;
}

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1480);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1490);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14A0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCD8);

void itfMesClearGlobalWords(void) {
    D_00357D80.unk0 = 0;
    D_00357D80.unk4 = 0;
    D_00357D80.unk8 = 0;
}

/* Slot record reached through the pool base, one 0x14 bytes per window. */
typedef struct ItfMesWindowRec {
    u8 pad00[0xC];
    ItfMesState *mes;
    s32 handle;
} ItfMesWindowRec;

extern ItfMesWindowRec D_003D6EC0[];

extern void btlReleaseEffectResourceHandles();

extern void itfReleaseUiResourceSlotHandles(s32 *arg0);

extern void sdfReleaseResourceAllocation(u32 allocation);

extern void itfReleasePoolNode();

void itfMesDestroyWindow(s32 window) {
    ItfMesWindowRec *rec = &D_003D6EC0[window];
    ItfMesState *mes = rec->mes;

    if (mes != NULL) {
        itfMesCleanupWindow(window, 1);
        itfMesResetWindow(window);
        btlReleaseEffectResourceHandles(mes);
        itfReleaseUiResourceSlotHandles(mes->tableD0);
        mes->flags = 0;
        sdfReleaseResourceAllocation(rec->handle);
        rec->mes = NULL;
        itfReleasePoolNode(rec, (u8 *)D_003D6EC0 - 0x10);
        *(s32 *)((u8 *)D_003D6EC0 - 0x20) -= 1;
    }
}

/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void itfMesRelocate(ItfMesBin *bin) {
    if (bin->relocated == 0) {
        sdfRelocatePackedResourceWords((int *)bin->data, (int)bin->data,
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

u32 itfMesGetWindowEntryItems(s32 window, s32 index) {
    return itfMesGetEntry(itfWindowSlots[window].mes, index)->itemList;
}

u32 itfMesGetEntryCount(s32 window) {
    return itfWindowSlots[window].mes->sub->entryCount;
}

u32 itfMesGetTableItem(ItfMesTable *table, s32 index) {
    s32 count = table->count;

    if (index < 0 || index >= count) {
        return 0;
    }
    return table->items[index];
}

/* Rebuild the current entry's glyph chain and cache it on blk24.
 * Keep byte-offset accesses here: typed block accesses disturb ee-gcc register allocation. */
void itfMesBuildEntryGlyph(ItfMesState *mes) {
    u8 *m = (u8 *)mes;
    u8 *blk = m + 0x24;
    s32 glyph;
    s32 handle;
    s32 helper;
    s8 flags;
    void (*hook)();

    handle = *(s32 *)(blk + 0xC);
    if (handle != 0) {
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)handle);
        *(s32 *)(blk + 0xC) = 0;
    }
    glyph = itfDrawCustomColorText(*(s32 *)(m + 0x24), *(s32 *)(blk + 4), blk[0x12], blk[0x13], blk[0x14], blk[0x15],
                          itfMesGetTableItem(*(ItfMesTable **)(blk + 8), *(s16 *)(blk + 0x18)), 0);
    if (*(s16 *)(m + 0x12) == 3) {
        if (func_00195E60(glyph) == 1) {
            func_00196088(0x1000, 0xC60, glyph);
        } else {
            func_00196088(0x1000, 0xBF8, glyph);
        }
    }
    if (!(*(u32 *)m & 0x400000) && (itfMesWork.flags & 1)) {
        itfMesEnableUnflaggedNodeContexts((ItfMesNode *)glyph);
    }
    itfMesCopyGlyphShade(glyph, blk);
    flags = itfTestTextInterfaceMask(3);
    blk[0x11] = flags;
    if (flags & 2) {
        *(u32 *)m |= 0x10000;
    } else {
        *(u32 *)m &= 0xFFFEFFFF;
    }
    if (itfTestTextInterfaceMask(4) != 0) {
        hook = *(void (**)())(m + 0x1DC);
        if (hook != NULL) {
            hook();
        }
    }
    if (itfTestTextInterfaceMask(8) != 0) {
        helper = func_00196BD8();
        evtLipsExecFunction(helper, func_00196BE0());
    }
    itfMesSetNodeChainRenderValue((ItfMesNode *)glyph, *(s32 *)(m + 0xC));
    *(s16 *)(blk + 0x16) = itfMesCountSpanSteps(itfMesGetLastNode(glyph), glyph);
    *(s32 *)(blk + 0xC) = glyph;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D460);

void itfMesInitCharTable(s32 *table) {
    s32 i;
    for (i = 0; i < 0x20; i++) {
        func_00196BC0(i, table[i]);
    }
}

u32 itfMesGetNextEntrySelectedItem(ItfMesIndex *req) {
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

/* Build one node per clear bit of mask, stacking them downward from y. */
ItfMesNode *itfMesBuildNodeRows(u32 *items, s32 count, u32 mask, s32 x, s32 y, s32 value) {
    ItfMesNode *node = NULL;
    s32 i;

    for (i = 0; i < count; i++, items++) {
        if (mask & 1) {
            mask >>= 1;
        } else {
            node = (ItfMesNode *)itfDrawCustomColorText(x, y, 0, 0, 0, 0x80, *items, node);
            mask >>= 1;
            y += node->rowHeightUnits << 3;
        }
    }
    if (node != NULL) {
        itfMesSetNodeChainRenderValue(node, value);
        itfMesRecolorNodeChildren(node, 0x80);
    }
    return node;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D730);

ItfMesNode *itfMesGetLastNode(ItfMesNode *node) {
    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

void itfMesCopyGlyphShade(ItfMesColorSrc *src, ItfMesColorDst *dst) {
    ItfMesShade *shade = src->shade;

    dst->shade15 = src->firstByte >> 1;
    dst->shade12 = shade->shade15;
    dst->shade13 = shade->shade14;
    dst->shade14 = shade->shade16;
}

s32 itfMesCountSpanSteps(ItfMesSpan *last, ItfMesSpan *first) {
    return ((first->y - last->y) >> 3) / first->rowHeightUnits + 1;
}

/* Translate every node in a linked row without disturbing its advance. */
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
void itfMesSetNodeChainRenderValue(ItfMesNode *node, s32 value) {
    while (node != NULL) {
        node->renderValue = value;
        node = node->next;
    }
}

/* Skip to the row (last - first - 1) rows below the node, then set the flag
 * byte on every child of that row's nodes. */
void itfMesSetRowItemFlag(ItfMesNode *node, s32 first, s32 last, s32 value) {
    s32 rows = last - first - 1;
    s32 row = node->y;
    s32 cur = row;
    ItfMesItem *item;
    u8 color;

    while (rows > 0) {
        while (row == cur) {
            node = node->next;
            if (node == NULL) {
                return;
            }
            cur = node->y;
        }
        rows--;
        row = cur;
    }
    color = value;
    do {
        for (item = node->child; item != NULL; item = item->next) {
            item->flag14 = color;
        }
        node = node->next;
    } while (node != NULL && row == node->y);
}

void itfMesSetChildChainFlags(ItfMesNode *node, u8 value) {
    ItfMesItem *item;

    for (; node != NULL; node = node->next) {
        for (item = node->child; item != NULL; item = item->next) {
            item->flag14 = value;
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

/* Find the widest contiguous row, scaling the accumulated advance by 16. */
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

s32 itfMesNthClearBit(s32 skip, u32 mask) {
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
void itfMesEnableUnflaggedNodeContexts(ItfMesNode *node) {
    for (; node != NULL; node = node->next) {
        if (node->child->flag16 == 0) {
            frFontEnableContextMode(node);
        }
    }
}

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMessageFlags);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB1F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB1F8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB208);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB210);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB218);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB220);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB228);

