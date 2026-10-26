#ifndef SDF_GRAPH_BUFFER_STATE_H
#define SDF_GRAPH_BUFFER_STATE_H

#include "common.h"

/* The two color buffers use s32 indices, while per-field selections and the
 * busy-buffer marker are signed bytes with -1 meaning no buffer. */
typedef s32 SdfGraphBufferIndex;
typedef s8 SdfGraphBufferState;

enum {
    SDF_GRAPH_BUFFER_NONE = -1,
    SDF_GRAPH_BUFFER_0 = 0,
    SDF_GRAPH_BUFFER_1 = 1,
};

#endif
