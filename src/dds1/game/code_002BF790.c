#include "sdf_gs_header.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "fpu.h"
#include "eff.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "itf_grid_text.h"
#include "itf_draw_grid.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);

extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);

typedef struct GridPosition {
    s32 x; // 0x00
    s32 y; // 0x04
} GridPosition; // 0x08

/* Four words filled together; their corner/channel interpretation is unknown. */
typedef struct UiQuadWords {
    u32 unk00[4];
} UiQuadWords; // 0x10

extern SdfPoolNode kwlnDrawSurfaces[];

typedef struct SdfDrawPacket SdfDrawPacket;

extern s32 sdfConsCalculateDrawPacketSize(s32 registerCount, s32 loopCount);

extern s32 sdfConsMeasurePacketWithHeader(s32 address);

extern void *sdfConsInitPacketHeader(SdfDrawPacket *packet, s32 primitive, s32 registerCount,
                                     s64 registers, s32 loopCount);

extern s32 sdfAllocPacketAligned(s32);

/* Resolve the indexed render entry before applying position, depth, and draw flags. */
void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z, s32 drawFlags, EffectSlotSet *object, s32 index, s32 surfaceIndex) {
    void *renderEntry = effGetSlotWorkOrOverride(object, index);
    func_002BF400(offsetX, offsetY, z, drawFlags, object, index, (BdWork *)renderEntry,
                  surfaceIndex);
}

/* Update the indexed slot's description countdown and carry its active state forward. */
void itfUpdateGridSlotDescription(EffectSlotSet *owner, s32 index) {
    BdWork *base = &owner->workEntries[index];
    const u32 timedByteOffset = (index + base->slotOffset) * sizeof(BdWork);
    BdWork *timed = (BdWork *)(timedByteOffset + (u32)owner->workEntries);
    BdWork *previous = (BdWork *)effGetSlotWorkOrOverride(owner, index + base->slotOffset);
    BdWork *next;
    u32 advance = 0;

    if (timed->remainingDescriptionUpdates == 0) {
        timed->remainingDescriptionUpdates =
            owner->descriptions[index + base->slotOffset].descriptionUpdateDelay;
        if ((u32)(index + timed->slotOffset + 1) < owner->count) {
            const s32 nextFlags = owner->descriptions[index + base->slotOffset + 1].flags &
                                  EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION;
            const u32 shouldAdvance = nextFlags > 0;
            advance = shouldAdvance;
        }
        if (advance == 1) {
            base->slotOffset++;
        } else {
            if (base->slotOffset == 0) {
                return;
            }
            base->slotOffset = 0;
        }
        effInitializeSlotWorkFromDescription(owner, index + base->slotOffset, previous);
        next = (BdWork *)effGetSlotWorkOrOverride(owner, index + base->slotOffset);
        next->states[0].flags = previous->states[0].flags;
        next->states[0].source = previous->states[0].source;
        next->states[0].value = previous->states[0].value;
    } else {
        timed->remainingDescriptionUpdates--;
    }
}

/* Resolve an entry by key, falling back to the object's stored value. */
s32 itfGridLookupValueOrDefault(EffectSlotSet *object, s32 key) {
    BdWork *entry = (BdWork *)effGetSlotWorkOrOverride(object, key);
    s32 result;

    if (entry->states[0].delay == 0) {
        itfUpdateGridSlotDescription(object, key);
    }
    result = (s32)effUpdateTimedStates(object, (u32)key, entry);
    if (result == 0) {
        result = object->defaultValue;
    }
    return result;
}

void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *object, s32 index, s32 x, s32 y, s32 width, s32 height) {
    EffectSlotDescription *entry = &object->descriptions[index];
    void *record = effGetSlotWorkOrOverride(object, index);

    entry->bounds[0] = x >> 4;
    entry->bounds[1] = y >> 3;
    entry->bounds[2] = width >> 4;
    entry->bounds[3] = height >> 3;
    effInitializeSlotWorkFromDescription(object, index, record);
}

/* Store pixel bounds quantized to the widget's 16x8 grid, then copy all four words. */
void itfGridSetQuantizedBounds(EffectSlotSet *object, s32 index, s32 x, s32 y,
                   s32 width, s32 height) {
    EffectSlotDescription *entry = &object->descriptions[index];
    u32 *destination = (u32 *)object->workEntries[index].bounds.grid.quantizedBounds;
    u32 *source;
    s32 remaining = 3;
    entry->bounds[0] = x >> 4;
    entry->bounds[1] = y >> 3;
    entry->bounds[2] = width >> 4;
    entry->bounds[3] = height >> 3;
    source = (u32 *)entry->bounds;
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

s32 itfGridApplySqrtBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)owner->source->status;
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

    table = (GridAngleTable *)owner->source->status;
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

/* The FLUSH_01 status payload stores the signed IN, WAIT and OUT durations. */
typedef struct GridFlushTable {
    s32 inDuration;
    s32 waitDuration;
    s32 outDuration;
} GridFlushTable;

/* Shrink the grid bounds and apply the three-phase flush envelope to corner alpha. */
s32 itfGridApplyTimedBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
    GridFlushTable *table;
    s32 phases[3];
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensionOut;
    s32 totalDuration;
    s32 numerator;
    s32 denominator;
    u32 *sourceColor;
    u32 *destColor;
    s32 i = 0;

    table = (GridFlushTable *)owner->source->status;
    totalDuration = table->inDuration;
    totalDuration += table->outDuration;
    totalDuration += table->waitDuration;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    dimensionOut = geometry->bounds;
    {
        s32 fractionalMask = 0xFFFF;

        for (; i < 2; i++) {
            s32 delta = deltas[i];
            s32 magnitude = delta < 0 ? -delta : delta;
            s32 product = magnitude * owner->value;
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

    phases[0] = (table->inDuration << 16) / totalDuration;
    phases[1] = (table->waitDuration << 16) / totalDuration;
    phases[2] = (table->outDuration << 16) / totalDuration;

    if (phases[1] + phases[2] < owner->value) {
        numerator = phases[0] - (owner->value - (phases[1] + phases[2]));
        denominator = phases[0];
    } else if (phases[2] < owner->value) {
        destColor = geometry->cornerColors;
        sourceColor = rectangle->savedColors;
        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            *destColor = *sourceColor;
        }
        return 0x10000 / totalDuration;
    } else {
        numerator = owner->value;
        denominator = phases[2];
    }

    destColor = geometry->cornerColors;
    sourceColor = rectangle->savedColors;
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

s32 itfGridApplyThresholdBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)owner->source->status;
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
    GridAngleTable *table = (GridAngleTable *)owner->source->status;
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

    table = (GridAngleTable *)owner->source->status;
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

s32 itfGridApplyLinearBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *owner) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)owner->source->status;
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

typedef struct DmaPacketHeader {
    u16 quadwords;
    u8 pad02[6];
    u32 reservedWord;
    u32 command;
    u8 pad10[0x10];
} DmaPacketHeader;

typedef struct ConsMatrixPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 reservedWord;
    u32 command;
    u8 matrixA[0x40];
    u8 matrixB[0x40];
    u8 vecC[0x10];
    u8 vecD[0x10];
    u8 vecE[0x10];
    u32 stmodCommand;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
} ConsMatrixPacket;

extern SdfListHead D_003DFAB0[2];

extern DmaPacketHeader D_003DFAF0[2];

extern SdfListHead D_003DFB30[2];

extern SdfLightingPacketStorage D_003DFB70[2];

extern ConsMatrixPacket D_003DFD30[2];

extern SdfLightSources D_00324770;

extern f32 kwlnDefaultColorVector[4];

extern u8 sdfViewMatrix[0x40];

extern u8 sdfViewEyeVector[];

extern u8 sdfViewTargetVector[];

extern u8 sdfViewUpVector[];

extern void sdfVuBuildLookAtBasis(void *, void *, void *);

extern void sdfConsAppendProgramReferencePacket(SdfListHead *, DmaPacketHeader *);

extern void sdfBuildLightingPacket(void *, SdfLightSources, f32 *);

/* Rebuild the view matrix and both frame banks' matrix and lighting packet lists. */
void itfInitDoubleBufferedScenePackets(void) {
    s32 i;

    sdfCameraBuildProjection(&sdfSceneProjectionParameters.camera);
    sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
    VU0_STORE_MATRIX_UNCLOBBERED(sdfViewMatrix);
    for (i = 0; i < 2; i++) {
        sdfInitPacketList(&D_003DFAB0[i]);
        sdfConsAppendProgramReferencePacket(&D_003DFAB0[i], &D_003DFAF0[i]);
        sdfConsBuildMatrixPacket((struct ConsMatrixPacket *)&D_003DFD30[i], &sdfSceneProjectionParameters, sdfViewMatrix);
        sdfAppendPacket(&D_003DFAB0[i], (u32)&D_003DFD30[i]);
        sdfInitPacketList(&D_003DFB30[i]);
        sdfBuildLightingPacket(&D_003DFB70[i], D_00324770, kwlnDefaultColorVector);
        sdfAppendPacket(&D_003DFB30[i], (u32)&D_003DFB70[i]);
    }
}

void itfGridDrawBooleanDescriptor(u8 value, s32 alternate, s32 kind) {
    u32 normalized = value != 0;
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
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
    entry->append(entry, context);
}

void itfSetPrimaryFramebufferAlphaFlag(u8 value, u32 kind) {
    itfGridDrawBooleanDescriptor(value, 0, kind);
}

void itfSubmitToggledGridWord(u64 data, s32 alternate, s32 kind) {
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
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
    entry->append(entry, context);
}

void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind) {
    itfSubmitToggledGridWord(data, 0, kind);
}

void sdfSubmitGsAlphaRegisterPacket(s32 data, s32 alternate, s32 kind) {
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
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
    entry->append(entry, context);
}

void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind) {
    sdfSubmitGsAlphaRegisterPacket(data, 0, kind);
}

void sdfSubmitGsPabeRegisterPacket(s32 data, s32 kind) {
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    descriptor[1] = 0x49;
    descriptor[0] = data;
    context = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, (u32)packet);
    entry = &kwlnDrawSurfaces[kind];
    entry->append(entry, context);
}

void sdfSubmitGsTexRegisterPacket(s32 data, s32 kind) {
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(1, 1));
    u64 *descriptor;
    SdfListHead *context;
    SdfPoolNode *entry;

    sdfConsInitPacketHeader(packet, 0, 1, 0xE, 1);
    descriptor = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    descriptor[1] = 0x14;
    descriptor[0] = data;
    context = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(context);
    sdfAppendPacket(context, (u32)packet);
    entry = &kwlnDrawSurfaces[kind];
    entry->append(entry, context);
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
    itfDrawColoredTriangle(xCoordinates, yCoordinates, z, vertexColors, surfaceIndex, extraA, extraB, extraC);
}

typedef struct GridPackedTriangleVertex {
    u64 channels[2];
    u64 xy;
    u64 depth;
} GridPackedTriangleVertex;

void itfDrawColoredTriangle(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 surfaceIndex)
{
    SdfDrawPacket *packet;
    GridPackedTriangleVertex *vertices;
    SdfListHead *list;
    SdfPoolNode *surface;

    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(6, 1));
    sdfConsInitPacketHeader(packet, 0x14B, 6, 0x515151, 1);
    vertices = (GridPackedTriangleVertex *)sdfConsMeasurePacketWithHeader((s32)packet);
    itfGridUnpackColorChannels(vertices[0].channels, colors[0]);
    vertices[0].xy = (xs[0] + 0x7000) | ((u64)(ys[0] + 0x7900) << 32);
    vertices[0].depth = z;
    itfGridUnpackColorChannels(vertices[1].channels, colors[1]);
    vertices[1].xy = (xs[1] + 0x7000) | ((u64)(ys[1] + 0x7900) << 32);
    vertices[1].depth = z;
    itfGridUnpackColorChannels(vertices[2].channels, colors[2]);
    vertices[2].xy = (xs[2] + 0x7000) | ((u64)(ys[2] + 0x7900) << 32);
    vertices[2].depth = z;
    list = (SdfListHead *)sdfAllocPacketAligned(sizeof(SdfListHead));
    sdfInitPacketList(list);
    sdfAppendPacket(list, (u32)packet);
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->append(surface, list);
}

extern void itfDrawColoredRectangle(u32, u32, u32, u32, u32, const u32 *, u32, u32);

/* Draw a rectangle as a four-vertex triangle strip with one packed color. */
void uiDrawUniformRgbaRange(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 gsContext, u32 surfaceIndex) {
    u32 vertexColors[4] = {color, color, color, color};
    itfDrawColoredRectangle(x, y, z, width, height, vertexColors, gsContext, surfaceIndex);
}

void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex) {
    uiDrawUniformRgbaRange(x, y, z, width, height, color, 0, surfaceIndex);
}

typedef struct GridPackedStripVertex {
    u64 channels[2];
    u64 xy;
    u64 depth;
} GridPackedStripVertex;

void itfDrawColoredRectangle(u32 x, u32 y, u32 z, u32 width, u32 height, const u32 *colors, u32 gsContext, u32 surfaceIndex) {
    u32 left = x + 0x7000;
    u32 top = y + 0x7900;
    u64 topWord = (u64)top << 32;
    SdfDrawPacket *packet;
    GridPackedStripVertex *vertices;
    SdfListHead *list;
    SdfPoolNode *surface;
    u64 right, bottom;

    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(8, 1));
    sdfConsInitPacketHeader(packet, 0x14C | (gsContext << 9), 8, 0x51515151, 1);
    vertices = (GridPackedStripVertex *)sdfConsMeasurePacketWithHeader((s32)packet);
    itfGridUnpackColorChannels(vertices[0].channels, colors[0]);
    vertices[0].xy = (u64)left | topWord;
    vertices[0].depth = z;
    itfGridUnpackColorChannels(vertices[1].channels, colors[1]);
    right = (u32)(left + width);
    vertices[1].xy = right | topWord;
    vertices[1].depth = z;
    itfGridUnpackColorChannels(vertices[2].channels, colors[2]);
    bottom = (u64)(top + height) << 32;
    vertices[2].xy = (u64)left | bottom;
    vertices[2].depth = z;
    itfGridUnpackColorChannels(vertices[3].channels, colors[3]);
    vertices[3].xy = right | bottom;
    vertices[3].depth = z;
    list = (SdfListHead *)sdfAllocPacketAligned(sizeof(SdfListHead));
    sdfInitPacketList(list);
    sdfAppendPacket(list, (u32)packet);
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->append(surface, list);
}

void uiDrawGradientColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, const u32 *vertexColors, u32 surfaceIndex) {
    itfDrawColoredRectangle(x, y, z, width, height, vertexColors, 0, surfaceIndex);
}

/* Draw four frame edges; the bottom edge extends 16 units beyond the right side. */
void uiDrawFrameEdges(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 context) {
    uiDrawUniformColorLine(x, y, z, x + width, y, z, color, context);
    uiDrawUniformColorLine(x, y, z, x, y + height, z, color, context);
    uiDrawUniformColorLine(x + width, y, z, x + width, y + height, z, color, context);
    uiDrawUniformColorLine(x, y + height, z, x + width + 0x10, y + height, z, color, context);
}

void itfDrawGradientLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                  const u32 *colors, u32 surfaceIndex);

void uiDrawUniformColorLine(u32 startX, u32 startY, u32 startZ, u32 endX, u32 endY, u32 endZ, u32 color, u32 surfaceIndex) {
    u32 vertexColors[2] = {color, color};
    itfDrawGradientLine(startX, startY, startZ, endX, endY, endZ, vertexColors, surfaceIndex);
}

typedef struct GridPackedLineVertex {
    u64 channels[2];
    u64 xy;
    u64 depth;
} GridPackedLineVertex;

void itfDrawGradientLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                  const u32 *colors, u32 surfaceIndex)
{
    SdfDrawPacket *packet;
    GridPackedLineVertex *vertices;
    SdfListHead *list;
    SdfPoolNode *surface;

    packet = (SdfDrawPacket *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(4, 1));
    sdfConsInitPacketHeader(packet, 0x149, 4, 0x5151, 1);
    vertices = (GridPackedLineVertex *)sdfConsMeasurePacketWithHeader((s32)packet);
    itfGridUnpackColorChannels(vertices[0].channels, colors[0]);
    vertices[0].xy = (x0 + 0x7000) | ((u64)(y0 + 0x7900) << 32);
    vertices[0].depth = z0;
    itfGridUnpackColorChannels(vertices[1].channels, colors[1]);
    vertices[1].xy = (x1 + 0x7000) | ((u64)(y1 + 0x7900) << 32);
    vertices[1].depth = z1;
    list = (SdfListHead *)sdfAllocPacketAligned(sizeof(SdfListHead));
    sdfInitPacketList(list);
    sdfAppendPacket(list, (u32)packet);
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->append(surface, list);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1228);

extern s32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_002D4C80(s32, SdfDmaReferenceChainPacket *, s32);

extern void func_002D4CC8(s32, SdfDmaReferenceChainPacket *, s32);

void uiDrawActiveSurfaceRegion(s32 surfaceIndex) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfDmaReferenceChainPacket *texture;
    sdfInitPacketList(list);
    texture = (SdfDmaReferenceChainPacket *)sdfAllocPacketAligned(0x40);
    func_002D4C80((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList(list, (u32)texture);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[surfaceIndex];
        surface->append(surface, list);
    }
}

void sdfDispatchSurfaceWithPreparedTexturePacket(s32 surfaceIndex) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfDmaReferenceChainPacket *texture;
    sdfInitPacketList(list);
    texture = (SdfDmaReferenceChainPacket *)sdfAllocPacketAligned(0x40);
    func_002D4CC8((s32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40), texture, 0);
    sdfAppendDmaTagToList(list, (u32)texture);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[surfaceIndex];
        surface->append(surface, list);
    }
}

void uiDrawTexturedSurfaceAtFarDepth(s32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, surface);
    uiDrawActiveSurfaceRegion(surface);
}
