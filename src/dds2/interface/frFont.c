#include "common.h"

typedef struct FrFontRecord {
    u16 id;      /* 0x00 */
    u8 unk02[2];
    u16 refs;    /* 0x04 */
    u8 unk06[2];
    void *list;  /* 0x08 */
} FrFontRecord;

typedef struct FrFontEntry {
    u8 unk00[4];
    void *header;      /* 0x04 */
    s32 count;         /* 0x08 */
    u8 unk0C[4];
    void *table;       /* 0x10 */
    u8 unk14[4];
    s32 *slots;        /* 0x18 */
    void *first;       /* 0x1C */
    u8 unk20[4];
} FrFontEntry; /* 0x24 */

extern u32 frFontMeasureGlyphChain(void *chain);

extern u32 D_00436568;

extern u32 D_00436554;

/* Glyph/record chain walked by func_001958A0/func_00195B78. */
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
    struct FrFontGlyph *firstChild; /* 0x1C: child glyph chain */
    struct FrFontGlyph *unk20; /* 0x20 */
    struct FrFontGlyph *previous; /* 0x24: back-link in the glyph chain */
    struct FrFontGlyph *next; /* 0x28: next glyph in chain */
    struct FrFontGlyph *chainHead; /* 0x2C: first glyph in the linked chain */
    u32 unk30;        /* 0x30 */
    u32 unk34;        /* 0x34 */
    u32 unk38;        /* 0x38 */
    s32 unk3C;        /* 0x3C */
    s32 unk40;        /* 0x40 */
} FrFontGlyph;

extern FrFontGlyph *D_004528B4[];

extern s32 func_00100400(void);

extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *previous, FrFontGlyph *next, s32 positionNext);

/* Triple word block with one getter per word. */
typedef struct FrFontSave {
    u32 unk0; /* 0x0: read by func_00194978 */
    u32 unk4; /* 0x4: read by effAllocSubWork */
    u32 unk8; /* 0x8: read by func_00194998 */
} FrFontSave;

extern FrFontSave D_00452864;

extern u8 D_00436564;

/* Record shared by the matched helpers below; offsets are from retail.
 * func_00195388 receives the message-window node itself (itfMesManager
 * func_0019DB40 passes its chain node straight in). */
typedef struct FrFontCtx {
    union {
        u32 word;            /* 0x0: whole word read by func_001963E0 */
        struct {
            u8 unk0;         /* 0x0 */
            u8 flag1;        /* 0x1: set by func_001953A8 */
            u8 unk2[2];      /* 0x2 */
        } bytes;
    } u0;
    u32 unk4;                /* 0x4 */
    u32 unk8;                /* 0x8 */
    union {
        u32 w;                   /* 0xC: word view */
        struct { u8 pad; s8 bD; s8 bE; u8 bF; } b; /* 0xC: byte views */
    } uC;                        /* 0xC: refreshed by func_001953A8 */
    u32 unk10;               /* 0x10 */
    union {
        u32 shifted;         /* 0x14: value stored shifted by func_00195460 */
        void *ptr;           /* 0x14: child pointer read by func_001963E0 */
    } u14;
    u32 unk18;               /* 0x18 */
    s8 flag1C;               /* 0x1C */
    s8 flag1D;               /* 0x1D */
    u8 unk1E[0x22];          /* 0x1E */
    u32 mode40;              /* 0x40: set by func_00195388 */
} FrFontCtx;

extern void func_0019D010(FrFontCtx *ctx, s32 value);

extern void func_0019D100(FrFontCtx *ctx, u32 first, u32 second);

void frFontCreateContext();

extern u8 D_00436578[];

/* Font system at D_00452720 (see game/code_0019B840.c); the two glyph slots
 * at +0x194/+0x198 are selected by func_00195B10. */
typedef struct FrFontSys {
    FrFontEntry entries[9];   /* 0x0 */
    s32 count;                /* 0x144 */
    u8 unk148[0x4C];          /* 0x148 */
    FrFontGlyph *slots[2];    /* 0x194 */
} FrFontSys;

extern FrFontSys D_00452720;

/* Value record reached through D_00452724 entries. */
typedef struct FrFontRecVal {
    u8 unk0[0x10];
    u16 unk10;
    u16 unk12;
} FrFontRecVal;

typedef struct FrFontRec {
    FrFontRecVal *val; /* 0x0 */
    u8 unk4[0x20];
} FrFontRec;

extern FrFontRec D_00452724[];

extern s32 D_00436558;

extern FrFontGlyph *func_0019CE78(void *glyphData, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern void *func_00328D68(s32 size);

typedef struct FrFontSegments {
    void *first;
    void *second;
    void *third;
} FrFontSegments;

extern void itfSplitRelativeSegments(void *block, FrFontSegments *out);
extern void func_001A00B8(void *dst, s32 option, void *block, FrFontSegments *segments);

extern FrFontGlyph *func_0019C4D0(FrFontGlyph *glyph);

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

extern s32 func_0019D200(FrFontGlyph *glyph);

extern FrFontGlyph *func_0019C850(FrFontGlyph *glyph, s32 option);

FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *previous, FrFontGlyph *next);

void func_0019D530(FrFontGlyph *glyph, s8 mode);

extern s32 func_0019D550(FrFontGlyph *glyph, s8 mode, u32 flags);

extern void func_0019BE20();

void frFontLoadDefaultFonts(void) {
    func_0019BE20(0, "/font/font0.fnt");
    func_0019BE20(1, "/font/font1.fnt");
    func_0019BE20(2, "/font/font2.fnt");
    func_0019BE20(3, "/font/font3.fnt");
}


void frFontFreeAllEntries(void) {
    FrFontEntry *entries = (FrFontEntry *)&D_00452720;
    s32 i;

    for (i = 0; i < 9; i++) {
        if (entries[i].first != NULL) {
            frFontFreeEntry(i & 0xFF);
        }
    }
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019C358);

void func_0019C418(FrFontGlyph *glyph) {
    FrFontRecord *item = (FrFontRecord *)glyph->firstChild;

    if (item != NULL) {
        if (item->refs == 0) {
            D_00452720.entries[glyph->u14.b[1]].slots[item->id] = 0;
            frFontListInsert(item->list);
            D_00452720.count--;
        }
    }
}

FrFontGlyph *func_0019C490(FrFontGlyph *glyph) {
    if (func_0019D200(glyph) != 0) {
        return glyph;
    }
    return func_0019C4D0(glyph);
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019C4D0);

s32 func_0019C5B0(FrFontGlyph *glyph) {
    FrFontGlyph **slot = &D_004528B4[func_00100400() & 0xFF];

    *slot = frFontLinkGlyph(*slot, glyph, 0);
    return 0;
}

s32 func_0019C608(void) {
    return D_00452864.unk0;
}

s32 func_0019C618(void) {
    return D_00452864.unk4;
}

s32 func_0019C628(void) {
    return D_00452864.unk8;
}

u32 func_0019C638(void) {
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019C640);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C850);

FrFontGlyph *func_0019C980(FrFontGlyph *source, FrFontGlyph *destination) {
    FrFontGlyph *glyph = func_0019C850(source, 0);

    if (glyph == NULL) {
        return destination;
    }
    return frFontLinkGlyphAfterPrevious(destination, glyph);
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019C9D0);

void *frFontCloneEntryResource(u8 index, s32 option) {
    FrFontEntry *entry = &D_00452720.entries[index];
    void *dst = func_00328D68(0x120);
    FrFontSegments segments;

    itfSplitRelativeSegments(entry->first, &segments);
    func_001A00B8(dst, option, entry->first, &segments);
    return dst;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019CB30);

/* Initialize a glyph record while retaining only the high bits of its flags. */
void frFontSetupGlyph(FrFontGlyph *glyph, s16 glyphId, s8 byte1, s8 byte0, s32 flags, s8 byte2) {
    glyph->u14.b[1] = byte1;
    glyph->u14.b[0] = byte0;
    glyph->u14.b[2] = byte2;
    glyph->u0.h = glyphId;
    glyph->u10.word = flags & ~0xFF;
    glyph->u14.b[3] = D_00436564;
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

INCLUDE_ASM(const s32, "interface/frFont", func_0019CCC0);

FrFontCtx *func_0019CE10(void *glyphData, s8 arg1, s8 arg2, s8 arg3, s32 previousGlyph) {
    FrFontGlyph *glyph = func_0019CE78(glyphData, arg1, arg2, arg3, 0);

    if (glyph == NULL) {
        return (FrFontCtx *)previousGlyph;
    }
    return (FrFontCtx *)frFontLinkGlyphAfterPrevious((FrFontGlyph *)previousGlyph, glyph);
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019CE78);

void func_0019D010(FrFontCtx *ctx, s32 value) {
    s32 doubled = (value & 0xFF) * 2;

    if (doubled >= 0x81) {
        ctx->u0.bytes.unk0 = -0x80;
    } else {
        ctx->u0.bytes.unk0 = doubled;
    }
}

void frFontEnableContextMode(FrFontCtx *ctx) {
    ctx->mode40 = 1;
    func_0019D010(ctx, 0x80);
}

void frFontSetFlagAndMeasureGlyphs(FrFontCtx *ctx, u8 flag) {
    u32 measured;

    ctx->u0.bytes.flag1 = flag;
    measured = frFontMeasureGlyphChain(ctx);
    ctx->uC.w = measured;
}

void func_0019D088(FrFontGlyph *glyph, s32 advance, s32 height) {
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

void func_0019D100(FrFontCtx *ctx, u32 first, u32 second) {
    ctx->unk4 = first;
    ctx->unk8 = second;
}

void func_0019D110(FrFontCtx *ctx, u32 value) {
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

void func_0019D1D0(s32 flags) {
    flags |= D_00436564;
    D_00436564 = flags;
}

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frFontClearFlagBits(u8 mask) {
    u8 old = D_00436564;

    D_00436564 = old & ~mask;
    return old;
}

void func_0019D1F8(u32 flags) {
    D_00436568 = flags;
}

s32 func_0019D200(FrFontGlyph *glyph) {
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

INCLUDE_ASM(const s32, "interface/frFont", func_0019D288);

void func_0019D518(FrFontGlyph *glyph) {
    func_0019D530(glyph, 0);
}

void func_0019D530(FrFontGlyph *glyph, s8 mode) {
    func_0019D550(glyph, mode, D_00436568);
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D550);

/* Advance one of two cached glyph slots, chosen by the current font index. */
/* Keep byte-base arithmetic: indexing FrFontSys.slots changes ee-gcc codegen. */
s32 frFontAdvanceSelectedGlyphSlot(void) {
    s32 selection = (func_00100400() & 0xFF) == 0;
    u8 *base = (u8 *)&D_00452720;
    FrFontGlyph **slot = (FrFontGlyph **)(base + selection * 4 + 0x194);

    *slot = func_0019C4D0(*slot);
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

void func_0019D8A8(u32 fontData) {
    func_0019C130(8, fontData, 0);
}

void func_0019D8C8(void) {
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
            s8 b1 = line->u0.b.b1;

            do {
                total += node->advance;
                node = node->next;
                total += b1;
            } while (node != NULL);
        }
    }
    return total;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D9A8);

u32 func_0019DA98(u8 fontIndex) {
    s32 index = fontIndex;

    if (index < 2) {
        if (index >= 0) {
            return D_00436558;
        }
    }
    return D_00452724[index].val->unk10;
}

u32 func_0019DAD8(u8 fontIndex) {
    s32 index = fontIndex;

    if (index < 2) {
        if (index >= 0) {
            return D_00436558;
        }
    }
    return D_00452724[index].val->unk12;
}

void func_0019DB18(void) {
    D_00436554 = 0x19;
}

void func_0019DB28(u32 value) {
    D_00436554 = value;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019DB30);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DBA8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DC68);

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

INCLUDE_ASM(const s32, "interface/frFont", func_0019DD48);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DE70);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DEE0);

/* Old-style definition: callers invoke it without arguments and rely on $a0. */
void frFontCreateContext(ctx)
    FrFontCtx *ctx;

{
    FrFontCtx *newCtx = func_0019CE10(&D_00436578, 0, ctx->uC.b.bD, ctx->uC.b.bE, ctx->u14.shifted);

    ctx->u14.ptr = newCtx;
    func_0019D010(newCtx, ctx->uC.b.bF);
    ctx->flag1C = 0;
}

void func_0019E0A0(FrFontCtx *ctx) {
    s8 flag;

    if (ctx->u14.ptr == NULL) {
        flag = ctx->flag1C;
    }
    else {
        if (((FrFontGlyph *)ctx->u14.ptr)->firstChild == NULL) {
            ctx->flag1C = 0;
        }
        flag = ctx->flag1C;
    }
    if (flag == '\0') {
        flag = ctx->flag1D;
    }
    else {
        frFontCreateContext();
        flag = ctx->flag1D;
    }
    if (flag != '\0') {
        func_0019D100(ctx->u14.ptr, ctx->u0.word, ctx->unk4);
        ctx->flag1D = 0;
    }
}

void func_0019E110(FrFontCtx *ctx) {
    ctx->unk4 += D_00436554 * 8;
    ctx->flag1C = 1;
    ctx->flag1D = 1;
}

INCLUDE_SDATA(const s32, "interface/frFont", D_00436550);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436554);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436558);

INCLUDE_SDATA(const s32, "interface/frFont", D_0043655C);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436560);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436564);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436568);

INCLUDE_SDATA(const s32, "interface/frFont", D_0043656C);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436570);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436578);

INCLUDE_SDATA(const s32, "interface/frFont", D_00436580);

