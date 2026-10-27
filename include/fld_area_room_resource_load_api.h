#ifndef FLD_AREA_ROOM_RESOURCE_LOAD_API_H
#define FLD_AREA_ROOM_RESOURCE_LOAD_API_H

#include "sdf_texture_offset_list.h"

struct sdfMemHandle_t;

/* Returns the resolver-owned F1 allocation as a borrowed alias, or NULL.
 * fldReleaseFieldResources controls its lifetime and releases the allocation. */
const struct sdfMemHandle_t *fldLoadAreaRoomResources(const char *resourceName);

/* Builds field resources from caller-owned buffers. The two mutable resource
 * buffers remain borrowed by the resolver until field release so it can undo
 * their address relocation. The returned handle is likewise a borrowed alias;
 * fldReleaseFieldResources owns the allocation's release. */
const struct sdfMemHandle_t *fldLoadAreaRoomResourcesFromRequest(
    u32 area, u32 room, u8 *resource0Data, u8 *resource1Data,
    const SdfTextureOffsetListHeader *textureOffsets, u32 flags);

#endif /* FLD_AREA_ROOM_RESOURCE_LOAD_API_H */
