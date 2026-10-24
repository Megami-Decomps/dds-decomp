#ifndef ITF_TEXT_SLOTS_API_H
#define ITF_TEXT_SLOTS_API_H

#include "common.h"

/* Test the active text-interface flags and return the requested bits. */
u32 frGetMessageResult(u32 mask);

/* Store read-only encoded text in the indexed font-text slot table. */
void frSetVariableRequest(s32 slotIndex, const u8 *encodedText);

#endif /* ITF_TEXT_SLOTS_API_H */
