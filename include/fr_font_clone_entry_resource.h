#ifndef FR_FONT_CLONE_ENTRY_RESOURCE_H
#define FR_FONT_CLONE_ENTRY_RESOURCE_H

#include "common.h"

/* Decode one packed font entry into an owned byte buffer. */
u8 *frFontCloneEntryResource(u8 fontIndex, s32 entryIndex);

#endif /* FR_FONT_CLONE_ENTRY_RESOURCE_H */
