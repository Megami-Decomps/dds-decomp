#include "common.h"

extern u32 func_0019D920(void);

extern u32 D_00436568;

extern u32 D_00436554;

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

extern FrFontGlyph *D_004528B4[];

extern s32 func_00100400(void);

extern FrFontGlyph *frFontLinkGlyph(FrFontGlyph *arg0, FrFontGlyph *arg1, s32 arg2);

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

extern void func_0019D010(FrFontCtx *ctx, s32 arg1);

extern void func_0019D100(FrFontCtx *ctx, u32 arg1, u32 arg2);

/* Word at +0x194/+0x198 selected by func_00195B10. */
typedef struct FrFontSys {
    u8 unk0[0x194];           /* 0x0 */
    FrFontGlyph *slots[2];    /* 0x194 */
} FrFontSys;

extern FrFontSys D_00452720;

extern FrFontGlyph *func_0019C4D0(FrFontGlyph *arg0);

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

INCLUDE_ASM(const s32, "interface/frFont", func_0019C2A8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C2F8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C358);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C418);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C490);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C4D0);

s32 func_0019C5B0(FrFontGlyph *arg0) {
    FrFontGlyph **slot = &D_004528B4[func_00100400() & 0xFF];

    *slot = frFontLinkGlyph(*slot, arg0, 0);
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

INCLUDE_ASM(const s32, "interface/frFont", func_0019C980);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C9D0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CAB0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CB30);

void frFontSetupGlyph(FrFontGlyph *arg0, s16 arg1, s8 arg2, s8 arg3, s32 arg4, s8 arg5) {
    arg0->u14.b[1] = arg2;
    arg0->u14.b[0] = arg3;
    arg0->u14.b[2] = arg5;
    arg0->u0.h = arg1;
    arg0->unk10 = arg4 & ~0xFF;
    arg0->u14.b[3] = D_00436564;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk2 = 0;
    arg0->unk1C = NULL;
    arg0->unk20 = NULL;
    arg0->unk24 = NULL;
    arg0->unk28 = NULL;
}

void frFontInitGlyph(FrFontGlyph *arg0) {
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

INCLUDE_ASM(const s32, "interface/frFont", func_0019CCC0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CE10);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CE78);

void func_0019D010(FrFontCtx *ctx, s32 arg1) {
    s32 val = (arg1 & 0xFF) * 2;

    if (val >= 0x81) {
        ctx->u0.bytes.unk0 = -0x80;
    } else {
        ctx->u0.bytes.unk0 = val;
    }
}

void func_0019D038(s32 arg0) {
    *(u32 *)(arg0 + 0x40) = 1;
    func_0019D010(arg0, 0x80);
}

void func_0019D058(s32 arg0, u8 arg1) {
    u32 temp_v0;

    *(u8 *)(arg0 + 1) = arg1;
    temp_v0 = func_0019D920();
    *(u32 *)(arg0 + 0xc) = temp_v0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D088);

void func_0019D100(FrFontCtx *ctx, u32 arg1, u32 arg2) {
    ctx->unk4 = arg1;
    ctx->unk8 = arg2;
}

void func_0019D110(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1 >> 4;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D120);

void func_0019D178(TextStyleNode *entry, u32 color) {
    for (; entry != NULL; entry = entry->next) {
        TextStyleNode *child;
        for (child = entry->firstChild; child != NULL; child = child->nextChild) {
            child->color = color;
        }
    }
}

void func_0019D1D0(s32 arg0) {
    arg0 |= D_00436564;
    D_00436564 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D1E0);

void func_0019D1F8(u32 arg0) {
    D_00436568 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D200);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D288);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D518);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D530);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D550);

s32 func_0019D7E0(void) {
    s32 sel = (func_00100400() & 0xFF) == 0;
    u8 *base = (u8 *)&D_00452720;
    FrFontGlyph **slot = (FrFontGlyph **)(base + sel * 4 + 0x194);

    *slot = func_0019C4D0(*slot);
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D830);

FrFontGlyph *frFontLinkGlyph(FrFontGlyph *arg0, FrFontGlyph *arg1, s32 arg2) {
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

void func_0019D8A8(u32 arg0) {
    func_0019C130(8, arg0, 0);
}

void func_0019D8C8(void) {
    frFontFreeEntry(8);
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D8E0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D920);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D958);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D9A8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DA98);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DAD8);

void func_0019DB18(void) {
    D_00436554 = 0x19;
}

void func_0019DB28(u32 arg0) {
    D_00436554 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019DB30);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DBA8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DC68);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DCF8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DD48);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DE70);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DEE0);

INCLUDE_ASM(const s32, "interface/frFont", frFontCreateContext);

void func_0019E0A0(u32 *arg0) {
    s8 temp_v0;

    if (arg0[5] == 0) {
        temp_v0 = *(s8 *)(arg0 + 7);
    }
    else {
        if (*(s32 *)(arg0[5] + 0x1c) == 0) {
            *(u8 *)(arg0 + 7) = 0;
        }
        temp_v0 = *(s8 *)(arg0 + 7);
    }
    if (temp_v0 == '\0') {
        temp_v0 = *(s8 *)((s32)arg0 + 0x1d);
    }
    else {
        frFontCreateContext();
        temp_v0 = *(s8 *)((s32)arg0 + 0x1d);
    }
    if (temp_v0 != '\0') {
        func_0019D100(arg0[5], *arg0, arg0[1]);
        *(u8 *)((s32)arg0 + 0x1d) = 0;
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

