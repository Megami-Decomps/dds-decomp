#ifndef MDL_H
#define MDL_H

#include "common.h"

typedef struct MdlCtx MdlCtx;

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNodeRefHalf(MdlCtx *ctx, s32 searchId);

#endif /* MDL_H */
