#ifndef SDF_MOVIE_API_H
#define SDF_MOVIE_API_H

#include "sdf.h"

/* Completion requires drained input and no completed output buffers pending. */
typedef s32 SdfMovieCompletionResult;
enum {
    SDF_MOVIE_COMPLETION_INCOMPLETE = 0,
    SDF_MOVIE_COMPLETION_FINISHED = 1
};

/* Copies the caller-owned descriptor into the stream and does not retain it. */
void movieOpenStream(movieStream_t *, const SdfMovieDescriptor *, const char *);
SdfMovieCompletionResult sdfMovieIsFinished(movieStream_t *);

#endif /* SDF_MOVIE_API_H */
