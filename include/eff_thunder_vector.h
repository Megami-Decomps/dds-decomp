#ifndef EFF_THUNDER_VECTOR_H
#define EFF_THUNDER_VECTOR_H

#include "common.h"

struct ParSystem;
struct SdfMemBlock;

/* Indexed-vector parameters retain a dispatch pointer and geometry widths. */
typedef struct EffThunderVectorParams {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    u32 cellCount;
    u8 pad18[4];
    f32 radiusScale;
    f32 heightScale;
    f32 rotationScale;
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 perCell;
    u8 pad32[0xA];
    f32 bandWidth;
    void *dispatchArg;
    f32 edgeWidth;
    u8 pad48[4];
} EffThunderVectorParams;

/* Alpha-system parameters supply three packed words to the particle system. */
typedef struct EffThunderAlphaParams {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    u32 cellCount;
    u8 pad18[4];
    f32 radiusScale;
    f32 heightScale;
    f32 rotationScale;
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 perCell;
    u8 pad32[6];
    u32 centerAlphaWord;
    u32 reserved3C;
    u32 middleAlphaWord;
    u32 reserved44;
    u32 edgeAlphaWord;
} EffThunderAlphaParams;

/* Constructors copy all 0x4C bytes. The vector updater reads the retained
 * geometry bytes through the vector view for both constructor variants. */
typedef union EffThunderParameterHead {
    EffThunderVectorParams vector;
    EffThunderAlphaParams alpha;
} EffThunderParameterHead;

typedef struct EffThunderVectorCell {
    u32 delayFrames;
    u32 activeFrames;
    f32 placementVector[3];
    f32 rotationAxis[3];
    f32 rotationScale;
    f32 radius;
    u32 color;
} EffThunderVectorCell;

/* The complete work is followed by cellCount cells in the same allocation. */
typedef struct EffThunderVectorWork {
    EffThunderParameterHead head;
    EffThunderVectorCell *cells;
    u32 tintColor;
    f32 baseRadiusScale;
    f32 baseHeightScale;
    struct ParSystem *cellSystem;
    struct SdfMemBlock *allocationHandle;
} EffThunderVectorWork;

typedef char EffThunderVectorParams_size_check[sizeof(EffThunderVectorParams) == 0x4C ? 1 : -1];
typedef char EffThunderAlphaParams_size_check[sizeof(EffThunderAlphaParams) == 0x4C ? 1 : -1];
typedef char EffThunderParameterHead_size_check[sizeof(EffThunderParameterHead) == 0x4C ? 1 : -1];
typedef char EffThunderVectorCell_size_check[sizeof(EffThunderVectorCell) == 0x2C ? 1 : -1];
typedef char EffThunderVectorWork_size_check[sizeof(EffThunderVectorWork) == 0x64 ? 1 : -1];

EffThunderVectorWork *effCreateThunderCellSystemWork(EffThunderAlphaParams *parameters);
EffThunderVectorWork *effThunderWorkCreate(EffThunderVectorParams *parameters);
EffThunderParameterHead *effThunderGetParameterHead(EffThunderVectorWork *work);
void effThunderReleaseVectorWork(EffThunderVectorWork *work);
void effThunderSetVectorTint(EffThunderVectorWork *work, u32 tintColor);
void effThunderScaleVectorDimensions(f32 factor, EffThunderVectorWork *work);
void effThunderUpdateVectorCells(EffThunderVectorWork *work);

#endif
