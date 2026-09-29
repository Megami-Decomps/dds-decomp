#include "common.h"

/* Polygon-track data: its ring position wraps against the entry count. */
typedef struct {
    u8  pad_0x00[0x04]; /* 0x00 */
    u32 color;          /* 0x04 */
    s32 count;          /* 0x08 */
    u32 unk0C;          /* 0x0C */
    s32 position;       /* 0x10 */
    s32 step;           /* 0x14 */
    u8  pad_0x18[0x08]; /* 0x18 */
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

void func_001882D8(EffTrackPolyWork *work) {
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

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188480);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001884E8);

void func_00188510(EffTrackPolyWork ***tables, s32 index) {
    effTrackPolyReset((*tables)[index]);
}

void func_00188538(EffTrackPolyWork ***tables, s32 index, u32 value) {
    effTrackPolySetColor((*tables)[index], value);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188560);

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

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188958);

/* Advance around the track's ring, wrapping below the reserved first pair. */
void effTrackPolyAdvancePosition(EffTrackPolyData *data, s32 amount) {
    s32 pos = data->position + ((data->step - 1) * (amount - 1) + amount) * -2;

    if (pos < 2) {
        pos += data->count - 2;
    }
    data->position = pos;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A00);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A78);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188E10);
