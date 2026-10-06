#include "common.h"
#include "eff_blur.h"
#include "eff.h"
#include "pcp_vu0.h"

#define EFF_EVENT_VECTOR_COMPONENTS 4
#define EFF_EVENT_BEZIER_WEIGHT_COUNT 4
#define EFF_EVENT_BEZIER_SLOT_BYTES 0x60
#define EFF_EVENT_SLOT_HEADER_BYTES 0xC
#define EFF_EVENT_CURVE_POINT_STRIDE 3
#define EFF_EVENT_CURVE_FINISHED_INDEX 7
#define EFF_EVENT_CURVE_DEFAULT_STEP 0.05f
#define EFF_EVENT_CURVE_MIDDLE_FACTOR 3.0f
#define EFF_EVENT_DRAW_LIST_BYTES 0x20
#define EFF_EVENT_PROJECTED_X_BIAS 0x700
#define EFF_EVENT_PROJECTED_Y_BIAS 0xF20
#define EFF_EVENT_PROJECTED_Y_SCALE 2
#define EFF_EVENT_GS_X_BIAS 0x7000
#define EFF_EVENT_GS_Y_BIAS 0x7900
#define EFF_EVENT_GS_X_SHIFT 4
#define EFF_EVENT_GS_Y_SHIFT 3
#define EFF_EVENT_GS_X_SCALE 0x10
#define EFF_EVENT_GS_Y_SCALE 8
#define EFF_EVENT_OVERLAY_DEPTH 0xFF0000
#define EFF_EVENT_MARKER_WIDTH 0x20 /* GS coordinate units, not pixels. */
#define EFF_EVENT_MARKER_HEIGHT 0x10 /* GS coordinate units, not pixels. */
#define EFF_EVENT_MARKER_COLOR 0x60008080
#define EFF_EVENT_BLUR_SOURCE_BYTES 0x28
#define EFF_EVENT_BLUR_RESOURCE_INDEX 2
#define EFF_EVENT_STAGGERED_RESOURCE_INDEX 3
#define EFF_EVENT_SQUARE_RESOURCE_INDEX 0
#define EFF_EVENT_STAGGERED_SLOT_COUNT 4
#define EFF_EVENT_NEUTRAL_COLOR 0x80808080
#define EFF_EVENT_EVENT_RECORD_BYTES 0x30
#define EFF_EVENT_COMPACT_WORK_BYTES 0x38
#define EFF_EVENT_HALF_TURN 3.14159265f
#define EFF_EVENT_RANDOM_MIDPOINT 0.5f
#define EFF_EVENT_JITTER_RANGE 0.3f
#define EFF_EVENT_JITTER_BASE 0.7f


extern void *memcpy(void *, const void *, u32);



/* Packet-source layouts and concrete blur owners mirror their constructors.
 * Equal-sized parameter prefixes do not make the blur variants interchangeable. */

typedef struct EffScreenDrawParams {
    EffBlurQuad source;
    u8 pad28[8];
} EffScreenDrawParams;

typedef struct EffSolidRectParams {
    u32 color;
    s32 blendControl;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffSolidRectParams;

typedef struct EffBlurTemplateBody {
    s32 extent;
    EffBlurQuad source;
} EffBlurTemplateBody;

typedef struct EffBlurTemplate {
    EffBlurTemplateBody body;
    u32 resourceWord;
} EffBlurTemplate;






extern EffScreenDrawParams effBlurRectangleParameters;


extern EffBlurTemplate *effBlurPixelWork;

extern s32 effGetResourceFirstWord(s32 index);

extern EffBlurScatterWork *effFilterBlurWork;

extern EffBlurScaleWork *effStaggeredBlurWork;

extern EffScreenDrawParams D_003B2238;


extern EffSolidRectParams effColorRectangleParameters;


extern EffResourceRectWork *effTexturedSquareWork;

extern u8 D_003B2208[];

extern u8 D_003B21D8[];

extern u8 D_003B2278[];

extern u8 D_003B22A0[];

extern EffBlurTemplate *effCloneBlurTemplate(void *arg);


extern EffResourceRectWork *effCloneResourceTemplate(void *arg);

extern EffBlurScaleWork *effCloneBlurWorkWithSlots(void *arg);

extern EffScreenDrawParams D_003B22D0;


extern EffBlurTemplateBody D_003B2428;

extern EffBlurScatterParams D_003B25A0;

extern EffScreenDrawParams D_003B2778;

extern EffSolidRectParams D_003B28B8;


extern EffResourceRectParams D_003B29B8;

extern EffBlurScaleParams D_003B2AF8;

extern s8 effTexturedSquareEnabled;

extern s8 effColorRectangleEnabled;

extern s8 D_00436463;

extern s8 effStaggeredBlurEnabled;

extern s8 effFilterBlurEnabled;

extern s8 effTexturedBlurEnabled;

extern s8 effRectangleBlurEnabled;

extern void effDrawBlurRectangle(EffScreenDrawParams *arg);

extern void effDrawBlurPixelRectWithResource(EffBlurTemplate *arg);


extern void effBlurStepScaleSlotsAndDraw(EffBlurScaleWork *arg);

extern void effBlurDrawFramebufferQuad(EffScreenDrawParams *arg);

extern void func_0018F840(EffSolidRectParams *arg);

extern void effResourceRectDrawPixels(EffResourceRectWork *arg);

extern s32 sdfAllocGeneralBlock(s32);
extern u8 *sdfResourceRetainAddress(s32);

/* Allocate contiguous slots followed by their count and allocation handle. */
EffArrHdr *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * EFF_EVENT_BEZIER_SLOT_BYTES;
    s32 handle = sdfAllocGeneralBlock(slotBytes + EFF_EVENT_SLOT_HEADER_BYTES);
    EffSegmentedBezierSlot *slot = (EffSegmentedBezierSlot *)sdfResourceRetainAddress(handle);
    EffArrHdr *table = (EffArrHdr *)((u8 *)slot + slotBytes);
    u32 index = 0;
    table->allocation = (void *)handle;
    table->slots = slot;
    table->unk4 = count; /* The shared array header's slot count. */
    if (count != 0) {
        do {
            index++;
            slot->pointIndex = 0;
            slot->t = 0;
            slot->parameterStep = EFF_EVENT_CURVE_DEFAULT_STEP;
            slot++;
        } while (index < count);
    }
    return table;
}

/* Release the allocation handle stored after the contiguous slot array. */
void effReleaseSlotArrayAllocation(EffArrHdr *header) {
    sdfReleaseResourceAllocation(header->allocation);
}

/* Return 0 before evaluation only for the point-index sentinel 7. Overflow t
 * clamps to 1 and advances the index by three; there is no general bounds check. */
s32 effStepActiveBezierSlot(EffArrHdr *table, s32 index, f32 *out) {
    EffSegmentedBezierSlot *slot = &((EffSegmentedBezierSlot *)table->slots)[index];
    u32 pointIndex = slot->pointIndex;
    f32 weights[EFF_EVENT_BEZIER_WEIGHT_COUNT];
    f32 t;
    f32 u;
    EffBezierPoint *controlPoints;

    if (pointIndex == EFF_EVENT_CURVE_FINISHED_INDEX) {
        return 0;
    }
    t = slot->t;
    u = 1.0f - t;
    controlPoints = &slot->controlPoints[pointIndex];
    weights[0] = u * u * u;
    weights[1] = t * (u * u) * EFF_EVENT_CURVE_MIDDLE_FACTOR;
    weights[2] = t * t * u * EFF_EVENT_CURVE_MIDDLE_FACTOR;
    weights[3] = t * t * t;
    out[0] = controlPoints[0].x * weights[0] + controlPoints[1].x * weights[1] + controlPoints[2].x * weights[2] + controlPoints[3].x * weights[3];
    out[1] = controlPoints[0].y * weights[0] + controlPoints[1].y * weights[1] + controlPoints[2].y * weights[2] + controlPoints[3].y * weights[3];
    out[2] = controlPoints[0].z * weights[0] + controlPoints[1].z * weights[1] + controlPoints[2].z * weights[2] + controlPoints[3].z * weights[3];
    out[3] = 1.0f;
    t += slot->parameterStep;
    if (t > 1.0f) {
        t = 1.0f;
        pointIndex += EFF_EVENT_CURVE_POINT_STRIDE;
    }
    slot->t = t;
    slot->pointIndex = pointIndex;
    return 1;
}

/* Evaluate at the entry t before advancing. The first segment carries excess t;
 * the second stores t = 1 and returns 0 after producing output at the entry t. */
s32 effStepBezierSlotSegment(EffSegmentedBezierSlot *slot, f32 *out) {
    f32 weights[EFF_EVENT_BEZIER_WEIGHT_COUNT];
    u32 pointIndex = slot->pointIndex;
    f32 t = slot->t;
    EffBezierPoint *controlPoints = &slot->controlPoints[pointIndex];
    f32 u = 1.0f - t;

    weights[0] = u * u * u;
    weights[1] = t * (u * u) * EFF_EVENT_CURVE_MIDDLE_FACTOR;
    weights[2] = t * t * u * EFF_EVENT_CURVE_MIDDLE_FACTOR;
    weights[3] = t * t * t;
    out[0] = controlPoints[0].x * weights[0] + controlPoints[1].x * weights[1] + controlPoints[2].x * weights[2] + controlPoints[3].x * weights[3];
    out[1] = controlPoints[0].y * weights[0] + controlPoints[1].y * weights[1] + controlPoints[2].y * weights[2] + controlPoints[3].y * weights[3];
    out[2] = controlPoints[0].z * weights[0] + controlPoints[1].z * weights[1] + controlPoints[2].z * weights[2] + controlPoints[3].z * weights[3];
    out[3] = 1.0f;
    t += slot->parameterStep;
    if (t > 1.0f) {
        if (pointIndex < EFF_EVENT_CURVE_POINT_STRIDE) {
            t -= 1.0f;
            pointIndex += EFF_EVENT_CURVE_POINT_STRIDE;
        } else {
            slot->t = 1.0f;
            return 0;
        }
    }
    slot->pointIndex = pointIndex;
    slot->t = t;
    return 1;
}

/* Evaluate four control points starting at pointIndex into xyz with w = 1;
 * unlike the stepping paths, this does not update t or the point index. */
void effEvaluateSlotBezierPosition(EffSegmentedBezierSlot *slot, f32 *out) {
    EffBezierPoint *controlPoints = &slot->controlPoints[slot->pointIndex];
    f32 t = slot->t;
    f32 u = 1.0f - t;
    f32 w[EFF_EVENT_BEZIER_WEIGHT_COUNT];

    w[0] = u * u * u;
    w[1] = t * (u * u) * EFF_EVENT_CURVE_MIDDLE_FACTOR;
    w[2] = t * t * u * EFF_EVENT_CURVE_MIDDLE_FACTOR;
    w[3] = t * t * t;

    out[0] = controlPoints[0].x * w[0] + controlPoints[1].x * w[1] + controlPoints[2].x * w[2] + controlPoints[3].x * w[3];
    out[1] = controlPoints[0].y * w[0] + controlPoints[1].y * w[1] + controlPoints[2].y * w[2] + controlPoints[3].y * w[3];
    out[2] = controlPoints[0].z * w[0] + controlPoints[1].z * w[1] + controlPoints[2].z * w[2] + controlPoints[3].z * w[3];
    out[3] = 1.0f;
}

/* Reset only curve progress and its default step; retain all control points. */
void effInitSlotTail(EffArrHdr *table, s32 index) {
    EffSegmentedBezierSlot *slot = &((EffSegmentedBezierSlot *)table->slots)[index];

    slot->parameterStep = EFF_EVENT_CURVE_DEFAULT_STEP;
    slot->pointIndex = slot->t = 0;
}

/* Return the selected slot address through the native signed-word interface. */
s32 effGetSlotAt(EffArrHdr *table, s32 index) {
    return (s32)&((EffSegmentedBezierSlot *)table->slots)[index];
}

extern SdfPoolNode kwlnPositionedTextSurface;
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, s32);
extern void sdfAppendPacket(void *, void *);
extern void *func_0011F250(s32, s32, s32, s32, s32, s32, s32);
extern void sdfProjectVuVectorToScreen();

/* Project a point and draw a filled/bordered marker with 0x20/0x10 GS-unit extents. */
void effDrawMarkerBoxAtPoint(f32 *position) {
    void *list = sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 screen[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, position);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, screen);
    x = (s32)screen[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y = (s32)screen[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x;
    pixel[1] = y;
    sdfAppendPacket(list, func_0011F250((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_WIDTH, EFF_EVENT_MARKER_HEIGHT, EFF_EVENT_MARKER_COLOR, EFF_EVENT_MARKER_COLOR));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)list);
}

/* Project a point and use color for both the marker's fill and border. */
void effDrawColoredBoxAtPoint(f32 *position, s32 color) {
    void *list = sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 screen[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, position);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, screen);
    x = (s32)screen[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y = (s32)screen[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x;
    pixel[1] = y;
    sdfAppendPacket(list, func_0011F250((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_WIDTH, EFF_EVENT_MARKER_HEIGHT, color, color));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)list);
}

extern void *func_0011F3D8(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Draw a GS line between two projected points, using the fixed marker color. */
void effDrawMarkerLineBetweenPoints(f32 *from, f32 *to) {
    void *list = sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 start[EFF_EVENT_VECTOR_COMPONENTS];
    f32 end[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, from);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, start);
    VU0_LOAD_VF(vf10, to);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, end);
    x0 = (s32)start[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y0 = (s32)start[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    x1 = (s32)end[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y1 = (s32)end[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x0;
    pixel[1] = y0;
    pixel[2] = x1;
    pixel[3] = y1;
    sdfAppendPacket(list, func_0011F3D8((x0 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y0 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_COLOR, (x1 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y1 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_COLOR, 0));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)list);
}

/* Draw a GS line between two projected points, using color at both vertices. */
void effDrawColoredLineBetweenPoints(f32 *from, f32 *to, s32 color) {
    void *list = sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 start[EFF_EVENT_VECTOR_COMPONENTS];
    f32 end[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, from);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, start);
    VU0_LOAD_VF(vf10, to);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, end);
    x0 = (s32)start[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y0 = (s32)start[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    x1 = (s32)end[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y1 = (s32)end[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x0;
    pixel[1] = y0;
    pixel[2] = x1;
    pixel[3] = y1;
    sdfAppendPacket(list, func_0011F3D8((x0 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y0 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, color, (x1 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y1 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, color, 0));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)list);
}

/* Submit the native command at biased screen coordinates; payload args stay opaque. */
void effSubmitPositionedDrawPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    void *list = sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    sdfInitPacketList(list);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x * EFF_EVENT_GS_X_SCALE + EFF_EVENT_GS_X_BIAS, y * EFF_EVENT_GS_Y_SCALE + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, arg2, arg3));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)list);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00196740);

INCLUDE_ASM(const s32, "effect/effEvent", func_001969B8);

INCLUDE_ASM(const s32, "effect/effEvent", func_00196B08);

extern void *func_0011F250(s32, s32, s32, s32, s32, s32, s32);

/* Submit a filled rectangle and its line-strip border. Keep native width/height
 * scaling and the distinct shift/multiply forms used by the other draw paths. */
void effSubmitSizedDrawPacket(s32 x, s32 y, s32 width, s32 height, s32 fillColor, s32 borderColor) {
    void *list = sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0011F250(x * EFF_EVENT_GS_X_SCALE + EFF_EVENT_GS_X_BIAS, y * EFF_EVENT_GS_Y_SCALE + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, width * EFF_EVENT_GS_X_SCALE, height * EFF_EVENT_GS_Y_SCALE, fillColor, borderColor));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)list);
}

void effEnableRectangleBlur(void) {
    effRectangleBlurEnabled = 1;
}

void effDisableRectangleBlur(void) {
    effRectangleBlurEnabled = 0;
}

/* Copy only the 0x28-byte source prefix; retain the screen record's trailing bytes. */
void effCopyRectangleBlurParameters(void *parameters) {
    memcpy(&effBlurRectangleParameters, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the rectangle-blur parameter block by address. */
EffScreenDrawParams *effGetCh70Params(void) {
    return &effBlurRectangleParameters;
}

void effEnableTexturedBlur(void) {
    effTexturedBlurEnabled = 1;
}

void effDisableTexturedBlur(void) {
    effTexturedBlurEnabled = 0;
}

/* Copy the pixel rectangle only; retain its selected source resource. */
void effCopyTexturedBlurParameters(EffBlurTemplateBody *parameters) {
    effBlurPixelWork->body = *parameters;
}

/* Return the current textured-blur work without retaining it. */
EffBlurTemplate *effGetCh71Work(void) {
    return effBlurPixelWork;
}

void effSetCh71Id(u32 resourceWord) {
    effBlurPixelWork->resourceWord = resourceWord;
}

/* Select the same resource-table entry used by the filter-blur initializer. */
void effInitCh71Id(void) {
    effBlurPixelWork->resourceWord = effGetResourceFirstWord(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void effEnableFilterBlur(void) {
    effFilterBlurEnabled = 1;
}

void effDisableFilterBlur(void) {
    effFilterBlurEnabled = 0;
}

/* Update scatter parameters without replacing the owned allocation or slots. */
void effCopyFilterBlurParameters(EffBlurScatterParams *parameters) {
    effFilterBlurWork->params = *parameters;
}

/* Return the current filter-blur work without retaining it. */
EffBlurScatterWork *effGetCh72Work(void) {
    return effFilterBlurWork;
}

void effSetCh72Id(u32 sourceHandle) {
    effFilterBlurWork->sourceHandle = sourceHandle;
}

/* Install the filter-blur resource-table word without changing its slots. */
void effInitCh72Id(void) {
    effFilterBlurWork->sourceHandle = effGetResourceFirstWord(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void effEnableStaggeredBlur(void) {
    effStaggeredBlurEnabled = 1;
}

void effDisableStaggeredBlur(void) {
    effStaggeredBlurEnabled = 0;
}

/* Update scale parameters without replacing the owned allocation or slots. */
void effCopyStaggeredBlurParameters(EffBlurScaleParams *parameters) {
    effStaggeredBlurWork->params = *parameters;
}

/* Return the current staggered-blur work without retaining it. */
EffBlurScaleWork *effGetCh76Work(void) {
    return effStaggeredBlurWork;
}

void effSetCh76Id(u32 sourceHandle) {
    effStaggeredBlurWork->sourceHandle = sourceHandle;
}

/* Install the staggered-blur resource-table word without changing its slots. */
void effInitCh76Id(void) {
    effStaggeredBlurWork->sourceHandle = effGetResourceFirstWord(EFF_EVENT_STAGGERED_RESOURCE_INDEX);
}

void effEnableFramebufferQuad(void) {
    D_00436463 = 1;
}

void effDisableFramebufferQuad(void) {
    D_00436463 = 0;
}

/* Copy only the framebuffer source prefix, not the trailing screen-record bytes. */
void effCopyFramebufferQuadParameters(void *parameters) {
    memcpy(&D_003B2238, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the framebuffer-quad parameter block by address. */
EffScreenDrawParams *effGetCh73Params(void) {
    return &D_003B2238;
}

void effEnableColorRectangle(void) {
    effColorRectangleEnabled = 1;
}

void effDisableColorRectangle(void) {
    effColorRectangleEnabled = 0;
}

/* Replace the complete solid-rectangle parameter block. */
void effCopyColorRectangleParameters(EffSolidRectParams *parameters) {
    effColorRectangleParameters = *parameters;
}

/* Expose the solid-rectangle parameter block by address. */
EffSolidRectParams *effGetCh74Params(void) {
    return &effColorRectangleParameters;
}

void effEnableTexturedSquare(void) {
    effTexturedSquareEnabled = 1;
}

void effDisableTexturedSquare(void) {
    effTexturedSquareEnabled = 0;
}

/* Copy the resource template body while preserving its selected resource. */
void effCopyTexturedSquareParameters(EffResourceRectParams *parameters) {
    effTexturedSquareWork->params = *parameters;
}

/* Return the current textured-square work without retaining it. */
EffResourceRectWork *effGetCh75Work(void) {
    return effTexturedSquareWork;
}

void effSetCh75Id(u32 resourceWord) {
    effTexturedSquareWork->sourceHandle = resourceWord;
}

/* Install the textured-square resource-table word while retaining its body. */
void effInitCh75Id(void) {
    effTexturedSquareWork->sourceHandle = effGetResourceFirstWord(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}

/* Clone the four default work templates; only the staggered slot count is overridden. */
void effInitWorks(void) {
    effBlurPixelWork = effCloneBlurTemplate(D_003B2208);
    effFilterBlurWork = func_0018EBC8(D_003B21D8);
    effTexturedSquareWork = effCloneResourceTemplate(D_003B2278);
    effStaggeredBlurWork = effCloneBlurWorkWithSlots(D_003B22A0);
    effGetCh76Work()->params.count = EFF_EVENT_STAGGERED_SLOT_COUNT;
}

/* Dispatch enabled draw families independently, in their native order. */
void effDispatchActive(void) {
    if (effRectangleBlurEnabled) {
        effDrawBlurRectangle(&effBlurRectangleParameters);
    }
    if (effTexturedBlurEnabled) {
        effDrawBlurPixelRectWithResource(effBlurPixelWork);
    }
    if (effFilterBlurEnabled) {
        func_0018ECD0(effFilterBlurWork);
    }
    if (effStaggeredBlurEnabled) {
        effBlurStepScaleSlotsAndDraw(effStaggeredBlurWork);
    }
    if (D_00436463) {
        effBlurDrawFramebufferQuad(&D_003B2238);
    }
    if (effColorRectangleEnabled) {
        func_0018F840(&effColorRectangleParameters);
    }
    if (effTexturedSquareEnabled) {
        effResourceRectDrawPixels(effTexturedSquareWork);
    }
}

/* Optional callback/copy preparation shared by the seven setup paths below.
 * callbackResult must be valid whenever callback is present. */
typedef struct EffLoader {
    u8 pad00[0x1C];
    void *source;
    void *destination;
    u32 copyBytes;
    u8 pad28[4];
    s32 (*callback)(void *);
    u8 pad30[8];
    s32 *callbackResult;
} EffLoader;

extern EffLoader D_003B23E8;
extern s8 D_004364AD;
extern s8 D_0040B7DB[];
extern void func_00194A08();
extern void func_00194A28();
extern void func_00194A30();
extern void func_00194A38();

/* Prepare once per flag cycle, then run the registered operations while ready.
 * A negative control byte clears readiness only after those operations. */
s8 effUpdateCh72Params(void) {
    if (D_004364AD == 0) {
        if (D_003B23E8.callback != 0) {
            *D_003B23E8.callbackResult = D_003B23E8.callback(D_003B23E8.source);
        }
        if (D_003B23E8.destination != 0 && D_003B23E8.source != 0) {
            memcpy(D_003B23E8.destination, D_003B23E8.source, D_003B23E8.copyBytes);
        }
        D_004364AD = 1;
    }
    if ((u8)D_004364AD != 0) {
        func_00194A08(&D_003B23E8);
        func_00194A30();
        func_00194A28(&D_003B23E8);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364AD = 0;
    }
    return D_004364AD;
}

/* Expose this screen-draw setup block; no parameter copy is made. */
EffScreenDrawParams *effGetLoadDescA(void) {
    return &D_003B22D0;
}

/* Replace the screen-draw source prefix while preserving the trailing bytes. */
void func_001975F8(void *parameters) {
    memcpy(&D_003B22D0, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffLoader D_003B2560;
extern s8 D_004364BD;

/* Prepare the blur-template callback from its explicit parameter block,
 * then process the channel; its optional copy still uses source/destination. */
s8 effEventAdvanceBlurTemplateSetup(void) {
    if (D_004364BD == 0) {
        if (D_003B2560.callback != 0) {
            *D_003B2560.callbackResult = D_003B2560.callback(&D_003B2428);
        }
        if (D_003B2560.destination != 0 && D_003B2560.source != 0) {
            memcpy(D_003B2560.destination, D_003B2560.source, D_003B2560.copyBytes);
        }
        D_004364BD = 1;
    }
    if ((u8)D_004364BD != 0) {
        func_00194A08(&D_003B2560);
        func_00194A30();
        func_00194A28(&D_003B2560);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364BD = 0;
    }
    return D_004364BD;
}

/* Expose the blur-template setup parameters by address. */
EffBlurTemplateBody *effEventGetBlurTemplateSetupParams(void) {
    return &D_003B2428;
}

/* Replace the setup template; this does not modify the active blur work. */
void effEventSetBlurTemplateParameters(EffBlurTemplateBody *parameters) {
    D_003B2428 = *parameters;
}

extern EffLoader D_003B2738;
extern s8 D_004364EF;

/* Prepare scatter setup from its explicit parameters, then process the channel. */
s8 effEventAdvanceScatterBlurSetup(void) {
    if (D_004364EF == 0) {
        if (D_003B2738.callback != 0) {
            *D_003B2738.callbackResult = D_003B2738.callback(&D_003B25A0);
        }
        if (D_003B2738.destination != 0 && D_003B2738.source != 0) {
            memcpy(D_003B2738.destination, D_003B2738.source, D_003B2738.copyBytes);
        }
        D_004364EF = 1;
    }
    if ((u8)D_004364EF != 0) {
        func_00194A08(&D_003B2738);
        func_00194A30();
        func_00194A28(&D_003B2738);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364EF = 0;
    }
    return D_004364EF;
}

/* Expose the scatter-blur setup parameters by address. */
EffBlurScatterParams *effEventGetScatterBlurSetupParams(void) {
    return &D_003B25A0;
}

/* Replace setup parameters without replacing the active filter-blur work. */
void effEventSetScatterBlurParameters(EffBlurScatterParams *parameters) {
    D_003B25A0 = *parameters;
}

extern EffLoader D_003B2878;
extern s8 D_00436504;

/* Prepare from the channel's source, process it, then honor the control-byte reset. */
s8 func_001978B8(void) {
    if (D_00436504 == 0) {
        if (D_003B2878.callback != 0) {
            *D_003B2878.callbackResult = D_003B2878.callback(D_003B2878.source);
        }
        if (D_003B2878.destination != 0 && D_003B2878.source != 0) {
            memcpy(D_003B2878.destination, D_003B2878.source, D_003B2878.copyBytes);
        }
        D_00436504 = 1;
    }
    if ((u8)D_00436504 != 0) {
        func_00194A08(&D_003B2878);
        func_00194A30();
        func_00194A28(&D_003B2878);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_00436504 = 0;
    }
    return D_00436504;
}

/* Expose the other screen-draw setup block by address. */
EffScreenDrawParams *effGetLoadDescD(void) {
    return &D_003B2778;
}

/* Replace this screen-draw source prefix without changing trailing bytes. */
void func_00197980(void *parameters) {
    memcpy(&D_003B2778, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffLoader D_003B2978;
extern s8 D_00436517;

/* Prepare solid-rectangle setup from the channel source, then process it. */
s8 effEventAdvanceSolidRectangleSetup(void) {
    if (D_00436517 == 0) {
        if (D_003B2978.callback != 0) {
            *D_003B2978.callbackResult = D_003B2978.callback(D_003B2978.source);
        }
        if (D_003B2978.destination != 0 && D_003B2978.source != 0) {
            memcpy(D_003B2978.destination, D_003B2978.source, D_003B2978.copyBytes);
        }
        D_00436517 = 1;
    }
    if ((u8)D_00436517 != 0) {
        func_00194A08(&D_003B2978);
        func_00194A30();
        func_00194A28(&D_003B2978);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_00436517 = 0;
    }
    return D_00436517;
}

/* Expose the solid-rectangle setup parameters by address. */
EffSolidRectParams *effEventGetSolidRectangleSetupParams(void) {
    return &D_003B28B8;
}

/* Replace the setup block rather than the active solid-rectangle parameters. */
void effEventSetSolidRectangleParameters(EffSolidRectParams *parameters) {
    D_003B28B8 = *parameters;
}

extern EffLoader D_003B2AB8;
extern s8 D_0043651C;

/* Prepare resource-template setup from its explicit body, then process the channel. */
s8 effEventAdvanceResourceTemplateSetup(void) {
    if (D_0043651C == 0) {
        if (D_003B2AB8.callback != 0) {
            *D_003B2AB8.callbackResult = D_003B2AB8.callback(&D_003B29B8);
        }
        if (D_003B2AB8.destination != 0 && D_003B2AB8.source != 0) {
            memcpy(D_003B2AB8.destination, D_003B2AB8.source, D_003B2AB8.copyBytes);
        }
        D_0043651C = 1;
    }
    if ((u8)D_0043651C != 0) {
        func_00194A08(&D_003B2AB8);
        func_00194A30();
        func_00194A28(&D_003B2AB8);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_0043651C = 0;
    }
    return D_0043651C;
}

/* Expose the resource-template setup body by address. */
EffResourceRectParams *effEventGetResourceTemplateSetupParams(void) {
    return &D_003B29B8;
}

/* Replace the setup body without replacing the active textured-square work. */
void effEventSetResourceTemplateParameters(EffResourceRectParams *parameters) {
    D_003B29B8 = *parameters;
}

extern EffLoader D_003B2C90;
extern s8 D_0043652F;

/* Prepare scale-blur setup from its explicit parameters, then process the channel. */
s8 effEventAdvanceScaleBlurSetup(void) {
    if (D_0043652F == 0) {
        if (D_003B2C90.callback != 0) {
            *D_003B2C90.callbackResult = D_003B2C90.callback(&D_003B2AF8);
        }
        if (D_003B2C90.destination != 0 && D_003B2C90.source != 0) {
            memcpy(D_003B2C90.destination, D_003B2C90.source, D_003B2C90.copyBytes);
        }
        D_0043652F = 1;
    }
    if ((u8)D_0043652F != 0) {
        func_00194A08(&D_003B2C90);
        func_00194A30();
        func_00194A28(&D_003B2C90);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_0043652F = 0;
    }
    return D_0043652F;
}

/* Expose the scale-blur setup parameters by address. */
EffBlurScaleParams *effEventGetScaleBlurSetupParams(void) {
    return &D_003B2AF8;
}

/* Replace setup parameters without replacing the active staggered-blur work. */
void effEventSetScaleBlurParameters(EffBlurScaleParams *parameters) {
    D_003B2AF8 = *parameters;
}

u32 func_00197D38() {
    return sndMixerClone();
}

void func_00197D50() {
    sndReleaseAllVoices();
}
/* Copied event parameters: position, quaternion and aim dimensions (0x30). */
typedef struct EffEventInit {
    f32 position[4];       /* 0x00 */
    f32 orientation[4];    /* 0x10 */
    f32 range;             /* 0x20 */
    f32 height;            /* 0x24 */
    f32 param;             /* 0x28 */
    u32 color;             /* 0x2C */
} EffEventInit;

/* Compact event owner: the allocator reserves exactly 0x38 bytes. */
typedef struct EffEventWork {
    EffEventInit init;
    void *actor;          /* Linked model/event owner; concrete identity is unknown. */
    void *effect;
} EffEventWork;
extern void *sdfAllocSizeClassBlock(s32 size);
extern void *func_00168548(u32, u16, s32, s32);
extern void func_00169168(void *, f32);

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
EffEventWork *effEventCreate(u32 owner, u16 kind, const EffEventInit *params) {
    EffEventWork *work = sdfAllocSizeClassBlock(EFF_EVENT_COMPACT_WORK_BYTES);

    memcpy(work, params, sizeof(*params));
    work->actor = NULL;
    work->effect = func_00168548(owner, kind, 0, 0);
    func_00169168(work->effect, params->param);
    return work;
}



extern s32 D_00436530;

extern u32 D_00436534;

extern u32 D_00436538;

typedef struct {
    u8 bytes[EFF_EVENT_EVENT_RECORD_BYTES];
} __attribute__((packed)) FileRecordHeader;



extern u8 D_003B2D20[];

/* Release the attached effect before freeing the event work. */
void effEventReleaseNode(EffEventWork *work) {
    effReleaseBattleVoiceOwner(work->effect);
    sdfReleaseChipBlock(work);
}

/* Copy the packed 0x30-byte record while retaining its native packed layout. */
void effEventCopyFileRecordHeader(void *destination, const void *source) {
    *(FileRecordHeader *)destination = *(const FileRecordHeader *)source;
}

/* This copies a packed 0x30-byte record, not a complete 0x60-byte billboard state. */
void effEventCopySerializedRecord(const FileRecordHeader *source, FileRecordHeader *destination) {
    *destination = *source;
}

/* Forward the effect's scale value without modifying the record's init prefix. */
void effEventSetScale(EffEventWork *work, f32 scale) {
    func_00169168(work->effect, scale);
}

void effEventSetState(EffEventWork *work, void *actor) {
    work->actor = actor;
}

extern void func_00197F60(EffEventWork *work);

INCLUDE_ASM(const s32, "effect/effEvent", func_00197F60);


/* 0x3C-byte event holder: a handle, the event it owns, an init block copied to the event. */
typedef struct EffEventLight {
    u32 handle;           /* 0x00 */
    EffEventWork *owner;   /* 0x04 */
    EffEventInit init;    /* 0x08 */
    u8 active;            /* 0x38 */
} EffEventLight; /* 0x3C */

/* Initialize a holder and its effect record; the teardown flag starts set. */
EffEventLight *effEventLightCreate(u32 arg, f32 param) {
    EffEventLight *work = sdfAllocSizeClassBlock(sizeof(EffEventLight));

    work->init.orientation[3] = 1.0f;
    work->init.range = 50.0f;
    work->init.height = 180.0f;
    work->init.position[1] = -90.0f;
    work->init.param = param;
    work->init.color = EFF_EVENT_NEUTRAL_COLOR;
    work->init.orientation[0] = 0;
    work->init.orientation[1] = 0;
    work->init.orientation[2] = 0;
    work->init.position[0] = 0;
    work->init.position[2] = 0;
    work->init.position[3] = 0;
    work->handle = func_00197D38(arg);
    work->owner = effEventCreate(work->handle, 0, &work->init);
    work->active = 1;
    return work;
}

/* Release the effect, invoke shared teardown only when flagged, then free the holder. */
void effEventLightDestroy(EffEventLight *work) {
    effEventReleaseNode(work->owner);
    if (work->active != 0) {
        func_00197D50(work->handle);
    }
    sdfReleaseChipBlock(work);
}

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
EffEventLight *effEventLightClone(EffEventLight *src) {
    EffEventInit *block = &src->init;
    EffEventLight *work = sdfAllocSizeClassBlock(sizeof(EffEventLight));

    work->owner = effEventCreate(src->handle, 0, block);
    work->handle = src->handle;
    work->init = *block;
    work->active = 0;
    return work;
}

/* Apply the native owner operation to the pointer stored in the record prefix. */
void func_00198448(EffEventLight *work) {
    func_00197F60(work->owner);
}

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effEventLightSetPosition(EffEventLight *work, f32 *position) {
    work->init.position[0] = position[0];
    work->init.position[1] = position[1] - work->init.height * 0.5f;
    work->init.position[2] = position[2];
    work->init.position[3] = 0;
    effEventCopyFileRecordHeader((FileRecordHeader *)work->owner, (FileRecordHeader *)&work->init);
}

/* Copy the holder's updated packed tint to its event record. */
void effEventBindEffect(EffEventLight *work, u32 color) {
    work->init.color = color;
    effEventCopyFileRecordHeader((FileRecordHeader *)work->owner, (FileRecordHeader *)&work->init);
}


typedef struct EffAimParams {
    u8 pad0;
    u8 aimMode;           /* 0x01 */
    u8 directionMode;     /* 0x02 */
    u8 pad3;
    s32 rangeOverride;    /* 0x04: zero uses the source range */
} EffAimParams;

extern f32 D_003B2CD0[];
extern f32 D_003B2CE8[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);
extern void effMiscQuaternionToMatrixVU(void);
extern void func_00336818(f32 angle);
extern void sdfComposeVuMatrixFromRegisters(void);

/* vu0 routine: leave the selected aim point in vf10, not a C return value.
 * Preserve the non-camera output scratch w, which is not initialized here. */
void effEventLoadSelectedAimPositionVu(EffEventInit *src, EffAimParams *param) {
    f32 out[EFF_EVENT_VECTOR_COMPONENTS];
    f32 dir[EFF_EVENT_VECTOR_COMPONENTS];
    f32 pos[EFF_EVENT_VECTOR_COMPONENTS];
    f32 radius;
    f32 half;
    f32 y;
    s32 rangeOverride = param->rangeOverride;
    u8 directionMode = param->directionMode;
    u8 aimMode = param->aimMode;

    if (rangeOverride == 0) {
        radius = src->range;
    } else {
        radius = (f32)rangeOverride;
    }
    half = src->height * 0.5f;
    PCP_COPY_VECTOR(pos, src->position);
    if (aimMode == 5) {
        if (directionMode == 8 || directionMode == 10) {
            y = -1.0f;
            if (rangeOverride != 0) {
                y = -radius;
            }
        } else {
            y = -1.0f;
        }
    } else {
        y = pos[1] - D_003B2CD0[aimMode] * half;
        if (directionMode == 8 || directionMode == 10) {
            if (rangeOverride != 0) {
                y -= radius;
            }
        }
    }
    if (directionMode == 9 || aimMode == 4) {
        dir[2] = half < radius ? -radius : -half;
        dir[0] = dir[1] = 0.0f;
        pos[1] = y;
        sdfVuBuildLookAtBasis(pos, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
        VU0_LOAD_VF(vf10, dir);
        VU0_CLEAR_W(vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, pos);
        VU0_ADD(vf10, vf10, vf11);
        return;
    }
    if (directionMode == 8 || directionMode == 10) {
        out[0] = pos[0];
        out[1] = y;
        out[2] = pos[2];
    } else {
        dir[0] = 0.0f;
        dir[2] = 1.0f;
        dir[1] = 0.0f;
        VU0_LOAD_VF(vf10, src->orientation);
        effMiscQuaternionToMatrixVU();
        func_00336818(D_003B2CE8[directionMode]);
        sdfComposeVuMatrixFromRegisters();
        VU0_LOAD_VF(vf10, dir);
        VU0_CLEAR_W(vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        out[0] = pos[0] + radius * dir[0];
        out[1] = y + radius * dir[1];
        out[2] = pos[2] + radius * dir[2];
    }
    VU0_LOAD_VF(vf10, out);
}

/* Billboard emitter parameters (0x7C). Creation/copy clamps startDelaySpread,
   fadeIn and fadeOut to at least one; motionDelaySpread is left unchanged. */
typedef struct {
    f32 position[EFF_EVENT_VECTOR_COMPONENTS];
    u32 particleCount;
    s32 duration;
    s32 startDelaySpread;
    s32 motionDelaySpread;
    f32 spawnRadius;
    u32 color;
    s16 drawMode; /* Passed to the billboard mode setter in the direct-draw path. */
    u8 pad2A[2];
    s32 fadeIn;
    s32 fadeOut;
    u8 repeat;
    u8 pad35[3];
    f32 baseScale;
    f32 scaleRandomness;
    f32 scaleAmplitudeX;
    f32 scaleAmplitudeY;
    f32 scalePhaseStep;
    f32 angularSpeed;
    f32 verticalSpeed;
    f32 verticalSpeedRandomness;
    f32 lateralSpeed;
    f32 lateralSpeedRandomness;
    f32 accelerationPercent; /* Speeds multiply by 1 + this / 100 each motion step. */
    f32 swayPhaseStep;
    f32 swayAmplitude;
    f32 swayAmplitudeStep;
    f32 swayRandomness;
    s32 colorStartAge; /* Batched path: primary texture RGB fades from black. */
    s32 colorTransitionFrames;
} EffEventBillParams;

typedef struct {
    f32 position[3];
    u8 pad0C[4];
    f32 motion[3]; /* X/Z: unit sway direction; Y: accumulated vertical offset. */
    u8 pad1C[4];
    s32 age;
    s32 motionDelay;
    f32 baseScale;
    f32 scalePhaseX;
    f32 scalePhaseStepX;
    f32 scaleAmplitudeX;
    f32 scalePhaseY;
    f32 scalePhaseStepY;
    f32 scaleAmplitudeY;
    f32 rotation;
    f32 angularSpeed;
    f32 swayAmplitude;
    f32 swayPhase;
    f32 verticalSpeed;
    f32 lateralSpeed;
    u8 pad5C[4];
} EffEventBillParticle; /* 0x60 */

typedef struct {
    EffEventBillParams head;
    EffEventBillParticle *particles; /* 0x7C */
    u8 flag;              /* 0x80 */
    u8 pad81[3];
    u32 allocationHandle; /* Owner follows its particles in this allocation. */
} EffEventBillSet; /* 0x88 */

extern void *billCreateFromResource(s32 kind, const char *path);

/* Allocate particles plus their owner, copy parameters and acquire shared textures. */
INCLUDE_RODATA(const s32, "effect/effEvent", D_00414A00);

EffEventBillSet *effEventBillSetCreate(EffEventBillParams *src) {
    u32 particleCount = src->particleCount;
    u32 particleBytes = particleCount * sizeof(EffEventBillParticle);
    u32 allocationHandle = sdfAllocGeneralBlock(particleBytes + sizeof(EffEventBillSet));
    EffEventBillParticle *particle = (EffEventBillParticle *)sdfResourceRetainAddress(allocationHandle);
    EffEventBillSet *work = (EffEventBillSet *)((u8 *)particle + particleBytes);
    u32 i;

    work->head = *src;
    work->allocationHandle = allocationHandle;
    work->particles = particle;
    work->flag = 0;
    if (work->head.fadeIn <= 0) {
        work->head.fadeIn = 1;
    }
    if (work->head.fadeOut <= 0) {
        work->head.fadeOut = 1;
    }
    if (work->head.startDelaySpread <= 0) {
        work->head.startDelaySpread = 1;
    }
    D_00436530 = D_00436530 + 1;
    if (D_00436530 == 1) {
        D_00436534 = (u32)billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_00436538 = (u32)billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
    }
    particleCount = work->head.particleCount;
    for (i = 0; i < particleCount; i++) {
        particle->age = 0;
        particle++;
    }
    return work;
}

/* Drop the shared texture reference count; zero dispatches both resource handles.
 * The retained allocation is released afterwards, regardless of that count. */
void effEventReleaseSharedResources(EffEventBillSet *work) {
    D_00436530 = D_00436530 - 1;
    if (D_00436530 == 0) {
        billDispatchByKind(D_00436534);
        billDispatchByKind(D_00436538);
    }
    sdfReleaseResourceAllocation(work->allocationHandle);
}

extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_003AA868[];

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEventRandomizeBillboardParticle(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *particle = &work->particles[index];
    f32 direction[EFF_EVENT_VECTOR_COMPONENTS];
    f32 scaleFactor;
    f32 spawnRadius;
    s32 startDelaySpread = work->head.startDelaySpread;
    s32 motionDelaySpread = work->head.motionDelaySpread;

    particle->age = -(effMiscRand(D_003AA868) % startDelaySpread);
    particle->motionDelay = -(effMiscRand(D_003AA868) % motionDelaySpread);
    scaleFactor = effMiscRandUnitFloat(D_003AA868) * work->head.scaleRandomness + (1.0f - work->head.scaleRandomness);
    particle->baseScale = work->head.baseScale * scaleFactor;
    particle->scalePhaseX = effMiscRandUnitFloat(D_003AA868) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseY = effMiscRandUnitFloat(D_003AA868) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseStepX = work->head.scalePhaseStep * (effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f);
    particle->scalePhaseStepY = work->head.scalePhaseStep * (effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f);
    particle->scaleAmplitudeX = work->head.scaleAmplitudeX * (effMiscRandUnitFloat(D_003AA868) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->scaleAmplitudeY = work->head.scaleAmplitudeY * (effMiscRandUnitFloat(D_003AA868) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->rotation = effMiscRandUnitFloat(D_003AA868) * (EFF_EVENT_HALF_TURN * 2.0f);
    if (effMiscRand(D_003AA868) & 1) {
        particle->angularSpeed = work->head.angularSpeed * (effMiscRandUnitFloat(D_003AA868) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE);
    } else {
        particle->angularSpeed = -(work->head.angularSpeed * (effMiscRandUnitFloat(D_003AA868) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE));
    }
    spawnRadius = work->head.spawnRadius;
    direction[0] = (effMiscRandUnitFloat(D_003AA868) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[1] = (effMiscRandUnitFloat(D_003AA868) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[2] = (effMiscRandUnitFloat(D_003AA868) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    VU0_LOAD_VF(vf10, direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, direction);
    /* Each coordinate consumes its own radial random factor, not one shared radius. */
    particle->position[0] = spawnRadius * effMiscRandUnitFloat(D_003AA868) * direction[0];
    particle->position[1] = spawnRadius * effMiscRandUnitFloat(D_003AA868) * direction[1];
    particle->position[2] = spawnRadius * effMiscRandUnitFloat(D_003AA868) * direction[2];
    particle->motion[0] = (effMiscRandUnitFloat(D_003AA868) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    particle->motion[1] = 0;
    particle->motion[2] = (effMiscRandUnitFloat(D_003AA868) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    VU0_LOAD_VF(vf10, particle->motion);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, particle->motion);
    particle->swayAmplitude = work->head.swayAmplitude * (effMiscRandUnitFloat(D_003AA868) * work->head.swayRandomness + (1.0f - work->head.swayRandomness));
    particle->swayPhase = 0;
    particle->verticalSpeed = work->head.verticalSpeed * (effMiscRandUnitFloat(D_003AA868) * work->head.verticalSpeedRandomness + (1.0f - work->head.verticalSpeedRandomness));
    particle->lateralSpeed = work->head.lateralSpeed * (effMiscRandUnitFloat(D_003AA868) * work->head.lateralSpeedRandomness + (1.0f - work->head.lateralSpeedRandomness));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198D00);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199118);

/* Copy one full vector quadword through vf10, including its fourth component. */
void effEventCopyParameterVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Set the native byte flag without normalizing it to a boolean. */
void effEventSetWorkFlag(EffEventBillSet *work, u8 flag) {
    work->flag = flag;
}

/* Copy the serialized emitter parameters without changing their values. */
void effEventCopyParameterBlock(const EffEventBillParams *source, EffEventBillParams *destination) {
    *destination = *source;
}

/* Copy parameters and clamp only start-delay spread and fade divisors;
 * motionDelaySpread remains unvalidated on this native path. */
void effCopyEventBlockAndClampPositiveParameters(EffEventBillParams *destination, const EffEventBillParams *source) {
    *destination = *source;
    if (destination->fadeIn <= 0) {
        destination->fadeIn = 1;
    }
    if (destination->fadeOut <= 0) {
        destination->fadeOut = 1;
    }
    if (destination->startDelaySpread <= 0) {
        destination->startDelaySpread = 1;
    }
}

/* Create from the installed default emitter block; the returned pointer is unused. */
void effEventInstallBillParticleSet(void) {
    effEventBillSetCreate(D_003B2D20);
}

typedef struct EffEventChannelHead {
    f32 controlPoints[4][4];
    u8 enabled;
    u8 pad41[3];
    u32 count;
    s32 steps;
    s32 spread;
    s32 fadeIn;
    s32 fadeOut;
    f32 jitter[4];
    u8 pad68[0x100];
} EffEventChannelHead;

typedef struct EffEventChannelRecord {
    s32 delay;
    void *param;
} EffEventChannelRecord;

typedef struct EffEventChannelWork {
    EffEventChannelHead head;
    EffEventChannelRecord *records;
    s32 *slots;
    s32 buffer;
} EffEventChannelWork;

extern void *effAllocSlotArray(u32);
extern void *effParamWorkCreate(u16, void *);
extern void *effParamWorkDuplicate(void *);

void *effEventCreateChannelFromParams(void *source, u16 kind, void *params) {
    EffEventChannelHead *head = source;
    u32 recordCount = head->count;
    s32 handle = sdfAllocGeneralBlock(recordCount * sizeof(EffEventChannelRecord) + sizeof(EffEventChannelWork));
    EffEventChannelWork *work = (EffEventChannelWork *)sdfResourceRetainAddress(handle);
    EffEventChannelRecord *record = (EffEventChannelRecord *)(work + 1);
    void *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = *head;
    work->buffer = handle;
    work->records = record;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = effAllocSlotArray(recordCount);
    if (recordCount != 0) {
        delayModulus = work->head.spread;
        parameterTemplate = effParamWorkCreate(kind, params);
        record->param = parameterTemplate;
        record->delay = -(effMiscRand(D_003AA868) % delayModulus);
        record++;
        for (recordIndex = 1; recordIndex < recordCount; recordIndex++) {
            record->param = effParamWorkDuplicate(parameterTemplate);
            record->delay = -(effMiscRand(D_003AA868) % delayModulus);
            record++;
        }
    }
    return work;
}

extern void *effParamTableGetBlock(void *data, s32 index);
extern u32 effParamTableGetWord2(void *data, s32 index);

/* Forward block 0, block 1's word-2 kind (narrowed to u16), and block 1 unchanged. */
void effEventParticleSetCreateFromTable(void *data) {
    void *block0 = effParamTableGetBlock(data, 0);
    void *block1 = effParamTableGetBlock(data, 1);

    effEventCreateChannelFromParams(block0, effParamTableGetWord2(data, 1), block1);
}

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436458);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043645C);

INCLUDE_SDATA(const s32, "effect/effEvent", effRectangleBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effTexturedBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effFilterBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436463);

INCLUDE_SDATA(const s32, "effect/effEvent", effColorRectangleEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effTexturedSquareEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effStaggeredBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436468);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436470);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436478);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436480);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436488);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436490);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436498);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364A0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364A8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364B0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364B8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364C0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364C8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364D0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364D8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364E0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364E8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364F0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364F8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436500);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436504);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436508);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436510);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436518);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043651C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436520);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436528);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436530);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436534);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436538);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043653C);

