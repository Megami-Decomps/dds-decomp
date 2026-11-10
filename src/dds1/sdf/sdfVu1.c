#include "common.h"
#include "eeregs.h"
#include "ps2asm.h"
#include "sdf_lighting.h"
#include "sdf_resource.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};



struct sdfCorePacketFaceDivParm_t;

struct sdfCorePacketPointBuff_tag;

struct sdfMatPacket_t;

struct sdfTexPacket_t;

struct sdfGsMemSlice_t;

struct sdfTexHandle_t;

struct TMX_HEADER_t;

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

/* A texture coordinate transform in a material packet. sdfMatSetUvwParm fills
 * it either as six floats or, for the identity case, as three quadword halves. */
typedef union {
    f32 m[6];
    u64 q[3];
} sdfMatTexTm_t;


/* VU0 input basis and emitted base/delta color vectors. */
typedef struct sdfCorePacketFaceDivParm_t {
    u8 opaque00[4]; /* 0x00 */
    f32 scaledY; /* 0x04 */
    u8 opaque08[4]; /* 0x08 */
    f32 scale; /* 0x0C */
    f32 colorBase[4]; /* 0x10 */
    f32 colorDelta[4]; /* 0x20 */
} sdfCorePacketFaceDivParm_t;

#include "sdfCorePacketPointBuff_tag.h"

#define SDF_GIF_PRIM_SHIFT 47

#define SDF_VU_TEXTURED_FLAG 0x10

extern addr_t corePktCurrLight;

extern u128 *corePktFaceList[][3];

extern u16 corePktPrimBase;

extern u8 corePktSoftenBias;

extern u16 corePktBackFaceList[];

extern f32 corePktZBias;

extern f32 sdf3DEyeOffset[4];

/* Shared byte-storage view: the local screen structs are not compatible C types. */
extern u8 sdf3DScreenParam[];


extern u8 corePktWorldLightMtxCache[];

/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
typedef struct sdfMatPacket_t {
    u32 pad00;             /* 0x00 */
    u32 ambient;             /* 0x04 */
    u32 diffuse;             /* 0x08 */
    u32 secondaryColor;    /* 0x0C: packed color applied by the secondary draw state */
    u32 primaryStateWordThird; /* 0x10: copied from the owning asset, or forced to zero. */
    u32 refmapColor;             /* 0x14 */
    u32 pad18;             /* 0x18 */
    f32 constant;             /* 0x1C */
    u32 alphaState;        /* 0x20: packed GS ALPHA_2 state emitted by the VU transfer builder */
    u32 secondaryMode;     /* 0x24: secondary packet mode copied from sdfMat_t */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];           /* 0x30 */
    u64 tex1Data;          /* 0x38 */
    u64 tex0Data;
    u64 clampData;
    u64 mtexTex1Data;      /* 0x50 */
    u64 mtexTex0Data;
    u64 mtexClampData;
    /* sdfMatSetUvwParm writes one transform per texture: a 2x2 then an offset. */
    sdfMatTexTm_t texTm;   /* 0x68 */
    sdfMatTexTm_t mtexTexTm;
    u32 vifItopCommand;    /* 0x98: VIF ITOP with immediate 0xC */
    u32 vifMscalCommand;   /* 0x9C: VIF MSCAL with microprogram index zero */
} sdfMatPacket_t;

#include "sdfCorePacketCb_t.h"

void corePktEmitTexTris(sdfCorePacketCb_t *work);
void corePktEmitTris(sdfCorePacketCb_t *work);

extern void corePktXfmUV(void *vertices, s32 count, void *uv, void *transform);

extern void corePktApplyFaceDivColor(sdfCorePacketCb_t *cb, void *parameters);

extern void sdfGrClosePacketBuff(addr_t aBuffPtr);

void corePktEmitTexTrisCtx2(sdfCorePacketCb_t *cb, u64 samplingState, u64 textureState,
                          u64 clampState, u64 alphaState, s32 clearPrimitiveFlags);

/* Finish the selected node's transformed rows and emit its textured geometry. */ void corePktEmitMultiTexPass(sdfCorePacketCb_t *work);

/* Per-texture reference count and non-owning clone source (0x8). */
/* Texture buffer GPU command words (0x40); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct sdfTexPacket_t {
    s32 gifTagWord; /* Low GIFtag word; NLOOP occupies bits 0-14. */
    u32 gifTagControl; /* High GIFtag control word; one packed register. */
    u64 gifTagRegisters; /* Packed register list: A+D. */
    u64 samplingState; /* GS TEX1 data, including minification/magnification filters. */
    u64 samplingRegister; /* GS TEX1_1/TEX1_2 register selector. */
    u64 textureState; /* GS TEX0 data. */
    u64 textureRegister; /* GS TEX0_1/TEX0_2 register selector. */
    u64 clampState; /* GS CLAMP data. */
    u64 clampRegister; /* GS CLAMP_1/CLAMP_2 register selector. */
} sdfTexPacket_t;




/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

struct sdfTexHandle_t *sdfTexSetDirect(const TMX_HEADER_t *aTex);

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);

static inline sdfCorePacketPointBuff_tag *AllocNode(sdfCorePacketCb_t *work) {
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

extern saddr_t corePktPointBuff;

extern void corePktRelightBackFaces(u32 cb, s32 count);

void corePktSetupPointBuff(sdfCorePacketCb_t *cb, s32 vertCount);

void corePktInitCb(sdfCorePacketCb_t *cb, u16 *packetHdr, u32 attrMask);

void corePktLoadWorldLightMtx(void *vectors);

void corePktMakeFaceDivParm(sdfCorePacketFaceDivParm_t *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y);

void corePktSetFaceDivParm(sdfCorePacketFaceDivParm_t *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY);

void corePktLerpUVColorChain(sdfCorePacketPointBuff_tag *node);

void corePktSetConstColor(sdfCorePacketCb_t *cb, u32 color);

void corePktMakeEdgePoint(sdfCorePacketPointBuff_tag *out, sdfCorePacketPointBuff_tag *first, sdfCorePacketPointBuff_tag *second);

void corePktClipFace(sdfCorePacketCb_t *cb);

void corePktBuildFaceList(sdfCorePacketCb_t *cb);

void corePktBuildBackFaceList(sdfCorePacketCb_t *cb);

void corePktEmitBase(sdfCorePacketCb_t *cb);

void corePktEmitAllPasses(sdfCorePacketCb_t *cb);

void corePktOpenAndEmit(sdfCorePacketCb_t *cb);

void sdfVu1Init(void) {
}

void corePktSetupPointBuff(sdfCorePacketCb_t *cb, s32 vertCount) {
    void *ringEnd = (u8 *)cb + 0x78;
    if (vertCount < 0x80) {
        cb->freePointCount = 0x80 - vertCount;
        cb->extPointBuff = EE_ADDR_PTR(corePktPointBuff);
        cb->freePointBuff = EE_ADDR_PTR(vertCount * 96 + SDF_SCRATCHPAD_POINT_BUFFER);
        cb->extPointCount = 0x2000;
        cb->pointBuff = SDF_SCRATCHPAD_POINT_BUFFER;
    } else if (vertCount == 0x80) {
        cb->pointBuff = SDF_SCRATCHPAD_POINT_BUFFER;
        cb->freePointBuff = EE_ADDR_PTR(corePktPointBuff);
        cb->freePointCount = 0x2000;
        cb->extPointBuff = 0;
        cb->extPointCount = 0;
    } else {
        cb->freePointCount = 0x80;
        cb->pointBuff = corePktPointBuff;
        cb->freePointBuff = EE_ADDR_PTR(SDF_SCRATCHPAD_POINT_BUFFER);
        cb->extPointBuff = EE_ADDR_PTR(corePktPointBuff + vertCount * 96);
        cb->extPointCount = 0x2000 - vertCount;
    }
    cb->extPointLink = ringEnd;
}

void corePktInitCb(sdfCorePacketCb_t *cb, u16 *packetHdr, u32 attrMask) {
    u8 *payload = (u8 *)(packetHdr + 4);
    u16 ringPosition;
    u16 parameterFlags;
    u16 nextParameter;
    u16 selectedFlags;
    cb->faceCount = packetHdr[0];
    ringPosition = packetHdr[1];
    cb->pointCount = ringPosition;
    parameterFlags = packetHdr[2];
    nextParameter = packetHdr[3];
    selectedFlags = parameterFlags & attrMask;
    cb->orgAttrBits = parameterFlags;
    cb->packetBits = nextParameter;
    cb->attrBits = selectedFlags;
    corePktPrimBase = selectedFlags & 0x78;
    cb->dataPtr = payload;
    cb->packetOpenPtr = 0;
    corePktSetupPointBuff(cb, ringPosition);
}

/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void corePktLoadWorldLightMtx(void *vectors) {
    void *m = EE_ADDR_AS(void *, corePktCurrLight);
    VU0_ROTATE_BASIS_AND_CACHE(vectors, m, corePktWorldLightMtxCache);
}

/* vu0 routine: modulate and clamp packed colors against the reference rows. */
void corePktMakeFaceDivParm(sdfCorePacketFaceDivParm_t *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y) {
    f32 scaledY;

    if (scale < 1.0f) {
        scale = 1.0f;
    }
    scaledY = y * scale;
    result->scale = scale;
    result->scaledY = scaledY;
    VU0_BUILD_PACKED_COLOR_TRANSFORM(result, reference, packedColor,
                                     firstColor, secondColor, blend);
}

void corePktSetFaceDivParm(sdfCorePacketFaceDivParm_t *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY) {
    corePktMakeFaceDivParm(out, corePktCurrLight, packedColor,
                  work->diffuse, work->ambient,
                  work->constant,
                  work->x + deltaX,
                  work->y + deltaY);
}

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly and the arithmetic is not in it: it loads seven quadwords
 * of matrix state (the 4x4 at mtx+0x00 into vf28-vf31, the three light basis
 * rows at mtx+0x40 into vf24-vf26), then feeds vf2 and vf3 per vertex and runs
 * `vcallms 0`. The operation below is what that microprogram does, inferred
 * from the register setup and from corePktSkinVertFirst, which performs the
 * position half inline. The loop is software pipelined: the next vertex is
 * fetched before the current one's results are read back. */
#ifdef NON_MATCHING
void corePktSkinPointsFirst(sdfCorePacketPointBuff_tag *dst, s32 count, const f32 *mtx,
                            const f32 *weighted, const u32 *norm)
{
    do {
        const f32 *n = (const f32 *)norm;
        f32 *lighting = (f32 *)dst->opaque10;
        s32 lane;

        for (lane = 0; lane < 4; lane++) {
            dst->position[lane] = (mtx[lane] * weighted[0] + mtx[4 + lane] * weighted[1] +
                                   mtx[8 + lane] * weighted[2] + mtx[12 + lane]) * weighted[3];
        }
        for (lane = 0; lane < 4; lane++) {
            lighting[lane] = (mtx[16 + lane] * n[0] + mtx[20 + lane] * n[1] +
                              mtx[24 + lane] * n[2]) * weighted[3];
        }

        weighted += 4;
        norm += 3;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinPointsFirst);
#endif

/* NON_MATCHING: reference implementation, not a match candidate. As
 * corePktSkinPointsFirst, but the microprogram is `vcallms 0x48` and it also
 * takes the two destination quadwords in vf5 and vf6, so the bone's
 * contribution accumulates. Retail runs the loop count - 1 times and repeats
 * the body once after it, because the inputs for iteration i + 1 are fetched
 * inside iteration i. */
#ifdef NON_MATCHING
void corePktSkinPointsAccum(sdfCorePacketPointBuff_tag *dst, s32 count, const f32 *mtx,
                            const f32 *weighted, const u32 *norm)
{
    do {
        const f32 *n = (const f32 *)norm;
        f32 *lighting = (f32 *)dst->opaque10;
        s32 lane;

        for (lane = 0; lane < 4; lane++) {
            dst->position[lane] += (mtx[lane] * weighted[0] + mtx[4 + lane] * weighted[1] +
                                    mtx[8 + lane] * weighted[2] + mtx[12 + lane]) * weighted[3];
        }
        for (lane = 0; lane < 4; lane++) {
            lighting[lane] += (mtx[16 + lane] * n[0] + mtx[20 + lane] * n[1] +
                               mtx[24 + lane] * n[2]) * weighted[3];
        }

        weighted += 4;
        norm += 3;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinPointsAccum);
#endif

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly, and it has a register slip: the loop branches on the
 * destination pointer instead of the count, and the store is based on the
 * weight pointer instead of the destination. The C below is the intended
 * operation, the one corePktSkinVertAccum performs correctly. */
#ifdef NON_MATCHING
void corePktSkinVertFirst(sdfCorePacketPointBuff_tag *dst, s32 count, const f32 *mtx,
                          const f32 *weighted)
{
    do {
        s32 lane;

        /* Four words per entry, read as two ldr/ldl pairs because the stream is
         * 4-aligned and not 8-aligned: xyz is the vertex in bone space and w is
         * the bone weight. */
        for (lane = 0; lane < 4; lane++) {
            dst->position[lane] = (mtx[lane] * weighted[0] + mtx[4 + lane] * weighted[1] +
                                   mtx[8 + lane] * weighted[2] + mtx[12 + lane]) * weighted[3];
        }

        weighted += 4;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinVertFirst);
#endif

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly: a VU0 matrix multiply-add chain on vf28-vf31. */
#ifdef NON_MATCHING
void corePktSkinVertAccum(sdfCorePacketPointBuff_tag *dst, s32 count, const f32 *mtx,
                          const f32 *weighted)
{
    do {
        s32 lane;

        for (lane = 0; lane < 4; lane++) {
            dst->position[lane] += (mtx[lane] * weighted[0] + mtx[4 + lane] * weighted[1] +
                                    mtx[8 + lane] * weighted[2] + mtx[12 + lane]) * weighted[3];
        }

        weighted += 4;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktSkinVertAccum);
#endif

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly: it packs the three words with an unaligned ldr/ldl pair
 * and pcpyld, which cc1 does not select. */
#ifdef NON_MATCHING
void corePktLoadVerts(u8 *dst, s32 count, const u32 *src)
{
    do {
        s32 z = (s32)src[2];
        u32 *quad = (u32 *)dst;

        quad[0] = src[0];
        quad[1] = src[1];
        quad[2] = z;
        /* pcpyld leaves the lw's sign extension in the fourth word. */
        quad[3] = z >> 31;

        src += 3;
        dst += 0x60;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktLoadVerts);
#endif

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly: two unaligned ldr/ldl pairs per entry and pcpyld. */
/* NON_MATCHING. Behaviour checked against retail: 6 PCSX2 call vectors give the same return and memory,
 * field underground facility, field unknown realm savestates, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void corePktLoadVertsNorms(sdfCorePacketPointBuff_tag *dst, s32 count, const u32 *vert,
                           const u32 *norm)
{
    do {
        u32 *position = (u32 *)dst->position;
        u32 *lighting = (u32 *)dst->opaque10;

        position[0] = vert[0];
        position[1] = vert[1];
        position[2] = vert[2];
        /* pcpyld leaves the lw's sign extension in the fourth word. */
        position[3] = (s32)vert[2] >> 31;
        lighting[0] = norm[0];
        lighting[1] = norm[1];
        lighting[2] = norm[2];
        lighting[3] = (s32)norm[2] >> 31;

        vert += 3;
        norm += 3;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktLoadVertsNorms);
#endif

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly: two unaligned ldr/ldl pairs and two VU0 multiply-adds. */
#ifdef NON_MATCHING
void corePktMorphVertsNorms(sdfCorePacketPointBuff_tag *dst, s32 count, const u32 *vertDelta,
                            const u32 *normDelta, f32 weight)
{
    do {
        const f32 *dv = (const f32 *)vertDelta;
        const f32 *dn = (const f32 *)normDelta;
        u32 *position = (u32 *)dst->position;
        f32 *lighting = (f32 *)dst->opaque10;
        u32 *lightingWords = (u32 *)dst->opaque10;

        dst->position[0] += dv[0] * weight;
        dst->position[1] += dv[1] * weight;
        dst->position[2] += dv[2] * weight;
        position[3] = (s32)vertDelta[2] >> 31;
        lighting[0] += dn[0] * weight;
        lighting[1] += dn[1] * weight;
        lighting[2] += dn[2] * weight;
        lightingWords[3] = (s32)normDelta[2] >> 31;

        vertDelta += 3;
        normDelta += 3;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktMorphVertsNorms);
#endif

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly: an unaligned ldr/ldl pair, pcpyld and a VU0 multiply-add. */
#ifdef NON_MATCHING
void corePktMorphVerts(sdfCorePacketPointBuff_tag *dst, s32 count, const u32 *delta, f32 weight)
{
    do {
        const f32 *d = (const f32 *)delta;
        u32 *position = (u32 *)dst->position;

        dst->position[0] += d[0] * weight;
        dst->position[1] += d[1] * weight;
        dst->position[2] += d[2] * weight;
        /* vmaddx writes xyz but sqc2 stores the whole quadword, so w is left as
         * pcpyld built it: the sign extension of the delta's third word. */
        position[3] = (s32)delta[2] >> 31;

        delta += 3;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktMorphVerts);
#endif

/* NON_MATCHING: retail's own VU0 sequence, not a match candidate. vf1.x is the
 * 1.0 the engine keeps there, and vrsqrt clamps where a C division would turn a
 * zero normal into NaN. vmulq is xyz only, so the fourth word survives. The C
 * draft this replaced is below in comment form. */
/* NON_MATCHING. Behaviour checked against retail: 6 PCSX2 call vectors give the same return and memory,
 * field underground facility, field unknown realm savestates, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void corePktNormalizeNorms(sdfCorePacketPointBuff_tag *dst, s32 count)
{
    do {
        VU0_LOAD_VF2_AT16(dst);
        VU0_MUL_XYZ_VF3_VF2_VF2();
        VU0_ADDAY_X_ACC_VF3_VF3Y();
        VU0_MADDZ_X_VF4_VF1_VF3Z();
        VU0_RSQRT_Q_VF0W_VF4X();
        VU0_WAITQ();
        VU0_MULQ_XYZ_VF2_VF2_Q();
        VU0_STORE_VF2_AT16(dst);
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktNormalizeNorms);
#endif

// The C draft this replaced, kept as a note rather than code: a zero normal
// becomes NaN here under C's float rules, where retail's vrsqrt clamps it to
// zero.
//
// void corePktNormalizeNorms(sdfCorePacketPointBuff_tag *dst, s32 count)
// {
//     do {
//         f32 *lighting = (f32 *)dst->opaque10;
//         /* vmaddz.x against vf1.x, which the engine keeps at 1.0. */
//         f32 lengthSq = lighting[0] * lighting[0] + lighting[1] * lighting[1] +
//                        lighting[2] * lighting[2];
//         f32 scale = 1.0f / sdfSqrt(lengthSq);
//
//         lighting[0] *= scale;
//         lighting[1] *= scale;
//         lighting[2] *= scale;
//         /* vmulq is xyz only, so the fourth word survives untouched. */
//         dst++;
//     } while (--count != 0);
// }

/* NON_MATCHING: retail's own VU0 sequences, not match candidates. These five
 * routines are assembly, and two of their inputs are VU0 register state no C
 * expression reaches: vf28-vf31 hold the screen transform MultMatrixRev left
 * behind, and vf24-vf26 hold the world light basis from
 * corePktLoadWorldLightMtx, whose W lanes each rescales by corePktFaceDivParm
 * on entry. So each loop body runs retail's instructions, in retail's order,
 * for VU0's rounding, accumulator and clip flags; C runs the loop. Retail
 * pipelines the loops, loading the next vertex before storing this one, which
 * does not change the results. dds-decomp's hand/vu kernels are the same.
 *
 * Each writes one 96-byte record per vertex: the transformed position at
 * +0x00, the normal with the diffuse term in its W lane at +0x10, the
 * interpolated colour at +0x20, the UV pair at +0x30 for the UV variants, the
 * clip flags of the position at +0x51 and the two over/under bits of the
 * diffuse term at +0x50. The quadword stores at +0x10 and +0x30 only ever
 * have some lanes written by the VU, so the rest carry over whatever the
 * source register held.
 *
 * `cfc2 $vi18` reads a 24-bit shift register, six bits per vclip, so the byte
 * stored at +0x51 also carries the low two bits of the preceding clip in its
 * top two bits.
 *
 * The two shared steps come first; each routine then gets its own guard so
 * the drafts and the INCLUDE_ASM lines stay paired. The C draft these replaced
 * is after the last of them in comment form. */
#ifdef NON_MATCHING
extern sdfCorePacketFaceDivParm_t corePktFaceDivParm;

/* vf19-vf21 = the light; the basis rows' W = light.W * row.x. */
static void corePktLoadLight(void)
{
    VU0_LOAD_VF19_VF20_VF21(&corePktFaceDivParm);
    VU0_MULX_W_VF24_VF19();
    VU0_MULX_W_VF25_VF19();
    VU0_MULX_W_VF26_VF19();
}

/* vf16-vf18 = a texture transform's three 64-bit rows. */
static void corePktLoadUvTransform(const f32 *rows)
{
    VU0_LOAD_VF16_AT0(rows);
    VU0_LOAD_VF17_AT8(rows);
    VU0_LOAD_VF18_AT16(rows);
}

/* A lit vertex from vf2 (position) and vf6 (normal): its position, normal
 * (W = the light's 2d - 1), colour and clip bytes. With uv, vf4's coordinates
 * through vf16-vf18 as well. */
static void corePktLightVertex(sdfCorePacketPointBuff_tag *dst, s32 uv)
{
    if (uv) {
        VU0_MULAY_W_ACC_VF0_VF19Y();
        VU0_MADDAX_W_ACC_VF24_VF6X();
        VU0_MADDAY_W_ACC_VF25_VF6Y();
        VU0_MADDZ_W_VF7_VF26_VF6Z();
        VU0_MULAX_XYZW_ACC_VF28_VF2X();
        VU0_MADDAY_XYZW_ACC_VF29_VF2Y();
        VU0_MADDAZ_XYZW_ACC_VF30_VF2Z();
        VU0_MADDW_XYZW_VF3_VF31_VF0W();
        VU0_ADDAW_W_ACC_VF7_VF7W();
        VU0_MSUBW_W_VF6_VF0_VF0W();
        VU0_MAXX_W_VF8_VF7_VF0X();
        VU0_CLIPW_XYZW_VF3_VF3W();
        VU0_MULAX_XY_ACC_VF16_VF4X();
        VU0_ADDW_X_VF9_VF0_VF6W();
        VU0_MADDAY_XY_ACC_VF17_VF4Y();
        VU0_MINIW_W_VF8_VF8_VF0W();
        VU0_MADDW_XY_VF5_VF18_VF0W();
        VU0_CLIP_VF9_STORE_VERTEX_UV(dst);
        VU0_STORE_CLIP_UNDEROVER_AT80(dst);
    } else {
        VU0_MULAY_W_ACC_VF0_VF19Y();
        VU0_MADDAX_W_ACC_VF24_VF6X();
        VU0_MADDAY_W_ACC_VF25_VF6Y();
        VU0_MADDZ_W_VF7_VF26_VF6Z();
        VU0_MULAX_XYZW_ACC_VF28_VF2X();
        VU0_MADDAY_XYZW_ACC_VF29_VF2Y();
        VU0_MADDAZ_XYZW_ACC_VF30_VF2Z();
        VU0_MADDW_XYZW_VF3_VF31_VF0W();
        VU0_ADDAW_W_ACC_VF7_VF7W();
        VU0_MSUBW_W_VF6_VF0_VF0W();
        VU0_MAXX_W_VF8_VF7_VF0X();
        VU0_CLIPW_XYZW_VF3_VF3W();
        VU0_STORE_VF3_AT0(dst);
        VU0_ADDW_X_VF9_VF0_VF6W();
        VU0_MINIW_W_VF8_VF8_VF0W();
        VU0_STORE_VF6_AT16(dst);
        VU0_MULAW_XYZW_ACC_VF20_VF0W();
        VU0_CLIP_VF9_STORE_VF8_VI18(dst);
        VU0_STORE_CLIP_UNDEROVER_AT80(dst);
    }
}
#endif

#ifdef NON_MATCHING
void corePktXfmLightClip(sdfCorePacketPointBuff_tag *dst, s32 count, const u32 *vert,
                         const u32 *norm)
{
    corePktLoadLight();
    do {
        VU0_LOAD_NORM_VF6_VERT_VF2(vert, norm);
        corePktLightVertex(dst, 0);
        vert += 3;
        norm += 3;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClip);
#endif

#ifdef NON_MATCHING
void corePktXfmLightClipPB(sdfCorePacketPointBuff_tag *dst, s32 count)
{
    corePktLoadLight();
    do {
        VU0_LOAD_VF6_AT16(dst);
        VU0_LOAD_VF2_AT0(dst);
        corePktLightVertex(dst, 0);
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipPB);
#endif

#ifdef NON_MATCHING
void corePktXfmLightClipUV(sdfCorePacketPointBuff_tag *dst, s32 count, const u32 *vert,
                           const u32 *norm, const f32 *uv, const f32 *uvMtx)
{
    corePktLoadLight();
    corePktLoadUvTransform(uvMtx);
    do {
        VU0_LOAD_NORM_VF6_VERT_VF2(vert, norm);
        VU0_LOAD_UV_VF4(uv);
        corePktLightVertex(dst, 1);
        vert += 3;
        norm += 3;
        uv += 2;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipUV);
#endif

#ifdef NON_MATCHING
void corePktXfmLightClipUVPB(sdfCorePacketPointBuff_tag *dst, s32 count, const f32 *uv,
                             const f32 *uvMtx)
{
    corePktLoadLight();
    corePktLoadUvTransform(uvMtx);
    do {
        VU0_LOAD_VF6_AT16(dst);
        VU0_LOAD_VF2_AT0(dst);
        VU0_LOAD_UV_VF4(uv);
        corePktLightVertex(dst, 1);
        uv += 2;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmLightClipUVPB);
#endif

#ifdef NON_MATCHING
void corePktRelightBackFaces(u32 pointBuff, s32 count)
{
    sdfCorePacketPointBuff_tag *dst = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, pointBuff);

    /* The position and its clip flags are left alone; only the normal is
     * flipped and the shade and the over/under bits recomputed from it. */
    corePktLoadLight();
    do {
        VU0_LOAD_VF6_AT16(dst);
        VU0_SUB_XYZ_VF6_VF0_VF6();
        VU0_MULAY_W_ACC_VF0_VF19Y();
        VU0_MADDAX_W_ACC_VF24_VF6X();
        VU0_MADDAY_W_ACC_VF25_VF6Y();
        VU0_MADDZ_W_VF7_VF26_VF6Z();
        VU0_ADDAW_W_ACC_VF7_VF7W();
        VU0_MSUBW_W_VF6_VF0_VF0W();
        VU0_MAXX_W_VF8_VF7_VF0X();
        VU0_ADDW_X_VF9_VF0_VF6W();
        VU0_MINIW_W_VF8_VF8_VF0W();
        VU0_CLIPW_XYZW_VF9_VF0W();
        VU0_STORE_VF6_AT16(dst);
        VU0_MULAW_XYZW_ACC_VF20_VF0W();
        VU0_MADDW_XYZW_VF8_VF21_VF8W();
        VU0_NOP();
        VU0_NOP();
        VU0_NOP();
        VU0_STORE_CLIP_VF8_AT80(dst);
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktRelightBackFaces);
#endif

// The C draft these five replaced, kept as a note rather than code. It stood
// the two pieces of VU0 register state up as globals under notional names, and
// nothing in this tree ever writes them: a native build of it transforms every
// vertex to (0, 0, 0, 0), so the morphing-packet faces these routines serve do
// not draw. Treat it as an unverified shape, not a working reference, until
// something supplies real matrix and light-row values.
//
// static f32 corePktScreenMtx[4][4]; /* vf28-vf31 */
// static f32 corePktLightRowW[3];    /* vf24-vf26, W lane only, already scaled */
//
// static void corePktShadeAndClip(sdfCorePacketPointBuff_tag *dst, const f32 *n)
// {
//     f32 *lighting = (f32 *)dst->opaque10;
//     f32 diffuse = corePktFaceDivParm.scaledY + corePktLightRowW[0] * n[0] +
//                   corePktLightRowW[1] * n[1] + corePktLightRowW[2] * n[2];
//     f32 shade = diffuse;
//     s32 lane;
//
//     if (shade < 0.0f) {
//         shade = 0.0f;
//     }
//     if (shade > 1.0f) {
//         shade = 1.0f;
//     }
//     lighting[3] = diffuse + diffuse - 1.0f;
//     for (lane = 0; lane < 4; lane++) {
//         dst->color[lane] = corePktFaceDivParm.colorBase[lane] +
//                            corePktFaceDivParm.colorDelta[lane] * shade;
//     }
//     /* vclipw of (2 * diffuse - 1) against 1.0: over, then under. */
//     dst->region = (u8)((diffuse > 1.0f) | ((diffuse < 0.0f) << 1));
// }
//
// static void corePktXfmAndClip(sdfCorePacketPointBuff_tag *dst, const f32 *v)
// {
//     s32 lane;
//     u32 flags = 0;
//
//     for (lane = 0; lane < 4; lane++) {
//         dst->position[lane] = corePktScreenMtx[0][lane] * v[0] +
//                               corePktScreenMtx[1][lane] * v[1] +
//                               corePktScreenMtx[2][lane] * v[2] + corePktScreenMtx[3][lane];
//     }
//     for (lane = 0; lane < 3; lane++) {
//         flags |= (dst->position[lane] > dst->position[3]) << (lane * 2);
//         flags |= (dst->position[lane] < -dst->position[3]) << (lane * 2 + 1);
//     }
//     dst->clipFlags = (u8)flags;
// }
//
// void corePktXfmLightClip(sdfCorePacketPointBuff_tag *dst, s32 count, const u32 *vert,
//                          const u32 *norm)
// {
//     do {
//         f32 *lighting = (f32 *)dst->opaque10;
//
//         lighting[0] = ((const f32 *)norm)[0];
//         lighting[1] = ((const f32 *)norm)[1];
//         lighting[2] = ((const f32 *)norm)[2];
//         corePktXfmAndClip(dst, (const f32 *)vert);
//         corePktShadeAndClip(dst, lighting);
//         vert += 3;
//         norm += 3;
//         dst++;
//     } while (--count != 0);
// }
//
// The point buffer is already filled for the PB variants, so the position and
// the normal come back out of the record instead of out of two source streams.
//
// void corePktXfmLightClipPB(sdfCorePacketPointBuff_tag *dst, s32 count)
// {
//     do {
//         f32 position[4];
//         s32 lane;
//
//         for (lane = 0; lane < 4; lane++) {
//             position[lane] = dst->position[lane];
//         }
//         corePktXfmAndClip(dst, position);
//         corePktShadeAndClip(dst, (f32 *)dst->opaque10);
//         dst++;
//     } while (--count != 0);
// }
//
// void corePktXfmLightClipUV(sdfCorePacketPointBuff_tag *dst, s32 count, const u32 *vert,
//                            const u32 *norm, const f32 *uv, const f32 *uvMtx)
// {
//     do {
//         f32 *lighting = (f32 *)dst->opaque10;
//
//         lighting[0] = ((const f32 *)norm)[0];
//         lighting[1] = ((const f32 *)norm)[1];
//         lighting[2] = ((const f32 *)norm)[2];
//         corePktXfmAndClip(dst, (const f32 *)vert);
//         corePktShadeAndClip(dst, lighting);
//         dst->xy[0] = uvMtx[4] + uvMtx[0] * uv[0] + uvMtx[2] * uv[1];
//         dst->xy[1] = uvMtx[5] + uvMtx[1] * uv[0] + uvMtx[3] * uv[1];
//         vert += 3;
//         norm += 3;
//         uv += 2;
//         dst++;
//     } while (--count != 0);
// }
//
// void corePktXfmLightClipUVPB(sdfCorePacketPointBuff_tag *dst, s32 count, const f32 *uv,
//                              const f32 *uvMtx)
// {
//     do {
//         f32 position[4];
//         s32 lane;
//
//         for (lane = 0; lane < 4; lane++) {
//             position[lane] = dst->position[lane];
//         }
//         corePktXfmAndClip(dst, position);
//         corePktShadeAndClip(dst, (f32 *)dst->opaque10);
//         dst->xy[0] = uvMtx[4] + uvMtx[0] * uv[0] + uvMtx[2] * uv[1];
//         dst->xy[1] = uvMtx[5] + uvMtx[1] * uv[0] + uvMtx[3] * uv[1];
//         uv += 2;
//         dst++;
//     } while (--count != 0);
// }
//
// void corePktRelightBackFaces(u32 pointBuff, s32 count)
// {
//     sdfCorePacketPointBuff_tag *dst = (sdfCorePacketPointBuff_tag *)pointBuff;
//
//     /* The position and its clip flags are left alone; only the normal is
//      * flipped and the shade and the over/under bits recomputed from it. */
//     do {
//         f32 *lighting = (f32 *)dst->opaque10;
//
//         lighting[0] = -lighting[0];
//         lighting[1] = -lighting[1];
//         lighting[2] = -lighting[2];
//         corePktShadeAndClip(dst, lighting);
//         dst++;
//     } while (--count != 0);
// }

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly: a VU0 2x2-plus-offset multiply-add on the xy lanes, with
 * the loop rotated so the result of entry i is read back and stored while
 * entry i + 1 is being transformed. */
#ifdef NON_MATCHING
void corePktXfmUV(void *vertices, s32 count, void *uvSource, void *transform)
{
    sdfCorePacketPointBuff_tag *dst = vertices;
    const f32 *uv = uvSource;
    const f32 *mtx = transform;

    /* Three 8-byte rows: the 2x2 in mtx[0..3] and the offset in mtx[4..5]. */
    do {
        dst->xy[0] = mtx[4] + mtx[0] * uv[0] + mtx[2] * uv[1];
        dst->xy[1] = mtx[5] + mtx[1] * uv[0] + mtx[3] * uv[1];
        uv += 2;
        dst++;
    } while (--count != 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktXfmUV);
#endif

/* vu0 routine: lerp of the XY pair at +0x30 by the weight at node + 0x40 */
void corePktLerpUVChain(sdfCorePacketPointBuff_tag *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_XY(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}

/* vu0 routine: lerp of two source rows (+0x20, +0x30) by the weight at node + 0x40 */
void corePktLerpUVColorChain(sdfCorePacketPointBuff_tag *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_VECTORS(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}

/* NON_MATCHING: reference implementation, not a match candidate. The retail
 * body is assembly: 0.5 is broadcast into vf12 through a GPR, the colour loop
 * is rotated so each record's colour is stored one iteration late, and the
 * chain copy is a single lq/sq. */
#ifdef NON_MATCHING
void corePktApplyFaceDivColor(sdfCorePacketCb_t *cb, void *parameters)
{
    const sdfCorePacketFaceDivParm_t *parm = parameters;
    sdfCorePacketPointBuff_tag *point = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    sdfCorePacketPointBuff_tag *node;
    s32 count = cb->pointCount;

    do {
        const f32 *lighting = (const f32 *)point->opaque10;
        /* The W lane holds 2 * diffuse - 1, so this maps it back to [0,1]. */
        f32 shade = 0.5f + lighting[3] * 0.5f;
        s32 lane;

        if (shade < 0.0f) {
            shade = 0.0f;
        }
        if (shade > 1.0f) {
            shade = 1.0f;
        }
        for (lane = 0; lane < 4; lane++) {
            point->color[lane] = parm->colorBase[lane] + parm->colorDelta[lane] * shade;
        }
        point++;
    } while (--count != 0);

    /* Interpolated points share a colour with the point they were split from. */
    for (node = cb->extPointTop; node != NULL; node = node->next) {
        node->color[0] = node->sourceA->color[0];
        node->color[1] = node->sourceA->color[1];
        node->color[2] = node->sourceA->color[2];
        node->color[3] = node->sourceA->color[3];
    }
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktApplyFaceDivColor);
#endif

void corePktSetConstColor(sdfCorePacketCb_t *cb, u32 color) {
    f32 colorScale = 1.0f / 128.0f;
    sdfCorePacketPointBuff_tag *node;
    s32 remaining;
    u128 scaledColor;

    EE_MMI_RGBA_MULTIPLY_VECTOR(scaledColor, cb->color, color, colorScale);

    remaining = cb->pointCount;
    node = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    if (remaining != 0) {
        do {
            remaining--;
            *(u128 *)node->color = scaledColor;
            node++;
        } while (remaining != 0);
    }

    node = cb->extPointTop;
    while (node != NULL) {
        sdfCorePacketPointBuff_tag *source = (sdfCorePacketPointBuff_tag *)node->sourceA;
        SDF_VU0_COPYVECTOR_MEMOPS(node->color, source->color);
        node = node->next;
    }
}

/* vu0 routine: interpolate a clip vertex between two source rows, COP2 macro math only */
void corePktMakeEdgePoint(sdfCorePacketPointBuff_tag *out, sdfCorePacketPointBuff_tag *first, sdfCorePacketPointBuff_tag *second) {
    out->sourceA = first;
    out->sourceB = second;
    VU0_INTERPOLATE_CLIP_VERTEX(out, first, second);
}

/* vu0 routine: native clip facing and ordinary triangle fans. */
void corePktClipFace(sdfCorePacketCb_t *cb) {
    s32 triangleCount;
    s32 i;
    u16 *triangles;
    sdfCorePacketPointBuff_tag *nodes;
    s32 rejected;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale;

    out = corePktFaceList;
    triangleCount = (s16)cb->faceCount;
    rejected = 0;
    scale = cb->backfaceSign;
    acceptAll = corePktSoftenBias;
    triangles = (u16 *)cb->facePtr;
    nodes = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
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
            corePktBackFaceList[rejected++] = i;
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
            sdfCorePacketPointBuff_tag *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = AllocNode(cb);
                e = AllocNode(cb);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
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
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
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
                    f = AllocNode(cb);
                    g = AllocNode(cb);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
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
                d = AllocNode(cb);
                e = AllocNode(cb);
                f = AllocNode(cb);
                g = AllocNode(cb);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
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
    cb->backfaceCount = rejected;
    cb->drawFaceCount = out - corePktFaceList;
    *cb->extPointLink = NULL;
}

/* vu0 routine: native clip-plane setup and ordinary triangle fans. */
void corePktBuildFaceList(sdfCorePacketCb_t *cb) {
    s32 triangleCount;
    u16 *triangles;
    sdfCorePacketPointBuff_tag *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale = cb->backfaceSign;
    f32 clipBias = corePktZBias;
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(clipBias, clipPlane, projection);
    if (cb->attrBits & 0x4000) {
        corePktClipFace(cb);
    }
    triangleCount = (s16)cb->faceCount;
    acceptAll = corePktSoftenBias;
    out = corePktFaceList;
    triangles = (u16 *)cb->facePtr;
    nodes = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
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
            out[0][0] = (u128 *)a;
            out[0][1] = (u128 *)b;
            out[0][2] = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            sdfCorePacketPointBuff_tag *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = AllocNode(cb);
                e = AllocNode(cb);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
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
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
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
                    f = AllocNode(cb);
                    g = AllocNode(cb);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
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
                d = AllocNode(cb);
                e = AllocNode(cb);
                f = AllocNode(cb);
                g = AllocNode(cb);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
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
    cb->drawFaceCount = out - corePktFaceList;
    *cb->extPointLink = NULL;
}

void corePktBuildBackFaceList(sdfCorePacketCb_t *cb) {
    /* DDS1 node region uses the tail byte at +0x50. */
    f32 scale = corePktZBias;
    s32 remaining;
    u16 *triangles;
    u16 *indices;
    sdfCorePacketPointBuff_tag *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(scale, clipPlane, projection);
    acceptAll = corePktSoftenBias;
    remaining = cb->backfaceCount;
    indices = corePktBackFaceList;
    triangles = (u16 *)cb->facePtr;
    nodes = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    out = (u128 *(*)[3])corePktFaceList;
    if (remaining != 0) {
        do {
            u16 *triangle = triangles + *indices++ * 3;
            sdfCorePacketPointBuff_tag *a = nodes + triangle[0];
            sdfCorePacketPointBuff_tag *b = nodes + triangle[1];
            sdfCorePacketPointBuff_tag *c = nodes + triangle[2];
            u32 bRegion = b->region;
            u32 aRegion = a->region;
            u32 cRegion = c->region;
            sdfCorePacketPointBuff_tag *d, *e, *f, *g;
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
        corePktMakeEdgePoint(d, a, b);
        corePktMakeEdgePoint(e, a, c);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out += 1;
            if (bRegion != 0) {
            f = AllocNode(cb);
            g = AllocNode(cb);
            corePktMakeEdgePoint(f, b, a);
            corePktMakeEdgePoint(g, c, a);
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
        d = AllocNode(cb);
        e = AllocNode(cb);
        f = AllocNode(cb);
        g = AllocNode(cb);
        corePktMakeEdgePoint(d, b, a);
        corePktMakeEdgePoint(e, c, a);
        corePktMakeEdgePoint(f, b, c);
        corePktMakeEdgePoint(g, c, b);
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
            sdfCorePacketPointBuff_tag *oldA = a;
            u32 oldRegion = aRegion;
            a = b; b = c; c = oldA;
            aRegion = bRegion; bRegion = cRegion; cRegion = oldRegion;
        }
        classifyRegions:
            if (bRegion != cRegion) goto threeDifferentRegions;
        d = AllocNode(cb);
        e = AllocNode(cb);
            if (aRegion != 0) goto interpolateOutsideA;
        corePktMakeEdgePoint(d, b, a);
        corePktMakeEdgePoint(e, c, a);
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
    cb->drawFaceCount = out - (u128 *(*)[3])corePktFaceList;
    *(sdfCorePacketPointBuff_tag **)cb->extPointLink = NULL;
}

void corePktEmitTexTris(work)
    sdfCorePacketCb_t *work;
{
    s32 remaining = work->drawFaceCount;
    if (remaining != 0) {
        u128 *(*nodes)[3] = corePktFaceList;
        u32 *cursor = work->packetPtr;
        s32 first = 1;
        do {
            s32 chunk = (remaining <= 0x10) ? remaining : 0x10;
            remaining -= chunk;
            while ((EE_ADDR(cursor) & 0xC) != 4) {
                *cursor++ = 0;
            }
            if (first) {
                sdfMatPacket_t *node;
                *cursor++ = 0x6501C000;
                *cursor++ = 4;
                *cursor++ = ((chunk * 9 + 5) << 16) | 0x6C00C001;
                node = work->matPacketPtr;
                first = 0;
                *(u64 *)cursor = 0x1000000000000003ULL;
                cursor += 2;
                *(u64 *)cursor = 0xE;
                cursor += 2;
                *(u64 *)cursor = node->tex1Data;
                cursor += 2;
                *(u64 *)cursor = 0x14;
                cursor += 2;
                *(u64 *)cursor = node->tex0Data;
                cursor += 2;
                *(u64 *)cursor = 6;
                cursor += 2;
                *(u64 *)cursor = node->clampData;
                cursor += 2;
                *(u64 *)cursor = 8;
                cursor += 2;
            } else {
                *cursor++ = 0x6501C000;
                *cursor++ = 0;
                *cursor++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)cursor = ((u64)(corePktPrimBase | 3) << 47) | (chunk * 3) | 0x3000400000008000ULL;
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
        work->packetPtr = cursor;
    }
}

void corePktEmitTexTrisCtx2(sdfCorePacketCb_t *cb, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags) {
    s32 remainingTriangles = cb->drawFaceCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = corePktFaceList;
        u32 *packetCursor = cb->packetPtr;
        s32 firstBatch = 1;
        do {
            s32 batchTriangles = (remainingTriangles <= 0x10) ? remainingTriangles : 0x10;
            remainingTriangles -= batchTriangles;
            while ((EE_ADDR(packetCursor) & 0xC) != 4) {
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
            *(u64 *)packetCursor = ((u64)((corePktPrimBase & ~clearPrimitiveFlags) | 0x203) << SDF_GIF_PRIM_SHIFT) | (batchTriangles * 3) | 0x3000400000008000ULL;
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
            *packetCursor++ = 0x14000004;
        } while (remainingTriangles != 0);
        cb->packetPtr = packetCursor;
    }
}

void corePktEmitTris(work)
    sdfCorePacketCb_t *work;
{
    s32 remainingTriangles = work->drawFaceCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = corePktFaceList;
        u32 *packetCursor = work->packetPtr;
        do {
            s32 batchTriangles = (remainingTriangles <= 0x18) ? remainingTriangles : 0x18;
            remainingTriangles -= batchTriangles;
            while ((EE_ADDR(packetCursor) & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            *packetCursor++ = 0x6501C000;
            *packetCursor++ = 0x10000;
            *packetCursor++ = ((batchTriangles * 6 + 1) << 16) | 0x6C00C001;
            *(u64 *)packetCursor = (batchTriangles * 3) | ((u64)(corePktPrimBase | 3) << SDF_GIF_PRIM_SHIFT) | 0x2000400000008000ULL;
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
            *packetCursor++ = 0x14000004;
        } while (remainingTriangles != 0);
        work->packetPtr = packetCursor;
    }
}

void corePktEmitBase(sdfCorePacketCb_t *cb) {
    if ((cb->attrBits & SDF_VU_TEXTURED_FLAG) != 0) {
        corePktEmitTexTris(cb);
        return;
    }
    corePktEmitTris(cb);
}

void corePktEmitMultiTexPass(sdfCorePacketCb_t *cb) {
    sdfMatPacket_t *node;
    u32 mode;
    u32 secondaryColor;
    u32 primaryStateWordSecond;

    corePktXfmUV(EE_ADDR_AS(void *, cb->pointBuff), cb->pointCount, cb->uv2Ptr,
                  (u8 *)cb->matPacketPtr + 0x80);
    corePktLerpUVChain(cb->extPointTop);
    node = cb->matPacketPtr;
    mode = node->secondaryMode;
    secondaryColor = node->secondaryColor;
    primaryStateWordSecond = node->diffuse;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (primaryStateWordSecond != secondaryColor) {
                u128 parameters[3];

                corePktMakeFaceDivParm((sdfCorePacketFaceDivParm_t *)parameters, corePktCurrLight, cb->color,
                              secondaryColor, node->ambient, node->constant,
                              node->x + cb->fdSoften, node->y + cb->fdBias);
                corePktApplyFaceDivColor(cb, parameters);
            }
            break;
        case 3:
            corePktSetConstColor(cb, secondaryColor);
            break;
    }
    corePktEmitTexTrisCtx2(cb, node->mtexTex1Data,
                                  node->mtexTex0Data,
                                  node->mtexClampData, node->alphaState, 0);
}

#ifdef NON_MATCHING
extern u64 sdf3DRefMapParam;

void corePktEmitRefMapPass(sdfCorePacketCb_t *work)
{
    sdf3DLightParallelPacket_t *light = EE_ADDR_AS(sdf3DLightParallelPacket_t *, corePktCurrLight);
    sdfCorePacketPointBuff_tag *point = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, work->pointBuff);
    u32 count = (u16)work->pointCount;
    u32 materialColor = work->matPacketPtr->primaryStateWordThird;
    u64 scale = 0x3C0000003C000000ULL;

    SDF_VU0_LOADVECTOR(&light->TM.m[12]);
    if ((work->packetBits & 0x800) == 0) VU0_REFMAP_CACHED_BASIS(corePktWorldLightMtxCache);
    else VU0_REFMAP_DIRECT_BASIS();
    VU0_REFMAP_LOAD_NORMAL(point);
    VU0_REFMAP_COLORS(materialColor, work->color, scale);
    do {
        VU0_REFMAP_STEP(point, point + 1);
        point++;
        count--;
    } while (count != 0);
    corePktLerpUVColorChain(work->extPointTop);
    corePktEmitTexTrisCtx2(work, 0, sdf3DRefMapParam, 5, 0x48, 0x20);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktEmitRefMapPass);
#endif

void corePktEmitAllPasses(sdfCorePacketCb_t *cb) {
    u32 selectedFlags;

    corePktEmitBase(cb);
    selectedFlags = cb->attrBits;
    if ((selectedFlags & 0x1000) != 0) {
        corePktEmitMultiTexPass(cb);
        selectedFlags = cb->attrBits;
    }
    if ((selectedFlags & 1) != 0) {
        corePktEmitRefMapPass(cb);
        return;
    }
}

void corePktOpenAndEmit(sdfCorePacketCb_t *cb) {
    u32 packetStart = sdfGrOpenPacketBuff();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;
    u32 ringAddress = EE_UNCACHED_RING(EE_PHYSICAL(alignedStart + 0x40));
    cb->packetOpenPtr = packetStart;
    cb->packetTopPtr = alignedStart + 0x30;
    cb->packetStartPtr = EE_ADDR_AS(u32 *, ringAddress);
    cb->packetPtr = EE_ADDR_AS(u32 *, ringAddress);
    if ((cb->attrBits & 0x4000) != 0) {
        u8 savedVectors[0x30];
        VU0_STORE_VF24_VF25_VF26(savedVectors);
        corePktBuildFaceList(cb);
        corePktEmitAllPasses(cb);
        corePktSetupPointBuff(cb, cb->pointCount);
        VU0_LOAD_VF24_VF25_VF26(savedVectors);
        corePktRelightBackFaces(cb->pointBuff, cb->pointCount);
        corePktBuildBackFaceList(cb);
        corePktEmitAllPasses(cb);
    } else {
        corePktBuildFaceList(cb);
        corePktEmitAllPasses(cb);
    }
}

u64 *corePktClosePacket(sdfCorePacketCb_t *cb) {
    addr_t cursor;
    addr_t start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (cb->packetOpenPtr == 0) {
        return NULL;
    }

    cursor = EE_ADDR(cb->packetPtr);
    start = EE_ADDR(cb->packetStartPtr);
    if (cursor == start) {
        sdfGrClosePacketBuff(cb->packetOpenPtr);
        return NULL;
    }

    while (((u32)cursor & 0xC) != 0) {
        *EE_ADDR_AS(u32 *, cursor) = 0;
        cursor += 4;
    }
    quadwords = (s32)(cursor - start) >> 4;

    while (((u32)cursor & 0x30) != 0) {
        QUAD_ZERO(*EE_ADDR_AS(u128 *, cursor));
        cursor += 0x10;
    }

    sdfGrClosePacketBuff(EE_PHYSICAL(cursor));
    header = EE_ADDR_AS(u64 *, cb->packetTopPtr);
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}

#ifdef NON_MATCHING
u8 *corePktVu0Run(sdfCorePacketPointBuff_tag *output, const void *source, s32 count,
                  s32 transferCount, s32 transferStride)
{
    addr_t input = EE_ADDR(source);
    s32 done = 0;
    s32 batch, i, qwc;

    while (done < count) {
        batch = count - done;
        if (batch >= 25) batch = 24;
        done += batch;
        qwc = (s32)((u32)transferStride * (u32)batch + (u32)transferCount + 15) >> 4;
        HW_WRITE(D0_QWC, qwc);
        HW_WRITE(D0_MADR, EE_PHYSICAL(input));
        input += (u32)qwc << 4;
        HW_WRITE(D0_CHCR, 0x101);
        while ((HW_READ(D0_CHCR) & 0x100) != 0 || (HW_READ(VIF0_STAT) & 0x0F000003) != 0) { }
        VU0_BEGIN_POINT_UNPACK(0x60);
        for (i = 0; i < batch; i++) {
            VU0_UNPACK_POINT(output);
            output++;
        }
    }
    return EE_ADDR_AS(u8 *, input);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfVu1", corePktVu0Run);
#endif
