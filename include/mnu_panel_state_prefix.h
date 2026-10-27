#ifndef MNU_PANEL_STATE_PREFIX_H
#define MNU_PANEL_STATE_PREFIX_H

#include "mnu_panel_state.h"

typedef struct MenuPanelStatePrefix {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    MenuPanelSelectionIndex selectedIndex;
    u32 firstValueA;
    u32 firstValueB;
} MenuPanelStatePrefix;

typedef char MenuPanelStatePrefixSizeCheck[(sizeof(MenuPanelStatePrefix) == 0x20) ? 1 : -1];
typedef char MenuPanelStatePrefixWidthOffsetCheck[((u32)&((MenuPanelStatePrefix *)0)->width == 0x0C) ? 1 : -1];
typedef char MenuPanelStatePrefixHeightOffsetCheck[((u32)&((MenuPanelStatePrefix *)0)->height == 0x10) ? 1 : -1];
typedef char MenuPanelStatePrefixSelectionIndexOffsetCheck[((u32)&((MenuPanelStatePrefix *)0)->selectedIndex == 0x14) ? 1 : -1];
typedef char MenuPanelStatePrefixFirstValueAOffsetCheck[((u32)&((MenuPanelStatePrefix *)0)->firstValueA == 0x18) ? 1 : -1];
typedef char MenuPanelStatePrefixFirstValueBOffsetCheck[((u32)&((MenuPanelStatePrefix *)0)->firstValueB == 0x1C) ? 1 : -1];

#endif
