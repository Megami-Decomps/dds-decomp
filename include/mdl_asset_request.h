#ifndef MDL_ASSET_REQUEST_H
#define MDL_ASSET_REQUEST_H

#include "common.h"

/* Nonblocking requests return 0 when queued, -1 when already pending, or a
 * ready BattleGroupNode address; the blocking model facade casts that address. */
s32 mdlRequestAsset(s32 group, s32 id, s32 blocking);

#endif /* MDL_ASSET_REQUEST_H */
