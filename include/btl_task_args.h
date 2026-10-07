#ifndef BTL_TASK_ARGS_H
#define BTL_TASK_ARGS_H

#include "common.h"

/* Return the runtime scheduler task's allocated argument block, or NULL. */
void *btlGetTaskArguments(void *task);

#endif /* BTL_TASK_ARGS_H */
