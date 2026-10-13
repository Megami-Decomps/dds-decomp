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

/* Shared caption row; the final row's baseline also sets each title's
 * staff-roll end threshold. */
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
#ifndef VERSION_DDS2
extern MnuStaffTextRow D_00379F70[256];
#endif

/* Static staff-image timing/rectangle rows. DDS1 also stores the image-bank
 * index consumed when a row starts; DDS2's table has only the six common
 * words. */
typedef struct MnuStaffImageRecord {
    s32 startFrame;
    s32 movieIndex;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
#ifdef VERSION_DDS1
    u32 imageIndex;
#endif
} MnuStaffImageRecord;

#ifdef VERSION_DDS1
typedef char MnuStaffImageRecord_size_must_be_0x1C[
    (sizeof(MnuStaffImageRecord) == 0x1C) ? 1 : -1];
#else
typedef char MnuStaffImageRecord_size_must_be_0x18[
    (sizeof(MnuStaffImageRecord) == 0x18) ? 1 : -1];
#endif

#endif /* MNU_MOVIE_H */
