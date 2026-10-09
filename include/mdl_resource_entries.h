#ifndef MDL_RESOURCE_ENTRIES_H
#define MDL_RESOURCE_ENTRIES_H

#include "mdl.h"

/* Apply a viewer record list to its owning model context. */
void mdlApplyResourceEntries(MdlCtx *owner, s32 recordId, s32 subtype);

#endif /* MDL_RESOURCE_ENTRIES_H */
