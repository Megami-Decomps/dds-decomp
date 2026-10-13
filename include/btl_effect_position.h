#ifndef BTL_EFFECT_POSITION_H
#define BTL_EFFECT_POSITION_H

#include "common.h"

struct BtlUnit;

void btlSetActorEffectParameterOrMuzzlePosition(struct BtlUnit *unit, s32 mode);
/* Leaves the selected effect or muzzle position in vf10. */
void btlUnitGetEffectPosVU(struct BtlUnit *unit);
/* Leaves the unit's transformed muzzle position in vf10. */
void btlUnitGetMuzzlePosVU(struct BtlUnit *unit);

#endif
