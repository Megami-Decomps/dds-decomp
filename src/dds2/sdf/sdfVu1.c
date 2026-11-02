#include "sdf_gs_header.h"
#include "common.h"
#include "sdf_vu0.h"
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
#include "sdf_vu_transform_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "ee_mmi.h"
#include "sdf_texture_file.h"
#include "sdfCorePacketPointBuff_tag.h"

extern const struct SdfDmaTagHeader *corePktCurrLight;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
extern u8 sdfVuObjectBasisCache[];

extern u16 D_00439184;

typedef struct {
    f32 tmSave[4][4];      /* 0x00 */
    u16 faceCount;            /* 0x40 */
    s16 pointCount;            /* 0x42 */
    u32 attrBits;     /* 0x44 */
    u32 packetBits;         /* 0x48 */
    u32 orgAttrBits;             /* 0x4C */
    s16 drawFaceCount;         /* 0x50: geometry references submitted to VU in chunks */
    u8 pad52[2];
    SdfAssetEntry *matPacketPtr;  /* 0x54 */
    u32 color;       /* 0x58: packed RGBA word */
    f32 fdSoften;           /* 0x5C */
    f32 fdBias;           /* 0x60 */
    u32 pointBuff;           /* 0x64 */
    u32 freePointBuff;           /* 0x68 */
    u32 extPointBuff;          /* 0x6C */
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

extern s32 D_00439180;

void sdfVu1Init(void) {
}

/* Split ring addressing at position 128; retain the native 96-byte stride. */
void corePktSetupPointBuff(sdfCorePacketCb_t *work, s32 ringPosition) {
    sdfCorePacketPointBuff_tag **ringEnd = &work->extPointTop;
    if (ringPosition < 0x80) {
        work->freePointCount = 0x80 - ringPosition;
        work->extPointBuff = D_00439180;
        work->freePointBuff = 0x70001000 + ringPosition * 0x60;
        work->extPointCount = 0x2000;
        work->pointBuff = 0x70001000;
    } else if (ringPosition == 0x80) {
        work->pointBuff = 0x70001000;
        work->freePointBuff = D_00439180;
        work->freePointCount = 0x2000;
        work->extPointBuff = 0;
        work->extPointCount = 0;
    } else {
        work->pointBuff = D_00439180;
        work->freePointBuff = 0x70001000;
        work->freePointCount = 0x80;
        work->extPointBuff = D_00439180 + ringPosition * 0x60;
        work->extPointCount = 0x2000 - ringPosition;
    }
    work->extPointLink = ringEnd;
}

/* Decode four leading halfwords; selectionMask filters flags, not the payload. */
void corePktInitCb(sdfCorePacketCb_t *work, u16 *parameterWords, u32 selectionMask) {
    u8 *payload = (u8 *)(parameterWords + 4);
    u16 ringPosition;
    u16 parameterFlags;
    u16 nextParameter;
    u16 selectedFlags;
    work->faceCount = parameterWords[0];
    ringPosition = parameterWords[1];
    work->pointCount = ringPosition;
    parameterFlags = parameterWords[2];
    nextParameter = parameterWords[3];
    selectedFlags = parameterFlags & selectionMask;
    work->orgAttrBits = parameterFlags;
    work->packetBits = nextParameter;
    work->attrBits = selectedFlags;
    D_00439184 = selectedFlags & 0x78;
    work->dataPtr = payload;
    work->packetOpenPtr = 0;
    corePktSetupPointBuff(work, ringPosition);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void corePktLoadWorldLightMtx(void *vectors) {
    const struct SdfDmaTagHeader *m = corePktCurrLight;
    VU0_ROTATE_BASIS_AND_CACHE(vectors, m, sdfVuObjectBasisCache);
}

/* vu0 routine: modulate and clamp packed colors against the reference rows. */
void corePktMakeFaceDivParm(SdfVuColorTransform *result,
                   const struct SdfDmaTagHeader *reference, u32 packedColor,
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

void corePktSetFaceDivParm(SdfVuColorTransform *out, SdfAssetEntry *work, u32 packedColor, f32 deltaX, f32 deltaY) {
    corePktMakeFaceDivParm(out, corePktCurrLight, packedColor,
                  work->unk08, work->unk04,
                  work->unk1C,
                  work->x + deltaX,
                  work->y + deltaY);
}
