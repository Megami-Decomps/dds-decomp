#ifndef FLD_ENCOUNTER_REQUEST_H
#define FLD_ENCOUNTER_REQUEST_H

#include "common.h"

typedef struct FieldEncounterRequest {
    s32 tableIndex;          /* 0x00 */
    s32 encounterSelection;  /* 0x04 */
} FieldEncounterRequest;

typedef char FieldEncounterRequestSizeCheck[
    sizeof(FieldEncounterRequest) == 8 ? 1 : -1];

#endif
