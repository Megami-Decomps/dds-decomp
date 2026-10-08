#ifndef EFF_SCATTER_DRAW_H
#define EFF_SCATTER_DRAW_H

#include "common.h"

struct SdfAsset;
struct SdfMemBlock;
struct PcpScatterRes;

/* The drawable owns its geometry allocation and asset independently of the
 * effect instance that supplies its transform and scale. */
typedef struct PcpScatterDraw {
    f32 origin[4];             // 0x00
    f32 matrix[16];            // 0x10
    u32 unk50;                // 0x50
    u32 color;                // 0x54
    u32 particleCount;        // 0x58
    s32 vectorsPerParticle;   // 0x5C: two coordinate vectors per vertex pair
    f32 scale;                // 0x60
    f32 *points;              // 0x64
    f32 *uv;                  // 0x68
    u32 *vertexColors;        // 0x6C
    u32 *colors;              // 0x70
    struct SdfAsset *asset;    // 0x74
    struct SdfMemBlock *allocation; // 0x78
    struct PcpScatterRes *sharedResource; // 0x7C
} PcpScatterDraw;

typedef char PcpScatterDrawSizeCheck[sizeof(PcpScatterDraw) == 0x80 ? 1 : -1];
typedef char PcpScatterDrawAssetOffsetCheck[
    ((u32)&((PcpScatterDraw *)0)->asset == 0x74) ? 1 : -1];
typedef char PcpScatterDrawAllocationOffsetCheck[
    ((u32)&((PcpScatterDraw *)0)->allocation == 0x78) ? 1 : -1];

void effReleaseScatterObject(PcpScatterDraw *object);

#endif
