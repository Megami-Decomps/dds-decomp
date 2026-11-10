#ifndef DDS3_TASK_QUEUE_MUTATION_H
#define DDS3_TASK_QUEUE_MUTATION_H

#include "kernel.h"

void DeleteProcessList(dds3ProcessID_t *task);
void AddProcessList(dds3ProcessID_t *task);

#endif /* DDS3_TASK_QUEUE_MUTATION_H */
