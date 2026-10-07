#ifndef BTL_SOUND_H
#define BTL_SOUND_H

#include "common.h"

/* Shared 0x14-byte system-effect node prefix. Actor tasks maintain the
 * reference count at +4; timed effects maintain the active count at +8. */
typedef struct SoundEffectNode {
    u32 flags;
    s32 referenceCount;     /* 0x04 */
    u32 activeCount;        /* 0x08 */
    u8 padC[4];
    u32 handle;
} SoundEffectNode;

#endif /* BTL_SOUND_H */
