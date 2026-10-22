#ifndef SCR_WORK_TASK_API_H
#define SCR_WORK_TASK_API_H

#include "common.h"

struct scrProcessWork_s;

struct scrProcessWork_s *scrGetWorkTaskHandle(void);
u8 scrIsCurrentWorkTask(struct scrProcessWork_s *expected);

#endif /* SCR_WORK_TASK_API_H */
