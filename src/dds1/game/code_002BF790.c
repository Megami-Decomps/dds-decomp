#include "common.h"

extern s32 func_002C2540(u32, u32);

extern s32 func_002C1B30(u32);

typedef struct IntPair {
    s32 x; // 0x00
    s32 y; // 0x04
} IntPair; // 0x08

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
    u8 pad34[0xC];
} GridTextWidget;

extern s32 func_002C2568(s32, void *);

typedef struct GridQuantizedEntry {
    u8 pad0[0x44];
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} GridQuantizedEntry;

typedef struct RenderCallbackEntry {
    u8 reserved[0x10];
    void (*draw)(void *, s32);
    u8 tail[0xC];
} RenderCallbackEntry;

extern RenderCallbackEntry D_00324B48[];

extern s32 sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(s32);

void func_002BF790(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    s32 entry = func_002BD398(e, f);
    func_002BF400(a, b, c, d, e, f, entry, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BF828);

s32 func_002BF970(s32 object, s32 key) {
    s32 entry = func_002BD398(object);
    s32 result;

    if (*(s32 *)(entry + 0x30) == 0) {
        func_002BF828(object, key);
    }
    result = func_002BDF28(object, key, entry);
    if (result == 0) {
        result = *(s32 *)(object + 0x28);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BF9E0);

void itfGridSetQuantizedBounds(u8 *object, s32 index, s32 x, s32 y,
                   s32 width, s32 height) {
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(*(u8 **)(object + 0x10) + index * 0x80);
    u32 *destination = (u32 *)(*(u8 **)(object + 0x18) + index * 0xA0 + 0x6C);
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
    GridWidget *widget = (GridWidget *)func_002BD398(a, b);
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
}

void itfCopyGridEntryWords(s32 owner, s32 index) {
    u32 *destination;
    s32 remaining;

    remaining = 3;
    destination = (u32 *)(index * 0xa0 + *(s32 *)(owner + 0x18) + 0x14);
    do {
        remaining = remaining - 1;
        *destination = destination[0x1c];
        destination = destination + 1;
    } while (-1 < remaining);
}

void func_002BFB98(IntPair *p, s32 a, s32 b) {
    p->x = a;
    p->y = b;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFBA8);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFCE0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFE78);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0038);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0178);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0200);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0340);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C04C8);

void func_002C05F0(u64 *channels, u32 color) {
    u64 green;
    channels[0] = (color >> 24) | ((u64)((color >> 16) & 0xFF) << 32);
    green = color & 0xFF00;
    channels[1] = (green >> 8) | ((u64)(color & 0xFF) << 32);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0628);

void func_002C0778(u8 value, s32 alternate, s32 kind) {
    u32 normalized = value != 0;
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    func_002E1428(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_002E1420(packet);
    descriptor[0] = normalized;
    if (!alternate) {
        descriptor[1] = 0x4A;
    } else {
        descriptor[1] = 0x4B;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_00324B48[kind];
    entry->draw(entry, context);
}

void func_002C0858(u8 arg0, u32 arg1) {
    func_002C0778(arg0, 0, arg1);
}

void func_002C0878(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    func_002E1428(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_002E1420(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x47;
    } else {
        descriptor[1] = 0x48;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_00324B48[kind];
    entry->draw(entry, context);
}

void func_002C0950(u32 arg0, u32 arg1) {
    func_002C0878(arg0, 0, arg1);
}

void func_002C0970(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    func_002E1428(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_002E1420(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x42;
    } else {
        descriptor[1] = 0x43;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_00324B48[kind];
    entry->draw(entry, context);
}

void func_002C0A48(u32 arg0, u32 arg1) {
    func_002C0970(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0A68);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0B20);

void func_002C0BD8(QuadU32 *q, u32 value) {
    q->x = value;
    q->y = value;
    q->z = value;
    q->w = value;
}

void func_002C0BF0(u32 a, u32 b, u32 c, u32 value, u32 e, u32 f, u32 g, u32 h) {
    u32 rgb[3] = {value, value, value};
    func_002C0C20(a, b, c, rgb, e, f, g, h);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0C20);

void func_002C0DA8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 value, u32 g, u32 h) {
    u32 rgba[4] = {value, value, value, value};
    func_002C0DF8(a, b, c, d, e, rgba, g, h);
}

void func_002C0DD8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002C0DA8(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0DF8);

void func_002C0F88(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002C0DF8(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0FA8);

void func_002C1098(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 value) {
    u32 range[2] = {value, value};
    func_002C10C0(a, b, c, d, e, f, range);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C10C0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1228);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1380);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1430);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C14E0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1548);

void func_002C1588(u32 arg0) {
    func_002C0950(0x30000, arg0);
    func_002C0A48(0x44, arg0);
    func_002C0DD8(0, 0, 0, 0x2000, 0xe00, 0, arg0);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C15E0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1630);

void func_002C16E0(void) {
}

void func_002C16E8(void) {
}

u32 func_002C16F0(void) {
    return 0;
}

s32 func_002C16F8(s32 arg0) {
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
    GridTextWidget *widget = (GridTextWidget *)func_002CFEB8(0x40);
    u32 length;
    char *copy;

    memset(widget, 0, 0x40);
    length = strlen(text) + 1;
    copy = (char *)func_002CFEB8(length);
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

    func_002CFF98(widget->text);
    do {
        next = func_002C1B30(widget);
    } while (next != 0);
    func_002CFF98(widget);
    return 1;
}

u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    u32 list;

    func_002CFF98(widget->text);
    list = (u32)widget->children;
    if (list != 0) {
        do {
            u32 child = *(u32 *)(list + 0x20);
            if (child != 0) {
                itfDestroyGridTextWidgetTree((GridTextWidget *)child);
            }
            list = func_002C1B30(widget);
        } while (list != 0);
    }
    func_002CFF98(widget);
    return 1;
}

void itfExpandGridWidgetColumnWidth(s32 columns, GridTextWidget *work) {
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

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C19C0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1A30);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1B30);

void func_002C1CC8(GridTextWidget *widget, u8 *node, const char *text) {
    s32 length;
    s32 allocation;
    char *copy;

    func_002CFF98(*(u32 *)node);
    length = strlen(text);
    allocation = length + 1;
    copy = (char *)func_002CFEB8(allocation);
    *(u16 *)(node + 4) = allocation;
    *(char **)node = copy;
    memcpy(copy, text, allocation);
    if (widget != NULL) {
        itfExpandGridWidgetColumnWidth(length, widget);
    }
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1D78);

void func_002C1F18(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_002C2540(arg1, arg0);
    temp_v0 = *(s32 *)(temp_v1 + 8);
    pfVar3 = (float *)(temp_v1 + 0xc);
    temp_v2 = *(float *)(temp_v0 + 0xc);
    if (1 < arg2) {
        temp_v2 = temp_v2 * (float)(s32)arg2;
    }
    temp_v3 = *pfVar3;
    *pfVar3 = temp_v3 + temp_v2;
    if (*(float *)(temp_v0 + 8) < temp_v3 + temp_v2) {
        *pfVar3 = *(float *)(temp_v0 + 4);
    }
}

void func_002C1F88(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_002C1F18(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

void func_002C1FD0(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_002C2540(arg1, arg0);
    temp_v0 = *(s32 *)(temp_v1 + 8);
    pfVar3 = (float *)(temp_v1 + 0xc);
    temp_v2 = *(float *)(temp_v0 + 0xc);
    if (1 < arg2) {
        temp_v2 = temp_v2 * (float)(s32)arg2;
    }
    temp_v3 = *pfVar3;
    *pfVar3 = temp_v3 - temp_v2;
    if (temp_v3 - temp_v2 < *(float *)(temp_v0 + 4)) {
        *pfVar3 = *(float *)(temp_v0 + 8);
    }
}

void func_002C2040(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_002C1FD0(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

s32 func_002C2088(u8 *widget, u32 target) {
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

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C20F8);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C22F0);

s32 func_002C2540(u32 key, u32 head) {
    u32 n;

    n = *(u32 *)(head + 0x14);
    while (n != 0 && *(u16 *)(n + 6) != key) {
        n = *(u32 *)(n + 0x1C);
    }
    return n;
}

s32 func_002C2568(s32 index, void *w) {
    u8 *widget = (u8 *)w;
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

void func_002C25E0(u32 arg0) {
    func_002C2568(0, arg0);
}

s32 func_002C2600(s32 arg0) {
    return func_002C2568(*(s16 *)(arg0 + 0xa) - 1, (void *)arg0);
}

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD218);

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD220);

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD228);

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD230);

