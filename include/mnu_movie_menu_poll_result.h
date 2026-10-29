#ifndef MNU_MOVIE_MENU_POLL_RESULT_H
#define MNU_MOVIE_MENU_POLL_RESULT_H

#include "common.h"

/* The idle-timeout result is emitted after 601 ticks without menu input. */
typedef s32 MnuMovieMenuPollResult;
enum {
    MNU_MOVIE_MENU_POLL_IDLE_TIMEOUT = -1,
    MNU_MOVIE_MENU_POLL_WAITING = 0,
    MNU_MOVIE_MENU_POLL_ADVANCE = 1,
};

MnuMovieMenuPollResult mnuPollMovieMenuInputAndTimeout(void);

#endif /* MNU_MOVIE_MENU_POLL_RESULT_H */
