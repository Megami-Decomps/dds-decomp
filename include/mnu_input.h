#ifndef MNU_INPUT_H
#define MNU_INPUT_H

#include "common.h"

/* Button queries and the logical actions they produce are separate masks. */
typedef u32 MnuPadButtonQueryMask;
typedef u32 MnuInputActionFlags;

/* These mapped actions request whole-list page navigation when available. */
#define MNU_INPUT_ACTION_PREVIOUS_PAGE 0x00000400u
#define MNU_INPUT_ACTION_LAST_PAGE 0x00000800u

/*
 * Sample the requested native pad buttons and return their menu actions.
 * If query bit zero is requested and active, it suppresses all other actions.
 */
MnuInputActionFlags mnuMapPadMaskToFlags(MnuPadButtonQueryMask requestedButtons);

#ifdef VERSION_DDS2
struct MenuWindowContainer;
void mnuHandleListPageJumpInput(const void *resource,
                                struct MenuWindowContainer *window,
                                MnuInputActionFlags *actionFlags);
void mnuHandlePanelListPageJumpInput(struct MenuWindowContainer *window,
                                     MnuInputActionFlags *actionFlags);
#endif

#endif
