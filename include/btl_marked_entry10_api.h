#ifndef BTL_MARKED_ENTRY10_API_H
#define BTL_MARKED_ENTRY10_API_H

#include "common.h"

struct BtlLinkedCommand;

typedef u32 BtlMarkedEntry10Result;
#define BTL_MARKED_ENTRY10_NOT_FOUND ((BtlMarkedEntry10Result)0)
#define BTL_MARKED_ENTRY10_FOUND ((BtlMarkedEntry10Result)1)

/* Return whether a live linked group has its byte at 0x10 marked. */
BtlMarkedEntry10Result btlHasMarkedEntry10(struct BtlLinkedCommand *command);

#endif /* BTL_MARKED_ENTRY10_API_H */
