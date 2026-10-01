#include "common.h"

extern s32 func_00309638(u32);

extern s32 itfFindGridNodeByKey(u32, u32);

typedef struct QuadU32 {
    u32 x; // 0x00
    u32 y; // 0x04
    u32 z; // 0x08
    u32 w; // 0x0C
} QuadU32; // 0x10

typedef struct GridWidget {
    u8 pad0[0x50];
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} GridWidget;

typedef struct GridQuantizedEntry {
    u8 pad0[0x44];
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} GridQuantizedEntry;

typedef struct GridEntryStorage {
    u8 pad00[0x10];
    u8 *entries;           /* 0x10: 0x80 bytes per entry */
    u8 pad14[4];
    u8 *renderEntries;     /* 0x18: 0xA0 bytes per entry */
} GridEntryStorage;

typedef struct GridScrollRange {
    u32 reserved;
    float minimum;        /* 0x04 */
    float maximum;        /* 0x08 */
    float step;           /* 0x0C */
} GridScrollRange;

typedef struct GridScrollEntry {
    u8 pad00[8];
    GridScrollRange *range; /* 0x08 */
    float position;         /* 0x0C */
} GridScrollEntry;

/* Same 0x40-byte text widget layout as the DDS1 grid renderer. */
typedef struct GridTextWidget {
    char *text;           /* 0x00 */
    u16 textLength;       /* 0x04 */
    s16 rows;             /* 0x06 */
    u16 unk08;
    u16 unk0A;
    u32 flags;            /* 0x0C */
    void *unk10;
    void *unk14;
    void *children;       /* 0x18 */
    void *unk1C;
    s32 x;                /* 0x20 */
    s32 y;                /* 0x24 */
    s32 width;            /* 0x28 */
    s32 height;           /* 0x2C */
    u32 reference;        /* 0x30 */
    u8 pad34[8];
    s32 rowOffset;        /* 0x3C */
} GridTextWidget;

typedef struct GridListNode {
    u8 pad00[6];
    u16 key;            /* 0x06, also used as a span in list traversal */
    u8 pad08[0x10];
    void *head;         /* 0x18 */
    u8 *next;           /* 0x1C */
    u32 child;          /* 0x20: child widget for tree traversal */
} GridListNode;

typedef struct GridListOwner {
    u8 pad00[6];
    u16 count;          /* 0x06 */
    u8 pad08[8];
    GridListNode *list; /* 0x10 */
} GridListOwner;

/* Result bit 1: the linked list contains at least owner->count successors. */
enum { GRID_LIST_HAS_COUNT_FOLLOWERS = 2 };

typedef struct GridDrawWork {
    u8 pad00[0xC];
    s16 width;               /* 0x0C */
    s16 height;              /* 0x0E */
    u32 packetHandle;        /* 0x10 */
    u32 overlayHandle;       /* 0x14 */
    u8 overlayEnabled;       /* 0x18 */
    u8 pad19;
    u8 overlayKind;          /* 0x1A */
    u8 pad1B[0x19];
    s32 overlayDataSize;     /* 0x34 */
} GridDrawWork;

extern s32 effGetSlotWorkOrOverride();

extern void func_00306BF0(u32, u32, u32, u32, u32, u32, u32, u32);

extern s32 func_00100400(void);

extern u8 D_00381ED0[];

extern void func_0032DB30(const void *, void *, s32);

extern void sdfAppendDmaTagToList(void *, void *);

typedef struct GridAngleTable {
    s32 divisor;  /* 0x00 */
    s32 mirrored; /* 0x04 */
} GridAngleTable;

typedef struct GridAngleSlot {
    u8 pad00[0x20];
    GridAngleTable *table; /* 0x20 */
} GridAngleSlot;

typedef struct GridAngleOwner {
    u8 pad00[4];
    s32 angle; /* 0x04 */
    u8 pad08[8];
    GridAngleSlot *slot; /* 0x10 */
} GridAngleOwner;

void itfDrawGridWithResolvedSlot(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 context) {
    u32 record = effGetSlotWorkOrOverride(e, f);
    func_00306BF0(a, b, c, d, e, f, record, context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307018);

s32 itfGridLookupValueOrDefault(s32 object, s32 key) {
    s32 entry = effGetSlotWorkOrOverride(object);
    s32 result;

    if (*(s32 *)(entry + 0x30) == 0) {
        func_00307018(object, key);
    }
    result = effUpdateTimedStates(object, key, entry);
    if (result == 0) {
        result = *(s32 *)(object + 0x28);
    }
    return result;
}

extern void func_00304B18();

/* Store grid bounds in the renderer's fixed-point coordinate units. */
void itfSetGridEntryQuantizedAndRefresh(GridEntryStorage *object, s32 index, s32 x, s32 y, s32 width, s32 height) {
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(object->entries + index * 0x80);
    s32 record = effGetSlotWorkOrOverride(object, index);

    entry->x = x >> 4;
    entry->y = y >> 3;
    entry->width = width >> 4;
    entry->height = height >> 3;
    func_00304B18(object, index, record);
}

/* Copy the quantized bounds into the corresponding render entry as four words. */
void itfGridSetQuantizedBounds(GridEntryStorage *object, s32 index, s32 x, s32 y,
                   s32 width, s32 height) {
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(object->entries + index * 0x80);
    u32 *destination = (u32 *)(object->renderEntries + index * 0xA0 + 0x6C);
    u32 *source;
    s32 remaining = 3;
    entry->x = x >> 4;
    entry->y = y >> 3;
    entry->width = width >> 4;
    entry->height = height >> 3;
    source = (u32 *)&entry->x;
    do {
        *destination++ = *source++;
    } while (--remaining >= 0);
}

/* Set the unquantized bounds of the selected grid widget. */
void itfGridSetBounds(s32 a, s32 b, s32 x, s32 y, s32 width, s32 height) {
    GridWidget *widget = (GridWidget *)effGetSlotWorkOrOverride(a, b);
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
}

/* Copy four words from a render entry's +0x84 data to its +0x14 data. */
void itfGridCopyEntryQuad(s32 object, s32 index) {
    u32 *destination;
    s32 remaining;

    remaining = 3;
    destination = (u32 *)(index * 0xa0 + (s32)((GridEntryStorage *)object)->renderEntries + 0x14);
    do {
        remaining = remaining - 1;
        *destination = destination[0x1c];
        destination = destination + 1;
    } while (-1 < remaining);
}

void itfGridStorePosition(s32 *position, s32 x, s32 y) {
    position[0] = x;
    position[1] = y;
}

void itfCreateGridPacketWithDefaultFlags(u32 packetHandle, u32 width, u32 height, u32 data, u32 context) {
    sdfCreateDescriptorPacket(context, packetHandle, 0, 0, width, height, data, 0);
}

/* The overlay packet is present only when this work flag is set. */
u8 itfGridGetOverlayFlag(GridDrawWork *work) {
    return work->overlayEnabled;
}

/* Two overlay kinds use a 16x16 region; other kinds use 8x2. */
void itfDrawGridOverlayPacket(GridDrawWork *work, s32 x, s32 y) {
    s32 width;
    s32 height;
    if (work->overlayKind == 0x13 || work->overlayKind == 0x1B) {
        width = 0x10;
        height = 0x10;
    } else {
        width = 8;
        height = 2;
    }
    itfCreateGridPacketWithDefaultFlags(work->overlayHandle, width, height, x, y);
}

typedef struct RenderCallbackEntry {
    u8 reserved[0x10];
    void (*draw)(void *, s32);
    u8 tail[0xC];
} RenderCallbackEntry;

extern RenderCallbackEntry D_0037FB48[];

extern s32 sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(s32);

/* Build the optional overlay and main packet, then dispatch their draw callback. */
GridDrawWork *itfSubmitGridPacketsAndDraw(GridDrawWork *object, u8 *data, s32 kind) {
    s32 context = sdfAllocPacketAligned(0x20);
    u8 *cursor;
    RenderCallbackEntry *entry;
    sdfInitPacketList(context);
    cursor = data + (data[1] & 0xF0) + 0x40;
    if (itfGridGetOverlayFlag(object) != 0) {
        itfDrawGridOverlayPacket(object, (s32)cursor, context);
        cursor += object->overlayDataSize;
    }
    itfCreateGridPacketWithDefaultFlags(object->packetHandle, object->width,
                  object->height, (s32)cursor, context);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
    return object;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003074F0);

/* Set the selected descriptor word's high control bits. */
void itfSetGridDescriptorControlBit(s32 object, s32 index) {
    s32 descriptor;

    descriptor = *(s32 *)(*(s32 *)(index * 4 + *(s32 *)(object + 0x24)) + 0x28);
    *(u64 *)(descriptor + 0x20) = (*(u64 *)(descriptor + 0x20) & 0x1fffffffffffffff) | 0x4000000000000000;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003075D8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307710);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003078A8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307A68);

/* Convert the owner's fixed-point angle to degrees and return its angular step. */
s32 itfUpdateAngleAndGetCycleStep(s32 unused, u8 *out, GridAngleOwner *owner) {
    GridAngleTable *table = owner->slot->table;
    s32 repetitions = 3;

    do {
        if (table->mirrored == 0) {
            *(f32 *)(out + 0x24) = 360.0f - (f32)owner->angle * 360.0f * (1.0f / 65536.0f);
        } else {
            *(f32 *)(out + 0x24) = (f32)owner->angle * 360.0f * (1.0f / 65536.0f);
        }
    } while (--repetitions >= 0);
    return 0x10000 / table->divisor;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307C30);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307D70);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307EF8);

/* Unpack four 8-bit channels into the low and high halves of two 64-bit words. */
void itfGridUnpackColorChannels(u64 *channels, u32 color) {
    u64 green;
    channels[0] = (color >> 24) | ((u64)((color >> 16) & 0xFF) << 32);
    green = color & 0xFF00;
    channels[1] = (green >> 8) | ((u64)(color & 0xFF) << 32);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308058);

void itfGridDrawBooleanDescriptor(u8 value, s32 alternate, s32 kind) {
    u32 normalized = value != 0;
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[0] = normalized;
    if (!alternate) {
        descriptor[1] = 0x4A;
    } else {
        descriptor[1] = 0x4B;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void func_00308288(u8 value, u32 kind) {
    itfGridDrawBooleanDescriptor(value, 0, kind);
}

void itfSubmitToggledGridWord(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x47;
    } else {
        descriptor[1] = 0x48;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void sdfSubmitGsTestOneRegisterPacket(data, kind)
    u32 data;
    u32 kind;
{
    itfSubmitToggledGridWord(data, 0, kind);
}

void sdfSubmitGsAlphaRegisterPacket(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x42;
    } else {
        descriptor[1] = 0x43;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind) {
    sdfSubmitGsAlphaRegisterPacket(data, 0, kind);
}

void sdfSubmitGsPabeRegisterPacket(s32 data, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[1] = 0x49;
    descriptor[0] = data;
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void sdfSubmitGsTexRegisterPacket(s32 data, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[1] = 0x14;
    descriptor[0] = data;
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void uiFillQuadColorWords(QuadU32 *q, u32 value) {
    q->x = value;
    q->y = value;
    q->z = value;
    q->w = value;
}

void uiDrawUniformRgbRange(u32 a, u32 b, u32 c, u32 value, u32 e, u32 f, u32 g, u32 h) {
    u32 rgb[3] = {value, value, value};
    func_00308650(a, b, c, rgb, e, f, g, h);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308650);

void uiDrawUniformRgbaRange(u32 a, u32 b, u32 c, u32 d, u32 e, u32 value, u32 g, u32 h) {
    u32 rgba[4] = {value, value, value, value};
    func_00308828(a, b, c, d, e, rgba, g, h);
}

void func_00308808(u32 a, u32 b, u32 c, u32 d, u32 e, u32 value, u32 h) {
    uiDrawUniformRgbaRange(a, b, c, d, e, value, 0, h);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308828);

void func_003089B8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 colors, u32 h) {
    func_00308828(a, b, c, d, e, colors, 0, h);
}

/* Draw four frame edges; the bottom edge extends 16 units beyond the right side. */
void uiDrawFrameEdges(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 context) {
    uiDrawUniformColorLine(x, y, z, x + width, y, z, color, context);
    uiDrawUniformColorLine(x, y, z, x, y + height, z, color, context);
    uiDrawUniformColorLine(x + width, y, z, x + width, y + height, z, color, context);
    uiDrawUniformColorLine(x, y + height, z, x + width + 0x10, y + height, z, color, context);
}

void uiDrawUniformColorLine(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 value, u32 context) {
    u32 range[2] = {value, value};
    func_00308AF0(a, b, c, d, e, f, range, context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308AF0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308C58);

void uiDrawActiveSurfaceRegion(s32 surfaceIndex) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList((s32)list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        u8 *surface = (u8 *)D_0037FB48 + (surfaceIndex << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

extern void func_0032DB78(void *, void *, s32);

void sdfDispatchSurfaceWithPreparedTexturePacket(surfaceIndex)
    s32 surfaceIndex;
{
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList((s32)list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB78(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        u8 *surface = (u8 *)D_0037FB48 + (surfaceIndex << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void uiDrawTexturedSurfaceAtFarDepth(u32 context) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    func_00308808(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, context);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, context);
    uiDrawActiveSurfaceRegion(context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308F78);

void uiDrawActiveSurfaceWithTestMode(u32 context) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    func_00308808(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, context);
    sdfSubmitGsTestOneRegisterPacket(0x3001BL, context);
    uiDrawActiveSurfaceRegion(context);
}

void uiConfigureSurfaceAlphaState(s32 context) {
    sdfDispatchSurfaceWithPreparedTexturePacket();
    sdfSubmitGsTestOneRegisterPacket(0x5001BL, context);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, context);
}

void uiDrawSurfaceAtNearDepth(u32 context) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, context);
    func_00308808(0, 0, 0, 0x2000, 0xe00, 0, context);
}

u32 uiScaleColorRgb(u32 color, u32 scale) {
    u32 a = color >> 24;
    u32 b = (color >> 16) & 0xFF;
    u32 c = (color & 0xFF00) >> 8;
    u32 d = color & 0xFF;

    a = (a * scale) >> 8;
    b = (b * scale) >> 8;
    c = (c * scale) >> 8;
    return (a << 24) | (b << 16) | (c << 8) | d;
}

u32 uiBlendColors(u32 c0, u32 c1, u32 t) {
    u32 a0 = c0 >> 24;
    u32 r0 = (c0 & 0xFF0000) >> 16;
    u32 g0 = (c0 & 0xFF00) >> 8;
    u32 b0 = c0 & 0xFF;
    u32 a1 = c1 >> 24;
    u32 r1 = (c1 & 0xFF0000) >> 16;
    u32 g1 = (c1 & 0xFF00) >> 8;
    u32 b1 = c1 & 0xFF;
    u32 inv;

    if (t > 0x100) {
        t = 0x200 - t;
    }
    inv = 0x100 - t;
    a0 = (a0 * t + a1 * inv) >> 8;
    r0 = (r0 * t + r1 * inv) >> 8;
    g0 = (g0 * t + g1 * inv) >> 8;
    b0 = (b0 * t + b1 * inv) >> 8;
    return (a0 << 24) | (r0 << 16) | (g0 << 8) | b0;
}

void func_003091E8(void) {
}

void func_003091F0(void) {
}

u32 func_003091F8(void) {
    return 0;
}

s32 itfActivateGridTextWidget(s32 widget) {
    if (widget == 0) {
        return 0;
    }
    ((GridTextWidget *)widget)->flags = (((GridTextWidget *)widget)->flags & -2) | 2;
    return 1;
}

s32 itfSetWidgetFlagsAndActivateChild(u8 *object, u32 flags) {
    s32 child;
    if (object == 0) {
        return 0;
    }
    ((GridTextWidget *)object)->flags = (((GridTextWidget *)object)->flags & ~2) | flags;
    child = (s32)((GridTextWidget *)object)->children;
    if (child != 0) {
        s32 node = ((GridListNode *)child)->child;
        if (node != 0) {
            ((GridTextWidget *)node)->flags |= 2;
        }
    }
    return 1;
}

GridTextWidget *itfCreateGridTextWidget(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                                        u32 reference) {
    GridTextWidget *widget = (GridTextWidget *)func_00328D68(0x40);
    u32 length;
    char *copy;

    memset(widget, 0, 0x40);
    length = strlen(text) + 1;
    copy = (char *)func_00328D68(length);
    widget->textLength = length;
    widget->text = copy;
    memcpy(copy, text, length);
    widget->unk10 = NULL;
    widget->x = x << 4;
    widget->y = y << 3;
    widget->reference = reference;
    widget->rows = rows;
    widget->width = columns * 12 + 6;
    widget->height = rows * 14 + 6;
    widget->unk14 = NULL;
    widget->children = NULL;
    widget->unk1C = NULL;
    widget->flags = 0;
    widget->unk08 = 0;
    widget->unk0A = 0;
    return widget;
}

void itfSetGridDimensions(GridTextWidget *work, s32 columns, s32 rows) {
    s32 columnWidth = columns * 12 + 6;
    s32 rowHeight = rows * 14 + 6;

    if (columns != 0) {
        work->width = columnWidth;
    }
    if (rows != 0) {
        work->height = rowHeight;
        work->rows = rows;
    }
}

u32 itfDestroyGridTextWidget(GridTextWidget *widget) {
    s64 next;

    sdfReleaseChipBlock(widget->text);
    do {
        next = func_00309638(widget);
    } while (next != 0);
    sdfReleaseChipBlock(widget);
    return 1;
}

u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    u32 list;

    sdfReleaseChipBlock(widget->text);
    list = (u32)widget->children;
    if (list != 0) {
        do {
            u32 child = ((GridListNode *)list)->child;
            if (child != 0) {
                itfDestroyGridTextWidgetTree((GridTextWidget *)child);
            }
            list = func_00309638(widget);
        } while (list != 0);
    }
    sdfReleaseChipBlock(widget);
    return 1;
}

void itfExpandWidgetColumnWidth(s32 columns, GridTextWidget *work) {
    s32 flags = work->flags;
    s32 width;
    if (flags & 0x100) {
        columns += 4;
        if (flags & 0x200) {
            columns += 2;
        }
    }
    width = columns * 12 + 6;
    if (work->width < width) {
        work->width = width;
    }
}

u32 itfGetGridListLinkFlags(GridListOwner *owner) {
    GridListNode *node = owner->list;
    u32 flags;
    s32 i;

    if (node == 0) {
        return 0;
    }
    /* Bit 0 reports whether the first node owns a head object. */
    flags = node->head != 0;
    for (i = 0; i < owner->count; i++) {
        node = node->next;
        if (node == 0) {
            flags &= ~GRID_LIST_HAS_COUNT_FOLLOWERS;
            return flags;
        }
    }
    flags |= GRID_LIST_HAS_COUNT_FOLLOWERS;
    return flags;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309538);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309638);

void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, u8 *node, const char *text) {
    s32 length;
    s32 allocation;
    char *copy;

    sdfReleaseChipBlock(*(u32 *)node);
    length = strlen(text);
    allocation = length + 1;
    copy = (char *)func_00328D68(allocation);
    *(u16 *)(node + 4) = allocation;
    *(char **)node = copy;
    memcpy(copy, text, allocation);
    if (widget != NULL) {
        itfExpandWidgetColumnWidth(length, widget);
    }
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309880);

void itfAdvanceGridScrollPosition(u32 owner, u32 key, s32 steps) {
    GridScrollRange *range;
    GridScrollEntry *entry;
    float *position;
    float delta;
    float previous;

    entry = (GridScrollEntry *)itfFindGridNodeByKey(key, owner);
    range = entry->range;
    position = &entry->position;
    delta = range->step;
    if (1 < steps) {
        delta = delta * (float)(s32)steps;
    }
    previous = *position;
    *position = previous + delta;
    if (range->maximum < previous + delta) {
        *position = range->minimum;
    }
}

typedef struct GridScrollOwner {
    u8 pad00[0xC];
    u32 flags;
    u8 pad10[8];
    u8 *selected;     /* 0x18: node whose key supplies the scroll base */
    u8 pad1C[0x20];
    s32 keyOffset;    /* 0x3C */
} GridScrollOwner;

typedef struct GridScrollKey {
    u8 pad00[6];
    u16 key;
} GridScrollKey;

void itfAdvanceSelectedGridScroll(u32 owner, u32 steps) {
    s32 widgetAddr;

    widgetAddr = (s32)owner;
    if ((((GridScrollOwner *)widgetAddr)->flags & 1) != 0) {
        itfAdvanceGridScrollPosition(owner, (u32)((GridScrollKey *)((GridScrollOwner *)widgetAddr)->selected)->key + ((GridScrollOwner *)widgetAddr)->keyOffset,
                                    steps);
        return;
    }
}

void itfReverseGridScrollPosition(u32 owner, u32 key, s32 steps) {
    GridScrollRange *range;
    GridScrollEntry *entry;
    float *position;
    float delta;
    float previous;

    entry = (GridScrollEntry *)itfFindGridNodeByKey(key, owner);
    range = entry->range;
    position = &entry->position;
    delta = range->step;
    if (1 < steps) {
        delta = delta * (float)(s32)steps;
    }
    previous = *position;
    *position = previous - delta;
    if (previous - delta < range->minimum) {
        *position = range->maximum;
    }
}

void itfReverseSelectedGridScroll(u32 owner, u32 steps) {
    s32 widgetAddr;

    widgetAddr = (s32)owner;
    if ((((GridScrollOwner *)widgetAddr)->flags & 1) != 0) {
        itfReverseGridScrollPosition(owner, (u32)((GridScrollKey *)((GridScrollOwner *)widgetAddr)->selected)->key + ((GridScrollOwner *)widgetAddr)->keyOffset,
                                    steps);
        return;
    }
}

s32 itfGetGridChildLayoutMode(u8 *widget, u32 target) {
    u32 flags = ((GridTextWidget *)widget)->flags;

    if (flags & 2) {
        if (target == (u32)((GridTextWidget *)widget)->children) {
            return (flags & 1) ? 6 : 4;
        }
        return 0;
    }
    if (flags & 0x80) {
        if (target == (u32)((GridTextWidget *)widget)->children) {
            return 12;
        }
    } else if (target == (u32)((GridTextWidget *)widget)->children && (flags & 1)) {
        return 6;
    }
    return 0;
}

typedef struct GridValueEntry {
    char *label;     /* 0x00 */
    u16 pad04;
    u16 index;       /* 0x06 */
    s32 *kind;       /* 0x08: 0 decimal, 1 hex, 2 float */
    f32 value;       /* 0x0C */
    s32 width;       /* 0x10 */
    u8 pad14[0x14];
    void (*format)(void *, void *, char *, s32); /* 0x28 */
} GridValueEntry;

extern s32 strlen(const char *);
extern char *strcpy(char *, const char *);
extern s32 func_0035C860(char *, const char *, ...);
extern double fptodp(f32);

void func_00309C00(GridTextWidget *widget, GridValueEntry *entry, char *out) {
    char text[0x100];
    char prefix[0x100];
    char format[0x100];

    if (entry->format != NULL) {
        entry->format(widget, entry, text, 0x100);
    } else if (entry->kind == NULL) {
        func_0035C860(text, "%s", entry->label);
    } else {
        if (strlen(entry->label) == 0) {
            strcpy(prefix, "");
        } else {
            func_0035C860(prefix, "%s ", entry->label);
        }
        switch (*entry->kind) {
        case 0:
            func_0035C860(format, "%%s%%0%dd", entry->width);
            func_0035C860(text, format, prefix, (s32)entry->value);
            break;
        case 1:
            func_0035C860(format, "%%s0x%%0%dX", entry->width - 2);
            func_0035C860(text, format, prefix, (s32)entry->value);
            break;
        case 2:
            func_0035C860(format, "%%s%%0%d.1f", entry->width);
            func_0035C860(text, format, prefix, (double)entry->value);
            break;
        }
    }
    if (widget->flags & 0x100) {
        s32 row = entry->index + widget->rowOffset;

        if (!(widget->flags & 0x200)) {
            func_0035C860(out, "%03d:%s", row, text);
        } else {
            func_0035C860(out, "0x%03X:%s", row, text);
        }
    } else {
        strcpy(out, text);
    }
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309DF8);

/* The list cursor and its nodes overlap the common grid widget prefix. */
typedef struct GridListCursor {
    u8 pad00[6];
    u16 span;           /* 0x06 */
    u16 skipped;        /* 0x08 */
    s16 count;          /* 0x0A */
    u8 pad0C[4];
    u8 *current;        /* 0x10 */
    u8 *first;          /* 0x14 */
    u8 *selected;       /* 0x18 */
} GridListCursor;

s32 itfFindGridNodeByKey(u32 key, u32 head) {
    u32 n;

    n = (u32)((GridListCursor *)head)->first;
    while (n != 0 && ((GridListNode *)n)->key != key) {
        n = (u32)((GridListNode *)n)->next;
    }
    return n;
}

s32 sdfGridSeekSelectedNodeByIndex(s32 index, u8 *widget) {
    s16 count = ((GridListCursor *)widget)->count;
    u16 width;
    u8 *first;

    if (index >= count) {
        return 0;
    }
    first = ((GridListCursor *)widget)->first;
    ((GridListCursor *)widget)->skipped = 0;
    ((GridListCursor *)widget)->current = first;
    ((GridListCursor *)widget)->selected = first;
    if (index > 0) {
        width = ((GridListCursor *)widget)->span;
        do {
            u8 *current = ((GridListCursor *)widget)->current;
            if (width >= count - ((GridListNode *)current)->key) {
                ((GridListCursor *)widget)->skipped++;
            } else {
                ((GridListCursor *)widget)->current = ((GridListNode *)current)->next;
            }
            current = ((GridListCursor *)widget)->selected;
            ((GridListCursor *)widget)->selected = ((GridListNode *)current)->next;
        } while (--index != 0);
    }
    return 1;
}

void sdfGridSeekFirstNode(u32 widget) {
    sdfGridSeekSelectedNodeByIndex(0, widget);
}

void sdfGridSeekLastNode(u8 *entry) {
    sdfGridSeekSelectedNodeByIndex(((GridListCursor *)entry)->count - 1, entry);
}

