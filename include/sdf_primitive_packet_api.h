#ifndef SDF_PRIMITIVE_PACKET_API_H
#define SDF_PRIMITIVE_PACKET_API_H

#include "sdf_primitive.h"

/* Build the serialized VIF attribute stream and return its word cursor. */
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

#endif /* SDF_PRIMITIVE_PACKET_API_H */
