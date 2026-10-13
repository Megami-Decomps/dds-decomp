#ifndef DAT_PARTY_GROWTH_H
#define DAT_PARTY_GROWTH_H

#include "common.h"
#include "dat_stat.h"

/* One party member's per-level base-stat gains. The five leading bytes remain
 * opaque; the remainder is 99 level rows with one byte per base stat. */
typedef struct PartyStatGrowthBank {
    u8 unk0[5];
    u8 levelStats[99][DAT_BASE_STAT_COUNT];
} PartyStatGrowthBank;

typedef char PartyStatGrowthBank_size_must_be_0x1F4[
    (sizeof(PartyStatGrowthBank) == 0x1F4) ? 1 : -1];
typedef char PartyStatGrowthBank_levelStats_offset_must_be_5[
    ((u32)&((PartyStatGrowthBank *)0)->levelStats == 5) ? 1 : -1];

#endif
