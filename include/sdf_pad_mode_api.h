#ifndef SDF_PAD_MODE_API_H
#define SDF_PAD_MODE_API_H

#include "sdf_pad_index.h"
#include "sdf_pad_mode.h"

/* Store the mode request for a logical pad; setup applies it later. */
void sdfPadChangeMode(SdfPadIndex padIndex, SdfPadMode mode);

#endif /* SDF_PAD_MODE_API_H */
