#ifndef ITF_MES_STATE_API_H
#define ITF_MES_STATE_API_H

#include "common.h"

struct BtlFade;
struct instanceMes_tag;
struct ItfMesTextSlots;

/* Entry release may also release the primary speaker-caption text block. */
typedef s32 ItfMesMessageReleaseMode;
enum {
    ITF_MES_RELEASE_ENTRY_ONLY = 0,
    ITF_MES_RELEASE_ENTRY_AND_PRIMARY_TEXT = 1
};

/* Reset fade timing and alpha, optionally preserving its kind byte. */
void InitializeCurCtr(struct BtlFade *fade, s32 preserveKind);
/* Release an interface state's effect resources and clear its effect flags. */
void btlReleaseEffectResourceHandles(struct instanceMes_tag *effect);
/* Release texture handles owned by its message text slots. */
void ReleaseAllVariable(struct ItfMesTextSlots *slots);

#endif /* ITF_MES_STATE_API_H */
