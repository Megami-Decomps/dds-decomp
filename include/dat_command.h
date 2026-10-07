#ifndef DAT_COMMAND_H
#define DAT_COMMAND_H

#include "common.h"

/* DDS2 SKILL.TBL command rows. The resource contains 544 records. */
typedef union DatCommandAttribute {
    u32 bits;
    struct {
        u8 kind;
        u8 hitChance;
        u16 flagMask;
    } parts;
} DatCommandAttribute;

typedef struct DatCommandRecord {
    u8 flags;                     /* 0x00 */
    u8 unk_01;
    u8 kind;                      /* 0x02 */
    u8 costMode;                  /* 0x03 */
    u16 costPercentage;            /* 0x04 */
    u16 costBase;                  /* 0x06 */
    u8 unk_08;
    u8 options;                    /* 0x09 */
    u8 unk_0A[2];
    u16 restriction;               /* 0x0C */
    u8 unk_0E[3];
    u8 stat11;
    u8 unk_12[4];
    u16 primaryLimitKind;          /* 0x16 */
    s16 stat18;
    u16 secondaryLimitKind;        /* 0x1A */
    s16 stat1C;
    u8 unk_1E[6];
    DatCommandAttribute attribute; /* 0x24 */
    s32 requirementBits;           /* 0x28 */
    u8 unk_2C;
    u8 stat2D;
    u8 unk_2E[2];
    s32 unk30;
    s16 stat34;
    s16 stat36;
} DatCommandRecord;

typedef char DatCommandAttributeSizeCheck[sizeof(DatCommandAttribute) == 4 ? 1 : -1];
typedef char DatCommandRecordSizeCheck[sizeof(DatCommandRecord) == 0x38 ? 1 : -1];

typedef char DatCommandCostOffsetCheck[((u32)&((DatCommandRecord *)0)->costMode == 3) ? 1 : -1];
typedef char DatCommandAttributeOffsetCheck[((u32)&((DatCommandRecord *)0)->attribute == 0x24) ? 1 : -1];
typedef char DatCommandRequirementOffsetCheck[((u32)&((DatCommandRecord *)0)->requirementBits == 0x28) ? 1 : -1];
typedef char DatCommandLastStatOffsetCheck[((u32)&((DatCommandRecord *)0)->stat36 == 0x36) ? 1 : -1];

extern DatCommandRecord *datCommandRecords;

#endif /* DAT_COMMAND_H */
