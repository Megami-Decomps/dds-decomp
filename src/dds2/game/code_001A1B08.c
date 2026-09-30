#include "common.h"

/* 0x10-byte packet record in the work object buffer. */
typedef struct PktRec {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    s32 unk8; /* 0x8 */
    s32 unkC; /* 0xC */
} PktRec;

/* Rectangle corners as consumed by the border-drawing routine func_001A09C0. */
typedef struct PanelRect {
    s32 x0; /* 0x0 */
    s32 y0; /* 0x4 */
    s32 x1; /* 0x8 */
    s32 y1; /* 0xC */
} PanelRect;

/* Work object holding the packet buffer pointer. */
typedef struct PanelObj {
    u8 unk0[8];     /* 0x0 */
    PktRec *buf;    /* 0x8: packet buffer */
    s32 unkC;       /* 0xC */
    PanelRect rect; /* 0x10 */
    u8 unk20[0x18]; /* 0x20 */
    s32 unk38;      /* 0x38 */
    u8 handlerIndex; /* 0x3C: index into the panel handler table */
} PanelObj;

extern void (*D_003B4448[])(PanelObj *);

/* Flag byte reached as rec+0x24+0x10 (i.e. byte 0x34 of the record). */
typedef struct PanelRecSub {
    u8 unk0[0x10]; /* 0x0 */
    s8 status;     /* 0x10: status read and written by itfPanelGet/SetStatus */
} PanelRecSub;

/* Payload record referenced by the D_003D6ECC table. */
typedef struct PanelRec {
    u8 unk0[0x24];     /* 0x0 */
    PanelRecSub sub24; /* 0x24 */
    u8 unk35[0x1B];    /* 0x35 */
    s16 pairFirst;    /* 0x50 */
    s16 pairSecond;   /* 0x52 */
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
extern void itfMesCleanupWindow(s32 window, s32 arg1);
extern void func_001A4988(s32 window, s32 arg1, s32 arg2);
extern s32 scrGetWindow(void);
extern void itfScaleVectors(s32 *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const s32 *input, s32 count);
extern s32 D_003B44E8[];
extern s32 D_003B4548[];
extern s32 D_003B4588[];
extern s32 D_003B4598[];
extern void itfDrawQuadFlat4(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, u64 command);
extern u8 D_004365C8[8];
extern u8 D_004365D0[8];
extern void itfSendTablePacket(u64 command, s32 index, s32 flag);
extern void itfEmitQuadListWide(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, u64 command);
extern u8 D_004365D8[8];
extern u8 D_004365E0[8];
extern void func_001A09C0(PanelRect *rect, u32 *colors, u32 tail, s32 width, u64 command);
extern u32 D_003B4760[];
extern void *sdfAllocPacketAligned(s32);
extern u64 *func_0033A290(void *, s32);
extern void sdfAppendPacket(void *, void *);
extern s32 scrReadIntParameter(s32);
extern s32 itfMesStartEntry(s32 window, s32 arg1, s32 arg2);

typedef struct PanelVert {
    s32 x;
    s32 y;
} PanelVert;

static inline void panelSetVec4(u32 *vec, u32 red, u32 green, u32 blue, u32 alpha) {
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = alpha;
}

void itfPanelDispatchHandler(PanelObj *panel) {
    D_003B4448[panel->handlerIndex](panel);
}

void func_001A1B38(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[4];
    s32 i;

    xs[0] = x0;
    xs[1] = x0 + 0x200;
    xs[2] = x1 - 0x200;
    xs[3] = x1;
    if (xs[2] < xs[1]) {
        xs[1] = ((x1 - x0) >> 1) + x0;
        xs[2] = xs[1];
    }
    for (i = 0; i < 4; i++) {
        v->x = xs[i];
        v->y = y0;
        v++;
        v->x = xs[i];
        v->y = y1;
        v++;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1BB0);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A1D20);

void itfPanelSetRectVerts(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
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

void func_001A1E98(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[3];
    s32 ys[2];
    s32 i;
    s32 j;

    xs[0] = x0;
    xs[1] = x1 - 0x5A0;
    xs[2] = x1;
    ys[0] = y0;
    ys[1] = y1;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            v->x = xs[j];
            v->y = ys[i];
            v++;
        }
    }
}

void func_001A1F08(u8 *base, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[4];
    s32 ys[2];
    PanelVert *v = (PanelVert *)(base + 4);
    s32 i;

    xs[0] = 0x1A0;
    xs[1] = 0x480;
    xs[2] = 0x1B70;
    xs[3] = 0x1E50;
    ys[0] = y0 + 0x160;
    ys[1] = y1 - 0x140;
    for (i = 0; i < 4; i++) {
        v->x = xs[i];
        v->y = ys[0];
        v++;
        v->x = xs[i];
        v->y = ys[1];
        v++;
    }
}

/* Populate two panel vertex pairs at slots 8 and 10. */
void func_001A1F80(PanelVert *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
    PanelVert *pair = &vertices[8];
    pair->x = x0;
    pair->y = y0;
    pair[1].x = x1;
    pair[1].y = 0;
    pair = &vertices[10];
    pair->x = x0;
    pair->y = y0;
    pair[1].x = x1;
    pair[1].y = y1;
}

void func_001A1FB0(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x140);

    panelSetVec4(vec, red, green, blue, 1);
    vec = (u32 *)(base + 0x150);
    panelSetVec4(vec, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x160), red, green, blue, alpha, D_003B44E8, 6);
}

void func_001A2010(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x100);

    panelSetVec4(vec, red, green, blue, 1);
    vec = (u32 *)(base + 0x110);
    panelSetVec4(vec, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x120), red, green, blue, alpha, D_003B4548, 4);
}

void func_001A2070(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x20);

    panelSetVec4(vec, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x30), red, green, blue, alpha, D_003B4588, 1);
    itfScaleVectors((s32 *)(base + 0x40), red, green, blue, 0x80, D_003B4598, 2);
}

void itfPanelSetRectSpan(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    PanelVert *p = &v[4];
    p->x = x0;
    p->y = y0;
    p[1].x = x1;
    p[1].y = y1;
}

void itfPanelInitRects30(PanelVert *v, s32 a, s32 b, s32 c, s32 d) {
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

/* Set both panel colors to the same blue tint with zero alpha. */
void itfPanelSetBlueTint(s32 *rect) {
    s32 *color = rect + 0x11;
    color[0] = 0x73;
    color[1] = 0x87;
    color[2] = 0xFF;
    color[3] = 0;
    color = rect + 0x15;
    color[0] = 0x73;
    color[1] = 0x87;
    color[2] = 0xFF;
    color[3] = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2180);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2230);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2340);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2450);

void func_001A2598(PanelObj *panel, u64 command) {
    PktRec *colors = panel->buf + 2;

    colors->unkC = panel->unk38;
    itfDrawQuadFlat4(panel->buf, colors, D_004365C8, D_004365D0, panel->unkC, command);
}

void func_001A25D0(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 3;

    colors[1].unkC = panel->unk38;
    itfSendTablePacket(command, 2, 0);
    itfEmitQuadListWide(buf, colors, D_004365D8, D_004365E0, 6, panel->unkC, command);
    itfSendTablePacket(command, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2658);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A29D8);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2AF0);

void func_001A2C98(PanelObj *panel, u64 command) {
    PanelRect rect;

    D_003B4760[3] = panel->unk38 * 0x67 / 128;
    rect.x0 = panel->rect.x0;
    rect.y0 = panel->rect.y0;
    rect.x1 = panel->rect.x1;
    rect.y1 = panel->rect.y1;
    func_001A09C0(&rect, D_003B4760, panel->unkC, 0x240, command);
}

void func_001A2D10(void *list) {
    void *packet = sdfAllocPacketAligned(0x40);
    u64 *entry = func_0033A290(packet, 0x40);

    entry[4] = 0x5101B;
    entry[5] = 0x47;
    entry[6] = 0x44;
    entry[7] = 0x42;
    sdfAppendPacket(list, packet);
}

s32 itfPanelStartEntry(void) {
    s32 window = scrGetWindow();
    PanelRec *rec;
    PanelRecSub *sub;
    s32 value;
    s8 status;

    if (window < 0) {
        return 1;
    }
    rec = D_0045296C[window].ptr;
    value = scrReadIntParameter(0);
    sub = &rec->sub24;
    status = sub->status;
    if (status == 0) {
        if (itfMesStartEntry(window, value, 0) == 0) {
            return 1;
        }
    } else if (status < 0) {
        sub->status = 0;
        return 1;
    }
    return 0;
}

void itfPanelEmitRecord(s32 index, s32 value) {
    func_001A19C8(D_0045296C[index].ptr->unkA8, 0, value, 0, 0, 0);
}

s8 itfPanelGetStatus(s32 index) {
    return D_0045296C[index].ptr->sub24.status;
}

void itfPanelSetStatus(s32 index, s8 value) {
    D_0045296C[index].ptr->sub24.status = value;
}

s32 itfPanelAcquireHold(void) {
    PanelHold *hold;
    s32 window;

    window = scrGetWindow();
    if (window < 0) {
        return 1;
    }
    hold = &D_003B4780;
    hold->window = window;
    hold->unk4 = -1;
    hold->unk8 = -1;
    hold->unk6 = 0;
    func_001A4988(window, 3, 0);
    return 1;
}

s32 itfPanelReleaseHold(void) {
    PanelHold *hold = &D_003B4780;

    if (hold->window < 0) {
        return 1;
    }
    itfMesCleanupWindow(hold->window, 1);
    hold->window = -1;
    hold->unk4 = -1;
    hold->unk8 = -1;
    hold->unk6 = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2F60);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3008);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3138);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A31E0);

s16 itfPanelGetPairFirst(s32 index) {
    return D_0045296C[index].ptr->pairFirst;
}

void itfPanelSetPairFirst(s32 index, s16 value) {
    D_0045296C[index].ptr->pairFirst = value;
}

s16 itfPanelGetPairSecond(s32 index) {
    return D_0045296C[index].ptr->pairSecond;
}

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365A8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365E0);

