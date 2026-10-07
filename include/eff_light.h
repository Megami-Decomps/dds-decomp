#ifndef EFF_LIGHT_H
#define EFF_LIGHT_H

#include "dds3obj.h"

/* Complete 0x7C-byte payload owned by kind-9 world nodes. */
typedef struct EffLightData {
    f32 ambientColor[3];
    u32 word0C;
    u32 word10;
    f32 value14;
    f32 value18;
    u32 word1C;
    u8 pad20[0x20];
    f32 secondaryColor[4];
    f32 parameter50;
    f32 parameter54;
    f32 parameter58;
    u32 word5C;
    u32 mode;
    u32 flags;
    u32 packedColorA;
    u32 packedColorB;
    u16 blendFrames;
    u16 unk72;
    ObjBase *resourceState;
    void *buffer;
} EffLightData;

typedef char EffLightData_size_must_be_0x7C[(sizeof(EffLightData) == 0x7C) ? 1 : -1];
typedef char EffLightData_flags_at_0x64[((u32)&((EffLightData *)0)->flags == 0x64) ? 1 : -1];
typedef char EffLightData_resource_at_0x74[((u32)&((EffLightData *)0)->resourceState == 0x74) ? 1 : -1];

#endif
