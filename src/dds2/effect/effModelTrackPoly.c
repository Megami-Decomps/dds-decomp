#include "common.h"

typedef struct TrackPolyData {
    u32 unk0;
    u32 color;
} TrackPolyData;

typedef struct TrackPoly {
    u8 pad0[0x34];
    u32 state;
    TrackPolyData *data;
} TrackPoly;

void func_0018FE60(TrackPoly *track) {
    effTrackPolyFreeData(track->data);
    func_00328E48(track);
}

void func_0018FE90(TrackPoly *track) {
    track->state = 0;
    effTrackPolyInitData(track->data);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_0018FEB0);

void func_0018FF10(TrackPoly *track) {
    func_001906B0(track->data);
}

void func_0018FF28(TrackPoly *track, u32 color) {
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
    func_0018FE90(*(u32 *)(arg1 * 4 + *arg0));
}

void func_00190170(s32 *arg0, s32 arg1, u32 arg2) {
    func_0018FF28(*(u32 *)(arg1 * 4 + *arg0), arg2);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190198);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001901F8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190370);

void effTrackPolyFreeData(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 0x24));
}

void effTrackPolyInitData(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 2;
    *(u32 *)(arg0 + 4) = 0x80808080;
    *(u32 *)(arg0 + 0xc) = 0;
}

void func_001904A8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001904B0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190590);

void func_001905F0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10) + ((*(s32 *)(arg0 + 0x14) - 1) * (arg1 - 1) + arg1) * -2
    ;
    if (temp_v0 < 2) {
        temp_v0 = (temp_v0 + *(s32 *)(arg0 + 8)) - 2;
    }
    *(s32 *)(arg0 + 0x10) = temp_v0;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190638);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001906B0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190A48);
