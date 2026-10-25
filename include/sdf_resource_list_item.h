#ifndef SDF_RESOURCE_LIST_ITEM_H
#define SDF_RESOURCE_LIST_ITEM_H

#include "common.h"

struct DevRequest;
struct sdfTexHandle_t;

/* The list records each texture address as a 32-bit word and retains its
 * reference until a release policy drops it. */
void sdfTexListAdd(struct DevRequest *list, struct sdfTexHandle_t *texture);

#endif /* SDF_RESOURCE_LIST_ITEM_H */
