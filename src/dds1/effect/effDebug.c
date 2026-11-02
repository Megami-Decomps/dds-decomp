#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "sdf_projection.h"
#include "sdf_compact_vertex_vif_packet.h"
#include "bill_object_api.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_map_position_api.h"
#include "sdf_vu0.h"
#include "sdf_vu_rotation_axes.h"
#include "sdf_sif_command.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "eff_param.h"
#include "sdf_heap_api.h"
#include "ee_mmi.h"
#include "btl_sound.h"
#include "eff_blur.h"
#include "eff_rectangle_blur_api.h"
#include "eff_event_draw.h"
#include "eff.h"
#include "eff_resource_rect_api.h"
#include "eff_math.h"
#include "eff_channel.h"
#include "eff_event.h"
#include "eff_event_work_api.h"
#include "eff_event_sound.h"
#include "eff_event_setup.h"
#include "eff_event_setup_readiness.h"
#include "eff_event_bill.h"
#include "eff_event_bill_batch.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "sdf_texture_primary_buffer_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_gs_blend.h"
#include "sdf_packet_alloc.h"
#include "eff_packet_params.h"
#include "sdf_asset_create.h"
#include "sdf_asset_state.h"
#include "evt_unit.h"
#include "mdl.h"

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

/* The event curve array stores segmented slots, separate from effMath's cubic slots. */
typedef struct EffSegmentedBezierArray {
    EffSegmentedBezierSlot *slots;
    u32 slotCount;
    struct sdfMemHandle_t *allocation;
} EffSegmentedBezierArray;

extern sdfGrOt_t kwlnPositionedTextSurface;

/* Allocate contiguous slots followed by their count and allocation handle. */
EffSegmentedBezierArray *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * EFF_EVENT_BEZIER_SLOT_BYTES;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(slotBytes + EFF_EVENT_SLOT_HEADER_BYTES);
    EffSegmentedBezierSlot *slot = (EffSegmentedBezierSlot *)sdfMemLock(allocation);
    EffSegmentedBezierArray *table = (EffSegmentedBezierArray *)((u8 *)slot + slotBytes);
    u32 index = 0;
    table->allocation = allocation;
    table->slots = slot;
    table->slotCount = count;
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
void effReleaseSlotArrayAllocation(EffSegmentedBezierArray *header) {
    sdfMemFreeDirect(header->allocation);
}

/* Return 0 before evaluation only for the point-index sentinel 7. Overflow t
 * clamps to 1 and advances the index by three; there is no general bounds check. */
s32 effStepActiveBezierSlot(EffSegmentedBezierArray *table, s32 index, f32 *out) {
    EffSegmentedBezierSlot *slot = &table->slots[index];
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
s32 effPCPBezierEval(EffSegmentedBezierSlot *slot, f32 *out) {
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
void effInitSlotTail(EffSegmentedBezierArray *table, s32 index) {
    EffSegmentedBezierSlot *slot = &table->slots[index];

    slot->parameterStep = EFF_EVENT_CURVE_DEFAULT_STEP;
    slot->pointIndex = slot->t = 0;
}

/* Return the selected slot; index bounds remain the caller's responsibility. */
EffSegmentedBezierSlot *effGetSlotAt(EffSegmentedBezierArray *table, s32 index) {
    return &table->slots[index];
}

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

/* Project a point and draw a filled/bordered marker with 0x20/0x10 GS-unit extents. */
void effDebugPointDraw(f32 *position) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_EVENT_DRAW_LIST_BYTES);
    f32 screen[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;
    sdfGrOt_t *scene;

    sdfDrawTagInit(list);
    SDF_VU0_LOADVECTOR(position);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(screen);
    x = (s32)screen[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y = (s32)screen[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x;
    pixel[1] = y;
    sdfDrawTagLink(list, (u32)(dds3PrimFrameRect((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_WIDTH, EFF_EVENT_MARKER_HEIGHT, EFF_EVENT_MARKER_COLOR, EFF_EVENT_MARKER_COLOR)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (sdfDrawTag_t *)list);
}

/* Project a point and use color for both the marker's fill and border. */
void effDebugPointDraw2(f32 *position, s32 color) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_EVENT_DRAW_LIST_BYTES);
    f32 screen[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;
    sdfGrOt_t *scene;

    sdfDrawTagInit(list);
    SDF_VU0_LOADVECTOR(position);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(screen);
    x = (s32)screen[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y = (s32)screen[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x;
    pixel[1] = y;
    sdfDrawTagLink(list, (u32)(dds3PrimFrameRect((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_WIDTH, EFF_EVENT_MARKER_HEIGHT, color, color)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (sdfDrawTag_t *)list);
}

extern u64 *dds3PrimLine(s32 x0, s32 y0, s32 z0, s32 color0, s32 x1, s32 y1, s32 z1, s32 color1, s32 flags);

/* Draw a GS line between two projected points, using the fixed marker color. */
void effDebugLineDraw(f32 *from, f32 *to) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_EVENT_DRAW_LIST_BYTES);
    f32 start[EFF_EVENT_VECTOR_COMPONENTS];
    f32 end[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    sdfGrOt_t *scene;

    sdfDrawTagInit(list);
    SDF_VU0_LOADVECTOR(from);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(start);
    SDF_VU0_LOADVECTOR(to);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(end);
    x0 = (s32)start[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y0 = (s32)start[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    x1 = (s32)end[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y1 = (s32)end[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x0;
    pixel[1] = y0;
    pixel[2] = x1;
    pixel[3] = y1;
    sdfDrawTagLink(list, (u32)(dds3PrimLine((x0 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y0 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_COLOR, (x1 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y1 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, EFF_EVENT_MARKER_COLOR, 0)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (sdfDrawTag_t *)list);
}

/* Draw a GS line between two projected points, using color at both vertices. */
void effDebugLineDraw2(f32 *from, f32 *to, s32 color) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_EVENT_DRAW_LIST_BYTES);
    f32 start[EFF_EVENT_VECTOR_COMPONENTS];
    f32 end[EFF_EVENT_VECTOR_COMPONENTS];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    sdfGrOt_t *scene;

    sdfDrawTagInit(list);
    SDF_VU0_LOADVECTOR(from);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(start);
    SDF_VU0_LOADVECTOR(to);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(end);
    x0 = (s32)start[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y0 = (s32)start[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    x1 = (s32)end[0] - EFF_EVENT_PROJECTED_X_BIAS;
    y1 = (s32)end[1] * EFF_EVENT_PROJECTED_Y_SCALE - EFF_EVENT_PROJECTED_Y_BIAS;
    pixel[0] = x0;
    pixel[1] = y0;
    pixel[2] = x1;
    pixel[3] = y1;
    sdfDrawTagLink(list, (u32)(dds3PrimLine((x0 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y0 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, color, (x1 << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y1 << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, color, 0)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (sdfDrawTag_t *)list);
}

/* Submit a formatted native command at biased screen coordinates. */
void effDebugStringDraw(s32 x, s32 y, s32 commandIndex, s32 format) {
    sdfDrawTag_t *task = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_EVENT_DRAW_LIST_BYTES);
    sdfGrOt_t *scene;

    sdfDrawTagInit(task);
    sdfDrawTagLink(task, (u32)(sdfDevPrintf((x << EFF_EVENT_GS_X_SHIFT) + EFF_EVENT_GS_X_BIAS, (y << EFF_EVENT_GS_Y_SHIFT) + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, commandIndex, (const char *)format)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (sdfDrawTag_t *)task);
}

extern sdfGrOt_t sdfGrOtTable[];

void effDebugWireframeSphereDraw(const f32 *center, u32 color, f32 radius) {
    f32 point[4] __attribute__((aligned(16)));
    f32 matrix[4][4] __attribute__((aligned(16)));
    f32 vertices[40][4] __attribute__((aligned(16)));
    u32 colors[40];
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    f32 (*out)[4];
    f32 latitude;
    f32 longitude;
    f32 ringRadius;
    f32 ringY;
    u32 pass;
    u32 ring = 0;
    u32 segment;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
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
            ringY = radius * sdfCos(latitude);
            ringRadius = radius * sdfSin(latitude);
            VU0_LOAD_MATRIX(matrix);
            SDF_VU0_LOADVECTOR_B(center);
            point[0] = ringRadius;
            point[1] = ringY;
            point[2] = 0.0f;
            SDF_VU0_LOADVECTOR(point);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();

            for (segment = 0; segment < 20; segment++) {
                SDF_VU0_STOREVECTOR(*out);
                out++;
                longitude += 0.31415924429893494f;
                point[0] = sdfCos(longitude) * ringRadius;
                point[1] = ringY;
                point[2] = sdfSin(longitude) * ringRadius;
                SDF_VU0_LOADVECTOR(point);
                SDF_VU0_VECTOR_W_SET_ZERO();
                SDF_VU0_APPLYMATRIX();
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(*out);
                out++;
            }
            sdfDrawTagLink(list, (u32)effDebugDrawWireModel(vertices, colors, 40, 0x80));
        }
        SetRotateMatrixX(1.5707963f);
        VU0_LOAD_MATRIX_B_F32(matrix);
        MultMatrixNoArg();
        VU0_STORE_MATRIX_UNCLOBBERED(matrix);
    }
    surface = &sdfGrOtTable[55];
    surface->append(surface, list);
}

extern sdfMat_t *D_003BB068;

extern f32 D_003BB06C;

extern f32 effTriangleVertexScratch[4][4];

extern u32 D_00355870[4];

extern sdfGrOt_t D_00325028;

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *params);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset, s32 (*allocatePacket)(s32));

extern void sdf3DPrimLinkTm(sdfDrawTag_t *packetList, s32 (*allocatePacket)(s32));

void effDebugSelectCursorDraw(f32 *position) {
    sdfMat_t *asset = D_003BB068;
    sdfDrawTag_t *list;
    sdf3DPrimPolyParm_t packet;

    if (asset == NULL) {
        asset = sdfMatNew();
        D_003BB068 = asset;
        sdfMatSetConstant(asset, 1.0f);
    }

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    SetRotateMatrixY(D_003BB06C);
    D_003BB06C += 0.10471974313259125f;
    SDF_VU0_LOADVECTOR(position);
    VU0_ADD_VF31_VF10_XYZ();
    sdf3DPrimLinkTm(list, NULL);
    memset(&packet, 0, 0x2C);
    packet.attr = 0x4000;
    effTriangleVertexScratch[2][2] = 0.0f;
    packet.vertexColor = D_00355870;
    packet.color = 0x80808080;

    effTriangleVertexScratch[0][0] = 0.0f;
    effTriangleVertexScratch[0][1] = 0.0f;
    effTriangleVertexScratch[0][2] = 0.0f;
    effTriangleVertexScratch[1][0] = 15.0f;
    effTriangleVertexScratch[1][1] = -30.0f;
    effTriangleVertexScratch[1][2] = 0.0f;
    effTriangleVertexScratch[2][0] = -15.0f;
    effTriangleVertexScratch[2][1] = -30.0f;

    packet.faceCount = 1;
    packet.pointCount = 3;
    packet.vertex = (u128 *)effTriangleVertexScratch;
    packet.uv = NULL;
    sdf3DPrimLinkMat(list, D_003BB068, NULL);
    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&packet));
    D_00325028.append(&D_00325028, list);
}

/* Draw horizontal rings, vertical sides, and the two sets of cap diameters. */
void effDebugWireframeCylinderDraw(const f32 *center, u32 color, f32 radius, f32 height) {
    f32 point[4]; /* The line provider consumes xyz; preserve the untouched w lane. */
    f32 vertices[40][4];
    u32 colors[40];
    sdfDrawTag_t *list;
    f32 (*out)[4];
    sdfGrOt_t *surface;
    u32 ring;
    u32 segment;
    f32 y;
    f32 heightStep;
    f32 angleStep;
    f32 angle;
    f32 oppositeAngle;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
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
        SDF_VU0_LOADVECTOR_B(center);
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_ADDVECTOR();
        for (segment = 0; segment < 20; segment++) {
            SDF_VU0_STOREVECTOR(*out);
            out++;
            angle += angleStep;
            point[0] = sdfCos(angle) * radius;
            point[1] = y;
            point[2] = sdfSin(angle) * radius;
            SDF_VU0_LOADVECTOR(point);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(*out);
            out++;
        }
        sdfDrawTagLink(list, (u32)effDebugDrawWireModel(vertices, colors, 40, 0x80));
        y += heightStep;
    }

    out = vertices;
    SDF_VU0_LOADVECTOR_B(center);
    angle = 0.0f;
    for (segment = 0; segment < 20; segment++) {
        point[0] = sdfCos(angle) * radius;
        point[1] = -height * 0.5f;
        point[2] = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(*out);
        out++;
        point[0] = sdfCos(angle) * radius;
        point[1] = height * 0.5f;
        point[2] = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(*out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, (u32)effDebugDrawWireModel(vertices, colors, 40, 0x80));

    out = vertices;
    SDF_VU0_LOADVECTOR_B(center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point[0] = sdfCos(angle) * radius;
        point[1] = -height * 0.5f;
        point[2] = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(*out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point[0] = sdfCos(oppositeAngle) * radius;
        point[1] = -height * 0.5f;
        point[2] = sdfSin(oppositeAngle) * radius;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(*out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, (u32)effDebugDrawWireModel(vertices, colors, 20, 0x80));

    out = vertices;
    SDF_VU0_LOADVECTOR_B(center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point[0] = sdfCos(angle) * radius;
        point[1] = height * 0.5f;
        point[2] = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(*out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point[0] = sdfCos(oppositeAngle) * radius;
        point[1] = height * 0.5f;
        point[2] = sdfSin(oppositeAngle) * radius;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(*out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, (u32)effDebugDrawWireModel(vertices, colors, 20, 0x80));
    surface = &sdfGrOtTable[55];
    surface->append(surface, list);
}

/* Submit a filled rectangle and its line-strip border. Keep native width/height
 * scaling and the distinct shift/multiply forms used by the other draw paths. */
void effDebugWindowDraw(s32 x, s32 y, s32 width, s32 height, s32 fillColor, s32 borderColor) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_EVENT_DRAW_LIST_BYTES);
    sdfGrOt_t *scene;

    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (u32)(dds3PrimFrameRect(x * EFF_EVENT_GS_X_SCALE + EFF_EVENT_GS_X_BIAS, y * EFF_EVENT_GS_Y_SCALE + EFF_EVENT_GS_Y_BIAS, EFF_EVENT_OVERLAY_DEPTH, width * EFF_EVENT_GS_X_SCALE, height * EFF_EVENT_GS_Y_SCALE, fillColor, borderColor)));
    scene = &kwlnPositionedTextSurface;
    scene->append(scene, (sdfDrawTag_t *)list);
}
INCLUDE_SDATA(const s32, "effect/effDebug", D_003BB068);

INCLUDE_SDATA(const s32, "effect/effDebug", D_003BB06C);

