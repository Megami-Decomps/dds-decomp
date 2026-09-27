#include "common.h"

/* Data block touched by the init/step/free helpers below. */
typedef struct {
    u8  pad_0x00[0x04]; /* 0x00 */
    u32 color;          /* 0x04 */
    s32 unk08;          /* 0x08 */
    u32 unk0C;          /* 0x0C */
    s32 unk10;          /* 0x10 */
    s32 unk14;          /* 0x14 */
    u8  pad_0x18[0x08]; /* 0x18 */
    void *unk20;        /* 0x20 */
    void *unk24;        /* 0x24 */
} EffTrackPolyData; /* 0x28 */

/* Outer work area holding the table index field and the data pointer. */
typedef struct {
    u8             pad_0x00[0x34]; /* 0x00 */
    s32            unk34;          /* 0x34: cleared on reset */
    EffTrackPolyData *unk38;      /* 0x38 */
} EffTrackPolyWork; /* 0x3C */

/* Inner cell whose second word is written by func_001882F0. */
typedef struct {
    u32 unk0; /* 0x00 */
    u32 unk4; /* 0x04 */
} EffTrackPolyCell;

void func_00188228(EffTrackPolyWork *work) {
    effTrackPolyFreeData(work->unk38);
    func_002CFF98(work);
}

void func_00188258(EffTrackPolyWork *work) {
    work->unk34 = 0;
    effTrackPolyInitData(work->unk38);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188278);

void func_001882D8(EffTrackPolyWork *work) {
    func_00188A78(work->unk38);
}

void func_001882F0(EffTrackPolyWork *work, u32 value) {
    ((EffTrackPolyCell *)work->unk38)->unk4 = value;
}

void func_00188300(EffTrackPolyWork *work) {
    func_00188E10(work->unk38);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188318);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001883E0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188480);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001884E8);

void func_00188510(EffTrackPolyWork ***tables, s32 index) {
    func_00188258((*tables)[index]);
}

void func_00188538(EffTrackPolyWork ***tables, s32 index, u32 value) {
    func_001882F0((*tables)[index], value);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188560);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001885C0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188738);

void effTrackPolyFreeData(EffTrackPolyData *data) {
    func_002DAA68(data->unk20);
    func_002D0918(data->unk24);
}

void effTrackPolyInitData(EffTrackPolyData *data) {
    data->unk10 = 2;
    data->color = 0x80808080;
    data->unk0C = 0;
}

void func_00188870(u32 *dst, u32 value) {
    *dst = value;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188878);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188958);

void func_001889B8(EffTrackPolyData *data, s32 arg1) {
    s32 pos = data->unk10 + ((data->unk14 - 1) * (arg1 - 1) + arg1) * -2;

    if (pos < 2) {
        pos += data->unk08 - 2;
    }
    data->unk10 = pos;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A00);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A78);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188E10);
