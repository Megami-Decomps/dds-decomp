#ifndef ITF_MES_WINDOW_H
#define ITF_MES_WINDOW_H

#include "common.h"

u32 itfMesGetWindowFlags(s32 window);
s16 itfMesGetWindowClearBitCount(s32 window);
u32 itfMesGetWindowEntryItems(s32 window, s32 entryIndex);
u32 itfMesGetEntryCount(s32 window);
/* Replace only the high 16 bits, retaining the window's low status bits. */
void itfMesReplaceWindowHighFlags(s32 window, u32 highFlags);
/* Set or clear only bits from the high-half mask. */
void itfMesSetWindowHighFlags(s32 window, u32 mask);
void itfMesClearWindowHighFlags(s32 window, u32 mask);
void itfMesSetWindowCallbackAddress(s32 window, void (*callback)(void));
void itfMesFinishWindowAndClearStatus(s32 window);

#endif /* ITF_MES_WINDOW_H */
