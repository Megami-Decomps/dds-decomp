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
typedef struct dds3FrameDrawBank_t {
    sdfDrawTag_t initialList;             /* 0x0000 */
    DmaPacketHeader programReference;    /* 0x0020 */
    SdfDrawPacketGroups packetGroups[13]; /* 0x0040 */
    sdfDrawTag_t sceneList;               /* 0x1630 */
    sdfPreDrawTag_t linkedList;       /* 0x1650 */
    SdfSceneNode scene;                  /* 0x1660 */
    ConsMatrixPacket sceneMatrix;         /* 0x1880 */
    sdf3DLightParallelPacket_t scenePacketStorage; /* 0x1950 */
    dds3TextureBank_t heldTexture;          /* 0x1A30: the held texture's packets */
    sdfDrawTag_t thirdList;               /* 0x1D70 */
    ConsMatrixPacket thirdMatrix;        /* 0x1D90 */
    sdf3DLightParallelPacket_t overlayLightPacket; /* 0x1E60 */
} dds3FrameDrawBank_t;

typedef char dds3FrameDrawBank_size_must_be_0x1F40[
    (sizeof(dds3FrameDrawBank_t) == 0x1F40) ? 1 : -1];
typedef char dds3FrameDrawBank_initialList_offset_must_be_0[
    ((u32)&((dds3FrameDrawBank_t *)0)->initialList == 0) ? 1 : -1];
typedef char dds3FrameDrawBank_programReference_offset_must_be_0x20[
    ((u32)&((dds3FrameDrawBank_t *)0)->programReference == 0x20) ? 1 : -1];
typedef char dds3FrameDrawBank_packetGroups_offset_must_be_0x40[
    ((u32)&((dds3FrameDrawBank_t *)0)->packetGroups == 0x40) ? 1 : -1];
typedef char dds3FrameDrawBank_sceneList_offset_must_be_0x1630[
    ((u32)&((dds3FrameDrawBank_t *)0)->sceneList == 0x1630) ? 1 : -1];
typedef char dds3FrameDrawBank_linkedList_offset_must_be_0x1650[
    ((u32)&((dds3FrameDrawBank_t *)0)->linkedList == 0x1650) ? 1 : -1];
typedef char dds3FrameDrawBank_scene_offset_must_be_0x1660[
    ((u32)&((dds3FrameDrawBank_t *)0)->scene == 0x1660) ? 1 : -1];
typedef char dds3FrameDrawBank_sceneMatrix_offset_must_be_0x1880[
    ((u32)&((dds3FrameDrawBank_t *)0)->sceneMatrix == 0x1880) ? 1 : -1];
typedef char dds3FrameDrawBank_scenePacketStorage_offset_must_be_0x1950[
    ((u32)&((dds3FrameDrawBank_t *)0)->scenePacketStorage == 0x1950) ? 1 : -1];
typedef char dds3FrameDrawBank_heldTexture_offset_must_be_0x1A30[
    ((u32)&((dds3FrameDrawBank_t *)0)->heldTexture == 0x1A30) ? 1 : -1];
typedef char dds3FrameDrawBank_thirdList_offset_must_be_0x1D70[
    ((u32)&((dds3FrameDrawBank_t *)0)->thirdList == 0x1D70) ? 1 : -1];
typedef char dds3FrameDrawBank_thirdMatrix_offset_must_be_0x1D90[
    ((u32)&((dds3FrameDrawBank_t *)0)->thirdMatrix == 0x1D90) ? 1 : -1];
typedef char dds3FrameDrawBank_overlayLightPacket_offset_must_be_0x1E60[
    ((u32)&((dds3FrameDrawBank_t *)0)->overlayLightPacket == 0x1E60) ? 1 : -1];

#endif
