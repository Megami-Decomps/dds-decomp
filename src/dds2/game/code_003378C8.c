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
#include "sdfCorePacketPointBuff_tag.h"

#define SDF_GIF_REGISTER_AD 0xE

#define SDF_GIF_PRIM_SHIFT 47

#define SDF_VIF_MSCAL_TRIANGLES 0x14000004

#define SDF_VU_TEXTURED_FLAG 0x10

#define SDF_VU_TEXTURED_BATCH_LIMIT 0x10

#define SDF_VU_COLORED_BATCH_LIMIT 0x18

extern const struct SdfDmaTagHeader *corePktCurrLight;



extern u16 D_00439184;

typedef struct {
    f32 tmSave[4][4];      /* 0x00 */
    u16 faceCount;            /* 0x40 */
    s16 pointCount;            /* 0x42 */
    u32 attrBits;     /* 0x44 */
    u32 packetBits;         /* 0x48 */
    u32 orgAttrBits;             /* 0x4C */
    s16 drawFaceCount;         /* 0x50: geometry references submitted to VU in chunks */
    s16 backfaceCount; /* 0x52: entries in the rejected-triangle index list */
    SdfAssetEntry *matPacketPtr;  /* 0x54 */
    u32 color;       /* 0x58: packed RGBA word */
    f32 fdSoften;           /* 0x5C */
    f32 fdBias;           /* 0x60 */
    u32 pointBuff;           /* 0x64 */
    sdfCorePacketPointBuff_tag *freePointBuff;  /* 0x68: next free clipped vertex */
    sdfCorePacketPointBuff_tag *extPointBuff; /* 0x6C: ring restart */
    u32 freePointCount;     /* 0x70 */
    u32 extPointCount;         /* 0x74 */
    sdfCorePacketPointBuff_tag *extPointTop;    /* 0x78 */
    sdfCorePacketPointBuff_tag **extPointLink; /* 0x7C: next-link destination */
    u32 packetOpenPtr;             /* 0x80 */
    u32 packetTopPtr;            /* 0x84 */
    u8 *packetStartPtr;         /* 0x88 */
    u8 *packetPtr;            /* 0x8C */
    u8 *dataPtr;           /* 0x90 */
    u8 *facePtr;             /* 0x94 */
    u8 *vertexPtr;         /* 0x98 */
    u8 *normalPtr;           /* 0x9C */
    u8 *uvPtr;       /* 0xA0 */
    u8 *uv2Ptr; /* 0xA4 */
    u8 *vertColPtr;      /* 0xA8 */
    f32 backfaceSign;             /* 0xAC */
} sdfCorePacketCb_t;

typedef struct VuGeomRef {
    u128 *a;
    u128 *b;
    u128 *c;
} VuGeomRef;

extern VuGeomRef corePktFaceList[];

extern void corePktEmitBase(sdfCorePacketCb_t *work);

void corePktLerpUVChain(sdfCorePacketPointBuff_tag *node);

void corePktSetConstColor(sdfCorePacketCb_t *work, u32 color) {
    f32 colorScale = 1.0f / 128.0f;
    sdfCorePacketPointBuff_tag *node;
    s32 remaining;
    u128 scaledColor;

    EE_MMI_RGBA_MULTIPLY_VECTOR(scaledColor, work->color, color, colorScale);

    remaining = work->pointCount;
    node = (sdfCorePacketPointBuff_tag *)work->pointBuff;
    if (remaining != 0) {
        do {
            remaining--;
            *(u128 *)node->color = scaledColor;
            node++;
        } while (remaining != 0);
    }

    node = work->extPointTop;
    while (node != NULL) {
        sdfCorePacketPointBuff_tag *source = (sdfCorePacketPointBuff_tag *)node->sourceA;
        PCP_COPY_VECTOR_F32(node->color, source->color);
        node = node->next;
    }
}

void corePktMakeEdgePoint(sdfCorePacketPointBuff_tag *out, sdfCorePacketPointBuff_tag *first, sdfCorePacketPointBuff_tag *second) {
    out->sourceA = first;
    out->sourceB = second;
    VU0_INTERPOLATE_CLIP_VERTEX(out, first, second);
}

extern u8 corePktSoftenBias;
extern u16 D_00474210[];
extern f32 sdf3DEyeOffset[4];
extern f32 D_00439190;
static inline sdfCorePacketPointBuff_tag *sdfVuAllocClipNode(sdfCorePacketCb_t *work) {
    sdfCorePacketPointBuff_tag *node = work->freePointBuff;
    if (--work->freePointCount == 0) {
        work->freePointBuff = work->extPointBuff;
        work->freePointCount = work->extPointCount;
    } else {
        work->freePointBuff = node + 1;
    }
    *work->extPointLink = node;
    work->extPointLink = &node->next;
    return node;
}
/* vu0 routine: native clip-facing test and ordinary triangle fans. */
void corePktClipFace(sdfCorePacketCb_t *work) {
    s32 triangleCount;
    s32 i;
    u16 *triangles;
    sdfCorePacketPointBuff_tag *nodes;
    s32 rejected;
    s32 acceptAll;
    VuGeomRef *out;
    f32 scale;

    out = corePktFaceList;
    triangleCount = (s16)work->faceCount;
    rejected = 0;
    scale = work->backfaceSign;
    acceptAll = corePktSoftenBias;
    triangles = (u16 *)work->facePtr;
    nodes = (sdfCorePacketPointBuff_tag *)work->pointBuff;
    for (i = 0; i != triangleCount; i++) {
        u16 *triangle = triangles;
        sdfCorePacketPointBuff_tag *a = nodes + triangle[0];
        sdfCorePacketPointBuff_tag *b = nodes + triangle[1];
        sdfCorePacketPointBuff_tag *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        sdfCorePacketPointBuff_tag *d, *e, *f, *g;
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
            D_00474210[rejected++] = i;
            continue;
        }
        aRegion = a->region;
        bRegion = b->region;
        cRegion = c->region;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0].a = (u128 *)a;
            out[0].b = (u128 *)b;
            out[0].c = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            sdfCorePacketPointBuff_tag *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = sdfVuAllocClipNode(work);
                e = sdfVuAllocClipNode(work);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
                    out[0].a = (u128 *)a;
                    out[0].b = (u128 *)d;
                    out[0].c = (u128 *)e;
                    out[1].a = (u128 *)e;
                    out[1].b = (u128 *)d;
                    out[1].c = (u128 *)b;
                    out[2].a = (u128 *)e;
                    out[2].b = (u128 *)b;
                    out[2].c = (u128 *)c;
                    out += 3;
                    break;
                }
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
                out[0].a = (u128 *)a;
                out[0].b = (u128 *)d;
                out[0].c = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0].a = (u128 *)b;
                    out[0].b = (u128 *)e;
                    out[0].c = (u128 *)d;
                    out[1].a = (u128 *)b;
                    out[1].b = (u128 *)c;
                    out[1].c = (u128 *)e;
                    out += 2;
                } else {
                    f = sdfVuAllocClipNode(work);
                    g = sdfVuAllocClipNode(work);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
                    out[0].a = (u128 *)e;
                    out[0].b = (u128 *)d;
                    out[0].c = (u128 *)f;
                    out[1].a = (u128 *)e;
                    out[1].b = (u128 *)f;
                    out[1].c = (u128 *)g;
                    out[2].a = (u128 *)g;
                    out[2].b = (u128 *)f;
                    out[2].c = (u128 *)b;
                    out[3].a = (u128 *)g;
                    out[3].b = (u128 *)b;
                    out[3].c = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = sdfVuAllocClipNode(work);
                e = sdfVuAllocClipNode(work);
                f = sdfVuAllocClipNode(work);
                g = sdfVuAllocClipNode(work);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
                out[0].a = (u128 *)a;
                out[0].b = (u128 *)d;
                out[0].c = (u128 *)e;
                out[1].a = (u128 *)d;
                out[1].b = (u128 *)b;
                out[1].c = (u128 *)f;
                out[2].a = (u128 *)e;
                out[2].b = (u128 *)d;
                out[2].c = (u128 *)f;
                out[3].a = (u128 *)e;
                out[3].b = (u128 *)f;
                out[3].c = (u128 *)g;
                out[4].a = (u128 *)e;
                out[4].b = (u128 *)g;
                out[4].c = (u128 *)c;
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
    work->backfaceCount = rejected;
    work->drawFaceCount = out - corePktFaceList;
    *work->extPointLink = NULL;
}

/* vu0 routine: native clip projection and ordinary triangle fans. */
void corePktBuildFaceList(sdfCorePacketCb_t *work) {
    s32 triangleCount;
    u16 *triangles;
    sdfCorePacketPointBuff_tag *nodes;
    s32 acceptAll;
    VuGeomRef *out;
    f32 scale = work->backfaceSign;
    f32 clipBias = D_00439190;
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = (const u8 *)&sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(clipBias, clipPlane, projection);
    if (work->attrBits & 0x4000) {
        corePktClipFace(work);
    }
    triangleCount = (s16)work->faceCount;
    acceptAll = corePktSoftenBias;
    out = corePktFaceList;
    triangles = (u16 *)work->facePtr;
    nodes = (sdfCorePacketPointBuff_tag *)work->pointBuff;
    for (; triangleCount != 0; triangleCount--) {
        u16 *triangle = triangles;
        sdfCorePacketPointBuff_tag *a = nodes + triangle[0];
        sdfCorePacketPointBuff_tag *b = nodes + triangle[1];
        sdfCorePacketPointBuff_tag *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        sdfCorePacketPointBuff_tag *d, *e, *f, *g;
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
            out[0].a = (u128 *)a;
            out[0].b = (u128 *)b;
            out[0].c = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            sdfCorePacketPointBuff_tag *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = sdfVuAllocClipNode(work);
                e = sdfVuAllocClipNode(work);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
                    out[0].a = (u128 *)a;
                    out[0].b = (u128 *)d;
                    out[0].c = (u128 *)e;
                    out[1].a = (u128 *)e;
                    out[1].b = (u128 *)d;
                    out[1].c = (u128 *)b;
                    out[2].a = (u128 *)e;
                    out[2].b = (u128 *)b;
                    out[2].c = (u128 *)c;
                    out += 3;
                    break;
                }
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
                out[0].a = (u128 *)a;
                out[0].b = (u128 *)d;
                out[0].c = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0].a = (u128 *)b;
                    out[0].b = (u128 *)e;
                    out[0].c = (u128 *)d;
                    out[1].a = (u128 *)b;
                    out[1].b = (u128 *)c;
                    out[1].c = (u128 *)e;
                    out += 2;
                } else {
                    f = sdfVuAllocClipNode(work);
                    g = sdfVuAllocClipNode(work);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
                    out[0].a = (u128 *)e;
                    out[0].b = (u128 *)d;
                    out[0].c = (u128 *)f;
                    out[1].a = (u128 *)e;
                    out[1].b = (u128 *)f;
                    out[1].c = (u128 *)g;
                    out[2].a = (u128 *)g;
                    out[2].b = (u128 *)f;
                    out[2].c = (u128 *)b;
                    out[3].a = (u128 *)g;
                    out[3].b = (u128 *)b;
                    out[3].c = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = sdfVuAllocClipNode(work);
                e = sdfVuAllocClipNode(work);
                f = sdfVuAllocClipNode(work);
                g = sdfVuAllocClipNode(work);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
                out[0].a = (u128 *)a;
                out[0].b = (u128 *)d;
                out[0].c = (u128 *)e;
                out[1].a = (u128 *)d;
                out[1].b = (u128 *)b;
                out[1].c = (u128 *)f;
                out[2].a = (u128 *)e;
                out[2].b = (u128 *)d;
                out[2].c = (u128 *)f;
                out[3].a = (u128 *)e;
                out[3].b = (u128 *)f;
                out[3].c = (u128 *)g;
                out[4].a = (u128 *)e;
                out[4].b = (u128 *)g;
                out[4].c = (u128 *)c;
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
    work->drawFaceCount = out - corePktFaceList;
    *work->extPointLink = NULL;
}


/* vu0 routine: native projection setup with ordinary C clipping. */
void corePktBuildBackFaceList(sdfCorePacketCb_t *work) {
    f32 scale = D_00439190;
    s32 remaining;
    u16 *triangles;
    u16 *indices;
    sdfCorePacketPointBuff_tag *nodes;
    s32 acceptAll;
    VuGeomRef *out;
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = (const u8 *)&sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(scale, clipPlane, projection);
    acceptAll = corePktSoftenBias;
    remaining = work->backfaceCount;
    indices = D_00474210;
    triangles = (u16 *)work->facePtr;
    nodes = (sdfCorePacketPointBuff_tag *)work->pointBuff;
    out = corePktFaceList;
    for (; remaining != 0; remaining--) {
        u16 *triangle = triangles + *indices++ * 3;
        sdfCorePacketPointBuff_tag *a = nodes + triangle[0];
        sdfCorePacketPointBuff_tag *b = nodes + triangle[1];
        sdfCorePacketPointBuff_tag *c = nodes + triangle[2];
        u32 bRegion = b->region;
        u32 aRegion = a->region;
        u32 cRegion = c->region;
        sdfCorePacketPointBuff_tag *d, *e, *f, *g;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0].a = (u128 *)a;
            out[0].b = (u128 *)b;
            out[0].c = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            sdfCorePacketPointBuff_tag *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = sdfVuAllocClipNode(work);
                e = sdfVuAllocClipNode(work);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
                    out[0].a = (u128 *)a;
                    out[0].b = (u128 *)d;
                    out[0].c = (u128 *)e;
                    out[1].a = (u128 *)e;
                    out[1].b = (u128 *)d;
                    out[1].c = (u128 *)b;
                    out[2].a = (u128 *)e;
                    out[2].b = (u128 *)b;
                    out[2].c = (u128 *)c;
                    out += 3;
                    break;
                }
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
                out[0].a = (u128 *)a;
                out[0].b = (u128 *)d;
                out[0].c = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0].a = (u128 *)b;
                    out[0].b = (u128 *)e;
                    out[0].c = (u128 *)d;
                    out[1].a = (u128 *)b;
                    out[1].b = (u128 *)c;
                    out[1].c = (u128 *)e;
                    out += 2;
                } else {
                    f = sdfVuAllocClipNode(work);
                    g = sdfVuAllocClipNode(work);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
                    out[0].a = (u128 *)e;
                    out[0].b = (u128 *)d;
                    out[0].c = (u128 *)f;
                    out[1].a = (u128 *)e;
                    out[1].b = (u128 *)f;
                    out[1].c = (u128 *)g;
                    out[2].a = (u128 *)g;
                    out[2].b = (u128 *)f;
                    out[2].c = (u128 *)b;
                    out[3].a = (u128 *)g;
                    out[3].b = (u128 *)b;
                    out[3].c = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = sdfVuAllocClipNode(work);
                e = sdfVuAllocClipNode(work);
                f = sdfVuAllocClipNode(work);
                g = sdfVuAllocClipNode(work);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
                out[0].a = (u128 *)a;
                out[0].b = (u128 *)d;
                out[0].c = (u128 *)e;
                out[1].a = (u128 *)d;
                out[1].b = (u128 *)b;
                out[1].c = (u128 *)f;
                out[2].a = (u128 *)e;
                out[2].b = (u128 *)d;
                out[2].c = (u128 *)f;
                out[3].a = (u128 *)e;
                out[3].b = (u128 *)f;
                out[3].c = (u128 *)g;
                out[4].a = (u128 *)e;
                out[4].b = (u128 *)g;
                out[4].c = (u128 *)c;
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
    work->drawFaceCount = out - corePktFaceList;
    *work->extPointLink = NULL;
}

void corePktEmitTexTris(work)
    sdfCorePacketCb_t *work;
{
    s32 remaining = work->drawFaceCount;
    s32 chunk;
    s32 first;
    u32 *out;
    VuGeomRef *ref;
    SdfAssetEntry *asset;

    if (remaining != 0) {
        out = (u32 *)work->packetPtr;
        ref = corePktFaceList;
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
                asset = work->matPacketPtr;
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
        work->packetPtr = (u8 *)out;
    }
}

/* Emit up to 16 triangles per batch; TEX1/TEX0/CLAMP/ALPHA context 2 state
 * precedes the first batch only. clearPrimitiveFlags filters GIF PRIM bits. */
void corePktEmitTexTrisCtx2(sdfCorePacketCb_t *work, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags) {
    s32 remainingTriangles = work->drawFaceCount;
    s32 batchTriangles;
    s32 firstBatch;
    u32 *packetCursor;
    VuGeomRef *triangleRefs;

    if (remainingTriangles != 0) {
        packetCursor = (u32 *)work->packetPtr;
        triangleRefs = corePktFaceList;
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
        work->packetPtr = (u8 *)packetCursor;
    }
}

/* Emit up to 24 colored triangles per batch, copying vertex quadwords 0 and 2.
 * Keep the native K&R declaration and caller convention. */
void corePktEmitTris(work)
    sdfCorePacketCb_t *work;
{
    s32 remainingTriangles = work->drawFaceCount;
    s32 batchTriangles;
    u32 *packetCursor;
    VuGeomRef *triangleRefs;

    if (remainingTriangles != 0) {
        packetCursor = (u32 *)work->packetPtr;
        triangleRefs = corePktFaceList;
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
        work->packetPtr = (u8 *)packetCursor;
    }
}

/* Select textured or colored emission. */
void corePktEmitBase(sdfCorePacketCb_t *work) {
    if ((work->attrBits & SDF_VU_TEXTURED_FLAG) != 0) {
        corePktEmitTexTris(work);
        return;
    }
    corePktEmitTris(work);
}

/* Finish the selected node's transformed rows and emit its textured geometry. */
void corePktApplyFaceDivColor(sdfCorePacketCb_t *work, const u128 *parameters);

void corePktEmitMultiTexPass(sdfCorePacketCb_t *work) {
    SdfAssetEntry *asset;
    u32 mode;
    u32 secondaryColor;
    u32 primaryStateWordSecond;

    corePktXfmUV((void *)work->pointBuff, work->pointCount, work->uv2Ptr,
                  (u8 *)work->matPacketPtr + 0x80);
    corePktLerpUVChain(work->extPointTop);
    asset = work->matPacketPtr;
    mode = asset->secondaryMode;
    secondaryColor = asset->secondaryColor;
    primaryStateWordSecond = asset->unk08;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (primaryStateWordSecond != secondaryColor) {
                u128 parameters[3];

                corePktMakeFaceDivParm((SdfVuColorTransform *)parameters, corePktCurrLight, work->color,
                              secondaryColor, asset->unk04, asset->unk1C,
                              asset->x + work->fdSoften, asset->y + work->fdBias);
                corePktApplyFaceDivColor(work, parameters);
            }
            break;
        case 3:
            corePktSetConstColor(work, secondaryColor);
            break;
    }
    corePktEmitTexTrisCtx2(work, asset->secondaryTextureState.sampling,
                                  asset->secondaryTextureState.texture,
                                  asset->secondaryTextureState.clamp, asset->alphaState, 0);
}
