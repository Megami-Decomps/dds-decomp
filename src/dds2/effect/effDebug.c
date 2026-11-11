#include "common.h"
#include "sdfDevCons.h"
#include "libsdf.h"
#include "ps2asm.h"

#include "effect.h"

/* DDS's names for the same objects. */

/* Screen pixel pair in the debug overlay's own 1/16 pixel grid. */
typedef struct
{
    s32 x;
    s32 y;
} effDebugPoint_t;

extern sdfGrOt_t sdfGrOtTable[85];

extern void *sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(void *a);
extern void sdfDrawTagLink(void *aTag, void *aPacket);
extern void effScreenOffset(void);
extern void *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);
extern void *dds3PrimLine(s32 aX0, s32 aY0, s32 aZ0, u32 aColor0, s32 aX1, s32 aY1, s32 aZ1, u32 aColor1, s32 aMode);


sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
void *sdfMemLock(sdfMemHandle_t *aHandle);
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
/* SDK matrix-bank helpers used by the wire sphere's second pass. */
typedef struct
{
    f32 rows[4][4];
} effDebugMatrix_t;

typedef struct
{
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

extern sdfMat_t *effDebugSelectCursorMat;
extern f32 effDebugSelectCursorAngle;
extern f32 effDebugTriPositions[4][4];
extern u32 effDebugTriColors[4];
f32 sdfCos(f32 aRadius);
f32 sdfSin(f32 aRadius);
void SetRotateMatrixX(f32 aAngle);
void SetRotateMatrixY(f32 aAngle);
void MultMatrixNoArg(void);
void *effDebugDrawWireModel(const sdfVector4_t *positions, u32 *colors, u32 count, u32 mode);
sdfMat_t *sdfMatNew(void);
void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);
void sdf3DPrimLinkTm(void *aTag, void *aAllocator);
void sdf3DPrimLinkMat(void *aTag, sdfMat_t *aMat, void *aAllocator);
void *memset(void *a, s32 b, size_t c);
void *effPkt3DPrimMakePoly(effDebugPolyParams_t *aParm);

/* Allocate contiguous slots followed by their count and allocation handle. */
effArrHdr_t *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * 0x60;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(slotBytes + 0xC);
    effBezierCurve_t *slot = (effBezierCurve_t *)sdfMemLock(allocation);
    effArrHdr_t *table = (effArrHdr_t *)((u8 *)slot + slotBytes);
    u32 index = 0;
    table->allocation = allocation;
    table->slots = slot;
    table->count = count; /* The shared array header's slot count. */
    if (count != 0) {
        do {
            index++;
            slot->pointIndex = 0;
            slot->t = 0;
            slot->parameterStep = 0.05f;
            slot++;
        } while (index < count);
    }
    return table;
}

void effReleaseSlotArrayAllocation(effArrHdr_t *header) {
    sdfMemFreeDirect(header->allocation);
}

/* Point-index sentinel 7 stops evaluation; otherwise sample before clamping t and advancing three controls. */
s32 effStepActiveBezierSlot(effArrHdr_t *table, s32 index, f32 *out) {
    effBezierCurve_t *slot = &((effBezierCurve_t *)table->slots)[index];
    u32 pointIndex = slot->pointIndex;
    sdfVector4_t weights;
    f32 t;
    f32 u;
    effBezierPoint_t *controlPoints;

    if (pointIndex == 7) {
        return 0;
    }
    t = slot->t;
    u = 1.0f - t;
    controlPoints = &slot->controlPoints[pointIndex];
    weights.x = u * u * u;
    weights.y = t * (u * u) * 3.0f;
    weights.z = t * t * u * 3.0f;
    weights.w = t * t * t;
    out[0] = controlPoints[0].x * weights.x + controlPoints[1].x * weights.y + controlPoints[2].x * weights.z + controlPoints[3].x * weights.w;
    out[1] = controlPoints[0].y * weights.x + controlPoints[1].y * weights.y + controlPoints[2].y * weights.z + controlPoints[3].y * weights.w;
    out[2] = controlPoints[0].z * weights.x + controlPoints[1].z * weights.y + controlPoints[2].z * weights.z + controlPoints[3].z * weights.w;
    out[3] = 1.0f;
    t += slot->parameterStep;
    if (t > 1.0f) {
        t = 1.0f;
        pointIndex += 3;
    }
    slot->t = t;
    slot->pointIndex = pointIndex;
    return 1;
}

/* Carry excess t across the first segment; the second finishes after sampling its entry t. */
s32 effPCPBezierEval(effBezierCurve_t *slot, f32 *out) {
    sdfVector4_t weights;
    u32 pointIndex = slot->pointIndex;
    f32 t = slot->t;
    effBezierPoint_t *controlPoints = &slot->controlPoints[pointIndex];
    f32 u = 1.0f - t;

    weights.x = u * u * u;
    weights.y = t * (u * u) * 3.0f;
    weights.z = t * t * u * 3.0f;
    weights.w = t * t * t;
    out[0] = controlPoints[0].x * weights.x + controlPoints[1].x * weights.y + controlPoints[2].x * weights.z + controlPoints[3].x * weights.w;
    out[1] = controlPoints[0].y * weights.x + controlPoints[1].y * weights.y + controlPoints[2].y * weights.z + controlPoints[3].y * weights.w;
    out[2] = controlPoints[0].z * weights.x + controlPoints[1].z * weights.y + controlPoints[2].z * weights.z + controlPoints[3].z * weights.w;
    out[3] = 1.0f;
    t += slot->parameterStep;
    if (t > 1.0f) {
        if (pointIndex < 3) {
            t -= 1.0f;
            pointIndex += 3;
        } else {
            slot->t = 1.0f;
            return 0;
        }
    }
    slot->pointIndex = pointIndex;
    slot->t = t;
    return 1;
}

/* Evaluate without changing progress; write homogeneous W = 1. */
void effEvaluateSlotBezierPosition(effBezierCurve_t *slot, f32 *out) {
    effBezierPoint_t *controlPoints = &slot->controlPoints[slot->pointIndex];
    f32 t = slot->t;
    f32 u = 1.0f - t;
    sdfVector4_t w;

    w.x = u * u * u;
    w.y = t * (u * u) * 3.0f;
    w.z = t * t * u * 3.0f;
    w.w = t * t * t;

    out[0] = controlPoints[0].x * w.x + controlPoints[1].x * w.y + controlPoints[2].x * w.z + controlPoints[3].x * w.w;
    out[1] = controlPoints[0].y * w.x + controlPoints[1].y * w.y + controlPoints[2].y * w.z + controlPoints[3].y * w.w;
    out[2] = controlPoints[0].z * w.x + controlPoints[1].z * w.y + controlPoints[2].z * w.z + controlPoints[3].z * w.w;
    out[3] = 1.0f;
}

/* Reset progress and its default step, retaining the control points. */
void effInitSlotTail(effArrHdr_t *table, s32 index) {
    effBezierCurve_t *slot = &((effBezierCurve_t *)table->slots)[index];

    slot->parameterStep = 0.05f;
    slot->pointIndex = slot->t = 0;
}

effBezierCurve_t *effGetSlotAt(effArrHdr_t *table, s32 index) {
    return &((effBezierCurve_t *)table->slots)[index];
}

/* vu0 routine: project a world point and frame it */
void effDebugPointDraw(sdfVector4_t *pPos)
{
    sdfVector4_t screen;
    effDebugPoint_t point;
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(pPos);
    effScreenOffset();
    SDF_VU0_STOREVECTOR_MEMOPS(screen);
    point.x = (s32)screen.x - 0x700;
    point.y = ((s32)screen.y << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimFrameRect(point.x * 16 + 0x7000, point.y * 8 + 0x7900,
                                          0xFF0000, 0x20, 0x10, 0x60008080, 0x60008080));
    ot = &sdfGrOtTable[96];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

/* vu0 routine: project a world point and frame it in a given mode */
void effDebugPointDraw2(sdfVector4_t *pPos, u32 color)
{
    sdfVector4_t screen;
    effDebugPoint_t point;
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(pPos);
    effScreenOffset();
    SDF_VU0_STOREVECTOR_MEMOPS(screen);
    point.x = (s32)screen.x - 0x700;
    point.y = ((s32)screen.y << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimFrameRect(point.x * 16 + 0x7000, point.y * 8 + 0x7900,
                                          0xFF0000, 0x20, 0x10, color, color));
    ot = &sdfGrOtTable[96];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

/* vu0 routine: project two world points and draw the line */
void effDebugLineDraw(sdfVector4_t *pPos0, sdfVector4_t *pPos1)
{
    sdfVector4_t screen[2];
    effDebugPoint_t point[2];
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(pPos0);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[0]);
    SDF_VU0_LOADVECTOR(pPos1);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[1]);
    point[0].x = (s32)screen[0].x - 0x700;
    point[0].y = ((s32)screen[0].y << 1) - 0xF20;
    point[1].x = (s32)screen[1].x - 0x700;
    point[1].y = ((s32)screen[1].y << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimLine(point[0].x * 16 + 0x7000, point[0].y * 8 + 0x7900,
                                     0xFF0000, 0x60008080,
                                     point[1].x * 16 + 0x7000, point[1].y * 8 + 0x7900,
                                     0xFF0000, 0x60008080, 0));
    ot = &sdfGrOtTable[96];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

/* vu0 routine: project two world points and draw the line in a given mode */
void effDebugLineDraw2(sdfVector4_t *from, sdfVector4_t *to, u32 mode)
{
    sdfVector4_t screen[2];
    effDebugPoint_t point[2];
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    SDF_VU0_LOADVECTOR(from);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[0]);
    SDF_VU0_LOADVECTOR(to);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&screen[1]);
    point[0].x = (s32)screen[0].x - 0x700;
    point[0].y = ((s32)screen[0].y << 1) - 0xF20;
    point[1].x = (s32)screen[1].x - 0x700;
    point[1].y = ((s32)screen[1].y << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimLine(point[0].x * 16 + 0x7000, point[0].y * 8 + 0x7900,
                                     0xFF0000, mode,
                                     point[1].x * 16 + 0x7000, point[1].y * 8 + 0x7900,
                                     0xFF0000, mode, 0));
    ot = &sdfGrOtTable[96];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

/* Submit formatted text at biased GS coordinates. */
void effDebugStringDraw(s32 x, s32 y, s32 commandIndex, const char *format) {
    sdfDrawTag_t *task = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *scene;

    sdfDrawTagInit(task);
    sdfDrawTagLink(task, (sdfDevPrintf((x << 4) + 0x7000, (y << 3) + 0x7900, 0xFF0000, commandIndex, format)));
    scene = &sdfGrOtTable[96];
    ((void (*)(sdfGrOt_t *, void *))scene->regFunc)(scene, task);
}

/* vu0 routine: build two perpendicular sets of wire rings. */
void effDebugWireframeSphereDraw(const f32 *center, u32 color, f32 radius) {
    f32 point[4] __attribute__((aligned(16)));
    f32 matrix[4][4] __attribute__((aligned(16)));
    sdfVector4_t vertices[40];
    u32 colors[40];
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    sdfVector4_t *out;
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

    SDF_VU0_STOREIDENTITY(matrix);
    for (pass = 0; pass < 2; pass++) {
        latitude = 0.0f;
        for (ring = 0; ring < 14; ring++) {
            latitude += 0.20943949f;
            out = vertices;
            longitude = 0.0f;
            ringY = radius * sdfCos(latitude);
            ringRadius = radius * sdfSin(latitude);
            SDF_VU0_LOADMATRIX(matrix);
            SDF_VU0_LOADVECTOR_B( center);
            point[0] = ringRadius;
            point[1] = ringY;
            point[2] = 0.0f;
            SDF_VU0_LOADVECTOR( point);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();

            for (segment = 0; segment < 20; segment++) {
                SDF_VU0_STOREVECTOR(out);
                out++;
                longitude += 0.31415924429893494f;
                point[0] = sdfCos(longitude) * ringRadius;
                point[1] = ringY;
                point[2] = sdfSin(longitude) * ringRadius;
                SDF_VU0_LOADVECTOR( point);
                SDF_VU0_VECTOR_W_SET_ZERO();
                SDF_VU0_APPLYMATRIX();
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(out);
                out++;
            }
            sdfDrawTagLink(list, effDebugDrawWireModel(vertices, colors, 40, 0x80));
        }
        SetRotateMatrixX(1.5707963f);
        SDF_VU0_LOADMATRIX_B_MEMOPS(matrix);
        MultMatrixNoArg();
        SDF_VU0_STOREMATRIX_NOMEM(matrix);
    }
    surface = &sdfGrOtTable[55];
    ((void (*)(sdfGrOt_t *, void *))surface->regFunc)(surface, list);
}

/* vu0 routine: build and submit the debug geometry. */
void effDebugSelectCursorDraw(f32 *position) {
    sdfMat_t *asset = effDebugSelectCursorMat;
    sdfDrawTag_t *list;
    effDebugPolyParams_t packet;
    sdfGrOt_t *surface;

    if (asset == NULL) {
        asset = sdfMatNew();
        effDebugSelectCursorMat = asset;
        sdfMatSetConstant(asset, 1.0f);
    }

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    SetRotateMatrixY(effDebugSelectCursorAngle);
    effDebugSelectCursorAngle += 0.10471974313259125f;
    SDF_VU0_LOADVECTOR( position);
    VU0_ADD_VF31_VF10_XYZ();
    sdf3DPrimLinkTm(list, NULL);
    memset(&packet, 0, 0x2C);
    packet.primitive = 0x4000;
    effDebugTriPositions[2][2] = 0.0f;
    packet.colors = effDebugTriColors;
    packet.color = 0x80808080;

    effDebugTriPositions[0][0] = 0.0f;
    effDebugTriPositions[0][1] = 0.0f;
    effDebugTriPositions[0][2] = 0.0f;
    effDebugTriPositions[1][0] = 15.0f;
    effDebugTriPositions[1][1] = -30.0f;
    effDebugTriPositions[1][2] = 0.0f;
    effDebugTriPositions[2][0] = -15.0f;
    effDebugTriPositions[2][1] = -30.0f;

    packet.parameterCount = 1;
    packet.vertexCount = 3;
    packet.positions = (u128 *)effDebugTriPositions;
    packet.texcoords = NULL;
    sdf3DPrimLinkMat(list, effDebugSelectCursorMat, NULL);
    sdfDrawTagLink(list, effPkt3DPrimMakePoly(&packet));
    surface = &sdfGrOtTable[39];
    ((void (*)(sdfGrOt_t *, void *))surface->regFunc)(surface, list);
}

/* vu0 routine: build and submit the debug geometry. */
void effDebugWireframeCylinderDraw(const f32 *center, u32 color, f32 radius, f32 height) {
    sdfVector4_t point; /* The line provider consumes xyz; preserve the untouched w lane. */
    sdfVector4_t vertices[40];
    u32 colors[40];
    sdfDrawTag_t *list;
    sdfVector4_t *out;
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
        point.x = radius;
        point.y = y;
        point.z = 0.0f;
        SDF_VU0_LOADVECTOR_B( center);
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        for (segment = 0; segment < 20; segment++) {
            SDF_VU0_STOREVECTOR(out);
            out++;
            angle += angleStep;
            point.x = sdfCos(angle) * radius;
            point.y = y;
            point.z = sdfSin(angle) * radius;
            SDF_VU0_LOADVECTOR( &point);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(out);
            out++;
        }
        sdfDrawTagLink(list, effDebugDrawWireModel(vertices, colors, 40, 0x80));
        y += heightStep;
    }

    out = vertices;
    SDF_VU0_LOADVECTOR_B( center);
    angle = 0.0f;
    for (segment = 0; segment < 20; segment++) {
        point.x = sdfCos(angle) * radius;
        point.y = -height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        point.x = sdfCos(angle) * radius;
        point.y = height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, effDebugDrawWireModel(vertices, colors, 40, 0x80));

    out = vertices;
    SDF_VU0_LOADVECTOR_B( center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point.x = sdfCos(angle) * radius;
        point.y = -height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point.x = sdfCos(oppositeAngle) * radius;
        point.y = -height * 0.5f;
        point.z = sdfSin(oppositeAngle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, effDebugDrawWireModel(vertices, colors, 20, 0x80));

    out = vertices;
    SDF_VU0_LOADVECTOR_B( center);
    angle = 0.0f;
    for (segment = 0; segment < 10; segment++) {
        point.x = sdfCos(angle) * radius;
        point.y = height * 0.5f;
        point.z = sdfSin(angle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        oppositeAngle = angle + 3.1415926f;
        point.x = sdfCos(oppositeAngle) * radius;
        point.y = height * 0.5f;
        point.z = sdfSin(oppositeAngle) * radius;
        SDF_VU0_LOADVECTOR( &point);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(out);
        out++;
        angle += angleStep;
    }
    sdfDrawTagLink(list, effDebugDrawWireModel(vertices, colors, 20, 0x80));
    surface = &sdfGrOtTable[55];
    ((void (*)(sdfGrOt_t *, void *))surface->regFunc)(surface, list);
}

/* Submit the filled rectangle and border using native horizontal/vertical coordinate scales. */
void effDebugWindowDraw(s32 x, s32 y, s32 width, s32 height, s32 fillColor, s32 borderColor) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *scene;

    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (dds3PrimFrameRect(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, width * 0x10, height * 8, fillColor, borderColor)));
    scene = &sdfGrOtTable[96];
    ((void (*)(sdfGrOt_t *, void *))scene->regFunc)(scene, list);
}

INCLUDE_SDATA(const s32, "effect/effDebug", effDebugSelectCursorMat);

INCLUDE_SDATA(const s32, "effect/effDebug", effDebugSelectCursorAngle);

