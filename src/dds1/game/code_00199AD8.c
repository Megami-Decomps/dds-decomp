#include "mnu.h"

/* 0x10-byte packet record in the work object buffer. */
typedef struct PktRec {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    s32 unk8; /* 0x8 */
    s32 unkC; /* 0xC */
} PktRec;

/* Rectangle corners as consumed by the border-drawing routine func_00198990. */
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
    s32 tail;    /* 0xC: tail arg of itfDrawQuadFlat4 */
    PanelRect rect; /* 0x10 */
    u8 unk20[0x18]; /* 0x20 */
    s32 alpha;   /* 0x38: copied to a colour record's alpha */
    u8 handlerIndex; /* 0x3C: index into the panel handler table */
} PanelObj;

/* Flag byte reached as rec+0x24+0x10 (i.e. byte 0x34 of the record). */
typedef struct PanelRecSub {
    u8 unk0[0x10]; /* 0x0 */
    s8 status;      /* 0x10 */
} PanelRecSub;

/* Payload record referenced by the itfWindowSlots table. */
typedef struct PanelRec {
    u8 unk0[0x24];     /* 0x0 */
    PanelRecSub sub24; /* 0x24 */
    u8 unk35[0x1B];    /* 0x35 */
    s16 pairFirst;    /* 0x50 */
    s16 pairSecond;   /* 0x52 */
    u8 unk54[0x54];    /* 0x54 */
    s32 unkA8;         /* 0xA8 */
} PanelRec;

/* itfWindowSlots entry (0x14 bytes); payload pointer is the first word. */
typedef struct PanelEntry {
    PanelRec *ptr;  /* 0x0 */
    u8 unk4[0x10];  /* 0x4 */
} PanelEntry;

extern PanelEntry itfWindowSlots[];
extern void (*itfPanelHandlers[])(PanelObj *);
extern void itfAdvancePanelLayoutAndNotify(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 scrGetWindow(void);
extern void itfMesCleanupWindow(s32 window, s32 arg1);
extern void func_0019C968(s32 window, s32 arg1, s32 arg2);
extern void itfScaleVectors(s32 *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const s32 *input, s32 count);
extern s32 itfPanelColorTemplates[];
extern s32 D_00357B50[];
extern void itfDrawQuadFlat4(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, u64 command);
extern u8 D_003BB1C8[8];
extern u8 D_003BB1D0[8];
extern void *sdfAllocPacketAligned(s32);
extern u64 *sdfConsFinalizePacketHeader(void *, s32);
extern void sdfAppendPacket(void *, void *);
extern void func_00198990(PanelRect *rect, u32 *colors, u32 tail, s32 width, u64 command);
extern u32 D_00357D68[];
extern void itfSendTablePacket(u64 command, s32 index, s32 flag);
extern void itfEmitQuadListWide(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, u64 command);
extern u8 D_003BB1D8[8];
extern u8 D_003BB1E0[8];
extern s32 itfPanelGradientColorTemplate[];
extern s32 itfPanelGradientColorPair[];
extern s32 scrReadIntParameter(s32);
extern s32 itfMesStartEntry(s32 window, s32 arg1, s32 arg2);
extern u8 D_00357BF0[], D_00357C08[], D_00357C20[];
extern u8 D_00357C38[], D_00357C50[], D_00357C68[];
extern u8 D_003BB1A8[8], D_003BB1B0[8];
extern void itfEmitQuadListA(void *, void *, u8 *, u8 *, s32, u32, u64);


typedef struct PanelHoldItem {
    s32 first;  /* 0x0 */
    s16 second; /* 0x4 */
    s16 third;  /* 0x6 */
} PanelHoldItem;

/* Selection state shared by the id helpers below. */
typedef struct PanelHold {
    s32 window;  /* 0x0: window id (>= 0 valid) */
    s16 unk4;  /* 0x4 */
    s16 count;   /* 0x6: entries used in items[] */
    s32 unk8;  /* 0x8 */
    PanelHoldItem items[0x40]; /* 0xC: queued entries, count bounds them */
} PanelHold;

extern PanelHold itfHeldPanelCursor;

/* Screen-space vertex (x, y) written by the panel rect builders. */
typedef struct PanelPt {
    s32 x;
    s32 y;
} PanelPt;

void itfPanelDispatchHandler(PanelObj *panel) {
    itfPanelHandlers[panel->handlerIndex](panel);
}

void itfPanelSetFourColumnVertices(PanelPt *v, s32 x0, s32 y0, s32 x1, s32 y1) {
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

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199B80);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199CF0);

void itfPanelSetRectVerts(PanelPt *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    PanelPt *p;

    v->x = x0;
    v->y = y0;
    p = v + 1;
    p->x = x1;
    p->y = y0;
    p = v + 2;
    p->x = x1;
    p->y = y1;
    p = v + 3;
    p->x = x0;
    p->y = y1;
}

void itfSetPanelCornerGrid(PanelPt *v, s32 x0, s32 y0, s32 x1, s32 y1) {
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

void itfSetPanelInsetVertexColumns(u8 *base, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[4];
    s32 ys[2];
    PanelPt *v = (PanelPt *)(base + 4);
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

/* Write the same RGB at two offsets, with a transparent first color. */
void itfPanelSetVertexPairs(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x40);

    panelSetVec4(vec, red, green, blue, 0);
    vec = (u32 *)(base + 0x50);
    panelSetVec4(vec, red, green, blue, alpha);
}

void itfSetPanelColorAndAlphaVectors(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x140);

    panelSetVec4(vec, red, green, blue, 1);
    vec = (u32 *)(base + 0x150);
    panelSetVec4(vec, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x160), red, green, blue, alpha, itfPanelColorTemplates, 6);
}

void itfSetPanelColorVectors(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x100);

    panelSetVec4(vec, red, green, blue, 1);
    vec = (u32 *)(base + 0x110);
    panelSetVec4(vec, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x120), red, green, blue, alpha, D_00357B50, 4);
}

void itfSetPanelGradientColor(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x20);

    panelSetVec4(vec, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x30), red, green, blue, alpha, itfPanelGradientColorTemplate, 1);
    itfScaleVectors((s32 *)(base + 0x40), red, green, blue, 0x80, itfPanelGradientColorPair, 2);
}

void itfPanelSetRectSpan(u8 *base, s32 red, s32 green, s32 blue, s32 alpha) {
    u32 *color = (u32 *)(base + 0x20);
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}

/* Two adjacent panel colors: the first is transparent. */
void itfPanelInitRects30(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x30);

    panelSetVec4(vec, red, green, blue, 0);
    vec = (u32 *)(base + 0x40);
    panelSetVec4(vec, red, green, blue, alpha);
}

/* Set both panel colors to the same blue tint with zero alpha. */
void itfPanelSetBlueTint(u8 *base) {
    u32 *vec = (u32 *)(base + 0x44);

    panelSetVec4(vec, 0x73, 0x87, 0xFF, 0);
    vec = (u32 *)(base + 0x54);
    panelSetVec4(vec, 0x73, 0x87, 0xFF, 0);
}

extern u8 D_00357BD0[];
extern u8 D_00357BE0[];

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void itfDrawIndexedPanelFlatQuads(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 4;
    s32 i;

    colors[1].unkC = panel->alpha;
    for (i = 0; i < 3; i++) {
        itfDrawQuadFlat4(buf, colors, &D_00357BD0[i * 4], &D_00357BE0[i * 4], panel->tail, command);
    }
}

void func_0019A200(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 21;
    s32 alpha = panel->alpha;
    s32 i;

    for (i = 0; i < 7; i++, colors++) {
        colors->unkC = alpha;
    }
    colors = buf + 20;
    for (i = 0; i < 3; i++) {
        itfEmitQuadListWide(buf, colors, &D_00357BF0[i * 8], &D_00357C08[i * 8], 8, panel->tail, command);
    }
    for (i = 0; i < 6; i++) {
        itfEmitQuadListA(buf + 8 + i * 2, colors, D_003BB1A8, &D_00357C20[i * 4], 4, panel->tail, command);
    }
}

void func_0019A310(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 17;
    s32 alpha = panel->alpha;
    s32 i;

    for (i = 0; i < 5; i++, colors++) {
        colors->unkC = alpha;
    }
    colors = buf + 16;
    for (i = 0; i < 3; i++) {
        itfEmitQuadListWide(buf, colors, &D_00357C38[i * 8], &D_00357C50[i * 8], 8, panel->tail, command);
    }
    for (i = 0; i < 2; i++) {
        itfEmitQuadListA(buf + 8 + i * 2, colors, D_003BB1B0, &D_00357C68[i * 4], 4, panel->tail, command);
    }
}

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A420);

void itfDrawPanelQuadWithCommand(PanelObj *panel, u64 command) {
    PktRec *colors = panel->buf + 2;

    colors->unkC = panel->alpha;
    itfDrawQuadFlat4(panel->buf, colors, D_003BB1C8, D_003BB1D0, panel->tail, command);
}

void itfEmitPanelQuadPacket(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 3;

    colors[1].unkC = panel->alpha;
    itfSendTablePacket(command, 2, 0);
    itfEmitQuadListWide(buf, colors, D_003BB1D8, D_003BB1E0, 6, panel->tail, command);
    itfSendTablePacket(command, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A628);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A9A8);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AAC0);

void itfDrawTintedPanelRect(PanelObj *panel, u64 command) {
    PanelRect rect;

    D_00357D68[3] = panel->alpha * 0x67 / 128;
    rect.x0 = panel->rect.x0;
    rect.y0 = panel->rect.y0;
    rect.x1 = panel->rect.x1;
    rect.y1 = panel->rect.y1;
    func_00198990(&rect, D_00357D68, panel->tail, 0x240, command);
}

void itfAppendGsPanelStatePacket(void *list) {
    void *packet = sdfAllocPacketAligned(0x40);
    u64 *entry = sdfConsFinalizePacketHeader(packet, 0x40);

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
    rec = itfWindowSlots[window].ptr;
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
    itfAdvancePanelLayoutAndNotify(itfWindowSlots[index].ptr->unkA8, 0, value, 0, 0, 0);
}

s8 itfPanelGetStatus(s32 index) {
    return itfWindowSlots[index].ptr->sub24.status;
}

void itfPanelSetStatus(s32 index, s8 status) {
    itfWindowSlots[index].ptr->sub24.status = status;
}

s32 itfPanelAcquireHold(void) {
    PanelHold *cursor;
    s32 window;

    window = scrGetWindow();
    if (window < 0) {
        return 1;
    }
    cursor = &itfHeldPanelCursor;
    cursor->window = window;
    cursor->unk4 = -1;
    cursor->unk8 = -1;
    cursor->count = 0;
    func_0019C968(window, 3, 0);
    return 1;
}

s32 itfPanelReleaseHold(void) {
    PanelHold *cursor = &itfHeldPanelCursor;

    if (cursor->window < 0) {
        return 1;
    }
    itfMesCleanupWindow(cursor->window, 1);
    cursor->window = -1;
    cursor->unk4 = -1;
    cursor->unk8 = -1;
    cursor->count = 0;
    return 1;
}

/* Queue one (first, second, third) entry from the script parameters; ignored when no hold is active or the queue is full. */
s32 itfCommandQueueHeldPanelEntry(void) {
    PanelHold *cursor = &itfHeldPanelCursor;
    s32 first;
    s32 second;
    s32 third;

    if (cursor->window < 0) {
        return 1;
    }
    if (cursor->count >= 0x40) {
        return 1;
    }
    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    third = scrReadIntParameter(2);
    cursor->items[cursor->count].second = first;
    cursor->items[cursor->count].first = second;
    cursor->items[cursor->count].third = third;
    cursor->count++;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019AFD8);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019B108);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019B1B0);

s16 itfPanelGetPairFirst(s32 index) {
    return itfWindowSlots[index].ptr->pairFirst;
}

void itfPanelSetPairFirst(s32 index, s16 value) {
    itfWindowSlots[index].ptr->pairFirst = value;
}

s16 itfPanelGetPairSecond(s32 index) {
    return itfWindowSlots[index].ptr->pairSecond;
}

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1A8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1B0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1B8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1C0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1C8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1D0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1D8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1E0);

