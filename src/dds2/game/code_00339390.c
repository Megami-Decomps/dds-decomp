#include "sdf_gs_header.h"
#include "common.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_alloc.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "sdf_primitive.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf_texture_file.h"
#include "sdf_vu_blend_node.h"

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF



typedef struct {
    f32 matrix[4][4];      /* 0x00 */
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50: geometry references submitted to VU in chunks */
    u8 pad52[2];
    SdfAssetEntry *asset;  /* 0x54 */
    u32 packedColor;       /* 0x58: packed RGBA word */
    f32 offsetX;           /* 0x5C */
    f32 offsetY;           /* 0x60 */
    u32 ringSrc;           /* 0x64 */
    u32 ringDst;           /* 0x68 */
    u32 ringWrap;          /* 0x6C */
    u32 ringWrapCount;     /* 0x70 */
    u32 ringCount;         /* 0x74 */
    VuBlendNode *blend;    /* 0x78 */
    VuBlendNode **ringEnd; /* 0x7C: next-link destination */
    u32 state;             /* 0x80 */
    u32 header;            /* 0x84 */
    u8 *dataStart;         /* 0x88 */
    u8 *cursor;            /* 0x8C */
    u8 *payload;           /* 0x90 */
    u8 *strip;             /* 0x94 */
    u8 *positions;         /* 0x98 */
    u8 *normals;           /* 0x9C */
    u8 *coordinates;       /* 0xA0 */
    u8 *secondCoordinates; /* 0xA4 */
    u8 *vertexColors;      /* 0xA8 */
    f32 depth;             /* 0xAC */
} corePktCb_t;

extern void corePktEmitBase(corePktCb_t *work);

/* Finish the selected node's transformed rows and emit its textured geometry. */ void corePktEmitMultiTexPass(corePktCb_t *work);

/* Split ring addressing at position 128; retain the native 96-byte stride. */ void corePktSetupPointBuff(corePktCb_t *work, s32 ringPosition);

/* Emit selected geometry, then apply flag callbacks; the first may change flags. */
void corePktEmitAllPasses(corePktCb_t *work) {
    u32 selectedFlags;

    corePktEmitBase(work);
    selectedFlags = work->selectedFlags;
    if ((selectedFlags & 0x1000) != 0) {
        corePktEmitMultiTexPass(work);
        selectedFlags = work->selectedFlags;
    }
    if ((selectedFlags & 1) != 0) {
        corePktEmitRefMapPass((u32)work);
        return;
    }
}

extern void corePktBuildFaceList(corePktCb_t *);

extern void corePktRelightBackFaces(u32, s32);

extern void corePktBuildBackFaceList(corePktCb_t *);

/* Align the packet to 64 bytes; the optional second pass preserves vf24-vf26. */
void corePktOpenAndEmit(corePktCb_t *work) {
    u128 savedVectors[3];
    u32 packetStart = sdfGrOpenPacketBuff();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;

    work->state = packetStart;
    work->header = alignedStart + 0x30;
    work->cursor = work->dataStart = (u8 *)(((alignedStart + 0x40) & SDF_DMA_ADDRESS_MASK) | 0x30000000);
    if (work->selectedFlags & 0x4000) {
        VU0_STORE_VF_UNCLOBBERED(vf24, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf25, 16, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf26, 32, savedVectors);
        corePktBuildFaceList(work);
        corePktEmitAllPasses(work);
        corePktSetupPointBuff(work, work->param1);
        VU0_LOAD_VF(vf24, savedVectors);
        VU0_LOAD_VF_AT(vf25, 16, savedVectors);
        VU0_LOAD_VF_AT(vf26, 32, savedVectors);
        corePktRelightBackFaces(work->ringSrc, work->param1);
        corePktBuildBackFaceList(work);
        corePktEmitAllPasses(work);
    } else {
        corePktBuildFaceList(work);
        corePktEmitAllPasses(work);
    }
}

/* Pad and finalize the packet; inactive or empty work returns NULL. */
u64 *corePktClosePacket(corePktCb_t *work) {
    u32 cursor;
    u32 start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (work->state == 0) {
        return NULL;
    }

    cursor = (u32)work->cursor;
    start = (u32)work->dataStart;
    if (cursor == start) {
        sdfGrClosePacketBuff(work->state);
        return NULL;
    }

    while (((u32)cursor & 0xC) != 0) {
        *(u32 *)cursor = 0;
        cursor += 4;
    }
    quadwords = (s32)(cursor - start) >> 4;

    while (((u32)cursor & 0x30) != 0) {
        *(u128 *)cursor = 0;
        cursor += 0x10;
    }

    sdfGrClosePacketBuff(cursor & 0x0FFFFFFF);
    header = (u64 *)work->header;
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}
