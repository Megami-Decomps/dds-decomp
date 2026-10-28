#ifndef FLD_ENCOUNT_FLAGS_H
#define FLD_ENCOUNT_FLAGS_H

#include "common.h"

/* The field's control flags (fldEncountSetFlag, fldEncountClearFlag,
 * fldEncountTestFlag). */
extern u32 fldEncountFlags;

/* The player controls the player's object: set as the field hands it the
 * player's control (fldPreparePlayerSceneCameraTarget, a script's
 * PAD_ENABLE), cleared as it takes it back (fldResetPlayerSceneObjectState,
 * a script's PAD_DISABLE) and as a scene loads. */
#define FLD_ENCOUNT_FLAG_PLAYER_CONTROL 0x40

#endif /* FLD_ENCOUNT_FLAGS_H */
