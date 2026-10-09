#ifndef SDF_TEXTURE_REGISTRY_H
#define SDF_TEXTURE_REGISTRY_H

#include "common.h"

struct SdfTex;

/* The active resource registry contains SdfTex nodes; lookup compares the
 * texture's resource key and follows its previous-node link. */
extern struct SdfTex *sdfResourceListHead;
struct SdfTex *sdfFindTextureByResourceKey(s32 resourceKey);

#endif /* SDF_TEXTURE_REGISTRY_H */
