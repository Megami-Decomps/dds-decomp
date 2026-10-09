#ifndef SDF_REQUEST_H
#define SDF_REQUEST_H

#include "sdf_draw.h"

/* Operations on the complete buffered-request owner, distinct from draw packets. */
void sdfDestroyDevRequest(DevRequest *request);

#endif
