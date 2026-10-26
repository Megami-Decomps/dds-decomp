#ifndef ITF_MES_WINDOW_H
#define ITF_MES_WINDOW_H

#include "common.h"
#include "itf_mes_entry_items_result.h"
#include "itf_mes_window_index.h"

struct ItfMesSub;

ItfMesWindowIndex itfMesMngInitialize(struct ItfMesSub *sub);
void itfMesMngRelease(ItfMesWindowIndex window);
u32 itfMesMngGetStat(ItfMesWindowIndex window);
s16 itfMesMngGetSelect(ItfMesWindowIndex window);
/* Return the entry's item-list address, NONE, or the BRANCH sentinel. */
ItfMesEntryItemsResult itfMesGetWindowEntryItems(ItfMesWindowIndex window, s32 entryIndex);
u32 itfMesGetEntryCount(ItfMesWindowIndex window);
/* Borrow window-owned replacement text; valid until slot replacement or window release. */
const u8 *itfMesMngGetVar(ItfMesWindowIndex window, s32 slotIndex);
/* Replace only the high 16 bits, retaining the window's low status bits. */
void itfMesMngSetStat(ItfMesWindowIndex window, u32 highFlags);
/* Set or clear only bits from the high-half mask. */
void itfMesMngMarkBitStat(ItfMesWindowIndex window, u32 mask);
void itfMesMngClearBitStat(ItfMesWindowIndex window, u32 mask);
void itfMesMngSetMaskSelect(ItfMesWindowIndex window, u32 panelMask);
void itfMesMngSetSelect(ItfMesWindowIndex window, s32 selectedBitIndex);
void itfMesMngChangeWindowType(ItfMesWindowIndex window, s32 firstValue, s32 secondValue);
void itfMesMngSetKutiCallback(ItfMesWindowIndex window, void (*callback)(void));
void itfMesMngMsgWndCls(ItfMesWindowIndex window);
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
