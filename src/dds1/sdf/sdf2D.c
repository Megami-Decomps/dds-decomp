#include "common.h"
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

struct sdfDrawTag_t;

struct sdf2DTexPacket_t;

struct sdfTexPacket_t;

struct sdfGsMemSlice_t;

struct sdfTexHandle_t;

struct TMX_HEADER_t;

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;


#define SDF_GIF_PRIM_SHIFT 47

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_GIF_NREG_SHIFT 60

#define SDF_GIF_PRE_EOP_FLAGS 0x400000008000LL

#define SDF_VIF_DIRECT_COMMAND 0x50000000

/* Fixed-size texture packet; constructor allocation and writes cover 0x50. */
typedef struct sdf2DTexPacket_t {
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t tex1;
    sdfGrTagStdReg_t tex0;
    sdfGrTagStdReg_t clamp;
} sdf2DTexPacket_t;

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, u32 aPacket);

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



/* Return packed 64-bit TEX0, TEX1, and CLAMP states from the primary GS packet. */
u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);

u64 sdfTexGetTex1Reg(sdfTexHandle_t *aHandle);

u64 sdfTexGetClampReg(sdfTexHandle_t *aHandle);

/* DMA/VIF transfer header and packed A+D GIF tag, excluding register payload. */
void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords);

u32 sdf2DGetTexPacketSize(sdfTexHandle_t *aTexHndl);


/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

struct sdfTexHandle_t *sdfTexSetDirect(const TMX_HEADER_t *aTex);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

sdf2DTexPacket_t *sdf2DMakeTexPacket(sdf2DTexPacket_t *aPacket, sdfTexHandle_t *aTexHndl, s32 aCtx);

s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

u32 sdf2DMakeStdPacket(u32 aPacket, s32 aLeng);

s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim);

sdfGrPacketHeader_t *sdf2DAllocSprite(s32 aNumOfSpr);

u32 sdf2DGetTexPacketSize(sdfTexHandle_t *aTexHndl) {
    return 5 << SDF_DMA_QWORD_SHIFT;
}

sdf2DTexPacket_t *sdf2DMakeTexPacket(sdf2DTexPacket_t *aPacket, sdfTexHandle_t *aTexHndl, s32 aCtx) {
    aPacket->header.dmatag.qwc = 4;
    aPacket->header.giftag.parm = 0x1000000000008003ULL;
    aPacket->header.dmatag.dat1 = 0x50000004;
    aPacket->header.dmatag.dat0 = 0;
    aPacket->header.giftag.regs = SDF_GIF_REGISTER_AD;
    aPacket->tex1.data = sdfTexGetTex1Reg(aTexHndl);
    aPacket->tex1.reg = aCtx + 0x14;
    aPacket->tex0.data = sdfTexGetTex0Reg(aTexHndl);
    aPacket->tex0.reg = aCtx + 6;
    aPacket->clamp.data = sdfTexGetClampReg(aTexHndl);
    aPacket->clamp.reg = aCtx + 8;
    return aPacket;
}

s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx) {
    s32 packetBytes = sdf2DGetTexPacketSize(aTexHndl);
    sdf2DTexPacket_t *packet = (sdf2DTexPacket_t *)sdfGrAllocPacketBuff(packetBytes);
    s32 packetAddress = (s32)sdf2DMakeTexPacket(packet, aTexHndl, aCtx);
    sdfDrawTagLink(aTag, packetAddress);
    return packetAddress;
}

u32 sdf2DMakeStdPacket(u32 aPacket, s32 aLeng) {
    sdfGrSetRegsPacketHeader((sdfGrPacketHeader_t *)aPacket, (aLeng >> SDF_DMA_QWORD_SHIFT) - 2);
    return aPacket;
}

s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim) {
    return (aNumOfRegs * aNumOfPrim + 2) << SDF_DMA_QWORD_SHIFT;
}

void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *aPacketPtr) {
    return aPacketPtr + 1;
}

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim) {
    s32 packetQuadwords = aNumOfRegs * aNumOfPrim + 1;
    s64 gifTag = aNumOfPrim | ((s64)aNumOfRegs << SDF_GIF_NREG_SHIFT);

    gifTag |= (s64)aPrimReg << SDF_GIF_PRIM_SHIFT;
    gifTag |= SDF_GIF_PRE_EOP_FLAGS;
    aPacketPtr->giftag.parm = gifTag;
    aPacketPtr->dmatag.dat1 = packetQuadwords | SDF_VIF_DIRECT_COMMAND;
    aPacketPtr->giftag.regs = aGifRegs;
    aPacketPtr->dmatag.dat0 = 0;
    aPacketPtr->dmatag.qwc = packetQuadwords;
    return aPacketPtr;
}

sdfGrPacketHeader_t *sdf2DAllocSprite(s32 aNumOfSpr) {
    sdfGrPacketHeader_t *packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, aNumOfSpr));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x53531, aNumOfSpr);
    return packet;
}
