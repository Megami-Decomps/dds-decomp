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

extern void (*D_003B4448[])(PanelObj *);

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

extern PanelEntry D_0045296C[];

void func_001A1B08(PanelObj *arg0) {
    D_003B4448[arg0->unk3C](arg0);
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1B38);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1BB0);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1D20);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1E68);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1E98);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1F08);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1F80);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1FB0);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2010);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2070);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2100);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2118);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2148);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2180);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2230);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2340);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2450);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2598);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A25D0);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2658);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A29D8);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2AF0);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2C98);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2D10);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2D80);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2E20);

s8 func_001A2E68(s32 arg0) {
    return D_0045296C[arg0].ptr->sub24.unk10;
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2E88);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2EA8);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2F08);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2F60);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3008);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3138);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A31E0);

s16 func_001A3310(s32 arg0) {
    return D_0045296C[arg0].ptr->unk50;
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3330);

s16 func_001A3350(s32 arg0) {
    return D_0045296C[arg0].ptr->unk52;
}
INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365A8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D8);


INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365E0);

