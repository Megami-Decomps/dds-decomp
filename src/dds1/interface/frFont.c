#include "common.h"

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

/* Triple word block with one getter per word. */
typedef struct FrFontSave {
    u32 unk0; /* 0x0: read by func_00194978 */
    u32 unk4; /* 0x4: read by func_00194988 */
    u32 unk8; /* 0x8: read by func_00194998 */
} FrFontSave;

/* Glyph/record chain walked by func_001958A0/func_00195B78. */
typedef struct FrFontGlyph {
    union {
        s16 h;                        /* 0x0: halfword view */
        struct { s8 b0; s8 b1; } b;   /* 0x0: byte views */
    } u0;
    s16 unk2;         /* 0x2 */
    s32 unk4;         /* 0x4 */
    s32 unk8;         /* 0x8 */
    s32 unkC;         /* 0xC */
    u32 unk10;        /* 0x10 */
    union {
        u32 w;        /* 0x14: word view */
        u8 b[4];      /* 0x14: byte views */
    } u14;
    union {
        u32 w;            /* 0x18: word view */
        u8 b[4];          /* 0x18: byte views */
    } unk18;
    struct FrFontGlyph *unk1C; /* 0x1C */
    struct FrFontGlyph *unk20; /* 0x20 */
    struct FrFontGlyph *unk24; /* 0x24 */
    struct FrFontGlyph *unk28; /* 0x28 */
    struct FrFontGlyph *unk2C; /* 0x2C */
    u32 unk30;        /* 0x30 */
    u32 unk34;        /* 0x34 */
    u32 unk38;        /* 0x38 */
    s32 unk3C;        /* 0x3C */
    s32 unk40;        /* 0x40 */
} FrFontGlyph;

/* Word at +0x194/+0x198 selected by func_00195B10. */
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
extern u32 func_00195C50(void *arg0);
extern void func_00195450(FrFontCtx *ctx, u32 arg1, u32 arg2);
void func_00196390();
extern void func_00195360(FrFontCtx *ctx, s32 arg1);
extern s32 func_00100518(void);
extern s32 func_00195550(FrFontGlyph *arg0);
extern FrFontGlyph *func_00194840(FrFontGlyph *arg0);
extern FrFontGlyph *func_00194BA0(FrFontGlyph *arg0, s32 arg1);
extern FrFontGlyph *func_00195B78(FrFontGlyph *arg0, FrFontGlyph *arg1, s32 arg2);
extern s32 func_001958A0(FrFontGlyph *arg0, s8 arg1, u32 arg2);
extern FrFontCtx *func_00195160(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_00195880(FrFontGlyph *arg0, s8 arg1);
FrFontGlyph *func_00195B60(FrFontGlyph *arg0, FrFontGlyph *arg1);

INCLUDE_ASM(const s32, "interface/frFont", func_00194618);

INCLUDE_ASM(const s32, "interface/frFont", func_00194668);

INCLUDE_ASM(const s32, "interface/frFont", func_001946C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194788);

FrFontGlyph *func_00194800(FrFontGlyph *arg0) {
    if (func_00195550(arg0) != 0) {
        return arg0;
    }
    return func_00194840(arg0);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00194840);

s32 func_00194920(FrFontGlyph *arg0) {
    FrFontGlyph **slot = &D_003D6E14[func_00100518() & 0xFF];

    *slot = func_00195B78(*slot, arg0, 0);
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

FrFontGlyph *func_00194CD0(FrFontGlyph *arg0, FrFontGlyph *arg1) {
    FrFontGlyph *res = func_00194BA0(arg0, 0);

    if (res == NULL) {
        return arg1;
    }
    return func_00195B60(arg1, res);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00194D20);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E00);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E80);

void func_00194F78(FrFontGlyph *arg0, s16 arg1, s8 arg2, s8 arg3, s32 arg4, s8 arg5) {
    arg0->u14.b[1] = arg2;
    arg0->u14.b[0] = arg3;
    arg0->u14.b[2] = arg5;
    arg0->u0.h = arg1;
    arg0->unk10 = arg4 & ~0xFF;
    arg0->u14.b[3] = D_003BB174;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk2 = 0;
    arg0->unk1C = NULL;
    arg0->unk20 = NULL;
    arg0->unk24 = NULL;
    arg0->unk28 = NULL;
}

void func_00194FC0(FrFontGlyph *arg0) {
    arg0->u0.b.b0 = -0x80;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->u0.b.b1 = 0;
    arg0->unkC = 0;
    arg0->u14.w = 0;
    arg0->unk24 = NULL;
    arg0->unk28 = NULL;
    arg0->unk2C = arg0;
    arg0->unk1C = NULL;
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

void func_00195388(FrFontCtx *ctx) {
    ctx->mode40 = 1;
    func_00195360(ctx, 0x80);
}

void func_001953A8(FrFontCtx *ctx, u8 flag) {
    ctx->u0.bytes.flag1 = flag;
    ctx->uC.w = func_00195C50(ctx);
}

INCLUDE_ASM(const s32, "interface/frFont", func_001953D8);

void func_00195450(FrFontCtx *ctx, u32 arg1, u32 arg2) {
    ctx->unk4 = arg1;
    ctx->unk8 = arg2;
}

void func_00195460(FrFontCtx *ctx, u32 value) {
    ctx->u14.shifted = value >> 4;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195470);

INCLUDE_ASM(const s32, "interface/frFont", func_001954C8);

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

s32 func_00195B10(void) {
    s32 sel = (func_00100518() & 0xFF) == 0;
    u8 *base = (u8 *)&D_003D6C80;
    FrFontGlyph **slot = (FrFontGlyph **)(base + sel * 4 + 0x194);

    *slot = func_00194840(*slot);
    return 0;
}

FrFontGlyph *func_00195B60(FrFontGlyph *arg0, FrFontGlyph *arg1) {
    return func_00195B78(arg0, arg1, 1);
}

FrFontGlyph *func_00195B78(FrFontGlyph *arg0, FrFontGlyph *arg1, s32 arg2) {
    if (arg0 == NULL) {
        return arg1;
    }
    if (arg1 == NULL) {
        return arg0;
    }
    arg0->unk28 = arg1->unk2C;
    arg1->unk2C->unk24 = arg0;
    arg1->unk2C = arg0->unk2C;
    if (arg2 == 1) {
        arg1->unk4 = arg0->unk4 + (arg0->unkC << 4);
        arg1->unk8 = arg0->unk8;
    }
    return arg1;
}

void func_00195BD8(u32 arg0) {
    func_001944A0(8, arg0, 0);
}

void func_00195BF8(void) {
    func_001945A8(8);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195C10);

u32 func_00195C50(void *arg0) {
    FrFontGlyph *glyph = arg0;
    FrFontGlyph *node = glyph->unk1C;
    s32 total = 0;

    if (node != NULL) {
        s8 b1 = glyph->u0.b.b1;

        do {
            total += node->unkC;
            node = node->unk28;
            total += b1;
        } while (node != NULL);
    }
    return total;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195C88);

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

INCLUDE_ASM(const s32, "interface/frFont", func_00196038);

INCLUDE_ASM(const s32, "interface/frFont", func_00196088);

INCLUDE_ASM(const s32, "interface/frFont", func_001961B0);

INCLUDE_ASM(const s32, "interface/frFont", func_00196220);

/* Old-style definition: callers invoke it without arguments and rely on $a0. */
void func_00196390(ctx)
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
        func_00196390();
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

