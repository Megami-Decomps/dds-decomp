#ifndef DDS_SDF_TEXTURE_BLOCK_LIST_H
#define DDS_SDF_TEXTURE_BLOCK_LIST_H

#include "sdf.h"

/* Return a borrowed endpoint of the VRAM block list; callers may inspect but do not own its nodes. */
SdfTexResource *sdfGetTextureBlockListHead(void);

#endif
