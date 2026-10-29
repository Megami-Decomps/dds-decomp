#ifndef SDF_GS_CONTEXT_SELECTOR_H
#define SDF_GS_CONTEXT_SELECTOR_H

#include "common.h"

/* Select the GS register bank targeted by an SDF packet. */
typedef s32 SdfGsContext;

#ifndef SDF_GS_CONTEXT_1
#define SDF_GS_CONTEXT_1 0
#endif
#ifndef SDF_GS_CONTEXT_2
#define SDF_GS_CONTEXT_2 1
#endif

#endif /* SDF_GS_CONTEXT_SELECTOR_H */
