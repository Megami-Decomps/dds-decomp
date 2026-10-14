#ifndef MNU_EVENT_FLAG_ENTRIES_H
#define MNU_EVENT_FLAG_ENTRIES_H

#include "common.h"

#define MNU_SCENE_FLAG_ENTRY_COUNT 4
#define MNU_PARTY_FLAG_ENTRY_COUNT 16

/* Scene rows carry the two event flags followed by their text-table indices. */
typedef struct MnuSceneFlagEntry {
    s32 requiredFlag;
    s32 handledFlag;
    u16 areaIndex;
    u8 nameIndex;
    u8 unknown0B;
    u16 dialogIndex;
    u16 unknown0E;
} MnuSceneFlagEntry;

/* Each party row maps a party identity to the required/handled flag pair. */
typedef struct MnuPartyFlagEntry {
    s32 requiredFlag;
    s32 handledFlag;
} MnuPartyFlagEntry;

typedef char MnuSceneFlagEntry_size_is_10[(sizeof(MnuSceneFlagEntry) == 0x10) ? 1 : -1];
typedef char MnuSceneFlagEntry_offsets_are_stable[
    (((u32)&((MnuSceneFlagEntry *)0)->requiredFlag == 0x00) &&
     ((u32)&((MnuSceneFlagEntry *)0)->handledFlag == 0x04) &&
     ((u32)&((MnuSceneFlagEntry *)0)->areaIndex == 0x08) &&
     ((u32)&((MnuSceneFlagEntry *)0)->nameIndex == 0x0A) &&
     ((u32)&((MnuSceneFlagEntry *)0)->unknown0B == 0x0B) &&
     ((u32)&((MnuSceneFlagEntry *)0)->dialogIndex == 0x0C) &&
     ((u32)&((MnuSceneFlagEntry *)0)->unknown0E == 0x0E)) ? 1 : -1];
typedef char MnuPartyFlagEntry_size_is_8[(sizeof(MnuPartyFlagEntry) == 8) ? 1 : -1];
typedef char MnuPartyFlagEntry_offsets_are_stable[
    (((u32)&((MnuPartyFlagEntry *)0)->requiredFlag == 0x00) &&
     ((u32)&((MnuPartyFlagEntry *)0)->handledFlag == 0x04)) ? 1 : -1];

#endif
