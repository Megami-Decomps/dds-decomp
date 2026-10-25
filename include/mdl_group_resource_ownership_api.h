#ifndef MDL_GROUP_RESOURCE_OWNERSHIP_API_H
#define MDL_GROUP_RESOURCE_OWNERSHIP_API_H

#include "mdl.h"
#include "mdl_group_resource_ownership.h"

/* Narrow typed view of the ownership state persisted in the group record. */
static inline MdlGroupResourceOwnership mdlGetGroupResourceOwnership(
    const dds3ModelResrc_t *groupNode) {
    return groupNode->ownsResources;
}

static inline void mdlSetGroupResourceOwnership(
    dds3ModelResrc_t *groupNode, MdlGroupResourceOwnership ownership) {
    groupNode->ownsResources = ownership;
}

#endif /* MDL_GROUP_RESOURCE_OWNERSHIP_API_H */
