#include "common.h"

/* 0x10-byte packet record in the work object buffer. */
typedef struct PktRec {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    s32 unk8; /* 0x8 */
    s32 unkC; /* 0xC */
} PktRec;

/* Work object holding the packet buffer pointer. */
typedef struct PanelObj {
    u8 unk0[8];     /* 0x0 */
    PktRec *buf;    /* 0x8: packet buffer */
    s32 unkC;       /* 0xC */
    s32 unk10;      /* 0x10 */
    s32 unk14;      /* 0x14 */
    s32 unk18;      /* 0x18 */
    s32 unk1C;      /* 0x1C */
    u8 unk20[0x18]; /* 0x20 */
    s32 unk38;      /* 0x38 */
    u8 unk3C;       /* 0x3C: handler index for func_00199AD8 */
} PanelObj;

/* Flag byte reached as rec+0x24+0x10 (i.e. byte 0x34 of the record). */
typedef struct PanelRecSub {
    u8 unk0[0x10]; /* 0x0 */
    s8 unk10;      /* 0x10 */
} PanelRecSub;

/* Payload record referenced by the D_003D6ECC table. */
typedef struct PanelRec {
    u8 unk0[0x24];     /* 0x0 */
    PanelRecSub sub24; /* 0x24 */
    u8 unk35[0x1B];    /* 0x35 */
    s16 unk50;         /* 0x50 */
    s16 unk52;         /* 0x52 */
    u8 unk54[0x54];    /* 0x54 */
    s32 unkA8;         /* 0xA8 */
} PanelRec;

/* D_003D6ECC entry (0x14 bytes); payload pointer is the first word. */
typedef struct PanelEntry {
    PanelRec *ptr;  /* 0x0 */
    u8 unk4[0x10];  /* 0x4 */
} PanelEntry;

extern PanelEntry D_003D6ECC[];
extern void (*D_00357A50[])(PanelObj *);
extern void func_00199998(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 func_0010D690(void);

/* Selection state shared by the id helpers below. */
typedef struct PanelCursor {
    s32 unk0;  /* 0x0: id (>= 0 valid) */
    s16 unk4;  /* 0x4 */
    s16 unk6;  /* 0x6: counter */
    s32 unk8;  /* 0x8 */
    s32 unkC;  /* 0xC */
    s16 unk10; /* 0x10 */
    s16 unk12; /* 0x12 */
} PanelCursor;

extern PanelCursor D_00357D90;

void func_00199AD8(PanelObj *arg0) {
    D_00357A50[arg0->unk3C](arg0);
}

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199B08);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199B80);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199CF0);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199E38);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199E68);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199ED8);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199F50);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199F80);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199FE0);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A040);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A0D0);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A0E8);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A118);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A150);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A200);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A310);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A420);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A568);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A5A0);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A628);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A9A8);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AAC0);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AC68);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019ACE0);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AD50);

void func_0019ADF0(s32 arg0, s32 arg1) {
    func_00199998(D_003D6ECC[arg0].ptr->unkA8, 0, arg1, 0, 0, 0);
}

s8 func_0019AE38(s32 arg0) {
    return D_003D6ECC[arg0].ptr->sub24.unk10;
}

void func_0019AE58(s32 arg0, s8 arg1) {
    D_003D6ECC[arg0].ptr->sub24.unk10 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AE78);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AED8);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AF30);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AFD8);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019B108);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019B1B0);

s16 func_0019B2E0(s32 arg0) {
    return D_003D6ECC[arg0].ptr->unk50;
}

void func_0019B300(s32 arg0, s16 arg1) {
    D_003D6ECC[arg0].ptr->unk50 = arg1;
}

s16 func_0019B320(s32 arg0) {
    return D_003D6ECC[arg0].ptr->unk52;
}



INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1A8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1B0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1B8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1C0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1C8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1D0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1D8);


INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1E0);

