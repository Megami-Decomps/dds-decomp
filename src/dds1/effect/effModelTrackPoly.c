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

/* Outer work area holding track state and the data pointer. */
typedef struct {
    u8             pad_0x00[0x34]; /* 0x00 */
    s32            state;          /* 0x34: cleared on reset */
    EffTrackPolyData *data;        /* 0x38 */
} EffTrackPolyWork; /* 0x3C */


/* Release both owned resources before freeing this track. */
void effTrackPolyRelease(EffTrackPolyWork *work) {
    effTrackPolyFreeData(work->data);
    func_002CFF98(work);
}

void effTrackPolyReset(EffTrackPolyWork *work) {
    work->state = 0;
    effTrackPolyInitData(work->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188278);

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

void func_00188510(EffTrackPolyWork ***tables, s32 index) {
    effTrackPolyReset((*tables)[index]);
}

void func_00188538(EffTrackPolyWork ***tables, s32 index, u32 value) {
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

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001885C0);

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
