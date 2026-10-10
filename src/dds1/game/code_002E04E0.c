#include "sdf_gs_header.h"
#include "common.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
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

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

extern s32 sdfGetPacketCursor(void);

typedef struct VuBlendNode {
    u8 pad00[0x30];
    u8 result[0x10];
    struct VuBlendNode *next;
    void *sourceA;
    void *sourceB;
} VuBlendNode;

typedef struct {
    f32 matrix[4][4];      /* 0x00 */
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50 */
    u8 pad52[2];
    SdfAssetEntry *node;   /* 0x54 */
    u32 packedColor;       /* 0x58: packed RGBA word */
    f32 offsetX;           /* 0x5C */
    f32 offsetY;           /* 0x60 */
    u32 dmaBase;           /* 0x64 */
    u32 dmaAddrA;          /* 0x68 */
    u32 dmaAddrB;          /* 0x6C */
    u32 dmaCountA;         /* 0x70 */
    u32 dmaCountB;         /* 0x74 */
    VuBlendNode *blendList; /* 0x78 */
    void *dmaEnd;          /* 0x7C */
    u32 packetStart;       /* 0x80 */
    u32 header;            /* 0x84: aligned packet cursor + 0x30 */
    u32 *ringStart;        /* 0x88 */
    u32 *cursor;           /* 0x8C */
    u8 *payload;           /* 0x90 */
    u8 *strip;             /* 0x94 */
    u8 *positions;         /* 0x98 */
    u8 *normals;           /* 0x9C */
    u8 *coordinates;       /* 0xA0 */
    u8 *secondCoordinates; /* 0xA4 */
    u8 *vertexColors;      /* 0xA8 */
    f32 depth;             /* 0xAC */
} VuWork;

/* Finish the selected node's transformed rows and emit its textured geometry. */ void func_002E02D8(u32 workAddress);

/* Split ring addressing at position 128; retain the native 96-byte stride. */ void sdfVuConfigureWorkRingDma(VuWork *work, s32 ringPosition);

/* Select textured or colored emission; preserve the native no-argument calls. */ void sdfVuEmitSelectedNodePacket(VuWork *work);

/* Emit selected geometry, then apply flag callbacks; the first may change flags. */
void sdfVuApplySelectedWorkFlags(u32 workAddress) {
    u32 selectedFlags;
    s32 signedWorkAddress;

    signedWorkAddress = (s32)workAddress;
    sdfVuEmitSelectedNodePacket((VuWork *)signedWorkAddress);
    selectedFlags = ((VuWork *)signedWorkAddress)->selectedFlags;
    if ((selectedFlags & 0x1000) != 0) {
        func_002E02D8(workAddress);
        selectedFlags = ((VuWork *)signedWorkAddress)->selectedFlags;
    }
    if ((selectedFlags & 1) != 0) {
        func_002E03C0(workAddress);
        return;
    }
}

extern void func_002DF128(VuWork *work);

extern void func_002DF710(VuWork *work);

extern void func_002DE7D8(u32 base, s32 count);

/* Align the packet to 64 bytes; the optional second pass preserves vf24-vf26. */
void sdfVuBeginPacketFromWork(VuWork *work) {
    u32 packetStart = sdfGetPacketCursor();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;
    u32 ringAddress = ((alignedStart + 0x40) & SDF_DMA_ADDRESS_MASK) | 0x30000000;
    work->packetStart = packetStart;
    work->header = alignedStart + 0x30;
    work->ringStart = (u32 *)ringAddress;
    work->cursor = (u32 *)ringAddress;
    if ((work->selectedFlags & 0x4000) != 0) {
        u8 savedVectors[0x30];
        VU0_STORE_VF_UNCLOBBERED(vf24, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf25, 16, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf26, 32, savedVectors);
        func_002DF128(work);
        sdfVuApplySelectedWorkFlags((u32)work);
        sdfVuConfigureWorkRingDma(work, work->param1);
        VU0_LOAD_VF(vf24, savedVectors);
        VU0_LOAD_VF_AT(vf25, 16, savedVectors);
        VU0_LOAD_VF_AT(vf26, 32, savedVectors);
        func_002DE7D8(work->dmaBase, work->param1);
        func_002DF710(work);
        sdfVuApplySelectedWorkFlags((u32)work);
    } else {
        func_002DF128(work);
        sdfVuApplySelectedWorkFlags((u32)work);
    }
}

u64 *func_002E0618(VuWork *work) {
    u32 cursor;
    u32 start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (work->packetStart == 0) {
        return NULL;
    }

    cursor = (u32)work->cursor;
    start = (u32)work->ringStart;
    if (cursor == start) {
        sdfSetPacketCursorAligned(work->packetStart);
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

    sdfSetPacketCursorAligned(cursor & 0x0FFFFFFF);
    header = (u64 *)work->header;
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}
