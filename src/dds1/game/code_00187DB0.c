#include "common.h"

INCLUDE_ASM(const s32, "game/code_00187DB0", effResourceQuadDraw);

typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffResourceRectBounds;

/* Nine copied words; the selected source handle belongs to the owner. */
typedef struct {
    s32 extent;
    s32 centerX;
    s32 centerY;
    u8 color[4];
    s32 blendControl;
    EffResourceRectBounds bounds;
} EffResourceRectParams; /* 0x24 */

typedef struct {
    EffResourceRectParams params;
    u32 sourceHandle;
} EffResourceRectWork; /* 0x28 */

extern void *func_002CFEB8(s32 size);
extern u32 effGetResourceFirstWord(s32 index);

/* Clone rectangle parameters and select a fresh source handle. */
EffResourceRectWork *effCloneResourceTemplate(EffResourceRectParams *src) {
    EffResourceRectWork *dst = func_002CFEB8(sizeof(EffResourceRectWork));

    dst->sourceHandle = effGetResourceFirstWord(0);
    dst->params = *src;
    return dst;
}

void func_00188050(void) {
    sdfReleaseChipBlock();
}

extern s32 effResourceQuadDraw(void *data, s32 resource, s32 flags);

/* Generate pixel-coordinate bounds; the renderer applies GS coordinate scale. */
s32 effResourceRectDrawPixels(EffResourceRectWork *work) {
    s32 extent = (s32)((f32)work->params.extent * 1.4f);
    s32 x = work->params.centerX + 0x100;
    s32 y = work->params.centerY + 0xE0;

    work->params.bounds.left = x - extent;
    work->params.bounds.top = y - extent;
    work->params.bounds.right = x + extent;
    work->params.bounds.bottom = y + extent;
    return effResourceQuadDraw(work->params.color, work->sourceHandle, 0);
}

/* Generate already-scaled GS coordinates; the renderer must not scale again. */
void effResourceRectDrawGsCoords(EffResourceRectWork *work) {
    f32 scaledExtent = (f32)work->params.extent * 1.4f;
    s32 x = work->params.centerX + 0x1000;
    s32 y = (work->params.centerY + 0xE00) >> 1;
    s32 extent = (s32)scaledExtent;
    s32 right = x + extent;
    s32 bottom;

    x -= extent;
    extent >>= 1;
    work->params.bounds.left = x;
    bottom = y + extent;
    y -= extent;
    work->params.bounds.right = right;
    work->params.bounds.top = y;
    work->params.bounds.bottom = bottom;
    effResourceQuadDraw(work->params.color, work->sourceHandle, 1);
}

typedef struct EffTrackPolyModel EffTrackPolyModel;
typedef struct EffTrackPolyData EffTrackPolyData;

/* Same parameter/owner layout as effect/effModelTrackPoly. */
typedef struct {
    EffTrackPolyModel *model;
    s32 idA;
    s32 idB;
    f32 unk0C;
    f32 unk10;
    s32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 kind;
    u32 gradientColors[4];
} EffTrackPolyParams; /* 0x34 */

typedef struct {
    EffTrackPolyParams params;
    u32 updateCount;
    EffTrackPolyData *data;
} EffTrackPolyWork; /* 0x3C */

extern EffTrackPolyData *func_00188738(s32 historyLength, s32 step);
extern void func_00188878(EffTrackPolyData *data, u32 *colors);
extern void func_00188870(EffTrackPolyData *data, u32 kind);

/* Clone a model track with independent history and gradient storage. */
EffTrackPolyWork *effTrackPolyCreateWork(EffTrackPolyParams *src) {
    EffTrackPolyWork *dst = func_002CFEB8(sizeof(EffTrackPolyWork));

    dst->params = *src;
    dst->params.unk1C = 0xC;
    dst->updateCount = 0;
    dst->data = func_00188738(dst->params.historyLength, 0xC);
    func_00188878(dst->data, src->gradientColors);
    func_00188870(dst->data, src->kind);
    return dst;
}
