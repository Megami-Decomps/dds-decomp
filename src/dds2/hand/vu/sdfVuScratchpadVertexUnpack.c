#include "common.h"

#ifdef NON_MATCHING
#include "common.h"
#include "eeregs.h"

#ifndef D0_CHCR
#define D0_CHCR ((volatile u32 *)0x10008000)
#define D0_MADR ((volatile u32 *)0x10008010)
#define D0_QWC ((volatile u32 *)0x10008020)
#endif
#ifndef VIF0_STAT
#define VIF0_STAT ((volatile u32 *)0x10003800)
#endif

#define SDF_EE_PHYSICAL_MASK 0x0FFFFFFF
#define SDF_VIF0_DMA_START 0x101   /* CHCR: from memory, STR */
#define SDF_DMA_STR 0x100
#define SDF_VIF0_BUSY 0x0F000003   /* VIF0_STAT: FQC and VPS */
#define SDF_UNPACK_BATCH 24        /* vertices per transfer */
#define SDF_UNPACK_OUTPUT_QWORD 0x60 /* the microprogram's first output, in VU0 data memory */
#define SDF_VU0_MEMORY ((const u8 *)0x11004000)

/* Run vertices through VU0's microprogram in batches and unpack its results
 * to output. Each batch sends transferCount + vertices * transferStride bytes
 * from source to VIF0 (whose codes unpack the vertices and start the
 * microprogram), waits for the transfer and VU0, then copies each vertex's
 * four result quadwords to the output (the last two swapped) with a word
 * packed from the w field of the third: its low two bits, then bits 6..13.
 * Returns the source address after the last batch. Upstream: hand-written
 * assembly (hand/vu/corePktVu0Run.s), read through the VU0
 * macro instructions it uses (vlqi, vilwr, cfc2). Retail leaves vf4..vf7,
 * $vi2 and $vi10 as the last vertex set them; this sets only $vi2, to the first
 * result, through retail's interlocking ctc2. */
u32 corePktVu0Run(u8 *output, u32 source, s32 count, s32 transferCount, s32 transferStride) {
    s32 done = 0;

    while (done < count) {
        s32 batch = count - done < SDF_UNPACK_BATCH + 1 ? count - done : SDF_UNPACK_BATCH;
        u32 quadwords = (u32)(transferStride * batch + transferCount + 15) >> 4;
        const u8 *result = SDF_VU0_MEMORY + SDF_UNPACK_OUTPUT_QWORD * 16;
        s32 i;

        done += batch;
        HW_WRITE(D0_QWC, quadwords);
        HW_WRITE(D0_MADR, source & SDF_EE_PHYSICAL_MASK);
        source += quadwords << 4;
        HW_WRITE(D0_CHCR, SDF_VIF0_DMA_START);
        while ((HW_READ(D0_CHCR) & SDF_DMA_STR) || (HW_READ(VIF0_STAT) & SDF_VIF0_BUSY)) {
        }
        /* Retail's ctc2.i waits for VU0's microprogram before the results are read. */
        __asm__ volatile("ctc2.i %0, $vi2" : : "r"(SDF_UNPACK_OUTPUT_QWORD));
        for (i = 0; i < batch; i++) {
            u16 flags = *(const u16 *)(result + 0x2C); /* vilwr.w: the third quadword's w */

            *(u128 *)(output + 0x00) = *(const u128 *)(result + 0x00);
            *(u128 *)(output + 0x10) = *(const u128 *)(result + 0x10);
            *(u128 *)(output + 0x30) = *(const u128 *)(result + 0x20);
            *(u128 *)(output + 0x20) = *(const u128 *)(result + 0x30);
            *(u32 *)(output + 0x50) = (flags & 3) | ((flags >> 6) & 0xFF) << 8 | ((flags >> 14) & 0xFF) << 24;
            result += 0x40;
            output += 0x60;
        }
    }
    return source;
}
#else
INCLUDE_ASM(const s32, "hand/vu/sdfVuScratchpadVertexUnpack", corePktVu0Run);
#endif
