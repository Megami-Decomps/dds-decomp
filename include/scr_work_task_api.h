#ifndef SCR_WORK_TASK_API_H
#define SCR_WORK_TASK_API_H

#include "common.h"

struct scrProcessWork_s;

/* Report whether a process node is the event script currently executing. */
typedef u8 ScrCurrentWorkTaskResult;
enum {
    SCR_WORK_TASK_NOT_CURRENT = 0,
    SCR_WORK_TASK_CURRENT = 1
};

struct scrProcessWork_s *scrGetWorkTaskHandle(void);
ScrCurrentWorkTaskResult scrIsCurrentWorkTask(const struct scrProcessWork_s *expected);

#endif /* SCR_WORK_TASK_API_H */
