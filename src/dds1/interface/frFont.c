#include "common.h"

/* Record shared by the matched helpers below; offsets are from retail.
 * frFontEnableContextMode receives the message-window node itself (itfMesManager
 * func_0019DB40 passes its chain node straight in). */
typedef struct FrFontCtx {
    union {
        u32 word;            /* 0x0: whole word read by func_001963E0 */
        struct {
            u8 unk0;         /* 0x0 */
            u8 flag1;        /* 0x1: set by frFontSetFlagAndMeasureGlyphs */
            u8 unk2[2];      /* 0x2 */
        } bytes;
    } u0;
    u32 unk4;                /* 0x4 */
    u32 unk8;                /* 0x8 */
    union {
        u32 w;                   /* 0xC: word view */
        struct { u8 pad; s8 bD; s8 bE; u8 bF; } b; /* 0xC: byte views */
    } uC;                        /* 0xC: refreshed by frFontSetFlagAndMeasureGlyphs */
    u32 unk10;               /* 0x10 */
    union {
        u32 shifted;         /* 0x14: value stored shifted by func_00195460 */
        void *ptr;           /* 0x14: child pointer read by func_001963E0 */
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

/* Word at +0x194/+0x198 selected by frFontAdvanceSelectedGlyphSlot. */
typedef struct FrFontSys {
    u8 unk0[0x194];           /* 0x0 */
    FrFontGlyph *slots[2];    /* 0x194 */
} FrFontSys;

extern u32 D_003BB164;

extern u8 D_003BB174;

extern u32 D_003BB178;

extern u8 D_003BB180[];

extern FrFontSave D_003D6DC4;

extern FrFontSys D_003D6C80;

extern FrFontGlyph *D_003D6E14[];

extern u32 frFontMeasureGlyphChain(void *arg0);

extern void func_00195450(FrFontCtx *ctx, u32 arg1, u32 arg2);

void frFontCreateContext();

extern void func_00195360(FrFontCtx *ctx, s32 arg1);

extern s32 func_00100518(void);

extern s32 func_00195550(FrFontGlyph *arg0);

extern FrFontGlyph *func_00194840(FrFontGlyph *arg0);

extern FrFontGlyph *func_00194BA0(FrFontGlyph *arg0, s32 arg1);

extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *arg0, FrFontGlyph *arg1, s32 arg2);

extern s32 func_001958A0(FrFontGlyph *arg0, s8 arg1, u32 arg2);

extern FrFontCtx *func_00195160(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_00195880(FrFontGlyph *arg0, s8 arg1);

FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *arg0, FrFontGlyph *arg1);

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

INCLUDE_ASM(const s32, "interface/frFont", func_00194618);

INCLUDE_ASM(const s32, "interface/frFont", func_00194668);

INCLUDE_ASM(const s32, "interface/frFont", func_001946C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194788);

FrFontGlyph *func_00194800(FrFontGlyph *glyph) {
    if (func_00195550(glyph) != 0) {
        return glyph;
    }
    return func_00194840(glyph);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00194840);

s32 func_00194920(FrFontGlyph *glyph) {
    FrFontGlyph **slot = &D_003D6E14[func_00100518() & 0xFF];

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

FrFontGlyph *func_00194CD0(FrFontGlyph *source, FrFontGlyph *destination) {
    FrFontGlyph *glyph = func_00194BA0(source, 0);

    if (glyph == NULL) {
        return destination;
    }
    return frFontLinkGlyphAfterPrevious(destination, glyph);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00194D20);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E00);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E80);

void frFontSetupGlyph(FrFontGlyph *arg0, s16 arg1, s8 arg2, s8 arg3, s32 arg4, s8 arg5) {
    arg0->u14.b[1] = arg2;
    arg0->u14.b[0] = arg3;
    arg0->u14.b[2] = arg5;
    arg0->u0.h = arg1;
    arg0->unk10 = arg4 & ~0xFF;
    arg0->u14.b[3] = D_003BB174;
    arg0->x = 0;
    arg0->y = 0;
    arg0->advance = 0;
    arg0->unk2 = 0;
    arg0->firstChild = NULL;
    arg0->unk20 = NULL;
    arg0->previous = NULL;
    arg0->next = NULL;
}

void frFontInitGlyph(FrFontGlyph *arg0) {
    arg0->u0.b.b0 = -0x80;
    arg0->x = 0;
    arg0->y = 0;
    arg0->u0.b.b1 = 0;
    arg0->advance = 0;
    arg0->u14.w = 0;
    arg0->previous = NULL;
    arg0->next = NULL;
    arg0->chainHead = arg0;
    arg0->firstChild = NULL;
    arg0->unk20 = NULL;
    arg0->unk18.w = 0;
    arg0->unk30 = 0;
    arg0->unk34 = 0;
    arg0->unk38 = 0;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195010);

INCLUDE_ASM(const s32, "interface/frFont", func_00195160);

INCLUDE_ASM(const s32, "interface/frFont", func_001951C8);

void func_00195360(FrFontCtx *ctx, s32 arg1) {
    s32 val = (arg1 & 0xFF) * 2;

    if (val >= 0x81) {
        ctx->u0.bytes.unk0 = -0x80;
    } else {
        ctx->u0.bytes.unk0 = val;
    }
}

void frFontEnableContextMode(FrFontCtx *ctx) {
    ctx->mode40 = 1;
    func_00195360(ctx, 0x80);
}

void frFontSetFlagAndMeasureGlyphs(FrFontCtx *ctx, u8 flag) {
    ctx->u0.bytes.flag1 = flag;
    ctx->uC.w = frFontMeasureGlyphChain(ctx);
}

INCLUDE_ASM(const s32, "interface/frFont", func_001953D8);

void func_00195450(FrFontCtx *ctx, u32 arg1, u32 arg2) {
    ctx->unk4 = arg1;
    ctx->unk8 = arg2;
}

void func_00195460(FrFontCtx *ctx, u32 value) {
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

void func_00195520(s32 arg0) {
    arg0 |= D_003BB174;
    D_003BB174 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195530);

void func_00195548(u32 arg0) {
    D_003BB178 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195550);

INCLUDE_ASM(const s32, "interface/frFont", func_001955D8);

void func_00195868(FrFontGlyph *arg0) {
    func_00195880(arg0, 0);
}

void func_00195880(FrFontGlyph *arg0, s8 arg1) {
    func_001958A0(arg0, arg1, D_003BB178);
}

INCLUDE_ASM(const s32, "interface/frFont", func_001958A0);

s32 frFontAdvanceSelectedGlyphSlot(void) {
    s32 selection = (func_00100518() & 0xFF) == 0;
    u8 *base = (u8 *)&D_003D6C80;
    FrFontGlyph **slot = (FrFontGlyph **)(base + selection * 4 + 0x194);

    *slot = func_00194840(*slot);
    return 0;
}

FrFontGlyph *frFontLinkGlyphAfterPrevious(FrFontGlyph *arg0, FrFontGlyph *arg1) {
    return frFontLinkGlyph(arg0, arg1, 1);
}

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

void func_00195BD8(u32 arg0) {
    func_001944A0(8, arg0, 0);
}

void func_00195BF8(void) {
    frFontFreeEntry(8);
}

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

u32 frFontMeasureGlyphChain(void *arg0) {
    FrFontGlyph *glyph = arg0;
    FrFontGlyph *node = glyph->firstChild;
    s32 total = 0;

    if (node != NULL) {
        s8 b1 = glyph->u0.b.b1;

        do {
            total += node->advance;
            node = node->next;
            total += b1;
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

INCLUDE_ASM(const s32, "interface/frFont", func_00195CD8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195DC8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195E08);

void func_00195E48(void) {
    D_003BB164 = 0x15;
}

void func_00195E58(u32 arg0) {
    D_003BB164 = arg0;
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
    FrFontCtx *newCtx = func_00195160(&D_003BB180, 0, ctx->uC.b.bD, ctx->uC.b.bE, ctx->u14.shifted);

    ctx->u14.ptr = newCtx;
    func_00195360(newCtx, ctx->uC.b.bF);
    ctx->flag1C = 0;
}

void func_001963E0(FrFontCtx *ctx) {
    s8 pending;

    if (ctx->u14.ptr == NULL) {
        pending = ctx->flag1C;
    } else {
        if (*(s32 *)((u8 *)ctx->u14.ptr + 0x1c) == 0) {
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
        func_00195450(ctx->u14.ptr, ctx->u0.word, ctx->unk4);
        ctx->flag1D = 0;
    }
}

void func_00196450(FrFontCtx *ctx) {
    ctx->unk4 += D_003BB164 * 8;
    ctx->flag1C = 1;
    ctx->flag1D = 1;
}

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB160);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB164);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB168);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB16C);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB170);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB174);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB178);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB17C);

INCLUDE_SDATA(const s32, "interface/frFont", D_003BB180);

