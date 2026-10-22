#ifndef MDL_FLAG_API_H
#define MDL_FLAG_API_H

#include "common.h"

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);
void evtBitOff(s32 flag);

/* Clear all model flags. */
void evtBitAllOff(void);

/* Return the selected model-flag bit as 0 or 1. */
s32 evtBitCheck(s32 flag);

#endif /* MDL_FLAG_API_H */
