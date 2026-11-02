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
#include "sdf_heap_api.h"
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
#include "sdfCorePacketCb_t.h"

extern const struct SdfDmaTagHeader *corePktCurrLight;

extern u8 sdfVuObjectBasisCache[];

extern u16 D_003BDA24;

extern s32 D_003BDA20;

void sdfVu1Init(void) {
}

/* Split ring addressing at position 128; retain the native 96-byte stride. */
void corePktSetupPointBuff(sdfCorePacketCb_t *work, s32 ringPosition) {
    sdfCorePacketPointBuff_tag **ringEnd = &work->extPointTop;
    if (ringPosition < 0x80) {
        work->freePointCount = 0x80 - ringPosition;
        work->extPointBuff = D_003BDA20;
        work->freePointBuff = ringPosition * 96 + 0x70001000;
        work->extPointCount = 0x2000;
        work->pointBuff = 0x70001000;
    } else if (ringPosition == 0x80) {
        work->pointBuff = 0x70001000;
        work->freePointBuff = D_003BDA20;
        work->freePointCount = 0x2000;
        work->extPointBuff = 0;
        work->extPointCount = 0;
    } else {
        work->freePointCount = 0x80;
        work->pointBuff = D_003BDA20;
        work->freePointBuff = 0x70001000;
        work->extPointBuff = D_003BDA20 + ringPosition * 96;
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
    D_003BDA24 = selectedFlags & 0x78;
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
