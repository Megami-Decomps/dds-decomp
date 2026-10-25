#ifndef FLD_RESOURCE_WAIT_API_H
#define FLD_RESOURCE_WAIT_API_H

#include "common.h"

typedef u8 FldResourceWaitResult;
#define FLD_RESOURCE_WAIT_PENDING 0
#define FLD_RESOURCE_WAIT_FINISHED 1

/* Return whether the field's pending resource work has finished. */
FldResourceWaitResult fldIsFieldResourceWaitFinished(void);

#endif /* FLD_RESOURCE_WAIT_API_H */
