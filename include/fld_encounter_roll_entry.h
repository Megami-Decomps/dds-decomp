#ifndef FLD_ENCOUNTER_ROLL_ENTRY_H
#define FLD_ENCOUNTER_ROLL_ENTRY_H

#include "common.h"

typedef s16 FldEncounterEventFlagIndex;
typedef s16 FldEncounterTableIndex;

#define FLD_ENCOUNTER_EVENT_FLAG_INDEX_UNGATED (-1)
#define FLD_ENCOUNTER_TABLE_INDEX_NO_SELECTION (-1)

typedef struct FldEncEntry {
    s16 stage;
    FldEncounterEventFlagIndex eventFlagIndex;
    s16 chancePercent; /* Compared against a random value in [0, 100). */
    FldEncounterTableIndex encounterTableIndex;
} FldEncEntry;

typedef char FldEncEntry_size_must_be_8[
    (sizeof(FldEncEntry) == 8) ? 1 : -1];
typedef char FldEncEntry_stage_offset_must_be_0[
    ((u32)&((FldEncEntry *)0)->stage == 0) ? 1 : -1];
typedef char FldEncEntry_eventFlagIndex_offset_must_be_2[
    ((u32)&((FldEncEntry *)0)->eventFlagIndex == 2) ? 1 : -1];
typedef char FldEncEntry_chancePercent_offset_must_be_4[
    ((u32)&((FldEncEntry *)0)->chancePercent == 4) ? 1 : -1];
typedef char FldEncEntry_encounterTableIndex_offset_must_be_6[
    ((u32)&((FldEncEntry *)0)->encounterTableIndex == 6) ? 1 : -1];

#endif
