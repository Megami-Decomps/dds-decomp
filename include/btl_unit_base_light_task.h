#ifndef BTL_UNIT_BASE_LIGHT_TASK_H
#define BTL_UNIT_BASE_LIGHT_TASK_H

#include "common.h"

struct BtlUnit;
struct BtlRuntimeTask;

struct BtlRuntimeTask *btlCreateUnitBaseLightTask(struct BtlUnit *unit);

#endif
