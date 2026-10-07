#include "mnu.h"
#include "itf.h"
#include "sdf.h"
#include "itf_panel_draw.h"

#define ITF_PANEL_COLUMN_COUNT 4
#define ITF_PANEL_ROW_COUNT 2
#define ITF_PANEL_COLUMN_INSET 0x200
#define ITF_PANEL_CORNER_COLUMN_COUNT 3
#define ITF_PANEL_CORNER_STRIP_WIDTH 0x5A0
#define ITF_PANEL_FULL_COLOR_SCALE 0x80
#define ITF_PANEL_HOLD_CAPACITY 0x40
#define ITF_PANEL_STATE_PACKET_BYTES 0x40
#define ITF_PANEL_QUAD_INDEX_COUNT 4
#define ITF_PANEL_WIDE_INDEX_COUNT 8
#define ITF_PANEL_WIDE_PASS_COUNT 3
#define ITF_PANEL_SEVEN_COLOR_COUNT 7
#define ITF_PANEL_FIVE_COLOR_COUNT 5
#define ITF_PANEL_SEVEN_SIDE_PASS_COUNT 6
#define ITF_PANEL_FIVE_SIDE_PASS_COUNT 2
#define ITF_PANEL_MESSAGE_FADE_TICKS 30
#define ITF_PANEL_MESSAGE_EXIT_TICKS 10
#define ITF_PANEL_TINT_ALPHA_NUMERATOR 0x67
#define ITF_PANEL_TINT_ALPHA_DENOMINATOR 128
#define ITF_PANEL_TINT_BORDER_WIDTH 0x240


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

/* Flag byte reached as rec+0x24+0x10 (i.e. byte 0x34 of the record). */
typedef struct PanelRecSub {
    u8 unk0[0x10]; /* 0x0 */
    s8 status;      /* 0x10 */
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

/* Payload record referenced by the itfWindowSlots table. */
typedef struct PanelRec {
    u8 unk0[0x24];     /* 0x0 */
    PanelRecSub sub24; /* 0x24 */
    u8 unk35[0xB];     /* 0x35 */
    PanelOptionBlock options; /* 0x40 */
    u8 unk58[0x50];    /* 0x58 */
    s32 unkA8;         /* 0xA8 */
} PanelRec;

/* itfWindowSlots entry (0x14 bytes); payload pointer is the first word. */
typedef struct PanelEntry {
    PanelRec *ptr;  /* 0x0 */
    u8 unk4[0x10];  /* 0x4 */
} PanelEntry;

extern PanelEntry itfWindowSlots[];
extern void (*itfPanelHandlers[])(UiSprite *, SdfListHead *);
extern void itfAdvancePanelLayoutAndNotify(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern u32 scrGetWindow(void);
extern void scrSetIntegerReturnValue(s32);
extern void itfMesBuildOptionList(s32 window, s32 entryIndex);
extern void itfMesResetWindow(s32 window);
extern void itfMesCleanupWindow(s32 window, s32 arg1);
extern void itfMesSetWindowPageAndRefresh(s32 window, s32 arg1, s32 arg2);
extern void itfScaleVectors(s32 *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const s32 *input, s32 count);
extern s32 itfPanelColorTemplates[];
extern s32 D_00357B50[];
extern void itfDrawQuadFlat4(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, SdfListHead *command);
extern u8 D_003BB1C8[8];
extern u8 D_003BB1D0[8];
extern s32 sdfAllocPacketAligned(s32);
extern u32 sdfConsFinalizePacketHeader(u32, s32);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void func_00198990(PanelRect *rect, u32 *colors, u32 tail, s32 width, SdfListHead *command);
extern u32 D_00357D68[];
extern void itfSendTablePacket(SdfListHead *command, s32 index, s32 flag);
extern void itfEmitQuadListWide(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, SdfListHead *command);
extern u8 D_003BB1D8[8];
extern u8 D_003BB1E0[8];
extern s32 itfPanelGradientColorTemplate[];
extern s32 itfPanelGradientColorPair[];
extern s32 scrReadIntParameter(s32);
extern s32 itfMesStartEntry(s32 window, s32 arg1, s32 arg2);
extern u8 D_00357BF0[], D_00357C08[], D_00357C20[];
extern u8 D_00357C38[], D_00357C50[], D_00357C68[];
extern u8 D_003BB1A8[8], D_003BB1B0[8];
extern void itfEmitQuadListA(void *, void *, u8 *, u8 *, s32, u32, SdfListHead *);
extern u8 D_00357C78[];
extern u8 D_00357C88[];
extern u8 D_003BB1B8[8];
extern u8 D_003BB1C0[8];


typedef struct PanelHoldItem {
    s32 first;  /* 0x0 */
    s16 second; /* 0x4 */
    s16 third;  /* 0x6 */
} PanelHoldItem;

/* Selection state shared by the id helpers below. */
typedef struct PanelHold {
    s32 window;  /* 0x0: window id (>= 0 valid) */
    s16 unk4;  /* 0x4 */
    s16 queuedCount; /* 0x6: entries used in items[] */
    s32 unk8;  /* 0x8 */
    PanelHoldItem items[ITF_PANEL_HOLD_CAPACITY]; /* 0xC: native 64-entry queue */
} PanelHold;

extern PanelHold itfHeldPanelCursor;

/* Screen-space vertex (x, y) written by the panel rect builders. */
typedef struct PanelPt {
    s32 x;
    s32 y;
} PanelPt;

/* Invoke the indexed panel handler; panel and kind are trusted. */
void itfPanelDispatchHandler(UiSprite *panel, SdfListHead *list) {
    itfPanelHandlers[panel->kind](panel, list);
}

/* Emit top/bottom vertices for four columns; collapse overlapping inner columns to the midpoint.
   Coordinates and inset remain in the renderer's native integer units. */
void itfPanelSetFourColumnVertices(PanelPt *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_COLUMN_COUNT];
    s32 columnIndex;

    columnX[0] = x0;
    columnX[1] = x0 + ITF_PANEL_COLUMN_INSET;
    columnX[2] = x1 - ITF_PANEL_COLUMN_INSET;
    columnX[3] = x1;
    if (columnX[2] < columnX[1]) {
        columnX[1] = ((x1 - x0) >> 1) + x0;
        columnX[2] = columnX[1];
    }
    for (columnIndex = 0; columnIndex < ITF_PANEL_COLUMN_COUNT; columnIndex++) {
        vertices->x = columnX[columnIndex];
        vertices->y = y0;
        vertices++;
        vertices->x = columnX[columnIndex];
        vertices->y = y1;
        vertices++;
    }
}

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199B80);

INCLUDE_ASM(const s32, "game/code_00199AD8", func_00199CF0);

/* Write the rectangle corners in top-left, top-right, bottom-right, bottom-left order. */
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

/* Write two rows of three columns; the middle column is the right edge minus a fixed strip width. */
void itfSetPanelCornerGrid(PanelPt *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_CORNER_COLUMN_COUNT];
    s32 rowY[ITF_PANEL_ROW_COUNT];
    s32 rowIndex;
    s32 columnIndex;

    columnX[0] = x0;
    columnX[1] = x1 - ITF_PANEL_CORNER_STRIP_WIDTH;
    columnX[2] = x1;
    rowY[0] = y0;
    rowY[1] = y1;
    for (rowIndex = 0; rowIndex < ITF_PANEL_ROW_COUNT; rowIndex++) {
        for (columnIndex = 0; columnIndex < ITF_PANEL_CORNER_COLUMN_COUNT; columnIndex++) {
            v->x = columnX[columnIndex];
            v->y = rowY[rowIndex];
            v++;
        }
    }
}

/* Write four fixed horizontal columns with inset top/bottom coordinates.
   x0 and x1 are intentionally unused; do not replace the fixed horizontal positions. */
void itfSetPanelInsetVertexColumns(UiSpriteBandPayload *base, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_COLUMN_COUNT];
    s32 rowY[ITF_PANEL_ROW_COUNT];
    DrawVertex *vertices = base->vertices;
    s32 columnIndex;
    columnX[0] = 0x1A0;
    columnX[1] = 0x480;
    columnX[2] = 0x1B70;
    columnX[3] = 0x1E50;
    rowY[0] = y0 + 0x160;
    rowY[1] = y1 - 0x140;
    for (columnIndex = 0; columnIndex < ITF_PANEL_COLUMN_COUNT; columnIndex++) {
        vertices->x = columnX[columnIndex];
        vertices->y = rowY[0];
        vertices++;
        vertices->x = columnX[columnIndex];
        vertices->y = rowY[1];
        vertices++;
    }
}

/* Populate RGBA rows at bytes 0x40/0x50; the first row is transparent. */
void itfPanelSetVertexPairs(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x40);

    panelSetVec4(colorRow, red, green, blue, 0);
    colorRow = (u32 *)(base + 0x50);
    panelSetVec4(colorRow, red, green, blue, alpha);
}

/* Write the two color rows, then scale six template rows; the first fourth component is one. */
void itfSetPanelColorAndAlphaVectors(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x140);

    panelSetVec4(colorRow, red, green, blue, 1);
    colorRow = (u32 *)(base + 0x150);
    panelSetVec4(colorRow, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x160), red, green, blue, alpha, itfPanelColorTemplates, 6);
}

/* Write the two color rows, then scale four template rows; the first fourth component is one. */
void itfSetPanelColorVectors(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x100);

    panelSetVec4(colorRow, red, green, blue, 1);
    colorRow = (u32 *)(base + 0x110);
    panelSetVec4(colorRow, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x120), red, green, blue, alpha, D_00357B50, 4);
}

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void itfSetPanelGradientColor(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x20);

    panelSetVec4(colorRow, red, green, blue, alpha);
    itfScaleVectors((s32 *)(base + 0x30), red, green, blue, alpha, itfPanelGradientColorTemplate, 1);
    itfScaleVectors((s32 *)(base + 0x40), red, green, blue, ITF_PANEL_FULL_COLOR_SCALE, itfPanelGradientColorPair, 2);
}

/* Set the four color words at byte 0x20, consumed as a color record by the quad path. */
void itfPanelSetRectSpan(u8 *base, s32 red, s32 green, s32 blue, s32 alpha) {
    u32 *color = (u32 *)(base + 0x20);
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}

/* Populate RGBA rows at bytes 0x30/0x40; the first row is transparent. */
void itfPanelInitRects30(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x30);

    panelSetVec4(colorRow, red, green, blue, 0);
    colorRow = (u32 *)(base + 0x40);
    panelSetVec4(colorRow, red, green, blue, alpha);
}

/* Set both panel colors to the same blue tint with zero alpha. */
void itfPanelSetBlueTint(UiSpriteBandPayload *base) {
    u32 *colorRow = base->colors[0].components;

    panelSetVec4(colorRow, 0x73, 0x87, 0xFF, 0);
    colorRow = base->colors[1].components;
    panelSetVec4(colorRow, 0x73, 0x87, 0xFF, 0);
}

extern u8 D_00357BD0[];
extern u8 D_00357BE0[];

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void itfDrawIndexedPanelFlatQuads(UiSprite *panel, SdfListHead *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 4;
    s32 quadIndex;

    colors[1].unkC = panel->unk38;
    for (quadIndex = 0; quadIndex < ITF_PANEL_WIDE_PASS_COUNT; quadIndex++) {
        itfDrawQuadFlat4(drawBuffer, colors, &D_00357BD0[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], &D_00357BE0[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], panel->unk0C, command);
    }
}

/* Update seven alpha words, then emit three wide and six side strips.
   The first color row is deliberately excluded from the alpha update. */
void itfDrawSevenColorPanelQuads(UiSprite *panel, SdfListHead *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 21;
    s32 alpha = panel->unk38;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_COLOR_COUNT; rowIndex++, colors++) {
        colors->unkC = alpha;
    }
    colors = drawBuffer + 20;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfEmitQuadListWide(drawBuffer, colors, &D_00357BF0[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &D_00357C08[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, panel->unk0C, command);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_SIDE_PASS_COUNT; rowIndex++) {
        itfEmitQuadListA(drawBuffer + 8 + rowIndex * 2, colors, D_003BB1A8, &D_00357C20[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, panel->unk0C, command);
    }
}

/* Update five alpha words, then emit three wide and two side strips.
   The first color row is deliberately excluded from the alpha update. */
void itfDrawFiveColorPanelQuads(UiSprite *panel, SdfListHead *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 17;
    s32 alpha = panel->unk38;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_COLOR_COUNT; rowIndex++, colors++) {
        colors->unkC = alpha;
    }
    colors = drawBuffer + 16;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfEmitQuadListWide(drawBuffer, colors, &D_00357C38[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &D_00357C50[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, panel->unk0C, command);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_SIDE_PASS_COUNT; rowIndex++) {
        itfEmitQuadListA(drawBuffer + 8 + rowIndex * 2, colors, D_003BB1B0, &D_00357C68[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, panel->unk0C, command);
    }
}

void func_0019A420(UiSprite *panel, SdfListHead *command) {
    PanelPt vertices[12];
    PktRec *buf = (PktRec *)panel->payload;
    PktRec *colors = buf + 2;
    PanelPt *input = (PanelPt *)buf;
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 xOffset = i * 16;
        s32 yOffset = i * 8;
        PanelPt *out = &vertices[i * 4];

        out[0].x = input[0].x + xOffset;
        out[0].y = input[0].y + yOffset;
        out[1].x = input[1].x - xOffset;
        out[1].y = input[1].y + yOffset;
        out[2].x = input[2].x - xOffset;
        out[2].y = input[2].y - yOffset;
        out[3].x = input[3].x + xOffset;
        out[3].y = input[3].y - yOffset;
    }

    itfDrawQuadFlat4(buf, colors, D_003BB1B8, D_003BB1C0, panel->unk0C, command);
    for (i = 0; i < 3; i++) {
        itfEmitQuadListA(vertices, colors, &D_00357C78[i * 5], &D_00357C88[i * 5], 5, panel->unk0C, command);
    }
}

/* Update the first color row's alpha and emit one indexed quad. */
void itfDrawPanelQuadWithCommand(UiSprite *panel, SdfListHead *command) {
    PktRec *colors = (PktRec *)panel->payload + 2;

    colors->unkC = panel->unk38;
    itfDrawQuadFlat4(panel->payload, colors, D_003BB1C8, D_003BB1D0, panel->unk0C, command);
}

/* Bracket the six-index panel draw with table-state two and its native reset to zero. */
void itfEmitPanelQuadPacket(UiSprite *panel, SdfListHead *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 3;

    colors[1].unkC = panel->unk38;
    itfSendTablePacket(command, 2, 0);
    itfEmitQuadListWide(drawBuffer, colors, D_003BB1D8, D_003BB1E0, 6, panel->unk0C, command);
    itfSendTablePacket(command, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00199AD8", func_0019A628);

extern DrawColorRec D_00357D28;
extern DrawColorRec D_00357D38;
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);
extern void itfQueueColoredTexturedQuadPacket(DrawVertex *, DrawColorRec *, DrawColorRec *, u32, s32, SdfListHead *);

/* Bind the texture for the left cap, then draw the center and reversed right
 * cap. Insets are captured before the first submission; Y bounds are live. */
void func_0019A9A8(UiSprite *panel, SdfListHead *command) {
    DrawVertex rect[2];
    DrawColorRec *color = (DrawColorRec *)&panel->unk2C;
    UiSpriteTexturePayload *payload = (UiSpriteTexturePayload *)panel->payload;
    s32 leftInset = panel->left + 0x3E0;
    s32 rightInset = panel->right - 0x3E0;

    rect[0].x = panel->left;
    rect[0].y = panel->top;
    rect[1].x = leftInset;
    rect[1].y = panel->bottom;
    itfQueueTextureBoundQuadPacket(rect, &D_00357D28, color, panel->unk0C,
                                  payload->texture, 0, command);

    rect[0].x = leftInset;
    rect[0].y = panel->top;
    rect[1].x = rightInset;
    rect[1].y = panel->bottom;
    itfQueueColoredTexturedQuadPacket(rect, &D_00357D38, color, panel->unk0C, 0, command);

    rect[0].x = panel->right;
    rect[0].y = panel->top;
    rect[1].x = rightInset;
    rect[1].y = panel->bottom;
    itfQueueColoredTexturedQuadPacket(rect, &D_00357D28, color, panel->unk0C, 0, command);
}


extern u8 D_00357D48[];
extern u8 D_00357D58[];
extern DrawColorRec D_00357A78;
extern DrawColorRec D_00357A88;
extern DrawColorRec D_00357A98;
extern DrawColorRec D_00357AA8;

/* Draw the three inset flat quads, then split a tall texture into top, stretch
 * and bottom bands. The short path uses the live rectangle in the panel. */
void func_0019AAC0(UiSprite *panel, SdfListHead *command) {
    DrawVertex rect[2];
    UiSpriteBandPayload *payload = (UiSpriteBandPayload *)panel->payload;
    DrawVertex *vertices = payload->vertices;
    DrawColorRec *colors = payload->colors;
    s32 i;
    s32 top;
    s32 bottom;

    colors[1].components[3] = (panel->unk38 * 0x20) >> 7;
    for (i = 0; i < 3; i++) {
        itfDrawQuadFlat4(vertices, colors, &D_00357D48[i * 4],
                        &D_00357D58[i * 4], panel->unk0C, command);
    }
    bottom = panel->bottom;
    top = panel->top;
    if (bottom - top >= 0x569) {
        s32 topEnd = top + 0x2B4;
        s32 bottomStart = bottom - 0x2B4;
        s32 *color = &panel->unk2C;

        rect[0].x = panel->left;
        rect[0].y = top;
        rect[1].x = panel->right;
        rect[1].y = topEnd;
        itfQueueTextureBoundQuadPacket(rect, &D_00357A78, color, panel->unk0C,
                                      payload->texture, 0, command);
        rect[0].y = topEnd;
        rect[1].y = bottomStart;
        itfQueueTextureBoundQuadPacket(rect, &D_00357A88, color, panel->unk0C,
                                      payload->texture, 0, command);
        rect[0].y = bottomStart;
        rect[1].y = panel->bottom;
        itfQueueTextureBoundQuadPacket(rect, &D_00357A98, color, panel->unk0C,
                                      payload->texture, 0, command);
    } else {
        itfQueueTextureBoundQuadPacket(&panel->left, &D_00357AA8,
                                      &panel->unk2C, panel->unk0C,
                                      payload->texture, 0, command);
    }
}


/* Copy bounds and draw the tinted border; alpha uses the native signed 103/128 scale. */
void itfDrawTintedPanelRect(UiSprite *panel, SdfListHead *command) {
    PanelRect rect;

    D_00357D68[3] = panel->unk38 * ITF_PANEL_TINT_ALPHA_NUMERATOR / ITF_PANEL_TINT_ALPHA_DENOMINATOR;
    rect.x0 = panel->left;
    rect.y0 = panel->top;
    rect.x1 = panel->right;
    rect.y1 = panel->bottom;
    func_00198990(&rect, D_00357D68, panel->unk0C, ITF_PANEL_TINT_BORDER_WIDTH, command);
}

/* Append two GS A+D state writes: TEST_1 (0x47), then ALPHA_1 (0x42). */
void itfAppendGsPanelStatePacket(SdfListHead *list) {
    s32 packet = sdfAllocPacketAligned(ITF_PANEL_STATE_PACKET_BYTES);
    u64 *stateWords = (u64 *)sdfConsFinalizePacketHeader(packet, ITF_PANEL_STATE_PACKET_BYTES);

    stateWords[4] = 0x5101B;
    stateWords[5] = 0x47;
    stateWords[6] = 0x44;
    stateWords[7] = 0x42;
    sdfAppendPacket(list, packet);
}

/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */
s32 itfPanelStartEntry(void) {
    s32 window = scrGetWindow();
    PanelRec *record;
    PanelRecSub *entryState;
    s32 entryIndex;
    s8 status;

    if (window < 0) {
        return 1;
    }
    record = itfWindowSlots[window].ptr;
    entryIndex = scrReadIntParameter(0);
    entryState = &record->sub24;
    status = entryState->status;
    if (status == 0) {
        if (itfMesStartEntry(window, entryIndex, 0) == 0) {
            return 1;
        }
    } else if (status < 0) {
        entryState->status = 0;
        return 1;
    }
    return 0;
}

/* Forward this record's opaque layout word and the supplied value; index is unchecked. */
void itfPanelEmitRecord(s32 index, s32 value) {
    itfAdvancePanelLayoutAndNotify(itfWindowSlots[index].ptr->unkA8, 0, value, 0, 0, 0);
}

/* Return the signed byte entry status; this is separate from the option-block s16 status. */
s8 itfPanelGetStatus(s32 index) {
    return itfWindowSlots[index].ptr->sub24.status;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfPanelSetStatus(s32 index, s8 status) {
    itfWindowSlots[index].ptr->sub24.status = status;
}

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
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
    hold->queuedCount = 0;
    itfMesSetWindowPageAndRefresh(window, 3, 0);
    return 1;
}

/* Clean up the active held window and reset its queue; an inactive hold is a no-op. */
s32 itfPanelReleaseHold(void) {
    PanelHold *hold = &itfHeldPanelCursor;

    if (hold->window < 0) {
        return 1;
    }
    itfMesCleanupWindow(hold->window, 1);
    hold->window = -1;
    hold->unk4 = -1;
    hold->unk8 = -1;
    hold->queuedCount = 0;
    return 1;
}

/* Queue script words in native second/first/third field order; first and third narrow to s16.
   Inactive/full queues are ignored with status one; negative queuedCount is not checked. */
s32 itfCommandQueueHeldPanelEntry(void) {
    PanelHold *cursor = &itfHeldPanelCursor;
    s32 first;
    s32 second;
    s32 third;

    if (cursor->window < 0) {
        return 1;
    }
    if (cursor->queuedCount >= ITF_PANEL_HOLD_CAPACITY) {
        return 1;
    }
    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    third = scrReadIntParameter(2);
    cursor->items[cursor->queuedCount].second = first;
    cursor->items[cursor->queuedCount].first = second;
    cursor->items[cursor->queuedCount].third = third;
    cursor->queuedCount++;
    return 1;
}

s32 func_0019AFD8(void) {
    s32 frame;
    s32 latest = -1;
    s32 i;

    if (itfHeldPanelCursor.window < 0) {
        return 1;
    }
    frame = scrGetCommandTimer();
    if (itfHeldPanelCursor.queuedCount == 0) {
        return 1;
    }
    for (i = 0; i < itfHeldPanelCursor.queuedCount; i++) {
        s32 start = itfHeldPanelCursor.items[i].first;

        if (start >= latest) {
            latest = start;
        }
        if (start == frame) {
            s16 entry = itfHeldPanelCursor.items[i].second;
            s32 deadline = frame + itfHeldPanelCursor.items[i].third;

            itfMesStartEntry(itfHeldPanelCursor.window, entry, 0);
            itfHeldPanelCursor.unk4 = entry;
            itfHeldPanelCursor.unk8 = deadline;
        }
    }
    if (frame == itfHeldPanelCursor.unk8) {
        itfMesCleanupWindow(itfHeldPanelCursor.window, 1);
        itfHeldPanelCursor.unk4 = itfHeldPanelCursor.unk8 = -1;
    }
    if (frame >= latest && frame >= itfHeldPanelCursor.unk8) {
        itfPanelReleaseHold();
        return 1;
    }
    return 0;
}

s32 func_0019B108(void) {
    s32 window;
    s32 entry;
    PanelRec *record;
    PanelOptionBlock *options;

    window = scrGetWindow();
    if (window < 0) {
        return 1;
    }
    record = itfWindowSlots[window].ptr;
    entry = scrReadIntParameter(0);
    options = &record->options;
    if (options->status == 0) {
        itfMesBuildOptionList(window, entry);
    } else if (options->status < 0) {
        options->status = 0;
        scrSetIntegerReturnValue(options->clearBitCount);
        itfMesResetWindow(window);
        return 1;
    }
    return 0;
}

extern u32 scrGetCommandTimer(void);
extern void kwlnDrawSetDc8Second(u32);
extern void kwlnDrawSetDc8First(u32);
extern void kwlnDrawSetupDc8(s32);
extern void kwlnDrawEnableDc8(s32);
extern s32 D_003BD820;

/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */
s32 itfCommandSelectOptionWithFade(void) {
    s32 window;
    s32 elapsedTicks;
    s32 entryIndex;
    PanelRec *record;
    PanelOptionBlock *options;

    window = scrGetWindow();
    if (window < 0) {
        return 1;
    }
    elapsedTicks = scrGetCommandTimer();
    if (scrGetCommandTimer() == 0) {
        kwlnDrawSetDc8Second(0x44);
        kwlnDrawSetDc8First(0x5E1C1C1C);
        kwlnDrawSetupDc8(ITF_PANEL_MESSAGE_FADE_TICKS);
        D_003BD820 = -1;
        return 0;
    }
    if (elapsedTicks < ITF_PANEL_MESSAGE_FADE_TICKS) {
        return 0;
    }
    if (elapsedTicks == ITF_PANEL_MESSAGE_FADE_TICKS) {
        itfMesSetWindowPageAndRefresh(window, 3, 0);
    } else if (D_003BD820 == -1) {
        record = itfWindowSlots[window].ptr;
        entryIndex = scrReadIntParameter(0);
        options = &record->options;
        if (options->status == 0) {
            itfMesBuildOptionList(window, entryIndex);
        } else if (options->status < 0) {
            options->status = 0;
            scrSetIntegerReturnValue(options->clearBitCount);
            itfMesResetWindow(window);
            kwlnDrawEnableDc8(ITF_PANEL_MESSAGE_EXIT_TICKS);
            D_003BD820 = 0;
            return 1;
        }
    }
    return 0;
}

/* Return the option-block signed status word, not the separate entry status byte. */
s16 itfPanelGetPairFirst(s32 index) {
    return itfWindowSlots[index].ptr->options.status;
}

/* Store the option-block signed status word without validating the slot. */
void itfPanelSetPairFirst(s32 index, s16 status) {
    itfWindowSlots[index].ptr->options.status = status;
}

/* Return the same signed word forwarded as the script result when an option completes. */
s16 itfPanelGetPairSecond(s32 index) {
    return itfWindowSlots[index].ptr->options.clearBitCount;
}

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1A8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1B0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1B8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1C0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1C8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1D0);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1D8);

INCLUDE_SDATA(const s32, "game/code_00199AD8", D_003BB1E0);

