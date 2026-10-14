#ifndef EFF_POINT_SET_TABLE_H
#define EFF_POINT_SET_TABLE_H

#include "common.h"

struct EffPointSet;

typedef struct EffPointSetRow {
    struct EffPointSet *set; /* 0x00 */
    s32 key;                 /* 0x04 */
    u32 color;               /* 0x08 */
    f32 angle;               /* 0x0C */
} EffPointSetRow;

typedef struct EffPointSetTable {
    EffPointSetRow *rows;
} EffPointSetTable;

typedef char EffPointSetRowSizeCheck[(sizeof(EffPointSetRow) == 0x10) ? 1 : -1];
typedef char EffPointSetRowKeyOffsetCheck[
    ((u32)&((EffPointSetRow *)0)->key == 0x04) ? 1 : -1];
typedef char EffPointSetRowColorOffsetCheck[
    ((u32)&((EffPointSetRow *)0)->color == 0x08) ? 1 : -1];
typedef char EffPointSetTableSizeCheck[(sizeof(EffPointSetTable) == 0x04) ? 1 : -1];

#endif
