#ifndef SDF_SCENE_PACKET_H
#define SDF_SCENE_PACKET_H

#include "sdf.h"
#include "sdf_packet_patch.h"
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

/* Linked metadata precedes the DMA drawing payload by one quadword. */
typedef struct SdfSceneNode {
    SdfPacketPatchLink link;
    SdfGraphObj *view;
    u8 padC[4];
    SdfGsPacketHeader header;
    SdfGsDrawDefaultsRegisters drawDefaults;
    SdfGsContextRegisters contextOne;
    SdfGsContextRegisters contextTwo;
    SdfGsCenteredBoundsRegisters centeredBounds;
    SdfGsSceneBlendRegisters blendState;
    u64 framePacketWords[4]; /* Separate packet whose internals remain opaque. */
    SdfTexBuf texturePackets[2];
} SdfSceneNode;

typedef char SdfSceneNode_size_must_be_0x220[
    (sizeof(SdfSceneNode) == 0x220) ? 1 : -1];
typedef char SdfSceneNode_view_at_0x08[
    ((u32)&((SdfSceneNode *)0)->view == 0x08) ? 1 : -1];
typedef char SdfSceneNode_header_at_0x10[
    ((u32)&((SdfSceneNode *)0)->header == 0x10) ? 1 : -1];
typedef char SdfSceneNode_framePacket_at_0x180[
    ((u32)&((SdfSceneNode *)0)->framePacketWords == 0x180) ? 1 : -1];
typedef char SdfSceneNode_textures_at_0x1A0[
    ((u32)&((SdfSceneNode *)0)->texturePackets == 0x1A0) ? 1 : -1];

void sdfInitSceneNode(SdfSceneNode *node, SdfGraphObj *view);

#endif
