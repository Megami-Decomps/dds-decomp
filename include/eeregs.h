#ifndef EEREGS_H
#define EEREGS_H

#include "common.h"

/*
 * Subset of the Sony SDK eeregs.h: the EE and GS registers retail code
 * touches, by their SDK names.
 */

/* A register as a pointer to its address. A build with no register space at
 * these addresses defines this to point into storage of its own. */
#ifndef SDF_HW_REG
#define SDF_HW_REG(type, addr) ((volatile type *)(addr))
#endif

#define T0_COUNT SDF_HW_REG(u32, 0x10000000)
#define T0_MODE SDF_HW_REG(u32, 0x10000010)

#define IPU_CMD SDF_HW_REG(u32, 0x10002000)
#define IPU_CTRL SDF_HW_REG(u32, 0x10002010)

#define GIF_CTRL SDF_HW_REG(u32, 0x10003000)
#define GIF_STAT SDF_HW_REG(u32, 0x10003020)
#define VIF0_STAT SDF_HW_REG(u32, 0x10003800)
#define VIF1_STAT SDF_HW_REG(u32, 0x10003C00)

/* D0 and D1 are the VIF DMA channels, D3 fromIPU and D4 toIPU. */
#define D0_CHCR SDF_HW_REG(u32, 0x10008000)
#define D0_MADR SDF_HW_REG(u32, 0x10008010)
#define D0_QWC SDF_HW_REG(u32, 0x10008020)
#define D1_CHCR SDF_HW_REG(u32, 0x10009000)
#define D1_MADR SDF_HW_REG(u32, 0x10009010)
#define D1_TADR SDF_HW_REG(u32, 0x10009030)
/* Channel 2 is the GIF's. sdk/libgraph.c spells this register itself, as a
 * dereference rather than a pointer, because it includes no eeregs.h; a
 * consumer transcribing sceGsSyncPath reads it through HW_READ and needs the
 * pointer form here. */
#define D2_CHCR SDF_HW_REG(u32, 0x1000A000)
#define D3_CHCR SDF_HW_REG(u32, 0x1000B000)
#define D3_MADR SDF_HW_REG(u32, 0x1000B010)
#define D3_QWC SDF_HW_REG(u32, 0x1000B020)
#define D4_CHCR SDF_HW_REG(u32, 0x1000B400)
#define D4_MADR SDF_HW_REG(u32, 0x1000B410)
#define D4_QWC SDF_HW_REG(u32, 0x1000B420)
#define D4_TADR SDF_HW_REG(u32, 0x1000B430)
#define D_ENABLER SDF_HW_REG(u32, 0x1000F520)
#define D_ENABLEW SDF_HW_REG(u32, 0x1000F590)

/* GS privileged registers. */
#define GS_CSR SDF_HW_REG(u64, 0x12001000)
#define GS_BUSDIR SDF_HW_REG(u64, 0x12001040)

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

/* A DMA tag: the address in the high word, the control word in the low one.
 * Absorbs the physical conversion, so a consumer that redefines EE_PHYSICAL
 * gets every tag with it and never meets the mask at a tag site.
 *
 * word is widened without narrowing first, which preserves what the
 * open-coded sites did including sign extension. Three callers pass a
 * computed word rather than a literal (sdfModel.c's qwc, count and packed,
 * each OR-ed with 0x30000000), so if one of those ever had bit 31 set the
 * widening would reach into the address half. That is the existing
 * behaviour, not a change, and it is named here because the macro hides a
 * shift the open-coded form showed. */
#ifndef SDF_DMA_TAG
#define SDF_DMA_TAG(addr, word) (((u64)EE_PHYSICAL(addr) << 32) | (u64)(word))
#endif

/* The same tag with the operands in the other order, for the two sites whose
 * source order the compiler reproduces: both pass a (u32) control word, and
 * writing the address first moves the zero extension and changes the
 * instructions. Same value, same layout, order preserved. The word keeps
 * whatever type the caller gives it rather than being widened here, because
 * that is what those two sites do. */
#ifndef SDF_DMA_TAG_WORD_FIRST
#define SDF_DMA_TAG_WORD_FIRST(word, addr) ((word) | ((u64)EE_PHYSICAL(addr) << 32))
#endif

/* A cross-thread flag another thread writes, polled for lack of a scheduler.
 * addr keys a host wait and wake table; cast to void so the target still
 * type-checks it against the flag SDF_FLAG_SET writes. A host redefinition
 * must wake under the wait's lock and publish the value first; SET does both.
 * Writes that run before any thread exists stay plain assignments, because
 * there is no consumer to signal and a wake there would be meaningless rather
 * than wrong. A consumer must not assume SDF_FLAG_SET can only run once its
 * own wait table exists: nothing here guarantees that ordering. */
#ifndef SDF_FLAG_WAIT
#define SDF_FLAG_WAIT(addr, cond) do { (void)(addr); while (cond) { } } while (0)
#endif
#ifndef SDF_FLAG_SET
#define SDF_FLAG_SET(addr, lvalue, value) do { (void)(addr); (lvalue) = (value); } while (0)
#endif

/* The EE's plain physical addressing, which is what a DMA engine reads: the
 * low 28 bits name RAM and the top nibble is the cache mode. Off target there
 * is no cache mode and no 28-bit RAM, so the address passes through. */
#ifndef EE_PHYSICAL
#define EE_PHYSICAL(addr) ((addr) & 0x0FFFFFFF)
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
