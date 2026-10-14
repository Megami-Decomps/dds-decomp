#ifndef EFF_QUATERNION_NLERP_H
#define EFF_QUATERNION_NLERP_H

#include "common.h"

/* Blend the quaternion in vf10 toward vf11 along the shorter arc using
 * blendAmount, normalize the result, and leave it in vf10. Providers:
 * DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void effMiscQuaternionNlerpVU(f32 blendAmount);

#endif /* EFF_QUATERNION_NLERP_H */
