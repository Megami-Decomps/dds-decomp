#ifndef SDF_CHIP_PAGE_LINK_VIEW_H
#define SDF_CHIP_PAGE_LINK_VIEW_H

#include "sdf.h"

/* SdfChipPage.next holds the link for the page's current list membership. */
typedef union SdfChipPageLinkView {
    SdfChipPage *nextInFreePagePool;
    SdfChipPage *nextInSizeClassAvailableList;
} SdfChipPageLinkView;

typedef char SdfChipPageLinkView_size_must_match_next[
    (sizeof(SdfChipPageLinkView) == sizeof(((SdfChipPage *)0)->next)) ? 1 : -1];

#define SDF_CHIP_PAGE_LINK_VIEW(page) ((SdfChipPageLinkView *)(page))

#endif
