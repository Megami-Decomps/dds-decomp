#ifndef PRF_REQUIREMENT_FALLBACK_H
#define PRF_REQUIREMENT_FALLBACK_H

#include "common.h"

struct DatPartyRecord;

/* Check rule-backed profile requirements, then explicit fallback flags. */
s32 prfReqCheckWithFallback(struct DatPartyRecord *operand, u16 requirementId);

#endif
