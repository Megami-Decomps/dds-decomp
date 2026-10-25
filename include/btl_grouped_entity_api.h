#ifndef BTL_GROUPED_ENTITY_API_H
#define BTL_GROUPED_ENTITY_API_H

#include "mdl.h"
#include "mdl_registration_mode.h"

struct sdfMemHandle_t;

/* Return the first node of this type in the group, or NULL. */
dds3ModelResrc_t *mdlSearchResrc(s32 group, s32 type);

/* Create or retire a group/type node, applying resource-ownership mode bits. */
void mdlRegist(s32 group, s32 type, MdlRegistrationMode registrationMode,
                        DevRequest *resourceList,
                        struct sdfModelDef_t *itemList,
                        struct sdfMemHandle_t *requestAllocation);
void mdlUnRegistResrc(dds3ModelResrc_t *groupNode);
void mdlUnRegist(s32 group, s32 type);

#endif
