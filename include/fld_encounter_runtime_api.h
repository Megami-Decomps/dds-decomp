#ifndef FLD_ENCOUNTER_RUNTIME_API_H
#define FLD_ENCOUNTER_RUNTIME_API_H

#include "common.h"

/* Field encounter modes; 0-2 select runtime fades and 3 bypasses that runtime. */
typedef s32 FldEncounterRuntimeMode;
enum FldEncounterRuntimeModeValue {
    FLD_ENCOUNTER_MODE_STANDARD = 0,
    FLD_ENCOUNTER_MODE_ALTERNATE = 1,
    FLD_ENCOUNTER_MODE_SCRIPT_EVENT = 2,
    FLD_ENCOUNTER_MODE_BYPASS_BATTLE_RUNTIME = 3
};

s32 fldGetEncounterRuntimeResult(void);
void fldSetEncounterPendingValue(u32 value);
s32 fldSetEncounterMode(FldEncounterRuntimeMode mode);
void fldRequestEncounterWithFade(FldEncounterRuntimeMode mode, s32 recordIndex);
extern FldEncounterRuntimeMode fldEncounterRuntimeState;

#endif /* FLD_ENCOUNTER_RUNTIME_API_H */
