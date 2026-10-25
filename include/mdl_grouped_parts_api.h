#ifndef MDL_GROUPED_PARTS_API_H
#define MDL_GROUPED_PARTS_API_H

#include "sdf_pac_work.h"
#include "mdl_registration_mode.h"

/* Apply a grouped packet sequence with its registration mode and return the
 * packet after that sequence. */
PacWork *mdlPackSet(PacWork *packetWork, s32 group, s32 id, MdlRegistrationMode registrationMode);

#endif /* MDL_GROUPED_PARTS_API_H */
