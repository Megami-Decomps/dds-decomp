#ifndef BTL_ROSTER_PAIR_H
#define BTL_ROSTER_PAIR_H

#include "btl.h"

/* Both native enumerators write a halfword count and 12-byte actor pairs.
 * DDS1 reserves 0x800 bytes at 00359160; DDS2 reserves 0xB00 at 003B5D10.
 * Four bytes after the last complete row belong to the allocated buffer. */
typedef struct BattleRosterPair {
    u16 skill;
    u8 pad02[2];
    BtlUnit *first;
    BtlUnit *second;
} BattleRosterPair;

#if defined(VERSION_DDS1)
#define BTL_ROSTER_PAIR_MAX 170
#elif defined(VERSION_DDS2)
#define BTL_ROSTER_PAIR_MAX 234
#endif

typedef struct BattleRosterTable {
    u16 count;
    u8 pad02[2];
    BattleRosterPair entries[BTL_ROSTER_PAIR_MAX];
    u8 padEnd[4];
} BattleRosterTable;

typedef char BattleRosterPairSizeCheck[sizeof(BattleRosterPair) == 12 ? 1 : -1];
typedef char BattleRosterTableRowsOffsetCheck[((u32)&((BattleRosterTable *)0)->entries == 4) ? 1 : -1];
#if defined(VERSION_DDS1)
typedef char BattleRosterTableSizeCheck[sizeof(BattleRosterTable) == 0x800 ? 1 : -1];
#elif defined(VERSION_DDS2)
typedef char BattleRosterTableSizeCheck[sizeof(BattleRosterTable) == 0xB00 ? 1 : -1];
#endif

/* The complete retail buffers are in .data, outside the GP window. */
#ifdef VERSION_DDS1
extern BattleRosterTable D_00359160 __attribute__((section(".data")));
#endif
#ifdef VERSION_DDS2
extern BattleRosterTable D_003B5D10 __attribute__((section(".data")));
BattleRosterTable *fldGetCachedSceneActorNameAndId(s32, u16 *);
#endif

#endif
