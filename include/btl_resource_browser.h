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

struct BtlResourceEntry;
struct BtlResourceEntryList;
struct SdfTex;

/* Browser viewport/selection and texture ownership; the entry list is borrowed. */
typedef struct BtlResourceDescriptor {
    s32 originX;            /* 0x00: horizontal browser origin */
    s32 originY;            /* 0x04: vertical browser origin */
    u32 selectionStatus;    /* 0x08: pending, accepted, or canceled */
    s32 entryCount;         /* 0x0C */
    u32 word10;             /* 0x10 */
    u32 selectedIndex;      /* 0x14 */
    u32 visibleIndex;       /* 0x18 */
    u32 previewActive;      /* 0x1C */
    u32 repeatDelay;        /* 0x20 */
    u32 drawSurfaceIndex;   /* 0x24 */
    u32 borderColor;        /* 0x28: line-strip color */
    u32 fillColor;          /* 0x2C: sprite-fill color */
    struct BtlResourceEntry *firstVisibleEntry; /* 0x30 */
    struct BtlResourceEntry *selectedEntry; /* 0x34 */
    struct BtlResourceEntry *cachedEntry; /* 0x38: last entry whose preview was updated */
    struct SdfTex *texture;        /* 0x3C: owned or borrowed preview texture */
    s32 textureCategory;    /* 0x40: TMX is owned; GENERAL is borrowed. */
    struct BtlResourceEntryList *entryList; /* 0x44 */
} BtlResourceDescriptor;

typedef char BtlResourceDescriptor_size_must_be_0x48[
    (sizeof(BtlResourceDescriptor) == 0x48) ? 1 : -1];

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
