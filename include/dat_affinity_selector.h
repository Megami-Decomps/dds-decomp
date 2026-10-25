#ifndef DAT_AFFINITY_SELECTOR_H
#define DAT_AFFINITY_SELECTOR_H

#include "common.h"

/* Selector accepted by the affinity lookup: table columns plus built-in
 * default selectors. The flag mapper returns the sentinel for no exact match. */
typedef s32 DatAffinitySelector;
#define DAT_AFFINITY_SELECTOR_UNMAPPED (-1)

/* Map one exact channel flag to an affinity selector. */
DatAffinitySelector datFlagToElementIndex(s32 channelFlag);

#endif /* DAT_AFFINITY_SELECTOR_H */
