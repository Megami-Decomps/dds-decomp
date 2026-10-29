#ifndef EFF_PARAMETER_POSITION_READINESS_H
#define EFF_PARAMETER_POSITION_READINESS_H

#include "common.h"

struct dds3Basic_t;

/* GetESPPos publishes the ready position through VU state and reports readiness. */
typedef s32 EffectParameterPositionReadiness;
enum {
    EFFECT_PARAMETER_POSITION_NOT_READY = 0,
    EFFECT_PARAMETER_POSITION_READY = 1
};

EffectParameterPositionReadiness GetESPPos(struct dds3Basic_t *object);

#endif /* EFF_PARAMETER_POSITION_READINESS_H */
