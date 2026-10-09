#ifndef SDF_ASSET_STATE_H
#define SDF_ASSET_STATE_H

#include "common.h"

struct SdfAsset;
struct SdfTex;
union SdfSubParam;

/* Queue an asset owner for the deferred SdfAsset release callback. */
void sdfQueueAssetRelease(struct SdfAsset *asset);

/* Store the primary or secondary texture owner and mark its draw state dirty. */
void sdfSetAssetPrimaryTextureAddress(struct SdfAsset *asset, struct SdfTex *texture);
void sdfSetAssetSecondaryTextureAddress(struct SdfAsset *asset, struct SdfTex *texture);

/* Refresh one of the two draw entries and retain the other entry's dirty bits. */
void sdfAssetApplyEntryChanges(struct SdfAsset *asset, s32 entryIndex);

void sdfSetPrimaryStateWordFirst(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordSecond(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordThird(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordFourth(struct SdfAsset *asset, u32 bits);
void sdfSetPrimaryStateFloat(struct SdfAsset *asset, f32 value);
void sdfSetAssetSecondaryColor(struct SdfAsset *asset, u32 packedColor);
void sdfSetAssetSecondaryMode(struct SdfAsset *asset, u32 packetMode);

union SdfSubParam *sdfEnsurePrimaryTextSubParam(struct SdfAsset *asset);
void sdfSetPrimaryTextScalars(struct SdfAsset *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfCopyPrimaryTextScalars(struct SdfAsset *asset, const f32 *sourceScalars);
union SdfSubParam *sdfEnsureSecondaryTextSubParam(struct SdfAsset *asset);
void sdfSetSecondaryTextScalars(struct SdfAsset *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfCopySecondaryTextScalars(struct SdfAsset *asset, const f32 *sourceScalars);
void sdfSetTextScalarPair(struct SdfAsset *asset, f32 first, f32 second);

#endif
