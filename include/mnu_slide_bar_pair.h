#ifndef MNU_SLIDE_BAR_PAIR_H
#define MNU_SLIDE_BAR_PAIR_H

#include "common.h"

/* Title-menu work shared by paired slide-bar update and flag helpers. */
typedef struct MnuSlideBarPair {
    u8 active[2];
    u8 padding[2];
    s32 position[2];
} MnuSlideBarPair;

typedef char MnuSlideBarPair_size_must_be_0x0C[
    sizeof(MnuSlideBarPair) == 0x0C ? 1 : -1];
typedef char MnuSlideBarPair_active_at_0[
    (u32)&((MnuSlideBarPair *)0)->active == 0 ? 1 : -1];
typedef char MnuSlideBarPair_position_at_4[
    (u32)&((MnuSlideBarPair *)0)->position == 4 ? 1 : -1];

#endif /* MNU_SLIDE_BAR_PAIR_H */
