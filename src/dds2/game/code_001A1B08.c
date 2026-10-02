#include "mnu.h"

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
    s32 tail;    /* 0xC: tail arg of itfDrawQuadFlat4 */
    PanelRect rect; /* 0x10 */
    u8 unk20[0x18]; /* 0x20 */
    s32 alpha;   /* 0x38: copied to a colour record's alpha */
    u8 handlerIndex; /* 0x3C: index into the panel handler table */
} PanelObj;

extern void (*itfPanelHandlers[])(PanelObj *);

/* Flag byte reached as rec+0x24+0x10 (i.e. byte 0x34 of the record). */
typedef struct PanelRecSub {
    u8 unk0[0x10]; /* 0x0 */
    s8 status;     /* 0x10: status read and written by itfPanelGet/SetStatus */
} PanelRecSub;

typedef struct FrFontGlyph FrFontGlyph;

/* The same option-list block used by itfMesBuildOptionList at window +0x40. */
typedef struct PanelOptionBlock {
    u32 x;
    u32 y;
    FrFontGlyph *glyphChain;
    u32 panelValue;
    s16 status;
    s16 clearBitCount;
    u16 unk14;
    s16 rowCount;
} PanelOptionBlock;

/* Payload record referenced by the D_003D6ECC table. */
typedef struct PanelRec {
    u8 unk0[0x24];     /* 0x0 */
    PanelRecSub sub24; /* 0x24 */
    u8 unk35[0xB];     /* 0x35 */
    PanelOptionBlock options; /* 0x40 */
    u8 unk58[0x50];    /* 0x58 */
    s32 unkA8;         /* 0xA8 */
} PanelRec;

/* D_003D6ECC entry (0x14 bytes); payload pointer is the first word. */
typedef struct PanelEntry {
    PanelRec *ptr;  /* 0x0 */
    u8 unk4[0x10];  /* 0x4 */
} PanelEntry;

typedef struct PanelHoldItem {
    s32 first;  /* 0x0 */
    s16 second; /* 0x4 */
    s16 third;  /* 0x6 */
} PanelHoldItem;

typedef struct PanelHold {
    s32 window;
    s16 unk4;
    s16 count;   /* 0x6: entries used in items[] */
    s32 unk8;
    PanelHoldItem items[0x40]; /* 0xC: queued entries, count bounds them */
} PanelHold;

extern PanelHold itfHeldPanelCursor;

extern PanelEntry itfWindowSlots[];

extern void itfAdvancePanelLayoutAndNotify(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void itfMesCleanupWindow(s32 window, s32 arg1);
extern void itfMesSetWindowPageAndRefresh(s32 window, s32 arg1, s32 arg2);
extern u32 scrGetWindow(void);
extern void itfScaleVectors(s32 *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const s32 *input, s32 count);
extern s32 itfPanelColorTemplates[];
extern s32 D_003B4548[];
extern s32 itfPanelGradientColorTemplate[];
extern s32 itfPanelGradientColorPair[];
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
extern u64 *sdfConsFinalizePacketHeader(void *, s32);
extern void sdfAppendPacket(void *, void *);
extern s32 scrReadIntParameter(s32);
extern s32 itfMesStartEntry(s32 window, s32 arg1, s32 arg2);
extern u8 D_003B45E8[], D_003B4600[], D_003B4618[];
extern u8 D_003B4630[], D_003B4648[], D_003B4660[];
extern u8 D_004365A8[8], D_004365B0[8];
extern void itfEmitQuadListA(void *, void *, u8 *, u8 *, s32, u32, u64);


typedef struct PanelVert {
    s32 x;
    s32 y;
} PanelVert;

void itfPanelDispatchHandler(PanelObj *panel) {
    itfPanelHandlers[panel->handlerIndex](panel);
}

void itfPanelSetFourColumnVertices(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
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

void itfSetPanelCornerGrid(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
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
void itfPanelSetVertexPairs(PanelVert *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
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
    itfScaleVectors((s32 *)(base + 0x120), red, green, blue, alpha, D_003B4548, 4);
}

void itfSetPanelGradientColor(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *vec = (u32 *)(base + 0x20);

    panelSetVec4(vec, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x30), red, green, blue, alpha, itfPanelGradientColorTemplate, 1);
    itfScaleVectors((s32 *)(base + 0x40), red, green, blue, 0x80, itfPanelGradientColorPair, 2);
}

void itfPanelSetRectSpan(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    PanelVert *p = &v[4];
    p->x = x0;
    p->y = y0;
    p[1].x = x1;
    p[1].y = y1;
}

void itfPanelInitRects30(PanelVert *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    PanelVert *p = &v[6];
    p->x = x0;
    p->y = y0;
    p[1].x = x1;
    p[1].y = 0;
    p = &v[8];
    p->x = x0;
    p->y = y0;
    p[1].x = x1;
    p[1].y = y1;
}

/* The panel's two four-channel colors begin 0x44 and 0x54 bytes in. */
typedef struct PanelColorPair {
    s32 pad00[0x11];
    s32 first[4];
    s32 second[4];
} PanelColorPair;

/* Set both panel colors to the same blue tint with zero alpha. */
void itfPanelSetBlueTint(PanelColorPair *panel) {
    s32 *color = panel->first;
    color[0] = 0x73;
    color[1] = 0x87;
    color[2] = 0xFF;
    color[3] = 0;
    color = panel->second;
    color[0] = 0x73;
    color[1] = 0x87;
    color[2] = 0xFF;
    color[3] = 0;
}

extern u8 D_003B45C8[];
extern u8 D_003B45D8[];

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void itfDrawIndexedPanelFlatQuads(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 4;
    s32 i;

    colors[1].unkC = panel->alpha;
    for (i = 0; i < 3; i++) {
        itfDrawQuadFlat4(buf, colors, &D_003B45C8[i * 4], &D_003B45D8[i * 4], panel->tail, command);
    }
}

void itfDrawSevenColorPanelQuads(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 21;
    s32 alpha = panel->alpha;
    s32 i;

    for (i = 0; i < 7; i++, colors++) {
        colors->unkC = alpha;
    }
    colors = buf + 20;
    for (i = 0; i < 3; i++) {
        itfEmitQuadListWide(buf, colors, &D_003B45E8[i * 8], &D_003B4600[i * 8], 8, panel->tail, command);
    }
    for (i = 0; i < 6; i++) {
        itfEmitQuadListA(buf + 8 + i * 2, colors, D_004365A8, &D_003B4618[i * 4], 4, panel->tail, command);
    }
}

void itfDrawFiveColorPanelQuads(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 17;
    s32 alpha = panel->alpha;
    s32 i;

    for (i = 0; i < 5; i++, colors++) {
        colors->unkC = alpha;
    }
    colors = buf + 16;
    for (i = 0; i < 3; i++) {
        itfEmitQuadListWide(buf, colors, &D_003B4630[i * 8], &D_003B4648[i * 8], 8, panel->tail, command);
    }
    for (i = 0; i < 2; i++) {
        itfEmitQuadListA(buf + 8 + i * 2, colors, D_004365B0, &D_003B4660[i * 4], 4, panel->tail, command);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2450);

void itfDrawPanelQuadWithCommand(PanelObj *panel, u64 command) {
    PktRec *colors = panel->buf + 2;

    colors->unkC = panel->alpha;
    itfDrawQuadFlat4(panel->buf, colors, D_004365C8, D_004365D0, panel->tail, command);
}

void itfEmitPanelQuadPacket(PanelObj *panel, u64 command) {
    PktRec *buf = panel->buf;
    PktRec *colors = buf + 3;

    colors[1].unkC = panel->alpha;
    itfSendTablePacket(command, 2, 0);
    itfEmitQuadListWide(buf, colors, D_004365D8, D_004365E0, 6, panel->tail, command);
    itfSendTablePacket(command, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2658);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A29D8);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A2AF0);

void itfDrawTintedPanelRect(PanelObj *panel, u64 command) {
    PanelRect rect;

    D_003B4760[3] = panel->alpha * 0x67 / 128;
    rect.x0 = panel->rect.x0;
    rect.y0 = panel->rect.y0;
    rect.x1 = panel->rect.x1;
    rect.y1 = panel->rect.y1;
    func_001A09C0(&rect, D_003B4760, panel->tail, 0x240, command);
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

void itfPanelSetStatus(s32 index, s8 value) {
    itfWindowSlots[index].ptr->sub24.status = value;
}

s32 itfPanelAcquireHold(void) {
    PanelHold *hold;
    s32 window;

    window = scrGetWindow();
    if (window < 0) {
        return 1;
    }
    hold = &itfHeldPanelCursor;
    hold->window = window;
    hold->unk4 = -1;
    hold->unk8 = -1;
    hold->count = 0;
    itfMesSetWindowPageAndRefresh(window, 3, 0);
    return 1;
}

s32 itfPanelReleaseHold(void) {
    PanelHold *hold = &itfHeldPanelCursor;

    if (hold->window < 0) {
        return 1;
    }
    itfMesCleanupWindow(hold->window, 1);
    hold->window = -1;
    hold->unk4 = -1;
    hold->unk8 = -1;
    hold->count = 0;
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

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3008);

INCLUDE_ASM(const s32, "game/code_001A1B08", func_001A3138);

extern u32 scrGetCommandTimer(void);
extern void scrSetIntegerReturnValue(s32);
extern void itfMesBuildOptionList(s32 window, s32 entryIndex);
extern void itfMesResetWindow(s32 window);
extern void kwlnDrawSetDc8Second(u32);
extern void kwlnDrawSetDc8First(u32);
extern void kwlnDrawSetupDc8(s32);
extern void kwlnDrawEnableDc8(s32);
extern s32 D_00438F28;

/* Fade the message page in before polling the option block for completion. */
s32 func_001A31E0(void) {
    s32 window;
    s32 timer;
    s32 entry;
    PanelRec *record;
    PanelOptionBlock *options;

    window = scrGetWindow();
    if (window < 0) {
        return 1;
    }
    timer = scrGetCommandTimer();
    if (scrGetCommandTimer() == 0) {
        kwlnDrawSetDc8Second(0x44);
        kwlnDrawSetDc8First(0x5E1C1C1C);
        kwlnDrawSetupDc8(30);
        D_00438F28 = -1;
        return 0;
    }
    if (timer < 30) {
        return 0;
    }
    if (timer == 30) {
        itfMesSetWindowPageAndRefresh(window, 3, 0);
    } else if (D_00438F28 == -1) {
        record = itfWindowSlots[window].ptr;
        entry = scrReadIntParameter(0);
        options = &record->options;
        if (options->status == 0) {
            itfMesBuildOptionList(window, entry);
        } else if (options->status < 0) {
            options->status = 0;
            scrSetIntegerReturnValue(options->clearBitCount);
            itfMesResetWindow(window);
            kwlnDrawEnableDc8(10);
            D_00438F28 = 0;
            return 1;
        }
    }
    return 0;
}

s16 itfPanelGetPairFirst(s32 index) {
    return itfWindowSlots[index].ptr->options.status;
}

void itfPanelSetPairFirst(s32 index, s16 value) {
    itfWindowSlots[index].ptr->options.status = value;
}

s16 itfPanelGetPairSecond(s32 index) {
    return itfWindowSlots[index].ptr->options.clearBitCount;
}

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365A8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365E0);

