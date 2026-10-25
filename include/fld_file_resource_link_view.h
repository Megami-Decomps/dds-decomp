#ifndef FLD_FILE_RESOURCE_LINK_VIEW_H
#define FLD_FILE_RESOURCE_LINK_VIEW_H

#include "fld_resource_name.h"

/* Read-only view of the borrowed linked-name pointer at FldFileResource +0x14. */
typedef struct FldFileResourceLinkView {
    u8 opaque00[0x14];
    const FldResourceName *linkedResourceName;
} FldFileResourceLinkView;

typedef char FldFileResourceLinkView_link_offset_check[
    ((u32)&((FldFileResourceLinkView *)0)->linkedResourceName == 0x14) ? 1 : -1];
typedef char FldFileResourceLinkView_size_check[
    (sizeof(FldFileResourceLinkView) == 0x18) ? 1 : -1];

#endif
