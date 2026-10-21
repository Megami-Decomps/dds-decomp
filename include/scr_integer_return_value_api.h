#ifndef SCR_INTEGER_RETURN_VALUE_API_H
#define SCR_INTEGER_RETURN_VALUE_API_H

#include "common.h"

/* Declared void for the callers that ignore its result: ee-gcc 2.96
 * allocates registers around the call as retail did only then. The
 * definition (scrTraceCode.c) returns the context it set the value in, for
 * the script commands that pass it on as their result; their units declare
 * it so. */
void scrSetIntegerReturnValue(s32 retVal);

#endif /* SCR_INTEGER_RETURN_VALUE_API_H */
