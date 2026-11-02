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

#define GIF_CTRL ((volatile u32 *)0x10003000)
#define GIF_STAT ((volatile u32 *)0x10003020)
#define VIF0_STAT ((volatile u32 *)0x10003800)
#define VIF1_STAT ((volatile u32 *)0x10003C00)

/* D0 and D1 are the VIF DMA channels, D3 fromIPU and D4 toIPU. */
#define D0_CHCR ((volatile u32 *)0x10008000)
#define D0_MADR ((volatile u32 *)0x10008010)
#define D0_QWC ((volatile u32 *)0x10008020)
#define D1_CHCR ((volatile u32 *)0x10009000)
#define D1_MADR ((volatile u32 *)0x10009010)
#define D1_TADR ((volatile u32 *)0x10009030)
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

/* Scratchpad RAM: 16KB of on-chip memory with no cache, mapped at a fixed
 * physical address. sdfVu1.c's point buffer uses one fixed region of it as
 * a second ring alongside corePktPointBuff. A build with no scratchpad
 * defines this as a host buffer's address instead. */
#ifndef SDF_SCRATCHPAD_POINT_BUFFER
#define SDF_SCRATCHPAD_POINT_BUFFER 0x70001000
#endif

/* VU0's data memory, reached from the EE side as ordinary memory while VU0
 * is idle. sdfCoreDraw.c writes transform, light and texture data here
 * directly ahead of a VU0 microprogram run, bypassing the vf-register
 * macros. A build with no VU0 data memory mapped this way defines this to
 * wherever that data lands instead. */
#ifndef SDF_VU0_DATA_MEMORY
#define SDF_VU0_DATA_MEMORY ((volatile u128 *)0x11004000)
#endif

/* Forces an address into the EE's uncached mirror by setting one of the top
 * two address bits, for a DMA engine or coprocessor reading memory the CPU
 * just wrote: EE_UNCACHED (bit 29) for an ordinary buffer or struct,
 * EE_UNCACHED_RING (bit 28) for a DMA ring address. On a host build there is
 * no cache to bypass and no mirror to land in, so both default to the plain
 * address. */
#ifndef EE_UNCACHED
#define EE_UNCACHED(addr) ((addr) | 0x20000000)
#endif
#ifndef EE_UNCACHED_RING
#define EE_UNCACHED_RING(addr) ((addr) | 0x30000000)
#endif

/* The EE hardware register space and the GS privileged register space, as
 * address ranges rather than individual registers: MovieViewer.c's debug
 * watcher classifies an arbitrary address by whether it falls in one of
 * them. A build with no mapped register range defines these so the test
 * never matches. */
#ifndef EE_REG_RANGE_BASE
#define EE_REG_RANGE_BASE 0x10000000
#define EE_REG_RANGE_END 0x10010000
#define GS_PRIV_REG_RANGE_BASE 0x12000000
#define GS_PRIV_REG_RANGE_END 0x12002000
#endif

/* Register access: a plain volatile access, unless the build supplies its
 * own. */
#ifndef HW_READ
#define HW_READ(reg) (*(reg))
#define HW_WRITE(reg, value) (*(reg) = (value))
#endif

/* A register reached as a struct's member (a DMA channel's chcr through
 * sceDmaGetChan) is named as the member itself, since taking its address
 * changes ee-gcc's code: read, its bits set, its bits cleared. A plain
 * access, unless the build supplies its own. */
#ifndef HW_FIELD_READ
#define HW_FIELD_READ(field) (field)
#define HW_FIELD_OR(field, bits) ((field) |= (bits))
#define HW_FIELD_AND(field, mask) ((field) &= (mask))
#endif

#endif /* EEREGS_H */
