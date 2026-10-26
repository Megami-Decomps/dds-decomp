#ifndef EFF_MAPPED_RESOURCE_DESTROY_RESULT_H
#define EFF_MAPPED_RESOURCE_DESTROY_RESULT_H

#include "common.h"

/* effDestroyPackedBatch has one normal outcome after freeing all owned storage. */
typedef s32 EffMappedResourceDestroyResult;
enum {
    EFF_MAPPED_RESOURCE_DESTROYED = 1
};

#endif /* EFF_MAPPED_RESOURCE_DESTROY_RESULT_H */
