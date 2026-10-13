#ifndef EFF_BILL_RESOURCE_INIT_H
#define EFF_BILL_RESOURCE_INIT_H

#include "common.h"

/* Shared 0x0C billboard-resource row. The first word has title-specific use;
 * only the following kind and scale fields are named in this common view. */
typedef struct EffBillResourceInit {
    u32 word00;         /* 0x00, title-specific */
    s16 billboardKind;  /* 0x04 */
    u16 pad06;          /* 0x06 */
    f32 scale;          /* 0x08 */
} EffBillResourceInit;

typedef char EffBillResourceInit_size_must_be_0x0C[
    (sizeof(EffBillResourceInit) == 0x0C) ? 1 : -1];
typedef char EffBillResourceInit_billboardKind_at_4[
    ((u32)&((EffBillResourceInit *)0)->billboardKind == 4) ? 1 : -1];
typedef char EffBillResourceInit_scale_at_8[
    ((u32)&((EffBillResourceInit *)0)->scale == 8) ? 1 : -1];

#endif /* EFF_BILL_RESOURCE_INIT_H */
