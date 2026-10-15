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
struct BtlRuntimeTask *btlCreateActorModelBlendTask(struct BtlUnit *unit, struct BtlUnit *target, s32 index, s32 value, f32 scale);
/* Keep the common unit/target/scale prefix; DDS2 adds an interpolation mode.
 * EE EABI assigns scale to f12 and the extra mode to a2. */
#ifdef VERSION_DDS2
struct BtlRuntimeTask *btlCreateUnitDefeatFadeInTask(struct BtlUnit *unit, s32 option);
struct BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(struct BtlUnit *unit, f32 *target, f32 scale, s8 mode);
#else
struct BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(struct BtlUnit *unit, f32 *target, f32 scale);
#endif

#endif
