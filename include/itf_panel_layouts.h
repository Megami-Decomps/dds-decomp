#ifndef ITF_PANEL_LAYOUTS_H
#define ITF_PANEL_LAYOUTS_H

#include "common.h"
#include "itf.h"

#define ITF_PANEL_HOLD_CAPACITY 0x40

/* The draw buffer is traversed in 0x10-byte records; its payload is opaque. */
typedef struct PktRec {
    u8 bytes00[0xC];
    u32 alpha; /* +0xC: set from panel alpha or the draw alpha argument */
} PktRec;

/* Integer corners passed to the panel-border drawer. */
typedef struct PanelRect {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} PanelRect;

/* Point written by the panel vertex builders. */
typedef struct PanelVertex {
    s32 x;
    s32 y;
} PanelVertex;

/* Panel table payload. Only the status byte in this subrecord is interpreted. */
typedef struct PanelRecSub {
    u8 bytes[0x10];
    s8 status;
} PanelRecSub;

/* Option-list fields used by the panel status and row helpers. */
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

/* The surrounding table payload remains opaque; known subviews are retained. */
typedef struct PanelRec {
    u8 bytes00[0x24];
    PanelRecSub sub24;
    u8 bytes35[0xB];
    PanelOptionBlock options;
    u8 bytes58[0x50];
    s32 unkA8;
} PanelRec;

/* Table entry begins with its payload pointer; the trailing words are unknown. */
typedef struct PanelEntry {
    PanelRec *ptr;
    u8 bytes04[0x10];
} PanelEntry;

typedef struct PanelHoldItem {
    s32 first;
    s16 second;
    s16 third;
} PanelHoldItem;

/* Bounded held-panel queue. Unknown scalar slots stay unnamed. */
typedef struct PanelHold {
    s32 window;
    s16 unk4;
    s16 queuedCount;
    s32 unk8;
    PanelHoldItem items[ITF_PANEL_HOLD_CAPACITY];
} PanelHold;

typedef char PktRec_size_must_be_0x10[sizeof(PktRec) == 0x10 ? 1 : -1];
typedef char PktRec_alpha_at_0x0C[
    ((u32)&((PktRec *)0)->alpha == 0x0C) ? 1 : -1];
typedef char PanelRect_size_must_be_0x10[sizeof(PanelRect) == 0x10 ? 1 : -1];
typedef char PanelRect_x1_at_0x08[
    ((u32)&((PanelRect *)0)->x1 == 0x08) ? 1 : -1];
typedef char PanelRecSub_status_at_0x10[
    ((u32)&((PanelRecSub *)0)->status == 0x10) ? 1 : -1];
typedef char PanelOptionBlock_size_must_be_0x18[
    sizeof(PanelOptionBlock) == 0x18 ? 1 : -1];
typedef char PanelRec_subrecord_at_0x24[
    ((u32)&((PanelRec *)0)->sub24 == 0x24) ? 1 : -1];
typedef char PanelRec_options_at_0x40[
    ((u32)&((PanelRec *)0)->options == 0x40) ? 1 : -1];
typedef char PanelRec_opaque_word_at_0xA8[
    ((u32)&((PanelRec *)0)->unkA8 == 0xA8) ? 1 : -1];
typedef char PanelEntry_ptr_at_0x00[
    ((u32)&((PanelEntry *)0)->ptr == 0x00) ? 1 : -1];
typedef char PanelEntry_size_must_be_0x14[
    sizeof(PanelEntry) == 0x14 ? 1 : -1];
typedef char PanelHoldItem_size_must_be_8[sizeof(PanelHoldItem) == 8 ? 1 : -1];
typedef char PanelHold_items_at_0x0C[
    ((u32)&((PanelHold *)0)->items == 0x0C) ? 1 : -1];
typedef char PanelHold_size_must_be_0x20C[
    sizeof(PanelHold) == 0x20C ? 1 : -1];
typedef char PanelVertex_size_must_be_8[sizeof(PanelVertex) == 8 ? 1 : -1];
typedef char PanelVertex_y_at_0x04[
    ((u32)&((PanelVertex *)0)->y == 4) ? 1 : -1];

#endif
