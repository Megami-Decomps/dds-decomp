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
void itfMesSetWindowPanelValue(s32 window, u32 panelMask);
void itfMesCountClearBits(s32 window, s32 selectedBitIndex);
void itfMesSetWindowPageAndRefresh(s32 window, s32 firstValue, s32 secondValue);
void itfMesSetWindowCallbackAddress(s32 window, void (*callback)(void));
void itfMesFinishWindowAndClearStatus(s32 window);

struct SdfTex;
struct SdfTex *itfMesGetGlobalWindowValue(void);

#endif /* ITF_MES_WINDOW_H */
