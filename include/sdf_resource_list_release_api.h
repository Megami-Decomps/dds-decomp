#ifndef SDF_RESOURCE_LIST_RELEASE_API_H
#define SDF_RESOURCE_LIST_RELEASE_API_H

#include "common.h"

struct DevRequest;

void sdfTexListFree(struct DevRequest *list, s32 releaseItems);

#endif /* SDF_RESOURCE_LIST_RELEASE_API_H */
