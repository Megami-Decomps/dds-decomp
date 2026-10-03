#include "common.h"
#include "fpu.h"

extern s32 func_00309638(u32);

extern s32 itfFindGridNodeByKey(u32, u32);

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

typedef struct GridQuantizedEntry {
    u8 pad0[0x44];
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} GridQuantizedEntry;

typedef struct GridEntryStorage {
    u8 pad00[0x10];
    u8 *quantizedEntries; /* 0x10: 0x80 bytes per entry */
    u8 pad14[4];
    u8 *renderEntries;     /* 0x18: 0xA0 bytes per entry */
} GridEntryStorage;

typedef struct GridScrollRange {
    u32 reserved;
    float minimum;        /* 0x04 */
    float maximum;        /* 0x08 */
    float step;           /* 0x0C */
} GridScrollRange;

typedef struct GridNumericDescriptor {
    s32 mode;
    f32 minimum;
    f32 maximum;
    f32 step;
    f32 value;
} GridNumericDescriptor;

typedef struct GridTextListItem GridTextListItem;

/* Native 0x40-byte text/list widget. Navigation and child layout share these links. */
typedef struct GridTextWidget {
    char *text;           /* 0x00 */
    u16 textLength;       /* 0x04 */
    s16 rows;             /* 0x06 */
    u16 cursorRow;       /* 0x08: selected row within the visible window */
    s16 itemCount;       /* 0x0A */
    u32 flags;            /* 0x0C */
    GridTextListItem *firstVisible; /* 0x10 */
    GridTextListItem *head;         /* 0x14 */
    GridTextListItem *selected;     /* 0x18 */
    GridTextListItem *tail;         /* 0x1C */
    s32 x;                /* 0x20 */
    s32 y;                /* 0x24 */
    s32 width;            /* 0x28 */
    s32 height;           /* 0x2C */
    u32 reference;        /* 0x30 */
    u8 pad34[8];
    s32 rowOffset;        /* 0x3C */
} GridTextWidget;

/* Native 0x2C-byte list item. parameter points to a range or numeric kind
 * for value rows; plain text rows leave it NULL. */
struct GridTextListItem {
    char *text;
    u16 textLength;
    u16 index;
    void *parameter; /* 0x08 */
    f32 number;      /* 0x0C: numeric value or scroll position */
    s32 formatWidth; /* 0x10 */
    u32 value;       /* 0x14: caller-supplied value for a plain text item */
    struct GridTextListItem *previous;
    struct GridTextListItem *next;
    GridTextWidget *child; /* 0x20 */
    void (*select)(GridTextWidget *); /* 0x24 */
    void (*format)(void *, void *, char *, s32); /* 0x28 */
};

/* Result bit 1: at least widget->rows successors remain from firstVisible. */
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

extern s32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_0032DB30(const void *, void *, s32);

extern void sdfAppendDmaTagToList(void *, void *);

typedef struct GridAngleTable {
    s32 divisor;      /* 0x00 */
    s32 mirrored;     /* 0x04 */
    s32 cycleDivisor; /* 0x08 */
} GridAngleTable;

typedef struct GridFlushTable {
    s32 fadeOutDuration; /* 0x00 */
    s32 holdDuration;    /* 0x04 */
    s32 fadeInDuration;  /* 0x08 */
} GridFlushTable;

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

/* Resolve the indexed render entry before applying position, depth, and draw flags. */
void itfDrawGridWithResolvedSlot(u32 offsetX, u32 offsetY, u32 z, u32 drawFlags, u32 object, u32 index, u32 surfaceIndex) {
    u32 renderEntry = effGetSlotWorkOrOverride(object, index);
    func_00306BF0(offsetX, offsetY, z, drawFlags, object, index, renderEntry, surfaceIndex);
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
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(object->quantizedEntries + index * 0x80);
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
    GridQuantizedEntry *entry = (GridQuantizedEntry *)(object->quantizedEntries + index * 0x80);
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
void itfGridSetBounds(s32 object, s32 index, s32 x, s32 y, s32 width, s32 height) {
    GridWidget *widget = (GridWidget *)effGetSlotWorkOrOverride(object, index);
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

/* Store the two grid position coordinates. */
void itfGridStorePosition(GridPosition *position, s32 x, s32 y) {
    position->x = x;
    position->y = y;
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

extern RenderCallbackEntry kwlnDrawSurfaces[];

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
    entry = &kwlnDrawSurfaces[kind];
    entry->draw(entry, context);
    return object;
}

typedef struct ItfGridResource {
    u32 handle;
    u8 pad04[0x18];
    u32 entryCount;
} ItfGridResource;

extern u8 *sdfResourceRetainAddress(u32);
extern void sdfDecrementAllocationReferenceCount(u32);

/* Return the address of entry `index` of the resource's 8-byte-stride offset table, or NULL when out of range. */
u8 *itfGetGridResourceEntryData(ItfGridResource *object, u32 index) {
    u8 *base;
    u8 *cursor;
    u8 *result;
    u32 count;
    u32 i;

    if (object->handle == 0) {
        return 0;
    }
    cursor = sdfResourceRetainAddress(object->handle);
    base = cursor;
    count = object->entryCount;
    cursor += *(u32 *)(cursor + 0xC);
    for (i = 0; i < count; i++, cursor += 8) {
        result = base + *(u32 *)(cursor + 4);
        if (index == i) {
            sdfDecrementAllocationReferenceCount(object->handle);
            return result;
        }
    }
    sdfDecrementAllocationReferenceCount(object->handle);
    return 0;
}

/* Set the selected descriptor word's high control bits. */
void itfSetGridDescriptorControlBit(s32 object, s32 index) {
    s32 descriptor;

    descriptor = *(s32 *)(*(s32 *)(index * 4 + *(s32 *)(object + 0x24)) + 0x28);
    *(u64 *)(descriptor + 0x20) = (*(u64 *)(descriptor + 0x20) & 0x1fffffffffffffff) | 0x4000000000000000;
}

typedef struct GridAngleRectangle {
    u8 pad00[0x6C];
    s32 left;       /* 0x6C */
    s32 top;        /* 0x70 */
    s32 right;      /* 0x74 */
    s32 bottom;     /* 0x78 */
    s32 ratioWidth;  /* 0x7C */
    s32 ratioHeight; /* 0x80 */
    u32 colors[4];  /* 0x84 */
} GridAngleRectangle;

typedef struct GridAngleAdjustment {
    u8 pad00[4];
    s32 dimensions[2]; /* 0x04 */
    s32 anchors[2];    /* 0x0C */
    u32 colors[4];     /* 0x14 */
} GridAngleAdjustment;

/* Apply the MOVE_01 easing to the bounds and packed colors, then return its cycle step. */
s32 itfGridApplySqrtBoundsAndColorScale(GridAngleRectangle *rectangle, GridAngleAdjustment *out, GridAngleOwner *owner) {
    GridAngleTable *table = owner->slot->table;
    s32 deltas[2];
    s32 *dimensionOut = (s32 *)((u8 *)out + 4);
    u32 *sourceColor;
    u32 *destColor;
    s32 colorFactor;
    s32 i = 0;

    deltas[0] = (rectangle->right - rectangle->left) << 4;
    deltas[1] = (rectangle->bottom - rectangle->top) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 scaled = (s32)(fsqrtf((f32)delta) * (f32)owner->angle * (1.0f / 65536.0f));

        if (delta > 0) {
            dimensionOut[i] = delta - scaled * scaled;
        } else {
            dimensionOut[i] = scaled * scaled + delta;
        }
    }

    if (table->mirrored != 0) {
        colorFactor = 0x10000 - owner->angle;
    } else {
        colorFactor = owner->angle;
    }
    sourceColor = rectangle->colors;
    destColor = (u32 *)((u8 *)dimensionOut + 0x10);
    {
        s32 colorMask = -0x100;
        s32 fractionalMask = 0xFFFF;

        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            u32 color = *sourceColor;
            s32 lowByte = *(u8 *)sourceColor;
            s32 product = lowByte * colorFactor;
            s32 negative = 0;

            /* Signed fixed-point division rounds toward zero. */
            if (product < 0) {
                negative++;
            }
            *destColor = (color & colorMask) |
                         ((product + negative * fractionalMask) >> 16);
        }
    }
    return 0x10000 / table->divisor;
}

/* Apply the ZOOM_01 easing to the adjustment bounds and fade their alpha. */
s32 func_00307710(GridAngleRectangle *rectangle, GridAngleAdjustment *out, GridAngleOwner *owner) {
    GridAngleTable *table;
    s32 squares[2];
    s32 deltas[2];
    s32 previous[2];
    s32 scaledWidth;
    s32 scaledHeight;
    s32 widthAdjustment;
    s32 heightAdjustment;
    u32 *sourceColor;
    u32 *destColor;
    s32 colorMask;
    s32 fractionalMask;
    s32 i;

    table = owner->slot->table;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->ratioHeight) / rectangle->ratioWidth;
    previous[0] = out->dimensions[0];
    previous[1] = out->dimensions[1];
    scaledWidth = (s32)(fsqrtf((f32)deltas[0]) * (f32)owner->angle * (1.0f / 65536.0f));
    squares[0] = scaledWidth * scaledWidth;
    widthAdjustment = -((deltas[0] - squares[0]) / 2);
    scaledHeight = (s32)(fsqrtf((f32)deltas[1]) * (f32)owner->angle * (1.0f / 65536.0f));
    squares[1] = scaledHeight * scaledHeight;
    heightAdjustment = -((deltas[1] - squares[1]) / 2);

    out->dimensions[0] = widthAdjustment;
    out->anchors[0] += (previous[0] - widthAdjustment) * 2;
    out->dimensions[1] = heightAdjustment;
    out->anchors[1] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->colors;
    destColor = out->colors;
    i = 3;

    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 product = alpha * owner->angle;
        s32 negative = 0;

        if (product < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((product + negative * fractionalMask) >> 16);
    }
    return 0x10000 / table->cycleDivisor;
}

/* Apply the FLUSH_01 three-phase color fade while contracting the grid bounds. */
s32 func_003078A8(GridAngleRectangle *rectangle, GridAngleAdjustment *out, GridAngleOwner *owner) {
    GridFlushTable *table;
    s32 phases[3];
    s32 deltas[2];
    s32 *dimensionOut = (s32 *)((u8 *)out + 4);
    s32 totalDuration;
    s32 numerator;
    s32 denominator;
    u32 *sourceColor;
    u32 *destColor;
    s32 i = 0;

    table = (GridFlushTable *)owner->slot->table;
    totalDuration = table->fadeOutDuration;
    totalDuration += table->fadeInDuration;
    totalDuration += table->holdDuration;
    deltas[0] = (rectangle->right - rectangle->left) << 4;
    deltas[1] = (rectangle->bottom - rectangle->top) << 3;
    {
        s32 fractionalMask = 0xFFFF;
        for (; i < 2; i++) {
            s32 delta = deltas[i];
            s32 magnitude = delta < 0 ? -delta : delta;
            s32 product = magnitude * owner->angle;
            s32 negative = 0;
            s32 scaled;

            if (product < 0) {
                negative++;
            }
            /* Signed fixed-point division rounds toward zero. */
            scaled = (product + negative * fractionalMask) >> 16;
            if (delta > 0) {
                dimensionOut[i] = delta - scaled;
            } else {
                dimensionOut[i] = delta + scaled;
            }
        }
    }

    phases[0] = (table->fadeOutDuration << 16) / totalDuration;
    phases[1] = (table->holdDuration << 16) / totalDuration;
    phases[2] = (table->fadeInDuration << 16) / totalDuration;

    if (phases[1] + phases[2] < owner->angle) {
        numerator = phases[0] - (owner->angle - (phases[1] + phases[2]));
        denominator = phases[0];
    } else if (phases[2] < owner->angle) {
        destColor = (u32 *)((u8 *)dimensionOut + 0x10);
        sourceColor = rectangle->colors;
        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            *destColor = *sourceColor;
        }
        return 0x10000 / totalDuration;
    } else {
        numerator = owner->angle;
        denominator = phases[2];
    }

    destColor = (u32 *)((u8 *)dimensionOut + 0x10);
    sourceColor = rectangle->colors;
    {
        s32 colorMask = -0x100;

        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            u32 color = *sourceColor;
            s32 lowByte = *(u8 *)sourceColor;

            *destColor = (color & colorMask) | (lowByte * numerator / denominator);
        }
    }
    return 0x10000 / totalDuration;
}

/* Contract the grid bounds and fade each packed color's low byte as the angle advances. */
s32 func_00307A68(GridAngleRectangle *rectangle, GridAngleAdjustment *out, GridAngleOwner *owner) {
    GridAngleTable *table = owner->slot->table;
    s32 deltas[2];
    s32 *dimensionOut = (s32 *)((u8 *)out + 4);
    s32 factor;
    u32 *sourceColor;
    u32 *destColor;
    s32 i = 0;

    deltas[0] = (rectangle->right - rectangle->left) << 4;
    deltas[1] = (rectangle->bottom - rectangle->top) << 3;
    {
        s32 fractionalMask = 0xFFFF;
        for (; i < 2; i++) {
            s32 delta = deltas[i];
            s32 magnitude = delta < 0 ? -delta : delta;
            s32 product = magnitude * owner->angle;
            s32 negative = 0;
            s32 scaled;

            /* Signed fixed-point division rounds toward zero. */
            if (product < 0) {
                negative++;
            }
            scaled = (product + negative * fractionalMask) >> 16;

            if (delta > 0) {
                dimensionOut[i] = delta - scaled;
            } else {
                dimensionOut[i] = delta + scaled;
            }
        }
    }

    destColor = (u32 *)((u8 *)dimensionOut + 0x10);
    factor = ((100 - table->mirrored) << 16) / 100;
    sourceColor = rectangle->colors;
    {
        s32 colorMask = -0x100;

        for (i = 3; i >= 0; i--, destColor++, sourceColor++) {
            if (owner->angle < factor) {
                u32 color = *sourceColor;
                s32 lowByte = *(u8 *)sourceColor;

                *destColor = (color & colorMask) | (lowByte * owner->angle / factor);
            } else {
                *destColor = *sourceColor;
            }
        }
    }
    return 0x10000 / table->divisor;
}

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

/* Apply linear ZOOM easing to the adjustment bounds and fade their alpha. */
s32 func_00307D70(GridAngleRectangle *rectangle, GridAngleAdjustment *out, GridAngleOwner *owner) {
    GridAngleTable *table;
    s32 scaled[2];
    s32 deltas[2];
    s32 previous[2];
    s32 scaledWidth;
    s32 scaledHeight;
    s32 widthAdjustment;
    s32 heightAdjustment;
    s32 angle;
    u32 *sourceColor;
    u32 *destColor;
    s32 colorMask;
    s32 fractionalMask;
    s32 i;

    table = owner->slot->table;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->ratioHeight) / rectangle->ratioWidth;
    angle = owner->angle;
    previous[0] = out->dimensions[0];
    previous[1] = out->dimensions[1];
    scaledWidth = deltas[0] * angle;
    if (scaledWidth < 0) {
        scaledWidth += 0xFFFF;
    }
    scaled[0] = scaledWidth >> 16;
    widthAdjustment = -((deltas[0] - scaled[0]) / 2);
    out->dimensions[0] = widthAdjustment;
    out->anchors[0] += (previous[0] - widthAdjustment) * 2;

    scaledHeight = deltas[1] * angle;
    if (scaledHeight < 0) {
        scaledHeight += 0xFFFF;
    }
    scaled[1] = scaledHeight >> 16;
    heightAdjustment = -((deltas[1] - scaled[1]) / 2);
    out->dimensions[1] = heightAdjustment;
    out->anchors[1] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->colors;
    destColor = out->colors;
    i = 3;
    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 colorProduct = alpha * owner->angle;
        s32 negative = 0;

        if (colorProduct < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((colorProduct + negative * fractionalMask) >> 16);
    }
    return 0x10000 / table->cycleDivisor;
}

/* Apply the angle-driven grid contraction and mirrored packed-color fade. */
s32 func_00307EF8(GridAngleRectangle *rectangle, GridAngleAdjustment *out, GridAngleOwner *owner) {
    GridAngleTable *table = owner->slot->table;
    s32 deltas[2];
    s32 *dimensionOut = (s32 *)((u8 *)out + 4);
    s32 colorFactor;
    u32 *sourceColor;
    u32 *destColor;
    s32 i = 0;

    deltas[0] = (rectangle->right - rectangle->left) << 4;
    deltas[1] = (rectangle->bottom - rectangle->top) << 3;
    {
        s32 fractionalMask = 0xFFFF;

        for (; i < 2; i++) {
            s32 delta = deltas[i];
            s32 magnitude = delta < 0 ? -delta : delta;
            s32 product = magnitude * owner->angle;
            s32 negative = 0;
            s32 scaled;

            if (product < 0) {
                negative++;
            }
            scaled = (product + negative * fractionalMask) >> 16;
            if (delta > 0) {
                dimensionOut[i] = delta - scaled;
            } else {
                dimensionOut[i] = delta + scaled;
            }
        }
    }

    if (table->mirrored != 0) {
        colorFactor = 0x10000 - owner->angle;
    } else {
        colorFactor = owner->angle;
    }
    sourceColor = rectangle->colors;
    destColor = (u32 *)((u8 *)dimensionOut + 0x10);
    {
        s32 colorMask = -0x100;
        s32 fractionalMask = 0xFFFF;

        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            u32 color = *sourceColor;
            s32 alpha = *(u8 *)sourceColor;
            s32 product = alpha * colorFactor;
            s32 negative = 0;

            if (product < 0) {
                negative++;
            }
            *destColor = (color & colorMask) |
                         ((product + negative * fractionalMask) >> 16);
        }
    }
    return 0x10000 / table->divisor;
}

/* Unpack engine RGBA order into GS packed R/G and B/A word pairs. */
void itfGridUnpackColorChannels(u64 *channels, u32 color) {
    u64 blueBits;
    channels[0] = (color >> 24) | ((u64)((color >> 16) & 0xFF) << 32);
    blueBits = color & 0xFF00;
    channels[1] = (blueBits >> 8) | ((u64)(color & 0xFF) << 32);
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

/* Draw a triangle with the same packed color at all three vertices. */
void uiDrawUniformRgbRange(u32 xCoordinates, u32 yCoordinates, u32 z, u32 color, u32 surfaceIndex, u32 extraA, u32 extraB, u32 extraC) {
    u32 vertexColors[3] = {color, color, color};
    func_00308650(xCoordinates, yCoordinates, z, vertexColors, surfaceIndex, extraA, extraB, extraC);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308650);

/* Draw a rectangle as a four-vertex triangle strip with one packed color. */
void uiDrawUniformRgbaRange(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 gsContext, u32 surfaceIndex) {
    u32 vertexColors[4] = {color, color, color, color};
    func_00308828(x, y, z, width, height, vertexColors, gsContext, surfaceIndex);
}

void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex) {
    uiDrawUniformRgbaRange(x, y, z, width, height, color, 0, surfaceIndex);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308828);

void uiDrawGradientColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 vertexColors, u32 surfaceIndex) {
    func_00308828(x, y, z, width, height, vertexColors, 0, surfaceIndex);
}

/* Draw four frame edges; the bottom edge extends 16 units beyond the right side. */
void uiDrawFrameEdges(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 context) {
    uiDrawUniformColorLine(x, y, z, x + width, y, z, color, context);
    uiDrawUniformColorLine(x, y, z, x, y + height, z, color, context);
    uiDrawUniformColorLine(x + width, y, z, x + width, y + height, z, color, context);
    uiDrawUniformColorLine(x, y + height, z, x + width + 0x10, y + height, z, color, context);
}

void uiDrawUniformColorLine(u32 startX, u32 startY, u32 startZ, u32 endX, u32 endY, u32 endZ, u32 color, u32 surfaceIndex) {
    u32 vertexColors[2] = {color, color};
    func_00308AF0(startX, startY, startZ, endX, endY, endZ, vertexColors, surfaceIndex);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308AF0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308C58);

void uiDrawActiveSurfaceRegion(s32 surfaceIndex) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList((s32)list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        u8 *surface = (u8 *)kwlnDrawSurfaces + (surfaceIndex << 5);
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
    func_0032DB78(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        u8 *surface = (u8 *)kwlnDrawSurfaces + (surfaceIndex << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void uiDrawTexturedSurfaceAtFarDepth(u32 context) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    uiDrawUniformColorRect(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, context);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, context);
    uiDrawActiveSurfaceRegion(context);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308F78);

void uiDrawActiveSurfaceWithTestMode(u32 context) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    uiDrawUniformColorRect(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, context);
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
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xe00, 0, context);
}

/* Scale packed RGB by a Q8 factor, preserving the low alpha byte. */
u32 uiScaleColorRgb(u32 color, u32 scaleFactor) {
    u32 red = color >> 24;
    u32 green = (color >> 16) & 0xFF;
    u32 blue = (color & 0xFF00) >> 8;
    u32 alpha = color & 0xFF;

    red = (red * scaleFactor) >> 8;
    green = (green * scaleFactor) >> 8;
    blue = (blue * scaleFactor) >> 8;
    return (red << 24) | (green << 16) | (blue << 8) | alpha;
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

/* Apply parent flags and enable bit 1 on the linked child widget, if present. */
s32 itfSetWidgetFlagsAndActivateChild(u8 *widget, u32 flags) {
    s32 childLink;
    if (widget == 0) {
        return 0;
    }
    ((GridTextWidget *)widget)->flags = (((GridTextWidget *)widget)->flags & ~2) | flags;
    childLink = (s32)((GridTextWidget *)widget)->selected;
    if (childLink != 0) {
        s32 childWidget = (s32)((GridTextListItem *)childLink)->child;
        if (childWidget != 0) {
            ((GridTextWidget *)childWidget)->flags |= 2;
        }
    }
    return 1;
}

/* Own a NUL-terminated text copy; store X scaled by 16 and Y scaled by 8. */
GridTextWidget *itfCreateGridTextWidget(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                                        u32 reference) {
    GridTextWidget *widget = (GridTextWidget *)sdfAllocSizeClassBlock(0x40);
    u32 textBytes;
    char *textCopy;

    memset(widget, 0, 0x40);
    textBytes = strlen(text) + 1;
    textCopy = (char *)sdfAllocSizeClassBlock(textBytes);
    widget->textLength = textBytes;
    widget->text = textCopy;
    memcpy(textCopy, text, textBytes);
    widget->firstVisible = NULL;
    widget->x = x << 4;
    widget->y = y << 3;
    widget->reference = reference;
    widget->rows = rows;
    widget->width = columns * 12 + 6;
    widget->height = rows * 14 + 6;
    widget->head = NULL;
    widget->selected = NULL;
    widget->tail = NULL;
    widget->flags = 0;
    widget->cursorRow = 0;
    widget->itemCount = 0;
    return widget;
}

/* A zero column or row count leaves that dimension unchanged. */
void itfSetGridDimensions(GridTextWidget *widget, s32 columns, s32 rows) {
    s32 columnWidth = columns * 12 + 6;
    s32 rowHeight = rows * 14 + 6;

    if (columns != 0) {
        widget->width = columnWidth;
    }
    if (rows != 0) {
        widget->height = rowHeight;
        widget->rows = rows;
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

/* Destroy linked child widgets recursively before releasing the parent widget. */
u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    u32 childLink;

    sdfReleaseChipBlock(widget->text);
    childLink = (u32)widget->selected;
    if (childLink != 0) {
        do {
            u32 childWidget = (u32)((GridTextListItem *)childLink)->child;
            if (childWidget != 0) {
                itfDestroyGridTextWidgetTree((GridTextWidget *)childWidget);
            }
            childLink = func_00309638(widget);
        } while (childLink != 0);
    }
    sdfReleaseChipBlock(widget);
    return 1;
}

/* Grow only; numbered prefixes reserve four columns, plus two for hexadecimal. */
void itfExpandWidgetColumnWidth(s32 columns, GridTextWidget *widget) {
    s32 flags = widget->flags;
    s32 requiredWidth;
    if (flags & 0x100) {
        columns += 4;
        if (flags & 0x200) {
            columns += 2;
        }
    }
    requiredWidth = columns * 12 + 6;
    if (widget->width < requiredWidth) {
        widget->width = requiredWidth;
    }
}

/* Bit 0: the visible node has a predecessor. Bit 1: at least rows successors remain. */
u32 itfGetGridListLinkFlags(GridTextWidget *owner) {
    GridTextListItem *node = owner->firstVisible;
    u32 flags;
    s32 i;

    if (node == 0) {
        return 0;
    }
    /* Bit 0 reports a predecessor, not a separately owned head object. */
    flags = node->previous != 0;
    for (i = 0; i < (u16)owner->rows; i++) {
        node = node->next;
        if (node == 0) {
            flags &= ~GRID_LIST_HAS_COUNT_FOLLOWERS;
            return flags;
        }
    }
    flags |= GRID_LIST_HAS_COUNT_FOLLOWERS;
    return flags;
}

GridTextListItem *func_00309538(GridTextWidget *owner, const char *text, u32 value) {
    GridTextListItem *item = (GridTextListItem *)sdfAllocSizeClassBlock(0x2C);
    GridTextListItem *tail;
    s32 length;
    s32 allocation;
    char *copy;

    memset(item, 0, 0x2C);
    if (owner->itemCount == 0) {
        owner->firstVisible = item;
        owner->selected = item;
        owner->head = item;
    }
    length = strlen(text);
    allocation = length + 1;
    copy = (char *)sdfAllocSizeClassBlock(allocation);
    item->textLength = allocation;
    item->text = copy;
    memcpy(copy, text, allocation);
    itfExpandWidgetColumnWidth(length, owner);

    /* Initialize the node links, then append it after the current tail. */
    item->previous = owner->tail;
    item->next = 0;
    item->value = value;
    tail = owner->tail;
    item->parameter = NULL;
    item->previous = tail;
    if (tail != 0) {
        tail->next = item;
    }
    owner->tail = item;
    item->index = owner->itemCount;
    owner->itemCount++;
    return item;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309638);

/* Replace the owned text and grow the optional parent column to fit its byte length. */
void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, u8 *node, const char *text) {
    s32 textLength;
    s32 textBytes;
    char *textCopy;

    sdfReleaseChipBlock(*(u32 *)node);
    textLength = strlen(text);
    textBytes = textLength + 1;
    textCopy = (char *)sdfAllocSizeClassBlock(textBytes);
    *(u16 *)(node + 4) = textBytes;
    *(char **)node = textCopy;
    memcpy(textCopy, text, textBytes);
    if (widget != NULL) {
        itfExpandWidgetColumnWidth(textLength, widget);
    }
}

s32 func_00309880(GridTextWidget *widget, GridTextListItem *item,
                  GridNumericDescriptor *descriptor) {
    GridNumericDescriptor *copy;
    f32 maximum;
    s32 width = 1;
    s32 length;

    copy = (GridNumericDescriptor *)sdfAllocSizeClassBlock(sizeof(GridNumericDescriptor));
    item->parameter = copy;
    memcpy(copy, descriptor, sizeof(GridNumericDescriptor));
    item->number = descriptor->value;
    length = strlen(item->text);
    maximum = ((GridNumericDescriptor *)item->parameter)->maximum;

    switch (descriptor->mode) {
        case 1:
            while (maximum >= 16.0f) {
                maximum *= 0.0625f;
                width++;
            }
            width += 2;
            break;
        case 2:
            while (maximum >= 10.0f) {
                maximum /= 10.0f;
                width++;
            }
            width += 2;
            break;
        default:
            while (maximum >= 10.0f) {
                maximum /= 10.0f;
                width++;
            }
            break;
    }

    item->formatWidth = width;
    if (widget->flags & 0x100) {
        length += width;
    } else {
        length += width + 1;
    }
    itfExpandWidgetColumnWidth(length, widget);
    return 1;
}

/* Advance by at least one configured step; crossing the maximum wraps to minimum. */
void itfAdvanceGridScrollPosition(u32 owner, u32 key, s32 steps) {
    GridScrollRange *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = (GridTextListItem *)itfFindGridNodeByKey(key, owner);
    range = entry->parameter;
    position = &entry->number;
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

void itfAdvanceSelectedGridScroll(u32 owner, u32 steps) {
    s32 widgetAddr;

    widgetAddr = (s32)owner;
    if ((((GridTextWidget *)widgetAddr)->flags & 1) != 0) {
        itfAdvanceGridScrollPosition(owner, (u32)((GridTextWidget *)widgetAddr)->selected->index + ((GridTextWidget *)widgetAddr)->rowOffset,
                                    steps);
        return;
    }
}

/* Reverse by at least one configured step; crossing the minimum wraps to maximum. */
void itfReverseGridScrollPosition(u32 owner, u32 key, s32 steps) {
    GridScrollRange *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = (GridTextListItem *)itfFindGridNodeByKey(key, owner);
    range = entry->parameter;
    position = &entry->number;
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
    if ((((GridTextWidget *)widgetAddr)->flags & 1) != 0) {
        itfReverseGridScrollPosition(owner, (u32)((GridTextWidget *)widgetAddr)->selected->index + ((GridTextWidget *)widgetAddr)->rowOffset,
                                    steps);
        return;
    }
}

s32 itfGetGridChildLayoutMode(u8 *widget, u32 target) {
    u32 flags = ((GridTextWidget *)widget)->flags;

    if (flags & 2) {
        if (target == (u32)((GridTextWidget *)widget)->selected) {
            return (flags & 1) ? 6 : 4;
        }
        return 0;
    }
    if (flags & 0x80) {
        if (target == (u32)((GridTextWidget *)widget)->selected) {
            return 12;
        }
    } else if (target == (u32)((GridTextWidget *)widget)->selected && (flags & 1)) {
        return 6;
    }
    return 0;
}

extern s32 strlen(const char *);
extern char *strcpy(char *, const char *);
extern s32 func_0035C860(char *, const char *, ...);
extern double fptodp(f32);

/* Format a native value row, then optionally prefix its decimal/hex row number. */
void itfFormatGridValueEntryText(GridTextWidget *widget, GridTextListItem *entry, char *out) {
    char text[0x100];
    char prefix[0x100];
    char format[0x100];

    if (entry->format != NULL) {
        entry->format(widget, entry, text, 0x100);
    } else if (entry->parameter == NULL) {
        func_0035C860(text, "%s", entry->text);
    } else {
        if (strlen(entry->text) == 0) {
            strcpy(prefix, "");
        } else {
            func_0035C860(prefix, "%s ", entry->text);
        }
        switch (*(s32 *)entry->parameter) {
        case 0:
            func_0035C860(format, "%%s%%0%dd", entry->formatWidth);
            func_0035C860(text, format, prefix, (s32)entry->number);
            break;
        case 1:
            func_0035C860(format, "%%s0x%%0%dX", entry->formatWidth - 2);
            func_0035C860(text, format, prefix, (s32)entry->number);
            break;
        case 2:
            func_0035C860(format, "%%s%%0%d.1f", entry->formatWidth);
            func_0035C860(text, format, prefix, (double)entry->number);
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

extern void fldLmapSubmitPositionedCommandPacket(s32, s32, s32, s32, s32, s32);

/* Draw visible local-map rows and invoke the selected row callback once. */
void func_00309DF8(s32 offsetX, s32 offsetY, s32 z, GridTextWidget *widget,
                   s32 surfaceIndex) {
    char text[0x100];
    GridTextListItem *item;
    s32 rowEnd;
    u32 column;
    s32 itemIndex = 0;
    s32 invokeSelected = 1;

    offsetY = widget->y + offsetY + 0x10;
    rowEnd = offsetY + ((widget->height << 3) + widget->y) - 0x6C;
    offsetX = widget->x + offsetX + 0x20;
    item = widget->firstVisible;
    if (item != NULL) {
        if (offsetY < rowEnd) {
            if ((u16)widget->rows != 0) {
                do {
                    s32 layout = itfGetGridChildLayoutMode((u8 *)widget, (u32)item);
                    s32 drawMode = 0;

                    if (invokeSelected != 0) {
                        drawMode = layout;
                    }
                    if (!(widget->flags & 0x40)) {
                        if (item == widget->selected && (widget->flags & 4)) {
                            column = (s32)item->number;

                            if (widget->flags & 0x100) {
                                column += 4;
                                if (widget->flags & 0x200) {
                                    column += 2;
                                }
                            }
                            column = (column * 3) << 6;
                            uiDrawUniformColorRect(offsetX + column, offsetY, z,
                                                   0xD0, 0x78, 0x40408080,
                                                   surfaceIndex);
                        }
                        if ((widget->flags & 8) && item->value != 0) {
                            s32 width = strlen(item->text) * 0xC0 + 0x10;
                            s32 indent;

                            if (widget->flags & 0x100) {
                                indent = 0x300;
                                if (widget->flags & 0x200) {
                                    indent = 0x480;
                                }
                            } else {
                                indent = 0;
                            }
                            uiDrawUniformColorRect(offsetX + indent, offsetY, z,
                                                   width, 0x78,
                                                   0x40408080, surfaceIndex);
                        }
                        itfFormatGridValueEntryText(widget, item, text);
                        fldLmapSubmitPositionedCommandPacket(offsetX, offsetY, z,
                                                             drawMode, (s32)text,
                                                             surfaceIndex);
                    }
                    if (item->select != NULL && item == widget->selected &&
                        invokeSelected != 0) {
                        invokeSelected = 0;
                        item->select(widget);
                    }
                    offsetY += 0x70;
                    item = item->next;
                    itemIndex++;
                    if (item == NULL) {
                        break;
                    }
                    if (offsetY >= rowEnd) {
                        break;
                    }
                } while (itemIndex < (u16)widget->rows);
            }
        }
    }
}

s32 itfFindGridNodeByKey(u32 key, u32 head) {
    u32 n;

    n = (u32)((GridTextWidget *)head)->head;
    while (n != 0 && ((GridTextListItem *)n)->index != key) {
        n = (u32)((GridTextListItem *)n)->next;
    }
    return n;
}

s32 sdfGridSeekSelectedNodeByIndex(s32 index, u8 *widget) {
    s16 count = ((GridTextWidget *)widget)->itemCount;
    u16 width;
    u8 *first;

    if (index >= count) {
        return 0;
    }
    first = (u8 *)((GridTextWidget *)widget)->head;
    ((GridTextWidget *)widget)->cursorRow = 0;
    ((GridTextWidget *)widget)->firstVisible = (GridTextListItem *)first;
    ((GridTextWidget *)widget)->selected = (GridTextListItem *)first;
    if (index > 0) {
        width = (u16)((GridTextWidget *)widget)->rows;
        do {
            u8 *current = (u8 *)((GridTextWidget *)widget)->firstVisible;
            if (width >= count - ((GridTextListItem *)current)->index) {
                ((GridTextWidget *)widget)->cursorRow++;
            } else {
                ((GridTextWidget *)widget)->firstVisible = ((GridTextListItem *)current)->next;
            }
            current = (u8 *)((GridTextWidget *)widget)->selected;
            ((GridTextWidget *)widget)->selected = ((GridTextListItem *)current)->next;
        } while (--index != 0);
    }
    return 1;
}

void sdfGridSeekFirstNode(u32 widget) {
    sdfGridSeekSelectedNodeByIndex(0, widget);
}

void sdfGridSeekLastNode(u8 *entry) {
    sdfGridSeekSelectedNodeByIndex(((GridTextWidget *)entry)->itemCount - 1, entry);
}
