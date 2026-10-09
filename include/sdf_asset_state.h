#ifndef SDF_ASSET_STATE_H
#define SDF_ASSET_STATE_H

#include "common.h"

struct SdfAsset;

void sdfSetPrimaryStateWordFirst(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordSecond(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordThird(struct SdfAsset *asset, u32 value);
void sdfSetPrimaryStateWordFourth(struct SdfAsset *asset, u32 bits);
void sdfSetPrimaryStateFloat(struct SdfAsset *asset, f32 value);

#endif
