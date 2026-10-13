#include "sdf_projection.h"
#include "bill_object_api.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "eff_param.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "btl_sound.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "eff.h"
#include "eff_math.h"
#include "eff_channel.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "eff_event_setup.h"
#include "eff_event_bill.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "sdf_texture_primary_buffer_api.h"
#include "sdf_vu_matrix.h"

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



/* Packet-source layouts and concrete blur owners mirror their constructors.
 * Equal-sized parameter prefixes do not make the blur variants interchangeable. */

/* EffBlurTemplate and its copied body are declared in eff_blur.h. */









extern EffScreenDrawParams effBlurRectangleParameters;
extern EffScreenDrawParams D_00355908;
extern EffSolidRectParams effColorRectangleParameters;
extern EffScreenDrawParams D_003559A0;
extern EffBlurTemplateBody D_00355AF8;
extern EffBlurScatterParams D_00355C70;
extern EffScreenDrawParams D_00355E48;
extern EffSolidRectParams D_00355F88;
extern EffResourceRectParams D_00356088;
extern EffBlurScaleParams D_003561C8;
extern void *memcpy(void *, const void *, u32);

extern EffResourceRectWork *effTexturedSquareWork;

extern s8 effTexturedSquareEnabled;

extern s8 effColorRectangleEnabled;

extern s8 D_003BB073;

extern EffBlurScaleWork *effStaggeredBlurWork;

extern s8 effStaggeredBlurEnabled;

extern EffBlurScatterWork *effFilterBlurWork;

extern s8 effFilterBlurEnabled;

extern EffBlurTemplate *effBlurPixelWork;

extern s8 effTexturedBlurEnabled;

extern s8 effRectangleBlurEnabled;
extern u8 D_003558D8[];
extern u8 D_003558A8[];
extern u8 D_00355948[];
extern u8 D_00355970[];
extern SdfPoolNode kwlnPositionedTextSurface;
#include "sdf_packet_alloc.h"
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);

extern EffResourceRectWork *effCloneResourceTemplate(EffResourceRectParams *params);
extern void effBlurStepScaleSlotsAndDraw(EffBlurScaleWork *arg);
extern void func_00187C08(EffSolidRectParams *arg);
extern void effResourceRectDrawPixels(EffResourceRectWork *arg);

/* Allocate contiguous slots followed by their count and allocation handle. */
EffArrHdr *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * EFF_EVENT_BEZIER_SLOT_BYTES;
    SdfMemBlock *allocation = sdfAllocGeneralBlock(slotBytes + EFF_EVENT_SLOT_HEADER_BYTES);
    EffSegmentedBezierSlot *slot = (EffSegmentedBezierSlot *)sdfResourceRetainAddress(allocation);
    EffArrHdr *table = (EffArrHdr *)((u8 *)slot + slotBytes);
    u32 index = 0;
    table->allocation = allocation;
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

/* Return the selected slot; index bounds remain the caller's responsibility. */
EffSegmentedBezierSlot *effGetSlotAt(EffArrHdr *table, s32 index) {
    return &((EffSegmentedBezierSlot *)table->slots)[index];
}

extern void *sdfCreateBorderedRectanglePacket(s32, s32, s32, s32, s32, s32, s32);

/* Project a point and draw a filled/bordered marker with 0x20/0x10 GS-unit extents. */
void effDrawMarkerBoxAtPoint(f32 *position) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 screen[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;
    SdfPoolNode *scene;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, position);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, screen);
    x = (s32)screen[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y = (s32)screen[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x;
    pixel[1] = y;
    sdfAppendPacket(list, (u32)(sdfCreateBorderedRectanglePacket((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_WIDTH, EFF_EVENT_MARKER_HEIGHT, EFF_EVENT_MARKER_COLOR, EFF_EVENT_MARKER_COLOR)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (SdfListHead *)list);
}

/* Project a point and use color for both the marker's fill and border. */
void effDrawColoredBoxAtPoint(f32 *position, s32 color) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 screen[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;
    SdfPoolNode *scene;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, position);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, screen);
    x = (s32)screen[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y = (s32)screen[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x;
    pixel[1] = y;
    sdfAppendPacket(list, (u32)(sdfCreateBorderedRectanglePacket((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_WIDTH, EFF_EVENT_MARKER_HEIGHT, color, color)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (SdfListHead *)list);
}

extern void *fldCreateColoredLinePacket(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Draw a GS line between two projected points, using the fixed marker color. */
void effDrawMarkerLineBetweenPoints(f32 *from, f32 *to) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 start[EFF_EVENT_VECTOR_COMPONENTS];
    f32 end[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    SdfPoolNode *scene;

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
    sdfAppendPacket(list, (u32)(fldCreateColoredLinePacket((x0 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y0 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_COLOR, (x1 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y1 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_COLOR, 0)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (SdfListHead *)list);
}

/* Draw a GS line between two projected points, using color at both vertices. */
void effDrawColoredLineBetweenPoints(f32 *from, f32 *to, s32 color) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    f32 start[EFF_EVENT_VECTOR_COMPONENTS];
    f32 end[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    SdfPoolNode *scene;

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
    sdfAppendPacket(list, (u32)(fldCreateColoredLinePacket((x0 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y0 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, color, (x1 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y1 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, color, 0)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (SdfListHead *)list);
}

/* Submit the native command at biased screen coordinates; payload args stay opaque. */
void effSubmitPositionedDrawPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    SdfListHead *task = (SdfListHead *)sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    SdfPoolNode *scene;

    sdfInitPacketList(task);
    sdfAppendPacket(task, (u32)(sdfCreateFormattedSifCommand((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, arg2, (const char *)arg3)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (SdfListHead *)task);
}

extern void *sdfDrawCachedMatrixLinePairsVU(const f32 [][4], const u32 *, s32, u32);
extern void func_002DD608(f32);
extern SdfPoolNode kwlnDrawSurfaces[];

void effDrawSphericalWireRings(const f32 *center, u32 color, f32 radius) {
    f32 point[4] __attribute__((aligned(16)));
    f32 matrix[4][4] __attribute__((aligned(16)));
    f32 vertices[40][4] __attribute__((aligned(16)));
    u32 colors[40];
    SdfListHead *list;
    SdfPoolNode *surface;
    f32 (*out)[4];
    f32 latitude;
    f32 longitude;
    f32 ringRadius;
    f32 ringY;
    u32 pass;
    u32 ring = 0;
    u32 segment;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    do {
        colors[ring++] = color;
    } while (ring < 40);

    EE_MMI_UNIT_MATRIX(matrix);
    for (pass = 0; pass < 2; pass++) {
        latitude = 0.0f;
        for (ring = 0; ring < 14; ring++) {
            latitude += 0.20943949f;
            out = vertices;
            longitude = 0.0f;
            ringY = radius * sdfEvaluateCosineViaSinePhaseShift(latitude);
            ringRadius = radius * sdfSinPoly(latitude);
            VU0_LOAD_MATRIX(matrix);
            VU0_LOAD_VF(vf11, center);
            point[0] = ringRadius;
            point[1] = ringY;
            point[2] = 0.0f;
            VU0_LOAD_VF(vf10, point);
            VU0_CLEAR_W(vf10);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_ADD(vf10, vf10, vf11);

            for (segment = 0; segment < 20; segment++) {
                VU0_STORE_VF(vf10, *out);
                out++;
                longitude += 0.31415924429893494f;
                point[0] = sdfEvaluateCosineViaSinePhaseShift(longitude) * ringRadius;
                point[1] = ringY;
                point[2] = sdfSinPoly(longitude) * ringRadius;
                VU0_LOAD_VF(vf10, point);
                VU0_CLEAR_W(vf10);
                VU0_APPLY_MATRIX(vf10, vf10);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF(vf10, *out);
                out++;
            }
            sdfAppendPacket(list, (u32)sdfDrawCachedMatrixLinePairsVU(vertices, colors, 40, 0x80));
        }
        func_002DD608(1.5707963f);
        VU0_LOAD_MATRIX_B_F32(matrix);
        sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX_UNCLOBBERED(matrix);
    }
    surface = &kwlnDrawSurfaces[55];
    surface->append(surface, list);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_0018ED80);

extern void *sdfDrawCachedMatrixLinePairsVU(const f32 [][4], const u32 *, s32, u32);
extern SdfPoolNode kwlnDrawSurfaces[];

/* Draw horizontal rings, vertical sides, and the two sets of cap diameters. */
void effDrawWireCylinder(const f32 *center, u32 color, f32 radius, f32 height) {
    f32 point[4]; /* The line provider consumes xyz; preserve the untouched w lane. */
    f32 vertices[40][4];
    u32 colors[40];
    SdfListHead *list;
    f32 (*out)[4];
    SdfPoolNode *surface;
    u32 ring;
    u32 segment;
    f32 y;
    f32 heightStep;
    f32 angleStep;
    f32 angle;
    f32 oppositeAngle;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    for (ring = 0; ring < 40; ring++) {
        colors[ring] = color;
    }
    heightStep = height / 19.0f;
    angleStep = 0.31415924429893494f;
    y = -height * 0.5f;
    for (ring = 0; ring < 20; ring++) {
        angle = 0.0f;
        out = vertices;
        point[0] = radius;
        point[1] = y;
        point[2] = 0.0f;
        VU0_LOAD_VF(vf11, center);
        VU0_LOAD_VF(vf10, point);
        VU0_ADD(vf10, vf10, vf11);
        for (segment = 0; segment < 20; segment++) {
            VU0_STORE_VF(vf10, *out);
            out++;
            angle += angleStep;
            point[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
            point[1] = y;
            point[2] = sdfSinPoly(angle) * radius;
            VU0_LOAD_VF(vf10, point);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, *out);
            out++;
        }
        sdfAppendPacket(list, (u32)sdfDrawCachedMatrixLinePairsVU(vertices, colors, 40, 0x80));
        y += heightStep;
    }

    out = vertices;
    VU0_LOAD_VF(vf11, center);
    angle = 0.0f;
    for (segment = 0; segment < 20; segment++) {
        point[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        point[1] = -height * 0.5f;
        point[2] = sdfSinPoly(angle) * radius;
        VU0_LOAD_VF(vf10, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, *out);
        out++;
        point[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        point[1] = height * 0.5f;
        point[2] = sdfSinPoly(angle) * radius;
        VU0_LOAD_VF(vf10, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, *out);
        out++;
        angle += angleStep;
    }
    sdfAppendPacket(list, (u32)sdfDrawCachedMatrixLinePairsVU(vertices, colors, 40, 0x80));

    out = vertices;
    VU0_LOAD_VF(vf11, center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        point[1] = -height * 0.5f;
        point[2] = sdfSinPoly(angle) * radius;
        VU0_LOAD_VF(vf10, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, *out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point[0] = sdfEvaluateCosineViaSinePhaseShift(oppositeAngle) * radius;
        point[1] = -height * 0.5f;
        point[2] = sdfSinPoly(oppositeAngle) * radius;
        VU0_LOAD_VF(vf10, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, *out);
        out++;
        angle += angleStep;
    }
    sdfAppendPacket(list, (u32)sdfDrawCachedMatrixLinePairsVU(vertices, colors, 20, 0x80));

    out = vertices;
    VU0_LOAD_VF(vf11, center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        point[1] = height * 0.5f;
        point[2] = sdfSinPoly(angle) * radius;
        VU0_LOAD_VF(vf10, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, *out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point[0] = sdfEvaluateCosineViaSinePhaseShift(oppositeAngle) * radius;
        point[1] = height * 0.5f;
        point[2] = sdfSinPoly(oppositeAngle) * radius;
        VU0_LOAD_VF(vf10, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, *out);
        out++;
        angle += angleStep;
    }
    sdfAppendPacket(list, (u32)sdfDrawCachedMatrixLinePairsVU(vertices, colors, 20, 0x80));
    surface = &kwlnDrawSurfaces[55];
    surface->append(surface, list);
}



/* Submit a filled rectangle and its line-strip border. Keep native width/height
 * scaling and the distinct shift/multiply forms used by the other draw paths. */
void effSubmitSizedDrawPacket(s32 x, s32 y, s32 width, s32 height, s32 fillColor, s32 borderColor) {
    SdfListHead *list = (SdfListHead *)sdfAllocPacketAligned(EFF_EVENT_DRAW_LIST_BYTES);
    SdfPoolNode *scene;

    sdfInitPacketList(list);
    sdfAppendPacket(list, (u32)(sdfCreateBorderedRectanglePacket(x * EFF_EVENT_GS_X_SCALE + EFF_EVENT_GS_X_BIAS, y * EFF_EVENT_GS_Y_SCALE + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, width * EFF_EVENT_GS_X_SCALE, height * EFF_EVENT_GS_Y_SCALE, fillColor, borderColor)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (SdfListHead *)list);
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

void effSetCh71Id(SdfTex *texture) {
    effBlurPixelWork->texture = texture;
}

/* Select the same resource-table entry used by the filter-blur initializer. */
void effInitCh71Id(void) {
    effBlurPixelWork->texture = effGetBillResourceTexture(EFF_EVENT_BLUR_RESOURCE_INDEX);
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

void effSetCh72Id(SdfTex *sourceHandle) {
    effFilterBlurWork->texture = sourceHandle;
}

/* Install the filter-blur resource-table texture without changing its slots. */
void effInitCh72Id(void) {
    effFilterBlurWork->texture = effGetBillResourceTexture(EFF_EVENT_BLUR_RESOURCE_INDEX);
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

void effSetCh76Id(SdfTex *sourceHandle) {
    effStaggeredBlurWork->texture = sourceHandle;
}

/* Install the staggered-blur resource-table texture without changing its slots. */
void effInitCh76Id(void) {
    effStaggeredBlurWork->texture = effGetBillResourceTexture(EFF_EVENT_STAGGERED_RESOURCE_INDEX);
}

void effEnableFramebufferQuad(void) {
    D_003BB073 = 1;
}

void effDisableFramebufferQuad(void) {
    D_003BB073 = 0;
}

/* Copy only the framebuffer source prefix, not the trailing screen-record bytes. */
void effCopyFramebufferQuadParameters(void *parameters) {
    memcpy(&D_00355908, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the framebuffer-quad parameter block by address. */
EffScreenDrawParams *effGetCh73Params(void) {
    return &D_00355908;
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

void effSetCh75Id(SdfTex *texture) {
    effTexturedSquareWork->texture = texture;
}

/* Install the textured-square resource-table texture while retaining its body. */
void effInitCh75Id(void) {
    effTexturedSquareWork->texture = effGetBillResourceTexture(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}

/* Clone the four default work templates; only the staggered slot count is overridden. */
void effInitWorks(void) {
    effBlurPixelWork = effCloneBlurTemplate((EffBlurTemplateBody *)D_003558D8);
    effFilterBlurWork = effBlurCreateScatterWork((EffBlurScatterParams *)D_003558A8);
    effTexturedSquareWork = effCloneResourceTemplate((EffResourceRectParams *)D_00355948);
    effStaggeredBlurWork = effCloneBlurWorkWithSlots((EffBlurScaleParams *)D_00355970);
    effGetCh76Work()->params.count = EFF_EVENT_STAGGERED_SLOT_COUNT;
}

/* Dispatch enabled draw families independently, in their native order. */
void effDispatchActive(void) {
    if (effRectangleBlurEnabled) {
        effDrawBlurRectangle(&effBlurRectangleParameters.source);
    }
    if (effTexturedBlurEnabled) {
        effDrawBlurPixelRectWithResource(effBlurPixelWork);
    }
    if (effFilterBlurEnabled) {
        effBlurStepScatterSlotsAndDraw(effFilterBlurWork);
    }
    if (effStaggeredBlurEnabled) {
        effBlurStepScaleSlotsAndDraw(effStaggeredBlurWork);
    }
    if (D_003BB073) {
        effBlurDrawFramebufferQuad(&D_00355908.source);
    }
    if (effColorRectangleEnabled) {
        func_00187C08(&effColorRectangleParameters);
    }
    if (effTexturedSquareEnabled) {
        effResourceRectDrawPixels(effTexturedSquareWork);
    }
}

/* callbackResult must be valid whenever callback is present. */
extern EffEventSetupRecord D_00355AB8;
extern s8 D_003BB0BD;
extern s8 D_0039862B[];
extern void func_0018CDF8(void);
extern void func_0018CDF0(void *);
extern void func_0018CE00(void);

/* Prepare once per flag cycle, then run the registered operations while ready.
 * A negative control byte clears readiness only after those operations. */
s32 effUpdateCh72Params(void) {
    u8 ready = D_003BB0BD;

    if (D_003BB0BD == 0) {
        EffEventSetupRecord *state = &D_00355AB8;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(state->source);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_003BB0BD = ready;
    }
    if (ready != 0) {
        func_0018CDD0((u32)&D_00355AB8);
        func_0018CDF8();
        func_0018CDF0(&D_00355AB8);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0BD = 0;
    }
    return D_003BB0BD;
}

/* Expose this screen-draw setup block; no parameter copy is made. */
EffScreenDrawParams *effGetLoadDescA(void) {
    return &D_003559A0;
}

/* Replace the screen-draw source prefix while preserving the trailing bytes. */
void effSetLoadDescA(void *parameters) {
    memcpy(&D_003559A0, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffEventSetupRecord D_00355C30;
extern s8 D_003BB0CD;

/* Prepare the blur-template callback from its explicit parameter block,
 * then process the channel; its optional copy still uses source/destination. */
s32 effEventAdvanceBlurTemplateSetup(void) {
    u8 ready = D_003BB0CD;

    if (D_003BB0CD == 0) {
        EffEventSetupRecord *state = &D_00355C30;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_00355AF8);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_003BB0CD = ready;
    }
    if (ready != 0) {
        func_0018CDD0((u32)&D_00355C30);
        func_0018CDF8();
        func_0018CDF0(&D_00355C30);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0CD = 0;
    }
    return D_003BB0CD;
}

/* Expose the blur-template setup parameters by address. */
EffBlurTemplateBody *effEventGetBlurTemplateSetupParams(void) {
    return &D_00355AF8;
}

/* Replace the setup template; this does not modify the active blur work. */
void effEventSetBlurTemplateParameters(EffBlurTemplateBody *parameters) {
    D_00355AF8 = *parameters;
}

extern EffEventSetupRecord D_00355E08;
extern s8 D_003BB0FF;

/* Prepare scatter setup from its explicit parameters, then process the channel. */
s32 effEventAdvanceScatterBlurSetup(void) {
    u8 ready = D_003BB0FF;

    if (D_003BB0FF == 0) {
        EffEventSetupRecord *state = &D_00355E08;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_00355C70);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_003BB0FF = ready;
    }
    if (ready != 0) {
        func_0018CDD0((u32)&D_00355E08);
        func_0018CDF8();
        func_0018CDF0(&D_00355E08);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0FF = 0;
    }
    return D_003BB0FF;
}

/* Expose the scatter-blur setup parameters by address. */
EffBlurScatterParams *effEventGetScatterBlurSetupParams(void) {
    return &D_00355C70;
}

/* Replace setup parameters without replacing the active filter-blur work. */
void effEventSetScatterBlurParameters(EffBlurScatterParams *parameters) {
    D_00355C70 = *parameters;
}

extern EffEventSetupRecord D_00355F48;
extern s8 D_003BB114;

/* Prepare from the channel's source, process it, then honor the control-byte reset. */
s32 effEventAdvanceScreenDrawSetup(void) {
    u8 ready = D_003BB114;

    if (D_003BB114 == 0) {
        EffEventSetupRecord *state = &D_00355F48;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(state->source);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_003BB114 = ready;
    }
    if (ready != 0) {
        func_0018CDD0((u32)&D_00355F48);
        func_0018CDF8();
        func_0018CDF0(&D_00355F48);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB114 = 0;
    }
    return D_003BB114;
}

/* Expose the other screen-draw setup block by address. */
EffScreenDrawParams *effGetLoadDescD(void) {
    return &D_00355E48;
}

/* Replace this screen-draw source prefix without changing trailing bytes. */
void effSetLoadDescD(void *parameters) {
    memcpy(&D_00355E48, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffEventSetupRecord D_00356048;
extern s8 D_003BB127;

/* Prepare solid-rectangle setup from the channel source, then process it. */
s32 effEventAdvanceSolidRectangleSetup(void) {
    u8 ready = D_003BB127;

    if (D_003BB127 == 0) {
        EffEventSetupRecord *state = &D_00356048;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(state->source);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_003BB127 = ready;
    }
    if (ready != 0) {
        func_0018CDD0((u32)&D_00356048);
        func_0018CDF8();
        func_0018CDF0(&D_00356048);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB127 = 0;
    }
    return D_003BB127;
}

/* Expose the solid-rectangle setup parameters by address. */
EffSolidRectParams *effEventGetSolidRectangleSetupParams(void) {
    return &D_00355F88;
}

/* Replace the setup block rather than the active solid-rectangle parameters. */
void effEventSetSolidRectangleParameters(EffSolidRectParams *parameters) {
    D_00355F88 = *parameters;
}

extern EffEventSetupRecord D_00356188;
extern s8 D_003BB12C;

/* Prepare resource-template setup from its explicit body, then process the channel. */
s32 effEventAdvanceResourceTemplateSetup(void) {
    u8 ready = D_003BB12C;

    if (D_003BB12C == 0) {
        EffEventSetupRecord *state = &D_00356188;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_00356088);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_003BB12C = ready;
    }
    if (ready != 0) {
        func_0018CDD0((u32)&D_00356188);
        func_0018CDF8();
        func_0018CDF0(&D_00356188);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB12C = 0;
    }
    return D_003BB12C;
}

/* Expose the resource-template setup body by address. */
EffResourceRectParams *effEventGetResourceTemplateSetupParams(void) {
    return &D_00356088;
}

/* Replace the setup body without replacing the active textured-square work. */
void effEventSetResourceTemplateParameters(EffResourceRectParams *parameters) {
    D_00356088 = *parameters;
}

extern EffEventSetupRecord D_00356360;
extern s8 D_003BB13F;

/* Prepare scale-blur setup from its explicit parameters, then process the channel. */
s32 effEventAdvanceScaleBlurSetup(void) {
    u8 ready = D_003BB13F;

    if (D_003BB13F == 0) {
        EffEventSetupRecord *state = &D_00356360;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_003561C8);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_003BB13F = ready;
    }
    if (ready != 0) {
        func_0018CDD0((u32)&D_00356360);
        func_0018CDF8();
        func_0018CDF0(&D_00356360);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB13F = 0;
    }
    return D_003BB13F;
}

/* Expose the scale-blur setup parameters by address. */
EffBlurScaleParams *effEventGetScaleBlurSetupParams(void) {
    return &D_003561C8;
}

/* Replace setup parameters without replacing the active staggered-blur work. */
void effEventSetScaleBlurParameters(EffBlurScaleParams *parameters) {
    D_003561C8 = *parameters;
}

SoundMixer *effEventCloneSoundMixer(SoundMixer *source) {
    return sndMixerClone(source);
}

void effEventReleaseSoundMixerVoices(SoundMixer *mixer) {
    sndReleaseAllVoices(mixer);
}


extern u8 D_003563F0[];


extern s32 D_003BB140;

extern u32 D_003BB144;

extern u32 D_003BB148;

/* Compact event owner: the allocator reserves exactly 0x38 bytes. */
struct EffEventWork {
    EffEventInit init;
    void *actor;          /* Linked model/event owner; concrete identity is unknown. */
    BattleEffect *effect;
};

typedef struct {
    u8 bytes[EFF_EVENT_EVENT_RECORD_BYTES];
} __attribute__((packed)) FileRecordHeader;


extern void effBattleSetScale(BattleEffect *, f32);

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
EffEventWork *effEventCreate(SoundMixer *owner, u16 kind, const void *params) {
    const EffEventInit *init = (const EffEventInit *)params;
    EffEventWork *work = sdfAllocSizeClassBlock(EFF_EVENT_COMPACT_WORK_BYTES);

    memcpy(work, init, sizeof(*init));
    work->actor = NULL;
    work->effect = func_00160958(owner, kind, 0, 0);
    effBattleSetScale(work->effect, init->param);
    return work;
}

/* Release the attached effect before freeing the event work. */
void effEventReleaseNode(EffEventWork *work) {
    effReleaseBattleVoiceOwner(work->effect);
    sdfReleaseChipBlock(work);
}

/* Copy the packed 0x30-byte record while retaining its native packed layout. */
void effEventCopyFileRecordHeader(EffEventWork *destination, const void *source) {
    *(FileRecordHeader *)destination = *(const FileRecordHeader *)source;
}

/* This copies a packed 0x30-byte record, not a complete 0x60-byte billboard state. */
void effEventCopySerializedRecord(const FileRecordHeader *source, FileRecordHeader *destination) {
    *destination = *source;
}

/* Forward the effect's scale value without modifying the record's init prefix. */
void effEventSetScale(EffEventWork *work, f32 scale) {
    effBattleSetScale(work->effect, scale);
}

void effEventSetState(EffEventWork *work, void *actor) {
    work->actor = actor;
}

INCLUDE_ASM(const s32, "effect/effEvent", effEventUpdateEffectParameters);

/* Initialize a holder and its effect record; the teardown flag starts set. */
EffEventLight *effEventLightCreate(SoundMixer *arg, f32 param) {
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
    work->handle = effEventCloneSoundMixer(arg);
    work->owner = effEventCreate(work->handle, 0, &work->init);
    work->active = 1;
    return work;
}

/* Release the effect, invoke shared teardown only when flagged, then free the holder. */
void effEventLightDestroy(EffEventLight *work) {
    effEventReleaseNode(work->owner);
    if (work->active != 0) {
        effEventReleaseSoundMixerVoices(work->handle);
    }
    sdfReleaseChipBlock(work);
}

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
EffEventLight *effEventLightClone(EffEventLight *src) {
    EffEventInit *block = &src->init;
    EffEventLight *work = sdfAllocSizeClassBlock(sizeof(EffEventLight));

    work->owner = effEventCreate(src->handle, 0, block);
    work->handle = src->handle;
    memcpy(&work->init, block, sizeof(work->init));
    work->active = 0;
    return work;
}

/* Apply the native owner operation to the pointer stored in the record prefix. */
void effEventLightUpdateEffectParameters(EffEventLight *work) {
    effEventUpdateEffectParameters(work->owner);
}

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effEventLightSetPosition(EffEventLight *work, f32 *position) {
    work->init.position[0] = position[0];
    work->init.position[1] = position[1] - work->init.height * 0.5f;
    work->init.position[2] = position[2];
    work->init.position[3] = 0;
    effEventCopyFileRecordHeader(work->owner, &work->init);
}

/* Copy the holder's updated packed tint to its event record. */
void effEventBindEffect(EffEventLight *work, u32 color) {
    work->init.color = color;
    effEventCopyFileRecordHeader(work->owner, &work->init);
}



extern f32 D_003563A0[];
extern f32 D_003563B8[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
extern void func_002DD968(f32 angle);

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
        y = pos[1] - D_003563A0[aimMode] * half;
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
        func_002DD968(D_003563B8[directionMode]);
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

extern BillObj *billCreateFromResource(s32 kind, const char *path);

/* Allocate particles plus their owner, copy parameters and acquire shared textures. */
INCLUDE_RODATA(const s32, "effect/effEvent", D_003A12E0);

EffEventBillSet *effEventBillSetCreate(EffEventBillParams *src) {
    u32 particleCount = src->particleCount;
    u32 particleBytes = particleCount * sizeof(EffEventBillParticle);
    SdfMemBlock *allocationHandle = sdfAllocGeneralBlock(particleBytes + sizeof(EffEventBillSet));
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
    D_003BB140 = D_003BB140 + 1;
    if (D_003BB140 == 1) {
        D_003BB144 = (u32)billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_003BB148 = (u32)billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
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
    D_003BB140 = D_003BB140 - 1;
    if (D_003BB140 == 0) {
        billDispatchByKind((BillObj *)D_003BB144);
        billDispatchByKind((BillObj *)D_003BB148);
    }
    sdfReleaseResourceAllocation(work->allocationHandle);
}

extern u8 effDefaultRandomState[];

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEventRandomizeBillboardParticle(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *particle = &work->particles[index];
    f32 direction[EFF_EVENT_VECTOR_COMPONENTS];
    f32 scaleFactor;
    f32 spawnRadius;
    s32 startDelaySpread = work->head.startDelaySpread;
    s32 motionDelaySpread = work->head.motionDelaySpread;

    particle->age = -(effMiscRand((EffRandState *)effDefaultRandomState) % startDelaySpread);
    particle->motionDelay = -(effMiscRand((EffRandState *)effDefaultRandomState) % motionDelaySpread);
    scaleFactor = effMiscRandUnitFloat(effDefaultRandomState) * work->head.scaleRandomness + (1.0f - work->head.scaleRandomness);
    particle->baseScale = work->head.baseScale * scaleFactor;
    particle->scalePhaseX = effMiscRandUnitFloat(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseY = effMiscRandUnitFloat(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseStepX = work->head.scalePhaseStep * (effMiscRandUnitFloat(effDefaultRandomState) * 0.5f + 0.5f);
    particle->scalePhaseStepY = work->head.scalePhaseStep * (effMiscRandUnitFloat(effDefaultRandomState) * 0.5f + 0.5f);
    particle->scaleAmplitudeX = work->head.scaleAmplitudeX * (effMiscRandUnitFloat(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->scaleAmplitudeY = work->head.scaleAmplitudeY * (effMiscRandUnitFloat(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->rotation = effMiscRandUnitFloat(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    if (effMiscRand((EffRandState *)effDefaultRandomState) & 1) {
        particle->angularSpeed = work->head.angularSpeed * (effMiscRandUnitFloat(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE);
    } else {
        particle->angularSpeed = -(work->head.angularSpeed * (effMiscRandUnitFloat(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE));
    }
    spawnRadius = work->head.spawnRadius;
    direction[0] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[1] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[2] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    VU0_LOAD_VF(vf10, direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, direction);
    /* Each coordinate consumes its own radial random factor, not one shared radius. */
    particle->position[0] = spawnRadius * effMiscRandUnitFloat(effDefaultRandomState) * direction[0];
    particle->position[1] = spawnRadius * effMiscRandUnitFloat(effDefaultRandomState) * direction[1];
    particle->position[2] = spawnRadius * effMiscRandUnitFloat(effDefaultRandomState) * direction[2];
    particle->motion[0] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    particle->motion[1] = 0;
    particle->motion[2] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    VU0_LOAD_VF(vf10, particle->motion);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, particle->motion);
    particle->swayAmplitude = work->head.swayAmplitude * (effMiscRandUnitFloat(effDefaultRandomState) * work->head.swayRandomness + (1.0f - work->head.swayRandomness));
    particle->swayPhase = 0;
    particle->verticalSpeed = work->head.verticalSpeed * (effMiscRandUnitFloat(effDefaultRandomState) * work->head.verticalSpeedRandomness + (1.0f - work->head.verticalSpeedRandomness));
    particle->lateralSpeed = work->head.lateralSpeed * (effMiscRandUnitFloat(effDefaultRandomState) * work->head.lateralSpeedRandomness + (1.0f - work->head.lateralSpeedRandomness));
}

INCLUDE_ASM(const s32, "effect/effEvent", effEventUpdateBillboardParticles);

/* Fifteen billboard records are packed into each compact VIF batch. */
typedef struct EffEventBillBatch {
    u128 positions[15];
    u32 colors[15];
    s16 uv[15][8];
    f32 corners[15][8];
    s32 count;
} EffEventBillBatch;
typedef char EffEventBillBatch_size[(sizeof(EffEventBillBatch) == 0x400) ? 1 : -1];

struct DmaPacketHeader;
extern void sdfConsInitDmaPacketHeader(struct DmaPacketHeader *packet, u32 sourceAddress, s32 payloadBytes);
extern void sdfInitGeometryDmaPacket(u8 *packet, const f32 *matrix);
extern u32 sdfBuildCompactVertexVifPacket(const u128 *positions, const void *byteAttributes,
    const void *halfAttributes, const void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32));
extern f32 sdfViewEyeVector[4];
extern SdfPoolNode D_003252A8;

/* DDS1 event-bill renderer, reconstructed from the native owner and its
 * current providers. The VIF position stream consumes XYZ only: native never
 * initializes the temporary position's W before its full-vector add/copy. */
void effEventDrawBillSet(EffEventBillSet *work) {
    EffEventBillBatch primary, secondary;
    f32 matrix[4][4];
    f32 position[4], origin[4], lateral[4];
    SdfListHead *primaryList, *secondaryList;
    SdfTex *primaryTexture, *secondaryTexture;
    EffEventBillParticle *particle;
    s32 repeat;
    u32 index, particleCount;
    s32 duration, fadeIn, fadeOut, colorStart, colorFrames;
    u32 color;
    f32 acceleration, swayPhaseStep, swayAmplitudeStep;
    struct DmaPacketHeader *texturePacket;
    SdfTexBuf *textureBuffer;
    s32 textureBytes;
    u8 *geometryPacket;
    SdfListHead *stateList;
    u64 *statePacket;
    SdfPoolNode *surface;
    primary.count = 0;
    secondary.count = 0;
    primaryList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(primaryList);
    secondaryList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(secondaryList);
    primaryTexture = (SdfTex *)billGetChildValue((BillObj *)D_003BB144);
    texturePacket = (struct DmaPacketHeader *)sdfAllocPacketAligned(0x20);
    textureBuffer = sdfTexGetPrimaryBuffer(primaryTexture);
    textureBytes = sdfTexGetPrimaryBufferSize(primaryTexture);
    sdfConsInitDmaPacketHeader(texturePacket, (u32)textureBuffer, textureBytes);
    sdfAppendReferencePacket(primaryList, (u32)texturePacket);
    secondaryTexture = (SdfTex *)billGetChildValue((BillObj *)D_003BB148);
    texturePacket = (struct DmaPacketHeader *)sdfAllocPacketAligned(0x20);
    textureBuffer = sdfTexGetPrimaryBuffer(secondaryTexture);
    textureBytes = sdfTexGetPrimaryBufferSize(secondaryTexture);
    sdfConsInitDmaPacketHeader(texturePacket, (u32)textureBuffer, textureBytes);
    sdfAppendReferencePacket(secondaryList, (u32)texturePacket);
    EE_MMI_UNIT_MATRIX(matrix);
    geometryPacket = (u8 *)sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket(geometryPacket, matrix[0]);
    sdfAppendPacket(primaryList, (u32)geometryPacket);
    geometryPacket = (u8 *)sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket(geometryPacket, matrix[0]);
    sdfAppendPacket(secondaryList, (u32)geometryPacket);
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, sdfViewUpVector);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, lateral);
    particle = work->particles;
    particleCount = work->head.particleCount;
    acceleration = work->head.accelerationPercent / 100.0f + 1.0f;
    repeat = work->head.repeat;
    duration = work->head.duration;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    colorStart = work->head.colorStartAge;
    colorFrames = work->head.colorTransitionFrames;
    color = work->head.color;
    swayPhaseStep = work->head.swayPhaseStep;
    swayAmplitudeStep = work->head.swayAmplitudeStep;
    PCP_COPY_VECTOR(origin, work->head.position);
    for (index = 0; index < particleCount; index++, particle++) {
        s32 age = particle->age;
        if (age == 0) {
            effEventRandomizeBillboardParticle(work, index);
        }
        if (age > 0 && age <= duration) {
            f32 swaySine, sway, alpha, transition;
            f32 modulationX, scaleX, scaleY, phaseCosine;
            f32 halfWidth, halfHeight;
            u32 primaryColor, secondaryColor;
            s32 slot;
            s16 u, v;
            if (particle->motionDelay >= 0) {
                f32 lateralSpeed = particle->lateralSpeed;
                particle->position[0] += lateral[0] * lateralSpeed;
                particle->position[1] += lateral[1] * lateralSpeed;
                particle->position[2] += lateral[2] * lateralSpeed;
                swaySine = sdfSinPoly(particle->swayPhase);
                sway = particle->swayAmplitude * swaySine;
                particle->motion[1] += particle->verticalSpeed;
                position[0] = particle->position[0] + particle->motion[0] * sway;
                position[1] = particle->position[1] + particle->motion[1];
                position[2] = particle->position[2] + particle->motion[2] * sway;
                particle->verticalSpeed *= acceleration;
                particle->lateralSpeed *= acceleration;
                particle->swayAmplitude += swayAmplitudeStep;
                particle->swayPhase += swayPhaseStep;
            } else {
                particle->motionDelay++;
                position[0] = particle->position[0];
                position[1] = particle->position[1];
                position[2] = particle->position[2];
            }
            if (age < fadeIn) {
                alpha = (f32)age / (f32)fadeIn;
            } else if (duration - age <= fadeOut) {
                alpha = (f32)(duration - age) / (f32)fadeOut;
            } else {
                alpha = 1.0f;
            }
            if (age < colorStart) {
                primaryColor = effBlendColor(0, color & 0xFF000000, alpha);
            } else if (age < colorStart + colorFrames) {
                if (colorFrames > 0) {
                    transition = (f32)(age - colorStart) / (f32)colorFrames;
                    if (1.0f < transition) transition = 1.0f;
                } else {
                    transition = 1.0f;
                }
                primaryColor = effBlendColor(color & 0xFF000000, color, transition);
                primaryColor = effBlendColor(primaryColor & 0xFFFFFF, primaryColor, alpha);
            } else {
                primaryColor = effBlendColor(color & 0xFFFFFF, color, alpha);
            }
            secondaryColor = effBlendColor(color & 0xFFFFFF, color, alpha);
            scaleY = particle->baseScale;
            phaseCosine = sdfEvaluateCosineViaSinePhaseShift(particle->scalePhaseX);
            modulationX = particle->scaleAmplitudeX * phaseCosine;
            phaseCosine = sdfEvaluateCosineViaSinePhaseShift(particle->scalePhaseY);
            scaleX = scaleY + modulationX;
            scaleY = scaleY + particle->scaleAmplitudeY * phaseCosine;
            VU0_LOAD_VF(vf10, position);
            VU0_LOAD_VF(vf11, origin);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, position);
            {
                f32 cosine, sine, cornerX, cornerY;
                f32 center = 0.0f;
            slot = primary.count;
            PCP_COPY_VECTOR(&primary.positions[slot], position);
            u = (s16)((u16)primaryTexture->width << 4);
            v = (s16)((u16)primaryTexture->height << 4);
            primary.colors[slot] = primaryColor;
            primary.uv[slot][0] = 0;
            primary.uv[slot][1] = 0;
            primary.uv[slot][2] = u;
            primary.uv[slot][3] = 0;
            primary.uv[slot][4] = u;
            primary.uv[slot][5] = v;
            primary.uv[slot][6] = 0;
            primary.uv[slot][7] = v;
            halfWidth = (f32)(u >> 5) * 0.0625f * scaleX;
            halfHeight = (f32)(v >> 5) * 0.0625f * scaleY;
            cosine = sdfEvaluateCosineViaSinePhaseShift(particle->rotation);
            sine = sdfSinPoly(particle->rotation);
            cornerX = center - halfWidth;
            cornerY = center - halfHeight;
            primary.corners[slot][0] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][1] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = center - halfHeight;
            primary.corners[slot][2] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][3] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = halfHeight + center;
            primary.corners[slot][4] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][5] = cornerX * sine + cornerY * cosine;
            cornerX = center - halfWidth;
            cornerY = halfHeight + center;
            primary.corners[slot][6] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][7] = cornerX * sine + cornerY * cosine;
            primary.count++;
            }
            {
                f32 cosine, sine, cornerX, cornerY;
                f32 center = 0.0f;
            slot = secondary.count;
            PCP_COPY_VECTOR(&secondary.positions[slot], position);
            u = (s16)((u16)secondaryTexture->width << 4);
            v = (s16)((u16)secondaryTexture->height << 4);
            secondary.colors[slot] = secondaryColor;
            secondary.uv[slot][0] = 0;
            secondary.uv[slot][1] = 0;
            secondary.uv[slot][2] = u;
            secondary.uv[slot][3] = 0;
            secondary.uv[slot][4] = u;
            secondary.uv[slot][5] = v;
            secondary.uv[slot][6] = 0;
            secondary.uv[slot][7] = v;
            halfWidth = (f32)(u >> 5) * 0.0625f * scaleX;
            halfHeight = (f32)(v >> 5) * 0.0625f * scaleY;
            cosine = sdfEvaluateCosineViaSinePhaseShift(particle->rotation);
            sine = sdfSinPoly(particle->rotation);
            cornerX = center - halfWidth;
            cornerY = center - halfHeight;
            secondary.corners[slot][0] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][1] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = center - halfHeight;
            secondary.corners[slot][2] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][3] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = halfHeight + center;
            secondary.corners[slot][4] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][5] = cornerX * sine + cornerY * cosine;
            cornerX = center - halfWidth;
            cornerY = halfHeight + center;
            secondary.corners[slot][6] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][7] = cornerX * sine + cornerY * cosine;
            secondary.count++;
            }
            if (primary.count == 15) {
                sdfAppendPacket(primaryList, sdfBuildCompactVertexVifPacket(primary.positions,
                    primary.colors, primary.uv, primary.corners, 15, 0));
                primary.count = 0;
            }
            if (secondary.count == 15) {
                sdfAppendPacket(secondaryList, sdfBuildCompactVertexVifPacket(secondary.positions,
                    secondary.colors, secondary.uv, secondary.corners, 15, 0));
                secondary.count = 0;
            }
            particle->scalePhaseX += particle->scalePhaseStepX;
            particle->scalePhaseY += particle->scalePhaseStepY;
            particle->rotation += particle->angularSpeed;
        }
        if (duration == age && repeat) particle->age = 0;
        else particle->age++;
    }
    if (primary.count > 0) {
        sdfAppendPacket(primaryList, sdfBuildCompactVertexVifPacket(primary.positions,
            primary.colors, primary.uv, primary.corners, primary.count, 0));
    }
    if (secondary.count > 0) {
        sdfAppendPacket(secondaryList, sdfBuildCompactVertexVifPacket(secondary.positions,
            secondary.colors, secondary.uv, secondary.corners, secondary.count, 0));
    }
    stateList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(stateList);
    statePacket = (u64 *)sdfAllocPacketAligned(0x40);
    statePacket[0] = 3;
    statePacket[1] = 0x5000000310000000ULL;
    statePacket[2] = 0x1000000000008002ULL;
    statePacket[3] = 0xE;
    if (work->flag) statePacket[4] = 0x51801;
    else statePacket[4] = 0x31801;
    statePacket[5] = 0x47;
    statePacket[6] = 0x44;
    statePacket[7] = 0x42;
    sdfAppendPacket(stateList, (u32)statePacket);
    D_003252A8.append(&D_003252A8, stateList);
    stateList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(stateList);
    statePacket = (u64 *)sdfAllocPacketAligned(0x40);
    statePacket[0] = 3;
    statePacket[1] = 0x5000000310000000ULL;
    statePacket[2] = 0x1000000000008002ULL;
    statePacket[3] = 0xE;
    if (work->flag) statePacket[4] = 0x51801;
    else statePacket[4] = 0x31801;
    statePacket[5] = 0x47;
    statePacket[6] = 0x42;
    statePacket[7] = 0x42;
    sdfAppendPacket(stateList, (u32)statePacket);
    surface = &kwlnDrawSurfaces[60];
    surface->append(surface, stateList);
    (surface - 1)->append(surface - 1, primaryList);
    surface->append(surface, secondaryList);
}

/* Copy one full vector quadword through vf10, including its fourth component. */
void effEventCopyVector(void *dst, void *src) {
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
    effEventBillSetCreate(D_003563F0);
}


EffChanWork *effEventCreateChannelFromParams(const EffChanHead *source, u16 kind, void *params) {
    const EffChanHead *head = source;
    u32 recordCount = head->count;
    SdfMemBlock *allocation = sdfAllocGeneralBlock(recordCount * sizeof(EffChanRecord) + sizeof(EffChanWork));
    EffChanWork *work = (EffChanWork *)sdfResourceRetainAddress(allocation);
    EffChanRecord *record = (EffChanRecord *)(work + 1);
    EffParamWork *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = *head;
    work->buffer = allocation;
    work->records = record;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = effAllocSlotArray(recordCount);
    if (recordCount != 0) {
        delayModulus = work->head.spread;
        parameterTemplate = effParamWorkCreate(kind, params);
        record->param = parameterTemplate;
        record->delay = -(effMiscRand((EffRandState *)effDefaultRandomState) % delayModulus);
        record++;
        for (recordIndex = 1; recordIndex < recordCount; recordIndex++) {
            record->param = effParamWorkDuplicate(parameterTemplate);
            record->delay = -(effMiscRand((EffRandState *)effDefaultRandomState) % delayModulus);
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

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB068);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB06C);

INCLUDE_SDATA(const s32, "effect/effEvent", effRectangleBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effTexturedBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effFilterBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB073);

INCLUDE_SDATA(const s32, "effect/effEvent", effColorRectangleEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effTexturedSquareEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effStaggeredBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB078);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB080);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB088);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB090);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB098);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0A0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0A8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0B0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0B8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0C0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0C8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0D0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0D8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0E0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0E8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0F0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0F8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB100);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB108);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB110);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB114);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB118);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB120);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB128);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB12C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB130);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB138);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB140);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB144);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB148);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB14C);

