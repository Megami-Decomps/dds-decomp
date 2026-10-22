#ifndef DDS_SDF_TEXTURE_LISTS_H
#define DDS_SDF_TEXTURE_LISTS_H

#include "sdf.h"

/* Return borrowed endpoints; callers may inspect list nodes but do not own them. */
sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);
sdfGsMemSlice_t *sdfGsMemGetLinkTail(void);

#endif
