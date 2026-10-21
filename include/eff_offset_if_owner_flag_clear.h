#ifndef EFF_OFFSET_IF_OWNER_FLAG_CLEAR_H
#define EFF_OFFSET_IF_OWNER_FLAG_CLEAR_H

#include "common.h"

struct BtlUnit;

/* Add one to the supplied effect offset when the owner's bit 9 is clear. */
s32 effOffsetIfOwnerFlagClear(struct BtlUnit *owner, s32 base);

#endif /* EFF_OFFSET_IF_OWNER_FLAG_CLEAR_H */
