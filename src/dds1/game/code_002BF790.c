#include "common.h"

extern s32 itfFindGridNodeByKey(u32, u32);

extern s32 func_002C1B30(u32);

typedef struct GridPosition {
    s32 x; // 0x00
    s32 y; // 0x04
} GridPosition; // 0x08

/* Four words filled together; their corner/channel interpretation is unknown. */
typedef struct UiQuadWords {
    u32 unk00[4];
} UiQuadWords; // 0x10

typedef struct GridWidget {
    u8 pad0[0x50];
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} GridWidget;

typedef struct GridTextWidget {
    char *text;          /* 0x00 */
    u16 textLength;      /* 0x04 */
    s16 rows;            /* 0x06 */
    u16 unk08;
    u16 unk0A;
    u32 flags;           /* 0x0C */
    void *unk10;
    void *unk14;
    void *children;      /* 0x18 */
    void *unk1C;
    s32 x;               /* 0x20 */
    s32 y;               /* 0x24 */
    s32 width;           /* 0x28 */
    s32 height;          /* 0x2C */
    u32 reference;       /* 0x30 */
    u8 pad34[8];
    s32 rowOffset;       /* 0x3C */
} GridTextWidget;

extern s32 sdfGridSeekSelectedNodeByIndex(s32, void *);

typedef struct GridQuantizedEntry {
    u8 pad0[0x44];
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} GridQuantizedEntry;

typedef struct GridEntryOwner {
    u8 pad00[0x10];
    u8 *quantizedEntries; /* 0x10: 0x80-byte entries */
    u8 pad14[4];
    u8 *records;          /* 0x18: 0xA0-byte entries */
    u8 pad1C[0xC];
    s32 defaultValue;     /* 0x28 */
} GridEntryOwner;

typedef struct GridChildLink {
    u8 pad00[0x20];
    GridTextWidget *child; /* 0x20 */
} GridChildLink;

typedef struct RenderCallbackEntry {
    u8 reserved[0x10];
    void (*draw)(void *, s32);
    u8 tail[0xC];
} RenderCallbackEntry;

extern RenderCallbackEntry kwlnDrawSurfaces[];

extern s32 sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(s32);

void itfDrawGridWithResolvedSlot(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    s32 entry = effGetSlotWorkOrOverride(e, f);
    func_002BF400(a, b, c, d, e, f, entry, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BF828);

/* Resolve an entry by key, falling back to the object's stored value. */
s32 itfGridLookupValueOrDefault(s32 object, s32 key) {
    s32 entry = effGetSlotWorkOrOverride(object);
    s32 result;

    if (*(s32 *)(entry + 0x30) == 0) {
        func_002BF828(object, key);
    }
    result = effUpdateTimedStates(object, key, entry);
    if (result == 0) {
        result = ((GridEntryOwner *)object)->defaultValue;
    }
    return result;
}

extern void func_002BD3D8(void *, s32, void *);

void itfSetGridEntryQuantizedAndRefresh(u8 *object, s32 index, s32 x, s32 y, s32 width, s32 height) {
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(((GridEntryOwner *)object)->quantizedEntries + index * 0x80);
    s32 record = effGetSlotWorkOrOverride((s32)object, index);

    entry->x = x >> 4;
    entry->y = y >> 3;
    entry->width = width >> 4;
    entry->height = height >> 3;
    func_002BD3D8(object, index, (void *)(s32)record);
}

/* Store pixel bounds quantized to the widget's 16x8 grid, then copy all four words. */
void itfGridSetQuantizedBounds(u8 *object, s32 index, s32 x, s32 y,
                   s32 width, s32 height) {
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(((GridEntryOwner *)object)->quantizedEntries + index * 0x80);
    u32 *destination = (u32 *)(((GridEntryOwner *)object)->records + index * 0xA0 + 0x6C);
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

void itfGridSetBounds(s32 a, s32 b, s32 x, s32 y, s32 width, s32 height) {
    GridWidget *widget = (GridWidget *)effGetSlotWorkOrOverride(a, b);
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
}

void itfGridCopyEntryQuad(s32 owner, s32 index) {
    u32 *destination;
    s32 remaining;

    remaining = 3;
    destination = (u32 *)(index * 0xa0 + (s32)((GridEntryOwner *)owner)->records + 0x14);
    do {
        remaining = remaining - 1;
        *destination = destination[0x1c];
        destination = destination + 1;
    } while (-1 < remaining);
}

/* Store the two grid position coordinates. */
void itfGridStorePosition(GridPosition *position, s32 x, s32 y) {
    position->x = x;
    position->y = y;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFBA8);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFCE0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFE78);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0038);

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

typedef struct GridAngleOutput {
    u8 pad00[0x24];
    f32 angleDegrees; /* 0x24 */
} GridAngleOutput;

s32 itfUpdateAngleAndGetCycleStep(s32 unused, u8 *out, GridAngleOwner *owner) {
    GridAngleTable *table = owner->slot->table;
    s32 i = 3;

    do {
        if (table->mirrored == 0) {
            ((GridAngleOutput *)out)->angleDegrees = 360.0f - (f32)owner->angle * 360.0f * (1.0f / 65536.0f);
        } else {
            ((GridAngleOutput *)out)->angleDegrees = (f32)owner->angle * 360.0f * (1.0f / 65536.0f);
        }
    } while (--i >= 0);
    return 0x10000 / table->divisor;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0200);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0340);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C04C8);

/* Unpack four 8-bit channels into the low and high halves of two 64-bit words. */
void itfGridUnpackColorChannels(u64 *channels, u32 color) {
    u64 green;
    channels[0] = (color >> 24) | ((u64)((color >> 16) & 0xFF) << 32);
    green = color & 0xFF00;
    channels[1] = (green >> 8) | ((u64)(color & 0xFF) << 32);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0628);

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
    entry = &kwlnDrawSurfaces[kind];
    entry->draw(entry, context);
}

void itfSetPrimaryFramebufferAlphaFlag(u8 value, u32 kind) {
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
    entry = &kwlnDrawSurfaces[kind];
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
    entry = &kwlnDrawSurfaces[kind];
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
    entry = &kwlnDrawSurfaces[kind];
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
    entry = &kwlnDrawSurfaces[kind];
    entry->draw(entry, context);
}

/* Fill all four words with value without assigning a corner or channel order. */
void uiFillQuadColorWords(UiQuadWords *quad, u32 value) {
    quad->unk00[0] = value;
    quad->unk00[1] = value;
    quad->unk00[2] = value;
    quad->unk00[3] = value;
}

void uiDrawUniformRgbRange(u32 a, u32 b, u32 c, u32 value, u32 e, u32 f, u32 g, u32 h) {
    u32 rgb[3] = {value, value, value};
    func_002C0C20(a, b, c, rgb, e, f, g, h);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0C20);

void uiDrawUniformRgbaRange(u32 a, u32 b, u32 c, u32 d, u32 e, u32 value, u32 g, u32 h) {
    u32 rgba[4] = {value, value, value, value};
    func_002C0DF8(a, b, c, d, e, rgba, g, h);
}

void uiDrawUniformColorRect(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    uiDrawUniformRgbaRange(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0DF8);

void uiDrawGradientColorRect(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002C0DF8(a, b, c, d, e, f, 0, g);
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
    func_002C10C0(a, b, c, d, e, f, range, context);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C10C0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1228);

extern s32 kwlnGetDrawBufferIndex(void);
extern u8 kwlnFrameDrawPacketRecords[];
extern void func_002D4C80(const void *, void *, s32);
extern void func_002D4CC8(const void *, void *, s32);
extern void sdfAppendDmaTagToList(void *, void *);

void uiDrawActiveSurfaceRegion(s32 surfaceIndex) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList((s32)list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4C80(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        u8 *surface = (u8 *)kwlnDrawSurfaces + (surfaceIndex << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void sdfDispatchSurfaceWithPreparedTexturePacket(s32 surfaceIndex) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList((s32)list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4CC8(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        u8 *surface = (u8 *)kwlnDrawSurfaces + (surfaceIndex << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void uiDrawTexturedSurfaceAtFarDepth(s32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, surface);
    uiDrawActiveSurfaceRegion(surface);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1548);

void uiDrawSurfaceAtNearDepth(u32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xe00, 0, surface);
}

/* Scale the top three bytes of a packed color by scale / 256; the low byte is kept. */
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

/* Blend two packed 8-bit-channel colors: weight t (mirrored above 0x100) for the first, 0x100 - t for the second. */
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

void func_002C16E0(void) {
}

void func_002C16E8(void) {
}

u32 func_002C16F0(void) {
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
        s32 node = (s32)((GridChildLink *)child)->child;
        if (node != 0) {
            ((GridTextWidget *)node)->flags |= 2;
        }
    }
    return 1;
}

GridTextWidget *itfCreateGridTextWidget(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                               u32 reference) {
    GridTextWidget *widget = (GridTextWidget *)sdfAllocSizeClassBlock(0x40);
    u32 length;
    char *copy;

    memset(widget, 0, 0x40);
    length = strlen(text) + 1;
    copy = (char *)sdfAllocSizeClassBlock(length);
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
        next = func_002C1B30(widget);
    } while (next != 0);
    sdfReleaseChipBlock(widget);
    return 1;
}

typedef struct GridIndexedNode {
    u8 pad00[6];
    u16 key;                     /* 0x06 */
    struct GridScrollBounds *bounds; /* 0x08 */
    f32 position;                /* 0x0C */
    u8 pad10[0xC];
    struct GridIndexedNode *next; /* 0x1C */
    struct GridTextWidget *child; /* 0x20 */
} GridIndexedNode;

typedef struct GridScrollBounds {
    u8 pad00[4];
    f32 start; /* 0x04 */
    f32 end;   /* 0x08 */
    f32 step;  /* 0x0C */
} GridScrollBounds;

typedef struct GridScrollControl {
    u8 pad00[6];
    u16 width;              /* 0x06 */
    u16 skipped;            /* 0x08 */
    s16 count;              /* 0x0A */
    u32 flags;              /* 0x0C */
    GridIndexedNode *cursor; /* 0x10 */
    GridIndexedNode *first;  /* 0x14 */
    GridIndexedNode *selected; /* 0x18 */
    u8 pad1C[0x20];
    s32 keyOffset;          /* 0x3C */
} GridScrollControl;

u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    u32 list;

    sdfReleaseChipBlock(widget->text);
    list = (u32)widget->children;
    if (list != 0) {
        do {
            u32 child = (u32)((GridIndexedNode *)list)->child;
            if (child != 0) {
                itfDestroyGridTextWidgetTree((GridTextWidget *)child);
            }
            list = func_002C1B30(widget);
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

typedef struct GridListNode {
    u8 pad0[0x18];
    void *head;                 /* 0x18 */
    struct GridListNode *next;  /* 0x1C */
} GridListNode;

typedef struct GridListOwner {
    u8 pad00[6];
    u16 count;                  /* 0x06 */
    u8 pad08[8];
    GridListNode *list;         /* 0x10 */
} GridListOwner;

/* Bit 0: the first node has a head. Bit 1: the list has at least `count` links. */
u32 itfGetGridListLinkFlags(GridListOwner *owner) {
    GridListNode *node = owner->list;
    u32 flags;
    s32 i;

    if (node == 0) {
        return 0;
    }
    flags = node->head != 0;
    for (i = 0; i < owner->count; i++) {
        node = node->next;
        if (node == 0) {
            flags &= ~2;
            return flags;
        }
    }
    flags |= 2;
    return flags;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1A30);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1B30);

void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, u8 *node, const char *text) {
    s32 length;
    s32 allocation;
    char *copy;

    sdfReleaseChipBlock(((GridTextWidget *)node)->text);
    length = strlen(text);
    allocation = length + 1;
    copy = (char *)sdfAllocSizeClassBlock(allocation);
    ((GridTextWidget *)node)->textLength = allocation;
    ((GridTextWidget *)node)->text = copy;
    memcpy(copy, text, allocation);
    if (widget != NULL) {
        itfExpandWidgetColumnWidth(length, widget);
    }
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1D78);

void itfAdvanceGridScrollPosition(u32 widget, u32 key, s32 steps) {
    s32 bounds;
    s32 node;
    float *position;
    float increment;
    float previousPosition;

    node = itfFindGridNodeByKey(key, widget);
    bounds = (s32)((GridIndexedNode *)node)->bounds;
    position = &((GridIndexedNode *)node)->position;
    increment = ((GridScrollBounds *)bounds)->step;
    if (1 < steps) {
        increment = increment * (float)(s32)steps;
    }
    previousPosition = *position;
    *position = previousPosition + increment;
    if (((GridScrollBounds *)bounds)->end < previousPosition + increment) {
        *position = ((GridScrollBounds *)bounds)->start;
    }
}

void itfAdvanceSelectedGridScroll(u32 widget, u32 steps) {
    s32 widgetAddress;

    widgetAddress = (s32)widget;
    if ((((GridScrollControl *)widgetAddress)->flags & 1) != 0) {
        itfAdvanceGridScrollPosition(widget, (u32)((GridScrollControl *)widgetAddress)->selected->key + ((GridScrollControl *)widgetAddress)->keyOffset,
                                    steps);
        return;
    }
}

void itfReverseGridScrollPosition(u32 widget, u32 key, s32 steps) {
    s32 bounds;
    s32 node;
    float *position;
    float increment;
    float previousPosition;

    node = itfFindGridNodeByKey(key, widget);
    bounds = (s32)((GridIndexedNode *)node)->bounds;
    position = &((GridIndexedNode *)node)->position;
    increment = ((GridScrollBounds *)bounds)->step;
    if (1 < steps) {
        increment = increment * (float)(s32)steps;
    }
    previousPosition = *position;
    *position = previousPosition - increment;
    if (previousPosition - increment < ((GridScrollBounds *)bounds)->start) {
        *position = ((GridScrollBounds *)bounds)->end;
    }
}

void itfReverseSelectedGridScroll(u32 widget, u32 steps) {
    s32 widgetAddress;

    widgetAddress = (s32)widget;
    if ((((GridScrollControl *)widgetAddress)->flags & 1) != 0) {
        itfReverseGridScrollPosition(widget, (u32)((GridScrollControl *)widgetAddress)->selected->key + ((GridScrollControl *)widgetAddress)->keyOffset,
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
extern s32 func_003014F0(char *, const char *, ...);
extern double fptodp(f32);

void itfFormatGridValueEntryText(GridTextWidget *widget, GridValueEntry *entry, char *out) {
    char text[0x100];
    char prefix[0x100];
    char format[0x100];

    if (entry->format != NULL) {
        entry->format(widget, entry, text, 0x100);
    } else if (entry->kind == NULL) {
        func_003014F0(text, "%s", entry->label);
    } else {
        if (strlen(entry->label) == 0) {
            strcpy(prefix, "");
        } else {
            func_003014F0(prefix, "%s ", entry->label);
        }
        switch (*entry->kind) {
        case 0:
            func_003014F0(format, "%%s%%0%dd", entry->width);
            func_003014F0(text, format, prefix, (s32)entry->value);
            break;
        case 1:
            func_003014F0(format, "%%s0x%%0%dX", entry->width - 2);
            func_003014F0(text, format, prefix, (s32)entry->value);
            break;
        case 2:
            func_003014F0(format, "%%s%%0%d.1f", entry->width);
            func_003014F0(text, format, prefix, (double)entry->value);
            break;
        }
    }
    if (widget->flags & 0x100) {
        s32 row = entry->index + widget->rowOffset;

        if (!(widget->flags & 0x200)) {
            func_003014F0(out, "%03d:%s", row, text);
        } else {
            func_003014F0(out, "0x%03X:%s", row, text);
        }
    } else {
        strcpy(out, text);
    }
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C22F0);

s32 itfFindGridNodeByKey(u32 key, u32 head) {
    u32 n;

    n = (u32)((GridScrollControl *)head)->first;
    while (n != 0 && ((GridIndexedNode *)n)->key != key) {
        n = (u32)((GridIndexedNode *)n)->next;
    }
    return n;
}

s32 sdfGridSeekSelectedNodeByIndex(s32 index, void *w) {
    u8 *widget = (u8 *)w;
    s16 count = ((GridScrollControl *)widget)->count;
    u16 width;
    u8 *first;

    if (index >= count) {
        return 0;
    }
    first = (u8 *)((GridScrollControl *)widget)->first;
    ((GridScrollControl *)widget)->skipped = 0;
    ((GridScrollControl *)widget)->cursor = (GridIndexedNode *)first;
    ((GridScrollControl *)widget)->selected = (GridIndexedNode *)first;
    if (index > 0) {
        width = ((GridScrollControl *)widget)->width;
        do {
            u8 *current = (u8 *)((GridScrollControl *)widget)->cursor;
            if (width >= count - ((GridIndexedNode *)current)->key) {
                ((GridScrollControl *)widget)->skipped++;
            } else {
                ((GridScrollControl *)widget)->cursor = ((GridIndexedNode *)current)->next;
            }
            current = (u8 *)((GridScrollControl *)widget)->selected;
            ((GridScrollControl *)widget)->selected = ((GridIndexedNode *)current)->next;
        } while (--index != 0);
    }
    return 1;
}

void sdfGridSeekFirstNode(u32 widget) {
    sdfGridSeekSelectedNodeByIndex(0, widget);
}

s32 sdfGridSeekLastNode(s32 widget) {
    return sdfGridSeekSelectedNodeByIndex(((GridScrollControl *)widget)->count - 1, (void *)widget);
}

