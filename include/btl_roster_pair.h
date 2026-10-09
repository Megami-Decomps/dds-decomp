#ifndef BTL_ROSTER_PAIR_H
#define BTL_ROSTER_PAIR_H

#include "btl.h"

/* DDS2 func_001AC750 clears the count at +0, writes 12-byte rows at +4,
 * and stores their two actor pointers at row +4/+8. The allocated global
 * capacity is not inferred from the writer's current contents. */
typedef struct BattleRosterPair {
    u16 skill;
    u8 pad02[2];
    BtlUnit *first;
    BtlUnit *second;
} BattleRosterPair;

typedef struct BattleRosterTable {
    u16 count;
    u8 pad02[2];
    BattleRosterPair entries[0];
} BattleRosterTable;

typedef char BattleRosterPairSizeCheck[sizeof(BattleRosterPair) == 12 ? 1 : -1];
typedef char BattleRosterTableRowsOffsetCheck[((u32)&((BattleRosterTable *)0)->entries == 4) ? 1 : -1];

/* Retail places this variable-length record in .data, outside the GP window. */
extern BattleRosterTable D_003B5D10 __attribute__((section(".data")));
BattleRosterTable *fldGetCachedSceneActorNameAndId(s32, u16 *);

#endif
