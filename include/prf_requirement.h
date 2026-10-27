#ifndef PRF_REQUIREMENT_H
#define PRF_REQUIREMENT_H
#include "common.h"
struct datUnitWork_t;

#ifdef VERSION_DDS1
/* Evaluate either serialized rule set against the borrowed party record. */
s32 prfCheckRequirementRules(u32 state, struct datUnitWork_t *unit,
                             u16 requirementId);

/* DDS1's 0x54-byte requirement row is distinct from the DDS2 rule record.
 * Bits 0x8/0x10 select state 0/1 rule aggregation; other bits remain unknown.
 * State 0 reads two 0x14-byte slots starting at stateZeroFirstOperation. */
typedef struct PrfDds1RequirementSlot {
    s32 operation;
    u8 levelThreshold;
    u8 profileThreshold;
    u8 reserved06[2];
    s32 minimum;
    u8 profileIds[8];
} PrfDds1RequirementSlot;

typedef struct PrfDds1RequirementRecord {
    u32 stateFlags;
    u32 stateZeroFirstOperation;
    u8 pad08[0x10];
    u32 stateZeroSecondOperation;
    u8 pad1C[0x10];
    PrfDds1RequirementSlot slots[2];
} PrfDds1RequirementRecord;

extern PrfDds1RequirementRecord D_00391230[];
PrfDds1RequirementRecord *prfReqGetEntryRecord(u16 index);

typedef char PrfDds1RequirementSlot_size[(sizeof(PrfDds1RequirementSlot) == 0x14) ? 1 : -1];
typedef char PrfDds1RequirementSlot_status[((u32)&((PrfDds1RequirementSlot *)0)->operation == 0) ? 1 : -1];
typedef char PrfDds1RequirementSlot_level[((u32)&((PrfDds1RequirementSlot *)0)->levelThreshold == 4) ? 1 : -1];
typedef char PrfDds1RequirementSlot_profile[((u32)&((PrfDds1RequirementSlot *)0)->profileThreshold == 5) ? 1 : -1];
typedef char PrfDds1RequirementSlot_minimum[((u32)&((PrfDds1RequirementSlot *)0)->minimum == 8) ? 1 : -1];
typedef char PrfDds1RequirementSlot_ids[((u32)&((PrfDds1RequirementSlot *)0)->profileIds == 0xC) ? 1 : -1];
typedef char PrfDds1RequirementRecord_size[(sizeof(PrfDds1RequirementRecord) == 0x54) ? 1 : -1];
typedef char PrfDds1RequirementRecord_flags04[((u32)&((PrfDds1RequirementRecord *)0)->stateZeroFirstOperation == 4) ? 1 : -1];
typedef char PrfDds1RequirementRecord_flags18[((u32)&((PrfDds1RequirementRecord *)0)->stateZeroSecondOperation == 0x18) ? 1 : -1];
typedef char PrfDds1RequirementRecord_slots[((u32)&((PrfDds1RequirementRecord *)0)->slots == 0x2C) ? 1 : -1];
#endif

#ifdef VERSION_DDS2
/* Serialized rule: operation-specific byte thresholds, word minimum and ID list. */
typedef enum PrfRequirementOperationFlags {
    PRF_REQUIREMENT_OP_ALL_PROFILES = 0x01,
    PRF_REQUIREMENT_OP_PROFILE_COUNT = 0x02,
    PRF_REQUIREMENT_OP_PROFILE_VALUE_COUNT = 0x04,
    PRF_REQUIREMENT_OP_PROFILES_IN_PARTY = 0x08,
    PRF_REQUIREMENT_OP_UNIT_LEVEL = 0x10,
    PRF_REQUIREMENT_OP_GLOBAL_MINIMUM = 0x20
} PrfRequirementOperationFlags;

typedef struct PrfRequirementOperand {
    PrfRequirementOperationFlags operation;
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
s32 prfReqEvaluateRules(u32, u32, struct datUnitWork_t *, u32, u32 *);
s32 prfCheckRequirementRules(u32, struct datUnitWork_t *, u16);
typedef char PrfRequirementRecord_size[(sizeof(PrfRequirementRecord)==0x2C)?1:-1];
typedef char PrfRequirementRecord_rules[((u32)&((PrfRequirementRecord*)0)->rules==4)?1:-1];
typedef char PrfRequirementTable_extent[(sizeof(D_00402BE0)==0x1E40)?1:-1];
typedef char PrfRequirementOperand_size[(sizeof(PrfRequirementOperand)==0x14)?1:-1];
typedef char PrfRequirementOperationFlags_size[(sizeof(PrfRequirementOperationFlags)==4)?1:-1];
typedef char PrfRequirementOperand_operation[((u32)&((PrfRequirementOperand*)0)->operation==0)?1:-1];
typedef char PrfRequirementOperand_level[((u32)&((PrfRequirementOperand*)0)->levelThreshold==4)?1:-1];
typedef char PrfRequirementOperand_profile[((u32)&((PrfRequirementOperand*)0)->profileThreshold==5)?1:-1];
typedef char PrfRequirementOperand_reserved[((u32)&((PrfRequirementOperand*)0)->reserved06==6)?1:-1];
typedef char PrfRequirementOperand_minimum[((u32)&((PrfRequirementOperand*)0)->minimum==8)?1:-1];
typedef char PrfRequirementOperand_ids[((u32)&((PrfRequirementOperand*)0)->profileIds==0xC)?1:-1];
#endif
#endif
