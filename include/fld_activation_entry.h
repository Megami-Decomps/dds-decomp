#ifndef FLD_ACTIVATION_ENTRY_H
#define FLD_ACTIVATION_ENTRY_H

#include "common.h"

/* One activation record: script selectors expose these same five members. */
typedef struct FldActivationEntry {
    s32 kind;               /* 0x00: activation dispatcher selector. */
    s16 parameter;          /* 0x04: activation parameter. */
    s16 selectedValue2;     /* 0x06: script command selector 2. */
    s32 selectedValue3;     /* 0x08: script command selector 3. */
    s32 selectedValue4;     /* 0x0C: script command selector 4. */
} FldActivationEntry;

typedef char FldActivationEntry_size[(sizeof(FldActivationEntry) == 0x10) ? 1 : -1];
typedef char FldActivationEntry_selectedValue2_at_6[
    ((u32)&((FldActivationEntry *)0)->selectedValue2 == 0x06) ? 1 : -1];
typedef char FldActivationEntry_selectedValue3_at_8[
    ((u32)&((FldActivationEntry *)0)->selectedValue3 == 0x08) ? 1 : -1];
typedef char FldActivationEntry_selectedValue4_at_C[
    ((u32)&((FldActivationEntry *)0)->selectedValue4 == 0x0C) ? 1 : -1];

#endif
