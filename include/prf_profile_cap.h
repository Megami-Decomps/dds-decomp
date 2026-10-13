#ifndef PRF_PROFILE_CAP_H
#define PRF_PROFILE_CAP_H

#include "common.h"

/* Read the unsigned capacity configured for a profile record. */
u32 prfGetCapValue(u16 profileId);
u32 ptyGetProfileRecordCap(u16 profileId);

#endif
