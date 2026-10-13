#ifndef DDS3_WORLD_COUNTER_H
#define DDS3_WORLD_COUNTER_H

#include "common.h"

/* Advance the wrapping 16-bit world object sequence carried in a u32. */
u32 dds3AdvanceWorldCounter(void);

#endif /* DDS3_WORLD_COUNTER_H */
