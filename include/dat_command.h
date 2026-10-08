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

/* Named values used by the costMode byte; other values remain possible. */
#define DAT_COMMAND_COST_MODE_HP 1
#define DAT_COMMAND_COST_MODE_MP 2

/* Named values used by attribute.parts.kind to interpret its mask. */
#define DAT_COMMAND_ATTRIBUTE_KIND_ELEMENT_MASK 1
#define DAT_COMMAND_ATTRIBUTE_KIND_FLAG_MASK 2
#define DAT_COMMAND_ATTRIBUTE_KIND_RANDOM_ELEMENT_MASK 3

typedef struct DatCommandRecord {
    u8 flags;                     /* 0x00 */
    u8 unk_01;
    u8 kind;                      /* 0x02 */
    u8 costMode;                  /* 0x03 */
    u16 costPercentage;            /* 0x04 */
    u16 costBase;                  /* 0x06 */
    u8 targetType;                 /* 0x08; SKILL.TBL target_type */
    u8 options;                    /* 0x09 */
    u8 unk_0A[2];
    u16 restriction;               /* 0x0C */
    u8 unk_0E[3];
    u8 stat11;
    u8 unk_12[2];
    u8 rangeMin;                  /* 0x14 */
    u8 rangeMax;                  /* 0x15 */
    u16 primaryLimitKind;          /* 0x16 */
    s16 stat18;
    u16 secondaryLimitKind;        /* 0x1A */
    s16 stat1C;
    u8 unk_1E[4];
    u16 unk22;
    DatCommandAttribute attribute; /* 0x24 */
    s32 requirementBits;           /* 0x28 */
    u8 unk_2C;
    u8 stat2D;
    u16 unk2E;
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

/* Two-byte command selectors; DDS2 func_001ABB10 reads the signed kind at +1. */
typedef struct DatCommandSelector {
    s8 stat;
    s8 kind;
} DatCommandSelector;

typedef char DatCommandSelectorSizeCheck[sizeof(DatCommandSelector) == 2 ? 1 : -1];

extern DatCommandRecord *datCommandRecords;
extern DatCommandSelector *datCommandSelectors;

#endif /* DAT_COMMAND_H */
