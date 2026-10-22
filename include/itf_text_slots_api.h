#ifndef ITF_TEXT_SLOTS_API_H
#define ITF_TEXT_SLOTS_API_H

#include "common.h"

/* Test the active text-interface flags and return the requested bits. */
u32 frGetMessageResult(u32 mask);

/* Store an encoded text address word in the indexed font-text slot table. */
void frSetVariableRequest(s32 slotIndex, s32 encodedTextAddress);

#endif /* ITF_TEXT_SLOTS_API_H */
