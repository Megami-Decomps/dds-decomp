#include "common.h"
#include "pcp_vu0.h"

/* Polygon-track data: its ring position wraps against the entry count. */
typedef struct EffTrackPolyData {
    u32 kind;           /* 0x00: draw-target selector */
    u32 color;          /* 0x04 */
    s32 count;          /* 0x08: vertex slots, including the reserved first pair */
    s32 activePointCount; /* 0x0C */
    s32 position;       /* 0x10 */
    s32 step;           /* 0x14 */
    u128 *points;      /* 0x18 */
    u32 *colors;        /* 0x1C: per-vertex gradient table after the points */
    void *nodeHandle;   /* 0x20: owned draw asset */
    u32 resourceHandle; /* 0x24: allocation containing points/colors/data */
} EffTrackPolyData; /* 0x28 */
/* Draw-state record read by func_0015FE20 (same layout as ParDrawState in code_0015A758). */
typedef struct EffTrackPolyDraw {
    u16 width;      /* 0x00 */
    u16 height;     /* 0x02 */
    u16 flags;
    u8 pad06[2];
    u32 color;      /* 0x08 */
    void *unk0C;
    u128 *points;   /* 0x10: the same vertex records owned by track data */
    u8 pad14[0xC];
    u32 *colors;   /* 0x20: gradient color for each input vertex */
    u8 pad24[8];
} EffTrackPolyDraw; /* 0x2C */

/* Model handle the track's point queries resolve against. */
typedef struct {
    u8 pad00[0x1C];
    f32 value; /* 0x1C */
} EffTrackPolyModelState;

typedef struct {
    u8 pad00[0x18]; /* 0x00 */
    void *param;       /* 0x18: map-record position-query source */
    EffTrackPolyModelState *state; /* 0x1C */
} EffTrackPolyModel;

/* The constructor copies these 13 words before creating owned track data.
 * The model pointer is one parameter, not the constructor's entire input.
 * unk0C/unk10 bound the model value tested by the update routine. */
typedef struct {
    EffTrackPolyModel *model;
    s32 idA;
    s32 idB;
    f32 unk0C;
    f32 unk10;
    u32 sampleInterval; /* 0x14: updateCount modulus for endpoint sampling */
    s32 historyLength;  /* 0x18: multiplied by the constructor's step count */
    u32 unk1C;          /* 0x1C: constructor writes its fixed step count here */
    u32 kind;
    u32 gradientColors[4];
} EffTrackPolyParams; /* 0x34 */

typedef struct {
    EffTrackPolyParams params;
    u32 updateCount;    /* 0x34: increments each active update; gates sampling */
    EffTrackPolyData *data; /* 0x38 */
} EffTrackPolyWork; /* 0x3C */


/* Release both owned resources before freeing this track. */
void effTrackPolyRelease(EffTrackPolyWork *work) {
    effTrackPolyFreeData(work->data);
    sdfReleaseChipBlock(work);
}

void effTrackPolyReset(EffTrackPolyWork *work) {
    work->updateCount = 0;
    effTrackPolyInitData(work->data);
}

extern s32 sdfLoadMapRecordPositionVector(void *param, s32 id);
void func_00188A78(EffTrackPolyData *data, u128 *src);

void effSampleTrackPolyEndpoints(EffTrackPolyWork *work) {
    EffTrackPolyModel *model = work->params.model;
    u128 points[2];

    sdfLoadMapRecordPositionVector(model->param, work->params.idA);
    VU0_STORE_VF(vf10, points);
    sdfLoadMapRecordPositionVector(model->param, work->params.idB);
    VU0_STORE_VF(vf10, &points[1]);
    func_00188A78(work->data, points);
}

void effTrackPolyPushWorkEndpoints(EffTrackPolyWork *work, void *data) {
    func_00188A78(work->data, data);
}

void effTrackPolySetColor(EffTrackPolyWork *work, u32 color) {
    work->data->color = color;
}

void effTrackPolyDrawWork(EffTrackPolyWork *work) {
    effTrackPolyDrawStrips(work->data);
}

/* Per-frame update: while the model's value lies inside [unk0C, unk10] the endpoints are resampled every sampleInterval updates and the strips drawn; above the range the strips fade out, below it the track restarts. */
void effTrackPolyUpdate(EffTrackPolyWork *work) {
    EffTrackPolyModel *model = work->params.model;
    f32 value = model->state->value;

    if (work->params.unk0C <= value) {
        if (value <= work->params.unk10) {
            if (work->updateCount % work->params.sampleInterval == 0) {
                effSampleTrackPolyEndpoints(work);
            }
            effTrackPolyDrawStrips(work->data);
        } else {
            EffTrackPolyData *data = work->data;

            if (data->color & 0xFF000000) {
                data->color += 0xE0000000;
            }
            effTrackPolyDrawStrips(data);
        }
        work->updateCount++;
    } else {
        effTrackPolyReset(work);
    }
}

typedef struct EffTrackPolyList {
    EffTrackPolyWork **items; /* 0x00 */
    u32 count;                /* 0x04 */
    u32 handle;               /* 0x08 */
} EffTrackPolyList;

extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern EffTrackPolyWork *effTrackPolyCreateWork(EffTrackPolyParams *params);

/* Clone count tracks from one parameter block, each with its own data. */
EffTrackPolyList *effTrackPolyCreateModelWorkList(EffTrackPolyParams *params, u32 count) {
    u32 handle = sdfAllocGeneralBlock(count * sizeof(EffTrackPolyWork *) + sizeof(EffTrackPolyList));
    EffTrackPolyList *list = (EffTrackPolyList *)sdfResourceRetainAddress(handle);
    u32 i;

    list->handle = handle;
    list->items = (EffTrackPolyWork **)(list + 1);
    list->count = count;
    for (i = 0; i < count; i++) {
        list->items[i] = effTrackPolyCreateWork(params);
    }
    return list;
}

void effTrackPolyDestroyModelWorkList(EffTrackPolyList *list) {
    u32 count = list->count;
    u32 i = 0;

    while (i < count) {
        effTrackPolyRelease(list->items[i]);
        i++;
    }
    sdfReleaseResourceAllocation(list->handle);
}

void effTrackPolyPushIndexedWorkEndpoints(EffTrackPolyList *list, s32 index, void *data) {
    effTrackPolyPushWorkEndpoints(list->items[index], data);
}

void effTrackPolyResetIndexedWork(EffTrackPolyList *list, s32 index) {
    effTrackPolyReset(list->items[index]);
}

void effTrackPolySetIndexedColor(EffTrackPolyList *list, s32 index, u32 color) {
    effTrackPolySetColor(list->items[index], color);
}

void effTrackPolyDrawModelWorkList(EffTrackPolyList *list) {
    u32 count = list->count;
    u32 i = 0;

    while (i < count) {
        effTrackPolyDrawWork(list->items[i]);
        i++;
    }
}

/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void effTrackPolyInterpolateCatmullRomPoint(f32 (*p)[4], f32 t)
{
    f32 tan[2][4];
    f32 half[4];
    f32 h[4][4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    VEC3_SPLAT(half, 0.5f);
    VU0_LOAD_VF(vf10, p[1]);
    VU0_LOAD_VF(vf11, p[0]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, p[2]);
    VU0_LOAD_VF(vf11, p[1]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_LOAD_VF(vf11, half);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, tan[0]);
    VU0_LOAD_VF(vf10, p[2]);
    VU0_LOAD_VF(vf11, p[1]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, p[3]);
    VU0_LOAD_VF(vf11, p[2]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_LOAD_VF(vf11, half);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, tan[1]);
    VEC3_SPLAT(h[0], 2.0f * t3 - 3.0f * t2 + 1.0f);
    VEC3_SPLAT(h[1], t3 - 2.0f * t2 + t);
    VEC3_SPLAT(h[2], t3 - t2);
    VEC3_SPLAT(h[3], -2.0f * t3 + 3.0f * t2);
    VU0_LOAD_VF(vf10, h[0]);
    VU0_LOAD_VF(vf11, p[1]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, h[1]);
    VU0_LOAD_VF(vf11, tan[0]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf12, vf12, vf10);
    VU0_LOAD_VF(vf10, h[2]);
    VU0_LOAD_VF(vf11, tan[1]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf12, vf12, vf10);
    VU0_LOAD_VF(vf10, h[3]);
    VU0_LOAD_VF(vf11, p[2]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
}
extern EffTrackPolyDraw D_003D6640;
extern u8 D_003556D0[];
extern void *sdfCreateAssetWithDrawEntries(void);
extern void func_002DA420(void *asset, f32 scale);

EffTrackPolyData *effTrackPolyAllocateHistoryData(s32 historyLength, s32 steps) {
    s32 count = historyLength * steps * 2 + 4;
    s32 bytes = count * (sizeof(u128) + sizeof(u32));
    u32 handle = sdfAllocGeneralBlock(bytes + sizeof(EffTrackPolyData));
    u8 *cursor = sdfResourceRetainAddress(handle);
    EffTrackPolyData *data = (EffTrackPolyData *)(cursor + bytes);

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
    data->nodeHandle = sdfCreateAssetWithDrawEntries();
    func_002DA420(data->nodeHandle, 1.0f);
    memset(&D_003D6640, 0, sizeof(D_003D6640));
    D_003D6640.flags = 0x4000;
    D_003D6640.unk0C = D_003556D0;
    return data;
}

void effTrackPolyFreeData(EffTrackPolyData *data) {
    sdfQueueAssetRelease(data->nodeHandle);
    sdfReleaseResourceAllocation(data->resourceHandle);
}

void effTrackPolyInitData(EffTrackPolyData *data) {
    data->position = 2;
    data->color = 0x80808080;
    data->activePointCount = 0;
}

/* Select the draw target on the data created by the track constructor. */
void effTrackPolySetDrawKind(EffTrackPolyData *data, u32 kind) {
    data->kind = kind;
}

extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

void effTrackPolyFillGradientColors(EffTrackPolyData *data, u32 *gradientColors) {
    f32 t = 0.0f;
    u32 count = data->count >> 1;
    f32 step = 1.0f / count;
    u32 i = 0;
    u32 *colors = data->colors;

    if (count != 0) {
        do {
            colors[0] = effBlendColor(gradientColors[2], gradientColors[0], t);
            colors[1] = effBlendColor(gradientColors[3], gradientColors[1], t);
            t += step;
            colors += 2;
            i++;
        } while (i < count);
    }
}

/* Copy the pair of track points at the wrapped position into the two outputs. */
void effTrackPolyCopyHistoryPointPair(EffTrackPolyData *data, u128 *dst0, u128 *dst1, s32 amount) {
    s32 pos = data->position - (data->step * (amount - 1) + amount) * 2;
    u128 *p;

    if (pos < 2) {
        pos = (pos + data->count) - 2;
    }
    p = &data->points[pos];
    PCP_COPY_VECTOR(dst0, p);
    p++;
    PCP_COPY_VECTOR(dst1, p);
}

/* Advance around the track's ring, wrapping below the reserved first pair. */
void effTrackPolyAdvancePosition(EffTrackPolyData *data, s32 amount) {
    s32 pos = data->position + ((data->step - 1) * (amount - 1) + amount) * -2;

    if (pos < 2) {
        pos += data->count - 2;
    }
    data->position = pos;
}

/* Append a point pair at the ring position, wrapping back to the reserved pair. */
void effTrackPolyAppendPointPair(EffTrackPolyData *data, u128 *src) {
    s32 position = data->position;
    u128 *points = data->points;
    s32 count;

    PCP_COPY_VECTOR(&points[position], src);
    PCP_COPY_VECTOR(&points[position + 1], src + 1);
    position += 2;
    count = data->count;
    data->position = position;
    if (position == count) {
        PCP_COPY_VECTOR(&points[0], src);
        PCP_COPY_VECTOR(&points[1], src + 1);
        data->position = 2;
    }
    if (data->activePointCount < count - 2) {
        data->activePointCount += 2;
    }
}

/* Resample the newest history span at a fixed interval, smoothing established
 * tracks with Catmull-Rom interpolation and linearly extending short tracks. */
void func_00188A78(EffTrackPolyData *data, u128 *src) {
    f32 points0[4][4];
    f32 points1[4][4];
    f32 base0[4];
    f32 base1[4];
    f32 delta0[4];
    f32 delta1[4];
    f32 out0[4];
    f32 out1[4];
    s32 activePointCount = data->activePointCount;
    s32 step = data->step;
    f32 interval;
    f32 t;
    s32 i;
    u128 *dst1;

    if (activePointCount == 0) {
        effTrackPolyAppendPointPair(data, src);
        return;
    }

    if (activePointCount >= step * 6) {
        s32 position = data->position;
        u128 *dstBase;
        u128 *dst;

        effTrackPolyCopyHistoryPointPair(data, (u128 *)points0[0], (u128 *)points1[0], 3);
        effTrackPolyCopyHistoryPointPair(data, (u128 *)points0[1], (u128 *)points1[1], 2);
        effTrackPolyCopyHistoryPointPair(data, (u128 *)points0[2], (u128 *)points1[2], 1);
        PCP_COPY_VECTOR(points0[3], src);
        PCP_COPY_VECTOR(points1[3], src + 1);
        t = 0.0f;
        interval = 1.0f / (f32)step;
        effTrackPolyAdvancePosition(data, 2);
        if (step > 0) {
            dst = (u128 *)out0;
            dst1 = (u128 *)out1;
            dstBase = dst;
            i = step;
            do {
                t += interval;
                effTrackPolyInterpolateCatmullRomPoint(points0, t);
                VU0_STORE_VF_UNCLOBBERED(vf10, dst);
                effTrackPolyInterpolateCatmullRomPoint(points1, t);
                VU0_STORE_VF_UNCLOBBERED(vf10, dst1);
                dst = dstBase;
                i--;
                effTrackPolyAppendPointPair(data, dst);
            } while (i != 0);
        }
        data->activePointCount = activePointCount;
        data->position = position;
    }

    if (activePointCount >= step * 4) {
        u128 *dstBase;
        u128 *dst;

        effTrackPolyCopyHistoryPointPair(data, (u128 *)points0[0], (u128 *)points1[0], 2);
        effTrackPolyCopyHistoryPointPair(data, (u128 *)points0[1], (u128 *)points1[1], 1);
        PCP_COPY_VECTOR(points0[2], src);
        PCP_COPY_VECTOR(points1[2], src + 1);
        PCP_COPY_VECTOR(points0[3], src);
        PCP_COPY_VECTOR(points1[3], src + 1);
        t = 0.0f;
        interval = 1.0f / (f32)step;
        i = 0;
        if (step > 0) {
            dst = (u128 *)out0;
            dst1 = (u128 *)out1;
            dstBase = dst;
            do {
                t += interval;
                effTrackPolyInterpolateCatmullRomPoint(points0, t);
                VU0_STORE_VF_UNCLOBBERED(vf10, dst);
                effTrackPolyInterpolateCatmullRomPoint(points1, t);
                VU0_STORE_VF_UNCLOBBERED(vf10, dst1);
                dst = dstBase;
                effTrackPolyAppendPointPair(data, dst);
                i++;
            } while (i < step);
        }
    } else {
        u128 *linearDst;

        effTrackPolyCopyHistoryPointPair(data, (u128 *)base0, (u128 *)base1, 1);
        VU0_LOAD_VF(vf10, src);
        VU0_LOAD_VF(vf11, base0);
        VU0_SUB(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, delta0);
        VU0_LOAD_VF(vf10, src + 1);
        VU0_LOAD_VF(vf11, base1);
        VU0_SUB(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, delta1);
        t = 0.0f;
        interval = 1.0f / (f32)step;
        if (step > 0) {
            i = step;
            linearDst = (u128 *)out0;
            do {
                t += interval;
                i--;
                out0[0] = base0[0] + delta0[0] * t;
                out0[1] = base0[1] + delta0[1] * t;
                out0[2] = base0[2] + delta0[2] * t;
                out1[0] = base1[0] + delta1[0] * t;
                out1[1] = base1[1] + delta1[1] * t;
                out1[2] = base1[2] + delta1[2] * t;
                effTrackPolyAppendPointPair(data, linearDst);
            } while (i != 0);
        }
    }
}


typedef struct EffTrackPolyFinish {
    u8 pad00[0x10];
    void (*finish)(void *, s32); /* 0x10 */
} EffTrackPolyFinish;

extern EffTrackPolyFinish *D_00355710[];
extern EffTrackPolyFinish D_00325248;
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(s32);
extern void sdfConsAppendClearPacket(s32, s32);
extern void sdfConsAppendAssetPacket(s32, s32, s32);
extern void sdfAppendPacket(s32, s32);
extern s32 func_0015FE20(EffTrackPolyDraw *);

/* Walk at most two ring runs. Each full strip consumes 16 vertices, with
 * two additional vertices overlapping the next strip (18 inputs total). */
void effTrackPolyDrawStrips(EffTrackPolyData *data) {
    s32 list = sdfAllocPacketAligned(0x20);
    s32 start[4];
    s32 len[2];
    EffTrackPolyDraw *draw;
    s32 i;
    s32 remaining;
    s32 wrapped;
    s32 recent;
    s32 list2;
    u64 *packet;

    sdfInitPacketList(list);
    sdfConsAppendClearPacket(list, 0);
    sdfConsAppendAssetPacket(list, (s32)data->nodeHandle, 0);
    recent = data->activePointCount;
    start[0] = data->position - recent;
    if (start[0] < 2) {
        wrapped = start[0] - 2;
        start[1] = 0;
        start[0] = wrapped + data->count;
        len[0] = -wrapped;
        len[1] = recent - len[0] + 2;
    } else {
        len[0] = recent;
        len[1] = 0;
    }
    D_003D6640.colors = data->colors;
    for (i = 0; i < 2; i++) {
        draw = &D_003D6640;
        draw->points = &data->points[start[i]];
        draw->color = data->color;
        draw->width = 0x10;
        draw->height = 0x12;
        remaining = len[i];
        while (remaining >= 0x12) {
            remaining -= 0x10;
            sdfAppendPacket(list, func_0015FE20(&D_003D6640));
            D_003D6640.points += 0x10;
            D_003D6640.colors += 0x10;
        }
        if (remaining >= 4) {
            draw->height = remaining;
            draw->width = remaining - 2;
            sdfAppendPacket(list, func_0015FE20(draw));
            draw->colors += remaining;
        }
    }
    if (data->kind < 4) {
        D_00355710[data->kind]->finish(D_00355710[data->kind], list);
    } else {
        list2 = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list2);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 6;
        packet[5] = 0x42;
        sdfAppendPacket(list2, (s32)packet);
        D_00325248.finish(&D_00325248, list2);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(list, (s32)packet);
        D_00325248.finish(&D_00325248, list);
    }
}
