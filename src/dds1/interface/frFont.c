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
    u32 unkC;                /* 0xC: refreshed by func_001953A8 */
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

extern u32 D_003BB164;

extern u32 D_003BB178;

extern u32 func_00195C50(void);
extern void func_00195450(void *, u32, u32);
extern void func_00196390(void);

INCLUDE_ASM(const s32, "interface/frFont", func_00194618);

INCLUDE_ASM(const s32, "interface/frFont", func_00194668);

INCLUDE_ASM(const s32, "interface/frFont", func_001946C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194788);

INCLUDE_ASM(const s32, "interface/frFont", func_00194800);

INCLUDE_ASM(const s32, "interface/frFont", func_00194840);

INCLUDE_ASM(const s32, "interface/frFont", func_00194920);

INCLUDE_ASM(const s32, "interface/frFont", func_00194978);

INCLUDE_ASM(const s32, "interface/frFont", func_00194988);

INCLUDE_ASM(const s32, "interface/frFont", func_00194998);

u32 func_001949A8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_001949B0);

INCLUDE_ASM(const s32, "interface/frFont", func_00194BA0);

INCLUDE_ASM(const s32, "interface/frFont", func_00194CD0);

INCLUDE_ASM(const s32, "interface/frFont", func_00194D20);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E00);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E80);

INCLUDE_ASM(const s32, "interface/frFont", func_00194F78);

INCLUDE_ASM(const s32, "interface/frFont", func_00194FC0);

INCLUDE_ASM(const s32, "interface/frFont", func_00195010);

INCLUDE_ASM(const s32, "interface/frFont", func_00195160);

INCLUDE_ASM(const s32, "interface/frFont", func_001951C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195360);

void func_00195388(FrFontCtx *ctx) {
    ctx->mode40 = 1;
    func_00195360(ctx, 0x80);
}

void func_001953A8(FrFontCtx *ctx, u8 flag) {
    ctx->u0.bytes.flag1 = flag;
    ctx->unkC = func_00195C50();
}

INCLUDE_ASM(const s32, "interface/frFont", func_001953D8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195450);

void func_00195460(FrFontCtx *ctx, u32 value) {
    ctx->u14.shifted = value >> 4;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195470);

INCLUDE_ASM(const s32, "interface/frFont", func_001954C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195520);

INCLUDE_ASM(const s32, "interface/frFont", func_00195530);

void func_00195548(u32 arg0) {
    D_003BB178 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195550);

INCLUDE_ASM(const s32, "interface/frFont", func_001955D8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195868);

INCLUDE_ASM(const s32, "interface/frFont", func_00195880);

INCLUDE_ASM(const s32, "interface/frFont", func_001958A0);

INCLUDE_ASM(const s32, "interface/frFont", func_00195B10);

INCLUDE_ASM(const s32, "interface/frFont", func_00195B60);

INCLUDE_ASM(const s32, "interface/frFont", func_00195B78);

void func_00195BD8(u32 arg0) {
    func_001944A0(8, arg0, 0);
}

void func_00195BF8(void) {
    func_001945A8(8);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195C10);

INCLUDE_ASM(const s32, "interface/frFont", func_00195C50);

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

INCLUDE_ASM(const s32, "interface/frFont", func_00196390);

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

INCLUDE_ASM(const s32, "interface/frFont", func_00196450);
