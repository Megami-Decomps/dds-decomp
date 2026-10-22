#ifndef SDF_ASSET_STATE_H
#define SDF_ASSET_STATE_H

#include "common.h"

struct sdfMat_t;
struct sdfTexHandle_t;
union SdfSubParam;

/* Queue an asset owner for the deferred sdfMat_t release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *asset);

/* Store the primary or secondary texture owner and mark its draw state dirty. */
void sdfSetAssetPrimaryTextureAddress(struct sdfMat_t *asset, struct sdfTexHandle_t *texture);
void sdfMatSetMultiTex(struct sdfMat_t *asset, struct sdfTexHandle_t *texture);

/* Refresh one of the two draw entries and retain the other entry's dirty bits. */
void sdfMatUpdatePacket(struct sdfMat_t *asset, s32 entryIndex);

void sdfMatSetAmbient(struct sdfMat_t *asset, u32 value);
void sdfMatSetDiffuse(struct sdfMat_t *asset, u32 value);
void sdfMatSetSpecular(struct sdfMat_t *asset, u32 value);
void sdfSetPrimaryStateWordFourth(struct sdfMat_t *asset, u32 bits);
void sdfMatSetConstant(struct sdfMat_t *asset, f32 value);
void sdfMatSetMultiDiffuse(struct sdfMat_t *asset, u32 packedColor);
void sdfMatSetMultiMethod(struct sdfMat_t *asset, u32 packetMode);

union SdfSubParam *sdfMatGetTexUvwParm(struct sdfMat_t *asset);
void sdfSetPrimaryTextScalars(struct sdfMat_t *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfMatSetTexUvwByPtr(struct sdfMat_t *asset, const f32 *sourceScalars);
union SdfSubParam *sdfMatGetMultiTexUvwParm(struct sdfMat_t *asset);
void sdfMatSetMultiTexUvw(struct sdfMat_t *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfMatSetMultiTexUvwByPtr(struct sdfMat_t *asset, const f32 *sourceScalars);
void sdfMatSetFaceDivParm(struct sdfMat_t *asset, f32 first, f32 second);

#endif
