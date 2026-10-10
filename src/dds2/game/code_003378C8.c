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

#define SDF_GIF_REGISTER_AD 0xE

#define SDF_GIF_PRIM_SHIFT 47

#define SDF_VIF_MSCAL_TRIANGLES 0x14000004

#define SDF_VU_TEXTURED_FLAG 0x10

#define SDF_VU_TEXTURED_BATCH_LIMIT 0x10

#define SDF_VU_COLORED_BATCH_LIMIT 0x18

extern u32 D_00439188;

typedef struct VuBlendNode {
    f32 position[4];
    u8 pad10[0x20];
    u8 result[0x10];
    struct VuBlendNode *next;
    void *sourceA;
    void *sourceB;
    f32 weight;
    u8 region;
    u8 clipFlags;
    u8 pad52[0xE];
} VuBlendNode;

extern u16 D_00439184;

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
    u32 ringEnd;           /* 0x7C */
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
} VuWork;

typedef struct VuGeomRef {
    u128 *a;
    u128 *b;
    u128 *c;
} VuGeomRef;

extern VuGeomRef D_00468210[];

typedef struct SdfVuColorTransform {
    u8 pad00[4];
    f32 scaledY;
    u8 pad08[4];
    f32 scale;
    f32 colorBase[4];
    f32 colorDelta[4];
} SdfVuColorTransform;

extern void func_00336EC0(SdfVuColorTransform *, u32, u32, u32, u32, f32, f32, f32);

extern void sdfVuEmitSelectedNodePacket(s32 workAddress);

void sdfVuBlendNodeXY(VuBlendNode *node);

INCLUDE_ASM(const s32, "game/code_003378C8", func_003378C8);

INCLUDE_ASM(const s32, "game/code_003378C8", func_00337970);

INCLUDE_ASM(const s32, "game/code_003378C8", func_003379F0);

INCLUDE_ASM(const s32, "game/code_003378C8", func_00337FD8);

INCLUDE_ASM(const s32, "game/code_003378C8", func_003385C0);

void sdfVuEmitTexturedTriangleBatches(work)
    VuWork *work;
{
    s32 remaining = work->nodeCount;
    s32 chunk;
    s32 first;
    u32 *out;
    VuGeomRef *ref;
    SdfAssetEntry *asset;

    if (remaining != 0) {
        out = (u32 *)work->cursor;
        ref = D_00468210;
        first = 1;
        do {
            chunk = remaining < 0x11 ? remaining : 0x10;
            remaining -= chunk;
            while (((u32)out & 0xC) != 4) {
                *out++ = 0;
            }
            if (first) {
                *out++ = 0x6501C000;
                *out++ = 4;
                *out++ = ((chunk * 9 + 5) << 16) | 0x6C00C001;
                asset = work->asset;
                *(u64 *)out = 0x1000000000000003ULL;
                out += 2;
                *(u64 *)out = 0xE;
                out += 2;
                first = 0;
                *(u64 *)out = asset->primaryTextureState.sampling;
                out += 2;
                *(u64 *)out = 0x14;
                out += 2;
                *(u64 *)out = asset->primaryTextureState.texture;
                out += 2;
                *(u64 *)out = 6;
                out += 2;
                *(u64 *)out = asset->primaryTextureState.clamp;
                out += 2;
                *(u64 *)out = 8;
                out += 2;
            } else {
                *out++ = 0x6501C000;
                *out++ = 0;
                *out++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)out = ((u64)(D_00439184 | 3) << 47) | (u64)(chunk * 3) | 0x3000400000008000ULL;
            out += 2;
            *(u64 *)out = 0x412;
            out += 2;
            do {
                ((u128 *)out)[0] = ref->a[0];
                ((u128 *)out)[1] = ref->a[3];
                ((u128 *)out)[2] = ref->a[2];
                ((u128 *)out)[3] = ref->b[0];
                ((u128 *)out)[4] = ref->b[3];
                ((u128 *)out)[5] = ref->b[2];
                ((u128 *)out)[6] = ref->c[0];
                ((u128 *)out)[7] = ref->c[3];
                ((u128 *)out)[8] = ref->c[2];
                ref++;
                out += 36;
                chunk--;
            } while (chunk != 0);
            *out++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = (u8 *)out;
    }
}

/* Emit up to 16 triangles per batch; TEX1/TEX0/CLAMP/ALPHA context 2 state
 * precedes the first batch only. clearPrimitiveFlags filters GIF PRIM bits. */
void sdfBuildChunkedVuNodeTransfer(VuWork *work, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags) {
    s32 remainingTriangles = work->nodeCount;
    s32 batchTriangles;
    s32 firstBatch;
    u32 *packetCursor;
    VuGeomRef *triangleRefs;

    if (remainingTriangles != 0) {
        packetCursor = (u32 *)work->cursor;
        triangleRefs = D_00468210;
        firstBatch = 1;
        do {
            batchTriangles = remainingTriangles < 0x11 ? remainingTriangles : SDF_VU_TEXTURED_BATCH_LIMIT;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            if (firstBatch) {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 6;
                *packetCursor++ = ((batchTriangles * 9 + 7) << 16) | 0x6C00C001;
                *(u64 *)packetCursor = 0x1000000000000005ULL;
                packetCursor += 2;
                *(u64 *)packetCursor = SDF_GIF_REGISTER_AD;
                packetCursor += 2;
                *(u64 *)packetCursor = samplingState;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x15;
                packetCursor += 2;
                *(u64 *)packetCursor = textureState;
                packetCursor += 2;
                *(u64 *)packetCursor = 7;
                packetCursor += 2;
                *(u64 *)packetCursor = clampState;
                packetCursor += 2;
                *(u64 *)packetCursor = 9;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x51801;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x48;
                packetCursor += 2;
                *(u64 *)packetCursor = alphaState;
                packetCursor += 2;
                firstBatch = 0;
                *(u64 *)packetCursor = 0x43;
                packetCursor += 2;
            } else {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 0;
                *packetCursor++ = ((batchTriangles * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)packetCursor = ((u64)((D_00439184 & ~clearPrimitiveFlags) | 0x203) << SDF_GIF_PRIM_SHIFT) | (u64)(batchTriangles * 3) | 0x3000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x412;
            packetCursor += 2;
            do {
                ((u128 *)packetCursor)[0] = triangleRefs->a[0];
                ((u128 *)packetCursor)[1] = triangleRefs->a[3];
                ((u128 *)packetCursor)[2] = triangleRefs->a[2];
                ((u128 *)packetCursor)[3] = triangleRefs->b[0];
                ((u128 *)packetCursor)[4] = triangleRefs->b[3];
                ((u128 *)packetCursor)[5] = triangleRefs->b[2];
                ((u128 *)packetCursor)[6] = triangleRefs->c[0];
                ((u128 *)packetCursor)[7] = triangleRefs->c[3];
                ((u128 *)packetCursor)[8] = triangleRefs->c[2];
                triangleRefs++;
                packetCursor += 36;
                batchTriangles--;
            } while (batchTriangles != 0);
            *packetCursor++ = SDF_VIF_MSCAL_TRIANGLES;
        } while (remainingTriangles != 0);
        work->cursor = (u8 *)packetCursor;
    }
}

/* Emit up to 24 colored triangles per batch, copying vertex quadwords 0 and 2.
 * Keep the native K&R declaration and caller convention. */
void sdfVuEmitColoredTriangleBatches(work)
    VuWork *work;
{
    s32 remainingTriangles = work->nodeCount;
    s32 batchTriangles;
    u32 *packetCursor;
    VuGeomRef *triangleRefs;

    if (remainingTriangles != 0) {
        packetCursor = (u32 *)work->cursor;
        triangleRefs = D_00468210;
        do {
            batchTriangles = remainingTriangles < 0x19 ? remainingTriangles : SDF_VU_COLORED_BATCH_LIMIT;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            *packetCursor++ = 0x6501C000;
            *packetCursor++ = 0x10000;
            *packetCursor++ = ((batchTriangles * 6 + 1) << 16) | 0x6C00C001;
            *(u64 *)packetCursor = (u64)(batchTriangles * 3) | ((u64)(D_00439184 | 3) << SDF_GIF_PRIM_SHIFT) | 0x2000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x41;
            packetCursor += 2;
            do {
                ((u128 *)packetCursor)[0] = triangleRefs->a[0];
                ((u128 *)packetCursor)[1] = triangleRefs->a[2];
                ((u128 *)packetCursor)[2] = triangleRefs->b[0];
                ((u128 *)packetCursor)[3] = triangleRefs->b[2];
                ((u128 *)packetCursor)[4] = triangleRefs->c[0];
                ((u128 *)packetCursor)[5] = triangleRefs->c[2];
                triangleRefs++;
                packetCursor += 24;
                batchTriangles--;
            } while (batchTriangles != 0);
            *packetCursor++ = SDF_VIF_MSCAL_TRIANGLES;
        } while (remainingTriangles != 0);
        work->cursor = (u8 *)packetCursor;
    }
}

/* Select textured or colored emission; preserve the native no-argument calls. */
void sdfVuEmitSelectedNodePacket(s32 workAddress) {
    if ((*(u32 *)(workAddress + 0x44) & SDF_VU_TEXTURED_FLAG) != 0) {
        sdfVuEmitTexturedTriangleBatches();
        return;
    }
    sdfVuEmitColoredTriangleBatches();
}

/* Finish the selected node's transformed rows and emit its textured geometry. */
void func_00339188(u32 workAddress) {
    VuWork *work = (VuWork *)workAddress;
    SdfAssetEntry *asset;
    u32 mode;
    u32 paramC;
    u32 param8;

    func_00337718((void *)work->ringSrc, work->param1, work->secondCoordinates,
                  (u8 *)work->asset + 0x80);
    sdfVuBlendNodeXY(work->blend);
    asset = work->asset;
    mode = asset->secondaryMode;
    paramC = asset->secondaryColor;
    param8 = asset->unk08;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (param8 != paramC) {
                u128 parameters[3];

                func_00336EC0((SdfVuColorTransform *)parameters, D_00439188, work->packedColor,
                              paramC, asset->unk04, asset->unk1C,
                              asset->x + work->offsetX, asset->y + work->offsetY);
                func_00337830(work, parameters);
            }
            break;
        case 3:
            func_003378C8(work, paramC);
            break;
    }
    sdfBuildChunkedVuNodeTransfer(work, asset->secondaryTextureState.sampling,
                                  asset->secondaryTextureState.texture,
                                  asset->secondaryTextureState.clamp, asset->alphaState, 0);
}
