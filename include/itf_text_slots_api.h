#ifndef ITF_TEXT_SLOTS_API_H
#define ITF_TEXT_SLOTS_API_H

#include "common.h"

/* Test the active text-interface flags and return the requested bits. */
u32 itfTestTextInterfaceMask(u32 mask);

/* Store an encoded text address word in the indexed font-text slot table. */
void itfSetIndexedTextSlot(s32 slotIndex, s32 encodedTextAddress);

#endif /* ITF_TEXT_SLOTS_API_H */
