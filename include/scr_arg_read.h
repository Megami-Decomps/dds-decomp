#ifndef DDS_SCR_ARG_READ_H
#define DDS_SCR_ARG_READ_H

#include "common.h"

/* Read the current script argument as a float, converting supported numeric tags. */
f32 bfWaitReadArgFloat(s32 idx);

#endif
