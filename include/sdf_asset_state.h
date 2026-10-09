#ifndef SDF_ASSET_STATE_H
#define SDF_ASSET_STATE_H

#include "common.h"

struct SdfAsset;
union SdfSubParam;

void sdfSetPrimaryStateWordFirst(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordSecond(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordThird(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordFourth(struct SdfAsset *asset, u32 bits);
void sdfSetPrimaryStateFloat(struct SdfAsset *asset, f32 value);

union SdfSubParam *sdfEnsurePrimaryTextSubParam(struct SdfAsset *asset);
void sdfSetPrimaryTextScalars(struct SdfAsset *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfCopyPrimaryTextScalars(struct SdfAsset *asset, const f32 *sourceScalars);
union SdfSubParam *sdfEnsureSecondaryTextSubParam(struct SdfAsset *asset);
void sdfSetSecondaryTextScalars(struct SdfAsset *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfCopySecondaryTextScalars(struct SdfAsset *asset, const f32 *sourceScalars);
void sdfSetTextScalarPair(struct SdfAsset *asset, f32 first, f32 second);

#endif
