#ifndef BTL_RESOURCE_H
#define BTL_RESOURCE_H

#include "common.h"

struct EffectSlotSet;
struct SdfMemBlock;

/* Panel resource root: both native constructors allocate 0x28 bytes.
 * Preserve each game's established allocator-handle contract. */
typedef struct BtlResBlock {
#ifdef VERSION_DDS1
    s32 unk0;
#else
    struct SdfMemBlock *unk0;
#endif
    struct SdfMemBlock *resourceAllocationA; /* 0x04: /battle/panel/batle_01.spr */
    struct SdfMemBlock *resourceAllocationB; /* 0x08: /battle/panel/batle_02.spr */
    struct SdfMemBlock *resourceAllocationC; /* 0x0C: /battle/panel/battle_03.spr */
    struct EffectSlotSet *resA; /* 0x10 */
    struct EffectSlotSet *resB; /* 0x14 */
    struct EffectSlotSet *resC; /* 0x18 */
    s32 unk1C;
    u8 reserved20[8]; /* Undecoded tail of the native allocation. */
} BtlResBlock;

typedef char BtlResBlock_size_must_be_0x28[(sizeof(BtlResBlock) == 0x28) ? 1 : -1];
typedef char BtlResBlock_resourceAllocationA_at_0x04[
    ((u32)&((BtlResBlock *)0)->resourceAllocationA == 0x04) ? 1 : -1];
typedef char BtlResBlock_resourceAllocationB_at_0x08[
    ((u32)&((BtlResBlock *)0)->resourceAllocationB == 0x08) ? 1 : -1];
typedef char BtlResBlock_resourceAllocationC_at_0x0C[
    ((u32)&((BtlResBlock *)0)->resourceAllocationC == 0x0C) ? 1 : -1];
typedef char BtlResBlock_resA_at_0x10[((u32)&((BtlResBlock *)0)->resA == 0x10) ? 1 : -1];
typedef char BtlResBlock_tail_at_0x20[((u32)&((BtlResBlock *)0)->reserved20 == 0x20) ? 1 : -1];

#endif /* BTL_RESOURCE_H */
