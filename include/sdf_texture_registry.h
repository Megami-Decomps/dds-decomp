#ifndef SDF_TEXTURE_REGISTRY_H
#define SDF_TEXTURE_REGISTRY_H

#include "common.h"

struct sdfTexHandle_t;

/* The active resource registry contains sdfTexHandle_t nodes; lookup compares the
 * texture's resource key and follows its previous-node link. */
extern struct sdfTexHandle_t *sdfTexHandleLink;
struct sdfTexHandle_t *sdfTexSearch(s32 resourceKey);

#endif /* SDF_TEXTURE_REGISTRY_H */
