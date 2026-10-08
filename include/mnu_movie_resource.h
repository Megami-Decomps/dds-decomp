#ifndef MNU_MOVIE_RESOURCE_H
#define MNU_MOVIE_RESOURCE_H

#include "sdf.h"

/* Indexed movie-resource row: a filename pointer followed by the decoder's
 * complete 0x14-byte descriptor. The title tables have different extents. */
typedef struct MnuMovieResourceEntry {
    const char *fileName;
    SdfMovieDescriptor descriptor;
} MnuMovieResourceEntry;

typedef char MnuMovieResourceEntry_size_must_be_0x18[
    (sizeof(MnuMovieResourceEntry) == 0x18) ? 1 : -1];
typedef char MnuMovieResourceEntry_descriptor_must_be_at_0x04[
    ((u32)&((MnuMovieResourceEntry *)0)->descriptor == 0x04) ? 1 : -1];

#endif /* MNU_MOVIE_RESOURCE_H */
