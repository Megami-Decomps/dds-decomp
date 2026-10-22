#ifndef PRF_REQUIREMENT_FALLBACK_H
#define PRF_REQUIREMENT_FALLBACK_H

#include "common.h"

struct datUnitWork_t;

/* Check rule-backed profile requirements, then explicit fallback flags. */
s32 prfReqCheckWithFallback(struct datUnitWork_t *operand, u16 requirementId);

#endif
