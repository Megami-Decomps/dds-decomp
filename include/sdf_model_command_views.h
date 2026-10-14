#ifndef SDF_MODEL_COMMAND_VIEWS_H
#define SDF_MODEL_COMMAND_VIEWS_H

#include "common.h"

/* One DMA tag followed by the two VIF codes used by the model packet writers. */
typedef struct SdfModelPacket {
    union {
        s64 bits;
        struct {
            u16 quadwordCount;
            u8 reservedByte;
            u8 control; /* PCE, tag ID and IRQ in the high tag byte */
            u32 address;
        } fields;
    } dmaTag;
    union {
        s64 bits;
        struct {
            s32 firstCode;
            s32 secondCode;
        } fields;
    } vifCodes;
} SdfModelPacket;

typedef char SdfModelPacket_size_must_be_0x10[
    (sizeof(SdfModelPacket) == 0x10) ? 1 : -1];

/* Fields read by sdfModelBuildIndexedCommandPayload; the kind-specific gap
 * remains opaque, and no extent beyond these demonstrated words is claimed. */
typedef struct SdfIndexedCommandView {
    u8 kind;
    u8 pad01[3];
    u32 assetIndexAndCount;
    u32 address;
} SdfIndexedCommandView;

typedef char SdfIndexedCommandView_kind_at_0[
    ((u32)&((SdfIndexedCommandView *)0)->kind == 0) ? 1 : -1];
typedef char SdfIndexedCommandView_assetIndexAndCount_at_4[
    ((u32)&((SdfIndexedCommandView *)0)->assetIndexAndCount == 4) ? 1 : -1];
typedef char SdfIndexedCommandView_address_at_8[
    ((u32)&((SdfIndexedCommandView *)0)->address == 8) ? 1 : -1];

#endif /* SDF_MODEL_COMMAND_VIEWS_H */
