#ifndef KWLN_FRAME_DRAW_BANK_H
#define KWLN_FRAME_DRAW_BANK_H

#include "sdf.h"
#include "sdf_dma_packet_header.h"
#include "sdf_linked_packet.h"
#include "sdf_scene_packet.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_draw.h"
#include "kwln_texture_bank.h"
#include "sdf_draw_packet_groups.h"

/* Two frame banks for the main scene and overlay packet chains. */
typedef struct KwlnFrameDrawBank {
    SdfListHead initialList;             /* 0x0000 */
    DmaPacketHeader programReference;    /* 0x0020 */
    SdfDrawPacketGroups packetGroups[13]; /* 0x0040 */
    SdfListHead sceneList;               /* 0x1630 */
    SdfLinkedPacketList linkedList;       /* 0x1650 */
    SdfSceneNode scene;                  /* 0x1660 */
    ConsMatrixPacket sceneMatrix;         /* 0x1880 */
    u8 scenePacketStorage[0xE0];          /* 0x1950: opaque packet storage */
    KwlnTextureBank heldTexture;          /* 0x1A30: the held texture's packets */
    SdfListHead thirdList;               /* 0x1D70 */
    ConsMatrixPacket thirdMatrix;        /* 0x1D90 */
    SdfLightingPacketStorage overlayLightPacket; /* 0x1E60 */
} KwlnFrameDrawBank;

typedef char KwlnFrameDrawBank_size_must_be_0x1F40[
    (sizeof(KwlnFrameDrawBank) == 0x1F40) ? 1 : -1];
typedef char KwlnFrameDrawBank_initialList_offset_must_be_0[
    ((u32)&((KwlnFrameDrawBank *)0)->initialList == 0) ? 1 : -1];
typedef char KwlnFrameDrawBank_programReference_offset_must_be_0x20[
    ((u32)&((KwlnFrameDrawBank *)0)->programReference == 0x20) ? 1 : -1];
typedef char KwlnFrameDrawBank_packetGroups_offset_must_be_0x40[
    ((u32)&((KwlnFrameDrawBank *)0)->packetGroups == 0x40) ? 1 : -1];
typedef char KwlnFrameDrawBank_sceneList_offset_must_be_0x1630[
    ((u32)&((KwlnFrameDrawBank *)0)->sceneList == 0x1630) ? 1 : -1];
typedef char KwlnFrameDrawBank_linkedList_offset_must_be_0x1650[
    ((u32)&((KwlnFrameDrawBank *)0)->linkedList == 0x1650) ? 1 : -1];
typedef char KwlnFrameDrawBank_scene_offset_must_be_0x1660[
    ((u32)&((KwlnFrameDrawBank *)0)->scene == 0x1660) ? 1 : -1];
typedef char KwlnFrameDrawBank_sceneMatrix_offset_must_be_0x1880[
    ((u32)&((KwlnFrameDrawBank *)0)->sceneMatrix == 0x1880) ? 1 : -1];
typedef char KwlnFrameDrawBank_scenePacketStorage_offset_must_be_0x1950[
    ((u32)&((KwlnFrameDrawBank *)0)->scenePacketStorage == 0x1950) ? 1 : -1];
typedef char KwlnFrameDrawBank_heldTexture_offset_must_be_0x1A30[
    ((u32)&((KwlnFrameDrawBank *)0)->heldTexture == 0x1A30) ? 1 : -1];
typedef char KwlnFrameDrawBank_thirdList_offset_must_be_0x1D70[
    ((u32)&((KwlnFrameDrawBank *)0)->thirdList == 0x1D70) ? 1 : -1];
typedef char KwlnFrameDrawBank_thirdMatrix_offset_must_be_0x1D90[
    ((u32)&((KwlnFrameDrawBank *)0)->thirdMatrix == 0x1D90) ? 1 : -1];
typedef char KwlnFrameDrawBank_overlayLightPacket_offset_must_be_0x1E60[
    ((u32)&((KwlnFrameDrawBank *)0)->overlayLightPacket == 0x1E60) ? 1 : -1];

#endif
