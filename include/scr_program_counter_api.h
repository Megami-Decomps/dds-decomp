#ifndef DDS_SCR_PROGRAM_COUNTER_API_H
#define DDS_SCR_PROGRAM_COUNTER_API_H

#include "scr.h"

typedef s32 ScrLabelIndex;

ScrProgramCounter scrGetProcedureCodeAddress(ScrProcedureIndex procedureIndex);
ScrProgramCounter scrGetLabelCodeAddress(ScrLabelIndex labelIndex);
ScrProgramCounter scrGetCodePointer(void);
void scrSetCodePointer(ScrProgramCounter programCounter);

#endif /* DDS_SCR_PROGRAM_COUNTER_API_H */
