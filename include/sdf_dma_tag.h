#ifndef SDF_DMA_TAG_H
#define SDF_DMA_TAG_H

#include "common.h"

/* DMA tag prefix followed by the two VIF words used by indexed-model writers.
 * The control byte stays whole, and address is a hardware word rather than a
 * CPU pointer. */
typedef struct SdfDmaTagHeader {
    u16 quadwordCount;
    u8 reservedByte;
    u8 control;
    u32 address;
    u32 firstVifCode;
    u32 secondVifCode;
} SdfDmaTagHeader;

/* Packet-list code views the same 16-byte row as QWC/ID/ADDR/VIF commands. */
typedef struct SdfDmaTag {
    u16 quadwordCount;
    u8 pad02;
    u8 kind;
    u32 address;
    u64 vifCommands;
} SdfDmaTag;

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
typedef char SdfDmaTag_size_must_be_0x10[
    (sizeof(SdfDmaTag) == 0x10) ? 1 : -1];
typedef char SdfDmaTag_quadwordCount_at_0[
    ((u32)&((SdfDmaTag *)0)->quadwordCount == 0) ? 1 : -1];
typedef char SdfDmaTag_kind_at_3[
    ((u32)&((SdfDmaTag *)0)->kind == 3) ? 1 : -1];
typedef char SdfDmaTag_address_at_4[
    ((u32)&((SdfDmaTag *)0)->address == 4) ? 1 : -1];
typedef char SdfDmaTag_vifCommands_at_8[
    ((u32)&((SdfDmaTag *)0)->vifCommands == 8) ? 1 : -1];

#endif /* SDF_DMA_TAG_H */
