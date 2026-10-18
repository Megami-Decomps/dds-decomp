#ifndef SDF_DEBUG_API_H
#define SDF_DEBUG_API_H

#include "common.h"

/* SDK printf: the native provider returns vfprintf's result unchanged. */
s32 sdfDebugPrintf(const char *format, ...);

#endif
