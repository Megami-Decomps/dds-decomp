#ifndef ITF_MESSAGE_MANAGER_TASK_H
#define ITF_MESSAGE_MANAGER_TASK_H

#include "kwln_task_runner.h"

KwlnTaskUpdateResult CalcMsgMng_Process(dds3ProcessID_t *task);
KwlnTaskUpdateResult DrawMsgMng_Process(dds3ProcessID_t *task);
void DrawMsgMng_Shutdown(dds3ProcessID_t *task);

#endif /* ITF_MESSAGE_MANAGER_TASK_H */
