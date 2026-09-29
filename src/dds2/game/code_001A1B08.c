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
    u8 handlerIndex; /* 0x3C: index into the panel handler table */
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

typedef struct PanelHold {
    s32 window;
    s16 unk4;
    s16 unk6;
    s32 unk8;
} PanelHold;

extern PanelHold D_003B4780;

extern PanelEntry D_0045296C[];

extern void func_001A19C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void itfPanelDispatchHandler(PanelObj *panel) {
    D_003B4448[panel->handlerIndex](panel);
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1B38);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1BB0);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1D20);

typedef struct PanelVert {
    s32 x;
    s32 y;
} PanelVert;

void func_001A1E68(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    PanelVert *p;
    v->x = x0;
    v->y = y0;
    p = &v[1];
    p->y = y0;
    p->x = x1;
    p = &v[2];
    p->x = x1;
    p->y = y1;
    p = &v[3];
    p->x = x0;
    p->y = y1;
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1E98);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1F08);

void func_001A1F80(PanelVert *v, s32 a, s32 b, s32 c, s32 d) {
    PanelVert *p = &v[8];
    p->x = a;
    p->y = b;
    p[1].x = c;
    p[1].y = 0;
    p = &v[10];
    p->x = a;
    p->y = b;
    p[1].x = c;
    p[1].y = d;
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1FB0);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2010);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2070);

void func_001A2100(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    PanelVert *p = &v[4];
    p->x = x0;
    p->y = y0;
    p[1].x = x1;
    p[1].y = y1;
}

void func_001A2118(PanelVert *v, s32 a, s32 b, s32 c, s32 d) {
    PanelVert *p = &v[6];
    p->x = a;
    p->y = b;
    p[1].x = c;
    p[1].y = 0;
    p = &v[8];
    p->x = a;
    p->y = b;
    p[1].x = c;
    p[1].y = d;
}

void func_001A2148(s32 *rect) {
    s32 *c = rect + 0x11;
    c[0] = 0x73;
    c[1] = 0x87;
    c[2] = 0xFF;
    c[3] = 0;
    c = rect + 0x15;
    c[0] = 0x73;
    c[1] = 0x87;
    c[2] = 0xFF;
    c[3] = 0;
}

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

void itfPanelEmitRecord(s32 index, s32 value) {
    func_001A19C8(D_0045296C[index].ptr->unkA8, 0, value, 0, 0, 0);
}

s8 itfPanelGetStatus(s32 index) {
    return D_0045296C[index].ptr->sub24.unk10;
}

void itfPanelSetStatus(s32 index, s8 value) {
    D_0045296C[index].ptr->sub24.unk10 = value;
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2EA8);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2F08);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2F60);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3008);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3138);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A31E0);

s16 itfPanelGetPairFirst(s32 index) {
    return D_0045296C[index].ptr->unk50;
}

void itfPanelSetPairFirst(s32 index, s16 value) {
    D_0045296C[index].ptr->unk50 = value;
}

s16 itfPanelGetPairSecond(s32 index) {
    return D_0045296C[index].ptr->unk52;
}

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365A8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365E0);

