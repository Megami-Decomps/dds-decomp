#ifndef MDL_GROUP_RESOURCE_OWNERSHIP_H
#define MDL_GROUP_RESOURCE_OWNERSHIP_H

#include "common.h"

/* Persistent ownership state resolved from the registration mode bit. */
typedef u8 MdlGroupResourceOwnership;

/* Owned groups release the shared list, request allocation, and per-motion
 * handles; borrowed groups leave those with their external owner. The separate
 * part list and group handle have their own unconditional teardown. */
#define MDL_GROUP_RESOURCES_BORROWED 0
#define MDL_GROUP_RESOURCES_OWNED 1

#endif /* MDL_GROUP_RESOURCE_OWNERSHIP_H */
