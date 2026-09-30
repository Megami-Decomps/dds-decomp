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
    u8 pad34[0xC];
} GridTextWidget;

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

extern s32 func_00304AD8();

extern void func_00306BF0(u32, u32, u32, u32, u32, u32, u32, u32);

extern s32 func_00100400(void);

extern u8 D_00381ED0[];

extern void func_0032DB30(const void *, void *, s32);

extern void func_0032CF98(void *, void *);

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

void func_00306F80(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 context) {
    u32 record = func_00304AD8(e, f);
    func_00306BF0(a, b, c, d, e, f, record, context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307018);

s32 itfGridLookupValueOrDefault(s32 object, s32 key) {
    s32 entry = func_00304AD8(object);
    s32 result;

    if (*(s32 *)(entry + 0x30) == 0) {
        func_00307018(object, key);
    }
    result = func_003056B0(object, key, entry);
    if (result == 0) {
        result = *(s32 *)(object + 0x28);
    }
    return result;
}

extern void func_00304B18();

/* Store grid bounds in the renderer's fixed-point coordinate units. */
void func_003071D0(GridEntryStorage *object, s32 index, s32 x, s32 y, s32 width, s32 height) {
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(object->entries + index * 0x80);
    s32 record = func_00304AD8(object, index);

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
    GridWidget *widget = (GridWidget *)func_00304AD8(a, b);
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

void func_00307398(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    sdfCreateDescriptorPacket(arg4, arg0, 0, 0, arg1, arg2, arg3, 0);
}

/* The overlay packet is present only when this work flag is set. */
u8 itfGridGetOverlayFlag(GridDrawWork *work) {
    return work->overlayEnabled;
}

/* Two overlay kinds use a 16x16 region; other kinds use 8x2. */
void func_003073D8(GridDrawWork *work, s32 x, s32 y) {
    s32 width;
    s32 height;
    if (work->overlayKind == 0x13 || work->overlayKind == 0x1B) {
        width = 0x10;
        height = 0x10;
    } else {
        width = 8;
        height = 2;
    }
    func_00307398(work->overlayHandle, width, height, x, y);
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
GridDrawWork *func_00307428(GridDrawWork *object, u8 *data, s32 kind) {
    s32 context = sdfAllocPacketAligned(0x20);
    u8 *cursor;
    RenderCallbackEntry *entry;
    sdfInitPacketList(context);
    cursor = data + (data[1] & 0xF0) + 0x40;
    if (itfGridGetOverlayFlag(object) != 0) {
        func_003073D8(object, (s32)cursor, context);
        cursor += object->overlayDataSize;
    }
    func_00307398(object->packetHandle, object->width,
                  object->height, (s32)cursor, context);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
    return object;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003074F0);

/* Set the selected descriptor word's high control bits. */
void func_003075A0(s32 object, s32 index) {
    s32 descriptor;

    descriptor = *(s32 *)(*(s32 *)(index * 4 + *(s32 *)(object + 0x24)) + 0x28);
    *(u64 *)(descriptor + 0x20) = (*(u64 *)(descriptor + 0x20) & 0x1fffffffffffffff) | 0x4000000000000000;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003075D8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307710);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003078A8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307A68);

s32 func_00307BA8(s32 unused, u8 *out, GridAngleOwner *owner) {
    GridAngleTable *table = owner->slot->table;
    s32 i = 3;

    do {
        if (table->mirrored == 0) {
            *(f32 *)(out + 0x24) = 360.0f - (f32)owner->angle * 360.0f * (1.0f / 65536.0f);
        } else {
            *(f32 *)(out + 0x24) = (f32)owner->angle * 360.0f * (1.0f / 65536.0f);
        }
    } while (--i >= 0);
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
    descriptor = (u64 *)func_0033A2D0(packet);
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

void func_00308288(u8 arg0, u32 arg1) {
    itfGridDrawBooleanDescriptor(arg0, 0, arg1);
}

void func_003082A8(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_0033A2D0(packet);
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

void func_00308380(arg0, arg1)
    u32 arg0;
    u32 arg1;
{
    func_003082A8(arg0, 0, arg1);
}

void func_003083A0(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_0033A2D0(packet);
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

void func_00308478(u32 arg0, u32 arg1) {
    func_003083A0(arg0, 0, arg1);
}

void func_00308498(s32 data, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_0033A2D0(packet);
    descriptor[1] = 0x49;
    descriptor[0] = data;
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void func_00308550(s32 data, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_0033A2D0(packet);
    descriptor[1] = 0x14;
    descriptor[0] = data;
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void func_00308608(QuadU32 *q, u32 value) {
    q->x = value;
    q->y = value;
    q->z = value;
    q->w = value;
}

void func_00308620(u32 a, u32 b, u32 c, u32 value, u32 e, u32 f, u32 g, u32 h) {
    u32 rgb[3] = {value, value, value};
    func_00308650(a, b, c, rgb, e, f, g, h);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308650);

void func_003087D8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 value, u32 g, u32 h) {
    u32 rgba[4] = {value, value, value, value};
    func_00308828(a, b, c, d, e, rgba, g, h);
}

void func_00308808(u32 a, u32 b, u32 c, u32 d, u32 e, u32 value, u32 h) {
    func_003087D8(a, b, c, d, e, value, 0, h);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308828);

void func_003089B8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 colors, u32 h) {
    func_00308828(a, b, c, d, e, colors, 0, h);
}

/* Draw four frame edges; the bottom edge extends 16 units beyond the right side. */
void func_003089D8(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 context) {
    func_00308AC8(x, y, z, x + width, y, z, color, context);
    func_00308AC8(x, y, z, x, y + height, z, color, context);
    func_00308AC8(x + width, y, z, x + width, y + height, z, color, context);
    func_00308AC8(x, y + height, z, x + width + 0x10, y + height, z, color, context);
}

void func_00308AC8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 value, u32 context) {
    u32 range[2] = {value, value};
    func_00308AF0(a, b, c, d, e, f, range, context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308AF0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308C58);

void func_00308DB0(s32 surfaceIndex) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList((s32)list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    func_0032CF98(list, texture);
    {
        u8 *surface = (u8 *)D_0037FB48 + (surfaceIndex << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

extern void func_0032DB78(void *, void *, s32);

void func_00308E60(surfaceIndex)
    s32 surfaceIndex;
{
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList((s32)list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB78(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    func_0032CF98(list, texture);
    {
        u8 *surface = (u8 *)D_0037FB48 + (surfaceIndex << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void func_00308F10(u32 context) {
    func_00308380(0x30000, context);
    func_00308808(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, context);
    func_00308380(0x3000DL, context);
    func_00308DB0(context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308F78);

void func_00308FE8(u32 context) {
    func_00308380(0x30000, context);
    func_00308808(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, context);
    func_00308380(0x3001BL, context);
    func_00308DB0(context);
}

void func_00309050(s32 context) {
    func_00308E60();
    func_00308380(0x5001BL, context);
    func_00308478(0x44, context);
}

void func_00309090(u32 arg0) {
    func_00308380(0x30000, arg0);
    func_00308478(0x44, arg0);
    func_00308808(0, 0, 0, 0x2000, 0xe00, 0, arg0);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003090E8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309138);

void func_003091E8(void) {
}

void func_003091F0(void) {
}

u32 func_003091F8(void) {
    return 0;
}

s32 func_00309200(s32 arg0) {
    if (arg0 == 0) {
        return 0;
    }
    *(s32 *)(arg0 + 0xC) = (*(s32 *)(arg0 + 0xC) & -2) | 2;
    return 1;
}

s32 itfSetWidgetFlagsAndActivateChild(u8 *object, u32 flags) {
    s32 child;
    if (object == 0) {
        return 0;
    }
    *(u32 *)(object + 0xc) = (*(u32 *)(object + 0xc) & ~2) | flags;
    child = *(s32 *)(object + 0x18);
    if (child != 0) {
        s32 node = *(s32 *)(child + 0x20);
        if (node != 0) {
            *(u32 *)(node + 0xc) |= 2;
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

    func_00328E48(widget->text);
    do {
        next = func_00309638(widget);
    } while (next != 0);
    func_00328E48(widget);
    return 1;
}

u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    u32 list;

    func_00328E48(widget->text);
    list = (u32)widget->children;
    if (list != 0) {
        do {
            u32 child = *(u32 *)(list + 0x20);
            if (child != 0) {
                itfDestroyGridTextWidgetTree((GridTextWidget *)child);
            }
            list = func_00309638(widget);
        } while (list != 0);
    }
    func_00328E48(widget);
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

INCLUDE_ASM(const s32, "game/code_00306F80", func_003094C8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309538);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309638);

void func_003097D0(GridTextWidget *widget, u8 *node, const char *text) {
    s32 length;
    s32 allocation;
    char *copy;

    func_00328E48(*(u32 *)node);
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

void func_00309A90(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        itfAdvanceGridScrollPosition(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
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

void func_00309B48(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        itfReverseGridScrollPosition(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

s32 func_00309B90(u8 *widget, u32 target) {
    u32 flags = *(u32 *)(widget + 0xC);

    if (flags & 2) {
        if (target == *(u32 *)(widget + 0x18)) {
            return (flags & 1) ? 6 : 4;
        }
        return 0;
    }
    if (flags & 0x80) {
        if (target == *(u32 *)(widget + 0x18)) {
            return 12;
        }
    } else if (target == *(u32 *)(widget + 0x18) && (flags & 1)) {
        return 6;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309C00);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309DF8);

s32 itfFindGridNodeByKey(u32 key, u32 head) {
    u32 n;

    n = *(u32 *)(head + 0x14);
    while (n != 0 && *(u16 *)(n + 6) != key) {
        n = *(u32 *)(n + 0x1C);
    }
    return n;
}

s32 func_0030A070(s32 index, u8 *widget) {
    s16 count = *(s16 *)(widget + 0xA);
    u16 width;
    u8 *first;

    if (index >= count) {
        return 0;
    }
    first = *(u8 **)(widget + 0x14);
    *(u16 *)(widget + 8) = 0;
    *(u8 **)(widget + 0x10) = first;
    *(u8 **)(widget + 0x18) = first;
    if (index > 0) {
        width = *(u16 *)(widget + 6);
        do {
            u8 *current = *(u8 **)(widget + 0x10);
            if (width >= count - *(u16 *)(current + 6)) {
                (*(u16 *)(widget + 8))++;
            } else {
                *(u8 **)(widget + 0x10) = *(u8 **)(current + 0x1C);
            }
            current = *(u8 **)(widget + 0x18);
            *(u8 **)(widget + 0x18) = *(u8 **)(current + 0x1C);
        } while (--index != 0);
    }
    return 1;
}

void func_0030A0E8(u32 arg0) {
    func_0030A070(0, arg0);
}

void func_0030A108(u8 *entry) {
    func_0030A070(*(s16 *)(entry + 0xA) - 1, entry);
}

INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438868);

INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438870);

INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438878);

INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438880);

