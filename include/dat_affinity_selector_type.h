#ifndef DAT_AFFINITY_SELECTOR_TYPE_H
#define DAT_AFFINITY_SELECTOR_TYPE_H

#include "common.h"

/* Selector accepted by the affinity lookup: table columns plus built-in
 * default selectors. The flag mapper returns the sentinel for no exact match. */
typedef s32 DatAffinitySelector;
#define DAT_AFFINITY_SELECTOR_UNMAPPED (-1)

#endif /* DAT_AFFINITY_SELECTOR_TYPE_H */
