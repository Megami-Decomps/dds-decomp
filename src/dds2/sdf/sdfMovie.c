#include "common.h"

typedef struct MovSub {
    u8 pad0[0x4];
    u32 unk4;
    u8 pad8[0x4];
    u32 unkC;
    s32 unk10;
    u8 pad14[0x3C];
    s32 unk50;
    u8 pad54[0x8];
    s32 unk5C;
    u8 pad60[0x8];
    s32 unk68;
    s32 unk6C;
} MovSub;

typedef struct MovObj {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    u8 pad4[0xC];
    s32 unk10;
    u8 pad14[0x4];
    s32 unk18;
    MovSub *stream;
} MovObj;

/* Called with 3 args (sdfMovieProcessPendingData) and 4 args (func_002ECF70); keep K&R. */
void func_0033FBF0();

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00345E18);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00345EB0);

void sdfMovieProcessPendingData(MovObj *movie) {
    MovSub *stream;
    s32 remaining;

    stream = movie->stream;
    remaining = movie->unk18;
    if (remaining == 0) {
        return;
    }
    if ((0x10000 - stream->unk5C) < 0x4000 || ((stream->unk6C - stream->unk68) + 0x10000) < 0x4000) {
        movie->unk1 = 5;
        return;
    }
    movie->unk1 = 4;
    func_0033FBF0(movie->unk10, stream->unk50, remaining <= 0x4000 ? remaining : 0x4000);
}

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_003460D8);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00346468);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_003465E8);
