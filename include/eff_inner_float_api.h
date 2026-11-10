#ifndef EFF_INNER_FLOAT_API_H
#define EFF_INNER_FLOAT_API_H

#include "eff_transform.h"

/* Set the radius stored by the world node's inner transform.
 * Providers: basic/dds3Basic.c in both games. */
void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value);

#endif /* EFF_INNER_FLOAT_API_H */
