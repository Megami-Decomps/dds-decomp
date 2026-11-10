#ifndef ITF_MESSAGE_MANAGER_TASK_H
#define ITF_MESSAGE_MANAGER_TASK_H

#include "kwln_task_runner.h"

dds3TaskUpdateResult_t CalcMsgMng_Process(dds3ProcessID_t *task);
dds3TaskUpdateResult_t DrawMsgMng_Process(dds3ProcessID_t *task);
void DrawMsgMng_Shutdown(dds3ProcessID_t *task);

#endif /* ITF_MESSAGE_MANAGER_TASK_H */
