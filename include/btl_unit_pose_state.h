#ifndef BTL_UNIT_POSE_STATE_H
#define BTL_UNIT_POSE_STATE_H

/* The model-resource callback sets this after writing the unit's pose;
 * btlUpdateUnitActors clears it and the damage-shake task stops while set. */
#define BTL_UNIT_STATE_MODEL_EFFECT_POSE_ACTIVE 0x200000

#endif
