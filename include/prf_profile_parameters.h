#ifndef PRF_PROFILE_PARAMETERS_H
#define PRF_PROFILE_PARAMETERS_H

#include "common.h"

/* Each profile's parameters, by profile id: flags, two values, the level it
 * requires, a capacity and its skill list. DDS2's records add six parameter
 * bytes before the skills and a reserved halfword after them. */
#define PRF_SKILL_LIST_ENTRY_COUNT 8

#ifdef VERSION_DDS1
/* Native table stride in u16 entries; the eight skills begin at row+0xC. */
#define PRF_SKILL_TABLE_STRIDE 14
#define PRF_PROFILE_COUNT 0x60

typedef struct Dds1ProfileParamRecord {
    u32 flags;
    u8 mantraDisplayParameter;
    u8 value05;
    u16 requiredLevel;
    u32 capacity;
    u16 skills[PRF_SKILL_LIST_ENTRY_COUNT];
} Dds1ProfileParamRecord;

typedef char Dds1ProfileParamRecordLayoutCheck[
    (sizeof(Dds1ProfileParamRecord) == 0x1C &&
     PRF_SKILL_TABLE_STRIDE * sizeof(u16) == sizeof(Dds1ProfileParamRecord) &&
     (u32)&((Dds1ProfileParamRecord *)0)->flags == 0 &&
     (u32)&((Dds1ProfileParamRecord *)0)->mantraDisplayParameter == 4 &&
     (u32)&((Dds1ProfileParamRecord *)0)->value05 == 5 &&
     (u32)&((Dds1ProfileParamRecord *)0)->requiredLevel == 6 &&
     (u32)&((Dds1ProfileParamRecord *)0)->capacity == 8 &&
     (u32)&((Dds1ProfileParamRecord *)0)->skills == 0xC) ? 1 : -1];

extern Dds1ProfileParamRecord D_003907B0[PRF_PROFILE_COUNT];
typedef char Dds1ProfileParamTableExtentCheck[
    (sizeof(D_003907B0) == 0xA80) ? 1 : -1];
#else
#define PRF_SKILL_TABLE_STRIDE 18
#define PRF_PROFILE_PARAM_BYTES 36
#define PRF_PROFILE_COUNT 0xB0

/* Native profile parameter data: 176 complete 0x24-byte records. */
typedef struct PrfProfileParameters {
    u32 flags;
    u8 mantraDisplayParameter;
    u8 value05;
    u16 requiredLevel;
    u32 capacity;
    u8 parameters[6];
    u16 skills[8];
    u16 reserved22;
} PrfProfileParameters;

typedef char PrfProfileParametersLayoutCheck[
    (sizeof(PrfProfileParameters) == 0x24 &&
     (u32)&((PrfProfileParameters *)0)->flags == 0 &&
     (u32)&((PrfProfileParameters *)0)->mantraDisplayParameter == 4 &&
     (u32)&((PrfProfileParameters *)0)->value05 == 5 &&
     (u32)&((PrfProfileParameters *)0)->requiredLevel == 6 &&
     (u32)&((PrfProfileParameters *)0)->capacity == 8 &&
     (u32)&((PrfProfileParameters *)0)->parameters == 0xC &&
     (u32)&((PrfProfileParameters *)0)->skills == 0x12 &&
     (u32)&((PrfProfileParameters *)0)->reserved22 == 0x22) ? 1 : -1];

extern PrfProfileParameters D_00401320[PRF_PROFILE_COUNT];
typedef char PrfProfileParametersTableExtentCheck[
    (sizeof(D_00401320) == 0x18C0) ? 1 : -1];
#endif

#endif /* PRF_PROFILE_PARAMETERS_H */
