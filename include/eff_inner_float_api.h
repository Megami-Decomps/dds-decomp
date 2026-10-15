#ifndef EFF_INNER_FLOAT_API_H
#define EFF_INNER_FLOAT_API_H

#include "eff_transform.h"

/* Set the radius stored by the world node's inner transform.
 * Providers: DDS1 game/code_0010EEF0.c and DDS2 game/code_0010F118.c. */
void effObjSetInnerFloat(EffWorldNode *node, f32 value);

#endif /* EFF_INNER_FLOAT_API_H */
