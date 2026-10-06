#ifndef PRF_REQUIREMENT_H
#define PRF_REQUIREMENT_H
#include "common.h"
struct DatPartyRecord;
/* Serialized rule: operation-specific byte thresholds, word minimum and ID list. */
typedef struct PrfRequirementOperand {
    u32 operation;
    u8 levelThreshold;
    u8 profileThreshold;
    u8 reserved06[2];
    u32 minimum;
    u8 profileIds[8];
} PrfRequirementOperand;
#ifdef VERSION_DDS2
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
#endif
typedef char PrfRequirementOperand_size[(sizeof(PrfRequirementOperand)==0x14)?1:-1];
typedef char PrfRequirementOperand_operation[((u32)&((PrfRequirementOperand*)0)->operation==0)?1:-1];
typedef char PrfRequirementOperand_level[((u32)&((PrfRequirementOperand*)0)->levelThreshold==4)?1:-1];
typedef char PrfRequirementOperand_profile[((u32)&((PrfRequirementOperand*)0)->profileThreshold==5)?1:-1];
typedef char PrfRequirementOperand_reserved[((u32)&((PrfRequirementOperand*)0)->reserved06==6)?1:-1];
typedef char PrfRequirementOperand_minimum[((u32)&((PrfRequirementOperand*)0)->minimum==8)?1:-1];
typedef char PrfRequirementOperand_ids[((u32)&((PrfRequirementOperand*)0)->profileIds==0xC)?1:-1];
#endif
