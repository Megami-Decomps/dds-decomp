#include "mnu.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_parameter_index.h"
#include "scr_integer_return_value_api.h"
#include "scr_command_timer_api.h"
#include "sdf_packet_list.h"
#include "itf.h"
#include "itf_mes_window.h"
#include "itf_mes_panel_mode.h"
#include "itf_mes_entry_start_api.h"
#include "sdf.h"
#include "itf_panel_draw.h"
#include "itf_text_vector.h"
#include "itf_panel_api.h"
#include "itf_panel_layouts.h"
#include "itf_held_panel_message_step.h"

#define ITF_PANEL_COLUMN_COUNT 4
#define ITF_PANEL_ROW_COUNT 2
#define ITF_PANEL_COLUMN_INSET 0x200
#define ITF_PANEL_CORNER_COLUMN_COUNT 3
#define ITF_PANEL_CORNER_STRIP_WIDTH 0x5A0
#define ITF_PANEL_FULL_COLOR_SCALE 0x80
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


extern void (*DrawTable[])(itfpanel_t *, sdfDrawTag_t *);

extern PanelHold itfHeldPanelCursor;

extern PanelEntry pinst[];

extern void itfMesMngRequestMessageRelease(s32 window, s32 releasePrimaryBlock);
extern ItfMesWindowIndex scrGetMessageHandle(void);
extern void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);
extern void itfMesMngRequestSelectRelease(s32 window);
extern void itfMiscSetRatioRGB(DrawColorRec *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const TextVector *input, s32 count);
extern TextVector itfPanelColorTemplates[];
extern TextVector D_003B4548[];
extern TextVector itfPanelGradientColorTemplate[];
extern TextVector itfPanelGradientColorPair[];
extern void itfMiscDrawRectanglePrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);
extern u8 D_004365C8[8];
extern u8 D_004365D0[8];
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *command, s32 index, s32 flag);
extern void itfMiscDrawTriangleStripPrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);
extern u8 D_004365D8[8];
extern u8 D_004365E0[8];
extern void itfDrawThreePanelBorderBands(PanelRect *bounds, u32 *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);
extern u32 D_003B4760[];
#include "sdf_packet_alloc.h"
extern void *sdf2DMakeStdPacket(void *packet, s32 packetBytes);

extern u8 D_003B45E8[], D_003B4600[], D_003B4618[];
extern u8 D_003B4630[], D_003B4648[], D_003B4660[];
extern u8 D_004365A8[8], D_004365B0[8];
extern void itfMiscDrawLineStripPrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);
extern u8 D_003B4670[];
extern u8 D_003B4680[];
extern u8 D_004365B8[8];
extern u8 D_004365C0[8];


/* Invoke the indexed panel handler; panel and kind are trusted. */
void itfPnlDrawTag(itfpanel_t *panel, sdfDrawTag_t *list) {
    DrawTable[panel->kind](panel, list);
}

/* Emit top/bottom vertices for four columns; collapse overlapping inner columns to the midpoint.
   Coordinates and inset remain in the renderer's native integer units. */
void CreateVertexNormal(DrawVertex *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
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

extern s32 D_003B44D8[4];

/* Emit the outer frame grid, then the table-spaced top/bottom edge rows. */
void CreateVertexMesWin(DrawVertex *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[ITF_PANEL_COLUMN_COUNT];
    s32 ys[4];
    s32 i;
    s32 j;

    xs[0] = x0 - 0x100;
    xs[1] = x0 + ITF_PANEL_COLUMN_INSET;
    xs[2] = x1 - ITF_PANEL_COLUMN_INSET;
    xs[3] = x1 + 0x100;
    ys[0] = y0 - 0x80;
    ys[1] = y0 + 0x80;
    ys[2] = y1 - 0x80;
    ys[3] = y1 + 0x80;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[i];
            vertices++;
        }
    }
    xs[0] = x0 - 0x100;
    xs[1] = x0 + 0x400;
    xs[2] = x1 - 0x400;
    xs[3] = x1 + 0x100;
    ys[0] = y0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] + D_003B44D8[i];
            vertices++;
        }
    }
    ys[0] = y1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] - D_003B44D8[i];
            vertices++;
        }
    }
}

/* Emit the outer frame grid, then the close top/bottom edge rows. */
void CreateVertexSelWin(DrawVertex *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[ITF_PANEL_COLUMN_COUNT];
    s32 ys[4];
    s32 i;
    s32 j;

    xs[0] = x0 - 0x100;
    xs[1] = x0 + ITF_PANEL_COLUMN_INSET;
    xs[2] = x1 - ITF_PANEL_COLUMN_INSET;
    xs[3] = x1 + 0x100;
    ys[0] = y0 - 0x80;
    ys[1] = y0 + 0x80;
    ys[2] = y1 - 0x80;
    ys[3] = y1 + 0x80;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[i];
            vertices++;
        }
    }
    xs[0] = x0 - 0x100;
    xs[1] = x0 + 0x400;
    xs[2] = x1 - 0x400;
    xs[3] = x1 + 0x100;
    ys[0] = y0;
    for (i = 0; i < ITF_PANEL_ROW_COUNT; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] + i * 8;
            vertices++;
        }
    }
    ys[0] = y1;
    for (i = 0; i < ITF_PANEL_ROW_COUNT; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] - i * 8;
            vertices++;
        }
    }
}

/* Write the rectangle corners in top-left, top-right, bottom-right, bottom-left order. */
void CreateVertexPlain(DrawVertex *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    DrawVertex *p;
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

/* Write two rows of three columns; the middle column is the right edge minus a fixed strip width. */
void CreateVertexLeftFade(DrawVertex *v, s32 x0, s32 y0, s32 x1, s32 y1) {
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
void CreateVertexFacility(UiSpriteBandPayload *base, s32 x0, s32 y0, s32 x1, s32 y1) {
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

/* Populate DrawColorRec RGBA rows at bytes 0x40/0x50; the first row is transparent. */
void CreateColorNormal(DrawVertex *panelData, s32 red, s32 green, s32 blue, s32 alpha) {
    DrawColorRec *colorRow = (DrawColorRec *)((u8 *)panelData + 0x40);
    colorRow->components[0] = red;
    colorRow->components[1] = green;
    colorRow->components[2] = blue;
    colorRow->components[3] = 0;
    colorRow = (DrawColorRec *)((u8 *)panelData + 0x50);
    colorRow->components[0] = red;
    colorRow->components[1] = green;
    colorRow->components[2] = blue;
    colorRow->components[3] = alpha;
}

/* Write the two color rows, then scale six template rows; the first fourth component is one. */
void CreateColorMesWin(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x140);

    panelSetVec4(colorRow, red, green, blue, 1);
    colorRow = (u32 *)(base + 0x150);
    panelSetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((DrawColorRec *)(base + 0x160), red, green, blue, alpha, itfPanelColorTemplates, 6);
}

/* Write the two color rows, then scale four template rows; the first fourth component is one. */
void CreateColorSelWin(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x100);

    panelSetVec4(colorRow, red, green, blue, 1);
    colorRow = (u32 *)(base + 0x110);
    panelSetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((DrawColorRec *)(base + 0x120), red, green, blue, alpha, D_003B4548, 4);
}

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void CreateColorPlain(u8 *base, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = (u32 *)(base + 0x20);

    panelSetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((DrawColorRec *)(base + 0x30), red, green, blue, alpha, itfPanelGradientColorTemplate, 1);
    itfMiscSetRatioRGB((DrawColorRec *)(base + 0x40), red, green, blue, ITF_PANEL_FULL_COLOR_SCALE, itfPanelGradientColorPair, 2);
}

/* Set the four components of the DrawColorRec at byte 0x20. */
void CreateColorPera1(DrawVertex *panelData, s32 red, s32 green, s32 blue, s32 alpha) {
    DrawColorRec *color = (DrawColorRec *)((u8 *)panelData + 0x20);
    color->components[0] = red;
    color->components[1] = green;
    color->components[2] = blue;
    color->components[3] = alpha;
}

/* Populate DrawColorRec RGBA rows at bytes 0x30/0x40; the first row is transparent. */
void CreateColorLeftFade(DrawVertex *panelData, s32 red, s32 green, s32 blue, s32 alpha) {
    DrawColorRec *colorRow = (DrawColorRec *)((u8 *)panelData + 0x30);
    colorRow->components[0] = red;
    colorRow->components[1] = green;
    colorRow->components[2] = blue;
    colorRow->components[3] = 0;
    colorRow = (DrawColorRec *)((u8 *)panelData + 0x40);
    colorRow->components[0] = red;
    colorRow->components[1] = green;
    colorRow->components[2] = blue;
    colorRow->components[3] = alpha;
}



/* Set both panel colors to the same blue tint with zero alpha. */
void CreateColorFacility(UiSpriteBandPayload *panel) {
    u32 *colorRow = panel->colors[0].components;
    colorRow[0] = 0x73;
    colorRow[1] = 0x87;
    colorRow[2] = 0xFF;
    colorRow[3] = 0;
    colorRow = panel->colors[1].components;
    colorRow[0] = 0x73;
    colorRow[1] = 0x87;
    colorRow[2] = 0xFF;
    colorRow[3] = 0;
}

extern u8 D_003B45C8[];
extern u8 D_003B45D8[];

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void DrawNormal(itfpanel_t *panel, sdfDrawTag_t *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 4;
    s32 quadIndex;

    colors[1].alpha = panel->alpha;
    for (quadIndex = 0; quadIndex < ITF_PANEL_WIDE_PASS_COUNT; quadIndex++) {
        itfMiscDrawRectanglePrim(drawBuffer, colors, &D_003B45C8[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], &D_003B45D8[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], panel->renderValue, command);
    }
}

/* Update seven alpha words, then emit three wide and six side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawMesWin(itfpanel_t *panel, sdfDrawTag_t *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 21;
    s32 alpha = panel->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_COLOR_COUNT; rowIndex++, colors++) {
        colors->alpha = alpha;
    }
    colors = drawBuffer + 20;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawTriangleStripPrim(drawBuffer, colors, &D_003B45E8[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &D_003B4600[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, panel->renderValue, command);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_SIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawLineStripPrim((DrawVertex *)(drawBuffer + 8 + rowIndex * 2), (DrawColorRec *)colors, D_004365A8, &D_003B4618[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, panel->renderValue, command);
    }
}

/* Update five alpha words, then emit three wide and two side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawSelWin(itfpanel_t *panel, sdfDrawTag_t *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 17;
    s32 alpha = panel->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_COLOR_COUNT; rowIndex++, colors++) {
        colors->alpha = alpha;
    }
    colors = drawBuffer + 16;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawTriangleStripPrim(drawBuffer, colors, &D_003B4630[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &D_003B4648[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, panel->renderValue, command);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_SIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawLineStripPrim((DrawVertex *)(drawBuffer + 8 + rowIndex * 2), (DrawColorRec *)colors, D_004365B0, &D_003B4660[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, panel->renderValue, command);
    }
}

void DrawPlain(itfpanel_t *panel, sdfDrawTag_t *command) {
    DrawVertex vertices[12];
    PktRec *buf = (PktRec *)panel->payload;
    PktRec *colors = buf + 2;
    DrawVertex *input = (DrawVertex *)buf;
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 xOffset = i * 16;
        s32 yOffset = i * 8;
        DrawVertex *out = &vertices[i * 4];

        out[0].x = input[0].x + xOffset;
        out[0].y = input[0].y + yOffset;
        out[1].x = input[1].x - xOffset;
        out[1].y = input[1].y + yOffset;
        out[2].x = input[2].x - xOffset;
        out[2].y = input[2].y - yOffset;
        out[3].x = input[3].x + xOffset;
        out[3].y = input[3].y - yOffset;
    }

    itfMiscDrawRectanglePrim(buf, colors, D_004365B8, D_004365C0, panel->renderValue, command);
    for (i = 0; i < 3; i++) {
        itfMiscDrawLineStripPrim(vertices, (DrawColorRec *)colors, &D_003B4670[i * 5], &D_003B4680[i * 5], 5, panel->renderValue, command);
    }
}

/* Update the first color row's alpha and emit one indexed quad. */
void DrawPera1(itfpanel_t *panel, sdfDrawTag_t *command) {
    PktRec *colors = (PktRec *)panel->payload + 2;

    colors->alpha = panel->alpha;
    itfMiscDrawRectanglePrim(panel->payload, colors, D_004365C8, D_004365D0, panel->renderValue, command);
}

/* Bracket the six-index panel draw with table-state two and its native reset to zero. */
void DrawLeftFade(itfpanel_t *panel, sdfDrawTag_t *command) {
    PktRec *drawBuffer = (PktRec *)panel->payload;
    PktRec *colors = drawBuffer + 3;

    colors[1].alpha = panel->alpha;
    itfMiscDrawAlphaPrim(command, 2, 0);
    itfMiscDrawTriangleStripPrim(drawBuffer, colors, D_004365D8, D_004365E0, 6, panel->renderValue, command);
    itfMiscDrawAlphaPrim(command, 0, 0);
}

/*BEGIN itfDrawFramedPanelWithTextureStrips*/
extern u32 D_003B4690[];
extern DrawColorRec D_003B4700;
extern DrawColorRec D_003B4710;

/* Draw the framed panel: two bordered bands, then six texture-bound strips whose
 * rectangles are rebuilt from the live panel bounds. */
void itfDrawFramedPanelWithTextureStrips(itfpanel_t *panel, sdfDrawTag_t *command) {
    PanelRect rect;
    UiSpriteTexturePayload *payload = (UiSpriteTexturePayload *)panel->payload;
    s32 *color = &panel->red;
    s32 alpha;
    s32 x;

    alpha = panel->alpha * 57 / 128;
    D_003B4690[3] = alpha;
    rect.x0 = panel->left;
    rect.y0 = panel->top;
    rect.x1 = panel->right;
    rect.y1 = panel->bottom;
    itfDrawThreePanelBorderBands(&rect, D_003B4690, panel->renderValue, 0x490, command);
    alpha = panel->alpha * 38 / 128;
    D_003B4690[7] = alpha;
    rect.x0 = panel->left + 0x720;
    rect.y0 = panel->top;
    rect.x1 = panel->left + 0x17E0;
    rect.y1 = panel->bottom;
    itfDrawThreePanelBorderBands(&rect, &D_003B4690[4], panel->renderValue, 0x680, command);
    rect.x0 = panel->left - 0x140;
    rect.y0 = panel->top + 0x48;
    rect.x1 = panel->left + 0xB40;
    rect.y1 = panel->bottom;
    itfMiscDrawSpritePrim((DrawVertex *)&rect, (DrawColorRec *)&D_003B4700, (DrawColorRec *)color, panel->renderValue, payload->borrowedTexture, 0, command);
    if (panel->scrollSpan > 0 && panel->screenY != panel->top) {
        alpha = 0x80 - ((panel->screenY - panel->top) << 7) / panel->scrollSpan;
    } else {
        alpha = panel->alpha;
    }
    D_003B4690[15] = alpha;
    rect.x0 = panel->right - 0xB40;
    rect.y0 = panel->top;
    rect.x1 = panel->right + 0x140;
    rect.y1 = panel->bottom - 0x48;
    itfMiscDrawSpritePrim((DrawVertex *)&rect, (DrawColorRec *)&D_003B4710, (DrawColorRec *)&D_003B4690[12], panel->renderValue, payload->borrowedTexture, 0, command);
    D_003B4690[11] = panel->alpha * 70 / 128;
    rect.x0 = panel->left - 0x140;
    rect.y0 = panel->top - 0xB8;
    rect.x1 = panel->left + 0x1A40;
    rect.y1 = panel->top + 8;
    itfMiscDrawSpritePrim((DrawVertex *)&rect, (DrawColorRec *)&D_003B4710 - 1, (DrawColorRec *)&D_003B4690[8], panel->renderValue, payload->borrowedTexture, 0, command);
    rect.x0 = panel->left + 0x8B0;
    rect.y0 = panel->top - 0x10;
    rect.x1 = panel->left + 0xFC0;
    rect.y1 = panel->top;
    itfMiscDrawSpritePrim((DrawVertex *)&rect, (DrawColorRec *)&D_003B4710 - 3, (DrawColorRec *)color, panel->renderValue, payload->borrowedTexture, 0, command);
    x = rect.x1;
    rect.x0 = x;
    rect.x1 = x + 0x1040;
    itfMiscDrawSpritePrim((DrawVertex *)&rect, (DrawColorRec *)&D_003B4710 - 2, (DrawColorRec *)color, panel->renderValue, payload->borrowedTexture, 0, command);
    rect.x0 = panel->left + 0x1020;
    rect.y0 = panel->bottom - 0x10;
    rect.x1 = panel->left + 0x1730;
    rect.y1 = panel->bottom;
    itfMiscDrawSpritePrim((DrawVertex *)&rect, (DrawColorRec *)&D_003B4710 - 4, (DrawColorRec *)color, panel->renderValue, payload->borrowedTexture, 0, command);
    x = rect.x0;
    rect.x1 = x;
    rect.x0 = x - 0x1220;
    itfMiscDrawSpritePrim((DrawVertex *)&rect, (DrawColorRec *)&D_003B4710 - 2, (DrawColorRec *)color, panel->renderValue, payload->borrowedTexture, 0, command);
}
/*END itfDrawFramedPanelWithTextureStrips*/

extern DrawColorRec D_003B4720;
extern DrawColorRec D_003B4730;
extern void itfMiscDrawSpritePrim2(DrawVertex *, DrawColorRec *, DrawColorRec *, u32, s32, sdfDrawTag_t *);

/* Bind the texture for the left cap, then draw the center and reversed right
 * cap. Insets are captured before the first submission; Y bounds are live. */
void DrawSpkWin2(itfpanel_t *panel, sdfDrawTag_t *command) {
    DrawVertex rect[2];
    DrawColorRec *color = (DrawColorRec *)&panel->red;
    UiSpriteTexturePayload *payload = (UiSpriteTexturePayload *)panel->payload;
    s32 leftInset = panel->left + 0x3E0;
    s32 rightInset = panel->right - 0x3E0;

    rect[0].x = panel->left;
    rect[0].y = panel->top;
    rect[1].x = leftInset;
    rect[1].y = panel->bottom;
    itfMiscDrawSpritePrim((DrawVertex *)rect, (DrawColorRec *)&D_003B4720, (DrawColorRec *)color, panel->renderValue,
                                  payload->borrowedTexture, 0, command);

    rect[0].x = leftInset;
    rect[0].y = panel->top;
    rect[1].x = rightInset;
    rect[1].y = panel->bottom;
    itfMiscDrawSpritePrim2(rect, &D_003B4730, color, panel->renderValue, 0, command);

    rect[0].x = panel->right;
    rect[0].y = panel->top;
    rect[1].x = rightInset;
    rect[1].y = panel->bottom;
    itfMiscDrawSpritePrim2(rect, &D_003B4720, color, panel->renderValue, 0, command);
}


extern u8 D_003B4740[];
extern u8 D_003B4750[];
extern DrawColorRec D_003B4470;
extern DrawColorRec D_003B4480;
extern DrawColorRec D_003B4490;
extern DrawColorRec D_003B44A0;

/* Draw the three inset flat quads, then split a tall texture into top, stretch
 * and bottom bands. The short path uses the live rectangle in the panel. */
void DrawFacility(itfpanel_t *panel, sdfDrawTag_t *command) {
    DrawVertex rect[2];
    UiSpriteBandPayload *payload = (UiSpriteBandPayload *)panel->payload;
    DrawVertex *vertices = payload->vertices;
    DrawColorRec *colors = payload->colors;
    s32 i;
    s32 top;
    s32 bottom;

    colors[1].components[3] = (panel->alpha * 0x20) >> 7;
    for (i = 0; i < 3; i++) {
        itfMiscDrawRectanglePrim(vertices, colors, &D_003B4740[i * 4],
                        &D_003B4750[i * 4], panel->renderValue, command);
    }
    bottom = panel->bottom;
    top = panel->top;
    if (bottom - top >= 0x569) {
        s32 topEnd = top + 0x2B4;
        s32 bottomStart = bottom - 0x2B4;
        s32 *color = &panel->red;

        rect[0].x = panel->left;
        rect[0].y = top;
        rect[1].x = panel->right;
        rect[1].y = topEnd;
        itfMiscDrawSpritePrim((DrawVertex *)rect, (DrawColorRec *)&D_003B4470, (DrawColorRec *)color, panel->renderValue,
                                      payload->borrowedTexture, 0, command);
        rect[0].y = topEnd;
        rect[1].y = bottomStart;
        itfMiscDrawSpritePrim((DrawVertex *)rect, (DrawColorRec *)&D_003B4480, (DrawColorRec *)color, panel->renderValue,
                                      payload->borrowedTexture, 0, command);
        rect[0].y = bottomStart;
        rect[1].y = panel->bottom;
        itfMiscDrawSpritePrim((DrawVertex *)rect, (DrawColorRec *)&D_003B4490, (DrawColorRec *)color, panel->renderValue,
                                      payload->borrowedTexture, 0, command);
    } else {
        itfMiscDrawSpritePrim((DrawVertex *)&panel->left, (DrawColorRec *)&D_003B44A0,
                                      (DrawColorRec *)&panel->red, panel->renderValue,
                                      payload->borrowedTexture, 0, command);
    }
}


/* Copy bounds and draw the tinted border; alpha uses the native signed 103/128 scale. */
void itfDrawTintedPanelRect(itfpanel_t *panel, sdfDrawTag_t *command) {
    PanelRect rect;

    D_003B4760[3] = panel->alpha * ITF_PANEL_TINT_ALPHA_NUMERATOR / ITF_PANEL_TINT_ALPHA_DENOMINATOR;
    rect.x0 = panel->left;
    rect.y0 = panel->top;
    rect.x1 = panel->right;
    rect.y1 = panel->bottom;
    itfDrawThreePanelBorderBands(&rect, D_003B4760, panel->renderValue, ITF_PANEL_TINT_BORDER_WIDTH, command);
}

/* Append two GS A+D state writes: TEST_1 (0x47), then ALPHA_1 (0x42). */
void DrawTestAlphaPrim(sdfDrawTag_t *list) {
    void *packet = (void *)sdfGrAllocPacketBuff(ITF_PANEL_STATE_PACKET_BYTES);
    u64 *stateWords = (u64 *)sdf2DMakeStdPacket(packet, ITF_PANEL_STATE_PACKET_BYTES);

    stateWords[4] = 0x5101B;
    stateWords[5] = 0x47;
    stateWords[6] = 0x44;
    stateWords[7] = 0x42;
    sdfDrawTagLink(list, (u32)packet);
}

/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */
s32 itfMesMngMessage(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    PanelRec *record;
    PanelRecSub *entryState;
    s32 entryIndex;
    s8 status;

    if (window < 0) {
        return 1;
    }
    record = pinst[window].ptr;
    entryIndex = scrGetIntPara(0);
    entryState = &record->sub24;
    status = entryState->status;
    if (status == 0) {
        if (itfMesStartEntry(window, entryIndex, 0) == ITF_MES_ENTRY_START_EMPTY_TABLE) {
            return 1;
        }
    } else if (status < 0) {
        entryState->status = 0;
        return 1;
    }
    return 0;
}

/* Forward this record's layout sprite address and the supplied value; index is unchecked. */
void itfMesMngSetOffsetY1(s32 windowIndex, s32 topDelta) {
    itfPnlSetPositionOffset(pinst[windowIndex].ptr->layoutSprite, 0, topDelta, 0, 0, 0);
}

/* Return the signed byte entry status; this is separate from the option-block s16 status. */
s8 itfMesMngGetPhase(s32 windowIndex) {
    return pinst[windowIndex].ptr->sub24.status;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 windowIndex, s8 status) {
    pinst[windowIndex].ptr->sub24.status = status;
}

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
s32 itfPanelAcquireHold(void) {
    PanelHold *hold;
    ItfMesWindowIndex window;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    hold = &itfHeldPanelCursor;
    hold->window = window;
    hold->unk4 = -1;
    hold->entryEndFrame = -1;
    hold->queuedCount = 0;
    itfMesMngChangeWindowType(window, ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION, 0);
    return 1;
}

/* Clean up the active held window and reset its queue; an inactive hold is a no-op. */
s32 itfPanelReleaseHold(void) {
    PanelHold *hold = &itfHeldPanelCursor;

    if (hold->window < 0) {
        return 1;
    }
    itfMesMngRequestMessageRelease(hold->window, 1);
    hold->window = -1;
    hold->unk4 = -1;
    hold->entryEndFrame = -1;
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
    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    third = scrGetIntPara(2);
    cursor->items[cursor->queuedCount].second = first;
    cursor->items[cursor->queuedCount].first = second;
    cursor->items[cursor->queuedCount].third = third;
    cursor->queuedCount++;
    return 1;
}

ItfHeldPanelMessageStepResult itfStepHeldPanelMessages(void) {
    s32 frame;
    s32 latest = -1;
    s32 i;

    if (itfHeldPanelCursor.window < 0) {
        return ITF_HELD_PANEL_MESSAGES_COMPLETE;
    }
    frame = scrGetCommandTimer();
    if (itfHeldPanelCursor.queuedCount == 0) {
        return ITF_HELD_PANEL_MESSAGES_COMPLETE;
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
            itfHeldPanelCursor.entryEndFrame = deadline;
        }
    }
    if (frame == itfHeldPanelCursor.entryEndFrame) {
        itfMesMngRequestMessageRelease(itfHeldPanelCursor.window, 1);
        itfHeldPanelCursor.unk4 = itfHeldPanelCursor.entryEndFrame = -1;
    }
    if (frame >= latest && frame >= itfHeldPanelCursor.entryEndFrame) {
        itfPanelReleaseHold();
        return ITF_HELD_PANEL_MESSAGES_COMPLETE;
    }
    return ITF_HELD_PANEL_MESSAGES_PENDING;
}


s32 itfMesMngSelect(void) {
    ItfMesWindowIndex window;
    s32 entry;
    PanelRec *record;
    PanelOptionBlock *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    record = pinst[window].ptr;
    entry = scrGetIntPara(0);
    options = &record->options;
    if (options->status == 0) {
        itfMesBuildOptionList(window, entry);
    } else if (options->status < 0) {
        options->status = 0;
        scrSetIntReturnValue(options->clearBitCount);
        itfMesMngRequestSelectRelease(window);
        return 1;
    }
    return 0;
}

extern void dds3SetFilter2Para(u32);
extern void dds3SetFilter2RGBA(u32);
extern void dds3Filter2On(s32);
extern void dds3Filter2Off(s32);
extern s32 D_00438F28;

/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */
s32 itfCommandSelectOptionWithFade(void) {
    ItfMesWindowIndex window;
    s32 elapsedTicks;
    s32 entryIndex;
    PanelRec *record;
    PanelOptionBlock *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    elapsedTicks = scrGetCommandTimer();
    if (scrGetCommandTimer() == 0) {
        dds3SetFilter2Para(0x44);
        dds3SetFilter2RGBA(0x5E1C1C1C);
        dds3Filter2On(ITF_PANEL_MESSAGE_FADE_TICKS);
        D_00438F28 = -1;
        return 0;
    }
    if (elapsedTicks < ITF_PANEL_MESSAGE_FADE_TICKS) {
        return 0;
    }
    if (elapsedTicks == ITF_PANEL_MESSAGE_FADE_TICKS) {
        itfMesMngChangeWindowType(window, ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION, 0);
    } else if (D_00438F28 == -1) {
        record = pinst[window].ptr;
        entryIndex = scrGetIntPara(0);
        options = &record->options;
        if (options->status == 0) {
            itfMesBuildOptionList(window, entryIndex);
        } else if (options->status < 0) {
            options->status = 0;
            scrSetIntReturnValue(options->clearBitCount);
            itfMesMngRequestSelectRelease(window);
            dds3Filter2Off(ITF_PANEL_MESSAGE_EXIT_TICKS);
            D_00438F28 = 0;
            return 1;
        }
    }
    return 0;
}

/* Return the option-block signed status word, not the separate entry status byte;
 * its callers read the sign-extended halfword as a word. */
s32 itfMesMngGetSelPhase(s32 windowIndex) {
    return pinst[windowIndex].ptr->options.status;
}

/* Store the option-block signed status word without validating the slot. */
void itfMesMngSetSelPhase(s32 windowIndex, s16 status) {
    pinst[windowIndex].ptr->options.status = status;
}

/* Return the same signed word forwarded as the script result when an option completes;
 * its callers read the sign-extended halfword as a word. */
s32 itfMesMngGetSelectNo(s32 windowIndex) {
    return pinst[windowIndex].ptr->options.clearBitCount;
}

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365A8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365B8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365C8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D0);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365D8);

INCLUDE_SDATA(const s32, "game/code_001A1B08", D_004365E0);

