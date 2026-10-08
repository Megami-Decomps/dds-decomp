#ifndef BTL_UNIT_TASKS_H
#define BTL_UNIT_TASKS_H

#include "common.h"

struct BtlUnit;
struct BtlRuntimeTask;

/* Actor creation and scheduler tasks that retain an actor. */
struct BtlUnit *btlCreateUnit(void);
struct BtlRuntimeTask *btlCreateModelLoadPollTask(struct BtlUnit *unit, u32 index, u32 value, s8 mode);
struct BtlRuntimeTask *btlCreateUnitBaseLightTask(struct BtlUnit *unit);
struct BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(struct BtlUnit *unit, f32 *target, f32 scale);
#ifdef VERSION_DDS2
struct BtlRuntimeTask *func_001E5FF8(struct BtlUnit *unit, s32 option);
struct BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(struct BtlUnit *unit, f32 *target, s8 mode, f32 scale);
#else
struct BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(struct BtlUnit *unit, f32 *target, f32 scale);
#endif

#endif
