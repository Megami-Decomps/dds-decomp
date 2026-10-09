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

/* Stored entry categories, including synthetic GENERAL entries; not scan masks. */
enum BtlResourceEntryCategory {
    BTL_RESOURCE_ENTRY_CATEGORY_TMX = 0x01,
    BTL_RESOURCE_ENTRY_CATEGORY_P2A = 0x02,
    BTL_RESOURCE_ENTRY_CATEGORY_PB = 0x04,
    BTL_RESOURCE_ENTRY_CATEGORY_GENERAL = 0x08,
    BTL_RESOURCE_ENTRY_CATEGORY_EPL = 0x10,
    BTL_RESOURCE_ENTRY_CATEGORY_EP = 0x20,
    BTL_RESOURCE_ENTRY_CATEGORY_TLP = 0x40,
    BTL_RESOURCE_ENTRY_CATEGORY_F2 = 0x80,
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
s32 btlUpdateAndDrawResourceBrowser(struct BtlResourceDescriptor *descriptor);
s32 btlFormatSelectedResourceName(struct BtlResourceDescriptor *descriptor,
                                  char *output);
s32 btlTrimResourceName(struct BtlResourceDescriptor *descriptor,
                        char *output);
u32 btlGetResourcePathVariant(struct BtlResourceDescriptor *descriptor);
void btlDrawResourcePreview(struct BtlResourceDescriptor *descriptor);

void btlSetResourceBrowserPosition(struct BtlResourceDescriptor *descriptor, s32 x, s32 y);
u32 btlGetResourceBrowserSelectionStatus(const struct BtlResourceDescriptor *descriptor);

#endif
