#ifndef BTL_RESOURCE_NAME_H
#define BTL_RESOURCE_NAME_H

#include "common.h"
#include "btl_resource_selection.h"

struct BtlResourceNameRecord;

struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension);
void btlSetResourceNameHeaderPairAlternate(struct BtlResourceNameRecord *record, s32 firstWord, s32 secondWord);
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name);
void btlFormatResourceNameWithPrefix(struct BtlResourceNameRecord *record, char *output);
void btlFormatResourceNameWithoutPrefix(struct BtlResourceNameRecord *record, char *output);

/* Poll the overwrite prompt; an absent file also permits the write immediately. */
u32 btlPollResourceNameOverwrite(struct BtlResourceNameRecord *record,
                                 const char *directoryPath);

u32 btlGetResourceNameSelectionStatus(const struct BtlResourceNameRecord *record);
void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength);

#ifdef VERSION_DDS1
void func_001FC2E8(void *allocation);
void func_001FC300(struct BtlResourceNameRecord *record);
#else
void func_0020E368(void *allocation);
void func_0020E380(struct BtlResourceNameRecord *record);
#endif

#endif /* BTL_RESOURCE_NAME_H */
