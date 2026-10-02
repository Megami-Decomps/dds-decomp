#include "common.h"

/* Record shared by the matched helpers below; offsets are from retail.
 * frFontEnableContextMode receives the message-window node itself (itfMesManager
 * itfMesEnableUnflaggedNodeContexts passes its chain node straight in). */
typedef struct FrFontCtx {
    union {
        u32 word;            /* 0x0: whole word read by frFontCheckPendingGlyphState */
        struct {
            u8 unk0;         /* 0x0 */
            u8 flag1;        /* 0x1: set by frFontSetFlagAndMeasureGlyphs */
            u8 unk2[2];      /* 0x2 */
        } bytes;
    } u0;
    u32 contextCursor;     /* 0x4: advanced by frFontAdvanceContextCursor */
    u32 unk8;                /* 0x8 */
    union {
        u32 w;                   /* 0xC: word view */
        struct { u8 pad; s8 bD; s8 bE; u8 bF; } b; /* 0xC: byte views */
    } uC;                        /* 0xC: refreshed by frFontSetFlagAndMeasureGlyphs */
    u32 unk10;               /* 0x10 */
    union {
        u32 shifted;         /* 0x14: value stored shifted by frFontStoreShiftedContextValue */
        void *ptr;           /* 0x14: child pointer read by frFontCheckPendingGlyphState */
    } u14;
    u32 unk18;               /* 0x18 */
    s8 flag1C;               /* 0x1C */
    s8 flag1D;               /* 0x1D */
    u8 unk1E[0x22];          /* 0x1E */
    u32 mode40;              /* 0x40: set by frFontEnableContextMode */
} FrFontCtx;

/* Triple word block with one getter per word. */
typedef struct FrFontSave {
    u32 unk0; /* 0x0: read by func_00194978 */
    u32 unk4; /* 0x4: read by func_00194988 */
    u32 unk8; /* 0x8: read by func_00194998 */
} FrFontSave;

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
    union {
        u32 word;     /* 0x10: word view */
        u16 half[2];  /* 0x10: halfword views */
    } u10;
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

/* Font slot record (0x24 bytes); `resource` is non-NULL while the slot is loaded. */
typedef struct FrFontEntry {
    u8 unk0[0x18];  /* 0x0 */
    s32 *table;     /* 0x18: per-slot word table */
    void *resource; /* 0x1C: loaded font data */
    u8 unk20[4];    /* 0x20 */
} FrFontEntry;

/* Font system at frFontWork (see game/code_00193C08.c). */
typedef struct FrFontSys {
    FrFontEntry entries[9];   /* 0x0 */
    s32 count;                /* 0x144 */
    s32 itemCount;            /* 0x148 */
    s32 glyphCount;           /* 0x14C */
    s32 itemPool;             /* 0x150 */
    s32 glyphPool;            /* 0x154 */
    u8 unk158[0x3C];          /* 0x158 */
    FrFontGlyph *slots[2];    /* 0x194 */
} FrFontSys;

/* Value record reached through frFontResourceRecords entries. */
typedef struct FrFontRecVal {
    u8 unk0[0x10];
    u16 cellWidth;  /* 0x10: returned by frFontGetGlyphCellWidth */
    u16 cellHeight; /* 0x12: returned by frFontGetGlyphCellHeight */
} FrFontRecVal;

typedef struct FrFontRec {
    FrFontRecVal *val; /* 0x0 */
    u8 unk4[0x20];
} FrFontRec;

extern u32 frFontContextCursorSpacing;

extern u8 frFontSharedGlyphFlags;

extern u32 frFontSharedRenderFlags;

extern u8 D_003BB180[];

extern FrFontSave D_003D6DC4;

extern FrFontSys frFontWork;

extern FrFontRec frFontResourceRecords[];

extern s32 frFontDefaultGlyphCellSize;

extern FrFontGlyph *D_003D6E14[];

extern u32 frFontMeasureGlyphChain(void *chain);

extern void frFontSetContextPair(FrFontCtx *ctx, u32 first, u32 second);

void frFontCreateContext();

extern void frFontSetContextEncodedByte(FrFontCtx *ctx, s32 value);

extern s32 kwlnGetDrawBufferIndex(void);

extern s32 frFontAdvanceGlyphFade(FrFontGlyph *glyph);

extern FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph);

extern FrFontGlyph *func_00194BA0(FrFontGlyph *glyph, s32 option);

extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *previous, FrFontGlyph *next, s32 positionNext);

extern s32 func_001958A0(FrFontGlyph *glyph, s8 mode, u32 flags);

extern FrFontCtx *frFontAppendGlyphFromData(void *text, s8 fontIndex, s8 firstOption, s8 secondOption, s32 previousGlyph);

extern FrFontGlyph *func_001951C8(void *text, s32 fontIndex, s32 firstOption, s32 secondOption, s32 existingGlyph);

void frFontDrawGlyphWithSharedFlags(FrFontGlyph *glyph, s8 mode);

FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *previous, FrFontGlyph *next);

typedef struct TextStyleNode {
    u8 pad00[4];
    u32 x;
    u32 y;
    u8 pad0C[4];
    u32 color;
    u8 pad14[8];
    struct TextStyleNode *firstChild;
    u8 pad20[4];
    struct TextStyleNode *next;
    struct TextStyleNode *nextChild;
} TextStyleNode;

extern void frFontEnsureSlotLoaded(s32 id, const char *path);

void frFontLoadDefaultFonts(void) {
    frFontEnsureSlotLoaded(0, "/font/font0.fnt");
    frFontEnsureSlotLoaded(1, "/font/font1.fnt");
    frFontEnsureSlotLoaded(2, "/font/font2.fnt");
    frFontEnsureSlotLoaded(3, "/font/font3.fnt");
}

void frFontFreeAllEntries(void) {
    s32 i;

    for (i = 0; i < 9; i++) {
        if (frFontWork.entries[i].resource != NULL) {
            frFontFreeEntry((u8)i);
        }
    }
}

INCLUDE_ASM(const s32, "interface/frFont", func_001946C8);

/* Record referenced by a glyph's first child pointer. */
typedef struct FrFontItem {
    u16 index;    /* 0x0 */
    u8 unk2[2];   /* 0x2 */
    u16 refs;     /* 0x4 */
    u8 unk6[2];   /* 0x6 */
    void *node;   /* 0x8 */
} FrFontItem;

void frFontReleaseUnreferencedGlyphItem(FrFontGlyph *glyph) {
    FrFontItem *item = (FrFontItem *)glyph->firstChild;

    if (item != NULL) {
        if (item->refs == 0) {
            frFontWork.entries[glyph->u14.b[1]].table[item->index] = 0;
            frFontListInsert(item->node);
            frFontWork.count--;
        }
    }
}

FrFontGlyph *frFontAdvanceOrRetainFadingGlyph(FrFontGlyph *glyph) {
    if (frFontAdvanceGlyphFade(glyph) != 0) {
        return glyph;
    }
    return frFontReleaseGlyphChain(glyph);
}

extern s32 itfEnqueueMemNode(void *node, s32 pool);

/* Release a glyph chain, walking back along `previous`: drop each child's record reference (releasing the record once unreferenced), return the child and then the glyph itself to their node pools, and keep the live counts. */
FrFontGlyph *frFontReleaseGlyphChain(FrFontGlyph *glyph) {
    FrFontGlyph *current = glyph;
    FrFontGlyph *child;
    FrFontGlyph *nextChild;
    FrFontGlyph *previous;

    if (current == NULL) {
        return NULL;
    }
    do {
        child = current->firstChild;
        while (child != NULL) {
            nextChild = child->next;
            if (child->unk20 == NULL) {
                ((FrFontItem *)child->firstChild)->refs--;
                frFontReleaseUnreferencedGlyphItem(child);
            }
            itfEnqueueMemNode(child, frFontWork.itemPool);
            frFontWork.itemCount--;
            child = nextChild;
        }
        previous = current->previous;
        itfEnqueueMemNode(current, frFontWork.glyphPool);
        current = previous;
        frFontWork.glyphCount--;
    } while (current != NULL);
    return NULL;
}

s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *glyph) {
    FrFontGlyph **slot = &D_003D6E14[kwlnGetDrawBufferIndex() & 0xFF];

    *slot = frFontLinkGlyph(*slot, glyph, 0);
    return 0;
}

s32 func_00194978(void) {
    return D_003D6DC4.unk0;
}

s32 func_00194988(void) {
    return D_003D6DC4.unk4;
}

s32 func_00194998(void) {
    return D_003D6DC4.unk8;
}

u32 func_001949A8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_001949B0);

INCLUDE_ASM(const s32, "interface/frFont", func_00194BA0);

FrFontGlyph *frFontAppendClonedGlyph(FrFontGlyph *source, FrFontGlyph *destination) {
    FrFontGlyph *glyph = func_00194BA0(source, 0);

    if (glyph == NULL) {
        return destination;
    }
    return frFontLinkGlyphAfterPrevious(destination, glyph);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00194D20);

typedef struct FrFontSegments {
    void *first;
    void *second;
    void *third;
} FrFontSegments;

extern void *func_002CFEB8(s32 size);
extern void itfSplitRelativeSegments(void *block, FrFontSegments *out);
extern void func_00198088(void *dst, s32 option, void *block, FrFontSegments *segments);

void *frFontCloneEntryResource(u8 index, s32 option) {
    FrFontEntry *entry = &frFontWork.entries[index];
    void *dst = func_002CFEB8(0x120);
    FrFontSegments segments;

    itfSplitRelativeSegments(entry->resource, &segments);
    func_00198088(dst, option, entry->resource, &segments);
    return dst;
}

extern u16 frFontGetSlotCellWidth(s32 index);
extern u16 frFontGetSlotCellHeight(s32 index);
extern FrFontItem *func_00194D20(s32 first, s32 second, void *resource, s32 count);

/* Return the cached item for `id` in the glyph's font slot (taking a reference),
 * or clone the slot resource and create and cache a new one. */
FrFontItem *frFontRetainOrCreateCachedItem(FrFontGlyph *glyph, s32 id) {
    FrFontEntry *entry = &frFontWork.entries[glyph->u14.b[1]];
    FrFontItem *item = ((FrFontItem **)entry->table)[id];
    void *resource;

    if (item != NULL) {
        item->refs++;
        return item;
    }
    resource = frFontCloneEntryResource(glyph->u14.b[1], id);
    item = func_00194D20(frFontGetSlotCellWidth(glyph->u14.b[1]), frFontGetSlotCellHeight(glyph->u14.b[1]), resource, 1);
    item->index = id;
    ((FrFontItem **)entry->table)[id] = item;
    frFontWork.count++;
    return item;
}

/* Initialize a glyph record while retaining only the high bits of its flags. */
void frFontSetupGlyph(FrFontGlyph *glyph, s16 glyphId, s8 byte1, s8 byte0, s32 flags, s8 byte2) {
    glyph->u14.b[1] = byte1;
    glyph->u14.b[0] = byte0;
    glyph->u14.b[2] = byte2;
    glyph->u0.h = glyphId;
    glyph->u10.word = flags & ~0xFF;
    glyph->u14.b[3] = frFontSharedGlyphFlags;
    glyph->x = 0;
    glyph->y = 0;
    glyph->advance = 0;
    glyph->unk2 = 0;
    glyph->firstChild = NULL;
    glyph->unk20 = NULL;
    glyph->previous = NULL;
    glyph->next = NULL;
}

/* Reset a glyph as a standalone chain head (0x80 is the empty glyph sentinel). */
void frFontInitGlyph(FrFontGlyph *glyph) {
    glyph->u0.b.b0 = -0x80;
    glyph->x = 0;
    glyph->y = 0;
    glyph->u0.b.b1 = 0;
    glyph->advance = 0;
    glyph->u14.w = 0;
    glyph->previous = NULL;
    glyph->next = NULL;
    glyph->chainHead = glyph;
    glyph->firstChild = NULL;
    glyph->unk20 = NULL;
    glyph->unk18.w = 0;
    glyph->unk30 = 0;
    glyph->unk34 = 0;
    glyph->unk38 = 0;
    glyph->unk3C = 0;
    glyph->unk40 = 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195010);

/* Append text glyphs; a negative fontIndex keeps the current font selection. */
FrFontCtx *frFontAppendGlyphFromData(void *text, s8 fontIndex, s8 firstOption, s8 secondOption, s32 previousGlyph) {
    FrFontGlyph *glyph = func_001951C8(text, fontIndex, firstOption, secondOption, 0);

    if (glyph == NULL) {
        return (FrFontCtx *)previousGlyph;
    }
    return (FrFontCtx *)frFontLinkGlyphAfterPrevious((FrFontGlyph *)previousGlyph, glyph);
}

INCLUDE_ASM(const s32, "interface/frFont", func_001951C8);

void frFontSetContextEncodedByte(FrFontCtx *ctx, s32 value) {
    s32 val = (value & 0xFF) * 2;

    if (val >= 0x81) {
        ctx->u0.bytes.unk0 = -0x80;
    } else {
        ctx->u0.bytes.unk0 = val;
    }
}

void frFontEnableContextMode(FrFontCtx *ctx) {
    ctx->mode40 = 1;
    frFontSetContextEncodedByte(ctx, 0x80);
}

void frFontSetFlagAndMeasureGlyphs(FrFontCtx *ctx, u8 flag) {
    ctx->u0.bytes.flag1 = flag;
    ctx->uC.w = frFontMeasureGlyphChain(ctx);
}

void frFontSetGlyphChainDimensions(FrFontGlyph *glyph, s32 advance, s32 height) {
    FrFontGlyph *head = glyph;
    FrFontGlyph *child;

    head->u10.half[0] = advance;
    head->u10.half[1] = height;
    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->advance = advance;
            child->unk18.b[0] = advance;
            child->unk18.b[1] = height;
        }
    }
    head->advance = frFontMeasureGlyphChain(head);
}

void frFontSetContextPair(FrFontCtx *ctx, u32 first, u32 second) {
    ctx->contextCursor = first;
    ctx->unk8 = second;
}

void frFontStoreShiftedContextValue(FrFontCtx *ctx, u32 value) {
    ctx->u14.shifted = value >> 4;
}

void frFontSetChainFlag(FrFontGlyph *glyph, u8 value) {
    FrFontGlyph *child;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->u14.b[0] = value;
        }
    }
}

void frFontSetChildColors(TextStyleNode *entry, u32 color) {
    for (; entry != NULL; entry = entry->next) {
        TextStyleNode *child;
        for (child = entry->firstChild; child != NULL; child = child->nextChild) {
            child->color = color;
        }
    }
}

void frFontAddSharedGlyphFlags(s32 flags) {
    flags |= frFontSharedGlyphFlags;
    frFontSharedGlyphFlags = flags;
}

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frFontClearFlagBits(u8 mask) {
    u8 old = frFontSharedGlyphFlags;

    frFontSharedGlyphFlags = old & ~mask;
    return old;
}

void frFontSetSharedRenderFlags(u32 flags) {
    frFontSharedRenderFlags = flags;
}

s32 frFontAdvanceGlyphFade(FrFontGlyph *glyph) {
    FrFontGlyph *child;
    s32 changed = 0;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            if (child->u14.b[2] == 2) {
                u32 flags = child->u10.word;
                s32 alpha = flags & 0xFF;

                if (alpha != 0) {
                    alpha -= 8;
                    if (alpha < 0) {
                        alpha = 0;
                    }
                    child->y += 0x10;
                    child->u10.word = (flags & ~0xFF) | alpha;
                    changed = 1;
                }
            }
        }
    }
    return changed;
}

INCLUDE_ASM(const s32, "interface/frFont", func_001955D8);

void frFontDrawGlyphInDefaultMode(FrFontGlyph *glyph) {
    frFontDrawGlyphWithSharedFlags(glyph, 0);
}

void frFontDrawGlyphWithSharedFlags(FrFontGlyph *glyph, s8 mode) {
    func_001958A0(glyph, mode, frFontSharedRenderFlags);
}

INCLUDE_ASM(const s32, "interface/frFont", func_001958A0);

/* Advance one of two cached glyph slots, chosen by the current font index. */
s32 frFontAdvanceSelectedGlyphSlot(void) {
    s32 selection = (kwlnGetDrawBufferIndex() & 0xFF) == 0;
    u8 *base = (u8 *)&frFontWork;
    FrFontGlyph **slot = (FrFontGlyph **)(base + selection * 4 + 0x194);

    *slot = frFontReleaseGlyphChain(*slot);
    return 0;
}

FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *previous, FrFontGlyph *next) {
    return frFontLinkGlyph(previous, next, 1);
}

/* Splice chains; optionally place the new head after the previous glyph's advance. */
FrFontGlyph *frFontLinkGlyph(FrFontGlyph *previous, FrFontGlyph *next, s32 positionNext) {
    if (previous == NULL) {
        return next;
    }
    if (next == NULL) {
        return previous;
    }
    previous->next = next->chainHead;
    next->chainHead->previous = previous;
    next->chainHead = previous->chainHead;
    if (positionNext == 1) {
        next->x = previous->x + (previous->advance << 4);
        next->y = previous->y;
    }
    return next;
}

void frFontLoadTemporaryEntry(u32 fontData) {
    frFontBindResourceSections(8, fontData, 0);
}

void frFontFreeTemporaryEntry(void) {
    frFontFreeEntry(8);
}

/* Count single-byte characters and two-byte lead/trail sequences. */
s32 frFontCountChars(s8 *str) {
    s32 count = 0;

    while (*str != 0) {
        if (*str >= 0) {
            str++;
        } else {
            str += 2;
        }
        count++;
    }
    return count;
}

/* Sum child advances, including one spacing value per child (even the last). */
u32 frFontMeasureGlyphChain(void *chain) {
    FrFontGlyph *glyph = chain;
    FrFontGlyph *node = glyph->firstChild;
    s32 total = 0;

    if (node != NULL) {
        s8 spacing = glyph->u0.b.b1;

        do {
            total += node->advance;
            node = node->next;
            total += spacing;
        } while (node != NULL);
    }
    return total;
}

u32 frFontMeasureLines(FrFontGlyph *glyph) {
    FrFontGlyph *line;
    FrFontGlyph *node;
    s32 total = 0;

    for (line = glyph->chainHead; line != NULL; line = line->next) {
        node = line->firstChild;
        if (node != NULL) {
            s8 spacing = line->u0.b.b1;

            do {
                total += node->advance;
                node = node->next;
                total += spacing;
            } while (node != NULL);
        }
    }
    return total;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195CD8);

u32 frFontGetGlyphCellWidth(u8 fontIndex) {
    s32 index = fontIndex;

    if (index < 2) {
        if (index >= 0) {
            return frFontDefaultGlyphCellSize;
        }
    }
    return frFontResourceRecords[index].val->cellWidth;
}

u32 frFontGetGlyphCellHeight(u8 fontIndex) {
    s32 index = fontIndex;

    if (index < 2) {
        if (index >= 0) {
            return frFontDefaultGlyphCellSize;
        }
    }
    return frFontResourceRecords[index].val->cellHeight;
}

void frFontResetContextCursorSpacing(void) {
    frFontContextCursorSpacing = 0x15;
}

void frFontSetContextCursorSpacing(u32 value) {
    frFontContextCursorSpacing = value;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195E60);

INCLUDE_ASM(const s32, "interface/frFont", func_00195ED8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195FA8);
void frFontMoveChainTo(s32 x, s32 y, FrFontGlyph *glyph) {
    FrFontGlyph *node;
    s32 dx;
    s32 dy;

    if (glyph != NULL) {
        node = glyph->chainHead;
        dx = x - node->x;
        dy = y - node->y;
        for (; node != NULL; node = node->next) {
            node->x += dx;
            node->y += dy;
        }
    }
}

INCLUDE_ASM(const s32, "interface/frFont", func_00196088);

INCLUDE_ASM(const s32, "interface/frFont", func_001961B0);

INCLUDE_ASM(const s32, "interface/frFont", func_00196220);

/* Old-style definition: callers invoke it without arguments and rely on $a0. */
void frFontCreateContext(ctx)
    FrFontCtx *ctx;

{
    FrFontCtx *newCtx = frFontAppendGlyphFromData(&D_003BB180, 0, ctx->uC.b.bD, ctx->uC.b.bE, ctx->u14.shifted);

    ctx->u14.ptr = newCtx;
    frFontSetContextEncodedByte(newCtx, ctx->uC.b.bF);
    ctx->flag1C = 0;
}

void frFontCheckPendingGlyphState(FrFontCtx *ctx) {
    s8 pending;

    if (ctx->u14.ptr == NULL) {
        pending = ctx->flag1C;
    } else {
        if (((FrFontGlyph *)ctx->u14.ptr)->firstChild == NULL) {
            ctx->flag1C = 0;
        }
        pending = ctx->flag1C;
    }
    if (pending == 0) {
        pending = ctx->flag1D;
    } else {
        frFontCreateContext();
        pending = ctx->flag1D;
    }
    if (pending != 0) {
                frFontSetContextPair(ctx->u14.ptr, ctx->u0.word, ctx->contextCursor);
        ctx->flag1D = 0;
    }
}

void frFontAdvanceContextCursor(FrFontCtx *ctx) {
    ctx->contextCursor += frFontContextCursorSpacing * 8;
    ctx->flag1C = 1;
    ctx->flag1D = 1;
}

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB160);

INCLUDE_SDATA(const s32, "interface/frFont", frFontContextCursorSpacing);

INCLUDE_SDATA(const s32, "interface/frFont", frFontDefaultGlyphCellSize);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB16C);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB170);

INCLUDE_SDATA(const s32, "interface/frFont", frFontSharedGlyphFlags);

INCLUDE_SDATA(const s32, "interface/frFont", frFontSharedRenderFlags);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB17C);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB180);

