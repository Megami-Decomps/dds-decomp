#ifndef ITF_MES_STATE_API_H
#define ITF_MES_STATE_API_H

#include "common.h"

struct BtlFade;
struct ItfMesState;

/* Reset fade timing and alpha, optionally preserving its kind byte. */
void itfResetBattleFadeState(struct BtlFade *fade, s32 preserveKind);
/* Release an interface state's effect resources and clear its effect flags. */
void btlReleaseEffectResourceHandles(struct ItfMesState *effect);

#endif /* ITF_MES_STATE_API_H */
