#ifndef DDS3_OBJECT_FLAGS_H
#define DDS3_OBJECT_FLAGS_H

#include "common.h"

/* Test whether any requested object flag bit is set. */
u8 dds3TestObjectFlags(void *object, s32 mask);

#endif
