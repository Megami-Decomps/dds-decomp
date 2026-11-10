#ifndef DDS_SCR_PROGRAM_COUNTER_API_H
#define DDS_SCR_PROGRAM_COUNTER_API_H

#include "scr.h"

typedef s32 ScrLabelIndex;

s32 scrGetProcedureCodeAddress(ScrProcedureIndex procedureIndex);
s32 scrGetLabelCodeAddress(ScrLabelIndex labelIndex);
s32 scrGetCodePointer(void);
void scrSetCodePointer(s32 programCounter);

#endif /* DDS_SCR_PROGRAM_COUNTER_API_H */
