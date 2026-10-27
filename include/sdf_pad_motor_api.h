#ifndef SDF_PAD_MOTOR_API_H
#define SDF_PAD_MOTOR_API_H

#include "sdf_pad_index.h"

/* These APIs index the two-entry logical-pad table without bounds checks. */
void sdfPadSetAct0(SdfPadIndex padIndex, u16 strength);
void sdfPadSetAct1(SdfPadIndex padIndex, u8 strength);
void sdfPadSetAct(SdfPadIndex padIndex, s32 smallMotor, s32 largeMotor);

#endif /* SDF_PAD_MOTOR_API_H */
