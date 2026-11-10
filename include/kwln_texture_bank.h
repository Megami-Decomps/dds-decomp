#ifndef KWLN_TEXTURE_BANK_H
#define KWLN_TEXTURE_BANK_H

#include "sdf.h"
#include "sdf_scene_packet.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_draw.h"

/* The held texture's packets in one frame bank (dds3FrameDrawBank_t::heldTexture). */
typedef struct dds3TextureBank_t {
    sdfDrawTag_t list;
    SdfSceneDrawPacket scene;
    ConsMatrixPacket matrix;
    sdf3DLightParallelPacket_t lighting;
} dds3TextureBank_t;

typedef char dds3TextureBank_size_must_be_0x340[
    (sizeof(dds3TextureBank_t) == 0x340) ? 1 : -1];
typedef char dds3TextureBank_scene_offset_must_be_0x20[
    ((u32)&((dds3TextureBank_t *)0)->scene == 0x20) ? 1 : -1];
typedef char dds3TextureBank_matrix_offset_must_be_0x190[
    ((u32)&((dds3TextureBank_t *)0)->matrix == 0x190) ? 1 : -1];
typedef char dds3TextureBank_lighting_offset_must_be_0x260[
    ((u32)&((dds3TextureBank_t *)0)->lighting == 0x260) ? 1 : -1];

#endif
