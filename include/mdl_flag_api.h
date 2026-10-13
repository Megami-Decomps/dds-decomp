#ifndef MDL_FLAG_API_H
#define MDL_FLAG_API_H

#include "common.h"

/* Signed indices are adjusted before selecting a model-flag word. */
/* Returns the selected model-flag bit as 0 or 1. */
s32 mdlFlagTest(s32 flag);

#endif /* MDL_FLAG_API_H */
