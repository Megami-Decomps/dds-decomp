#ifndef MNU_INPUT_H
#define MNU_INPUT_H

#include "common.h"

/* Translate a requested pad-button mask into menu input flags. */
s32 mnuMapPadMaskToFlags(s32 buttonMask);

#ifdef VERSION_DDS2
struct MenuWindowContainer;
void mnuHandleListPageJumpInput(s32 active, struct MenuWindowContainer *window,
                                u32 *buttons);
void mnuHandlePanelListPageJumpInput(struct MenuWindowContainer *window,
                                     u32 *buttons);
#endif

#endif
