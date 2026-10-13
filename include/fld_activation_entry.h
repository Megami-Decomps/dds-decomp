#ifndef FLD_ACTIVATION_ENTRY_H
#define FLD_ACTIVATION_ENTRY_H

#include "common.h"

typedef struct FldActivationEntry {
    s32 kind;          /* 0x00 */
    s16 parameter;     /* 0x04 */
    u8 opaque06[0x0A];
} FldActivationEntry;

typedef char FldActivationEntry_size[(sizeof(FldActivationEntry) == 0x10) ? 1 : -1];

#endif
