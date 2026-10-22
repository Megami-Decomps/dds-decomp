#ifndef SDF_RESOURCE_LIST_ITEM_H
#define SDF_RESOURCE_LIST_ITEM_H

#include "common.h"

struct DevRequest;
struct sdfTexHandle_t;

/* The list records the texture address as a 32-bit word. Its release path
 * releases that retained texture reference when releaseItems is set. */
void sdfTexListAdd(struct DevRequest *list, struct sdfTexHandle_t *texture);

#endif /* SDF_RESOURCE_LIST_ITEM_H */
