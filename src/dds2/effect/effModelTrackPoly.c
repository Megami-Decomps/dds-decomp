#include "common.h"

/* Polygon-track data: its ring position wraps against the entry count. */
typedef struct EffTrackPolyData {
    u32 unk0;
    u32 color;
    u32 count;
    u32 unkC;
    s32 position;
    s32 step;
    u8 pad18[8];
    u32 nodeHandle;
    u32 resourceHandle;
} EffTrackPolyData;

/* Outer work area holding track state and the data pointer. */
typedef struct EffTrackPolyWork {
    u8 pad0[0x34];
    u32 state;
    EffTrackPolyData *data;
} EffTrackPolyWork;

/* Release both owned resources before freeing this track. */
void effTrackPolyRelease(EffTrackPolyWork *track) {
    effTrackPolyFreeData(track->data);
    func_00328E48(track);
}

void effTrackPolyReset(EffTrackPolyWork *track) {
    track->state = 0;
    effTrackPolyInitData(track->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_0018FEB0);

void func_0018FF10(EffTrackPolyWork *track) {
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

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001900B8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190120);

void func_00190148(EffTrackPolyWork ***tables, s32 index) {
    effTrackPolyReset((*tables)[index]);
}

void func_00190170(EffTrackPolyWork ***tables, s32 index, u32 color) {
    effTrackPolySetColor((*tables)[index], color);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190198);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001901F8);

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

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190590);

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

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190638);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001906B0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190A48);
