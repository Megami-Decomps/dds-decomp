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

#ifndef VERSION_DDS2
/* Complete caption table. The final row's baseline also sets the roll's end
 * threshold (the native staff update reads row 255 at table offset 0xFFC). */
typedef struct MnuStaffTextRow {
    const char *leftText;
    const char *rightText;
    s8 leftFont;
    s8 rightFont;
    u8 pad0A[2];
    s32 offsetY;
} MnuStaffTextRow;

typedef char MnuStaffTextRow_size_must_be_0x10[
    (sizeof(MnuStaffTextRow) == 0x10) ? 1 : -1];
extern MnuStaffTextRow D_00379F70[256];
#endif

#endif /* MNU_MOVIE_H */
