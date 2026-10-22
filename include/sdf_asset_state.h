#ifndef SDF_ASSET_STATE_H
#define SDF_ASSET_STATE_H

#include "common.h"

struct SdfAsset;
struct SdfTex;
union SdfSubParam;

/* Queue an asset owner for the deferred SdfAsset release callback. */
void sdfMatFreeDelayed(struct SdfAsset *asset);

/* Store the primary or secondary texture owner and mark its draw state dirty. */
void sdfSetAssetPrimaryTextureAddress(struct SdfAsset *asset, struct SdfTex *texture);
void sdfMatSetMultiTex(struct SdfAsset *asset, struct SdfTex *texture);

/* Refresh one of the two draw entries and retain the other entry's dirty bits. */
void sdfMatUpdatePacket(struct SdfAsset *asset, s32 entryIndex);

void sdfMatSetAmbient(struct SdfAsset *asset, u32 value);
void sdfMatSetDiffuse(struct SdfAsset *asset, u32 value);
void sdfMatSetSpecular(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordFourth(struct SdfAsset *asset, u32 bits);
void sdfMatSetConstant(struct SdfAsset *asset, f32 value);
void sdfMatSetMultiDiffuse(struct SdfAsset *asset, u32 packedColor);
void sdfMatSetMultiMethod(struct SdfAsset *asset, u32 packetMode);

union SdfSubParam *sdfMatGetTexUvwParm(struct SdfAsset *asset);
void sdfSetPrimaryTextScalars(struct SdfAsset *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfMatSetTexUvwByPtr(struct SdfAsset *asset, const f32 *sourceScalars);
union SdfSubParam *sdfMatGetMultiTexUvwParm(struct SdfAsset *asset);
void sdfMatSetMultiTexUvw(struct SdfAsset *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfMatSetMultiTexUvwByPtr(struct SdfAsset *asset, const f32 *sourceScalars);
void sdfMatSetFaceDivParm(struct SdfAsset *asset, f32 first, f32 second);

#endif
