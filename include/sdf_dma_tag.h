#ifndef SDF_DMA_TAG_H
#define SDF_DMA_TAG_H

#include "common.h"

/* DMA tag bytes followed by two VIF words. The control byte is kept whole;
 * the address is a hardware address word, rather than an owned CPU pointer. */
typedef struct SdfDmaTagHeader {
    u16 quadwordCount;
    u8 reservedByte;
    u8 control;
    u32 address;
    u32 firstVifCode;
    u32 secondVifCode;
} SdfDmaTagHeader;

typedef char SdfDmaTagHeader_size_must_be_0x10[
    (sizeof(SdfDmaTagHeader) == 0x10) ? 1 : -1];
typedef char SdfDmaTagHeader_alignment_must_be_4[
    (__alignof__(SdfDmaTagHeader) == 4) ? 1 : -1];
typedef char SdfDmaTagHeader_control_at_3[
    ((u32)&((SdfDmaTagHeader *)0)->control == 3) ? 1 : -1];
typedef char SdfDmaTagHeader_address_at_4[
    ((u32)&((SdfDmaTagHeader *)0)->address == 4) ? 1 : -1];
typedef char SdfDmaTagHeader_firstVifCode_at_8[
    ((u32)&((SdfDmaTagHeader *)0)->firstVifCode == 8) ? 1 : -1];
typedef char SdfDmaTagHeader_secondVifCode_at_C[
    ((u32)&((SdfDmaTagHeader *)0)->secondVifCode == 0xC) ? 1 : -1];

#endif /* SDF_DMA_TAG_H */
