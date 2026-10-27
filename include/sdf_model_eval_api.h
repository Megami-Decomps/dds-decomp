#ifndef SDF_MODEL_EVAL_API_H
#define SDF_MODEL_EVAL_API_H

#include "sdf_graph_buffer_state.h"

struct sdfModelRootNode_t;

/* Propagate the root transform into the selected graph-buffer workspace. */
void sdfModelEvalTm2(struct sdfModelRootNode_t *model, SdfGraphBufferIndex bufferIndex);

#endif /* SDF_MODEL_EVAL_API_H */
