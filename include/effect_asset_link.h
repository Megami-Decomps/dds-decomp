#ifndef EFFECT_ASSET_LINK_H
#define EFFECT_ASSET_LINK_H

#include "common.h"

/* Sentinel-terminated map from asset identifiers to their runtime objects. */
typedef struct EffectAssetLink {
    u8 *asset;
    void *object;
    u8 pad_08[0x14];
} EffectAssetLink;

#endif
