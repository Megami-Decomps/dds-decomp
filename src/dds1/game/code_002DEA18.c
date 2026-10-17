#include "sdf_gs_header.h"
#include "common.h"
#include "sdf_vu_color_transform.h"
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
#include "sdf_vu_blend_node.h"

#define SDF_GIF_REGISTER_AD 0xE

#define SDF_GIF_PRIM_SHIFT 47

#define SDF_VIF_MSCAL_TRIANGLES 0x14000004

#define SDF_VU_TEXTURED_FLAG 0x10

#define SDF_VU_TEXTURED_BATCH_LIMIT 0x10

#define SDF_VU_COLORED_BATCH_LIMIT 0x18

extern u32 D_003BDA28;

extern u128 *D_003EB860[][3];

extern u16 D_003BDA24;



typedef struct {
    f32 matrix[4][4];      /* 0x00 */
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50 */
    s16 selectedTriangleCount; /* 0x52: rejected-triangle index count */
    SdfAssetEntry *node;   /* 0x54 */
    u32 packedColor;       /* 0x58: packed RGBA word */
    f32 offsetX;           /* 0x5C */
    f32 offsetY;           /* 0x60 */
    u32 dmaBase;           /* 0x64 */
    VuBlendNode *dmaAddrA; /* 0x68: next free ring node */
    VuBlendNode *dmaAddrB; /* 0x6C: ring wrap node */
    u32 dmaCountA;         /* 0x70 */
    u32 dmaCountB;         /* 0x74 */
    VuBlendNode *blendList; /* 0x78 */
    VuBlendNode **dmaEnd;  /* 0x7C: list tail link */
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

void sdfVuBlendNodeXY(VuBlendNode *node);

void sdfVuMultiplyNodeColors(VuWork *work, u32 color) {
    f32 colorScale = 1.0f / 128.0f;
    VuBlendNode *node;
    s32 remaining;
    u128 scaledColor;

    EE_MMI_RGBA_MULTIPLY_VECTOR(scaledColor, work->packedColor, color, colorScale);

    remaining = work->param1;
    node = (VuBlendNode *)work->dmaBase;
    if (remaining != 0) {
        do {
            remaining--;
            *(u128 *)node->color = scaledColor;
            node++;
        } while (remaining != 0);
    }

    node = work->blendList;
    while (node != NULL) {
        VuBlendNode *source = (VuBlendNode *)node->sourceA;
        PCP_COPY_VECTOR_F32(node->color, source->color);
        node = node->next;
    }
}

void sdfVuInterpolateClipVertex(VuBlendNode *out, VuBlendNode *first, VuBlendNode *second) {
    out->sourceA = first;
    out->sourceB = second;
    VU0_INTERPOLATE_CLIP_VERTEX(out, first, second);
}

extern u8 D_003BD344;
extern u16 D_003F7860[];
static inline VuBlendNode *vuAllocNode(VuWork *work) {
    VuBlendNode *node = work->dmaAddrA;

    if (--work->dmaCountA == 0) {
        work->dmaAddrA = work->dmaAddrB;
        work->dmaCountA = work->dmaCountB;
    } else {
        work->dmaAddrA = node + 1;
    }
    *work->dmaEnd = node;
    work->dmaEnd = &node->next;
    return node;
}
/* vu0 routine: native clip facing and ordinary triangle fans. */
void func_002DEB40(VuWork *work) {
    s32 triangleCount;
    s32 i;
    u16 *triangles;
    VuBlendNode *nodes;
    s32 rejected;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale;

    out = D_003EB860;
    triangleCount = (s16)work->param0;
    rejected = 0;
    scale = work->depth;
    acceptAll = D_003BD344;
    triangles = (u16 *)work->strip;
    nodes = (VuBlendNode *)work->dmaBase;
    for (i = 0; i != triangleCount; i++) {
        u16 *triangle = triangles;
        VuBlendNode *a = nodes + triangle[0];
        VuBlendNode *b = nodes + triangle[1];
        VuBlendNode *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        VuBlendNode *d, *e, *f, *g;
        f32 facing;
        u32 clipA = a->clipFlags;
        u32 clipB = b->clipFlags;
        u32 clipC = c->clipFlags;

        triangles += 3;
        if ((clipA & clipB & clipC & 0x3F) != 0 ||
            ((clipA | clipB | clipC) & 0x30) != 0) {
            continue;
        }
        VU0_TRIANGLE_FACING(facing, a->position, b->position, c->position);
        facing *= scale;
        if (facing < 0.0f) {
            D_003F7860[rejected++] = i;
            continue;
        }
        aRegion = a->region;
        bRegion = b->region;
        cRegion = c->region;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0][0] = (u128 *)a;
            out[0][1] = (u128 *)b;
            out[0][2] = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            VuBlendNode *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                if (aRegion == 0) {
                    sdfVuInterpolateClipVertex(d, b, a);
                    sdfVuInterpolateClipVertex(e, c, a);
                    out[0][0] = (u128 *)a;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)e;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)d;
                    out[1][2] = (u128 *)b;
                    out[2][0] = (u128 *)e;
                    out[2][1] = (u128 *)b;
                    out[2][2] = (u128 *)c;
                    out += 3;
                    break;
                }
                sdfVuInterpolateClipVertex(d, a, b);
                sdfVuInterpolateClipVertex(e, a, c);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0][0] = (u128 *)b;
                    out[0][1] = (u128 *)e;
                    out[0][2] = (u128 *)d;
                    out[1][0] = (u128 *)b;
                    out[1][1] = (u128 *)c;
                    out[1][2] = (u128 *)e;
                    out += 2;
                } else {
                    f = vuAllocNode(work);
                    g = vuAllocNode(work);
                    sdfVuInterpolateClipVertex(f, b, a);
                    sdfVuInterpolateClipVertex(g, c, a);
                    out[0][0] = (u128 *)e;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)f;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)f;
                    out[1][2] = (u128 *)g;
                    out[2][0] = (u128 *)g;
                    out[2][1] = (u128 *)f;
                    out[2][2] = (u128 *)b;
                    out[3][0] = (u128 *)g;
                    out[3][1] = (u128 *)b;
                    out[3][2] = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                f = vuAllocNode(work);
                g = vuAllocNode(work);
                sdfVuInterpolateClipVertex(d, b, a);
                sdfVuInterpolateClipVertex(e, c, a);
                sdfVuInterpolateClipVertex(f, b, c);
                sdfVuInterpolateClipVertex(g, c, b);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out[1][0] = (u128 *)d;
                out[1][1] = (u128 *)b;
                out[1][2] = (u128 *)f;
                out[2][0] = (u128 *)e;
                out[2][1] = (u128 *)d;
                out[2][2] = (u128 *)f;
                out[3][0] = (u128 *)e;
                out[3][1] = (u128 *)f;
                out[3][2] = (u128 *)g;
                out[4][0] = (u128 *)e;
                out[4][1] = (u128 *)g;
                out[4][2] = (u128 *)c;
                out += 5;
                break;
            }
            firstVertex = a;
            firstCode = aRegion;
            a = b;
            aRegion = bRegion;
            b = c;
            bRegion = cRegion;
            c = firstVertex;
            cRegion = firstCode;
        }
    }
    work->selectedTriangleCount = rejected;
    work->nodeCount = out - D_003EB860;
    *work->dmaEnd = NULL;
}

extern f32 D_003BDA30;
extern f32 D_003984F0[4];
extern u8 D_003983D0[];
/* vu0 routine: native clip-plane setup and ordinary triangle fans. */
void sdfVuBuildVisibleTriangleList(VuWork *work) {
    s32 triangleCount;
    u16 *triangles;
    VuBlendNode *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale = work->depth;
    f32 clipBias = D_003BDA30;
    const f32 *clipPlane = D_003984F0;
    const u8 *projection = D_003983D0;

    VU0_SET_CLIP_PLANES(clipBias, clipPlane, projection);
    if (work->selectedFlags & 0x4000) {
        func_002DEB40(work);
    }
    triangleCount = (s16)work->param0;
    acceptAll = D_003BD344;
    out = D_003EB860;
    triangles = (u16 *)work->strip;
    nodes = (VuBlendNode *)work->dmaBase;
    for (; triangleCount != 0; triangleCount--) {
        u16 *triangle = triangles;
        VuBlendNode *a = nodes + triangle[0];
        VuBlendNode *b = nodes + triangle[1];
        VuBlendNode *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        VuBlendNode *d, *e, *f, *g;
        f32 facing;
        u32 clipA = a->clipFlags;
        u32 clipB = b->clipFlags;
        u32 clipC = c->clipFlags;

        triangles += 3;
        if ((clipA & clipB & clipC & 0x3F) != 0 ||
            ((clipA | clipB | clipC) & 0x30) != 0) {
            continue;
        }
        VU0_TRIANGLE_FACING(facing, a->position, b->position, c->position);
        facing *= scale;
        if (facing < 0.0f) {
            continue;
        }
        aRegion = a->region;
        bRegion = b->region;
        cRegion = c->region;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0][0] = (u128 *)a;
            out[0][1] = (u128 *)b;
            out[0][2] = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            VuBlendNode *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                if (aRegion == 0) {
                    sdfVuInterpolateClipVertex(d, b, a);
                    sdfVuInterpolateClipVertex(e, c, a);
                    out[0][0] = (u128 *)a;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)e;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)d;
                    out[1][2] = (u128 *)b;
                    out[2][0] = (u128 *)e;
                    out[2][1] = (u128 *)b;
                    out[2][2] = (u128 *)c;
                    out += 3;
                    break;
                }
                sdfVuInterpolateClipVertex(d, a, b);
                sdfVuInterpolateClipVertex(e, a, c);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0][0] = (u128 *)b;
                    out[0][1] = (u128 *)e;
                    out[0][2] = (u128 *)d;
                    out[1][0] = (u128 *)b;
                    out[1][1] = (u128 *)c;
                    out[1][2] = (u128 *)e;
                    out += 2;
                } else {
                    f = vuAllocNode(work);
                    g = vuAllocNode(work);
                    sdfVuInterpolateClipVertex(f, b, a);
                    sdfVuInterpolateClipVertex(g, c, a);
                    out[0][0] = (u128 *)e;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)f;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)f;
                    out[1][2] = (u128 *)g;
                    out[2][0] = (u128 *)g;
                    out[2][1] = (u128 *)f;
                    out[2][2] = (u128 *)b;
                    out[3][0] = (u128 *)g;
                    out[3][1] = (u128 *)b;
                    out[3][2] = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                f = vuAllocNode(work);
                g = vuAllocNode(work);
                sdfVuInterpolateClipVertex(d, b, a);
                sdfVuInterpolateClipVertex(e, c, a);
                sdfVuInterpolateClipVertex(f, b, c);
                sdfVuInterpolateClipVertex(g, c, b);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out[1][0] = (u128 *)d;
                out[1][1] = (u128 *)b;
                out[1][2] = (u128 *)f;
                out[2][0] = (u128 *)e;
                out[2][1] = (u128 *)d;
                out[2][2] = (u128 *)f;
                out[3][0] = (u128 *)e;
                out[3][1] = (u128 *)f;
                out[3][2] = (u128 *)g;
                out[4][0] = (u128 *)e;
                out[4][1] = (u128 *)g;
                out[4][2] = (u128 *)c;
                out += 5;
                break;
            }
            firstVertex = a;
            firstCode = aRegion;
            a = b;
            aRegion = bRegion;
            b = c;
            bRegion = cRegion;
            c = firstVertex;
            cRegion = firstCode;
        }
    }
    work->nodeCount = out - D_003EB860;
    *work->dmaEnd = NULL;
}

/* Indexed triangle count occupies the two bytes following nodeCount. */
typedef struct {
    u8 pad00[0x52];
    s16 indexedTriangleCount;
} VuIndexedTriangleCount;
void func_002DF710(VuWork *work) {
    /* DDS1 node region uses the tail byte at +0x50. */
    f32 scale = D_003BDA30;
    s32 remaining;
    u16 *triangles;
    u16 *indices;
    VuBlendNode *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    const f32 *clipPlane = D_003984F0;
    const u8 *projection = D_003983D0;

    VU0_SET_CLIP_PLANES(scale, clipPlane, projection);
    acceptAll = D_003BD344;
    remaining = ((VuIndexedTriangleCount *)work)->indexedTriangleCount;
    indices = D_003F7860;
    triangles = (u16 *)work->strip;
    nodes = (VuBlendNode *)work->dmaBase;
    out = (u128 *(*)[3])D_003EB860;
    if (remaining != 0) {
        do {
            u16 *triangle = triangles + *indices++ * 3;
            VuBlendNode *a = nodes + triangle[0];
            VuBlendNode *b = nodes + triangle[1];
            VuBlendNode *c = nodes + triangle[2];
            u32 bRegion = b->region;
            u32 aRegion = a->region;
            u32 cRegion = c->region;
            VuBlendNode *d, *e, *f, *g;
            if (!acceptAll && (aRegion != bRegion || aRegion != cRegion)) {
                goto classifyRegions;
            }
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)b;
        out[0][2] = (u128 *)c;
        out += 1;
            goto advanceTriangle;
        appendInsidePair:
        out[0][0] = (u128 *)b;
        out[0][1] = (u128 *)e;
        out[0][2] = (u128 *)d;
        out[1][0] = (u128 *)b;
        out[1][1] = (u128 *)c;
        out[1][2] = (u128 *)e;
        out += 2;
            goto advanceTriangle;
        interpolateOutsideA:
        sdfVuInterpolateClipVertex(d, a, b);
        sdfVuInterpolateClipVertex(e, a, c);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out += 1;
            if (bRegion != 0) {
            f = vuAllocNode(work);
            g = vuAllocNode(work);
            sdfVuInterpolateClipVertex(f, b, a);
            sdfVuInterpolateClipVertex(g, c, a);
            out[0][0] = (u128 *)e;
            out[0][1] = (u128 *)d;
            out[0][2] = (u128 *)f;
            out[1][0] = (u128 *)e;
            out[1][1] = (u128 *)f;
            out[1][2] = (u128 *)g;
            out[2][0] = (u128 *)g;
            out[2][1] = (u128 *)f;
            out[2][2] = (u128 *)b;
            out[3][0] = (u128 *)g;
            out[3][1] = (u128 *)b;
            out[3][2] = (u128 *)c;
            out += 4;

            } else goto appendInsidePair;
            goto advanceTriangle;
        threeDifferentRegions: {
            if (aRegion != 0 || (bRegion ^ cRegion) != 3) goto rotateTriangle;
        d = vuAllocNode(work);
        e = vuAllocNode(work);
        f = vuAllocNode(work);
        g = vuAllocNode(work);
        sdfVuInterpolateClipVertex(d, b, a);
        sdfVuInterpolateClipVertex(e, c, a);
        sdfVuInterpolateClipVertex(f, b, c);
        sdfVuInterpolateClipVertex(g, c, b);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out[1][0] = (u128 *)d;
        out[1][1] = (u128 *)b;
        out[1][2] = (u128 *)f;
        out[2][0] = (u128 *)e;
        out[2][1] = (u128 *)d;
        out[2][2] = (u128 *)f;
        out[3][0] = (u128 *)e;
        out[3][1] = (u128 *)f;
        out[3][2] = (u128 *)g;
        out[4][0] = (u128 *)e;
        out[4][1] = (u128 *)g;
        out[4][2] = (u128 *)c;
        out += 5;
            goto advanceTriangle;
        }
        rotateTriangle: {
            VuBlendNode *oldA = a;
            u32 oldRegion = aRegion;
            a = b; b = c; c = oldA;
            aRegion = bRegion; bRegion = cRegion; cRegion = oldRegion;
        }
        classifyRegions:
            if (bRegion != cRegion) goto threeDifferentRegions;
        d = vuAllocNode(work);
        e = vuAllocNode(work);
            if (aRegion != 0) goto interpolateOutsideA;
        sdfVuInterpolateClipVertex(d, b, a);
        sdfVuInterpolateClipVertex(e, c, a);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out[1][0] = (u128 *)e;
        out[1][1] = (u128 *)d;
        out[1][2] = (u128 *)b;
        out[2][0] = (u128 *)e;
        out[2][1] = (u128 *)b;
        out[2][2] = (u128 *)c;
        out += 3;
        advanceTriangle:
            ;
        } while (--remaining != 0);
    }
    work->nodeCount = out - (u128 *(*)[3])D_003EB860;
    *(VuBlendNode **)work->dmaEnd = NULL;
}

void sdfVuEmitTexturedTriangleBatches(work)
    VuWork *work;
{
    s32 remaining = work->nodeCount;
    if (remaining != 0) {
        u128 *(*nodes)[3] = D_003EB860;
        u32 *cursor = work->cursor;
        s32 first = 1;
        do {
            s32 chunk = (remaining <= 0x10) ? remaining : 0x10;
            remaining -= chunk;
            while (((u32)cursor & 0xC) != 4) {
                *cursor++ = 0;
            }
            if (first) {
                SdfAssetEntry *node;
                *cursor++ = 0x6501C000;
                *cursor++ = 4;
                *cursor++ = ((chunk * 9 + 5) << 16) | 0x6C00C001;
                node = work->node;
                first = 0;
                *(u64 *)cursor = 0x1000000000000003ULL;
                cursor += 2;
                *(u64 *)cursor = 0xE;
                cursor += 2;
                *(u64 *)cursor = node->primaryTextureState.sampling;
                cursor += 2;
                *(u64 *)cursor = 0x14;
                cursor += 2;
                *(u64 *)cursor = node->primaryTextureState.texture;
                cursor += 2;
                *(u64 *)cursor = 6;
                cursor += 2;
                *(u64 *)cursor = node->primaryTextureState.clamp;
                cursor += 2;
                *(u64 *)cursor = 8;
                cursor += 2;
            } else {
                *cursor++ = 0x6501C000;
                *cursor++ = 0;
                *cursor++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)cursor = ((u64)(D_003BDA24 | 3) << 47) | (chunk * 3) | 0x3000400000008000ULL;
            cursor += 2;
            *(u64 *)cursor = 0x412;
            cursor += 2;
            do {
                u128 *a = (*nodes)[0];
                u128 *b = (*nodes)[1];
                u128 *c = (*nodes)[2];
                nodes++;
                ((u128 *)cursor)[0] = a[0];
                ((u128 *)cursor)[1] = a[3];
                ((u128 *)cursor)[2] = a[2];
                ((u128 *)cursor)[3] = b[0];
                ((u128 *)cursor)[4] = b[3];
                ((u128 *)cursor)[5] = b[2];
                ((u128 *)cursor)[6] = c[0];
                ((u128 *)cursor)[7] = c[3];
                ((u128 *)cursor)[8] = c[2];
                cursor += 0x24;
            } while (--chunk != 0);
            *cursor++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = cursor;
    }
}

/* Emit up to 16 triangles per batch; TEX1/TEX0/CLAMP/ALPHA context 2 state
 * precedes the first batch only. clearPrimitiveFlags filters GIF PRIM bits. */
void sdfBuildChunkedVuNodeTransfer(VuWork *work, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags) {
    s32 remainingTriangles = work->nodeCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = D_003EB860;
        u32 *packetCursor = work->cursor;
        s32 firstBatch = 1;
        do {
            s32 batchTriangles = (remainingTriangles <= SDF_VU_TEXTURED_BATCH_LIMIT) ? remainingTriangles : SDF_VU_TEXTURED_BATCH_LIMIT;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            if (firstBatch) {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 6;
                *packetCursor++ = ((batchTriangles * 9 + 7) << 16) | 0x6C00C001;
                firstBatch = 0;
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
                *(u64 *)packetCursor = 0x43;
                packetCursor += 2;
            } else {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 0;
                *packetCursor++ = ((batchTriangles * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)packetCursor = ((u64)((D_003BDA24 & ~clearPrimitiveFlags) | 0x203) << SDF_GIF_PRIM_SHIFT) | (batchTriangles * 3) | 0x3000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x412;
            packetCursor += 2;
            do {
                u128 *vertexA = (*triangleRefs)[0];
                u128 *vertexB = (*triangleRefs)[1];
                u128 *vertexC = (*triangleRefs)[2];
                triangleRefs++;
                ((u128 *)packetCursor)[0] = vertexA[0];
                ((u128 *)packetCursor)[1] = vertexA[3];
                ((u128 *)packetCursor)[2] = vertexA[2];
                ((u128 *)packetCursor)[3] = vertexB[0];
                ((u128 *)packetCursor)[4] = vertexB[3];
                ((u128 *)packetCursor)[5] = vertexB[2];
                ((u128 *)packetCursor)[6] = vertexC[0];
                ((u128 *)packetCursor)[7] = vertexC[3];
                ((u128 *)packetCursor)[8] = vertexC[2];
                packetCursor += 0x24;
            } while (--batchTriangles != 0);
            *packetCursor++ = SDF_VIF_MSCAL_TRIANGLES;
        } while (remainingTriangles != 0);
        work->cursor = packetCursor;
    }
}

/* Emit up to 24 colored triangles per batch, copying vertex quadwords 0 and 2.
 * Keep the native K&R declaration and caller convention. */
void sdfVuEmitColoredTriangleBatches(work)
    VuWork *work;
{
    s32 remainingTriangles = work->nodeCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = D_003EB860;
        u32 *packetCursor = work->cursor;
        do {
            s32 batchTriangles = (remainingTriangles <= SDF_VU_COLORED_BATCH_LIMIT) ? remainingTriangles : SDF_VU_COLORED_BATCH_LIMIT;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            *packetCursor++ = 0x6501C000;
            *packetCursor++ = 0x10000;
            *packetCursor++ = ((batchTriangles * 6 + 1) << 16) | 0x6C00C001;
            *(u64 *)packetCursor = (batchTriangles * 3) | ((u64)(D_003BDA24 | 3) << SDF_GIF_PRIM_SHIFT) | 0x2000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x41;
            packetCursor += 2;
            do {
                u128 *vertexA = (*triangleRefs)[0];
                u128 *vertexB = (*triangleRefs)[1];
                u128 *vertexC = (*triangleRefs)[2];
                triangleRefs++;
                ((u128 *)packetCursor)[0] = vertexA[0];
                ((u128 *)packetCursor)[1] = vertexA[2];
                ((u128 *)packetCursor)[2] = vertexB[0];
                ((u128 *)packetCursor)[3] = vertexB[2];
                ((u128 *)packetCursor)[4] = vertexC[0];
                ((u128 *)packetCursor)[5] = vertexC[2];
                packetCursor += 0x18;
            } while (--batchTriangles != 0);
            *packetCursor++ = SDF_VIF_MSCAL_TRIANGLES;
        } while (remainingTriangles != 0);
        work->cursor = packetCursor;
    }
}

/* Select textured or colored emission; preserve the native no-argument calls. */
void sdfVuEmitSelectedNodePacket(VuWork *work) {
    if ((work->selectedFlags & SDF_VU_TEXTURED_FLAG) != 0) {
        sdfVuEmitTexturedTriangleBatches();
        return;
    }
    sdfVuEmitColoredTriangleBatches();
}

/* Finish the selected node's transformed rows and emit its textured geometry. */
void sdfVuUpdateSelectedNodeGeometry(VuWork *work) {
    SdfAssetEntry *node;
    u32 mode;
    u32 secondaryColor;
    u32 primaryStateWordSecond;

    func_002DE868((void *)work->dmaBase, work->param1, work->secondCoordinates,
                  (u8 *)work->node + 0x80);
    sdfVuBlendNodeXY(work->blendList);
    node = work->node;
    mode = node->secondaryMode;
    secondaryColor = node->secondaryColor;
    primaryStateWordSecond = node->unk08;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (primaryStateWordSecond != secondaryColor) {
                u128 parameters[3];

                sdfVuBuildPackedColorTransform((SdfVuColorTransform *)parameters, D_003BDA28, work->packedColor,
                              secondaryColor, node->unk04, node->unk1C,
                              node->x + work->offsetX, node->y + work->offsetY);
                func_002DE980(work, parameters);
            }
            break;
        case 3:
            sdfVuMultiplyNodeColors(work, secondaryColor);
            break;
    }
    sdfBuildChunkedVuNodeTransfer(work, node->secondaryTextureState.sampling,
                                  node->secondaryTextureState.texture,
                                  node->secondaryTextureState.clamp, node->alphaState, 0);
}
