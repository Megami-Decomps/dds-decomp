#ifndef PRF_REQUIREMENT_H
#define PRF_REQUIREMENT_H
#include "common.h"
struct DatPartyRecord;

#ifdef VERSION_DDS1
/* DDS1's 0x54-byte requirement row is distinct from the DDS2 rule record.
 * Word 0 is read as a value by the provider, but its broader role is unknown. */
typedef struct PrfDds1RequirementSlot {
    s32 status;
    u8 unknown04[4];
    s32 sourceValue;
    u8 profileIds[8];
} PrfDds1RequirementSlot;

typedef struct PrfDds1RequirementRecord {
    u32 unknown00;
    u32 flags04;
    u8 pad08[0x10];
    u32 flags18;
    u8 pad1C[0x10];
    PrfDds1RequirementSlot slots[2];
} PrfDds1RequirementRecord;

extern PrfDds1RequirementRecord D_00391230[];
PrfDds1RequirementRecord *prfReqGetEntryRecord(u16 index);

typedef char PrfDds1RequirementSlot_size[(sizeof(PrfDds1RequirementSlot) == 0x14) ? 1 : -1];
typedef char PrfDds1RequirementSlot_status[((u32)&((PrfDds1RequirementSlot *)0)->status == 0) ? 1 : -1];
typedef char PrfDds1RequirementSlot_value[((u32)&((PrfDds1RequirementSlot *)0)->sourceValue == 8) ? 1 : -1];
typedef char PrfDds1RequirementSlot_ids[((u32)&((PrfDds1RequirementSlot *)0)->profileIds == 0xC) ? 1 : -1];
typedef char PrfDds1RequirementRecord_size[(sizeof(PrfDds1RequirementRecord) == 0x54) ? 1 : -1];
typedef char PrfDds1RequirementRecord_flags04[((u32)&((PrfDds1RequirementRecord *)0)->flags04 == 4) ? 1 : -1];
typedef char PrfDds1RequirementRecord_flags18[((u32)&((PrfDds1RequirementRecord *)0)->flags18 == 0x18) ? 1 : -1];
typedef char PrfDds1RequirementRecord_slots[((u32)&((PrfDds1RequirementRecord *)0)->slots == 0x2C) ? 1 : -1];
#endif

#ifdef VERSION_DDS2
/* Serialized rule: operation-specific byte thresholds, word minimum and ID list. */
typedef struct PrfRequirementOperand {
    u32 operation;
    u8 levelThreshold;
    u8 profileThreshold;
    u8 reserved06[2];
    u32 minimum;
    u8 profileIds[8];
} PrfRequirementOperand;
typedef struct PrfRequirementRecord {
    u32 state;
    PrfRequirementOperand rules[2];
} PrfRequirementRecord;
extern PrfRequirementRecord D_00402BE0[176];
PrfRequirementRecord *scrGetEntryDescriptor(u16);
s32 func_00315C68(u32, u32, struct DatPartyRecord *, u32, u32 *);
s32 func_00315FA0(u32, struct DatPartyRecord *, u16);
typedef char PrfRequirementRecord_size[(sizeof(PrfRequirementRecord)==0x2C)?1:-1];
typedef char PrfRequirementRecord_rules[((u32)&((PrfRequirementRecord*)0)->rules==4)?1:-1];
typedef char PrfRequirementTable_extent[(sizeof(D_00402BE0)==0x1E40)?1:-1];
typedef char PrfRequirementOperand_size[(sizeof(PrfRequirementOperand)==0x14)?1:-1];
typedef char PrfRequirementOperand_operation[((u32)&((PrfRequirementOperand*)0)->operation==0)?1:-1];
typedef char PrfRequirementOperand_level[((u32)&((PrfRequirementOperand*)0)->levelThreshold==4)?1:-1];
typedef char PrfRequirementOperand_profile[((u32)&((PrfRequirementOperand*)0)->profileThreshold==5)?1:-1];
typedef char PrfRequirementOperand_reserved[((u32)&((PrfRequirementOperand*)0)->reserved06==6)?1:-1];
typedef char PrfRequirementOperand_minimum[((u32)&((PrfRequirementOperand*)0)->minimum==8)?1:-1];
typedef char PrfRequirementOperand_ids[((u32)&((PrfRequirementOperand*)0)->profileIds==0xC)?1:-1];
#endif
#endif
