#ifndef MNU_TRANSITION_H
#define MNU_TRANSITION_H

#include "common.h"

typedef u32 (*MenuPopupCallback)();

/* Insert a newly selected popup before the current top transition entry. */
#define MNU_POPUP_INSERT_BEFORE_TOP 0x20000

typedef struct MenuPopupEntry {
    u32 flags;
    MenuPopupCallback enter;
    MenuPopupCallback leave;
    MenuPopupCallback start;
    MenuPopupCallback update;
    MenuPopupCallback finish;
    MenuPopupCallback canEnter;
} MenuPopupEntry;

/* Both games keep sixteen saved entries and the current/previous entry pointers. */
typedef struct MenuPopupState {
    s32 count;
    MenuPopupEntry *entries[16];
    MenuPopupEntry *entryAddress;
    MenuPopupEntry *lastEntryAddress;
} MenuPopupState;

typedef char MenuPopupState_size_must_be_0x4C[
    (sizeof(MenuPopupState) == 0x4C) ? 1 : -1];

void mnuClearPanelTransitionState(MenuPopupState *state);
void mnuDrainPanelTransitions(MenuPopupState *state, u32 callbackArgument);
void mnuBindPresentMenuEntry(MenuPopupState *state, s32 *entrySlot);

#endif /* MNU_TRANSITION_H */
