#include "common.h"

typedef struct TrackPolyData {
    u32 unk0;
    u32 color;
    u32 count;
    u32 unkC;
    s32 position;
    s32 step;
    u8 pad18[8];
    u32 nodeHandle;
    u32 resourceHandle;
} TrackPolyData;

typedef struct TrackPoly {
    u8 pad0[0x34];
    u32 state;
    TrackPolyData *data;
} TrackPoly;

void effTrackPolyRelease(TrackPoly *track) {
    effTrackPolyFreeData(track->data);
    func_00328E48(track);
}

void effTrackPolyReset(TrackPoly *track) {
    track->state = 0;
    effTrackPolyInitData(track->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_0018FEB0);

void func_0018FF10(TrackPoly *track) {
    func_001906B0(track->data);
}

void effTrackPolySetColor(TrackPoly *track, u32 color) {
    track->data->color = color;
}

void func_0018FF38(TrackPoly *track) {
    func_00190A48(track->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_0018FF50);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190018);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001900B8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190120);

void func_00190148(s32 *arg0, s32 arg1) {
    effTrackPolyReset(*(u32 *)(arg1 * 4 + *arg0));
}

void func_00190170(s32 *arg0, s32 arg1, u32 arg2) {
    effTrackPolySetColor(*(u32 *)(arg1 * 4 + *arg0), arg2);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190198);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001901F8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190370);

void effTrackPolyFreeData(TrackPolyData *data) {
    sdfQueueAssetRelease(data->nodeHandle);
    func_003297C8(data->resourceHandle);
}

void effTrackPolyInitData(TrackPolyData *data) {
    data->position = 2;
    data->color = 0x80808080;
    data->unkC = 0;
}

void func_001904A8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001904B0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190590);

void effTrackPolyAdvancePosition(TrackPolyData *data, s32 amount) {
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
