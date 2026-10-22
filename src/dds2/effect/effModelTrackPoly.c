#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_math.h"
#include "eff_track_poly_list.h"
#include "mdl.h"
#include "sdf.h"
#include "sdf_chunk.h"
#include "pcp_vu0.h"
#include "eff_track_poly_data.h"
#include "eff_packet_params.h"

void pMeshDelete(dds3HermitianTrackPoly_tag *data);

/* Release both owned resources before freeing this track. */
void effTrackPolyRelease(EffTrackPolyWork *work) {
    pMeshDelete(work->data);
    sdfChipFreeDirect(work);
}

void dds3HermitianTrackPolyReset(dds3HermitianTrackPoly_tag *data);

void effTrackPolyReset(EffTrackPolyWork *work) {
    work->updateCount = 0;
    dds3HermitianTrackPolyReset(work->data);
}

void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *data, const u128 *src);

void Eff_ModelTrackPolyGetMarkPosition(EffTrackPolyWork *work) {
    dds3ModelHandle_t *model = work->params.model;
    u128 points[2];

    sdfModelGetMarkPos(model->inner, work->params.idA);
    SDF_VU0_STOREVECTOR(points);
    sdfModelGetMarkPos(model->inner, work->params.idB);
    SDF_VU0_STOREVECTOR(&points[1]);
    dds3HermitianTrackPolySetVertex(work->data, points);
}

void Eff_ModelTrackPolyVertexSet(EffTrackPolyWork *work, const u128 *endpoints) {
    dds3HermitianTrackPolySetVertex(work->data, endpoints);
}

void effTrackPolySetColor(EffTrackPolyWork *work, u32 color) {
    work->data->color = color;
}

void dds3HermitianTrackPolyDraw(dds3HermitianTrackPoly_tag *data);

void Eff_ModelTrackPolyDrawOnly(EffTrackPolyWork *work) {
    dds3HermitianTrackPolyDraw(work->data);
}

/* Sample endpoints between startFrame and endFrame (inclusive), fade after
 * endFrame, and reset history before startFrame. */
void effTrackPolyUpdate(EffTrackPolyWork *work) {
    dds3ModelHandle_t *model = work->params.model;
    f32 currentFrame = model->first->currentFrame;

    if (work->params.startFrame <= currentFrame) {
        if (currentFrame <= work->params.endFrame) {
            if (work->updateCount % work->params.sampleInterval == 0) {
                Eff_ModelTrackPolyGetMarkPosition(work);
            }
            dds3HermitianTrackPolyDraw(work->data);
        } else {
            dds3HermitianTrackPoly_tag *data = work->data;

            if (data->color & 0xFF000000) {
                data->color += 0xE0000000;
            }
            dds3HermitianTrackPolyDraw(data);
        }
        work->updateCount++;
    } else {
        effTrackPolyReset(work);
    }
}

/* Clone count tracks from one parameter block, each with its own data. */
effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(EffTrackPolyParams *params, u32 count) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(count * sizeof(EffTrackPolyWork *) + sizeof(effMTPolyListHandle_t));
    effMTPolyListHandle_t *list = (effMTPolyListHandle_t *)sdfMemLock(handle);
    u32 workIndex;

    list->handle = handle;
    list->items = (EffTrackPolyWork **)(list + 1);
    list->count = count;
    for (workIndex = 0; workIndex < count; workIndex++) {
        list->items[workIndex] = effTrackPolyCreateWork(params);
    }
    return list;
}

void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list) {
    u32 count = list->count;
    u32 workIndex = 0;

    while (workIndex < count) {
        effTrackPolyRelease(list->items[workIndex]);
        workIndex++;
    }
    sdfMemFreeDirect(list->handle);
}

void Eff_ModelTrackPolyListVertexSet(effMTPolyListHandle_t *list, s32 index, const u128 *endpoints) {
    Eff_ModelTrackPolyVertexSet(list->items[index], endpoints);
}

void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *list, s32 index) {
    effTrackPolyReset(list->items[index]);
}

void effTrackPolySetIndexedColor(effMTPolyListHandle_t *list, s32 index, u32 color) {
    effTrackPolySetColor(list->items[index], color);
}

void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *list) {
    u32 count = list->count;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDrawOnly(list->items[workIndex]);
        workIndex++;
    }
}

/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void Eff_HermitianSpline(f32 (*p)[4], f32 t)
{
    f32 tan[2][4];
    f32 half[4];
    f32 h[4][4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    VEC3_SPLAT(half, 0.5f);
    SDF_VU0_LOADVECTOR(p[1]);
    SDF_VU0_LOADVECTOR_B(p[0]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(p[2]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(tan[0]);
    SDF_VU0_LOADVECTOR(p[2]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(p[3]);
    SDF_VU0_LOADVECTOR_B(p[2]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(tan[1]);
    VEC3_SPLAT(h[0], 2.0f * t3 - 3.0f * t2 + 1.0f);
    VEC3_SPLAT(h[1], t3 - 2.0f * t2 + t);
    VEC3_SPLAT(h[2], t3 - t2);
    VEC3_SPLAT(h[3], -2.0f * t3 + 3.0f * t2);
    SDF_VU0_LOADVECTOR(h[0]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(h[1]);
    SDF_VU0_LOADVECTOR_B(tan[0]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_C_A();
    SDF_VU0_LOADVECTOR(h[2]);
    SDF_VU0_LOADVECTOR_B(tan[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_C_A();
    SDF_VU0_LOADVECTOR(h[3]);
    SDF_VU0_LOADVECTOR_B(p[2]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
}
extern EffPacketParams effTrackPolyDrawParams;
extern const u32 effTrackPolyTriangleIndices[];


dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 historyLength, s32 steps) {
    s32 count = historyLength * steps * 2 + 4;
    s32 bytes = count * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *handle = sdfMemAllocDirect(bytes + sizeof(dds3HermitianTrackPoly_tag));
    u8 *cursor = (u8 *)sdfMemLock(handle);
    dds3HermitianTrackPoly_tag *data = (dds3HermitianTrackPoly_tag *)(cursor + bytes);

    data->points = (u128 *)cursor;
    cursor += count * sizeof(u128);
    data->kind = 2;
    data->color = 0x80808080;
    data->count = count;
    data->activePointCount = 0;
    data->position = 2;
    data->step = steps;
    data->resourceHandle = handle;
    data->colors = (u32 *)cursor;
    data->nodeHandle = sdfMatNew();
    sdfMatSetConstant((struct sdfMat_t *)data->nodeHandle, 1.0f);
    memset(&effTrackPolyDrawParams, 0, sizeof(effTrackPolyDrawParams));
    effTrackPolyDrawParams.primitive = 0x4000;
    effTrackPolyDrawParams.parameters = effTrackPolyTriangleIndices;
    return data;
}

void pMeshDelete(dds3HermitianTrackPoly_tag *data) {
    sdfMatFreeDelayed((sdfMat_t *)data->nodeHandle);
    sdfMemFreeDirect(data->resourceHandle);
}

void dds3HermitianTrackPolyReset(dds3HermitianTrackPoly_tag *data) {
    data->position = 2;
    data->color = 0x80808080;
    data->activePointCount = 0;
}

/* Select the draw target on the data created by the track constructor. */
void dds3HermitianTrackPolySetBlend(dds3HermitianTrackPoly_tag *data, u32 kind) {
    data->kind = kind;
}

void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *data, u32 *gradientColors) {
    f32 t = 0.0f;
    u32 count = data->count >> 1;
    f32 step = 1.0f / count;
    u32 i = 0;
    u32 *colors = data->colors;

    if (count != 0) {
        do {
            colors[0] = effColorBlend(gradientColors[2], gradientColors[0], t);
            colors[1] = effColorBlend(gradientColors[3], gradientColors[1], t);
            t += step;
            colors += 2;
            i++;
        } while (i < count);
    }
}

/* Copy the pair of track points at the wrapped position into the two outputs. */
void dds3HermitianTrackPolyGetVertex(dds3HermitianTrackPoly_tag *data, u128 *dst0, u128 *dst1, s32 amount) {
    s32 pos = data->position - (data->step * (amount - 1) + amount) * 2;
    u128 *p;

    if (pos < 2) {
        pos = (pos + data->count) - 2;
    }
    p = &data->points[pos];
    SDF_VU0_COPYVECTOR(dst0, p);
    p++;
    SDF_VU0_COPYVECTOR(dst1, p);
}

/* Advance around the track's ring, wrapping below the reserved first pair. */
void dds3HermitianTrackPolySetCurrent(dds3HermitianTrackPoly_tag *data, s32 amount) {
    s32 position;

    position = data->position + ((data->step - 1) * (amount - 1) + amount) * -2
    ;
    if (position < 2) {
        position = (position + data->count) - 2;
    }
    data->position = position;
}

/* Append a point pair at the ring position, wrapping back to the reserved pair. */
void dds3HermitianTrackPolySetVertexSub(dds3HermitianTrackPoly_tag *data, const u128 *src) {
    s32 position = data->position;
    u128 *points = data->points;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], src);
    SDF_VU0_COPYVECTOR(&points[position + 1], src + 1);
    position += 2;
    count = data->count;
    data->position = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], src);
        SDF_VU0_COPYVECTOR(&points[1], src + 1);
        data->position = 2;
    }
    if (data->activePointCount < count - 2) {
        data->activePointCount += 2;
    }
}

/* Resample the newest history span at a fixed interval, smoothing established
 * tracks with Catmull-Rom interpolation and linearly extending short tracks. */
void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *data, const u128 *src) {
    f32 firstHistory[4][4];
    f32 secondHistory[4][4];
    f32 firstBasePoint[4];
    f32 secondBasePoint[4];
    f32 firstPointDelta[4];
    f32 secondPointDelta[4];
    /* A sample's pair, first then second: dds3HermitianTrackPolySetVertexSub reads
       them as two consecutive quadwords, as retail's frame keeps them. */
    f32 samplePair[2][4];
    s32 activePointCount = data->activePointCount;
    s32 sampleCount = data->step;
    f32 sampleWeightStep;
    f32 sampleWeight;
    s32 sampleIndex;
    u128 *secondSampleOutput;

    if (activePointCount == 0) {
        dds3HermitianTrackPolySetVertexSub(data, src);
        return;
    }

    if (activePointCount >= sampleCount * 6) {
        s32 savedRingPosition = data->position;
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[0], (u128 *)secondHistory[0], 3);
        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[1], (u128 *)secondHistory[1], 2);
        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[2], (u128 *)secondHistory[2], 1);
        SDF_VU0_COPYVECTOR(firstHistory[3], src);
        SDF_VU0_COPYVECTOR(secondHistory[3], src + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        dds3HermitianTrackPolySetCurrent(data, 2);
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)samplePair[0];
            secondSampleOutput = (u128 *)samplePair[1];
            firstSampleOutputBase = firstSampleOutput;
            sampleIndex = sampleCount;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                sampleIndex--;
                dds3HermitianTrackPolySetVertexSub(data, firstSampleOutput);
            } while (sampleIndex != 0);
        }
        data->activePointCount = activePointCount;
        data->position = savedRingPosition;
    }

    if (activePointCount >= sampleCount * 4) {
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[0], (u128 *)secondHistory[0], 2);
        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[1], (u128 *)secondHistory[1], 1);
        SDF_VU0_COPYVECTOR(firstHistory[2], src);
        SDF_VU0_COPYVECTOR(secondHistory[2], src + 1);
        SDF_VU0_COPYVECTOR(firstHistory[3], src);
        SDF_VU0_COPYVECTOR(secondHistory[3], src + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        sampleIndex = 0;
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)samplePair[0];
            secondSampleOutput = (u128 *)samplePair[1];
            firstSampleOutputBase = firstSampleOutput;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                dds3HermitianTrackPolySetVertexSub(data, firstSampleOutput);
                sampleIndex++;
            } while (sampleIndex < sampleCount);
        }
    } else {
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstBasePoint, (u128 *)secondBasePoint, 1);
        SDF_VU0_LOADVECTOR(src);
        SDF_VU0_LOADVECTOR_B(firstBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(firstPointDelta);
        SDF_VU0_LOADVECTOR(src + 1);
        SDF_VU0_LOADVECTOR_B(secondBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(secondPointDelta);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        if (sampleCount > 0) {
            sampleIndex = sampleCount;
            firstSampleOutput = (u128 *)samplePair[0];
            do {
                sampleWeight += sampleWeightStep;
                sampleIndex--;
                samplePair[0][0] = firstBasePoint[0] + firstPointDelta[0] * sampleWeight;
                samplePair[0][1] = firstBasePoint[1] + firstPointDelta[1] * sampleWeight;
                samplePair[0][2] = firstBasePoint[2] + firstPointDelta[2] * sampleWeight;
                samplePair[1][0] = secondBasePoint[0] + secondPointDelta[0] * sampleWeight;
                samplePair[1][1] = secondBasePoint[1] + secondPointDelta[1] * sampleWeight;
                samplePair[1][2] = secondBasePoint[2] + secondPointDelta[2] * sampleWeight;
                dds3HermitianTrackPolySetVertexSub(data, firstSampleOutput);
            } while (sampleIndex != 0);
        }
    }
}



extern SdfPoolNode *effTrackPolyDrawSurfaces[];
extern SdfPoolNode D_00380248;
#include "sdf_packet_alloc.h"
extern void *effPkt3DPrimMakePoly(EffPacketParams *);

/* Walk at most two ring runs. Each full strip consumes 16 vertices, with
 * two additional vertices overlapping the next strip (18 inputs total). */
void dds3HermitianTrackPolyDraw(dds3HermitianTrackPoly_tag *data) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 runStart[4];
    s32 runPointCount[2];
    EffPacketParams *draw;
    s32 runIndex;
    s32 pointsRemaining;
    s32 wrappedStartOffset;
    s32 activePointCount;
    sdfDrawTag_t *list2;
    u64 *packet;

    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, data->nodeHandle, 0);
    activePointCount = data->activePointCount;
    runStart[0] = data->position - activePointCount;
    if (runStart[0] < 2) {
        wrappedStartOffset = runStart[0] - 2;
        runStart[1] = 0;
        runStart[0] = wrappedStartOffset + data->count;
        runPointCount[0] = -wrappedStartOffset;
        runPointCount[1] = activePointCount - runPointCount[0] + 2;
    } else {
        runPointCount[0] = activePointCount;
        runPointCount[1] = 0;
    }
    effTrackPolyDrawParams.colors = data->colors;
    for (runIndex = 0; runIndex < 2; runIndex++) {
        draw = &effTrackPolyDrawParams;
        draw->positions = &data->points[runStart[runIndex]];
        draw->color = data->color;
        draw->parameterCount = 0x10;
        draw->vertexCount = 0x12;
        pointsRemaining = runPointCount[runIndex];
        while (pointsRemaining >= 0x12) {
            pointsRemaining -= 0x10;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&effTrackPolyDrawParams));
            effTrackPolyDrawParams.positions += 0x10;
            effTrackPolyDrawParams.colors += 0x10;
        }
        if (pointsRemaining >= 4) {
            draw->vertexCount = pointsRemaining;
            draw->parameterCount = pointsRemaining - 2;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(draw));
            draw->colors += pointsRemaining;
        }
    }
    if (data->kind < 4) {
        effTrackPolyDrawSurfaces[data->kind]->append(effTrackPolyDrawSurfaces[data->kind], list);
    } else {
        list2 = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list2);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 6;
        packet[5] = 0x42;
        sdfDrawTagLink(list2, (u32)packet);
        D_00380248.append(&D_00380248, list2);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
        D_00380248.append(&D_00380248, list);
    }
}
