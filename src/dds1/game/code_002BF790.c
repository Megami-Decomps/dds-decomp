#include "common.h"
#include "fpu.h"
#include "eff.h"
#include "itf_grid_text.h"
#include "sdf.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);
extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);

extern GridTextListItem *itfFindGridNodeByKey(u32, GridTextWidget *);

extern GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *);

typedef struct GridPosition {
    s32 x; // 0x00
    s32 y; // 0x04
} GridPosition; // 0x08

/* Four words filled together; their corner/channel interpretation is unknown. */
typedef struct UiQuadWords {
    u32 unk00[4];
} UiQuadWords; // 0x10


extern s32 sdfGridSeekSelectedNodeByIndex(s32, GridTextWidget *);


extern SdfPoolNode kwlnDrawSurfaces[];

extern s32 sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);

/* Resolve the indexed render entry before applying position, depth, and draw flags. */
void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z, s32 drawFlags, s32 object, s32 index, s32 surfaceIndex) {
    void *renderEntry = effGetSlotWorkOrOverride((EffectSlotSet *)object, index);
    func_002BF400(offsetX, offsetY, z, drawFlags, (EffectSlotSet *)object, index, (BdWork *)renderEntry,
                  surfaceIndex);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BF828);

/* Resolve an entry by key, falling back to the object's stored value. */
s32 itfGridLookupValueOrDefault(EffectSlotSet *object, s32 key) {
    BdWork *entry = (BdWork *)effGetSlotWorkOrOverride(object, key);
    s32 result;

    if (entry->states[0].delay == 0) {
        func_002BF828(object, key);
    }
    result = (s32)effUpdateTimedStates(object, (u32)key, entry);
    if (result == 0) {
        result = object->defaultValue;
    }
    return result;
}

extern void func_002BD3D8(void *, s32, void *);

void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *object, s32 index, s32 x, s32 y, s32 width, s32 height) {
    EffectSlotDescription *entry = &object->descriptions[index];
    void *record = effGetSlotWorkOrOverride(object, index);

    entry->xOffset = x >> 4;
    entry->yOffset = y >> 3;
    entry->width = width >> 4;
    entry->height = height >> 3;
    func_002BD3D8(object, index, record);
}

/* Store pixel bounds quantized to the widget's 16x8 grid, then copy all four words. */
void itfGridSetQuantizedBounds(EffectSlotSet *object, s32 index, s32 x, s32 y,
                   s32 width, s32 height) {
    EffectSlotDescription *entry = &object->descriptions[index];
    u32 *destination = (u32 *)object->workEntries[index].bounds.grid.quantizedBounds;
    u32 *source;
    s32 remaining = 3;
    entry->xOffset = x >> 4;
    entry->yOffset = y >> 3;
    entry->width = width >> 4;
    entry->height = height >> 3;
    source = (u32 *)&entry->xOffset;
    do {
        *destination++ = *source++;
    } while (--remaining >= 0);
}

void itfGridSetBounds(EffectSlotSet *object, s32 index, s32 x, s32 y, s32 width, s32 height) {
    BdWork *widget = (BdWork *)effGetSlotWorkOrOverride(object, index);
    widget->parameters[0] = x;
    widget->parameters[1] = y;
    widget->parameters[2] = width;
    widget->parameters[3] = height;
}

/* Restore four saved palette words in the native packed work-record buffer. */
void itfGridCopyEntryQuad(s32 owner, s32 index) {
    u32 *destination;
    s32 remaining;

    remaining = 3;
    destination = (u32 *)(index * sizeof(BdWork)
                         + (u32)((EffectSlotSet *)owner)->workEntries) + 5;
    do {
        remaining = remaining - 1;
        *destination = destination[0x1C];
        destination = destination + 1;
    } while (-1 < remaining);
}

/* Store the two grid position coordinates. */
void itfGridStorePosition(GridPosition *position, s32 x, s32 y) {
    position->x = x;
    position->y = y;
}


typedef struct GridAngleTable {
    s32 divisor;      /* 0x00 */
    s32 mirrored;     /* 0x04 */
    s32 cycleDivisor; /* 0x08 */
} GridAngleTable;

typedef struct GridAngleSlot {
    u8 pad00[0x20];
    GridAngleTable *table; /* 0x20 */
} GridAngleSlot;


s32 itfGridApplySqrtBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)((EffMappedRecord *)owner->source)->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 scaled = (s32)(fsqrtf((f32)delta) * (f32)owner->value * (1.0f / 65536.0f));
        if (delta > 0) {
            dimensions[i] = delta - scaled * scaled;
        } else {
            dimensions[i] = scaled * scaled + delta;
        }
    }
    if (table->mirrored != 0) {
        factor = 0x10000 - owner->value;
    } else {
        factor = owner->value;
    }
    sourceColor = rectangle->savedColors;
    destColor = geometry->cornerColors;
    for (i = 0; i < 4; i++, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        *destColor = (color & ~0xFF) | (alpha * factor / 0x10000);
    }
    return 0x10000 / table->divisor;
}

/* Apply the ZOOM_01 easing to the adjustment bounds and fade their alpha. */
s32 itfGridApplyQuadraticZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
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

    table = ((GridAngleSlot *)owner->source)->table;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->sourceHeight) / rectangle->sourceWidth;
    previous[0] = out->geometry.bounds[0];
    previous[1] = out->geometry.bounds[1];
    scaledWidth = (s32)(fsqrtf((f32)deltas[0]) * (f32)owner->value * (1.0f / 65536.0f));
    squares[0] = scaledWidth * scaledWidth;
    widthAdjustment = -((deltas[0] - squares[0]) / 2);
    scaledHeight = (s32)(fsqrtf((f32)deltas[1]) * (f32)owner->value * (1.0f / 65536.0f));
    squares[1] = scaledHeight * scaledHeight;
    heightAdjustment = -((deltas[1] - squares[1]) / 2);

    out->geometry.bounds[0] = widthAdjustment;
    out->geometry.bounds[2] += (previous[0] - widthAdjustment) * 2;
    out->geometry.bounds[1] = heightAdjustment;
    out->geometry.bounds[3] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->savedColors;
    destColor = out->geometry.cornerColors;
    i = 3;

    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 product = alpha * owner->value;
        s32 negative = 0;

        if (product < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((product + negative * fractionalMask) >> 16);
    }
    return 0x10000 / table->cycleDivisor;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFE78);

s32 func_002C0038(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)((EffMappedRecord *)owner->source)->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 magnitude = delta < 0 ? -delta : delta;
        s32 amount = magnitude * owner->value / 0x10000;
        if (delta > 0) {
            dimensions[i] = delta - amount;
        } else {
            dimensions[i] = delta + amount;
        }
    }
    factor = ((100 - table->mirrored) << 16) / 100;
    destColor = geometry->cornerColors;
    sourceColor = rectangle->savedColors;
    for (i = 0; i < 4; i++, destColor++, sourceColor++) {
        if (owner->value < factor) {
            u32 color = *sourceColor;
            s32 alpha = *(u8 *)sourceColor;
            *destColor = (color & ~0xFF) | (alpha * owner->value / factor);
        } else {
            *destColor = *sourceColor;
        }
    }
    return 0x10000 / table->divisor;
}


s32 itfUpdateAngleAndGetCycleStep(BdWork *unused, BdWork *out, EffTimedState *owner) {
    GridAngleTable *table = ((GridAngleSlot *)owner->source)->table;
    s32 i = 3;

    do {
        if (table->mirrored == 0) {
            out->geometry.angleDegrees = 360.0f - (f32)owner->value * 360.0f * (1.0f / 65536.0f);
        } else {
            out->geometry.angleDegrees = (f32)owner->value * 360.0f * (1.0f / 65536.0f);
        }
    } while (--i >= 0);
    return 0x10000 / table->divisor;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0200);

/* Apply linear ZOOM easing to the adjustment bounds and fade their alpha. */
s32 itfGridApplyLinearZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
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

    table = ((GridAngleSlot *)owner->source)->table;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->sourceHeight) / rectangle->sourceWidth;
    angle = owner->value;
    previous[0] = out->geometry.bounds[0];
    previous[1] = out->geometry.bounds[1];
    scaledWidth = deltas[0] * angle;
    if (scaledWidth < 0) {
        scaledWidth += 0xFFFF;
    }
    scaled[0] = scaledWidth >> 16;
    widthAdjustment = -((deltas[0] - scaled[0]) / 2);
    out->geometry.bounds[0] = widthAdjustment;
    out->geometry.bounds[2] += (previous[0] - widthAdjustment) * 2;

    scaledHeight = deltas[1] * angle;
    if (scaledHeight < 0) {
        scaledHeight += 0xFFFF;
    }
    scaled[1] = scaledHeight >> 16;
    heightAdjustment = -((deltas[1] - scaled[1]) / 2);
    out->geometry.bounds[1] = heightAdjustment;
    out->geometry.bounds[3] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->savedColors;
    destColor = out->geometry.cornerColors;
    i = 3;
    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 colorProduct = alpha * owner->value;
        s32 negative = 0;

        if (colorProduct < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((colorProduct + negative * fractionalMask) >> 16);
    }
    return 0x10000 / table->cycleDivisor;
}

s32 func_002C04C8(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)((EffMappedRecord *)owner->source)->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 magnitude = delta < 0 ? -delta : delta;
        s32 amount = magnitude * owner->value / 0x10000;
        if (delta > 0) {
            dimensions[i] = delta - amount;
        } else {
            dimensions[i] = delta + amount;
        }
    }
    if (table->mirrored != 0) {
        factor = 0x10000 - owner->value;
    } else {
        factor = owner->value;
    }
    sourceColor = rectangle->savedColors;
    destColor = geometry->cornerColors;
    for (i = 0; i < 4; i++, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        *destColor = (color & ~0xFF) | (alpha * factor / 0x10000);
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

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0628);

void itfGridDrawBooleanDescriptor(u8 value, s32 alternate, s32 kind) {
    u32 normalized = value != 0;
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[0] = normalized;
    if (!alternate) {
        descriptor[1] = 0x4A;
    } else {
        descriptor[1] = 0x4B;
    }
    context = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, (u32)packet);
    entry = &kwlnDrawSurfaces[kind];
    entry->append((SdfListHead *)entry, context);
}

void itfSetPrimaryFramebufferAlphaFlag(u8 value, u32 kind) {
    itfGridDrawBooleanDescriptor(value, 0, kind);
}

void itfSubmitToggledGridWord(s32 data, s32 alternate, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x47;
    } else {
        descriptor[1] = 0x48;
    }
    context = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, (u32)packet);
    entry = &kwlnDrawSurfaces[kind];
    entry->append((SdfListHead *)entry, context);
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
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[0] = data;
    if (!alternate) {
        descriptor[1] = 0x42;
    } else {
        descriptor[1] = 0x43;
    }
    context = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, (u32)packet);
    entry = &kwlnDrawSurfaces[kind];
    entry->append((SdfListHead *)entry, context);
}

void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind) {
    sdfSubmitGsAlphaRegisterPacket(data, 0, kind);
}

void sdfSubmitGsPabeRegisterPacket(s32 data, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[1] = 0x49;
    descriptor[0] = data;
    context = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, (u32)packet);
    entry = &kwlnDrawSurfaces[kind];
    entry->append((SdfListHead *)entry, context);
}

void sdfSubmitGsTexRegisterPacket(s32 data, s32 kind) {
    s32 packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader(packet);
    descriptor[1] = 0x14;
    descriptor[0] = data;
    context = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, (u32)packet);
    entry = &kwlnDrawSurfaces[kind];
    entry->append((SdfListHead *)entry, context);
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
    func_002C0C20(xCoordinates, yCoordinates, z, vertexColors, surfaceIndex, extraA, extraB, extraC);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0C20);

/* Draw a rectangle as a four-vertex triangle strip with one packed color. */
void uiDrawUniformRgbaRange(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 gsContext, u32 surfaceIndex) {
    u32 vertexColors[4] = {color, color, color, color};
    func_002C0DF8(x, y, z, width, height, vertexColors, gsContext, surfaceIndex);
}

void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex) {
    uiDrawUniformRgbaRange(x, y, z, width, height, color, 0, surfaceIndex);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0DF8);

void uiDrawGradientColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 vertexColors, u32 surfaceIndex) {
    func_002C0DF8(x, y, z, width, height, vertexColors, 0, surfaceIndex);
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
    func_002C10C0(startX, startY, startZ, endX, endY, endZ, vertexColors, surfaceIndex);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C10C0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1228);

extern s32 kwlnGetDrawBufferIndex(void);
extern u8 kwlnFrameDrawPacketRecords[];
extern void func_002D4C80(const void *, void *, s32);
extern void func_002D4CC8(const void *, void *, s32);
extern void sdfAppendDmaTagToList(SdfListHead *, u32);

void uiDrawActiveSurfaceRegion(s32 surfaceIndex) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4C80(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, (u32)texture);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[surfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

void sdfDispatchSurfaceWithPreparedTexturePacket(s32 surfaceIndex) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4CC8(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, (u32)texture);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[surfaceIndex];
        surface->append((SdfListHead *)surface, list);
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

s32 itfActivateGridTextWidget(GridTextWidget *widget) {
    if (widget == 0) {
        return 0;
    }
    widget->flags = (widget->flags & -2) | 2;
    return 1;
}

/* Apply parent flags and enable bit 1 on the linked child widget, if present. */
s32 itfSetWidgetFlagsAndActivateChild(GridTextWidget *widget, u32 flags) {
    GridTextListItem *childLink;
    if (widget == 0) {
        return 0;
    }
    widget->flags = (widget->flags & ~2) | flags;
    childLink = widget->selected;
    if (childLink != 0) {
        GridTextWidget *childWidget = childLink->child;
        if (childWidget != 0) {
            childWidget->flags |= 2;
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
    GridTextListItem *next;

    sdfReleaseChipBlock(widget->text);
    do {
        next = itfRemoveSelectedGridTextItem(widget);
    } while (next != NULL);
    sdfReleaseChipBlock(widget);
    return 1;
}

/* Destroy linked child widgets recursively before releasing the parent widget. */
u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    GridTextListItem *childLink;

    sdfReleaseChipBlock(widget->text);
    childLink = widget->selected;
    if (childLink != NULL) {
        do {
            GridTextWidget *childWidget = childLink->child;
            if (childWidget != NULL) {
                itfDestroyGridTextWidgetTree(childWidget);
            }
            childLink = itfRemoveSelectedGridTextItem(widget);
        } while (childLink != NULL);
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
    flags = node->previous != 0;
    for (i = 0; i < (u16)owner->rows; i++) {
        node = node->next;
        if (node == 0) {
            flags &= ~2;
            return flags;
        }
    }
    flags |= 2;
    return flags;
}

GridTextListItem *itfAppendGridTextItem(GridTextWidget *owner, const char *text, u32 value) {
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

INCLUDE_ASM(const s32, "game/code_002BF790", itfRemoveSelectedGridTextItem);

/* Replace the owned text and grow the optional parent column to fit its byte length. */
void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, GridTextListItem *item, const char *text) {
    s32 textLength;
    s32 textBytes;
    char *textCopy;

    sdfReleaseChipBlock(item->text);
    textLength = strlen(text);
    textBytes = textLength + 1;
    textCopy = (char *)sdfAllocSizeClassBlock(textBytes);
    item->textLength = textBytes;
    item->text = textCopy;
    memcpy(textCopy, text, textBytes);
    if (widget != NULL) {
        itfExpandWidgetColumnWidth(textLength, widget);
    }
}

s32 itfSetGridNumericItemDescriptor(GridTextWidget *widget, GridTextListItem *item,
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
        case GRID_NUMERIC_FORMAT_HEXADECIMAL:
            while (maximum >= 16.0f) {
                maximum *= 0.0625f;
                width++;
            }
            width += 2;
            break;
        case GRID_NUMERIC_FORMAT_FLOAT:
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
    if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
        length += width;
    } else {
        length += width + 1;
    }
    itfExpandWidgetColumnWidth(length, widget);
    return 1;
}

/* Advance by at least one configured step; crossing the maximum wraps to minimum. */
void itfAdvanceGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps) {
    GridNumericDescriptor *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = itfFindGridNodeByKey(key, widget);
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

void itfAdvanceSelectedGridScroll(GridTextWidget *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfAdvanceGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

/* Reverse by at least one configured step; crossing the minimum wraps to maximum. */
void itfReverseGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps) {
    GridNumericDescriptor *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = itfFindGridNodeByKey(key, widget);
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

void itfReverseSelectedGridScroll(GridTextWidget *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfReverseGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

s32 itfGetGridChildLayoutMode(GridTextWidget *widget, GridTextListItem *target) {
    u32 flags = widget->flags;

    if (flags & 2) {
        if (target == widget->selected) {
            return (flags & 1) ? 6 : 4;
        }
        return 0;
    }
    if (flags & 0x80) {
        if (target == widget->selected) {
            return 12;
        }
    } else if (target == widget->selected && (flags & 1)) {
        return 6;
    }
    return 0;
}

extern s32 strlen(const char *);
extern char *strcpy(char *, const char *);
extern s32 func_003014F0(char *, const char *, ...);
extern double fptodp(f32);

/* Format a native value row, then optionally prefix its decimal/hex row number. */
void itfFormatGridValueEntryText(GridTextWidget *widget, GridTextListItem *entry, char *out) {
    char text[0x100];
    char prefix[0x100];
    char format[0x100];

    if (entry->format != NULL) {
        entry->format(widget, entry, text, 0x100);
    } else if (entry->parameter == NULL) {
        func_003014F0(text, "%s", entry->text);
    } else {
        if (strlen(entry->text) == 0) {
            strcpy(prefix, "");
        } else {
            func_003014F0(prefix, "%s ", entry->text);
        }
        switch (((GridNumericDescriptor *)entry->parameter)->mode) {
        case GRID_NUMERIC_FORMAT_DECIMAL:
            func_003014F0(format, "%%s%%0%dd", entry->formatWidth);
            func_003014F0(text, format, prefix, (s32)entry->number);
            break;
        case GRID_NUMERIC_FORMAT_HEXADECIMAL:
            func_003014F0(format, "%%s0x%%0%dX", entry->formatWidth - 2);
            func_003014F0(text, format, prefix, (s32)entry->number);
            break;
        case GRID_NUMERIC_FORMAT_FLOAT:
            func_003014F0(format, "%%s%%0%d.1f", entry->formatWidth);
            func_003014F0(text, format, prefix, (double)entry->number);
            break;
        }
    }
    if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
        s32 row = entry->index + widget->rowOffset;

        if (!(widget->flags & GRID_TEXT_HEX_ROW_INDEX)) {
            func_003014F0(out, "%03d:%s", row, text);
        } else {
            func_003014F0(out, "0x%03X:%s", row, text);
        }
    } else {
        strcpy(out, text);
    }
}

extern void fldLmapSubmitPositionedCommandPacket(s32, s32, s32, s32, const char *, s32);

/* Draw visible local-map rows and invoke the selected row callback once. */
void itfDrawGridTextRows(s32 offsetX, s32 offsetY, s32 z, GridTextWidget *widget,
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
                    s32 layout = itfGetGridChildLayoutMode(widget, item);
                    s32 drawMode = 0;

                    if (invokeSelected != 0) {
                        drawMode = layout;
                    }
                    if (!(widget->flags & GRID_TEXT_HIDE_ROWS)) {
                        if (item == widget->selected && (widget->flags & GRID_TEXT_HIGHLIGHT_SELECTION)) {
                            column = (s32)item->number;

                            if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
                                column += 4;
                                if (widget->flags & GRID_TEXT_HEX_ROW_INDEX) {
                                    column += 2;
                                }
                            }
                            column = (column * 3) << 6;
                            uiDrawUniformColorRect(offsetX + column, offsetY, z,
                                                   0xD0, 0x78, 0x40408080,
                                                   surfaceIndex);
                        }
                        if ((widget->flags & GRID_TEXT_HIGHLIGHT_VALUES) && item->value != 0) {
                            s32 width = strlen(item->text) * 0xC0 + 0x10;
                            s32 indent;

                            if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
                                indent = 0x300;
                                if (widget->flags & GRID_TEXT_HEX_ROW_INDEX) {
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
                                                             drawMode, text,
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

GridTextListItem *itfFindGridNodeByKey(u32 key, GridTextWidget *widget) {
    GridTextListItem *item = widget->head;

    while (item != NULL && item->index != key) {
        item = item->next;
    }
    return item;
}

s32 sdfGridSeekSelectedNodeByIndex(s32 index, GridTextWidget *widget) {
    s16 count = widget->itemCount;
    u16 width;
    GridTextListItem *first;

    if (index >= count) {
        return 0;
    }
    first = widget->head;
    widget->cursorRow = 0;
    widget->firstVisible = first;
    widget->selected = first;
    if (index > 0) {
        width = (u16)widget->rows;
        do {
            GridTextListItem *current = widget->firstVisible;
            if (width >= count - current->index) {
                widget->cursorRow++;
            } else {
                widget->firstVisible = current->next;
            }
            current = widget->selected;
            widget->selected = current->next;
        } while (--index != 0);
    }
    return 1;
}

s32 sdfGridSeekFirstNode(GridTextWidget *widget) {
    return sdfGridSeekSelectedNodeByIndex(0, widget);
}

s32 sdfGridSeekLastNode(GridTextWidget *widget) {
    return sdfGridSeekSelectedNodeByIndex(widget->itemCount - 1, widget);
}

INCLUDE_RODATA(const s32, "game/code_002BF790", fldLocalMapTaskName);

