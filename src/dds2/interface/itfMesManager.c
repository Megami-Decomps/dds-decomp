#include "common.h"

void sdfRelocatePackedResourceWords(int *param_1, int param_2, u8 *param_3, int param_4);

extern u32 itfMessageFlags;

/* One 0x14-byte slot per message window; the first field points at its state. */
typedef struct ItfMesSlot {
    struct ItfMesState *mes;
    u8 unk4[0x10];
} ItfMesSlot;

extern ItfMesSlot itfWindowSlots[];

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

/* Globals behind D_003D6EA0: word at +0x4, bitfield at +0xC. */
typedef struct ItfMesGlobals {
    u32 activeWindowCount; /* 0x0: incremented on creation, decremented on destruction */
    u32 windowTexture;     /* 0x4: /itf/MESWIN.TMX resource */
    u32 unk8; /* 0x8 */
    u16 flags; /* 0xC: set/cleared by itfMesSetGlobalFlags */
    u16 unkE; /* 0xE */
    ItfMesPool pool; /* 0x10 */
    ItfMesPoolNode nodes[0x40]; /* 0x20 */
} ItfMesGlobals;

extern ItfMesGlobals itfMesWork;

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
    s16 bitCount; /* 0x1A: number of selection-mask bits / option rows */
    u32 items[1];   /* 0x1C */
} ItfMesTable;

/* 8-byte entry selected by func_0019D1D8/func_0019D1F0. */
typedef struct ItfMesEntry {
    u32 itemList;        /* 0x0: selected by itfMesGetNextEntrySelectedItem */
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
    u32 x;              /* +0x0: block horizontal position */
    u32 y;              /* +0x4: block vertical position */
    FrFontGlyph *glyphChain; /* +0x8: positioned and released with this block */
    u16 selectedIndex;   /* +0xC: chooses an item in the next entry */
    u16 padE;
} ItfMesBlk14;

/* Selected entry's text position, cached glyphs and four color channels. */
typedef struct ItfMesEntryBlock {
    u32 x;
    u32 y;
    ItfMesTable *table;
    FrFontGlyph *glyphChain;
    u8 unk10;
    u8 unk11; /* Written with the text-interface mask after glyph construction. */
    u8 color[4]; /* Passed in order to itfDrawCustomColorText. */
    s16 unk16; /* Written with the glyph span-step count. */
    s16 itemIndex;
    s16 tableCount;
} ItfMesEntryBlock;

/* Block at ItfMesState +0x40. */
typedef struct ItfMesBlk40 {
    u32 x;              /* +0x0: block horizontal position */
    u32 y;              /* +0x4: block vertical position */
    FrFontGlyph *glyphChain; /* +0x8: option-list glyphs */
    u32 panelValue;      /* +0xC: set by itfMesScriptSetPanelValue */
    u16 unk10;           /* +0x10 */
    s16 clearBitCount;  /* +0x12: cached by itfMesCountClearBits */
    u16 unk14;           /* +0x14 */
    s16 rowCount;        /* +0x16: number of displayed option rows */
} ItfMesBlk40;

/* Block at ItfMesState +0xA4. */
typedef struct ItfMesBlkA4 {
    u8 unk0[4];          /* +0x0 */
    void *unk4;          /* +0x4 */
    u32 panelHandle;      /* +0x8: panel handle */
    u8 unkC[0x1C];       /* +0xC */
    u32 unk28;           /* +0x28 */
} ItfMesBlkA4;

/* Parallel banks for replacement-text addresses and their owned heap handles. */
typedef struct ItfMesTextSlots {
    u32 addresses[0x20];
    u32 handles[0x20];
} ItfMesTextSlots;

/* Message-window state behind each ItfMesSlot. */
typedef struct ItfMesState {
    u32 flags;          /* 0x0: low half status, high half mask */
    ItfMesSub *sub;     /* 0x4 */
    s32 temporaryFontEntry; /* 0x8: optional entry passed to frFontLoadTemporaryEntry */
    s32 renderValue;    /* 0xC: propagated to glyph nodes and option frame */
    s16 unk10;          /* 0x10 */
    s16 unk12;          /* 0x12 */
    ItfMesBlk14 blk14;    /* 0x14: passed to func_0019DDA8 */
    ItfMesEntryBlock entryBlock; /* 0x24: selected entry and cached glyph chain */
    ItfMesBlk40 blk40;  /* 0x40 */
    u8 unk58[0x4C];     /* 0x58 */
    ItfMesBlkA4 blkA4;  /* 0xA4 */
    ItfMesTextSlots textSlots;
    u8 pad1D0[0xC];
    u32 callbackAddress; /* 0x1DC: invoked when glyph command 4 is set */
} ItfMesState;

ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 index);

ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub);

u32 itfMesGetTableItem(ItfMesTable *table, s32 index);

/* Item chained off a window node (+0x28); recolored by itfMesRecolorNodeChildren. */
typedef struct ItfMesItem {
    u8 unk0[0x10];         /* 0x0 */
    u32 word10;            /* 0x10: low byte is the color */
    u8 flag14;             /* 0x14: byte set by itfMesSetRowItemFlag */
    u8 unk15;              /* 0x15 */
    u8 flag16;             /* 0x16: tested by itfMesEnableUnflaggedNodeContexts */
    u8 unk17[0x11];        /* 0x17 */
    struct ItfMesItem *next; /* 0x28 */
} ItfMesItem;

/* Window chain node shared by positioning and child-formatting helpers. */
typedef struct ItfMesNode {
    u8 unk0[4];        /* 0x0 */
    s32 x;             /* 0x4: adjusted with horizontal node offsets */
    s32 y;             /* 0x8: groups nodes on the same row */
    s32 advance;       /* 0xC: accumulated within a row */
    u8 unk10[2];       /* 0x10 */
    s16 rowHeightUnits; /* 0x12: row height in 1/8 units */
    s32 renderValue;    /* 0x14: copied from message-window renderValue */
    u8 unk18[4];       /* 0x18 */
    ItfMesItem *child; /* 0x1C */
    u8 unk20[4];       /* 0x20 */
    struct ItfMesNode *next; /* 0x24 */
} ItfMesNode;

/* Shade bytes copied from a glyph's auxiliary color record. */
typedef struct ItfMesShade {
    u8 pad00[0x14];
    u8 shade14;
    u8 shade15;
    u8 shade16;
} ItfMesShade;

typedef struct ItfMesColorSrc {
    u8 firstByte;
    u8 pad01[0x1F];
    ItfMesShade *shade;
} ItfMesColorSrc;


/* Operands of itfMesCountSpanSteps: word at +0x8, divisor at +0x12. */
typedef struct ItfMesSpan {
    u8 unk0[8]; /* 0x0 */
    s32 y;      /* 0x8: node's vertical position */
    u8 unkC[6]; /* 0xC */
    s16 rowHeightUnits; /* 0x12: height divisor after converting y to eighths */
} ItfMesSpan;

s32 scrGetWindow(void);

s32 scrReadIntParameter(s32 parameterIndex);

void itfMesSetWindowPanelValue(s32 window, u32 value);

void itfMesCountClearBits(s32 window, s32 count);

void func_00154F18(s32);

void itfMesSetWindowHighFlags(s32 window, u32 value);

void func_001A45C0(s32 window, s32 x, s32 y, s32 width);

void itfMesClearWindowHighFlags(s32 window, u32 value);

void itfMesBlk24MoveTo(s32 window, s32 x, s32 y);

void itfMesSetWindowPageAndRefresh(s32 window, s32 first, s32 second);

void func_001A4A10(s32 window, s32 first, s32 second);

void itfMesFinishWindowAndClearStatus();

void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock);

void itfMesResetWindow(s32 window);

void itfMesDestroyWindow(s32 window);

void frFontQueueGlyphInSelectedSlot(FrFontGlyph *glyph);

void itfMesResetCursorState(void *, s32);

void itfResetCursorPositionAndState(void *, s32);

void itfMesInitCharTable(s32 *table);

s32 itfMesMaxGroupedExtent(ItfMesNode *node);

extern void frFontLoadTemporaryEntry(u32);

extern ItfMesNode *itfDrawDefaultColorText(s32 x, s32 y, s32 encodedText, s32 sub);

extern u32 itfLoadTextureFromAsset(const char *path);

extern void itfInitPool(ItfMesPool *pool, ItfMesPoolNode *nodes, s32 count, s32 stride);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern s32 sndVisitQueuedResources(void);

extern void sndFlushMessageQueue(void);

extern void func_001A76C8(void);

extern struct ItfMesPoolNode *itfAcquirePoolNode();

typedef struct MemBlock MemBlock;
typedef struct SdfAllocation SdfAllocation;
extern MemBlock *sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(SdfAllocation *);
extern void sdfReleaseResourceAllocation(SdfAllocation *);
extern u32 strlen(const char *);
extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);
void func_001A5480();

extern void itfInitializeCursorResetState();

extern void itfResetWindowResourceBlock();

extern void itfClearDrawStateWords();

extern void itfResetBattleFadeState();

extern void func_0019DD48();

extern s32 func_0019DBA8();

extern s32 func_001A1858();

extern void itfSetPanelLayoutAndNotify();

extern void itfPanelUpdateValuesAndNotify();

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
    func_00154F18(2);
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
        func_00154F18(3);
    }
    return state < 1;
}

void itfMesFinishWindowAndClearStatus(s32 window, s32 unused1, s32 unused2) {
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
    x = scrReadIntParameter(0);
    y = scrReadIntParameter(1);
    width = scrReadIntParameter(2);
    func_001A45C0(window, x, y, width);
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
    func_001A4A10(window, first, second);
    return 1;
}

u32 itfMesGetGlobalWindowValue(void) {
    return itfMesWork.windowTexture;
}

void itfMesSetFlags(u32 flags) {
    itfMessageFlags = itfMessageFlags | flags;
}

void itfMesClearFlags(u32 flags) {
    itfMessageFlags = itfMessageFlags & ~flags;
}

s32 itfMesCreateWindow(ItfMesSub *sub) {
    ItfMesPoolNode *node = itfAcquirePoolNode(&itfMesWork.pool);
    s32 window = node->index;
    ItfMesState *mes;
    u32 handle;

    handle = (u32)sdfAllocGeneralBlock(0x1E0);
    node->resourceHandle = handle;
    mes = (ItfMesState *)sdfResourceRetainAddress((SdfAllocation *)handle);
    node->stateAddress = (s32)mes;
    mes->sub = NULL;
    itfMesSetSubResource(window, sub);
    mes->flags = 0;
    mes->unk10 = 0x53;
    mes->renderValue = 0xFFFFF0;
    mes->unk12 = 0;
    mes->callbackAddress = 0;
    itfResetCursorPositionAndState(&mes->blk14, 1);
    itfMesResetCursorState(&mes->entryBlock, 1);
    itfInitializeCursorResetState(&mes->blk40);
    itfResetWindowResourceBlock(&mes->blkA4);
    itfClearDrawStateWords(&mes->textSlots);
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

/* Select a window's entry-table item; return text initialization's result, or zero for an empty table. */
s32 itfMesSelectTableItemText(s32 window, s32 entryIndex, s32 item) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesTable *table = itfMesGetEntry(mes, entryIndex)->table;
    ItfMesEntryBlock *blk = &mes->entryBlock;

    blk->itemIndex = item;
    blk->table = table;
    blk->tableCount = table->count;
    if (table->count != 0) {
        return itfInitTextDrawArgs(itfMesGetTableItem(table, (s16)item), 0);
    }
    return 0;
}

void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock) {
    ItfMesState *mes;
    ItfMesEntryBlock *entryBlock;
    ItfMesBlk14 *blk14;

    if (window < 0) {
        return;
    }
    mes = itfWindowSlots[window].mes;
    entryBlock = &mes->entryBlock;
    blk14 = &mes->blk14;
    if (entryBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(entryBlock->glyphChain);
        entryBlock->glyphChain = NULL;
    }
    itfMesResetCursorState(entryBlock, 0);
    mes->flags &= ~7;
    mes->flags &= 0xFFFDFFFF;
    if (releasePrimaryBlock == 0) {
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

    func_0019DD48(0x1000, 0x4B0, blk->glyphChain);
    height = blk->rowCount * 25 * 8;
    for (i = 0; i < blk->rowCount; i++) {
        w = func_0019DBA8(i, blk->glyphChain);
        if (width < w) {
            width = w;
        }
    }
    half = width / 2;
    rect[0] = 0xE00 - half;
    rect[1] = 0x430;
    rect[2] = 0x1200 + half;
    rect[3] = 0x530 + height;
    blkA4->panelHandle = func_001A1858(9, itfMesWork.windowTexture);
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
    y = blk->y - ((count - 1) * 25 << 3);
    itfMesInitCharTable((s32 *)mes->textSlots.addresses);
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

void itfMesCountClearBits(s32 window, s32 count) {
    ItfMesBlk40 *blk = &itfWindowSlots[window].mes->blk40;
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
    blk->clearBitCount = zeros;
    blk->unk14 = zeros;
}

void itfMesOffsetNodeChain(ItfMesNode *node, s32 dx, s32 dy);

void itfMesBlk14MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk14 *blk = &itfWindowSlots[window].mes->blk14;
    s32 delta[2];
    delta[0] = x - blk->x;
    delta[1] = y - blk->y;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, delta[0], delta[1]);
        blk->x = x;
        blk->y = y;
    }
}

void itfMesBlk14MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk14 *blk = &itfWindowSlots[window].mes->blk14;
    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, dx, dy);
    blk->x += dx;
    blk->y += dy;
}

void itfMesBlk24MoveTo(s32 window, s32 x, s32 y) {
    ItfMesEntryBlock *blk = &itfWindowSlots[window].mes->entryBlock;
    s32 delta[2];
    delta[0] = x - blk->x;
    delta[1] = y - blk->y;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, delta[0], delta[1]);
        blk->x = x;
        blk->y = y;
    }
}

void itfMesBlk24MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesEntryBlock *blk = &itfWindowSlots[window].mes->entryBlock;
    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, dx, dy);
    blk->x += dx;
    blk->y += dy;
}

void itfMesBlk40MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk40 *blk = &itfWindowSlots[window].mes->blk40;
    s32 delta[2];
    delta[0] = x - blk->x;
    delta[1] = y - blk->y;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, delta[0], delta[1]);
        blk->x = x;
        blk->y = y;
    }
}

void itfMesBlk40MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk40 *blk = &itfWindowSlots[window].mes->blk40;
    itfMesOffsetNodeChain((ItfMesNode *)blk->glyphChain, dx, dy);
    blk->x += dx;
    blk->y += dy;
}

void itfUpdateMessageWindowRenderValue(s32 window, s32 value) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    if (mes->renderValue != value) {
        itfMesSetNodeChainRenderValue((ItfMesNode *)mes->blk14.glyphChain, value);
        itfMesSetNodeChainRenderValue((ItfMesNode *)mes->entryBlock.glyphChain, value);
        itfMesSetNodeChainRenderValue((ItfMesNode *)mes->blk40.glyphChain, value);
        mes->renderValue = value;
    }
}

u32 itfMesGetWindowFlags(s32 window) {
    return itfWindowSlots[window].mes->flags;
}

void itfMesReplaceWindowHighFlags(s32 window, u32 value) {
    ItfMesState *mes;

    mes = itfWindowSlots[window].mes;
    mes->flags = (u32)(u16)mes->flags | (value & 0xffff0000);
}

void itfMesSetWindowHighFlags(s32 window, u32 value) {
    ItfMesState *mes;

    mes = itfWindowSlots[window].mes;
    mes->flags = mes->flags | (value & 0xffff0000);
}

void itfMesClearWindowHighFlags(s32 window, u32 value) {
    ItfMesState *mes;

    mes = itfWindowSlots[window].mes;
    mes->flags = mes->flags & (~value | 0xffff);
}

s16 itfMesGetWindowClearBitCount(s32 window) {
    return itfWindowSlots[window].mes->blk40.clearBitCount;
}

/* Return the selected table's cached item count for this window. */
s16 itfMesGetWindowTableCount(s32 window) {
    return itfWindowSlots[window].mes->entryBlock.tableCount;
}

/* Return the selected item index for this window. */
s16 itfMesGetWindowItemIndex(s32 window) {
    return itfWindowSlots[window].mes->entryBlock.itemIndex;
}

u32 itfMesGetWindowTableValue(s32 window, s32 index) {
    return itfWindowSlots[window].mes->textSlots.addresses[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A45C0);

void itfMesCopyStringToWindowTableSlot(s32 window, u32 first, u32 second) {
    func_001A5480((u32)itfWindowSlots[window].mes, first, second, 0);
}

void func_001A4888(s32 window) {
    func_001A5480((u32)itfWindowSlots[window].mes);
}

ItfMesSub *itfMesSetSubResource(s32 window, ItfMesSub *sub) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesSub *previous = mes->sub;
    ItfMesEntry *entry;
    s32 value;
    mes->sub = sub;
    itfMesRelocate(sub);
    entry = itfMesGetNextEntry(sub);
    value = 0;
    if (((ItfMesRelocResource *)sub)->magic == ITF_MES_MAGIC_MSG1) {
        value = *(s32 *)((u8 *)entry + 8);
    }
    mes->temporaryFontEntry = value;
    return previous;
}

u32 itfMesGetEntryTableItem(s32 window, s32 entryIndex, s32 itemIndex) {
    return itfMesGetTableItem(itfMesGetEntry(itfWindowSlots[window].mes, entryIndex)->table, itemIndex);
}

extern void func_001A6078(ItfMesBlkA4 *blk, s32 arg1, s32 arg2);
extern void itfPanelReleasePrimitiveResources(void *primitive);

void itfMesSetWindowPageAndRefresh(s32 window, s32 first, s32 second) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlkA4 *blk = &mes->blkA4;

    if ((mes->flags & 0x3300) == 0) {
        if (mes->unk12 != first || blk->unk28 != second) {
            mes->unk12 = first;
            func_001A6078(blk, first, second);
            if (blk->unk4 != NULL) {
                itfPanelReleasePrimitiveResources(blk->unk4);
                blk->unk4 = NULL;
            }
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4A10);

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
    itfMesInitCharTable((s32 *)mes->textSlots.addresses);
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

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesSetEntryLastColorChannel(s32 window, u8 value) {
    itfWindowSlots[window].mes->entryBlock.color[3] = value;
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
    kwlnTaskCreate("DrawMsgMng", 0x2B1A, 0, 0, func_001A76C8, sndFlushMessageQueue, NULL);
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
extern void itfReleaseUiResourceSlotHandles();
extern void itfReleasePoolNode();

void itfMesDestroyWindow(s32 window) {
    ItfMesWindowRec *rec = &D_00452960[window];
    ItfMesState *mes = rec->mes;
    if (mes != NULL) {
        itfMesCleanupWindow(window, 1);
        itfMesResetWindow(window);
        btlReleaseEffectResourceHandles(mes);
        itfReleaseUiResourceSlotHandles(&mes->textSlots);
        mes->flags = 0;
        sdfReleaseResourceAllocation((SdfAllocation *)rec->handle);
        rec->mes = NULL;
        itfReleasePoolNode(rec, (u8 *)D_00452960 - 0x10);
        /* This address is itfMesWork.unk0; using the pool-array-relative address preserves the compiled access. */
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

extern s32 func_0019DB30();
extern void func_0019DD48();
extern s32 itfDrawCustomColorText();
extern s8 itfTestTextInterfaceMask();
extern s32 func_0019E908();
extern s32 func_0019E910();
extern void evtLipsExecFunction();
extern void itfMesCopyGlyphShade();
extern void itfMesEnableUnflaggedNodeContexts();

/* Rebuild the selected entry's colored glyph chain and cache its span count. */
void itfMesBuildEntryGlyph(ItfMesState *mes) {
    ItfMesEntryBlock *block = &mes->entryBlock;
    s32 glyph;
    s32 handle;
    s32 helper;
    s8 flags;
    void (*hook)();

    handle = (s32)block->glyphChain;
    if (handle != 0) {
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)handle);
        block->glyphChain = NULL;
    }
    glyph = itfDrawCustomColorText((s32)mes->entryBlock.x, (s32)block->y, block->color[0], block->color[1], block->color[2], block->color[3],
                          itfMesGetTableItem(block->table, block->itemIndex), 0);
    if (mes->unk12 == 3) {
        if (func_0019DB30(glyph) == 1) {
            func_0019DD48(0x1000, 0xC60, glyph);
        } else {
            func_0019DD48(0x1000, 0xBF8, glyph);
        }
    }
    if (!(mes->flags & 0x400000) && (itfMesWork.flags & 1)) {
        itfMesEnableUnflaggedNodeContexts((ItfMesNode *)glyph);
    }
    itfMesCopyGlyphShade(glyph, block);
    flags = itfTestTextInterfaceMask(3);
    block->unk11 = flags;
    if (flags & 2) {
        mes->flags |= 0x10000;
    } else {
        mes->flags &= 0xFFFEFFFF;
    }
    if (itfTestTextInterfaceMask(4) != 0) {
        hook = (void (*)())mes->callbackAddress;
        if (hook != NULL) {
            hook();
        }
    }
    if (itfTestTextInterfaceMask(8) != 0) {
        helper = func_0019E908();
        evtLipsExecFunction(helper, func_0019E910());
    }
    itfMesSetNodeChainRenderValue((ItfMesNode *)glyph, mes->renderValue);
    block->unk16 = itfMesCountSpanSteps(itfMesGetLastNode(glyph), glyph);
    block->glyphChain = (FrFontGlyph *)glyph;
}

void func_001A5480(mes, index, source, count)
    ItfMesState *mes;
    s32 index;
    const char *source;
    s32 count;
{
    ItfMesTextSlots *slots = &mes->textSlots;
    u32 *text = &slots->addresses[index];
    s32 size;

    if (*text != 0) {
        sdfReleaseResourceAllocation((SdfAllocation *)slots->handles[index]);
        *text = 0;
    }
    if (count <= 0) {
        size = (strlen(source) + 4) & ~3;
        slots->handles[index] = (u32)sdfAllocGeneralBlock(size);
        *text = sdfResourceRetainAddress((SdfAllocation *)slots->handles[index]);
        memset((void *)*text, 0, size);
        /* String mode copies the padded span, rather than only strlen + 1. */
        memcpy((void *)*text, source, size);
        return;
    }
    size = (count + 5) & ~3;
    slots->handles[index] = (u32)sdfAllocGeneralBlock(size);
    *text = sdfResourceRetainAddress((SdfAllocation *)slots->handles[index]);
    memset((void *)*text, 0, size);
    memcpy((void *)*text, source, count);
}

extern void func_0019E8F0();

void itfMesInitCharTable(s32 *table) {
    s32 i;
    for (i = 0; i < 0x20; i++) {
        func_0019E8F0(i, table[i]);
    }
}

u32 itfMesGetNextEntrySelectedItem(ItfMesState *mes) {
    s32 *items;

    items = (s32 *)itfMesGetNextEntry(mes->sub);
    return ((u32 *)*items)[mes->blk14.selectedIndex];
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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5760);

ItfMesNode *itfMesGetLastNode(ItfMesNode *node) {
    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

/* Copy glyph shade components into the current message block's color bytes. */
void itfMesCopyGlyphShade(ItfMesColorSrc *glyph, ItfMesEntryBlock *block) {
    ItfMesShade *shade = glyph->shade;

    block->color[3] = glyph->firstByte >> 1;
    block->color[0] = shade->shade15;
    block->color[1] = shade->shade14;
    block->color[2] = shade->shade16;
}

s32 itfMesCountSpanSteps(ItfMesSpan *last, ItfMesSpan *first) {
    return ((first->y - last->y) >> 3) / first->rowHeightUnits + 1;
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

void itfMesSetChildChainFlags(FrFontGlyph *glyph, u8 value) {
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

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436608);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436610);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436618);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436620);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436628);

