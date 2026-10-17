#ifndef EEREGS_H
#define EEREGS_H

#include "common.h"

/*
 * Subset of the Sony SDK eeregs.h: the EE and GS registers retail code
 * touches, by their SDK names.
 */
#define T0_COUNT ((volatile u32 *)0x10000000)
#define T0_MODE ((volatile u32 *)0x10000010)

#define IPU_CMD ((volatile u32 *)0x10002000)
#define IPU_CTRL ((volatile u32 *)0x10002010)

/* D1 is the VIF1 DMA channel, D3 fromIPU and D4 toIPU. */
#define D1_CHCR ((volatile u32 *)0x10009000)
#define D3_CHCR ((volatile u32 *)0x1000B000)
#define D3_MADR ((volatile u32 *)0x1000B010)
#define D3_QWC ((volatile u32 *)0x1000B020)
#define D4_CHCR ((volatile u32 *)0x1000B400)
#define D4_MADR ((volatile u32 *)0x1000B410)
#define D4_QWC ((volatile u32 *)0x1000B420)
#define D4_TADR ((volatile u32 *)0x1000B430)
#define D_ENABLER ((volatile u32 *)0x1000F520)
#define D_ENABLEW ((volatile u32 *)0x1000F590)

/* GS privileged registers. */
#define GS_CSR ((volatile u64 *)0x12001000)
#define GS_BUSDIR ((volatile u64 *)0x12001040)

/* Register access: a plain volatile access, unless the build supplies its
 * own. */
#ifndef HW_READ
#define HW_READ(reg) (*(reg))
#define HW_WRITE(reg, value) (*(reg) = (value))
#endif

#endif /* EEREGS_H */
