#ifndef MNU_TRANSITION_H
#define MNU_TRANSITION_H

#include "common.h"

typedef u32 (*MenuPopupCallback)();

typedef struct MenuPopupEntry {
    u32 flags;
    MenuPopupCallback enter;
    MenuPopupCallback leave;
    MenuPopupCallback start;
    MenuPopupCallback update;
    MenuPopupCallback finish;
    MenuPopupCallback canEnter;
} MenuPopupEntry;

/* Both games keep sixteen saved entries and the two closed-entry addresses. */
typedef struct MenuPopupState {
    s32 count;
    MenuPopupEntry *entries[16];
    s32 entryAddress;
    s32 lastEntryAddress;
} MenuPopupState;

typedef char MenuPopupState_size_must_be_0x4C[
    (sizeof(MenuPopupState) == 0x4C) ? 1 : -1];

void mnuClearPanelTransitionState(MenuPopupState *state);
void mnuDrainPanelTransitions(MenuPopupState *state, u32 callbackArgument);

#endif /* MNU_TRANSITION_H */
