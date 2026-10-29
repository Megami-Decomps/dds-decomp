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
#include "sdf_resource.h"
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
#define EFF_EVENT_HALF_TURN 3.14159265f
#define EFF_EVENT_RANDOM_MIDPOINT 0.5f
#define EFF_EVENT_JITTER_RANGE 0.3f
#define EFF_EVENT_JITTER_BASE 0.7f

/* The event curve array stores segmented slots, separate from effMath's cubic slots. */
typedef struct EffSegmentedBezierArray {
    EffSegmentedBezierSlot *slots;
    u32 slotCount;
    struct sdfMemHandle_t *allocation;
} EffSegmentedBezierArray;

typedef char EffSegmentedBezierArraySizeAssert[(sizeof(EffSegmentedBezierArray) == 0xC) ? 1 : -1];
typedef char EffSegmentedBezierArraySlotsOffsetAssert[
    ((u32)&((EffSegmentedBezierArray *)0)->slots == 0x00) ? 1 : -1];
typedef char EffSegmentedBezierArrayCountOffsetAssert[
    ((u32)&((EffSegmentedBezierArray *)0)->slotCount == 0x04) ? 1 : -1];
typedef char EffSegmentedBezierArrayAllocationOffsetAssert[
    ((u32)&((EffSegmentedBezierArray *)0)->allocation == 0x08) ? 1 : -1];



/* Packet-source layouts and concrete blur owners mirror their constructors.
 * Equal-sized parameter prefixes do not make the blur variants interchangeable. */

/* EffBlurTemplate and its copied body are declared in eff_blur.h. */









extern EffScreenDrawParams Eve_Blur;
extern EffScreenDrawParams D_00355908;
extern dds3Effect_Filter_t Eve_Filter;
extern EffScreenDrawParams D_003559A0;
extern dds3Effect_PartialMultiBlurParam_t effTexturedBlurSetupParameters;
extern dds3Effect_DistortionBlurParam_t effScatterBlurSetupParameters;
extern EffScreenDrawParams D_00355E48;
extern dds3Effect_Filter_t D_00355F88;
extern dds3Effect_PartialMultiFilterParam_t effResourceRectangleSetupParameters;
extern dds3Effect_RippleBlurParam_t effStaggeredBlurSetupParameters;

extern EffResourceRectWork *hEve_PartialMultiFilter;

extern s8 Eve_PartialMultiFilterShow;

extern s8 Eve_FilterShow;

extern s8 D_003BB073;

extern EffBlurScaleWork *hEve_RippleBlur;

extern s8 Eve_RippleBlurShow;

extern EffBlurScatterWork *hEve_DistortionBlur;

extern s8 Eve_DistortionBlurShow;

extern EffBlurTemplate *hEve_PartialMultiBlur;

extern s8 Eve_PartialMultiBlurShow;

extern s8 Eve_BlurShow;
extern u8 D_003558D8[];
extern u8 D_003558A8[];
extern u8 D_00355948[];
extern u8 D_00355970[];
extern sdfGrOt_t kwlnPositionedTextSurface;
#include "sdf_packet_alloc.h"

extern void effBlurStepScaleSlotsAndDraw(EffBlurScaleWork *arg);
extern void dds3EffectFilter(dds3Effect_Filter_t *arg);

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
        sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX_UNCLOBBERED(matrix);
    }
    surface = &sdfGrOtTable[55];
    surface->append(surface, list);
}

#include "eff_packet_params.h"
#include "sdf_asset_create.h"
#include "sdf_asset_state.h"

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

extern sdfGrOt_t sdfGrOtTable[];

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

void Eve_BlurON(void) {
    Eve_BlurShow = 1;
}

void Eve_BlurOFF(void) {
    Eve_BlurShow = 0;
}

/* Copy only the 0x28-byte source prefix; retain the screen record's trailing bytes. */
void Eve_BlurSetParam(const dds3Effect_Blur_t *parameters) {
    memcpy(&Eve_Blur, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the rectangle-blur parameter block by address. */
EffScreenDrawParams *Eve_BlurGetParam(void) {
    return &Eve_Blur;
}

void Eve_PartialMultiBlurON(void) {
    Eve_PartialMultiBlurShow = 1;
}

void Eve_PartialMultiBlurOFF(void) {
    Eve_PartialMultiBlurShow = 0;
}

/* Copy the pixel rectangle only; retain its selected source resource. */
void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *parameters) {
    hEve_PartialMultiBlur->body = *parameters;
}

/* Return the current textured-blur work without retaining it. */
EffBlurTemplate *Eve_PartialMultiBlurGetParams(void) {
    return hEve_PartialMultiBlur;
}

void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *texture) {
    hEve_PartialMultiBlur->texture = texture;
}

/* Select the same resource-table entry used by the filter-blur initializer. */
void Eve_PartialMultiBlurSetDefTex(void) {
    hEve_PartialMultiBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void Eve_DistortionBlurON(void) {
    Eve_DistortionBlurShow = 1;
}

void Eve_DistortionBlurOFF(void) {
    Eve_DistortionBlurShow = 0;
}

/* Update scatter parameters without replacing the owned allocation or slots. */
void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *parameters) {
    hEve_DistortionBlur->params = *parameters;
}

/* Return the current filter-blur work without retaining it. */
EffBlurScatterWork *Eve_DistortionBlurGetParams(void) {
    return hEve_DistortionBlur;
}

void Eve_DistortionBlurSetTex(sdfTexHandle_t *sourceHandle) {
    hEve_DistortionBlur->texture = sourceHandle;
}

/* Install the filter-blur resource-table texture without changing its slots. */
void Eve_DistortionBlurSetDefTex(void) {
    hEve_DistortionBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void Eve_RippleBlurON(void) {
    Eve_RippleBlurShow = 1;
}

void Eve_RippleBlurOFF(void) {
    Eve_RippleBlurShow = 0;
}

/* Update scale parameters without replacing the owned allocation or slots. */
void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *parameters) {
    hEve_RippleBlur->params = *parameters;
}

/* Return the current staggered-blur work without retaining it. */
EffBlurScaleWork *Eve_RippleBlurGetParams(void) {
    return hEve_RippleBlur;
}

void Eve_RippleBlurSetTex(sdfTexHandle_t *sourceHandle) {
    hEve_RippleBlur->texture = sourceHandle;
}

/* Install the staggered-blur resource-table texture without changing its slots. */
void Eve_RippleBlurSetDefTex(void) {
    hEve_RippleBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_STAGGERED_RESOURCE_INDEX);
}

void effEnableFramebufferQuad(void) {
    D_003BB073 = 1;
}

void effDisableFramebufferQuad(void) {
    D_003BB073 = 0;
}

/* Copy only the framebuffer source prefix, not the trailing screen-record bytes. */
void effCopyFramebufferQuadParameters(const dds3Effect_Blur_t *source) {
    memcpy(&D_00355908, source, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the framebuffer-quad parameter block by address. */
EffScreenDrawParams *effGetCh73Params(void) {
    return &D_00355908;
}

void Eve_FilterON(void) {
    Eve_FilterShow = 1;
}

void Eve_FilterOFF(void) {
    Eve_FilterShow = 0;
}

/* Replace the complete solid-rectangle parameter block. */
void Eve_FilterSetParam(dds3Effect_Filter_t *parameters) {
    Eve_Filter = *parameters;
}

/* Expose the solid-rectangle parameter block by address. */
dds3Effect_Filter_t *Eve_FilterGetParams(void) {
    return &Eve_Filter;
}

void Eve_PartialMultiFilterON(void) {
    Eve_PartialMultiFilterShow = 1;
}

void Eve_PartialMultiFilterOFF(void) {
    Eve_PartialMultiFilterShow = 0;
}

/* Copy the resource template body while preserving its selected resource. */
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters) {
    hEve_PartialMultiFilter->params = *parameters;
}

/* Return the current textured-square work without retaining it. */
EffResourceRectWork *Eve_PartialMultiFilterGetParams(void) {
    return hEve_PartialMultiFilter;
}

void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *texture) {
    hEve_PartialMultiFilter->texture = texture;
}

/* Install the textured-square resource-table texture while retaining its body. */
void Eve_PartialMultiFilterSetDefTex(void) {
    hEve_PartialMultiFilter->texture = dds3BillboardGetGeneralTex(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}

/* Clone the four default work templates; only the staggered slot count is overridden. */
void Eve_EffectInit(void) {
    hEve_PartialMultiBlur = dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)D_003558D8);
    hEve_DistortionBlur = dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)D_003558A8);
    hEve_PartialMultiFilter = dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)D_00355948);
    hEve_RippleBlur = dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)D_00355970);
    Eve_RippleBlurGetParams()->params.count = EFF_EVENT_STAGGERED_SLOT_COUNT;
}

/* Dispatch enabled draw families independently, in their native order. */
void effDispatchActive(void) {
    if (Eve_BlurShow) {
        effDrawBlurRectangle(&Eve_Blur.source);
    }
    if (Eve_PartialMultiBlurShow) {
        effDrawBlurPixelRectWithResource(hEve_PartialMultiBlur);
    }
    if (Eve_DistortionBlurShow) {
        effBlurStepScatterSlotsAndDraw(hEve_DistortionBlur);
    }
    if (Eve_RippleBlurShow) {
        effBlurStepScaleSlotsAndDraw(hEve_RippleBlur);
    }
    if (D_003BB073) {
        effBlurDrawFramebufferQuad(&D_00355908.source);
    }
    if (Eve_FilterShow) {
        dds3EffectFilter(&Eve_Filter);
    }
    if (Eve_PartialMultiFilterShow) {
        effResourceRectDrawPixels(hEve_PartialMultiFilter);
    }
}

/* callbackResult must be valid whenever callback is present. */
extern EffEventSetupRecord effRectangleBlurSetupRecord;
extern s8 effRectangleBlurSetupReady;
extern s8 D_0039862B[];
extern void effEventSetupBeforeRecordNoOp(void);
extern void effEventSetupRecordNoOp(void *);
extern void effEventSetupAfterRecordNoOp(void);

/* Prepare once per flag cycle, then run the registered operations while ready.
 * A negative control byte clears readiness only after those operations. */
EffEventSetupReadiness effUpdateCh72Params(void) {
    u8 ready = effRectangleBlurSetupReady;

    if (effRectangleBlurSetupReady == 0) {
        EffEventSetupRecord *state = &effRectangleBlurSetupRecord;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(state->source);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        effRectangleBlurSetupReady = ready;
    }
    if (ready != 0) {
        effReturnEventSetupWord((u32)&effRectangleBlurSetupRecord);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&effRectangleBlurSetupRecord);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0039862B[0] < 0) {
        effRectangleBlurSetupReady = 0;
    }
    return effRectangleBlurSetupReady;
}

/* Expose this screen-draw setup block; no parameter copy is made. */
EffScreenDrawParams *effGetLoadDescA(void) {
    return &D_003559A0;
}

/* Replace the screen-draw source prefix while preserving the trailing bytes. */
void effSetLoadDescA(const dds3Effect_Blur_t *parameters) {
    memcpy(&D_003559A0, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffEventSetupRecord mblurDebugMenu;
extern s8 mblurDone;

/* Prepare the blur-template callback from its explicit parameter block,
 * then process the channel; its optional copy still uses source/destination. */
EffEventSetupReadiness effEventAdvanceBlurTemplateSetup(void) {
    u8 ready = mblurDone;

    if (mblurDone == 0) {
        EffEventSetupRecord *state = &mblurDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&effTexturedBlurSetupParameters);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        mblurDone = ready;
    }
    if (ready != 0) {
        effReturnEventSetupWord((u32)&mblurDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&mblurDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0039862B[0] < 0) {
        mblurDone = 0;
    }
    return mblurDone;
}

/* Expose the blur-template setup parameters by address. */
dds3Effect_PartialMultiBlurParam_t *effEventGetBlurTemplateSetupParams(void) {
    return &effTexturedBlurSetupParameters;
}

/* Replace the setup template; this does not modify the active blur work. */
void effEventSetBlurTemplateParameters(dds3Effect_PartialMultiBlurParam_t *parameters) {
    effTexturedBlurSetupParameters = *parameters;
}

extern EffEventSetupRecord dblurDebugMenu;
extern s8 dblurDone;

/* Prepare scatter setup from its explicit parameters, then process the channel. */
EffEventSetupReadiness effEventAdvanceScatterBlurSetup(void) {
    u8 ready = dblurDone;

    if (dblurDone == 0) {
        EffEventSetupRecord *state = &dblurDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&effScatterBlurSetupParameters);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        dblurDone = ready;
    }
    if (ready != 0) {
        effReturnEventSetupWord((u32)&dblurDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&dblurDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0039862B[0] < 0) {
        dblurDone = 0;
    }
    return dblurDone;
}

/* Expose the scatter-blur setup parameters by address. */
dds3Effect_DistortionBlurParam_t *effEventGetScatterBlurSetupParams(void) {
    return &effScatterBlurSetupParameters;
}

/* Replace setup parameters without replacing the active filter-blur work. */
void effEventSetScatterBlurParameters(dds3Effect_DistortionBlurParam_t *parameters) {
    effScatterBlurSetupParameters = *parameters;
}

extern EffEventSetupRecord effFramebufferQuadSetupRecord;
extern s8 D_003BB114;

/* Prepare from the channel's source, process it, then honor the control-byte reset. */
EffEventSetupReadiness effEventAdvanceScreenDrawSetup(void) {
    u8 ready = D_003BB114;

    if (D_003BB114 == 0) {
        EffEventSetupRecord *state = &effFramebufferQuadSetupRecord;

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
        effReturnEventSetupWord((u32)&effFramebufferQuadSetupRecord);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&effFramebufferQuadSetupRecord);
        effEventSetupAfterRecordNoOp();
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
void effSetLoadDescD(const dds3Effect_Blur_t *parameters) {
    memcpy(&D_00355E48, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffEventSetupRecord filterDebugMenu;
extern s8 evtViewerDirty;

/* Prepare solid-rectangle setup from the channel source, then process it. */
EffEventSetupReadiness evtViewerCheckDirty(void) {
    u8 ready = evtViewerDirty;

    if (evtViewerDirty == 0) {
        EffEventSetupRecord *state = &filterDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(state->source);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        evtViewerDirty = ready;
    }
    if (ready != 0) {
        effReturnEventSetupWord((u32)&filterDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&filterDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0039862B[0] < 0) {
        evtViewerDirty = 0;
    }
    return evtViewerDirty;
}

/* Expose the solid-rectangle setup parameters by address. */
dds3Effect_Filter_t *effEventGetSolidRectangleSetupParams(void) {
    return &D_00355F88;
}

/* Replace the setup block rather than the active solid-rectangle parameters. */
void effEventSetSolidRectangleParameters(dds3Effect_Filter_t *parameters) {
    D_00355F88 = *parameters;
}

extern EffEventSetupRecord mfilterDebugMenu;
extern s8 D_003BB12C;

/* Prepare resource-template setup from its explicit body, then process the channel. */
EffEventSetupReadiness mfilterRun(void) {
    u8 ready = D_003BB12C;

    if (D_003BB12C == 0) {
        EffEventSetupRecord *state = &mfilterDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&effResourceRectangleSetupParameters);
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
        effReturnEventSetupWord((u32)&mfilterDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&mfilterDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0039862B[0] < 0) {
        D_003BB12C = 0;
    }
    return D_003BB12C;
}

/* Expose the resource-template setup body by address. */
dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void) {
    return &effResourceRectangleSetupParameters;
}

/* Replace the setup body without replacing the active textured-square work. */
void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters) {
    effResourceRectangleSetupParameters = *parameters;
}

extern EffEventSetupRecord rblurDebugMenu;
extern s8 rblurDone;

/* Prepare scale-blur setup from its explicit parameters, then process the channel. */
EffEventSetupReadiness rblurProcess(void) {
    u8 ready = rblurDone;

    if (rblurDone == 0) {
        EffEventSetupRecord *state = &rblurDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&effStaggeredBlurSetupParameters);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        rblurDone = ready;
    }
    if (ready != 0) {
        effReturnEventSetupWord((u32)&rblurDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&rblurDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0039862B[0] < 0) {
        rblurDone = 0;
    }
    return rblurDone;
}

/* Expose the scale-blur setup parameters by address. */
dds3Effect_RippleBlurParam_t *rblurGetParam(void) {
    return &effStaggeredBlurSetupParameters;
}

/* Replace setup parameters without replacing the active staggered-blur work. */
void rblurSetParam(dds3Effect_RippleBlurParam_t *parameters) {
    effStaggeredBlurSetupParameters = *parameters;
}

SoundMixer *effEveBEDCreate(SoundMixer *source) {
    return sndMixerClone(source);
}

void effEveBEDDelete(SoundMixer *mixer) {
    sndMixerDestroy(mixer);
}


extern u8 D_003563F0[];


extern s32 D_003BB140;

extern billHandle_t *D_003BB144;

extern billHandle_t *D_003BB148;

/* Compact event owner: the allocator reserves exactly 0x38 bytes. */
struct effEveBEDCutHandle_t {
    EffEventInit init;
    struct evtUnitHandle_t *actor; /* Linked event unit supplies its model owner. */
    effBTLCutHandle_t *effect;
};



struct BtlUnit;

extern void effBTLCutSetScale(effBTLCutHandle_t *effect, f32 scale);

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
effEveBEDCutHandle_t *effEveBEDCutCreate(SoundMixer *owner, u16 kind, const void *params) {
    const EffEventInit *init = (const EffEventInit *)params;
    effEveBEDCutHandle_t *work = sdfChipAllocDirect(sizeof(effEveBEDCutHandle_t));

    memcpy(work, init, sizeof(*init));
    work->actor = NULL;
    work->effect = effCreateBattleVoiceOwner(owner, kind, 0, 0);
    effBTLCutSetScale(work->effect, init->param);
    return work;
}

/* Release the attached effect before freeing the event work. */
void effEveBEDCutDelete(effEveBEDCutHandle_t *work) {
    effBTLCutDelete(work->effect);
    sdfChipFreeDirect(work);
}

/* Copy the serialized init prefix, retaining the event's actor and effect links. */
void effEveBEDCutSetParams(effEveBEDCutHandle_t *destination, const void *source) {
    memcpy(&destination->init, source, sizeof(destination->init));
}

/* Copy one serialized event record; either byte buffer may be unaligned. */
void effEveBEDCutGetParams(const void *source, void *destination) {
    memcpy(destination, source, EFF_EVENT_EVENT_RECORD_BYTES);
}

/* Forward the effect's scale value without modifying the record's init prefix. */
void effEveBEDCutSetScale(effEveBEDCutHandle_t *work, f32 scale) {
    effBTLCutSetScale(work->effect, scale);
}

void effEveBEDCutSetUnitHandle(effEveBEDCutHandle_t *work, struct evtUnitHandle_t *actor) {
    work->actor = actor;
}

#include "evt_unit.h"
#include "mdl.h"

extern void effEveBEDPartGetPositionPtr(EffEventInit *source, EffectVectorRequest *params);
extern const f32 D_003A12E0[4];

/* Callbacks consume the VU0 output buffers; see docs/idioms.md,
 * "Event-effect SDK output frontiers". */
void effEventUpdateEffectParameters(effEveBEDCutHandle_t *work) {
    effBTLCutHandle_t *effect;
    EffBattleEntryList *list;
    EffBattleEntry *entry;
    sdfModelRootNode_t *model;
    f32 position[4];
    f32 rotationMatrix[4][4];
    f32 matrix[4][4];
    f32 scale;
    s32 entryIndex;
    u32 frame;
    u32 color;
    u16 count;

    effect = work->effect;
    frame = effect->currentFrame;
    list = effect->list;
    SDF_VU0_LOADVECTOR(work->init.orientation);
    SDF_VU0_LOADVECTOR_B(D_003A12E0);
    effMiscQuatMultiplyVU();
    effMiscQuaternionToMatrixVU();
    VU0_STORE_MATRIX_NOCLOBBER(rotationMatrix);
    count = list->count;
    entry = list->entries;
    color = work->init.color;
    for (entryIndex = 0; entryIndex < count; entryIndex++, entry++) {
        if (entry->zeroKindCount < frame && effect->parameterWorks[entryIndex] != NULL) {
            if (entry->aimParams.kind != 5 || work->actor == NULL) {
                effEveBEDPartGetPositionPtr(&work->init, &entry->aimParams);
                SDF_VU0_STOREVECTOR_NOMEM(position);
                dds3EffectPartSetPosition(effect->parameterWorks[entryIndex], position);
                dds3EffectPartSetRotateMatrix(effect->parameterWorks[entryIndex], rotationMatrix);
            } else {
                model = work->actor->owner->inner;
                sdfModelGetMarkPos(model, entry->aimParams.count);
                SDF_VU0_STOREVECTOR_NOMEM(position);
                dds3EffectPartSetPosition(effect->parameterWorks[entryIndex], position);
                /* The callback can change the entry's scale kind. */
                if (entry->aimParams.kind == 5) {
                    sdfModelGetMarkPosMatrix(model, entry->aimParams.count);
                    SDF_VU0_LOADMATRIX_B(rotationMatrix);
                    sdfComposeVuMatrixFromRegisters();
                    VU0_STORE_MATRIX_NOCLOBBER(matrix);
                    dds3EffectPartSetRotateMatrix(effect->parameterWorks[entryIndex], matrix);
                } else {
                    mdlGetScale(work->actor->owner);
                    SDF_VU0_GETVECTOR_X(scale);
                    sdfModelGetMarkPosMatrix(model, entry->aimParams.count);
                    if (scale != 0.0f) {
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALAR_OP(1.0f / scale, "vmulx.xyzw vf10, vf10, vf2x");
                        SDF_VU0_SCALE_XYZ();
                    }
                    VU0_STORE_MATRIX_NOCLOBBER(matrix);
                    dds3EffectPartSetRotateMatrix(effect->parameterWorks[entryIndex], matrix);
                }
            }
            dds3EffectPartSetColor(effect->parameterWorks[entryIndex], color);
            dds3EffectPartDraw(effect->parameterWorks[entryIndex]);
        }
    }
    if (effect->triggerFrame == frame && effect->triggerFrameCallback != NULL) {
        effect->triggerFrameCallback(effect);
    }
    effect->currentFrame++;
}

/* Initialize a holder and its effect record; the teardown flag starts set. */
effSimpleBED_t *effSimpleBEDCreate(SoundMixer *mixer, f32 param) {
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

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
    work->handle = effEveBEDCreate(mixer);
    work->owner = effEveBEDCutCreate(work->handle, 0, &work->init);
    work->handleOwnership = EFF_SIMPLE_BED_HANDLE_OWNED;
    return work;
}

/* Release the effect, invoke shared teardown only when flagged, then free the holder. */
void effSimpleBEDDelete(effSimpleBED_t *work) {
    effEveBEDCutDelete(work->owner);
    if (work->handleOwnership != EFF_SIMPLE_BED_HANDLE_BORROWED) {
        effEveBEDDelete(work->handle);
    }
    sdfChipFreeDirect(work);
}

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
effSimpleBED_t *effSimpleBEDCopy(effSimpleBED_t *src) {
    EffEventInit *block = &src->init;
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->owner = effEveBEDCutCreate(src->handle, 0, block);
    work->handle = src->handle;
    memcpy(&work->init, block, sizeof(work->init));
    work->handleOwnership = EFF_SIMPLE_BED_HANDLE_BORROWED;
    return work;
}

/* Apply the native owner operation to the pointer stored in the record prefix. */
void effSimpleBEDDraw(effSimpleBED_t *work) {
    effEventUpdateEffectParameters(work->owner);
}

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effSimpleBEDSetPosition(effSimpleBED_t *work, f32 *position) {
    work->init.position[0] = position[0];
    work->init.position[1] = position[1] - work->init.height * 0.5f;
    work->init.position[2] = position[2];
    work->init.position[3] = 0;
    effEveBEDCutSetParams(work->owner, &work->init);
}

/* Copy the holder's updated packed tint to its event record. */
void effSimpleBEDSetRGBA(effSimpleBED_t *work, u32 color) {
    work->init.color = color;
    effEveBEDCutSetParams(work->owner, &work->init);
}



extern f32 D_003563A0[];
extern f32 D_003563B8[];
extern f32 sdfViewTargetVector[4];
extern f32 aViewDown[4];

/* vu0 routine: leave the selected aim point in vf10, not a C return value.
 * Preserve the non-camera output scratch w, which is not initialized here. */
void effEveBEDPartGetPositionPtr(EffEventInit *source, EffectVectorRequest *params) {
    f32 out[EFF_EVENT_VECTOR_COMPONENTS];
    f32 dir[EFF_EVENT_VECTOR_COMPONENTS];
    f32 pos[EFF_EVENT_VECTOR_COMPONENTS];
    f32 radius;
    f32 half;
    f32 y;
    s32 rangeOverride = params->extentOverride;
    u8 directionMode = params->directionMode;
    u8 aimMode = params->count;

    if (rangeOverride == 0) {
        radius = source->range;
    } else {
        radius = (f32)rangeOverride;
    }
    half = source->height * 0.5f;
    SDF_VU0_COPYVECTOR(pos, source->position);
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
        LookAtMatrix(pos, sdfViewTargetVector, aViewDown);
        ReverseMatrix();
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(pos);
        SDF_VU0_ADDVECTOR();
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
        SDF_VU0_LOADVECTOR(source->orientation);
        effMiscQuaternionToMatrixVU();
        sdfVuLoadSecondaryRotationMatrixY(D_003563B8[directionMode]);
        sdfComposeVuMatrixFromRegisters();
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(dir);
        out[0] = pos[0] + radius * dir[0];
        out[1] = y + radius * dir[1];
        out[2] = pos[2] + radius * dir[2];
    }
    SDF_VU0_LOADVECTOR(out);
}

extern billHandle_t *billCreateFromResource(s32 kind, const char *path);

/* Allocate particles plus their owner, copy parameters and acquire shared textures. */
INCLUDE_RODATA(const s32, "effect/effEvent", D_003A12E0);

EffEventBillSet *effEVECombParSmokeCreate(EffEventBillParams *src) {
    u32 particleCount = src->particleCount;
    u32 particleBytes = particleCount * sizeof(EffEventBillParticle);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(particleBytes + sizeof(EffEventBillSet));
    EffEventBillParticle *particle = (EffEventBillParticle *)sdfMemLock(allocationHandle);
    EffEventBillSet *work = (EffEventBillSet *)((u8 *)particle + particleBytes);
    u32 particleIndex;

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
        D_003BB144 = billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_003BB148 = billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
    }
    particleCount = work->head.particleCount;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        particle->age = 0;
        particle++;
    }
    return work;
}

/* Drop the shared texture reference count; zero dispatches both resource handles.
 * The retained allocation is released afterwards, regardless of that count. */
void effEVECombParSmokeDelete(EffEventBillSet *work) {
    D_003BB140 = D_003BB140 - 1;
    if (D_003BB140 == 0) {
        dds3BillboardDelete(D_003BB144);
        dds3BillboardDelete(D_003BB148);
    }
    sdfMemFreeDirect(work->allocationHandle);
}

extern EffRandState effDefaultRandomState[];

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEVECombParSmokePartInit(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *particle = &work->particles[index];
    f32 direction[EFF_EVENT_VECTOR_COMPONENTS];
    f32 scaleFactor;
    f32 spawnRadius;
    s32 startDelaySpread = work->head.startDelaySpread;
    s32 motionDelaySpread = work->head.motionDelaySpread;

    particle->age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % startDelaySpread);
    particle->motionDelay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % motionDelaySpread);
    scaleFactor = dds3GetRandFloatA(effDefaultRandomState) * work->head.scaleRandomness + (1.0f - work->head.scaleRandomness);
    particle->baseScale = work->head.baseScale * scaleFactor;
    particle->scalePhaseX = dds3GetRandFloatA(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseY = dds3GetRandFloatA(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseStepX = work->head.scalePhaseStep * (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f);
    particle->scalePhaseStepY = work->head.scalePhaseStep * (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f);
    particle->scaleAmplitudeX = work->head.scaleAmplitudeX * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->scaleAmplitudeY = work->head.scaleAmplitudeY * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->rotation = dds3GetRandFloatA(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    if (dds3GetRandA((EffRandState *)effDefaultRandomState) & 1) {
        particle->angularSpeed = work->head.angularSpeed * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE);
    } else {
        particle->angularSpeed = -(work->head.angularSpeed * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE));
    }
    spawnRadius = work->head.spawnRadius;
    direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(direction);
    /* Each coordinate consumes its own radial random factor, not one shared radius. */
    particle->position[0] = spawnRadius * dds3GetRandFloatA(effDefaultRandomState) * direction[0];
    particle->position[1] = spawnRadius * dds3GetRandFloatA(effDefaultRandomState) * direction[1];
    particle->position[2] = spawnRadius * dds3GetRandFloatA(effDefaultRandomState) * direction[2];
    particle->motion[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    particle->motion[1] = 0;
    particle->motion[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(particle->motion);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->motion);
    particle->swayAmplitude = work->head.swayAmplitude * (dds3GetRandFloatA(effDefaultRandomState) * work->head.swayRandomness + (1.0f - work->head.swayRandomness));
    particle->swayPhase = 0;
    particle->verticalSpeed = work->head.verticalSpeed * (dds3GetRandFloatA(effDefaultRandomState) * work->head.verticalSpeedRandomness + (1.0f - work->head.verticalSpeedRandomness));
    particle->lateralSpeed = work->head.lateralSpeed * (dds3GetRandFloatA(effDefaultRandomState) * work->head.lateralSpeedRandomness + (1.0f - work->head.lateralSpeedRandomness));
}

extern f32 aViewTarget[4];
extern void dds3BillboardDraw(billHandle_t *obj);

/* vu0 routine: camera-right normalization and position addition use the
 * native SDK pipeline. As in effEVECombParSmokeDraw, position W is untouched. */
void effEventUpdateBillboardParticles(EffEventBillSet *work) {
    f32 position[4], origin[4], lateral[4];
    EffEventBillParticle *particle;
    s32 repeat;
    u32 index, particleCount;
    s32 duration, fadeIn, fadeOut;
    u32 color;
    f32 acceleration, swayPhaseStep, swayAmplitudeStep;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lateral);
    particle = work->particles;
    particleCount = work->head.particleCount;
    acceleration = work->head.accelerationPercent / 100.0f + 1.0f;
    repeat = work->head.repeat;
    duration = work->head.duration;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    color = work->head.color;
    swayPhaseStep = work->head.swayPhaseStep;
    swayAmplitudeStep = work->head.swayAmplitudeStep;
    SDF_VU0_COPYVECTOR(origin, work->head.position);
    dds3BillboardSetMode(D_003BB144, work->head.drawMode);
    dds3BillboardSetMode(D_003BB148, 3);
    for (index = 0; index < particleCount; index++, particle++) {
        s32 age = particle->age;
        if (age == 0) {
            effEVECombParSmokePartInit(work, index);
        }
        if (age > 0 && age <= duration) {
            f32 swaySine, sway, alpha;
            f32 modulationX, scaleX, scaleY, phaseCosine;
            u32 drawColor;
            if (particle->motionDelay >= 0) {
                f32 lateralSpeed = particle->lateralSpeed;
                particle->position[0] += lateral[0] * lateralSpeed;
                particle->position[1] += lateral[1] * lateralSpeed;
                particle->position[2] += lateral[2] * lateralSpeed;
                swaySine = sdfSin(particle->swayPhase);
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
            drawColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            scaleY = particle->baseScale;
            phaseCosine = sdfCos(particle->scalePhaseX);
            modulationX = particle->scaleAmplitudeX * phaseCosine;
            phaseCosine = sdfCos(particle->scalePhaseY);
            scaleX = scaleY + modulationX;
            scaleY = scaleY + particle->scaleAmplitudeY * phaseCosine;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(position);
            effCopyVector(D_003BB144->position, position);
            dds3BillboardSetScale(D_003BB144, scaleX, scaleY);
            dds3BillboardSetRGBA(D_003BB144, drawColor);
            dds3BillboardSetRotate(D_003BB144, particle->rotation);
            dds3BillboardDraw(D_003BB144);
            effCopyVector(D_003BB148->position, position);
            dds3BillboardSetScale(D_003BB148, scaleX, scaleY);
            dds3BillboardSetRGBA(D_003BB148, drawColor);
            dds3BillboardSetRotate(D_003BB148, particle->rotation);
            dds3BillboardDraw(D_003BB148);
            particle->scalePhaseX += particle->scalePhaseStepX;
            particle->scalePhaseY += particle->scalePhaseStepY;
            particle->rotation += particle->angularSpeed;
        }
        if (duration == age && repeat) {
            particle->age = 0;
        } else {
            /* Native 00191468..84 reloads after callbacks and wraps the word. */
            particle->age = (s32)((u32)particle->age + 1U);
        }
    }
}

struct DmaPacketHeader;
extern void sdf3DMakeDirectRefPacket(struct DmaPacketHeader *packet, u32 sourceAddress, s32 payloadBytes);
extern void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix);
extern sdfGrOt_t D_003252A8;

extern sdfTexHandle_t *dds3BillboardGetTex(billHandle_t *effect);

/* DDS1 event-bill renderer, reconstructed from the native owner and its
 * current providers. The VIF position stream consumes XYZ only: native never
 * initializes the temporary position's W before its full-vector add/copy. */
void effEVECombParSmokeDraw(EffEventBillSet *work) {
    EffEventBillBatch primary, secondary;
    f32 matrix[4][4];
    f32 position[4], origin[4], lateral[4];
    sdfDrawTag_t *primaryList, *secondaryList;
    sdfTexHandle_t *primaryTexture, *secondaryTexture;
    EffEventBillParticle *particle;
    s32 repeat;
    u32 index, particleCount;
    s32 duration, fadeIn, fadeOut, colorStart, colorFrames;
    u32 color;
    f32 acceleration, swayPhaseStep, swayAmplitudeStep;
    struct DmaPacketHeader *texturePacket;
    const sdfTexPacket_t *textureBuffer;
    s32 textureBytes;
    u8 *geometryPacket;
    sdfDrawTag_t *stateList;
    sdfPrimTransPacket_t *statePacket;
    sdfGrOt_t *surface;
    primary.count = 0;
    secondary.count = 0;
    primaryList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(primaryList);
    secondaryList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(secondaryList);
    primaryTexture = dds3BillboardGetTex(D_003BB144);
    texturePacket = (struct DmaPacketHeader *)sdfGrAllocPacketBuff(0x20);
    textureBuffer = sdfTexGetPacket(primaryTexture);
    textureBytes = sdfTexGetPacketLength(primaryTexture);
    sdf3DMakeDirectRefPacket(texturePacket, (u32)textureBuffer, textureBytes);
    sdfAppendReferencePacket(primaryList, (u32)texturePacket);
    secondaryTexture = dds3BillboardGetTex(D_003BB148);
    texturePacket = (struct DmaPacketHeader *)sdfGrAllocPacketBuff(0x20);
    textureBuffer = sdfTexGetPacket(secondaryTexture);
    textureBytes = sdfTexGetPacketLength(secondaryTexture);
    sdf3DMakeDirectRefPacket(texturePacket, (u32)textureBuffer, textureBytes);
    sdfAppendReferencePacket(secondaryList, (u32)texturePacket);
    EE_MMI_UNIT_MATRIX(matrix);
    geometryPacket = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(geometryPacket, matrix[0]);
    sdfDrawTagLink(primaryList, (u32)geometryPacket);
    geometryPacket = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(geometryPacket, matrix[0]);
    sdfDrawTagLink(secondaryList, (u32)geometryPacket);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lateral);
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
    SDF_VU0_COPYVECTOR(origin, work->head.position);
    for (index = 0; index < particleCount; index++, particle++) {
        s32 age = particle->age;
        if (age == 0) {
            effEVECombParSmokePartInit(work, index);
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
                swaySine = sdfSin(particle->swayPhase);
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
                primaryColor = effColorBlend(0, color & 0xFF000000, alpha);
            } else if (age < colorStart + colorFrames) {
                if (colorFrames > 0) {
                    transition = (f32)(age - colorStart) / (f32)colorFrames;
                    if (1.0f < transition) transition = 1.0f;
                } else {
                    transition = 1.0f;
                }
                primaryColor = effColorBlend(color & 0xFF000000, color, transition);
                primaryColor = effColorBlend(primaryColor & 0xFFFFFF, primaryColor, alpha);
            } else {
                primaryColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            }
            secondaryColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            scaleY = particle->baseScale;
            phaseCosine = sdfCos(particle->scalePhaseX);
            modulationX = particle->scaleAmplitudeX * phaseCosine;
            phaseCosine = sdfCos(particle->scalePhaseY);
            scaleX = scaleY + modulationX;
            scaleY = scaleY + particle->scaleAmplitudeY * phaseCosine;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(position);
            {
                f32 cosine, sine, cornerX, cornerY;
                f32 center = 0.0f;
            slot = primary.count;
            SDF_VU0_COPYVECTOR(&primary.positions[slot], position);
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
            cosine = sdfCos(particle->rotation);
            sine = sdfSin(particle->rotation);
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
            SDF_VU0_COPYVECTOR(&secondary.positions[slot], position);
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
            cosine = sdfCos(particle->rotation);
            sine = sdfSin(particle->rotation);
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
                sdfDrawTagLink(primaryList, (u32)sdfBillBoardMakeN(primary.positions,
                    primary.colors, primary.uv, primary.corners[0], 15, 0));
                primary.count = 0;
            }
            if (secondary.count == 15) {
                sdfDrawTagLink(secondaryList, (u32)sdfBillBoardMakeN(secondary.positions,
                    secondary.colors, secondary.uv, secondary.corners[0], 15, 0));
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
        sdfDrawTagLink(primaryList, (u32)sdfBillBoardMakeN(primary.positions,
            primary.colors, primary.uv, primary.corners[0], primary.count, 0));
    }
    if (secondary.count > 0) {
        sdfDrawTagLink(secondaryList, (u32)sdfBillBoardMakeN(secondary.positions,
            secondary.colors, secondary.uv, secondary.corners[0], secondary.count, 0));
    }
    stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(stateList);
    statePacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    statePacket->header.dmaTag = 3;
    statePacket->header.vifCommands = 0x5000000310000000ULL;
    statePacket->header.gifTag = 0x1000000000008002ULL;
    statePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    if (work->flag) statePacket->registers.test.value = 0x51801;
    else statePacket->registers.test.value = 0x31801;
    statePacket->registers.test.registerId = SDF_GS_TEST_1;
    statePacket->registers.alpha.value = 0x44;
    statePacket->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(stateList, (u32)statePacket);
    D_003252A8.append(&D_003252A8, stateList);
    stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(stateList);
    statePacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    statePacket->header.dmaTag = 3;
    statePacket->header.vifCommands = 0x5000000310000000ULL;
    statePacket->header.gifTag = 0x1000000000008002ULL;
    statePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    if (work->flag) statePacket->registers.test.value = 0x51801;
    else statePacket->registers.test.value = 0x31801;
    statePacket->registers.test.registerId = SDF_GS_TEST_1;
    statePacket->registers.alpha.value = 0x42;
    statePacket->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(stateList, (u32)statePacket);
    surface = &sdfGrOtTable[60];
    surface->append(surface, stateList);
    (surface - 1)->append(surface - 1, primaryList);
    surface->append(surface, secondaryList);
}

/* Copy one full vector quadword through vf10, including its fourth component. */
void effEVECombParSmokeSetPosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

/* Set the native byte flag without normalizing it to a boolean. */
void effEVECombParSmokeSetZtest(EffEventBillSet *work, u8 flag) {
    work->flag = flag;
}

/* Copy the serialized emitter parameters without changing their values. */
void effEVECombParSmokeGetParams(const EffEventBillParams *source, EffEventBillParams *destination) {
    *destination = *source;
}

/* Copy parameters and clamp only start-delay spread and fade divisors;
 * motionDelaySpread remains unvalidated on this native path. */
void effEVECombParSmokeSetParams(EffEventBillParams *destination, const EffEventBillParams *source) {
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
void effEVECombParSmokeCreate2(void) {
    effEVECombParSmokeCreate(D_003563F0);
}


EffChanWork *effAPPoint4Create(const EffChanHead *source, u16 kind, void *params) {
    const EffChanHead *head = source;
    u32 recordCount = head->count;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(recordCount * sizeof(EffChanRecord) + sizeof(EffChanWork));
    EffChanWork *work = (EffChanWork *)sdfMemLock(allocation);
    EffChanRecord *record = (EffChanRecord *)(work + 1);
    effPartHandle_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = *head;
    work->buffer = allocation;
    work->records = record;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        delayModulus = work->head.spread;
        parameterTemplate = dds3EffectPartCreate(kind, params);
        record->param = parameterTemplate;
        record->delay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayModulus);
        record++;
        for (recordIndex = 1; recordIndex < recordCount; recordIndex++) {
            record->param = dds3EffectPartCopy(parameterTemplate);
            record->delay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayModulus);
            record++;
        }
    }
    return work;
}

/* Forward block 0, block 1's word-2 kind (narrowed to u16), and block 1 unchanged. */
EffChanWork *effAPPoint4CreatePCF(void *data) {
    void *block0 = dds3EffectPCPDataGetData(data, 0);
    void *block1 = dds3EffectPCPDataGetData(data, 1);

    return effAPPoint4Create(block0, dds3EffectPCPDataGetType(data, 1), block1);
}

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB068);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB06C);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_BlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_PartialMultiBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_DistortionBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB073);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_FilterShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_PartialMultiFilterShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_RippleBlurShow);

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
