#include "common.h"
#include "ps2asm.h"
#include "sdf_lighting.h"
#include "sdf_resource.h"

struct sdfDrawTag_t;

union sdfMatUvwParm_t;



struct SdfDmaTagHeader;

struct sdf3DTmPacketData_t;

struct sdfMatPacket_t;

struct sdfTexPacket_t;

struct sdfGsMemSlice_t;

struct sdfTexHandle_t;

struct sdfMat_t;

struct sdf3DPrimPolyParm_t;

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;


/* Optional packed or five-float state block owned by an sdfMat_t. */
typedef union sdfMatUvwParm_t {
    struct {
        u64 unk0;
        u64 unk8;
        u32 unk10;
        u32 unk14;
    } packed;
    struct {
        f32 values[5];
        u32 unk14;
    } scalar;
    /* Named view of scalar.values; the last word remains opaque. */
    struct {
        f32 centerOffsetX;
        f32 centerOffsetY;
        f32 basisScaleX;
        f32 basisScaleY;
        f32 rotationInput;
        u32 unk14;
    } transformInputs;
} sdfMatUvwParm_t;

/* A texture coordinate transform in a material packet. sdfMatSetUvwParm fills
 * it either as six floats or, for the identity case, as three quadword halves. */
typedef union {
    f32 m[6];
    u64 q[3];
} sdfMatTexTm_t;


/* DMA tag prefix followed by the two VIF words used by indexed-model writers.
 * The control byte stays whole, and address is a hardware word rather than a
 * CPU pointer. */
typedef struct SdfDmaTagHeader {
    u16 quadwordCount;
    u8 reservedByte;
    u8 control;
    u32 address;
    u32 firstVifCode;
    u32 secondVifCode;
} QWORD_ALIGNED SdfDmaTagHeader;

/* One frame of transformed matrix rows, normalized lighting rows and VIF codes. */
typedef struct sdf3DTmPacketData_t {
    u128 matrix[4];
    u128 scaledRows[3];
    u32 tag[4];
} sdf3DTmPacketData_t;

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

#define SDF_VIF_ITOP_LIGHTING 0x04000010

#define SDF_VIF_MSCAL_COMMAND 0x14000000

extern u8 sdf3DPrimIdentityTm[];

extern vu8 sdfFrameId;

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern u32 sdf3DPrimDefaultFaceIndex[];

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

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, u32 aPacket);

void sdfDrawTagLinkRef(struct sdfDrawTag_t *aTag, u32 aPacket);

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




/* Refresh one of the two draw entries and retain the other entry's dirty bits. */
void sdfMatUpdatePacket(struct sdfMat_t *aMat, s32 aId);

/* Native 0x2C VIF primitive request, shared by its quad/triangle producers.
 * Attribute buffers carry serialized words; coordinate floats retain their bits.
 * Native callers supply a positive vertex count and aligned 16-byte position/normal rows. */
typedef struct sdf3DPrimPolyParm_t {
    s16 faceCount;
    s16 pointCount;
    u16 attr;
    u16 attrInhibit;
    u32 color;
    const void *faceIndex;
    const void *vertex;
    const void *normal;
    const void *uv;
    const void *multiUv;
    const void *vertexColor;
    void *(*allocator)(s32);
    f32 zBias;
} sdf3DPrimPolyParm_t;

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

extern void *memcpy(void *, const void *, unsigned int);

void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag,
                             s32 (*aAllocator)(s32));

void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *aPacket);

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32));

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);

void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag,
                             s32 (*aAllocator)(s32)) {
    u64 *referencePacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    referencePacket = (u64 *)aAllocator(0x20);
    referencePacket[0] = ((u64)((u32)sdf3DPrimIdentityTm & SDF_DMA_ADDRESS_MASK) << 32) | 0x30000008;
    referencePacket[1] = 0x6C07C000ULL << 32;
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(aTag, (u32)referencePacket);
}

/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */
void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *aPacket) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(aPacket);
    aPacket->tag[0] = SDF_VIF_ITOP_LIGHTING;
    aPacket->tag[1] = SDF_VIF_MSCAL_COMMAND;
    aPacket->tag[2] = 0;
    aPacket->tag[3] = 0;
}

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32)) {
    u64 *dmaPacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    dmaPacket = (u64 *)aAllocator(0x90);
    dmaPacket[0] = ((u64)((u32)(dmaPacket + 2) & SDF_DMA_ADDRESS_MASK) << 32) | 0x20000008;
    dmaPacket[1] = 0x6C07C000ULL << 32;
    sdf3DPrimSetTmPacket((sdf3DTmPacketData_t *)(dmaPacket + 2));
    sdfDrawTagLink(aTag, (u32)dmaPacket);
}

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32)) {
    u64 *referencePacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    sdfMatUpdatePacket(aMat, (s8)sdfFrameId);
    referencePacket = (u64 *)aAllocator(0x20);
    sdfMatMakePacket(aMat, (struct SdfDmaTagHeader *)referencePacket, (s8)sdfFrameId);
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(aTag, (u32)referencePacket);
}

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm) {
    s32 stripWords = aParm->faceCount;
    s32 count = aParm->pointCount;
    s32 headerWords = 18;
    s32 wordsPerVertex = 3;
    u32 primitive = aParm->attr | 0x68;
    u32 format = 0x60;
    s32 bytes;
    void *(*allocator)(s32);
    u32 *packet;
    u32 *cursor;
    const u128 *vector;
    const u32 *coordinates;
    const u32 *secondCoordinates;
    s32 i;
    u32 countCode;

    if (aParm->normal != NULL) {
        format = 0xE0;
        headerWords = 19;
        wordsPerVertex = 6;
    }
    if (aParm->uv != NULL) {
        format |= 0x100;
        primitive |= 0x10;
        headerWords++;
        wordsPerVertex += 2;
        if (aParm->multiUv != NULL) {
            primitive |= 0x1000;
            wordsPerVertex += 2;
        }
    }
    if (aParm->vertexColor != NULL) {
        format |= 0x200;
        primitive |= 0x800;
        headerWords++;
        wordsPerVertex++;
    }
    bytes = (headerWords + stripWords + wordsPerVertex * count) * 4;
    bytes = (bytes + 15) & ~15;
    allocator = aParm->allocator;
    if (allocator == NULL) allocator = sdfGrAllocPacketBuff;
    packet = allocator(bytes);
    *(u64 *)packet = (u64)(u16)((bytes >> 4) - 1) | 0x20000000ULL;
    packet[2] = 0x6102C000;
    packet[3] = (~aParm->attrInhibit) & 0xFFFF;
    packet[4] = 0x6E01C002;
    packet[5] = aParm->color;
    packet[6] = 0x6003C003;
    cursor = packet + 7;
    *cursor++ = (u32)aParm->zBias;
    *cursor++ = (u32)sdfFaceDivSoften;
    *cursor++ = (u32)sdfFaceDivBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)stripWords | ((u32)(u16)count << 16);
    *cursor++ = (primitive & 0xFFFF) | (format << 16);
    *cursor++ = (stripWords << 16) | 0x6E00C001;
    if (aParm->faceIndex == NULL) {
        memcpy(cursor, sdf3DPrimDefaultFaceIndex, stripWords * 4);
    } else {
        memcpy(cursor, aParm->faceIndex, stripWords * 4);
    }
    cursor += stripWords;
    stripWords++;
    countCode = count << 16;
    *cursor++ = countCode | stripWords | 0x6800C000;
    vector = aParm->vertex;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, *vector);
        cursor += 3;
        vector++;
        i++;
    } while (i != count);
    stripWords += count;
    vector = aParm->normal;
    if (vector != NULL) {
        *cursor++ = countCode | stripWords | 0x6800C000;
        i = 0;
        do {
            MMI_STORE_VEC3_VALUE(cursor, *vector);
            cursor += 3;
            vector++;
            i++;
        } while (i != count);
        stripWords += count;
    }
    coordinates = aParm->uv;
    if (coordinates != NULL) {
        secondCoordinates = aParm->multiUv;
        if (secondCoordinates == NULL) {
            *cursor++ = countCode | stripWords | 0x6400C000;
            memcpy(cursor, coordinates, count * 8);
            cursor += count * 2;
        } else {
            *cursor++ = countCode | stripWords | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *coordinates++;
                *cursor++ = *coordinates++;
                *cursor++ = *secondCoordinates++;
                *cursor++ = *secondCoordinates++;
                i++;
            } while (i != count);
        }
        stripWords += count;
    }
    if (aParm->vertexColor != NULL) {
        *cursor++ = countCode | stripWords | 0x6E00C000;
        memcpy(cursor, aParm->vertexColor, count * 4);
        cursor += count;
    }
    *cursor = 0x1400000C;
    while ((u32)++cursor & 15) {
        *cursor = 0;
    }
    return packet;
}
