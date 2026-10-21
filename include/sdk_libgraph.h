#ifndef SDK_LIBGRAPH_H
#define SDK_LIBGRAPH_H

/* SDK graphics synthesizer library (libgraph). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

typedef struct sceGsStoreImage sceGsStoreImage;

#ifndef SDK_SKIP_sceGsExecLoadImage
void sceGsExecLoadImage(void *image, s32 source);
#endif
#ifndef SDK_SKIP_sceGsExecStoreImage
s32 sceGsExecStoreImage(sceGsStoreImage *image, void *destination);
#endif
#ifndef SDK_SKIP_sceGsPutDispEnv
void sceGsPutDispEnv(void *env);
#endif
#ifndef SDK_SKIP_sceGsResetGraph
void sceGsResetGraph(s32 mode, s32 interlace, s32 outputMode, s32 ffmode);
#endif
#ifndef SDK_SKIP_sceGsResetPath
void sceGsResetPath(void);
#endif
#ifndef SDK_SKIP_sceGsSetDefDispEnv
void sceGsSetDefDispEnv(void *env, s32 psm, s32 width, s32 height, s32 dx, s32 dy);
#endif
#ifndef SDK_SKIP_sceGsSetDefLoadImage
void sceGsSetDefLoadImage(void *image, s16 base, s16 width, s32 psm, s32 x, s32 y, s16 w, s16 h);
#endif
#ifndef SDK_SKIP_sceGsSetDefStoreImage
s32 sceGsSetDefStoreImage(sceGsStoreImage *image, s16 base, s16 width, s16 psm, s16 x, s16 y, s16 w, s16 h);
#endif
#ifndef SDK_SKIP_sceGsSyncPath
s32 sceGsSyncPath(s32 mode, s32 timeout);
#endif

#endif
