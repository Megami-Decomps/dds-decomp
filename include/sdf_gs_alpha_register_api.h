#ifndef SDF_GS_ALPHA_REGISTER_API_H
#define SDF_GS_ALPHA_REGISTER_API_H

#include "common.h"

/* Submit a single GS ALPHA-register value through the active packet stream. */
void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

#endif
