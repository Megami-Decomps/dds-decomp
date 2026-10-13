#ifndef SDF_VECTOR_LATCH_H
#define SDF_VECTOR_LATCH_H

#include "common.h"

/* Copy the pending three-float vector into the committed vector and mark it
 * changed. The paired providers take no arguments and return no value. */
void sdfCommitPendingVectorAndMarkChanged(void);

#endif /* SDF_VECTOR_LATCH_H */
