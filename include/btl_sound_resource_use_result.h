#ifndef BTL_SOUND_RESOURCE_USE_RESULT_H
#define BTL_SOUND_RESOURCE_USE_RESULT_H

#include "common.h"

/* A resource must remain alive while referenced by a task or used by a timed effect. */
typedef s32 BtlSoundResourceUseResult;

#define BTL_SOUND_RESOURCE_NOT_IN_USE 0
#define BTL_SOUND_RESOURCE_IN_USE 1

struct SoundResourceNode;
BtlSoundResourceUseResult sndIsResourceNodeReferencedOrActive(struct SoundResourceNode *resource);

#endif /* BTL_SOUND_RESOURCE_USE_RESULT_H */
