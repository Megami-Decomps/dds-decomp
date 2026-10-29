#ifndef SND_TRACK_LOAD_ACTIVITY_H
#define SND_TRACK_LOAD_ACTIVITY_H

#include "common.h"

typedef s32 SndTrackLoadActivity;

#define SND_TRACK_LOAD_INACTIVE ((SndTrackLoadActivity)0)
#define SND_TRACK_LOAD_ACTIVE ((SndTrackLoadActivity)1)

/* Report whether any sound-slot owner is currently loading a packed track. */
SndTrackLoadActivity sndHasActiveFileLoad(void);

#endif /* SND_TRACK_LOAD_ACTIVITY_H */
