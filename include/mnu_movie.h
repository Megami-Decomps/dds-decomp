#ifndef MNU_MOVIE_H
#define MNU_MOVIE_H

#include "common.h"

/* Shared DDS1 movie-roll row; updated by the roll scheduler and rendered by
 * the movie draw task. */
typedef struct MnuMovieRollEntry {
    f32 x;
    f32 y;
    s16 timer;
    s16 duration;
    s8 active;
    u8 pad0D[3];
    f32 velocity;
    s32 sprite;
    s32 column;
} MnuMovieRollEntry;

typedef char MnuMovieRollEntry_size_must_be_0x1C[
    (sizeof(MnuMovieRollEntry) == 0x1C) ? 1 : -1];

extern MnuMovieRollEntry D_003DC1E0[];

#endif /* MNU_MOVIE_H */
