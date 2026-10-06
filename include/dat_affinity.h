#ifndef DAT_AFFINITY_H
#define DAT_AFFINITY_H

#include "common.h"

/* One 16-byte affinity (combination command) record; datAffinityRecords is indexed
 * from command DAT_AFFINITY_FIRST_COMMAND. Each of the three participant requirements is an
 * affinity kind word (0x20000000 | element bit, 0x40000000 override) or -1 when unused. */
#define DAT_AFFINITY_FIRST_COMMAND 0x1AB

typedef struct DatAffinityRecord {
    s32 requirements[3];
    s8 slotCost;         /* 0x0C: scene slots the command needs */
    u8 pad0D;
    u16 flags;           /* 0x0E: bit 1 selects partners by the alternate rule */
} DatAffinityRecord;

typedef char DatAffinityRecordSizeCheck[sizeof(DatAffinityRecord) == 0x10 ? 1 : -1];

extern DatAffinityRecord *datAffinityRecords;

#endif /* DAT_AFFINITY_H */
