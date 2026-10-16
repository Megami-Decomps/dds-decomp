#ifndef FLD_SCRIPT_RESOURCE_H
#define FLD_SCRIPT_RESOURCE_H

#include "sdf_model.h"

struct MotionTable;

typedef struct FldScriptResource {
    u8 opaque00[0x10];
    SdfItemListRef itemList;
    struct MotionTable *motionTable;
} FldScriptResource;

typedef char FldScriptResourceItemListOffsetCheck[
    ((u32)&((FldScriptResource *)0)->itemList == 0x10) ? 1 : -1];
typedef char FldScriptResourceMotionTableOffsetCheck[
    ((u32)&((FldScriptResource *)0)->motionTable == 0x20) ? 1 : -1];
typedef char FldScriptResourceSizeCheck[(sizeof(FldScriptResource) == 0x24) ? 1 : -1];

#endif
