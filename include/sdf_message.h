#ifndef DDS_SDF_MESSAGE_H
#define DDS_SDF_MESSAGE_H

#include "common.h"
#include "sdf_model_surface_set.h"

/* A model provider's draw surfaces: a row of four pool entries in a surface
 * group table. The second is a key-sorted pool (SdfPool), whose release
 * callbacks the store installs. */
void sdfStoreMessageWordsAndNotifyConsumer(SdfModelDrawSurfaceSet surfaces, struct SdfPoolNode *first,
                                           struct SdfPoolNode *pool, struct SdfPoolNode *third,
                                           struct SdfPoolNode *fourth);

#endif
