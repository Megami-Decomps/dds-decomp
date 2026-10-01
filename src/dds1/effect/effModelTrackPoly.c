#include "common.h"
#include "pcp_vu0.h"

/* Polygon-track data: its ring position wraps against the entry count. */
typedef struct {
    u8  pad_0x00[0x04]; /* 0x00 */
    u32 color;          /* 0x04 */
    s32 count;          /* 0x08 */
    s32 unk0C;          /* 0x0C */
    s32 position;       /* 0x10 */
    s32 step;           /* 0x14 */
    u128 *points;      /* 0x18 */
    u8  pad_0x1C[0x04]; /* 0x1C */
    void *nodeHandle;   /* 0x20 */
    void *resourceHandle; /* 0x24 */
} EffTrackPolyData; /* 0x28 */

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
    s32            state;     /* 0x34: cleared on reset */
    EffTrackPolyData *data;   /* 0x38 */
} EffTrackPolyWork; /* 0x3C */


/* Release both owned resources before freeing this track. */
void effTrackPolyRelease(EffTrackPolyWork *work) {
    effTrackPolyFreeData(work->data);
    sdfReleaseChipBlock(work);
}

void effTrackPolyReset(EffTrackPolyWork *work) {
    work->state = 0;
    effTrackPolyInitData(work->data);
}

extern s32 func_002D9E98(void *param, s32 id);
extern void func_00188A78();

void func_00188278(EffTrackPolyWork *work) {
    EffTrackPolyModel *model = work->model;
    u128 points[2];

    func_002D9E98(model->param, work->idA);
    VU0_STORE_VF(vf10, points);
    func_002D9E98(model->param, work->idB);
    VU0_STORE_VF(vf10, &points[1]);
    func_00188A78(work->data, points);
}

void func_001882D8(EffTrackPolyWork *work, void *data) {
    func_00188A78(work->data);
}

void effTrackPolySetColor(EffTrackPolyWork *work, u32 color) {
    work->data->color = color;
}

void func_00188300(EffTrackPolyWork *work) {
    func_00188E10(work->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188318);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001883E0);

void func_00188480(EffTrackPolyWork ***list) {
    u32 count = list[1];
    u32 i = 0;

    while (i < count) {
        effTrackPolyRelease(list[0][i]);
        i++;
    }
    func_002D0918(list[2]);
}

void func_001884E8(EffTrackPolyWork ***tables, s32 index, void *data) {
    func_001882D8((*tables)[index], data);
}

void effTrackPolyResetIndexedWork(EffTrackPolyWork ***tables, s32 index) {
    effTrackPolyReset((*tables)[index]);
}

void effTrackPolySetIndexedColor(EffTrackPolyWork ***tables, s32 index, u32 value) {
    effTrackPolySetColor((*tables)[index], value);
}

void func_00188560(EffTrackPolyWork ***list) {
    u32 count = list[1];
    u32 i = 0;

    while (i < count) {
        func_00188300(list[0][i]);
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
INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188738);

void effTrackPolyFreeData(EffTrackPolyData *data) {
    sdfQueueAssetRelease(data->nodeHandle);
    func_002D0918(data->resourceHandle);
}

void effTrackPolyInitData(EffTrackPolyData *data) {
    data->position = 2;
    data->color = 0x80808080;
    data->unk0C = 0;
}

void func_00188870(u32 *dst, u32 value) {
    *dst = value;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188878);

/* Copy the pair of track points at the wrapped position into the two outputs. */
void func_00188958(EffTrackPolyData *data, u128 *dst0, u128 *dst1, s32 amount) {
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
void func_00188A00(EffTrackPolyData *data, u128 *src) {
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
    if (data->unk0C < count - 2) {
        data->unk0C += 2;
    }
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A78);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188E10);
