#ifndef MNU_PANEL_SPRITE_H
#define MNU_PANEL_SPRITE_H

#include "common.h"
#include "mnu.h"

/* A sprite drawn from a panel's resource sets: which set, its slot, and its
 * offset from the panel's origin. Panels copy constant tables of these. */
typedef struct MenuPanelSprite {
    s32 set;
    s32 slot;
    MenuPoint offset;
} MenuPanelSprite;

/* A sprite that travels from one offset to another as its phase advances. */
typedef struct MenuPanelSpritePath {
    s32 set;
    s32 slot;
    MenuPoint from;
    MenuPoint to;
} MenuPanelSpritePath;

typedef char MenuPanelSprite_size_check[(sizeof(MenuPanelSprite) == 0x10) ? 1 : -1];
typedef char MenuPanelSpritePath_size_check[(sizeof(MenuPanelSpritePath) == 0x18) ? 1 : -1];

#endif
