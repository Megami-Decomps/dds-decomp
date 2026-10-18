#ifndef DDS3_NODE_DIAGNOSTIC_ROW_H
#define DDS3_NODE_DIAGNOSTIC_ROW_H

#include "common.h"

/* Row of the world-node diagnostic list table D_003ACD78 (17 rows of 12 bytes, ending in a row whose kind is 0).
 * The task works on a stack copy and records, per row, how many entries it drew (count) and how many text rows
 * they took (rows). */
typedef struct NodeDiagnosticRow {
    const char *label; /* 0x00 */
    s32 kind;          /* 0x04: world object list index (EvtWorldTable.slots[kind]) */
    s16 rows;          /* 0x08 */
    s16 count;         /* 0x0A */
} NodeDiagnosticRow;
typedef char NodeDiagnosticRowSizeCheck[(sizeof(NodeDiagnosticRow) == 0x0C) ? 1 : -1];

#endif
