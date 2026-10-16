#ifndef KWLN_TASK_QUEUE_MUTATION_H
#define KWLN_TASK_QUEUE_MUTATION_H

#include "kwln.h"

void kwlnTaskRemoveFromStateQueue(KwlnTask *task);
void kwlnTaskInsertIntoOrderedStateQueue(KwlnTask *task);

#endif /* KWLN_TASK_QUEUE_MUTATION_H */
