#ifndef MDL_WORLD_OBJECT_SOURCE_VECTOR_API_H
#define MDL_WORLD_OBJECT_SOURCE_VECTOR_API_H

#include "common.h"

/* Copy the source object's transform to the target object, with its rotation offset. */
void mdlAttachWorldObjectToSourceVector(u32 targetKey, u32 sourceKey);

#endif /* MDL_WORLD_OBJECT_SOURCE_VECTOR_API_H */
