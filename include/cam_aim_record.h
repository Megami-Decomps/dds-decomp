#ifndef CAM_AIM_RECORD_H
#define CAM_AIM_RECORD_H

#include "common.h"

typedef struct CamAim {
    u8 opaque00[0x50];
    f32 quat[4];
    u8 opaque60[8];
    u32 flags;
} CamAim;

typedef char CamAimSizeCheck[(sizeof(CamAim) == 0x6C) ? 1 : -1];
typedef char CamAimQuaternionOffsetCheck[((u32)&((CamAim *)0)->quat == 0x50) ? 1 : -1];
typedef char CamAimFlagsOffsetCheck[((u32)&((CamAim *)0)->flags == 0x68) ? 1 : -1];

#endif
