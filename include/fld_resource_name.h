#ifndef FLD_RESOURCE_NAME_H
#define FLD_RESOURCE_NAME_H

#include "common.h"

typedef struct FldResourceName {
    u32 unk00;
    const char *name;
} FldResourceName;

typedef char FldResourceName_size_must_be_8[
    (sizeof(FldResourceName) == 8) ? 1 : -1];
typedef char FldResourceName_name_offset_must_be_4[
    ((u32)&((FldResourceName *)0)->name == 4) ? 1 : -1];

#endif
