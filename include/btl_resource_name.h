#ifndef BTL_RESOURCE_NAME_H
#define BTL_RESOURCE_NAME_H

#include "common.h"
#include "btl_resource_selection.h"

typedef struct BtlResourceNameRecord {
    s32 x; /* UI origin for the name and overwrite prompts. */
    s32 y;
    u32 selectionStatus; /* Pending, accepted, or canceled. */
    s32 selection; /* Keyboard selection or overwrite yes/no choice. */
    s32 nameLength;
    u32 maximumNameLength;
    s32 overwritePromptActive;
    char extension[5];
    char resourceName[0x17];
} BtlResourceNameRecord;

typedef char BtlResourceNameRecordSizeCheck[
    (sizeof(BtlResourceNameRecord) == 0x38) ? 1 : -1];
typedef char BtlResourceNameRecordExtensionOffsetCheck[
    ((u32)&((BtlResourceNameRecord *)0)->extension == 0x1C) ? 1 : -1];
typedef char BtlResourceNameRecordNameOffsetCheck[
    ((u32)&((BtlResourceNameRecord *)0)->resourceName == 0x21) ? 1 : -1];

struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension);
void btlSetResourceNamePosition(struct BtlResourceNameRecord *record, s32 x, s32 y);
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name);
void btlFormatResourceNameWithExtension(struct BtlResourceNameRecord *record, char *output);
void btlFormatResourceNameWithoutExtension(struct BtlResourceNameRecord *record, char *output);

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
