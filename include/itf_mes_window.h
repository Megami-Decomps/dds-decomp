#ifndef ITF_MES_WINDOW_H
#define ITF_MES_WINDOW_H

#include "common.h"

u32 itfMesMngGetStat(s32 window);
s16 itfMesMngGetSelect(s32 window);
/* Return an entry's relocated item-address table; address 1 marks a branch. */
u32 *itfMesGetWindowEntryItems(s32 window, s32 entryIndex);
u32 itfMesGetEntryCount(s32 window);
/* Return the replacement text stored in a window slot. */
u8 *itfMesMngGetVar(s32 window, s32 slotIndex);
/* Replace only the high 16 bits, retaining the window's low status bits. */
void itfMesMngSetStat(s32 window, u32 highFlags);
/* Set or clear only bits from the high-half mask. */
void itfMesMngMarkBitStat(s32 window, u32 mask);
void itfMesMngClearBitStat(s32 window, u32 mask);
void itfMesMngSetMaskSelect(s32 window, u32 panelMask);
void itfMesMngSetSelect(s32 window, s32 selectedBitIndex);
void itfMesMngChangeWindowType(s32 window, s32 firstValue, s32 secondValue);
void itfMesMngSetKutiCallback(s32 window, void (*callback)(void));
void itfMesMngMsgWndCls(s32 window);
void evtLipsExecFunction(s32 unitUniqueId, s32 motionId);

struct ItfMesBlk40;
struct ItfMesBlkA4;
struct ItfMesTextSlots;

/* Initialize the cursor state used by a newly reset message window. */
void itfInitializeCursorResetState(struct ItfMesBlk40 *cursor);
/* Release the window's three sprites and restore its default bounds/fade limit. */
void itfResetWindowResourceBlock(struct ItfMesBlkA4 *block);
/* Adjust the panel fade limit from pad input and mirror it to the sprite. */
void DebugMoveWinAlpha(struct ItfMesBlkA4 *panel);
/* Clear the 32 encoded-text address slots owned by a message window. */
void InitializeVariable(struct ItfMesTextSlots *slots);

struct sdfTexHandle_t;
struct sdfTexHandle_t *itfMesGetWindowTexture(void);

#endif /* ITF_MES_WINDOW_H */
