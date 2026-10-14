#ifndef MNU_MOVIE_LIST_NODE_H
#define MNU_MOVIE_LIST_NODE_H

#include "common.h"

/* The movie-list builder allocates a size-class block per entry and appends
 * the complete NUL-terminated path after the link. The allocation extent is
 * path-dependent, so this view intentionally does not describe its full size. */
typedef struct MnuMovieListNode {
    struct MnuMovieListNode *next; /* 0x00 */
    char path[1];                  /* 0x04: variable-length path tail */
} MnuMovieListNode;

#endif /* MNU_MOVIE_LIST_NODE_H */
