#ifndef SDF_GS_REGISTER_SUBMIT_API_H
#define SDF_GS_REGISTER_SUBMIT_API_H

#include "common.h"

/* Append a TEST-register write to the active GS packet stream. */
void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);

#endif
