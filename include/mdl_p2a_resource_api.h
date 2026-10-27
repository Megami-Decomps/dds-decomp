#ifndef MDL_P2A_RESOURCE_API_H
#define MDL_P2A_RESOURCE_API_H

#include "sdf_draw.h"

/* Consume packet-backed P2A bytes synchronously; callers may release the
 * source allocation after this builder returns. */
void mdlCreateEffectResrc_P2A(DevRequest *partList, const u8 *resourceBytes);

#endif /* MDL_P2A_RESOURCE_API_H */
