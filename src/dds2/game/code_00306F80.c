#include "common.h"

extern s32 func_00309638(u32);

extern s32 func_0030A048(u32, u32);

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

extern s32 func_00304AD8();
extern void func_00306BF0(u32, u32, u32, u32, u32, u32, u32, u32);

void func_00306F80(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 context) {
    u32 record = func_00304AD8(e, f);
    func_00306BF0(a, b, c, d, e, f, record, context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307018);

s32 func_00307160(s32 object, s32 key) {
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

INCLUDE_ASM(const s32, "game/code_00306F80", func_003071D0);

void func_00307270(u8 *object, s32 index, s32 x, s32 y,
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
    GridWidget *widget = (GridWidget *)func_00304AD8(a, b);
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
}

void func_00307340(s32 object, s32 index) {
    u32 *destination;
    s32 remaining;

    remaining = 3;
    destination = (u32 *)(index * 0xa0 + *(s32 *)(object + 0x18) + 0x14);
    do {
        remaining = remaining - 1;
        *destination = destination[0x1c];
        destination = destination + 1;
    } while (-1 < remaining);
}

void func_00307388(s32 *position, s32 x, s32 y) {
    position[0] = x;
    position[1] = y;
}

void func_00307398(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    sdfCreateDescriptorPacket(arg4, arg0, 0, 0, arg1, arg2, arg3, 0);
}

u8 func_003073D0(s32 arg0) {
    return *(u8 *)(arg0 + 0x18);
}

void func_003073D8(u8 *work, s32 x, s32 y) {
    s32 width;
    s32 height;
    if (work[0x1A] == 0x13 || work[0x1A] == 0x1B) {
        width = 0x10;
        height = 0x10;
    } else {
        width = 8;
        height = 2;
    }
    func_00307398(*(u32 *)(work + 0x14), width, height, x, y);
}

typedef struct RenderCallbackEntry {
    u8 reserved[0x10];
    void (*draw)(void *, s32);
    u8 tail[0xC];
} RenderCallbackEntry;

extern RenderCallbackEntry D_0037FB48[];
extern s32 sdfAllocPacketAligned(s32);
extern void sdfResetPacketList(s32);

u8 *func_00307428(u8 *object, u8 *data, s32 kind) {
    s32 context = sdfAllocPacketAligned(0x20);
    u8 *cursor;
    RenderCallbackEntry *entry;
    sdfResetPacketList(context);
    cursor = data + (data[1] & 0xF0) + 0x40;
    if (func_003073D0((s32)object) != 0) {
        func_003073D8(object, (s32)cursor, context);
        cursor += *(s32 *)(object + 0x34);
    }
    func_00307398(*(u32 *)(object + 0x10), *(s16 *)(object + 0xC),
                  *(s16 *)(object + 0xE), (s32)cursor, context);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
    return object;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003074F0);

void func_003075A0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg1 * 4 + *(s32 *)(arg0 + 0x24)) + 0x28);
    *(u64 *)(temp_v0 + 0x20) = (*(u64 *)(temp_v0 + 0x20) & 0x1fffffffffffffff) | 0x4000000000000000;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003075D8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307710);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003078A8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307A68);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307BA8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307C30);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307D70);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307EF8);

void func_00308020(u64 *channels, u32 color) {
    u64 green;
    channels[0] = (color >> 24) | ((u64)((color >> 16) & 0xFF) << 32);
    green = color & 0xFF00;
    channels[1] = (green >> 8) | ((u64)(color & 0xFF) << 32);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308058);

void func_003081A8(u8 value, s32 alternate, s32 kind) {
    u32 normalized = value != 0;
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    func_0033A2D8(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_0033A2D0(packet);
    descriptor[0] = normalized;
    if (!alternate) {
        descriptor[1] = 0x4A;
    } else {
        descriptor[1] = 0x4B;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void func_00308288(u8 arg0, u32 arg1) {
    func_003081A8(arg0, 0, arg1);
}

void func_003082A8(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    func_0033A2D8(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_0033A2D0(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x47;
    } else {
        descriptor[1] = 0x48;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void func_00308380(u32 arg0, u32 arg1) {
    func_003082A8(arg0, 0, arg1);
}

void func_003083A0(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    s32 context;
    RenderCallbackEntry *entry;

    func_0033A2D8(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)func_0033A2D0(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x42;
    } else {
        descriptor[1] = 0x43;
    }
    context = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(context);
    sdfAppendPacket(context, packet);
    entry = &D_0037FB48[kind];
    entry->draw(entry, context);
}

void func_00308478(u32 arg0, u32 arg1) {
    func_003083A0(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308498);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308550);

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

INCLUDE_ASM(const s32, "game/code_00306F80", func_003089D8);

void func_00308AC8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 value) {
    u32 range[2] = {value, value};
    func_00308AF0(a, b, c, d, e, f, range);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308AF0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308C58);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308DB0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308E60);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308F10);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308F78);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308FE8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309050);

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

u8 *func_00309278(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                  u32 reference) {
    u8 *widget = (u8 *)func_00328D68(0x40);
    u32 length;
    char *copy;

    memset(widget, 0, 0x40);
    length = strlen(text) + 1;
    copy = (char *)func_00328D68(length);
    *(u16 *)(widget + 4) = length;
    *(char **)widget = copy;
    memcpy(copy, text, length);
    *(u32 *)(widget + 0x10) = 0;
    *(s32 *)(widget + 0x20) = x << 4;
    *(s32 *)(widget + 0x24) = y << 3;
    *(u32 *)(widget + 0x30) = reference;
    *(s16 *)(widget + 6) = rows;
    *(s32 *)(widget + 0x28) = columns * 12 + 6;
    *(s32 *)(widget + 0x2C) = rows * 14 + 6;
    *(u32 *)(widget + 0x14) = 0;
    *(u32 *)(widget + 0x18) = 0;
    *(u32 *)(widget + 0x1C) = 0;
    *(u32 *)(widget + 0xC) = 0;
    *(u16 *)(widget + 8) = 0;
    *(u16 *)(widget + 0xA) = 0;
    return widget;
}

void itfSetGridDimensions(u8 *work, s32 columns, s32 rows) {
    s32 columnWidth = columns * 12 + 6;
    s32 rowHeight = rows * 14 + 6;

    if (columns != 0) {
        *(s32 *)(work + 0x28) = columnWidth;
    }
    if (rows != 0) {
        *(s32 *)(work + 0x2c) = rowHeight;
        *(s16 *)(work + 6) = rows;
    }
}

u32 func_003093D0(u32 arg0) {
    s64 temp_v0;

    func_00328E48(*(u32 *)arg0);
    do {
        temp_v0 = func_00309638(arg0);
    } while (temp_v0 != 0);
    func_00328E48(arg0);
    return 1;
}

u32 func_00309418(u32 work) {
    u32 list;

    func_00328E48(*(u32 *)work);
    list = *(u32 *)(work + 0x18);
    if (list != 0) {
        do {
            u32 child = *(u32 *)(list + 0x20);
            if (child != 0) {
                func_00309418(child);
            }
            list = func_00309638(work);
        } while (list != 0);
    }
    func_00328E48(work);
    return 1;
}

void expandWidgetColumnWidth(s32 columns, u8 *work) {
    s32 flags = *(s32 *)(work + 0xc);
    s32 width;
    if (flags & 0x100) {
        columns += 4;
        if (flags & 0x200) {
            columns += 2;
        }
    }
    width = columns * 12 + 6;
    if (*(s32 *)(work + 0x28) < width) {
        *(s32 *)(work + 0x28) = width;
    }
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003094C8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309538);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309638);

void func_003097D0(u8 *widget, u8 *node, const char *text) {
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
        expandWidgetColumnWidth(length, widget);
    }
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309880);

void func_00309A20(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_0030A048(arg1, arg0);
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

void func_00309A90(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_00309A20(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

void func_00309AD8(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_0030A048(arg1, arg0);
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

void func_00309B48(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_00309AD8(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
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

s32 func_0030A048(u32 key, u32 head) {
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

