#ifndef ITF_OPTION_BIT_INDEX_API_H
#define ITF_OPTION_BIT_INDEX_API_H

#include "common.h"

/* Index of an option bit in the message-selection mask. */
typedef s32 ItfMesOptionBitIndex;

/* Zero-based ordinal among the options enabled by clear mask bits. */
typedef s32 ItfMesVisibleOptionOrdinal;

#define ITF_MES_OPTION_BIT_INDEX_NOT_FOUND ((ItfMesOptionBitIndex)0x20)

/* Map a visible option ordinal to its clear bit, or return the 32-bit mask
 * width when that ordinal is absent. */
ItfMesOptionBitIndex itfMesFindOptionBitIndex(ItfMesVisibleOptionOrdinal optionOrdinal, u32 optionMask);

#endif /* ITF_OPTION_BIT_INDEX_API_H */
