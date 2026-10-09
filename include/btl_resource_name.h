#ifndef BTL_RESOURCE_NAME_H
#define BTL_RESOURCE_NAME_H

#include "common.h"

struct BtlResourceNameRecord;

struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension);
void btlSetResourceNameHeaderPairAlternate(struct BtlResourceNameRecord *record, s32 firstWord, s32 secondWord);
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name);
void btlFormatResourceNameWithPrefix(struct BtlResourceNameRecord *record, char *output);
void btlFormatResourceNameWithoutPrefix(struct BtlResourceNameRecord *record, char *output);

#ifdef VERSION_DDS1
void func_001FC2E8(void *allocation);
void func_001FC300(struct BtlResourceNameRecord *record);
u32 func_001FC730(struct BtlResourceNameRecord *record);
void func_001FC7D0(struct BtlResourceNameRecord *record, u32 value);
u32 func_001FC7D8(struct BtlResourceNameRecord *record, u32 *result);
#else
void func_0020E368(void *allocation);
void func_0020E380(struct BtlResourceNameRecord *record);
void func_0020E850(struct BtlResourceNameRecord *record, u32 value);
u32 func_0020E7B0(struct BtlResourceNameRecord *record);
u32 func_0020E858(struct BtlResourceNameRecord *record, u32 *result);
#endif

#endif /* BTL_RESOURCE_NAME_H */
