#include "common.h"
#include "pcp_vu0.h"

/* Polygon-track data: its ring position wraps against the entry count. */
typedef struct {
    u32 kind;           /* 0x00: draw-target selector */
    u32 color;          /* 0x04 */
    s32 count;          /* 0x08 */
    s32 activePointCount; /* 0x0C */
    s32 position;       /* 0x10 */
    s32 step;           /* 0x14 */
    u128 *points;      /* 0x18 */
    s32 uvBase;         /* 0x1C */
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

extern s32 sdfLoadMapRecordPositionVector(void *param, s32 id);
extern void func_00188A78();

void effSampleTrackPolyEndpoints(EffTrackPolyWork *work) {
    EffTrackPolyModel *model = work->model;
    u128 points[2];

    sdfLoadMapRecordPositionVector(model->param, work->idA);
    VU0_STORE_VF(vf10, points);
    sdfLoadMapRecordPositionVector(model->param, work->idB);
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
    effTrackPolyDrawStrips(work->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188318);

typedef struct EffTrackPolyList {
    EffTrackPolyWork **items; /* 0x00 */
    u32 count;                /* 0x04 */
    u32 handle;               /* 0x08 */
} EffTrackPolyList;

extern u32 func_002D03F8(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern EffTrackPolyWork *effTrackPolyCreateWork(EffTrackPolyModel *model);

/* Build a list of `count` track-poly works over one model. */
EffTrackPolyList *effTrackPolyCreateModelWorkList(EffTrackPolyModel *model, u32 count) {
    u32 handle = func_002D03F8(count * 4 + 0xC);
    EffTrackPolyList *list = (EffTrackPolyList *)sdfResourceRetainAddress(handle);
    u32 i;

    list->handle = handle;
    list->items = (EffTrackPolyWork **)(list + 1);
    list->count = count;
    for (i = 0; i < count; i++) {
        list->items[i] = effTrackPolyCreateWork(model);
    }
    return list;
}

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
    data->activePointCount = 0;
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
    if (data->activePointCount < count - 2) {
        data->activePointCount += 2;
    }
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A78);

/* Draw-state record read by func_0015FE20 (same layout as ParDrawState in code_0015A758). */
typedef struct EffTrackPolyDraw {
    u16 width;      /* 0x00 */
    u16 height;     /* 0x02 */
    u8 pad04[4];
    u32 color;      /* 0x08 */
    u8 pad0C[4];
    u8 *points;     /* 0x10 */
    u8 pad14[0xC];
    s32 uvOffset;   /* 0x20 */
    u8 pad24[8];
} EffTrackPolyDraw; /* 0x2C */

typedef struct EffTrackPolyFinish {
    u8 pad00[0x10];
    void (*finish)(void *, s32); /* 0x10 */
} EffTrackPolyFinish;

extern EffTrackPolyDraw D_003D6640;
extern EffTrackPolyFinish *D_00355710[];
extern EffTrackPolyFinish D_00325248;
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(s32);
extern void sdfConsAppendClearPacket(s32, s32);
extern void sdfConsAppendAssetPacket(s32, s32, s32);
extern void sdfAppendPacket(s32, s32);
extern s32 func_0015FE20(EffTrackPolyDraw *);

/* Draw the track as strips of 16 point pairs: the ring buffer is walked as at most two runs (the recent end, then the wrapped start). */
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
    D_003D6640.uvOffset = data->uvBase;
    for (i = 0; i < 2; i++) {
        draw = &D_003D6640;
        draw->points = (u8 *)data->points + (start[i] << 4);
        draw->color = data->color;
        draw->width = 0x10;
        draw->height = 0x12;
        remaining = len[i];
        while (remaining >= 0x12) {
            remaining -= 0x10;
            sdfAppendPacket(list, func_0015FE20(&D_003D6640));
            D_003D6640.points += 0x100;
            D_003D6640.uvOffset += 0x40;
        }
        if (remaining >= 4) {
            draw->height = remaining;
            draw->width = remaining - 2;
            sdfAppendPacket(list, func_0015FE20(draw));
            draw->uvOffset += remaining * 4;
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
