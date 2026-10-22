#ifndef BRS_ICON_RECORD_H
#define BRS_ICON_RECORD_H

#include "common.h"

/* A placed menu icon: four halfwords. The menus' icon tables share the layout. */
typedef s16 BrsIconRecord[4];

enum {
    BRS_ICON_TEXTURE = 0, /* the sprite set, of the scene's effect slots */
    BRS_ICON_ID = 1,      /* the sprite within it */
    BRS_ICON_X = 2,       /* in pixels; drawn at x << 4 */
    BRS_ICON_Y = 3,       /* in pixels; drawn at y << 3 */
};

/* The menus' common icons, placed. */
extern BrsIconRecord mnuCommonIconPlacements[];

#endif
