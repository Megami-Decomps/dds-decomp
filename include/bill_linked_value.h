#ifndef BILL_LINKED_VALUE_H
#define BILL_LINKED_VALUE_H

#include "common.h"

typedef struct BillLinkedValue {
    s32 opaque00;
    s32 value;
} BillLinkedValue;

typedef char BillLinkedValue_size[
    (sizeof(BillLinkedValue) == 8) ? 1 : -1];
typedef char BillLinkedValue_value_offset[
    ((u32)&((BillLinkedValue *)0)->value == 4) ? 1 : -1];

#endif
