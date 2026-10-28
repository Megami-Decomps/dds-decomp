#ifndef BTL_MODEL_AND_SOUND_READINESS_API_H
#define BTL_MODEL_AND_SOUND_READINESS_API_H

#include "common.h"

typedef s32 BtlModelAndSoundReadiness;
enum {
    BTL_MODEL_AND_SOUND_NOT_READY = 0,
    BTL_MODEL_AND_SOUND_READY = 1,
};

/* Whether the model and its associated motion-sound work are complete. */
BtlModelAndSoundReadiness btlEnsureModelAndSoundReady(s32 kind, s32 id, s32 unused);

#endif /* BTL_MODEL_AND_SOUND_READINESS_API_H */
