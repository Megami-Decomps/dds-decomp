#ifndef FLD_AREA_ROOM_RESOURCE_LOAD_API_H
#define FLD_AREA_ROOM_RESOURCE_LOAD_API_H

struct sdfMemHandle_t;

/* Returns the resolver-owned F1 allocation as a borrowed alias, or NULL.
 * fldReleaseFieldResources controls its lifetime and releases the allocation. */
const struct sdfMemHandle_t *fldLoadAreaRoomResources(const char *resourceName);

#endif /* FLD_AREA_ROOM_RESOURCE_LOAD_API_H */
