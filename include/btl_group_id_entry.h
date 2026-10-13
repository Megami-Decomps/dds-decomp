#ifndef BTL_GROUP_ID_ENTRY_H
#define BTL_GROUP_ID_ENTRY_H

#include "common.h"

typedef struct BattleGroupIdEntry {
    struct BattleGroupIdEntry *next;
    s32 id;
} BattleGroupIdEntry;

typedef char BattleGroupIdEntry_size_must_be_8[(sizeof(BattleGroupIdEntry) == 8) ? 1 : -1];
typedef char BattleGroupIdEntry_id_must_be_at_4[
    ((u32)&((BattleGroupIdEntry *)0)->id == 4) ? 1 : -1];

#endif /* BTL_GROUP_ID_ENTRY_H */
