#ifndef SDF_GENERAL_HEAP_API_H
#define SDF_GENERAL_HEAP_API_H

#include "common.h"
#include "sdf_mem_block_state.h"

struct sdfMemHandle_t;

/* Borrowed read-only descriptors; exclude heap mutation while traversing. */
const struct sdfMemHandle_t *sdfMemGetTop(void);
/* Return NULL when the following link is the end marker. */
const struct sdfMemHandle_t *sdfMemGetNext(const struct sdfMemHandle_t *block);
/* Return free/used/end-sentinel state, not a resource kind. */
SdfMemBlockState sdfMemGetType(const struct sdfMemHandle_t *block);

#endif /* SDF_GENERAL_HEAP_API_H */
