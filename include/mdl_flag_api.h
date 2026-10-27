#ifndef MDL_FLAG_API_H
#define MDL_FLAG_API_H

#include "common.h"

typedef s32 EvtBitTestResult;

#define EVT_BIT_TEST_RESULT_NOT_SET 0
#define EVT_BIT_TEST_RESULT_SET 1

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);
void evtBitOff(s32 flag);

/* Clear all model flags. */
void evtBitAllOff(void);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

#endif /* MDL_FLAG_API_H */
