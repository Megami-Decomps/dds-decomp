#ifndef SDF_RESOURCE_LIST_ITEM_H
#define SDF_RESOURCE_LIST_ITEM_H

#include "common.h"

struct DevRequest;
struct sdfTexHandle_t;

/* Store one acquired texture reference without incrementing its refcount.
 * Freeing the list with RELEASE decrements it; KEEP only discards the pointer. */
void sdfTexListAdd(struct DevRequest *list,
                   struct sdfTexHandle_t *acquiredTextureReference);

#endif /* SDF_RESOURCE_LIST_ITEM_H */
