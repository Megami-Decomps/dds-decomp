#ifndef SDF_SCENE_PACKET_H
#define SDF_SCENE_PACKET_H

#include "sdf_gs_context.h"
#include "sdf_gs_scene_state.h"

struct SdfGraphObj;

/* Complete DMA/GIF owner used by scene builders and held-texture draw banks. */
typedef struct SdfSceneDrawPacket {
    SdfGsPacketHeader header;
    SdfGsDrawDefaultsRegisters drawDefaults;
    SdfGsContextRegisters contextOne;
    SdfGsContextRegisters contextTwo;
    SdfGsCenteredBoundsRegisters centeredBounds;
    SdfGsSceneBlendRegisters blendState;
} SdfSceneDrawPacket;

typedef char SdfSceneDrawPacket_size_must_be_0x170[
    (sizeof(SdfSceneDrawPacket) == 0x170) ? 1 : -1];
typedef char SdfSceneDrawPacket_defaults_at_0x20[
    ((u32)&((SdfSceneDrawPacket *)0)->drawDefaults == 0x20) ? 1 : -1];
typedef char SdfSceneDrawPacket_contextOne_at_0x60[
    ((u32)&((SdfSceneDrawPacket *)0)->contextOne == 0x60) ? 1 : -1];
typedef char SdfSceneDrawPacket_contextTwo_at_0xA0[
    ((u32)&((SdfSceneDrawPacket *)0)->contextTwo == 0xA0) ? 1 : -1];
typedef char SdfSceneDrawPacket_centeredBounds_at_0xE0[
    ((u32)&((SdfSceneDrawPacket *)0)->centeredBounds == 0xE0) ? 1 : -1];
typedef char SdfSceneDrawPacket_blendState_at_0x130[
    ((u32)&((SdfSceneDrawPacket *)0)->blendState == 0x130) ? 1 : -1];

void sdfBuildTextureScenePacket(SdfSceneDrawPacket *packet,
    struct SdfGraphObj *view, s32 bufferIndex);

#endif
