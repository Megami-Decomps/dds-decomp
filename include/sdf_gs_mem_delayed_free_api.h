#ifndef DDS_SDF_GS_MEM_DELAYED_FREE_API_H
#define DDS_SDF_GS_MEM_DELAYED_FREE_API_H

#include "sdf.h"

/* Queue a non-NULL block for deferred release by sdfGsMemFree. After queueing,
 * callers must not use or release the block again; it remains allocated until
 * delayed-resource processing invokes the free callback. */
void sdfGsMemFreeDelayed(sdfGsMemSlice_t *texture);

#endif
