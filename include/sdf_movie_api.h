#ifndef SDF_MOVIE_API_H
#define SDF_MOVIE_API_H

#include "sdf.h"

/* Copies the caller-owned descriptor into the stream and does not retain it. */
void movieOpenStream(movieStream_t *, const SdfMovieDescriptor *, const char *);

#endif /* SDF_MOVIE_API_H */
