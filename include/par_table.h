#ifndef PAR_TABLE_H
#define PAR_TABLE_H

#include "eff.h"
#include "sdf_resource.h"

typedef struct ParSlot {
    u128 *points;
    u16 pointCount;
    u8 pad06[2];
    u32 color;
    f32 billboardScale;
} ParSlot;

typedef struct ParTable {
    u16 slotCount;
    u16 pointCapacity;
    ParSlot *slots;
    BillObj **billboardRef;
    struct SdfMemBlock *allocation;
} ParTable;

typedef char ParSlot_size_must_be_0x10[(sizeof(ParSlot) == 0x10) ? 1 : -1];
typedef char ParTable_size_must_be_0x10[(sizeof(ParTable) == 0x10) ? 1 : -1];

void effParReleaseNodeResource(ParTable *table);

#endif
