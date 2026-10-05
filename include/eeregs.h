#ifndef EEREGS_H
#define EEREGS_H

#include "common.h"

/*
 * Subset of the Sony SDK eeregs.h: only the registers retail code touches.
 * D4 is the toIPU DMA channel.
 */
#define D4_CHCR ((volatile u32 *)0x1000B400)
#define D4_MADR ((volatile u32 *)0x1000B410)
#define D4_QWC ((volatile u32 *)0x1000B420)
#define D4_TADR ((volatile u32 *)0x1000B430)

#endif /* EEREGS_H */
