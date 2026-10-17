#ifndef FLD_ENCOUNTER_ROLL_ENTRY_H
#define FLD_ENCOUNTER_ROLL_ENTRY_H

#include "common.h"

typedef struct FldEncEntry {
    s16 stage;
    s16 flag;
    s16 chance;
    s16 result;
} FldEncEntry;

typedef char FldEncEntry_size_must_be_8[
    (sizeof(FldEncEntry) == 8) ? 1 : -1];
typedef char FldEncEntry_stage_offset_must_be_0[
    ((u32)&((FldEncEntry *)0)->stage == 0) ? 1 : -1];
typedef char FldEncEntry_flag_offset_must_be_2[
    ((u32)&((FldEncEntry *)0)->flag == 2) ? 1 : -1];
typedef char FldEncEntry_chance_offset_must_be_4[
    ((u32)&((FldEncEntry *)0)->chance == 4) ? 1 : -1];
typedef char FldEncEntry_result_offset_must_be_6[
    ((u32)&((FldEncEntry *)0)->result == 6) ? 1 : -1];

#endif
