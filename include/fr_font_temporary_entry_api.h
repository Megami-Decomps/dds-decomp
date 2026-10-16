#ifndef FR_FONT_TEMPORARY_ENTRY_API_H
#define FR_FONT_TEMPORARY_ENTRY_API_H

#include "common.h"

/* Bind caller-owned, relocated font resource bytes into the temporary slot. */
void frFontLoadTemporaryEntry(u8 *fontData);

#endif /* FR_FONT_TEMPORARY_ENTRY_API_H */
