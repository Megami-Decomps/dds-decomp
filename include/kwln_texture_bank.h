#ifndef KWLN_TEXTURE_BANK_H
#define KWLN_TEXTURE_BANK_H

#include "sdf.h"
#include "sdf_scene_packet.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_draw.h"

/* Packet storage for one held-texture render-target frame. */
typedef struct KwlnTextureBank {
    SdfListHead list;
    SdfSceneDrawPacket scene;
    ConsMatrixPacket matrix;
    SdfLightingPacketStorage lighting;
    u8 pad[0x1F40 - 0x260 - sizeof(SdfLightingPacketStorage)];
} KwlnTextureBank;

typedef char KwlnTextureBank_size_must_be_0x1F40[
    (sizeof(KwlnTextureBank) == 0x1F40) ? 1 : -1];
typedef char KwlnTextureBank_scene_offset_must_be_0x20[
    ((u32)&((KwlnTextureBank *)0)->scene == 0x20) ? 1 : -1];
typedef char KwlnTextureBank_matrix_offset_must_be_0x190[
    ((u32)&((KwlnTextureBank *)0)->matrix == 0x190) ? 1 : -1];
typedef char KwlnTextureBank_lighting_offset_must_be_0x260[
    ((u32)&((KwlnTextureBank *)0)->lighting == 0x260) ? 1 : -1];

#endif
