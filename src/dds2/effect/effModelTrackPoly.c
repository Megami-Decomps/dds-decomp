#include "common.h"
#include "pcp_vu0.h"

/* Polygon-track data: its ring position wraps against the entry count. */
typedef struct EffTrackPolyData {
    u32 unk0;
    u32 color;
    s32 count;
    s32 unkC;
    s32 position;
    s32 step;
    u128 *points;      /* 0x18 */
    u8 pad_0x1C[4];    /* 0x1C */
    u32 nodeHandle;
    u32 resourceHandle;
} EffTrackPolyData;

/* Model handle the track's point queries resolve against. */
typedef struct {
    u8 pad_0x00[0x18]; /* 0x00 */
    void *param;       /* 0x18: sdf text parameter */
} EffTrackPolyModel;

/* Outer work area holding track state and the data pointer. */
typedef struct {
    EffTrackPolyModel *model; /* 0x00 */
    s32 idA;                  /* 0x04 */
    s32 idB;                  /* 0x08 */
    f32 unk0C;                /* 0x0C */
    f32 unk10;                /* 0x10 */
    s32 step;                 /* 0x14 */
    u8  pad_0x18[0x1C];       /* 0x18 */
    u32           state;      /* 0x34: cleared on reset */
    EffTrackPolyData *data;   /* 0x38 */
} EffTrackPolyWork; /* 0x3C */

/* Release both owned resources before freeing this track. */
void effTrackPolyRelease(EffTrackPolyWork *track) {
    effTrackPolyFreeData(track->data);
    sdfReleaseChipBlock(track);
}

void effTrackPolyReset(EffTrackPolyWork *track) {
    track->state = 0;
    effTrackPolyInitData(track->data);
}

extern s32 sdfLoadMapRecordPositionVector(void *param, s32 id);
extern void func_001906B0();

void effSampleTrackPolyEndpoints(EffTrackPolyWork *track) {
    EffTrackPolyModel *model = track->model;
    u128 points[2];

    sdfLoadMapRecordPositionVector(model->param, track->idA);
    VU0_STORE_VF(vf10, points);
    sdfLoadMapRecordPositionVector(model->param, track->idB);
    VU0_STORE_VF(vf10, &points[1]);
    func_001906B0(track->data, points);
}

void func_0018FF10(EffTrackPolyWork *track, void *data) {
    func_001906B0(track->data);
}

void effTrackPolySetColor(EffTrackPolyWork *track, u32 color) {
    track->data->color = color;
}

void func_0018FF38(EffTrackPolyWork *track) {
    func_00190A48(track->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_0018FF50);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190018);

void func_001900B8(EffTrackPolyWork ***list) {
    u32 count = list[1];
    u32 i = 0;

    while (i < count) {
        effTrackPolyRelease(list[0][i]);
        i++;
    }
    func_003297C8(list[2]);
}

void func_00190120(EffTrackPolyWork ***tables, s32 index, void *data) {
    func_0018FF10((*tables)[index], data);
}

void effTrackPolyResetIndexedWork(EffTrackPolyWork ***tables, s32 index) {
    effTrackPolyReset((*tables)[index]);
}

void effTrackPolySetIndexedColor(EffTrackPolyWork ***tables, s32 index, u32 color) {
    effTrackPolySetColor((*tables)[index], color);
}

void func_00190198(EffTrackPolyWork ***list) {
    u32 count = list[1];
    u32 i = 0;

    while (i < count) {
        func_0018FF38(list[0][i]);
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
INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190370);

void effTrackPolyFreeData(EffTrackPolyData *data) {
    sdfQueueAssetRelease(data->nodeHandle);
    func_003297C8(data->resourceHandle);
}

void effTrackPolyInitData(EffTrackPolyData *data) {
    data->position = 2;
    data->color = 0x80808080;
    data->unkC = 0;
}

void func_001904A8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001904B0);

/* Copy the pair of track points at the wrapped position into the two outputs. */
void func_00190590(EffTrackPolyData *data, u128 *dst0, u128 *dst1, s32 amount) {
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
    s32 position;

    position = data->position + ((data->step - 1) * (amount - 1) + amount) * -2
    ;
    if (position < 2) {
        position = (position + data->count) - 2;
    }
    data->position = position;
}

/* Append a point pair at the ring position, wrapping back to the reserved pair. */
void func_00190638(EffTrackPolyData *data, u128 *src) {
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
    if (data->unkC < count - 2) {
        data->unkC += 2;
    }
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001906B0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190A48);
