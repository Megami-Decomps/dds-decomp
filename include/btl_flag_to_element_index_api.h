#ifndef BTL_FLAG_TO_ELEMENT_INDEX_API_H
#define BTL_FLAG_TO_ELEMENT_INDEX_API_H

#include "dat_affinity_selector.h"

/* Map a battle channel flag to an affinity selector or the unmapped sentinel. */
DatAffinitySelector btlFlagToElementIndex(s32 channelFlag);

#endif /* BTL_FLAG_TO_ELEMENT_INDEX_API_H */
