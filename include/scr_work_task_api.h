#ifndef SCR_WORK_TASK_API_H
#define SCR_WORK_TASK_API_H

#include "common.h"

struct ScrData;

struct ScrData *scrGetWorkTaskHandle(void);
u8 scrIsCurrentWorkTask(struct ScrData *expected);

#endif /* SCR_WORK_TASK_API_H */
