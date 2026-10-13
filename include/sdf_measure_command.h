#ifndef SDF_MEASURE_COMMAND_H
#define SDF_MEASURE_COMMAND_H

#include "common.h"

/* Prefix consumed by sdfCommandListMeasure; the kind-specific payload is opaque. */
typedef struct SdfMeasureCommandPrefix {
    u8 kind;
    u8 opaque01[0xB];
    u16 quadwordCount; /* +0x0C, consumed for kind 2 */
} SdfMeasureCommandPrefix;

typedef char SdfMeasureCommandPrefix_quadwordCount_at_0x0C[
    ((u32)&((SdfMeasureCommandPrefix *)0)->quadwordCount == 0x0C) ? 1 : -1];

#endif /* SDF_MEASURE_COMMAND_H */
