#ifndef ITF_GRID_STATUS_RECORDS_H
#define ITF_GRID_STATUS_RECORDS_H

#include "common.h"

/* Consumed prefixes of effect status payloads; any resource tail remains opaque. */
typedef struct GridAngleTable {
    s32 divisor;
    s32 mirrored;
    s32 cycleDivisor;
} GridAngleTable;

typedef struct GridFlushTable {
    s32 inDuration;
    s32 waitDuration;
    s32 outDuration;
} GridFlushTable;

typedef struct GridColorTargetTable {
    s32 divisor;
    s32 alpha;
    s32 blue;
    s32 green;
    s32 red;
} GridColorTargetTable;

typedef char GridAngleTable_size_must_be_0x0C[(sizeof(GridAngleTable) == 0x0C) ? 1 : -1];
typedef char GridFlushTable_size_must_be_0x0C[(sizeof(GridFlushTable) == 0x0C) ? 1 : -1];
typedef char GridColorTargetTable_size_must_be_0x14[(sizeof(GridColorTargetTable) == 0x14) ? 1 : -1];

#define GRID_STATUS_ASSERT_FIELD(type, field, expectedOffset) \
    typedef char type##_##field##_offset[(unsigned long)&((type *)0)->field == (expectedOffset) ? 1 : -1]; \
    typedef char type##_##field##_size[(sizeof(((type *)0)->field) == sizeof(s32)) ? 1 : -1]

GRID_STATUS_ASSERT_FIELD(GridAngleTable, divisor, 0x00);
GRID_STATUS_ASSERT_FIELD(GridAngleTable, mirrored, 0x04);
GRID_STATUS_ASSERT_FIELD(GridAngleTable, cycleDivisor, 0x08);

GRID_STATUS_ASSERT_FIELD(GridFlushTable, inDuration, 0x00);
GRID_STATUS_ASSERT_FIELD(GridFlushTable, waitDuration, 0x04);
GRID_STATUS_ASSERT_FIELD(GridFlushTable, outDuration, 0x08);

GRID_STATUS_ASSERT_FIELD(GridColorTargetTable, divisor, 0x00);
GRID_STATUS_ASSERT_FIELD(GridColorTargetTable, alpha, 0x04);
GRID_STATUS_ASSERT_FIELD(GridColorTargetTable, blue, 0x08);
GRID_STATUS_ASSERT_FIELD(GridColorTargetTable, green, 0x0C);
GRID_STATUS_ASSERT_FIELD(GridColorTargetTable, red, 0x10);

#undef GRID_STATUS_ASSERT_FIELD

#endif
