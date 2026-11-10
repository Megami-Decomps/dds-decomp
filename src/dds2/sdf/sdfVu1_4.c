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
