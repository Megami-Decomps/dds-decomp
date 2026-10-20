#ifndef DDS3_OBJECT_FLAGS_H
#define DDS3_OBJECT_FLAGS_H

#include "common.h"

struct EffWorldNode;

/* Test whether any requested object flag bit is set. */
u8 dds3ObjectBaseCheckFlag(struct EffWorldNode *object, u32 mask);

#endif
