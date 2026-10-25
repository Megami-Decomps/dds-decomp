#ifndef KWLN_PROCESS_MAIN_RESULT_H
#define KWLN_PROCESS_MAIN_RESULT_H

#include "common.h"

/* Main-loop scheduler step result: STOP breaks the frame loop; CONTINUE advances it. */
typedef s32 KwlnProcessMainResult;

#define KWLN_PROCESS_MAIN_STOP 0
#define KWLN_PROCESS_MAIN_CONTINUE 1

#endif /* KWLN_PROCESS_MAIN_RESULT_H */
