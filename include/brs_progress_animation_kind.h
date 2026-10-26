#ifndef BRS_PROGRESS_ANIMATION_KIND_H
#define BRS_PROGRESS_ANIMATION_KIND_H

#include "common.h"

/* Keep the selector's signed-byte convention while naming its two call modes. */
typedef s8 BrsProgressAnimationKind;

/* brsCalcProgressAnimationStep advances one of these separate progress stores. */
enum BrsProgressAnimationMode {
    BRS_PROGRESS_ANIMATION_LEVEL_EXPERIENCE = 0,
    BRS_PROGRESS_ANIMATION_PROFILE_POINTS = 1
};

struct BrsSkillPackageWork;
struct BrsProgressRow;

s32 brsCalcProgressAnimationStep(struct BrsSkillPackageWork *work,
                                 struct BrsProgressRow *progressRow,
                                 s32 rowIndex,
                                 BrsProgressAnimationKind progressKind);

#endif
