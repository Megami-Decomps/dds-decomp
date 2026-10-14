#ifndef SDF_KERNEL_THREAD_PARAM_H
#define SDF_KERNEL_THREAD_PARAM_H

#include "common.h"

/* Native EE CreateThread descriptor. The observed sound-worker setup reads or
 * writes the first six slots; retain the remaining descriptor bytes opaque. */
typedef struct SdfKernelThreadParam {
    s32 status;             /* 0x00 */
    void (*entry)(void);    /* 0x04 */
    void *stack;            /* 0x08 */
    s32 stackSize;          /* 0x0C */
    void *gp;               /* 0x10 */
    s32 priority;           /* 0x14 */
    u8 opaque18[0x18];      /* 0x18 */
} SdfKernelThreadParam;

typedef char SdfKernelThreadParam_size_must_be_0x30[
    (sizeof(SdfKernelThreadParam) == 0x30) ? 1 : -1];
typedef char SdfKernelThreadParam_entry_offset_must_be_0x04[
    ((u32)&((SdfKernelThreadParam *)0)->entry == 0x04) ? 1 : -1];
typedef char SdfKernelThreadParam_stack_offset_must_be_0x08[
    ((u32)&((SdfKernelThreadParam *)0)->stack == 0x08) ? 1 : -1];
typedef char SdfKernelThreadParam_stackSize_offset_must_be_0x0C[
    ((u32)&((SdfKernelThreadParam *)0)->stackSize == 0x0C) ? 1 : -1];
typedef char SdfKernelThreadParam_gp_offset_must_be_0x10[
    ((u32)&((SdfKernelThreadParam *)0)->gp == 0x10) ? 1 : -1];
typedef char SdfKernelThreadParam_priority_offset_must_be_0x14[
    ((u32)&((SdfKernelThreadParam *)0)->priority == 0x14) ? 1 : -1];
typedef char SdfKernelThreadParam_opaque18_offset_must_be_0x18[
    ((u32)&((SdfKernelThreadParam *)0)->opaque18 == 0x18) ? 1 : -1];

#endif
