#include "common.h"
#include "itf.h"

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
    u8 unk0[8];
    u32 magic;
    u8 unkC[0xC];
    u32 entryCount;        /* 0x18: index of next entry */
    u8 unk1C[4];          /* 0x1C */
    ItfMesEntry entries[1]; /* 0x20 */
} ItfMesSub;

typedef struct FrFontGlyph FrFontGlyph;
typedef struct ItfMesWindowRec ItfMesWindowRec;


/* Block at ItfMesState +0x14. */
typedef struct ItfMesBlk14 {
    u32 x;               /* +0x0 */
    u32 y;               /* +0x4 */
    FrFontGlyph *glyphChain; /* +0x8: positioned and released with this block */
    u32 unkC;            /* +0xC */
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
    UiSprite *unk4; /* +0x4: placement initializer stores the constructed sprite here. */
    u32 panelHandle;       /* +0x8: panel handle */
    s32 offsetLeft;      /* +0xC */
    s32 offsetTop;       /* +0x10 */
    s32 offsetRight;     /* +0x14 */
    s32 offsetBottom;    /* +0x18 */
    u8 unk1C[0xC];       /* +0x1C */
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
    ItfMesBlk14 blk14;    /* 0x14: passed to itfResetCursorPositionAndState */
    ItfMesEntryBlock entryBlock; /* 0x24: selected entry and cached glyph chain */
    ItfMesBlk40 blk40;  /* 0x40 */
    u8 unkA0[4];        /* 0xA0 */
    ItfMesBlkA4 blkA4;  /* 0xA4 */
    ItfMesTextSlots textSlots;
    BtlFade fade;       /* 0x1D0 */
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
    u32 colorWord;         /* 0x10: recoloring clears the low byte, then ORs an unmasked word */
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
    struct ItfMesNode *forward; /* 0x28: opposite link in the glyph chain */
    struct ItfMesNode *chainHead; /* 0x2C */
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



/* State of the interactive message-layout inspector. */
typedef struct ItfMesDebugState {
    s32 selectedItem;
    s32 mode;
    ItfMesWindowRec *window;
} ItfMesDebugState;

/* Operands of itfMesCountSpanSteps: word at +0x8, divisor at +0x12. */
typedef struct ItfMesSpan {
    u8 unk0[8]; /* 0x0 */
    s32 y;      /* 0x8: node's vertical position */
    u8 unkC[6]; /* 0xC */
    s16 rowHeightUnits; /* 0x12: height divisor after converting y to eighths */
} ItfMesSpan;

extern ItfMesSlot itfWindowSlots[];

extern ItfMesGlobals itfMesWork;

extern ItfMesDebugState D_00357D80;

extern u32 itfMessageFlags;

s32 scrGetWindow(void);

s32 scrReadIntParameter(s32 arg0);

void func_0014DAF0(s32 arg0);

void itfMesSetWindowPanelValue(s32 window, u32 panelMask);

void itfMesCountClearBits(s32 window, s32 selectedBitIndex);

void itfMesBlk24MoveTo(s32 window, s32 x, s32 y);

void itfMesSetWindowHighFlags(s32 window, u32 mask);

void itfMesClearWindowHighFlags(s32 window, u32 mask);

void itfMesSetWindowPageAndRefresh(s32 window, s32 firstValue, s32 secondValue);

void itfMesFinishWindowAndClearStatus(s32 window);

void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock);

void itfMesResetWindow(s32 window);

ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 entryIndex);

ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub);

u32 itfMesGetTableItem(ItfMesTable *table, s32 itemIndex);

void itfMesRelocate(ItfMesBin *bin);

void itfMesDestroyWindow(s32 window);

void itfMesSetTextSlotFromValue(s32 window, s32 slotIndex, s32 value, s32 selector);

void func_0019C9F0(s32 window, s32 arg1, s32 arg2);

s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *arg0);

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

void itfMesSetNodeChainRenderValue(ItfMesNode *node, s32 renderValue);

ItfMesNode *itfMesGetLastNode(ItfMesNode *node);

s32 itfMesCountSpanSteps();

void itfMesInitCharTable(s32 *table);

s32 itfMesMaxGroupedExtent(ItfMesNode *node);

extern void frFontLoadTemporaryEntry(u32 arg0);

extern ItfMesNode *itfDrawDefaultColorText(s32 x, s32 y, s32 encodedText, s32 sub);

extern SdfTex *itfLoadTextureFromAsset(const char *path);

extern void itfInitPool(ItfMesPool *pool, ItfMesPoolNode *nodes, s32 count, s32 stride);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern s32 sndVisitQueuedResources(void);

extern void sndFlushMessageQueue(void);

extern void func_0019F6A0(void);

extern struct ItfMesPoolNode *itfAcquirePoolNode();

typedef struct MemBlock MemBlock;
typedef struct SdfResource SdfResource;
extern MemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfResourceRetainAddress(SdfResource *resource);
extern void sdfReleaseResourceAllocation(SdfResource *resource);
extern u32 strlen(const char *text);
extern void *memset(void *destination, s32 value, u32 size);
extern void *memcpy(void *destination, const void *source, u32 size);
void func_0019D460();

extern void itfInitializeCursorResetState();

extern void itfResetWindowResourceBlock();

extern void itfClearDrawStateWords();

extern void itfResetBattleFadeState();

extern s32 func_00195ED8();

extern UiSprite *func_00199828(s32 kind, u32 payload);

extern void itfSetPanelLayoutAndNotify();

extern void itfPanelUpdateValuesAndNotify();

#define ITF_MES_MAGIC_MSG0 0x3047534d
#define ITF_MES_MAGIC_MSG1 0x3147534d
#define ITF_MES_SCRIPT_PANEL_BIT 0x200000
#define ITF_MES_LOWER_STATUS_PAIR_MASK 0x300
#define ITF_MES_LOWER_STATUS_FIRST_BIT 0x100
#define ITF_MES_LOWER_STATUS_SECOND_BIT 0x200
#define ITF_MES_UPPER_STATUS_PAIR_MASK 0x3000
#define ITF_MES_CONTENT_STATUS_MASK 0x3300
#define ITF_MES_CLEAR_SCRIPT_PANEL_BIT 0xFFDFFFFF
#define ITF_MES_SKIP_CONTEXT_ENABLE_BIT 0x400000
#define ITF_MES_TEXT_X_SHIFT 4
#define ITF_MES_TEXT_Y_SHIFT 3
#define ITF_MES_DEFAULT_TEXT_X 0x4B0
#define ITF_MES_DEFAULT_TEXT_Y 0xAF8
#define ITF_MES_OPTION_STATE_MASK 0x38
#define ITF_MES_OPTION_LIST_READY 0x10
#define ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK 0xFFFBFFFF
#define ITF_MES_ENTRY_STATE_MASK 7
#define ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK 0xFFFDFFFF
#define ITF_MES_HIGH_FLAGS_MASK 0xffff0000
#define ITF_MES_LOW_FLAGS_MASK 0xffff
#define ITF_MES_WINDOW_CAPACITY 0x40
#define ITF_MES_POOL_NODE_BYTES 0x14
#define ITF_MES_OPTION_ROW_SPACING 21
#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00
#define ITF_MES_DEFAULT_COLOR_VALUE 0x80
#define ITF_MES_TEXT_SLOT_COUNT 0x20
#define ITF_MES_MASK_BIT_COUNT 0x20
#define ITF_MES_STRING_COPY_ROUND_BIAS 4
#define ITF_MES_BINARY_COPY_ROUND_BIAS 5
#define ITF_MES_COPY_ALIGN_MASK 3

/* Store the script's panel mask; a negative script window is a no-op. Return 1. */
s32 itfMesScriptSetPanelValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowPanelValue(window, scrReadIntParameter(0));
    return 1;
}

/* Cache the clear-bit rank for the script's selected bit index; return 1. */
s32 itfMesScriptSetWindowValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesCountClearBits(window, scrReadIntParameter(0));
    return 1;
}

/* Request script panel activation, then notify the script-side state routine. */
s32 itfMesScriptActivatePanel(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowHighFlags(window, ITF_MES_SCRIPT_PANEL_BIT);
    func_0014DAF0(2);
    return 1;
}

/* Finish single-bit states of the lower status pair; return whether its original
 * value was zero. Finishing a nonzero state therefore still returns 0. */
s32 itfMesFinishScriptWindowIfActive(void) {
    s32 window = scrGetWindow();
    ItfMesState *mes;
    u32 scriptStatus;

    if (window < 0) {
        return 1;
    }
    mes = itfWindowSlots[window].mes;
    scriptStatus = mes->flags & ITF_MES_LOWER_STATUS_PAIR_MASK;
    if (scriptStatus == ITF_MES_LOWER_STATUS_FIRST_BIT || scriptStatus == ITF_MES_LOWER_STATUS_SECOND_BIT) {
        itfMesFinishWindowAndClearStatus(window);
        func_0014DAF0(3);
    }
    return scriptStatus < 1;
}

/* Mark each originally nonzero status pair with both bits, then clean/reset
 * content and clear the script panel bit. The status pairs are set, not cleared. */
void itfMesFinishWindowAndClearStatus(s32 window) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    u32 previousFlags = mes->flags;

    if (previousFlags & ITF_MES_LOWER_STATUS_PAIR_MASK) {
        mes->flags = previousFlags | ITF_MES_LOWER_STATUS_PAIR_MASK;
    }
    if (previousFlags & ITF_MES_UPPER_STATUS_PAIR_MASK) {
        mes->flags = mes->flags | ITF_MES_UPPER_STATUS_PAIR_MASK;
    }
    itfMesCleanupWindow(window, 1);
    itfMesResetWindow(window);
    mes->flags &= ITF_MES_CLEAR_SCRIPT_PANEL_BIT;
}

u32 func_0019B530(void) {
    return 1;
}

u32 func_0019B538(void) {
    return 1;
}

/* Read the text-slot index, value and selector from script parameters. */
s32 itfMesScriptSetTextSlotFromValue(void) {
    s32 window = scrGetWindow();
    s32 slotIndex;
    s32 value;
    s32 selector;

    if (window < 0) {
        return 1;
    }
    slotIndex = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    selector = scrReadIntParameter(2);
    itfMesSetTextSlotFromValue(window, slotIndex, value, selector);
    return 1;
}

/* Toggle suppression of automatic glyph-context enabling from script argument 0. */
s32 itfMesScriptToggleMessageFlag(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (scrReadIntParameter(0)) {
        itfMesSetWindowHighFlags(window, ITF_MES_SKIP_CONTEXT_ENABLE_BIT);
    } else {
        itfMesClearWindowHighFlags(window, ITF_MES_SKIP_CONTEXT_ENABLE_BIT);
    }
    return 1;
}

/* Convert script coordinates to the text units (x * 16, y * 8); return 1. */
s32 itfMesScriptSetScaledPosition(void) {
    s32 window = scrGetWindow();
    s32 x;
    s32 y;

    if (window < 0) {
        return 1;
    }
    x = scrReadIntParameter(0);
    y = scrReadIntParameter(1);
    itfMesBlk24MoveTo(window, x << ITF_MES_TEXT_X_SHIFT, y << ITF_MES_TEXT_Y_SHIFT);
    return 1;
}

/* Move selected-entry text to the fixed default position; return 1. */
s32 itfMesScriptSetDefaultBounds(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesBlk24MoveTo(window, ITF_MES_DEFAULT_TEXT_X, ITF_MES_DEFAULT_TEXT_Y);
    return 1;
}

/* The two opaque high bits use inverted script polarity: true clears both.
 * Keep the two separate calls and their original order. */
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

/* Refresh the script window with argument 0 and a zero second value; return 1. */
s32 itfMesScriptSetMessageOption(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowPageAndRefresh(window, scrReadIntParameter(0), 0);
    return 1;
}

/* Read both refresh values in order; a negative script window is a no-op. */
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

/* Pass both script range values in order; a negative script window is a no-op. */
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

/* Return the loaded message-window texture resource, not a window index. */
SdfTex *itfMesGetGlobalWindowValue(void) {
    return itfMesWork.windowTexture;
}

/* Set requested bits in the separate message-global word. */
void itfMesSetFlags(u32 mask) {
    itfMessageFlags |= mask;
}

/* Clear requested bits in the separate message-global word. */
void itfMesClearFlags(u32 mask) {
    itfMessageFlags &= ~mask;
}

s32 itfMesCreateWindow(ItfMesSub *sub) {
    ItfMesPoolNode *node = itfAcquirePoolNode(&itfMesWork.pool);
    s32 window = node->index;
    ItfMesState *mes;
    u32 handle;

    handle = (u32)sdfAllocGeneralBlock(0x1E0);
    node->resourceHandle = handle;
    mes = (ItfMesState *)sdfResourceRetainAddress((SdfResource *)handle);
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
    itfResetBattleFadeState(&mes->fade, 0);
    itfMesWork.activeWindowCount++;
    return window;
}

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesDestroyWindowIfPresent(s32 window) {
    if (window >= 0 && itfWindowSlots[window].mes != NULL) {
        itfMesDestroyWindow(window);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesStartEntry);

extern s32 itfInitTextDrawArgs(s32 encodedText, s32 sub);

/* Cache the selected table and narrowed item index, then initialize its text.
 * Empty tables return 0; nonempty tables return the text initializer's result. */
s32 itfMesSelectTableItemText(s32 window, s32 entryIndex, s32 itemIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesTable *table = itfMesGetEntry(mes, entryIndex)->table;
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;

    entryBlock->itemIndex = itemIndex;
    entryBlock->table = table;
    entryBlock->tableCount = table->count;
    if (table->count != 0) {
        return itfInitTextDrawArgs(itfMesGetTableItem(table, (s16)itemIndex), 0);
    }
    return 0;
}

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock) {
    ItfMesState *mes;
    ItfMesEntryBlock *entryBlock;
    ItfMesBlk14 *primaryTextBlock;

    if (window < 0) {
        return;
    }
    mes = itfWindowSlots[window].mes;
    entryBlock = &mes->entryBlock;
    primaryTextBlock = &mes->blk14;
    if (entryBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(entryBlock->glyphChain);
        entryBlock->glyphChain = NULL;
    }
    itfMesResetCursorState(entryBlock, 0);
    mes->flags &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->flags &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (releasePrimaryBlock == 0) {
        return;
    }
    if (primaryTextBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(primaryTextBlock->glyphChain);
        primaryTextBlock->glyphChain = NULL;
    }
    itfResetCursorPositionAndState(primaryTextBlock, 0);
}

/* Build the option panel around the widest measured row, using this game's
 * fixed row spacing. Preserve the existing panel creation/layout order. */
void itfMesBuildOptionFrame(ItfMesState *mes) {
    ItfMesBlk40 *optionBlock = &mes->blk40;
    ItfMesBlkA4 *panelBlock = &mes->blkA4;
    s32 bounds[4];
    s32 maxRowWidth = 0;
    s32 rowsHeight;
    s32 halfWidth;
    s32 rowIndex;
    s32 rowWidth;

    func_00196088(0x1000, 0x4B0, optionBlock->glyphChain);
    rowsHeight = optionBlock->rowCount * ITF_MES_OPTION_ROW_SPACING * 8;
    for (rowIndex = 0; rowIndex < optionBlock->rowCount; rowIndex++) {
        rowWidth = func_00195ED8(rowIndex, optionBlock->glyphChain);
        if (maxRowWidth < rowWidth) {
            maxRowWidth = rowWidth;
        }
    }
    halfWidth = maxRowWidth / 2;
    bounds[0] = 0xE00 - halfWidth;
    bounds[1] = 0x430;
    bounds[2] = 0x1200 + halfWidth;
    bounds[3] = 0x530 + rowsHeight;
    panelBlock->panelHandle = (s32)func_00199828(9, (u32)itfMesWork.windowTexture);
    itfSetPanelLayoutAndNotify(panelBlock->panelHandle, bounds[0], bounds[1], bounds[2], bounds[3], mes->renderValue);
    itfPanelUpdateValuesAndNotify(panelBlock->panelHandle, 0, 0, 0, 0);
    mes->flags = (mes->flags & ~0xC00) | 0x400;
}

/* Replace the option glyph chain with rows selected by clear panel-mask bits.
 * Item zero is skipped; mode 3 additionally builds the surrounding frame. */
void itfMesBuildOptionList(s32 window, s32 entryIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlk40 *optionBlock = &mes->blk40;
    ItfMesEntry *entry = itfMesGetEntry(mes, entryIndex);
    ItfMesTable *table;
    s32 visibleRowCount;
    s32 firstRowY;

    if (optionBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(optionBlock->glyphChain);
        optionBlock->glyphChain = NULL;
    }
    table = entry->table;
    visibleRowCount = itfMesCountZeroBits(table->bitCount, optionBlock->panelValue);
    firstRowY = optionBlock->y - ((visibleRowCount - 1) * ITF_MES_OPTION_ROW_SPACING << ITF_MES_TEXT_Y_SHIFT);
    itfMesInitCharTable((s32 *)mes->textSlots.addresses);
    if (mes->temporaryFontEntry != 0) {
        frFontLoadTemporaryEntry(mes->temporaryFontEntry);
    }
    optionBlock->glyphChain = (FrFontGlyph *)itfMesBuildNodeRows(&table->items[1], table->bitCount, optionBlock->panelValue, optionBlock->x, firstRowY, mes->renderValue);
    optionBlock->rowCount = visibleRowCount;
    mes->flags = (mes->flags & ~ITF_MES_OPTION_STATE_MASK) | ITF_MES_OPTION_LIST_READY;
    if (mes->unk12 == 3) {
        itfMesBuildOptionFrame(mes);
    }
    optionBlock->unk10 = 1;
}

/* Release option glyphs, clear option selection/state and invalidate its rank.
 * Other content and the window allocation remain installed. */
void itfMesResetWindow(s32 window) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlk40 *optionBlock = &mes->blk40;

    if (optionBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(optionBlock->glyphChain);
        optionBlock->glyphChain = NULL;
    }
    optionBlock->panelValue = 0;
    optionBlock->unk10 = 0;
    optionBlock->rowCount = 0;
    optionBlock->clearBitCount = -1;
    mes->flags &= ~ITF_MES_OPTION_STATE_MASK;
    mes->flags &= ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK;
}

/* Store the mask whose clear bits select displayed option rows. */
void itfMesSetWindowPanelValue(s32 window, u32 panelMask) {
    itfWindowSlots[window].mes->blk40.panelValue = panelMask;
}

/* Cache the number of clear bits below selectedBitIndex only when that bit is
 * itself clear; a set bit produces zero. Negative indices also produce zero;
 * no upper bound is checked. */
void itfMesCountClearBits(s32 window, s32 selectedBitIndex) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;
    u32 remainingMask = optionBlock->panelValue;
    s32 bitValue = remainingMask & 1;
    s32 clearBitCount = bitValue == 0;
    s32 bitIndex = 0;

    if (selectedBitIndex > 0) {
        do {
            bitIndex++;
            remainingMask >>= 1;
            bitValue = remainingMask & 1;
            if (bitValue == 0) {
                clearBitCount++;
            }
        } while (bitIndex < selectedBitIndex);
    }
    clearBitCount = bitValue == 0 ? clearBitCount - 1 : 0;
    optionBlock->clearBitCount = clearBitCount;
    optionBlock->unk14 = clearBitCount;
}

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesBlk14MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk14 *primaryTextBlock = &itfWindowSlots[window].mes->blk14;
    s32 delta[2];

    delta[0] = x - primaryTextBlock->x;
    delta[1] = y - primaryTextBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    itfMesOffsetNodeChain((ItfMesNode *)primaryTextBlock->glyphChain, delta[0], delta[1]);
    primaryTextBlock->x = x;
    primaryTextBlock->y = y;
}

extern void itfMesOffsetNodeChain(ItfMesNode *, s32, s32);

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesBlk14MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk14 *primaryTextBlock = &itfWindowSlots[window].mes->blk14;

    itfMesOffsetNodeChain((ItfMesNode *)primaryTextBlock->glyphChain, dx, dy);
    primaryTextBlock->x += dx;
    primaryTextBlock->y += dy;
}

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesBlk24MoveTo(s32 window, s32 x, s32 y) {
    ItfMesEntryBlock *entryBlock = &itfWindowSlots[window].mes->entryBlock;
    s32 delta[2];

    delta[0] = x - entryBlock->x;
    delta[1] = y - entryBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    itfMesOffsetNodeChain((ItfMesNode *)entryBlock->glyphChain, delta[0], delta[1]);
    entryBlock->x = x;
    entryBlock->y = y;
}

/* Translate selected-entry text and update its cached position. */
void itfMesBlk24MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesEntryBlock *entryBlock = &itfWindowSlots[window].mes->entryBlock;

    itfMesOffsetNodeChain((ItfMesNode *)entryBlock->glyphChain, dx, dy);
    entryBlock->x += dx;
    entryBlock->y += dy;
}

/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesBlk40MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;
    s32 delta[2];

    delta[0] = x - optionBlock->x;
    delta[1] = y - optionBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    itfMesOffsetNodeChain((ItfMesNode *)optionBlock->glyphChain, delta[0], delta[1]);
    optionBlock->x = x;
    optionBlock->y = y;
}

/* Translate option text and update its cached position. */
void itfMesBlk40MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;

    itfMesOffsetNodeChain((ItfMesNode *)optionBlock->glyphChain, dx, dy);
    optionBlock->x += dx;
    optionBlock->y += dy;
}

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfUpdateMessageWindowRenderValue(s32 window, s32 renderValue) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    if (mes->renderValue == renderValue) {
        return;
    }
    itfMesSetNodeChainRenderValue((ItfMesNode *)mes->blk14.glyphChain, renderValue);
    itfMesSetNodeChainRenderValue((ItfMesNode *)mes->entryBlock.glyphChain, renderValue);
    itfMesSetNodeChainRenderValue((ItfMesNode *)mes->blk40.glyphChain, renderValue);
    mes->renderValue = renderValue;
}

/* Return the complete status/flag word, not just its high-half flags. */
u32 itfMesGetWindowFlags(s32 window) {
    return itfWindowSlots[window].mes->flags;
}

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesReplaceWindowHighFlags(s32 window, u32 highFlags) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    mes->flags = (u32)(u16)mes->flags | (highFlags & ITF_MES_HIGH_FLAGS_MASK);
}

/* Set only high-half mask bits; low-half status is unchanged. */
void itfMesSetWindowHighFlags(s32 window, u32 mask) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    mes->flags |= mask & ITF_MES_HIGH_FLAGS_MASK;
}

/* Clear only high-half mask bits; low-half status is unchanged. */
void itfMesClearWindowHighFlags(s32 window, u32 mask) {
    ItfMesState *mes = itfWindowSlots[window].mes;

    mes->flags &= ~mask | ITF_MES_LOW_FLAGS_MASK;
}

/* Return the cached clear-bit rank; reset state uses -1. */
s16 itfMesGetWindowClearBitCount(s32 window) {
    return itfWindowSlots[window].mes->blk40.clearBitCount;
}

/* Return the selected table's cached item count, without another table lookup. */
s16 itfMesGetWindowTableCount(s32 window) {
    return itfWindowSlots[window].mes->entryBlock.tableCount;
}

/* Return the stored signed-halfword item index, without another table lookup. */
s16 itfMesGetWindowItemIndex(s32 window) {
    return itfWindowSlots[window].mes->entryBlock.itemIndex;
}

/* Return a replacement-text address; caller supplies an in-range slot index. */
u32 itfMesGetWindowTableValue(s32 window, s32 slotIndex) {
    return itfWindowSlots[window].mes->textSlots.addresses[slotIndex];
}

extern s32 D_003BAA70;
extern s32 D_003BAA74;
extern s32 D_003BAA84;
extern s32 D_003BAA88;
extern s32 D_003BAA8C;
extern s32 D_003BAA90;
extern char D_00347C68[][0x20];
extern char D_003482A8[][0x20];
extern char *D_0032ACA8[];
extern char D_003BB1F0[];
extern void func_003014F0(char *dst, const char *format, ...);
extern void itfConvertText(char *dst, const char *src);
extern u16 *txtFormatNumberU16(s32 value, u16 *dst);
void itfMesCopyStringToWindowTableSlot(s32 window, u32 slotIndex, u32 sourceAddress);

/* Copy one of the built-in interface strings into a window replacement slot. */
void itfMesSetTextSlotFromValue(s32 window, s32 slotIndex, s32 value, s32 selector) {
    char formatted[0x10];
    u16 number[0x20];
    char converted12[0x11];
    char converted13[0x19];
    char converted15[0x11];
    char converted14[0x11];
    char *converted;

    switch (selector) {
    case 0:
        func_003014F0(formatted, D_003BB1F0, value);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)formatted);
        break;
    case 1:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003BAA74 + value * 0x11);
        break;
    case 14:
        memset(converted14, 0, sizeof(converted14));
        itfConvertText(converted14, (char *)D_003BAA74 + value * 0x11);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)converted14);
        break;
    case 8:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003BAA88 + value * 7);
        break;
    case 2:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003BAA70 + value * 0x11);
        break;
    case 15:
        memset(converted15, 0, sizeof(converted15));
        itfConvertText(converted15, (char *)D_003BAA70 + value * 0x11);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)converted15);
        break;
    case 11:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003BAA90 + value * 0x11);
        break;
    case 12:
        memset(converted12, 0, sizeof(converted12));
        itfConvertText(converted12, (char *)D_003BAA90 + value * 0x11);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)converted12);
        break;
    case 3:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003BAA84 + value * 0x19);
        break;
    case 13:
        memset(converted13, 0, sizeof(converted13));
        itfConvertText(converted13, (char *)D_003BAA84 + value * 0x19);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)converted13);
        break;
    case 4:
        converted = (char *)number;
        if (value >= 1001) {
            *(u16 *)converted = 0xB280;
            number[1] = 0;
        } else {
            if (value < 0) {
                value = -value;
                *(u16 *)converted = 0xA280;
                converted += 2;
            }
            converted = (char *)txtFormatNumberU16(value, (u16 *)converted);
            ((u16 *)converted)[0] = 0xA680;
            ((u16 *)converted)[1] = 0;
        }
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)number);
        break;
    case 5:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003BAA8C + value * 0x11);
        break;
    case 6:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)D_00347C68[value]);
        break;
    case 7:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)D_003482A8[value]);
        break;
    case 9:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)D_0032ACA8[value]);
        break;
    case 10:
        break;
    }
}

/* Replace a text slot with a copied NUL-terminated string; sourceAddress
 * remains a raw address in the existing interface. */
void itfMesCopyStringToWindowTableSlot(s32 window, u32 slotIndex, u32 sourceAddress) {
    func_0019D460((u32)itfWindowSlots[window].mes, slotIndex, sourceAddress, 0);
}

/* Legacy short-arity entry point: retain its one-argument copier call.
 * This function does not specify the copier's other parameters. */
void func_0019C868(s32 window) {
    func_0019D460((u32)itfWindowSlots[window].mes);
}

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
ItfMesSub *itfMesSetSubResource(s32 window, ItfMesSub *sub) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesSub *previous = mes->sub;
    ItfMesEntry *entry;
    s32 temporaryFontEntry;

    mes->sub = sub;
    itfMesRelocate(sub);
    entry = itfMesGetNextEntry(sub);
    temporaryFontEntry = 0;
    if (sub->magic == ITF_MES_MAGIC_MSG1) {
        temporaryFontEntry = *(s32 *)((u8 *)entry + 8);
    }
    mes->temporaryFontEntry = temporaryFontEntry;
    return previous;
}

/* Look up an entry-table item; only the item index has a bounds check. */
u32 itfMesGetEntryTableItem(s32 window, s32 entryIndex, s32 itemIndex) {
    return itfMesGetTableItem(itfMesGetEntry(itfWindowSlots[window].mes, entryIndex)->table, itemIndex);
}

extern void func_0019E048(ItfMesBlkA4 *blk, s32 arg1, s32 arg2);
extern void itfPanelReleasePrimitiveResources(void *primitive);

/* Refresh changed panel values only when both content-status pairs are clear.
 * The first value is stored as a halfword; release the old primitive afterward. */
void itfMesSetWindowPageAndRefresh(s32 window, s32 firstValue, s32 secondValue) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlkA4 *panelBlock = &mes->blkA4;

    if ((mes->flags & ITF_MES_CONTENT_STATUS_MASK) == 0) {
        if (mes->unk12 != firstValue || panelBlock->unk28 != secondValue) {
            mes->unk12 = firstValue;
            func_0019E048(panelBlock, firstValue, secondValue);
            if (panelBlock->unk4 != NULL) {
                itfPanelReleasePrimitiveResources(panelBlock->unk4);
                panelBlock->unk4 = NULL;
            }
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C9F0);

/* Build and release a temporary glyph chain to measure the widest grouped row.
 * Empty tables and zero/missing encoded items return zero. */
s32 itfMesMeasureEntryItem(s32 window, s32 entryIndex, s32 itemIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesTable *table;
    u32 encodedText;
    ItfMesNode *glyphChain;
    s32 textExtent;

    table = itfMesGetEntry(mes, entryIndex)->table;
    if (table->count == 0) {
        return 0;
    }
    itfMesInitCharTable((s32 *)mes->textSlots.addresses);
    if (mes->temporaryFontEntry != 0) {
        frFontLoadTemporaryEntry(mes->temporaryFontEntry);
    }
    encodedText = itfMesGetTableItem(table, itemIndex);
    if (encodedText == 0) {
        return encodedText;
    }
    glyphChain = itfDrawDefaultColorText(0, 0, encodedText, 0);
    textExtent = itfMesMaxGroupedExtent(glyphChain);
    frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyphChain);
    return textExtent;
}

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesSetEntryLastColorChannel(s32 window, u8 channelValue) {
    itfWindowSlots[window].mes->entryBlock.color[3] = channelValue;
}

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesSetWindowCallbackAddress(s32 window, u32 callbackAddress) {
    itfWindowSlots[window].mes->callbackAddress = callbackAddress;
}

/* Initialize the fixed window pool, load its texture and register the named
 * calculation/drawing tasks. Keep the existing task flags and callback casts. */
void itfMesInit(void) {
    ItfMesPoolNode *poolNode;

    itfMesWork.activeWindowCount = 0;
    itfMesWork.unk8 = 0;
    itfMesWork.flags = 0;
    itfMesWork.windowTexture = itfLoadTextureFromAsset("/itf/MESWIN.TMX");
    itfInitPool(&itfMesWork.pool, itfMesWork.nodes, ITF_MES_WINDOW_CAPACITY, ITF_MES_POOL_NODE_BYTES);
    for (poolNode = itfMesWork.pool.firstFree; poolNode != NULL; poolNode = poolNode->next) {
        poolNode->stateAddress = 0;
    }
    kwlnTaskCreate("CalcMsgMng", 0x409, 0, 0, (void (*)(void))sndVisitQueuedResources, sndFlushMessageQueue, NULL);
    kwlnTaskCreate("DrawMsgMng", 0x2B1A, 0, 0, func_0019F6A0, sndFlushMessageQueue, NULL);
}

/* Set manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesSetGlobalFlags(u32 mask) {
    itfMesWork.flags |= mask;
}

/* Clear manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesClearGlobalFlags(u32 mask) {
    itfMesWork.flags &= ~mask;
}

/* Read the manager's 16-bit flags, distinct from itfMessageFlags. */
u16 itfMesGetGlobalFlags(void) {
    return itfMesWork.flags;
}


struct ItfMesWindowRec {
    u8 pad00[0xC];
    ItfMesState *mes;
    s32 handle;
};

extern s8 D_00324510[];
extern char *D_00357FA0[];
extern char *D_003BB200[2];
extern char D_003BB208[];
extern char D_003BB210[];
extern char D_003BB218[];
extern char D_003BB220[];
extern char D_003BB228[];
extern SdfPoolNode kwlnPositionedTextSurface;
extern s32 sdfCreateResetPacketList(void);
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);
extern void sdfAppendPacket(SdfListHead *list, u32 packetAddress);
extern void kwlnDrawSpriteCell();
extern ItfMesWindowRec *func_0019FA70(ItfMesWindowRec *window);
extern void itfAdjustPanelBoundsWithPad(ItfMesBlkA4 *panel, s32 selectedItem);
extern void sndStepIndexByPad(ItfMesBlkA4 *panel);

/* Run and draw the interactive message-layout inspector. */
INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1480);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1490);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14A0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14B0);

s32 itfMesRunPanelLayoutInspector(void) {
    ItfMesBlkA4 *panel;
    UiSprite *panelSprite;
    s32 *position;
    SdfListHead *packetList;
    s32 item;
    s32 y;
    const char *format;
    const char *marker;
    const char *positionFormat;

    if (D_00357D80.window == NULL) {
        ItfMesWindowRec *window = func_0019FA70(NULL);

        D_00357D80.window = window;
        if (window == NULL) {
            return 0;
        }
    }
    {
        ItfMesDebugState *debug = &D_00357D80;

        panel = &debug->window->mes->blkA4;
        switch (debug->mode) {
        case 0:
            if (D_00324510[0x36] & 2) {
                if (--debug->selectedItem < 0) {
                    debug->selectedItem = 2;
                }
            } else if (D_00324510[0x37] & 2) {
                if (++debug->selectedItem >= 5) {
                    debug->selectedItem = 0;
                }
            }
            if (D_00324510[0x31] < 0) {
                ItfMesDebugState *confirmDebug = &D_00357D80;

                switch (confirmDebug->selectedItem) {
                case 0:
                case 1:
                case 2:
                    confirmDebug->mode = 1;
                    break;
                case 3:
                    confirmDebug->mode = 2;
                    break;
                case 4:
                    confirmDebug->window = func_0019FA70(confirmDebug->window);
                    break;
                }
            } else if (D_00324510[0x33] < 0) {
                return -1;
            }
            break;
        case 1:
            itfAdjustPanelBoundsWithPad(panel, debug->selectedItem);
            if (D_00324510[0x33] < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            sndStepIndexByPad(panel);
            if (D_00324510[0x33] < 0) {
                D_00357D80.mode = 0;
            }
            break;
        }
    }

    packetList = (SdfListHead *)sdfCreateResetPacketList();
    kwlnDrawSpriteCell(packetList, 0x10, 0x10, 0x1E, 9);
    format = D_003BB218;
    y = 0x7A00;
    for (item = 0; item < 5; item++, y += 0x60) {
        if (D_00357D80.selectedItem == item) {
            marker = D_003BB200[D_00357D80.mode != 0];
        } else {
            marker = D_003BB220;
        }
        sdfAppendPacket(packetList,
                        (u32)sdfCreateFormattedSifCommand(0x7180, y, 0xFFFFF0, 0,
                                                     format, marker, D_00357FA0[item]));
    }

    positionFormat = "( %3d,%3d )";
    panelSprite = panel->unk4;
    position = &panelSprite->left;
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7A00, 0xFFFFF0, 0,
                                                 positionFormat, position[0] >> 4,
                                                 position[1] >> 3));
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7A60, 0xFFFFF0, 0,
                                                 positionFormat, position[2] >> 4,
                                                 position[3] >> 3));
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7B20, 0xFFFFF0, 0,
                                                 D_003BB228, panel->unk28));
    position = &D_00357D80.window->mes->blkA4.offsetLeft;
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7180, 0x7C40, 0xFFFFF0, 0,
                                                 "OFFSET : %3d,%3d - %3d,%3d",
                                                 position[0] >> 4,
                                                 position[1] >> 3,
                                                 position[2] >> 4,
                                                 position[3] >> 3));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, packetList);
    return 0;
}

/* Reset the three otherwise unnamed message-state words. */
void itfMesClearGlobalWords(void) {
    D_00357D80.selectedItem = 0;
    D_00357D80.mode = 0;
    D_00357D80.window = NULL;
}

/* Slot record reached through the pool base, one 0x14 bytes per window. */
extern ItfMesWindowRec D_003D6EC0[];

extern void btlReleaseEffectResourceHandles();

extern void itfReleaseUiResourceSlotHandles(s32 *arg0);


extern void itfReleasePoolNode();

/* Tear down installed content/resources, return the pool record and decrement
 * the pool-array-relative counter. An empty record is a no-op. */
void itfMesDestroyWindow(s32 window) {
    ItfMesWindowRec *windowRecord = &D_003D6EC0[window];
    ItfMesState *mes = windowRecord->mes;

    if (mes != NULL) {
        itfMesCleanupWindow(window, 1);
        itfMesResetWindow(window);
        btlReleaseEffectResourceHandles(mes);
        itfReleaseUiResourceSlotHandles((s32 *)&mes->textSlots);
        mes->flags = 0;
        sdfReleaseResourceAllocation((SdfResource *)windowRecord->handle);
        windowRecord->mes = NULL;
        itfReleasePoolNode(windowRecord, (u8 *)D_003D6EC0 - 0x10);
        ((ItfMesGlobals *)((u8 *)D_003D6EC0 - 0x20))->activeWindowCount -= 1;
    }
}

/* Relocate packed payload words once, then set the resource's relocated marker. */
/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void itfMesRelocate(ItfMesBin *bin) {
    if (bin->relocated == 0) {
        sdfRelocatePackedResourceWords((int *)bin->data, (int)bin->data,
                      (u8 *)bin + bin->fixupOff, bin->fixupSize);
        bin->relocated = 1;
    }
}

/* Recognize MSG0/MSG1 magic only; this is not complete format validation. */
u32 itfMesIsMsgData(ItfMesBin *bin) {
    u32 isMessageResource;

    isMessageResource = 0;
    if (bin->magic == ITF_MES_MAGIC_MSG0 || bin->magic == ITF_MES_MAGIC_MSG1) { /* "MSG0"/"MSG1" */
        isMessageResource = 1;
    }
    return isMessageResource;
}

/* Address an installed resource entry; caller supplies an in-range index. */
ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 entryIndex) {
    ItfMesEntry *entries = mes->sub->entries;

    return &entries[entryIndex];
}

/* Return the trailing entry at entryCount; do not advance or validate the count. */
ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub) {
    ItfMesEntry *entries = sub->entries;

    return &entries[sub->entryCount];
}

/* Return the entry's raw item-list word; the entry index is not validated. */
u32 itfMesGetWindowEntryItems(s32 window, s32 entryIndex) {
    return itfMesGetEntry(itfWindowSlots[window].mes, entryIndex)->itemList;
}

/* Read the installed sub-resource's entry count without slot/resource validation. */
u32 itfMesGetEntryCount(s32 window) {
    return itfWindowSlots[window].mes->sub->entryCount;
}

/* Return an encoded item word, or zero for a negative/past-end item index. */
u32 itfMesGetTableItem(ItfMesTable *table, s32 itemIndex) {
    s32 itemCount = table->count;

    if (itemIndex < 0 || itemIndex >= itemCount) {
        return 0;
    }
    return table->items[itemIndex];
}

/* Rebuild colored entry glyphs, synchronize interface flags/hooks and cache
 * the span count. Existing span helpers require a valid created chain. */
void itfMesBuildEntryGlyph(ItfMesState *mes) {
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;
    s32 glyphAddress;
    s32 previousGlyphAddress;
    s32 lipsValue;
    s8 interfaceMask;
    void (*callback)();

    previousGlyphAddress = (s32)entryBlock->glyphChain;
    if (previousGlyphAddress != 0) {
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)previousGlyphAddress);
        entryBlock->glyphChain = NULL;
    }
    glyphAddress = itfDrawCustomColorText((s32)mes->entryBlock.x, (s32)entryBlock->y, entryBlock->color[0], entryBlock->color[1], entryBlock->color[2], entryBlock->color[3],
                          itfMesGetTableItem(entryBlock->table, entryBlock->itemIndex), 0);
    if (mes->unk12 == 3) {
        if (func_00195E60(glyphAddress) == 1) {
            func_00196088(0x1000, 0xC60, glyphAddress);
        } else {
            func_00196088(0x1000, 0xBF8, glyphAddress);
        }
    }
    if (!(mes->flags & ITF_MES_SKIP_CONTEXT_ENABLE_BIT) && (itfMesWork.flags & 1)) {
        itfMesEnableUnflaggedNodeContexts((ItfMesNode *)glyphAddress);
    }
    itfMesCopyGlyphShade(glyphAddress, entryBlock);
    interfaceMask = itfTestTextInterfaceMask(3);
    entryBlock->unk11 = interfaceMask;
    /* Copy interface-mask bit 0x2 into window flag 0x10000. */
    if (interfaceMask & 2) {
        mes->flags |= 0x10000;
    } else {
        mes->flags &= 0xFFFEFFFF;
    }
    if (itfTestTextInterfaceMask(4) != 0) {
        callback = (void (*)())mes->callbackAddress;
        if (callback != NULL) {
            callback();
        }
    }
    if (itfTestTextInterfaceMask(8) != 0) {
        lipsValue = func_00196BD8();
        evtLipsExecFunction(lipsValue, func_00196BE0());
    }
    itfMesSetNodeChainRenderValue((ItfMesNode *)glyphAddress, mes->renderValue);
    entryBlock->unk16 = itfMesCountSpanSteps(itfMesGetLastNode(glyphAddress), glyphAddress);
    entryBlock->glyphChain = (FrFontGlyph *)glyphAddress;
}

/* Replace a retained text-slot allocation. Nonpositive byteCount copies the
 * padded string span; positive counts copy that many bytes into a zeroed block.
 * DDS1 string mode intentionally has no memset. Keep the K&R declaration form. */
void func_0019D460(mes, slotIndex, source, byteCount)
    ItfMesState *mes;
    s32 slotIndex;
    const char *source;
    s32 byteCount;
{
    ItfMesTextSlots *slots = &mes->textSlots;
    u32 *textAddress = &slots->addresses[slotIndex];
    s32 allocationBytes;

    if (*textAddress != 0) {
        sdfReleaseResourceAllocation((SdfResource *)slots->handles[slotIndex]);
        *textAddress = 0;
    }
    if (byteCount <= 0) {
        allocationBytes = (strlen(source) + ITF_MES_STRING_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
        slots->handles[slotIndex] = (u32)sdfAllocGeneralBlock(allocationBytes);
        *textAddress = sdfResourceRetainAddress((SdfResource *)slots->handles[slotIndex]);
        /* String mode copies the padded span, rather than only strlen + 1. */
        memcpy((void *)*textAddress, source, allocationBytes);
        return;
    }
    allocationBytes = (byteCount + ITF_MES_BINARY_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
    slots->handles[slotIndex] = (u32)sdfAllocGeneralBlock(allocationBytes);
    *textAddress = sdfResourceRetainAddress((SdfResource *)slots->handles[slotIndex]);
    memset((void *)*textAddress, 0, allocationBytes);
    memcpy((void *)*textAddress, source, byteCount);
}

/* Install all replacement-text addresses into the font's fixed slot table. */
void itfMesInitCharTable(s32 *table) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < ITF_MES_TEXT_SLOT_COUNT; slotIndex++) {
        func_00196BC0(slotIndex, table[slotIndex]);
    }
}

/* Read the requested word through the trailing entry's first word, without
 * index validation. Preserve this game's existing request-record interface. */
u32 itfMesGetNextEntrySelectedItem(ItfMesIndex *req) {
    u32 *items;

    items = *(u32 **)itfMesGetNextEntry(req->handle);
    return items[req->index];
}

/* Count clear low bits by consuming one bit per iteration. Nonpositive counts
 * return zero; counts beyond the mask width also count shifted-in zero bits. */
s32 itfMesCountZeroBits(s32 bitCount, u32 remainingMask) {
    s32 zeroCount;
    u32 bitValue;

    zeroCount = 0;
    while (bitCount > 0) {
        bitValue = remainingMask & 1;
        remainingMask >>= 1;
        bitCount--;
        if (bitValue == 0) {
            zeroCount++;
        }
    }
    return zeroCount;
}

/* Append text for each clear mask bit. Masked items are still consumed, while
 * y advances only for built text. Return NULL if nothing was built. */
ItfMesNode *itfMesBuildNodeRows(u32 *items, s32 itemCount, u32 mask, s32 x, s32 y, s32 renderValue) {
    ItfMesNode *glyphChain = NULL;
    s32 itemIndex;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++, items++) {
        if (mask & 1) {
            mask >>= 1;
        } else {
            glyphChain = (ItfMesNode *)itfDrawCustomColorText(x, y, 0, 0, 0, ITF_MES_DEFAULT_COLOR_VALUE, *items, glyphChain);
            mask >>= 1;
            y += glyphChain->rowHeightUnits << ITF_MES_TEXT_Y_SHIFT;
        }
    }
    if (glyphChain != NULL) {
        itfMesSetNodeChainRenderValue(glyphChain, renderValue);
        itfMesRecolorNodeChildren(glyphChain, ITF_MES_DEFAULT_COLOR_VALUE);
    }
    return glyphChain;
}

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
ItfMesNode *itfMesTrimGlyphChainToRow(ItfMesNode *node, s32 from, s32 to) {
    s32 rowsToDiscard = to - from - 1;
    s32 rowY = node->y;
    ItfMesNode *nextNode;
    ItfMesNode *rowHead;
    ItfMesNode *rowTail;

    while (rowsToDiscard > 0) {
        while (rowY == node->y) {
            nextNode = node->next;
            node->forward = NULL;
            node->chainHead = node;
            node->next = NULL;
            frFontQueueGlyphInSelectedSlot((FrFontGlyph *)node);
            node = nextNode;
            if (node == NULL) {
                return NULL;
            }
        }
        rowsToDiscard--;
        rowY = node->y;
    }
    rowHead = node;
    do {
        rowTail = node;
        node = node->next;
    } while (node != NULL && rowY == node->y);
    while (node != NULL) {
        nextNode = node->next;
        node->forward = NULL;
        node->chainHead = node;
        node->next = NULL;
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)node);
        node = nextNode;
    }
    rowHead->forward = NULL;
    rowTail->next = NULL;
    for (node = rowHead; node != NULL; node = node->next) {
        /* Despite its current name, this link points to the retained row tail. */
        node->chainHead = rowTail;
    }
    return rowHead;
}

/* Return the final next-linked node; input must be non-NULL. */
ItfMesNode *itfMesGetLastNode(ItfMesNode *node) {
    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

/* Copy shade bytes in their observed order; the fourth channel is firstByte/2.
 * Leave the shade component names neutral rather than assuming RGB order. */
void itfMesCopyGlyphShade(ItfMesColorSrc *glyph, ItfMesEntryBlock *entryBlock) {
    ItfMesShade *shade = glyph->shade;

    entryBlock->color[3] = glyph->firstByte >> 1;
    entryBlock->color[0] = shade->shade15;
    entryBlock->color[1] = shade->shade14;
    entryBlock->color[2] = shade->shade16;
}

/* Inclusive step count from the y delta in eighths and the first row's height.
 * Both pointers and a nonzero first-row height are required. */
s32 itfMesCountSpanSteps(ItfMesSpan *last, ItfMesSpan *first) {
    return ((first->y - last->y) >> ITF_MES_TEXT_Y_SHIFT) / first->rowHeightUnits + 1;
}

/* Translate every linked node in text units; a NULL chain is a no-op. */
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

/* Assign the render value across the complete chain; a NULL chain is a no-op. */
/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void itfMesSetNodeChainRenderValue(ItfMesNode *node, s32 renderValue) {
    while (node != NULL) {
        node->renderValue = renderValue;
        node = node->next;
    }
}

/* Skip (last - first - 1) preceding y-groups when positive, then set each
 * child's flag byte in the next row. Requires non-NULL input; past-end is a no-op. */
void itfMesSetRowItemFlag(ItfMesNode *node, s32 first, s32 last, s32 requestedFlags) {
    s32 rowsToSkip = last - first - 1;
    s32 rowY = node->y;
    s32 currentY = rowY;
    ItfMesItem *child;
    u8 flagValue;

    while (rowsToSkip > 0) {
        while (rowY == currentY) {
            node = node->next;
            if (node == NULL) {
                return;
            }
            currentY = node->y;
        }
        rowsToSkip--;
        rowY = currentY;
    }
    flagValue = requestedFlags;
    do {
        for (child = node->child; child != NULL; child = child->next) {
            child->flag14 = flagValue;
        }
        node = node->next;
    } while (node != NULL && rowY == node->y);
}

/* Set every child's flag byte across the parent chain; NULL is a no-op. */
void itfMesSetChildChainFlags(ItfMesNode *node, u8 flagValue) {
    ItfMesItem *child;

    for (; node != NULL; node = node->next) {
        for (child = node->child; child != NULL; child = child->next) {
            child->flag14 = flagValue;
        }
    }
}

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
/* Persona 4 func_0027a4d0 @ 0027A4D0 (src/itfMesManager.c), recompiled unchanged */
void itfMesRecolorNodeChildren(ItfMesNode *node, u32 color) {
    ItfMesItem *child;

    for (; node != NULL; node = node->next) {
        for (child = node->child; child != NULL; child = child->next) {
            child->colorWord = child->colorWord & ITF_MES_COLOR_BYTE_CLEAR_MASK | color;
        }
    }
}

/* Find the maximum summed advance of contiguous equal-y groups, scaled by 16.
 * NULL returns zero; the initial zero also excludes negative group totals. */
s32 itfMesMaxGroupedExtent(ItfMesNode *node) {
    s32 maxAdvance = 0;

    while (node != NULL) {
        s32 rowY = node->y;
        s32 rowAdvance = 0;

        do {
            rowAdvance += node->advance;
            node = node->next;
        } while (node != NULL && rowY == node->y);
        if (rowAdvance > maxAdvance) {
            maxAdvance = rowAdvance;
        }
    }
    return maxAdvance << ITF_MES_TEXT_X_SHIFT;
}

/* Return the bit index of the zero-based clear-bit ordinal, or 32 if absent.
 * Keep the predecrement; the ordinal itself is not validated. */
s32 itfMesNthClearBit(s32 clearBitsToSkip, u32 mask) {
    s32 bitIndex = 0;
    while (bitIndex < ITF_MES_MASK_BIT_COUNT) {
        if ((mask & 1) == 0) {
            if (--clearBitsToSkip < 0) {
                break;
            }
        }
        bitIndex++;
        mask >>= 1;
    }
    return bitIndex;
}

/* Enable parent contexts whose first child's tested flag is zero.
 * NULL chain is allowed, but every visited node must have a first child. */
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

char *D_003BB200[2] __attribute__((section(".sdata"))) = {D_003BB210, D_003BB208};

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB208);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB210);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB218);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB220);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB228);

