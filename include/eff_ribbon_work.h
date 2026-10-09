#ifndef EFF_RIBBON_WORK_H
#define EFF_RIBBON_WORK_H

#include "common.h"
#include "eff_ref_obj.h"
#include "sdf.h"

/* Shared 0x34-byte backing record used by the alternate ribbon renderer.
 * surfaceIndex comes from the alpha track; drawMode is the caller's byte
 * mode, which the renderer tests only for zero/nonzero. */
typedef struct EffRibbonWork {
    u32 count;                 /* 0x00 */
    u32 surfaceIndex;          /* 0x04 */
    u32 color;                 /* 0x08 */
    s32 rowStride;             /* 0x0C */
    s32 repeat;                /* 0x10 */
    u8 drawMode;               /* 0x14 */
    u8 pad_15[3];
    RefObj *resource;          /* 0x18: null selects the shared wind texture */
    u32 *colors;               /* 0x1C */
    u8 *positions;             /* 0x20 */
    u8 *uvs;                    /* 0x24 */
    u8 *extra;                  /* 0x28 */
    SdfAsset *handle;           /* 0x2C */
    struct SdfMemBlock *allocation; /* 0x30 */
} EffRibbonWork;

EffRibbonWork *effCreateRibbonWork(u32 count, u32 repeat);
EffRibbonWork *effCreateRibbonWithSharedResource(
    u32 count, u32 repeat, struct SdfTextureFileHeader *resource);
EffRibbonWork *effCloneRibbonWithSharedResource(EffRibbonWork *source);
void effSharedAssetReferenceRelease(EffRibbonWork *work);

typedef char EffRibbonWorkSizeCheck[(sizeof(EffRibbonWork) == 0x34) ? 1 : -1];
typedef char EffRibbonWorkCountOffsetCheck[((u32)&((EffRibbonWork *)0)->count == 0x00) ? 1 : -1];
typedef char EffRibbonWorkSurfaceIndexOffsetCheck[((u32)&((EffRibbonWork *)0)->surfaceIndex == 0x04) ? 1 : -1];
typedef char EffRibbonWorkColorOffsetCheck[((u32)&((EffRibbonWork *)0)->color == 0x08) ? 1 : -1];
typedef char EffRibbonWorkRowStrideOffsetCheck[((u32)&((EffRibbonWork *)0)->rowStride == 0x0C) ? 1 : -1];
typedef char EffRibbonWorkRepeatOffsetCheck[((u32)&((EffRibbonWork *)0)->repeat == 0x10) ? 1 : -1];
typedef char EffRibbonWorkDrawModeOffsetCheck[((u32)&((EffRibbonWork *)0)->drawMode == 0x14) ? 1 : -1];
typedef char EffRibbonWorkResourceOffsetCheck[((u32)&((EffRibbonWork *)0)->resource == 0x18) ? 1 : -1];
typedef char EffRibbonWorkColorsOffsetCheck[((u32)&((EffRibbonWork *)0)->colors == 0x1C) ? 1 : -1];
typedef char EffRibbonWorkPositionsOffsetCheck[((u32)&((EffRibbonWork *)0)->positions == 0x20) ? 1 : -1];
typedef char EffRibbonWorkUvsOffsetCheck[((u32)&((EffRibbonWork *)0)->uvs == 0x24) ? 1 : -1];
typedef char EffRibbonWorkExtraOffsetCheck[((u32)&((EffRibbonWork *)0)->extra == 0x28) ? 1 : -1];
typedef char EffRibbonWorkHandleOffsetCheck[((u32)&((EffRibbonWork *)0)->handle == 0x2C) ? 1 : -1];
typedef char EffRibbonWorkAllocationOffsetCheck[((u32)&((EffRibbonWork *)0)->allocation == 0x30) ? 1 : -1];

#endif
