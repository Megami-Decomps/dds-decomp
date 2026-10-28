#ifndef MNU_MOVIE_PLAYBACK_API_H
#define MNU_MOVIE_PLAYBACK_API_H

#include "sdf_movie_api.h"

/* Forward the decoder's completion result to menu movie callers. */
SdfMovieCompletionResult mnuIsMoviePlaybackFinished(void);

#endif /* MNU_MOVIE_PLAYBACK_API_H */
