#ifndef KWLN_PROCESS_REGISTRATION_H
#define KWLN_PROCESS_REGISTRATION_H

#include "common.h"

/* Outcome of searching the scheduler queues for a task handle. */
typedef s32 KwlnProcessRegistrationResult;

#define KWLN_PROCESS_NOT_REGISTERED 0
#define KWLN_PROCESS_REGISTERED 1

struct dds3ProcessID_t;

/* Test membership in the starting, running, and destruction-pending queues. */
KwlnProcessRegistrationResult dds3ExistProcess(const struct dds3ProcessID_t *task);

#endif /* KWLN_PROCESS_REGISTRATION_H */
