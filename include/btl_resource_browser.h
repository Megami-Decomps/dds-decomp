#ifndef BTL_RESOURCE_BROWSER_H
#define BTL_RESOURCE_BROWSER_H

#include "common.h"
#include "btl_resource_selection.h"

/* Filters consumed by the directory scan; these are independent of entry categories. */
enum BtlResourceScanFlags {
    BTL_RESOURCE_SCAN_TMX = 0x01,
    BTL_RESOURCE_SCAN_P2A = 0x02,
    BTL_RESOURCE_SCAN_PB = 0x04,
    BTL_RESOURCE_SCAN_GENERAL = 0x08,
    BTL_RESOURCE_SCAN_EPL = 0x10,
    BTL_RESOURCE_SCAN_EP = 0x20,
    BTL_RESOURCE_SCAN_TLP = 0x40,
    BTL_RESOURCE_SCAN_F2 = 0x80,
};

struct BtlResourceEntryList;
struct BtlResourceDescriptor;

s32 btlOpenPfsDebugDirectory(const char *directoryName);
struct BtlResourceEntryList *btlScanDirectory(const char *path, s32 flags);
void btlDestroyEntryList(struct BtlResourceEntryList *list);
void btlAppendEntry(struct BtlResourceEntryList *list, const char *name,
                    s32 category, s32 value, s32 id);

struct BtlResourceDescriptor *
btlCreateResourceDescriptor(struct BtlResourceEntryList *list);
void btlDestroyResourceDescriptor(struct BtlResourceDescriptor *descriptor);
s32 func_001FBA38(struct BtlResourceDescriptor *descriptor);
s32 func_0020DAB8(struct BtlResourceDescriptor *descriptor);
s32 btlFormatSelectedResourceName(struct BtlResourceDescriptor *descriptor,
                                  char *output);
s32 btlTrimResourceName(struct BtlResourceDescriptor *descriptor,
                        char *output);
u32 btlGetResourcePathVariant(struct BtlResourceDescriptor *descriptor);

/* These two word-prefix helpers are shared with another 0x38-byte record. */
void btlSetResourceNameHeaderPair(void *owner, s32 firstWord, s32 secondWord);
u32 func_001FBF48(const void *owner);
u32 func_0020DFC8(const void *owner);

#endif
