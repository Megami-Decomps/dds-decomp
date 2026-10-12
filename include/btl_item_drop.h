#ifndef BTL_ITEM_DROP_H
#define BTL_ITEM_DROP_H

#include "common.h"

/* Battle reward slot: item identity and owned drop count are observed by clients. */
typedef struct BattleItemDrop {
    u16 id; /* 0x00 */
    u8 count; /* 0x02 */
    u8 unknown_03;
} BattleItemDrop;

typedef char BattleItemDrop_size_must_be_4[
    (sizeof(BattleItemDrop) == 4) ? 1 : -1];
typedef char BattleItemDrop_count_offset_must_be_2[
    ((u32)&((BattleItemDrop *)0)->count == 2) ? 1 : -1];
typedef char BattleItemDrop_unknown_offset_must_be_3[
    ((u32)&((BattleItemDrop *)0)->unknown_03 == 3) ? 1 : -1];

#endif /* BTL_ITEM_DROP_H */
